# Decoder files are modeled on the *.js files found here:
#  https://github.com/TheThingsNetwork/lorawan-devices/tree/master/vendor

import mqtt
import string

var LwRegions = ["EU868","US915","IN865","AU915","KZ865","RU864","AS923","AS923-1","AS923-2","AS923-3"]
var LwDeco

class LwSensorFormatter_cls
  static var Formatter = {
    "string":           { "u": nil,   "f": " %s",    "i": nil         },
    "volt":             { "u": "V",   "f": " %.1f",  "i": "&#x26A1;"  },
    "milliamp":         { "u": "mA",  "f": " %.0f",  "i": "&#x1F50C;" },
    "power_factor%":    { "u": "%",   "f": " %.0f",  "i": "&#x1F4CA;" },
    "power":            { "u": "W",   "f": " %.0f",  "i": "&#x1F4A1;" },
    "energy":           { "u": "Wh",  "f": " %.0f",  "i": "&#x1F9EE;" },
    "altitude":         { "u": "mt",  "f": " %d",    "i": "&#x26F0;"  },
    "empty":            { "u": nil,   "f": nil,      "i": nil         }
  }

  var msg_buffer

  def init()
    self.msg_buffer = bytes(512)
    self.msg_buffer.clear()
  end

  def start_line()
    self.msg_buffer .. "<tr class='htr'><td colspan='4'>&#9478;"
    return self
  end

  def end_line()
    self.msg_buffer .. "{e}"
    return self
  end

  def next_line()
    self.msg_buffer .. "{e}<tr class='htr'><td colspan='4'>&#9478;"
    return self
  end

  def begin_tooltip(ttip)
    self.msg_buffer .. format("&nbsp;<div title='%s' class='si'>", ttip)
    return self
  end

  def end_tooltip()
    self.msg_buffer .. "</div>"
    return self
  end

  def add_link(title, url, target)
    if !target target = "_blank" end
    self.msg_buffer .. format(" <a target=%s href='%s'>%s</a>", target, url, title)
    return self
  end

  def add_sensor(formatter, value, tooltip, alt_icon)

    if tooltip self.begin_tooltip(tooltip) end

    var fmt = self.Formatter.find(formatter)

    if alt_icon
      self.msg_buffer .. format(" %s", alt_icon)
    elif fmt && fmt.find("i") && fmt["i"]
      self.msg_buffer .. format(" %s", fmt["i"])
    end

    if fmt && fmt.find("f") && fmt["f"]
      self.msg_buffer .. format(fmt["f"], value)
    else
      self.msg_buffer .. str(value)
    end

    if fmt && fmt.find("u") && fmt["u"]
      self.msg_buffer .. format("%s", fmt["u"])
    end

    if tooltip self.end_tooltip() end
    return self
  end

  def get_msg()
    return self.msg_buffer.asstring()
  end
end

