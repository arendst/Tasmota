# GradientMeterAnimation - VU meter style animation with palette gradient colors
#
# Displays a gradient-colored bar from the start of the strip up to a level (0-255).
# Includes optional peak hold indicator that shows the maximum level for a configurable time.
#
# Visual representation:
#   level=128 (50%), peak at 200
#   [████████████████--------•-------]
#   ^                        ^
#   |                        peak indicator (single pixel)
#   filled gradient area

import "./core/param_encoder" as encode_constraints

#@ solidify:GradientMeterAnimation,weak
class GradientMeterAnimation : animation.palette_gradient_animation
  # Instance variables for peak tracking
  var peak_level        # Current peak level (0-255)
  var peak_time         # Time when peak was set (ms)
  var _level            # Cached value for 'self.level'

  # Parameter definitions - extends PaletteGradientAnimation params
  static var PARAMS = animation.enc_params({
    # Inherited from PaletteGradientAnimation: color_source, shift_period, spatial_period, phase_shift
    # New meter-specific parameters
    "level": {"min": 0, "max": 255, "default": 255},
    "peak_hold": {"min": 0, "default": 1000}  # 0 = disabled, >0 = hold time in ms
  })

  # Initialize a new GradientMeterAnimation
  def init(engine)
    super(self).init(engine)

    # Initialize peak tracking
    self.peak_level = 0
    self.peak_time = 0
    self._level = 0

    # Override gradient defaults for meter use - static gradient
    self.shift_period = 0
  end

  # Override update to handle peak tracking with absolute time
  def update(time_ms)
    var peak_hold = self.peak_hold

    if peak_hold > 0
      var level = self.level
      self._level = level     # cache value to be used in 'render()'
      var peak_level = self.peak_level
      # Update peak tracking using absolute time
      if level >= peak_level
        # New peak detected, or rearm current peak
        self.peak_level = level
        self.peak_time = time_ms
      elif peak_level > 0
        # Check if peak hold has expired
        var elapsed_since_peak = time_ms - self.peak_time
        if elapsed_since_peak > peak_hold
          # Peak hold expired, reset to current level
          self.peak_level = level
          self.peak_time = time_ms
        end
      end
    end

    # Call parent update (computes value_buffer with gradient values)
    super(self).update(time_ms)
  end

  # Override render to only display filled pixels and peak indicator
  def render(frame, time_ms, strip_length)
    var color_source = self.get_param('color_source')
    if color_source == nil
      return false
    end

    var elapsed = time_ms - self.start_time
    var level = self._level           # use cached value in 'update()'
    var peak_hold = self.peak_hold

    # Calculate fill position (how many pixels to fill)
    var fill_pixels = tasmota.scale_uint(level, 0, 255, 0, strip_length)

    # Calculate peak pixel position
    var peak_pixel = -1
    if peak_hold > 0 && self.peak_level > level
      peak_pixel = tasmota.scale_uint(self.peak_level, 0, 255, 0, strip_length) - 1
    end


    # Optimization for LUT patterns
    var lut
    if isinstance(color_source, animation.color_provider) && (lut := color_source.get_lut()) != nil
      var lut_factor = color_source.LUT_FACTOR    # default = 1, we have only 128 cached values
      var lut_max = 256 >> lut_factor
      var i = 0
      var frame_ptr = frame.pixels._buffer()
      var lut_ptr = lut._buffer()
      var buffer = self.value_buffer._buffer()
      while (i < fill_pixels)
        var byte_value = buffer[i]
        var lut_index = byte_value >> lut_factor  # Divide by 2 using bit shift
        if byte_value == 255
          lut_index = lut_max
        end

        var lut_color_ptr = lut_ptr + (lut_index << 2)  # calculate the pointer for LUT color
        frame_ptr[0] = lut_color_ptr[0]
        frame_ptr[1] = lut_color_ptr[1]
        frame_ptr[2] = lut_color_ptr[2]
        frame_ptr[3] = lut_color_ptr[3]

        # advance to next
        i += 1
        frame_ptr += 4
      end
    else
      # Render only filled pixels and peak indicator (leave rest transparent)
      var i = 0
      while i < fill_pixels
        var byte_value = self.value_buffer[i]
        var color = color_source.get_color_for_value(byte_value, elapsed)
        frame.set_pixel_color(i, color)
        # Unfilled pixels stay transparent (not rendered)
        i += 1
      end
    end

    # Do we need to show peak pixel?
    if peak_pixel >= fill_pixels
      var byte_value = self.value_buffer[peak_pixel]
      var color = color_source.get_color_for_value(byte_value, elapsed)
      frame.set_pixel_color(peak_pixel, color)
    end

    return true
  end

  # String representation
  def tostring()
    var level = self.level
    var peak_hold = self.peak_hold
    return f"GradientMeterAnimation(level={level}, peak_hold={peak_hold}ms, peak={self.peak_level})"
  end
end

return {'palette_meter_animation': GradientMeterAnimation}
