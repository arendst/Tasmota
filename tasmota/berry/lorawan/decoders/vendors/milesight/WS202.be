# LoRaWAN Decoder file for Milesight WS202
#
# References
#  WS202 User Manual:     https://resource.milesight.com/milesight/iot/document/ws202-user-guide-en.pdf
#  TTN Device Repository: https://github.com/TheThingsNetwork/lorawan-devices/blob/master/vendor/milesight-iot/ws202.js

import string

if !global.ws202Nodes      # data survive to decoder reload
  global.ws202Nodes = {}
end

class LwDecoWS202
  static def decodeUplink(Name, Node, RSSI, FPort, Bytes)
    var data = {"Device":"Milesight WS202"}

    var valid_values = false

    var rssi = RSSI
    var last_seen = 1451602800
    var battery = 0
    var battery_last_seen = 1451602800
    var pir = 0     # 0=Normal 1=Trigger
    var pir_last_seen = 1451602800
    var light = 0   # 0=Dark >0=light
    var light_last_seen = 1451602800

    if global.ws202Nodes.find(Node)
      battery             = global.ws202Nodes.item(Node)[4]
      battery_last_seen   = global.ws202Nodes.item(Node)[5]
      pir                 = global.ws202Nodes.item(Node)[6]
      pir_last_seen       = global.ws202Nodes.item(Node)[7]
      light               = global.ws202Nodes.item(Node)[8]
      light_last_seen     = global.ws202Nodes.item(Node)[9]
    end

    var i = 0
    while i < (Bytes.size()-1)
        last_seen = tasmota.rtc('local')

        var channel_id = Bytes[i]
        i += 1
        var channel_type = Bytes[i]
        i += 1

        if channel_id == 0x01 && channel_type == 0x75
            battery_last_seen = tasmota.rtc('local')
            battery = Bytes[i]
            i += 1
            valid_values = true

        elif channel_id == 0x03 && channel_type == 0x00
            pir_last_seen = tasmota.rtc('local')
            pir = Bytes[i] == 0 ? false : true
            data.insert("PIR", pir)
            i += 1
            valid_values = true

        elif channel_id == 0x04 && channel_type == 0x00
            light_last_seen = tasmota.rtc('local')
            light = Bytes[i]
            data.insert("Light", light)
            i += 1
            valid_values = true

        else
          # Ignore other
          valid_values = false
          i = Bytes.size()
        end
    end

    if valid_values
      if global.ws202Nodes.find(Node)
        global.ws202Nodes.remove(Node)
      end
      #                         sensor[0]   [1]   [2]        [3]                [4]      [5]   [6]  [7]            [8]    [9]
      global.ws202Nodes.insert(Node, [Name, Node, last_seen, battery_last_seen, battery, RSSI, pir, pir_last_seen, light, light_last_seen])
    end

    return data
  end #decodeUplink()

  static def add_web_sensor()
    var msg = ""
    for sensor: global.ws202Nodes
      var name = sensor[0]
      if string.find(name, "WS202") > -1                                    # If LoRaWanName contains WS202 use WS202-<node>
        name = string.format("WS202-%i", sensor[1])
      end
      var name_tooltip = "Milesight WS202"
      var last_seen = sensor[2]
      var battery_last_seen = sensor[3]
      var battery = sensor[4]
      var rssi = sensor[5]
      msg += lwdecode.header(name, name_tooltip, battery + 100000, battery_last_seen, rssi, last_seen)

      # Sensors
      var pir = lwdecode.dhm(sensor[7])
      var pir_ls = lwdecode.dhm_tt(sensor[7])
      var pir_alt = (sensor[6] == true ? "&#x1F6AB;" : "&#x1F193;") # No Entry 🚫 / Free 🆓
      
      var light     = lwdecode.dhm(sensor[9])
      var light_ls  = lwdecode.dhm_tt(sensor[9])
      var light_alt = (sensor[8] == 0) ? "&#x1F315;" : "&#x1F31E;"  # Moon 🌕 / Sun 🌞

      var fmt = LwSensorFormatter_cls()
      msg += fmt.start_line()
        .add_sensor( "string", pir,   pir_ls,   pir_alt )
        .add_sensor( "string", light, light_ls, light_alt )
        .end_line()
        .get_msg()
    end
    return msg
  end #add_web_sensor()
end #class

LwDeco = LwDecoWS202
