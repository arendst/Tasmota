# LoRaWAN Decoder file for Dragino SN50v3-LB/LS
#
# References
#  User Manual:     https://wiki.dragino.com/xwiki/bin/view/Main/User%20Manual%20for%20LoRaWAN%20End%20Nodes/SN50v3-LB/
#  Codec Repository: https://github.com/dragino/dragino-end-node-decoder/tree/main/SN50_v3-LB

import string

if !global.DrgSN50v3LNodes      # data survive to decoder reload
  global.DrgSN50v3LNodes = {}
end

class LwDecoDrgSN50v3L
  def unload()
    global.undef("DrgSN50v3LNodes")
    global.undef("LwDecoDrgSN50v3L")
  end

  static def decodeUplink(Name, Node, RSSI, FPort, Bytes)
    var data = {"Device":"Dragino SN50v3-L"}
    
    var valid_values = false
    var last_seen = 1451602800
    var battery_last_seen = 1451602800
    var battery = 1000
    var rssi = RSSI
    var WorkingMode = ''
    var WorkingModes = ['IIC','Distance', '3ADC+IIC', '3DS18B20','Weight','1Count','3Interrupt','3ADC+1DS18B20','3DS18B20+2Count','PWM','TMP117','Count+SHT31']
    var mode = -1

    if global.DrgSN50v3LNodes.find(Node)
      last_seen         = global.DrgSN50v3LNodes.item(Node)[2]
      battery_last_seen = global.DrgSN50v3LNodes.item(Node)[3]
      battery           = global.DrgSN50v3LNodes.item(Node)[4]
      rssi              = global.DrgSN50v3LNodes.item(Node)[5]
    end

    ## SENSOR DATA ##
    if 2 == FPort && Bytes.size() > 10         # Variable length, depending on mode, but always 11 bytes or more
      valid_values = true
      last_seen = tasmota.rtc('local')  

      mode = (Bytes[6] & 0x7C) >> 2
      if (mode +1) > size(WorkingModes)
        mode = 0
      end
      WorkingMode = WorkingModes[mode]
      data.insert("WorkingMode", WorkingMode)  # mode in data = 0..11. Mode in documentation = 1..12

      battery = (Bytes[0]<<8 | Bytes[1])/1000.0
      data.insert("BattV", battery)
      battery_last_seen = tasmota.rtc('local')
		
      ### TBA - handle all of the many cases	
      if 0 == mode                             # Mode 1 (default)
        var temp_pc13 = 1000
        var adc_pa4 = -1
        var digital_status
        var digital_in_pb15
        var pb15_last_seen = 1451602800
        var digital_int_pa8
        var pa8_last_seen = 1451602800
        var illum_bh1750 = -1
        var temp_sht = 1000
        var humidity_sht
        if global.DrgSN50v3LNodes.find(Node)
          temp_pc13       = global.DrgSN50v3LNodes.item(Node)[8]
          adc_pa4         = global.DrgSN50v3LNodes.item(Node)[9]
          digital_status  = global.DrgSN50v3LNodes.item(Node)[10]
          digital_in_pb15 = global.DrgSN50v3LNodes.item(Node)[11]
          pb15_last_seen  = global.DrgSN50v3LNodes.item(Node)[12]
          digital_int_pa8 = global.DrgSN50v3LNodes.item(Node)[13]
          pa8_last_seen   = global.DrgSN50v3LNodes.item(Node)[14]
          illum_bh1750    = global.DrgSN50v3LNodes.item(Node)[15]
          temp_sht        = global.DrgSN50v3LNodes.item(Node)[16]
          humidity_sht    = global.DrgSN50v3LNodes.item(Node)[17]
        end

        if ((Bytes[2] != 0x7f) || (Bytes[3] != 0xFF))
          temp_pc13 = (Bytes[2]<<8 | Bytes[3])/10.0
          data.insert('TempC1', temp_pc13)
        end

        adc_pa4 = (Bytes[4]<<8 | Bytes[5])/1000.0
        data.insert('ADC1_V', adc_pa4)

        digital_status = (Bytes[6] & 0x01) ? 1 : 0
        data.insert('Digital_IStatus', (digital_status) ? 'TRUE':'FALSE')

        var last_state = digital_in_pb15
        digital_in_pb15 = (Bytes[6] & 0x02) ? 1 : 0
        data.insert('EXTI_Trigger', (digital_in_pb15) ? 'High':'Low')
        if last_state != digital_in_pb15
          pb15_last_seen = tasmota.rtc('local')
        end

        last_state = digital_int_pa8
        digital_int_pa8 = (Bytes[6] & 0x80) ? 1 : 0
        data.insert('Door_status', (digital_int_pa8) ? 'CLOSE':'OPEN')
        if last_state != digital_int_pa8
          pa8_last_seen = tasmota.rtc('local')
        end

        if((Bytes[9]<<8 | Bytes[10])==0)
          illum_bh1750 = (Bytes[7]<<8 | Bytes[8])
          data.insert('Illum', illum_bh1750)
        else 
          var noshowTemp = ((Bytes[7]==0x7f)&&(Bytes[8]==0xff))||((Bytes[7]==0xff)&&(Bytes[8]==0xff))
          if !noshowTemp
            temp_sht = (Bytes[7]<<24>>16 | Bytes[8])/10.0
            data.insert('TempC_SHT', temp_sht)
          end
          if((Bytes[9]!=0xff)||(Bytes[10]!=0xff))
            humidity_sht = (Bytes[9]<<8 | Bytes[10])/10.0
            data.insert('Hum_SHT', humidity_sht)
          end
        end

        if global.DrgSN50v3LNodes.find(Node)
          global.DrgSN50v3LNodes.remove(Node)
        end
        #                             sensor[0]    [1]   [2]        [3]                [4]      [5]   [6]          [7]   [8]        [9]      [10]            [11]             [12]            [13]             [14]           [15]          [16]      [17]
        global.DrgSN50v3LNodes.insert(Node, [Name, Node, last_seen, battery_last_seen, battery, RSSI, WorkingMode, mode, temp_pc13, adc_pa4, digital_status, digital_in_pb15, pb15_last_seen, digital_int_pa8, pa8_last_seen, illum_bh1750, temp_sht, humidity_sht])   
        valid_values = false                   # Already updated

      end # mode 0

    ## STATUS DATA ##
    elif 5 == FPort && Bytes.size() == 7
      valid_values = true
      data.insert("Sensor_Model",Bytes[0])
      data.insert("Firmware_Version", f'v{Bytes[1]:%u}.{Bytes[2]>>4:%u}.{Bytes[2]&0xF:%u}')
      data.insert("Freq_Band",LwRegions[Bytes[3]-1])
      data.insert("Sub_Band",Bytes[4])

      battery = ((Bytes[5]<<8) | Bytes[6])/1000.0
      data.insert("BattV", battery)
      battery_last_seen = tasmota.rtc('local')

    else
    # Ignore other Fports

    end #Fport

    if valid_values
      if global.DrgSN50v3LNodes.find(Node)
        global.DrgSN50v3LNodes.remove(Node)
      end
      mode = -1                                # Report incomplete list
      #                             sensor[0]    [1]   [2]        [3]                [4]      [5]   [6]          [7]
      global.DrgSN50v3LNodes.insert(Node, [Name, Node, last_seen, battery_last_seen, battery, RSSI, WorkingMode, mode])   
    end

    return data
  end #decodeUplink()

  static def add_web_sensor()
    var fmt = global.LwSensorFormatter_cls()
    var msg = ""
    for sensor: global.DrgSN50v3LNodes
      var name = sensor[0]
      if string.find(name, "SN50v3-L") > -1                              # If LoRaWanName contains SN50v3 use SN50v3-<node>
        name = string.format("SN50v3-L-%i", sensor[1])
      end
      var name_tooltip = "Dragino SN50v3-L"
      var last_seen         = sensor[2]
      var battery_last_seen = sensor[3]
      var battery           = sensor[4]
      var rssi              = sensor[5]
      var workingMode       = sensor[6]
      var mode              = sensor[7]

      msg += fmt.header(name, name_tooltip, battery, battery_last_seen, rssi, last_seen)

      # Sensors
      msg += "<tr class='htr'><td colspan='4'>&#9478;"                   # |

      if mode == 0
        var temp_pc13       = sensor[8]
        var adc_pa4         = sensor[9]
        var digital_status  = sensor[10]
        var digital_in_pb15 = sensor[11]
        var pb15_last_seen  = sensor[12]
        var digital_int_pa8 = sensor[13]
        var pa8_last_seen   = sensor[14]
        var illum_bh1750    = sensor[15]
        var temp_sht        = sensor[16]
        var humidity_sht    = sensor[17]

        if temp_pc13 < 1000
          msg += string.format(" &#x2600;&#xFE0F; %.1f°C", temp_pc13)    # Sunshine - Temperature ds18b20
        end
        if illum_bh1750 > 0
          msg += string.format(" &#x263C;&#xFE0F; %.1flx", illum_bh1750) # Sunshine - Illuminance BH1750
        end
        if temp_sht < 1000
          msg += string.format(" &#x2600;&#xFE0F; %.1f°C", temp_sht)     # Sunshine - Temperature SHT
          msg += string.format(" &#x1F4A7; %.1f%%", humidity_sht)        # Raindrop - Humidity SHT
        end
        msg += string.format(" %s1 %s", (digital_in_pb15) ? "&#x1F513;" : "&#x1F512;", # Open or Closed lock - Door
                                        fmt.dhm(pb15_last_seen))
        msg += string.format(" %s2 %s", (digital_int_pa8) ? "&#x1F513;" : "&#x1F512;", # Open or Closed lock - Door
                                        fmt.dhm(pa8_last_seen))
      else
        msg += string.format(" &#xFE0F; %s", workingMode)         
      end

      msg += "{e}"                                                       # = </td></tr>
    end
    return msg
  end #add_web_sensor()
end #class

global.LwDeco = LwDecoDrgSN50v3L
