#-
  Show optional devicename and/or version with topic/hostname and IP address and uptime from MQTT discovery and STATE messages in GUI

  Enable either
   self.line_option = 1  : Scroll 'line_cnt' lines
  or
   self.line_option = 2  : Show devices updating within 'line_teleperiod'
-#

import mqtt 
import string
import webserver
import persist

class mqttdata_cls
  var line_option                                   # Line option
  var line_cnt                                      # Number of lines
  var line_teleperiod                               # Skip any device taking longer to respond (probably offline)
  var line_highlight                                # Highlight latest change duration
  var line_highlight_color                          # Latest change highlight color
  var line_lowuptime_color                          # Low uptime highlight color
  var line_duration                                 # Duration option
  var line_topic_is_hostname                        # Treat topic as hostname
  var bool_devicename                               # Show device name
  var bool_version                                  # Show version
  var list_buffer                                   # Buffer storing lines
  var list_config                                   # Buffer storing retained config

  def init()
#    self.line_option = 1                            # Scroll line_cnt lines
    self.line_option = 2                            # Show devices updating within line_teleperiod

    self.line_cnt = 10                              # Option 1 number of lines to show
    self.line_teleperiod = 600                      # Option 2 number of teleperiod seconds for devices to be shown
    self.line_highlight = 10                        # Highlight latest change duration in seconds
    self.line_highlight_color = "yellow"            # Latest change highlight HTML color like "#FFFF00" or "yellow"
    self.line_lowuptime_color = "lime"              # Low uptime highlight HTML color like "#00FF00" or "lime"
    self.line_duration = 0                          # Show duration of last state message (1)
    self.line_topic_is_hostname = 0                 # Treat topic as hostname (1)
    self.bool_devicename = persist.std_devicename   # Show device name
    self.bool_version = persist.std_version         # Show version

    self.list_buffer = []                           # Init line buffer list
    self.list_config = []                           # Init retained config buffer list

    if global.mqttdata_driver
      global.mqttdata_driver.stop()                 # Let previous instance bail out cleanly
    end
    tasmota.add_driver(global.mqttdata_driver := self)

    # Assume default Fulltopic (%prefix%/%topic%/) and Prefix3 (tele)
    mqtt.subscribe("tele/#", /topic, idx, data, databytes -> self.handle_state_data(topic, idx, data, databytes))
    mqtt.subscribe("tasmota/discovery/+/config", /topic, idx, data, databytes -> self.handle_discovery_data(topic, idx, data, databytes))
  end

  def stop()
    mqtt.unsubscribe("tasmota/discovery/+/config")
    mqtt.unsubscribe("tele/#")                      # Assume default Fulltopic (%prefix%/%topic%/) and Prefix3 (tele)
    tasmota.remove_driver(self)
  end

  def handle_discovery_data(full_topic, idx, data, databytes)
    import json

    var config = json.load(data)
    if config
      # tasmota/discovery/142B2F9FAF38/config = {"ip":"192.168.2.208","dn":"AtomLite2","fn":["Tasmota",null,null,null,null,null,null,null],"hn":"atomlite2","mac":"142B2F9FAF38","md":"M5Stack Atom Lite","ty":0,"if":0,"cam":0,"ofln":"Offline","onln":"Online","state":["OFF","ON","TOGGLE","HOLD"],"sw":"15.0.1.4","t":"atomlite2","ft":"%prefix%/%topic%/","tp":["cmnd","stat","tele"],"rl":[2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"swc":[-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],"swn":[null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null],"btn":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"so":{"4":0,"11":0,"13":0,"17":0,"20":0,"30":0,"68":0,"73":0,"82":0,"114":0,"117":0},"lk":1,"lt_st":3,"bat":0,"dslp":0,"sho":[],"sht":[],"ver":1} (retained)
      var topic = config['t']
      var hostname = config['hn']
      var ipaddress = config['ip']
      var devicename = config['dn']
      var version = config['sw']
      var line = format("%s\001%s\001%s\001%s\001%s", topic, hostname, ipaddress, devicename, version)
