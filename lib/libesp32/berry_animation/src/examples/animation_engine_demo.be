# Animation Engine Demo
#
# This example demonstrates how to use the unified AnimationEngine
# to manage animations for an LED strip.

# Import the animation module
import animation
import math
import tasmota
import global

# Create a mock WS2812 class for testing
class WS2812
  var length_value
  var pixels
  var show_called
  var can_show_result
  
  def init(length)
    self.length_value = length
    self.pixels = {}
    self.show_called = false
    self.can_show_result = true
  end
  
  def length()
    return self.length_value
  end
  
  def set_pixel_color(index, r, g, b)
    self.pixels[index] = {
      "r": r,
      "g": g,
      "b": b
    }
  end
  
  def show()
    self.show_called = true
  end
  
  def can_show()
    return self.can_show_result
  end
end

# Create a simple animation that pulses a color
class PulseAnimation : animation.animation
  var color
  var min_brightness
  var max_brightness
  
  def init(color, min_brightness, max_brightness, duration)
    # Initialize with priority 1, specified duration, and looping enabled
    super(self).init(1, duration, true, "pulse")
    
    # Store parameters
    self.color = color
    self.min_brightness = min_brightness
    self.max_brightness = max_brightness
    
    # Register parameters with validation
    self.register_param("color", {"default": 0xFF0000})  # Default to red
    self.register_param("min_brightness", {"min": 0, "max": 100, "default": 10})
    self.register_param("max_brightness", {"min": 0, "max": 100, "default": 100})
    
    # Set initial values
    self.set_param("color", color)
    self.set_param("min_brightness", min_brightness)
    self.set_param("max_brightness", max_brightness)
  end
  
  def render(frame)
    if !self.is_running || frame == nil
      return false
    end
    
    # Calculate current brightness based on progress
    var progress = self.get_progress()
    
    # Convert progress to 0.0-1.0 range and use sine wave for smooth pulsing (0 to 1 to 0)
    var progress_float = progress / 255.0
    var brightness_factor = math.sin(progress_float * math.pi)
    
    # Scale brightness between min and max
    var brightness_range = self.max_brightness - self.min_brightness
    var brightness = self.min_brightness + (brightness_factor * brightness_range)
    
    # Extract color components
    var r = self.color & 0xFF
    var g = (self.color >> 8) & 0xFF
    var b = (self.color >> 16) & 0xFF
    
    # Apply brightness
    r = tasmota.scale_uint(r, 0, 255, 0, tasmota.scale_uint(brightness, 0, 100, 0, 255))
    g = tasmota.scale_uint(g, 0, 255, 0, tasmota.scale_uint(brightness, 0, 100, 0, 255))
    b = tasmota.scale_uint(b, 0, 255, 0, tasmota.scale_uint(brightness, 0, 100, 0, 255))
    
    # Fill the frame with the color at the calculated brightness
    frame.fill_pixels(animation.frame_buffer.to_color(r, g, b, 255))
    
    return true
  end
  
  def on_param_changed(name, value)
    if name == "color"
      self.color = value
    elif name == "min_brightness"
      self.min_brightness = value
    elif name == "max_brightness"
      self.max_brightness = value
    end
  end
end

# Create a simple animation that moves a dot across the strip
class MovingDotAnimation : animation.animation
  var color
  var background_color
  var dot_size
  var direction
  var position
  
  def init(color, background_color, dot_size, duration)
    # Initialize with priority 2, specified duration, and looping enabled
    super(self).init(2, duration, true, "moving_dot")
    
    # Store parameters
    self.color = color
    self.background_color = background_color
    self.dot_size = dot_size
    self.direction = 1  # 1 for right, -1 for left
    self.position = 0
    
    # Register parameters
    self.register_param("color", {"default": 0xFFFFFF})  # Default to white
    self.register_param("background_color", {"default": 0x000000})  # Default to black
    self.register_param("dot_size", {"min": 1, "default": 3})
    self.register_param("direction", {"enum": [1, -1], "default": 1})
    
    # Set initial values
    self.set_param("color", color)
    self.set_param("background_color", background_color)
    self.set_param("dot_size", dot_size)
    self.set_param("direction", self.direction)
  end
  
  def render(frame)
    if !self.is_running || frame == nil
      return false
    end
    
    # Calculate current position based on progress
    var progress = self.get_progress()
    var width = frame.width
    
    # Calculate position (0 to width-1) - convert progress from 0-255 to 0.0-1.0 range
    var progress_float = progress / 255.0
    self.position = int(progress_float * width)
    
    # Fill background
    frame.fill_pixels(animation.frame_buffer.to_color(
      self.background_color & 0xFF,
      (self.background_color >> 8) & 0xFF,
      (self.background_color >> 16) & 0xFF,
      255
    ))
    
    # Draw dot
    var half_size = self.dot_size / 2
    var start_pos = self.position - half_size
    var end_pos = self.position + half_size
    
    # Ensure dot is within bounds
    if start_pos < 0
      start_pos = 0
    end
    if end_pos >= width
      end_pos = width - 1
    end
    
    # Draw the dot
    for i: start_pos..end_pos
      frame.set_pixel_color(i, 
        animation.frame_buffer.to_color(
          self.color & 0xFF,
          (self.color >> 8) & 0xFF,
          (self.color >> 16) & 0xFF
        )
      )
    end
    
    return true
  end
  
  def on_param_changed(name, value)
    if name == "color"
      self.color = value
    elif name == "background_color"
      self.background_color = value
    elif name == "dot_size"
      self.dot_size = value
    elif name == "direction"
      self.direction = value
    end
  end
