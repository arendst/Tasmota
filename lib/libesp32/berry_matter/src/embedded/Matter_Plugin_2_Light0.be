#
# Matter_Plugin_Light0.be - implements the behavior for a generic Lighting (OnOff only)
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

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Light0,weak

class Matter_Plugin_Light0 : Matter_Plugin_Device
  static var TYPE = "light0"                        # name of the plug-in in json
  static var DISPLAY_NAME = "Light 0 OnOff"         # display name of the plug-in
  static var ARG  = "relay"                         # additional argument name (or empty if none)
  static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  static var ARG_HINT = "Relay<x> number"
  static var UPDATE_TIME = 250                      # update every 250ms
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                             # Identify 1.2 p.16
    # 0x0004: inherited                             # Groups 1.3 p.21
    # 0x0005: inherited                             # Scenes 1.4 p.30 - no writable
    0x0006: [0],                                    # On/Off 1.5 p.48
  })
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "Power")
  static var TYPES = { 0x0100: 2 }                  # OnOff Light, but not actually used because Relay is managed by OnOff

  # Inherited
  # var device                                        # reference to the `device` global object
  # var endpoint                                      # current endpoint
  # var clusters                                      # map from cluster to list of attributes, typically constructed from CLUSTERS hierachy
  # var tick                                          # tick value when it was last updated
  # var node_label                                    # name of the endpoint, used only in bridge mode, "" if none
  var tasmota_relay_index                             # Relay number in Tasmota (1 based), may be nil for Lights 1/2/3 internal
  var light_index                                   # index number when using `light.get()` and `light.set()`
  var shadow_onoff                                    # (bool) status of the light power on/off

  #############################################################
  # Constructor
  def init(device, endpoint, config)
    self.shadow_onoff = false
    self.light_index = 0                              # default is 0 for light object
    super(self).init(device, endpoint, config)
  end

  #############################################################
  # parse_configuration
  #
  # Parse configuration map
  def parse_configuration(config)
    super(self).parse_configuration(config)
    # with Light0 we always need relay number but we don't for Light1/2/3 so self.tasmota_relay_index may be `nil`
    self.tasmota_relay_index = int(config.find(self.ARG #-'relay'-#, nil))
    if (self.tasmota_relay_index != nil && self.tasmota_relay_index <= 0)    self.tasmota_relay_index = 1    end
  end

  #############################################################
  # Update shadow
  #
  def update_shadow()
    if !self.VIRTUAL && !self.BRIDGE
      if (self.tasmota_relay_index != nil)
        var pow = tasmota.get_power(self.tasmota_relay_index - 1)
        if pow != nil
          if self.shadow_onoff != bool(pow)
            self.attribute_updated(0x0006, 0x0000)
          end
          self.shadow_onoff = pow
        end
      end
    end
    super(self).update_shadow()
  end

  #############################################################
  # Model
  #
  def set_onoff(pow)
    if self.BRIDGE
      var ret = self.call_remote_sync("Power" + str(self.tasmota_relay_index), pow ? "1" : "0")
      if ret != nil
        self.parse_status(ret, 11)        # update shadow from return value
        # self.tick = self.device.tick      # prevent an explicit Status11 for as it is not needed if the subscription update is sent in same tick
      end
    elif self.VIRTUAL
      if pow != self.shadow_onoff
        self.attribute_updated(0x0006, 0x0000)
        self.shadow_onoff = pow
      end
    else
      if (self.tasmota_relay_index != nil)
        tasmota.set_power(self.tasmota_relay_index - 1, bool(pow))
        self.update_shadow()
      else
        import light
        light.set({'power':pow}, self.light_index)
        self.update_shadow()
      end
    end
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
        return tlv_solo.set(TLV.BOOL, self.shadow_onoff)
      end

    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  #############################################################
  # Invoke a command
  #
  # returns a TLV object if successful, contains the response
  #   or an `int` to indicate a status
  def invoke_request(session, val, ctx)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var command = ctx.command

    # ====================================================================================================
    if   cluster == 0x0006              # ========== On/Off 1.5 p.48 ==========
      self.update_shadow_lazy()
      if   command == 0x0000            # ---------- Off ----------
        self.set_onoff(false)
        self.publish_command('Power', 0)
        return true
      elif command == 0x0001            # ---------- On ----------
        self.set_onoff(true)
        self.publish_command('Power', 1)
        return true
      elif command == 0x0002            # ---------- Toggle ----------
        self.set_onoff(!self.shadow_onoff)
        self.publish_command('Power', self.shadow_onoff ? 1 : 0)
        return true
      end
    end

  end

  #############################################################
  # update_virtual
  #
  # Update internal state for virtual devices
  def update_virtual(payload)
    var val_onoff = payload.find("Power")
    if val_onoff != nil
      self.set_onoff(bool(val_onoff))
    end
    super(self).update_virtual(payload)
  end

  #############################################################
  # For Zigbee devices
  #############################################################
  #############################################################
  # attributes_refined
  #
  # Filtered to only events for this endpoint
  # Contains common code for Light 0/1/2/3 to avoid code duplication
  #
  # Can be called only if `self.ZIGBEE` is true
  def zigbee_received(frame, attr_list)
    import math
    log(f"MTR: zigbee_received Ox{self.zigbee_mapper.shortaddr:04X} {attr_list=} {type(attr_list)=}", 3)
    var idx = 0
    var update_list = {}
    while (idx < size(attr_list))
      var entry = attr_list[idx]
      if (entry.key == "Power")
        update_list['Power'] = int(entry.val)
      end
      if (entry.key == "Dimmer")
        update_list['Dimmer'] = int(entry.val)
      end
      if (entry.key == "CT")
        update_list['CT'] = int(entry.val)
      end
      idx += 1
    end
    if (size(update_list) > 0)
      self.update_virtual(update_list)
      log(f"MTR: [{self.endpoint:02X}] Light2 updated {update_list}", 3)
    end
  end

  #############################################################
  # For Bridge devices
  #############################################################
  #############################################################
  # Stub for updating shadow values (local copies of what we published to the Matter gateway)
  #
  # This call is synnchronous and blocking.
  def parse_status(data, index)
    if index == 11                              # Status 11
      var state = false

      if self.tasmota_relay_index == 1 && data.contains("POWER")        # special case, can be `POWER` or `POWER1`
        state = (data.find("POWER") == "ON")
      else
        state = (data.find("POWER" + str(self.tasmota_relay_index)) == "ON")
      end

      if self.shadow_onoff != bool(state)
        self.attribute_updated(0x0006, 0x0000)
        self.shadow_onoff = state
      end
    end
  end

  #############################################################
  # web_values
  #
  # Show values of the remote device as HTML
  def web_values()
    import webserver
    self.web_values_prefix()        # display '| ' and name if present
    webserver.content_send(format("%s", self.web_value_onoff(self.shadow_onoff)))
  end

  # Show prefix before web value
  def web_values_prefix()
    import webserver
    var name = self.get_name()
    if !name
      name = "Power" + str(self.tasmota_relay_index)
    end
    webserver.content_send(format(self.PREFIX, name ? webserver.html_escape(name) : ""))
  end
  #############################################################
  #############################################################

end
matter.Plugin_Light0 = Matter_Plugin_Light0
