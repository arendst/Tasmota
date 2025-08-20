## Version: 2.2.9 | Framework: LwDecode | Platform: Tasmota Berry

import mqtt
import string

var LwRegions = ["EU868","US915","IN865","AU915","KZ865","RU864","AS923","AS923-1","AS923-2","AS923-3"]
var LwDeco
var lwdecode
var webpage

class LwSensorFormatter_cls
  static var Formatter = {
    "string":           { "u": nil,   "f": " %s",    "i": nil         },
    "volt":             { "u": "V",   "f": " %.1f",  "i": "&#x26A1;"  },
    "milliamp":         { "u": "mA",  "f": " %.0f",  "i": "&#x1F50C;" },
    "power_factor%":    { "u": "%",   "f": " %.0f",  "i": "&#x1F4CA;" },
    "power":            { "u": "W",   "f": " %.0f",  "i": "&#x1F4A1;" },
    "energy":           { "u": "Wh",  "f": " %.0f",  "i": "&#x1F9EE;" },
    "altitude":         { "u": "mt",  "f": " %d",    "i": "&#x26F0;"  },

## Driver Added formatter begin here - AI Generated don't edit this section
    "temp":              { "u": "°C",  "f": " %.1f",  "i": "🌡️"        }, # Added by SE01LB at 2025-08-20 14:42:35
    "humidity":          { "u": "%",   "f": " %.1f",  "i": "💧"        }, # Added by SE01LB at 2025-08-20 14:42:35
    "conductivity":      { "u": "µS/cm", "f": " %d",    "i": "⚡"        }, # Added by SE01LB at 2025-08-20 14:42:35
    "distance":          { "u": "cm",  "f": " %.1f",  "i": "📏"        }, # Added by SN50v3LB at 2025-08-20 16:30:00
    "weight":            { "u": "kg",  "f": " %.1f",  "i": "⚖️"        }, # Added by SN50v3LB at 2025-08-20 16:30:00
## Driver Added formatter stop here

    "empty":            { "u": nil,   "f": nil,      "i": nil         }
  }

  var msg_buffer

  def init()
    self.msg_buffer = bytes(512)
    self.msg_buffer.clear()
  end

  def clear()
    self.msg_buffer.clear()
  end

  def header(name, name_tooltip, battery, battery_last_seen, rssi, last_update, simulated)
    self.msg_buffer .. lwdecode.header(name, name_tooltip, battery, battery_last_seen, rssi, last_update, simulated)
    return self
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
    if ttip   self.msg_buffer .. format("&nbsp;<div title='%s' class='si'>", ttip) end
    return self
  end

  def end_tooltip()
    self.msg_buffer .. "</div>"
    return self
  end

  def add_link(title, url, target)
    var _t = title ? title : ""
    var _u = url ? url : ""
    var _tg = target ? target : "_blank"

    self.msg_buffer .. format(" <a target=%s href='%s'>%s</a>", _tg, _u, _t)
    return self
  end

  def add_sensor(formatter, value, tooltip, alt_icon)
    if tooltip self.begin_tooltip(tooltip) end

    var fmt = formatter ? self.Formatter.find(formatter) : self.Formatter.find("empty")

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

  def add_status(text, icon, tooltip)
    if tooltip 
      self.begin_tooltip(tooltip) 
    end
    
    if icon
      self.msg_buffer .. format(" %s", icon)
    end
    
    self.msg_buffer .. format(" %s", text)
    
    if tooltip 
      self.end_tooltip() 
    end
    
    return self
  end

  def get_msg()
    var buff = self.msg_buffer.asstring()
    self.clear()
    return ( buff ? buff : "nil" )
  end
end

