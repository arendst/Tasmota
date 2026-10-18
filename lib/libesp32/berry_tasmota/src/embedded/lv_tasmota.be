#- ------------------------------------------------------------ -#
#  Module `lv_tasmota` - piggybacks on `lv` to extend it
#- ------------------------------------------------------------ -#
lv_tasmota = module("lv_tasmota")

# rename `lv` to `lv_ntv` and replace `lv` with `lv_tasmota`
#@ solidify:lv_tasmota.init,weak
def init(lv_tasmota)
  import lv
  lv.start = lv_tasmota.start
  lv._constants = lv_tasmota._constants
  lv.splash_init = lv_tasmota.splash_init
  lv.splash_remove = lv_tasmota.splash_remove
  lv.splash = lv_tasmota.splash
  lv._splash = nil                            # store a reference for the current splash screen parent

  lv.font_montserrat = lv_tasmota.font_montserrat
  lv.montserrat_font = lv_tasmota.font_montserrat

  lv.font_seg7 = lv_tasmota.font_seg7
  lv.seg7_font = lv_tasmota.font_seg7
  lv.font_embedded = lv_tasmota.font_embedded

  lv.load_freetype_font = lv_tasmota.load_freetype_font

  lv.register_button_encoder = lv_tasmota.register_button_encoder
  lv.screenshot = lv_tasmota.screenshot
  lv.set_paint_cb = lv_tasmota.set_paint_cb
  lv.set_stream_cb = lv_tasmota.set_stream_cb
  lv.touch_pointer = lv_tasmota.touch_pointer
  lv._touch_pointer = nil                     # driver instance once the touch pointer was enabled

  # add version information
  lv.version = lv.version_major()
  # use the following to retrofit the version:
  #-
    def fix_lv_version()
      import introspect
      var v = introspect.get(lv, "version")
      # if `lv.version` does not exist, v is `module('undefined')`
      if type(v) != 'int'  lv.version = 8 end
    end
  -#

  # add widgets
  lv.clock = lv_clock
  lv.clock_icon = lv_clock_icon
  lv.signal_arcs = lv_signal_arcs
  lv.signal_bars = lv_signal_bars
  lv.wifi_arcs_icon = lv_wifi_arcs_icon
  lv.wifi_arcs = lv_wifi_arcs
  lv.wifi_bars_icon = lv_wifi_bars_icon
  lv.wifi_bars = lv_wifi_bars

  # display splash screen unless `SetOption135 1` is set
  if tasmota.get_option(135) == 0
    lv.splash_init()
  end

  return nil
end
lv_tasmota.init = init

# run splash now or schedlue later
#@ solidify:lv_tasmota.splash_init,weak
def splash_init()
  import display
  if display.started()
    lv.splash()                     # if display is ready, just do splash now
  else
    # register a driver until display starts

    # create a class for deferred run
    class splash_runner
      def init()
        tasmota.add_driver(self)    # register driver
      end

      def display(cmd, idx, payload, raw)
        import display
        if cmd == "dim" && display.started()
          tasmota.remove_driver(self)
          lv.splash()
        end
      end
    end

    splash_runner()     # create an instance, it auto-registers
  end
end
lv_tasmota.splash_init = splash_init

# remove splash
#@ solidify:lv_tasmota.splash_remove,weak
def splash_remove()
  var _splash = lv._splash
  if _splash
    lv._splash = nil
    _splash.del()
  end
end
lv_tasmota.splash_remove = splash_remove

