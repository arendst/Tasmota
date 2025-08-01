# Example: PulsePositionAnimation with ValueProvider support
#
# This example demonstrates how to modify an animation to support both static values
# and dynamic ValueProvider instances for its parameters.

# Import the animation framework
import animation

# Create a modified PulsePositionAnimation that supports ValueProviders
class PulsePositionAnimationWithProviders : animation.pulse_position_animation
  
  # Override the render method to use value providers
  def render(frame)
    if !self.is_running || frame == nil
      return false
    end
    
    var current_time = tasmota.millis()
    var pixel_size = frame.width
    
    # Get values using the value provider system
    var back_color = self.get_param_value("back_color", current_time)
    var pos = self.get_param_value("pos", current_time)
    var slew_size = self.get_param_value("slew_size", current_time)
    var pulse_size = self.get_param_value("pulse_size", current_time)
    var color = self.get_param_value("color", current_time)
    
    # Ensure we have valid values (fallback to defaults if providers return nil)
    if back_color == nil back_color = 0xFF000000 end
    if pos == nil pos = 0 end
    if slew_size == nil slew_size = 0 end
    if pulse_size == nil pulse_size = 1 end
    if color == nil color = 0xFFFFFFFF end
    
    # Ensure non-negative values
    if slew_size < 0 slew_size = 0 end
    if pulse_size < 0 pulse_size = 0 end
    
    # Fill background if not transparent
    if back_color != 0xFF000000
      frame.fill_pixels(back_color)
    end
    
    # Calculate pulse boundaries
    var pulse_min = pos
    var pulse_max = pos + pulse_size
    
    # Clamp to frame boundaries
    if pulse_min < 0
      pulse_min = 0
    end
    if pulse_max >= pixel_size
      pulse_max = pixel_size
    end
    
    # Draw the main pulse
    var i = pulse_min
    while i < pulse_max
      frame.set_pixel_color(i, color)
      i += 1
    end
    
    # Draw slew regions if slew_size > 0
    if slew_size > 0
      # Left slew (fade from background to pulse color)
      var left_slew_min = pos - slew_size
      var left_slew_max = pos
      
      if left_slew_min < 0
        left_slew_min = 0
      end
      if left_slew_max >= pixel_size
        left_slew_max = pixel_size
      end
      
      i = left_slew_min
      while i < left_slew_max
        # Calculate blend factor (255 = background, 0 = pulse color)
        var blend_factor
        if slew_size == 1
          # For single pixel slew, use 50% blend
          blend_factor = 128
        else
          blend_factor = tasmota.scale_uint(i, pos - slew_size, pos - 1, 255, 0)
        end
        # Create color with appropriate alpha for blending
        var alpha = 255 - blend_factor  # Invert so 0 = transparent, 255 = opaque
        var blend_color = (alpha << 24) | (color & 0x00FFFFFF)
        var blended_color = frame.blend(back_color, blend_color)
        frame.set_pixel_color(i, blended_color)
        i += 1
      end
      
      # Right slew (fade from pulse color to background)
      var right_slew_min = pos + pulse_size
      var right_slew_max = pos + pulse_size + slew_size
      
      if right_slew_min < 0
        right_slew_min = 0
      end
      if right_slew_max >= pixel_size
        right_slew_max = pixel_size
      end
      
      i = right_slew_min
      while i < right_slew_max
        # Calculate blend factor (0 = pulse color, 255 = background)
        var blend_factor
        if slew_size == 1
          # For single pixel slew, use 50% blend
          blend_factor = 128
        else
          blend_factor = tasmota.scale_uint(i, pos + pulse_size, pos + pulse_size + slew_size - 1, 0, 255)
        end
        # Create color with appropriate alpha for blending
        var alpha = 255 - blend_factor  # Start opaque, fade to transparent
        var blend_color = (alpha << 24) | (color & 0x00FFFFFF)
        var blended_color = frame.blend(back_color, blend_color)
        frame.set_pixel_color(i, blended_color)
        i += 1
      end
    end
    
    return true
  end
  
  # Override parameter setters to support both static values and providers
  def set_color(color_or_provider)
    self.set_param_value("color", color_or_provider)
    return self
  end
  
  def set_back_color(color_or_provider)
    self.set_param_value("back_color", color_or_provider)
    return self
  end
  
  def set_pos(pos_or_provider)
    self.set_param_value("pos", pos_or_provider)
    return self
  end
  
  def set_slew_size(size_or_provider)
    self.set_param_value("slew_size", size_or_provider)
    return self
  end
  
  def set_pulse_size(size_or_provider)
    self.set_param_value("pulse_size", size_or_provider)
    return self
  end
