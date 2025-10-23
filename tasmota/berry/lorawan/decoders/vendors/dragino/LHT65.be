# LoRaWAN Decoder file for Dragino LHT65
#
# References
#  User Manual:     https://www.dragino.com/downloads/downloads/LHT65/UserManual/LHT65_Temperature_Humidity_Sensor_UserManual_v1.8.5.pdf
#  TTN Device Repository: https://github.com/TheThingsNetwork/lorawan-devices/blob/master/vendor/dragino/lht65.js

import string
var LHT65_BatteryStatus = ["Very low <= 2.5V","Low <=2.55V","OK","Good >= 2.65V"]

if !global.lht65Nodes      # data survive to decoder reload
  global.lht65Nodes = {}
end

class LwDecoLHT65
  static def decodeUplink(Name, Node, RSSI, FPort, Bytes)
    var data = {"Device":"Dragino LHT65"}

    var valid_values = false
    var last_seen = 1451602800
    var battery_last_seen = 1451602800
    var battery = 1000
    var rssi = RSSI
    var temp_int = 1000
    var humidity
    var temp_ext = 1000
    var door_open = 1000
    var door_open_last_seen = 1451602800
    if global.lht65Nodes.find(Node)
      last_seen           = global.lht65Nodes.item(Node)[2]
      battery_last_seen   = global.lht65Nodes.item(Node)[3]
      battery             = global.lht65Nodes.item(Node)[4]
      RSSI                = global.lht65Nodes.item(Node)[5]
      temp_int            = global.lht65Nodes.item(Node)[6]
      humidity            = global.lht65Nodes.item(Node)[7]
      temp_ext            = global.lht65Nodes.item(Node)[8]
      door_open           = global.lht65Nodes.item(Node)[9]
      door_open_last_seen = global.lht65Nodes.item(Node)[10]
    end

    var Ext = Bytes[6] & 0x0F  #External sensor type
    var NoConnect = (Bytes[6] & 0x80) >> 7

    ## SENSOR DATA ##
    data.insert("poll_message_status",(Bytes[6] & 0x40) >> 6)

    if 2 == FPort && Bytes.size() == 11
      var TempC 

      if Ext == 9 #Sensor E3, Temperature Sensor, Datalog Mod
        last_seen = tasmota.rtc('local')
        TempC = ((Bytes[0] << 8) | Bytes[1])
        if 0x7FFF == TempC
          data.insert("Ext_SensorConnected", false)
        else
          data.insert("Ext_SensorConnected", true)
          if Bytes[0]>0x7F
            TempC -= 0x10000
          end
          temp_ext = TempC / 100.0
          data.insert("TempC_External", temp_ext)
          valid_values = true
        end
        data.insert("Bat_status", LHT65_BatteryStatus[Bytes[4] >> 6])
      else 
        data.insert("BattV",(((Bytes[0] << 8) | Bytes[1]) & 0x3fff) / 1000.0)
        battery_last_seen = tasmota.rtc('local')
        battery = (((Bytes[0] << 8) | Bytes[1]) & 0x3fff) / 1000.0
        data.insert("Bat_status", LHT65_BatteryStatus[Bytes[0] >> 6])
        valid_values = true
      end
      
      if Ext != 0x0F 
        last_seen = tasmota.rtc('local')
        TempC = ((Bytes[2] << 8) | Bytes[3])
        if Bytes[2]>0x7F
          TempC -= 0x10000
        end	 
        temp_int = TempC / 100.0
        data.insert("TempC_Internal", temp_int)
        humidity = (((Bytes[4] << 8) | Bytes[5]) / 10.0)
        data.insert("Hum_Internal"  , humidity)
        valid_values = true
      end
      
      if NoConnect
        data.insert('No_connect','No connection to external sensor')
      end
    
      if 0 == Ext
        data.insert("Ext_sensor", 'No external sensor')
      elif 1==Ext
        last_seen = tasmota.rtc('local')
        data.insert("Ext_sensor",'Temperature Sensor')
        TempC = ((Bytes[7] << 8) | Bytes[8])
        if 0x7FFF == TempC
          data.insert("Ext_SensorConnected", false)
        else
          data.insert("Ext_SensorConnected", true)
          if Bytes[7]>0x7F
            TempC -= 0x10000
          end
          temp_ext = TempC / 100.0
          data.insert("TempC_External", temp_ext)
          valid_values = true
        end		
      elif 4 == Ext
        last_seen = tasmota.rtc('local')
        data.insert("Work_mode", 'Interrupt Sensor send')
        door_open = ( Bytes[7] ) ? 0 : 1    # DS sensor
        data.insert("Exti_pin_level", Bytes[7] ? 'High' : 'Low')
        data.insert("Exti_status", Bytes[8] ? 'True' : 'False')
        if Bytes[8]
          door_open_last_seen = tasmota.rtc('local')
        end
        valid_values = true
      elif 5 == Ext
        data.insert("Work_mode", 'Illumination Sensor')
        data.insert("ILL_lx", (Bytes[7] << 8) | Bytes[8])
      elif 6 == Ext
        data.insert("Work_mode", 'ADC Sensor')
        data.insert("ADC_V", ((Bytes[7] << 8) | Bytes[8]) / 1000.0)
      elif 7 == Ext
        data.insert("Work_mode", 'Interrupt Sensor count')
        data.insert("Exit_count", (Bytes[7] << 8) | Bytes[8])
      elif 8 == Ext
        data.insert("Work_mode", 'Interrupt Sensor count')
        data.insert("Exit_count", (Bytes[7] << 24) | (Bytes[8] << 16) | (Bytes[9] << 8) | Bytes[10])
      elif 9 == Ext
        data.insert("Work_mode", 'DS18B20 & timestamp')
        var epoch = (Bytes[7] << 24) | (Bytes[8] << 16) | (Bytes[9] << 8) | Bytes[10]
        data.insert("Systimestamp",tasmota.time_str(epoch))
      elif 15 == Ext
        data.insert("Work_mode",'DS18B20ID')
        data.insert("ID",f"{Bytes[2]:%02X}" + f"{Bytes[3]:%02X}" + f"{Bytes[4]:%02X}" + f"{Bytes[5]:%02X}" + f"{Bytes[6]:%02X}" + f"{Bytes[8]:%02X}" + f"{Bytes[9]:%02X}" + f"{Bytes[10]:%02X}" ) 
      else
        data.insert("Ext_sensor", 'Unknown')
      end   
    
    elif 5 == FPort && Bytes.size() == 7
      data.insert("Sensor_Model",Bytes[0])
      data.insert("Firmware_Version", f'v{Bytes[1]:%u}.{Bytes[2]>>4:%u}.{Bytes[2]&0xF:%u}')
      data.insert("Freq_Band",LwRegions[Bytes[3]-1])
      data.insert("Sub_Band",Bytes[4])
      data.insert("BattV",((Bytes[5] << 8) | Bytes[6]) / 1000.0)
      battery_last_seen = tasmota.rtc('local')
      battery = ((Bytes[5] << 8) | Bytes[6]) / 1000.0
      valid_values = true
    else
      # Ignore other Fports  
    end #Fport 

    if valid_values
      if global.lht65Nodes.find(Node)
        global.lht65Nodes.remove(Node)
      end
      #                         sensor[0]   [1]   [2]        [3]                [4]      [5]   [6]       [7]       [8]       [9]        [10]
      global.lht65Nodes.insert(Node, [Name, Node, last_seen, battery_last_seen, battery, rssi, temp_int, humidity, temp_ext, door_open, door_open_last_seen])
    end

    return data
  end # decodeUplink()

  static def add_web_sensor()
    var msg = ""
    for sensor: global.lht65Nodes
      var name = sensor[0]
      if string.find(name, "LHT65") > -1                                 # If LoRaWanName contains LHT65 use LHT65-<node>
        name = string.format("LHT65-%i", sensor[1])
      end
      var name_tooltip = "Dragino LHT65"
      var last_seen = sensor[2]
      var battery_last_seen = sensor[3]
      var battery = sensor[4]
      var rssi = sensor[5]
      msg += lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_seen)

      # Sensors
      var temp_int = sensor[6]
      var humidity = sensor[7]
      var temp_ext = sensor[8]
      var door_open = sensor[9]
      var door_open_last_seen = sensor[10]
      msg += "<tr class=\"htr\"><td colspan=\"4\">&#9478;"               # |
      if temp_int < 1000
        msg += string.format(" &#x2600;&#xFE0F; %.1f°C", temp_int)       # Sunshine - Temperature
        msg += string.format(" &#x1F4A7; %.1f%%", humidity)              # Raindrop - Humidity
      end
      if temp_ext < 1000
        msg += string.format(" &#x2600;&#xFE0F; ext %.1f°C", temp_ext)   # Sunshine - Temperature external
      end
      if door_open < 1000
        msg += string.format(" %s %s", (door_open) ? "&#x1F513" : "&#x1F512", # Open or Closed lock - Door
                                       lwdecode.dhm(door_open_last_seen))
      end
      msg += "{e}"                                                       # = </td></tr>
    end
    return msg
  end #add_web_sensor()
end # class

LwDeco = LwDecoLHT65