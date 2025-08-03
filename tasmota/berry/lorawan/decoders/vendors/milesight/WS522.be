# LoRaWAN Decoder file for Milesight WS522
#
# References
#  WS522 User Manual:     https://resource.milesight.com/milesight/iot/document/ws52x-user-guide-en.pdf
#  TTN Device Repository: https://github.com/TheThingsNetwork/lorawan-devices/blob/master/vendor/milesight-iot/ws52x.js

import string

global.ws522Nodes = {}

def uint16le(value)
  return string.format( "%02x%02x",
    value & 0xFF,
    (value >> 8) & 0xFF
  )
end

def uint32le(value)
  return string.format( "%02x%02x%02x%02x", 
    value & 0xFF,
    (value >> 8)  & 0xFF,
    (value >> 16) & 0xFF,
    (value >> 24) & 0xFF
  )
end

class LwDecoWS522
  static def decodeUplink(Name, Node, RSSI, FPort, Bytes)
    var data = {"Device":"Milesight WS522"}

    var valid_values = false

    var rssi = RSSI

    var last_seen = 1451602800
    
    var voltage = 0.0         # 0.1 Volt
    var active_power = 0      # Watt
    var power_factor = 0      # %
    var energy_sum = 0        # kW
    var current = 0           
    var button_state = false  # false=close true=open

    var voltage_ls = 1451602800
    var active_power_ls = 1451602800
    var power_factor_ls = 1451602800
    var energy_sum_ls = 1451602800
    var current_ls = 1451602800
    var button_state_ls = 1451602800
    var command_init = false

    if global.ws522Nodes.find(Node)
      voltage         = global.ws522Nodes.item(Node)[4]
      active_power    = global.ws522Nodes.item(Node)[5]
      power_factor    = global.ws522Nodes.item(Node)[6]
      energy_sum      = global.ws522Nodes.item(Node)[7]
      current         = global.ws522Nodes.item(Node)[8]
      button_state    = global.ws522Nodes.item(Node)[9]

      voltage_ls      = global.ws522Nodes.item(Node)[10]
      active_power_ls = global.ws522Nodes.item(Node)[11]
      power_factor_ls = global.ws522Nodes.item(Node)[12]
      energy_sum_ls   = global.ws522Nodes.item(Node)[13]
      current_ls      = global.ws522Nodes.item(Node)[14]
      button_state_ls = global.ws522Nodes.item(Node)[15]
  
      command_init    = global.ws522Nodes.item(Node)[16]
    end

    var i = 0
    while i < (Bytes.size()-1)
      last_seen = tasmota.rtc('local')
      valid_values = true

      var channel_id = Bytes[i]
      i += 1
      var channel_type = Bytes[i]
      i += 1

      # VOLTAGE
      if channel_id == 0x03 && channel_type == 0x74
        voltage_ls = tasmota.rtc('local')
        voltage = ((Bytes[i+1] << 8) | Bytes[i]) / 10.0
        data.insert("Voltage", voltage)
        i += 2

      # ACTIVE POWER
      elif channel_id == 0x04 && channel_type == 0x80
        active_power_ls = tasmota.rtc('local')
        active_power = (Bytes[i+3] << 24) | (Bytes[i+2] << 16) | (Bytes[i+1] << 8) | Bytes[i]
        data.insert("Active_Power", active_power)
        i += 4

      # POWER FACTOR
      elif channel_id == 0x05 && channel_type == 0x81
        power_factor_ls = tasmota.rtc('local')
        power_factor = Bytes[i]
        data.insert("Power_Factor", power_factor)
        i += 1

      # ENERGY SUM
      elif channel_id == 0x06 && channel_type == 0x83
        energy_sum_ls = tasmota.rtc('local')
        energy_sum = (Bytes[i+3] << 24) | (Bytes[i+2] << 16) | (Bytes[i+1] << 8) | Bytes[i]
        data.insert("Energy_Sum", energy_sum)
        i += 4

      # CURRENT
      elif channel_id == 0x07 && channel_type == 0xc9
        current_ls = tasmota.rtc('local')
        current = (Bytes[i+1] << 8) | Bytes[i]
        data.insert("Current", current)
        i += 2

      # STATE
      elif channel_id == 0x08 && channel_type == 0x70
        button_state_ls = tasmota.rtc('local')
        button_state = Bytes[i] == 1 ? true : false
        data.insert("Button_State", button_state ? "Open" : "Close" )
        i += 1

      else
        log( string.format("WS522: something missing? id={%s} type={%s}", channel_id, channel_type), 1)

        # Ignore other
        valid_values = false
        i = Bytes.size()
      end
    end

    if valid_values
      if !command_init
          tasmota.add_cmd( "LoraWS522Power",
              def (cmd, idx, payload)
                  if global.ws522Nodes.find(idx)
                      if payload == "1" || string.toupper(payload) == "ON"
                          tasmota.cmd(string.format("LoraWanSend%d 080100FF",idx))  
                      elif payload == "0" || string.toupper(payload) == "OFF"
                          tasmota.cmd(string.format("LoraWanSend%d 080000FF",idx))  
                      else
                          # nothing else
                      end
                  end
              end
          )

          tasmota.add_cmd( "LoraWS522Period",
              def (cmd, idx, payload)
                  if global.ws522Nodes.find(idx)
                      if number(payload) > 30  
                          tasmota.cmd( string.format("LoraWanSend%d FF02%s", idx, uint16le(number(payload))) )
                      end
                  end
              end
          )

          tasmota.add_cmd( "LoraWS522Reboot",
              def (cmd, idx, payload)
                  if global.ws522Nodes.find(idx)
                      tasmota.cmd( string.format("LoraWanSend%d FF10FF", idx) )
                  end
              end
          )

          tasmota.add_cmd( "LoraWS522ResetPowerUsage",
              def (cmd, idx, payload)
                  if global.ws522Nodes.find(idx)
                      tasmota.cmd( string.format("LoraWanSend%d FF27FF", idx) )
                  end
              end
          )

          tasmota.add_cmd( "LoraWS522PowerLock",
              def (cmd, idx, payload)
                  if global.ws522Nodes.find(idx)
                      if payload == "1" || string.toupper(payload) == "ON"
                          tasmota.cmd(string.format("LoraWanSend%d FF250080",idx))  
                      elif payload == "0" || string.toupper(payload) == "OFF"
                          tasmota.cmd(string.format("LoraWanSend%d FF250000",idx))  
                      else
                          # nothing else
                      end
                  end
              end
          )
          command_init = true
      end

      if global.ws522Nodes.find(Node)
        global.ws522Nodes.remove(Node)
      end

      global.ws522Nodes.insert(Node,
        [                     # sensor
          Name,               # [0]
          Node,               # [1]
          last_seen,          # [2]
          rssi,               # [3]
          voltage,            # [4]
          active_power,       # [5]
          power_factor,       # [6]
          energy_sum,         # [7]
          current,            # [8]
          button_state,       # [9]
          voltage_ls,         # [10]
          active_power_ls,    # [11]
          power_factor_ls,    # [12]
          energy_sum_ls,      # [13]
          current_ls,         # [14]
          button_state_ls,    # [15]
          command_init        # [16]
        ]
      )
    end

    return data
  end #decodeUplink()

  static def add_web_sensor()
    var msg = ""
    for sensor: global.ws522Nodes
      var name = sensor[0]

      # If LoRaWanName contains WS522 use WS522-<node>
      if string.find(name, "WS522") > -1   
        name = string.format("WS522-%i", sensor[1])
      end

      var name_tooltip = "Milesight WS522"

      var last_seen = sensor[2]
      var rssi = sensor[3]
      
      msg += lwdecode.header(name, name_tooltip, 1000, last_seen, rssi, last_seen)

      # IEC Power Symbols
      #  Power	        &#x23FB; ⏻
      #  Toggle Power	  &#x23FC; ⏼
      #  Power On	      &#x23FD; ⏽
      #  Power Off	    &#x2B58; ⭘
      #  Sleep Mode	    &#x23FE; ⏾

      # Sensors
      var voltage = sensor[4]
      var active_power = sensor[5]
      var power_factor = sensor[6]
      var energy_sum = sensor[7]
      var current = sensor[8]
      var button_state = sensor[9] ? "&#x23FD;" : "&#x2B58;"
      var voltage_ls = sensor[10]
      var active_power_ls = sensor[11]
      var power_factor_ls = sensor[12]
      var energy_sum_ls = sensor[13]
      var current_ls = sensor[14]
      var button_state_ls = sensor[15]

      msg += "<tr class='htr'><td colspan='4'>&#9478;"                               # |
      msg += string.format(" %s %.1fV", "&#x26A1;",  voltage)                        # High Voltage Icon
      msg += string.format(" %s %dmA",  "&#x1F50C;", current)                        # Electric Plug Icon
      msg += string.format(" %s %d%%",  "&#x1F4CA;", power_factor)                   # Bar Chart Icon
      msg += string.format(" %s %dw",   "&#x1F4A1;", active_power)                   # Light Bulb Icon
      msg += "{e}<tr class='htr'><td colspan='4'>&#9478;"                               # |
      msg += string.format(" %s",       button_state)                                # Button Sate ON | OFF icon
      msg += string.format(" %s %s",    "&#x23F1;",  lwdecode.dhm(button_state_ls))  # Stopwatch icon
      msg += string.format(" %s %dWh",  "&#x1F9EE;", energy_sum)                     # Abacus Icon
      msg += "{e}"                                                                   # = </td></tr>
    end
    return msg
  end #add_web_sensor()
end #class

LwDeco = LwDecoWS522
