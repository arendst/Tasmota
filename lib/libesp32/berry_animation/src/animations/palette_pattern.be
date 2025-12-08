# PaletteGradient animation effect for Berry Animation Framework
#
# This animation creates gradient patterns with palette colors.
# It supports shifting gradients, spatial periods, and phase shifts.

import "./core/param_encoder" as encode_constraints

# Gradient pattern animation - creates shifting gradient patterns
#@ solidify:PaletteGradientAnimation,weak
class PaletteGradientAnimation : animation.animation
  var value_buffer     # Buffer to store values for each pixel (bytes object)
  var _spatial_period  # Cached spatial_period for static pattern optimization
  var _phase_shift     # Cached phase_shift for static pattern optimization
  
  # Static definitions of parameters with constraints
  static var PARAMS = animation.enc_params({
    # Gradient-specific parameters
    "color_source": {"default": nil, "type": "instance"},
    "shift_period": {"min": 0, "default": 0},           # Time for one complete shift cycle in ms (0 = static)
    "spatial_period": {"min": 0, "default": 0},         # Spatial period in pixels (0 = full strip)
    "phase_shift": {"min": 0, "max": 255, "default": 0} # Phase shift in 0-255 range
  })
  
  # Initialize a new gradient pattern animation
  #
  # @param engine: AnimationEngine - Required animation engine reference
  def init(engine)
    # Call parent constructor with engine
    super(self).init(engine)
    
    # Initialize non-parameter instance variables only
    self.value_buffer = bytes()
    
    # Initialize value buffer with default frame width
    self._initialize_value_buffer()
  end
  
  # Initialize the value buffer based on current strip length
  def _initialize_value_buffer()
    var strip_length = self.engine.strip_length
    self.value_buffer.resize(strip_length)
    
    # Initialize with zeros
    var i = 0
    while i < strip_length
      self.value_buffer[i] = 0
      i += 1
    end
  end
  
  # Update the value buffer to generate gradient pattern
  def _update_value_buffer(time_ms, strip_length)
    # Cache parameter values for performance
    var shift_period = self.member("shift_period")
    var spatial_period = self.member("spatial_period")
    var phase_shift = self.member("phase_shift")
    
    # Optimization: for static patterns (shift_period == 0), skip recomputation
    # if spatial_period, phase_shift, and strip_length haven't changed
    if shift_period == 0
      if self._spatial_period != nil &&
         self._spatial_period == spatial_period &&
         self._phase_shift == phase_shift &&
         size(self.value_buffer) == strip_length
        return  # No changes, skip recomputation
      end
      # Update cached values
      self._spatial_period = spatial_period
      self._phase_shift = phase_shift
    end
    
    # Determine effective spatial period (0 means full strip)
    var effective_spatial_period = spatial_period > 0 ? spatial_period : strip_length
    
    # Calculate the temporal shift position (how much the pattern has moved over time)
    var temporal_offset = 0
    if shift_period > 0
      temporal_offset = tasmota.scale_uint(time_ms % shift_period, 0, shift_period, 0, effective_spatial_period)
    end
    
    # Calculate the phase shift offset in pixels
    var phase_offset = tasmota.scale_uint(phase_shift, 0, 255, 0, effective_spatial_period)
    
    # Calculate values for each pixel
    var i = 0
    # Calculate position within the spatial period, including temporal and phase offsets
    var spatial_pos = (temporal_offset + phase_offset) % effective_spatial_period

    # Calculate the increment per pixel, in 1/1024 of pixels
    # We calculate 1024*255/effective_spatial_period
    # But for rounding we actually calculate
    # ((1024 * 255 * 2) + 1) / (2 * effective_spatial_period)
    # Note: (1024 * 255 * 2) + 1 = 522241
    var incr_1024 = (522241 / effective_spatial_period) >> 1

    # 'spatial_1024' is our accumulator in 1/1024th of pixels, 2^10
    var spatial_1024 = spatial_pos * incr_1024
    var buffer = self.value_buffer._buffer()    # 'buffer' is of type 'comptr'

    # var effective_spatial_period_1 = effective_spatial_period - 1
    # # Calculate the increment in 1/256 of values
    # var increment = tasmota.scale_uint(effective_spatial_period)
    while i < strip_length
      buffer[i] = spatial_1024 >> 10
      spatial_1024 += incr_1024     # we don't really care about overflow since we clamp modula 255 anyways
      i += 1
    end
  end
  
  # Update animation state based on current time
  #
  # @param time_ms: int - Current time in milliseconds
  def update(time_ms)
    # Calculate elapsed time since animation started
    var elapsed = time_ms - self.start_time
    
    var strip_length = self.engine.strip_length

    # Resize buffer if strip length changed
    if size(self.value_buffer) != strip_length
      self.value_buffer.resize(strip_length)
    end
    
    # Update the value buffer
    self._update_value_buffer(elapsed, strip_length)
  end
  
  # Render the pattern to the provided frame buffer
  #
  # @param frame: FrameBuffer - The frame buffer to render to
  # @param time_ms: int - Current time in milliseconds
  # @param strip_length: int - Length of the LED strip in pixels
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms, strip_length)
    # Get current parameter values (cached for performance)
    var color_source = self.get_param('color_source')     # use get_param to avoid resolving of color_provider
    if color_source == nil
      return false
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
      while (i < strip_length)
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
    else    # no LUT, do one color at a time
      # Calculate elapsed time since animation started
      var elapsed = time_ms - self.start_time
      var i = 0
      while (i < strip_length)
        var byte_value = self.value_buffer[i]
        
        # Use the color_source to get color for the byte value (0-255)
        var color = color_source.get_color_for_value(byte_value, elapsed)
        
        frame.set_pixel_color(i, color)
        i += 1
      end
    end
    
    return true
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    super(self).on_param_changed(name, value)
    if name == "color_source"
      # Reinitialize value buffer when color source changes
      self._initialize_value_buffer()
    end
  end

  # String representation of the animation
  def tostring()
    var strip_length = self.engine.strip_length
    return f"{classname(self)}(strip_length={strip_length}, priority={self.priority}, running={self.is_running})"
  end
end

return {
  'palette_gradient_animation': PaletteGradientAnimation
}