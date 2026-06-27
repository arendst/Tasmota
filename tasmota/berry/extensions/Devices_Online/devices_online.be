###################################################################################
# devices_online.be - ESP32 Extension for Tasmota
#
# SPDX-FileCopyrightText: 2025 Stephan Hadinger & Theo Arends
#
# SPDX-License-Identifier: GPL-3.0-only
###################################################################################
# Display in Main GUI Devices Online based on non-invasive MQTT information from 
#  retained Tasmota Discovery Config and STATE reports.
# Enable Tasmota Discovery with command `SetOption19 0`.
# For realtime power control do execute once command `SetOption59 1` on every
#  online device (ex. `publish cmnd/tasmotas/so59 1`).
#  This will send a STATE report on every POWER change.
###################################################################################
# rm Devices_Online.tapp; zip -j -0 Devices_Online.tapp Devices_Online/*
###################################################################################

import mqtt 
import json
import string
import webserver
import persist

###################################################################################
# Display Configuration Devices Online GUI
#---------------------------------------------------------------------------------#
class dev_online_settings
  #################################################################################
  # init
  #
  # install the extension and allocate all resources
  #################################################################################
  def init()
    tasmota.add_driver(self)
    if tasmota.is_network_up()
      self.web_add_handler()                        # If init is called after the network is up, `web_add_handler` event is not fired
    end
  end

  def close()
    webserver.remove_route("/dvo")
    tasmota.remove_driver(self)
  end

  def web_add_config_button()
    webserver.content_send("<p></p><form id=ac action='dvo' style='display: block;' method='get'><button>Devices Online</button></form>")
  end

  def page_dev_online()
    if !webserver.check_privileged_access() return nil end

    if webserver.has_arg('save')
      persist.dvo_lines = webserver.arg('dvo11')
      persist.dvo_online_window = webserver.arg('dvo12')
      persist.dvo_time_highlight = webserver.arg('dvo13')
      persist.dvo_offline = str(int(webserver.arg('dvo15')))  # Only allow numbers
      persist.dvo_hostname_suffix = webserver.arg('dvo14')
      persist.dvo_devicename = (webserver.arg('dvo1')) ? 1 : 0
      persist.dvo_ipaddress = (webserver.arg('dvo3')) ? 1 : 0
      persist.dvo_power = (webserver.arg('dvo8')) ? 1 : 0
      persist.dvo_version = (webserver.arg('dvo2')) ? 1 : 0
      persist.dvo_heap = (webserver.arg('dvo6')) ? 1 : 0
      persist.dvo_berryheap = (webserver.arg('dvo4')) ? 1 : 0
      persist.dvo_berryobject = (webserver.arg('dvo7')) ? 1 : 0
      persist.dvo_wifirssi = (webserver.arg('dvo5')) ? 1 : 0
      persist.dvo_wifichnl = (webserver.arg('dvo9')) ? 1 : 0
      persist.save()

      webserver.redirect("/cn?")                    # Go back to Configuration menu
    end

    var dvo_lines = persist.find("dvo_lines", 0)
    var dvo_online_window = persist.find("dvo_online_window", 600)
    var dvo_time_highlight = persist.find("dvo_time_highlight", 10)
    var dvo_offline = persist.find("dvo_offline", 600)
    var dvo_hostname_suffix = persist.find("dvo_hostname_suffix", "")
    var dvo_devicename = (persist.find("dvo_devicename", 0)) ? " checked" : ""
    var dvo_ipaddress = (persist.find("dvo_ipaddress", 0)) ? " checked" : ""
    var dvo_power = (persist.find("dvo_power", 0)) ? " checked" : ""
    var dvo_version = (persist.find("dvo_version", 0)) ? " checked" : ""
    var dvo_heap = (persist.find("dvo_heap", 0)) ? " checked" : ""
    var dvo_berryheap = (persist.find("dvo_berryheap", 0)) ? " checked" : ""
    var dvo_berryobject = (persist.find("dvo_berryobject", 0)) ? " checked" : ""
    var dvo_wifirssi = (persist.find("dvo_wifirssi", 0)) ? " checked" : ""
    var dvo_wifichnl = (persist.find("dvo_wifichnl", 0)) ? " checked" : ""

    webserver.content_start("DevOnline")            #- title of the web page -#
    webserver.content_send_style()                  #- send standard Tasmota styles -#

    webserver.content_send(
     f"<fieldset>"
        "<legend><b>&nbsp;Devices Online&nbsp;</b></legend>"
        "<form method='get' action='dvo'>"
        "<p></p>"
        "<table>"
          "<tr>"
            "<td style='width:250px'><b>Scroll display lines</b></td>"
            "<td style='width:80px'><input id='dvo11' name='dvo11' type='number' min='0' max='60' step='2' value='{dvo_lines}'></td>"
          "</tr>"
          "<tr>"
            "<td style='width:250px'><b>Online window (sec)</b></td>"
            "<td style='width:80px'><input id='dvo12' name='dvo12' type='number' min='300' max='1000' step='30' value='{dvo_online_window}'></td>"
          "</tr>"
          "<tr>"
            "<td style='width:250px'><b>Highlight refreshed (sec)</b></td>"
            "<td style='width:80px'><input id='dvo13' name='dvo13' type='number' min='10' max='60' step='2' value='{dvo_time_highlight}'></td>"
          "</tr>"
          "<tr>"
            "<td style='width:250px'><b>Keep offline visible (sec)</b></td>"
            "<td style='width:80px'><input id='dvo15' name='dvo15' value='{dvo_offline}'></td>"
          "</tr>"
          "<tr>"
            "<td style='width:250px'><b>Hostname suffix</b></td>"
            "<td style='width:80px'><input id='dvo14' name='dvo14' value='{dvo_hostname_suffix}'></td>"
          "</tr>"
        "</table>"
        "<p></p>"
    )

    webserver.content_send(
       f"<fieldset>"
          "<legend><b>&nbsp;Columns Shown&nbsp;</b></legend>"
          "<table>"
            "<tr>"
              "<td style=width:40px'><input id='dvo1' name='dvo1' type='checkbox'{dvo_devicename}></td>"
              "<td><label for='dvo1'><b>Device Name</b></label></td>"
            "</tr>"
            "<tr>"
              "<td style=width:40px'> </td>"
              "<td><b>Hostname</b></td>"
            "</tr>"
            "<tr>"
              "<td style=width:40px'><input id='dvo3' name='dvo3' type='checkbox'{dvo_ipaddress}></td>"
              "<td><label for='dvo3'><b>IP Address</b></label></td>"
            "</tr>"
            "<tr>"
              "<td style=width:40px'><input id='dvo8' name='dvo8' type='checkbox'{dvo_power}></td>"
              "<td><label for='dvo8'><b>Power Control</b></label></td>"
            "</tr>"
            "<tr>"
              "<td style=width:40px'><input id='dvo2' name='dvo2' type='checkbox'{dvo_version}></td>"
              "<td><label for='dvo2'><b>Version</b></label></td>"
            "</tr>"
            "<tr>"
              "<td style=width:40px'><input id='dvo6' name='dvo6' type='checkbox'{dvo_heap}></td>"
              "<td><label for='dvo6'><b>Heap Free (KB)</b></label></td>"
            "</tr>"
            "<tr>"
              "<td style=width:40px'><input id='dvo4' name='dvo4' type='checkbox'{dvo_berryheap}></td>"
              "<td><label for='dvo4'><b>Berry Heap Usage (KB)</b></label></td>"
            "</tr>"
            "<tr>"
              "<td style=width:40px'><input id='dvo7' name='dvo7' type='checkbox'{dvo_berryobject}></td>"
              "<td><label for='dvo7'><b>Berry Object Count</b></label></td>"
            "</tr>"
            "<tr>"
              "<td style=width:40px'><input id='dvo5' name='dvo5' type='checkbox'{dvo_wifirssi}></td>"
              "<td><label for='dvo5'><b>WiFi RSSI</b></label></td>"
            "</tr>"
            "<tr>"
              "<td style=width:40px'><input id='dvo9' name='dvo9' type='checkbox'{dvo_wifichnl}></td>"
              "<td><label for='dvo9'><b>WiFi Channel</b></label></td>"
            "</tr>"
            "<tr>"
              "<td style=width:40px'> </td>"
              "<td><b>Uptime</b></td>"
            "</tr>"
          "</table>"
       "</fieldset>"
       "<br>"
       "<button name='save' type='submit' class='button bgrn'>Save</button>"
      "</form>"
      "</fieldset>"
    )

    webserver.content_button(webserver.BUTTON_CONFIGURATION) #- button back to conf page -#
    webserver.content_stop()                                 #- end of web page -#
  end

  def web_add_handler()
    webserver.on("/dvo", / -> self.page_dev_online())
  end
