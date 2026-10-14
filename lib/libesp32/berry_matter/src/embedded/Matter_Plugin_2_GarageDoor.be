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
# Matter 1.6.1 Device Specification - Closure (0x0230)
#################################################################################
# Device Type: Closure (0x0230) - introduced in Matter 1.5.0
# Device Type Revision: 1 (unchanged in Matter 1.6.1 Device Library)
# Class: Simple | Scope: Endpoint
#
# The Closure device type is the unified umbrella (Matter 1.5.0+) for window
# coverings, doors, gates, cabinets and garage doors (GDO). A garage door is a
# Closure whose motion is exposed through the Closure Control cluster.
#
# CLUSTERS (Server):
# - 0x0104: Closure Control (M) - unified movement/state interface
# - 0x0003: Identify (M) - inherited from base class
# - 0x001D: Descriptor (M) - TagList with exactly one Closure namespace tag
#
# DESCRIPTOR TAGLIST:
# - A Closure SHALL carry exactly one tag from the Closure namespace (0x44).
#   This plugin exposes {MfgCode:null, NamespaceID:0x44, Tag:0x05 GarageDoor}
#   and sets the Descriptor TAGLIST feature (bit 0).
#
# NOTES:
# - This plugin drives a position-aware Tasmota Shutter (relay based garage
#   door opener) and reports fully-open / fully-closed / partially-open state.
# - Finer-grained percentage positioning would be modelled by a companion
#   Closure Panel (0x0231) endpoint with the Closure Dimension cluster (0x0105);
#   not implemented here (garage doors are typically enum-position only).
#################################################################################

