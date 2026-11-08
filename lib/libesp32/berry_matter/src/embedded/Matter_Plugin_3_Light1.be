#
# Matter_Plugin_Light1.be - implements the behavior for a Light with 1 channel (Dimmer)
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

#################################################################################
# Matter 1.4.1 Device Specification
#################################################################################
# Device Type: Dimmable Light (0x0101)
# Device Type Revision: 3 (Matter 1.4.1)
# Class: Simple | Scope: Endpoint
# Superset: On/Off Light (0x0100)
#
# CLUSTERS (Server):
# - 0x0003: Identify (M)
# - 0x0004: Groups (M)
# - 0x0062: Scenes Management (P, M)
# - 0x0006: On/Off (M)
# - 0x0008: Level Control (M)
# - 0x0406: Occupancy Sensing (C, O)
#
# ELEMENT OVERRIDES:
# - Identify: TriggerEffect cmd M
# - Scenes Management: CopyScene cmd P, M
# - On/Off: Lighting feature M
# - Level Control: OnOff feature M, Lighting feature M, CurrentLevel 1-254, MinLevel 1, MaxLevel 254
#
# NOTES:
# - Extends On/Off Light with dimming capability
# - Level Control cluster is mandatory for this device type
# - CurrentLevel range is 1-254 (0 and 255 are not allowed for lights)
#################################################################################

#################################################################################
# Matter 1.4.1 Level Control Cluster (0x0008)
#################################################################################
# Cluster Revision: 6 (Matter 1.4.1)
# Role: Application | Scope: Endpoint
#
# PURPOSE:
# Provides brightness/level control for dimmable lights and other devices
# with adjustable output levels. Supports smooth transitions and on/off
# integration.
#
# FEATURES:
# Bit | Code | Feature      | Conf | Summary
# ----|------|--------------|------|---------------------------
# 0   | OO   | OnOff        | M    | Dependency on On/Off
# 1   | LT   | Lighting     | O    | Lighting behavior
# 2   | FQ   | Frequency    | P    | Frequency control
#
# ATTRIBUTES:
# ID     | Name                  | Type         | Constraint      | Quality | Default | Access | Conf
# -------|-----------------------|--------------|-----------------|---------|---------|--------|-----
# 0x0000 | CurrentLevel          | uint8        | 1-254           | XQN     | null    | R V    | M
# 0x0001 | RemainingTime         | uint16       | all             | Q       | 0       | R V    | LT
# 0x0002 | MinLevel              | uint8        | 1-254           |         | 1/0     | R V    | O
# 0x0003 | MaxLevel              | uint8        | MinLevel-254    |         | 254     | R V    | O
# 0x0004 | CurrentFrequency      | uint16       | MinFreq-MaxFreq | QP      | 0       | R V    | FQ
# 0x0005 | MinFrequency          | uint16       | 0-MaxFreq       | F       | 0       | R V    | FQ
# 0x0006 | MaxFrequency          | uint16       | MinFreq-65535   | F       | 0       | R V    | FQ
# 0x000F | Options               | OptionsBitmap| all             |         | 0       | RW VO  | M
# 0x0010 | OnOffTransitionTime   | uint16       | all             |         | 0       | RW VO  | O
# 0x0011 | OnLevel               | uint8        | MinLevel-MaxLevel| XN     | null    | RW VO  | M
# 0x0012 | OnTransitionTime      | uint16       | all             | XN      | null    | RW VO  | O
# 0x0013 | OffTransitionTime     | uint16       | all             | XN      | null    | RW VO  | O
# 0x0014 | DefaultMoveRate       | uint8        | all             | XN      | null    | RW VO  | O
# 0x4000 | StartUpCurrentLevel   | uint8        | 1-254           | XN      | MS      | RW VM  | LT
# 0xFFFC | FeatureMap            | map32        | all             | F       | 0       | R V    | M
# 0xFFFD | ClusterRevision       | uint16       | all             | F       | 6       | R V    | M
#
# DATA TYPES:
# - OptionsBitmap: ExecuteIfOff=Bit0, CoupleColorTempToLevel=Bit1
# - MoveModeEnum: Up=0, Down=1
# - StepModeEnum: Up=0, Down=1
#
# COMMANDS:
# ID   | Name                  | Dir  | Response | Access | Conf
# -----|----------------------|------|----------|--------|-----
# 0x00 | MoveToLevel          | C→S  | Y        | O      | M
# 0x01 | Move                 | C→S  | Y        | O      | M
# 0x02 | Step                 | C→S  | Y        | O      | M
# 0x03 | Stop                 | C→S  | Y        | O      | M
# 0x04 | MoveToLevelWithOnOff | C→S  | Y        | O      | M
# 0x05 | MoveWithOnOff        | C→S  | Y        | O      | M
# 0x06 | StepWithOnOff        | C→S  | Y        | O      | M
# 0x07 | StopWithOnOff        | C→S  | Y        | O      | M
# 0x08 | MoveToClosestFrequency| C→S | Y        | O      | FQ
#
# MoveToLevel: {Level:uint8(0-254), TransitionTime:uint16|X, OptionsMask:OptionsBitmap, OptionsOverride:OptionsBitmap}
# Move: {MoveMode:MoveModeEnum, Rate:uint8|X, OptionsMask:OptionsBitmap, OptionsOverride:OptionsBitmap}
# Step: {StepMode:StepModeEnum, StepSize:uint8, TransitionTime:uint16|X, OptionsMask:OptionsBitmap, OptionsOverride:OptionsBitmap}
# Stop: {OptionsMask:OptionsBitmap, OptionsOverride:OptionsBitmap}
# MoveToLevelWithOnOff: Same as MoveToLevel (also controls On/Off)
# MoveWithOnOff: Same as Move (also controls On/Off)
# StepWithOnOff: Same as Step (also controls On/Off)
# StopWithOnOff: Same as Stop (also controls On/Off)
# MoveToClosestFrequency: {Frequency:uint16}
#
# QUALITY FLAGS:
# - X: Nullable
# - Q: Quieter reporting (less frequent updates)
# - N: NonVolatile (persists across power cycles)
# - P: Periodic reporting
#
# IMPLEMENTATION NOTES:
# - CurrentLevel: 0=Off, 1-254=brightness levels, 255 not allowed for lights
# - For Tasmota: Maps to Dimmer (0-100%) scaled to 0-254
# - OnLevel: Level to use when turning on (null = use previous level)
# - MinLevel/MaxLevel: Constrain the usable range (default 1-254)
# - OnOff feature (bit 0) is mandatory - level changes can affect on/off state
# - Lighting feature (bit 1) adds StartUpCurrentLevel and RemainingTime
# - WithOnOff commands: Automatically turn on when increasing level from 0
# - TransitionTime: In 1/10th second units (null = use default)
#################################################################################

