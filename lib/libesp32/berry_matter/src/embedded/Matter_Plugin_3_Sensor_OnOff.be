#
# Matter_Plugin_3_Sensor_OnOff.be - implements the behavior for a OnOff Sensor
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
# Matter 1.4.1 Device Specification - On/Off Sensor (0x0850)
#################################################################################
# Device Type: On/Off Sensor (0x0850)
# Device Type Revision: 3 (Matter 1.4.1 Device Library)
# Class: Simple | Scope: Endpoint
#
# CLUSTERS (Server):
# - 0x0003: Identify (M) - Device identification
# - 0x001D: Descriptor (M) - Inherited from base class
#
# CLUSTERS (Client):
# - 0x0003: Identify (M) - Can identify other devices
# - 0x0004: Groups (O) - Group management
# - 0x0062: Scenes Management (P,O) - Scene control
# - 0x0006: On/Off (M) - Controls On/Off devices
# - 0x0008: Level Control (O) - Controls dimmable devices
# - 0x0300: Color Control (O) - Controls color devices
#
# NOTES:
# - This is a CLIENT device that controls other devices
# - Reads switch state and sends commands to bound devices
# - Can control lights, plugs, and other On/Off devices
# - Supports optional dimming and color control
# - Inherits from Matter_Plugin_Sensor_Boolean base class
#################################################################################

#################################################################################
# Matter 1.4.1 On/Off Cluster (0x0006) - On/Off Sensor Usage
#################################################################################
# Cluster Revision: 6 (Matter 1.4.1)
# Role: Application | Scope: Endpoint
#
# NOTE: On/Off Sensor uses On/Off cluster as CLIENT to control other devices
# This implementation also exposes the switch state as a server attribute
#
# ATTRIBUTES (Server - for state reporting):
# ID     | Name   | Type | Constraint | Quality | Default | Access | Conf
# -------|--------|------|------------|---------|---------|--------|-----
# 0x0000 | OnOff  | bool | all        | S,N     | FALSE   | R V    | M
#
# Quality Flags:
# - S: Scene-capable (can be stored in scenes)
# - N: Non-volatile (persists across reboots)
#
# Access Control:
# - R: Read
# - V: View privilege required
#
# COMMANDS (Client - sent to controlled devices):
# ID   | Name   | Dir  | Response | Access | Conf
# -----|--------|------|----------|--------|-----
# 0x00 | Off    | C→S  | Y        | O      | M
# 0x01 | On     | C→S  | Y        | O      | M
# 0x02 | Toggle | C→S  | Y        | O      | M
#
# TASMOTA IMPLEMENTATION:
# - Reads from Tasmota Switch<x> input (Status 10)
# - Switch ON = OnOff true
# - Switch OFF = OnOff false
# - Update interval: 750ms for responsive control
# - Can send On/Off commands to bound Matter devices
#
# TYPICAL APPLICATIONS:
# - Wall switches controlling smart lights
# - Remote controls for plugs and appliances
# - Wireless switches for home automation
# - Button panels for scene control
# - Occupancy-based lighting control
#
# BINDING MECHANISM:
# - Uses Matter binding to associate with target devices
# - Switch state changes trigger commands to bound devices
# - Supports group control via Groups cluster
# - Can control multiple devices simultaneously
#
# CONFIGURATION:
# - ARG: "switch" - Tasmota Switch number (1-based)
# - Example: switch=1 uses Switch1 input
# - Binding configured via Matter controller
#################################################################################

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Sensor_OnOff,weak

class Matter_Plugin_Sensor_OnOff : Matter_Plugin_Sensor_Boolean
  static var TYPE = "onoff"                         # name of the plug-in in json
  static var DISPLAY_NAME = "OnOff Sensor"                  # display name of the plug-in
  # static var ARG  = "switch"                        # additional argument name (or empty if none)
  # static var ARG_HINT = "Switch<x> number"
  # static var UPDATE_TIME = 750                      # update every 750ms
  static var JSON_NAME = "OnOff"                    # Name of the sensor attribute in JSON payloads
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "OnOff")
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    0x0006: [0],                                    # On/Off 1.5 p.48
  })
  static var TYPES = { 0x0850: 3 }                  # OnOff Sensor - Matter 1.4.1 Device Library Rev 3

  # var tasmota_switch_index                          # Switch number in Tasmota (one based)
  # var shadow_bool_value

  #############################################################
  # value_updated
  #
  # This is triggered when a new value is changed, for subscription
  # This method is meant to be overloaded and maximize shared code
  def value_updated()
    self.attribute_updated(0x0006, 0x0000)
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0006              # ========== On/Off 1.5 p.48 ==========
      self.update_shadow_lazy()
      if   attribute == 0x0000          #  ---------- OnOff / bool ----------
        return tlv_solo.set(TLV.BOOL, self.shadow_bool_value)
      end

    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  #############################################################
  # append_state_json
  #
  # Output the current state in JSON
  # New values need to be appended with `,"key":value` (including prefix comma)
  #
  # Override the default behavior to use the key `OnOff` instead of `Power`
  def append_state_json()
    return f',"OnOff":{int(self.shadow_bool_value)}'
  end

end
matter.Plugin_Sensor_OnOff = Matter_Plugin_Sensor_OnOff
