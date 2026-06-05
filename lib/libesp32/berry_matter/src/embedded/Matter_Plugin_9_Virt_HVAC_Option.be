#
# Matter_Plugin_9_Virt_HVAC_Option.be - virtual switch bound to a v.HVAC IRHVAC option
#
# Copyright (C) 2026  Stephan Hadinger & Theo Arends
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
# Virtual On/Off switch that toggles an extra IRHVAC option on a v.HVAC endpoint.
#################################################################################

import matter

#@ solidify:Matter_Plugin_Virt_HVAC_Option,weak

class Matter_Plugin_Virt_HVAC_Option : Matter_Plugin_OnOff
  static var TYPE = "v_hvac_option"
  static var DISPLAY_NAME = "v.HVAC option"
  static var SCHEMA = "hvac_ep|l:HVAC endpoint|t:i|h:Endpoint number of the v.HVAC device, usually 2|r:1"
  static var SCHEMA2 = "option|l:Option|t:s|d:Econo|o:Econo:Econo,Quiet:Quiet,Turbo:Turbo,Light:Light,Filter:Filter,Clean:Clean,Beep:Beep,iFeel:iFeel"
  static var VIRTUAL = true
  static var AGGREGATE = false

  var hvac_ep
  var option_name

  def parse_configuration(config)
    super(self).parse_configuration(config)
    self.hvac_ep = int(config.find("hvac_ep", 0))
    self.option_name = config.find("option", "Econo")
  end

  def set_onoff(pow)
    var old_onoff = self.shadow_onoff
    super(self).set_onoff(pow)
    if old_onoff != self.shadow_onoff
      self.trigger_hvac()
    end
  end

  def trigger_hvac()
    if self.hvac_ep <= 0
      return nil
    end
    var hvac = self.device.find_plugin_by_endpoint(self.hvac_ep)
    if hvac != nil && hvac.TYPE == "v_hvac"
      return hvac.send_irhvac()
    end
    log("MTR: v.HVAC option could not find v.HVAC endpoint " + str(self.hvac_ep), 2)
    return nil
  end

end
matter.Plugin_Virt_HVAC_Option = Matter_Plugin_Virt_HVAC_Option
