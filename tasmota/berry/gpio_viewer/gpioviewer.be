#
# webserber_gpioviewer.be - implements a generic async non-blocking HTTP server
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

var gpio_viewer = module('gpio_viewer')

class GPIO_viewer
  var port
  var web
  var sampling_interval
  var pin_actual       # actual value
  var last_pin_states       # state converted to 0..255
  var new_pin_states        # get a snapshot of newest values
  var pin_types             # array of types
  var payload1, payload2               # temporary object bytes() to avoid reallocation

  static var TYPE_DIGITAL = 0
  static var TYPE_PWM = 1
  static var TYPE_ANALOG = 2

  static var SAMPLING = 100
  static var GPIO_RELEASE = "1.0.7"
  static var HTML_HEAD = 
    "<!DOCTYPE HTML><html><head><title>ESP32 GPIO State</title>"
    "<base href ='https://thelastoutpostworkshop.github.io/microcontroller_devkit/gpio_viewer/assets/'>"
    "<link id='defaultStyleSheet' rel='stylesheet' href=''>"
    "<link id='boardStyleSheet' rel='stylesheet' href=''>"
    "<link rel='icon' href='favicon.ico' type='image/x-icon'>"

    "<script src='script/webSocket.js'></script>"
    "<script src='script/boardSwitcher.js'></script>"
    "</head>"
  static var HTML_BODY =
    "<body><div class='grid-container'>\n"
    "<header class='header'>"
    "</header>"
    # Image
    "<div class='image-container'>\n"
    "<div id='imageWrapper' class='image-wrapper'>"
    "<img id='boardImage' src='' alt='Board Image'>\n"
    "<div id='indicators'></div>"
    "</div></div></div>"
  static var HTML_SCRIPT =
    # Append the script variables
    "<script>var serverPort = %i;</script>"
    "<script>var source = new EventSource('http://%s:%i/events');</script>"
    "<script>var ip = '%s';</script>"
    "<script>var sampling_interval = '%i';</script>"
    "<script>var freeSketchSpace = '%i';</script>"
    "</body></html>"
  
  def init(port)
    self.port = port
    self.web = webserver_async(5555)
    self.sampling_interval = self.SAMPLING
    self.payload1 = bytes(100)              # reserve 100 bytes by default
    self.payload2 = bytes(100)              # reserve 100 bytes by default

    # pins
    import gpio
    self.pin_actual = []
    self.pin_actual.resize(gpio.MAX_GPIO)    # full of nil
    self.last_pin_states = []
    self.last_pin_states.resize(gpio.MAX_GPIO)    # full of nil
    self.new_pin_states = []
    self.new_pin_states.resize(gpio.MAX_GPIO)     # full of nil
    self.pin_types = []
    self.pin_types.resize(gpio.MAX_GPIO)     # full of nil

    self.web.set_chunked(true)
    self.web.set_cors(true)
    self.web.on("/release", self, self.send_release_page)
    self.web.on("/events", self, self.send_events_page)
    self.web.on("/", self, self.send_index_page)

    tasmota.add_driver(self)
  end

  def close()
    tasmota.remove_driver(self)
    self.web.close()
  end

  def send_index_page(cnx, uri, verb)
    import string

    cnx.send(200, "text/html")
    cnx.write(self.HTML_HEAD)
    cnx.write(self.HTML_BODY)

    var host = cnx.header_host
    var host_split = string.split(host, ':')      # need to make it stronger
    var ip = host_split[0]
    var port = 80
    if size(host_split) > 1
      port = int(host_split[1])
    end

    var free_space = f"{tasmota.memory().find('program_free', 0)} KB"
    var html = format(self.HTML_SCRIPT, port, ip, port, ip, self.sampling_interval, free_space)
    cnx.write(html)
    cnx.content_stop()
  end
  
  def send_release_page(cnx, uri, verb)
    var release = f'{{"release":"{self.GPIO_RELEASE}"}}'
    cnx.send(200, "application/json", release)
    cnx.content_stop()
  end

  def send_events_page(cnx, uri, verb)
    cnx.set_chunked(false)     # no chunking since we use EventSource
    cnx.set_cors(true)
    cnx.send(200, "text/event-stream")

    self.send_events_tick(cnx)
  end

  def send_events_tick(cnx)
    import gpio
    var max_gpio = gpio.MAX_GPIO
    var payload1 = self.payload1
    payload1.clear()
    payload1 .. '{'
    var dirty = false
    var pin = 0
    self.read_states()

    while pin < max_gpio
      var prev = self.last_pin_states[pin]
      var val = self.new_pin_states[pin]
      if (prev != val) || (val != nil)      # TODO for now send everything every time
        if dirty
          # msg += ","
          payload1 .. ","
        end
        payload1 .. '"'
        payload1 .. str(pin)
        payload1 .. '":{"s":'
        payload1 .. str(val)
        payload1 .. ',"v":'
        payload1 .. str(self.pin_actual[pin])
        payload1 .. ',"t":'
        payload1 .. str(self.pin_types[pin])
        payload1 .. '}'
        # msg += f'"{pin}":{{"s":{val},"v":{prev},"t":{self.pin_types[pin]}}}}'
        dirty = true

        self.last_pin_states[pin] = val
      end
      pin += 1
    end
    payload1 .. '}'
    # msg += "}"

    if dirty
      # prepare payload
      var payload2 = self.payload2
      payload2.clear()
      payload2 .. 'id:'
      payload2 .. str(tasmota.millis())
      payload2 .. "\r\nevent:gpio-state\r\ndata:"
      payload2 .. payload1
      payload2 .. "\r\n\r\n"
      # var payload = f"id:{tasmota.millis()}\r\n"
      #             "event:gpio-state\r\n"
      #             "data:{msg}\r\n\r\n"

      # tasmota.log(f"GPV: sending '{msg}'", 3)
      cnx.write(payload2)
    end

    # send free heap
    var payload = f"id:{tasmota.millis()}\r\n"
                   "event:free_heap\r\n"
                   "data:{tasmota.memory().find('heap_free', 0)} KB\r\n\r\n"
    cnx.write(payload)

    tasmota.set_timer(self.sampling_interval, def () self.send_events_tick(cnx) end)
  end

  # read all GPIO values, store in `pin_actual` and `new_pin_states`
  def read_states()
    import gpio
    var max_gpio = gpio.MAX_GPIO
    var pin = 0
    while pin < max_gpio
      # check if PWM
      var pwm_resolution = gpio.read_pwm_resolution(pin)
      if (pwm_resolution > 0)
        var pwm_val = gpio.read_pwm(pin)
        var pwm_state = tasmota.scale_uint(pwm_val, 0, pwm_resolution, 0, 255)    # bring back to 0..255
        self.pin_actual[pin] = pwm_val
        self.new_pin_states[pin] = pwm_state
        self.pin_types[pin] = self.TYPE_PWM
      elif gpio.get_pin_type(pin) > 0
        # digital read
        var digital_val = gpio.digital_read(pin)     # returns 0 or 1
        self.pin_actual[pin] = digital_val
        self.new_pin_states[pin] = digital_val ? 256 : 0
        self.pin_types[pin] = self.TYPE_DIGITAL
      else
        self.pin_actual[pin] = nil
        self.new_pin_states[pin] = nil
        self.pin_types[pin] = self.TYPE_DIGITAL
      end
      pin += 1
    end
  end

  # Add button 'GPIO Viewer' redirects to '/part_wiz?'
  def web_add_button()
    import webserver
    var ip = tasmota.wifi().find('ip')
    if (ip == nil)
      ip = tasmota.eth().find('ip')
    end
    if (ip != nil)
      webserver.content_send(
        f"<form style='display: block;' action='http://{ip}:{self.port}/' method='post' target='_blank'><button>GPIO Viewer</button></form><p></p>")
    end
  end

end

gpio_viewer.GPIO_viewer = GPIO_viewer

if tasmota
  global.gpioviewer = GPIO_viewer(5555)
end

return global.gpioviewer

#- Test

var gpio_viewer = GPIO_viewer(5555)

-#