end
#---------------------------------------------------------------------------------#
# Display Configuration Devices Online GUI
###################################################################################


###################################################################################
# extension devices_online
#---------------------------------------------------------------------------------#
class devices_online
  var mqtt_state                                    # MQTT tele STATE subscribe format
  var mqtt_topic_idx                                # Index of %topic% within full topic
  var mqtt_step                                     # MQTT message state
  var every_second_counter
  var sort_last_column                              # Sort last column
  var list_devices                                  # Buffer storing devices
  var dvo_settings

  #################################################################################
  # init
  #
  # install the extension and allocate all resources
  #################################################################################
  def init()
    if !persist.has("dvo_column")
      persist.dvo_lines = 0                         # Show growing list of devices
      persist.dvo_online_window = 600               # Number of teleperiod seconds for devices to be shown as online
      persist.dvo_time_highlight = 10               # Highlight latest change duration in seconds
      persist.dvo_offline = 600                     # Number of seconds for devices to be shown as offline
      persist.dvo_hostname_suffix = ""              # Optional hostname suffix like "."
      persist.dvo_devicename = 0                    # Show device name
      persist.dvo_ipaddress = 0                     # Show IP address
      persist.dvo_power = 0                         # Show power
      persist.dvo_version = 0                       # Show version
      persist.dvo_heap = 0                          # Show Heap (KB)
      persist.dvo_berryheap = 0                     # Show Berry HeapUsed (KB)
      persist.dvo_berryobject = 0                   # Show Berry Object Count
      persist.dvo_wifirssi = 0                      # Show WiFi RSSI (%)
      persist.dvo_column = 0                        # Sort column, default Hostname
      persist.dvo_direction = 0                     # Sort direction (0) Up or (1) Down, default Up
      persist.save()
    end
    self.sort_last_column = persist.dvo_column      # Sort last column to detect direction toggle
    self.every_second_counter = 0

    self.list_devices = []                          # Init device buffer list

    var parts = string.split(tasmota.cmd('_FullTopic', true)['FullTopic'], '/')
    var prefix3 = tasmota.cmd("Prefix", true)['Prefix3'] # tele = Prefix3 used by STATE message
    self.mqtt_topic_idx = -1
    for ix : 0..size(parts)-1
      var level = parts[ix]
      if level == '%prefix%' 
        parts[ix] = prefix3
      elif level == '%topic%'
        parts[ix] = '+'
        self.mqtt_topic_idx = ix
      elif level == ''
        parts[ix] = 'STATE'
      else
        parts[ix] = '+'
      end
    end
    self.mqtt_state = parts.concat('/')             # default = tele/+/STATE

    if self.mqtt_topic_idx == -1
      log("DVO: ERROR No %topic% in FullTopic defined", 1)
      return
    end

    tasmota.add_driver(self)

    mqtt.subscribe(self.mqtt_state, /topic, idx, data, databytes -> self.handle_state_data(topic, idx, data, databytes))
    mqtt.subscribe("tasmota/discovery/+/config", /topic, idx, data, databytes -> self.handle_discovery_data(topic, idx, data, databytes))

    self.mqtt_step = 0
    if !mqtt.connected()
      log("DVO: Need MQTT connected", 1)
    end

    self.dvo_settings = dev_online_settings()
  end

  #################################################################################
  # unload
  #
  # Uninstall the extension and deallocate all resources
  #################################################################################
  def unload()
    mqtt.unsubscribe("tasmota/discovery/+/config")
    mqtt.unsubscribe(self.mqtt_state)
    self.dvo_settings.close()
    tasmota.remove_driver(self.dvo_settings)
    tasmota.remove_driver(self)
    self.list_devices.clear()
  end

  #################################################################################
  # handle_discovery_data(discovery_topic, idx, data, databytes)
  #
  # Handle retained MQTT Tasmota Discovery Config data
  #
  # A restarted device also provides retained Tasmota Discovery Config data but
  #  that will be received AFTER a restart STATE message therefor we need to handle
  #  discovery data as an update to already received STATE data
  #################################################################################
  def handle_discovery_data(discovery_topic, idx, data, databytes)
    if self.mqtt_step == 0
      self.mqtt_step = 1
      log("DVO: Discovery started...", 3)