end

# Example usage demonstrating both static values and value providers

# Create some simple example value providers
class SimpleTimeProvider : animation.value_provider
  var multiplier, offset
  
  def init(multiplier, offset)
    self.multiplier = multiplier != nil ? multiplier : 1
    self.offset = offset != nil ? offset : 0
  end
  
  def get_value(time_ms)
    # Simple time-based value that changes every second
    return self.offset + ((time_ms / 1000) * self.multiplier) % 60
  end
  
  # Specific method for position parameter
  def get_pos(time_ms)
    return int(self.get_value(time_ms))
  end
  
  # Specific method for pulse_size parameter
  def get_pulse_size(time_ms)
    var value = int(self.get_value(time_ms)) % 8 + 1  # 1-8 pixels
    return value
  end
end

class AlternatingColorProvider : animation.value_provider
  var color1, color2, switch_period
  
  def init(color1, color2, switch_period)
    self.color1 = color1 != nil ? color1 : 0xFFFF0000  # Red
    self.color2 = color2 != nil ? color2 : 0xFF0000FF  # Blue
    self.switch_period = switch_period != nil ? switch_period : 1000  # 1 second
  end
  
  def get_value(time_ms)
    # Alternate between two colors based on time
    var cycle = (time_ms / self.switch_period) % 2
    return cycle < 1 ? self.color1 : self.color2
  end
  
  # Specific method for color parameter
  def get_color(time_ms)
    return self.get_value(time_ms)
  end
end

# Demo function
def demo_value_providers()
  print("=== ValueProvider Demo ===")
  
  # Create LED strip (60 pixels)
  var strip = global.Leds(60, 1)
  var engine = animation.create_engine(strip)
  
  # Create value providers
  var moving_pos = SimpleTimeProvider(2, 10)  # Position moves slowly across strip
  var changing_size = SimpleTimeProvider(1, 1)  # Size changes over time
  var alternating_color = AlternatingColorProvider(0xFFFF0000, 0xFF00FF00, 2000)  # Red/green every 2s
  
  # Create animation with mixed static and dynamic parameters
  var pulse_anim = PulsePositionAnimationWithProviders(
    0xFFFFFFFF,  # Static white color (will be overridden)
    3,           # Static pulse size (will be overridden)
    1,           # Static slew size
    10,          # Priority
    0,           # Infinite duration
    false,       # No loop
    "dynamic_pulse"
  )
  
  # Set some parameters to use value providers
  pulse_anim.set_pos(moving_pos)           # Dynamic position
  pulse_anim.set_pulse_size(changing_size) # Dynamic size
  pulse_anim.set_color(alternating_color)  # Dynamic color
  # back_color and slew_size remain static
  
  # Add to engine and start
  engine.add_animation(pulse_anim)
  engine.start()
  
  print("Animation started with dynamic parameters:")
  print("- Position: Moves across strip over time")
  print("- Pulse size: Changes from 1 to 8 pixels cyclically")
  print("- Color: Alternates between red and green every 2 seconds")
  print("- Background and slew: Static values")
  
  return engine
end

# Export the demo function
animation.demo_value_providers = demo_value_providers

print("ValueProvider example loaded. Run animation.demo_value_providers() to see it in action.")

return PulsePositionAnimationWithProviders