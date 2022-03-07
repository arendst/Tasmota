# lv_tasmota_log class

class lv_tasmota_log_roboto : lv.label
  var lines
  var line_len
  var log_reader
  var log_level

  def init(parent, line_len)
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

    self.set_style_text_color(lv.color(0x00FF00), lv.PART_MAIN | lv.STATE_DEFAULT)
    self.set_long_mode(lv.LABEL_LONG_CLIP)
    var roboto12 = lv.font_robotocondensed_latin1(12)
    self.set_style_text_font(roboto12, lv.PART_MAIN | lv.STATE_DEFAULT)
    # var lg_font = lv.font_montserrat(10)
    # self.set_style_text_font(lg_font, lv.PART_MAIN | lv.STATE_DEFAULT)
    self.set_text("")

  	self.line_len = line_len
  	self.log_reader = tasmota_log_reader()
  	self.log_level = 2

    # fill with empty strings
  	self.lines = []
  	for i:1..line_len
      self.lines.push("")
    end
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
    self.set_text(msg)
  end
end

return lv_tasmota_log_roboto

# import lv_tasmota_log
# var lg = lv_tasmota_log(scr, 6)
# lg.set_size(hres, 95)
# lg.set_pos(0, stat_line.get_height() + 40)
# tasmota.add_driver(lg)

# var roboto12 = lv.font_robotocondensed_latin1(12) lg.set_style_text_font(roboto12, lv.PART_MAIN | lv.STATE_DEFAULT)