#      tasmota.log(format("STD: 111 Size %03d, Topic '%s', Line '%s'", self.list_config.size(), topic, line), 3)
      if self.list_config.size()
        var list_index = 0
        var list_size = size(self.list_config)
        var topic_delim = format("%s\001", topic)   # Add find delimiter
        while list_index < list_size                # Use while loop as counter is decremented
          if 0 == string.find(self.list_config[list_index], topic_delim)
            self.list_config.remove(list_index)     # Remove current config
            list_size -= 1                          # Continue for duplicates
          end
          list_index += 1
        end
      end
      self.list_config.push(line)                   # Add (re-discovered) config as last entry
#      tasmota.log(format("STD: 222 Size %03d, Topic '%s', Line '%s'", self.list_config.size(), topic, line), 3)
    end
  end

  def handle_state_data(full_topic, idx, data, databytes)
    import json

    var subtopic = string.split(full_topic, "/")    # Assume default Fulltopic (%prefix%/%topic%/)
    if subtopic[-1] == "STATE"                      # tele/atomlite2/STATE
      var topic = subtopic[1]                       # atomlite2

      var topic_index = -1
      for i: self.list_config.keys()
        if 0 == string.find(self.list_config[i], topic)
          topic_index = i
          break
        end
      end
#      tasmota.log(format("STD: Topic '%s', Index %d, Size %d, Line '%s'", topic, topic_index, self.list_config.size(), self.list_config[topic_index]), 3)
      if topic_index == -1 return true end          # Assume topic is in retained discovery list

      var state = json.load(data)
      if state                                      # Valid JSON state message
        var config_splits = string.split(self.list_config[topic_index], "\001")
        topic = config_splits[1]                    # Hostname
        var ipaddress = config_splits[2]
        var devicename = config_splits[3]
        var version = config_splits[4]

        # tele/atomlite2/STATE = {"Time":"2025-09-24T14:13:00","Uptime":"0T00:15:09","UptimeSec":909,"Heap":142,"SleepMode":"Dynamic","Sleep":50,"LoadAvg":19,"MqttCount":1,"Berry":{"HeapUsed":12,"Objects":167},"POWER":"OFF","Dimmer":10,"Color":"1A0000","HSBColor":"0,100,10","Channel":[10,0,0],"Scheme":0,"Width":1,"Fade":"OFF","Speed":1,"LedTable":"ON","Wifi":{"AP":1,"SSId":"indebuurt_IoT","BSSId":"18:E8:29:CA:17:C1","Channel":11,"Mode":"HT40","RSSI":100,"Signal":-28,"LinkCount":1,"Downtime":"0T00:00:04"},"Hostname":"atomlite2","IPAddress":"192.168.2.208"}
        var uptime = state['Uptime']                # 0T00:15:09
        if state.find('Hostname')
          topic = state['Hostname']                 # atomlite2
          ipaddress = state['IPAddress']            # 192.168.2.208
        end
        var last_seen = tasmota.rtc('local')
        var line = format("%s\001%s\001%s\001%d\001%s\001%s", topic, ipaddress, uptime, last_seen, devicename, version)

        if self.list_buffer.size()
          var list_index = 0
          var list_size = size(self.list_buffer)
          var topic_delim = format("%s\001", topic) # Add find delimiter
          while list_index < list_size              # Use while loop as counter is decremented
            if 0 == string.find(self.list_buffer[list_index], topic_delim)
              self.list_buffer.remove(list_index)   # Remove current state
              list_size -= 1                        # Continue for duplicates
            end
            list_index += 1
          end
        end
        self.list_buffer.push(line)                 # Add state as last entry

      end
    end
    return true
  end

  def sort(l, cmp)                                  # Sort list
    for i:1..size(l)-1
      var k = l[i]
      var j = i
      while (j > 0) && !cmp(l[j-1], k)
        l[j] = l[j-1]
        j -= 1
      end
      l[j] = k
    end
    return l
  end

  def dhm(last_time)                                # Duration
    var since = tasmota.rtc('local') - last_time
    var unit = "d"
    if since > (24 * 3600)
      since /= (24 * 3600)
      if since > 99 since = 99 end
    elif  since > 3600
      since /= 3600
      unit = "h"
    else
      since /= 60
      unit = "m"
    end
    return format("%02d%s", since, unit)
  end

  def persist_save()
    persist.std_devicename = self.bool_devicename
    persist.std_version = self.bool_version
    persist.save()