#      log(format("DVO: Devices size %d, Memory %s", self.list_devices.size(), tasmota.memory()), 3)
    end
    var config = json.load(data)
    if config
      # tasmota/discovery/142B2F9FAF38/config = {"ip":"192.168.2.208","dn":"AtomLite2","fn":["Tasmota",null,null,null,null,null,null,null],"hn":"atomlite2","mac":"142B2F9FAF38","md":"M5Stack Atom Lite","ty":0,"if":0,"cam":0,"ofln":"Offline","onln":"Online","state":["OFF","ON","TOGGLE","HOLD"],"sw":"15.0.1.4","t":"atomlite2","ft":"%prefix%/%topic%/","tp":["cmnd","stat","tele"],"rl":[2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"swc":[-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],"swn":[null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null],"btn":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"so":{"4":0,"11":0,"13":0,"17":0,"20":0,"30":0,"68":0,"73":0,"82":0,"114":0,"117":0},"lk":1,"lt_st":3,"bat":0,"dslp":0,"sho":[],"sht":[],"ver":1} (retained)
      var topic = config['t']
      var hostname = config['hn']
      var ipaddress = config['ip']
      var devicename = config['dn']
      var version = config['sw']
      var version_splits = string.split(version, ".")
      var version_int = 0
      var multiplier = 0x1000000
      for split : version_splits
        version_int += int(split) * multiplier
        if multiplier
          multiplier /= 0x100
        end
      end
      var version_num = format("%011i", version_int) # 00235143427 - Convert to string to enable multicolumn sort
      var last_seen = str(tasmota.rtc('local'))
      var uptime = " "
      var uptime_sec = " "
      var heap = " "
      var berryheap = " "
      var berryobject = " "
      var wifirssi = " "
      var wifichnl = " "
      var power = []

      #           0      1         2          3           4        5            6          7       8           9          10        11    12           13     14
      var line = [topic, hostname, ipaddress, devicename, version, version_num, last_seen, uptime, uptime_sec, berryheap, wifirssi, heap, berryobject, power, wifichnl]
      var update = 0;
      for i: self.list_devices.keys()
        if self.list_devices[i][0] == topic
          update = 1
          if self.list_devices[i][8] != " "         # Process posible received STATE data
            uptime = self.list_devices[i][7]
            uptime_sec = self.list_devices[i][8]
            berryheap = self.list_devices[i][9]
            wifirssi = self.list_devices[i][10]
            heap = self.list_devices[i][11]
            berryobject = self.list_devices[i][12]
            power = self.list_devices[i][13]
            wifichnl = self.list_devices[i][14]
          end  
