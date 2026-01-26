# LoRaWAN Decoder file for Glamos Walker
#
# References                https://glamos.eu/product/walker/

import string

if !global.walkerNodes
  global.walkerNodes = {}
end

class LwDecoWALKER
  static var hashCheck = false

  static def decodeUplink(Name, Node, RSSI, FPort, Bytes)
    var data = {"Device":"Glamos WALKER"}

    var valid_values = false

    var rssi = RSSI

    var last_seen = 1451602800
    
    var latitude  = 0.0      # 3 bytes
    var longitude = 0.0      # 3 bytes
    var altitude  = 0.0      # 2 bytes
    var antenna   = 0        # 1 byte (manually set by user on the device)
    var position  = 0        # 1 byte (manually set by user on the device)
    var counter   = 0
    var command_init = false
    var track_enabled = false
    var history = []

    if global.walkerNodes.find(Node)
      latitude      = global.walkerNodes.item(Node)[4]
      longitude     = global.walkerNodes.item(Node)[5]
      altitude      = number( global.walkerNodes.item(Node)[6] )
      antenna       = number( global.walkerNodes.item(Node)[7] )
      position      = number( global.walkerNodes.item(Node)[8] )
      counter       = number( global.walkerNodes.item(Node)[9] )
      command_init  = global.walkerNodes.item(Node)[10]
      track_enabled = global.walkerNodes.item(Node)[11]
      history       = global.walkerNodes.item(Node)[12]
    end

    try 
      if Bytes.size() == 10
        valid_values = true

        last_seen = tasmota.rtc('local')

        latitude  = (((Bytes[0] << 16) | (Bytes[1] << 8) | (Bytes[2] << 0)) / 16777215.0) * 180.0 - 90.0
        longitude = (((Bytes[3] << 16) | (Bytes[4] << 8) | (Bytes[5] << 0)) / 16777215.0) * 360.0 - 180.0
        altitude  = (Bytes[6] << 8) | (Bytes[7] << 0)
        antenna   = Bytes[8]
        position  = Bytes[9]
        counter  += 1

        data.insert("Latitude",     latitude)
        data.insert("Longitude",    longitude)
        data.insert("Altitude",     altitude)
        data.insert("Antenna",      antenna)
        data.insert("Position",     position)
        data.insert("Counter",      counter)
        data.insert("Tracking",     track_enabled)
        data.insert("History Size", history.size())
      end
    except .. as e, m
        print(e .. ': ' .. m)
    end

    if valid_values
      if !command_init
        var pfx = 'Walker'

        tasmota.remove_cmd(pfx + 'ReloadCmd')
        tasmota.add_cmd( pfx + 'ReloadCmd',
          def (cmd, idx, payload)
            if global.walkerNodes.find(idx)
              global.walkerNodes.item(idx)[10] = false  # reload command after a 'LwReload'
              return tasmota.resp_cmnd_done()
            end
          end
        )

        tasmota.remove_cmd(pfx + 'ResetCounter')
        tasmota.add_cmd( pfx + 'ResetCounter',
          def (cmd, idx, payload)
            if global.walkerNodes.find(idx)
              global.walkerNodes.item(idx)[9] = 0
              return tasmota.resp_cmnd_done()
            end
          end
        )

        tasmota.remove_cmd(pfx + 'EnableTrack')
        tasmota.add_cmd( pfx + 'EnableTrack',
          def (cmd, idx, payload)
            if global.walkerNodes.find(idx)
              global.walkerNodes.item(idx)[11] = true
              return tasmota.resp_cmnd_done()
            end
          end
        )

        tasmota.remove_cmd(pfx + 'DisableTrack')
        tasmota.add_cmd( pfx + 'DisableTrack',
          def (cmd, idx, payload)
            if global.walkerNodes.find(idx)
              global.walkerNodes.item(idx)[11] = false
              return tasmota.resp_cmnd_done()
            end
          end
        )

        tasmota.remove_cmd(pfx + 'ClearHistory')
        tasmota.add_cmd( pfx + 'ClearHistory',
          def (cmd, idx, payload)
            if global.walkerNodes.find(idx)
              global.walkerNodes.item(idx)[12].clear()
              return tasmota.resp_cmnd_done()
            end
          end
        )
        
        tasmota.remove_cmd(pfx + 'GetHistory')
        tasmota.add_cmd( pfx + 'GetHistory',
          def (cmd, idx, payload)
            try 
              if global.walkerNodes.find(idx)
                import json
                return tasmota.resp_cmnd( '{"' .. pfx .. 'History":' .. json.dump( global.walkerNodes.item(idx)[12]) .. '}' )
              end
            except .. as e, m
                print(e .. ': ' .. m)
            end
          end
        )

        command_init = true
      end

      try
        if track_enabled
          if history.size() > 50  # may be dynamic?
            history.remove(0)
          end

          history.push( [
             last_seen
            ,latitude
            ,longitude
            ,altitude
            ,antenna
            ,position
            ,counter
          ])
        end
      except .. as e, m
          print(e .. ': ' .. m)
      end

      if global.walkerNodes.find(Node)
        global.walkerNodes.remove(Node)
      end

      global.walkerNodes.insert(Node,
        [                 # sensor
           Name           # [0]
          ,Node           # [1]
          ,last_seen      # [2]
          ,rssi           # [3]
          ,latitude       # [4]
          ,longitude      # [5]
          ,altitude       # [6]
          ,antenna        # [7]
          ,position       # [8]
          ,counter        # [9]
          ,command_init   # [10]
          ,track_enabled  # [11]
          ,history        # [12]
        ]
      )
    end

    return data
  end #decodeUplink()

  static def add_web_sensor()
    var msg = ""
    try
      for sensor: global.walkerNodes
        var name = sensor[0]

        # If LoRaWanName contains WALKER use WALKER-<node>
        if string.find(name, "WALKER") > -1   
          name = string.format("WALKER-%i", sensor[1])
        end

        var name_tooltip = "Glamos Walker"

        var last_seen = sensor[2]
        var rssi = sensor[3]
        
        msg += lwdecode.header(name, name_tooltip, 1000, last_seen, rssi, last_seen)

        # Sensors
        var latitude      = sensor[4]
        var longitude     = sensor[5]
        var altitude      = sensor[6]
        var antenna       = sensor[7]
        var position      = sensor[8]
        var counter       = sensor[9] 
        var track_enabled = sensor[11] 
        var history       = sensor[12] 

        var latlon = format("&#x1F3AF; %.4f, %.4f", latitude, longitude)
        var map_link = format("https://www.google.com/maps/place/%.6f,%.6f",latitude,longitude)

        var te_value  = (track_enabled ? str(history.size()) : "") 
        var te_tt     = "Tracking " .. (track_enabled ? "On" : "Off")
        var te_icon   = (track_enabled ? "&#x1F534;" : "&#x26AA;")

        var fmt = LwSensorFormatter_cls()

        msg += fmt.start_line()
          .add_link( latlon, map_link )                                               # Direct History        🎯
          .next_line()
          .add_sensor("altitude", altitude,                 "Altitude" )              # Mountain              ⛰ 
          .add_sensor("string",   format("%d",   antenna),  "Antenna",  "&#x1F4E1;" ) # Satellite Antenna     📡
          .add_sensor("string",   format("%d",   position), "Position", "&#x1F4CD;" ) # Round Pushpin         📍
          .add_sensor("string",   format("%d",   counter),  "Counter",  "&#x23F1;"  ) # Chronometer           ⏱️
          .add_sensor("string",   te_value,                 te_tt,      te_icon)      # Track ON/OFF          🔴 ⚪
          .end_line()
          .get_msg()
      end
    except .. as e, m
        print(e .. ': ' .. m)
    end
    return msg
  end #add_web_sensor()
end #class

LwDeco = LwDecoWALKER
