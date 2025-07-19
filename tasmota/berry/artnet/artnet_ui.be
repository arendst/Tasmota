#######################################################################
# DMX ArtNet UI for ESP32(C3/S2/S3)
#
#######################################################################

var artnet_ui = module('artnet_ui')

#################################################################################
# ArtNet_UI
#
# WebUI
#################################################################################
class ArtNet_UI

  def init()
    import persist

    if persist.find("artnet_autorun") == true
      # autorun
    end
  end

  # ####################################################################################################
  # Init web handlers
  # ####################################################################################################
  # Displays a "DMX ArtNet" button on the configuration page
  def web_add_config_button()
    import webserver
    webserver.content_send("<p><form id=artnet_ui action='artnet_ui' style='display: block;' method='get'><button>Configure ArtNet animations</button></form></p>")
  end

  # ####################################################################################################
  # Get WS2812 gpios
  #
  # Returns an array of valid WS2812 gpios as defined in the template, or empty array
  # ####################################################################################################
  def get_ws2812_gpios()
    import gpio
    var ret = []
    for p:0..31
      if gpio.pin_used(gpio.WS2812, p)
        ret.push(p)
      end
    end
    return ret
  end

  static def read_persist()
    import persist
    var conf = dyn()

    conf.gpio = persist.find("artnet_gpio", 0)      # gpio number from template
    conf.rows = persist.find("artnet_rows", 5)      # number of rows (min: 1)
    conf.cols = persist.find("artnet_cols", 5)      # number of columns (min: 1)
    conf.offs = persist.find("artnet_offs", 0)      # offset in the led strip where the matrix starts (min: 0)
    conf.alt  = persist.find("artnet_alt", false)   # are the rows in alternate directions

    conf.univ = persist.find("artnet_univ", 0)      # start universe

    # conf.addr = persist.find("artnet_addr", "uni")  # listening mode, either 'uni' or 'multi' for multicast
    conf.port = persist.find("artnet_port", 6454)   # UDP port number

    conf.auto = persist.find("artnet_auto", true)  # autorun at startup
    return conf
  end

  def save_persist(conf)
    import persist
    persist.artnet_gpio = conf.gpio
    persist.artnet_rows = conf.rows
    persist.artnet_cols = conf.cols
    persist.artnet_offs = conf.offs
    persist.artnet_alt = conf.alt

    persist.artnet_univ = conf.univ

    # persist.artnet_addr = conf.addr
    persist.artnet_port = conf.port
    
    persist.artnet_auto = conf.auto

    persist.save()
  end

  #######################################################################
  # Display the complete page on `/artnet_ui`
  #######################################################################
  def page_artnet_ui()
    import webserver
    if !webserver.check_privileged_access() return nil end

    # read configuration
    var conf = self.read_persist()

    webserver.content_start("ArtNet")           #- title of the web page -#
    webserver.content_send_style()                  #- send standard Tasmota styles -#

    # webserver.content_send("<p style='width:320px;'><b style='color:#f56'>Warning:</b> actions below can brick your device.</p>")
    # webserver.content_send("<p><small>&nbsp;(This feature requires an internet connection)</small></p>")
    
    webserver.content_send("<fieldset><style>.bdis{background:#888;}.bdis:hover{background:#888;}</style>")
    webserver.content_send(format("<legend><b title='ArtNet'>&nbsp;ArtNet configuration</b></legend>"))

    webserver.content_send("<p><form id=artnet_ui style='display: block;' action='/artnet_ui' method='post'>")

    # WS2812 bus configuration
    webserver.content_send(format("<p>WS2812 configuration: </p>"))
    webserver.content_send(format(
      "<table style='width:100%%'>"
      "<tr><td style='width:150px'><b>GPIO</b></td><td style='width:150px'>"))

    var ws2812_list = self.get_ws2812_gpios()
    var ws2812_gpio
    if size(ws2812_list) == 0
      webserver.content_send("<b>**Not configured**</b>")
    else
      webserver.content_send("<select id='ws2812'>")
      for gp:ws2812_list
        webserver.content_send(format("<option value='%i'>%s</option>", gp, "WS2812 - " + str(gp+1)))
      end
      webserver.content_send("</select>")
    end
    webserver.content_send("</td><td></td></tr>")

    webserver.content_send(format("<tr><td><b>Rows</b></td><td>"))
    webserver.content_send(format("<input type='number' min='1' name='rows' value='%i'>", conf.rows))
    webserver.content_send("</td></tr>")
    webserver.content_send(format("<tr><td><b>Columns</b></td><td>"))
    webserver.content_send(format("<input type='number' min='1' name='cols' value='%i'>", conf.cols))
    webserver.content_send("</td></tr>")
    webserver.content_send(format("<tr><td><b>Offset</b></td><td>"))
    webserver.content_send(format("<input type='number' min='0' name='offs' value='%i'>", conf.offs))
    webserver.content_send("</td></tr>")

    webserver.content_send(format("<tr><td><b>Alternate rows</b></td><td>"))
    webserver.content_send(format("<input type='checkbox' name='alt'%s></p>", conf.alt ? " checked" : ""))
    webserver.content_send("</td></tr>")

    webserver.content_send("<tr><td>&nbsp;</td></tr>")
    webserver.content_send(format("<tr><td><b>DMX universe</b></td><td>"))
    webserver.content_send(format("<input type='number' min='0' name='univ' value='%i'>", conf.univ))
    webserver.content_send("</td></tr>")
    # description
    webserver.content_send("<tr><td colspan='3' style='word-wrap: break-word;'>")
    webserver.content_send("This this the universe number for<br>the first row, and gets incremented<br>for each row.")
    webserver.content_send("</td></tr>")

    webserver.content_send("</table><hr>")

    # IP configuration
    webserver.content_send(format("<p>IP listener: </p>"))
    webserver.content_send("<table style='width:100%%'>")
      # "<tr><td style='width:120px'><b>IP mode</b></td><td style='width:180px'>"))
    # webserver.content_send("<select id='ip'>")
    # webserver.content_send(format("<option value='uni'%s>unicast</option>", conf.addr == 'uni' ? " selected" : ""))
    # webserver.content_send(format("<option value='multi'%s>multicast</option>", conf.addr == 'multi' ? " selected" : ""))
    # webserver.content_send("</select>")
    # webserver.content_send("</td><td></td></tr>")

    webserver.content_send("<tr><td style='width:120px'><b>Port</b></td><td style='width:180px'>")
    # webserver.content_send(format("<tr><td><b>Port</b></td><td>"))
    webserver.content_send(format("<input type='number' min='1' name='port' value='%i'>", conf.port))
    webserver.content_send("</td></tr>")
    webserver.content_send("</table><hr>")

    # auto-run
    webserver.content_send(format("<p><b>Auto-run at boot:</b> <input type='checkbox' name='auto'%s></p>", conf.auto ? " checked" : ""))

    # button
    webserver.content_send("<button name='artnetapply' class='button bgrn'>Apply and Run</button>")
    webserver.content_send("</form></p>")

    webserver.content_send("<p></p></fieldset><p></p>")
    webserver.content_button(webserver.BUTTON_CONFIGURATION)
    webserver.content_stop()
  end

  #######################################################################
  # Web Controller, called by POST to `/artnet_ui`
  #######################################################################
  def page_artnet_ctl()
    import webserver
    if !webserver.check_privileged_access() return nil end

    import persist
    import introspect
    
    try
      if webserver.has_arg("artnetapply")

        # read argumments, sanity check and put in conf object
        var conf = dyn()

        # read gpio
        var ws2812_list = self.get_ws2812_gpios()
        var gp_ws2812 = int(webserver.arg("ws2812"))
        if ws2812_list.find(gp_ws2812) != nil
          conf.gpio = gp_ws2812
        else
          conf.gpio = -1
        end
        
        # read rows and cols
        var rows = int(webserver.arg("rows"))
        if rows < 1 || rows > 999    rows = 1 end
        conf.rows = rows
        var cols = int(webserver.arg("cols"))
        if cols < 1 || cols > 999    cols = 1 end
        conf.cols = cols
        # alternate
        conf.alt = webserver.arg("alt") == 'on'

        # offset
        var offs = int(webserver.arg("offs"))
        if offs < 0 || offs > 999    offs = 0 end
        conf.offs = offs

        # universe
        var univ = int(webserver.arg("univ"))
        if univ < 0 || univ > 999    univ = 0 end
        conf.univ = univ

        # universe
        var port = int(webserver.arg("port"))
        if port < 1 || port > 65535    port = 6454 end
        conf.port = port

        # autorun
        conf.auto = webserver.arg("auto") == 'on'

        self.save_persist(conf)

        artnet.stop_global()
        artnet.run_from_conf()

        # tasmota.log("BRY: conf=" + str(conf), 2);
        webserver.redirect("/cn?")
      else
        raise "value_error", "Unknown command"
      end
    except .. as e, m
      print(format("BRY: Exception> '%s' - %s", e, m))
      #- display error page -#
      webserver.content_start("Parameter error")           #- title of the web page -#
      webserver.content_send_style()                  #- send standard Tasmota styles -#

      webserver.content_send(format("<p style='width:340px;'><b>Exception:</b><br>'%s'<br>%s</p>", e, m))

      webserver.content_button(webserver.BUTTON_CONFIGURATION) #- button back to management page -#
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
    webserver.on("/artnet_ui", / -> self.page_artnet_ui(), webserver.HTTP_GET)
    webserver.on("/artnet_ui", / -> self.page_artnet_ctl(), webserver.HTTP_POST)
  end
end
artnet_ui.ArtNet_UI = ArtNet_UI


#- create and register driver in Tasmota -#
if tasmota
  var artnet_ui_instance = artnet_ui.ArtNet_UI()
  tasmota.add_driver(artnet_ui_instance)
  ## can be removed if put in 'autoexec.bat'
  artnet_ui_instance.web_add_handler()
end

return artnet_ui

#- Example

import partition

# read
p = partition.Partition()
print(p)

-#
