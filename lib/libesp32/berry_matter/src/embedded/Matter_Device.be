#
# Matter_Device.be - implements a generic Matter device (commissionee)
#
# Copyright (C) 2023  Stephan Hadinger & Theo Arends
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

#@ solidify:Matter_Device,weak

class Matter_Device
  static var UDP_PORT = 5540          # this is the default port for group multicast, we also use it for unicast
  static var PASSCODE_DEFAULT = 20202021
  static var PBKDF_ITERATIONS = 1000  # I don't see any reason to choose a different number
  static var VENDOR_ID = 0xFFF1
  static var PRODUCT_ID = 0x8000
  static var FILENAME = "_matter_device.json"
  static var PASE_TIMEOUT = 10*60     # default open commissioning window (10 minutes)
  var started                         # is the Matter Device started (configured, mDNS and UDPServer started)
  var plugins                         # list of plugins
  var udp_server                      # `matter.UDPServer()` object
  var message_handler                     # `matter.MessageHandler()` object
  var sessions                        # `matter.Session_Store()` objet
  var ui
  # Commissioning open
  var commissioning_open              # timestamp for timeout of commissioning (millis()) or `nil` if closed
  var commissioning_iterations        # current PBKDF number of iterations
  var commissioning_discriminator     # commissioning_discriminator
  var commissioning_salt              # current salt
  var commissioning_w0                # current w0 (SPAKE2+)
  var commissioning_L                 # current L (SPAKE2+)
  var commissioning_admin_fabric      # the fabric that opened the currint commissioning window, or `nil` for default
  # information about the device
  var commissioning_instance_wifi     # random instance name for commissioning (mDNS)
  var commissioning_instance_eth      # random instance name for commissioning (mDNS)
  var hostname_wifi                   # MAC-derived hostname for commissioning
  var hostname_eth                    # MAC-derived hostname for commissioning
  var vendorid
  var productid
  # mDNS active announces
  var mdns_pase_eth                   # do we have an active PASE mDNS announce for eth
  var mdns_pase_wifi                  # do we have an active PASE mDNS announce for wifi
  # saved in parameters
  var root_discriminator              # as `int`
  var root_passcode                   # as `int`
  var ipv4only                        # advertize only IPv4 addresses (no IPv6)
  # context for PBKDF
  var root_iterations                 # PBKDF number of iterations
  # PBKDF information used only during PASE (freed afterwards)
  var root_salt
  var root_w0
  var root_L

  #############################################################
  def init()
    import crypto
    import string
    if !tasmota.get_option(matter.MATTER_OPTION)
      matter.UI(self)   # minimal UI
      return
    end    # abort if SetOption 151 is not set

    self.started = false
    self.plugins = []
    self.vendorid = self.VENDOR_ID
    self.productid = self.PRODUCT_ID
    self.root_iterations = self.PBKDF_ITERATIONS
    self.root_salt = crypto.random(16)         # bytes("5350414B453250204B65792053616C74")
    self.ipv4only = false
    self.load_param()

    self.sessions = matter.Session_Store()
    self.sessions.load_fabrics()
    self.message_handler = matter.MessageHandler(self)
    self.ui = matter.UI(self)

    if tasmota.wifi()['up'] || tasmota.eth()['up']
      self.start()
    end
    if !tasmota.wifi()['up']
      tasmota.add_rule("Wifi#Connected", def ()
          self.start()
          tasmota.remove_rule("Wifi#Connected", "matter_start")
        end, "matter_start")
    end
    if !tasmota.eth()['up']
      tasmota.add_rule("Eth#Connected", def ()
          self.start()
          tasmota.remove_rule("Eth#Connected", "matter_start")
        end, "matter_start")
    end

    self._init_basic_commissioning()

    tasmota.add_driver(self)
  end

  #############################################################
  # Start Matter device server when the first network is coming up
  def start()
    if self.started  return end      # abort if already started

    # add the default plugin
    self.plugins.push(matter.Plugin_Root(self, 0))
    # autoconfigure other plugins
    self.autoconf_device()

    # for now read sensors every 5 seconds
    tasmota.add_cron("*/5 * * * * *", def () self._trigger_read_sensors() end, "matter_sensors_5s")

    self._start_udp(self.UDP_PORT)

    self.start_mdns_announce_hostnames()

    self.started = true
  end

  #############################################################
  # Start Basic Commissioning Window if needed at startup
  def _init_basic_commissioning()
    # if no fabric is configured, automatically open commissioning at restart
    if self.sessions.count_active_fabrics() == 0
      self.start_root_basic_commissioning()
    end
  end

  #############################################################
  # Start Basic Commissioning with root parameters
  #
  # Open window for `timeout_s` (default 10 minutes)
  def start_root_basic_commissioning(timeout_s)
    import string
    if timeout_s == nil   timeout_s = self.PASE_TIMEOUT end

    # show Manual pairing code in logs
    var pairing_code = self.compute_manual_pairing_code()
    tasmota.log(string.format("MTR: Manual pairing code: %s-%s-%s", pairing_code[0..3], pairing_code[4..6], pairing_code[7..]), 2)
    
    # compute PBKDF
    self._compute_pbkdf(self.root_passcode, self.root_iterations, self.root_salt)
    self.start_basic_commissioning(timeout_s, self.root_iterations, self.root_discriminator, self.root_salt, self.root_w0, #-self.root_w1,-# self.root_L, nil)
  end

  #####################################################################
  # Remove a fabric and clean all corresponding values and mDNS entries
  def remove_fabric(fabric_parent)
    var sub_fabrics = self.sessions.find_children_fabrics(fabric_parent.get_fabric_index())
    if sub_fabrics == nil return end
    for fabric_index : sub_fabrics
      var fabric = self.sessions.find_fabric_by_index(fabric_index)
      if fabric != nil
        self.message_handler.im.subs_shop.remove_by_fabric(fabric)
        self.mdns_remove_op_discovery(fabric)
        self.sessions.remove_fabric(fabric)
      end
    end
    self.sessions.save_fabrics()
  end

  #############################################################
  # Start Basic Commissioning Window with custom parameters
  def start_basic_commissioning(timeout_s, iterations, discriminator, salt, w0, L, admin_fabric)
    self.commissioning_open = tasmota.millis() + timeout_s * 1000
    self.commissioning_iterations = iterations
    self.commissioning_discriminator = discriminator
    self.commissioning_salt = salt
    self.commissioning_w0 = w0
    self.commissioning_L  = L
    self.commissioning_admin_fabric = admin_fabric

    if tasmota.wifi()['up'] || tasmota.eth()['up']
      self.mdns_announce_PASE()
    else
      tasmota.add_rule("Wifi#Connected", def ()
          self.mdns_announce_PASE()
          tasmota.remove_rule("Wifi#Connected", "mdns_announce_PASE")
        end, "mdns_announce_PASE")
        tasmota.add_rule("Eth#Connected", def ()
            self.mdns_announce_PASE()
            tasmota.remove_rule("Eth#Connected", "mdns_announce_PASE")
          end, "mdns_announce_PASE")
    end
  end

  #############################################################
  # Is root commissioning currently open. Mostly for UI to know if QRCode needs to be shown.
  def is_root_commissioning_open()
    return self.commissioning_open != nil && self.commissioning_admin_fabric == nil
  end

  #############################################################
  # Stop PASE commissioning, mostly called when CASE is about to start
  def stop_basic_commissioning()
    self.commissioning_open = nil

    self.mdns_remove_PASE()

    # clear any PBKDF information to free memory
    self.commissioning_iterations = nil
    self.commissioning_discriminator = nil
    self.commissioning_salt = nil
    self.commissioning_w0 = nil
    # self.commissioning_w1 = nil
    self.commissioning_L = nil
    self.commissioning_admin_fabric = nil
  end
  def is_commissioning_open()
    return self.commissioning_open != nil
  end
  
  #############################################################
  # (internal) Compute the PBKDF parameters for SPAKE2+ from root parameters
  #
  def _compute_pbkdf(passcode_int, iterations, salt)
    import crypto
    import string
    var passcode = bytes().add(passcode_int, 4)

    var tv = crypto.PBKDF2_HMAC_SHA256().derive(passcode, salt, iterations, 80)
    var w0s = tv[0..39]
    var w1s = tv[40..79]

    self.root_w0 = crypto.EC_P256().mod(w0s)
    var w1 = crypto.EC_P256().mod(w1s)    # w1 is temporarily computed then discarded
    # self.root_w1 = crypto.EC_P256().mod(w1s)
    self.root_L = crypto.EC_P256().public_key(w1)

    # tasmota.log("MTR: ******************************", 4)
    # tasmota.log("MTR: salt          = " + self.root_salt.tohex(), 4)
    # tasmota.log("MTR: passcode_hex  = " + passcode.tohex(), 4)
    # tasmota.log("MTR: w0            = " + self.root_w0.tohex(), 4)
    # tasmota.log("MTR: L             = " + self.root_L.tohex(), 4)
    # tasmota.log("MTR: ******************************", 4)
  end

  #############################################################
  # Compute QR Code content - can be done only for root PASE
  def compute_qrcode_content()
    var raw = bytes().resize(11)    # we don't use TLV Data so it's only 88 bits or 11 bytes
    # version is `000` dont touch
    raw.setbits(3, 16, self.vendorid)
    raw.setbits(19, 16, self.productid)
    # custom flow = 0 (offset=35, len=2)
    raw.setbits(37, 8, 0x04)        # already on IP network
    raw.setbits(45, 12, self.root_discriminator & 0xFFF)
    raw.setbits(57, 27, self.root_passcode & 0x7FFFFFF)
    # padding (offset=84 len=4)
    return "MT:" + matter.Base38.encode(raw)
  end


  #############################################################
  # Compute the 11 digits manual pairing code (wihout vendorid nor productid) p.223
  # <BR>
  # can be done only for root PASE (we need the passcode, but we don't get it with OpenCommissioningWindow command)
  def compute_manual_pairing_code()
    import string
    var digit_1 = (self.root_discriminator & 0x0FFF) >> 10
    var digit_2_6 = ((self.root_discriminator & 0x0300) << 6) | (self.root_passcode & 0x3FFF)
    var digit_7_10 = (self.root_passcode >> 14)

    var ret = string.format("%1i%05i%04i", digit_1, digit_2_6, digit_7_10)
    ret += matter.Verhoeff.checksum(ret)
    return ret
  end

  #############################################################
  # dispatch every second click to sub-objects that need it
  def every_second()
    self.sessions.every_second()
    self.message_handler.every_second()
    if self.commissioning_open != nil && tasmota.time_reached(self.commissioning_open)    # timeout reached, close provisioning
      self.commissioning_open = nil
    end
    # call all plugins
    var idx = 0
    while idx < size(self.plugins)
      self.plugins[idx].every_second()
      idx += 1
    end
  end

  #############################################################
  # trigger a read_sensors and dispatch to plugins
  # Internally used by cron
  def _trigger_read_sensors()
    import json
    var rs_json = tasmota.read_sensors()
    if rs_json == nil   return  end
    var rs = json.load(rs_json)
    if rs != nil

      # call all plugins
      var idx = 0
      while idx < size(self.plugins)
        self.plugins[idx].parse_sensors(rs)
        idx += 1
      end

    else
      tasmota.log("MTR: unable to parse read_sensors: "+str(rs_json), 3)
    end

  end

  #############################################################
  # dispatch every 250ms click to sub-objects that need it
  def every_250ms()
    self.message_handler.every_250ms()
  end

  #############################################################
  def stop()
    tasmota.remove_driver(self)
    if self.udp_server    self.udp_server.stop() end
  end

  #############################################################
  # Callback when message is received.
  # Send to `message_handler`
  def msg_received(raw, addr, port)
    return self.message_handler.msg_received(raw, addr, port)
  end

  #############################################################
  # Global entry point for sending a message.
  # Delegates to `udp_server`
  def msg_send(msg)
    return self.udp_server.send_UDP(msg)
  end

  #############################################################
  # Signals that a ack was received.
  # Delegates to `udp_server` to remove from resending list.
  def received_ack(msg)
    return self.udp_server.received_ack(msg)
  end

  #############################################################
  # (internal) Start UDP Server
  def _start_udp(port)
    if self.udp_server    return end        # already started
    if port == nil      port = 5540 end
    tasmota.log("MTR: starting UDP server on port: " + str(port), 2)
    self.udp_server = matter.UDPServer("", port)
    self.udp_server.start(/ raw, addr, port -> self.msg_received(raw, addr, port))
  end

  #############################################################
  # Start Operational Discovery for this session
  #
  # Deferred until next tick.
  def start_operational_discovery_deferred(session)
    # defer to next click
    tasmota.set_timer(0, /-> self.start_operational_discovery(session))
  end

  #############################################################
  # Start Commissioning Complete for this session
  #
  # Deferred until next tick.
  def start_commissioning_complete_deferred(session)
    # defer to next click
    tasmota.set_timer(0, /-> self.start_commissioning_complete(session))
  end

  #############################################################
  # Start Operational Discovery for this session
  #
  # Stop Basic Commissioning and clean PASE specific values (to save memory).
  # Announce fabric entry in mDNS.
  def start_operational_discovery(session)
    import crypto
    import mdns
    import string

    self.stop_basic_commissioning()    # close all PASE commissioning information
    # clear any PBKDF information to free memory
    self.root_w0 = nil
    # self.root_w1 = nil
    self.root_L = nil

    # we keep the PASE session for 1 minute
    session.set_expire_in_seconds(60)

    self.mdns_announce_op_discovery(session.get_fabric())
  end

  #############################################################
  # Commissioning Complete
  #
  # Stop basic commissioning.
  def start_commissioning_complete(session)
    tasmota.log("MTR: *** Commissioning complete ***", 2)
    self.stop_basic_commissioning()     # by default close commissioning when it's complete
  end


  #################################################################################
  # Simple insertion sort - sorts the list in place, and returns the list
  # remove duplicates
  #################################################################################
  static def sort_distinct(l)
    # insertion sort
    for i:1..size(l)-1
      var k = l[i]
      var j = i
      while (j > 0) && (l[j-1] > k)
        l[j] = l[j-1]
        j -= 1
      end
      l[j] = k
    end
    # remove duplicate now that it's sorted
    var i = 1
    if size(l) <= 1  return l end     # no duplicate if empty or 1 element
    var prev = l[0]
    while i < size(l)
      if l[i] == prev
        l.remove(i)
      else
        prev = l[i]
        i += 1
      end
    end
    return l
  end

  #############################################################
  # Signal that an attribute has been changed and propagate
  # to any active subscription.
  #
  # Delegates to `message_handler`
  def attribute_updated(endpoint, cluster, attribute, fabric_specific)
    if fabric_specific == nil   fabric_specific = false end
    var ctx = matter.Path()
    ctx.endpoint = endpoint
    ctx.cluster = cluster
    ctx.attribute = attribute
    self.message_handler.im.subs_shop.attribute_updated_ctx(ctx, fabric_specific)
  end

  #############################################################
  # Proceed to attribute expansion (used for Attribute Read/Write/Subscribe)
  #
  # Called only when expansion is needed, so we don't need to report any error since they are ignored
  #
  # calls `cb(pi, ctx, direct)` for each attribute expanded.
  # `pi`: plugin instance targeted by the attribute (via endpoint). Note: nothing is sent if the attribute is not declared in supported attributes in plugin.
  # `ctx`: context object with `endpoint`, `cluster`, `attribute` (no `command`)
  # `direct`: `true` if the attribute is directly targeted, `false` if listed as part of a wildcard
  # returns: `true` if processed succesfully, `false` if error occured. If `direct`, the error is returned to caller, but if expanded the error is silently ignored and the attribute skipped.
  # In case of `direct` but the endpoint/cluster/attribute is not suppported, it calls `cb(nil, ctx, true)` so you have a chance to encode the exact error (UNSUPPORTED_ENDPOINT/UNSUPPORTED_CLUSTER/UNSUPPORTED_ATTRIBUTE/UNREPORTABLE_ATTRIBUTE)
  def process_attribute_expansion(ctx, cb)
    #################################################################################
    # Returns the keys of a map as a sorted list
    #################################################################################
    def keys_sorted(m)
      var l = []
      for k: m.keys()
        l.push(k)
      end
      # insertion sort
      for i:1..size(l)-1
        var k = l[i]
        var j = i
        while (j > 0) && (l[j-1] > k)
          l[j] = l[j-1]
          j -= 1
        end
        l[j] = k
      end
      return l
    end
  
    import string
    var endpoint = ctx.endpoint
    # var endpoint_mono = [ endpoint ]
    var endpoint_found = false                # did any endpoint match
    var cluster = ctx.cluster
    # var cluster_mono = [ cluster ]
    var cluster_found = false
    var attribute = ctx.attribute
    # var attribute_mono = [ attribute ]
    var attribute_found = false

    var direct = (ctx.endpoint != nil) && (ctx.cluster != nil) && (ctx.attribute != nil) # true if the target is a precise attribute, false if it results from an expansion and error are ignored

    tasmota.log(string.format("MTR: process_attribute_expansion %s", str(ctx)), 4)

    # build the list of candidates

    # list of all endpoints
    var all = {}                          # map of {endpoint: {cluster: {attributes:[pi]}}
    tasmota.log(string.format("MTR: endpoint=%s cluster=%s attribute=%s", endpoint, cluster, attribute), 4)
    for pi: self.plugins
      var ep = pi.get_endpoint()    # get supported endpoints for this plugin

      if endpoint != nil && ep != endpoint    continue      end       # skip if specific endpoint and no match
      # from now on, 'ep' is a good candidate
      if !all.contains(ep)                    all[ep] = {}  end       # create empty structure if not already in the list
      endpoint_found = true

      # now explore the cluster list for 'ep'
      var cluster_list = pi.get_cluster_list(ep)                      # cluster_list is the actual list of candidate cluster for this pluging and endpoint
      tasmota.log(string.format("MTR: pi=%s ep=%s cl_list=%s", str(pi), str(ep), str(cluster_list)), 4)
      for cl: cluster_list
        if cluster != nil && cl != cluster    continue      end       # skip if specific cluster and no match
        # from now on, 'cl' is a good candidate
        if !all[ep].contains(cl)              all[ep][cl] = {}  end
        cluster_found = true

        # now filter on attributes
        var attr_list = pi.get_attribute_list(ep, cl)
        tasmota.log(string.format("MTR: pi=%s ep=%s cl=%s at_list=%s", str(pi), str(ep), str(cl), str(attr_list)), 4)
        for at: attr_list
          if attribute != nil && at != attribute  continue  end       # skip if specific attribute and no match
          # from now on, 'at' is a good candidate
          if !all[ep][cl].contains(at)        all[ep][cl][at] = [] end
          attribute_found = true

          all[ep][cl][at].push(pi)                                    # add plugin to the list
        end
      end
    end

    # import json
    # tasmota.log("MTR: all = " + json.dump(all), 2)

    # iterate on candidates
    for ep: keys_sorted(all)
      for cl: keys_sorted(all[ep])
        for at: keys_sorted(all[ep][cl])
          for pi: all[ep][cl][at]
            tasmota.log(string.format("MTR: expansion [%02X]%04X/%04X", ep, cl, at), 3)
            ctx.endpoint = ep
            ctx.cluster = cl
            ctx.attribute = at
            var finished = cb(pi, ctx, direct)   # call the callback with the plugin and the context
            if direct && finished     return end
          end
        end
      end
    end

    # we didn't have any successful match, report an error if direct (non-expansion request)
    if direct
      # since it's a direct request, ctx has already the correct endpoint/cluster/attribute
      if   !endpoint_found      ctx.status = matter.UNSUPPORTED_ENDPOINT
      elif !cluster_found       ctx.status = matter.UNSUPPORTED_CLUSTER
      elif !attribute_found     ctx.status = matter.UNSUPPORTED_ATTRIBUTE
      else                      ctx.status = matter.UNREPORTABLE_ATTRIBUTE
      end
      cb(nil, ctx, true)
    end
  end

  #############################################################
  # Return the list of endpoints from all plugins (distinct), exclud endpoint zero if `exclude_zero` is `true`
  def get_active_endpoints(exclude_zero)
    var ret = []
    for p:self.plugins
      var ep = p.get_endpoint()
      if exclude_zero && ep == 0   continue end
      if ret.find(ep) == nil
        ret.push(ep)
      end
    end
    return ret
  end

  #############################################################
  # Persistance of Matter Device parameters
  #
  #############################################################
  # 
  def save_param()
    import json
    var j = json.dump({'distinguish':self.root_discriminator, 'passcode':self.root_passcode, 'ipv4only':self.ipv4only})
    try
      import string
      var f = open(self.FILENAME, "w")
      f.write(j)
      f.close()
      return j
    except .. as e, m
      tasmota.log("MTR: Session_Store::save Exception:" + str(e) + "|" + str(m), 2)
      return j
    end
  end

  #############################################################
  # Load Matter Device parameters
  def load_param()
    import string
    import crypto
    try

      var f = open(self.FILENAME)
      var s = f.read()
      f.close()

      import json
      var j = json.load(s)

      self.root_discriminator = j.find("distinguish", self.root_discriminator)
      self.root_passcode = j.find("passcode", self.root_passcode)
      self.ipv4only = bool(j.find("ipv4only", false))
    except .. as e, m
      if e != "io_error"
        tasmota.log("MTR: Session_Store::load Exception:" + str(e) + "|" + str(m), 2)
      end
    end

    var dirty = false
    if self.root_discriminator == nil
      self.root_discriminator = crypto.random(2).get(0,2) & 0xFFF
      dirty = true
    end
    if self.root_passcode == nil
      self.root_passcode = self.PASSCODE_DEFAULT
      dirty = true
    end
    if dirty    self.save_param() end
  end


  #############################################################
  # Matter plugin management
  #
  # Plugins allow to specify response to read/write attributes
  # and command invokes
  #############################################################
  def invoke_request(session, val, ctx)
    var idx = 0
    var endpoint = ctx.endpoint
    while idx < size(self.plugins)
      var plugin = self.plugins[idx]

      if plugin.endpoint == endpoint
        return plugin.invoke_request(session, val, ctx)
      end

      idx += 1
    end
    ctx.status = matter.UNSUPPORTED_ENDPOINT
  end

  #############################################################
  # mDNS Configuration
  #############################################################
  # Start mDNS and announce hostnames for Wifi and ETH from MAC
  #
  # When the announce is active, `hostname_wifi` and `hostname_eth`
  # are defined
  def start_mdns_announce_hostnames()
    if tasmota.wifi()['up']
      self._mdns_announce_hostname(false)
    else
      tasmota.add_rule("Wifi#Connected", def ()
          self._mdns_announce_hostname(false)
          tasmota.remove_rule("Wifi#Connected", "matter_mdns_host")
        end, "matter_mdns_host")
    end

    if tasmota.eth()['up']
      self._mdns_announce_hostname(true)
    else
      tasmota.add_rule("Eth#Connected", def ()
          self._mdns_announce_hostname(true)
          tasmota.remove_rule("Eth#Connected", "matter_mdns_host")
        end, "matter_mdns_host")
    end
  end

  #############################################################
  # Start UDP mDNS announcements hostname
  # This announcement is independant from commissioning windows
  #
  # eth is `true` if ethernet turned up, `false` is wifi turned up
  def _mdns_announce_hostname(is_eth)
    import mdns
    import string

    mdns.start()

    try
      if is_eth
        # Add Hostname (based on MAC) with IPv4/IPv6 addresses
        var eth = tasmota.eth()
        self.hostname_eth  = string.replace(eth.find("mac"), ':', '')
        if !self.ipv4only
          tasmota.log(string.format("MTR: calling mdns.add_hostname(%s, %s, %s)", self.hostname_eth, eth.find('ip6local',''), eth.find('ip','')), 3)
          mdns.add_hostname(self.hostname_eth, eth.find('ip6local',''), eth.find('ip',''), eth.find('ip6',''))
        else
          tasmota.log(string.format("MTR: calling mdns.add_hostname(%s, %s)", self.hostname_eth, eth.find('ip','')), 3)
          mdns.add_hostname(self.hostname_eth, eth.find('ip',''))
        end
      else
        var wifi = tasmota.wifi()
        self.hostname_wifi = string.replace(wifi.find("mac"), ':', '')
        if !self.ipv4only
          tasmota.log(string.format("MTR: calling mdns.add_hostname(%s, %s, %s)", self.hostname_wifi, wifi.find('ip6local',''), wifi.find('ip','')), 3)
          mdns.add_hostname(self.hostname_wifi, wifi.find('ip6local',''), wifi.find('ip',''), wifi.find('ip6',''))
        else
          tasmota.log(string.format("MTR: calling mdns.add_hostname(%s, %s)", self.hostname_eth, wifi.find('ip','')), 3)
          mdns.add_hostname(self.hostname_wifi, wifi.find('ip',''))
        end
      end
      tasmota.log(string.format("MTR: start mDNS on %s host '%s.local'", is_eth ? "eth" : "wifi", is_eth ? self.hostname_eth : self.hostname_wifi), 2)
    except .. as e, m
      tasmota.log("MTR: Exception" + str(e) + "|" + str(m), 2)
    end

    self.mdns_announce_op_discovery_all_fabrics()
  end

  #############################################################
  # Announce MDNS for PASE commissioning
  def mdns_announce_PASE()
    import mdns
    import string
    import crypto

    var services = {
      "VP":str(self.vendorid) + "+" + str(self.productid),
      "D": self.commissioning_discriminator,
      "CM":1,                           # requires passcode
      "T":0,                            # no support for TCP
      "SII":5000, "SAI":300
    }

    self.commissioning_instance_wifi = crypto.random(8).tohex()    # 16 characters random hostname
    self.commissioning_instance_eth = crypto.random(8).tohex()    # 16 characters random hostname

    try
      if self.hostname_eth
        # Add Matter `_matterc._udp` service
        tasmota.log(string.format("MTR: calling mdns.add_service(%s, %s, %i, %s, %s, %s)", "_matterc", "_udp", 5540, str(services), self.commissioning_instance_eth, self.hostname_eth), 3)
        mdns.add_service("_matterc", "_udp", 5540, services, self.commissioning_instance_eth, self.hostname_eth)
        self.mdns_pase_eth = true

        tasmota.log(string.format("MTR: announce mDNS on %s '%s' ptr to `%s.local`", "eth", self.commissioning_instance_eth, self.hostname_eth), 2)

        # `mdns.add_subtype(service:string, proto:string, instance:string, hostname:string, subtype:string) -> nil`
        var subtype = "_L" + str(self.commissioning_discriminator & 0xFFF)
        tasmota.log("MTR: adding subtype: "+subtype, 2)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_eth, self.hostname_eth, subtype)
        subtype = "_S" + str((self.commissioning_discriminator & 0xF00) >> 8)
        tasmota.log("MTR: adding subtype: "+subtype, 2)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_eth, self.hostname_eth, subtype)
        subtype = "_V" + str(self.vendorid)
        tasmota.log("MTR: adding subtype: "+subtype, 2)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_eth, self.hostname_eth, subtype)
        subtype = "_CM1"
        tasmota.log("MTR: adding subtype: "+subtype, 2)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_eth, self.hostname_eth, subtype)
      end
      if self.hostname_wifi

        tasmota.log(string.format("MTR: calling mdns.add_service(%s, %s, %i, %s, %s, %s)", "_matterc", "_udp", 5540, str(services), self.commissioning_instance_wifi, self.hostname_wifi), 3)
        mdns.add_service("_matterc", "_udp", 5540, services, self.commissioning_instance_wifi, self.hostname_wifi)
        self.mdns_pase_wifi = true

        tasmota.log(string.format("MTR: starting mDNS on %s '%s' ptr to `%s.local`", "wifi", self.commissioning_instance_wifi, self.hostname_wifi), 2)

        # `mdns.add_subtype(service:string, proto:string, instance:string, hostname:string, subtype:string) -> nil`
        var subtype = "_L" + str(self.commissioning_discriminator & 0xFFF)
        tasmota.log("MTR: adding subtype: "+subtype, 2)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_wifi, self.hostname_wifi, subtype)
        subtype = "_S" + str((self.commissioning_discriminator & 0xF00) >> 8)
        tasmota.log("MTR: adding subtype: "+subtype, 2)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_wifi, self.hostname_wifi, subtype)
        subtype = "_V" + str(self.vendorid)
        tasmota.log("MTR: adding subtype: "+subtype, 2)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_wifi, self.hostname_wifi, subtype)
        subtype = "_CM1"
        tasmota.log("MTR: adding subtype: "+subtype, 2)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_wifi, self.hostname_wifi, subtype)
      end
    except .. as e, m
      tasmota.log("MTR: Exception" + str(e) + "|" + str(m), 2)
    end

  end

  #############################################################
  # MDNS remove any PASE announce
  def mdns_remove_PASE()
    import mdns
    import string

    try
      if self.mdns_pase_eth
        tasmota.log(string.format("MTR: calling mdns.remove_service(%s, %s, %s, %s)", "_matterc", "_udp", self.commissioning_instance_eth, self.hostname_eth), 3)
        tasmota.log(string.format("MTR: remove mDNS on %s '%s'", "eth", self.commissioning_instance_eth), 2)
        self.mdns_pase_eth = false
        mdns.remove_service("_matterc", "_udp", self.commissioning_instance_eth, self.hostname_eth)
      end
      if self.mdns_pase_wifi
        tasmota.log(string.format("MTR: calling mdns.remove_service(%s, %s, %s, %s)", "_matterc", "_udp", self.commissioning_instance_wifi, self.hostname_wifi), 3)
        tasmota.log(string.format("MTR: remove mDNS on %s '%s'", "wifi", self.commissioning_instance_wifi), 2)
        self.mdns_pase_wifi = false
        mdns.remove_service("_matterc", "_udp", self.commissioning_instance_wifi, self.hostname_wifi)
      end
    except .. as e, m
      tasmota.log("MTR: Exception" + str(e) + "|" + str(m), 2)
    end
  end

  #############################################################
  # Start UDP mDNS announcements for commissioning for all persisted sessions
  def mdns_announce_op_discovery_all_fabrics()
    for fabric: self.sessions.active_fabrics()
      if fabric.get_device_id() && fabric.get_fabric_id()
        self.mdns_announce_op_discovery(fabric)
      end
    end
  end

  #############################################################
  # Start UDP mDNS announcements for commissioning
  def mdns_announce_op_discovery(fabric)
    import mdns
    import string
    try
      var device_id = fabric.get_device_id().copy().reverse()
      var k_fabric = fabric.get_fabric_compressed()
      var op_node = k_fabric.tohex() + "-" + device_id.tohex()
      tasmota.log("MTR: Operational Discovery node = " + op_node, 2)

      # mdns
      if (tasmota.eth().find("up"))
        tasmota.log(string.format("MTR: adding mDNS on %s '%s' ptr to `%s.local`", "eth", op_node, self.hostname_eth), 3)
        mdns.add_service("_matter","_tcp", 5540, nil, op_node, self.hostname_eth)
        var subtype = "_I" + k_fabric.tohex()
        tasmota.log("MTR: adding subtype: "+subtype, 3)
        mdns.add_subtype("_matter", "_tcp", op_node, self.hostname_eth, subtype)
      end
      if (tasmota.wifi().find("up"))
        tasmota.log(string.format("MTR: adding mDNS on %s '%s' ptr to `%s.local`", "wifi", op_node, self.hostname_wifi), 3)
        mdns.add_service("_matter","_tcp", 5540, nil, op_node, self.hostname_wifi)
        var subtype = "_I" + k_fabric.tohex()
        tasmota.log("MTR: adding subtype: "+subtype, 3)
        mdns.add_subtype("_matter", "_tcp", op_node, self.hostname_wifi, subtype)
      end
    except .. as e, m
      tasmota.log("MTR: Exception" + str(e) + "|" + str(m), 2)
    end
  end

  #############################################################
  # Remove all mDNS announces for all fabrics
  def mdns_remove_op_discovery_all_fabrics()
    for fabric: self.sessions.active_fabrics()
      if fabric.get_device_id() && fabric.get_fabric_id()
        self.mdns_remove_op_discovery(fabric)
      end
    end
  end

  #############################################################
  # Remove mDNS announce for fabric
  def mdns_remove_op_discovery(fabric)
    import mdns
    import string
    try
      var device_id = fabric.get_device_id().copy().reverse()
      var k_fabric = fabric.get_fabric_compressed()
      var op_node = k_fabric.tohex() + "-" + device_id.tohex()

      # mdns
      if (tasmota.eth().find("up"))
        tasmota.log(string.format("MTR: remove mDNS on %s '%s'", "eth", op_node), 2)
        mdns.remove_service("_matter", "_tcp", op_node, self.hostname_eth)
      end
      if (tasmota.wifi().find("up"))
        tasmota.log(string.format("MTR: remove mDNS on %s '%s'", "wifi", op_node), 2)
        mdns.remove_service("_matter", "_tcp", op_node, self.hostname_wifi)
      end
    except .. as e, m
      tasmota.log("MTR: Exception" + str(e) + "|" + str(m), 2)
    end
  end

  #############################################################
  # Try to clean MDNS entries before restart.
  #
  # Called by Tasmota loop as a Tasmota driver.
  def save_before_restart()
    self.stop_basic_commissioning()
    self.mdns_remove_op_discovery_all_fabrics()
  end

  #############################################################
  # Autoconfigure device from template
  #
  def autoconf_device()
    import string
    # check if we have a light
    var endpoint = 1
    var light_present = false

    import light
    var light_status = light.get()
    if light_status != nil
      var channels_count = size(light_status.find('channels', ""))
      if channels_count > 0
        if   channels_count == 1
          self.plugins.push(matter.Plugin_Light1(self, endpoint))
          tasmota.log(string.format("MTR: Endpoint:%i Light_Dimmer", endpoint), 2)
        elif channels_count == 2
          self.plugins.push(matter.Plugin_Light2(self, endpoint))
          tasmota.log(string.format("MTR: Endpoint:%i Light_CT", endpoint), 2)
        else
          self.plugins.push(matter.Plugin_Light3(self, endpoint))
          tasmota.log(string.format("MTR: Endpoint:%i Light_RGB", endpoint), 2)
        end
        light_present = true
        endpoint += 1
      end
    end

    # how many relays are present
    var relay_count = size(tasmota.get_power())
    var relay_index = 0         # start at index 0
    if light_present    relay_count -= 1  end       # last power is taken for lights

    while relay_index < relay_count
      self.plugins.push(matter.Plugin_OnOff(self, endpoint, relay_index))
      tasmota.log(string.format("MTR: Endpoint:%i Relay_%i", endpoint, relay_index + 1), 2)
      relay_index += 1
      endpoint += 1
    end

  end
end
matter.Device = Matter_Device

#-
import global
global.matter_device = matter_device()
return matter_device
-#
