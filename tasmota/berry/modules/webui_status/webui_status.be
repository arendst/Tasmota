# display Wifi and Free Heap in the Web Status line
class webui_status
  static var HTTP_HEAD_STYLE_WIFI =
    "<style>"
    ".wifi{width:18px;height:18px;position:relative}"
    ".arc{padding:0;position:absolute;border:2px solid transparent;border-radius:50%%;border-top-color:%s}"
    ".a0{width:2px;height:3px;top:10px;left:11px}"
    ".a1{width:6px;height:6px;top:7px;left:9px}"
    ".a2{width:12px;height:12px;top:4px;left:6px}"
    ".a3{width:18px;height:18px;top:1px;left:3px}"
    ".arc.active{border-top-color:%s}"
    "</style>"

  def init()
   tasmota.add_driver(self)
  end

  def remove()
    tasmota.remove_driver(self)
  end
  def web_status_line_left()
    import webserver
    # display wifi
    if tasmota.wifi('up')
      webserver.content_send(format(self.HTTP_HEAD_STYLE_WIFI, tasmota.webcolor(2 #-COL_FORM-#), tasmota.webcolor(18 #-COL_TITLE-#)))
      var rssi = tasmota.wifi('rssi')
      webserver.content_send(format("<div class='wifi' title='RSSI %d%%, %d dBm' style='padding:0 2px 0 2px;'><div class='arc a3 %s'></div><div class='arc a2 %s'></div><div class='arc a1 %s'></div><div class='arc a0 active'></div></div>",
                                    tasmota.wifi('quality'), rssi,
                                    rssi >= -55 ? "active" : "",
                                    rssi >= -70 ? "active" : "",
                                    rssi >= -85 ? "active" : ""))
    end
    # display free heap
    webserver.content_send(f"<span>&nbsp;{tasmota.memory('heap_free')}k</span>")
   end
end
return webui_status()
