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
  var hdr       # bytes() object reused when generating payload
  var paint_cb  # callback to paint
  var server
  var connections
  var lvgl_btn_file

  static var PORT = 8881       # streaming port
  static var CHUNK_SIZE = 2000

  def init(port)
    self.hdr = bytes(-10)
    self.hdr.set(0, 0x4C56, 2) # 2(LV) + 2(x) + 2(y) + 2(w) + 2(h)

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
    if (self.paint_cb != nil)
      import introspect
      lv.set_paint_cb(introspect.toptr(0))
      cb.free_cb(self.paint_cb)
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
        cnx.close()
        self.connections.remove(idx)
        if (idx == 0 && self.paint_cb != nil)
          import introspect
          import cb
          lv.set_paint_cb(introspect.toptr(0))
          cb.free_cb(self.paint_cb)
          self.paint_cb = nil
        end
      else
        idx += 1
      end
    end

    if self.server.hasclient() # check for incoming connections
      var cnx = self.server.acceptasync()
      var req = cnx.read()
      var w = lv.get_hor_res()
      var h = lv.get_ver_res()
      var res = bytes().fromstring(f"HTTP/1.1 200 OK\r\n"
        "Content-Type: application/octet-stream\r\n"
        "Screen-Size: {w}x{h}\r\n"
        "Accept-Ranges: none\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: *\r\n"
        "Access-Control-Allow-Headers: *\r\n"
        "Access-Control-Expose-Headers: Screen-Size\r\n"
        "Connection: close\r\n\r\n"
      )
      self.send(cnx, res)
      self.connections.push(cnx)
      if (self.paint_cb == nil)
        import cb
        self.paint_cb = cb.gen_cb(/x1,y1,x2,y2,pixels -> self.paint(x1,y1,x2,y2,pixels))
        lv.set_paint_cb(self.paint_cb)
      end
      lv.scr_act().invalidate()
    end
  end

  def send(cnx, data)
    var data_size = size(data)
    var sent = 0
    while sent < data_size && cnx.connected()
      sent += cnx.write(data)
    end
  end

  def paint(x1,y1,x2,y2,pixels)
    import introspect
    var width = x2 - x1 + 1
    var height = y2 - y1 + 1

    var hdr = self.hdr
    hdr.set(2, x1, 2)
    hdr.set(4, y1, 2)
    hdr.set(6, width, 2)
    hdr.set(8, height, 2)

    var total_bytes = width * height * 2
    var data = bytes(introspect.toptr(pixels), total_bytes)

    for cnx: self.connections
      var sent = 0
      while sent < 10 && cnx.connected()
        sent += cnx.write(hdr)
      end

      var offset = 0
      while offset < total_bytes && cnx.connected()
        var remaining = total_bytes - offset
        var data_size = (remaining < self.CHUNK_SIZE) ? remaining : self.CHUNK_SIZE
        offset += cnx.write(data, offset, data_size)
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
    if webserver.has_arg('x') && webserver.has_arg('y')
      var x = int(webserver.arg('x'))
      var y = int(webserver.arg('y'))
      # log(f'>>>TS: lvgl_touch x: {x}, y: {y}')
      display.touch_update(1, x, y, 0)
    end

    webserver.content_send('HTTP/1.1 200 OK\r\n'
        'Access-Control-Allow-Origin: *\r\n'
        'Access-Control-Allow-Methods: POST\r\n'
        'Content-Length: 5\r\n\r\n'
    )
    webserver.content_close()
  end

end

return lvgl_panel()