#          log(f"DVO: Discovery --- update {self.list_devices[i]}", 3)
          var update_line = [topic, hostname, ipaddress, devicename, version, version_num, last_seen, uptime, uptime_sec, berryheap, wifirssi, heap, berryobject, power, wifichnl]
#          log(f"DVO: Discovery +++ update {update_line}", 3)
          self.list_devices[i] = update_line        # Update current config
          break
        end
      end
      if update == 0
#        log(f"DVO: Discovery +++ add    {line}", 3)
        self.list_devices.push(line)                # Add (re-discovered) config as last entry
      end
    end
    return true                                     # return true to stop propagation as a Tasmota cmd
  end

  #################################################################################
  # handle_state_data(tele_topic, idx, data, databytes)
  #
  # Handle MQTT STATE data
  #################################################################################
  def handle_state_data(tele_topic, idx, data, databytes)
    if self.mqtt_step == 1 
      self.mqtt_step = 2
      log("DVO: Discovery complete", 3)
    end
#    log(f"DVO: STATE topic '{tele_topic}'", 4)
    var subtopic = string.split(tele_topic, "/")
    if subtopic[-1] == "STATE"                      # we are only serving topic ending in STATE
      var topic = subtopic[self.mqtt_topic_idx]
      var state = json.load(data)                   # Assume topic is in retained discovery list
      if state                                      # Valid JSON state message

        var hostname = " "
        var ipaddress = " "
        var devicename = " "
        var version = " "
        var version_num = " "
        var topic_index = -1
        for i: self.list_devices.keys()
          if self.list_devices[i][0] == topic
            hostname = self.list_devices[i][1]
            ipaddress = self.list_devices[i][2]
            devicename = self.list_devices[i][3]
            version = self.list_devices[i][4]
            version_num = self.list_devices[i][5]
            topic_index = i
            break
          end
        end

        # tele/atomlite2/STATE = {"Time":"2025-09-24T14:13:00","Uptime":"0T00:15:09","UptimeSec":909,"Heap":142,"SleepMode":"Dynamic","Sleep":50,"LoadAvg":19,"MqttCount":1,"Berry":{"HeapUsed":12,"Objects":167},"POWER":"OFF","Dimmer":10,"Color":"1A0000","HSBColor":"0,100,10","Channel":[10,0,0],"Scheme":0,"Width":1,"Fade":"OFF","Speed":1,"LedTable":"ON","Wifi":{"AP":1,"SSId":"indebuurt_IoT","BSSId":"18:E8:29:CA:17:C1","Channel":11,"Mode":"HT40","RSSI":100,"Signal":-28,"LinkCount":1,"Downtime":"0T00:00:04"},"Hostname":"atomlite2","IPAddress":"192.168.2.208"}
        if state.find('Hostname')
          # > Tasmota v15.0.1.2
          hostname = state['Hostname']              # atomlite2
          ipaddress = state['IPAddress']            # 192.168.2.208
        end

        var power = []                              # [0]
        var power_state = ""
        for i: 1..4
          if i == 1
            if state.find("POWER")
              power_state = state["POWER"]
            end
          end
          var keystr = f"POWER{i}"
          if state.find(keystr)
            power_state = state[keystr]
          end
          if size(power_state)
            power.push((power_state == "OFF") ? 0 : 1)
            power_state = ""
          end
        end

        var last_seen = str(tasmota.rtc('local'))
        var uptime = state['Uptime']                # 0T00:15:09
        var uptime_sec_int
        if state.find('UptimeSec')
          uptime_sec_int = state['UptimeSec']       # 909
        else
          # < Tasmota v7.0.0.0
          var uptime_str = string.replace(uptime, "T", ":")  # 0T00:15:09 -> 0:00:15:09
          var uptime_splits = string.split(uptime_str, ":")
          uptime_sec_int = (int(uptime_splits[0]) * 86400) + # 0 * 86400
                           (int(uptime_splits[1]) * 3600) +  # 00 * 3600
                           (int(uptime_splits[2]) * 60) +    # 15 * 60
                           int(uptime_splits[3])    # 09 
        end
        var uptime_sec = format("%011d", uptime_sec_int) # 00000000909 - Convert to string to enable multicolumn sort

        var iheap = 0
        if state.find('Heap')
          iheap = state['Heap']                     # 142
        end
        var heap = format("%05i", iheap)            # 00142 - Convert to string to enable multicolumn sort

        var bheap = 0
        if state.find('Berry') && state['Berry'].find('HeapUsed')
          bheap = state['Berry']['HeapUsed']        # 12
        end
        var berryheap = format("%05i", bheap)       # 00012 - Convert to string to enable multicolumn sort

        var bobject = 0
        if state.find('Berry') && state['Berry'].find('Objects')
          bobject = state['Berry']['Objects']       # 167
        end
        var berryobject = format("%05i", bobject)   # 00167 - Convert to string to enable multicolumn sort

        var wrssi = 0
        if state.find('Wifi') && state['Wifi'].find('RSSI')
          wrssi = state['Wifi']['RSSI']             # 100
        end
        var wifirssi = format("%03i", wrssi)        # 100 - Convert to string to enable multicolumn sort

        var wchnl = 0
        if state.find('Wifi') && state['Wifi'].find('Channel')
          wchnl = state['Wifi']['Channel']          # 11
        end
        var wifichnl = format("%03i", wchnl)        # 011 - Convert to string to enable multicolumn sort

        #           0      1         2          3           4        5            6          7       8           9          10        11    12           13     14
        var line = [topic, hostname, ipaddress, devicename, version, version_num, last_seen, uptime, uptime_sec, berryheap, wifirssi, heap, berryobject, power, wifichnl]

        var dvo_online_window = int(persist.find("dvo_online_window", 600))
        var dvo_offline = int(persist.find("dvo_offline", 600))
        var time_window = int(last_seen) - dvo_online_window
        var update = 0;
        var list_size = size(self.list_devices)
        if list_size > 0
          var list_index = 0
          while list_index < list_size              # Use while loop as counter is decremented
            var list_last_seen = int(self.list_devices[list_index][6])
            var list_uptime_sec = int(self.list_devices[list_index][8]) # Is incremental negative if offline
            var time_window_offline = dvo_offline + list_uptime_sec     # Is negative if offline for over dvo_offline seconds
            var list_heap = int(self.list_devices[list_index][11])
            if update == 0 && 
               self.list_devices[list_index][0] == topic
              update = 1
