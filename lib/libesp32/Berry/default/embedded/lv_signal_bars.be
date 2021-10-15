#- LVGL lv_signal_bars and lv_wifi_bars
 -
--#

class lv_signal_bars : lv.obj
  var percentage              # value to display, range 0..100
  var p1, p2, area, line_dsc  # instances of objects kept to avoid re-instanciating at each call

  def init(parent)
    # init custom widget (don't call super constructor)
    _lvgl.create_custom_widget(self, parent)
    # own values
    self.percentage = 100
    # pre-allocate buffers
    self.p1 = lv.point()
    self.p2 = lv.point()
    self.area = lv.area()
    self.line_dsc = lv.draw_line_dsc()
  end

  def widget_event(cl, event)
    # Call the ancestor's event handler
    if lv.obj_event_base(cl, event) != lv.RES_OK  return  end
    var code = event.code

    def atleast1(x) if x >= 1 return x else return 1 end end
    # the model is that we have 4 bars and inter-bar (1/4 of width)
    var height = self.get_height()
    var width = self.get_width()

    var inter_bar = atleast1(width / 15)
    var bar = atleast1((width - inter_bar * 3) / 4)
    var bar_offset = bar / 2

    if code == lv.EVENT_DRAW_MAIN
      var clip_area = lv.area(event.param)

      # get coordinates of object
      self.get_coords(self.area)
      var x_ofs = self.area.x1
      var y_ofs = self.area.y1

      lv.draw_line_dsc_init(self.line_dsc)                  # initialize lv_draw_line_dsc structure
      self.init_draw_line_dsc(lv.PART_MAIN, self.line_dsc)  # copy the current values

      self.line_dsc.round_start = 1
      self.line_dsc.round_end = 1
      self.line_dsc.width = bar
      var on_color = self.get_style_line_color(lv.PART_MAIN | lv.STATE_DEFAULT)
      var off_color = self.get_style_bg_color(lv.PART_MAIN | lv.STATE_DEFAULT)

      lv.event_send(self, lv.EVENT_DRAW_PART_BEGIN, self.line_dsc)
      for i:0..3    # 4 bars
        self.line_dsc.color = self.percentage >= (i+1)*20 ? on_color : off_color
        self.p1.y = y_ofs + height - 1 - bar_offset
        self.p1.x = x_ofs + i * (bar + inter_bar) + bar_offset
        self.p2.y = y_ofs + ((3 - i) * (height - bar)) / 4 + bar_offset
        self.p2.x = self.p1.x
        lv.draw_line(self.p1, self.p2, clip_area, self.line_dsc)
      end
      lv.event_send(self, lv.EVENT_DRAW_PART_END, self.line_dsc)
    end
  end

  def set_percentage(v)
    var old_bars = self.percentage / 20
    if v > 100 v = 100 end
    if v < 0   v = 0 end
    self.percentage = v
    if old_bars != v / 20
      self.invalidate()    # be frugal and avoid updating the widget if it's not needed
    end
  end

  def get_percentage()
    return self.percentage
  end
end

class lv_wifi_bars: lv_signal_bars
  def init(parent)
    super(self).init(parent)
    tasmota.add_driver(self)
    self.set_percentage(0)    # we generally start with 0, meaning not connected
  end

  def every_second()
    var wifi = tasmota.wifi()
    var quality = wifi.find("quality")
    var ip = wifi.find("ip")
    if ip == nil
      self.set_percentage(0)
    elif quality != nil
      self.set_percentage(quality)
    end
  end

  def del()
    super(self).del()
    tasmota.remove_driver(self)
  end
end

class lv_wifi_bars_icon: lv_wifi_bars
  def init(parent)
    super(self).init(parent)
    self.set_style_line_color(lv.color(lv.COLOR_WHITE), lv.PART_MAIN | lv.STATE_DEFAULT)
    self.set_style_bg_color(lv.color(lv.COLOR_BLACK), lv.PART_MAIN | lv.STATE_DEFAULT)
    if parent != nil
      var parent_height = parent.get_height()
      var pad_right = parent.get_style_pad_right(lv.PART_MAIN | lv.STATE_DEFAULT)
      self.set_height(parent_height)
      self.set_width(parent_height)
      self.set_x(parent.get_width() - parent_height - pad_right)
      parent.set_style_pad_right(pad_right + parent_height + 1, lv.PART_MAIN | lv.STATE_DEFAULT)
    end
  end
end
