#
# Matter_MQTT_remote.be - implements an interface to query remotely Tasmota device via MQTT
#
# Copyright (C) 2024  Stephan Hadinger, Christian Baars & Theo Arends
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

#@ solidify:Matter_MQTT_remote,weak

#############################################################
# This class implements the MQTT transport for remote Tasmota devices.
#
# Subscriptions (4 total, per-device):
#   - tele/<topic>/STATE             - full state (TelePeriod, default 300s)
#   - tele/<topic>/SENSOR            - sensor data
#   - tele/<topic>/LWT               - online/offline status
#   - stat/<topic>/RESULT            - immediate command feedback
#
# Publishes:
#   - cmnd/<topic>/<Command>         - send commands to remote device
#
# Design principles:
#   - Subscribe-only, no polling
#   - Discovery handled globally by Matter_Device (not per-remote)
#   - Cached state for optimistic updates
#   - LWT handling for reachability

class Matter_MQTT_remote
  var device                                        # reference to matter_device
  var topic                                         # MQTT topic of the remote device
  var reachable                                     # is the device reachable
  var reachable_utc                                 # last tick when reachability was seen
  var info                                          # map with name, version, mac, hardware
  var cached_state                                  # cached state from RESULT/STATE
  var subscribed                                    # list of subscribed topics for cleanup
  var cmnd_base                                     # expanded MQTT command topic base
  var stat_base                                     # expanded MQTT status topic base
  var tele_base                                     # expanded MQTT telemetry topic base
  var confirmed                                     # true after a live command response in the current epoch
  var refresh_pending                               # true while a post-connect refresh is scheduled
  var refresh_phase                                 # 0=idle, 1=StatusRetain probe, 2=Status10/11
  var refresh_status                                # bit 0=Status10, bit 1=Status11 received live
  var active                                        # false once close() has been called

  #############################################################
  # init
  def init(device, topic, info)
    self.device = device
    self.topic = topic
    self.reachable = false
    self.reachable_utc = nil
    self.info = info ? info : {}
    self.cached_state = {}
    self.subscribed = []
    self.confirmed = false
    self.refresh_pending = false
    self.refresh_phase = 0
    self.refresh_status = 0
    self.active = true

    self.configure_topics()
    self.subscribe_topics()

    import mqtt
    if mqtt.connected()
      self.schedule_refresh()
    end
  end

  #############################################################
  # topic_base
  #
  # Expand a discovery FullTopic for command (0), stat (1) or tele (2).
  def topic_base(prefix_index)
    import string
    var fulltopic = self.info.find("_ft", "%prefix%/%topic%/")
    var prefixes = self.info.find("_tp", ["cmnd", "stat", "tele"])
    var prefix = prefixes[prefix_index]

    # Tasmota appends %prefix% for commands if FullTopic does not contain it.
    if prefix_index == 0 && string.find(fulltopic, "%prefix%") < 0
      if fulltopic[-1] != '/'   fulltopic += '/'   end
      fulltopic += "%prefix%"
    end

    fulltopic = string.replace(fulltopic, "%prefix%", prefix)
    fulltopic = string.replace(fulltopic, "%topic%", self.topic)
    fulltopic = string.replace(fulltopic, "%hostname%", self.info.find("_hn", ""))
    fulltopic = string.replace(fulltopic, "%id%", self.info.find("_id", ""))
    fulltopic = string.replace(fulltopic, "#", "")
    while string.find(fulltopic, "//") >= 0
      fulltopic = string.replace(fulltopic, "//", "/")
    end
    if fulltopic[-1] != '/'   fulltopic += '/'   end
    return fulltopic
  end

  def configure_topics()
    self.cmnd_base = self.topic_base(0)
    self.stat_base = self.topic_base(1)
    self.tele_base = self.topic_base(2)
  end

  #############################################################
  # subscribe_topics
  #
  # Subscribe to all relevant MQTT topics
  def subscribe_topics()
    # tele topics
    self.subscribe(self.tele_base + "STATE", / topic, idx, data, databytes -> self.handle_mqtt_state(data))
    self.subscribe(self.tele_base + "SENSOR", / topic, idx, data, databytes -> self.handle_mqtt_sensor(data))
    self.subscribe(self.tele_base + "LWT", / topic, idx, data, databytes -> self.handle_mqtt_lwt(data))

    # RESULT with SetOption4 disabled, command-specific topics when enabled,
    # and STATUS10/STATUS11 responses used to establish a fresh epoch.
    self.subscribe(self.stat_base + "+", / topic, idx, data, databytes -> self.handle_mqtt_stat(topic, data))
  end

  #############################################################
  # subscribe helper
  #
  # Subscribe to a topic and track it for cleanup
  def subscribe(topic, closure)
    import mqtt
    mqtt.subscribe(topic, closure)
    self.subscribed.push(topic)
  end

  #############################################################
  # unsubscribe helper
  #
  # Unsubscribe from a specific topic
  def unsubscribe(topic)
    import mqtt
    mqtt.unsubscribe(topic)
    self.subscribed.remove(topic)
  end

  #############################################################
  # discovery_info
  #
  # Convert Tasmota discovery fields to the canonical remote info map
  static def discovery_info(config)
    var info = {}
    var value = config.find("dn")
    if value != nil   info['name'] = value       end
    value = config.find("sw")
    if value != nil   info['version'] = value    end
    value = config.find("mac")
    if value != nil   info['mac'] = value        end
    value = config.find("md")
    if value != nil   info['hardware'] = value   end
    value = config.find("ft")
    if value != nil   info['_ft'] = value        end
    value = config.find("tp")
    if value != nil   info['_tp'] = value        end
    value = config.find("hn")
    if value != nil   info['_hn'] = value        end
    value = config.find("mac")
    if value != nil   info['_id'] = value        end
    return info
  end

  # Update remote metadata from discovery, returning true only on change
  def set_info_from_discovery(config)
    var info = self.discovery_info(config)
    if size(self.info) == size(info)
      var same = true
      for key: info.keys()
        if self.info.find(key) != info[key]   same = false  break   end
      end
      if same   return false   end
    end

    var old_cmnd = self.cmnd_base
    var old_stat = self.stat_base
    var old_tele = self.tele_base
    self.info = info
    self.configure_topics()
    if old_cmnd != self.cmnd_base || old_stat != self.stat_base || old_tele != self.tele_base
      import mqtt
      for topic: self.subscribed
        mqtt.unsubscribe(topic)
      end
      self.subscribed = []
      self.subscribe_topics()
      self.begin_epoch(true)
    end
    return true
  end

  #############################################################
  # generate_config_from_discovery
  #
  # Auto-generate endpoint configuration from discovery config and sensors
  # Similar to Matter_UI.generate_config_from_status() but uses discovery data
  static def generate_config_from_discovery(device, config, sensors)
    var config_list = []

    # rl entries are: 0=unused, 1=relay, 2=light, 3=shutter
    var rl = config.find("rl")
    if rl != nil && size(rl) > 0
      var lt_st = config.find("lt_st", 0)
      var so = config.find("so", {})
      var pwm_multi = bool(so.find("68", 0))
      for i: 0 .. size(rl) - 1
        var relay_type = rl[i]
        if relay_type == 1
          config_list.push({'type': 'light0', 'relay': i + 1})
        elif relay_type == 2
          var light_type = 'light0'
          if pwm_multi || lt_st == 1
            light_type = 'light1'
          elif lt_st == 2
            light_type = 'light2'
          elif lt_st >= 3
            # RGB, RGBW and RGBCW all expose the Matter extended-color features
            light_type = 'light3'
          end
          config_list.push({'type': light_type, 'relay': i + 1})
        elif relay_type == 3
          # No remote shutter bridge plug-in exists; never expose its relays as switches.
          log(f"MTR: MQTT discovery skipping unsupported shutter relay {i + 1}", 3)
        end
      end
    end

    if sensors != nil
      config_list += device.autoconf.autoconf_sensors_list(sensors.find("sn", {}))
    end

    return config_list
  end

  #############################################################
  # handle_mqtt_state
  #
  # Handle tele/<topic>/STATE messages
  # This is the full state published every TelePeriod
  def handle_mqtt_state(data)
    if data == nil || !self.confirmed   return   end

    var j = data
    if type(j) == 'string'
      import json
      j = json.load(j)
    end

    if j != nil
      # mark device as alive
      self.device_is_alive(true)

      # merge into cached state
      self.merge_state(j)

      # dispatch to any registered callbacks
      self.dispatch_cb(11, j)

      log(f"MTR: MQTT STATE received from {self.topic}: {data}", 3)
    end
  end

  #############################################################
  # handle_mqtt_sensor
  #
  # Handle tele/<topic>/SENSOR messages
  def handle_mqtt_sensor(data)
    if data == nil || !self.confirmed   return   end

    var j = data
    if type(j) == 'string'
      import json
      j = json.load(j)
    end

    if j != nil
      # mark device as alive
      self.device_is_alive(true)

      # merge into cached state
      self.merge_state(j)

      # dispatch to any registered callbacks
      self.dispatch_cb(10, j)

      log(f"MTR: MQTT SENSOR received from {self.topic}: {data}", 3)
    end
  end

  #############################################################
  # handle_mqtt_lwt
  #
  # Handle tele/<topic>/LWT messages
  # "Online" or "Offline"
  def handle_mqtt_lwt(data)
    if data == nil   return   end

    if data == "Online"
      self.begin_epoch(true)
      log(f"MTR: MQTT device {self.topic} came online", 3)
    elif data == "Offline"
      self.begin_epoch(false)
      log(f"MTR: MQTT device {self.topic} went offline", 3)
    end
  end

  #############################################################
  # handle_mqtt_stat
  #
  # Handle RESULT, STATUS10/11 and SetOption4 command-specific responses.
  def handle_mqtt_stat(topic, data)
    if data == nil   return   end

    var j = data
    if type(j) == 'string'
      # SetOption4 POWER topics may also carry a raw ON/OFF payload.
      if size(j) == 0 || j[0] != '{'   return   end
      import json
      j = json.load(j)
    end

    if j == nil || !isinstance(j, map)   return   end

    if !self.confirmed
      # STATUS10/11 themselves may be retained. First require the ordinary,
      # non-retained response to our StatusRetain query before accepting them.
      if self.refresh_phase == 1
        if !j.contains("StatusRetain")   return   end
        self.refresh_phase = 2
        import mqtt
        mqtt.publish(self.cmnd_base + "Status", "10")
        mqtt.publish(self.cmnd_base + "Status", "11")
        return
      elif self.refresh_phase == 2
        if j.contains("StatusSNS")
          var payload = j["StatusSNS"]
          self.merge_state(payload)
          self.dispatch_cb(10, payload)
          self.refresh_status |= 1
        elif j.contains("StatusSTS")
          var payload = j["StatusSTS"]
          self.merge_state(payload)
          self.dispatch_cb(11, payload)
          self.refresh_status |= 2
        else
          return
        end
        if self.refresh_status == 3
          self.confirmed = true
          self.refresh_phase = 0
          self.device_is_alive(true)
        end
        return
      else
        return
      end
    end

    self.device_is_alive(true)

    if j.contains("StatusSNS")
      var payload = j["StatusSNS"]
      self.merge_state(payload)
      self.dispatch_cb(10, payload)
    elif j.contains("StatusSTS")
      var payload = j["StatusSTS"]
      self.merge_state(payload)
      self.dispatch_cb(11, payload)
    else
      # Command responses are often partial; dispatch the merged snapshot so
      # unrelated endpoints retain their last known fields within this epoch.
      self.merge_state(j)
      self.dispatch_cb(11, self.cached_state)
    end

    log(f"MTR: MQTT stat received from {self.topic}: {data}", 3)
  end

  #############################################################
  # merge_state
  #
  # Merge new state into cached_state
  def merge_state(new_state)
    for k: new_state.keys()
      self.cached_state[k] = new_state[k]
    end
  end

  #############################################################
  # get/set remote_info map
  def get_info()      return self.info                    end
  def set_info(v)     self.info = v                       end
  def info_changed()  self.device.save_param()            end

  #############################################################
  # device is alive, update reachable_utc
  def device_is_alive(alive)
    var changed = self.reachable != alive
    if alive
      self.reachable = true
      self.reachable_utc = tasmota.rtc_utc()
    else
      self.reachable = false
    end
    if changed
      self.device.mqtt_reachable_changed(self)
    end
  end

  #############################################################
  # Start a new remote freshness epoch.
  def begin_epoch(refresh)
    self.confirmed = false
    self.cached_state = {}
    self.refresh_phase = 0
    self.refresh_status = 0
    self.device_is_alive(false)
    if refresh   self.schedule_refresh()   end
  end

  def mqtt_connected()
    self.begin_epoch(true)
  end

  def mqtt_disconnected()
    self.begin_epoch(false)
  end

  def schedule_refresh()
    if !self.active || self.refresh_pending   return   end
    self.refresh_pending = true
    tasmota.set_timer(100, / -> self.refresh_state())
  end

  def refresh_state()
    self.refresh_pending = false
    if !self.active   return   end
    import mqtt
    if !mqtt.connected()   return   end
    self.refresh_phase = 1
    self.refresh_status = 0
    mqtt.publish(self.cmnd_base + "StatusRetain", "")
    log(f"MTR: MQTT freshness probe requested from {self.topic}", 3)
  end

  def can_send()
    import mqtt
    return mqtt.connected() && self.confirmed && self.reachable
  end

  #############################################################
  # call_sync
  #
  # Synchronous (non-blocking for MQTT)
  # Returns nil - actual state update comes via RESULT subscription
  def call_sync(cmd, timeout)
    import mqtt
    import string
    if !self.can_send()   return nil   end
    # publish command via MQTT
    var space_idx = string.find(cmd, " ")
    var cmnd_topic = self.cmnd_base
    var payload = ""
    if space_idx > 0
      cmnd_topic += cmd[0 .. space_idx - 1]
      payload = cmd[space_idx + 1 ..]
    else
      cmnd_topic += cmd
    end
    mqtt.publish(cmnd_topic, payload)
    log(f"MTR: MQTT command sent to {self.topic}: {cmnd_topic} payload='{payload}'", 3)

    # return nil - actual state update comes via RESULT subscription
    return nil
  end

  #############################################################
  # dispatch_cb
  #
  # Dispatch status response to registered callbacks
  var async_cb_map

  def add_async_cb(cb, cmd)
    if self.async_cb_map == nil    self.async_cb_map = {}    end
    self.async_cb_map[cb] = cmd
  end

  def dispatch_cb(status, payload)
    if self.async_cb_map == nil    return    end
    for cb: self.async_cb_map.keys()
      var cmd_filter = self.async_cb_map[cb]
      if cmd_filter == nil || cmd_filter == status
        cb(status, payload, nil)
      end
    end
  end

  #############################################################
  # web_last_seen
  #
  # Show when the device was last seen
  def web_last_seen()
    var seconds = -1                      # default if no known value
    if self.reachable_utc != nil
      seconds = tasmota.rtc_utc() - self.reachable_utc
    end
    return matter.seconds_to_dhm(seconds)
  end

  #############################################################
  # close
  #
  # Unsubscribe from all topics and clean up
  def close()
    import mqtt
    self.active = false
    for topic: self.subscribed
      mqtt.unsubscribe(topic)
      log(f"MTR: MQTT unsubscribed from {topic}", 3)
    end
    self.subscribed = []
    self.reachable = false
    self.cached_state = {}
    self.info = {}
    self.async_cb_map = nil
    log(f"MTR: MQTT remote {self.topic} closed", 3)
  end
end
matter.MQTT_remote = Matter_MQTT_remote
