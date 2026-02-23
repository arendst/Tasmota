#
# Matter_Plugin_2_Sensor_Waterleak.be - implements the behavior for a Water leak Sensor
#
# Copyright (C) 2024  Stephan Hadinger & Theo Arends
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
# Matter 1.4.1 Device Specification - Water Leak Detector (0x0043)
#################################################################################
# Device Type: Water Leak Detector (0x0043)
# Device Type Revision: 1 (Matter 1.4.1 Device Library)
# Class: Simple | Scope: Endpoint
#
# CLUSTERS (Server):
# - 0x0045: Boolean State (M) - Water leak detection state
# - 0x0080: Boolean State Configuration (O) - Alarm configuration
# - 0x0003: Identify (M) - Device identification
# - 0x001D: Descriptor (M) - Inherited from base class
#
# BOOLEAN STATE SEMANTICS:
# - true: Leak detected
# - false: No leak
#
# ELEMENT OVERRIDES:
# - Boolean State: StateChange event is MANDATORY (not optional)
#
# NOTES:
# - Critical safety device for water damage prevention
# - Binary sensor for water/moisture detection
# - Typically uses conductivity or capacitive sensing
# - Fast response time essential for leak detection
# - Inherits from Matter_Plugin_Sensor_Boolean base class
#################################################################################

#################################################################################
# Matter 1.4.1 Boolean State Cluster (0x0045) - Water Leak Detector Usage
#################################################################################
# Cluster Revision: 1 (Matter 1.4.1)
# Role: Application | Scope: Endpoint
#
# ATTRIBUTES:
# ID     | Name       | Type | Constraint | Quality | Default | Access | Conf
# -------|------------|------|------------|---------|---------|--------|-----
# 0x0000 | StateValue | bool | all        | P       | -       | R V    | M
#
# Quality Flags:
# - P: Periodic reporting (changes reported automatically)
#
# Access Control:
# - R: Read
# - V: View privilege required
#
# EVENTS:
# ID   | Name        | Priority | Access | Conf
# -----|-------------|----------|--------|-----
# 0x00 | StateChange | INFO     | V      | M (overridden from O)
#
# StateChange Event Fields:
# - StateValue: bool - New state value
#
# STATE VALUE SEMANTICS FOR WATER LEAK DETECTOR:
# - true (1): Leak detected - water/moisture present
# - false (0): No leak - dry conditions
#
# TASMOTA IMPLEMENTATION:
# - Reads from Tasmota Switch<x> input (Status 10)
# - Switch ON = Leak detected (true)
# - Switch OFF = No leak (false)
# - Update interval: 750ms for fast leak detection
# - StateChange event sent on every state transition
#
# TYPICAL APPLICATIONS:
# - Under sinks and appliances
# - Basement and crawl space monitoring
# - Water heater leak detection
# - Washing machine overflow protection
# - Bathroom and kitchen leak prevention
# - Smart home water shutoff automation
#
# SAFETY CONSIDERATIONS:
# - StateChange event is MANDATORY for immediate notification
# - Fast update interval (750ms) for quick response
# - Critical for preventing water damage
# - Should trigger immediate alerts/actions
#
# CONFIGURATION:
# - ARG: "switch" - Tasmota Switch number (1-based)
# - Example: switch=1 uses Switch1 input
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Sensor_Waterleak,weak

class Matter_Plugin_Sensor_Waterleak : Matter_Plugin_Sensor_Boolean
  static var TYPE = "waterleak"                     # name of the plug-in in json
  static var DISPLAY_NAME = "Waterleak"             # display name of the plug-in
  # static var ARG  = "switch"                        # additional argument name (or empty if none)
  # static var ARG_HINT = "Switch<x> number"
  # static var UPDATE_TIME = 750                      # update every 750ms
  static var JSON_NAME = "Waterleak"                # Name of the sensor attribute in JSON payloads
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "Waterleak")
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    0x0045: [0],                                    # Boolean State p.70 - no writable
  })
  # MATTER_WATER_LEAK_DETECTOR_DEVICE_TYPE_ID 0x0043
  static var TYPES = { 0x0043: 1 }                  # Waterleak Sensor, rev 1

  # var tasmota_switch_index                          # Switch number in Tasmota (one based)
  # var shadow_bool_value

  #############################################################
  # value_updated
  #
  # This is triggered when a new value is changed, for subscription
  # This method is meant to be overloaded and maximize shared code
  def value_updated()
    self.attribute_updated(0x0045, 0x0000)
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0045              # ========== Boolean State ==========
      if   attribute == 0x0000          #  ---------- StateValue / bool ----------
        return tlv_solo.set(TLV.BOOL, self.shadow_bool_value)
      end

    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end
  
  #############################################################
  # For Bridge devices
  #############################################################
  #############################################################
  # web_values
  #
  # Show values of the remote device as HTML
  def web_values()
    import webserver
    self.web_values_prefix()        # display '| ' and name if present
    webserver.content_send(format("Waterleak%i %s", self.tasmota_switch_index, self.web_value_onoff(self.shadow_bool_value)))
  end

  # Show prefix before web value
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
matter.Plugin_Sensor_Waterleak = Matter_Plugin_Sensor_Waterleak
