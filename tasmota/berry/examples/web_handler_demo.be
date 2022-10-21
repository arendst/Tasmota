#-----------------------------------------------------------------------
- Demo of custom web handler
-
- Registers a custom web hanlder for URI prefix `/hello`
- and displays a Tasmota styled web page
-
- Copy the following in `autoexec.be` in Tasmota file system
-----------------------------------------------------------------------#

import webserver
class Web_page_demo : Driver

  #- this method displays the web page -#
  def page_say_hello()
    if !webserver.check_privileged_access() return nil end

    webserver.content_start("Hello page")           #- title of the web page -#
    webserver.content_send_style()                  #- send standard Tasmota styles -#
    webserver.content_send("Tasmota says hello!")   #- send any html -#
    webserver.content_button(webserver.BUTTON_MAIN) #- button back to main page -#
    webserver.content_stop()                        #- end of web page -#
  end

  #- this is called at Tasmota start-up, as soon as Wifi/Eth is up and web server running -#
  def web_add_handler()
    #- we need to register a closure, not just a function, that captures the current instance -#
    webserver.on("/hello", / -> self.page_say_hello())
  end
end

#- create and register driver in Tasmota -#
web_page_demo_instance = Web_page_demo()
tasmota.add_driver(web_page_demo_instance)

#- For debugging purposes, you can manually call the following to register the web handler -#
#- as it is automatically called only if the instance was registered at startup, for example
#- in `autoexec.be` -#
#-

web_page_demo_instance.web_add_handler()

-#