# TouchScreen calibration
#
# rm DisplayCalibrate.tapp; zip -j -0 DisplayCalibrate.tapp ts_calibrate/*
#
var ts_calibrate = module("ts_calibrate")

ts_calibrate.init = def (m)
  class TS_Calibrate
    var l1, l2                  # LVGL lines used to draw a cross
    var p1, p2, p3, p4          # points needs to be kept in memory across calls
    var pt_arr1, pt_arr2
    var f20                     # font Montserrat 20
    var scr_orig                # original screen
    var scr_ts                  # screen used for calibration
    var hres, vres
    var instr_label             # label for instructions
  #
    var raw_pts                 # 4x raw points for 4 corners
    var raw_pts_weight          # weight for current corner, used for an aveage measure
    #
    var state_closure           # closure to call at next iteration
    var state_corner            # which corner are we now tracking
    #
    var m_line                  # display.ini parsed, array of 2 string (before and after :M line)

    static msg_prefix = "Touch Screen calibration\n"
    static msg_touch = "Press the center of\n"
    static msg_corners = [ "upper left cross", "upper right cross", "lower left cross", "lower right cross" ]
    static msg_ok = "Calibration successful\nRestarting in 5 seconds"
    static msg_nok = "Calibration failed\nPlease try again"

    static round = def (v) return int(v + 0.5) end

    def init()
    end

    def start()
      self.state_corner= -1                   # no corner being tracked yet
      self.m_line = nil

      # check display.ini to see if touchscreen is present with ':M' line in display.ini
      self.m_line = self.load_m_line()
      if !self.m_line
        tasmota.log("TS : Abort, no touchscreen ':M' line present in 'display.ini'", 2)
        return
      end

      lv.start()

      # check if `DisplayRotate` is set to zero
      var display_rotate_ret = tasmota.cmd("DisplayRotate")
      if display_rotate_ret != nil && display_rotate_ret["DisplayRotate"] != 0
        tasmota.log("TS : Calibration requires 'DisplayRotate 0'")
        return
      end

      self.raw_pts = [ lv.point(), lv.point(), lv.point(), lv.point() ]

      self.scr_orig = lv.scr_act()            # save the current screen to restore it later
      self.f20 = lv.montserrat_font(20)       # load embedded Montserrat 20
      self.hres = lv.get_hor_res()
      self.vres = lv.get_ver_res()

      self.scr_ts = lv.obj(0)                 # create a new temporary screen for the calibration
      lv.scr_load(self.scr_ts)

      self.instr_label = lv.label(self.scr_ts)
      self.instr_label.center()
      if self.f20 != nil self.instr_label.set_style_text_font(self.f20, lv.PART_MAIN | lv.STATE_DEFAULT) end
      self.instr_label.set_text(self.msg_prefix)

      self.l1 = lv.line(self.scr_ts)
      self.l2 = lv.line(self.scr_ts)
      self.l1.set_style_line_width(2, lv.PART_MAIN | lv.STATE_DEFAULT)
      self.l2.set_style_line_width(2, lv.PART_MAIN | lv.STATE_DEFAULT)

      self.p1 = lv.point()
      self.p2 = lv.point()
      self.p3 = lv.point()
      self.p4 = lv.point()

      # register ourselves as driver
      tasmota.add_driver(self)

      # start calibrate in 2 seconds
      tasmota.set_timer(2000, /-> self.do_next_corner())
    end

    def do_cross_n(n)
      self.state_corner = n
      self.raw_pts_weight = 0         # reset weight (for average of measures)

      if   n == 0    self.draw_cross(20,20,30)
      elif n == 1    self.draw_cross(self.hres - 20, 20, 30)
      elif n == 2    self.draw_cross(20, self.vres - 20, 30)
      else           self.draw_cross(self.hres - 20, self.vres - 20, 30)
      end
      # set message
      self.instr_label.set_text(self.msg_prefix + self.msg_touch + self.msg_corners[n])
    end

    # remove and restore previous state
    def del()
      lv.scr_load(self.scr_orig)            # restore previous screen
      self.scr_ts.del()                     # delete all objects
      tasmota.remove_driver(self)
    end

    # draw cross
    def draw_cross(x, y, sz)
      var sz2 = sz / 2
      self.p1.x = x - sz2
      self.p1.y = y
      self.p2.x = x + sz2
      self.p2.y = y

      self.pt_arr1 = lv.lv_point_arr([self.p1, self.p2])
      self.l1.set_points(self.pt_arr1, 2)

      self.p3.x = x
      self.p3.y = y - sz2
      self.p4.x = x
      self.p4.y = y + sz2

      self.pt_arr2 = lv.lv_point_arr([self.p3, self.p4])
      self.l2.set_points(self.pt_arr2, 2)
    end

    def every_50ms()
      #
      if self.state_closure   self.state_closure() end
    end

    def track_touch_screen()
      var tracking = lv.get_ts_calibration()
      if tracking.state
        # screen is pressed, compute an average of all previous measures
        self.raw_pts[self.state_corner].x = (self.raw_pts[self.state_corner].x * self.raw_pts_weight + tracking.raw_x) / (self.raw_pts_weight + 1)
        self.raw_pts[self.state_corner].y = (self.raw_pts[self.state_corner].y * self.raw_pts_weight + tracking.raw_y) / (self.raw_pts_weight + 1)
        self.raw_pts_weight += 1                     # we now have 1 more measure
      else
        # screen is not pressed anymore
        if (self.raw_pts_weight >= 3)
          self.state_closure = nil                          # stop tracking
          tasmota.set_timer(0, /-> self.do_next_corner())   # defer to next corner

        end    # we need at least 3 succesful measures do consider complete
      end
    end

    def do_next_corner()
      # start the measure of the next corner
      self.state_corner += 1
      if self.state_corner <= 3
        self.do_cross_n(self.state_corner)
        self.state_closure = /-> self.track_touch_screen()
      else
        # finished
        self.l1.del()
        self.l2.del()
        self.finish()
      end
    end

    # All values are computed and correct, log results and store to 'display.ini'
    def finish()
      # calibration is finished, do the housekeeping
      var p0x = real(self.raw_pts[0].x)
      var p0y = real(self.raw_pts[0].y)
      var p1x = real(self.raw_pts[1].x)
      var p1y = real(self.raw_pts[1].y)
      var p2x = real(self.raw_pts[2].x)
      var p2y = real(self.raw_pts[2].y)
      var p3x = real(self.raw_pts[3].x)
      var p3y = real(self.raw_pts[3].y)
      tasmota.log(format("TS : Calibration (%i,%i) - (%i,%i) - (%i,%i) - (%i,%i)",
                                int(p0x), int(p0y), int(p1x), int(p1y), int(p2x), int(p2y), int(p3x), int(p3y))
                                , 2)
      var m_line = self.calc_geometry(p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y, self.hres, self.vres, 20)
      var ok = false
      if m_line
        ok = self.update_display_ini(m_line)
      end
      if ok
        self.instr_label.set_text(self.msg_prefix + self.msg_ok)
      else
        self.instr_label.set_text(self.msg_prefix + self.msg_nok)
      end
      tasmota.set_timer(3000, /->self.cleanup(ok))
    end

    # cleanup display, remove any widgets created
    def cleanup(restart)
      self.del()

      if (restart)
        tasmota.cmd("Restart 1")
      end
    end

    # Find 'display.ini' file either in root folder or in autoconf file
    # and check that it contains a line ':M*'
    # Returns an array of 2 strings (before and after :M line)
    # or 'nil' if not found
    def load_m_line()
      try
        import re
        import path

        # try display.ini at root
        var disp_ini
        if path.exists("display.ini")
          var disp_f = open("display.ini")
          disp_ini = disp_f.read()
          disp_f.close()
        elif autoconf.get_current_module_path() && path.exists(autoconf.get_current_module_path() + "#display.ini")
          var disp_f = open(autoconf.get_current_module_path() + "#display.ini")
          disp_ini = disp_f.read()
          disp_f.close()
        else
          return nil
        end

        # look for ":M" line
        var sp = re.split(":M.*?\n", disp_ini)
        if size(sp) == 2
          return sp     # found
        end
      except .. as e, m
        tasmota.log("TS : Couldn't open 'display.ini': "+str(e)+" '"+str(m)+"'")
      end
      return nil
    end

    # try to update 'display.ini' if present in file-system
    def update_display_ini(m_line)
      try

        # found
        var disp_ini = self.m_line[0] + str(m_line) + "\n" + self.m_line[1]

        # write back file
        var disp_f = open("display.ini", "w")
        disp_f.write(disp_ini)
        disp_f.close()
        tasmota.log("TS : Successfully updated 'display.ini'", 2)
        return true
      except .. as e, m
        tasmota.log("TS : Error updating 'display.ini': "+str(e)+" '"+str(m)+"'")
      end
      return false
    end

    def calc_geometry(p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y, hres, vres, padding)
      import math

      tasmota.log(format("TS : Geometry (%i,%i) (%i,%i) (%i,%i) (%i,%i) - %ix%i pad %i",
                                  int(p0x), int(p0y),
                                  int(p1x), int(p1y),
                                  int(p2x), int(p2y),
                                  int(p3x), int(p3y),
                                  int(hres), int(vres), int(padding)
                                ), 3)

      var vec_01_x = p1x - p0x
      var vec_01_y = p1y - p0y
      var norm_01 = math.sqrt(vec_01_x * vec_01_x + vec_01_y * vec_01_y)

      var vec_02_x = p2x - p0x
      var vec_02_y = p2y - p0y
      var norm_02 = math.sqrt(vec_01_x * vec_01_x + vec_01_y * vec_01_y)

      var scalar_01_02 = vec_01_x * vec_02_x + vec_01_y * vec_02_y
      var cos_th_01_02 = math.abs(scalar_01_02) / norm_01 / norm_02

      tasmota.log("cos_th_01_02=" + str(cos_th_01_02), 4)

      if (cos_th_01_02 > 0.05)
        tasmota.log("TS : Wrong geometry - bad angle. Try again.", 2)
        return nil
      end

      # Now check the center is valid
      var center_03_x = (p0x + p3x) / 2
      var center_03_y = (p0y + p3y) / 2
      var center_12_x = (p1x + p2x) / 2
      var center_12_y = (p1y + p2y) / 2
      var norm_delta_centers = ((center_12_x - center_03_x) * (center_12_x - center_03_x) + (center_12_y - center_03_y) * (center_12_y - center_03_y)) / norm_01 / norm_02
      tasmota.log("norm_delta_centers=" + str(norm_delta_centers), 4)

      if (norm_delta_centers > 0.02)
        tasmota.log("TS : Wrong geometry - bad center. Try again", 2)
        return nil
      end

      var xmin = (p0x + p2x) / 2
      var xmax = (p1x + p3x) / 2
      var ymin = (p0y + p1y) / 2
      var ymax = (p2y + p3y) / 2
      tasmota.log("raw xmin=" + str(xmin) + " xmax=" + str(xmax) + " ymin=" + str(ymin) + " ymax=" + str(ymax), 4)

      var range_x = xmax - xmin
      var range_y = ymax - ymin

      tasmota.log("range_x=" + str(range_x) + " range_y=" + str(range_y), 4)
      var extend_x = (range_x / (hres - 2*padding) * hres - range_x) / 2
      var extend_y = (range_y / (vres - 2*padding) * vres - range_y) / 2

      xmin -= extend_x
      xmax += extend_x
      ymin -= extend_y
      ymax += extend_y
      tasmota.log("final xmin=" + str(xmin) + " xmax=" + str(xmax) + " ymin=" + str(ymin) + " ymax=" + str(ymax), 4)

      var M_string = format(":M,%i,%i,%i,%i", int(xmin), int(xmax), int(ymin), int(ymax))
      tasmota.log(format("TS : Add this to display.ini '%s'", M_string))
      return M_string
    end
  end
  return TS_Calibrate()
end

return ts_calibrate