#@ solidify:lv_tasmota.splash,weak
def splash()
  import display

  if !display.started() return end

  lv.start()                        # just in case it was not already started

  var bg = lv.obj(lv.scr_act())     # create a parent object for splash screen
  var f28 = lv.montserrat_font(28)  # load embedded Montserrat 28
  var white = lv.color(lv.COLOR_WHITE)

  bg.set_style_bg_color(lv.color(0x000066), 0) # lv.PART_MAIN | lv.STATE_DEFAULT
  bg.set_style_radius(0, 0)
  bg.set_style_pad_all(0, 0)
  bg.set_style_border_width(0, 0)
  bg.set_size(lv.pct(100), lv.pct(100))
  bg.refr_pos()
  bg.refr_size()
  # 0x53706C68 = 'Splh' indicating the screen is Splash screen
  bg.set_user_data(0x53706C68)

  var tas_logo = lv.img(bg)
  tas_logo.set_tasmota_logo36()
  tas_logo.align(lv.ALIGN_LEFT_MID, 0, 0)

  var tas = lv.label(bg)
  # tas.set_style_bg_opa(lv.OPA_TRANSP, lv.PART_MAIN | lv.STATE_DEFAULT)
  tas.set_style_text_color(white, 0)          # lv.PART_MAIN | lv.STATE_DEFAULT
  tas.set_text("TASMOTA")
  if lv.get_hor_res() >= 200
    if f28 != nil tas.set_style_text_font(f28, 0) end
  end
  tas.set_align(lv.ALIGN_LEFT_MID)
  tas.set_x(42)

  var driver_name = display.driver_name()
  var disp
  if size(driver_name) > 0
    disp = lv.label(bg)
    disp.set_align(lv.ALIGN_BOTTOM_MID)
    # disp.set_style_bg_opa(lv.OPA_TRANSP, lv.PART_MAIN | lv.STATE_DEFAULT)
    disp.set_style_text_color(lv.color(0xFFFFFF), 0)    # lv.PART_MAIN | lv.STATE_DEFAULT
    disp.set_text(driver_name)
  end

  # force full refresh now and not at next tick
  lv.refr_now(0)

  lv._splash = bg                          # keep a reference to the splash screen

  tasmota.set_timer(5000, lv.splash_remove)    # delete the object in the future
end
lv_tasmota.splash = splash

#- ------------------------------------------------------------ -#
# `lv.touch_pointer(enable:bool [, color:int|lv.color [, size:int]]) -> bool`
#
# Show a dot under the finger while the touch screen is pressed,
# useful for demos and screen recordings. Default is red, 30 px.
#
# The dot is the LVGL cursor of the first pointer input device: LVGL
# moves it at each touch read, on `layer_sys` above all screens, and it
# is not clickable. LVGL can't detach a cursor, so the object is created
# once and only hidden when disabled. A driver shows it while pressed.
#
# Returns `false` if LVGL is not started or there is no pointer device.
#- ------------------------------------------------------------ -#
#@ solidify:lv_tasmota.touch_pointer,weak
def touch_pointer(enable, color, sz)
  class touch_pointer_driver
    var indev, obj, enabled, shown

    def init(indev, obj)
      self.indev = indev
      self.obj = obj
      self.enabled = true
      self.shown = false
      tasmota.add_driver(self)
    end

    def every_50ms()
      var pressed = self.enabled && (self.indev.get_state() == lv.INDEV_STATE_PRESSED)
      if pressed != self.shown          # only touch the object on change
        self.obj.set_hidden(!pressed)
        self.shown = pressed
      end
    end
  end

  import introspect
  var tp = lv._touch_pointer
  if tp == nil
    if !enable return false end
    var null_ptr = introspect.toptr(0)
    # find the first pointer input device
    var indev = lv.indev(nil).get_next()
    while (indev._p != null_ptr) && (indev.get_type() != lv.INDEV_TYPE_POINTER)
      indev = indev.get_next()
    end
    if indev._p == null_ptr return false end

    var obj = lv.obj(lv.layer_sys())
    obj.set_style_radius(lv.RADIUS_CIRCLE, 0)
    obj.set_style_border_width(0, 0)
    obj.set_style_shadow_width(0, 0)
    obj.set_style_pad_all(0, 0)
    obj.set_style_bg_opa(200, 0)
    obj.set_scrollable(false)
    obj.set_hidden(true)
    indev.set_cursor(obj)              # also makes it non clickable
    tp = touch_pointer_driver(indev, obj)
    lv._touch_pointer = tp
    if color == nil  color = 0xFF0000  end
    if sz == nil  sz = 30  end
  end

  tp.enabled = bool(enable)
  if color != nil
    if type(color) == 'int'  color = lv.color(color)  end
    tp.obj.set_style_bg_color(color, 0)
  end
  if sz != nil
    sz = int(sz)
    tp.obj.set_size(sz, sz)
    # LVGL places the top-left corner at the touch point, center it instead
    tp.obj.set_style_translate_x(-sz / 2, 0)
    tp.obj.set_style_translate_y(-sz / 2, 0)
  end
  return true
end
lv_tasmota.touch_pointer = touch_pointer

return lv_tasmota
