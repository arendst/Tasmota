# LoRaWAN Decoder file for Milesight WS522
#
# References
#  WS522 User Manual:     https://resource.milesight.com/milesight/iot/document/ws52x-user-guide-en.pdf
#  Device Decoder:        https://github.com/Milesight-IoT/SensorDecoders/blob/main/WS_Series/WS52x/WS52x_Decoder.js

import string

if !global.ws522Nodes      # data survive to decoder reload
  global.ws522Nodes = {}
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

      # FE03(ReportInterval) 3C00=>60  5802=>600
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

      elif channel_id == 0xFF && channel_type == 0x0B i += 1  # FF0b(PowerOn) Deviceison
      elif channel_id == 0xFF && channel_type == 0x16 i += 8  # FF16(DeviceSN) 16digits
      elif channel_id == 0xFF && channel_type == 0x0F i += 1  # FF0f(DeviceType) 00:ClassA,01:ClassB,02:ClassC
      elif channel_id == 0xFF && channel_type == 0xFF i += 2  # TSL VERSION
      elif channel_id == 0xFF && channel_type == 0xFE i += 1  # RESET EVENT

      elif channel_id == 0xFE && channel_type == 0x03 i += 2  # id=0xFE yy  Downlink Reporting Event
      elif channel_id == 0xFE && channel_type == 0x10 i += 1
      elif channel_id == 0xFE && channel_type == 0x22 i += 4
      elif channel_id == 0xFE && channel_type == 0x23 i += 2
      elif channel_id == 0xFE && channel_type == 0x24 i += 2
      elif channel_id == 0xFE && channel_type == 0x25 i += 2
      elif channel_id == 0xFE && channel_type == 0x26 i += 1
      elif channel_id == 0xFE && channel_type == 0x27 i += 1
      elif channel_id == 0xFE && channel_type == 0x28 i += 1
      elif channel_id == 0xFE && channel_type == 0x2F i += 1
      elif channel_id == 0xFE && channel_type == 0x30 i += 2

      else
        log( string.format("WS522: something missing? id={%s} type={%s}", channel_id, channel_type), 1)

        # Ignore other
        valid_values = false
        i = Bytes.size()
      end
    end

    if valid_values
      if !command_init
          #   
          #      Downlink            Commands 
          #      =================   ===============================
          #   ✅ 08 00 00 FF         Close
          #   ✅ 08 01 00 FF         Open
          #   ✅ FF 03 ss ss         SetReportingInterval (2 bytes, seconds)
          #   ✅ FF 10 FF            Reboot
          #   ✅ FF 22 00 ss ss aa   AddDelayTask (ss=delay seconds, aa=action 10=close/11=open)
          #   ✅ FF 23 00 FF         DeleteDelayTask
          #   ❓ FF 24 xx yy         OvercurrentAlarm (xx: 00=off/01=on, yy=threshold)
          #   ✅ FF 25 00 xx         ButtonLock (xx: 00=off/80=on)
          #   ✅ FF 26 yy            PowerConsumption (yy: 00=off/01=on)
          #   ✅ FF 27 FF            ResetPowerConsumption
          #   ✅ FF 28 FF            EnquireElectricalStatus
          #   ✅ FF 2F xx            LEDMode (xx: 00=off/01=on)
          #   ❓ FF 30 xx yy         OvercurrentProtection (XX: 00=off/01=on, YY=threshold)
          #   
          #   ✅ = Verified  ❓= Not verified yet   ❌=Issue, under investigation
          #

          var pfx = 'LwWS522'

          tasmota.remove_cmd( pfx + 'Power' )
          tasmota.add_cmd( pfx + 'Power',
            def (cmd, idx, payload)
              return lwdecode.SendDownlinkMap(global.ws522Nodes, cmd, idx, payload, { '1|ON': ['080100FF', 'ON'], '0|OFF': ['080000FF', 'OFF']  })
            end
          )

          tasmota.remove_cmd( pfx + 'Period' )
          tasmota.add_cmd( pfx + 'Period',
            def (cmd, idx, payload)
              return lwdecode.SendDownlink(global.ws522Nodes, cmd, idx, format('FF03%s',lwdecode.uint16le(number(payload))), number(payload))
            end
          )

          tasmota.remove_cmd( pfx + 'Reboot' )
          tasmota.add_cmd( pfx + 'Reboot',
            def (cmd, idx, payload)
              return lwdecode.SendDownlink(global.ws522Nodes, cmd, idx, 'FF10FF', 'Done')
            end
          )

          tasmota.remove_cmd( pfx + 'ResetPowerUsage' )
          tasmota.add_cmd( pfx + 'ResetPowerUsage',
            def (cmd, idx, payload)
              return lwdecode.SendDownlink(global.ws522Nodes, cmd, idx, 'FF27FF', 'Done')
            end
          )

          tasmota.remove_cmd( pfx + 'PowerLock' )
          tasmota.add_cmd( pfx + 'PowerLock',
            def (cmd, idx, payload)
              return lwdecode.SendDownlinkMap(global.ws522Nodes, cmd, idx, payload, { '1|ON': ['FF250080', 'ON'], '0|OFF': ['FF250000', 'OFF']  })
            end
          )

          tasmota.remove_cmd( pfx + 'DelayTask' )
          tasmota.add_cmd( pfx + 'DelayTask',
            def (cmd, idx, payload)
              var parts = string.split(payload,',')
              if parts.size() != 2
                return tasmota.resp_cmnd_str("Usage: delay_seconds,action (action: 0=close, 1=open)")
              end
              var delay = number(parts[0])
              var action = number(parts[1]) == 1 ? '11' : '10'
              return lwdecode.SendDownlink(global.ws522Nodes, cmd, idx, format('FF2200%s%s',lwdecode.uint16le(delay),action), payload)
            end
          )

          tasmota.remove_cmd( pfx + 'DelTask' )
          tasmota.add_cmd( pfx + 'DelTask',
            def (cmd, idx, payload)
              return lwdecode.SendDownlink(global.ws522Nodes, cmd, idx, 'FF2300FF', 'Done')
            end
          )

          tasmota.remove_cmd( pfx + 'OcAlarm' )
          tasmota.add_cmd( pfx + 'OcAlarm',
            def (cmd, idx, payload)
              var parts = string.split(payload,',')
              if parts.size() != 2 
                return tasmota.resp_cmnd_str("Usage: enable,threshold (enable: 0/1, threshold: 0-255)")
              end
              var enable = number(parts[0]) ? '01' : '00'
              var threshold = format('%02X', number(parts[1]))
              return lwdecode.SendDownlink(global.ws522Nodes, cmd, idx, format('FF24%s%s',enable,threshold), payload)
            end
          )

          tasmota.remove_cmd( pfx + 'PwrUsage' )
          tasmota.add_cmd( pfx + 'PwrUsage',
            def (cmd, idx, payload)
              return lwdecode.SendDownlinkMap(global.ws522Nodes, cmd, idx, payload, { '1|ON': ['FF2601FF', 'ON'], '0|OFF': ['FF2600FF', 'OFF'] })
            end
          )

          tasmota.remove_cmd( pfx + 'Status' )
          tasmota.add_cmd( pfx + 'Status',
           def (cmd, idx, payload)
             return lwdecode.SendDownlink(global.ws522Nodes, cmd, idx, 'FF28FF', 'Done')
           end
          )

          tasmota.remove_cmd( pfx + 'LED' )
          tasmota.add_cmd( pfx + 'LED',
           def (cmd, idx, payload)
             return lwdecode.SendDownlinkMap(global.ws522Nodes, cmd, idx, payload, { '1|ON': ['FF2F01', 'ON'], '0|OFF': ['FF2F00', 'OFF'] })
           end
          )

          tasmota.remove_cmd( pfx + 'OcProt' )
          tasmota.add_cmd( pfx + 'OcProt',
           def (cmd, idx, payload)
             var parts = string.split(payload,',')
             if parts.size() != 2
               return tasmota.resp_cmnd_str("Usage: enable,threshold (enable: 0/1, threshold: 0-255)")
             end
             var enable = number(parts[0]) ? '01' : '00'
             var threshold = format('%02X', number(parts[1]))
             return lwdecode.SendDownlink(global.ws522Nodes, cmd, idx, format('FF30%s%s',enable,threshold), payload)
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