class lwdecode_cls
  var lw_decoders
  var topic_cached
  var last_payload_hash
  var web_msg_cache
  var cache_timeout
  var decoder_timestamps

  def init()
    self.lw_decoders = {}
    self.last_payload_hash = 0
    self.web_msg_cache = ""
    self.cache_timeout = 0
    self.decoder_timestamps = {}

    self._cache_topic()

    if global.lwdecode_driver
      global.lwdecode_driver.stop() # Let previous instance bail out cleanly
    end
    tasmota.add_driver(global.lwdecode_driver := self)
    tasmota.add_rule("LwReceived", /value, trigger, payload -> self.lw_decode(payload))
    tasmota.add_cmd('LwReload', /cmd, idx, payload, payload_json -> self.cmd_reload_decoder(cmd, idx, payload))
  end

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

  def _cache_topic()
    var full_topic = tasmota.cmd('_FullTopic',true)['FullTopic']
    var topic = tasmota.cmd('_Status',true)['Status']['Topic']
    var prefix = tasmota.cmd('_Prefix',true)['Prefix3']

    self.topic_cached = string.replace(string.replace(full_topic, '%topic%', topic), '%prefix%', prefix) + 'SENSOR'
  end

  def SendDownlink(nodes, cmd, idx, payload, ok_result)
    if !nodes.find(idx) return nil end

    var _send    = 'LoRaWanSend'
    var _cmdSend = _send + str(idx) + ' ' + payload
    var _out     = tasmota.cmd(_cmdSend, true)

    return tasmota.resp_cmnd(
      format('{"%s%i":"%s","%s":"%s","Payload":"%s"}',
        cmd,
        idx,
        ok_result,
        _send,
        _out[_send],
        payload
      )
    )
  end

  def SendDownlinkMap(nodes, cmd, idx, payload, choice_map)
    var key = string.toupper(str(payload))
    for choice_key : choice_map.keys()
      if string.find(choice_key, key) >= 0 && (choice_key == key || string.find(choice_key, '|' + key + '|') >= 0 || string.find(choice_key, key + '|') == 0 || string.find(choice_key, '|' + key) == size(choice_key) - size(key) - 1)
        var choice = choice_map[choice_key]
        return self.SendDownlink(nodes, cmd, idx, choice[0], choice[1])
      end
    end
    return tasmota.resp_cmnd_error()
  end

  def reload_decoder(decoder_name)
    try
      if self.lw_decoders.find(decoder_name)
        self.lw_decoders.remove(decoder_name)
      end
      LwDeco = nil
      load(decoder_name)
      if LwDeco
        self.lw_decoders[decoder_name] = LwDeco
        self.decoder_timestamps[decoder_name] = tasmota.millis()
        print(format("Decoder %s reloaded successfully", decoder_name))
        return true
      else
        print(format("Failed to reload decoder %s", decoder_name))
        return false
      end
    except .. as e, m
      print(format("Error reloading decoder %s: %s", decoder_name, m))
      return false
    end
  end

  def cmd_reload_decoder(cmd, idx, payload)
    if payload == ""
      var reloaded = []
      var failed = []
      for decoder_name : self.lw_decoders.keys()
        if self.reload_decoder(decoder_name)
          reloaded.push(decoder_name)
        else
          failed.push(decoder_name)
        end
      end
      var result = format("Reloaded: %i", reloaded.size())
      if failed.size() > 0
        result += format(", Failed: %i", failed.size())
      end
      return tasmota.resp_cmnd(format('{"LwReload":"%s"}', result))
    else
      var success = self.reload_decoder(payload)
      var status = success ? "OK" : "Failed"
      return tasmota.resp_cmnd(format('{"LwReload":"%s %s"}', payload, status))
    end
  end

  def _calculate_payload_hash(payload)
    var hash = 0
    for i:0..payload.size()-1
      hash = (hash * 31 + payload[i]) & 0xFFFFFFFF
    end
    return hash
  end

  def lw_decode(data)
    import json

    var device_data = data['LwReceived']
    var device_name = device_data.keys()()
    var device_info = device_data[device_name]

    var decoder = device_info.find('Decoder')
    if !decoder return true end

    var payload = device_info['Payload']
    if !payload || payload.size() == 0 return true end

    if !self.lw_decoders.find(decoder)
      try
        LwDeco = nil
        load(decoder)
        if LwDeco
          self.lw_decoders[decoder] = LwDeco
        else
          log("LwD: Unable to load decoder",1)
          return true
        end
      except .. as e, m
        log(format("LwD: Decoder load error: %s", m),1)
        return true
      end
    end

    var hashCheck
    # check if the decoder driver have the hashCheck properties
    try
      hashCheck = self.lw_decoders[decoder].hashCheck
    except .. as e, m
      hashCheck = true
    end
    
    if hashCheck
      var current_hash = self._calculate_payload_hash(payload)
      if current_hash == self.last_payload_hash return true end
      self.last_payload_hash = current_hash
    end

    try
      var decoded = self.lw_decoders[decoder].decodeUplink(
        device_info['Name'],
        device_info['Node'],
        device_info['RSSI'],
        device_info['FPort'],
        payload
      )

      decoded['Node'] = device_info['Node']
      decoded['RSSI'] = device_info['RSSI']

      var mqtt_data
      if tasmota.get_option(83) == 0  # SetOption83 - Remove LwDecoded form JSON message (1)
        mqtt_data = {"LwDecoded": {device_name: decoded}}
      else
        mqtt_data = {device_name: decoded}
      end

      var topic
      if tasmota.get_option(89) == 1  # SetOption89 - Distinct MQTT topics per device (1)
        topic = format("%s/%s%s", self.topic_cached, device_info['DevEUIh'], device_info['DevEUIl'])
      else
        topic = self.topic_cached
      end

      mqtt.publish(topic, json.dump(mqtt_data))
      tasmota.global.restart_flag = 0 # Signal LwDecoded successful (default state)

    except .. as e, m
      log(format("LwD: Decode error for %s: %s", device_name, m),1)
    end

    return true
  end

  def dhm(last_time)
    var since = tasmota.rtc('local') - last_time
    var unit = "d"
    if since > 86400
      since /= 86400
      if since > 99 since = 99 end
    elif since > 3600
      since /= 3600
      unit = "h"
    else
      since /= 60
      unit = "m"
    end
    return format("%02d%s", since, unit)
  end

  def dhm_tt(last_time)
    return format("Received %s ago", self.dhm(last_time))
  end

  def header(name, name_tooltip, battery, battery_last_seen, rssi, last_seen)
    var msg = format("<tr class='ltd htr'><td><b title='%s'>%s</b></td>", name_tooltip, name)

    if battery < 1000
      # Battery low <= 2.5V (0%), high >= 3.1V (100%)
      var batt_percent = (battery * 1000) - 2500
      batt_percent /= 6                                       # 3.1V - 2.5V = 0.6V = 100%
      if batt_percent < 0 batt_percent = 0 end
      if batt_percent > 98 batt_percent = 98 end              # 98% / 14px = 7
      batt_percent /= 7                                       # 1..14px showing battery load
      msg += format("<td><i class=\"bt\" title=\"%.3fV (%s)\" style=\"--bl:%dpx;color:var(--c_txt)\"></i></td>",
                   battery, self.dhm(battery_last_seen), batt_percent)
    elif battery >= 100000 && battery <= 100100               # battery already expressed in %
      var pbatt = battery - 100000
      var batt_percent = pbatt
      if batt_percent > 98 batt_percent = 98 end              # 98% / 14px = 7
      batt_percent /= 7                                       # 1..14px showing battery load
      msg += format("<td><i class=\"bt\" title=\"%d%% (%s)\" style=\"--bl:%dpx;color:var(--c_txt)\"></i></td>",
                   pbatt, self.dhm(battery_last_seen), batt_percent)
    else
      msg += "<td>&nbsp;</td>"
    end

    if rssi < 1000
      if rssi < -132 rssi = -132 end
      var num_bars = 4 - ((rssi * -1) / 33)
      msg += format("<td><div title='RSSI %i' class='si'>", rssi)
      for j:0..3
        msg += format("<i class='b%d%s'></i>", j, (num_bars < j) ? " o30" : "")         # Bars
      end
      msg += "</div></td>"                                    # Close RSSI
    else
      msg += "<td>&nbsp;</td>"
    end

    msg += format("<td style='color:var(--c_txt)'>&#x1F557;%s</td></tr>", self.dhm(last_seen))

    return msg
  end #sensor()

  #------------------------------------------------------------
  Display sensor value in the web UI and react to button
  Called every WebRefresh time
  ------------------------------------------------------------#
  def web_sensor()
    var current_time = tasmota.millis()

    if current_time < self.cache_timeout
      tasmota.web_send_decimal(self.web_msg_cache)
      return
    end

    var msg = ""
    for decoder: self.lw_decoders
      msg += decoder.add_web_sensor()
    end

    if msg
      var full_msg = format("</table>"                        # Terminate current two column table and open new table
        "<style>"
        # Table CSS
        ".ltd td:not(:first-child){width:20px;font-size:70%%}"
        ".ltd td:last-child{width:45px}"
        ".ltd .bt{margin-right:10px;}"                        # Margin right should be half of the not-first width
        ".htr{line-height:20px}"
        # Signal Strength Indicator
        ".si{display:inline-flex;align-items:flex-end;height:15px;padding:0}"
        ".si i{width:3px;margin-right:1px;border-radius:3px;background-color:var(--c_txt)}" # WebColor(COL_TEXT)
        ".si .b0{height:25%%}.si .b1{height:50%%}.si .b2{height:75%%}.si .b3{height:100%%}.o30{opacity:.3}"
        "</style>"
        "{t}%s</table>{t}",
        msg)

      self.web_msg_cache = full_msg
      self.cache_timeout = current_time + 5000
      tasmota.web_send_decimal(full_msg)
    end
  end