#
# LwDecode driver
#
class LwDecode_cls : Driver
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
    tasmota.add_cmd('LwSimulate', /cmd, idx, payload, payload_json -> self.cmd_simulate(cmd, idx, payload))

    log("LwD: Driver running",1)
  end

  def stop()
    log("LwD: Driver stopped",1)
  end

  #
  # Helper funtion to execute a command (with index option) and return the result, 
  #
  # example of command: LoRaWanAppKey3 12345678123456781234567812345678
  #                     LoRaWanNode
  #
  # example of use:     appKey = lwdecode._cmd_result(node,'LoRaWanAppKey') # get the AppKey for index 3
  #                     lwdecode._cmd_result(node,'LoRaWanAppKey',key)      # set 'key'  as AppKey for index 3
  #                     var enables = string.split( lwdecode._cmd_result(nil,'LoRaWanNode'), ',') # [1,!2,!3,!4,5,6]

  def _cmd_result(node,cmd,arg)
    var full_cmd = format('%s%s%s', cmd, ( node ? node : ""), (arg ? " "+arg : ""))
    var out 
    out = tasmota.cmd(full_cmd, true)
    if type(out) == "string"
      return out
    elif out.find(cmd..node)
      return out[cmd..node]
    elif out.find(cmd)
      return out[cmd]
    else
      return out
    end
  end

  #
  # Helper funtion for Tasmota convention to qualify an empty string (a single double quote char) into the console command
  #
  def _empty(val)
    return (val ? val : '"')
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

  def uint16be(value)
    return string.format( "%02x%02x",
      (value >> 8) & 0xFF,
      value & 0xFF
    )
  end

  def uint32be(value)
    return string.format( "%02x%02x%02x%02x",
      (value >> 24) & 0xFF,
      (value >> 16) & 0xFF,
      (value >> 8)  & 0xFF,
      value & 0xFF
    )
  end

  def int16le(value)
    if value < 0
      value = 0x10000 + value
    end
    return self.uint16le(value)
  end

  def int32le(value)
    if value < 0
      value = 0x100000000 + value
    end
    return self.uint32le(value)
  end

  def float32le(value)
    import struct
    var b = struct.pack('f', value)
    return string.format("%02x%02x%02x%02x", b[0], b[1], b[2], b[3])
  end

  def float32be(value)
    import struct
    var b = struct.pack('f', value)
    return string.format("%02x%02x%02x%02x", b[3], b[2], b[1], b[0])
  end

  def bcd_encode(value)
    var hex = ""
    while value > 0
      var digit = value % 10
      value = value / 10
      var next_digit = value % 10
      value = value / 10
      hex = string.format("%02x", (next_digit << 4) | digit) + hex
    end
    return hex
  end

  def bcd_decode(hex_str)
    var value = 0
    for i: 0..size(hex_str)-1..2
      var byte = int('0x' + hex_str[i..i+1])
      value = value * 10 + ((byte >> 4) & 0x0F)
      value = value * 10 + (byte & 0x0F)
    end
    return value
  end

  def crc16_modbus(data)
    var crc = 0xFFFF
    for i: 0..size(data)-1
      crc ^= data[i]
      for j: 0..7
        if crc & 0x0001
          crc = (crc >> 1) ^ 0xA001
        else
          crc = crc >> 1
        end
      end
    end
    return crc
  end

  def crc8(data)
    var crc = 0x00
    for i: 0..size(data)-1
      crc ^= data[i]
      for j: 0..7
        if crc & 0x80
          crc = (crc << 1) ^ 0x07
        else
          crc = crc << 1
        end
      end
    end
    return crc & 0xFF
  end

  def pack_bits(bit_array)
    var result = bytes()
    var byte = 0
    var bit_count = 0
    
    for bit: bit_array
      if bit
        byte |= (1 << (7 - bit_count))
      end
      bit_count += 1
      
      if bit_count == 8
        result.add(byte)
        byte = 0
        bit_count = 0
      end
    end
    
    if bit_count > 0
      result.add(byte)
    end
    
    return result
  end

  def unpack_bits(data)
    var bits = []
    for i: 0..size(data)-1
      var byte = data[i]
      for j: 7..0..-1
        bits.push((byte >> j) & 0x01)
      end
    end
    return bits
  end

  def log_error(context, error, message, payload_info)
    log(format("LwD: ERROR [%s]: e=%s m=%s p='%s'", context, error, message, payload_info), 1)
  end

  def safe_load_decoder(decoder_name)
    var attempts = 0
    while attempts < 3
      try
        LwDeco = nil
        load(decoder_name)
        if LwDeco && LwDeco.decodeUplink
          return LwDeco
        else
          self.log_error("LOAD", "Invalid decoder", 
            format("Missing decodeUplink method attempt %d", attempts), decoder_name)
        end
      except .. as e, m
        self.log_error("LOAD", e, m, format("attempt %d for %s", attempts, decoder_name))
        attempts += 1
        if attempts < 3
          tasmota.delay(100)  # Brief retry delay
        end
      end
    end
    return nil
  end

  def get_decoder_property(decoder, property, default_value)
    try
      if decoder.find(property)
        return decoder[property]
      else
        return default_value
      end
    except .. as e, m
      return default_value
    end
  end

  def safe_decode_uplink(decoder, name, node, rssi, fport, payload, simulated)
    try
      # Validate inputs
      if !name || !node || !payload return nil end
      if payload.size() == 0 return nil end
      
      return decoder.decodeUplink(name, node, rssi, fport, payload, simulated)

      # var decoded = decoder.decodeUplink(name, node, rssi, fport, payload, simulated)
      #  
      # # Validate output
      # if decoded && type(decoded) == 'map'
      #   return decoded
      # else
      #   self.log_error("DECODE", "Invalid result", 
      #     format("Expected map, got %s", type(decoded)), 
      #     format("FPort:%d, Size:%d", fport, payload.size()))
      #   return nil
      # end
    except .. as e, m
      self.log_error("DECODE", e, m, format("Device:%s, Node:%s, FPort:%d, Size:%d", name, node, fport, payload.size()))
      return nil
    end
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
      
      var decoder = self.safe_load_decoder(decoder_name)
      if decoder
        self.lw_decoders[decoder_name] = decoder
        self.decoder_timestamps[decoder_name] = tasmota.millis()
        print(format("Decoder %s reloaded successfully", decoder_name))
        return true
      else
        self.log_error("RELOAD", "Load failed", "Failed to reload decoder", decoder_name)
        return false
      end
    except .. as e, m
      self.log_error("RELOAD", e, m, decoder_name)
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

  def cmd_simulate(cmd, idx, payload)
    import string

    if !payload return end
    
    tasmota.resp_cmnd_done()
    
    tasmota.set_timer(0, def ()
      # Parse parameters: "rssi,fport,hex_payload"
      # Or use defaults: "hex_payload" only
      var parts = string.split(payload, ',')
      
      var name = self._cmd_result(idx,'LoRaWanName')
      var decoder = self._cmd_result(idx,'LoRaWanDecoder')
      var node = f'{idx}'
      var rssi = -75
      var fport = 85
      var hex_payload = payload
      
      if size(parts) == 3
          rssi = int(parts[0])
          fport = int(parts[1])
          hex_payload = parts[2]
      elif size(parts) == 2
          # Simple format: fport,hex
          fport = int(parts[0])
          hex_payload = parts[1]
      end
      
      import json

      var _b = bytes(hex_payload)
      var _p = []
      for i: 0..size(_b)-1
          _p.push(_b[i])
      end

      # Create the event payload
      var event_payload = {}
      event_payload['LwReceived'] = {}
      event_payload['LwReceived'][name] = {
        'Name': name,
        'Node': node,
        'RSSI': rssi,
        'FPort': fport,
        'Decoder': decoder,
        'Payload': _p,
        'simulated': true,
      }

      var _evp = json.dump(event_payload)

      # Trigger the LwReceived rule
      tasmota.exec_rules(_evp, true)
    end)
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

    var simulated = device_info.find('simulated') ? device_info['simulated'] : false

    if simulated
        log(f"LwD: simulated uplink received", 1)

        device_info['DevEUIh'] = 'simu'
        device_info['DevEUIl'] = 'lated'
    end

    var payload = device_info['Payload']
    if !payload || payload.size() == 0 return true end

    if !self.lw_decoders.find(decoder)
      var loaded_decoder = self.safe_load_decoder(decoder)
      if loaded_decoder
        self.lw_decoders[decoder] = loaded_decoder
      else
        self.log_error("LOAD", "Decoder unavailable", "Unable to load decoder", decoder)
        return true
      end
    end

    var hashCheck = self.get_decoder_property(self.lw_decoders[decoder], 'hashCheck', true)
    
    if hashCheck
      var current_hash = self._calculate_payload_hash(payload)
      if current_hash == self.last_payload_hash return true end
      self.last_payload_hash = current_hash
    end

    var decoded = self.safe_decode_uplink(
      self.lw_decoders[decoder],
      device_info['Name'],
      device_info['Node'],
      device_info['RSSI'],
      device_info['FPort'],
      payload,
      simulated
    )

    if !decoded
      self.log_error("DECODE", "Decode failed", "Decoder returned nil", 
        format("Device:%s, FPort:%d", device_name, device_info['FPort']))
      return true
    end

    decoded['Node'] = device_info['Node']
    decoded['RSSI'] = device_info['RSSI']
    decoded['simulated'] = simulated

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

  def header(name, name_tooltip, battery, battery_last_seen, rssi, last_seen, simulated)
    var msg = format("<tr class='ltd htr'><td><b title='%s'>%s%s</b></td>", name_tooltip, name, simulated ? '&nbsp;&nbsp;&nbsp;🔧' : '')

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

