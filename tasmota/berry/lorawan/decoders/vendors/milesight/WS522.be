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

      # FE02(ReportInterval) 3C00=>06
      elif channel_id == 0xFE && channel_type == 0x02
        data.insert("Period", ((Bytes[i+1] << 8) | Bytes[i]) )
        i += 2

      # FF01(ProtocolVersion) 01=>V1 
      elif channel_id == 0xFF && channel_type == 0x01
        data.insert("Protocol Version", Bytes[i] )
        i += 1

      # FF09(HardwareVersion) 0140=>V1.4 
      elif channel_id == 0xFF && channel_type == 0x09
        data.insert("Hardware Version",  format("v%02x.%02x", Bytes[i], Bytes[i+1]) )
        i += 2

      # FF0a(SoftwareVersion) 0114=>V1.14 
      elif channel_id == 0xFF && channel_type == 0x0A
        data.insert("Software Version",  format("v%02x.%02x", Bytes[i], Bytes[i+1]) )
        i += 2

      # FF0b(PowerOn) Deviceison
      elif channel_id == 0xFF && channel_type == 0x0B
        i += 1

      # FF16(DeviceSN) 16digits
      elif channel_id == 0xFF && channel_type == 0x16
        i += 8

      # FF0f(DeviceType) 00:ClassA,01:ClassB,02:ClassC
      elif channel_id == 0xFF && channel_type == 0x0F
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
          tasmota.add_cmd( "LwWS522Power",
            def (cmd, idx, payload)
              if payload == "1" || string.toupper(payload) == "ON"
                  return lwdecode.SendDownlink(global.ws522Nodes, cmd, idx, '080100FF', 'ON')
              elif payload == "0" || string.toupper(payload) == "OFF"
                  return lwdecode.SendDownlink(global.ws522Nodes, cmd, idx, '080000FF', 'OFF')
              end
            end
          )

          tasmota.add_cmd( "LwWS522Period",
            def (cmd, idx, payload)
                return lwdecode.SendDownlink(global.ws522Nodes, cmd, idx, format('FF02%s',uint16le(number(payload))), number(payload))
            end
          )

          tasmota.add_cmd( "LwWS522Reboot",
            def (cmd, idx, payload)
                return lwdecode.SendDownlink(global.ws522Nodes, cmd, idx, 'FF10FF', 'Done')
            end
          )

          tasmota.add_cmd( "LwWS522ResetPowerUsage",
            def (cmd, idx, payload)
                return lwdecode.SendDownlink(global.ws522Nodes, cmd, idx, 'FF27FF', 'Done')
            end
          )

          tasmota.add_cmd( "LwWS522PowerLock", 
            def (cmd, idx, payload)
              if payload == "1" || string.toupper(payload) == "ON"
                return lwdecode.SendDownlink(global.ws522Nodes, cmd, idx, 'FF250080', 'ON')
              elif payload == "0" || string.toupper(payload) == "OFF"
                return lwdecode.SendDownlink(global.ws522Nodes, cmd, idx, 'FF250080', 'OFF')
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

      # Sensors
      var voltage = sensor[4]
      var voltage_tt = lwdecode.dhm(sensor[10])

      var active_power = sensor[5]
      var active_power_tt = lwdecode.dhm(sensor[11])

      var power_factor = sensor[6]
      var power_factor_tt = lwdecode.dhm(sensor[12])

      var current = sensor[8]
      var current_tt = lwdecode.dhm(sensor[14])

      var button_state = lwdecode.dhm(sensor[15])
      var button_state_tt = lwdecode.dhm(sensor[15])
      var button_state_icon = (sensor[9] ? " &#x1F7E2; " : " &#x26AB; ") # Large Green Circle 🟢 | Medium Black Circle ⚫

      var energy_sum = sensor[7]
      var energy_sum_tt = lwdecode.dhm(sensor[13] )

      var fmt = LwSensorFormatter_cls()

      #             Formatter         Value           Tooltip             alternative icon
      #             ================  ============    ==================  ================
      msg += fmt.start_line()
        .add_sensor("volt",           voltage,        voltage_tt )
        .add_sensor("milliamp",       current,        current_tt )
        .add_sensor("power_factor%",  power_factor,   power_factor_tt )
        .add_sensor("power",          active_power,   active_power_tt )
        .next_line()
        .add_sensor("string",         button_state,   button_state_tt,    button_state_icon )
        .add_sensor("energy",         energy_sum,     energy_sum_tt )
        .end_line()
        .get_msg()
    end
    return msg
  end #add_web_sensor()
end #class

LwDeco = LwDecoWS522
