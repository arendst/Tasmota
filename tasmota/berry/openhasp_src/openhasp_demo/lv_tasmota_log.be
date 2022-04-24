# lv_tasmota_log class

class lv_tasmota_log : lv.obj
  var label             # contains the sub lv_label object
  var lines
  var line_len
  var log_reader
  var log_level

  def init(parent)
    super(self).init(parent)
    self.set_width(parent.get_width())
    self.set_pos(0, 0)

    self.set_style_bg_color(lv.color(0x000000), lv.PART_MAIN | lv.STATE_DEFAULT)
    self.set_style_bg_opa(255, lv.PART_MAIN | lv.STATE_DEFAULT)
    self.move_background()
    self.set_style_border_opa(255, lv.PART_MAIN | lv.STATE_DEFAULT)
    self.set_style_radius(0, lv.PART_MAIN | lv.STATE_DEFAULT)
    self.set_style_pad_all(2, lv.PART_MAIN | lv.STATE_DEFAULT)
    self.set_style_border_color(lv.color(0x0099EE), lv.PART_MAIN | lv.STATE_DEFAULT)
    self.set_style_border_width(1, lv.PART_MAIN | lv.STATE_DEFAULT)
    self.refr_size()
    self.refr_pos()

    self.label = lv.label(self)

    self.label.set_style_text_color(lv.color(0x00FF00), lv.PART_MAIN | lv.STATE_DEFAULT)
    self.label.set_long_mode(lv.LABEL_LONG_CLIP)
    self.label.set_text("")   # bug, still displays "Text"

    self.label.set_width(self.get_width() - 12)
    self.label.set_height(self.get_height() - 6)

    self.add_event_cb( / -> self._size_changed(), lv.EVENT_SIZE_CHANGED, 0)
    self.add_event_cb( / -> self._size_changed(), lv.EVENT_STYLE_CHANGED, 0)
    self.add_event_cb( / -> tasmota.remove_driver(self), lv.EVENT_DELETE, 0)
    
  	self.lines = []
  	self.line_len = 0
  	self.log_reader = tasmota_log_reader()
  	self.log_level = 2
    self._size_changed()

    tasmota.add_driver(self)
  end

  def set_lines_count(line_len)
    if   line_len > self.line_len       # increase lines
      for i: self.line_len .. line_len-1
        self.lines.insert(0, "")
      end
    elif line_len < self.line_len          # decrease lines
      for i: line_len .. self.line_len-1
        self.lines.remove(0)
      end
    end
    self.line_len = line_len
  end

  def _size_changed(obj, evt)
    # print(">>> lv.EVENT_SIZE_CHANGED")
    var pad_hor = self.get_style_pad_left(lv.PART_MAIN | lv.STATE_DEFAULT)
                + self.get_style_pad_right(lv.PART_MAIN | lv.STATE_DEFAULT)
                + self.get_style_border_width(lv.PART_MAIN | lv.STATE_DEFAULT) * 2
                + 3
    var pad_ver = self.get_style_pad_top(lv.PART_MAIN | lv.STATE_DEFAULT)
                + self.get_style_pad_bottom(lv.PART_MAIN | lv.STATE_DEFAULT)
                + self.get_style_border_width(lv.PART_MAIN | lv.STATE_DEFAULT) * 2
                + 3
    var w = self.get_width() - pad_hor - 2
    var h = self.get_height() - pad_ver
    self.label.set_size(w, h)
    # print("w",w,"h",h,"pad_hor",pad_hor,"pad_ver",pad_ver)

    # compute how many lines should be displayed
    var h_font = lv.font_get_line_height(self.label.get_style_text_font(0))   # current font's height
    var lines_count = ((h * 2 / h_font) + 1 ) / 2
    # print("h_font",h_font,"h",h,"lines_count",lines_count)
    self.set_lines_count(lines_count)
  end

  def every_second()
    var dirty = false
  	for n:0..20
  	  var line = self.log_reader.get_log(self.log_level)
  	  if line == nil break end  # no more logs
  	  self.lines.remove(0)            # remove first line
  	  self.lines.push(line)
  	  dirty = true
  	end
  	if dirty self.update() end
  end

  def update()
    var msg = self.lines.concat("\n")
    self.label.set_text(msg)
  end
end

return lv_tasmota_log
