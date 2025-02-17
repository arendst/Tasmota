#
# class `twai`
#
# When loaded by `preinit.be` it wil:
# - configure TWAI/CAN driver speed and mode when enabled by GPIOs `TWAI Tx` and `TWAI Rx`
# - decode known Remeha Calenta Ace boiler codes
# - send every second changed values to Domoticz Idx
# - provide JSON message at teleperiod time
# - update GUI
#

class twai_cls
  var active, first_teleperiod, pressure_next  # (bool, bool, bool)
  var ch_active, dz_ch_active                  # (bool, bool)
  var dhw_active, dz_dhw_active                # (bool, bool)
  var pump_active, dz_pump_active              # (bool, bool)
  var twai_speed, twai_mode                    # (int, int)
  var am012_status, dz_am012_status            # (int, int)
  var am014_substatus, dz_am014_substatus      # (int, int)
  var am024_power                              # (int)
  var pressure, dz_pressure                    # (float, float)
  var flow_temp, dz_flow_temp                  # (float, float)
  var setpoint_temp, dz_setpoint_temp          # (float, float)
  
  def init()
    self.twai_speed = 7        # 0 = 25K, 1 = 50K, 2 = 100K, 3 = 125K, 4 = 250K, 5 = 500K, 6 = 800K, 7 = 1Mbits
    self.twai_mode = 2         # 0 = TWAI_MODE_NORMAL, 1 = TWAI_MODE_NO_ACK, 2 = TWAI_MODE_LISTEN_ONLY
    self.active = 0
    self.first_teleperiod = 0
    self.am012_status = 0
    self.dz_am012_status = 0
    self.am014_substatus = 0
    self.dz_am014_substatus = 0
    self.am024_power = 0
    self.pressure_next = 0
    self.pressure = 0
    self.dz_pressure = 0
    self.setpoint_temp = 0
    self.dz_setpoint_temp = 0
    self.flow_temp = 0
    self.dz_flow_temp = 0
    self.ch_active = 0
    self.dz_ch_active = 0
    self.dhw_active = 0
    self.dz_dhw_active = 0
    self.pump_active = 0
    self.dz_pump_active = 0
  end
  
  #----------------------------------------------------------------------------------------------
  Allow TWAI driver configuration on restart (if this file is installed by preinit.be)
  ----------------------------------------------------------------------------------------------#
  def config(bus)
#    if bus != 1 return nil end                       # Exit if not my bus
    return self.twai_mode << 3 | self.twai_speed     # Initial configure TWAI driver
  end
  
  #----------------------------------------------------------------------------------------------
  Decodes Remeha Calenta Ace CAN-bus messages
  ----------------------------------------------------------------------------------------------#
  def decode(param, ident, data1, data2)
    var bus = param & 0xF                            # Bus number (1..3)
#    if bus != 1 return nil end                       # Exit if not my bus
    var len = param >> 4 & 0xF                       # Number of data bytes (0..8)
    var extended = ident >> 31 & 0x1                 # Extended identifier flag (0..1)
    if extended == 1 return nil end                  # Remeha uses 11-bit Standard Frame Format
    var id = ident & 0x1fffffff
    if id == 0x076                                   # Incremental counter from 0 to 255
      # 13:27:59.485 TWA: Bus1 param 00000011, ident 00000076, data1 00000029, data2 00000000
#      tasmota.log(f"RMH: 0x{id:03x} Count {data1}", 3)
#    elif id == 0x080                                 # Heartbeat every second
      # 13:28:00.115 TWA: Bus1 param 00000001, ident 00000080, data1 00000000, data2 00000000
    elif id == 0x100                                 # Date and Time
      # 13:28:05.977 TWA: Bus1 param 00000061, ident 00000100, data1 02E3D288, data2 00003A82
      var epoch = 441763200 + (data2 * 24 * 60 * 60) + (data1 / 1000)
#      tasmota.log(f"RMH: 0x{id:03x} Time {tasmota.time_str(epoch)}", 3)
    elif id == 0x1C1                                 # Many different data1/2
      if data1 & 0x00ffffff == 0x503f41              # Next time it's pressure
        # 13:28:01.981 TWA: Bus1 param 00000081, ident 000001C1, data1 00503F41, data2 00000028
        self.pressure_next = 1
      elif self.pressure_next == 1
        # 13:28:01.983 TWA: Bus1 param 00000081, ident 000001C1, data1 21012300, data2 16EE1909
        self.pressure = (data2 & 0xff00)/2560.0      # This must be pressure
        self.pressure_next = 0
      end
    elif id == 0x382
      # 13:28:00.140 TWA: Bus1 param 00000081, ident 00000382, data1 0716BC64, data2 00FFFFFF
      self.am024_power = data1 & 0xff                # Relative power
      self.setpoint_temp = (data1 & 0xffff00)/25600.0  # Setpoint
