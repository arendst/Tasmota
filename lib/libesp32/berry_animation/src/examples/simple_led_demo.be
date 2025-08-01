# Simple LED Demo
# This is a very basic demo that just sets different LED values directly
# to verify that we can display changing values

import global
import tasmota

# Helper function to dump LED values
def dump_strip(strip, size)
  var output = "["
  for i:0..size-1
    if i > 0
      output += ", "
    end
    output += format("0x%06X", strip.get_pixel_color(i) & 0xFFFFFF)
  end
  output += "]"
  print(output)
end

print("Starting Simple LED Demo")

# Create a strip with 3 LEDs
var strip = global.Leds(3)

# Define some test values
var test_values = [
  0x0000FF,  # Blue
  0x00FF00,  # Green
  0xFF0000,  # Red
  0xFFFF00,  # Yellow
  0xFF00FF,  # Magenta
  0x00FFFF,  # Cyan
  0xFFFFFF   # White
]

# Set each value and display the result
for i:0..size(test_values)-1
  var color = test_values[i]
  
  # Extract RGB components
  var r = (color >> 16) & 0xFF
  var g = (color >> 8) & 0xFF
  var b = color & 0xFF
  
  print(f"Setting color #{i+1}: RGB({r},{g},{b}) = 0x{color:06X}")
  
  # Set all LEDs to this color
  for j:0..2  # strip has 3 LEDs
    strip.set_pixel_color(j, color)
  end
  
  # Display the LED values
  var led_values = "LEDs: ["
  for j:0..strip.size-1
    if j > 0
      led_values += ", "
    end
    led_values += format("R:%d,G:%d,B:%d", r, g, b)
  end
  led_values += "]"
  print(led_values)
  
  # Show hex values
  dump_strip(strip, 3)
  
  # Add a separator
  print("---")
end

print("Simple LED Demo completed")

return true