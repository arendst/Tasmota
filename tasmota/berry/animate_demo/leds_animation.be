#################################################################################
# Web UI for Leds animation
#
#################################################################################

var leds_animation = module('leds_animation')

#################################################################################
# Leds_animation_UI
#
# WebUI for the Leds animation
#################################################################################
class Leds_animation_UI

  def init()
    tasmota.add_driver(self)
  end

  # create a method for adding a button to the main menu
  # the button 'Leds animation' redirects to '/leds_anim?'
  def web_add_button()
    import webserver
    webserver.content_send(
      "<p></p><form id=but_part_mgr style='display: block;' action='leds_anim' method='get'><button>Leds animation</button></form>")
  end

  #######################################################################
  # Show background colors
  #######################################################################
  def show_background_color()
    import webserver
    var back_color = 0xFF8800
    var back_anim_time = 5.0

      
    webserver.content_send("<fieldset><legend><b>&nbsp;Background &nbsp;</b></legend>"
                            "<form action='/leds_anim' method='post'>")

    # webserver.content_send(f"<input type='color' min='1' max='256' name='leds_back' value='#{back_color:06X}'>")
    # webserver.content_send("<hr>")
    webserver.content_send("<label>Animation time (sec)</label>")
    webserver.content_send(f"<input type='number' min='1' max='256' name='leds_size' value='{back_anim_time:.1f}'>")
    webserver.content_send("<p></p>")

    webserver.content_send("<label>Background color (static)</label>")
    webserver.content_send("<table style='width:100%;background:white;'>")
    webserver.content_send(f"<tr>"
                           "<td width='33%' style='color:#222;'><input type='radio' name='static' checked />Static</td>"
                           "<td width='67%'><input type='color' min='1' max='256' name='leds_back' value='#{back_color:06X}'></td>"
                           "</tr>")
    webserver.content_send("<tr>"
                           "<td width='33%' style='color:#222;'><input type='radio' name='static' />Palette1</td>"
                           "<td width='67%' style='background:linear-gradient(to right,#FF0000 0.0%,#FFA500 14.3%,#FFFF00 28.6%,#00EE00 42.9%,#0000FF 57.1%,#4B00FF 71.4%,#FF82FF 85.7%,#FF0000 100.0%);'></td>"
                           "</tr>")
    webserver.content_send("<tr>"
                          "<td width='33%' style='color:#222;'><input type='radio' name='static' />Palette1</td>"
                          "<td width='67%' style='background:linear-gradient(to right,#FF0000 0.0%,#FF0000 8.9%,#FFA500 14.3%,#FFA500 23.2%,#FFFF00 28.6%,#FFFF00 37.5%,#00FF00 42.9%,#00FF00 51.8%,#0000FF 57.1%,#0000FF 66.1%,#FF00FF 71.4%,#FF00FF 80.4%,#FFFFFF 85.7%,#FFFFFF 94.6%,#FF0000 100.0%);'></td>"
                          "</tr>")
    webserver.content_send("<tr>"
                          "<td width='33%' style='color:#222;'><input type='radio' name='static' />Palette1</td>"
                          "<td width='67%' style='background:linear-gradient(to right,#E60611 0.0%,#B66022 36.9%,#A0402A 52.2%,#88030D 100.0%);'></td>"
                          "</tr>")
    webserver.content_send("</table>")
    webserver.content_send("<p></p><button name='save' class='button bgrn'>Save</button></form></p>"
                            "</fieldset><p></p>")
  end

  #######################################################################
  # Show main config
  #######################################################################
  def show_main_config()
    import webserver
    var leds_anim_enabled = true
    var leds_size = 25
    
    webserver.content_send("<fieldset><legend><b>&nbsp;Leds configuration &nbsp;</b></legend>"
                           "<p style='width:320px;'>Check the <a href='https://tasmota.github.io/docs/Leds_animation/' target='_blank'>Leds animation documentation</a>.</p>"
                           "<form action='/leds_anim' method='post'>")

    # checkbox for Matter enable
    var leds_anim_enabled_checked = leds_anim_enabled ? 'checked' : ''
    webserver.content_send(f"<p><input id='menable' type='checkbox' name='menable' {leds_anim_enabled_checked}>")
    webserver.content_send("<label for='menable'><b>Leds anim enabled</b></label></p>")

    webserver.content_send("<label>Number of leds</label>")
    webserver.content_send(f"<input type='number' min='1' max='256' name='leds_size' value='{leds_size:i}'>")

    webserver.content_send("<p></p><button name='save' class='button bgrn'>Save</button></form></p>"
                           "</fieldset><p></p>")
  end

  #######################################################################
  # Display the complete page
  #######################################################################
  def page_view()
    import webserver
    if !webserver.check_privileged_access() return nil end

    webserver.content_start("Leds animation")         #- title of the web page -#
    webserver.content_send_style()                    #- send standard Tasmota styles -#

    self.show_main_config()
    self.show_background_color()
    webserver.content_button(webserver.BUTTON_MANAGEMENT) #- button back to management page -#

    webserver.content_stop()                        #- end of web page -#
  end

  #- ---------------------------------------------------------------------- -#
  # respond to web_add_handler() event to register web listeners
  #- ---------------------------------------------------------------------- -#
  #- this is called at Tasmota start-up, as soon as Wifi/Eth is up and web server running -#
  def web_add_handler()
    import webserver
    #- we need to register a closure, not just a function, that captures the current instance -#
    webserver.on("/leds_anim", / -> self.page_view(), webserver.HTTP_GET)
    webserver.on("/leds_anim", / -> self.page_ctl(), webserver.HTTP_POST)
  end
end
leds_animation.Leds_animation_UI = Leds_animation_UI


#- create and register driver in Tasmota -#
if tasmota
  var ui = leds_animation.Leds_animation_UI()
  ## can be removed if put in 'autoexec.bat'
  ui.web_add_handler()
end

return leds_animation

#- Example

import leds_animation

-#
