#
# Matter_Plugin_2_Sensor_Boolean.be - implements the behavior for an abstract boolean sensor - to be inherited
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

#################################################################################
# Matter 1.4.1 Base Boolean Sensor Class
#################################################################################
# CLASS: Matter_Plugin_Sensor_Boolean (Base class for boolean/binary sensors)
# INHERITS FROM: Matter_Plugin_Device
#
# PURPOSE:
# - Provides common functionality for binary state sensors
# - Handles boolean sensor value reading from Tasmota Switch inputs
# - Manages state change detection and reporting
# - Supports both physical and virtual boolean sensors
#
# TYPICAL USE CASES:
# - Contact sensors (door/window open/closed)
# - Water leak detectors (leak detected/no leak)
# - Rain sensors (rain detected/no rain)
# - Motion sensors (occupied/unoccupied)
# - Any binary state sensor
#
# BOOLEAN SENSOR READING MECHANISM:
# - Physical sensors: Reads from Tasmota Status 10 Switch<x> state
# - Virtual sensors: Receives updates via update_virtual() method
# - Automatic periodic updates every 750ms (UPDATE_TIME)
# - State changes trigger value_updated() callback
#
# DERIVED CLASSES MUST IMPLEMENT:
# - JSON_NAME: Name of boolean attribute in JSON payloads
# - CLUSTERS: Define appropriate cluster (typically Boolean State 0x0045)
# - TYPES: Define device type ID
# - value_updated() and read_attribute(): only when not using Boolean State
#   0x0045 (e.g. Occupancy 0x0406, OnOff 0x0006)
#
# BOOLEAN STATE 0x0045 (shared by Contact, Rain, Waterleak):
# - value_updated() reports StateValue 0x0000
# - read_attribute() answers StateValue 0x0000
# - CHGEVENT: set to true to enable the ChangeEvent feature. Then
#   value_updated() publishes the StateChange event 0x00 {StateValue:bool},
#   FeatureMap 0xFFFC is 0x01 and EventList 0xFFFA is [0x00]
# - CHGEVENT is mandatory for Rain, Water Leak and Water Freeze detectors,
#   optional for Contact Sensor (Matter 1.6.1)
#
# CONFIGURATION:
# - SCHEMA: "switch", label "Switch", hint "Switch<x> number", required (1-based)
# - UPDATE_TIME: 750ms - Fast update for responsive binary sensors
#
# TASMOTA INTEGRATION:
# - Reads Switch<x> state from Status 10 JSON response
# - Switch states: "ON" (true) or "OFF" (false)
# - Supports multiple switches via switch index configuration
# - Example JSON: {"Switch1":"ON", "Switch2":"OFF"}
#
# STATE MANAGEMENT:
# - shadow_bool_value: Cached boolean state
# - State changes detected by comparing new vs cached value
# - value_updated() called only when state actually changes, after
#   shadow_bool_value holds the new value (derived classes read it for the
#   Boolean State StateChange event payload)
# - Reduces unnecessary Matter attribute updates
#
# EXAMPLES:
# - Contact Sensor: Switch1 → Boolean State cluster
#   * true = Contact (closed), false = No contact (open)
# - Water Leak: Switch2 → Boolean State cluster
#   * true = Leak detected, false = No leak
# - Rain Sensor: Switch3 → Boolean State cluster
#   * true = Rain detected, false = No rain
#
# VIRTUAL SENSOR SUPPORT:
# - Virtual sensors receive boolean updates via Matter bridge
# - No Tasmota command execution for virtual devices
# - Values updated directly via update_virtual() method
# - Supports JSON payload with boolean or integer values
#
# MATTER CLUSTER MAPPING:
# - Most boolean sensors use Boolean State cluster (0x0045)
# - Attribute 0x0000 (StateValue): bool
# - true/false mapped to sensor-specific meanings
# - Derived classes define semantic meaning of states
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Sensor_Boolean,weak

