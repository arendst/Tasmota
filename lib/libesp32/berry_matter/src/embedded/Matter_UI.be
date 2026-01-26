#
# Matter_UI.be - WebUI for Matter configuration in Tasmota
#
# Copyright (C) 2023  Stephan Hadinger & Theo Arends
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

#######################################################################
# Matter Web UI
#
#######################################################################

import matter

#@ solidify:Matter_UI,weak

#################################################################################
# Partition_wizard_UI
#
# WebUI for the partition manager
#################################################################################
class Matter_UI
  static var _CLASSES_TYPES_STD =
                              "|relay|light0|light1|light2|light3|shutter|shutter+tilt"
                              "|gensw_btn"
                              "|temperature|pressure|illuminance|humidity|occupancy|onoff|contact|flow|rain|waterleak"
                              "|airquality"
  static var _CLASSES_TYPES_VIRTUAL = 
                              "-virtual|v_relay|v_light0|v_light1|v_light2|v_light3"
                              "|v_fan"
                              "|v_temp|v_pressure|v_illuminance|v_humidity|v_occupancy|v_contact|v_flow|v_rain|v_waterleak"
                              "|v_airquality"
  static var _CLASSES_TYPES2= "|http_relay|http_light0|http_light1|http_light2|http_light3"
                              "|http_temperature|http_pressure|http_illuminance|http_humidity"
                              "|http_occupancy|http_contact|http_flow|http_rain|http_waterleak"
                              "|http_airquality"
  var device
  var matter_enabled

  # ####################################################################################################
  # Static function to compare two maps (shallow compare)
  # return true if equal
  static def equal_map(a, b)
    # all items of a are in b
    for k: a.keys()
      if !b.contains(k)   return false  end
      if b[k] != a[k]     return false  end
    end
    for k: b.keys()
      if !a.contains(k)   return false  end
      if b[k] != a[k]     return false  end
    end
    return true
  end

  # ####################################################################################################
  # Constructor
  def init(device, matter_enabled)
    self.device = device
    self.matter_enabled = matter_enabled
    tasmota.add_driver(self)
  end

  # ####################################################################################################
  # Init web handlers
  # ####################################################################################################
  # Displays the Configure Matter button on the configuration page
  def web_add_config_button()
    import webserver
    # webserver.content_send("<p></p><form id=ac action='matterc' style='display: block;' method='get'><button>Configure Matter</button></form>")
    webserver.content_send("<p></p><form id=ac action='matterc' style='display: block;' method='get'><button>")
    webserver.content_send(matter._LOGO)
    webserver.content_send(" Matter</button></form>")
  end

  #- ---------------------------------------------------------------------- -#
  #- Show commissioning information and QR Code
  #
  # Returns true if Matter is enabled
  #- ---------------------------------------------------------------------- -#
  def show_enable()
    import webserver
    var matter_enabled = self.matter_enabled

    webserver.content_send("<fieldset><legend><b>&nbsp;Matter &nbsp;</b></legend>"
                           "<p style='width:320px;'>Check the <a href='https://tasmota.github.io/docs/Matter/' target='_blank'>Matter documentation</a>.</p>"
                           "<form action='/matterc' method='post'>")

    # checkbox for Matter enable
    var matter_enabled_checked = matter_enabled ? 'checked' : ''
    webserver.content_send(f"<p><input id='menable' type='checkbox' name='menable' {matter_enabled_checked}>")
    webserver.content_send("<label for='menable'><b>Matter enable</b></label></p>")

    if matter_enabled
      # checkbox for Matter commissioning
      var commissioning_open_checked = self.device.commissioning.commissioning_open != nil ? "checked" : ""
      webserver.content_send(f"<p><input id='comm' type='checkbox' name='comm' {commissioning_open_checked}>")
      webserver.content_send("<label for='comm'><b>Commissioning open</b></label></p>")
      var disable_bridge_mode_checked = self.device.disable_bridge_mode ? " checked" : ""
      webserver.content_send(f"<p><input type='checkbox' name='nobridge'{disable_bridge_mode_checked}><b>Force Static endpoints</b> (non-bridge)</p>")  
    end

    webserver.content_send("<p></p><button name='save' class='button bgrn'>Save</button></form></p>"
                           "</fieldset><p></p>")
  end

  #- ---------------------------------------------------------------------- -#
  #- Show QR Code
  #- ---------------------------------------------------------------------- -#
  def show_qrcode(qr_text)
    import webserver
    # QRCode via UTF8
    var empty = " "
    var lowhalf = "\342\226\204"
    var uphalf = "\342\226\200"
    var full = "\342\226\210"

    var qr = matter.QRCode.encode_str(qr_text)
    var bitmap = qr['bitmap']
    var sz = qr['size']

    webserver.content_send('<style>.qr{font-family:monospace; margin:0; padding:0; white-space:pre; font-size:18px; color:#fff; line-height:100%;}</style>')


    webserver.content_send("<div style='transform:scale(.8,1); display:inline-block;'>")

    var s = "<div class='qr'>"
    webserver.content_send(s)
    s = ""
    for i: 0 .. sz + 1    s += lowhalf  end
    s += "</div>"
    webserver.content_send(s)
    for i: 0 .. (sz+1)/2 - 1
      s = "<div class='qr' style='background-color:#000;'>" + full
      for j: 0 .. sz - 1
        var high = (bitmap[i*2][j] == " ")
        var low = (i*2+1 < sz) ? (bitmap[i*2+1][j] == " ") : true     # default to true for bottom margin if size is odd
        s += high ? (low ? full : uphalf) : (low ? lowhalf : empty)
      end
      s += full
      s += "</div>"
      webserver.content_send(s)
    end
    # webserver.content_send("</div>")
    if sz % 2 == 0
      s = "<div class='qr' style='background-color:#000;'>"
      for i: 0 .. sz + 1    s += uphalf  end
      s += "/<div>"
      webserver.content_send(s)
    end

    webserver.content_send("</div>")
  end

  #- ---------------------------------------------------------------------- -#
  #- Show commissioning information and QR Code
  #- ---------------------------------------------------------------------- -#
  def show_commissioning_info()
    import webserver

    var seconds_left = (self.device.commissioning.commissioning_open - tasmota.millis()) / 1000
    if seconds_left < 0   seconds_left = 0 end
    var min_left = (seconds_left + 30) / 60

    webserver.content_send(f"<fieldset><legend><b>&nbsp;Commissioning open for {min_left:i} min&nbsp;</b></legend><p></p>")

    var pairing_code = self.device.commissioning.compute_manual_pairing_code()
    webserver.content_send(f"<p>Manual pairing code:<br><b>{pairing_code[0..3]}-{pairing_code[4..6]}-{pairing_code[7..]}</b></p><hr>")

    webserver.content_send("<div><center>")
    var qr_text = self.device.commissioning.compute_qrcode_content()
    self.show_qrcode(qr_text)
    webserver.content_send(f"<p> {qr_text}</p>")
    webserver.content_send("</div><p></p></fieldset><p></p>")

  end

  #- ---------------------------------------------------------------------- -#
  #- Show Passcode / discriminator form
  #- ---------------------------------------------------------------------- -#
  def show_passcode_form()
    import webserver

    webserver.content_send("<fieldset><legend><b>&nbsp;Matter Advanced Configuration&nbsp;</b></legend><p></p>"
    
                           "<form action='/matterc' method='post' onsubmit='return confirm(\"This will cause a restart.\");'>"
                           "<p>Passcode:</p>")
    webserver.content_send(f"<input type='number' min='1' max='99999998' name='passcode' value='{self.device.root_passcode:i}'>")
    webserver.content_send("<p>Distinguish id:</p>")
    webserver.content_send(f"<input type='number' min='0' max='4095' name='discriminator' value='{self.device.root_discriminator:i}'>")
    var ipv4only_checked = self.device.ipv4only ? " checked" : ""
    webserver.content_send(f"<p><input type='checkbox' name='ipv4'{ipv4only_checked}>IPv4 only</p>")
    webserver.content_send("<p></p><button name='passcode' class='button bgrn'>Change</button></form></p>"
                           "<p></p></fieldset><p></p>")

  end

  #- ---------------------------------------------------------------------- -#
  #- Show commissioning information and QR Code
  #- ---------------------------------------------------------------------- -#
  def show_fabric_info()
    import webserver

    webserver.content_send("<fieldset><legend><b>&nbsp;Fabrics&nbsp;</b></legend><p></p>"
                           "<p>Associated fabrics:</p>")

    if size(self.device.sessions.sessions) == 0
      webserver.content_send("<p><b>None</b></p>")
    else
      var first = true
      for f : self.device.sessions.fabrics.persistables()
        if !first     webserver.content_send("<hr>") end
        first = false

        var label = f.fabric_label
        if !label   label = "<No label>"    end
        label = webserver.html_escape(label)      # protect against HTML injection
        
        webserver.content_send(f"<fieldset><legend><b>&nbsp;#{f.get_fabric_index():i} {label}</b> ({f.get_admin_vendor_name()})&nbsp;</legend><p></p>")

        var fabric_rev = f.get_fabric_id().copy().reverse()
        var deviceid_rev = f.get_device_id().copy().reverse()
        webserver.content_send(f"Fabric: {fabric_rev.tohex()}<br>")
        webserver.content_send(f"Device: {deviceid_rev.tohex()}<br>&nbsp;")

        webserver.content_send("<form action='/matterc' method='post' onsubmit='return confirm(\"Are you sure?\");'>")
        webserver.content_send(f"<input name='del_fabric' type='hidden' value='{f.get_fabric_index():i}'>")
        webserver.content_send("<button name='del' class='button bgrn'>Delete Fabric</button></form></p>"
        
                               "<p></p></fieldset><p></p>")
      end
    end

    webserver.content_send("<p></p></fieldset><p></p>")

  end

  #----------------------------------------------------------------------- -#
  #- Show Javasrcript hints for classes arguments
  #----------------------------------------------------------------------- -#
  #
  # Exmaple:
  #   var hm = {"relay":0,"shutter+tilt":0,"light2":1,"illuminance":2,"onoff":1,"pressure":2,"light1":1,"humidity":2,"shutter":0,"occupancy":3,"temperature":2,"light3":1,"light0":1};
  #   var hl = ["Enter Relay number","Not used","Enter Filter pattern","Enter Switch number"];
  def show_plugins_hints_js(*class_list)
    import webserver
    import json
    import string

    var class_types = []
    for cl: class_list
      class_types += string.split(cl, '|')
    end
    
    var hm = {}
    var hl = []

    for typ: class_types
      if typ == ''    continue  end
      var cl = self.device.plugins_classes.find(typ)
      if cl != nil
        var hint = cl.ARG_HINT
        if hint != nil
          var idx = hl.find(hint)
          if idx == nil           # not already in list, add it
            idx = size(hl)        # save index for assignment
            hl.push(hint)
          end
          hm[typ] = idx
        end
      end
    end

    webserver.content_send(f""
      "<script type='text/javascript'>"
      "var hm={json.dump(hm)};"
      "var hl={json.dump(hl)};"
      "</script>")

    webserver.content_send(matter._ADD_ENDPOINT_JS)

  end

  #----------------------------------------------------------------------- -#
  #- Show plugins configuration
  #----------------------------------------------------------------------- -#
  def show_plugins_configuration()
    import webserver
    import string
    import introspect

    webserver.content_send("<fieldset><legend><b>&nbsp;Current Configuration&nbsp;</b></legend><p></p>")

    webserver.content_send("<form action='/matterc' method='post'>"
                           "<p><b>Local sensors and devices</b></p>"
                           "<table style='width:100%'>")
    webserver.content_send("<tr>"
                           "<td width='25' style='font-size:smaller;'>#</td>"
                           "<td width='78' style='font-size:smaller;'>Name</td>"
                           "<td width='115' style='font-size:smaller;'>Type</td>"
                           "<td style='font-size:smaller;'>Parameter</td>"
                           "<td width='15' style='font-size:smaller;'></td>"
                           "</tr>")

    # display one line per plug-in
    self.device.plugins_config.remove("0")      # remove any leftover from ancient configuration
    var endpoints = self.device.k2l_num(self.device.plugins_config)
    var i = 0
    var found = false
  
    while i < size(endpoints)
      var ep = endpoints[i]
      var conf = self.device.plugins_config.find(str(ep))
      var typ = conf.find('type')
      if !typ   i += 1   continue    end

      # skip any remote class
      if string.find(typ, "http_") == 0   i += 1   continue    end

      var cl = self.device.plugins_classes.find(typ)
      var arg = ""
      var arg_hint = ""
      if cl != nil
        arg = cl.ui_conf_to_string(cl, conf)
        arg_hint = cl.ARG_HINT
      end

      found = true
      webserver.content_send(f"<tr><td style='font-size:smaller;'><b>{ep:i}</b></td>")
      webserver.content_send(format("<td style='font-size:smaller;'><input type='text' name='nam%i' size='1' value='%s'></td>",
                             ep, webserver.html_escape(conf.find('name', ''))))
      webserver.content_send(f"<td style='font-size:smaller;'><b>{self.plugin_name(conf.find('type', ''))}</b></td>")
      webserver.content_send(format("<td style='font-size:smaller;'><input type='text' name='arg%i' size='1' value='%s' placeholder='%s' title='%s'></td>",
                             ep, webserver.html_escape(arg), webserver.html_escape(arg_hint), webserver.html_escape(arg_hint)))
      webserver.content_send(f"<td style='text-align:center;'><button name='del{ep:i}' title='Delete Endpoint {ep:i}' "
                              "style='background:none;border:none;line-height:1;'"
                              " onclick=\"return confirm('Confirm removing endpoint {ep:i}')\""
                              ">"
                              "&#128293;</button></td></tr>")
      i += 1
    end
    webserver.content_send("</table>")

    # if array is empty, still display <none>
    if !found
      webserver.content_send("<p>&lt;none&gt;</p>")
    end
    webserver.content_send("<p></p>")

    # iterate on each remote device
    var remotes = []
    for conf: self.device.plugins_config
      var url = conf.find("url")
      if url != nil
        remotes.push(url)
      end
    end
    self.device.sort_distinct(remotes)
    # log("MTR: remotes: "+str(remotes), 4)

    for remote: remotes

      var remote_html = webserver.html_escape(remote)
      var host_device_name = webserver.html_escape( self.device.get_plugin_remote_info(remote).find('name', remote) )
      webserver.content_send(f"&#x1F517; <a target='_blank' title='http://{remote_html}/' href=\"http://{remote_html}/?\">{host_device_name}</a>")
      webserver.content_send("<table style='width:100%'>"
                             "<tr>"
                             "<td width='25'></td>"
                             "<td width='78'></td>"
                             "<td width='115'>"
                             "</td><td>"
                             "</td><td width='15'></td>"
                             "</tr>")

      found = false
      i = 0
      while i < size(endpoints)
        var ep = endpoints[i]
        var conf = self.device.plugins_config.find(str(ep))
        var typ = conf.find('type')
        if !typ   i += 1   continue    end

        # skip any non-remote class
        if string.find(typ, "http_") != 0   i += 1   continue    end
        # check if it's the right remote
        if conf.find("url") != remote   i += 1   continue    end

        var cl = self.device.plugins_classes.find(typ)
        var arg = ""
        var arg_hint = ""
        if cl != nil
          arg = cl.ui_conf_to_string(cl, conf)
          arg_hint = cl.ARG_HINT
        end

        found = true
        webserver.content_send(f"<tr><td width='22' style='font-size:smaller;'><b>{ep:i}</b></td>")
        webserver.content_send(format("<td width='78' style='font-size:smaller;'><input type='text' name='nam%i' size='1' value='%s' placeholder='(optional)'></td>",
                               ep, webserver.html_escape(conf.find('name', ''))))

        webserver.content_send(format("<td width='115' style='font-size:smaller;'><b>%s</b></select></td>", self.plugin_name(conf.find('type', ''))))
        webserver.content_send(format("<td style='font-size:smaller;'><input type='text' name='arg%i' size='8' value='%s' title='%s'></td>",
                              ep, webserver.html_escape(arg), webserver.html_escape(arg_hint)))
        webserver.content_send(f"<td width='15' style='text-align:center;'><button name='del{ep:i}' "
                                "style='background:none;border:none;line-height:1;'"
                                " onclick=\"return confirm('Confirm removing endpoint')\""
                                ">"
                                "&#128293;</button></td></tr>")
        i += 1
      end
      webserver.content_send("</table><p></p>")

    end # for remote: self.device.get_remotes_list()
    


    if !found
      webserver.content_send("<p>&lt;none&gt;</p>")
    end

    webserver.content_send("<button name='config' class='button bgrn'>"
                           "Change configuration</button></form><p></p></fieldset>")

    
    # Add new endpoint section
    if self.device.zigbee
      self.show_plugins_hints_js(self._CLASSES_TYPES_STD, self.device.zigbee._CLASSES_TYPES, self._CLASSES_TYPES_VIRTUAL)
    else
      self.show_plugins_hints_js(self._CLASSES_TYPES_STD, self._CLASSES_TYPES_VIRTUAL)
    end

    webserver.content_send("<p></p><fieldset><legend><b>&nbsp;Add to Configuration&nbsp;</b></legend><p></p>"
                           "<p><b>Add local sensor or device</b></p>"
                           "<form action='/matterc' method='post'>"
                           "<table style='width:100%'>"
                           "<tr>"
                           "<td width='100' style='font-size:smaller;'>Name</td>"
                           "<td width='115' style='font-size:smaller;'>Type</td>"
                           "<td style='font-size:smaller;'>Parameter</td>"
                           "</tr>"
                           
                           "<tr>"
                           "<td style='font-size:smaller;'><input type='text' name='nam' size='1' value='' placeholder='(optional)' title=''></td>"
                           "<td style='font-size:smaller;'><select id='pi' name='pi' onchange='otm(\"arg\",this.value)'>")
    if self.device.zigbee
      self.plugin_option('', self._CLASSES_TYPES_STD, self.device.zigbee._CLASSES_TYPES, self._CLASSES_TYPES_VIRTUAL)
    else
      self.plugin_option('', self._CLASSES_TYPES_STD, self._CLASSES_TYPES_VIRTUAL)
    end
    webserver.content_send("</select></td>"
                           "<td style='font-size:smaller;'><input type='text' id='arg' name='arg' size='1' value=''></td>"
                           "</tr></table>"
                           
                           "<div style='display: block;'></div>"
                           "<button name='addep' class='button bgrn'"
                           ">Create new endpoint</button></form>"

    # Add remote endpoint
                           "<hr><p><b>Add Remote Tasmota or OpenBK</b></p>"
                           "<form action='/matteradd' method='get'>"
                           "<table style='width:100%'>"

                           "<tr><td width='30' style='font-size:smaller;'><b>http://</b></td><td><input type='text' name='url' size='8' value='' required placeholder='IP or domain'></td><td width='10' style='font-size:smaller;'><b>/</b></td></tr>"
                           "</tr></table>"
                           
                           "<div style='display: block;'></div>"
                           "<button class='button bgrn'>"
                           "Auto-configure remote Tasmota</button></form><hr>"
    
    # button "Reset and Auto-discover"
                          "<form action='/matterc' method='post'"
                           "onsubmit='return confirm(\"This will RESET the configuration to the default. You will need to associate again.\");'>"
                           "<button name='auto' class='button bred'>Reset all and Auto-discover</button><p></p></form>"
                           
                           "<p></p></fieldset>")

  end

  #- ---------------------------------------------------------------------- -#
  #- Show pretty name for plugin class
  #- ---------------------------------------------------------------------- -#
  def plugin_name(cur, *class_list)
    if cur == ''  return ''  end
    return self.device.get_plugin_class_displayname(cur)
  end

  #- ---------------------------------------------------------------------- -#
  #- Show all possible classes for plugin
  #- ---------------------------------------------------------------------- -#
  def plugin_option(cur, *class_list)
    import webserver
    import string
    var class_types = []
    for cl: class_list
      class_types += string.split(cl, '|')
    end
    
    var i = 0
    while i < size(class_types)
      var typ = class_types[i]
      if typ == ''
        webserver.content_send("<option value=''></option>")
      elif typ == '-virtual'
        webserver.content_send("<option value='' disabled>--- Virtual Devices ---</option>")
      elif typ == '-zigbee'
        webserver.content_send("<option value='' disabled>--- Zigbee Devices ---</option>")
      else
        var nam = self.device.get_plugin_class_displayname(typ)
        webserver.content_send(format("<option value='%s'%s>%s</option>", typ, (typ == cur) ? " selected" : "", nam))
      end
      i += 1
    end
  end


  #######################################################################
  # Display the advanced configuration page
  #######################################################################
  def page_part_mgr_adv()
    import webserver

    if !webserver.check_privileged_access() return nil end

    webserver.content_start("Matter Advanced Configuration")           #- title of the web page -#
    webserver.content_send_style()                  #- send standard Tasmota styles -#

    if self.matter_enabled
      self.show_passcode_form()
      self.show_fabric_info()
    end
    self.web_add_config_button()
    #webserver.content_button(webserver.BUTTON_CONFIGURATION)
    webserver.content_stop()                        #- end of web page -#
  end


  #######################################################################
  # Display the complete page
  #######################################################################
  def page_part_mgr()
    import webserver

    if !webserver.check_privileged_access() return nil end

    webserver.content_start("Matter")           #- title of the web page -#
    webserver.content_send_style()                  #- send standard Tasmota styles -#

    self.show_enable()
    if self.matter_enabled
      self.show_plugins_configuration()
    end

    webserver.content_send("<div style='display: block;'></div>"
                           "<p></p><form id='butmat' style='display: block;' action='mattera' method='get'><button name=''>Advanced Configuration</button></form>")

    webserver.content_button(webserver.BUTTON_CONFIGURATION)
    webserver.content_stop()                        #- end of web page -#
  end

  #---------------------------------------------------------------------- -#
  # Generate configuration map from Status 10 and Status 11
  #
  # Returns a list of maps: [ {"type":"temperature", "filter":"ESP32#Temperature"} ]
  #---------------------------------------------------------------------- -#
  def generate_config_from_status(status10, status11)
    var config_list = []

    # count `Power` and `Power<x>`
    var power_cnt = 0
    if status11.contains("POWER")
      power_cnt = 1
    else
      var idx = 1
      while true
        if status11.contains("POWER" + str(idx))
          power_cnt = idx
          idx += 1
        else
          break
        end
      end
    end
    # Now `power_cnt` contains the number of Relays including light

    # detect lights
    var light1, light2, light3    # contains a relay number of nil
    if status11.contains("HSBColor")
      light3 = power_cnt
      power_cnt -= 1
    elif status11.contains("CT")
      light2 =  power_cnt
      power_cnt -= 1
    elif status11.contains("Dimmer")
      light1 =  power_cnt
      power_cnt -= 1
    end

    # rest is relays
    for i: 1..power_cnt
      config_list.push({'type': 'light0', 'relay': i})
    end

    # show lights
    if light1 != nil
      config_list.push({'type': 'light1', 'relay': light1})
    end
    if light2 != nil
      config_list.push({'type': 'light2', 'relay': light2})
    end
    if light3 != nil
      config_list.push({'type': 'light3', 'relay': light3})
    end


    # detect sensors
    config_list += self.device.autoconf.autoconf_sensors_list(status10)

    return config_list
  end

  #- ---------------------------------------------------------------------- -#
  #- Probe remote device
  #- ---------------------------------------------------------------------- -#
  def show_remote_autoconf(url)
    import webserver
    import json

    if url == ''  return end
    var timeout = matter.Plugin_Device.PROBE_TIMEOUT
    var http_remote = matter.HTTP_remote(nil, url, timeout)
    # Status 10
    var status10 = http_remote.call_sync('Status 10', timeout)
    if status10 != nil   status10 = json.load(status10)                end
    if status10 != nil   status10 = status10.find('StatusSNS')         end
    # Status 11
    var status11
    if status10 != nil
      status11 = http_remote.call_sync('Status 11', timeout)
      if status11 != nil   status11 = json.load(status11)           end
      if status11 != nil   status11 = status11.find('StatusSTS')     end
    end
    
    if status10 != nil && status11 != nil
      log(format("MTR: probed '%s' status10=%s satus11=%s", url, str(status10), str(status11)), 3)

      var config_list = self.generate_config_from_status(status10, status11)

      self.show_plugins_hints_js(self._CLASSES_TYPES2)

      webserver.content_send("<fieldset><legend><b>&nbsp;Matter Remote Device&nbsp;</b></legend><p></p>"
                             "<p><b>Add Remote sensor or device</b></p>")

      var remote_html = webserver.html_escape(url)
      webserver.content_send(f"<p>&#x1F517; <a target='_blank' href=\"http://{remote_html}/?\">{remote_html}</a></p>")

      # Add new endpoint section
      webserver.content_send("<form action='/matterc' method='post'>"
                             "<table style='width:100%'>"
                             "<tr>"
                             "<td width='100' style='font-size:smaller;'>Name</td>"
                             "<td width='115' style='font-size:smaller;'>Type</td>"
                             "<td style='font-size:smaller;'>Parameter</td>"
                             "</tr>")

      webserver.content_send(format("<input name='url' type='hidden' value='%s'>", webserver.html_escape(url)))

      var i = 0
      while i < size(config_list)
        var config = config_list[i]
        var typ = config.find('type', '')
        if typ != ''     typ = "http_" + typ     end

        var cl = self.device.plugins_classes.find(typ)
        var arg = ""
        var arg_hint = ""
        if cl != nil
          arg = cl.ui_conf_to_string(cl, config)
          arg_hint = cl.ARG_HINT
        end

        webserver.content_send(format("<tr><td style='font-size:smaller;'><input type='text' name='nam%i' size='1' value='' placeholder='(optional)'></td>", i))
        webserver.content_send(format("<td style='font-size:smaller;'><select name='pi%i' onchange='otm(\"arg%i\",this.value)'>", i, i))
        self.plugin_option(typ, self._CLASSES_TYPES2)
        webserver.content_send("</select></td>"
                               "<td style='font-size:smaller;'>")
        webserver.content_send(format("<input type='text' id='arg%i' name='arg%i' size='1' value='%s' placeholder='%s' title='%s'>",
                               i, i, webserver.html_escape(arg), webserver.html_escape(arg_hint), webserver.html_escape(arg_hint)))
        webserver.content_send("</td></tr>")
        i += 1
      end
      # empty line for new endpoint
      webserver.content_send(format("<tr><td style='font-size:smaller;'><input type='text' name='nam%i' size='1' value='' placeholder='(optional)'></td>", i))
      webserver.content_send(format("<td style='font-size:smaller;'><select name='pi%i' onchange='otm(\"arg%i\",this.value)'>", i, i))
      self.plugin_option('', self._CLASSES_TYPES2)
      webserver.content_send("</select></td>"
                             "<td style='font-size:smaller;'>")
      webserver.content_send(format("<input type='text' id='arg%i' name='arg%i' size='1' value='%s'>",
                             i, i, ''))
      webserver.content_send("</td></tr>")

      # end of table
      webserver.content_send("</table>"
                             
                             "<div style='display: block;'></div>"
                             "<button name='addrem' class='button bgrn'>"
                             "Add endpoints</button></form>"
                             
                             "</form></fieldset>")

    else
      webserver.content_send(format("<p><b>Unable to connect to '%s'</b></p>", webserver.html_escape(url)))
    end


  end

  #######################################################################
  # Display the page for adding a new endpoint
  #######################################################################
  def page_part_mgr_add()
    import webserver

    if !webserver.check_privileged_access() return nil end

    webserver.content_start("Matter Create new endpoint")           #- title of the web page -#
    webserver.content_send_style()                  #- send standard Tasmota styles -#

    var url = webserver.arg("url")
    if self.matter_enabled
      self.show_remote_autoconf(url)
    end
    webserver.content_button(webserver.BUTTON_CONFIGURATION)
    webserver.content_stop()                        #- end of web page -#
  end


  #######################################################################
  # Web Controller, called by POST to `/matterc`
  #######################################################################
  def page_part_ctl()
    import webserver
    if !webserver.check_privileged_access() return nil end

    import string
    import partition_core
    import persist

    var error

    try

      # debug information about parameters
      # for i:0..webserver.arg_size()-1
      #   log(format("MTR: Arg%i '%s' = '%s'", i, webserver.arg_name(i), webserver.arg(i)))
      # end

      #---------------------------------------------------------------------#
      # Change Passcode and/or Passcode
      #---------------------------------------------------------------------#
      if webserver.has_arg("passcode") || webserver.has_arg("discriminator")
        log(format("MTR: /matterc received '%s' command", 'passcode'), 3)
        if webserver.has_arg("passcode")
          self.device.root_passcode = int(webserver.arg("passcode"))
        end
        if webserver.has_arg("discriminator")
          self.device.root_discriminator = int(webserver.arg("discriminator"))
        end
        self.device.ipv4only = webserver.arg("ipv4") == 'on'
        self.device.save_param()

        #- and force restart -#
        webserver.redirect("/?rst=")

      elif webserver.has_arg("save")
        var matter_enabled_requested = webserver.has_arg("menable")
        var matter_commissioning_requested = webserver.has_arg("comm")
        var matter_disable_bridge_mode_requested = (webserver.arg("nobridge") == 'on')
        if self.device.disable_bridge_mode != matter_disable_bridge_mode_requested
          self.device.disable_bridge_mode = matter_disable_bridge_mode_requested
          self.device.save_param()
        end

        if matter_enabled_requested != self.matter_enabled
          if matter_enabled_requested
            log(format("MTR: /matterc received '%s' command", 'enable'), 3)
            tasmota.cmd("SetOption" + str(matter.MATTER_OPTION) + " 1")
          else
            log(format("MTR: /matterc received '%s' command", 'disable'), 3)
            tasmota.cmd("SetOption" + str(matter.MATTER_OPTION) + " 0")
          end
          #- and force restart -#
          webserver.redirect("/?rst=")
        elif matter_commissioning_requested != (self.device.commissioning.commissioning_open != nil)
          if matter_commissioning_requested
            self.device.commissioning.start_root_basic_commissioning()
          else
            self.device.commissioning.stop_basic_commissioning()
          end
        
          #- and force restart -#
          webserver.redirect("/")
        else
          webserver.redirect("/")
        end

      #---------------------------------------------------------------------#
      # Delete Fabric
      #---------------------------------------------------------------------#
      elif webserver.has_arg("del_fabric")
        log(format("MTR: /matterc received '%s' command", 'del_fabric'), 3)
        var del_fabric = int(webserver.arg("del_fabric"))
        var idx = 0
        var fabrics = self.device.sessions.fabrics
        while idx < size(fabrics)
          if fabrics[idx].get_fabric_index() == del_fabric
            self.device.remove_fabric(fabrics[idx])
            break
          else
            idx += 1
          end
        end
        #- reload advanced page -#
        webserver.redirect("/mattera?")

      #---------------------------------------------------------------------#
      # Reset to default auto-configuration
      #---------------------------------------------------------------------#
      elif webserver.has_arg("auto")
        log(format("MTR: /matterc received '%s' command", 'auto'), 3)
        self.device.reset_param()
        #- and force restart -#
        webserver.redirect("/?rst=")

      #---------------------------------------------------------------------#
      # Apply new configuration template
      #---------------------------------------------------------------------#
      elif webserver.has_arg("config")
        log(format("MTR: /matterc received '%s' command", 'config'), 3)
        var needs_saving = false
        # iterate by endpoint number
        for i:0..webserver.arg_size()-1
          var arg_name = webserver.arg_name(i)
          if string.find(arg_name, "arg") == 0      # 'arg<i>' with i being the endpoint
            var arg_ep = int(arg_name[3..])         # target endpoint as int
            var arg = webserver.arg(i)              # text value

            var conf_ep = self.device.plugins_config.find(str(arg_ep))    # find the corresponding configuration map

            if conf_ep != nil     # found
              var typ_class = self.device.plugins_classes.find(conf_ep.find('type', ''))
              if typ_class != nil
                log(format("MTR: ep=%i arg=%s", arg_ep, arg), 3)
                # compute the actual value
                var prev_arg = typ_class.ui_conf_to_string(typ_class, conf_ep)
                var changed = (prev_arg != arg)
                log(format("MTR: ep=%i prev_arg='%s' arg='%s' %s", arg_ep, prev_arg, arg, prev_arg != arg ? "changed" : ""), 3)

                if changed
                  needs_saving = true
                  typ_class.ui_string_to_conf(typ_class, conf_ep, arg)
                  var pl = self.device.find_plugin_by_endpoint(arg_ep)
                  if pl
                    log(format("MTR: apply conf '%s' (%i) to %s", conf_ep, arg_ep, pl), 3)
                    pl.parse_configuration(conf_ep)
                  end
                end

              end
            else            
              log(format("MTR: ep=%i not found", arg_ep), 3)
            end
          elif string.find(arg_name, "nam") == 0    # 'nam<i>' with i being the endpoint
            var nam_ep = int(arg_name[3..])         # target endpoint as int
            var nam = webserver.arg(i)              # text value

            var conf_ep = self.device.plugins_config.find(str(nam_ep))    # find the corresponding configuration map

            if conf_ep != nil     # found
              var prev_name = conf_ep.find("name", "")
              var changed = (prev_name != nam)
              if changed
                needs_saving = true
                var pl = self.device.find_plugin_by_endpoint(nam_ep)
                if pl
                  # apply or remove
                  pl.set_name(nam)
                  if nam
                    conf_ep['name'] = nam
                  else
                    conf_ep.remove('name')
                  end
                  log(format("MTR: apply name '%s' (%i) to %s", conf_ep, nam_ep, pl), 3)
                  pl.parse_configuration(conf_ep)
                end
              end
            end
          end
        end

        log(format("MTR: config = %s", str(self.device.plugins_config)), 3)

        if error
          log(format("MTR: config error = %s", error), 3)
        else
          if needs_saving || !self.device.plugins_persist
            self.device.plugins_persist = true
            self.device.save_param()
          end
          #- and reload -#
          webserver.redirect("/matterc?")
        end
      #---------------------------------------------------------------------#
      # Add new endpoint for local sensor or device
      #---------------------------------------------------------------------#
      elif webserver.has_arg("addep")
        var typ = webserver.arg('pi')
        var arg = webserver.arg('arg')
        var nam = webserver.arg('nam')
        log(format("MTR: add endpoint typ='%s' arg='%s'", typ, arg), 3)

        # check if type exists
        var typ_class = self.device.plugins_classes.find(typ)
        if typ_class != nil
          var config = {}
          if nam    config['name'] = nam  end
          typ_class.ui_string_to_conf(typ_class, config, arg)
          self.device.bridge_add_endpoint(typ, config)
        end
        #- and reload -#
        webserver.redirect("/matterc?")

      #---------------------------------------------------------------------#
      # Add new endpoint for remote sensor or device
      #---------------------------------------------------------------------#
      elif webserver.has_arg("addrem")
        var url = webserver.arg('url')
        if url == nil || url == ''    raise "value_error", "url shouldn't be null"  end

        # iterate by id
        var idx = 0
        var idx_str = str(idx)
        while webserver.has_arg('pi'+idx_str)
          var typ = webserver.arg('pi'+idx_str)
          var arg = webserver.arg('arg'+idx_str)
          var nam = webserver.arg('nam'+idx_str)

          if typ != ''
            # check if type exists
            var typ_class = self.device.plugins_classes.find(typ)
            if typ_class != nil
              var config = {'url': url, 'type': typ}
              if nam
                config['name'] = nam
              end
              typ_class.ui_string_to_conf(typ_class, config, arg)
              # check if configuration is already present
              var duplicate = false
              for c: self.device.plugins_config   # iterate on values, not on keys()
                # log(format("MTR: map_compare '%s' ?= '%s' -> %s", str(c), str(config), str(self.equal_map(c,config))), 3)
                if self.equal_map(c,config)   duplicate = true  break   end
              end
              # not a duplicate, add it
              if !duplicate
                log(format("MTR: remote add url='%s' type='%s' arg='%s'", url, typ, arg), 3)
                self.device.bridge_add_endpoint(typ, config)
              end
            end
          end
          idx += 1
          idx_str = str(idx)
        end
        #- and go back to Matter configuration -#
        webserver.redirect("/matterc?")

      else

      #---------------------------------------------------------------------#
      # Check if an endpoint needs to be deleted
      #---------------------------------------------------------------------#
        var ep_deleted
        for i:0..webserver.arg_size()-1
          var arg_name = webserver.arg_name(i)
          if string.find(arg_name, "del") == 0
            ep_deleted = int(arg_name[3..])
            break
          end
        end
        # check if we found an endpoint to be deleted
        if ep_deleted != nil
          self.device.bridge_remove_endpoint(ep_deleted)
          webserver.redirect("/matterc?")
        end

      end

      if error
        webserver.content_start("Parameter error")           #- title of the web page -#
        webserver.content_send_style()                  #- send standard Tasmota styles -#
        webserver.content_send(format("<p style='width:340px;'><b>Error:</b>%s</p>", webserver.html_escape(error)))
        webserver.content_button(webserver.BUTTON_CONFIGURATION) #- button back to configuration page -#
        webserver.content_stop()                        #- end of web page -#
      end

    except .. as e, m
      log(format("BRY: Exception> '%s' - %s", e, m), 2)
      #- display error page -#
      webserver.content_start("Parameter error")           #- title of the web page -#
      webserver.content_send_style()                  #- send standard Tasmota styles -#

      webserver.content_send(format("<p style='width:340px;'><b>Exception:</b><br>'%s'<br>%s</p>", e, m))

      webserver.content_button(webserver.BUTTON_CONFIGURATION) #- button back to configuration page -#
      webserver.content_stop()                        #- end of web page -#
    end
  end

  #######################################################################
  # Show bridge status
  #######################################################################
  def show_bridge_status()
    if (self.device.plugins == nil)   return  end
    import webserver
    var bridge_plugin_by_host
    
    var idx = 0
    while idx < size(self.device.plugins)
      var plg = self.device.plugins[idx]

      if plg.BRIDGE
        if bridge_plugin_by_host == nil     bridge_plugin_by_host = {}   end
        var host = plg.http_remote.addr

        if !bridge_plugin_by_host.contains(host)    bridge_plugin_by_host[host] = []    end
        bridge_plugin_by_host[host].push(plg)

      end
      idx += 1
    end

    if bridge_plugin_by_host == nil     return    end         # no remote device, abort

    # set specific styles
    webserver.content_send("<hr>")
    webserver.content_send("<table style='width:100%'>")
    webserver.content_send(matter._STYLESHEET)

    for host: self.device.k2l(bridge_plugin_by_host)
      var host_html = webserver.html_escape(host)
      var host_device_name = webserver.html_escape( self.device.get_plugin_remote_info(host).find('name', host) )
      webserver.content_send(f"<tr class='ztdm htrm'><td>&#x1F517; <a target='_blank' title='http://{host_html}/' href=\"http://{host_html}/?\"'>{host_device_name}</a></td>")
      var http_remote = bridge_plugin_by_host[host][0].http_remote    # get the http_remote object from the first in list
      webserver.content_send(http_remote.web_last_seen())

      for plg: bridge_plugin_by_host[host]
        webserver.content_send("<tr class='htrm'><td colspan='2'>")
        plg.web_values()                                      # show values
        webserver.content_send("</td></tr>")
      end
    end


    webserver.content_send("</table><hr>")

  end

  #- display sensor value in the web UI -#
  def web_sensor()
    import webserver

    if self.matter_enabled

      # mtc0 = close, mtc1 = open commissioning
      var fabrics_count = (self.device.sessions != nil) ? self.device.sessions.count_active_fabrics() : 0
      if fabrics_count == 0
        webserver.content_send(format("<div style='text-align:right;font-size:11px;color:#aaa;padding:0px;'>%s</div>", "Matter: No active association"))
      else
        var plural = fabrics_count > 1
        webserver.content_send(format("<div style='text-align:right;font-size:11px;color:#aaa;padding:0px;'>%s</div>", "Matter: " + str(fabrics_count) + " active association" + (plural ? "s" : "")))
      end

      self.show_bridge_status()

      if self.device.commissioning.is_root_commissioning_open()
        self.show_commissioning_info()
      end

    end
  end

  def web_get_arg()
    import webserver
    if   webserver.has_arg("mtc0")    # Close Commissioning
      self.device.commissioning.stop_basic_commissioning()
    elif webserver.has_arg("mtc1")    # Open Commissioning
      self.device.commissioning.start_root_basic_commissioning()
    end
  end

  #- ---------------------------------------------------------------------- -#
  # respond to web_add_handler() event to register web listeners
  #- ---------------------------------------------------------------------- -#
  #- this is called at Tasmota start-up, as soon as Wifi/Eth is up and web server running -#
  def web_add_handler()
    import webserver
    #- we need to register a closure, not just a function, that captures the current instance -#
    webserver.on("/matterc", / -> self.page_part_mgr(), webserver.HTTP_GET)
    webserver.on("/matterc", / -> self.page_part_ctl(), webserver.HTTP_POST)
    webserver.on("/mattera", / -> self.page_part_mgr_adv(), webserver.HTTP_GET)   # advanced
    webserver.on("/matteradd", / -> self.page_part_mgr_add(), webserver.HTTP_GET)   # add endpoint
  end
end
matter.UI = Matter_UI

# optimization of constants
import solidify
solidify.nocompact(Matter_UI.page_part_ctl)