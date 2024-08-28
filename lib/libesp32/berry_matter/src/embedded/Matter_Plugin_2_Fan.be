#
# Matter_Plugin_2_Fan.be - implements the behavior for a generic Fan
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

#@ solidify:Matter_Plugin_Fan,weak

class Matter_Plugin_Fan : Matter_Plugin_Device
  static var TYPE = "fan"                           # name of the plug-in in json
  static var DISPLAY_NAME = "Fan"                   # display name of the plug-in
  # static var ARG  = ""                              # additional argument name (or empty if none)
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                             # Identify 1.2 p.16
    # 0x0004: inherited                             # Groups 1.3 p.21
    # 0x0005: inherited                             # Scenes 1.4 p.30 - no writable
    0x0202: [0,1,2,3],                              # Fan
  })
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "FanMode", "FanSpeed", "FanSpeed255")
  static var TYPES = { 0x002B: 2 }                  # Fan

  # Inherited
  # var device                                        # reference to the `device` global object
  # var endpoint                                      # current endpoint
  # var clusters                                      # map from cluster to list of attributes, typically constructed from CLUSTERS hierachy
  # var tick                                          # tick value when it was last updated
  # var node_label                                    # name of the endpoint, used only in bridge mode, "" if none
  var shadow_fan_mode
  var shadow_fan_speed_pct
  #############################################################
  # FanMode:
  #     0: Off
  #     1: Low
  #     2: Medium
  #     3: High
  #     4: On -- deprecated
  #     5: Auto -- not declared as supported
  #     6: Smart -- deprecated

  #############################################################
  # Constructor
  def init(device, endpoint, config)
    super(self).init(device, endpoint, config)
    self.shadow_fan_mode = 0    # Off by default
    self.shadow_fan_speed_pct = 0
  end

  #############################################################
  # Model
  #
  def set_fan_mode(fan_mode)
    fan_mode = int(fan_mode)
    if (fan_mode < 0) fan_mode = 0 end        # force positive
    if fan_mode != self.shadow_fan_mode
      self.attribute_updated(0x0202, 0x0000)
      self.shadow_fan_mode = int(fan_mode)
      # compute new speed
      var new_speed_pct = self.shadow_fan_speed_pct
      if self.shadow_fan_mode == 0            # set to Off, we need to adjust speed to 0 (4.4.6.1.1)
        new_speed_pct = 0
      elif self.shadow_fan_mode > 3           # Auto mode or unsupported modes, since we don't support AUTO, set speed to max
        self.shadow_fan_mode = 3              # HIGH
        new_speed_pct = 100
      else                                    # set to value
        new_speed_pct = tasmota.scale_uint(fan_mode, 0, 3, 0, 100)
      end
      # adjust and advertize if speed changed
      if self.shadow_fan_speed_pct != new_speed_pct
        self.shadow_fan_speed_pct = new_speed_pct
        self.attribute_updated(0x0202, 0x0002)
      end
    end
  end

  def set_fan_speed_pct(fan_speed_pct)
    # guard value
    fan_speed_pct = int(fan_speed_pct)
    if (fan_speed_pct < 0)    fan_speed_pct = 0     end
    if (fan_speed_pct > 100)  fan_speed_pct = 100   end
    if fan_speed_pct != self.shadow_fan_speed_pct
      self.attribute_updated(0x0202, 0x0002)
      self.shadow_fan_speed_pct = fan_speed_pct
      # adjust mode if needed
      var new_mode = self.shadow_fan_mode
      if (fan_speed_pct == 0)
        new_mode = 0
      else
        new_mode = tasmota.scale_uint(fan_speed_pct, 1, 100, 1, 3)
      end
      # adjust and advertize if mode changed
      if (new_mode != self.shadow_fan_mode)
        self.shadow_fan_mode = new_mode
        self.attribute_updated(0x0202, 0x0000)
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
    if   cluster == 0x0202              # ========== Fan ==========
      self.update_shadow_lazy()
      if   attribute == 0x0000          #  ---------- FanMode / enum8 ----------
        return tlv_solo.set(TLV.U1, self.shadow_fan_mode)
      elif attribute == 0x0001          #  ---------- FanModeSequence / enum8 ----------
        return tlv_solo.set(TLV.U1, 2)  # Off/Low/Med/High/Auto
      elif attribute == 0x0002          #  ---------- PercentSetting / enum8 ----------
        return tlv_solo.set(TLV.U1, self.shadow_fan_speed_pct)
      elif attribute == 0x0003          #  ---------- PercentSetting / enum8 ----------
        return tlv_solo.set(TLV.U1, self.shadow_fan_speed_pct)
      end

    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  #############################################################
  # MVC Model
  #
  # Controller write attributes
  #############################################################
  #############################################################
  # write attribute
  def write_attribute(session, ctx, write_data)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute
    
    # ====================================================================================================
    if   cluster == 0x0202              # ========== Fan ==========
      self.update_shadow_lazy()
      if   attribute == 0x0000          #  ---------- FanMode / enum8 ----------
        if type(write_data) == 'int'
          self.set_fan_mode(write_data)
          self.publish_command('FanMode', self.shadow_fan_mode, 'FanSpeed', self.shadow_fan_speed_pct, 'FanSpeed255', tasmota.scale_uint(self.shadow_fan_speed_pct, 0, 100, 0, 255))
          return true
        else
          ctx.status = matter.CONSTRAINT_ERROR
          return false
        end
      elif attribute == 0x0002          #  ---------- PercentSetting / enum8 ----------
        if type(write_data) == 'int'
          self.set_fan_speed_pct(write_data)
          self.publish_command('FanMode', self.shadow_fan_mode, 'FanSpeed', self.shadow_fan_speed_pct, 'FanSpeed255', tasmota.scale_uint(self.shadow_fan_speed_pct, 0, 100, 0, 255))
          return true
        else
          ctx.status = matter.CONSTRAINT_ERROR
          return false
        end
      end

    end
    # return super(self).read_attribute(session, ctx, tlv_solo)   # not useful as there is nothing in superclass
    return nil
  end

  #############################################################
  # update_virtual
  #
  # Update internal state for virtual devices
  def update_virtual(payload)
    var val_fan_mode = payload.find("FanMode")
    if val_fan_mode != nil
      self.set_fan_mode(int(val_fan_mode))
    end
    var val_fan_speed255 = payload.find("FanSpeed255")
    if val_fan_speed255 != nil
      self.set_fan_speed_pct(tasmota.scale_uint(int(val_fan_speed255), 0, 255, 0, 100))
    end
    var val_fan_speed = payload.find("FanSpeed")
    if val_fan_speed != nil
      self.set_fan_speed_pct(int(val_fan_speed))
    end
    # super(self).update_virtual(payload)   # not useful as there is nothing in superclass
  end

end
matter.Plugin_Fan = Matter_Plugin_Fan
