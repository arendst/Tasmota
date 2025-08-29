# FrameBuffer class for Berry Animation Framework
#
# This class provides a buffer for storing and manipulating pixel data
# for LED animations. It uses a bytes object for efficient storage and
# provides methods for pixel manipulation.
#
# Each pixel is stored as a 32-bit value (ARGB format - 0xAARRGGBB):
# - 8 bits for Alpha (0-255, where 0 is fully transparent and 255 is fully opaque)
# - 8 bits for Red (0-255)
# - 8 bits for Green (0-255)
# - 8 bits for Blue (0-255)
#
# The class is optimized for performance and minimal memory usage.

class FrameBuffer
  # Blend modes
  # Currently only normal blending is implemented, but the structure allows for adding more modes later
  static BLEND_MODE_NORMAL = 0     # Normal alpha blending
  # Other blend modes can be added here in the future if needed

  var pixels          # Pixel data (bytes object)
  var width           # Number of pixels
  
  # Initialize a new frame buffer with the specified width
  # Takes either an int (width) or an instance of FrameBuffer (instance)
  def init(width_or_buffer)
    if type(width_or_buffer) == 'int'
      var width = width_or_buffer
      if width <= 0
        raise "value_error", "width must be positive"
      end
      
      self.width = width
      # Each pixel uses 4 bytes (ARGB), so allocate width * 4 bytes
      # Initialize with zeros to ensure correct size
      var buffer = bytes(width * 4)
      buffer.resize(width * 4)
      self.pixels = buffer
      self.clear()  # Initialize all pixels to transparent black
    elif type(width_or_buffer) == 'instance'
      self.width = width_or_buffer.width
      self.pixels = width_or_buffer.pixels.copy()
    else
      raise "value_error", "argument must be either int or instance"
    end
  end
  
  # Get the pixel color at the specified index
  # Returns the pixel value as a 32-bit integer (ARGB format - 0xAARRGGBB)
  def get_pixel_color(index)
    if index < 0 || index >= self.width
      raise "index_error", "pixel index out of range"
    end
    
    # Each pixel is 4 bytes, so the offset is index * 4
    return self.pixels.get(index * 4, 4)
  end
  
  # Set the pixel at the specified index with a 32-bit color value
  # color: 32-bit color value in ARGB format (0xAARRGGBB)
  def set_pixel_color(index, color)
    if index < 0 || index >= self.width
      raise "index_error", "pixel index out of range"
    end
    
    # Set the pixel in the buffer
    self.pixels.set(index * 4, color, 4)
  end

  # Clear the frame buffer (set all pixels to transparent black)
  def clear()
    self.pixels.clear()     # clear buffer
    self.pixels.resize(self.width * 4)  # resize to full size filled with transparent black (all zeroes)
  end
  
  # Resize the frame buffer to a new width
  # This is more efficient than creating a new frame buffer object
  def resize(new_width)
    if new_width <= 0
      raise "value_error", "width must be positive"
    end
    
    if new_width == self.width
      return  # No change needed
    end
    
    self.width = new_width
    # Resize the underlying bytes buffer
    self.pixels.resize(self.width * 4)
    # Clear to ensure all new pixels are transparent black
    self.clear()
  end
  
  # Convert separate a, r, g, b components to a 32-bit color value
  # r: red component (0-255)
  # g: green component (0-255)
  # b: blue component (0-255)
  # a: alpha component (0-255, default 255 = fully opaque)
  # Returns: 32-bit color value in ARGB format (0xAARRGGBB)
  static def to_color(r, g, b, a)
    # Default alpha to fully opaque if not specified
    if a == nil
      a = 255
    end
    
    # Ensure values are in valid range
    r = r & 0xFF
    g = g & 0xFF
    b = b & 0xFF
    a = a & 0xFF
    
    # Combine components into a 32-bit value (ARGB format - 0xAARRGGBB)
    return (a << 24) | (r << 16) | (g << 8) | b
  end
  

  
  # Fill the frame buffer with a specific color using a bytes object
  # This is an optimization for filling with a pre-computed color
  def fill_pixels(color)
    var i = 0
    while i < self.width
      self.pixels.set(i * 4, color, 4)
      i += 1
    end
  end
  
  # Blend two colors using their alpha channels and blend mode
  # Returns the blended color as a 32-bit integer (ARGB format - 0xAARRGGBB)
  # color1: destination color (ARGB format - 0xAARRGGBB)
  # color2: source color (ARGB format - 0xAARRGGBB)
  # mode: blending mode (default: BLEND_MODE_NORMAL)
  def blend(color1, color2, mode)
    # Default blend mode to normal if not specified
    if mode == nil
      mode = self.BLEND_MODE_NORMAL
    end
    
    # Extract components from color1 (ARGB format - 0xAARRGGBB)
    var a1 = (color1 >> 24) & 0xFF
    var r1 = (color1 >> 16) & 0xFF
    var g1 = (color1 >> 8) & 0xFF
    var b1 = color1 & 0xFF
    
    # Extract components from color2 (ARGB format - 0xAARRGGBB)
    var a2 = (color2 >> 24) & 0xFF
    var r2 = (color2 >> 16) & 0xFF
    var g2 = (color2 >> 8) & 0xFF
    var b2 = color2 & 0xFF
    
    # Fast path for common cases
    if a2 == 0
      # Source is fully transparent, no blending needed
      return color1
    end
    
    # Use the source alpha directly for blending
    var effective_opacity = a2
    
    # Normal alpha blending (currently the only supported mode)
    # Use tasmota.scale_uint for ratio conversion instead of integer arithmetic
    var r = tasmota.scale_uint(255 - effective_opacity, 0, 255, 0, r1) + tasmota.scale_uint(effective_opacity, 0, 255, 0, r2)
    var g = tasmota.scale_uint(255 - effective_opacity, 0, 255, 0, g1) + tasmota.scale_uint(effective_opacity, 0, 255, 0, g2)
    var b = tasmota.scale_uint(255 - effective_opacity, 0, 255, 0, b1) + tasmota.scale_uint(effective_opacity, 0, 255, 0, b2)
    
    # More accurate alpha blending using tasmota.scale_uint
    var a = a1 + tasmota.scale_uint((255 - a1) * a2, 0, 255 * 255, 0, 255)
    
    # Ensure values are in valid range
    r = r < 0 ? 0 : (r > 255 ? 255 : r)
    g = g < 0 ? 0 : (g > 255 ? 255 : g)
    b = b < 0 ? 0 : (b > 255 ? 255 : b)
    a = a < 0 ? 0 : (a > 255 ? 255 : a)
    
    # Combine components into a 32-bit value (ARGB format - 0xAARRGGBB)
    return (int(a) << 24) | (int(r) << 16) | (int(g) << 8) | int(b)
  end

  # Linear interpolation between two colors using explicit blend factor
  # Returns the blended color as a 32-bit integer (ARGB format - 0xAARRGGBB)
  # 
  # This function matches the original berry_animate frame.blend(color1, color2, blend_factor) behavior
  # Used for creating smooth gradients like beacon slew regions
  #
  # color1: destination/background color (ARGB format - 0xAARRGGBB)
  # color2: source/foreground color (ARGB format - 0xAARRGGBB)
  # blend_factor: blend factor (0-255 integer)
  #   - 0 = full color2 (foreground)
  #   - 255 = full color1 (background)
  def blend_linear(color1, color2, blend_factor)
    # Extract components from color1 (background/destination)
    var back_a = (color1 >> 24) & 0xFF
    var back_r = (color1 >> 16) & 0xFF
    var back_g = (color1 >> 8) & 0xFF
    var back_b = color1 & 0xFF
    
    # Extract components from color2 (foreground/source)
    var fore_a = (color2 >> 24) & 0xFF
    var fore_r = (color2 >> 16) & 0xFF
    var fore_g = (color2 >> 8) & 0xFF
    var fore_b = color2 & 0xFF
    
    # Linear interpolation: result = fore + (back - fore) * blend_factor / 255
    var result_a = fore_a + (back_a - fore_a) * blend_factor / 255
    var result_r = fore_r + (back_r - fore_r) * blend_factor / 255
    var result_g = fore_g + (back_g - fore_g) * blend_factor / 255
    var result_b = fore_b + (back_b - fore_b) * blend_factor / 255
    
    # Ensure values are in valid range
    result_a = result_a < 0 ? 0 : (result_a > 255 ? 255 : result_a)
    result_r = result_r < 0 ? 0 : (result_r > 255 ? 255 : result_r)
    result_g = result_g < 0 ? 0 : (result_g > 255 ? 255 : result_g)
    result_b = result_b < 0 ? 0 : (result_b > 255 ? 255 : result_b)
    
    # Combine components into a 32-bit value (ARGB format)
    return (int(result_a) << 24) | (int(result_r) << 16) | (int(result_g) << 8) | int(result_b)
  end
  
  # Blend this frame buffer with another frame buffer using per-pixel alpha
  # other_buffer: the other frame buffer to blend with
  # mode: blending mode (default: BLEND_MODE_NORMAL)
  # region_start: start index for blending (default: 0)
  # region_end: end index for blending (default: width-1)
  def blend_pixels(other_buffer, mode, region_start, region_end)
    # Default parameters
    
    if mode == nil
      mode = self.BLEND_MODE_NORMAL
    end
    
    if region_start == nil
      region_start = 0
    end
    
    if region_end == nil
      region_end = self.width - 1
    end
    
    # Validate parameters
    if self.width != other_buffer.width
      raise "value_error", "frame buffers must have the same width"
    end
    
    if region_start < 0 || region_start >= self.width
      raise "index_error", "region_start out of range"
    end
    
    if region_end < region_start || region_end >= self.width
      raise "index_error", "region_end out of range"
    end
    
    # Performance optimization: batch processing for normal blend mode
    if mode == self.BLEND_MODE_NORMAL
      # Fast path for normal blending
      var i = region_start
      while i <= region_end
        var color2 = other_buffer.get_pixel_color(i)
        var a2 = (color2 >> 24) & 0xFF
        
        # Only blend if the source pixel has some alpha
        if a2 > 0
          if a2 == 255
            # Fully opaque source pixel, just copy it
            self.pixels.set(i * 4, color2, 4)
          else
            # Partially transparent source pixel, need to blend
            var color1 = self.get_pixel_color(i)
            var blended = self.blend(color1, color2, mode)
            self.pixels.set(i * 4, blended, 4)
          end
        end
        
        i += 1
      end
      return
    end
    
    # General case: blend each pixel using the blend function
    var i = region_start
    while i <= region_end
      var color1 = self.get_pixel_color(i)
      var color2 = other_buffer.get_pixel_color(i)
      
      # Only blend if the source pixel has some alpha
      var a2 = (color2 >> 24) & 0xFF
      if a2 > 0
        var blended = self.blend(color1, color2, mode)
        self.pixels.set(i * 4, blended, 4)
      end
      
      i += 1
    end
  end
  
  # Convert the frame buffer to a hexadecimal string (for debugging)
  def tohex()
    return self.pixels.tohex()
  end
  
  # Support for array-like access using []
  def item(i)
    return self.get_pixel_color(i)
  end
  
  # Support for array-like assignment using []=
  def setitem(i, v)
    # Use the set_pixel_color method directly with the 32-bit value
    self.set_pixel_color(i, v)
  end
  
  # Create a gradient fill in the frame buffer
  # color1: start color (ARGB format - 0xAARRGGBB)
  # color2: end color (ARGB format - 0xAARRGGBB)
  # start_pos: start position (default: 0)
  # end_pos: end position (default: width-1)
  def gradient_fill(color1, color2, start_pos, end_pos)
    if start_pos == nil
      start_pos = 0
    end
    
    if end_pos == nil
      end_pos = self.width - 1
    end
    
    # Validate parameters
    if start_pos < 0 || start_pos >= self.width
      raise "index_error", "start_pos out of range"
    end
    
    if end_pos < start_pos || end_pos >= self.width
      raise "index_error", "end_pos out of range"
    end
    
    # Set first pixel directly
    self.set_pixel_color(start_pos, color1)
    
    # If only one pixel, we're done
    if start_pos == end_pos
      return
    end
    
    # Set last pixel directly
    self.set_pixel_color(end_pos, color2)
    
    # If only two pixels, we're done
    if end_pos - start_pos <= 1
      return
    end
    
    # Extract components from color1 (ARGB format - 0xAARRGGBB)
    var a1 = (color1 >> 24) & 0xFF
    var r1 = (color1 >> 16) & 0xFF
    var g1 = (color1 >> 8) & 0xFF
    var b1 = color1 & 0xFF
    
    # Extract components from color2 (ARGB format - 0xAARRGGBB)
    var a2 = (color2 >> 24) & 0xFF
    var r2 = (color2 >> 16) & 0xFF
    var g2 = (color2 >> 8) & 0xFF
    var b2 = color2 & 0xFF
    
    # Calculate the total number of steps
    var steps = end_pos - start_pos
    
    # Fill the gradient for intermediate pixels
    var i = start_pos + 1
    while (i < end_pos)
      var pos = i - start_pos
      
      # Use tasmota.scale_uint for ratio conversion instead of floating point arithmetic
      var r = tasmota.scale_uint(pos, 0, steps, r1, r2)
      var g = tasmota.scale_uint(pos, 0, steps, g1, g2)
      var b = tasmota.scale_uint(pos, 0, steps, b1, b2)
      var a = tasmota.scale_uint(pos, 0, steps, a1, a2)
      
      # Ensure values are in valid range
      r = r < 0 ? 0 : (r > 255 ? 255 : r)
      g = g < 0 ? 0 : (g > 255 ? 255 : g)
      b = b < 0 ? 0 : (b > 255 ? 255 : b)
      a = a < 0 ? 0 : (a > 255 ? 255 : a)
      
      # Combine components into a 32-bit value (ARGB format - 0xAARRGGBB)
      var color = (a << 24) | (r << 16) | (g << 8) | b
      self.set_pixel_color(i, color)
      i += 1
    end
  end
  
  # Apply a mask to this frame buffer
  # mask_buffer: the mask frame buffer (alpha channel is used as mask)
  # invert: if true, invert the mask (default: false)
  def apply_mask(mask_buffer, invert)
    if invert == nil
      invert = false
    end
    
    if self.width != mask_buffer.width
      raise "value_error", "frame buffers must have the same width"
    end
    
    var i = 0
    while i < self.width
      var color = self.get_pixel_color(i)
      var mask_color = mask_buffer.get_pixel_color(i)
      
      # Extract alpha from mask (0-255)
      var mask_alpha = (mask_color >> 24) & 0xFF
      
      # Invert mask if requested
      if invert
        mask_alpha = 255 - mask_alpha
      end
      
      # Extract components from color (ARGB format - 0xAARRGGBB)
      var a = (color >> 24) & 0xFF
      var r = (color >> 16) & 0xFF
      var g = (color >> 8) & 0xFF
      var b = color & 0xFF
      
      # Apply mask to alpha channel using tasmota.scale_uint
      a = tasmota.scale_uint(mask_alpha, 0, 255, 0, a)
      
      # Combine components into a 32-bit value (ARGB format - 0xAARRGGBB)
      var new_color = (a << 24) | (r << 16) | (g << 8) | b
      
      # Update the pixel
      self.set_pixel_color(i, new_color)
      
      i += 1
    end
  end
  
  # Create a copy of this frame buffer
  def copy()
    return animation.frame_buffer(self)   # return using the self copying constructor
  end
  
  # Blend a specific region with a solid color using the color's alpha channel
  # color: the color to blend (ARGB)
  # mode: blending mode (default: BLEND_MODE_NORMAL)
  # start_pos: start position (default: 0)
  # end_pos: end position (default: width-1)
  def blend_color(color, mode, start_pos, end_pos)
    
    if mode == nil
      mode = self.BLEND_MODE_NORMAL
    end
    
    if start_pos == nil
      start_pos = 0
    end
    
    if end_pos == nil
      end_pos = self.width - 1
    end
    
    # Validate parameters
    if start_pos < 0 || start_pos >= self.width
      raise "index_error", "start_pos out of range"
    end
    
    if end_pos < start_pos || end_pos >= self.width
      raise "index_error", "end_pos out of range"
    end
    
    # Extract components from color (ARGB format - 0xAARRGGBB)
    var a2 = (color >> 24) & 0xFF
    var r2 = (color >> 16) & 0xFF
    var g2 = (color >> 8) & 0xFF
    var b2 = color & 0xFF
    
    # Blend the pixels in the specified region
    var i = start_pos
    while i <= end_pos
      var color1 = self.get_pixel_color(i)
      
      # Only blend if the color has some alpha
      if a2 > 0
        var blended = self.blend(color1, color, mode)
        self.pixels.set(i * 4, blended, 4)
      end
      
      i += 1
    end
  end
  
  # Apply an opacity adjustment to the frame buffer
  # opacity: opacity factor (0-511) or another FrameBuffer to use as mask
  #   - Number: 0 is fully transparent, 255 is original, 511 is maximum opaque
  #   - FrameBuffer: uses alpha channel as opacity mask
  def apply_opacity(opacity)
    if opacity == nil
      opacity = 255
    end
    
    # Check if opacity is a FrameBuffer (mask mode)
    if isinstance(opacity, animation.frame_buffer)
      # Mask mode: use another frame buffer as opacity mask
      var mask_buffer = opacity
      
      if self.width != mask_buffer.width
        raise "value_error", "frame buffers must have the same width"
      end
      
      var i = 0
      while i < self.width
        var color = self.get_pixel_color(i)
        var mask_color = mask_buffer.get_pixel_color(i)
        
        # Extract alpha from mask as opacity factor (0-255)
        var mask_opacity = (mask_color >> 24) & 0xFF
        
        # Extract components from color (ARGB format - 0xAARRGGBB)
        var a = (color >> 24) & 0xFF
        var r = (color >> 16) & 0xFF
        var g = (color >> 8) & 0xFF
        var b = color & 0xFF
        
        # Apply mask opacity to alpha channel using tasmota.scale_uint
        a = tasmota.scale_uint(mask_opacity, 0, 255, 0, a)
        
        # Combine components into a 32-bit value (ARGB format - 0xAARRGGBB)
        var new_color = (a << 24) | (r << 16) | (g << 8) | b
        
        # Update the pixel
        self.set_pixel_color(i, new_color)
        
        i += 1
      end
    else
      # Number mode: uniform opacity adjustment
      var opacity_value = int(opacity)
      
      # Ensure opacity is in valid range (0-511)
      opacity_value = opacity_value < 0 ? 0 : (opacity_value > 511 ? 511 : opacity_value)
      
      # Apply opacity adjustment
      var i = 0
      while i < self.width
        var color = self.get_pixel_color(i)
        
        # Extract components (ARGB format - 0xAARRGGBB)
        var a = (color >> 24) & 0xFF
        var r = (color >> 16) & 0xFF
        var g = (color >> 8) & 0xFF
        var b = color & 0xFF
        
        # Adjust alpha using tasmota.scale_uint
        # For opacity 0-255: scale down alpha
        # For opacity 256-511: scale up alpha (but cap at 255)
        if opacity_value <= 255
          a = tasmota.scale_uint(opacity_value, 0, 255, 0, a)
        else
          # Scale up alpha: map 256-511 to 1.0-2.0 multiplier
          a = tasmota.scale_uint(a * opacity_value, 0, 255 * 255, 0, 255)
          a = a > 255 ? 255 : a  # Cap at maximum alpha
        end
        
        # Combine components into a 32-bit value (ARGB format - 0xAARRGGBB)
        color = (a << 24) | (r << 16) | (g << 8) | b
        
        # Update the pixel
        self.set_pixel_color(i, color)
        
        i += 1
      end
    end
  end

  # Apply a brightness adjustment to the frame buffer
  # brightness: brightness factor (0-511) or another FrameBuffer to use as mask
  #   - Number: 0 is black, 255 is original, 511 is maximum bright
  #   - FrameBuffer: uses alpha channel as brightness mask
  # start_pos: start position (default: 0)
  # end_pos: end position (default: width-1)
  def apply_brightness(brightness, start_pos, end_pos)
    if brightness == nil
      brightness = 255
    end
    
    if start_pos == nil
      start_pos = 0
    end
    
    if end_pos == nil
      end_pos = self.width - 1
    end
    
    # Validate parameters
    if start_pos < 0 || start_pos >= self.width
      raise "index_error", "start_pos out of range"
    end
    
    if end_pos < start_pos || end_pos >= self.width
      raise "index_error", "end_pos out of range"
    end
    
    # Check if brightness is a FrameBuffer (mask mode)
    if isinstance(brightness, animation.frame_buffer)
      # Mask mode: use another frame buffer as brightness mask
      var mask_buffer = brightness
      
      if self.width != mask_buffer.width
        raise "value_error", "frame buffers must have the same width"
      end
      
      var i = start_pos
      while i <= end_pos
        var color = self.get_pixel_color(i)
        var mask_color = mask_buffer.get_pixel_color(i)
        
        # Extract alpha from mask as brightness factor (0-255)
        var mask_brightness = (mask_color >> 24) & 0xFF
        
        # Extract components from color (ARGB format - 0xAARRGGBB)
        var a = (color >> 24) & 0xFF
        var r = (color >> 16) & 0xFF
        var g = (color >> 8) & 0xFF
        var b = color & 0xFF
        
        # Apply mask brightness to RGB channels using tasmota.scale_uint
        r = tasmota.scale_uint(mask_brightness, 0, 255, 0, r)
        g = tasmota.scale_uint(mask_brightness, 0, 255, 0, g)
        b = tasmota.scale_uint(mask_brightness, 0, 255, 0, b)
        
        # Combine components into a 32-bit value (ARGB format - 0xAARRGGBB)
        var new_color = (a << 24) | (r << 16) | (g << 8) | b
        
        # Update the pixel
        self.set_pixel_color(i, new_color)
        
        i += 1
      end
    else
      # Number mode: uniform brightness adjustment
      var brightness_value = int(brightness)
      
      # Ensure brightness is in valid range (0-511)
      brightness_value = brightness_value < 0 ? 0 : (brightness_value > 511 ? 511 : brightness_value)
      
      # Apply brightness adjustment
      var i = start_pos
      while i <= end_pos
        var color = self.get_pixel_color(i)
        
        # Extract components (ARGB format - 0xAARRGGBB)
        var a = (color >> 24) & 0xFF
        var r = (color >> 16) & 0xFF
        var g = (color >> 8) & 0xFF
        var b = color & 0xFF
        
        # Adjust brightness using tasmota.scale_uint
        # For brightness 0-255: scale down RGB
        # For brightness 256-511: scale up RGB (but cap at 255)
        if brightness_value <= 255
          r = tasmota.scale_uint(r, 0, 255, 0, brightness_value)
          g = tasmota.scale_uint(g, 0, 255, 0, brightness_value)
          b = tasmota.scale_uint(b, 0, 255, 0, brightness_value)
        else
          # Scale up RGB: map 256-511 to 1.0-2.0 multiplier
          var multiplier = brightness_value - 255  # 0-256 range
          r = r + tasmota.scale_uint(r * multiplier, 0, 255 * 256, 0, 255)
          g = g + tasmota.scale_uint(g * multiplier, 0, 255 * 256, 0, 255)
          b = b + tasmota.scale_uint(b * multiplier, 0, 255 * 256, 0, 255)
          r = r > 255 ? 255 : r  # Cap at maximum
          g = g > 255 ? 255 : g  # Cap at maximum
          b = b > 255 ? 255 : b  # Cap at maximum
        end
        
        # Combine components into a 32-bit value (ARGB format - 0xAARRGGBB)
        color = (a << 24) | (r << 16) | (g << 8) | b
        
        # Update the pixel
        self.set_pixel_color(i, color)
        
        i += 1
      end
    end
  end

  # String representation of the frame buffer
  def tostring()
    return f"FrameBuffer(width={self.width}, pixels={self.pixels})"
  end

  # Dump the pixels into AARRGGBB string separated with '|'
  def dump()
    var s = ""
    var i = 0
    while i < self.width
      var color = self.get_pixel_color(i)

      # Extract components from color (ARGB format - 0xAARRGGBB)
      var a = (color >> 24) & 0xFF
      var r = (color >> 16) & 0xFF
      var g = (color >> 8) & 0xFF
      var b = color & 0xFF

      s += f"{a:02X}{r:02X}{g:02X}{b:02X}|"
      i += 1
    end
    s = s[0..-2]  # remove last character
    return s
  end
end

return {'frame_buffer': FrameBuffer}