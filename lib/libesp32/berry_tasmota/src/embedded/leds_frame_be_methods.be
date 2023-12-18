#- Native code used for code solidification -#
#- Do not use it -#

tasmota = nil
#@ solidify:Leds_frame_be
class Leds_frame_be

  def init(pixels)
    if (pixels < 0)   pixels = -pixels  end
    self.pixel_size = pixels
    super(self).init(pixels * (-4))
  end

  def item(i)
    return self.get(i * 4, 4)
  end

  def setitem(i, v)
    self.set(i * 4, v, 4)
  end

  def set_pixel(i, r, g, b, alpha)
    if (alpha == nil)   alpha = 0   end
    var color = ((alpha & 0xFF) << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF)
    self.setitem(i, color)
  end

end
