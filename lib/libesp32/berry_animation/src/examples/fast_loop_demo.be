# Fast Loop Integration Demo
#
# This example demonstrates how the AnimationController integrates
# with Tasmota's fast_loop system for efficient animation updates.

# Import the animation module
import global
import tasmota
import animation

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
  
  def set_pixel_color(index, color)
    self.pixels[index] = {
      "r": color & 0xFF,
      "g": (color >> 8) & 0xFF,
      "b": (color >> 16) & 0xFF
    }
  end
  
  def show()
    self.show_called = true
  end
  
  def can_show()
    return self.can_show_result
  end
end

# Create a simple animation that shows the fast_loop timing
class TimingAnimation : animation.animation
  var color
  var last_update
  var update_count
  
  def init(color)
    # Initialize with priority 1, 1 second duration, and looping enabled
    super(self).init(1, 1000, true, "timing_animation")
    
    # Store parameters
    self.color = color
    self.last_update = 0
    self.update_count = 0
  end
  
  def update(time_ms)
    # Track update timing
    if self.last_update > 0
      var delta = time_ms - self.last_update
      
      # Print timing information every second
      if self.update_count % 200 == 0  # Roughly every second at ~200 updates/sec
        print(format("Animation update delta: %dms", delta))
      end
    end
    
    self.last_update = time_ms
    self.update_count += 1
    
    return super(self).update(time_ms)
  end
  
  def render(frame)
    if !self.is_running || frame == nil
      return false
    end
    
    # Extract color components
    var r = self.color & 0xFF
    var g = (self.color >> 8) & 0xFF
    var b = (self.color >> 16) & 0xFF
    
    # Fill the frame with the color
    frame.fill_pixels(animation.frame_buffer.to_color(r, g, b, 255))
    
    return true
  end
end

# Main demo function
def run_demo()
  # Create a WS2812 LED strip (adjust parameters for your setup)
  var strip_length = 30
  var strip = WS2812(strip_length)
  
  # Create the animation controller
  var controller = animation.animation_controller(strip)
  
  # Create timing animation
  var timing_anim = TimingAnimation(0x0000FF)  # Blue color
  
  # Add animation to the controller
  controller.add_animation(timing_anim)
  
  # Start the controller
  controller.start()
  
  print("Fast loop integration demo started")
  print("This demo shows the fast_loop integration with timing information")
  print("Press 's' to stop animations")
  print("Press 'r' to restart animations")
  print("Press 'q' to quit")
  
  # Set up a simple command handler
  def handle_command(cmd)
    if cmd == "s"
      controller.stop()
      print("Animations stopped")
    elif cmd == "r"
      controller.start()
      print("Animations restarted")
    elif cmd == "q"
      controller.stop()
      print("Demo ended")
      return false
    end
    return true
  end
  
  # In a real application, you would integrate with Tasmota's command system
  # For this demo, we'll just return the controller and handler for manual testing
  return {
    "controller": controller,
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