#- LVGL Examples
 -
 - drawing example #1 from https://docs.lvgl.io/latest/en/html/overview/drawing.html
--#

lv.start()

def max(a,b) if a > b return a else return b end end

class lv_signal_bars : lv_obj
  var ancestor_design    # previous design_cb
  var percentage

  def init(parent, copy)
    # init parent object
    super(self, lv_obj).init(parent, copy)
    # keep a copy of 
    self.ancestor_design = self.get_design_cb()
    self.set_design_cb(self.my_design_cb)
    # own values
    self.percentage = 100
    # set default style
    self.set_style_local_bg_opa(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv.OPA_TRANSP)
  end

  def my_design_cb(area, mode)
    # the model is that we have 4 bars and inter-bar (1/4 of width)
    var height = self.get_height()
    var width = self.get_width()

    var inter_bar = max(width / 15, 1)
    var bar = max((width - inter_bar * 3) / 4, 1)
    var bar_offset = bar / 2

    if mode == lv.DESIGN_COVER_CHK
      #- Return false if the object is not covers the clip_area area -#
      return self.ancestor_design.call(self, area, mode)

    elif mode == lv.DESIGN_DRAW_MAIN
      self.ancestor_design.call(self, area, mode)
      var line_dsc = lv_draw_line_dsc()
      lv.draw_line_dsc_init(line_dsc)

      line_dsc.round_start = 1
      line_dsc.round_end = 1
      line_dsc.width = bar
      line_dsc.color = 0xF0F
      var p1 = lv_point()
      var p2 = lv_point()

      for i:0..3
        p1.y = height - 1 - bar_offset
        p1.x = i * (bar + inter_bar) + bar_offset
        p2.y = ((3 - i) * (height - bar)) / 4 + bar_offset
        p2.x = p1.x
        lv.draw_line(p1, p2, area, line_dsc)
      end
    #elif mode == lv.DESIGN_DRAW_POST    # we don't want a frame around this object
      #self.ancestor_design.call(self, area, mode)
    end
    return lv.DESIGN_RES_OK
  end
end

bars = lv_signal_bars(lv.scr_act())
