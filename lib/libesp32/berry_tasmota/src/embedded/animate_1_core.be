# class Animate_core

##########################################################################################
#
# class Animate_core
#
# Simple framework to orchestrate all the animations for a led strip or led matrix
#
# You should pass a Leds or Leds_segment to push pixels at each tick
# This version uses `fast_loop` for up to 5ms animation time (200 Hz)
#
##########################################################################################
#@ solidify:Animate_core,weak
class Animate_core
  var strip         # neopixelbus object
  var pixel_count   # number of pixels in the strip
  var bri           # brightness of the animation, 0..100, default 50
  var running       # is the animation running
  var animators     # animators list
  var painters      # list of objects that paint layers on top of background (with transparency)
  var fast_loop_cb  # closure used for fast_loop
  static var FAST_LOOP_MIN = 20
  var fast_loop_next  # next time-stamp for fast_loop
  # cb for animation
  var obj             # object called at each tick
  var mth             # method of object called at each tick
  # frame ojects
  var frame           # Leds_frame frame object
  var layer           # Leds_frame for layer on top of frame
  # background color management (only for uniform background)
  var back_color      # background color RRGGBB

  def init(strip, bri)
    import animate

    self.strip = strip
    if (bri == nil)   bri = 50    end
    self.bri = bri      # percentage of brightness 0..100
    self.set_strip_bri()
    self.running = false
    self.pixel_count = strip.pixel_count()
    self.animators = []
    self.painters = []
    #
    self.clear()        # clear all leds first
    self.frame = animate.frame(self.pixel_count)
    self.layer = animate.frame(self.pixel_count)
    #
    self.fast_loop_cb = def() self.fast_loop() end
    self.back_color = 0x000000
    #
    self.set_current()
  end

  def set_strip_bri()
    self.strip.set_bri(tasmota.scale_uint(self.bri, 0, 100, 0, 255))
  end

  # set this animate.core as the current animator for configuration
  def set_current()
    global._cur_anim = self       # declare the current animate.core for painters and animators to register
  end

  # cb
  def set_back_color(col)
    self.back_color = col
  end

  def add_background_animator(anim)
    anim.set_cb(self, self.set_back_color)
    self.add_animator(anim)
  end

  def add_animator(anim)
    if self.animators.find(anim) == nil
      self.animators.push(anim)
    end
  end

  # remove a specific animator
  # remove all animators if no parameter or nil
  # silently ignores if the animator can't be found
  def remove_animator(anim)
    var animators = self.animators
    if (anim != nil)
      animators.remove(animators.find(anim))
    else
      animators.clear()
    end
  end

  def remove_painter(p)
    var painters = self.painters
    if (p != nil)
      painters.remove(painters.find(p))
    else
      painters.clear()
    end
  end


  def add_painter(painter)
    if self.painters.find(painter) == nil
      self.painters.push(painter)
    end
  end

  def clear()
    self.stop()
    self.strip.clear()
  end
  def start()
    self.running = true
    var animators = self.animators
    var idx = 0
    while idx < size(animators)
      animators[idx].start()
      idx += 1
    end
    self.fast_loop_next = 0           # run immediately
    tasmota.add_fast_loop(self.fast_loop_cb)
  end

  def stop()
    self.running = false
    var animators = self.animators
    var idx = 0
    while idx < size(animators)
      animators[idx].stop()
      idx += 1
    end
    tasmota.remove_fast_loop(self.fast_loop_cb)
  end

  def set_bri(bri)
    self.bri = bri
    self.set_strip_bri()
  end
  def get_bri(bri)
    return self.bri
  end

  def set_cb(obj, method)
    self.obj = obj
    self.mth = method
  end

  def fast_loop()
    if self.running && tasmota.time_reached(self.fast_loop_next) && self.strip.can_show()
      # prepare environnement
      self.frame.fill_pixels(self.back_color)
      # run animators first
      var i = 0
      var now = tasmota.millis()
      self.fast_loop_next = now + self.FAST_LOOP_MIN
      while i < size(self.animators)
        self.animators[i].animate(now)
        i += 1
        # var anim = self.animators[i]
        # if anim.is_running()
        #   anim.animate(now)
        #   i += 1
        # else
        #   self.animators.remove(i)    # remove any finished animator
        # end
      end
      # apply painters
      i = 0
      var frame = self.frame
      var layer = self.layer
      while i < size(self.painters)
        layer.fill_pixels(0xFF000000)      # fill with transparent color
        if (self.painters[i].paint(layer))
# print(f"frame0 {self.frame.tohex()}")
# print(f"layer0 {self.layer.tohex()}")
          frame.blend_pixels(layer)
# print(f"frame1 {self.frame.tohex()}")
        end
        i += 1
      end
      # tirgger animate and display
      var obj = self.obj
      var mth = self.mth
      if (obj && mth)
        mth(obj)
      end
      self.animate()
      # now display the frame
      self.frame.paste_pixels(self.strip.pixels_buffer(), self.strip.get_bri(), self.strip.get_gamma())
      self.strip.dirty()
      self.strip.show()
    end
  end

  def animate()
    # placeholder - do nothing by default
  end

  def remove()
    self.clear()
    tasmota.remove_fast_loop(self.fast_loop_cb)
  end
end
animate.core = global.Animate_core
