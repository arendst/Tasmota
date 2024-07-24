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

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Light1,weak

class Matter_Plugin_Light1 : Matter_Plugin_Light0
  static var TYPE = "light1"                                # name of the plug-in in json
  static var DISPLAY_NAME = "Light 1 Dimmer"                # display name of the plug-in
  static var ARG  = "light"                         # additional argument name (or empty if none)
  static var ARG_HINT = "(opt) Light number"
  # static var UPDATE_TIME = 250                      # update every 250ms
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                                     # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                                     # Identify 1.2 p.16
    # 0x0004: inherited                                     # Groups 1.3 p.21
    # 0x0005: inherited                                     # Scenes 1.4 p.30 - no writable
    # 0x0006: [0],                                    # On/Off 1.5 p.48
    0x0008: [0,2,3,0x0F,0x11],                      # Level Control 1.6 p.57
  })
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "Bri")
  static var TYPES = { 0x0101: 2 }                  # Dimmable Light

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
    # with Light0 we always need relay number but we don't for Light1/2/3 so self.tasmota_relay_index may be `nil`
    if self.BRIDGE
      self.tasmota_relay_index = int(config.find(self.ARG #-'relay'-#, nil))
      if (self.tasmota_relay_index != nil && self.tasmota_relay_index <= 0)    self.tasmota_relay_index = 1    end
    else
      if (self.tasmota_relay_index == nil) && (self.TYPE == "light1")   # only if `light1` and not for subclasses
        var light_index_arg = config.find(self.ARG #-'light'-#)
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
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0008              # ========== Level Control 1.6 p.57 ==========
      self.update_shadow_lazy()
      if   attribute == 0x0000          #  ---------- CurrentLevel / u1 ----------
        return tlv_solo.set(TLV.U1, self.shadow_bri)
      elif attribute == 0x0002          #  ---------- MinLevel / u1 ----------
        return tlv_solo.set(TLV.U1, 0)
      elif attribute == 0x0003          #  ---------- MaxLevel / u1 ----------
        return tlv_solo.set(TLV.U1, 254)
      elif attribute == 0x000F          #  ---------- Options / map8 ----------
        return tlv_solo.set(TLV.U1, 0)    #
      elif attribute == 0x0011          #  ---------- OnLevel / u1 ----------
        return tlv_solo.set(TLV.U1, self.shadow_bri)
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
        self.publish_command('Bri', bri_254, 'Dimmer', tasmota.scale_uint(bri_254, 0, 254, 0, 100), 'Power', onoff ? 1 : 0)
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
    
    else
      return super(self).invoke_request(session, val, ctx)
    end
  end

  #############################################################
  # update_virtual
  #
  # Update internal state for virtual devices
  def update_virtual(payload)
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
    end
  end

  #############################################################
  # web_values
  #
  # Show values of the remote device as HTML
  def web_values()
    import webserver
    self.web_values_prefix()        # display '| ' and name if present
    webserver.content_send(format("%s %s", self.web_value_onoff(self.shadow_onoff), self.web_value_dimmer()))
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
  #############################################################
  #############################################################

end
matter.Plugin_Light1 = Matter_Plugin_Light1
