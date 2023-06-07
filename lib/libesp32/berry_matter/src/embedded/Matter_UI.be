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
  static var _CLASSES_TYPES = "|relay|light0|light1|light2|light3|shutter|shutter+tilt"
                              "|temperature|pressure|illuminance|humidity|occupancy|onoff"
  static var _CLASSES_TYPES2= "-http|http_relay|http_light0|http_light1|http_light2|http_light3"
                              "|http_temperature|http_pressure|http_illuminance|http_humidity"
                              "|http_occupancy"
  var device

  def init(device)
    self.device = device
    tasmota.add_driver(self)
  end

  # ####################################################################################################
  # Init web handlers
  # ####################################################################################################
  # Displays a "Autoconf" button on the configuration page
  def web_add_config_button()
    import webserver
    # webserver.content_send("<p><form id=ac action='matterc' style='display: block;' method='get'><button>Configure Matter</button></form></p>")
    webserver.content_send("<p><form id=ac action='matterc' style='display: block;' method='get'><button>")
    webserver.content_send(matter._LOGO)
    webserver.content_send(" Configure Matter</button></form></p>")
  end

  #- ---------------------------------------------------------------------- -#
  #- Show commissioning information and QR Code
  #
  # Returns true if Matter is enabled
  #- ---------------------------------------------------------------------- -#
  def show_enable(p)
    import webserver
    import string
    var matter_enabled = tasmota.get_option(matter.MATTER_OPTION)

    webserver.content_send(string.format("<fieldset><legend><b>&nbsp;Matter %s&nbsp;</b></legend><p></p>",
                                          matter_enabled ? "Enabled" : "Disabled"))

    webserver.content_send("<p style='width:320px;'>Check the <a href='https://tasmota.github.io/docs/Matter/' target='_blank'>Matter documentation</a>.</p>")

    webserver.content_send("<form action='/matterc' method='post' onsubmit='return confirm(\"This will cause a restart.\");'>")
    webserver.content_send(string.format("<p></p><button name='%s' class='button bgrn'>", matter_enabled ? "disable" : "enable"))
    webserver.content_send(matter_enabled ? "Disable" : "Enable")
    webserver.content_send(" Matter</button></form></p>")

    webserver.content_send("<p></p></fieldset><p></p>")

    return matter_enabled
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
    import string

    var seconds_left = (self.device.commissioning_open - tasmota.millis()) / 1000
    if seconds_left < 0   seconds_left = 0 end
    var min_left = (seconds_left + 30) / 60

    webserver.content_send(string.format("<fieldset><legend><b>&nbsp;Commissioning open for %i min&nbsp;</b></legend><p></p>", min_left))

    var pairing_code = self.device.compute_manual_pairing_code()
    webserver.content_send(string.format("<p>Manual pairing code:<br><b>%s-%s-%s</b></p><hr>", pairing_code[0..3], pairing_code[4..6], pairing_code[7..]))

    webserver.content_send(string.format("<div><center>"))
    var qr_text = self.device.compute_qrcode_content()
    self.show_qrcode(qr_text)
    webserver.content_send(string.format("<p> %s</p>", qr_text))
    webserver.content_send(string.format("</div>"))

    webserver.content_send("<p></p></fieldset><p></p>")

  end

  #- ---------------------------------------------------------------------- -#
  #- Show Passcode / discriminator form
  #- ---------------------------------------------------------------------- -#
  def show_passcode_form()
    import webserver
    import string

    webserver.content_send("<fieldset><legend><b>&nbsp;Matter Passcode&nbsp;</b></legend><p></p>")
    # button for open/close commissioning
    webserver.content_send("<form action='/matterc' method='post'>")
    if self.device.commissioning_open == nil
      webserver.content_send(string.format("<p></p><button name='open_comm' class='button bgrn'>%s</button>", "Open Commissioning"))
    else
      webserver.content_send(string.format("<p></p><button name='clos_comm' class='button bgrn'>%s</button>", "Close Commissioning"))
    end
    webserver.content_send("</form></p>")
    #
    webserver.content_send("<form action='/matterc' method='post' onsubmit='return confirm(\"This will cause a restart.\");'>")
    webserver.content_send("<p>Passcode:</p>")
    webserver.content_send(string.format("<input type='number' min='1' max='99999998' name='passcode' value='%i'>", self.device.root_passcode))
    webserver.content_send("<p>Distinguish id:</p>")
    webserver.content_send(string.format("<input type='number' min='0' max='4095' name='discriminator' value='%i'>", self.device.root_discriminator))
    webserver.content_send(string.format("<p><input type='checkbox' name='ipv4'%s>IPv4 only</p>", self.device.ipv4only ? " checked" : ""))
    webserver.content_send("<p></p><button name='passcode' class='button bgrn'>Change</button></form></p>")
    webserver.content_send("<p></p></fieldset><p></p>")

  end

  #- ---------------------------------------------------------------------- -#
  #- Show commissioning information and QR Code
  #- ---------------------------------------------------------------------- -#
  def show_fabric_info()
    import webserver
    import string

    webserver.content_send("<fieldset><legend><b>&nbsp;Fabrics&nbsp;</b></legend><p></p>")
    webserver.content_send("<p>Associated fabrics:</p>")

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
        
        webserver.content_send(string.format("<fieldset><legend><b>&nbsp;#%i %s</b> (%s)&nbsp;</legend><p></p>", f.get_fabric_index(), label, f.get_admin_vendor_name()))

        var fabric_rev = f.get_fabric_id().copy().reverse()
        var deviceid_rev = f.get_device_id().copy().reverse()
        webserver.content_send(string.format("Fabric: %s<br>", fabric_rev.tohex()))
        webserver.content_send(string.format("Device: %s<br>&nbsp;", deviceid_rev.tohex()))

        webserver.content_send("<form action='/matterc' method='post' onsubmit='return confirm(\"Are you sure?\");'>")
        webserver.content_send(string.format("<input name='del_fabric' type='hidden' value='%i'>", f.get_fabric_index()))
        webserver.content_send("<button name='del' class='button bgrn'>Delete Fabric</button></form></p>")

        webserver.content_send("<p></p></fieldset><p></p>")
      end
    end

    webserver.content_send("<p></p></fieldset><p></p>")

  end

  #- ---------------------------------------------------------------------- -#
  #- Show plugins configuration
  #- ---------------------------------------------------------------------- -#
  def show_plugins_configuration()
    import webserver
    import string

    webserver.content_send("<fieldset><legend><b>&nbsp;Current Configuration&nbsp;</b></legend><p></p>")

    webserver.content_send("<form action='/matterc' method='post'")
    webserver.content_send("onsubmit='return confirm(\"This will RESET the configuration to the default. You will need to associate again.\");'>")
    webserver.content_send("<button name='auto' class='button bred'>Reset and Auto-discover</button><p></p></form>")

    webserver.content_send("<form action='/matterc' method='post'")
    webserver.content_send("onsubmit='return confirm(\"Changing the configuration requires to associate again.\");'>")
    webserver.content_send("<table style='width:100%'>")
    webserver.content_send("<tr><td width='35'><b>Ep.</b></td><td><b>Type</b></td><td><b>Param</b></td></tr>")

    # display one line per plug-in
    var endpoints = self.device.k2l_num(self.device.plugins_config)
    var i = 0
  
    # special case for root node
    # display a fake configuration item (disabled)
    webserver.content_send("<tr><td><input type='text' name='epnone' maxlength='4' size='3' value='0' readonly disabled></td>")
    webserver.content_send("<td><select name='pinone'>")
    webserver.content_send("<option value='' selected disabled>Root node</option>")
    webserver.content_send("</select></td>")
    webserver.content_send("<td><font size='-1'>&nbsp;</font></td>")

    while i < size(endpoints)
      var ep = endpoints[i]
      if ep == 0  i += 1  continue end          # skip endpoint 0 (leftover from previous versions)
      var conf = self.device.plugins_config[str(ep)]
      var typ = conf.find('type')
      if !typ   i += 1   continue    end

      var cl = self.device.plugins_classes.find(typ)
      var arg = ""
      if cl != nil
        arg = cl.ui_conf_to_string(cl, conf)
      end
      # var arg_name = self.device.get_plugin_class_arg(typ)
      # var arg = arg_name ? str(conf.find(arg_name, '')) : ''

      webserver.content_send(string.format("<tr><td><input type='text' name='ep%03i' maxlength='4' size='3' pattern='[0-9]{1,4}' value='%i'></td>", i, ep))

      webserver.content_send(string.format("<td><select name='pi%03i'>", i))
      self.plugin_option(conf.find('type', ''), self._CLASSES_TYPES, self._CLASSES_TYPES2)
      webserver.content_send(string.format("</select></td>"))
      webserver.content_send(string.format("<td><font size='-1'><input type='text' name='arg%03i' minlength='0' size='8' value='%s'></font></td>",
                             i, webserver.html_escape(arg)))
  
      i += 1
    end

    # add an empty line for adding a configuration
    webserver.content_send(string.format("<tr><td><input type='text' name='ep%03i' maxlength='4' size='3' pattern='[0-9]{1,4}' value=''></td>", i))
    webserver.content_send(string.format("<td><select name='pi%03i'>", i))
    self.plugin_option('', self._CLASSES_TYPES, self._CLASSES_TYPES2)
    webserver.content_send(string.format("</select></td>"))
    webserver.content_send(string.format("<td><font size='-1'><input type='text' name='arg%03i' minlength='0' size='8' value=''></font></td>", i))

    webserver.content_send("</table><p></p>")
    webserver.content_send("<button name='config' class='button bgrn'>Change configuration</button></form>")

    webserver.content_send("<p></p></fieldset><p></p>")

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
      elif typ == '-http'
        webserver.content_send("<option value='' disabled>--- Tasmota Remote ---</option>")
      else
        var nam = self.device.get_plugin_class_displayname(typ)
        webserver.content_send(string.format("<option value='%s'%s>%s</option>", typ, (typ == cur) ? " selected" : "", nam))
      end
      i += 1
    end
  end

  #######################################################################
  # Display the complete page
  #######################################################################
  def page_part_mgr()
    import webserver
    import string

    if !webserver.check_privileged_access() return nil end

    webserver.content_start("Matter")           #- title of the web page -#
    webserver.content_send_style()                  #- send standard Tasmota styles -#

    if self.show_enable()
      self.show_passcode_form()
      self.show_plugins_configuration()
      self.show_fabric_info()
    end
    webserver.content_button(webserver.BUTTON_CONFIGURATION)
    webserver.content_stop()                        #- end of web page -#
  end

  #######################################################################
  # Web Controller, called by POST to `/part_wiz`
  #######################################################################
  def page_part_ctl()
    import webserver
    if !webserver.check_privileged_access() return nil end

    import string
    import partition_core
    import persist

    var error

    try

      #---------------------------------------------------------------------#
      # Change Passcode and/or Passcode
      #---------------------------------------------------------------------#
      if webserver.has_arg("passcode") || webserver.has_arg("discriminator")
        tasmota.log(string.format("MTR: /matterc received '%s' command", 'passcode'), 3)
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

      #---------------------------------------------------------------------#
      # Enable or Disable Commissioning
      #---------------------------------------------------------------------#
      elif webserver.has_arg("open_comm")
        self.device.start_root_basic_commissioning()
        webserver.redirect("/")
      elif webserver.has_arg("clos_comm")
        self.device.stop_basic_commissioning()
        webserver.redirect("/")

      #---------------------------------------------------------------------#
      # Enable Matter
      #---------------------------------------------------------------------#
      elif webserver.has_arg("enable")
        tasmota.log(string.format("MTR: /matterc received '%s' command", 'enable'), 3)
        tasmota.cmd("SetOption" + str(matter.MATTER_OPTION) + " 1")
        #- and force restart -#
        webserver.redirect("/?rst=")

      #---------------------------------------------------------------------#
      # Disable Matter
      #---------------------------------------------------------------------#
      elif webserver.has_arg("disable")
        tasmota.log(string.format("MTR: /matterc received '%s' command", 'disable'), 3)
        tasmota.cmd("SetOption" + str(matter.MATTER_OPTION) + " 0")
        #- and force restart -#
        webserver.redirect("/?rst=")

      #---------------------------------------------------------------------#
      # Delete Fabric
      #---------------------------------------------------------------------#
      elif webserver.has_arg("del_fabric")
        tasmota.log(string.format("MTR: /matterc received '%s' command", 'del_fabric'), 3)
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
        #- reload same page -#
        webserver.redirect("/matterc?")

      #---------------------------------------------------------------------#
      # Reset to default auto-configuration
      #---------------------------------------------------------------------#
      elif webserver.has_arg("auto")
        tasmota.log(string.format("MTR: /matterc received '%s' command", 'auto'), 3)
        self.device.plugins_persist = false
        self.device.save_param()
        #- and force restart -#
        webserver.redirect("/?rst=")

      #---------------------------------------------------------------------#
      # Apply new configuration template
      #---------------------------------------------------------------------#
      elif webserver.has_arg("config")
        var config = {}

        tasmota.log(string.format("MTR: /matterc received '%s' command", 'config'), 3)
        # iterate by id
        var idx = 0
        var idx_str = string.format("%03i", idx)
        while webserver.has_arg('ep'+idx_str)

          var ep = webserver.arg('ep'+idx_str)
          var ep_int = int(ep)
          var typ = webserver.arg('pi'+idx_str)
          var arg = webserver.arg('arg'+idx_str)
          tasmota.log(string.format("MTR: ep=%i type=%s arg=%s", ep, typ, arg), 3)

          if ep != '' && typ != '' && ep != '0'
          
            # check if type exists
            var typ_class = self.device.plugins_classes.find(typ)
            if typ_class != nil
              var elt = {'type':typ}
              typ_class.ui_string_to_conf(typ_class, elt, arg)
              # var arg_name = typ_class.ARG
              # var arg_type = typ_class.ARG_TYPE
              # if arg && arg_name
              #   elt[arg_name] = arg_type(arg)
              # end
              config[ep] = elt

            else
              tasmota.log(string.format("MTR: unknown type = %s", typ), 2)
            end

          else
            tasmota.log("MTR: skipping parameter", 2)
          end

          idx += 1
          idx_str = string.format("%03i", idx)
        end

        tasmota.log(string.format("MTR: config = %s", str(config)), 3)

        if error
          tasmota.log(string.format("MTR: config error = %s", error), 3)
        else
          self.device.plugins_config = config
          self.device.plugins_persist = true
          self.device.save_param()
          #- and force restart -#
          webserver.redirect("/?rst=")
        end

      end

      if error
        webserver.content_start("Parameter error")           #- title of the web page -#
        webserver.content_send_style()                  #- send standard Tasmota styles -#
        webserver.content_send(string.format("<p style='width:340px;'><b>Error:</b>%s</p>", webserver.html_escape(error)))
        webserver.content_button(webserver.BUTTON_CONFIGURATION) #- button back to configuration page -#
        webserver.content_stop()                        #- end of web page -#
      end

    except .. as e, m
      tasmota.log(string.format("BRY: Exception> '%s' - %s", e, m), 2)
      #- display error page -#
      webserver.content_start("Parameter error")           #- title of the web page -#
      webserver.content_send_style()                  #- send standard Tasmota styles -#

      webserver.content_send(string.format("<p style='width:340px;'><b>Exception:</b><br>'%s'<br>%s</p>", e, m))

      webserver.content_button(webserver.BUTTON_CONFIGURATION) #- button back to configuration page -#
      webserver.content_stop()                        #- end of web page -#
    end
  end

  #######################################################################
  # Show bridge status
  #######################################################################
  def show_bridge_status()
    import webserver
    import string
    var bridge_plugin_by_host
    
    var idx = 0
    while idx < size(self.device.plugins)
      var plg = self.device.plugins[idx]

      if isinstance(plg, matter.Plugin_Bridge_HTTP)
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

    for host: bridge_plugin_by_host.keys()
      webserver.content_send(string.format("<tr class='ztdm htrm'><td><b>%s</b></td>", webserver.html_escape(host)))
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
    import string

    var matter_enabled = tasmota.get_option(matter.MATTER_OPTION)

    if matter_enabled

      # mtc0 = close, mtc1 = open commissioning
      var fabrics_count = self.device.sessions.count_active_fabrics()
      if fabrics_count == 0
        webserver.content_send(string.format("<div style='text-align:right;font-size:11px;color:#aaa;padding:0px;'>%s</div>", "Matter: No active association"))
      else
        var plural = fabrics_count > 1
        webserver.content_send(string.format("<div style='text-align:right;font-size:11px;color:#aaa;padding:0px;'>%s</div>", "Matter: " + str(fabrics_count) + " active association" + (plural ? "s" : "")))
      end

      self.show_bridge_status()

      if self.device.is_root_commissioning_open()
        self.show_commissioning_info()
      end

      # display the open/close commissioning button only if there is no active session
      if fabrics_count == 0
        webserver.content_send(string.format("<button onclick='la(\"&mtc%i=1\");'>", self.device.commissioning_open == nil ? 1 : 0))
        webserver.content_send(matter._LOGO)
        if self.device.commissioning_open == nil
          webserver.content_send(" Open Commissioning</button>")
        else
          webserver.content_send(" Close Commissioning</button>")
        end
      end
    end
  end

  def web_get_arg()
    import webserver
    if   webserver.has_arg("mtc0")    # Close Commissioning
      self.device.stop_basic_commissioning()
    elif webserver.has_arg("mtc1")    # Open Commissioning
      self.device.start_root_basic_commissioning()
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
  end
end
matter.UI = Matter_UI
