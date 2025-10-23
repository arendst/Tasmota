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

# Special import for FrameBufferNtv that is pure Berry but will be replaced
# by native code in Tasmota, so we don't register to 'animation' module
# so that it is not solidified
import "./core/frame_buffer_ntv" as FrameBufferNtv

class FrameBuffer : FrameBufferNtv
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
    if (size(self.pixels) != self.width * 4)
      self.pixels.resize(self.width * 4)  # resize to full size filled with transparent black (all zeroes)
    end
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
  
  # # Convert separate a, r, g, b components to a 32-bit color value
  # # r: red component (0-255)
  # # g: green component (0-255)
  # # b: blue component (0-255)
  # # a: alpha component (0-255, default 255 = fully opaque)
  # # Returns: 32-bit color value in ARGB format (0xAARRGGBB)
  # static def to_color(r, g, b, a)
  #   # Default alpha to fully opaque if not specified
  #   if a == nil
  #     a = 255
  #   end
    
  #   # Ensure values are in valid range
  #   r = r & 0xFF
  #   g = g & 0xFF
  #   b = b & 0xFF
  #   a = a & 0xFF
    
  #   # Combine components into a 32-bit value (ARGB format - 0xAARRGGBB)
  #   return (a << 24) | (r << 16) | (g << 8) | b
  # end
  
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
  
  # Create a copy of this frame buffer
  def copy()
    return animation.frame_buffer(self)   # return using the self copying constructor
  end

  # String representation of the frame buffer
  def tostring()
    return f"FrameBuffer(width={self.width}, pixels={self.pixels})"
  end
end

return {'frame_buffer': FrameBuffer}