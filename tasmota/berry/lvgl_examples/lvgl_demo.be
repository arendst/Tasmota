lv.start()

hres = lv.get_hor_res()
vres = lv.get_ver_res()

scr = lv.scr_act()
f20 = lv.montserrat_font(20)

scr.set_style_local_bg_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0x0000A0))
scr.set_style_local_bg_grad_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0x000000))
scr.set_style_local_bg_grad_dir(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv.GRAD_DIR_VER)

log = lv_label(scr)
if f20 != nil log.set_style_local_text_font(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, f20) end
log.set_long_mode(lv.LABEL_LONG_SROLL)
log.set_width(hres)
log.set_align(lv.LABEL_ALIGN_LEFT)
log.set_style_local_bg_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0xD00000))
log.set_style_local_bg_opa(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv.OPA_COVER)
log.set_style_local_text_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0xFFFFFF))
log.set_text("Welcome to Tasmota")

tastyle = lv_style()
tastyle.set_bg_color(lv.STATE_DEFAULT, lv_color(0x1fa3ec))
tastyle.set_border_color(lv.STATE_DEFAULT, lv_color(0x0000FF))
tastyle.set_bg_color(lv.STATE_FOCUSED, lv_color(0x0000FF))
tastyle.set_border_color(lv.STATE_FOCUSED, lv_color(0xFFFFFF))
tastyle.set_text_color(lv.STATE_DEFAULT, lv_color(0xFFFFFF))
tastyle.set_radius(lv.STATE_DEFAULT, 10)
tastyle.set_bg_opa(lv.STATE_DEFAULT, lv.OPA_COVER)
if f20 != nil tastyle.set_text_font(lv.STATE_DEFAULT, f20) end

prev_btn = lv_btn(scr)
prev_btn.set_pos(20,vres-40)
prev_btn.set_size(80, 30)
prev_btn.add_style(lv.OBJ_PART_MAIN, tastyle)
prev_label = lv_label(prev_btn)
prev_label.set_text("<")

next_btn = lv_btn(scr)
next_btn.set_pos(220,vres-40)
next_btn.set_size(80, 30)
next_btn.add_style(lv.OBJ_PART_MAIN, tastyle)
next_label = lv_label(next_btn)
next_label.set_text(">")

home_btn = lv_btn(scr)
home_btn.set_pos(120,vres-40)
home_btn.set_size(80, 30)
home_btn.add_style(lv.OBJ_PART_MAIN, tastyle)
home_label = lv_label(home_btn)
home_label.set_text(lv.SYMBOL_OK)




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

  def my_design_cb(area, mode)
    import global
    var lv = global.lv
    def atleast1(x) if x >= 1 return x else return 1 end end
    # the model is that we have 4 bars and inter-bar (1/4 of width)
    var height = self.get_height()
    var width = self.get_width()

    var inter_bar = atleast1(width / 15)
    var bar = atleast1((width - inter_bar * 3) / 4)
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





log_height = log.get_height()

# log.get_style_pad_right(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT)
log.set_style_local_pad_right(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, log_height+1)

bars = lv_signal_bars(log)

bars.set_style_local_bg_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(lv.BLACK))

bars.set_height(log_height)
bars.set_width(log_height)
bars.set_x(log.get_width() - log_height)

#bars.get_style_transform_width(lv.OBJ_PART_MAIN)
#bars.get_style_transform_height(lv.OBJ_PART_MAIN)