#              log(f"DVO:     State --- update {self.list_devices[list_index]}", 3)
              self.list_devices[list_index] = line  # Update state
#              log(f"DVO:     State +++ update {line}", 3)
            elif self.list_devices[list_index][1] == hostname ||      # Remove duplicate hostname
                 self.list_devices[list_index][2] == ipaddress ||     # Remove duplicate IP address
                 (list_heap == 0 && time_window > list_last_seen) ||  # Remove offlines never seen
                 time_window_offline < 0                              # Remove offlines after user visibility
#              log(f"DVO:     State --- delete {self.list_devices[list_index]}", 3)
              self.list_devices.remove(list_index)  # Remove duplicates or offline device
              list_size -= 1
              list_index -= 1
            end
            list_index += 1
          end
        end
        if update == 0
#          log(f"DVO:     State +++ add    {line}", 3)
          self.list_devices.push(line)              # Add state as last entry
        end
#        log(format("DVO: Device size %d, Memory %s", self.list_devices.size(), tasmota.memory()), 3)
      end
    end
    return true                                     # return true to stop propagation as a Tasmota cmd
  end

  #################################################################################
  # uptime_to_str(uptime_sec_int)
  #
  # return uptime string like "-0T01:02:03"
  #################################################################################
  def uptime_to_str(uptime_sec_int)
    var seconds = uptime_sec_int
    var sign = ""
    if seconds < 0
      sign = "-"
      seconds *= -1
    end
    var days = seconds / 86400
    seconds -= days * 86400
    var hours = seconds / 3600
    seconds -= hours * 3600
    var minutes = seconds / 60
    seconds -= minutes * 60
    var uptime = f"{sign}{days}T{hours:02d}:{minutes:02d}:{seconds:02d}"
    return uptime
  end

  #################################################################################
  # every_second()
  #
  # Update offline devices
  #################################################################################
  def every_second()
    if self.mqtt_step < 2 return end                # Do not do anything until discovery is complete

    self.every_second_counter += 1
    if self.every_second_counter >= 60
      self.every_second_counter = 0

      # Update visible offline devices
      var dvo_offline = int(persist.find("dvo_offline", 600))
      if (dvo_offline > 0)
        var dvo_online_window = int(persist.find("dvo_online_window", 600))
        var now = tasmota.rtc('local')
        for i: self.list_devices.keys()
          #  0      1         2          3           4        5            6          7       8           9          10        11    12           13     14
          # [topic, hostname, ipaddress, devicename, version, version_num, last_seen, uptime, uptime_sec, berryheap, wifirssi, heap, berryobject, power, wifichnl]
          var uptime = self.list_devices[i][7]
          if uptime == " " continue end             # No STATE info

          var last_seen = self.list_devices[i][6]
          var uptime_sec = self.list_devices[i][8]
          var uptime_sec_int = int(uptime_sec)    
          if uptime_sec_int < 0                     # Device is offline visible
            uptime_sec_int -= 60
          elif int(last_seen) < (now - dvo_online_window)
            uptime_sec_int = int(last_seen) - now   # Negative
          end
          if uptime_sec_int < 0
            self.list_devices[i][8] = format("%011d", uptime_sec_int) # Convert to string to enable multicolumn sort
            self.list_devices[i][7] = self.uptime_to_str(uptime_sec_int)
            self.list_devices[i][6] = str(now)
          end
        end
      end

    end
  end

  #################################################################################
  # sort_col(l, col, dir)
  #
  # Shell sort list of online devices based on user selected column and direction
  #################################################################################
  def sort_col(l, col, dir)
    var cmp = /a,b -> a < b                         # Sort up
    if dir
      cmp = /a,b -> a > b                           # Sort down
    end

    for i:1..size(l)-1
      var k = l[i]
      var ks = k[col]                               # Needle Primary key
      if col != 1                                   # Hostname (unique)
        if col == 14                                # Channel
          ks += k[10]                               # Needle RSSI
        end
        ks += k[1]                                  # Needle Hostname (unique)
      end
      var j = i
      while j > 0
        var ls = l[j-1][col]                        # Haystack Primary key
        if col != 1                                 # Hostname (unique)
          if col == 14                              # Channel
            ls += l[j-1][10]                        # Haystack RSSI
          end
          ls += l[j-1][1]                           # Haystack Hostname (unique)
        end
        if cmp(ls, ks)
          break
        end
        l[j] = l[j-1]
        j -= 1
      end
      l[j] = k
    end
  end

  #################################################################################
  # web_sensor
  #
  # Display Devices Online in user selected sorted columns
  #################################################################################
  def web_sensor()
    if self.mqtt_step < 2 return end                # Do not display until discovery is complete

    if webserver.has_arg("sd_sort")
      persist.dvo_column = int(webserver.arg("sd_sort")) # Change sort column
      if self.sort_last_column == persist.dvo_column
        persist.dvo_direction ^= 1                  # Toggle direction
      end
      self.sort_last_column = persist.dvo_column
      persist.save()
    elif webserver.has_arg("sd_pow")
      var power_splits = string.split(webserver.arg("sd_pow"), "_")
      var topic = power_splits[0]
      var power = int(power_splits[1])
      var fulltopic = tasmota.cmd('_FullTopic', true)['FullTopic']
      var prefix = tasmota.cmd("_Prefix", true)['Prefix1'] # cmnd = Prefix1 used by commands
      fulltopic = string.replace(fulltopic, "%prefix%", prefix)
      fulltopic = string.replace(fulltopic, "%topic%", topic)
      if fulltopic[-1] != '/'
        fulltopic += '/'
      end

      # The following code could be eliminated when `SetOption59 1` was enabled on all
      #  online devices.
      var power_state = 0
      for i: self.list_devices.keys()
        # Use topic as line index may have changed since GUI sd_pow was initiated
        if self.list_devices[i][0] == topic
          # We do not want to subscribe to power response so ...
          # we need some abracadabra as direct manipulation ends in exception
          var power_list = self.list_devices[i][13]
          power_state = power_list[power -1] ^ 1    # Toggle state
          power_list[power -1] = power_state
          self.list_devices[i][13] = power_list
          tasmota.cmd(format("publish %sPOWER%d %d", fulltopic, power, power_state), true)
          break;
        end
      end
      #  it could have been replaced by a simple
