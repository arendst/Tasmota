#
# Matter_Plugin_Bridge_Light1.be - implements the behavior for a remote generic Lighting (Dimmer) via HTTP
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

#@ solidify:Matter_Plugin_Bridge_Light1,weak

class Matter_Plugin_Bridge_Light1 : Matter_Plugin_Bridge_Light0
  static var TYPE = "http_light1"                   # name of the plug-in in json
  static var DISPLAY_NAME = "Light 1 Dimmer"      # display name of the plug-in
  # static var ARG  = "relay"                         # additional argument name (or empty if none)
  # static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                             # Identify 1.2 p.16
    # 0x0004: inherited                             # Groups 1.3 p.21
    # 0x0005: inherited                             # Scenes 1.4 p.30 - no writable
    # 0x0006: inherited                             # On/Off 1.5 p.48
    0x0008: [0,2,3,0x0F,0x11,0xFFFC,0xFFFD],        # Level Control 1.6 p.57
  })
  static var TYPES = { 0x0101: 2 }                  # Dimmable Light

  var shadow_bri
  # var tasmota_relay_index # ingerited
  # var shadow_onoff  # inherited

  #############################################################
  # Stub for updating shadow values (local copies of what we published to the Matter gateway)
  #
  # This call is synnchronous and blocking.
  def parse_update(data, index)
    super(self).parse_update(data, index)

    if index == 11                              # Status 11
      var dimmer = int(data.find("Dimmer"))     # 0..100
      if dimmer != nil
        var bri = tasmota.scale_uint(dimmer, 0, 100, 0, 254)
        if bri != self.shadow_bri
          self.attribute_updated(0x0008, 0x0000)
          self.shadow_bri = bri
        end
      end
    end
  end

  #############################################################
  # Model
  #
  def set_bri(v)
    var dimmer = tasmota.scale_uint(v, 0, 254, 0, 100)
    var ret = self.call_remote_sync("Dimmer", str(dimmer))
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
    if   cluster == 0x0008              # ========== Level Control 1.6 p.57 ==========
      self.update_shadow_lazy()
      if   attribute == 0x0000          #  ---------- CurrentLevel / u1 ----------
        if self.shadow_bri != nil
          return tlv_solo.set(TLV.U1, self.shadow_bri)
        else
          return tlv_solo.set(TLV.NULL, nil)
        end
      elif attribute == 0x0002          #  ---------- MinLevel / u1 ----------
        return tlv_solo.set(TLV.U1, 0)
      elif attribute == 0x0003          #  ---------- MaxLevel / u1 ----------
        return tlv_solo.set(TLV.U1, 254)
      elif attribute == 0x000F          #  ---------- Options / map8 ----------
        return tlv_solo.set(TLV.U1, 0)    #
      elif attribute == 0x0011          #  ---------- OnLevel / u1 ----------
        if self.shadow_bri != nil
          return tlv_solo.set(TLV.U1, self.shadow_bri)
        else
          return tlv_solo.set(TLV.NULL, nil)
        end
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return tlv_solo.set(TLV.U4, 0X01)    # OnOff
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return tlv_solo.set(TLV.U4, 5)    # "new data model format and notation"
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
    if   cluster == 0x0008              # ========== Level Control 1.6 p.57 ==========
      if   command == 0x0000            # ---------- MoveToLevel ----------
        var bri_in = val.findsubval(0)  # Hue 0..254
        self.set_bri(bri_in)
        ctx.log = "bri:"+str(bri_in)
        self.publish_command('Bri', tasmota.scale_uint(bri_in, 0, 254, 0, 255),
                             'Dimmer', tasmota.scale_uint(bri_in, 0, 254, 0, 100))
        return true
      elif command == 0x0001            # ---------- Move ----------
        # TODO, we don't really support it
        return true
      elif command == 0x0002            # ---------- Step ----------
        # TODO, we don't really support it
        return true
      elif command == 0x0003            # ---------- Stop ----------
        # TODO, we don't really support it
        return true
      elif command == 0x0004            # ---------- MoveToLevelWithOnOff ----------
        var bri_in = val.findsubval(0)  # Hue 0..254
        self.set_bri(bri_in)
        var onoff = bri_in > 0
        self.set_onoff(onoff)
        ctx.log = "bri:"+str(bri_in)
        self.publish_command('Bri', tasmota.scale_uint(bri_in, 0, 254, 0, 255),
                             'Dimmer', tasmota.scale_uint(bri_in, 0, 254, 0, 100),
                             'Power', onoff ? 1 : 0)
        return true
      elif command == 0x0005            # ---------- MoveWithOnOff ----------
        # TODO, we don't really support it
        return true
      elif command == 0x0006            # ---------- StepWithOnOff ----------
        # TODO, we don't really support it
        return true
      elif command == 0x0007            # ---------- StopWithOnOff ----------
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
    webserver.content_send(format("%s %s", self.web_value_onoff(self.shadow_onoff), self.web_value_dimmer()))
  end

  # Show on/off value as html
  def web_value_dimmer()
    var bri_html = ""
    if self.shadow_bri != nil
      var bri = tasmota.scale_uint(self.shadow_bri, 0, 254, 0, 100)
      bri_html = format("%i%%", bri)
    end
    return  "&#128261; " + bri_html;
  end
end
matter.Plugin_Bridge_Light1 = Matter_Plugin_Bridge_Light1
