# LVGL lv_clock_icon
# LVGL lv_signal_bars and lv_wifi_bars
# LVGL lv_signal_bars and lv_wifi_bars
#
#-

import path 
path.remove("lv_tasmota_widgets.bec")
load('lv_tasmota_widgets.be')

solidify.dump(lv_clock, true)
solidify.dump(lv_clock_icon, true)
solidify.dump(lv_signal_arcs, true)
solidify.dump(lv_wifi_arcs, true)
solidify.dump(lv_wifi_arcs_icon, true)
solidify.dump(lv_signal_bars, true)
solidify.dump(lv_wifi_bars, true)
solidify.dump(lv_wifi_bars_icon, true)

-#

class lv_label end    # for solidification

#@ solidify:lv_clock,weak
class lv_clock: lv_label
  var hour, minute, sec

  def init(parent)
    super(self).init(parent)
    var f_s7_16 = lv.seg7_font(16)
    if f_s7_16 != nil  self.set_style_text_font(f_s7_16, lv.PART_MAIN | lv.STATE_DEFAULT) end

    self.set_text("--:--")

    self.add_event_cb(/->self.before_del(), lv.EVENT_DELETE, 0)   # register `before_del` to be called when object is deleted
    tasmota.add_driver(self)
  end

  def set_time(hour, minute, sec)
    if hour != self.hour || minute != self.minute || sec != self.sec
      var txt = format("%02d%s%02d", hour, sec % 2 ? ":" : " ", minute)
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

  def before_del()
    tasmota.remove_driver(self)
  end
end

#@ solidify:lv_clock_icon,weak
class lv_clock_icon: lv_clock

  def init(parent)
    super(self).init(parent)

    if parent != nil
      var parent_height = parent.get_height()

      self.refr_size()
      var w = self.get_width()
      self.set_y((parent.get_height() - self.get_height()) / 2)   # center vertically

      var pad_right = parent.get_style_pad_right(lv.PART_MAIN | lv.STATE_DEFAULT)
      self.set_x(parent.get_width() - w - pad_right - 3)
      parent.set_style_pad_right(pad_right + w + 6, lv.PART_MAIN | lv.STATE_DEFAULT)

      self.set_style_bg_color(lv.color(lv.COLOR_BLACK), lv.PART_MAIN | lv.STATE_DEFAULT)
    end
  end

end

# ##########################################################################################
class lv_obj end    # for solidification

#@ solidify:lv_signal_arcs,weak
class lv_signal_arcs : lv_obj
  var percentage          # value to display, range 0..100
  var area, arc_dsc       # instances of objects kept to avoid re-instanciating at each call

  def init(parent)
    super(self).init(parent)
    self.set_style_bg_opa(0, 0)         # transparent background
    self.set_style_border_width(0, 0)   # remove border
    self.set_style_pad_all(0,0)
    # own values
    self.percentage = 100
    # pre-allocate buffers
    self.area = lv.area()
    self.arc_dsc = lv.draw_arc_dsc()
    # we don't need a closure with `add_event_cb` because of special handling
    self.add_event_cb(self.widget_event, lv.EVENT_DRAW_MAIN, 0)
  end

  def widget_event(event)
    var code = event.get_code()

    import math
    def atleast1(x) if x >= 1 return x else return 1 end end
    # the model is that we have 4 bars and inter-bar (1/4 of width)
    var height = self.get_height()
    var width = self.get_width()

    var inter_bar = atleast1(height / 8)
    var bar = atleast1((height - inter_bar * 2) / 3)
    var bar_offset = bar / 2
    #print("inter_bar", inter_bar, "bar", bar, "bar_offset", bar_offset)

    if code == lv.EVENT_DRAW_MAIN
      var arc_dsc = self.arc_dsc
      var param = event.get_param()
      var layer = event.get_layer()
    
      # get coordinates of object
      self.get_coords(self.area)
      var x_ofs = self.area.x1
      var y_ofs = self.area.y1

      lv.draw_arc_dsc_init(arc_dsc)                  # initialize lv.draw_line_dsc structure
      self.init_draw_arc_dsc(lv.PART_MAIN, arc_dsc)  # copy the current values

      arc_dsc.rounded = 1
      arc_dsc.width = (bar * 3 + 1) / 4
      var on_color = self.get_style_line_color(lv.PART_MAIN | lv.STATE_DEFAULT)
      var off_color = self.get_style_bg_color(lv.PART_MAIN | lv.STATE_DEFAULT)

      # initial calculation, but does not take into account bounding box
      # var angle = int(math.deg(math.atan2(width / 2, height)))

      # better calculation
      var hypotenuse = height - bar           # center if at bar/2 from bottom and circle stops at bar/2 from top
      var adjacent = width / 2 - bar_offset   # stop at bar_offset from side
      var angle = int(90 - math.deg(math.acos(real(adjacent) / real(hypotenuse))))
      if (angle > 45) angle = 45 end

      # print("hypotenuse",hypotenuse,"adjacent",adjacent,"angle",angle)
      arc_dsc.center_x = x_ofs + width / 2
      arc_dsc.center_y = y_ofs + height - 1 - bar_offset

      arc_dsc.color = self.percentage >= 25 ? on_color : off_color
      arc_dsc.radius = 0 * (bar + inter_bar) + bar_offset
      arc_dsc.start_angle = 0
      arc_dsc.end_angle = 306
      lv.draw_arc(layer, arc_dsc)

      arc_dsc.radius = 1 * (bar + inter_bar) + bar_offset - 1
      arc_dsc.start_angle = 270 - angle
      arc_dsc.end_angle = 270 + angle
      arc_dsc.color = self.percentage >= 50 ? on_color : off_color
      lv.draw_arc(layer, arc_dsc)

      arc_dsc.radius = 2 * (bar + inter_bar) + bar_offset - 2
      arc_dsc.start_angle = 270 - angle
      arc_dsc.end_angle = 270 + angle
      arc_dsc.color = self.percentage >= 75 ? on_color : off_color
      lv.draw_arc(layer, arc_dsc)
    end
  end

  def set_percentage(v)
    var old_bars = self.percentage / 25
    if v > 100 v = 100 end
    if v < 0   v = 0 end
    self.percentage = v
    if old_bars != v / 25
      self.invalidate()    # be frugal and avoid updating the widget if it's not needed
    end
  end

  def get_percentage()
    return self.percentage
  end
