#
# Matter_Plugin_Bridge_Light2.be - implements the behavior for a remote generic Lighting (CT) via HTTP
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

#@ solidify:Matter_Plugin_Bridge_Light2,weak

class Matter_Plugin_Bridge_Light2 : Matter_Plugin_Bridge_Light1
  static var TYPE = "http_light2"                   # name of the plug-in in json
  static var DISPLAY_NAME = "Light 2 CT"      # display name of the plug-in
  # static var ARG  = "relay"                         # additional argument name (or empty if none)
  # static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                             # Identify 1.2 p.16
    # 0x0004: inherited                             # Groups 1.3 p.21
    # 0x0005: inherited                             # Scenes 1.4 p.30 - no writable
    # 0x0006: inherited                             # On/Off 1.5 p.48
    # 0x0008: inherited                             # Level Control 1.6 p.57
    0x0300: [7,8,0xF,0x400A,0x400B,0x400C,0xFFFC,0xFFFD],  # Color Control 3.2 p.111
  })
  static var TYPES = { 0x010C: 2 }                  # Dimmable Light

  var shadow_ct
  var ct_min, ct_max

  #############################################################
  # Constructor
  def init(device, endpoint, arguments)
    super(self).init(device, endpoint, arguments)
    self.update_ct_minmax()
  end

  #############################################################
  # Stub for updating shadow values (local copies of what we published to the Matter gateway)
  #
  # TO BE OVERRIDDEN
  # This call is synnchronous and blocking.
  def parse_update(data, index)
    super(self).parse_update(data, index)

    if index == 11                              # Status 11
      var ct = int(data.find("CT"))             # 153..500
      if ct != nil
        if ct != self.shadow_ct
          if ct < self.ct_min   ct = self.ct_min    end
          if ct > self.ct_max   ct = self.ct_max    end
          self.attribute_updated(0x0300, 0x0007)
          self.shadow_ct = ct
        end
      end
    end
  end

  #############################################################
  # Update ct_min/max
  #
  def update_ct_minmax()
    var ct_alexa_mode = tasmota.get_option(82)      # if set, range is 200..380 instead of 153...500
    self.ct_min = ct_alexa_mode ? 200 : 153
    self.ct_max = ct_alexa_mode ? 380 : 500
  end

  #############################################################
  # Model
  #
  def set_ct(v)
    var ret = self.call_remote_sync("CT", str(v))
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
      if   attribute == 0x0007          #  ---------- ColorTemperatureMireds / u2 ----------
        if self.shadow_ct != nil
          return tlv_solo.set(TLV.U1, self.shadow_ct)
        else
          return tlv_solo.set(TLV.NULL, nil)
        end
      elif attribute == 0x0008          #  ---------- ColorMode / u1 ----------
        return tlv_solo.set(TLV.U1, 2)# 2 = ColorTemperatureMireds
      elif attribute == 0x000F          #  ---------- Options / u1 ----------
        return tlv_solo.set(TLV.U1, 0)
      elif attribute == 0x400B          #  ---------- ColorTempPhysicalMinMireds / u2 ----------
        return tlv_solo.set(TLV.U1, self.ct_min)
      elif attribute == 0x400C          #  ---------- ColorTempPhysicalMaxMireds / u2 ----------
        return tlv_solo.set(TLV.U1, self.ct_max)
      
      elif attribute == 0x400A          #  ---------- ColorCapabilities / map32 ----------
        return tlv_solo.set(TLV.U4, 0x10)    # CT
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return tlv_solo.set(TLV.U4, 0x10)    # CT
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
      if   command == 0x000A            # ---------- MoveToColorTemperature ----------
        var ct_in = val.findsubval(0)  # CT
        if ct_in < self.ct_min  ct_in = self.ct_min   end
        if ct_in > self.ct_max  ct_in = self.ct_max   end
        self.set_ct(ct_in)
        ctx.log = "ct:"+str(ct_in)
        self.publish_command('CT', ct_in)
        return true
      elif command == 0x0047            # ---------- StopMoveStep ----------
        # TODO, we don't really support it
        return true
      elif command == 0x004B            # ---------- MoveColorTemperature ----------
        # TODO, we don't really support it
        return true
      elif command == 0x004C            # ---------- StepColorTemperature ----------
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
                              self.web_value_ct()))
  end

  # Show on/off value as html
  def web_value_ct()
    var ct_html = ""
    if self.shadow_ct != nil
      var ct_k = (((1000000 / self.shadow_ct) + 25) / 50) * 50      # convert in Kelvin
      ct_html = format("%iK", ct_k)
    end
    return  "&#9898; " + ct_html;
  end
end
matter.Plugin_Bridge_Light2 = Matter_Plugin_Bridge_Light2
