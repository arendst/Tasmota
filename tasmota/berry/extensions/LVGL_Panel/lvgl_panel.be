#
# lvgl_panel.be - implements a real-time mirroring of LVGL display on the main page
#
# Copyright (C) 2026  Stephan Hadinger & Theo Arends & Milko Daskalov
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

class lvgl_panel
  static var PORT = 8881 # streaming port

  var stream_cb # callback with compressed stream
  var server
  var connections
  var lvgl_btn_file

  def init(port)
    self.lvgl_btn_file = tasmota.wd + 'lvgl.html'

    if tasmota.is_network_up()
      self.init_network()
    else
      tasmota.when_network_up(/ -> self.init_network())
    end
  end

  def init_network()
    self.server = tcpserver(self.PORT)
    self.connections = []
    tasmota.add_driver(self)
  end

  def unload()
    if (self.stream_cb != nil)
      import introspect
      lv.set_stream_cb(introspect.toptr(0))
      cb.free_cb(self.stream_cb)
    end
    self.server.close()
    for conn: self.connections  # close all active connections
      conn.close()
    end
    self.connections = nil      # and free memory
    tasmota.remove_driver(self) # remove driver, normally already done by tasmota.unload_ext
  end

  def every_50ms()
    var idx = 0
    while idx < size(self.connections)
      var cnx = self.connections[idx]
      if !cnx.connected()
        self.connections.remove(idx)
      else
        idx += 1
      end
    end

    if self.server.hasclient() # check for incoming connections
      var cnx = self.server.acceptasync()
      var req = cnx.read()
      var w = lv.get_hor_res()
      var h = lv.get_ver_res()
      cnx.write(f"HTTP/1.1 200 OK\r\n"
        "Content-Type: application/octet-stream\r\n"
        "Screen-Size: {w}x{h}\r\n"
        "Accept-Ranges: none\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: *\r\n"
        "Access-Control-Allow-Headers: *\r\n"
        "Access-Control-Expose-Headers: Screen-Size\r\n"
        "Connection: close\r\n\r\n"
      )
      self.connections.push(cnx)
      lv.scr_act().invalidate()
    end

    if size(self.connections) > 0 && self.stream_cb == nil
      import cb
      self.stream_cb = cb.gen_cb(/buf, len -> self.stream(buf, len))
      lv.set_stream_cb(self.stream_cb)
    elif size(self.connections) == 0 && self.stream_cb != nil
      import cb
      import introspect
      lv.set_stream_cb(introspect.toptr(0))
      cb.free_cb(self.stream_cb)
      self.stream_cb = nil
    end
  end

  def stream(buf, len)
    import introspect
    for cnx : self.connections
      if !cnx.writebytes(introspect.toptr(buf), len)
        cnx.close()
      end
    end
  end

  def web_add_main_button()
    import webserver
    var f = open(self.lvgl_btn_file)
    var lvgl_btn = f.read()
    f.close()
    webserver.content_send(lvgl_btn)
  end

  def web_add_handler()
    import webserver
    webserver.on('/lvgl_touch', / -> self.lvgl_touch())
  end

  def lvgl_touch()
    import webserver
    import display
    if !webserver.check_privileged_access() return nil end
    if webserver.has_arg('x') && webserver.has_arg('y') && webserver.has_arg('t')
      var t = int(webserver.arg('t'))
      var x = int(webserver.arg('x'))
      var y = int(webserver.arg('y'))
      display.touch_update(t, x, y, 0)
    end
    webserver.content_send('HTTP/1.1 200 OK\r\n'
        'Access-Control-Allow-Origin: *\r\n'
        'Access-Control-Allow-Methods: POST\r\n'
        'Content-Length: 0\r\n\r\n'
    )
    webserver.content_close()
  end

end

return lvgl_panel()