#      tasmota.log(f"RMH: 0x{id:03x} Busy {self.am024_power}%, Setpoint {self.setpoint_temp}", 3)
    elif id == 0x282
      # 13:28:00.141 TWA: Bus1 param 00000051, ident 00000282, data1 1B16EE01, data2 00000000
      self.flow_temp = (data1 & 0xffff00)/25600.0
#      tasmota.log(f"RMH: 0x{id:03x} DHW temp {self.flow_temp}", 3)
    elif id == 0x481                                 # Status information
      # 13:28:00.561 TWA: Bus1 param 00000071, ident 00000481, data1 FFFF1E03, data2 002101FF
      self.am012_status = data1 & 0xff
      self.am014_substatus = (data1 >> 8) &0xff
      self.dhw_active = (data2 >> 20) &1 
      self.ch_active = (data2 >> 21) &1
      self.pump_active = (data2 >> 16) &1
#    elif id == 0x7E5                                 # Heartbeat every 2 seconds
      # 13:27:57.515 TWA: Bus1 param 00000081, ident 000007E5, data1 00000B51, data2 00008000
    else      
      return
    end
    self.active = 1                                  # At least one valid decode
  end
  
  #----------------------------------------------------------------------------------------------
  Send changed values to Domoticz every second

  As many datagrams can occur sending at teleperiod time takes too long
  Also only send if changed to reduce TWAI wait time 
  ----------------------------------------------------------------------------------------------#
  def every_second()
    if !self.first_teleperiod return nil end         # Exit if there is no MQTT connection

    if self.dz_pressure != self.pressure             # Pressure
      self.dzsend(1, 523, self.pressure)
    end
    self.dz_pressure = self.pressure

    if self.dz_setpoint_temp != self.setpoint_temp   # Setpoint temp
      self.dzsend(1, 525, self.setpoint_temp)
    end
    self.dz_setpoint_temp = self.setpoint_temp

    if self.dz_flow_temp != self.flow_temp           # Flow temp
      self.dzsend(1, 526, self.flow_temp)
    end
    self.dz_flow_temp = self.flow_temp

    if self.dz_am012_status != self.am012_status     # Status
      self.dzsend(1, 536, self.am012_status)
    end
    self.dz_am012_status = self.am012_status

    if self.dz_am014_substatus != self.am014_substatus  # Substatus
      self.dzsend(1, 537, self.am014_substatus)
    end
    self.dz_am014_substatus = self.am014_substatus

    if self.dz_dhw_active != self.dhw_active         # Domestic Hot Water active
      self.dzsend(3, 552, self.dhw_active)
    end
    self.dz_dhw_active = self.dhw_active

    if self.dz_ch_active != self.ch_active           # Central Heating active
      self.dzsend(3, 553, self.ch_active)
    end
    self.dz_ch_active = self.ch_active
  end

  def dzsend(option, domoticz_idx, value)
    tasmota.cmd('_DzSend' .. option .. ' ' .. domoticz_idx .. ',' .. value)
#    tasmota.cmd('Fake' .. option .. ' ' .. domoticz_idx .. ',' .. value)
  end  
    
  #----------------------------------------------------------------------------------------------
  Add sensor value to teleperiod
  ----------------------------------------------------------------------------------------------#
  def json_append()
    if !self.active return nil end                   # Exit if never decoded something
    import string
    var msg = string.format(",\"Calenta\":{\"AM012\":%i,\"AM014\":%i,\"Pressure\":%.1f,\"Setpoint\":%.1f,\"Flow\":%.1f}",
                              self.am012_status, self.am014_substatus, self.pressure, self.setpoint_temp, self.flow_temp)
    tasmota.response_append(msg)
    self.first_teleperiod = 1
  end

  #----------------------------------------------------------------------------------------------
  Display sensor value in the web UI
  ----------------------------------------------------------------------------------------------#
  def web_sensor()
    if !self.active return nil end                   # Exit if never decoded something
    import string
    var msg = string.format("{s}CH / DHW Active{m}%i / %i{e}"..
                            "{s}AM012 / AM014 State{m}%i / %i{e}"..
                            "{s}AM024 Relative Power{m}%i %%{e}"..
                            "{s}Pressure{m}%.1f{e}"..
                            "{s}Setpoint Temperature{m}%.1f{e}"..
                            "{s}Flow Temperature{m}%.1f{e}",
                            self.ch_active, self.dhw_active,
                            self.am012_status, self.am014_substatus,
                            self.am024_power, self.pressure, self.setpoint_temp, self.flow_temp)
    tasmota.web_send_decimal(msg)
  end
end

twai = twai_cls()
tasmota.add_driver(twai)