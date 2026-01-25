#
# Matter_Plugin_3_Sensor_Contact.be - implements the behavior for a Contact Sensor
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
# Matter 1.4.1 Device Specification
#################################################################################
# Device Type: Contact Sensor (0x0015)
# Device Type Revision: 2 (Matter 1.4.1)
# Class: Simple | Scope: Endpoint
#
# CLUSTERS (Server):
# - 0x0003: Identify (M)
# - 0x0045: Boolean State (M)
# - 0x0080: Boolean State Configuration (O)
#
# NOTES:
# - Detects contact/open state (door, window, etc.)
# - Boolean State: true=closed/contact, false=open/no contact
# - Boolean State Configuration for alarm and sensitivity settings
#################################################################################

#################################################################################
# Matter 1.4.1 Boolean State Cluster (0x0045)
#################################################################################
# Cluster Revision: 1 (Matter 1.4.1)
# Role: Application | Scope: Endpoint
#
# PURPOSE:
# Provides a simple boolean state for sensors like contact sensors, leak
# detectors, and other binary state devices.
#
# ATTRIBUTES:
# ID     | Name       | Type | Constraint | Quality | Default | Access | Conf
# -------|------------|------|------------|---------|---------|--------|-----
# 0x0000 | StateValue | bool | all        | P       | MS      | R V    | M
# 0xFFFC | FeatureMap | map32| all        | F       | 0       | R V    | M
# 0xFFFD | ClusterRevision| uint16| all   | F       | 1       | R V    | M
#
# QUALITY FLAGS:
# - P: Periodic reporting (changes reported automatically)
#
# ATTRIBUTES DETAIL:
# - StateValue: Current boolean state
#   - For Contact Sensor: true=closed/contact, false=open/no contact
#   - For Water Leak: true=leak detected, false=no leak
#   - For Rain Sensor: true=rain detected, false=no rain
#   - For Water Freeze: true=freeze risk, false=no freeze risk
#
# EVENTS:
# ID   | Name        | Priority | Access | Conf
# -----|-------------|----------|--------|-----
# 0x00 | StateChange | INFO     | V      | O
#
# StateChange Event: {StateValue:bool}
#
# IMPLEMENTATION NOTES:
# - Simple boolean state, meaning depends on device type
# - For Contact Sensor in Tasmota: Maps to Switch state
# - StateChange event should be generated on state transitions
# - Value is stored in shadow_bool_value as boolean
# - Event is mandatory for Contact Sensor, Water Leak, Rain, and Freeze detectors
#################################################################################

#################################################################################
# Matter 1.4.1 Boolean State Configuration Cluster (0x0080)
#################################################################################
# Cluster Revision: 1 (Matter 1.4.1)
# Role: Application | Scope: Endpoint
#
# PURPOSE:
# Provides configuration for boolean state sensors including alarm settings
# and sensitivity levels.
#
# FEATURES:
# Bit | Code    | Feature           | Conf      | Summary
# ----|---------|-------------------|-----------|---------------------------
# 0   | VIS     | Visual            | O         | Visual alarms
# 1   | AUD     | Audible           | O         | Audible alarms
# 2   | SPRS    | AlarmSuppress     | [VIS\|AUD]| Suppress alarms
# 3   | SENSLVL | SensitivityLevel  | O         | Sensitivity setting
#
# ATTRIBUTES (if implemented):
# - CurrentSensitivityLevel: Current sensitivity (0 to SupportedSensitivityLevels-1)
# - SupportedSensitivityLevels: Number of sensitivity levels (2-10)
# - AlarmsActive: Which alarms are currently active
# - AlarmsSuppressed: Which alarms are suppressed
# - AlarmsEnabled: Which alarms are enabled
# - AlarmsSupported: Which alarm types are supported
# - SensorFault: Sensor fault status
#
# COMMANDS (if features implemented):
# - SuppressAlarm: Temporarily suppress alarms
# - EnableDisableAlarm: Enable or disable specific alarms
#
# NOTES:
# - Optional cluster for advanced sensor configuration
# - Typically not implemented for simple contact sensors
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Sensor_Contact,weak

class Matter_Plugin_Sensor_Contact : Matter_Plugin_Sensor_Boolean
  static var TYPE = "contact"                       # name of the plug-in in json
  static var DISPLAY_NAME = "Contact"                       # display name of the plug-in
  # static var ARG  = "switch"                        # additional argument name (or empty if none)
  # static var ARG_HINT = "Switch<x> number"
  # static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  # static var UPDATE_TIME = 750                      # update every 750ms
  static var JSON_NAME = "Contact"                  # Name of the sensor attribute in JSON payloads
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "Contact")
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    0x0045: [0],                                    # Boolean State p.70 - no writable
  })
  static var TYPES = { 0x0015: 2 }                  # Contact Sensor - Matter 1.4.1 Device Library Rev 2

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
        return tlv_solo.set_or_nil(TLV.BOOL, self.shadow_bool_value)
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
    webserver.content_send(format("Contact%i %s", self.tasmota_switch_index, self.web_value_onoff(self.shadow_bool_value)))
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
matter.Plugin_Sensor_Contact = Matter_Plugin_Sensor_Contact
