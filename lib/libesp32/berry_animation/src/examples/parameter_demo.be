# Example demonstrating the parameter handling functionality
# of the Berry Animation Framework
#
# Command to run:
#    ./berry -s -g -m lib/libesp32/berry_animation lib/libesp32/berry_animation/examples/parameter_demo.be

import global
import tasmota
import animation
import math

# Create a custom animation that uses parameters
class PulseAnimation : animation.animation
  # Initialize with custom parameters
  def init()
    animation.animation.init(self, 1, 255, 0, true, "pulse")
    
    # Register custom parameters with validation
    # Note: Parameter system now only accepts integers, so color is stored as integer
    self.register_param("color", {
      "min": 1,         # 1=red, 2=green, 3=blue, 4=white
      "max": 4,
      "default": 1
    })
    
    self.register_param("speed", {
      "min": 1,
      "max": 10,
      "default": 5
    })
    
    self.register_param("intensity", {
      "min": 0,
      "max": 100,
      "default": 50
    })
    
    # Set initial values
    self.set_param("color", 1)  # 1 = red
    self.set_param("speed", 5)
    self.set_param("intensity", 50)
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    print(format("Parameter '%s' changed to: %s", name, str(value)))
    
    # Recalculate derived values based on parameters
    if name == "speed"
      # Adjust duration based on speed (higher speed = lower duration)
      self.set_duration(2000 / value)
    end
  end
  
  # Render the animation
  def render(frame)
    if !self.is_running || self.frame_buffer == nil
      return false
    end
    
    # Get current parameters
    var color = self.get_param("color", 1)  # 1 = red
    var intensity = self.get_param("intensity", 50)
    
    # Calculate pulse value based on progress (0 to 255)
    var progress = self.get_progress()
    # Convert progress to 0.0-1.0 range for math.sin calculation
    var progress_float = progress / 255.0
    var pulse_value = math.sin(progress_float * 2 * math.pi) * 0.5 + 0.5
    
    # Scale by intensity
    pulse_value = tasmota.scale_uint(pulse_value, 0, 255, 0, tasmota.scale_uint(intensity, 0, 100, 0, 255))
    
    # Set color based on parameter (1=red, 2=green, 3=blue, 4=white)
    var r = 0
    var g = 0
    var b = 0
    
    if color == 1  # red
      r = int(255 * pulse_value)
    elif color == 2  # green
      g = int(255 * pulse_value)
    elif color == 3  # blue
      b = int(255 * pulse_value)
    elif color == 4  # white
      r = int(255 * pulse_value)
      g = int(255 * pulse_value)
      b = int(255 * pulse_value)
    end
    
    # Fill the frame with the calculated color
    frame.fill_pixels(animation.frame_buffer.to_color(r, g, b, 255))
    
    return true
  end
end

# Create a mock frame buffer for demonstration
var frame = animation.frame_buffer(10)

# Create the pulse animation
var pulse = PulseAnimation()
pulse.start()

# Simulate animation updates
print("Initial parameters:")
print(format("  color: %s", pulse.get_param("color", nil)))
print(format("  speed: %d", pulse.get_param("speed", nil)))
print(format("  intensity: %d", pulse.get_param("intensity", nil)))
print(format("  duration: %d", pulse.duration))

print("\nUpdating parameters:")
pulse.set_param("color", 3)  # 3 = blue
pulse.set_param("speed", 8)
pulse.set_param("intensity", 75)

print("\nFinal parameters:")
print(format("  color: %s", pulse.get_param("color", nil)))
print(format("  speed: %d", pulse.get_param("speed", nil)))
print(format("  intensity: %d", pulse.get_param("intensity", nil)))
print(format("  duration: %d", pulse.duration))

# Demonstrate validation
print("\nValidation tests:")
print(format("Set invalid color: %s", pulse.set_param("color", 5) ? "accepted" : "rejected"))  # 5 is not in enum [1,2,3,4]
print(format("Set invalid speed: %s", pulse.set_param("speed", 20) ? "accepted" : "rejected"))
print(format("Set invalid intensity: %s", pulse.set_param("intensity", -10) ? "accepted" : "rejected"))

# Demonstrate individual parameter updates (update_params method removed)
print("\nIndividual parameter updates:")
var color_result = pulse.set_param("color", 2)  # 2 = green
var speed_result = pulse.set_param("speed", 3)
var intensity_result = pulse.set_param("intensity", 90)
var all_success = color_result && speed_result && intensity_result
print(format("Update result: %s", all_success ? "success" : "partial failure"))

print("\nUpdated parameters:")
print(format("  color: %s", pulse.get_param("color", nil)))
print(format("  speed: %d", pulse.get_param("speed", nil)))
print(format("  intensity: %d", pulse.get_param("intensity", nil)))
print(format("  duration: %d", pulse.duration))

# Render a frame to demonstrate the animation
pulse.render(frame, tasmota.millis())
print("\nRendered frame with current parameters")

print("\nParameter demo completed!")