#################################################################################
# Matter 1.6.1 Closure Control Cluster (0x0104)
#################################################################################
# Cluster Revision: 1 (initial revision in Matter 1.5.0, unchanged in 1.6.1)
# Role: Application | Scope: Endpoint | PICS: CLCTRL
#
# FEATURES (this plugin):
# - Bit 0 (PS): Positioning - FEATURE_MAPS[0x0104] = 0x01 in Matter_Plugin_0
#   No LT, IS, SP, VT, PD, CL, PT, MO.
#
# ATTRIBUTES:
# ID     | Name                 | Type                      | Quality | Conf
# -------|----------------------|---------------------------|---------|-----
# 0x0000 | CountdownTime        | elapsed_s (uint32)        | X Q     | [PS&!IS]
# 0x0001 | MainState            | MainStateEnum (enum8)     |         | M
# 0x0002 | CurrentErrorList     | list[ClosureErrorEnum]    |         | M
# 0x0003 | OverallCurrentState  | OverallCurrentStateStruct | X       | M
# 0x0004 | OverallTargetState   | OverallTargetStateStruct  | X       | M
#
# MainStateEnum(enum8): Stopped=0, Moving=1, WaitingForMotion=2, Error=3,
#                       Calibrating=4, Protected=5, Disengaged=6, SetupRequired=7
# CurrentPositionEnum(enum8): FullyClosed=0, FullyOpened=1, PartiallyOpened=2,
#                       OpenedForPedestrian=3(PD), OpenedForVentilation=4(VT), OpenedAtSignature=5(M)
# TargetPositionEnum(enum8): MoveToFullyClosed=0, MoveToFullyOpen=1,
#                       MoveToPedestrianPosition=2(PD), MoveToVentilationPosition=3(VT),
#                       MoveToSignaturePosition=4(M)
#
# OverallCurrentStateStruct: 0:Position(CurrentPositionEnum,X,PS) 1:Latch(bool,X,LT)
#                            2:Speed(ThreeLevelAutoEnum,SP) 3:SecureState(bool,X,M)
# OverallTargetStateStruct : 0:Position(TargetPositionEnum,X,PS) 1:Latch(bool,X,LT)
#                            2:Speed(ThreeLevelAutoEnum,SP)
#
# COMMANDS:
# ID   | Name      | Dir  | Access | Conf | Notes
# -----|-----------|------|--------|------|---------------------------
# 0x00 | Stop      | C→S  | O      | !IS  | halt motion
# 0x01 | MoveTo    | C→S  | O T    | M    | timed invoke enforced in Matter_IM
#
# MoveTo: {Position:TargetPositionEnum, Latch:bool, Speed:ThreeLevelAutoEnum} (O.a+)
# - no field at all            -> INVALID_COMMAND
# - only Latch/Speed (no LT/SP) -> SUCCESS, no action
# - Position 2/3 (no PD/VT) or out of range -> CONSTRAINT_ERROR
#
# EVENTS:
# ID   | Name               | Priority | Conf | Emitted
# -----|--------------------|----------|------|-------------------------------
# 0x00 | OperationalError   | CRIT     | M    | never (no error source in Tasmota)
# 0x01 | MovementCompleted  | INFO     | !IS  | Moving -> Stopped
# 0x03 | SecureStateChanged | INFO     | M    | SecureState true <-> false
#
# TASMOTA IMPLEMENTATION:
# - ARG: "shutter" - Tasmota Shutter number (zero based)
# - Reads Position/Direction/Target from `ShutterPosition<x>`; honors
#   ShutterInvert (right-most char of `Status 13` SHT<x>.Opt)
# - MoveTo FullyOpen / Signature -> ShutterOpen<n>  (Signature = fully open)
# - MoveTo FullyClosed           -> ShutterClose<n>
# - Stop                         -> ShutterStop<n>
# - Position: Tasmota closed% (0=open .. 100=closed) -> Closed/Partial/Open enum
# - Target: derived from Tasmota `Target` (reflects local moves too)
# - SecureState: true when FullyClosed (spec formula for PS without LT)
#
# LIMITATION - SecureState is NOT a confirmed closed state:
# - Tasmota time-based shutters have no end-stop sensor: the position is
#   estimated from relay run time, not measured.
# - Moves made outside Tasmota are invisible: opener obstruction reversal
#   (photo-eye), car remote, wall button, or a pulse ignored by the opener.
#   Tasmota may then report FullyClosed / SecureState=true (and emit
#   SecureStateChanged) while the door is actually open.
# - Controllers and automations must not rely on SecureState as a security
#   guarantee for this plugin.
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
    0x001D: [4],                                    # Descriptor: add TagList to inherited [0,1,2,3]
    # 0x0003: inherited                             # Identify 1.2 p.16
    0x0104: [0,1,2,3,4],                            # Closure Control (Matter 1.5.0+)
  })
  static var TYPES = { 0x0230: 1 }                  # Closure - Matter 1.6.1 Device Library Rev 1

  # MainStateEnum
  static var MS_STOPPED = 0
  static var MS_MOVING  = 1
  # CurrentPositionEnum
  static var CP_CLOSED    = 0
  static var CP_OPENED    = 1
  static var CP_PARTIAL   = 2
  static var CP_SIGNATURE = 5
  # TargetPositionEnum
  static var TP_CLOSE     = 0
  static var TP_OPEN      = 1
  static var TP_SIGNATURE = 4

  var tasmota_shutter_index                         # Shutter number in Tasmota (zero based)
  var shadow_shutter_pos                            # last known position 0..100 (Tasmota convention)
  var shadow_shutter_target                         # last known target 0..100 (Tasmota convention)
  var shadow_shutter_direction                      # 1=opening -1=closing 0=not moving
  var shadow_shutter_inverted                       # 1=matter convention 0=must invert, -1=unknown
  var shadow_signature                              # true if the last Matter MoveTo was MoveToSignaturePosition

  #############################################################
  # parse_configuration
  #
  def parse_configuration(config)
    super(self).parse_configuration(config)
    self.tasmota_shutter_index = config.find('shutter')
    if self.tasmota_shutter_index == nil     self.tasmota_shutter_index = 0   end
    if self.VIRTUAL
      # Virtual garage doors use the native Tasmota convention:
      # 0 = fully closed, 100 = fully open.
      self.shadow_shutter_pos = 0
      self.shadow_shutter_target = 0
      self.shadow_shutter_direction = 0
      self.shadow_shutter_inverted = 0
    else
      self.shadow_shutter_inverted = -1
    end
    self.shadow_signature = false
  end

  #############################################################
  # Update "inverted" flag from `Status 13`
  #
  # `Status 13` returns nothing when SetOption80 is off: keep the flag
  # unknown (-1), positions are then reported as unknown (null).
  def update_inverted()
    var r_st13 = tasmota.cmd("Status 13", true)     # issue `Status 13`
    if isinstance(r_st13, map)
      var sht = r_st13.find('StatusSHT')
      if isinstance(sht, map)
        var d = sht.find("SHT"+str(self.tasmota_shutter_index))
        if isinstance(d, map)
          d = d.find('Opt')
          if type(d) == 'string' && size(d) > 0
            self.shadow_shutter_inverted = (d[size(d)-1] == '1') ? 1 : 0  # inverted is the right-most character
            return true
          end
        end
      end
    end
    return false
  end

  #############################################################
  # Update shadow from Tasmota `ShutterPosition`
  #
  def update_shadow()
    if !self.VIRTUAL && !self.BRIDGE
      var prev = self.state_snapshot()
      var old_inverted = self.shadow_shutter_inverted
      self.update_inverted()
      var sp = tasmota.cmd("ShutterPosition" + str(self.tasmota_shutter_index + 1), true)
      # `sp` can be nil, `{"ShutterPosition<n>":"Locked"}` (ShutterLock) or
      # another shutter's report: only a payload actually applied by
      # update_sensors refreshes the position.
      if !self.update_sensors(sp) && (old_inverted != self.shadow_shutter_inverted)
        # Never reinterpret a cached position using a new inversion setting.
        self.shadow_shutter_pos = nil
        self.shadow_shutter_target = nil
      end
      self.report_state_changes(prev)
    end
    super(self).update_shadow()
  end

  #############################################################
  # to_closed_pct
  #
  # Converts a Tasmota position (0..100) to closed% (100 = fully closed,
  # 0 = fully open), honoring ShutterInvert. Returns nil if the value or the
  # inversion flag is unknown: guessing could report an open door as closed.
  def to_closed_pct(v)
    if v == nil || self.shadow_shutter_inverted == nil || self.shadow_shutter_inverted < 0
      return nil
    end
    return (self.shadow_shutter_inverted == 0) ? 100 - v : v
  end

  #############################################################
  # is_moving
  #
  def is_moving()
    return (self.shadow_shutter_direction != nil) && (self.shadow_shutter_direction != 0)
  end

  #############################################################
  # current_position_enum -> CurrentPositionEnum or nil
  #
  # Signature position is fully open: reported as OpenedAtSignature when
  # reached through MoveToSignaturePosition.
  def current_position_enum()
    var cp = self.to_closed_pct(self.shadow_shutter_pos)
    if cp == nil            return nil               end
    if cp >= 100            return self.CP_CLOSED    end
    if cp <= 0              return self.shadow_signature ? self.CP_SIGNATURE : self.CP_OPENED   end
    return self.CP_PARTIAL
  end

  #############################################################
  # target_position_enum -> TargetPositionEnum or nil
  #
  # Derived from the Tasmota target so that local moves (buttons, rules)
  # are reflected too. A partial target (e.g. after Stop) is not
  # representable by TargetPositionEnum and is reported as null.
  def target_position_enum()
    var tc = self.to_closed_pct(self.shadow_shutter_target)
    if tc == nil            return nil               end
    if tc >= 100            return self.TP_CLOSE     end
    if tc <= 0              return self.shadow_signature ? self.TP_SIGNATURE : self.TP_OPEN   end
    return nil
  end

  #############################################################
  # set_signature
  #
  # Change the signature flag and report affected attributes
  def set_signature(b)
    if b != self.shadow_signature
      self.shadow_signature = b
      self.attribute_updated(0x0104, 0x0003)          # OverallCurrentState
      self.attribute_updated(0x0104, 0x0004)          # OverallTargetState
    end
  end

  #############################################################
  # state_snapshot / report_state_changes
  #
  # Compare derived Matter state before/after a shadow update, report
  # changed attributes and emit the mandatory events.
  def state_snapshot()
    return [self.current_position_enum(), self.is_moving(), self.target_position_enum()]
  end

  def report_state_changes(prev)
    var pos = self.current_position_enum()
    var moving = self.is_moving()
    var target = self.target_position_enum()
    var prev_pos = prev[0]
    if pos != prev_pos
      self.attribute_updated(0x0104, 0x0003)          # OverallCurrentState
      # SecureStateChanged, only between known states (not at startup)
      if (prev_pos != nil) && (pos != nil) && ((prev_pos == self.CP_CLOSED) != (pos == self.CP_CLOSED))
        self.publish_event(0x0104, 0x03, 1 #-matter.EVENT_INFO-#, matter.TLV.Matter_TLV_item().set(0x08 #-TLV.BOOL-#, pos == self.CP_CLOSED))
      end
    end
    if moving != prev[1]
      self.attribute_updated(0x0104, 0x0001)          # MainState
      if !moving                                      # Moving -> Stopped
        self.publish_event(0x0104, 0x01, 1 #-matter.EVENT_INFO-#)   # MovementCompleted, no field
      end
    end
    if target != prev[2]
      self.attribute_updated(0x0104, 0x0004)          # OverallTargetState
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
    if   cluster == 0x0104              # ========== Closure Control (Matter 1.5.0+) ==========
      self.update_shadow_lazy()
      if   attribute == 0x0000          #  ---------- CountdownTime / elapsed_s ----------
        return tlv_solo.set(0x14 #-TLV.NULL-#, nil)   # not supported -> null
      elif attribute == 0x0001          #  ---------- MainState / MainStateEnum ----------
        return tlv_solo.set(0x06 #-TLV.U4-#, self.is_moving() ? self.MS_MOVING : self.MS_STOPPED)
      elif attribute == 0x0002          #  ---------- CurrentErrorList / list ----------
        return TLV.Matter_TLV_array()   # no errors
      elif attribute == 0x0003          #  ---------- OverallCurrentState / struct ----------
        var pos = self.current_position_enum()
        if pos == nil
          return tlv_solo.set(0x14 #-TLV.NULL-#, nil)
        end
        var s = TLV.Matter_TLV_struct()
        s.add_TLV(0, 0x06 #-TLV.U4-#, pos)                       # Position (CurrentPositionEnum)
        # SecureState: fully closed per Tasmota's estimated position, not a
        # sensor-confirmed state (see LIMITATION in the header)
        s.add_TLV(3, 0x08 #-TLV.BOOL-#, pos == self.CP_CLOSED)
        return s
      elif attribute == 0x0004          #  ---------- OverallTargetState / struct ----------
        var target = self.target_position_enum()
        if target == nil
          return tlv_solo.set(0x14 #-TLV.NULL-#, nil)
        end
        var s = TLV.Matter_TLV_struct()
        s.add_TLV(0, 0x06 #-TLV.U4-#, target)                    # Position (TargetPositionEnum)
        return s
      elif attribute == 0xFFF9          #  ---------- AcceptedCommandList ----------
        var al = TLV.Matter_TLV_array()
        al.add_TLV(nil, 0x06 #-TLV.U4-#, 0x0000)    # Stop
        al.add_TLV(nil, 0x06 #-TLV.U4-#, 0x0001)    # MoveTo
        return al
      end

    # ====================================================================================================
    elif cluster == 0x001D              # ========== Descriptor Cluster 9.5 p.453 ==========
      if   attribute == 0x0004          #  ---------- TagList / list[SemanticTagStruct] ----------
        var tl = TLV.Matter_TLV_array()
        var t = tl.add_struct()
        t.add_TLV(0, 0x14 #-TLV.NULL-#, nil)        # MfgCode: null (standard namespace)
        t.add_TLV(1, 0x06 #-TLV.U4-#, 0x44)         # NamespaceID: Closure
        t.add_TLV(2, 0x06 #-TLV.U4-#, 0x05)         # Tag: GarageDoor
        return tl
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return tlv_solo.set(0x06 #-TLV.U4-#, 0x01)  # TAGLIST (bit 0)
      end

    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  #############################################################
  # run_shutter_command
  #
  # Execute a Tasmota shutter command and translate command rejection into
  # an Interaction Model status. In particular, ShutterLock answers "Locked"
  # without moving the shutter and must not be exposed as Matter SUCCESS.
  def run_shutter_command(command, signature, ctx)
    var was_moving = self.is_moving()
    var response = tasmota.cmd(command, true)
    if !isinstance(response, map)
      ctx.status = 0x01 #-matter.FAILURE-#
      ctx.log = command + ":NoResponse"
      return false
    end

    var result = response.find(command)
    if result == "Locked"
      ctx.status = 0xCB #-matter.INVALID_IN_STATE-#
      ctx.log = command + ":Locked"
      return false
    end

    # Moving commands return a Shutter<n> state object. Stop returns either
    # `{"ShutterStop":"Done"}` (no index) when the shutter was idle, or the
    # last position report, possibly of another shutter that stopped at the
    # same time. Only an unknown/invalid command (`{"Command":...}`) fails.
    var idx1 = str(self.tasmota_shutter_index + 1)
    var shutter = response.find("Shutter" + idx1)
    var stopped = (command == "ShutterStop" + idx1) && !response.contains("Command")
    if !stopped && !isinstance(shutter, map)
      ctx.status = 0x01 #-matter.FAILURE-#
      ctx.log = command + ":Failed"
      return false
    end

    # A Stop on an already-idle shutter is a no-op and must not change an
    # OpenedAtSignature state into an ordinary FullyOpened state.
    if !stopped || was_moving
      self.set_signature(signature)
    end
    if isinstance(shutter, map)
      self.parse_sensors(response)
    end
    self.update_shadow()
    return true
  end

  #############################################################
  # run_virtual_move
  #
  # Virtual moves complete immediately, but expose a Moving -> Stopped
  # transition so the mandatory MovementCompleted event is generated.
  # Position values use the Tasmota convention (0 closed, 100 open).
  def run_virtual_move(target, signature)
    var destination = (target == self.TP_CLOSE) ? 0 : 100
    var prev = self.state_snapshot()
    var current = self.shadow_shutter_pos

    self.shadow_shutter_target = destination
    self.shadow_signature = signature
    if (current != nil) && (current != destination)
      self.shadow_shutter_direction = (destination > current) ? 1 : -1
      self.report_state_changes(prev)
      prev = self.state_snapshot()
      self.shadow_shutter_pos = destination
      self.shadow_shutter_direction = 0
      self.report_state_changes(prev)
    else
      self.shadow_shutter_pos = destination
      self.shadow_shutter_direction = 0
      self.report_state_changes(prev)
    end

    self.publish_command('ShutterPos', self.shadow_shutter_pos,
                         'ShutterTarget', self.shadow_shutter_target,
                         'ShutterDirection', self.shadow_shutter_direction)
    return true
  end

  #############################################################
  # stop_virtual
  #
  # Stop preserves an idle signature position, matching the physical
  # shutter behavior. During motion the current position becomes the target.
  def stop_virtual()
    if self.is_moving()
      var prev = self.state_snapshot()
      self.shadow_shutter_direction = 0
      self.shadow_shutter_target = self.shadow_shutter_pos
      self.shadow_signature = false
      self.report_state_changes(prev)
    end
    self.publish_command('ShutterPos', self.shadow_shutter_pos,
                         'ShutterTarget', self.shadow_shutter_target,
                         'ShutterDirection', self.shadow_shutter_direction)
    return true
  end

  #############################################################
  # Invoke a command
  #
  # returns `true` for SUCCESS, or `nil` with `ctx.status` set for an error
  def invoke_request(session, val, ctx)
    var cluster = ctx.cluster
    var command = ctx.command

    # ====================================================================================================
    if   cluster == 0x0104              # ========== Closure Control (Matter 1.5.0+) ==========
      var idx1 = str(self.tasmota_shutter_index + 1)    # Tasmota shutter number is 1 based
      self.update_shadow_lazy()
      if   command == 0x0000            # ---------- Stop ----------
        if self.VIRTUAL
          return self.stop_virtual()
        end
        return self.run_shutter_command("ShutterStop" + idx1, false, ctx) ? true : nil
      elif command == 0x0001            # ---------- MoveTo ----------
        # O.a+ : at least one field must be present
        if (val == nil) || !val.is_struct
          ctx.status = 0x85 #-matter.INVALID_COMMAND-#
          return nil
        end
        if (val.findsub(0) == nil) && (val.findsub(1) == nil) && (val.findsub(2) == nil)
          ctx.status = 0x85 #-matter.INVALID_COMMAND-#
          return nil
        end
        var pos_item = val.findsub(0)
        if pos_item == nil
          ctx.log = "no-op"                             # only Latch/Speed: LT/SP not supported, nothing to do
          return true
        end
        var target = pos_item.val                       # Position (TargetPositionEnum), not nullable
        if type(target) != 'int'
          ctx.status = 0x85 #-matter.INVALID_COMMAND-#
          return nil
        end
        if   target == self.TP_CLOSE
          ctx.log = "MoveTo:Close"
          if self.VIRTUAL   return self.run_virtual_move(target, false)   end
          return self.run_shutter_command("ShutterClose" + idx1, false, ctx) ? true : nil
        elif target == self.TP_OPEN
          ctx.log = "MoveTo:Open"
          if self.VIRTUAL   return self.run_virtual_move(target, false)   end
          return self.run_shutter_command("ShutterOpen" + idx1, false, ctx) ? true : nil
        elif target == self.TP_SIGNATURE
          ctx.log = "MoveTo:Signature"
          if self.VIRTUAL   return self.run_virtual_move(target, true)   end
          return self.run_shutter_command("ShutterOpen" + idx1, true, ctx) ? true : nil
        else
          ctx.status = 0x87 #-matter.CONSTRAINT_ERROR-#   # Pedestrian/Ventilation (no PD/VT) or out of range
          return nil
        end
      end

    else
      return super(self).invoke_request(session, val, ctx)
    end
  end

  #############################################################
  # update_sensors
  #
  # Apply the output from `ShutterPosition` or teleperiod without reporting.
  # This lets update_shadow change ShutterInvert and position atomically.
  def update_sensors(payload)
    if !isinstance(payload, map)   return false   end
    var v = payload.find("Shutter" + str(self.tasmota_shutter_index + 1))
    if !isinstance(v, map)         return false   end
    var val = v.find("Position")
    if type(val) == 'int'    self.shadow_shutter_pos = val         end
    val = v.find("Direction")
    if type(val) == 'int'    self.shadow_shutter_direction = val   end
    val = v.find("Target")
    if type(val) == 'int'    self.shadow_shutter_target = val      end
    # signature applies only while the door is heading to / resting at fully open
    if self.shadow_signature
      var tc = self.to_closed_pct(self.shadow_shutter_target)
      if (tc != nil) && (tc > 0)
        self.shadow_signature = false
      end
    end
    return true
  end

  #############################################################
  # parse the output from `ShutterPosition` or teleperiod and report changes
  # Ex: `{"Shutter1":{"Position":50,"Direction":0,"Target":50,"Tilt":30}}`
  #
  # Only applies to local devices: `read_sensors()` is dispatched to every
  # plugin and contains `Shutter<n>` for each local shutter, which must not
  # overwrite virtual (`update_virtual()`) or bridged state.
  def parse_sensors(payload)
    if self.VIRTUAL || self.BRIDGE   return   end
    var prev = self.state_snapshot()
    if self.update_sensors(payload)
      self.report_state_changes(prev)
    end
  end

  #############################################################
  # update_virtual
  #
  # MtrUpdate fields use the same convention as Tasmota shutters:
  #   ShutterPos:       0 closed .. 100 open
  #   ShutterTarget:    0 closed .. 100 open
  #   ShutterDirection: -1 closing, 0 stopped, 1 opening
  def update_virtual(payload)
    var prev = self.state_snapshot()
    var val = payload.find("ShutterPos")
    if val != nil
      val = int(val)
      if val != nil
        if val < 0     val = 0     end
        if val > 100   val = 100   end
        self.shadow_shutter_pos = val
      end
    end

    val = payload.find("ShutterTarget")
    if val != nil
      val = int(val)
      if val != nil
        if val < 0     val = 0     end
        if val > 100   val = 100   end
        self.shadow_shutter_target = val
        if val != 100
          self.shadow_signature = false
        end
      end
    end

    val = payload.find("ShutterDirection")
    if val != nil
      val = int(val)
      if val != nil
        self.shadow_shutter_direction = (val < 0) ? -1 : ((val > 0) ? 1 : 0)
      end
    end

    self.report_state_changes(prev)
    super(self).update_virtual(payload)
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
    if   pos == self.CP_CLOSED                              label = "Closed"
    elif pos == self.CP_OPENED || pos == self.CP_SIGNATURE  label = "Open"
    elif pos == self.CP_PARTIAL                             label = "Partial"
    end
    webserver.content_send(format("&#x1F6AA; %s%s", label, self.is_moving() ? " (moving)" : ""))
  end

end
matter.Plugin_GarageDoor = Matter_Plugin_GarageDoor
