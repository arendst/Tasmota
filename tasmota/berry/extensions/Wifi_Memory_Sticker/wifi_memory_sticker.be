#######################################################################
# Wifi Memory Sticker
#
# Sticker to show realtime wifi strengh and memory (top left of main page)

#################################################################################
# Wifi_Memory_Sticker
#################################################################################
class Wifi_Memory_Sticker

  static var HTTP_HEAD_STYLE_WIFI =
    "<style>"
    ".wifi{width:18px;height:18px;position:relative}"
    ".arc{padding:0;position:absolute;border:2px solid transparent;border-radius:50%;border-top-color:var(--c_frm)}"
    ".a0{width:2px;height:3px;top:10px;left:11px}"
    ".a1{width:6px;height:6px;top:7px;left:9px}"
    ".a2{width:12px;height:12px;top:4px;left:6px}"
    ".a3{width:18px;height:18px;top:1px;left:3px}"
    ".arc.active{border-top-color:var(--c_ttl)}"
    "</style>"

  def init()

    tasmota.add_driver(self)
  end

  def unload()
    tasmota.remove_driver(self)
  end

  #################################################################################
  # called when displaying the left status line
  def web_status_line_left()
    import webserver
    # display wifi
    if tasmota.wifi('up')
      webserver.content_send(self.HTTP_HEAD_STYLE_WIFI)
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

return Wifi_Memory_Sticker()
