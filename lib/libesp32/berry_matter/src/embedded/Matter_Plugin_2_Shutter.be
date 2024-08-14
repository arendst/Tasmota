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

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Shutter,weak

class Matter_Plugin_Shutter : Matter_Plugin_Device
  static var TYPE = "shutter"                       # name of the plug-in in json
  static var DISPLAY_NAME = "Shutter"                       # display name of the plug-in
  static var ARG  = "shutter"                       # additional argument name (or empty if none)
  static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  static var ARG_HINT = "Relay<x> number"
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                             # Identify 1.2 p.16
    # 0x0004: inherited                             # Groups 1.3 p.21
    # 0x0005: inherited                             # Scenes 1.4 p.30 - no writable
    0x0102: [0,5,7,0xA,0xB,0xD,0xE,0x17],           # Window Covering 5.3 p.289
  })
  static var TYPES = { 0x0202: 2 }                  # New data model format and notation

  var tasmota_shutter_index                         # Shutter number in Tasmota (zero based)
  var shadow_shutter_pos
  var shadow_shutter_target
  var shadow_shutter_direction                      # 1=opening -1=closing 0=not moving TODO
  var shadow_shutter_inverted                       # 1=same as matter 0=matter must invert

  #############################################################
  # parse_configuration
  #
  # Parse configuration map
  def parse_configuration(config)
    self.tasmota_shutter_index = config.find(self.ARG #-'relay'-#)
    if self.tasmota_shutter_index == nil     self.tasmota_shutter_index = 0   end
    self.shadow_shutter_inverted = -1
  end

  #############################################################
  # Update inverted set
  #
  # Update "inverted" flag from Status 13
  def update_inverted()
    # get the min/max tilt values
    if (self.shadow_shutter_inverted == -1)
      var r_st13 = tasmota.cmd("Status 13", true)     # issue `Status 13`
      if r_st13.contains('StatusSHT')
        r_st13 = r_st13['StatusSHT']        # skip root
        var d = r_st13.find("SHT"+str(self.tasmota_shutter_index), {}).find('Opt')
        # log("MTR: opt: "+str(d))
        if d != nil
          self.shadow_shutter_inverted = int(d[size(d)-1])  # inverted is at the most right character
          # log("MTR: Inverted flag: "+str(self.shadow_shutter_inverted))
        end
      end
    end
  end

  #############################################################
  # Update shadow
  #
  def update_shadow()
    if !self.VIRTUAL
      self.update_inverted()
      var sp = tasmota.cmd("ShutterPosition" + str(self.tasmota_shutter_index + 1), true)
      if sp
        self.parse_sensors(sp)
      end
    end
    super(self).update_shadow()
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute
    var matter_position

    # ====================================================================================================
    if   cluster == 0x0102              # ========== Window Covering 5.3 p.289 ==========
      self.update_shadow_lazy()
      self.update_inverted()
      if   attribute == 0x0000          #  ---------- Type / enum8 ----------
        return tlv_solo.set(TLV.U1, 0xFF) # 0xFF = unknown type of shutter
      elif attribute == 0x0005          #  ---------- NumberOfActuationsLift / u16 ----------
        return tlv_solo.set(TLV.U2, 0)
      elif attribute == 0x0007          #  ---------- ConfigStatus / u8 ----------
        return tlv_solo.set(TLV.U1, 1 + 8)   # Operational + Lift Position Aware
      elif attribute == 0x000D          #  ---------- EndProductType / u8 ----------
        return tlv_solo.set(TLV.U1, 0xFF) # 0xFF = unknown type of shutter
      elif attribute == 0x000E          #  ---------- CurrentPositionLiftPercent100ths / u16 ----------
        if self.shadow_shutter_inverted == 0
          matter_position = (100 - self.shadow_shutter_pos) * 100
        else
          matter_position = self.shadow_shutter_pos * 100
        end
        return tlv_solo.set(TLV.U2, matter_position)
      elif attribute == 0x000A          #  ---------- OperationalStatus / u8 ----------
        var op = self.shadow_shutter_direction == 0 ? 0 : (self.shadow_shutter_direction > 0 ? 1 : 2)
        return tlv_solo.set(TLV.U1, op)
      elif attribute == 0x000B          #  ---------- TargetPositionLiftPercent100ths / u16 ----------
        if self.shadow_shutter_inverted == 0
          matter_position = (100 - self.shadow_shutter_target) * 100
        else
          matter_position = self.shadow_shutter_target * 100
        end
        return tlv_solo.set(TLV.U2, matter_position)

      elif attribute == 0x0017          #  ---------- Mode / u8 ----------
        return tlv_solo.set(TLV.U1, 0)    # normal mode
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
        log("MTR: Tilt = "+str(val), 2)
        var pos_100 = val.findsubval(0)
        if pos_100 != nil
          pos_100 = pos_100 / 100
          if self.shadow_shutter_inverted == 0
            pos_100 = 100 - pos_100
          end
          tasmota.cmd("ShutterPosition"+str(self.tasmota_shutter_index+1) + " " + str(pos_100), true)
          ctx.log = "pos%:"+str(pos_100)
          self.update_shadow()
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
  # parse the output from `ShutterPosition`
  # Ex: `{"Shutter1":{"Position":50,"Direction":0,"Target":50,"Tilt":30}}`
  def parse_sensors(payload)
    var k = "Shutter" + str(self.tasmota_shutter_index + 1)
    if payload.contains(k)
      var v = payload[k]
      # log(format("MTR: getting shutter values(%i): %s", self.endpoint, str(v)), 2)
      # Position
      var val_pos = v.find("Position")
      if val_pos != nil
        if val_pos != self.shadow_shutter_pos
          self.attribute_updated(0x0102, 0x000E)   # CurrentPositionLiftPercent100ths
        end
        self.shadow_shutter_pos = val_pos
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
