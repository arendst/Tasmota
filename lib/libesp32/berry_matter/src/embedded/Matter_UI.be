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
  #- Show commissioning information and QR Code
  #- ---------------------------------------------------------------------- -#
  def show_commissioning_info()
    import webserver
    import string

    webserver.content_send("<fieldset><legend><b>&nbsp;Matter Passcode&nbsp;</b></legend><p></p>")

    var pairing_code = self.device.compute_manual_pairing_code()
    webserver.content_send(string.format("<p>Manual pairing code:<br><b>%s-%s-%s</b></p><hr>", pairing_code[0..3], pairing_code[4..6], pairing_code[7..]))
    
    var qr_text = self.device.compute_qrcode_content()
    webserver.content_send('<div id="qrcode"></div>')
    webserver.content_send(string.format('<script type="text/javascript"> new QRCode(document.getElementById("qrcode"), "%s");</script>', qr_text))
    webserver.content_send(string.format("<p>%s</p><hr>", qr_text))

    webserver.content_send("<form action='/matterc' method='post' >")
    webserver.content_send("<p>Passcode:</p>")
    webserver.content_send(string.format("<input type='number' min='1' max='99999998' name='passcode' value='%i'>", self.device.passcode))
    webserver.content_send("<p>Distinguish id:</p>")
    webserver.content_send(string.format("<input type='number' min='0' max='2047' name='discriminator' value='%i'>", self.device.discriminator))
    webserver.content_send("<p></p><button name='passcode' class='button bgrn'>Change</button></form></p>")
    

    webserver.content_send("<p></p></fieldset><p></p>")

  end

  #- ---------------------------------------------------------------------- -#
  #- Show commissioning information and QR Code
  #- ---------------------------------------------------------------------- -#
  def show_session_info(p)
    import webserver
    import string

    webserver.content_send("<fieldset><legend><b>&nbsp;Sessions&nbsp;</b></legend><p></p>")
    webserver.content_send("<p>Existing sessions:</p>")

    if size(self.device.sessions.sessions) == 0
      webserver.content_send("<p><b>None</b></p>")
    else
      var i = 0
      var sz = size(self.device.sessions.sessions)
      while i < sz
        var s = self.device.sessions.sessions[i]
        if s.fabric
          webserver.content_send(string.format("<fieldset><legend><b>&nbsp;Session %i&nbsp;</b></legend><p></p>", s.local_session_id))
          if i != 0     webserver.content_send("<hr>") end
          var fabric_rev = s.fabric.copy().reverse()
          var deviceid_rev = s.deviceid.copy().reverse()
          webserver.content_send(string.format("Fabric: %s<br>", fabric_rev.tohex()))
          webserver.content_send(string.format("Device: %s<br>&nbsp;", deviceid_rev.tohex()))

          webserver.content_send("<form action='/matterc' method='post' ")
          webserver.content_send("onsubmit='return confirm(\"This will cause a restart.\");'>")
          webserver.content_send(string.format("<input name='del_session' type='hidden' value='%d'>", s.local_session_id))
          webserver.content_send("<button name='del' class='button bgrn'>Delete Session</button></form></p>")
      
          webserver.content_send("<p></p></fieldset><p></p>")
        end
        i += 1
      end
    end

    webserver.content_send("<p></p></fieldset><p></p>")
    
  end


  #######################################################################
  # Serve qrcode.min.js static file
  #######################################################################
  def page_qrcode_min_js()
    import webserver
    
    webserver.content_open(200, "text/javascript")
    webserver.content_send(matter._QRCODE_MINJS)
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

    webserver.content_send('<script type="text/javascript" src="qrcode.min.js"></script>')

    if self.show_enable()
      self.show_commissioning_info()
      self.show_session_info()
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
          self.device.passcode = int(webserver.arg("passcode"))
        end
        if webserver.has_arg("discriminator")
          self.device.discriminator = int(webserver.arg("discriminator"))
        end
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

      elif webserver.has_arg("del_session")
        var session = self.device.sessions.get_session_by_local_session_id(int(webserver.arg("del_session")))
        if session != nil
          self.device.sessions.remove_session(session)
          self.device.sessions.save()
        end

        #- and force restart -#
        webserver.redirect("/?rst=")

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

  #- ---------------------------------------------------------------------- -#
  # respond to web_add_handler() event to register web listeners
  #- ---------------------------------------------------------------------- -#
  #- this is called at Tasmota start-up, as soon as Wifi/Eth is up and web server running -#
  def web_add_handler()
    import webserver
    #- we need to register a closure, not just a function, that captures the current instance -#
    webserver.on("/matterc", / -> self.page_part_mgr(), webserver.HTTP_GET)
    webserver.on("/matterc", / -> self.page_part_ctl(), webserver.HTTP_POST)
    webserver.on("/qrcode.min.js", / -> self.page_qrcode_min_js(), webserver.HTTP_GET)
  end
end
matter.UI = Matter_UI
