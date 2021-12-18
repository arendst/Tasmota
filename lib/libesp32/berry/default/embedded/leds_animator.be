# class Leds_animator

class Leds_animator
  var strip         # neopixelbus object
  var pixel_count   # number of pixels in the strip
  var bri           # brightness of the animation, 0..100, default 50
  var running       # is the animation running
  var animators     # animators list

  def init(strip)
    self.strip = strip
    self.bri = 50      # percentage of brightness 0..100
    self.running = false
    self.pixel_count = strip.pixel_count()
    self.animators = []
    #
    self.clear()        # clear all leds first
    #
    tasmota.add_driver(self)
  end

  def add_anim(anim)
    self.animators.push(anim)
    anim.run()      # start the animator
  end

  def clear()
    self.stop()
    self.strip.clear()
  end
  def start()
    self.running = true
  end
  def stop()
    self.running = false
  end

  def set_bri(bri)
    self.bri = bri
  end
  def get_bri(bri)
    return self.bri
  end

  def every_50ms()
    if self.running
      # run animators first
      var i = 0
      while i < size(self.animators)
        var anim = self.animators[i]
        if anim.is_running()
          anim.animate()
          i += 1
        else
          self.animators.remove(i)    # remove any finished animator
        end
      end
      # tirgger animate and display
      self.animate()
    end
  end

  def animate()
    # placeholder - do nothing by default
  end

  def remove()
    tasmota.remove_driver(self)
  end
end