#################################################################################
# Matter 1.6.1 Color Control Cluster (0x0300) - shared server for Light2/3/5
#################################################################################
# Light1 hosts the Color Control server shared by Light2 (0x010C), Light3 and
# Light5 (0x010D), gated by the static `CC_FEAT`. With `CC_FEAT == 0` (Light1)
# every Color Control path is dormant: Light1 clusters, attributes,
# AcceptedCommandList and behaviour are unchanged.
#
# CC_FEAT = FeatureMap = ColorCapabilities:
# - HS 0x01 (HueSaturation), XY 0x08, CT 0x10 (ColorTemperature)
# - Light2 0x10 (CT), Light3 0x09 (HS | XY), Light5 0x19 (HS | XY | CT)
#
# 1.6.1 facts (ColorControl.xml, cluster revision 9, revision 6 kept):
# - 0x010D: XY M, CT M, RemainingTime 0x0002 M; 0x010C: CT M, RemainingTime M
# - Options 0x000F RW, volatile, only bit 0 (ExecuteIfOff) defined
# - StartUpColorTemperatureMireds 0x4010 RW, nullable, 1..0xFEFF (CT)
# - CoupleColorTempToLevelMinMireds 0x400D M with CT (= physical min here)
# - OptionsMask/OptionsOverride are always the last two command fields
#
# Timing (no transition engine):
# - MoveTo* and Step* are applied instantly, TransitionTime is validated and ignored
# - Move* and StopMoveStep are validated, then accepted as no-ops
# - RemainingTime 0x0002 is always 0
# - ExecuteIfOff: when off, commands are executed only if the effective
#   Options bit 0 is set ((Options & ~Mask) | (Override & Mask))
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Light1,weak