#    tasmota.log("STD: Persist saved", 3)
  end

  def web_sensor()
    if webserver.has_arg("sd_dn")
      # Toggle display Devicename
      if self.bool_devicename self.bool_devicename = false else self.bool_devicename = true end
      self.persist_save()
    elif webserver.has_arg("sd_sw")
      # Toggle display software version
      if self.bool_version self.bool_version = false else self.bool_version = true end
      self.persist_save()
    end

    if self.list_buffer.size()
      var now = tasmota.rtc('local')
      var time_window = now - self.line_teleperiod
      var list_index = 0
      var list_size = size(self.list_buffer)
      while list_index < list_size
        var splits = string.split(self.list_buffer[list_index], "\001")
        var last_seen = int(splits[3])
        if time_window > last_seen                  # Remove offline devices
          self.list_buffer.remove(list_index)
          list_size -= 1
        end
        list_index += 1
      end
      if !list_size return end                      # If list became empty bail out

      if 2 == self.line_option
        var less = /a,b -> a < b
        self.sort(self.list_buffer, less)           # Sort list by topic and/or hostname
      end

      list_index = 0
      if 1 == self.line_option
        list_index = list_size - self.line_cnt      # Offset in list using self.line_cnt
        if list_index < 0 list_index = 0 end
      end
      var msg = "</table><table style='width:100%;font-size:80%'>" # Terminate two column table and open new table
      while list_index < list_size
        var splits = string.split(self.list_buffer[list_index], "\001")
        var topic = splits[0]                       # topic or hostname
        var ipaddress = splits[1]
        var uptime = splits[2]
        var last_seen = int(splits[3])
        var devicename = splits[4]
        var version = splits[5]

        msg += "<tr>"
        if self.bool_devicename
          msg += format("<td>%s&nbsp</td>", devicename)
        end
        if self.bool_version
          msg += format("<td>%s&nbsp</td>", version)
        end
        if ipaddress
          msg += format("<td><a target=_blank href='http://%s.'>%s&nbsp</a></td><td><a target=_blank href='http://%s'>%s&nbsp</a></td>",
                        topic, topic, ipaddress, ipaddress)
        else
          if self.line_topic_is_hostname
            msg += format("<td><a target=_blank href='http://%s.'>%s&nbsp</a></td><td>&nbsp</td>",
                          topic, topic)
          else
            msg += format("<td>%s&nbsp</td><td>&nbsp</td>", topic)
          end
        end

        var uptime_str = string.replace(uptime, "T", ":")  # 11T21:50:34 -> 11:21:50:34
        var uptime_splits = string.split(uptime_str, ":")
        var uptime_sec = (int(uptime_splits[0]) * 86400) + # 11 * 86400
                         (int(uptime_splits[1]) * 3600) +  # 21 * 3600
                         (int(uptime_splits[2]) * 60) +    # 50 * 60
                         int(uptime_splits[3])      # 34 
        if last_seen >= (now - self.line_highlight) # Highlight changes within latest seconds
          msg += format("<td align='right' style='color:%s'>%s</td>", self.line_highlight_color, uptime)
        elif uptime_sec < self.line_teleperiod      # Highlight changes just after restart
          msg += format("<td align='right' style='color:%s'>%s</td>", self.line_lowuptime_color, uptime)
        else 
          msg += format("<td align='right'>%s</td>", uptime)
        end

        if self.line_duration
          msg += format("<td style='font-size:90%%'>&#x1F557;%s</td>",  # Clock
                        self.dhm(last_seen))
        end
        msg += "</tr>"
        list_index += 1
      end
      msg += "</table>{t}"                          # Terminate three/four/five column table and open new table: <table style='width:100%'>
      msg += format("{s}Devices online{m}%d{e}", list_size) # <tr><th>Devices online</th><td style='width:20px;white-space:nowrap'>%d</td></tr>

      msg += "</table><p></p>{t}"                   # Terminate two column table and open new table: <table style='width:100%'>
      msg += "<td style=\"width:50%\"><button onclick='la(\"&sd_dn=1\");'>Device Name</button></td>"
      msg += "<td style=\"width:50%\"><button onclick='la(\"&sd_sw=1\");'>Version</button></td>"
      msg += "</table>{t}"                          # Terminate two column table and open new table: <table style='width:100%'>

      tasmota.web_send(msg)                         # Do not use tasmota.web_send_decimal() which will replace IPAddress dots
      tasmota.web_send_decimal("")                  # Force horizontal line
    end
  end

end

mqttdata = mqttdata_cls()
