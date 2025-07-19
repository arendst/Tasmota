#
# debug_panel.be - implements a small panel on top of the Tasmota UI to view real-time information
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

class debug_panel
  var port
  var web
  var sampling_interval
  var p1

  static var SAMPLING = 100
  static var HTML_HEAD1 = 
    "<!DOCTYPE HTML><html><head>"
  static var HTML_URL_F = 
    "<script>"
    "var event_url='http://%s:%i/info_feed';"
    "</script>"
  static var HTML_HEAD2 = 
    "<script>"
    "var source = new EventSource(event_url);"

    'function initEventSource() {'
      'source.addEventListener('
        '"free_heap",'
        'function (e) {'
          'var freeHeap = document.getElementById("freeheap");'
          'if (freeHeap) {'
            'freeHeap.innerHTML = e.data;'
          '}'
        '},'
        'false'
      ');'
      'source.addEventListener('
        '"wifi_rssi",'
        'function (e) {'
          'var wifirssi = document.getElementById("wifirssi");'
          'if (wifirssi) {'
            'wifirssi.innerHTML = e.data;'
          '}'
        '},'
        'false'
      ');'
    '};'
    'window.addEventListener("load", initEventSource);'
    "</script>"
    "</head>"
    "<body><style> body{ font-family: verdana,sans-serif; color: #aaa; font-size: 11px; margin:0px;}</style>"
  static var HTML_CONTENT =
    '<table style="width:100%;" border="0";margin:0px;><tr>'
    '<td width="110">Free Heap <span id="freeheap">--- KB</span></td>'
    '<td width="90">Wifi RSSI <span id="wifirssi">--%</span></td><td></td>'
    '</tr></table>'
  static var HTML_END =
    "</body></html>"
  
  def init(port)
    if (port == nil)  port = 8887   end
    self.port = port
    self.web = webserver_async(port)
    self.sampling_interval = self.SAMPLING

    self.p1 = bytes(100)

    self.web.set_chunked(true)
    self.web.set_cors(true)
    self.web.on("/info_feed", self, self.send_info_feed)
    self.web.on("/info", self, self.send_info_page)

    tasmota.add_driver(self)
  end

  def close()
    tasmota.remove_driver(self)
    self.web.close()
  end

  def send_info_page(cnx, uri, verb)
    import string
    
    var host = cnx.header_host
    var host_split = string.split(host, ':')      # need to make it stronger
    var ip = host_split[0]
    var port = 80
    if size(host_split) > 1
      port = int(host_split[1])
    end

    cnx.send(200, "text/html")
    cnx.write(self.HTML_HEAD1)
    cnx.write(format(self.HTML_URL_F, ip, port))
    cnx.write(self.HTML_HEAD2)
    cnx.write(self.HTML_CONTENT)
    cnx.write(self.HTML_END)

    cnx.content_stop()
  end
  
  static class feeder
    var app                                   # overarching app (debug_panel)
    var cnx                                   # connection object

    def init(app, cnx)
      self.app = app
      self.cnx = cnx
      tasmota.add_driver(self)
    end

    def close()
      tasmota.remove_driver(self)
    end

    def every_100ms()
      self.send_feed()
    end

    def send_feed()
      var cnx = self.cnx
      if !cnx.connected()
        self.close()
        return
      end

      var payload1 = self.app.p1
      var server = self.cnx.server
      if cnx.buf_out_empty()
        # if out buffer is not empty, do not send any new information
        # var payload
        # send free heap
        payload1.clear()
        payload1 .. "id:"
        server.bytes_append_int(payload1, tasmota.millis())
        payload1 .. "\r\nevent:free_heap\r\ndata:"
        server.bytes_append_int(payload1, tasmota.memory('heap_free'), '---')
        payload1 .. " KB\r\n\r\n"
        # payload = f"id:{tasmota.millis()}\r\n"
        #           "event:free_heap\r\n"
        #           "data:{tasmota.memory().find('heap_free', 0)} KB\r\n\r\n"
        cnx.write(payload1)

        # send wifi rssi
        payload1.clear()
        payload1 .. "id:"
        server.bytes_append_int(payload1, tasmota.millis())
        payload1 .. "\r\nevent:wifi_rssi\r\ndata:"
        server.bytes_append_int(payload1, tasmota.wifi('quality'), '--')
        payload1 .. "%\r\n\r\n"

        # payload = f"id:{tasmota.millis()}\r\n"
        #           "event:wifi_rssi\r\n"
        #           "data:{tasmota.wifi().find('quality', '--')}%\r\n\r\n"
        cnx.write(payload1)
      end
    end

  end

  def send_info_feed(cnx, uri, verb)
    cnx.set_chunked(false)     # no chunking since we use EventSource
    cnx.send(200, "text/event-stream")
    #
    var feed = feeder(self, cnx)
    feed.send_feed()          # send first values immediately
  end

  # Add button 'GPIO Viewer' redirects to '/part_wiz?'
  def web_add_console_button()
    self.send_iframe_code()
  end
  def web_add_main_button()
    self.send_iframe_code()
  end
  def web_add_management_button()
    self.send_iframe_code()
  end
  def web_add_config_button()
    self.send_iframe_code()
  end

  def send_iframe_code()
    import webserver
    var ip = tasmota.wifi().find('ip')
    if (ip == nil)
      ip = tasmota.eth().find('ip')
    end
    if (ip != nil)
      webserver.content_send(
        f'<div style="left: 0px; border: 0px none; background-color: #252525; height: 28px; position: fixed; width: 340px; overflow: hidden; padding: 0px 0px; top: 0px; left: 50%; transform: translateX(-50%);">'
         '<iframe src="http://{ip}:{self.port}/info" scrolling="no" '
         'style="color:#eaeaea; border:0px none;height:20px;width:340px;margin:0px 8px 0px 8px;padding:0px 0px;">'
         '</iframe>'
         '<hr style="margin:0px;">'
         '</div>')
        # f"<form style='display: block;' action='http://{ip}:{self.port}/' method='post' target='_blank'><button>GPIO Viewer</button></form><p></p>")
    end
  end

end

return debug_panel

# if tasmota
#   global.debug_panel = debug_panel(8887)
# end

# return global.debug_panel
