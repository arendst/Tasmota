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

# Matter plug-in for core behavior

# dummy declaration for solidification
class Matter_Plugin_Shutter end

#@ solidify:Matter_Plugin_ShutterTilt,weak

class Matter_Plugin_ShutterTilt : Matter_Plugin_Shutter
  static var TYPE = "shutter+tilt"                  # name of the plug-in in json
  static var NAME = "Shutter + Tilt"                # display name of the plug-in
  # inherited static var ARG  = "shutter"                       # additional argument name (or empty if none)
  # inherited static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  static var CLUSTERS  = {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                             # Identify 1.2 p.16
    # 0x0004: inherited                             # Groups 1.3 p.21
    # 0x0005: inherited                             # Scenes 1.4 p.30 - no writable
    0x0102: [7,0xC,0xF,0xFFFC],                      # Window Covering 5.3 p.289
  }
  # inherited static var TYPES = { 0x0202: 2 }                  # New data model format and notation

  # below is inherited
  # var tasmota_shutter_index                         # Shutter number in Tasmota (zero based)
  # var shadow_shutter_pos
  # var shadow_shutter_target
  # var shadow_shutter_tilt
  # var shadow_shutter_direction                      # 1=opening -1=closing 0=not moving TODO

  #############################################################
  # Constructor inherited

  #############################################################
  # Update shadow inherited

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx)
    import string
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0102              # ========== Window Covering 5.3 p.289 ==========
      self.update_shadow_lazy()
      if   attribute == 0x0007          #  ---------- ConfigStatus / u8 ----------
        return TLV.create_TLV(TLV.U1, 1 + 8 + 16)   # Operational + Lift Position Aware + Tilt Position Aware

      elif attribute == 0x000F          #  ---------- CurrentPositionTiltPercent100ths / u8 ----------
        return TLV.create_TLV(TLV.U2, (100 - self.shadow_shutter_tilt) * 100)
      elif attribute == 0x000C          #  ---------- TargetPositionTiltPercent100ths / u16 ----------
        return TLV.create_TLV(TLV.U1, 0)    # TODO

      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return TLV.create_TLV(TLV.U4, 3 + 4 + 16)    # Lift + Tilt + PA_LF + PA_TL
      end
    end
    # else
    return super(self).read_attribute(session, ctx)
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
          tilt = tilt / 10
          ctx.log = "tilt%:"+str(tilt)
        end
        return true
      end
    end
    # else
    return super(self).invoke_request(session, val, ctx)

  end

  #############################################################
  # parse sensor inherited

end
matter.Plugin_ShutterTilt = Matter_Plugin_ShutterTilt