class Matter_Plugin_Light1 : Matter_Plugin_Light0
  static var TYPE = "light1"                                # name of the plug-in in json
  static var DISPLAY_NAME = "Light 1 Dimmer"                # display name of the plug-in

  static var SCHEMA = "light|"                      # arg name
                      "l:Light number (opt)|"       # label (display name)
                      "t:i|"                        # type: int
                      "h:(opt) Light number"        # hint
  # static var UPDATE_TIME = 250                      # update every 250ms
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                                     # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                                     # Identify 1.2 p.16
    # 0x0004: inherited                                     # Groups 1.3 p.21
    # 0x0062: inherited                                     # Scenes Management 1.4 (PROVISIONAL) - replaces 0x0005
    # 0x0006: [0],                                    # On/Off 1.5 p.48
    0x0008: [0,2,3,0x0F,0x11],                      # Level Control 1.6 p.57
  })
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "Bri")
  static var TYPES = { 0x0101: 3 }                  # Dimmable Light - Matter 1.4.1 Device Library Rev 3

  static var CC_FEAT = 0                            # Color Control FeatureMap/ColorCapabilities: 0 = none (Light1), HS 0x01, XY 0x08, CT 0x10
  # Color Control command fields, one char per field tag 0..n-1; uppercase = optional (absent or NULL -> 0)
  # h 0..254, d enum 0..3, b/B uint8, T 0..0xFFFE (optional), w uint16, x/X 0..0xFEFF, s int16, M map8
  # OptionsMask/OptionsOverride are always the last 2 fields
  static var CC_ARGS = {
    0x00: "hdTMM",    # MoveToHue: Hue, Direction, TransitionTime
    0x01: "dbMM",     # MoveHue: MoveMode, Rate
    0x02: "dbBMM",    # StepHue: StepMode, StepSize, TransitionTime(uint8)
    0x03: "hTMM",     # MoveToSaturation: Saturation, TransitionTime
    0x04: "dbMM",     # MoveSaturation: MoveMode, Rate
    0x05: "dbBMM",    # StepSaturation: StepMode, StepSize, TransitionTime(uint8)
    0x06: "hhTMM",    # MoveToHueAndSaturation: Hue, Saturation, TransitionTime
    0x07: "xxTMM",    # MoveToColor: ColorX, ColorY, TransitionTime
    0x08: "ssMM",     # MoveColor: RateX, RateY
    0x09: "ssTMM",    # StepColor: StepX, StepY, TransitionTime
    0x0A: "xTMM",     # MoveToColorTemperature: ColorTemperatureMireds, TransitionTime
    0x47: "MM",       # StopMoveStep
    0x4B: "dwXXMM",   # MoveColorTemperature: MoveMode, Rate, CTMinimumMireds, CTMaximumMireds
    0x4C: "dwTXXMM",  # StepColorTemperature: StepMode, StepSize, TransitionTime, CTMinimumMireds, CTMaximumMireds
  }
  static var CC_LIM = {'h':254, 'd':3, 'b':255, 'B':255, 'T':0xFFFE, 'w':0xFFFF, 'x':0xFEFF, 'X':0xFEFF, 's':0x7FFF, 'M':255}

  # Inherited
  # var device                                        # reference to the `device` global object
  # var endpoint                                      # current endpoint
  # var clusters                                      # map from cluster to list of attributes, typically constructed from CLUSTERS hierachy
  # var tick                                          # tick value when it was last updated
  # var node_label                                    # name of the endpoint, used only in bridge mode, "" if none
  # var tasmota_relay_index                             # Relay number in Tasmota (1 based), nil for internal light
  # var shadow_onoff                                  # (bool) status of the light power on/off
  # var light_index                                   # index number when using `light.get()` and `light.set()`
  var shadow_bri                                    # (int 0..254) brightness before Gamma correction - as per Matter 255 is not allowed
  # Color Control (initialized only by `cc_init()`, stay `nil` on Light1)
  var shadow_ct                                     # (int) ColorTemperatureMireds, CT plugins only
  var ct_min, ct_max                                # physical CT range: 153..500, or 200..380 with SetOption82
  var ct_startup                                    # StartUpColorTemperatureMireds (int 1..0xFEFF) or nil
  var shadow_color_mode                             # ColorMode: 0 HS, 1 XY, 2 CT
  var cc_options                                    # Color Control Options (map8, only bit 0, volatile)

  #############################################################
  # Constructor
  def init(device, endpoint, arguments)
    self.shadow_bri = 0
    super(self).init(device, endpoint, arguments)
  end

  #############################################################
  # parse_configuration
  #
  # Parse configuration map
  def parse_configuration(config)
    super(self).parse_configuration(config)
    # with Light0 we always need relay number but we don't for Light1/2/3 so self.tasmota_relay_index may be `nil`
    if self.BRIDGE
      self.tasmota_relay_index = int(config.find('relay', nil))
      if (self.tasmota_relay_index != nil && self.tasmota_relay_index <= 0)    self.tasmota_relay_index = 1    end
    else
      if (self.tasmota_relay_index == nil) && (self.TYPE == "light1")   # only if `light1` and not for subclasses
        var light_index_arg = config.find('light')
        if (light_index_arg == nil)
          if (tasmota.get_option(68) == 0)    # if default mode, and `SO68 0`, check if we have split RGB/W
            import light
            if (light.get(1) != nil)
              self.light_index = 1                        # default value is `0` from superclass
            end
          end
        else
          self.light_index = int(light_index_arg) - 1     # internal is 0-based
        end
      end
    end
  end

  #############################################################
  # Update shadow
  #
  def update_shadow()
    if !self.VIRTUAL && !self.BRIDGE
      import light
      var light_status = light.get(self.light_index)
      if light_status != nil
        var pow = light_status.find('power', nil)
        if pow != self.shadow_onoff
          self.attribute_updated(0x0006, 0x0000)
          self.shadow_onoff = pow
        end
        var bri = light_status.find('bri', nil)
        if bri != nil
          bri = tasmota.scale_uint(bri, 0, 255, 0, 254)
          if bri != self.shadow_bri
            self.attribute_updated(0x0008, 0x0000)
            self.shadow_bri = bri
          end
        end
        if self.CC_FEAT   self.cc_update(light_status)   end    # Color Control shadows, same `light.get()` map
      end
    end
    super(self).update_shadow()     # superclass manages 'power'
  end

  #############################################################
  # Set Bri
  #
  # `bri` is in range 0.255 and not 0..254 like in Matter
  # `pow` can be bool on `nil` if no change
  def set_bri(bri_254, pow)
    if (bri_254 < 0)    bri_254 = 0     end
    if (bri_254 > 254)  bri_254 = 254   end
    pow = (pow != nil) ? bool(pow) : nil        # nil or bool
    if self.BRIDGE
      var dimmer = tasmota.scale_uint(bri_254, 0, 254, 0, 100)
      var ret = self.call_remote_sync("Dimmer", str(dimmer))
      if ret != nil
        self.parse_status(ret, 11)        # update shadow from return value
      end
    elif self.VIRTUAL
      if (pow != nil) && (pow != self.shadow_onoff)
        self.attribute_updated(0x0006, 0x0000)
        self.shadow_onoff = pow
      end
      if bri_254 != self.shadow_bri
        self.attribute_updated(0x0008, 0x0000)
        self.shadow_bri = bri_254
      end
    else
      import light
      var bri_255 = tasmota.scale_uint(bri_254, 0, 254, 0, 255)
      if pow == nil
        light.set({'bri': bri_255}, self.light_index)
      else
        light.set({'bri': bri_255, 'power': pow}, self.light_index)
      end
      self.update_shadow()
    end
  end

  #############################################################
  # Color Control (0x0300) shared server, active only when CC_FEAT != 0
  #############################################################
  #############################################################
  # cc_init
  #
  # Shared Color Control init for Light2/Light3/Light5, called at the end of their init()
  def cc_init(config)
    self.cc_options = 0
    self.shadow_color_mode = (self.CC_FEAT & 0x01) ? 0 : 2     # HS lights start in HS mode, CT-only lights in CT mode
    if self.CC_FEAT & 0x10                                      # CT
      self.shadow_ct = 325                                      # not nullable, seeded for all variants including bridge
      self.update_ct_minmax()
      var su = config.find('ct_startup')
      if type(su) == 'int' && su > 0 && su <= 0xFEFF
        self.ct_startup = su
        # applied when plugins are created at Matter start (udp_server not yet running), never on UI reconfiguration
        if !self.BRIDGE && !self.ZIGBEE && self.device.udp_server == nil
          self.set_ct(su)                                       # clamped to the physical range, CT mode
        end
      end
    end
  end

  #############################################################
  # cc_update
  #
  # Shadow sync from Light1's single `light.get()` map (local light), called by update_shadow()
  # Light3 overrides it for Hue/Sat and calls super
  def cc_update(st)
    if self.CC_FEAT & 0x10                                      # CT
      if self.CC_FEAT & 0x01                                    # HS+CT (Light5): follow Tasmota colormode of a linked RGB+CT light
        var cm = st.find('colormode')
        if cm == 'ct'                                     self.set_color_mode(2)
        elif cm == 'rgb' && self.shadow_color_mode == 2   self.set_color_mode(0)    # keep XY (1) while Tasmota reports rgb
        end
      end
      self.set_shadow_ct(st.find('ct'))
    end
  end

  #############################################################
  # Update ct_min/ct_max
  #
  # Standard range is 153..500 but Alexa emulation reduces range to 200..380
  # Depending on `SetOption82`
  def update_ct_minmax()
    var ct_alexa_mode = tasmota.get_option(82)      # if set, range is 200..380 instead of 153...500
    self.ct_min = ct_alexa_mode ? 200 : 153
    self.ct_max = ct_alexa_mode ? 380 : 500
  end

  # clamp `ct` to the physical range
  def ct_clamp(ct)
    self.update_ct_minmax()
    return (ct < self.ct_min) ? self.ct_min : ((ct > self.ct_max) ? self.ct_max : ct)
  end

  # update ColorTemperatureMireds shadow (nil = no value), clamped, reported on change
  def set_shadow_ct(ct)
    if ct != nil
      ct = self.ct_clamp(ct)
      if ct != self.shadow_ct   self.shadow_ct = ct   self.attribute_updated(0x0300, 0x0007)   end
    end
  end

  #############################################################
  # set_ct
  #
  # Set CT and switch to CT mode; `light.set()` only, no power-on
  # Returns the clamped CT (published by invoke)
  def set_ct(ct)
    ct = self.ct_clamp(ct)
    self.set_color_mode(2)                                      # 2 = ColorTemperatureMireds, before update_shadow()
    if self.BRIDGE
      var ret = self.call_remote_sync("CT", str(ct))
      if ret != nil   self.parse_status(ret, 11)   end          # nil on MQTT bridges: state arrives via RESULT
    elif self.VIRTUAL
      self.set_shadow_ct(ct)
    else
      import light
      light.set({'ct': ct}, self.light_index)
      self.update_shadow()
    end
    return ct
  end

  #############################################################
  # set_color_mode
  #
  # `mode`: 0 = CurrentHue and CurrentSaturation, 1 = CurrentX and CurrentY, 2 = ColorTemperatureMireds
  # Reports ColorMode and EnhancedColorMode only on change
  def set_color_mode(mode)
    if mode != self.shadow_color_mode
      self.shadow_color_mode = mode
      self.attribute_updated(0x0300, 0x0008)
      self.attribute_updated(0x0300, 0x4001)
    end
  end

  # Color Control command `cmd` is supported (single source for invoke and AcceptedCommandList)
  def cc_supported(cmd)
    var need = (cmd < 0x07) ? 0x01 : ((cmd < 0x0A) ? 0x08 : ((cmd == 0x47) ? 0x19 : 0x10))    # HS / XY / any / CT
    return self.CC_ARGS.contains(cmd) && (self.CC_FEAT & need) != 0
  end

  #############################################################
  # cc_args
  #
  # Validate command fields against CC_ARGS
  # Returns the list of int values, or nil with ctx.status set
  # (UNSUPPORTED_COMMAND is preset by IM, INVALID_COMMAND 0x85, CONSTRAINT_ERROR 0x87)
  def cc_args(val, ctx)
    if !self.cc_supported(ctx.command)   return nil   end
    var spec = self.CC_ARGS[ctx.command]
    if val != nil && !val.is_struct   val = nil   end           # malformed scalar CommandFields
    var a = []
    var i = 0
    while i < size(spec)
      var c = spec[i]
      var v = (val != nil) ? val.findsubval(i) : nil            # nil if absent or NULL
      if v == nil && c < 'a'   v = 0   end                      # optional field
      if type(v) != 'int'   ctx.status = 0x85 #-matter.INVALID_COMMAND-#   return nil   end
      if v > self.CC_LIM[c] || v < ((c == 's') ? -0x8000 : 0)
        ctx.status = 0x87 #-matter.CONSTRAINT_ERROR-#
        return nil
      end
      a.push(v)
      i += 1
    end
    return a
  end

  #############################################################
  # get_accepted_commands
  #
  # AcceptedCommandList for Color Control only; Level Control 0x0008 is unchanged (empty)
  def get_accepted_commands(cluster)
    if cluster == 0x0300 && self.CC_FEAT
      var r = bytes()
      for c: 0..0x4C
        if self.cc_supported(c)   r.add(c, 1)   end             # ascending order
      end
      return r
    end
    return super(self).get_accepted_commands(cluster)
  end

  #############################################################
  # write attribute
  #
  # true = SUCCESS, false with ctx.status = CONSTRAINT_ERROR, nil = UNSUPPORTED_WRITE (preset by IM)
  def write_attribute(session, ctx, write_data)
    if ctx.cluster == 0x0300 && self.CC_FEAT
      var attribute = ctx.attribute
      if attribute == 0x000F                                    # Options map8, only bit 0 (ExecuteIfOff) defined
        if type(write_data) == 'int' && write_data >= 0 && write_data <= 0x01
          if write_data != self.cc_options   self.cc_options = write_data   self.attribute_updated(0x0300, 0x000F)   end
          return true
        end
      elif attribute == 0x4010 && (self.CC_FEAT & 0x10)         # StartUpColorTemperatureMireds, nullable 1..0xFEFF
        var is_null = (ctx.write_tlv != nil) && (ctx.write_tlv.typ == 0x14 #-TLV.NULL-#)
        if is_null || (type(write_data) == 'int' && write_data >= 1 && write_data <= 0xFEFF)
          if write_data != self.ct_startup                      # write_data is nil for NULL
            self.ct_startup = write_data
            self.attribute_updated(0x0300, 0x4010)
            var conf = self.device.plugins_config.find(str(self.endpoint))
            if conf != nil
              if write_data == nil   conf.remove('ct_startup')   else   conf['ct_startup'] = write_data   end
              self.device.plugins_persist = true                # same as Matter_UI, save_param() writes config only when set
              self.device.save_param()                          # only on change: no flash wear on repeated writes
            end
          end
          return true
        end
      else
        return nil                                              # read-only: UNSUPPORTED_WRITE preset by IM
      end
      ctx.status = 0x87 #-matter.CONSTRAINT_ERROR-#
      return false
    end
    return super(self).write_attribute(session, ctx, write_data)
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0008              # ========== Level Control 1.6 p.57 ==========
      self.update_shadow_lazy()
      if   attribute == 0x0000          #  ---------- CurrentLevel / u1 ----------
        return tlv_solo.set(0x06 #-TLV.U4-#, self.shadow_bri)
      elif attribute == 0x0002          #  ---------- MinLevel / u1 ----------
        return tlv_solo.set(0x06 #-TLV.U4-#, 0)
      elif attribute == 0x0003          #  ---------- MaxLevel / u1 ----------
        return tlv_solo.set(0x06 #-TLV.U4-#, 254)
      elif attribute == 0x000F          #  ---------- Options / map8 ----------
        return tlv_solo.set(0x06 #-TLV.U4-#, 0)    #
      elif attribute == 0x0011          #  ---------- OnLevel / u1 ----------
        return tlv_solo.set(0x06 #-TLV.U4-#, self.shadow_bri)
      end

    # ====================================================================================================
    elif cluster == 0x0300 && self.CC_FEAT    # ========== Color Control 3.2 p.111 (Light2/3/5 only) ==========
      self.update_shadow_lazy()
      if   attribute == 0x0000          #  ---------- CurrentHue / u1 (HS) ----------
        return tlv_solo.set(0x06 #-TLV.U4-#, self.shadow_hue)
      elif attribute == 0x0001          #  ---------- CurrentSaturation / u1 (HS) ----------
        return tlv_solo.set(0x06 #-TLV.U4-#, self.shadow_sat)
      elif attribute == 0x0002 || attribute == 0x0010   #  ---------- RemainingTime / u2: always 0 (no transitions), NumberOfPrimaries / u1: none described ----------
        return tlv_solo.set(0x06 #-TLV.U4-#, 0)
      elif attribute == 0x0003          #  ---------- CurrentX / u2 (XY) ----------
        return tlv_solo.set(0x06 #-TLV.U4-#, self.shadow_x)
      elif attribute == 0x0004          #  ---------- CurrentY / u2 (XY) ----------
        return tlv_solo.set(0x06 #-TLV.U4-#, self.shadow_y)
      elif attribute == 0x0007          #  ---------- ColorTemperatureMireds / u2 (CT) ----------
        return tlv_solo.set(0x06 #-TLV.U4-#, self.shadow_ct)
      elif attribute == 0x0008 || attribute == 0x4001   #  ---------- ColorMode / EnhancedColorMode / enum8 (no EHUE) ----------
        return tlv_solo.set(0x06 #-TLV.U4-#, self.shadow_color_mode)
      elif attribute == 0x000F          #  ---------- Options / map8 ----------
        return tlv_solo.set(0x06 #-TLV.U4-#, self.cc_options)
      elif attribute == 0x400A || attribute == 0xFFFC   #  ---------- ColorCapabilities / map16, FeatureMap / map32 ----------
        return tlv_solo.set(0x06 #-TLV.U4-#, self.CC_FEAT)
      elif attribute >= 0x400B && attribute <= 0x400D   #  ---------- ColorTempPhysicalMin/MaxMireds, CoupleColorTempToLevelMinMireds (= physical min) / u2 ----------
        self.update_ct_minmax()
        return tlv_solo.set(0x06 #-TLV.U4-#, (attribute == 0x400C) ? self.ct_max : self.ct_min)
      elif attribute == 0x4010          #  ---------- StartUpColorTemperatureMireds / u2, nullable ----------
        return tlv_solo.set_or_nil(0x06 #-TLV.U4-#, self.ct_startup)
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
    if   cluster == 0x0008              # ========== Level Control 1.6 p.57 ==========
      if !self.mqtt_command_ready(ctx)   return nil   end
      self.update_shadow_lazy()
      if   command == 0x0000            # ---------- MoveToLevel ----------
        var bri_254 = val.findsubval(0)  # Hue 0..254
        self.set_bri(bri_254)
        ctx.log = "bri:"+str(bri_254)
        self.publish_command('Bri', bri_254, 'Dimmer', tasmota.scale_uint(bri_254, 0, 254, 0, 100))
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
        var bri_254 = val.findsubval(0)  # Hue 0..254
        var onoff = bri_254 > 0
        self.set_bri(bri_254, onoff)
        ctx.log = "bri:"+str(bri_254)
        self.publish_command('Power', onoff ? 1 : 0, 'Bri', bri_254, 'Dimmer', tasmota.scale_uint(bri_254, 0, 254, 0, 100))
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

    # ====================================================================================================
    elif cluster == 0x0300 && self.CC_FEAT    # ========== Color Control 3.2 p.111 (Light2/3/5 only) ==========
      if !self.mqtt_command_ready(ctx)   return nil   end
      self.update_shadow_lazy()                                 # current values for Step*
      var a = self.cc_args(val, ctx)                            # field types and limits, sets ctx.status on error
      if a == nil   return nil   end
      var n = size(a)
      # CTMinimumMireds > CTMaximumMireds (both non-zero) -> CONSTRAINT_ERROR (SDK order: before the semantic checks)
      if command >= 0x4B && a[n - 4] != 0 && a[n - 3] != 0 && a[n - 4] > a[n - 3]
        ctx.status = 0x87 #-matter.CONSTRAINT_ERROR-#   return nil
      end
      # semantic checks, before ExecuteIfOff
      if command == 0x02 || command == 0x05 || command == 0x4C                 # Step*: StepMode, StepSize
        if a[1] == 0                  ctx.status = 0x85 #-matter.INVALID_COMMAND-#    return nil   end
        if a[0] != 1 && a[0] != 3     ctx.status = 0x87 #-matter.CONSTRAINT_ERROR-#   return nil   end   # Up=1, Down=3
      elif command == 0x01 || command == 0x04 || command == 0x4B               # Move*: MoveMode, Rate
        if a[0] == 2                  ctx.status = 0x87 #-matter.CONSTRAINT_ERROR-#   return nil   end   # Stop=0, Up=1, Down=3
        if a[0] != 0 && a[1] == 0     ctx.status = 0x85 #-matter.INVALID_COMMAND-#    return nil   end
      elif command == 0x09 && a[0] == 0 && a[1] == 0                           # StepColor with no step
        ctx.status = 0x85 #-matter.INVALID_COMMAND-#   return nil
      end
      # Move* and StopMoveStep: validated above, accepted, no state change (no transition engine)
      if command == 0x01 || command == 0x04 || command == 0x08 || command == 0x47 || command == 0x4B
        ctx.log = "no-op"
        return true
      end
      # ExecuteIfOff: OptionsMask bit 0 selects OptionsOverride bit 0 instead of Options bit 0
      if !self.shadow_onoff && !((((a[n - 2] & 1) != 0) ? a[n - 1] : self.cc_options) & 1)
        ctx.log = "off, ignored"
        return true                                             # not executed, SUCCESS
      end
      ctx.log = str(a)
      # MoveTo* and Step*: applied instantly, TransitionTime (and MoveToHue Direction) ignored
      var up = (a[0] == 1)                                      # StepMode Up (Step* only)
      if   command == 0x00 || command == 0x02                   # MoveToHue / StepHue (wraps modulo 255)
        var h = (command == 0x00) ? a[0] : (self.shadow_hue + (up ? a[1] : 255 - a[1])) % 255
        self.set_hue_sat(h, nil)
        self.publish_command('Hue', h)
      elif command == 0x03 || command == 0x05                   # MoveToSaturation / StepSaturation (clamped 0..254)
        var s = (command == 0x03) ? a[0] : self.shadow_sat + (up ? a[1] : -a[1])
        s = (s < 0) ? 0 : ((s > 254) ? 254 : s)
        self.set_hue_sat(nil, s)
        self.publish_command('Sat', s)
      elif command == 0x06                                      # MoveToHueAndSaturation
        self.set_hue_sat(a[0], a[1])
        self.publish_command('Hue', a[0], 'Sat', a[1])
      elif command == 0x07 || command == 0x09                   # MoveToColor (absolute) / StepColor (relative, clamped 0..0xFEFF)
        var x = a[0]
        var y = a[1]
        if command == 0x09
          x += self.shadow_x   x = (x < 0) ? 0 : ((x > 0xFEFF) ? 0xFEFF : x)
          y += self.shadow_y   y = (y < 0) ? 0 : ((y > 0xFEFF) ? 0xFEFF : y)
        end
        var hs = self.set_xy(x, y)                              # returns the [hue, sat] sent to the light
        self.publish_command('Hue', hs[0], 'Sat', hs[1])
      else                                                      # MoveToColorTemperature / StepColorTemperature
        var ct = a[0]
        if command == 0x4C                                      # bounds: 0 = physical limit, else clamped into it (SDK)
          self.update_ct_minmax()
          var lo = a[n - 4]   if lo < self.ct_min   lo = self.ct_min   end
          var hi = a[n - 3]   if hi == 0 || hi > self.ct_max   hi = self.ct_max   end
          ct = up ? self.shadow_ct + a[1] : self.shadow_ct - a[1]
          if up && ct > hi    ct = hi   end
          if !up && ct < lo   ct = lo   end
        end
        self.publish_command('CT', self.set_ct(ct))             # set_ct clamps to the physical range and returns it
      end
      return true

    else
      return super(self).invoke_request(session, val, ctx)
    end
  end

  #############################################################
  # update_virtual
  #
  # Update internal state for virtual devices
  def update_virtual(payload)
    if self.CC_FEAT & 0x10                          # CT: applied after Hue/Sat handled by Light3 (CT wins if both)
      var val_ct = int(payload.find("CT"))          # int or nil
      if val_ct != nil   self.set_ct(val_ct)   end
    end
    var val_onoff = payload.find("Power")
    var val_bri = payload.find("Bri")
    if val_bri != nil
      self.set_bri(int(val_bri), val_onoff)
      return    # don't call super() because we already handeld 'Power'
    end
    super(self).update_virtual(payload)
  end

  #############################################################
  # For Bridge devices
  #############################################################
  #############################################################
  # Stub for updating shadow values (local copies of what we published to the Matter gateway)
  #
  # This call is synnchronous and blocking.
  def parse_status(data, index)
    super(self).parse_status(data, index)

    if index == 11                              # Status 11
      var dimmer = int(data.find("Dimmer"))     # 0..100
      if dimmer != nil
        var bri = tasmota.scale_uint(dimmer, 0, 100, 0, 254)
        if bri != self.shadow_bri
          self.attribute_updated(0x0008, 0x0000)
          self.shadow_bri = bri
        end
      end
      if self.CC_FEAT & 0x10   self.set_shadow_ct(int(data.find("CT")))   end    # 153..500, clamped
    end
  end

  #############################################################
  # web_values
  #
  # Show values of the remote device as HTML
  # Color plugins: CT in kelvin in CT mode, RGB swatch otherwise
  def web_values()
    import webserver
    self.web_values_prefix()        # display '| ' and name if present
    var col = ""
    if self.CC_FEAT
      col = " " + ((self.shadow_color_mode == 2) ? self.web_value_ct() : self.web_value_RGB())
    end
    webserver.content_send(self.web_value_onoff(self.shadow_onoff) + " " + self.web_value_dimmer() + col)
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

  # Show color temperature as html
  def web_value_ct()
    var ct_html = ""
    if self.shadow_ct != nil
      var ct_k = (((1000000 / self.shadow_ct) + 25) / 50) * 50      # convert in Kelvin
      ct_html = format("%iK", ct_k)
    end
    return  "&#9898; " + ct_html
  end
  #############################################################
  #############################################################

end
matter.Plugin_Light1 = Matter_Plugin_Light1
