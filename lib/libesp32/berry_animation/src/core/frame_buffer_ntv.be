# FrameBuffeNtv class for Berry Animation Framework
#
# This class provides a place-holder for native implementation of some
# static methods.
#
# Below is a pure Berry implementation for emulator, while it is replaced
# by C++ code in Tasmota devices

class FrameBufferNtv

  # Blend two colors using their alpha channels
  # Returns the blended color as a 32-bit integer (ARGB format - 0xAARRGGBB)
  # color1: destination color (ARGB format - 0xAARRGGBB)
  # color2: source color (ARGB format - 0xAARRGGBB)
  static def blend(color1, color2)
    
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
    
    # Normal alpha blending
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
  static def blend_linear(color1, color2, blend_factor)
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
    
    # Linear interpolation using tasmota.scale_uint instead of integer mul/div
    # Maps blend_factor (0-255) to interpolate between fore and back colors
    var result_a = tasmota.scale_uint(blend_factor, 0, 255, fore_a, back_a)
    var result_r = tasmota.scale_uint(blend_factor, 0, 255, fore_r, back_r)
    var result_g = tasmota.scale_uint(blend_factor, 0, 255, fore_g, back_g)
    var result_b = tasmota.scale_uint(blend_factor, 0, 255, fore_b, back_b)
    
    # Combine components into a 32-bit value (ARGB format)
    return (int(result_a) << 24) | (int(result_r) << 16) | (int(result_g) << 8) | int(result_b)
  end
  
  # Fill a region of the buffer with a specific color
  # pixels: destination bytes buffer
  # color: the color to fill (ARGB format - 0xAARRGGBB)
  # start_pos: start position (default: 0)
  # end_pos: end position (default: -1 = last pixel)
  static def fill_pixels(pixels, color, start_pos, end_pos)
    # Default parameters
    if (start_pos == nil) start_pos = 0 end
    if (end_pos == nil) end_pos = -1 end
    
    # Validate region bounds
    var width = size(pixels) / 4
    
    # Handle negative indices (Python-style)
    if (start_pos < 0) start_pos += width end
    if (end_pos < 0) end_pos += width end
    
    # Clamp to valid range
    if (start_pos < 0) start_pos = 0 end
    if (end_pos < 0) end_pos = 0 end
    if (start_pos >= width) return end
    if (end_pos >= width) end_pos = width - 1 end
    if (end_pos < start_pos) return end
    
    # Fill the region with the color
    var i = start_pos
    while i <= end_pos
      pixels.set(i * 4, color, 4)
      i += 1
    end
  end
  
  # Blend destination buffer with source buffer using per-pixel alpha
  # dest_pixels: destination bytes buffer
  # src_pixels: source bytes buffer
  # region_start: start index for blending
  # region_end: end index for blending
  static def blend_pixels(dest_pixels, src_pixels, region_start, region_end)
    # Default parameters
    if (region_start == nil) region_start = 0 end
    if (region_end == nil) region_end = -1 end

    # Validate region bounds
    var dest_width = size(dest_pixels) / 4
    var src_width = size(src_pixels) / 4
    if (dest_width < src_width) dest_width = src_width end
    if (src_width < dest_width) src_width = dest_width end

    if (region_start < 0) region_start += dest_width end
    if (region_end < 0) region_end += dest_width end
    if (region_start < 0)  region_start = 0 end
    if (region_end < 0)region_end = 0 end
    if (region_start >= dest_width) return end
    if (region_end >= dest_width) region_end = dest_width - 1 end
    if (region_end < region_start) return end
    
    # Blend each pixel using the blend function
    var i = region_start
    while i <= region_end
      var color2 = src_pixels.get(i * 4, 4)
      var a2 = (color2 >> 24) & 0xFF
      
      # Only blend if the source pixel has some alpha
      if a2 > 0
        if a2 == 255
          # Fully opaque source pixel, just copy it
          dest_pixels.set(i * 4, color2, 4)
        else
          # Partially transparent source pixel, need to blend
          var color1 = dest_pixels.get(i * 4, 4)
          var blended = _class.blend(color1, color2)
          dest_pixels.set(i * 4, blended, 4)
        end
      end
      
      i += 1
    end
  end
  
  # Create a gradient fill in the buffer
  # pixels: destination bytes buffer
  # color1: start color (ARGB format - 0xAARRGGBB)
  # color2: end color (ARGB format - 0xAARRGGBB)
  # start_pos: start position (default: 0)
  # end_pos: end position (default: -1 = last pixel)
  static def gradient_fill(pixels, color1, color2, start_pos, end_pos)
    # Default parameters
    if (start_pos == nil) start_pos = 0 end
    if (end_pos == nil) end_pos = -1 end
    
    # Validate region bounds
    var width = size(pixels) / 4
    
    # Handle negative indices (Python-style)
    if (start_pos < 0) start_pos += width end
    if (end_pos < 0) end_pos += width end
    
    # Clamp to valid range
    if (start_pos < 0) start_pos = 0 end
    if (end_pos < 0) end_pos = 0 end
    if (start_pos >= width) return end
    if (end_pos >= width) end_pos = width - 1 end
    if (end_pos < start_pos) return end
    
    # Set first pixel directly
    pixels.set(start_pos * 4, color1, 4)
    
    # If only one pixel, we're done
    if start_pos == end_pos
      return
    end
    
    # Set last pixel directly
    pixels.set(end_pos * 4, color2, 4)
    
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
      pixels.set(i * 4, color, 4)
      i += 1
    end
  end
  
  # Blend a specific region with a solid color using the color's alpha channel
  # pixels: destination bytes buffer
  # color: the color to blend (ARGB format - 0xAARRGGBB)
  # start_pos: start position (default: 0)
  # end_pos: end position (default: -1 = last pixel)
  static def blend_color(pixels, color, start_pos, end_pos)
    # Default parameters
    if (start_pos == nil) start_pos = 0 end
    if (end_pos == nil) end_pos = -1 end
    
    # Validate region bounds
    var width = size(pixels) / 4
    
    # Handle negative indices (Python-style)
    if (start_pos < 0) start_pos += width end
    if (end_pos < 0) end_pos += width end
    
    # Clamp to valid range
    if (start_pos < 0) start_pos = 0 end
    if (end_pos < 0) end_pos = 0 end
    if (start_pos >= width) return end
    if (end_pos >= width) end_pos = width - 1 end
    if (end_pos < start_pos) return end
    
    # Extract alpha from color
    var a2 = (color >> 24) & 0xFF
    
    # Only blend if the color has some alpha
    if a2 == 0
      return  # Fully transparent, nothing to do
    end
    
    # Blend the pixels in the specified region
    var i = start_pos
    while i <= end_pos
      var color1 = pixels.get(i * 4, 4)
      var blended = _class.blend(color1, color)
      pixels.set(i * 4, blended, 4)
      i += 1
    end
  end
  
  # Apply an opacity adjustment to a region of the buffer
  # pixels: destination bytes buffer
  # opacity: opacity factor (0-511) OR mask_pixels (bytes buffer to use as mask)
  #   - Number: 0 is fully transparent, 255 is original, 511 is maximum opaque
  #   - bytes(): uses alpha channel as opacity mask
  # start_pos: start position (default: 0)
  # end_pos: end position (default: -1 = last pixel)
  static def apply_opacity(pixels, opacity, start_pos, end_pos)
    if opacity == nil opacity = 255 end
    
    # Default parameters
    if (start_pos == nil) start_pos = 0 end
    if (end_pos == nil) end_pos = -1 end
    
    # Validate region bounds
    var width = size(pixels) / 4
    
    # Handle negative indices (Python-style)
    if (start_pos < 0) start_pos += width end
    if (end_pos < 0) end_pos += width end
    
    # Clamp to valid range
    if (start_pos < 0) start_pos = 0 end
    if (end_pos < 0) end_pos = 0 end
    if (start_pos >= width) return end
    if (end_pos >= width) end_pos = width - 1 end
    if (end_pos < start_pos) return end
    
    # Check if opacity is a bytes buffer (mask mode)
    if isinstance(opacity, bytes)
      # Mask mode: use another buffer as opacity mask
      var mask_pixels = opacity
      var mask_width = size(mask_pixels) / 4
      
      # Validate mask size
      if mask_width < width
        width = mask_width
      end
      if end_pos >= width
        end_pos = width - 1
      end
      
      # Apply mask opacity
      var i = start_pos
      while i <= end_pos
        var color = pixels.get(i * 4, 4)
        var mask_color = mask_pixels.get(i * 4, 4)
        
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
        pixels.set(i * 4, new_color, 4)
        
        i += 1
      end
    else
      # Number mode: uniform opacity adjustment
      var opacity_value = int(opacity == nil ? 255 : opacity)
      
      # Ensure opacity is in valid range (0-511)
      opacity_value = opacity_value < 0 ? 0 : (opacity_value > 511 ? 511 : opacity_value)
      
      # Apply opacity adjustment
      var i = start_pos
      while i <= end_pos
        var color = pixels.get(i * 4, 4)
        
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
        pixels.set(i * 4, color, 4)
        
        i += 1
      end
    end
  end

  # Apply a brightness adjustment to a region of the buffer
  # pixels: destination bytes buffer
  # brightness: brightness factor (0-511) OR mask_pixels (bytes buffer to use as mask)
  #   - Number: 0 is black, 255 is original, 511 is maximum bright
  #   - bytes(): uses alpha channel as brightness mask
  # start_pos: start position (default: 0)
  # end_pos: end position (default: -1 = last pixel)
  static def apply_brightness(pixels, brightness, start_pos, end_pos)
    # Default parameters
    if (start_pos == nil) start_pos = 0 end
    if (end_pos == nil) end_pos = -1 end
    
    # Validate region bounds
    var width = size(pixels) / 4
    
    # Handle negative indices (Python-style)
    if (start_pos < 0) start_pos += width end
    if (end_pos < 0) end_pos += width end
    
    # Clamp to valid range
    if (start_pos < 0) start_pos = 0 end
    if (end_pos < 0) end_pos = 0 end
    if (start_pos >= width) return end
    if (end_pos >= width) end_pos = width - 1 end
    if (end_pos < start_pos) return end
    
    # Check if brightness is a bytes buffer (mask mode)
    if isinstance(brightness, bytes)
      # Mask mode: use another buffer as brightness mask
      var mask_pixels = brightness
      var mask_width = size(mask_pixels) / 4
      
      # Validate mask size
      if mask_width < width
        width = mask_width
      end
      if end_pos >= width
        end_pos = width - 1
      end
      
      # Apply mask brightness
      var i = start_pos
      while i <= end_pos
        var color = pixels.get(i * 4, 4)
        var mask_color = mask_pixels.get(i * 4, 4)
        
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
        pixels.set(i * 4, new_color, 4)
        
        i += 1
      end
    else
      # Number mode: uniform brightness adjustment
      var brightness_value = int(brightness == nil ? 255 : brightness)
      
      # Ensure brightness is in valid range (0-511)
      brightness_value = brightness_value < 0 ? 0 : (brightness_value > 511 ? 511 : brightness_value)
      
      # Apply brightness adjustment
      var i = start_pos
      while i <= end_pos
        var color = pixels.get(i * 4, 4)
        
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
        pixels.set(i * 4, color, 4)
        
        i += 1
      end
    end
  end
end

return FrameBufferNtv