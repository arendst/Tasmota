#
# Matter_Plugin_Bridge_Light0.be - implements the behavior for a remote generic Lighting (OnOff only) via HTTP
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

#@ solidify:Matter_Plugin_Bridge_Light0,weak

class Matter_Plugin_Bridge_Light0 : Matter_Plugin_Bridge_HTTP
  static var TYPE = "http_light0"                   # name of the plug-in in json
  static var DISPLAY_NAME = "Light 0 On"          # display name of the plug-in
  static var ARG  = "relay"                         # additional argument name (or empty if none)
  static var ARG_HINT = "Power<x> number"
  static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  # static var UPDATE_TIME = 3000                     # update every 3s
  # static var UPDATE_CMD = "Status 11"               # command to send for updates
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                             # Identify 1.2 p.16
    # 0x0004: inherited                             # Groups 1.3 p.21
    # 0x0005: inherited                             # Scenes 1.4 p.30 - no writable
    0x0006: [0],                                    # On/Off 1.5 p.48
  })
  static var TYPES = { 0x0100: 2 }                  # OnOff Light, but not actually used because Relay is managed by OnOff

  var tasmota_relay_index                           # Relay number in Tasmota (one based)
  var shadow_onoff                                  # fake status for now # TODO

  #############################################################
  # Constructor
  def init(device, endpoint, arguments)
    super(self).init(device, endpoint, arguments)
    self.shadow_onoff = false
    self.tasmota_relay_index = int(arguments.find(self.ARG #-'relay'-#, 1))
    if self.tasmota_relay_index <= 0    self.tasmota_relay_index = 1    end
  end

  #############################################################
  # Stub for updating shadow values (local copies of what we published to the Matter gateway)
  #
  # This call is synnchronous and blocking.
  def parse_update(data, index)
    if index == 11                              # Status 11
      var state = false

      if self.tasmota_relay_index == 1 && data.contains("POWER")        # special case, can be `POWER` or `POWER1`
        state = (data.find("POWER") == "ON")
      else
        state = (data.find("POWER" + str(self.tasmota_relay_index)) == "ON")
      end

      if self.shadow_onoff != nil && self.shadow_onoff != bool(state)
        self.attribute_updated(0x0006, 0x0000)
      end
      self.shadow_onoff = state
    end
  end

  #############################################################
  # Model
  #
  def set_onoff(v)
    var ret = self.call_remote_sync("Power" + str(self.tasmota_relay_index), v ? "1" : "0")
    if ret != nil
      self.parse_update(ret, 11)        # update shadow from return value
      # self.tick = self.device.tick      # prevent an explicit Status11 for as it is not needed if the subscription update is sent in same tick
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

end
matter.Plugin_Bridge_Light0 = Matter_Plugin_Bridge_Light0