#      tasmota.cmd(format("publish %sPOWER%d 2", fulltopic, power), true)
    end

    if self.list_devices.size()
      var dvo_online_window = int(persist.find("dvo_online_window", 600))
      var dvo_offline = int(persist.find("dvo_offline", 600))
      var dvo_lines = int(persist.find("dvo_lines", 0))
      var dvo_time_highlight = int(persist.find("dvo_time_highlight", 10))
      var dvo_hostname_suffix = persist.find("dvo_hostname_suffix", "")
      var dvo_devicename = persist.find("dvo_devicename", 0)
      var dvo_ipaddress = persist.find("dvo_ipaddress", 0)
      var dvo_power = persist.find("dvo_power", 0)
      var dvo_version = persist.find("dvo_version", 0)
      var dvo_heap = persist.find("dvo_heap", 0)
      var dvo_berryheap = persist.find("dvo_berryheap", 0)
      var dvo_berryobject = persist.find("dvo_berryobject", 0)
      var dvo_wifirssi = persist.find("dvo_wifirssi", 0)
      var dvo_wifichnl = persist.find("dvo_wifichnl", 0)

      var msg = "</table>"                          # Terminate two column table and open new table
      msg += "<style>"                              # Add style increasing space between columns
      msg += ".dvo{width:100%;font-size:80%;border-spacing:0px;}"
      msg += ".dvo td,.dvo th{padding:0px 3px;}"    # Column space
      msg += ".dvo td:first-child,.dvo th:first-child{padding-left:2px;}"
      msg += ".dvo td:last-child,.dvo th:last-child{padding-right:2px;}"
      msg += "</style>"
      msg += "<table class='dvo'>"                  # Open new condensed table

      msg += "<tr>"

      if dvo_lines > 0                              # Fixed number of last updated lines
        self.sort_col(self.list_devices, 6, 1)      # Sort list by last_seen and down

        if dvo_devicename
          msg += "<th>Device Name</th>"
        end
        msg += "<th>Hostname</th>"
        if dvo_ipaddress
          msg += "<th>IP Address</th>"
        end
        if dvo_power
          msg += "<th>Power</th>"
        end
        if dvo_version
          msg += "<th>Version</th>"
        end
        if dvo_heap
          msg += "<th align='right'>Heap</th>"
        end
        if dvo_berryheap
          msg += "<th align='right'>BHeap</th>"
        end
        if dvo_berryobject
          msg += "<th align='right'>BObject</th>"
        end
        if dvo_wifirssi
          msg += "<th align='right'>RSSI</th>"
        end
        if dvo_wifichnl
          msg += "<th align='right'>Ch</th>"
        end
        msg += "<th align='right'>Uptime</th>"
      else                                          # All devices sorted by user selected column
        #               0  1  2              3               4  5            6  7  8  9              10            11        12              13  14
        var list_dvo = [0, 1, dvo_ipaddress, dvo_devicename, 0, dvo_version, 0, 0, 1, dvo_berryheap, dvo_wifirssi, dvo_heap, dvo_berryobject, 0, dvo_wifichnl]
        if persist.dvo_column >= list_dvo.size() || list_dvo[persist.dvo_column] == 0
          persist.dvo_column = 1                    # Hostname as default sort column
          self.sort_last_column = persist.dvo_column
          persist.save()
        end
