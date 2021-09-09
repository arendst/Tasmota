#- LVGL lv_clock_icon
 -
--#

class lv_clock_icon: lv_label
  def init(parent, copy)
    super(self).init(parent, copy)
    var f_s7_16 = lv.seg7_font(16)
    if f_s7_16 != nil  self.set_style_local_text_font(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, f_s7_16) end

    if parent != nil
      var parent_height = parent.get_height()

      self.set_text("--:--")
      var w = self.get_width()
      self.set_y((parent.get_height() - self.get_height()) / 2)   # center vertically

      var pad_right = parent.get_style_pad_right(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT)
      self.set_x(parent.get_width() - w - pad_right - 3)
      parent.set_style_local_pad_right(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, pad_right + w + 6)

      self.set_style_local_bg_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(lv.COLOR_BLACK))
    end

    tasmota.add_driver(self)
  end

  def every_second()
  end

  def del()
    super(self).del()
    tasmota.remove_driver(self)
  end
end