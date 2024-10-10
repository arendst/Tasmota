#
# Matter_z_Zigbee.be - implements common Zigbee handling
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

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# IMPORTANT: this class is included in build only if `#define USE_ZIGBEE` is defined in `C`
#
# This is managed in matter_module with includes
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


#@ solidify:Matter_Zigbee_Mapper,weak
##########################################################################################
# Matter_Zigbee_Mapper
#
# Class used to enrich Matter plug-in and do the mapping.
# This helps avoiding duplication of code
##########################################################################################
class Matter_Zigbee_Mapper
  var pi                                            # plug-in for this mapper
  var device_arg                                    # contains the device shortaddr (int) or device name (str) as per configuration JSON
                                                    # we need to store it, because the zigbee subsystem is not initialized when Matter starts
                                                    # hence lookup needs to be postponed
  var zigbee_device                                 # zigbee device
  var shortaddr                                     # shortaddr to facilitatefiltering

  def init(pi)
    self.pi = pi
  end

  #############################################################
  # parse_configuration
  #
  # Parse configuration map
  def parse_configuration(config)
    import zigbee
    import string
    self.device_arg = config.find(self.pi.ARG #-'zigbee_device'-#, nil)
    # we accept hex integers
    if (type(self.device_arg) == 'string')
      if string.startswith(self.device_arg, "0x") || string.startswith(self.device_arg, "0X")
        self.device_arg = int(self.device_arg)
      end
    end
    if (self.device_arg != nil)
      tasmota.set_timer(100, /-> self.probe_zb_values())         # delayed on purpose to make sure all matter is initialized
    end
  end

  #############################################################
  # read_zb_info
  #
  # Run the equivalent of `ZbInfo` and return the atttribute_list
  #
  # Return nil if something went wrong
  def read_zb_info()
    if self.resolve_zb_device()
      import zigbee
      var device = zigbee.find(self.shortaddr)
      if (device != nil)
        var info = device.info()
        return info
      end
    end
  end

  #############################################################
  # probe_zb_values
  #
  # Probe stored values so we don't need to wait for the sensor
  # to send a new value
  #
  # It is virtually equivalent to doing `ZbInfo` and parsing
  # the last known value
  def probe_zb_values()
    var info = self.read_zb_info()
    if (info != nil)
      log(f"MTR: Read information for zigbee device 0x{self.shortaddr:%04X}", 3)
      # handle it like if it was attributes received
      self.pi.zigbee_received(nil, info)
    end
  end
  
  #############################################################
  # resolve_zb_device
  #
  # Lazily resolve the device id
  # return true if found, false if not found or zigbee not started
  def resolve_zb_device()
    import zigbee
    if (self.device_arg == nil)   return false    end
    if (self.shortaddr != nil)    return true     end

    self.zigbee_device = zigbee.find(self.device_arg)
    if self.zigbee_device
      self.shortaddr = self.zigbee_device.shortaddr
      return true
    else
      log(f"MTR: cannot find zigbee device '{self.device_arg}'", 3)
      return false
    end
  end

  #############################################################
  # zb_single_command
  #
  # Convert a `MtrReceived` unitary payload to a Zigbee command
  def zb_single_command(key, value)
    # to ease caller, we accept nil arguments and do nothing
    var cmd
    if   (key == 'Power')
      cmd = f'ZbSend {{"Device":"0x{self.shortaddr:04X}","Send":{{"Power":{value:i}}}}}'
    elif (key == 'Bri')
      cmd = f'ZbSend {{"Device":"0x{self.shortaddr:04X}","Send":{{"Dimmer":{value:i}}}}}'
    elif (key == 'CT')
      cmd = f'ZbSend {{"Device":"0x{self.shortaddr:04X}","Send":{{"CT":{value:i}}}}}'
    end
    # send command
    if (cmd != nil)
      if tasmota.loglevel(3)
        log(f"MTR: '{cmd}'", 3)
      end
      tasmota.cmd(cmd, true)
    end
  end

end

##########################################################################################
# Matter_Zigbee
#
# Helper class for managing mapping between Matter and Zigbee
##########################################################################################
class Matter_Zigbee
  static var Matter_Zigbee_Mapper = Matter_Zigbee_Mapper
  var device                          # reference to the main Device instance
  # UI
  static var _CLASSES_TYPES = # Zigbee
                              "-zigbee|z_light0|z_light1|z_light2|z_temp|z_pressure|z_humidity|z_occupancy"

  #############################################################
  def init(device)
    import zigbee
    self.device = device
    zigbee.add_handler(self)          # listen to all events received
  end


  #############################################################
  # attributes_refined
  #
  # Called by Zigbee mapping whenever a new event is received
  def attributes_final(event_type, frame, attr_list, shortaddr)
    # iterate on all applicable endpoint
    # log(f"MTR: attributes_final received '{attr_list}' for 0x{shortaddr:04X}", 3)
    var plugins = self.device.plugins
    var idx = 0
    while (idx < size(plugins))
      var pi = plugins[idx]
      if (pi.ZIGBEE && pi.zigbee_mapper)          # first test always works, while second works only if `zigbee` arrtibute exists
        if (pi.zigbee_mapper.resolve_zb_device())    # resolve if this wan't done before
          if (pi.zigbee_mapper.shortaddr == shortaddr)
            pi.zigbee_received(frame, attr_list)
          end
        end
      end
      idx += 1
    end
  end

end

#@ solidify:matter_zigbee,weak
matter_zigbee = module('matter_zigbee')
matter_zigbee.Matter_Zigbee = Matter_Zigbee

def matter_zigbee_init(m)
  return m.Matter_Zigbee
end
matter_zigbee.init = matter_zigbee_init

