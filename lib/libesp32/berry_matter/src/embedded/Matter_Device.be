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

import matter

#@ solidify:Matter_Device,weak

class Matter_Device
  static var UDP_PORT = 5540          # this is the default port for group multicast, we also use it for unicast
  static var PBKDF_ITERATIONS = 1000  # I don't see any reason to choose a different number
  static var VENDOR_ID = 0xFFF1
  static var PRODUCT_ID = 0x8000
  static var FILENAME = "_matter_device.json"
  static var PASE_TIMEOUT = 10*60     # default open commissioning window (10 minutes)
  var started                         # is the Matter Device started (configured, mDNS and UDPServer started)
  var plugins                         # list of plugins instances
  var plugins_persist                 # true if plugins configuration needs to be saved
  var plugins_classes                 # map of registered classes by type name
  var plugins_config                  # map of JSON configuration for plugins
  var plugins_config_remotes          # map of information on each remote under "remotes" key, '{}' when empty
  var udp_server                      # `matter.UDPServer()` object
  var profiler
  var message_handler                 # `matter.MessageHandler()` object
  var sessions                        # `matter.Session_Store()` objet
  var ui
  var tick                            # increment at each tick, avoids to repeat too frequently some actions
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
  # for brige mode, list of HTTP_remote objects (only one instance per remote object)
  var http_remotes                    # map of 'domain:port' to `Matter_HTTP_remote` instance or `nil` if no bridges
  # saved in parameters
  var root_discriminator              # as `int`
  var root_passcode                   # as `int`
  var ipv4only                        # advertize only IPv4 addresses (no IPv6)
  var disable_bridge_mode             # default is bridge mode, this flag disables this mode for some non-compliant controllers
  var next_ep                         # next endpoint to be allocated for bridge, start at 1
  # context for PBKDF
  var root_iterations                 # PBKDF number of iterations
  # PBKDF information used only during PASE (freed afterwards)
  var root_salt
  var root_w0
  var root_L

  #############################################################
  def init()
    import crypto
    if !tasmota.get_option(matter.MATTER_OPTION)
      matter.UI(self)   # minimal UI
      return
    end    # abort if SetOption 151 is not set

    matter.profiler = matter.Profiler()
    self.started = false
    self.tick = 0
    self.plugins = []
    self.plugins_persist = false                  # plugins need to saved only when the first fabric is associated
    self.plugins_classes = {}
    self.plugins_config_remotes = {}
    self.register_native_classes()                # register all native classes
    self.vendorid = self.VENDOR_ID
    self.productid = self.PRODUCT_ID
    self.root_iterations = self.PBKDF_ITERATIONS
    self.next_ep = 1                              # start at endpoint 1 for dynamically allocated endpoints
    self.root_salt = crypto.random(16)
    self.ipv4only = false
    self.disable_bridge_mode = false
    self.load_param()

    self.sessions = matter.Session_Store(self)
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

    self.register_commands()
  end

  #############################################################
  # Start Matter device server when the first network is coming up
  def start()
    if self.started  return end      # abort if already started

    # autoconfigure other plugins if needed
    self.autoconf_device()

    # for now read sensors every 30 seconds
    # TODO still needed?
    tasmota.add_cron("*/30 * * * * *", def () self._trigger_read_sensors() end, "matter_sensors_30s")

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
    if timeout_s == nil   timeout_s = self.PASE_TIMEOUT end

    # show Manual pairing code in logs
    var pairing_code = self.compute_manual_pairing_code()
    tasmota.log(format("MTR: Manual pairing code: %s", pairing_code), 2)
    
    # output MQTT
    var qr_code = self.compute_qrcode_content()
    tasmota.publish_result(format('{"Matter":{"Commissioning":1,"PairingCode":"%s","QRCode":"%s"}}', pairing_code, qr_code), 'Matter')

    # compute PBKDF
    self._compute_pbkdf(self.root_passcode, self.root_iterations, self.root_salt)
    self.start_basic_commissioning(timeout_s, self.root_iterations, self.root_discriminator, self.root_salt, self.root_w0, #-self.root_w1,-# self.root_L, nil)
  end

  #####################################################################
  # Remove a fabric and clean all corresponding values and mDNS entries
  def remove_fabric(fabric)
    if fabric != nil
      tasmota.log("MTR: removing fabric " + fabric.get_fabric_id().copy().reverse().tohex(), 2)
      self.message_handler.im.subs_shop.remove_by_fabric(fabric)
      self.mdns_remove_op_discovery(fabric)
      self.sessions.remove_fabric(fabric)
    end
    # var sub_fabrics = self.sessions.find_children_fabrics(fabric_parent.get_fabric_index())
    # if sub_fabrics == nil return end
    # for fabric_index : sub_fabrics
    #   var fabric = self.sessions.find_fabric_by_index(fabric_index)
    #   if fabric != nil
    #     tasmota.log("MTR: removing fabric " + fabric.get_fabric_id().copy().reverse().tohex(), 2)
    #     self.message_handler.im.subs_shop.remove_by_fabric(fabric)
    #     self.mdns_remove_op_discovery(fabric)
    #     self.sessions.remove_fabric(fabric)
    #   end
    # end
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
    if self.is_root_commissioning_open()
      tasmota.publish_result('{"Matter":{"Commissioning":0}}', 'Matter')
    end
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
    var digit_1 = (self.root_discriminator & 0x0FFF) >> 10
    var digit_2_6 = ((self.root_discriminator & 0x0300) << 6) | (self.root_passcode & 0x3FFF)
    var digit_7_10 = (self.root_passcode >> 14)

    var ret = format("%1i%05i%04i", digit_1, digit_2_6, digit_7_10)
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
  end

  #############################################################
  # dispatch every 250ms to all plugins
  def every_250ms()
    self.message_handler.every_250ms()
    # call all plugins, use a manual loop to avoid creating a new object
    var idx = 0
    while idx < size(self.plugins)
      self.plugins[idx].every_250ms()
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
  # ticks
  def every_50ms()
    self.tick += 1
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
    tasmota.log("MTR: Starting UDP server on port: " + str(port), 2)
    self.udp_server = matter.UDPServer(self, "", port)
    self.udp_server.start(/ raw, addr, port -> self.msg_received(raw, addr, port))
  end

  #############################################################
  # Start Operational Discovery for this session
  #
  # Deferred until next tick.
  def start_operational_discovery_deferred(fabric)
    # defer to next click
    tasmota.set_timer(0, /-> self.start_operational_discovery(fabric))
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
  def start_operational_discovery(fabric)
    import crypto
    import mdns

    self.stop_basic_commissioning()    # close all PASE commissioning information
    # clear any PBKDF information to free memory
    self.root_w0 = nil
    # self.root_w1 = nil
    self.root_L = nil

    self.mdns_announce_op_discovery(fabric)
  end

  #############################################################
  # Commissioning Complete
  #
  # Stop basic commissioning.
  def start_commissioning_complete(session)
    var fabric = session.get_fabric()
    var fabric_id = fabric.get_fabric_id().copy().reverse().tohex()
    var vendor_name = fabric.get_admin_vendor_name()
    tasmota.log(f"MTR: --- Commissioning complete for Fabric '{fabric_id}' (Vendor {vendor_name}) ---", 2)
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
  
    var endpoint = ctx.endpoint
    var cluster = ctx.cluster
    var attribute = ctx.attribute
    var endpoint_found = false                # did any endpoint match
    var cluster_found = false
    var attribute_found = false

    var direct = (ctx.endpoint != nil) && (ctx.cluster != nil) && (ctx.attribute != nil) # true if the target is a precise attribute, false if it results from an expansion and error are ignored

    # tasmota.log(f"MTR: process_attribute_expansion {str(ctx))}", 4)

    # build the list of candidates

    # list of all endpoints
    var all = {}                          # map of {endpoint: {cluster: {attributes:[pi]}}
    # tasmota.log(format("MTR: endpoint=%s cluster=%s attribute=%s", endpoint, cluster, attribute), 4)
    for pi: self.plugins
      var ep = pi.get_endpoint()    # get supported endpoints for this plugin

      if endpoint != nil && ep != endpoint    continue      end       # skip if specific endpoint and no match
      # from now on, 'ep' is a good candidate
      if !all.contains(ep)                    all[ep] = {}  end       # create empty structure if not already in the list
      endpoint_found = true

      # now explore the cluster list for 'ep'
      var cluster_list = pi.get_cluster_list()                        # cluster_list is the actual list of candidate cluster for this pluging and endpoint
      # tasmota.log(format("MTR: pi=%s ep=%s cl_list=%s", str(pi), str(ep), str(cluster_list)), 4)
      for cl: cluster_list
        if cluster != nil && cl != cluster    continue      end       # skip if specific cluster and no match
        # from now on, 'cl' is a good candidate
        if !all[ep].contains(cl)              all[ep][cl] = {}  end
        cluster_found = true

        # now filter on attributes
        var attr_list = pi.get_attribute_list(cl)
        # tasmota.log(format("MTR: pi=%s ep=%s cl=%s at_list=%s", str(pi), str(ep), str(cl), str(attr_list)), 4)
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
            # tasmota.log(format("MTR: expansion [%02X]%04X/%04X", ep, cl, at), 3)
            ctx.endpoint = ep
            ctx.cluster = cl
            ctx.attribute = at
            var finished = cb(pi, ctx, direct)   # call the callback with the plugin and the context
            # tasmota.log("MTR: gc="+str(tasmota.gc()), 2)
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
  # Optimized version for a single endpoint/cluster/attribute
  #
  # Retrieve the plugin for a read
  def process_attribute_read_solo(ctx)
    var endpoint = ctx.endpoint
    # var endpoint_found = false                # did any endpoint match
    var cluster = ctx.cluster
    # var cluster_found = false
    var attribute = ctx.attribute
    # var attribute_found = false

    # all 3 elements must be non-nil
    if endpoint == nil || cluster == nil || attribute == nil      return nil    end

    # look for plugin
    var pi = self.find_plugin_by_endpoint(endpoint)
    if pi == nil                                # endpoint not found
      ctx.status = matter.UNSUPPORTED_ENDPOINT
      return nil
    end

    # check cluster
    if !pi.contains_cluster(cluster)
      ctx.status = matter.UNSUPPORTED_CLUSTER
      return nil
    end

    # attribute list
    if !pi.contains_attribute(cluster, attribute)
      ctx.status = matter.UNSUPPORTED_ATTRIBUTE
      return nil
    end

    # all good
    return pi
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
  # Find plugin by endpoint
  def find_plugin_by_endpoint(ep)
    var idx = 0
    while idx < size(self.plugins)
      var pl = self.plugins[idx]
      if pl.get_endpoint() == ep
        return pl
      end
      idx += 1
    end
    return nil
  end

  #############################################################
  # Find plugin by endpoint
  def find_plugin_by_friendly_name(name)
    if (name == nil) || (size(name) == 0)   return nil      end     # invalid name

    var idx = 0
    while idx < size(self.plugins)
      var pl = self.plugins[idx]
      var pl_name = pl.get_name()
      if (pl_name != nil) && (size(pl_name) > 0) && (pl_name == name)
        return pl
      end
      idx += 1
    end
    return nil
  end

  #############################################################
  # Persistance of Matter Device parameters
  #
  #############################################################
  # 
  def save_param()
    import json
    self.update_remotes_info()    # update self.plugins_config_remotes

    var j = format('{"distinguish":%i,"passcode":%i,"ipv4only":%s,"disable_bridge_mode":%s,"nextep":%i', self.root_discriminator, self.root_passcode, self.ipv4only ? 'true':'false', self.disable_bridge_mode ? 'true':'false', self.next_ep)
    if self.plugins_persist
      j += ',"config":'
      j += json.dump(self.plugins_config)
      if size(self.plugins_config_remotes) > 0
        j += ',"remotes":'
        j += json.dump(self.plugins_config_remotes)
      end
    end
    j += '}'
    try
      var f = open(self.FILENAME, "w")
      f.write(j)
      f.close()
      tasmota.log(format("MTR: =Saved     parameters%s", self.plugins_persist ? " and configuration" : ""), 2)
      return j
    except .. as e, m
      tasmota.log("MTR: Session_Store::save Exception:" + str(e) + "|" + str(m), 2)
      return j
    end
  end

  #############################################################
  # Get remote info by url
  #
  # Return a map, potentially empty
  def get_plugin_remote_info(url)
    return self.plugins_config_remotes.find(url, {})
  end
    
  #############################################################
  # update back all remote information from actual remotes
  #
  # returns a map or `nil` if no information
  # also stores in `self.plugins_config_remotes`
  def update_remotes_info()
    var ret = {}
    if self.http_remotes != nil
      for url:self.http_remotes.keys()
        var info = self.http_remotes[url].get_info()
        if info != nil && size(info) > 0
          ret[url] = info
        end
      end
    end

    self.plugins_config_remotes = ret
    return ret
  end

  #############################################################
  # Load Matter Device parameters
  def load_param()
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
      self.disable_bridge_mode = bool(j.find("disable_bridge_mode", false))
      self.next_ep = j.find("nextep", self.next_ep)
      self.plugins_config = j.find("config")
      if self.plugins_config != nil
        tasmota.log("MTR: load_config = " + str(self.plugins_config), 3)
        self.adjust_next_ep()
        self.plugins_persist = true
      end
      self.plugins_config_remotes = j.find("remotes", {})
      if self.plugins_config_remotes
        tasmota.log("MTR: load_remotes = " + str(self.plugins_config_remotes), 3)
      end
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
      self.root_passcode = self.generate_random_passcode()
      dirty = true
    end
    if dirty    self.save_param() end
  end

  #############################################################
  # Convert a configuration to a log string
  static def conf_to_log(plugin_conf)
    var param_log = ''
    for k:_class.k2l(plugin_conf)
      if k == 'type'  continue  end
      param_log += format(" %s:%s", k, plugin_conf[k])
    end
    return param_log
  end

  #############################################################
  # Load plugins configuration from json
  #
  # 'config' is a map
  # Ex:
  #   {'32': {'filter': 'AXP192#Temperature', 'type': 'temperature'}, '40': {'filter': 'BMP280#Pressure', 'type': 'pressure'}, '34': {'filter': 'SHT3X#Temperature', 'type': 'temperature'}, '33': {'filter': 'BMP280#Temperature', 'type': 'temperature'}, '1': {'relay': 0, 'type': 'relay'}, '56': {'filter': 'SHT3X#Humidity', 'type': 'humidity'}, '0': {'type': 'root'}}
  def _instantiate_plugins_from_config(config)
    var endpoints = self.k2l_num(config)
    # tasmota.log("MTR: endpoints to be configured "+str(endpoints), 4)
    tasmota.log("MTR: Configuring endpoints", 2)

    # start with mandatory endpoint 0 for root node
    self.plugins.push(matter.Plugin_Root(self, 0, {}))
    tasmota.log(format("MTR:   endpoint = %5i type:%s%s", 0, 'root', ''), 2)

    # always include an aggregator for dynamic endpoints
    self.plugins.push(matter.Plugin_Aggregator(self, 0xFF00, {}))

    for ep: endpoints
      if ep == 0  continue end          # skip endpoint 0
      try
        var plugin_conf = config[str(ep)]
        # tasmota.log(format("MTR: endpoint %i config %s", ep, plugin_conf), 3)

        var pi_class_name = plugin_conf.find('type')
        if pi_class_name == nil   tasmota.log("MTR: no class name, skipping", 3)  continue end
        if pi_class_name == 'root'  tasmota.log("MTR: only one root node allowed", 3)  continue end
        var pi_class = self.plugins_classes.find(pi_class_name)
        if pi_class == nil        tasmota.log("MTR: unknown class name '"+str(pi_class_name)+"' skipping", 2)  continue  end

        var pi = pi_class(self, ep, plugin_conf)
        self.plugins.push(pi)

        tasmota.log(format("MTR:   endpoint = %5i type:%s%s", ep, pi_class_name, self.conf_to_log(plugin_conf)), 2)
      except .. as e, m
        tasmota.log("MTR: Exception" + str(e) + "|" + str(m), 2)
      end
    end
    tasmota.log(format("MTR:   endpoint = %5i type:%s%s", 0xFF00, 'aggregator', ''), 2)

    tasmota.publish_result('{"Matter":{"Initialized":1}}', 'Matter')
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
        if !self.ipv4only || !eth.contains('ip6local')
          # tasmota.log(format("MTR: calling mdns.add_hostname(%s, %s, %s)", self.hostname_eth, eth.find('ip6local',''), eth.find('ip','')), 4)
          mdns.add_hostname(self.hostname_eth, eth.find('ip6local',''), eth.find('ip',''), eth.find('ip6',''))
        else
          tasmota.log(format("MTR: calling mdns.add_hostname(%s, %s)", self.hostname_eth, eth.find('ip','')), 3)
          mdns.add_hostname(self.hostname_eth, eth.find('ip',''))
        end
      else
        var wifi = tasmota.wifi()
        self.hostname_wifi = string.replace(wifi.find("mac"), ':', '')
        if !self.ipv4only || !wifi.contains('ip6local')
          # tasmota.log(format("MTR: calling mdns.add_hostname(%s, %s, %s)", self.hostname_wifi, wifi.find('ip6local',''), wifi.find('ip','')), 4)
          mdns.add_hostname(self.hostname_wifi, wifi.find('ip6local',''), wifi.find('ip',''), wifi.find('ip6',''))
        else
          tasmota.log(format("MTR: calling mdns.add_hostname(%s, %s)", self.hostname_wifi, wifi.find('ip','')), 3)
          mdns.add_hostname(self.hostname_wifi, wifi.find('ip',''))
        end
      end
      tasmota.log(format("MTR: start mDNS on %s host '%s.local'", is_eth ? "eth" : "wifi", is_eth ? self.hostname_eth : self.hostname_wifi), 3)
    except .. as e, m
      tasmota.log("MTR: Exception" + str(e) + "|" + str(m), 2)
    end

    self.mdns_announce_op_discovery_all_fabrics()
  end

  #############################################################
  # Announce MDNS for PASE commissioning
  def mdns_announce_PASE()
    import mdns
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
        # tasmota.log(format("MTR: calling mdns.add_service(%s, %s, %i, %s, %s, %s)", "_matterc", "_udp", 5540, str(services), self.commissioning_instance_eth, self.hostname_eth), 4)
        mdns.add_service("_matterc", "_udp", 5540, services, self.commissioning_instance_eth, self.hostname_eth)
        self.mdns_pase_eth = true

        tasmota.log(format("MTR: announce mDNS on %s '%s' ptr to `%s.local`", "eth", self.commissioning_instance_eth, self.hostname_eth), 2)

        # `mdns.add_subtype(service:string, proto:string, instance:string, hostname:string, subtype:string) -> nil`
        var subtype = "_L" + str(self.commissioning_discriminator & 0xFFF)
        tasmota.log("MTR: adding subtype: "+subtype, 3)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_eth, self.hostname_eth, subtype)
        subtype = "_S" + str((self.commissioning_discriminator & 0xF00) >> 8)
        tasmota.log("MTR: adding subtype: "+subtype, 3)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_eth, self.hostname_eth, subtype)
        subtype = "_V" + str(self.vendorid)
        tasmota.log("MTR: adding subtype: "+subtype, 3)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_eth, self.hostname_eth, subtype)
        subtype = "_CM1"
        tasmota.log("MTR: adding subtype: "+subtype, 3)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_eth, self.hostname_eth, subtype)
      end
      if self.hostname_wifi

        # tasmota.log(format("MTR: calling mdns.add_service(%s, %s, %i, %s, %s, %s)", "_matterc", "_udp", 5540, str(services), self.commissioning_instance_wifi, self.hostname_wifi), 4)
        mdns.add_service("_matterc", "_udp", 5540, services, self.commissioning_instance_wifi, self.hostname_wifi)
        self.mdns_pase_wifi = true

        tasmota.log(format("MTR: starting mDNS on %s '%s' ptr to `%s.local`", "wifi", self.commissioning_instance_wifi, self.hostname_wifi), 3)

        # `mdns.add_subtype(service:string, proto:string, instance:string, hostname:string, subtype:string) -> nil`
        var subtype = "_L" + str(self.commissioning_discriminator & 0xFFF)
        tasmota.log("MTR: adding subtype: "+subtype, 3)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_wifi, self.hostname_wifi, subtype)
        subtype = "_S" + str((self.commissioning_discriminator & 0xF00) >> 8)
        tasmota.log("MTR: adding subtype: "+subtype, 3)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_wifi, self.hostname_wifi, subtype)
        subtype = "_V" + str(self.vendorid)
        tasmota.log("MTR: adding subtype: "+subtype, 3)
        mdns.add_subtype("_matterc", "_udp", self.commissioning_instance_wifi, self.hostname_wifi, subtype)
        subtype = "_CM1"
        tasmota.log("MTR: adding subtype: "+subtype, 3)
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

    try
      if self.mdns_pase_eth
        tasmota.log(format("MTR: calling mdns.remove_service(%s, %s, %s, %s)", "_matterc", "_udp", self.commissioning_instance_eth, self.hostname_eth), 3)
        tasmota.log(format("MTR: remove mDNS on %s '%s'", "eth", self.commissioning_instance_eth), 3)
        self.mdns_pase_eth = false
        mdns.remove_service("_matterc", "_udp", self.commissioning_instance_eth, self.hostname_eth)
      end
      if self.mdns_pase_wifi
        tasmota.log(format("MTR: calling mdns.remove_service(%s, %s, %s, %s)", "_matterc", "_udp", self.commissioning_instance_wifi, self.hostname_wifi), 3)
        tasmota.log(format("MTR: remove mDNS on %s '%s'", "wifi", self.commissioning_instance_wifi), 3)
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
    try
      var device_id = fabric.get_device_id().copy().reverse()
      var k_fabric = fabric.get_fabric_compressed()
      var op_node = k_fabric.tohex() + "-" + device_id.tohex()
      tasmota.log("MTR: Operational Discovery node = " + op_node, 3)

      # mdns
      if (tasmota.eth().find("up"))
        tasmota.log(format("MTR: adding mDNS on %s '%s' ptr to `%s.local`", "eth", op_node, self.hostname_eth), 3)
        mdns.add_service("_matter","_tcp", 5540, nil, op_node, self.hostname_eth)
        var subtype = "_I" + k_fabric.tohex()
        tasmota.log("MTR: adding subtype: "+subtype, 3)
        mdns.add_subtype("_matter", "_tcp", op_node, self.hostname_eth, subtype)
      end
      if (tasmota.wifi().find("up"))
        tasmota.log(format("MTR: adding mDNS on %s '%s' ptr to `%s.local`", "wifi", op_node, self.hostname_wifi), 3)
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
    try
      var device_id = fabric.get_device_id().copy().reverse()
      var k_fabric = fabric.get_fabric_compressed()
      var op_node = k_fabric.tohex() + "-" + device_id.tohex()

      # mdns
      if (tasmota.eth().find("up"))
        tasmota.log(format("MTR: remove mDNS on %s '%s'", "eth", op_node), 3)
        mdns.remove_service("_matter", "_tcp", op_node, self.hostname_eth)
      end
      if (tasmota.wifi().find("up"))
        tasmota.log(format("MTR: remove mDNS on %s '%s'", "wifi", op_node), 3)
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
  # Applies only if there are no plugins already configured
  ## TODO generate map instead
  def autoconf_device()
    import json

    if size(self.plugins) > 0   return end                    # already configured

    if !self.plugins_persist
      self.plugins_config = self.autoconf_device_map()
      self.plugins_config_remotes = {}
      self.adjust_next_ep()
      tasmota.log("MTR: autoconfig = " + str(self.plugins_config), 3)
    end
    self._instantiate_plugins_from_config(self.plugins_config)

    if !self.plugins_persist && self.sessions.count_active_fabrics() > 0
      self.plugins_persist = true
      self.save_param()
    end
  end

  #############################################################
  # Autoconfigure device from template to map
  #
  # Applies only if there are no plugins already configured
  def autoconf_device_map()
    import json
    var m = {}

    # check if we have a light
    var endpoint = 1
    var light_present = false

    import light
    var light_status = light.get()
    if light_status != nil
      var channels_count = size(light_status.find('channels', ""))
      if channels_count > 0
        if   channels_count == 1
          m[str(endpoint)] = {'type':'light1'}
        elif channels_count == 2
          m[str(endpoint)] = {'type':'light2'}
        else
          m[str(endpoint)] = {'type':'light3'}
        end
        light_present = true
        endpoint += 1
      end
    end

    # handle shutters before relays (as we steal relays for shutters)
    var r_st13 = tasmota.cmd("Status 13", true)     # issue `Status 13`
    var relays_reserved = []                        # list of relays that are used for non-relay (shutters)
    tasmota.log("MTR: Status 13 = "+str(r_st13), 3)

    if r_st13 != nil && r_st13.contains('StatusSHT')
      r_st13 = r_st13['StatusSHT']        # skip root
      # Shutter is enabled, iterate
      var idx = 0
      while true
        var k = 'SHT' + str(idx)                    # SHT is zero based
        if !r_st13.contains(k)   break     end           # no more SHTxxx
        var d = r_st13[k]
        tasmota.log(format("MTR: '%s' = %s", k, str(d)), 3)
        var relay1 = d.find('Relay1', -1)           # relay base 1 or -1 if none
        var relay2 = d.find('Relay2', -1)           # relay base 1 or -1 if none

        if relay1 > 0    relays_reserved.push(relay1 - 1)    end   # mark relay1/2 as non-relays
        if relay2 > 0    relays_reserved.push(relay2 - 1)    end

        tasmota.log(f"MTR: {relay1=} {relay2=}", 3)
        # is there tilt support
        var tilt_array = d.find('TiltConfig')
        var tilt_config = tilt_array && (tilt_array[2] > 0)
        # add shutter to definition
        m[str(endpoint)] = {'type': tilt_config ? 'shutter+tilt' : 'shutter', 'shutter':idx}
        endpoint += 1
        idx += 1
      end

    end

    # how many relays are present
    var relay_count = size(tasmota.get_power())
    var relay_index = 0         # start at index 0
    if light_present    relay_count -= 1  end       # last power is taken for lights

    while relay_index < relay_count
      if relays_reserved.find(relay_index) == nil   # if relay is actual relay
        m[str(endpoint)] = {'type':'relay','relay':relay_index + 1}     # Relay index start with 1
        endpoint += 1
      end
      relay_index += 1
    end

    # auto-detect sensors
    var sensors = json.load(tasmota.read_sensors())

    var sensors_list = self.autoconf_sensors_list(sensors)

    for s: sensors_list
      m[str(endpoint)] = s
      endpoint += 1
    end

    # tasmota.publish_result('{"Matter":{"Initialized":1}}', 'Matter')    # MQTT is not yet connected
    return m
  end


  #############################################################
  # Autoconfigure from sensors
  #
  # Returns an ordered list
  def autoconf_sensors_list(sensors)
    var ret = []
    # temperature sensors
    for k1:self.k2l(sensors)
      var sensor_2 = sensors[k1]
      if isinstance(sensor_2, map) && sensor_2.contains("Temperature")
        var temp_rule = k1 + "#Temperature"
        ret.push({'type':'temperature','filter':temp_rule})
      end
    end

    # pressure sensors
    for k1:self.k2l(sensors)
      var sensor_2 = sensors[k1]
      if isinstance(sensor_2, map) && sensor_2.contains("Pressure")
        var temp_rule = k1 + "#Pressure"
        ret.push({'type':'pressure','filter':temp_rule})
      end
    end

    # light sensors
    for k1:self.k2l(sensors)
      var sensor_2 = sensors[k1]
      if isinstance(sensor_2, map) && sensor_2.contains("Illuminance")
        var temp_rule = k1 + "#Illuminance"
        ret.push({'type':'illuminance','filter':temp_rule})
      end
    end

    # huidity sensors
    for k1:self.k2l(sensors)
      var sensor_2 = sensors[k1]
      if isinstance(sensor_2, map) && sensor_2.contains("Humidity")
        var temp_rule = k1 + "#Humidity"
        ret.push({'type':'humidity','filter':temp_rule})
      end
    end

    return ret
  end

  # get keys of a map in sorted order
  static def k2l(m) var l=[] if m==nil return l end for k:m.keys() l.push(k) end
    for i:1..size(l)-1 var k = l[i] var j = i while (j > 0) && (l[j-1] > k) l[j] = l[j-1] j -= 1 end l[j] = k end return l
  end

  # get keys of a map in sorted order, as numbers
  static def k2l_num(m) var l=[] if m==nil return l end for k:m.keys() l.push(int(k)) end
    for i:1..size(l)-1 var k = l[i] var j = i while (j > 0) && (l[j-1] > k) l[j] = l[j-1] j -= 1 end l[j] = k end return l
  end

  #############################################################
  # register_plugin_class
  #
  # Adds a class by name
  def register_plugin_class(cl)
    import introspect
    var typ = introspect.get(cl, 'TYPE')      # make sure we don't crash if TYPE does not exist
    if typ
      self.plugins_classes[typ] = cl
    end
  end

  #############################################################
  # get_plugin_class_displayname
  #
  # get a class name light "light0" and return displayname
  def get_plugin_class_displayname(name)
    var cl = self.plugins_classes.find(name)
    return cl ? cl.DISPLAY_NAME : ""
  end

  #############################################################
  # get_plugin_class_arg
  #
  # get a class name light "light0" and return the name of the json argumen (or empty)
  def get_plugin_class_arg(name)
    var cl = self.plugins_classes.find(name)
    return cl ? cl.ARG : ""
  end

  #############################################################
  # register_native_classes
  #
  # Adds a class by name
  def register_native_classes(name, cl)
    # try to register any class that starts with 'Plugin_'
    import introspect
    import string
    for k: introspect.members(matter)
      var v = introspect.get(matter, k)
      if type(v) == 'class' && string.find(k, "Plugin_") == 0
        self.register_plugin_class(v)
      end
    end
    # tasmota.log("MTR: registered classes "+str(self.k2l(self.plugins_classes)), 3)
  end
  
  #############################################################
  # Dynamic adding and removal of endpoints (bridge mode)
  #############################################################
  # Add endpoint
  #
  # Args:
  # `pi_class_name`: name of the type of pluging, ex: `light3`
  # `plugin_conf`: map of configuration as native Berry map
  # returns endpoint number newly allocated, or `nil` if failed
  def bridge_add_endpoint(pi_class_name, plugin_conf)
    var pi_class = self.plugins_classes.find(pi_class_name)
    if pi_class == nil        tasmota.log("MTR: unknown class name '"+str(pi_class_name)+"' skipping", 3)  return  end

    # get the next allocated endpoint number
    var ep = self.next_ep
    var ep_str = str(ep)

    var pi = pi_class(self, ep, plugin_conf)
    self.plugins.push(pi)

    # add to in-memoru config
    # Example: {'filter': 'AXP192#Temperature', 'type': 'temperature'}
    var pi_conf = {'type': pi_class_name}
    # copy args
    for k:plugin_conf.keys()
      pi_conf[k] = plugin_conf[k]
    end
    # add to main
    tasmota.log(format("MTR: adding endpoint = %i type:%s%s", ep, pi_class_name, self.conf_to_log(plugin_conf)), 2)
    self.plugins_config[ep_str] = pi_conf
    self.plugins_persist = true
    self.next_ep += 1     # increment next allocated endpoint before saving

    # try saving parameters
    self.save_param()
    self.signal_endpoints_changed()

    return ep
  end

  #############################################################
  # Remove an existing endpoint
  #
  def bridge_remove_endpoint(ep)
    import json

    var ep_str = str(ep)
    var config
    var f_in

    if !self.plugins_config.contains(ep_str)
      tasmota.log("MTR: Cannot remove an enpoint not configured: " + ep_str, 3)
      return
    end
    tasmota.log(format("MTR: deleting endpoint = %i", ep), 2)
    self.plugins_config.remove(ep_str)
    self.plugins_persist = true

    # now remove from in-memory configuration
    var idx = 0
    while idx < size(self.plugins)
      if ep == self.plugins[idx].get_endpoint()
        self.plugins.remove(idx)
        break
      else
        idx += 1
      end
    end

    # clean any orphan remote
    self.clean_remotes()

    # try saving parameters
    self.save_param()
    self.signal_endpoints_changed()
  end

  #############################################################
  # Signal to controller that endpoints changed via subcriptions
  #
  def signal_endpoints_changed()
    # mark parts lists as changed
    self.attribute_updated(0x0000, 0x001D, 0x0003, false)
    self.attribute_updated(0xFF00, 0x001D, 0x0003, false)
  end

  #############################################################
  # Adjust next_ep
  #
  # Make sure that next_ep (used to allow dynamic endpoints)
  # will not collide with an existing ep
  def adjust_next_ep()
    for k: self.plugins_config.keys()
      var ep = int(k)
      if ep >= self.next_ep
        self.next_ep = ep + 1
      end
    end
  end

  #####################################################################
  # Events
  #####################################################################
  def event_fabrics_saved()
    # if the plugins configuration was not persisted and a new fabric is saved, persist it
    if self.sessions.count_active_fabrics() > 0 && !self.plugins_persist
      self.plugins_persist = true
      self.save_param()
    end
  end

  #####################################################################
  # Generate random passcode
  #####################################################################
  static var PASSCODE_INVALID = [ 0, 11111111, 22222222, 33333333, 44444444, 55555555, 66666666, 77777777, 88888888, 99999999, 12345678, 87654321]
  def generate_random_passcode()
    import crypto
    var passcode
    while true
      passcode = crypto.random(4).get(0, 4) & 0x7FFFFFF
      if passcode > 0x5F5E0FE     continue      end         # larger than allowed
      for inv: self.PASSCODE_INVALID
        if passcode == inv    passcode = nil    end
      end
      if passcode != nil    return passcode     end
    end
  end

  #####################################################################
  # Manager HTTP remotes
  #####################################################################
  # register new http remote
  #
  # If already registered, return current instance and check timeout
  def register_http_remote(addr, timeout)
    if self.http_remotes == nil     self.http_remotes = {}    end     # lazy initialization
    var http_remote

    if self.http_remotes.contains(addr)
      http_remote = self.http_remotes[addr]
      if timeout < http_remote.get_timeout()
        http_remote.set_timeout(timeout)          # reduce timeout if new value is shorter
      end
    else
      http_remote = matter.HTTP_remote(self, addr, timeout)
      if self.plugins_config_remotes.contains(addr)
        http_remote.set_info(self.plugins_config_remotes[addr])
      end
      self.http_remotes[addr] = http_remote
    end
    return http_remote
  end

  #####################################################################
  # Remove HTTP remotes that are no longer referenced
  def clean_remotes()
    import introspect

    # print("clean_remotes", self.http_remotes)
    # init all remotes with count 0
    if self.http_remotes      # tests if `self.http_remotes` is not `nil` and not empty
      var remotes_map = {}    # key: remote object, value: count of references
  
      for http_remote: self.http_remotes
        remotes_map[http_remote] = 0
      end
      # print("remotes_map", remotes_map)

      # scan all endpoints
      for pi: self.plugins
        var http_remote = introspect.get(pi, "http_remote")
        if http_remote !=  nil
          remotes_map[http_remote] = remotes_map.find(http_remote, 0) + 1
        end
      end

      # print("remotes_map2", remotes_map)

      # tasmota.log("MTR: remotes references: " + str(remotes_map), 3)

      var remote_to_remove = []           # we first get the list of remotes to remove, to not interfere with map iterator
      for remote:remotes_map.keys()
        if remotes_map[remote] == 0
          remote_to_remove.push(remote)
        end
      end

      for remote: remote_to_remove
        tasmota.log("MTR: remove unused remote: " + remote.addr, 3)
        remote.close()
        self.http_remotes.remove(remote.addr)
      end

    end

  end

  # def get_remotes_list()
  #####################################################################
  # Get sorted list of remote endpoints
  # def get_remotes_list()
  #   var ret = []
  #   for hr: self.http_remotes
  #     ret.push(hr.addr)
  #   end
  #   return self.sort_distinct(ret)
  # end

  #####################################################################
  # Commands `Mtr___`
  #####################################################################
  #
  def register_commands()
    tasmota.add_cmd("MtrJoin", /cmd_found, idx, payload, payload_json -> self.MtrJoin(cmd_found, idx, payload, payload_json))
    tasmota.add_cmd("MtrUpdate", /cmd_found, idx, payload, payload_json -> self.MtrUpdate(cmd_found, idx, payload, payload_json))
    tasmota.add_cmd("MtrInfo", /cmd_found, idx, payload, payload_json -> self.MtrInfo(cmd_found, idx, payload, payload_json))
  end

  #####################################################################
  # `MtrJoin`
  #
  # Open or close commissioning
  #
  def MtrJoin(cmd_found, idx, payload, payload_json)
    var payload_int = int(payload)
    if payload_int
      self.start_root_basic_commissioning()
    else
      self.stop_basic_commissioning()
    end
    tasmota.resp_cmnd_done()
  end

  #####################################################################
  # `MtrUpdate`
  #
  # MtrUpdate {"ep":1, "Power":1}
  # MtrUpdate {"name":"ep1", "power":1}
  # MtrUpdate {"Name":"Light0", "Power":0}
  # MtrUpdate {"Name":"Light0", "Power":1}
  # MtrUpdate {"Name":"Light1", "Power":0}
  # MtrUpdate {"Name":"Light1", "Power":1,"Bri":55}
  # MtrUpdate {"Name":"Light2", "Power":0}
  # MtrUpdate {"Name":"Light2", "Power":1, "CT":400, "Bri":20}
  # MtrUpdate {"Name":"Light3", "Power":0}
  # MtrUpdate {"Name":"Light3", "Power":1, "Bri":20, "Hue":85, "Sat":200}
  #
  def MtrUpdate(cmd_found, idx, payload, payload_json)
    if payload_json == nil    return tasmota.resp_cmnd_str("Invalid JSON")    end

    var key_ep = tasmota.find_key_i(payload_json, 'Ep')
    var key_name = tasmota.find_key_i(payload_json, 'Name')
    if key_ep || key_name
      var pl = nil                  # plugin instance

      if key_ep
        var ep = int(payload_json[key_ep])
        if ep <= 0  return tasmota.resp_cmnd_str("Invalid 'Ep' attribute")         end
        pl = self.find_plugin_by_endpoint(ep)
        payload_json.remove(key_ep)
      end

      if key_name
        if pl == nil
          pl = self.find_plugin_by_friendly_name(payload_json[key_name])
        end
        payload_json.remove(key_name)
      end

      if (pl == nil)          return tasmota.resp_cmnd_str("Invalid Device")          end
      if (!pl.VIRTUAL)        return tasmota.resp_cmnd_str("Device is not virtual")   end
      # filter parameter accedpted by plugin, and rename with canonical
      # Ex: {"power":1,"HUE":2} becomes {"Power":1,"Hue":2}
      var uc = pl.consolidate_update_commands()
      # check that all commands are in the list of supported commands
      var cmd_cleaned = {}
      for k: payload_json.keys()
        var cleaned_command_idx = tasmota.find_list_i(uc, k)
        if (cleaned_command_idx == nil)
          tasmota.resp_cmnd_str(f"Invalid attribute '{k}'")
          return
        end
        cmd_cleaned[uc[cleaned_command_idx]] = payload_json[k]
      end
      # call plug-in
      pl.update_virtual(cmd_cleaned)
      var state_json = pl.state_json()
      if state_json
        var cmnd_status = f'{{"{cmd_found}":{state_json}}}'
        return tasmota.resp_cmnd(cmnd_status)
      else
        return tasmota.resp_cmnd_done()
      end
    end

    tasmota.resp_cmnd_str("Missing 'Device' attribute")
  end

  #####################################################################
  # `MtrInfo`
  #
  # MtrInfo 9
  def MtrInfo(cmd_found, idx, payload, payload_json)
    if payload == ""
      # dump all devices
    end

    if payload == ""
      # dump all
      for pl: self.plugins
        self.MtrInfo_one(pl.endpoint)
      end

    elif  type(payload_json) == 'int'
      # try ep number
      self.MtrInfo_one(payload_json)

    else
      # try by name
      var pl = self.find_plugin_by_friendly_name(payload)
      if pl != nil
        self.MtrInfo_one(pl.endpoint)
      end
    end

    tasmota.resp_cmnd_done()
  end

  # output for a single endpoint
  def MtrInfo_one(ep)
    var pl = self.find_plugin_by_endpoint(ep)
    if pl == nil    return    end     # abort

    var state_json = pl.state_json()
    if state_json
      var mtr_info = f'{{"' 'MtrInfo"' ':{state_json}}}'
      # publish
      # tasmota.publish_rule(mtr_info)
      tasmota.publish_result(mtr_info, "")
    end
  end

end
matter.Device = Matter_Device

#-
import global
global.matter_device = matter_device()
return matter_device
-#
