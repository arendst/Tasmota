#
# Matter_zz_Device.be - implements a generic Matter device (commissionee)
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
  static var VENDOR_ID = 0xFFF1
  static var PRODUCT_ID = 0x8000
  static var FILENAME = "_matter_device.json"
  static var EP = 2                   # first available endpoint number for devices
  var started                         # is the Matter Device started (configured, mDNS and UDPServer started) - 'nil' means that we wait for Wifi to connect, 'false' means that the start is scheduled but not yet triggered
  var plugins                         # list of plugins instances
  var plugins_persist                 # true if plugins configuration needs to be saved
  static var plugins_classes = matter.plugins_classes                # map of registered classes by type name
  var plugins_config                  # map of JSON configuration for plugins
  var plugins_config_remotes          # map of information on each remote under "remotes" key, '{}' when empty
  var udp_server                      # `matter.UDPServer()` object
  var profiler
  var message_handler                 # `matter.MessageHandler()` object
  var commissioning                   # `matter.Commissioning()` object
  var autoconf                        # `matter.Autoconf()` objects
  var sessions                        # `matter.Session_Store()` objet
  var zigbee                          # `Mattter_Zigbee()` object, only set if compiled with zigbee, `nil` otherwise
  var ui
  var tick                            # increment at each tick, avoids to repeat too frequently some actions
  # Events
  var events                          # Event handler
  # for brige mode, list of HTTP_remote objects (only one instance per remote object)
  var http_remotes                    # map of 'domain:port' to `Matter_HTTP_remote` instance or `nil` if no bridges
  # saved in parameters
  var root_discriminator              # as `int`
  var root_passcode                   # as `int`
  var ipv4only                        # advertize only IPv4 addresses (no IPv6)
  var disable_bridge_mode             # default is bridge mode, this flag disables this mode for some non-compliant controllers
  var next_ep                         # next endpoint to be allocated for bridge, start at 1
  var debug                           # debug mode, output all values when responding to read request with wildcard
  # cron equivalent to call `read_sensors()` regularly and dispatch to all entpoints
  var probe_sensor_time               # number of milliseconds to wait between each `read_sensors()` or `nil` if none active
  var probe_sensor_timestamp          # timestamp for `read_sensors()` probe (in millis())
                                      # if timestamp is `0`, this should be scheduled in priority


  #############################################################
  def init()
    import crypto
    if !tasmota.get_option(matter.MATTER_OPTION)
      self.ui = matter.UI(self, false)   # minimal UI
      return
    end    # abort if SetOption 151 is not set

    matter.profiler = matter.Profiler()
    self.tick = 0
    self.plugins = []
    self.plugins_persist = false                  # plugins need to saved only when the first fabric is associated
    self.plugins_config_remotes = {}
    self.next_ep = self.EP                        # start at endpoint 2 for dynamically allocated endpoints (1 reserved for aggregator)
    self.ipv4only = false
    self.disable_bridge_mode = false
    self.commissioning = matter.Commissioning(self)
    self.load_param()

    self.sessions = matter.Session_Store(self)
    self.sessions.load_fabrics()
    self.message_handler = matter.MessageHandler(self)
    self.events = matter.EventHandler(self)
    self.zigbee = self.init_zigbee()
    self.ui = matter.UI(self, true)

    tasmota.when_network_up(def () self.start() end)    # start when network is connected
    self.commissioning.init_basic_commissioning()
    tasmota.add_driver(self)

    self.register_commands()
  end

  #############################################################
  # Start Matter device server when the first network is coming up
  def start()
    # autoconfigure other plugins if needed
    self.autoconf_device()

    self._start_udp(self.UDP_PORT)

    self.commissioning.start_mdns_announce_hostnames()
  end

  #####################################################################
  # Remove a fabric and clean all corresponding values and mDNS entries
  def remove_fabric(fabric)
    if fabric != nil
      log("MTR: removing fabric " + fabric.get_fabric_id().copy().reverse().tohex(), 2)
      self.message_handler.im.subs_shop.remove_by_fabric(fabric)
      self.commissioning.mdns_remove_op_discovery(fabric)
      self.sessions.remove_fabric(fabric)
    end
    # var sub_fabrics = self.sessions.find_children_fabrics(fabric_parent.get_fabric_index())
    # if sub_fabrics == nil return end
    # for fabric_index : sub_fabrics
    #   var fabric = self.sessions.find_fabric_by_index(fabric_index)
    #   if fabric != nil
    #     log("MTR: removing fabric " + fabric.get_fabric_id().copy().reverse().tohex(), 2)
    #     self.message_handler.im.subs_shop.remove_by_fabric(fabric)
    #     self.mdns_remove_op_discovery(fabric)
    #     self.sessions.remove_fabric(fabric)
    #   end
    # end
    self.sessions.save_fabrics()
  end

  #####################################################################
  # Driver handling of buttons
  #####################################################################
  # Attach driver `button_pressed`
  def button_pressed(cmd, idx)
  	var state = (idx >> 16) & 0xFF
  	var last_state = (idx >> 8) & 0xFF
  	var index = (idx & 0xFF)
    var press_counter = (idx >> 24) & 0xFF
    self.button_handler(index + 1, (state != last_state) ? 1 : 0, state ? 0 : 1, press_counter)  # invert state, originally '0' means press, turn it into '1'
  end
  # Attach driver `button_multi_pressed`
  def button_multi_pressed(cmd, idx)
    var press_counter = (idx >> 8) & 0xFF
    var index = (idx & 0xFF)
    self.button_handler(index + 1, 2, 0, press_counter)
  end
  #####################################################################
  # Centralize to a single call
  #
  # Args:
  #   - button: (int) button number (base 1)
  #   - mode: (int) 0=static report every second, 1=button state changed (immediate), 2=multi-press status (delayed)
  #   - state: 1=button pressed, 0=button released, 2..5+=multi-press complete
  def button_handler(button, mode, state, press_counter)
    # log(f"MTR: button_handler({button=}, {mode=}, {state=})", 3)
    # call all plugins, use a manual loop to avoid creating a new object
    var idx = 0
    import introspect
    while idx < size(self.plugins)
      var pi = self.plugins[idx]
      if introspect.contains(pi, "button_handler")
        self.plugins[idx].button_handler(button, mode, state, press_counter)
      end
      idx += 1
    end
  end

  #############################################################
  # dispatch every second click to sub-objects that need it
  def every_second()
    self.sessions.every_second()
    self.message_handler.every_second()
    self.events.every_second()      # periodically remove bytes() representation of events
    self.commissioning.every_second()
  end

  #############################################################
  # dispatch every 250ms to all plugins
  def every_250ms()
    # call read_sensors if needed
    self.read_sensors_scheduler()
    # call all plugins, use a manual loop to avoid creating a new object
    var idx = 0
    while idx < size(self.plugins)
      self.plugins[idx].every_250ms()
      idx += 1
    end
  end

  #############################################################
  # add a scheduler for `read_sensors` and update schedule time
  # if it's more often than previously
  def add_read_sensors_schedule(update_time)
    if (self.probe_sensor_time == nil) || (self.probe_sensor_time > update_time)
      self.probe_sensor_time = update_time
      self.probe_sensor_timestamp = matter.jitter(update_time)
    end
  end

  #############################################################
  # check if we need to call `read_sensors()`
  def read_sensors_scheduler()
    if (self.probe_sensor_time == nil)    return  end       # nothing to schedule
    if (self.probe_sensor_timestamp == 0) || (tasmota.time_reached(self.probe_sensor_timestamp))
      self._trigger_read_sensors()
      # set new next timestamp
      self.probe_sensor_timestamp = tasmota.millis(self.probe_sensor_time)
    end
  end

  #############################################################
  # trigger a read_sensors and dispatch to plugins
  # Internally used by cron
  def _trigger_read_sensors()
    import json
    var rs_json = tasmota.read_sensors()
    if tasmota.loglevel(3)
      log("MTR: read_sensors: "+str(rs_json), 3)
    end
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
      log("MTR: unable to parse read_sensors: "+str(rs_json), 3)
    end

  end

  #############################################################
  # dispatch every 50ms
  # ticks
  def every_50ms()
    self.tick += 1
    self.message_handler.every_50ms()
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
    log("MTR: Starting UDP server on port: " + str(port), 2)
    self.udp_server = matter.UDPServer(self, "", port)
    self.udp_server.start(/ raw, addr, port -> self.msg_received(raw, addr, port))
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
  # calls `cb(pi, ctx, direct)` for each attribute expanded.
  # `pi`: plugin instance targeted by the attribute (via endpoint). Note: nothing is sent if the attribute is not declared in supported attributes in plugin.
  # `ctx`: context object with `endpoint`, `cluster`, `attribute` (no `command`)
  # `direct`: `true` if the attribute is directly targeted, `false` if listed as part of a wildcard
  # returns: `true` if processed succesfully, `false` if error occured. If `direct`, the error is returned to caller, but if expanded the error is silently ignored and the attribute skipped.
  # In case of `direct` but the endpoint/cluster/attribute is not suppported, it calls `cb(nil, ctx, true)` so you have a chance to encode the exact error (UNSUPPORTED_ENDPOINT/UNSUPPORTED_CLUSTER/UNSUPPORTED_ATTRIBUTE/UNREPORTABLE_ATTRIBUTE)
  def process_attribute_expansion(ctx, cb)
    var endpoint = ctx.endpoint
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # build the generator for all endpoint/cluster/attributes candidates
    var path_generator = matter.PathGenerator(self)
    path_generator.start(endpoint, cluster, attribute)

    var direct = path_generator.is_direct()
    var concrete_path
    while ((concrete_path := path_generator.next_attribute()) != nil)
      var finished = cb(path_generator.get_pi(), concrete_path)   # call the callback with the plugin and the context
    end
  end

  #############################################################
  # Optimized version for a single endpoint/cluster/attribute
  #
  # Retrieve the plugin for a read, or nil if not found
  # In case of error, ctx.status is updated accordingly
  def resolve_attribute_read_solo(ctx)
    var endpoint = ctx.endpoint
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # all 3 elements must be non-nil
    if (endpoint == nil) || (cluster == nil) || (attribute == nil)      return nil    end

    # look for plugin
    var pi = self.find_plugin_by_endpoint(endpoint)
    if (pi == nil)
      ctx.status = matter.UNSUPPORTED_ENDPOINT
      return nil
    else
      if   !pi.contains_cluster(cluster)
        ctx.status = matter.UNSUPPORTED_CLUSTER
        return nil
      elif !pi.contains_attribute(cluster, attribute)
        ctx.status = matter.UNSUPPORTED_ATTRIBUTE
        return nil
      end
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
    if self.debug
      j += ',"debug":true'
    end
    if self.plugins_persist
      j += ',\n"config":'
      j += json.dump(self.plugins_config)
      if size(self.plugins_config_remotes) > 0
        j += ',\n"remotes":'
        j += json.dump(self.plugins_config_remotes)
      end
    end
    j += '}'
    try
      var f = open(self.FILENAME, "w")
      f.write(j)
      f.close()
      log(format("MTR: =Saved     parameters%s", self.plugins_persist ? " and configuration" : ""), 2)
      return j
    except .. as e, m
      log("MTR: Session_Store::save Exception:" + str(e) + "|" + str(m), 2)
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
  # Reset configuration like a fresh new device
  def reset_param()
    self.plugins_persist = false
    self.next_ep = self.EP
    self.save_param()
  end

  #############################################################
  # Load Matter Device parameters
  def load_param()
    import crypto
    var dirty = false
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
      self.plugins_config = j.find("config", {})
      self.debug = bool(j.find("debug"))    # bool converts nil to false
      if self.plugins_config != nil
        log(f"MTR: Load_config = {self.plugins_config}", 3)
        self.adjust_next_ep()
        dirty = self.check_config_ep()
        self.plugins_persist = true
      end
      self.plugins_config_remotes = j.find("remotes", {})
      if self.plugins_config_remotes
        log("MTR: load_remotes = " + str(self.plugins_config_remotes), 3)
      end
    except .. as e, m
      if e != "io_error"
        log("MTR: load_param Exception:" + str(e) + "|" + str(m), 2)
      end
    end

    if self.root_discriminator == nil
      self.root_discriminator = crypto.random(2).get(0,2) & 0xFFF
      dirty = true
    end
    if self.root_passcode == nil
      self.root_passcode = self.commissioning.generate_random_passcode()
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
  # Try to clean MDNS entries before restart.
  #
  # Called by Tasmota loop as a Tasmota driver.
  def save_before_restart()
    self.commissioning.stop_basic_commissioning()
    self.commissioning.mdns_remove_op_discovery_all_fabrics()
  end

  #############################################################
  # Autoconfigure device from template
  #
  # Applies only if there are no plugins already configured
  ## TODO generate map instead
  def autoconf_device()
    import json

    if size(self.plugins) > 0   return end                    # already configured
    if (self.autoconf == nil)   self.autoconf = matter.Autoconf(self)   end

    if !self.plugins_persist
      self.plugins_config = self.autoconf.autoconf_device_map()
      self.plugins_config_remotes = {}
      self.adjust_next_ep()
      log("MTR: autoconfig = " + str(self.plugins_config), 3)
    end
    self.autoconf.instantiate_plugins_from_config(self.plugins_config)

    if !self.plugins_persist && self.sessions.count_active_fabrics() > 0
      self.plugins_persist = true
      self.save_param()
    end
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
    if pi_class == nil        log("MTR: unknown class name '"+str(pi_class_name)+"' skipping", 3)  return  end

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
    log(format("MTR: adding endpoint = %i type:%s%s", ep, pi_class_name, self.conf_to_log(plugin_conf)), 2)
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
      log("MTR: Cannot remove an enpoint not configured: " + ep_str, 3)
      return
    end
    log(format("MTR: deleting endpoint = %i", ep), 2)
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
    self.attribute_updated(matter.AGGREGATOR_ENDPOINT, 0x001D, 0x0003, false)
  end

  #############################################################
  # Check that all ep are valid, i.e. don't collied with root or aggregator
  #
  # return `true` if configuration was adjusted and needs to be saved
  def check_config_ep()
    # copy into list so we can change the map on the fly
    var dirty = false
    var keys = []
    for k: self.plugins_config.keys()   keys.push(int(k))    end
    for ep: keys
      if ep == matter.AGGREGATOR_ENDPOINT
        dirty = true
        log(f"MTR: endpoint {ep} collides wit aggregator, relocating to {self.next_ep}", 2)
        self.plugins_config[str(self.next_ep)] = self.plugins_config[str(ep)]
        self.plugins_config.remove(str(ep))
        self.next_ep += 1
      end
    end
    return dirty
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

      # log("MTR: remotes references: " + str(remotes_map), 3)

      var remote_to_remove = []           # we first get the list of remotes to remove, to not interfere with map iterator
      for remote:remotes_map.keys()
        if remotes_map[remote] == 0
          remote_to_remove.push(remote)
        end
      end

      for remote: remote_to_remove
        log("MTR: remove unused remote: " + remote.addr, 3)
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
      self.commissioning.start_root_basic_commissioning()
    else
      self.commissioning.stop_basic_commissioning()
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

  #####################################################################
  # Zigbee support
  #
  # Returns true if zigbee module is present
  #####################################################################
  def is_zigbee_present()
    import introspect
    return (introspect.module('matter_zigbee') != nil)
  end
  #
  def init_zigbee()
    if self.is_zigbee_present()
      import matter_zigbee
      return matter_zigbee(self)
    end
  end
  #
  def create_zb_mapper(pi)
    if self.zigbee
      return self.zigbee.Matter_Zigbee_Mapper(pi)
    end
  end


end
matter.Device = Matter_Device

#-
import global
global.matter_device = matter_device()
return matter_device
-#
