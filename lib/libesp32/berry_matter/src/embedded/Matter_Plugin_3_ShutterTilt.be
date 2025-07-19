#
# Matter_Plugin_ShutterTilt.be - implements the behavior for shutters with tilt control
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

#@ solidify:Matter_Plugin_ShutterTilt,weak

class Matter_Plugin_ShutterTilt : Matter_Plugin_Shutter
  static var TYPE = "shutter+tilt"                  # name of the plug-in in json
  static var DISPLAY_NAME = "Shutter + Tilt"                # display name of the plug-in
  # inherited static var ARG  = "shutter"                       # additional argument name (or empty if none)
  # inherited static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                             # Identify 1.2 p.16
    # 0x0004: inherited                             # Groups 1.3 p.21
    # 0x0005: inherited                             # Scenes 1.4 p.30 - no writable
    0x0102: [7,0xC,0xF],                            # Window Covering 5.3 p.289
  })
  # inherited static var TYPES = { 0x0202: 2 }                  # New data model format and notation

  # below is inherited
  # var tasmota_shutter_index                         # Shutter number in Tasmota (zero based)
  # var shadow_shutter_pos
  # var shadow_shutter_target
  # var shadow_shutter_tilt
  # var shadow_shutter_direction                      # 1=opening -1=closing 0=not moving TODO
  var shadow_shutter_tilt
  var tilt_min, tilt_max

  #############################################################
  # Constructor inherited

  #############################################################
  # Update shadow inherited

  #############################################################
  # parse sensor
  #
  # parse the output from `ShutterPosition`
  # Ex: `{"Shutter1":{"Position":50,"Direction":0,"Target":50,"Tilt":30}}`
  def parse_sensors(payload)
    var k = "Shutter" + str(self.tasmota_shutter_index + 1)
    if payload.contains(k)
      var v = payload[k]
      # Tilt - we can keep it here knowing that it won't change if not implemented
      var val_tilt = v.find("Tilt")
      if val_tilt != nil
        if val_tilt != self.shadow_shutter_tilt
          self.attribute_updated(0x0102, 0x000F)   # CurrentPositionTiltPercent100ths
        end
        self.shadow_shutter_tilt = val_tilt
      end
      #
    end
    super(self).parse_sensors(payload)            # parse other shutter values
  end

  #############################################################
  # Update tilt min/max
  #
  # Update the min and max tilt values from Status 12
  def update_tilt_min_max()
    # get the min/max tilt values
    var r_st13 = tasmota.cmd("Status 13", true)     # issue `Status 13`
    if r_st13.contains('StatusSHT')
      r_st13 = r_st13['StatusSHT']        # skip root
      var d = r_st13.find("SHT"+str(self.tasmota_shutter_index), {}).find('TiltConfig')
      if d != nil
        self.tilt_min = int(d[0])
        self.tilt_max = int(d[1])
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
    if   cluster == 0x0102              # ========== Window Covering 5.3 p.289 ==========
      self.update_shadow_lazy()
      if   attribute == 0x0007          #  ---------- ConfigStatus / u8 ----------
        return tlv_solo.set(TLV.U1, 1 + 8 + 16)   # Operational + Lift Position Aware + Tilt Position Aware

      elif attribute == 0x000F          #  ---------- CurrentPositionTiltPercent100ths / u8 ----------
        self.update_tilt_min_max()
        if self.tilt_min != nil && self.tilt_max != nil
          var tilt_percentage = tasmota.scale_uint(self.shadow_shutter_tilt - self.tilt_min, 0, self.tilt_max - self.tilt_min, 0, 10000)
          return tlv_solo.set(TLV.U2, tilt_percentage)
        else
          return tlv_solo.set(TLV.NULL, nil)                    # return invalid
        end
      elif attribute == 0x000C          #  ---------- TargetPositionTiltPercent100ths / u16 ----------
        if self.tilt_min != nil && self.tilt_max != nil
          var tilt_percentage = tasmota.scale_uint(self.shadow_shutter_tilt - self.tilt_min, 0, self.tilt_max - self.tilt_min, 0, 10000)
          return tlv_solo.set(TLV.U2, tilt_percentage)
        else
          return tlv_solo.set(TLV.NULL, nil)                    # return invalid
        end

      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return tlv_solo.set(TLV.U4, 3 + 4 + 16)    # Lift + Tilt + PA_LF + PA_TL
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
    import light
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var command = ctx.command

    # ====================================================================================================
    if   cluster == 0x0102              # ========== Window Covering 5.3 p.289 ==========
      self.update_shadow_lazy()
      if   command == 0x0008            # ---------- GoToTiltPercentage ----------
        var tilt = val.findsubval(0)
        if tilt != nil
          self.update_tilt_min_max()
          if self.tilt_min != nil && self.tilt_max !=  nil
            var tilt_val = self.tilt_min + tasmota.scale_uint(tilt, 0, 10000, 0, self.tilt_max - self.tilt_min)
            tasmota.cmd("ShutterTilt"+str(self.tasmota_shutter_index+1) + " " + str(tilt_val), false)   # TODO
            self.update_shadow()
            ctx.log = "tilt%:"+str(tilt)
          else
            ctx.log = "tilt%(no_tilt_support):"+str(tilt)
          end

        end
        return true
      end
    end
    # else
    return super(self).invoke_request(session, val, ctx)

  end

end
matter.Plugin_ShutterTilt = Matter_Plugin_ShutterTilt
