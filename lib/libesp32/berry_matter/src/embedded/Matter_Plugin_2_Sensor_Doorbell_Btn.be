#
# Matter_Plugin_Sensor_Doorbell_Btn.be - Doorbell (0x0148) mapped to a Tasmota Button
#
# Copyright (C) 2026
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
# Matter 1.6.1 Device Specification - Doorbell (0x0148)
#################################################################################
# Device Type: Doorbell (0x0148)
# Device Type Revision: 2 (Matter 1.6.1 Device Library, Doorbell.xml)
# Class: Simple | Scope: Endpoint
#
# CLUSTERS (Server):
# - 0x0003: Identify (M) - inherited from Matter_Plugin_Device
# - 0x003B: Switch (M) with feature MS (Momentary Switch) mandatory
#
# CLUSTERS (Client):
# - 0x0556: Chime (M) - NOT IMPLEMENTED in this plug-in (see NOTES)
#
# NOTES:
# - Phase 1 (this file): Switch cluster only. Button presses emit
#   InitialPress and ShortRelease events. No multi-press.
# - FeatureMap is 0x02 (MS only). MultiPressMax (0x0002) is not in the
#   Switch cluster attribute list, and the MultiPress events are not exposed.
# - The class derives from Matter_Plugin_Device, not from the generic switch
#   plug-in: consolidate_clusters() merges the parent's cluster map, so
#   inheriting the generic switch would keep MultiPressMax in the list.
# - The Chime client (0x0556) requires outbound Invoke support, which the
#   Berry Matter stack does not have yet. It is therefore not declared here.
#################################################################################

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Sensor_Doorbell_Btn,weak

class Matter_Plugin_Sensor_Doorbell_Btn : Matter_Plugin_Device
  static var TYPE = "doorbell_btn"                  # name of the plug-in in json
  static var DISPLAY_NAME = "Doorbell/Button"       # display name of the plug-in

  static var SCHEMA = "button|"                     # arg name
                      "l:Button|"                    # label (display name)
                      "t:i|"                        # type: int
                      "h:Button<x> number|"         # hint
                      "r:1"                         # required
  # Identify (0x0003) is inherited from Matter_Plugin_Device.
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    0x003B: [0, 1],                                 # Switch 1.12 - no MultiPressMax
  })
  static var TYPES = { 0x0148: 2 }                  # Doorbell - Matter 1.6.1 Device Library Rev 2

  var tasmota_switch_index                          # Switch number in Tasmota (one based)
  var shadow_position

  #############################################################
  # parse_configuration
  #
  # Parse configuration map
  def parse_configuration(config)
    super(self).parse_configuration(config)
    self.tasmota_switch_index = int(config.find('button', 1))
    if self.tasmota_switch_index <= 0    self.tasmota_switch_index = 1    end
    self.shadow_position = 0             # CurrentPosition is not nullable
  end

  #############################################################
  # Model
  #
  def set_position(position)
    if position != self.shadow_position
      self.shadow_position = position
      self.attribute_updated(0x003B, 0x0001)
    end
  end

  #############################################################
  # read_attribute
  #
  # Switch cluster is handled here. Other clusters are delegated
  # to the parent class.
  def read_attribute(session, ctx, tlv_solo)
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    if cluster == 0x003B
      if   attribute == 0x0000          # NumberOfPositions
        return tlv_solo.set(0x06 #-TLV.U4-#, 2)
      elif attribute == 0x0001          # CurrentPosition
        return tlv_solo.set(0x06 #-TLV.U4-#, self.shadow_position)

      elif attribute == 0xFFFA          # EventList: InitialPress, ShortRelease only
        var events = matter.TLV.Matter_TLV_array()
        for event_id : [0x01, 0x03]
          events.add_TLV(nil, 0x06 #-TLV.U4-#, event_id)
        end
        return events
      elif attribute == 0xFFFC          # FeatureMap: MS only
        return tlv_solo.set(0x06 #-TLV.U4-#, 0x02)
      end
    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  #############################################################
  # append_state_json
  #
  # Output the current state in JSON
  def append_state_json()
    return f',"Switch":{self.shadow_position}'
  end

  #####################################################################
  # button_handler
  #
  # Args:
  #   - button: (int) button number (base 1)
  #   - mode: (int) 0=static report every second, 1=button state changed (immediate), 2=multi-press status (ignored)
  #   - state: 1=button pressed, 0=button released
  def button_handler(button, mode, state, press_counter)
    if button != self.tasmota_switch_index return end
    # Update position on regular and state-change reports
    if (mode == 0) || (mode == 1)
      self.set_position(state)
    end
    # Multi-press (mode 2) is not declared for this device type
    if mode == 1
      if state              # InitialPress
        self.publish_event(0x003B, 0x01, 1 #-matter.EVENT_INFO-#, matter.TLV.Matter_TLV_item().set(0x06 #-matter.TLV.U4-#, 1))
      else                  # ShortRelease
        self.publish_event(0x003B, 0x03, 1 #-matter.EVENT_INFO-#, matter.TLV.Matter_TLV_item().set(0x06 #-matter.TLV.U4-#, 1))
      end
    end
  end

end
matter.Plugin_Sensor_Doorbell_Btn = Matter_Plugin_Sensor_Doorbell_Btn