class Matter_Plugin_Sensor_Boolean : Matter_Plugin_Device
  # static var TYPE = ""                              # name of the plug-in in json
  # static var DISPLAY_NAME = ""                      # display name of the plug-in

  static var SCHEMA = "switch|"                     # arg name
                      "l:Switch|"                   # label (display name)
                      "t:i|"                        # type: int
                      "h:Switch<x> number|"         # hint
                      "r:1"                         # required
  static var UPDATE_TIME = 750                      # update every 750ms
  static var CHGEVENT = false                       # Boolean State ChangeEvent feature, see header

  var tasmota_switch_index                          # Switch number in Tasmota (one based)
  var shadow_bool_value

  #############################################################
  # Constructor
  def init(device, endpoint, config)
    super(self).init(device, endpoint, config)
    self.shadow_bool_value = false
  end

  #############################################################
  # parse_configuration
  #
  # Parse configuration map
  def parse_configuration(config)
    super(self).parse_configuration(config)
    self.tasmota_switch_index = int(config.find('switch', 1))
    if self.tasmota_switch_index <= 0    self.tasmota_switch_index = 1    end
  end

  #############################################################
  # Update shadow
  #
  def update_shadow()
    super(self).update_shadow()
    if !self.VIRTUAL && !self.mqtt_remote
      var switch_str = "Switch" + str(self.tasmota_switch_index)

      var j = tasmota.cmd("Status 10", true)
      if j != nil   j = j.find("StatusSNS") end
      if j != nil && j.contains(switch_str)
        var state = (j.find(switch_str) == "ON")

        if (self.shadow_bool_value != state)
          self.shadow_bool_value = state
          self.value_updated()                    # called after the shadow holds the new value
        end
      end
    end
  end

  #############################################################
  # value_updated
  #
  # This is triggered when a new value is changed, for subscription
  # `shadow_bool_value` already holds the new value when this is called
  # Default implementation for Boolean State 0x0045, overloaded by
  # sensors using another cluster
  def value_updated()
    self.attribute_updated(0x0045, 0x0000)
    if self.CHGEVENT                    # StateChange event {StateValue:bool}
      self.publish_event(0x0045, 0x00, 1 #-matter.EVENT_INFO-#, matter.TLV.Matter_TLV_item().set(0x08 #-TLV.BOOL-#, self.shadow_bool_value))
    end
  end

  #############################################################
  # read an attribute
  #
  # Boolean State 0x0045, only reached when the cluster is in CLUSTERS
  def read_attribute(session, ctx, tlv_solo)
    if ctx.cluster == 0x0045            # ========== Boolean State ==========
      var attribute = ctx.attribute
      if   attribute == 0x0000          #  ---------- StateValue / bool ----------
        return tlv_solo.set_or_nil(0x08 #-TLV.BOOL-#, self.shadow_bool_value)
      elif self.CHGEVENT
        if   attribute == 0xFFFA        #  ---------- EventList ----------
          var events = matter.TLV.Matter_TLV_array()
          events.add_TLV(nil, 0x06 #-TLV.U4-#, 0x00)  # StateChange
          return events
        elif attribute == 0xFFFC        #  ---------- FeatureMap / map32 ----------
          return tlv_solo.set(0x06 #-TLV.U4-#, 0x01)  # CHGEVENT (bit 0)
        end
      end
    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  #############################################################
  # update_virtual
  #
  # Update internal state for virtual devices
  def update_virtual(payload)
    var prev = self.shadow_bool_value
    self.shadow_bool_value = self._parse_update_virtual(payload, self.JSON_NAME, prev, bool, nil, nil)   # publishing cluster/attr is delegated to `value_updated()`
    if self.shadow_bool_value != prev
      self.value_updated()
    end
    super(self).update_virtual(payload)
  end

  #############################################################
  # For Bridge devices
  #############################################################
  #############################################################
  # Update shadow values from Status 10 (local copies of what we published to the Matter gateway)
  #
  # This call is synchronous and blocking.
  def parse_status(data, index)
    if index == 10                             # Status 10
      var switch_key = "Switch" + str(self.tasmota_switch_index)
      if !data.contains(switch_key)   return   end
      var state = (data.find(switch_key) == "ON")

      if self.shadow_bool_value != state
        self.shadow_bool_value = state
        self.value_updated()                      # called after the shadow holds the new value
      end
    end
  end

  #############################################################
  # web_values_prefix
  #
  # Show prefix before web value, defaults to `Switch<x>` if no name
  def web_values_prefix()
    import webserver
    var name = self.get_name()
    if !name
      name = "Switch" + str(self.tasmota_switch_index)
    end
    webserver.content_send(format(self.PREFIX, name ? webserver.html_escape(name) : ""))
  end
  #############################################################
  #############################################################
end
matter.Plugin_Sensor_Boolean = Matter_Plugin_Sensor_Boolean
