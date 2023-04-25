#
# Matter_Plugin_Shutter.be - implements the behavior for shutters
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
class Matter_Plugin_Device end

#@ solidify:Matter_Plugin_Shutter,weak

class Matter_Plugin_Shutter : Matter_Plugin_Device
  static var TYPE = "shutter"                       # name of the plug-in in json
  static var NAME = "Shutter"                       # display name of the plug-in
  static var ARG  = "shutter"                       # additional argument name (or empty if none)
  static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  static var CLUSTERS  = {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                             # Identify 1.2 p.16
    # 0x0004: inherited                             # Groups 1.3 p.21
    # 0x0005: inherited                             # Scenes 1.4 p.30 - no writable
    0x0102: [0,5,6,7,8,9,0xA,0xB,0xC,0xD,0xE,0xF,0x17,0xFFFC,0xFFFD],                      # Window Covering 5.3 p.289
  }
  static var TYPES = { 0x0202: 2 }                  # New data model format and notation

  var tasmota_shutter_index                         # Shutter number in Tasmota (zero based)
  var shadow_shutter_pos
  var shadow_shutter_target
  var shadow_shutter_tilt
  var shadow_shutter_direction                      # 1=opening -1=closing 0=not moving TODO

  #############################################################
  # Constructor
  def init(device, endpoint, arguments)
    super(self).init(device, endpoint, arguments)
    self.tasmota_shutter_index = arguments.find(self.ARG #-'relay'-#)
    if self.tasmota_shutter_index == nil     self.tasmota_shutter_index = 0   end
  end

  #############################################################
  # Update shadow
  #
  def update_shadow()
    var sp = tasmota.cmd("ShutterPosition" + str(self.tasmota_shutter_index + 1), true)
    if sp
      self.parse_sensors(sp)
    end
    super(self).update_shadow()
  end

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
      if   attribute == 0x0000          #  ---------- Type / enum8 ----------
        return TLV.create_TLV(TLV.U1, 0xFF) # 0xFF = unknown type of shutter
      elif attribute == 0x0005          #  ---------- NumberOfActuationsLift / u16 ----------
        return TLV.create_TLV(TLV.U2, 0)
      elif attribute == 0x0006          #  ---------- NumberOfActuationsTilt / u16 ----------
        return TLV.create_TLV(TLV.U2, 0)
      elif attribute == 0x0007          #  ---------- ConfigStatus / u8 ----------
        return TLV.create_TLV(TLV.U1, 1 + 8 + 16)   # Operational + Lift Position Aware + Tilt Position Aware
      elif attribute == 0x000D          #  ---------- EndProductType / u8 ----------
        return TLV.create_TLV(TLV.U1, 0xFF) # 0xFF = unknown type of shutter

      elif attribute == 0x0008          #  ---------- CurrentPositionLiftPercentage / u8 ----------
        return TLV.create_TLV(TLV.U2, 100 - self.shadow_shutter_pos)
      elif attribute == 0x000E          #  ---------- CurrentPositionLiftPercent100ths / u16 ----------
        return TLV.create_TLV(TLV.U2, (100 - self.shadow_shutter_pos) * 100)
      elif attribute == 0x0009          #  ---------- CurrentPositionTiltPercentage / u8 ----------
        return TLV.create_TLV(TLV.U2, 100 - self.shadow_shutter_tilt)
      elif attribute == 0x000F          #  ---------- CurrentPositionTiltPercent100ths / u8 ----------
        return TLV.create_TLV(TLV.U2, (100 - self.shadow_shutter_tilt) * 100)
      elif attribute == 0x000A          #  ---------- OperationalStatus / u8 ----------
        var op = self.shadow_shutter_direction == 0 ? 0 : (self.shadow_shutter_direction > 0 ? 1 : 2)
        return TLV.create_TLV(TLV.U1, op)    # TODO from sensors
      elif attribute == 0x000B          #  ---------- TargetPositionLiftPercent100ths / u16 ----------
        return TLV.create_TLV(TLV.U2, (100 - self.shadow_shutter_target) * 100)
      elif attribute == 0x000C          #  ---------- TargetPositionTiltPercent100ths / u16 ----------
        return TLV.create_TLV(TLV.U1, 0)    # TODO

      elif attribute == 0x0017          #  ---------- Mode / u8 ----------
        return TLV.create_TLV(TLV.U1, 0)    # normal mode

      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return TLV.create_TLV(TLV.U4, 3 + 4 + 16)    # Lift + Tilt + PA_LF + PA_TL
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return TLV.create_TLV(TLV.U4, 5)    # New data model format and notation
      end

    else
      return super(self).read_attribute(session, ctx)
    end
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
      if   command == 0x0000            # ---------- UpOrOpen ----------
        tasmota.cmd("ShutterStopOpen"+str(self.tasmota_shutter_index+1), true)
        self.update_shadow()
        return true
      elif command == 0x0001            # ---------- DownOrClose ----------
        tasmota.cmd("ShutterStopClose"+str(self.tasmota_shutter_index+1), true)
        self.update_shadow()
        return true
      elif command == 0x0002            # ---------- StopMotion ----------
        tasmota.cmd("ShutterStop"+str(self.tasmota_shutter_index+1), true)
        self.update_shadow()
        return true
      elif command == 0x0005            # ---------- GoToLiftPercentage ----------
        tasmota.log("MTR: Tilt = "+str(val), 2)
        var pos_100 = val.findsubval(0)
        if pos_100 != nil
          pos_100 = pos_100 / 100
          tasmota.cmd("ShutterStopPosition"+str(self.tasmota_shutter_index+1) + " " + str(100 - pos_100), true)
          ctx.log = "pos%:"+str(pos_100)
          self.update_shadow()
        end
        return true
      elif command == 0x0008            # ---------- GoToTiltPercentage ----------
        var tilt = val.findsubval(0)
        if tilt != nil
          tilt = tilt / 10
          ctx.log = "tilt%:"+str(tilt)
        end
        return true
      end
    
    else
      return super(self).invoke_request(session, val, ctx)
    end

  end

  #############################################################
  # parse sensor
  #
  # The device calls regularly `tasmota.read_sensors()` and converts
  # it to json.
  def parse_sensors(payload)
    import string
    var k = "Shutter" + str(self.tasmota_shutter_index + 1)
    if payload.contains(k)
      var v = payload[k]
      # tasmota.log(string.format("MTR: getting shutter values(%i): %s", self.endpoint, str(v)), 2)
      # Position
      var val_pos = v.find("Position")
      if val_pos != nil
        if val_pos != self.shadow_shutter_pos
          # self.attribute_updated(0x0102, 0x0008)   # CurrentPositionLiftPercentage
          self.attribute_updated(0x0102, 0x000E)   # CurrentPositionLiftPercent100ths
        end
        self.shadow_shutter_pos = val_pos
      end
      # Tilt
      var val_tilt = v.find("Tilt")
      if val_tilt != nil
        if val_tilt != self.shadow_shutter_tilt
          # self.attribute_updated(0x0102, 0x0009)   # CurrentPositionTiltPercentage
          self.attribute_updated(0x0102, 0x000F)   # CurrentPositionTiltPercent100ths
        end
        self.shadow_shutter_tilt = val_tilt
      end
      # Direction
      var val_dir = v.find("Direction")
      if val_dir != nil
        if val_dir != self.shadow_shutter_direction
          self.attribute_updated(0x0102, 0x000A)   # OperationalStatus
        end
        self.shadow_shutter_direction = val_dir
      end
      # Target
      var val_target = v.find("Target")
      if val_target != nil
        if val_target != self.shadow_shutter_target
          self.attribute_updated(0x0102, 0x000B)   # TargetPositionLiftPercent100ths
        end
        self.shadow_shutter_target = val_target
      end
      #
    end
  end

end
matter.Plugin_Shutter = Matter_Plugin_Shutter
