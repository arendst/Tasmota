#
# Matter_Plugin_Bridge_Light3.be - implements the behavior for a remote generic Lighting (RGB) via HTTP
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

#@ solidify:Matter_Plugin_Bridge_Light3,weak

class Matter_Plugin_Bridge_Light3 : Matter_Plugin_Bridge_Light1
  static var TYPE = "http_light3"                   # name of the plug-in in json
  static var DISPLAY_NAME = "Light 3 RGB"         # display name of the plug-in
  # static var ARG  = "relay"                         # additional argument name (or empty if none)
  # static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                             # Identify 1.2 p.16
    # 0x0004: inherited                             # Groups 1.3 p.21
    # 0x0005: inherited                             # Scenes 1.4 p.30 - no writable
    # 0x0006: inherited                             # On/Off 1.5 p.48
    # 0x0008: inherited                             # Level Control 1.6 p.57
    0x0300: [0,1,7,8,0xF,0x4001,0x400A,0xFFFC,0xFFFD],# Color Control 3.2 p.111
  })
  static var TYPES = { 0x010D: 2 }                  # Extended Color Light

  var shadow_hue, shadow_sat                        # 0..254

  #############################################################
  # Constructor
  def init(device, endpoint, arguments)
    super(self).init(device, endpoint, arguments)
  end

  #############################################################
  # Stub for updating shadow values (local copies of what we published to the Matter gateway)
  #
  # This call is synnchronous and blocking.
  def parse_update(data, index)
    super(self).parse_update(data, index)

    if index == 11                              # Status 11
      var hsb = data.find("HSBColor")
      if hsb
        import string
        var hsb_list = string.split(hsb, ",")
        var hue = int(hsb_list[0])
        var sat = int(hsb_list[1])
        # dimmer is already available

        if hue != nil     hue = tasmota.scale_uint(hue, 0, 360, 0, 254)   else hue = self.shadow_hue      end
        if sat != nil     sat = tasmota.scale_uint(sat, 0, 100, 0, 254)   else sat = self.shadow_sat      end
        if hue != self.shadow_hue   self.attribute_updated(0x0300, 0x0000)   self.shadow_hue = hue   end
        if sat != self.shadow_sat   self.attribute_updated(0x0300, 0x0001)   self.shadow_sat = sat   end
      end
    end
  end

  #############################################################
  # Model
  #
  def set_hue(v)
    var hue_360 = tasmota.scale_uint(v, 0, 254, 0, 360)
    var ret = self.call_remote_sync("HSBColor1", hue_360)
    if ret != nil
      self.parse_update(ret, 11)        # update shadow from return value
    end
  end
  def set_sat(v)
    var sat_100 = tasmota.scale_uint(v, 0, 254, 0, 100)
    var ret = self.call_remote_sync("HSBColor2", sat_100)
    if ret != nil
      self.parse_update(ret, 11)        # update shadow from return value
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
    if   cluster == 0x0300              # ========== Color Control 3.2 p.111 ==========
      self.update_shadow_lazy()
      if   attribute == 0x0000          #  ---------- CurrentHue / u1 ----------
        if self.shadow_hue != nil
          return tlv_solo.set(TLV.U1, self.shadow_hue)
        else
          return tlv_solo.set(TLV.NULL, nil)
        end
      elif attribute == 0x0001          #  ---------- CurrentSaturation / u2 ----------
        if self.shadow_sat != nil
          return tlv_solo.set(TLV.U1, self.shadow_sat)
        else
          return tlv_solo.set(TLV.NULL, nil)
        end
      elif attribute == 0x0007          #  ---------- ColorTemperatureMireds / u2 ----------
        return tlv_solo.set(TLV.U1, 0)
      elif attribute == 0x0008          #  ---------- ColorMode / u1 ----------
        return tlv_solo.set(TLV.U1, 0)# 0 = CurrentHue and CurrentSaturation
      elif attribute == 0x000F          #  ---------- Options / u1 ----------
        return tlv_solo.set(TLV.U1, 0)
      elif attribute == 0x4001          #  ---------- EnhancedColorMode / u1 ----------
        return tlv_solo.set(TLV.U1, 0)
      elif attribute == 0x400A          #  ---------- ColorCapabilities / map32 ----------
        return tlv_solo.set(TLV.U4, 0x01)    # HS

      # Defined Primaries Information Attribute Set
      elif attribute == 0x0010          #  ---------- NumberOfPrimaries / u1 ----------
        return tlv_solo.set(TLV.U1, 0)

      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return tlv_solo.set(TLV.U4, 0x01)    # HS
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return tlv_solo.set(TLV.U4, 5)    # "new data model format and notation, FeatureMap support"
      end

    else
      return super(self).read_attribute(session, ctx, tlv_solo)
    end
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
    if   cluster == 0x0300              # ========== Color Control 3.2 p.111 ==========
      if   command == 0x0000            # ---------- MoveToHue ----------
        var hue_in = val.findsubval(0)  # Hue 0..254
        self.set_hue(hue_in)
        ctx.log = "hue:"+str(hue_in)
        self.publish_command('Hue', hue_in)
        return true
      elif command == 0x0001            # ---------- MoveHue ----------
        # TODO, we don't really support it
        return true
      elif command == 0x0002            # ---------- StepHue ----------
        # TODO, we don't really support it
        return true
      elif command == 0x0003            # ---------- MoveToSaturation ----------
        var sat_in = val.findsubval(0)  # Sat 0..254
        self.set_sat(sat_in)
        ctx.log = "sat:"+str(sat_in)
        self.publish_command('Sat', sat_in)
        return true
      elif command == 0x0004            # ---------- MoveSaturation ----------
        # TODO, we don't really support it
        return true
      elif command == 0x0005            # ---------- StepSaturation ----------
        # TODO, we don't really support it
        return true
      elif command == 0x0006            # ---------- MoveToHueAndSaturation ----------
        var hue_in = val.findsubval(0)  # Hue 0..254
        var sat_in = val.findsubval(1)  # Sat 0..254
        self.set_hue(hue_in)
        self.set_sat(sat_in)
        ctx.log = "hue:"+str(hue_in)+" sat:"+str(sat_in)
        self.publish_command('Hue', hue_in, 'Sat', sat_in)
        return true
      elif command == 0x0047            # ---------- StopMoveStep ----------
        # TODO, we don't really support it
        return true
      end
    
    else
      return super(self).invoke_request(session, val, ctx)
    end
  end

  #############################################################
  # web_values
  #
  # Show values of the remote device as HTML
  def web_values()
    import webserver
    self.web_values_prefix()        # display '| ' and name if present
    webserver.content_send(format("%s %s %s",
                              self.web_value_onoff(self.shadow_onoff), self.web_value_dimmer(),
                              self.web_value_RGB()))
  end

  # Show on/off value as html
  def web_value_RGB()
    if self.shadow_hue != nil && self.shadow_sat != nil
      var l = light_state(3)      # RGB virtual light state object
      l.set_bri(255)              # set full brightness to get full range RGB
      l.set_huesat(tasmota.scale_uint(self.shadow_hue, 0, 254, 0, 360), tasmota.scale_uint(self.shadow_sat, 0, 254, 0, 255))
      var rgb_hex = format("#%02X%02X%02X", l.r, l.g, l.b)
      var rgb_html = format('<i class="bxm" style="--cl:%s"></i>%s', rgb_hex, rgb_hex)
      return rgb_html
    end
    return ""
  end
end
matter.Plugin_Bridge_Light3 = Matter_Plugin_Bridge_Light3