end

# Main demo function
def run_demo()
  # Create a WS2812 LED strip (adjust parameters for your setup)
  var strip_length = 30
  var strip = WS2812(strip_length)
  
  # Create the animation engine
  var engine = animation.create_engine(strip)
  
  # Create animations
  var pulse = PulseAnimation(0xFF0000, 10, 100, 2000)  # Red pulse, 2 seconds duration
  var dot = MovingDotAnimation(0x00FF00, 0x000000, 3, 5000)  # Green dot, 5 seconds duration
  
  # Add animations to the engine
  engine.add_animation(pulse)
  engine.add_animation(dot)
  
  # Start the engine
  engine.start()
  
  print("Animation engine demo started")
  print("Press 'b+' to increase brightness")
  print("Press 'b-' to decrease brightness")
  print("Press 's' to stop animations")
  print("Press 'r' to restart animations")
  print("Press 'p' to pause animations (keeps fast_loop active)")
  print("Press 'c' to resume animations")
  print("Press 'i' to show controller info")
  print("Press 'q' to quit")
  
  # Set up a simple command handler
  def handle_command(cmd)
    if cmd == "b+"
      # Brightness is now handled by the strip object
      var current_bri = strip.get_bri()
      var new_bri = current_bri + 25  # Increase by ~10% (25/255)
      if new_bri > 255
        new_bri = 255
      end
      strip.set_bri(new_bri)
      print(f"Brightness set to {tasmota.scale_uint(new_bri, 0, 255, 0, 100)}%")
    elif cmd == "b-"
      # Brightness is now handled by the strip object
      var current_bri = strip.get_bri()
      var new_bri = current_bri - 25  # Decrease by ~10% (25/255)
      if new_bri < 0
        new_bri = 0
      end
      strip.set_bri(new_bri)
      print(f"Brightness set to {tasmota.scale_uint(new_bri, 0, 255, 0, 100)}%")
    elif cmd == "s"
      engine.stop()
      print("Animations stopped")
    elif cmd == "r"
      engine.start()
      print("Animations restarted")
    elif cmd == "p"
      # Pause individual animations
      for anim : engine.get_animations()
        anim.pause()
      end
      print("Animations paused (fast_loop still active)")
    elif cmd == "c"
      # Resume individual animations
      for anim : engine.get_animations()
        anim.resume()
      end
      print("Animations resumed")
    elif cmd == "i"
      print(engine)
      print(f"Last update: {engine.last_update} ms")
      print(f"Current time: {tasmota.millis()} ms")
    elif cmd == "q"
      engine.stop()
      print("Demo ended")
      return false
    end
    return true
  end
  
  # In a real application, you would integrate with Tasmota's command system
  # For this demo, we'll just return the engine and handler for manual testing
  return {
    "engine": engine,
    "handler": handle_command
  }
end

# Run the demo if this file is executed directly
try
  if tasmota != nil
    # When running in Tasmota
    var demo = run_demo()
    return demo
  end
except .. as e
  # When running in standalone Berry or if tasmota is not available
  print("This demo is designed to run in Tasmota")
  print("Running in simulation mode...")
  var demo = run_demo()
  print("Demo initialized successfully in simulation mode")
  return demo
end