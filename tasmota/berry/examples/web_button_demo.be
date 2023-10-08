#-----------------------------------------------------------------------
- Demo of custom web button
-
- Creates a button on the main page that increments a counter each
- time the button is pressed, and display the Counter value like
- a sensor (not included in teleperiod, or implement also 'json_append()')
-
- Copy the following in `autoexec.be` in Tasmota file system
-----------------------------------------------------------------------#

import webserver
class Button_counter_demo : Driver
  var counter

  def init()
    self.counter = 0    #- initialize the counter -#
  end

  #- this method receives events from buttons and displays values on main page -#
  def web_sensor()
    #- is 'incr_counter' included in the request -#
    if webserver.has_arg("incr_counter")
      var incr_counter = int(webserver.arg("incr_counter"))
      self.counter += incr_counter
    end

    #- display the counter in its own line -#
    webserver.content_send(format("{s}Counter{m}%i{e}", self.counter))
  end

  #- display button for Increase Counter and trigger 'incr_counter=1' when pressed -#
  def web_add_main_button()
    webserver.content_send("<p></p><button onclick='la(\"&incr_counter=1\");'>Increment Counter</button>")
  end
end

#- create and register driver in Tasmota -#
counter_demo_instance = Button_counter_demo()
tasmota.add_driver(counter_demo_instance)