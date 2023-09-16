#
# Matter_Plugin_Light2.be - implements the behavior for a Light with 2 channel (CT)
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

#@ solidify:Matter_Plugin_Light2,weak

class Matter_Plugin_Light2 : Matter_Plugin_Light1
  static var TYPE = "light2"                                # name of the plug-in in json
  static var DISPLAY_NAME = "Light 2 CT"                            # display name of the plug-in
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                                     # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                                     # Identify 1.2 p.16
    # 0x0004: inherited                                     # Groups 1.3 p.21
    # 0x0005: inherited                                     # Scenes 1.4 p.30 - no writable
    # 0x0006: inherited                                     # On/Off 1.5 p.48
    # 0x0008: inherited                                     # Level Control 1.6 p.57
    0x0300: [7,8,0xF,0x400B,0x400C,0xFFFC,0xFFFD],          # Color Control 3.2 p.111
  })
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "CT")
  static var TYPES = { 0x010C: 2 }                  # Color Temperature Light

  # Inherited
  # var device                                        # reference to the `device` global object
  # var endpoint                                      # current endpoint
  # var clusters                                      # map from cluster to list of attributes, typically constructed from CLUSTERS hierachy
  # var tick                                          # tick value when it was last updated
  # var node_label                                    # name of the endpoint, used only in bridge mode, "" if none
  # var shadow_onoff                                  # (bool) status of the light power on/off
  # var shadow_bri                                    # (int 0..254) brightness before Gamma correction - as per Matter 255 is not allowed
  var shadow_ct                                     # (int 153..500, default 325) Color Temperatur in mireds
  var ct_min, ct_max                                # min and max value allowed for CT, Alexa emulation requires to have a narrower range 200..380

  #############################################################
  # Constructor
  def init(device, endpoint, arguments)
    super(self).init(device, endpoint, arguments)
    self.shadow_ct = 325
    self.update_ct_minmax()                         # read SetOption to adjust ct min/max
  end

  #############################################################
  # Update shadow
  #
  def update_shadow()
    import light
    self.update_ct_minmax()
    super(self).update_shadow()
    var light_status = light.get()
    if light_status != nil
      var ct = light_status.find('ct', nil)
      if ct  == nil     ct = self.shadow_ct      end
      if ct  != self.shadow_ct
        self.attribute_updated(0x0300, 0x0007)
        self.shadow_ct = ct
      end
    end
  end

  #############################################################
  # Update ct_min/max
  #
  # Standard range is 153..500 but Alexa emulation reduces range to 200..380
  # Depending on `SetOption82`
  def update_ct_minmax()
    var ct_alexa_mode = tasmota.get_option(82)      # if set, range is 200..380 instead of 153...500
    self.ct_min = ct_alexa_mode ? 200 : 153
    self.ct_max = ct_alexa_mode ? 380 : 500
  end

  #############################################################
  # set_ct
  #
  def set_ct(ct)
    if ct < self.ct_min  ct = self.ct_min   end
    if ct > self.ct_max  ct = self.ct_max   end
    if !self.VIRTUAL
      import light
      light.set({'ct': ct})
      self.update_shadow()
    else
      if ct  != self.shadow_ct
        self.attribute_updated(0x0300, 0x0007)
        self.shadow_ct = ct
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
    if   cluster == 0x0300              # ========== Color Control 3.2 p.111 ==========
      self.update_shadow_lazy()
      if   attribute == 0x0007          #  ---------- ColorTemperatureMireds / u2 ----------
        return tlv_solo.set(TLV.U1, self.shadow_ct)
      elif attribute == 0x0008          #  ---------- ColorMode / u1 ----------
        return tlv_solo.set(TLV.U1, 2)# 2 = ColorTemperatureMireds
      elif attribute == 0x000F          #  ---------- Options / u1 ----------
        return tlv_solo.set(TLV.U1, 0)
      elif attribute == 0x400B          #  ---------- ColorTempPhysicalMinMireds / u2 ----------
        return tlv_solo.set(TLV.U1, self.ct_min)
      elif attribute == 0x400C          #  ---------- ColorTempPhysicalMaxMireds / u2 ----------
        return tlv_solo.set(TLV.U1, self.ct_max)
      
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
    import light
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var command = ctx.command

    # ====================================================================================================
    if   cluster == 0x0300              # ========== Color Control 3.2 p.111 ==========
      self.update_shadow_lazy()
      if   command == 0x000A            # ---------- MoveToColorTemperature ----------
        var ct_in = val.findsubval(0)  # CT
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
  # update_virtual
  #
  # Update internal state for virtual devices
  def update_virtual(payload_json)
    var val_ct = int(payload_json.find("CT"))         # int or nil
    if (val_ct != nil)
      self.set_ct(val_ct)
    end
    super(self).update_virtual(payload_json)
  end

end
matter.Plugin_Light2 = Matter_Plugin_Light2
