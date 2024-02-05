# lv_wifi_graph class

class lv_wifi_graph : lv.chart
  var ws_h
  var ws_w
  var ser1
  var wsl
  var delay

  def init(parent)
    super(self).init(parent)

    # enforce valid lv.version
    import introspect if type(introspect.get(lv, "version")) != 'int'  lv.version = 8 end
    
    self.set_style_bg_color(lv.color(0x000000), lv.PART_MAIN | lv.STATE_DEFAULT)
    self.set_style_bg_opa(100, lv.PART_MAIN | lv.STATE_DEFAULT)
    self.set_style_border_color(lv.color(0x0099EE), lv.PART_MAIN | lv.STATE_DEFAULT)
    self.set_style_border_width(1, lv.PART_MAIN | lv.STATE_DEFAULT)
    #self.set_style_border_opa(200, lv.PART_MAIN | lv.STATE_DEFAULT)
    self.set_style_radius(0, lv.PART_MAIN | lv.STATE_DEFAULT)
    self.set_style_pad_all(2, lv.PART_MAIN | lv.STATE_DEFAULT)

    self.set_range(lv.CHART_AXIS_PRIMARY_Y, 0, 100)
    self.set_div_line_count(0,0)		# no lines
    self.set_type(lv.CHART_TYPE_LINE)
    
    if lv.version == 8
      self.set_style_size(0, lv.PART_INDICATOR) # don't show dots
    else
      self.set_style_size(0, 0, lv.PART_INDICATOR) # don't show dots
    end
    self.set_style_line_rounded(true, lv.PART_ITEMS)
    self.set_style_line_width(2, lv.PART_ITEMS) # don't show dots
    self.set_update_mode(lv.CHART_UPDATE_MODE_SHIFT)
    
    self.set_point_count(40)

    self.ser1 = self.add_series(lv.color(0xEE4444), lv.CHART_AXIS_PRIMARY_Y)

    self.wsl = lv.label(self)
    self.wsl.set_style_text_color(lv.color(0xFFFFFF), lv.PART_MAIN | lv.STATE_DEFAULT)
    self.wsl.set_text("wifi")
    self.wsl.set_style_text_font(lv.font_montserrat(10), lv.PART_MAIN | lv.STATE_DEFAULT)
    self.wsl.set_align(lv.ALIGN_BOTTOM_MID)

    self.delay = 1

    self.add_event_cb(/->self.before_del(), lv.EVENT_DELETE, 0)   # register `before_del` to be called when object is deleted
    tasmota.add_driver(self)
  end

  def add_wifi_point()
    var wifi = tasmota.wifi()
    var quality = wifi.find("quality")
    if quality != nil
      self.set_next_value(self.ser1, quality)
    end
  end

  def every_second()
    self.delay -= 1
    if self.delay == 0
      self.add_wifi_point()
      self.delay = 2
    end
  end

  def before_del()
    tasmota.remove_driver(self)
  end
end

return lv_wifi_graph

#-
import lv_wifi_graph
ws_h = 40
ws_w = 80
ws = lv_wifi_graph(scr)
ws.set_size(ws_w,ws_h)
ws.set_pos(hres-ws_w, stat_line.get_height())
tasmota.add_driver(ws)
-#
