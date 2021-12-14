#- LVGL lv_clock_icon
 -
--#

class lv_clock_icon: lv.label
  var hour, minute, sec

  def init(parent)
    super(self).init(parent)
    var f_s7_16 = lv.seg7_font(16)
    if f_s7_16 != nil  self.set_style_text_font(f_s7_16, lv.PART_MAIN | lv.STATE_DEFAULT) end

    if parent != nil
      var parent_height = parent.get_height()

      self.set_text("--:--")
      self.refr_size()
      var w = self.get_width()
      self.set_y((parent.get_height() - self.get_height()) / 2)   # center vertically

      var pad_right = parent.get_style_pad_right(lv.PART_MAIN | lv.STATE_DEFAULT)
      self.set_x(parent.get_width() - w - pad_right - 3)
      parent.set_style_pad_right(pad_right + w + 6, lv.PART_MAIN | lv.STATE_DEFAULT)

      self.set_style_bg_color(lv.color(lv.COLOR_BLACK), lv.PART_MAIN | lv.STATE_DEFAULT)
    end

    tasmota.add_driver(self)
  end

  def set_time(hour, minute, sec)
    import string
    if hour != self.hour || minute != self.minute || sec != self.sec
      var txt = string.format("%02d%s%02d", hour, sec % 2 ? ":" : " ", minute)
      self.hour = hour
      self.minute = minute
      self.sec = sec
      #if txt[0] == '0'  txt = '!' .. string.split(txt,1)[1] end  # replace first char with '!'
      self.set_text(txt)
    end
  end

  def every_second()
    var now = tasmota.time_dump(tasmota.rtc()['local'])
    if now['year'] != 1970
      self.set_time(now['hour'], now['min'], now['sec'])
    end
  end

  def del()
    super(self).del()
    tasmota.remove_driver(self)
  end
end