#- LVGL Examples
 -
 - drawing example #1 from https://docs.lvgl.io/latest/en/html/overview/drawing.html
--#

lv.start()

class lv_signal_bars : lv_obj
  var ancestor_design    # previous design_cb
  var percentage
  var p1, p2, line_dsc, area

  def init(parent, copy)
    import global
    var lv = global.lv
    # init parent object
    super(self, global.lv_obj).init(parent, copy)
    # keep a copy of 
    self.ancestor_design = self.get_design_cb()
    self.set_design_cb(self.my_design_cb)
    # own values
    self.percentage = 100
    # pre-allocate buffers
    self.p1 = global.lv_point()
    self.p2 = global.lv_point()
    self.line_dsc = global.lv_draw_line_dsc()
    self.area = global.lv_area()
  end

  def atleast1(x) if x >= 1 return x else return 1 end end

  def my_design_cb(area, mode)
    import global
    var lv = global.lv
    #def atleast1(x) if x >= 1 return x else return 1 end end
    # the model is that we have 4 bars and inter-bar (1/4 of width)
    var height = self.get_height()
    var width = self.get_width()

    var inter_bar = self.atleast1(width / 15)
    var bar = self.atleast1((width - inter_bar * 3) / 4)
    var bar_offset = bar / 2

    if mode == lv.DESIGN_COVER_CHK
      #- Return false if the object is not covers the clip_area area -#
      return self.ancestor_design.call(self, area, mode)

    elif mode == lv.DESIGN_DRAW_MAIN
      #self.ancestor_design.call(self, area, mode)  - don't draw a background
    
      # get coordinates of area
      self.get_coords(area)
      var x_ofs = area.x1
      var y_ofs = area.y1

      lv.draw_line_dsc_init(self.line_dsc)
      self.init_draw_line_dsc(lv.OBJ_PART_MAIN, self.line_dsc)

      self.line_dsc.round_start = 1
      self.line_dsc.round_end = 1
      self.line_dsc.width = bar
      var on_color = self.get_style_line_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT)
      var off_color = self.get_style_bg_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT)

      for i:0..3
        self.line_dsc.color = self.percentage >= (i+1)*20 ? on_color : off_color
        self.p1.y = y_ofs + height - 1 - bar_offset
        self.p1.x = x_ofs + i * (bar + inter_bar) + bar_offset
        self.p2.y = y_ofs + ((3 - i) * (height - bar)) / 4 + bar_offset
        self.p2.x = self.p1.x
        lv.draw_line(self.p1, self.p2, area, self.line_dsc)
      end
    #elif mode == lv.DESIGN_DRAW_POST    # we don't want a frame around this object
      #self.ancestor_design.call(self, area, mode)
    end
    return lv.DESIGN_RES_OK
  end

  def set_percentage(v)
    var old_bars = self.percentage / 5
    if v > 100 v = 100 end
    if v < 0   v = 0 end
    self.percentage = v
    if old_bars != v / 5
      self.invalidate()    # be frugal and avoid updating the widget if it's not needed
    end
  end

  def get_percentage()
    return self.percentage
  end
end

bars = lv_signal_bars(lv.scr_act())
bars.set_height(16)
bars.set_width(16)
bars.set_percentage(45)
bars.set_style_local_line_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0xFF8000))