end

#@ solidify:lv_wifi_arcs,weak
class lv_wifi_arcs: lv_signal_arcs
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

  def before_del()    # called when the widget is being deleted
    tasmota.remove_driver(self)
  end
end

#@ solidify:lv_wifi_arcs_icon,weak
class lv_wifi_arcs_icon: lv_wifi_arcs
  def init(parent)
    super(self).init(parent)
    self.set_style_line_color(lv.color(lv.COLOR_WHITE), lv.PART_MAIN | lv.STATE_DEFAULT)
    self.set_style_bg_color(lv.color(lv.COLOR_BLACK), lv.PART_MAIN | lv.STATE_DEFAULT)
    if parent != nil
      var parent_height = parent.get_height()
      var pad_right = parent.get_style_pad_right(lv.PART_MAIN | lv.STATE_DEFAULT)
      self.set_height(parent_height)
      var w = (parent_height*4)/3
      self.set_width(w)	# 130%
      self.set_x(parent.get_width() - w - pad_right)
      parent.set_style_pad_right(pad_right + w + 1, lv.PART_MAIN | lv.STATE_DEFAULT)
    end
  end
end

# ##########################################################################################

#@ solidify:lv_signal_bars,weak
class lv_signal_bars : lv_obj
  var percentage              # value to display, range 0..100
  var area, line_dsc          # instances of objects kept to avoid re-instanciating at each call

  def init(parent)
    super(self).init(parent)
    self.set_style_bg_opa(0, 0)         # transparent background
    self.set_style_border_width(0, 0)   # remove border
    # own values
    self.percentage = 100
    # pre-allocate buffers
    self.area = lv.area()
    self.line_dsc = lv.draw_line_dsc()
    # we don't need a closure with `add_event_cb` because of special handling
    self.add_event_cb(self.widget_event, lv.EVENT_DRAW_MAIN, 0)
  end

  def widget_event(event)
    # Call the ancestor's event handler
    var code = event.get_code()

    def atleast1(x) if x >= 1 return x else return 1 end end
    # the model is that we have 4 bars and inter-bar (1/4 of width)
    var height = self.get_height()
    var width = self.get_width()

    var inter_bar = atleast1(width / 15)
    var bar = atleast1((width - inter_bar * 3) / 4)
    var bar_offset = bar / 2

    if code == lv.EVENT_DRAW_MAIN
      var line_dsc = self.line_dsc
      var param = event.get_param()
      var layer = event.get_layer()

      # get coordinates of object
      self.get_coords(self.area)
      var x_ofs = self.area.x1
      var y_ofs = self.area.y1

      lv.draw_line_dsc_init(line_dsc)                  # initialize lv_draw_line_dsc structure
      self.init_draw_line_dsc(lv.PART_MAIN, line_dsc)  # copy the current values

      line_dsc.round_start = 1
      line_dsc.round_end = 1
      line_dsc.width = bar
      var on_color = self.get_style_line_color(lv.PART_MAIN | lv.STATE_DEFAULT)
      var off_color = self.get_style_bg_color(lv.PART_MAIN | lv.STATE_DEFAULT)

      for i:0..3    # 4 bars
        line_dsc.color = self.percentage >= (i+1)*20 ? on_color : off_color
        line_dsc.p1_y = y_ofs + height - 1 - bar_offset
        line_dsc.p1_x = x_ofs + i * (bar + inter_bar) + bar_offset
        line_dsc.p2_y = y_ofs + ((3 - i) * (height - bar)) / 4 + bar_offset
        line_dsc.p2_x = line_dsc.p1_x
        lv.draw_line(layer, line_dsc)
      end
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

#@ solidify:lv_wifi_bars,weak
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

  def before_del()    # called when the widget is being deleted
    tasmota.remove_driver(self)
  end
end

#@ solidify:lv_wifi_bars_icon,weak
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
