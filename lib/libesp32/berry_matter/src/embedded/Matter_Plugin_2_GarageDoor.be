#
# Matter_Plugin_2_GarageDoor.be - implements the behavior for a Garage Door (Closure)
#
# Copyright (C) 2026  Ludovic BOUÉ
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
# Matter 1.6.0 Device Specification - Closure (0x0230)
#################################################################################
# Device Type: Closure (0x0230) - introduced in Matter 1.5.0
# Device Type Revision: 2 (as of Matter 1.6.0 Device Library)
# Class: Simple | Scope: Endpoint
#
# The Closure device type is the unified umbrella (Matter 1.5.0+) for window
# coverings, doors, gates, cabinets and garage doors (GDO). A garage door is a
# Closure whose motion is exposed through the Closure Control cluster.
#
# CLUSTERS (Server):
# - 0x0104: Closure Control (M) - unified movement/state interface
# - 0x0003: Identify (M) - inherited from base class
# - 0x001D: Descriptor (M) - inherited from base class
#
# NOTES:
# - This plugin drives a position-aware Tasmota Shutter (relay based garage
#   door opener) and reports fully-open / fully-closed / partially-open state.
# - Finer-grained percentage positioning would be modelled by a companion
#   Closure Panel (0x0231) endpoint with the Closure Dimension cluster (0x0105);
#   not implemented here (garage doors are typically enum-position only).
# - A semantic tag from the Closure namespace (garage door) could be added to
#   the Descriptor TagList to disambiguate the closure variant to controllers.
#################################################################################

#################################################################################
# Matter 1.6.0 Closure Control Cluster (0x0104)
#################################################################################
# Cluster Revision: 2 (introduced in Matter 1.5.0 as Rev 1, bumped to Rev 2 in Matter 1.6.0)
# Role: Application | Scope: Endpoint | PICS: CLCTRL
#
# FEATURES (this plugin):
# - Bit 0 (PS): Positioning - reports Closed/Open/PartiallyOpen position
#
# ATTRIBUTES:
# ID     | Name                 | Type                      | Quality | Conf
# -------|----------------------|---------------------------|---------|-----
# 0x0000 | CountdownTime        | elapsed_s (uint32)        | X       | O
# 0x0001 | MainState            | MainStateEnum (enum8)     |         | M
# 0x0002 | CurrentErrorList     | list[ClosureErrorEnum]    |         | M
# 0x0003 | OverallCurrentState  | OverallCurrentStateStruct | X       | PS|LT|SP
# 0x0004 | OverallTargetState   | OverallTargetStateStruct  | X       | PS|LT|SP
#
# MainStateEnum(enum8): Stopped=0, Moving=1, WaitingForMotion=2, Error=3,
#                       Calibrating=4, Protected=5, Disengaged=6, SetupRequired=7
# CurrentPositionEnum(enum8): FullyClosed=0, FullyOpened=1, PartiallyOpened=2,
#                       OpenedForPedestrian=3, OpenedForVentilation=4, OpenedAtSignature=5
# TargetPositionEnum(enum8): MoveToFullyClosed=0, MoveToFullyOpen=1,
#                       MoveToPedestrianPosition=2, MoveToVentilationPosition=3,
#                       MoveToSignaturePosition=4
#
# OverallCurrentStateStruct: 0:Position(CurrentPositionEnum,X,O) 1:Latch(bool,X,O)
#                            2:Speed(ThreeLevelAutoEnum,O) 3:SecureState(bool,X)
# OverallTargetStateStruct : 0:Position(TargetPositionEnum,X,O) 1:Latch(bool,X,O)
#                            2:Speed(ThreeLevelAutoEnum,O)
#
# COMMANDS:
# ID   | Name      | Dir  | Access | Conf | Notes
# -----|-----------|------|--------|------|---------------------------
# 0x00 | Stop      | C→S  | O      | O    | halt motion
# 0x01 | MoveTo    | C→S  | O      | M    | timed invoke required
#
# MoveTo: {Position:TargetPositionEnum(O), Latch:bool(O), Speed:ThreeLevelAutoEnum(O)}
#
# TASMOTA IMPLEMENTATION:
# - ARG: "shutter" - Tasmota Shutter number (zero based)
# - Reads position from `ShutterPosition<x>`; honors SetOption80 (Status 13 Opt)
# - MoveTo FullyOpen  -> ShutterOpen<n>
# - MoveTo FullyClosed-> ShutterClose<n>
# - Stop              -> ShutterStop<n>
# - Position: Tasmota closed% (0=open .. 100=closed) -> Closed/Partial/Open enum
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_GarageDoor,weak

