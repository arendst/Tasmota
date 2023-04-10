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

    webserver.content_send("<p style='width:320px;'>Matter support is experimental.</p>")

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
    webserver.content_send("<form action='/matterc' method='post' >")
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
  def show_fabric_info(p)
    import webserver
    import string

    webserver.content_send("<fieldset><legend><b>&nbsp;Fabrics&nbsp;</b></legend><p></p>")
    webserver.content_send("<p>Existing fabrics:</p>")

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
        
        webserver.content_send(string.format("<fieldset><legend><b>&nbsp;#%i %s&nbsp;</b></legend><p></p>", f.get_fabric_index(), label))

        var fabric_rev = f.get_fabric_id().copy().reverse()
        var deviceid_rev = f.get_device_id().copy().reverse()
        webserver.content_send(string.format("Fabric: %s<br>", fabric_rev.tohex()))
        webserver.content_send(string.format("Device: %s<br>&nbsp;", deviceid_rev.tohex()))

        webserver.content_send("<form action='/matterc' method='post'>")
        webserver.content_send(string.format("<input name='del_fabric' type='hidden' value='%i'>", f.get_fabric_index()))
        webserver.content_send("<button name='del' class='button bgrn'>Delete Fabric</button></form></p>")

        webserver.content_send("<p></p></fieldset><p></p>")
      end
    end

    webserver.content_send("<p></p></fieldset><p></p>")

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


    #- check that the partition is valid -#
    var p = partition_core.Partition()

    try

      #---------------------------------------------------------------------#
      # Change Passcode and/or Passcode
      #---------------------------------------------------------------------#
      if webserver.has_arg("passcode") || webserver.has_arg("discriminator")
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

      elif webserver.has_arg("enable")
        tasmota.cmd("SetOption" + str(matter.MATTER_OPTION) + " 1")
        #- and force restart -#
        webserver.redirect("/?rst=")

      elif webserver.has_arg("disable")
        tasmota.cmd("SetOption" + str(matter.MATTER_OPTION) + " 0")
        #- and force restart -#
        webserver.redirect("/?rst=")

      elif webserver.has_arg("del_fabric")
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

        #- and force restart -#
        webserver.redirect("/matterc?")

      end
    except .. as e, m
      tasmota.log(string.format("BRY: Exception> '%s' - %s", e, m), 2)
      #- display error page -#
      webserver.content_start("Parameter error")           #- title of the web page -#
      webserver.content_send_style()                  #- send standard Tasmota styles -#

      webserver.content_send(string.format("<p style='width:340px;'><b>Exception:</b><br>'%s'<br>%s</p>", e, m))

      webserver.content_button(webserver.BUTTON_MANAGEMENT) #- button back to management page -#
      webserver.content_stop()                        #- end of web page -#
    end
  end

  #- display sensor value in the web UI -#
  def web_sensor()
    import webserver
    import string

    var matter_enabled = tasmota.get_option(matter.MATTER_OPTION)

    if matter_enabled
      if self.device.is_root_commissioning_open()
        self.show_commissioning_info()
      end

      # mtc0 = close, mtc1 = open commissioning
      var fabrics_count = self.device.sessions.count_active_fabrics()
      if fabrics_count == 0
        webserver.content_send(string.format("<div style='text-align:right;font-size:11px;color:#aaa;'>%s</div>", "No active association"))
      else
        var plural = fabrics_count > 1
        webserver.content_send(string.format("<div style='text-align:right;font-size:11px;color:#aaa;'>%s</div>", str(fabrics_count) + " active association" + (plural ? "s" : "")))
      end

      webserver.content_send(string.format("<button onclick='la(\"&mtc%i=1\");'>", self.device.commissioning_open == nil ? 1 : 0))
      webserver.content_send(matter._LOGO)
      if self.device.commissioning_open == nil
        webserver.content_send(" Open Commissioning</button>")
      else
        webserver.content_send(" Close Commissioning</button>")
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