end

lwdecode = lwdecode_cls()

import webserver
class webPageLoRaWAN : Driver
  var max_node_cached

  def init()
    self.max_node_cached = nil
  end

  def web_add_config_button()
    webserver.content_send("<p></p><form id=ac action='lrw' style='display: block;' method='get'><button>LoRaWAN</button></form>")
  end

  def _get_max_nodes()
    if !self.max_node_cached
      var enables = string.split(tasmota.cmd('LoRaWanNode', true).find('LoRaWanNode'), ',')
      self.max_node_cached = enables.size()
    end
    return self.max_node_cached
  end

  #- this method displays the web page -#
  def pageLoRaWAN()
    if !webserver.check_privileged_access() return nil end

    var inode = 1
    var cmdArg
    if webserver.has_arg('save')
      inode = int(webserver.arg('node'))
      tasmota.cmd(format('LoRaWanAppKey%i %s', inode, webserver.arg('ak')), true)
      cmdArg = webserver.arg('dc')
      if !cmdArg cmdArg = '"' end
      tasmota.cmd(format('LoRaWanDecoder%i %s', inode, cmdArg), true)
      cmdArg = webserver.arg('an')
      if !cmdArg cmdArg = '"' end
      tasmota.cmd(format('LoRaWanName%i %s', inode, cmdArg), true)
      cmdArg = webserver.arg('ce')
      if !cmdArg
        cmdArg = '0'
      else
        cmdArg = '1'
      end
      tasmota.cmd(format('LoRaWanNode%i %s', inode, cmdArg), true)
    end

    var appKey, decoder, name, enabled
    var hintAK = '32 character Application Key'
    var hintDecoder = 'Decoder file, ending in .be'
    var hintAN = 'Device name for MQTT messages'
    var arg = 'LoRaWanNode'
    var enables = string.split(tasmota.cmd(arg, true).find(arg), ',') # [1,!2,!3,!4,5,6]
    var maxnode = enables.size()

    webserver.content_start("LoRaWAN")           #- title of the web page -#
    webserver.content_send_style()               #- send standard Tasmota styles -#

    webserver.content_send(
     "<style>"
     ".tl{float:left;border-radius:0;border:1px solid var(--c_frm);padding:1px;width:12.5%;}"
     ".tl:hover{background:var(--c_frm);}"
     ".inactive{background:var(--c_tab);color:var(--c_tabtxt);font-weight:normal;}"
     ".active{background:var(--c_frm);color:var(--c_txt);font-weight:bold;}"
     "</style>"
     "<script>"
     "function selNode(n){"
      "var i;"
      "var e=document.getElementById('n'+n);"
      "var o=document.getElementsByClassName('tl active');"
      "if(o.length){"
       "for(i=0;i<o.length;i++){"
        "o[i].classList.add('inactive');"
        "o[i].classList.remove('active');"
       "}"
      "}"
      "e.classList.add('active');"
      "for(i=1;i<=" + str(maxnode) + ";i++){"
       "document.getElementById('nd'+i).style.display=(i==n)?'block':'none';"
      "}"
     "}"
     "window.onload = function(){selNode(" + str(inode) + ");};"
     "</script>")

    webserver.content_send(
    format("<fieldset>"
     "<legend><b>&nbsp;LoRaWan End Device&nbsp;</b></legend>"
     "<br><div>"))                               #- Add space and indent to align form tabs -#
    for node:1 .. maxnode
     webserver.content_send(format("<button type='button' onclick='selNode(%i)' id='n%i' class='tl inactive'>%i</button>", node, node, node))
    end
    webserver.content_send("</div><br><br><br><br>")    #- Terminate indent and add space -#

    for node:1 .. maxnode
     enabled = ""
     if enables[node-1][0] != '!'
       enabled = ' checked'
     end
     arg = format('LoRaWanAppKey%i', node)
     appKey = tasmota.cmd(arg, true).find(arg)
     arg = format('LoRaWanName%i', node)
     name = tasmota.cmd(arg, true).find(arg)
     arg = format('LoRaWanDecoder%i', node)
     decoder = tasmota.cmd(arg, true).find(arg)

     webserver.content_send(
     format("<div id='nd%i' style='display:none'>"
      "<form action='' method='post'>"
       "<p><label><input id='ce' name='ce' type='checkbox'%s><b>Enabled</b></label></p>"
       "<p><b>Application Key</b>"
        "<input title='%s' pattern='[A-Fa-f0-9]{32}' id='ak' minlength='32' maxlength='32' required='' placeholder='%s' value='%s' name='ak' style='font-size:smaller'>"
       "</p>"
       "<p></p>"
       "<p><b>Device Name</b>"
        "<input id='an' placeholder='%s' value='%s' name='an'>"
       "</p>"
       "<p></p>"
       "<p><b>Decoder File</b>"
        "<input title='%s' id='dc' placeholder='%s' value='%s' name='dc'>"
       "</p>"
       "<br>"
       "<button name='save' class='button bgrn'>Save</button>"
       "<input type='hidden' name='node' value='%i'>"
      "</form>"
      "</div>", node, enabled, hintAK, hintAK, appKey, hintAN, name, hintDecoder, hintDecoder, decoder, node))
    end

    webserver.content_send("</fieldset>")


    webserver.content_button(webserver.BUTTON_CONFIGURATION) #- button back to conf page -#
    webserver.content_stop()                                 #- end of web page -#
  end

  #- this is called at Tasmota start-up, as soon as Wifi/Eth is up and web server running -#
  def web_add_handler()
    #- we need to register a closure, not just a function, that captures the current instance -#
    webserver.on("/lrw", / -> self.pageLoRaWAN())
  end
end

#- create and register driver in Tasmota -#
webPageLoRaWAN_instance = webPageLoRaWAN()
tasmota.add_driver(webPageLoRaWAN_instance)

tasmota.cmd('LoraOption3 off')    # Disable embedded decoding
tasmota.cmd('SetOption100 off')   # Keep LwReceived in JSON message
tasmota.cmd('SetOption118 off')   # Keep SENSOR as subtopic name
tasmota.cmd('SetOption119 off')   # Keep device address in JSON message
#tasmota.cmd('SetOption147 on')    # Hide LwReceived MQTT message but keep rule processing
tasmota.cmd('LoRaWanBridge on')