class Matter_Plugin_GarageDoor : Matter_Plugin_Device
  static var TYPE = "garage"                        # name of the plug-in in json
  static var DISPLAY_NAME = "Garage Door"           # display name of the plug-in

  static var SCHEMA = "shutter|"                    # arg name
                      "l:Shutter|"                  # label (display name)
                      "t:i|"                        # type: int
                      "h:Shutter<x> number (0 based)" # hint
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                             # Identify 1.2 p.16
    0x0104: [0,1,2,3,4],                            # Closure Control (Matter 1.6.0)
  })
  static var TYPES = { 0x0230: 2 }                  # Closure - Matter 1.6.0 Device Library Rev 2

  # MainStateEnum
  static var MS_STOPPED = 0
  static var MS_MOVING  = 1
  # CurrentPositionEnum
  static var CP_CLOSED  = 0
  static var CP_OPENED  = 1
  static var CP_PARTIAL = 2
  # TargetPositionEnum
  static var TP_CLOSE   = 0
  static var TP_OPEN    = 1

  var tasmota_shutter_index                         # Shutter number in Tasmota (zero based)
  var shadow_shutter_pos                            # last known position 0..100 (Tasmota convention)
  var shadow_shutter_direction                      # 1=opening -1=closing 0=not moving
  var shadow_shutter_inverted                       # 1=matter convention 0=must invert, -1=unknown
  var shadow_target                                 # TargetPositionEnum in flight, or nil

  #############################################################
  # parse_configuration
  #
  def parse_configuration(config)
    super(self).parse_configuration(config)
    self.tasmota_shutter_index = config.find('shutter')
    if self.tasmota_shutter_index == nil     self.tasmota_shutter_index = 0   end
    self.shadow_shutter_inverted = -1
  end

  #############################################################
  # Update "inverted" flag from `Status 13`
  #
  def update_inverted()
    if (self.shadow_shutter_inverted == -1)
      var r_st13 = tasmota.cmd("Status 13", true)     # issue `Status 13`
      if r_st13.contains('StatusSHT')
        r_st13 = r_st13['StatusSHT']        # skip root
        var d = r_st13.find("SHT"+str(self.tasmota_shutter_index), {}).find('Opt')
        if d != nil
          self.shadow_shutter_inverted = int(d[size(d)-1])  # inverted is the right-most character
        end
      end
    end
  end

  #############################################################
  # Update shadow from Tasmota `ShutterPosition`
  #
  def update_shadow()
    if !self.VIRTUAL && !self.BRIDGE
      self.update_inverted()
      var sp = tasmota.cmd("ShutterPosition" + str(self.tasmota_shutter_index + 1), true)
      if sp
        self.parse_sensors(sp)
      end
    end
    super(self).update_shadow()
  end

  #############################################################
  # closed_pct
  #
  # Returns 0..100 where 100 = fully closed, 0 = fully open, or nil if unknown.
  # Mirrors the Shutter plugin convention (honors SetOption80 inversion).
  def closed_pct()
    if self.shadow_shutter_pos == nil    return nil    end
    if self.shadow_shutter_inverted == 0
      return 100 - self.shadow_shutter_pos
    end
    return self.shadow_shutter_pos
  end

  #############################################################
  # current_position_enum -> CurrentPositionEnum or nil
  #
  def current_position_enum()
    var cp = self.closed_pct()
    if cp == nil            return nil               end
    if cp >= 100            return self.CP_CLOSED    end
    if cp <= 0              return self.CP_OPENED    end
    return self.CP_PARTIAL
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0104              # ========== Closure Control (Matter 1.6.0) ==========
      self.update_shadow_lazy()
      if   attribute == 0x0000          #  ---------- CountdownTime / elapsed_s ----------
        return tlv_solo.set(0x14 #-TLV.NULL-#, nil)   # not supported -> null
      elif attribute == 0x0001          #  ---------- MainState / MainStateEnum ----------
        var moving = (self.shadow_shutter_direction != nil) && (self.shadow_shutter_direction != 0)
        return tlv_solo.set(0x04 #-TLV.U1-#, moving ? self.MS_MOVING : self.MS_STOPPED)
      elif attribute == 0x0002          #  ---------- CurrentErrorList / list ----------
        return TLV.Matter_TLV_array()   # no errors
      elif attribute == 0x0003          #  ---------- OverallCurrentState / struct ----------
        var pos = self.current_position_enum()
        if pos == nil
          return tlv_solo.set(0x14 #-TLV.NULL-#, nil)
        end
        var s = TLV.Matter_TLV_struct()
        s.add_TLV(0, 0x04 #-TLV.U1-#, pos)                       # Position (CurrentPositionEnum)
        s.add_TLV(3, 0x08 #-TLV.BOOL-#, pos == self.CP_CLOSED)   # SecureState (secured when fully closed)
        return s
      elif attribute == 0x0004          #  ---------- OverallTargetState / struct ----------
        if self.shadow_target == nil
          return tlv_solo.set(0x14 #-TLV.NULL-#, nil)
        end
        var s = TLV.Matter_TLV_struct()
        s.add_TLV(0, 0x04 #-TLV.U1-#, self.shadow_target)        # Position (TargetPositionEnum)
        return s
      elif attribute == 0xFFF9          #  ---------- AcceptedCommandList ----------
        var al = TLV.Matter_TLV_array()
        al.add_TLV(nil, 0x06 #-TLV.U4-#, 0x0000)    # Stop
        al.add_TLV(nil, 0x06 #-TLV.U4-#, 0x0001)    # MoveTo
        return al
      end

    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  #############################################################
  # Invoke a command
  #
  def invoke_request(session, val, ctx)
    var cluster = ctx.cluster
    var command = ctx.command
    var idx1 = str(self.tasmota_shutter_index + 1)      # Tasmota shutter number is 1 based

    # ====================================================================================================
    if   cluster == 0x0104              # ========== Closure Control (Matter 1.6.0) ==========
      self.update_shadow_lazy()
      if   command == 0x0000            # ---------- Stop ----------
        tasmota.cmd("ShutterStop" + idx1, true)
        self.shadow_target = nil
        self.attribute_updated(0x0104, 0x0004)          # OverallTargetState
        self.update_shadow()
        return true
      elif command == 0x0001            # ---------- MoveTo ----------
        var target = val.findsubval(0)                  # Position (TargetPositionEnum)
        if   target == self.TP_OPEN
          tasmota.cmd("ShutterOpen" + idx1, true)
          self.shadow_target = self.TP_OPEN
          ctx.log = "MoveTo:Open"
        elif target == self.TP_CLOSE
          tasmota.cmd("ShutterClose" + idx1, true)
          self.shadow_target = self.TP_CLOSE
          ctx.log = "MoveTo:Close"
        else
          ctx.status = 0x87 #-matter.CONSTRAINT_ERROR-#   # unsupported target position
          return false
        end
        self.attribute_updated(0x0104, 0x0004)          # OverallTargetState
        self.update_shadow()
        return true
      end

    else
      return super(self).invoke_request(session, val, ctx)
    end
  end

  #############################################################
  # parse the output from `ShutterPosition`
  # Ex: `{"Shutter1":{"Position":50,"Direction":0,"Target":50,"Tilt":30}}`
  def parse_sensors(payload)
    var k = "Shutter" + str(self.tasmota_shutter_index + 1)
    if payload.contains(k)
      var v = payload[k]
      # Position
      var val_pos = v.find("Position")
      if val_pos != nil
        if val_pos != self.shadow_shutter_pos
          self.attribute_updated(0x0104, 0x0003)        # OverallCurrentState
        end
        self.shadow_shutter_pos = val_pos
      end
      # Direction (drives MainState)
      var val_dir = v.find("Direction")
      if val_dir != nil
        if val_dir != self.shadow_shutter_direction
          self.attribute_updated(0x0104, 0x0001)        # MainState
        end
        self.shadow_shutter_direction = val_dir
        if val_dir == 0    self.shadow_target = nil    end   # reached rest, clear target
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
    var pos = self.current_position_enum()
    var label = "Unknown"
    if   pos == self.CP_CLOSED    label = "Closed"
    elif pos == self.CP_OPENED    label = "Open"
    elif pos == self.CP_PARTIAL   label = "Partial"
    end
    var moving = (self.shadow_shutter_direction != nil) && (self.shadow_shutter_direction != 0)
    webserver.content_send(format("&#x1F6AA; %s%s", label, moving ? " (moving)" : ""))
  end

end
matter.Plugin_GarageDoor = Matter_Plugin_GarageDoor
