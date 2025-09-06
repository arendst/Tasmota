#-
  Show topic/hostname and IP address with uptime from MQTT STATE messages in GUI

  Enable either
   self.line_option = 1  : Scroll 'line_cnt' lines
  or
   self.line_option = 2  : Show devices updating within 'line_teleperiod'
-#

import mqtt 
import string

class mqttdata_cls
  var line_option                                   # Line option
  var line_cnt                                      # Number of lines
  var line_teleperiod                               # Skip any device taking longer to respond (probably offline)
  var line_duration                                 # Duration option
  var list_buffer                                   # Buffer storing lines

  def init()
#    self.line_option = 1                            # Scroll line_cnt lines
    self.line_option = 2                            # Show devices updating within line_teleperiod
    self.line_cnt = 10                              # Option 1 number of lines to show
    self.line_teleperiod = 1200                     # Option 2 number of teleperiod for devices to be shown
    self.line_duration = 0                          # Show duration of last state message (1)

    self.list_buffer = []                           # Init line buffer list

    if global.mqttdata_driver
      global.mqttdata_driver.stop()                 # Let previous instance bail out cleanly
    end
    tasmota.add_driver(global.mqttdata_driver := self)

    # Assume default Fulltopic (%prefix%/%topic%/) and Prefix3 (tele)
    mqtt.subscribe("tele/#", /topic, idx, data, databytes -> self.handle_state_data(topic, idx, data, databytes))
  end

  def stop()
    mqtt.unsubscribe("tele/#")                      # Assume default Fulltopic (%prefix%/%topic%/) and Prefix3 (tele)
    tasmota.remove_driver(self)
  end

  def handle_state_data(full_topic, idx, data, databytes)
    import json

    var subtopic = string.split(full_topic, "/")    # Assume default Fulltopic (%prefix%/%topic%/)
    if subtopic[-1] == "STATE"                      # tele/wemos7/STATE
      var topic = subtopic[1]                       # wemos7

      var state = json.load(data)
      if state                                      # Valid JSON state message
        var sub_option = 1
        var ipaddress = ""                          # Not used
        var uptime = state['Uptime']                # 129T10:52:41
        if state.find('Hostname')
          sub_option = 2
          topic = state['Hostname']                 # wemos7
          ipaddress = state['IPAddress']            # 192.168.2.123
        end
        var last_seen = tasmota.rtc('local')
        var line = format("%s,%s,%s,%d,%d", topic, ipaddress, uptime, last_seen, sub_option)

        if 1 == self.line_option
          self.list_buffer.push(line)               # Add state as last entry
          if self.list_buffer.size() > self.line_cnt  # Max number of lines in buffer
            self.list_buffer.remove(0)              # Remove first entry
          end
        elif 2 == self.line_option
          if self.list_buffer.size()
            var i = 0
            var list_size = size(self.list_buffer)
            while i < list_size                     # Use while loop as counter is decremented
              if 0 == string.find(self.list_buffer[i], topic)
                self.list_buffer.remove(i)          # Remove current state
                list_size -= 1                      # Continue for duplicates
              end
              i += 1
            end
          end
          self.list_buffer.push(line)               # Add state as last entry
        end  

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

  def web_sensor()
    if self.list_buffer.size()
      var msg = ""

      if 2 == self.line_option
        # Sort list
        var less = /a,b -> a < b
        self.sort(self.list_buffer, less)
      end

      var stx = false                               # If list_buffer is empty due to removes show nothing
      var time_window = tasmota.rtc('local') - self.line_teleperiod
      var i = 0
      var j = size(self.list_buffer)
      while i < j
        var splits = string.split(self.list_buffer[i], ",")
        var last_seen = int(splits[3])

        if time_window > last_seen                  # Remove offline devices
          self.list_buffer.remove(i)
          j -= 1
          continue
        end

        var topic = splits[0]                       # topic or hostname
        var ipaddress = splits[1]
        var uptime = splits[2]
        var sub_option = int(splits[4])

        if !stx
          stx = true
          msg = format("</table>{t}")               # Terminate two column table and open new table
        end
#        msg += format("<tr style='font-size:%d%%'>", 90 - (self.line_duration * 10))
        msg += "<tr style='font-size:80%'>"
        if 1 == sub_option
          msg += format("<td>%s</td><td>&nbsp</td><td align='right'>%s</td>",
                        topic, uptime)
        elif 2 == sub_option
          msg += format("<td><a target=_blank href='http://%s.'>%s</a></td><td><a target=_blank href='http://%s'>%s</a></td><td align='right'>%s</td>",
                        topic, topic, ipaddress, ipaddress, uptime)
        end
        if self.line_duration
          msg += format("<td style='font-size:90%%'>&#x1F557;%s</td>",  # Clock
                        self.dhm(last_seen))
        end
        msg += "</tr>"
        i += 1
      end
      if stx
        msg += "</table>{t}"                        # Terminate three column table and open new table
        tasmota.web_send(msg)                       # Do not use tasmota.web_send_decimal() which will replace IPAddress dots
        tasmota.web_send_decimal("")                # Force horizontal line
      end
    end
  end
end

mqttdata = mqttdata_cls()