import global
lwdecode = LwDecode_cls()

import webserver
class webPageLoRaWAN : Driver
  var max_node_cached

  def init()
    self.max_node_cached = nil
  end

  def web_add_config_button()
    webserver.content_send("<p><form id=ac action='lrw' style='display: block;' method='get'><button>LoRaWAN</button></form></p>")
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
    if webserver.has_arg('save')
      inode = int(webserver.arg('node'))
      lwdecode._cmd_result(inode, 'LoRaWanAppKey',  webserver.arg('ak') )
      lwdecode._cmd_result(inode, 'LoRaWanDecoder', lwdecode._empty(webserver.arg('dc')) )
      lwdecode._cmd_result(inode, 'LoRaWanName',    lwdecode._empty(webserver.arg('an')) )
      lwdecode._cmd_result(inode, 'LoRaWanNode',    ( webserver.arg('ce') ? '1' : '0' ) )
    end

    var appKey, decoder, name, enabled
    var hintAK = '32 character Application Key'
    var hintDecoder = 'Decoder file, ending in .be'
    var hintAN = 'Device name for MQTT messages'
    var enables = string.split( lwdecode._cmd_result(nil,'LoRaWanNode'), ',')
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
     
     appKey   = lwdecode._cmd_result(node,'LoRaWanAppKey')
     name     = lwdecode._cmd_result(node,'LoRaWanName')
     decoder  = lwdecode._cmd_result(node,'LoRaWanDecoder')

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
webpage = webPageLoRaWAN()
tasmota.add_driver(webpage)

tasmota.cmd('LoraOption3 off')    # Disable embedded decoding
tasmota.cmd('SetOption100 off')   # Keep LwReceived in JSON message
tasmota.cmd('SetOption118 off')   # Keep SENSOR as subtopic name
tasmota.cmd('SetOption119 off')   # Keep device address in JSON message
#tasmota.cmd('SetOption147 on')    # Hide LwReceived MQTT message but keep rule processing
tasmota.cmd('LoRaWanBridge on')