#        log(format("DVO: list_dvo '%s'", list_dvo), 3)

#        var start = tasmota.millis()
        self.sort_col(self.list_devices, persist.dvo_column, persist.dvo_direction) # Sort list by column
#        var stop = tasmota.millis()
#        log(format("DVO: Sort time %d ms", stop - start), 3)

        var icon_direction = persist.dvo_direction ? "&#x25BC" : "&#x25B2"
        if dvo_devicename
          msg += format("<th><a href='#p' onclick='la(\"&sd_sort=3\");'>Device Name</a>%s</th>", persist.dvo_column == 3 ? icon_direction : "")
        end
        msg += format("<th><a href='#p' onclick='la(\"&sd_sort=1\");'>Hostname</a>%s</th>", persist.dvo_column == 1 ? icon_direction : "")
        if dvo_ipaddress
          msg += format("<th><a href='#p' onclick='la(\"&sd_sort=2\");'>IP Address</a>%s</th>", persist.dvo_column == 2 ? icon_direction : "")
        end
        if dvo_power
          msg += format("<th>Power</th>")
        end
        if dvo_version
          msg += format("<th><a href='#p' onclick='la(\"&sd_sort=5\");'>Version</a>%s</th>", persist.dvo_column == 5 ? icon_direction : "")
        end
        if dvo_heap
          msg += format("<th align='right'><a href='#p' onclick='la(\"&sd_sort=11\");'>Heap</a>%s</th>", persist.dvo_column == 11 ? icon_direction : "")
        end
        if dvo_berryheap
          msg += format("<th align='right'><a href='#p' onclick='la(\"&sd_sort=9\");'>BHeap</a>%s</th>", persist.dvo_column == 9 ? icon_direction : "")
        end
        if dvo_berryobject
          msg += format("<th align='right'><a href='#p' onclick='la(\"&sd_sort=12\");'>BObject</a>%s</th>", persist.dvo_column == 12 ? icon_direction : "")
        end
        if dvo_wifirssi
          msg += format("<th align='right'><a href='#p' onclick='la(\"&sd_sort=10\");'>RSSI</a>%s</th>", persist.dvo_column == 10 ? icon_direction : "")
        end
        if dvo_wifichnl
          msg += format("<th align='right'><a href='#p' onclick='la(\"&sd_sort=14\");'>Ch</a>%s</th>", persist.dvo_column == 14 ? icon_direction : "")
        end
        msg += format("<th align='right'><a href='#p' onclick='la(\"&sd_sort=8\");'>Uptime</a>%s</th>", persist.dvo_column == 8 ? icon_direction : "")
      end

      msg += "</tr>"
      tasmota.web_send(msg)

      var list_index = 0
      var devices_online = 0
      var devices_offline = 0
      var now = tasmota.rtc('local')
      for i: self.list_devices.keys()
        #  0      1         2          3           4        5            6          7       8           9          10        11    12           13     14
        # [topic, hostname, ipaddress, devicename, version, version_num, last_seen, uptime, uptime_sec, berryheap, wifirssi, heap, berryobject, power, wifichnl]
        var uptime = self.list_devices[i][7]
        if uptime == " " continue end               # No STATE info

        var uptime_sec = self.list_devices[i][8]
        var offline = int(uptime_sec) < 0
        if offline
          devices_offline += 1
        else
          devices_online += 1
        end

        if dvo_lines
          list_index += 1
          if list_index > dvo_lines continue end    # Keep counting devices
        end

        var topic = self.list_devices[i][0]
        var hostname = self.list_devices[i][1]
        var ipaddress = self.list_devices[i][2]
        var devicename = self.list_devices[i][3]
        var version = self.list_devices[i][4]
        var last_seen = self.list_devices[i][6]
        var berryheap = self.list_devices[i][9]
        var wifirssi = self.list_devices[i][10]
        var heap = self.list_devices[i][11]
        var berryobject = self.list_devices[i][12]
        var power = self.list_devices[i][13]
        var wifichnl = self.list_devices[i][14]

        var color = ""                              # Default text color (this keeps refresh page size small)
        if offline
          color = " style='color:var(--c_tab);'"    # Offline color (this increases page size)
        end

        msg = "<tr>"
        if dvo_devicename
          msg += f"<td{color}>{devicename}</td>"
        end
        if offline
          msg += f"<td{color}>{hostname}</td>"
        else
          msg += f"<td><a target=_blank href='http://{hostname}{dvo_hostname_suffix}'>{hostname}</a></td>"
        end
        if dvo_ipaddress
          if offline
            msg += f"<td{color}>{ipaddress}</td>"
          else
            msg += f"<td><a target=_blank href='http://{ipaddress}'>{ipaddress}</a></td>"
          end
        end
        if dvo_power
          msg += "<td>"
          if !offline && power.size()
            for p: power.keys()
              var hcolor = (power[p]) ? "txtscc" : "btn"
              var picon = (power[p]) ? "&#x2612" : "&#x2610"
              msg += f"<a href='#p' style='color:var(--c_{hcolor});' title='Toggle POWER{p +1}' onclick='la(\"&sd_pow={topic}_{p +1}\");'>{picon}&nbsp;</a>"
            end
          end
          msg += "</td>"
        end
        if dvo_version
          msg += f"<td{color}>{version}</td>"
        end
        var svar
        if dvo_heap
          var iheap = int(heap)                     # Workaround to remove leading zeros
          svar = (iheap > 0) ? str(iheap) : ""
          msg += f"<td align='right'{color}>{svar}</td>"
        end
        if dvo_berryheap
          var bheap = int(berryheap)                # Workaround to remove leading zeros
          svar = (bheap > 0) ? str(bheap) : ""
          msg += f"<td align='right'{color}>{svar}</td>"
        end
        if dvo_berryobject
          var bobject = int(berryobject)            # Workaround to remove leading zeros
          svar = (bobject > 0) ? str(bobject) : ""
          msg += f"<td align='right'{color}>{svar}</td>"
        end
        if dvo_wifirssi
          var wrssi = int(wifirssi)                 # Workaround to remove leading zeros
          svar = str(wrssi)
          msg += f"<td align='right'{color}>{svar}%</td>"
        end
        if dvo_wifichnl
          var wchnl = int(wifichnl)                 # Workaround to remove leading zeros
          svar = str(wchnl)
          msg += f"<td align='right'{color}>{svar}</td>"
        end
        if int(last_seen) >= (now - dvo_time_highlight) # Highlight changes within latest seconds
          color = " style='color:var(--c_btnsv);'"
        elif int(uptime_sec) < dvo_online_window  # Highlight changes just after restart
          color = " style='color:var(--c_txtwrn);'"
        end
        msg += f"<td align='right'{color}>{uptime}</td>"

        msg += "</tr>"
        tasmota.web_send(msg)
      end
      msg = "</table>{t}"                           # Terminate multi-column table and open new table: <table style='width:100%'>
      msg += f"{{s}}Devices</th><td align='right'>{devices_online} online"
      if devices_offline
        msg += f" / {devices_offline} offline"
      end
      msg += "{e}</table>{t}"                       # Terminate multi-column table and open new table: <table style='width:100%'>

      tasmota.web_send_decimal(msg)                 # Force horizontal line
    end
  end

end
#---------------------------------------------------------------------------------#
# extension devices_online
###################################################################################

return devices_online()
