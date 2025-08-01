# Twinkle Animation Demo
# Demonstrates the TwinkleAnimation class with various configurations

import global
import animation

print("=== Twinkle Animation Demo ===")

# Demo configuration
var STRIP_LENGTH = 30
var DEMO_DURATION = 8000  # 8 seconds per demo

print(f"Strip length: {STRIP_LENGTH} pixels")
print(f"Demo duration: {DEMO_DURATION}ms per animation")

# Demo 1: Classic White Twinkle
print("\n--- Demo 1: Classic White Twinkle ---")
var classic_twinkle = animation.twinkle_animation.classic(128, STRIP_LENGTH, 10)
print(f"Created classic twinkle: {classic_twinkle}")
print("This creates a classic twinkling stars effect with:")
print("- White color")
print("- Medium density (128/255)")
print("- 6 Hz twinkle speed")
print("- Moderate fade speed")

# Demo 2: Gentle Blue Twinkle
print("\n--- Demo 2: Gentle Blue Twinkle ---")
var gentle_twinkle = animation.twinkle_animation.gentle(0xFF4169E1, STRIP_LENGTH, 10)  # Royal Blue
print(f"Created gentle twinkle: {gentle_twinkle}")
print("This creates a gentle twinkling effect with:")
print("- Royal blue color")
print("- Low density (64/255)")
print("- Slow twinkle speed (3 Hz)")
print("- Slow fade for longer-lasting twinkles")

# Demo 3: Intense Red Twinkle
print("\n--- Demo 3: Intense Red Twinkle ---")
var intense_twinkle = animation.twinkle_animation.intense(0xFFDC143C, STRIP_LENGTH, 10)  # Crimson
print(f"Created intense twinkle: {intense_twinkle}")
print("This creates an intense twinkling effect with:")
print("- Crimson red color")
print("- High density (200/255)")
print("- Fast twinkle speed (12 Hz)")
print("- Fast fade for rapid twinkling")

# Demo 4: Rainbow Twinkle
print("\n--- Demo 4: Rainbow Twinkle ---")
var rainbow_twinkle = animation.twinkle_animation.rainbow(100, STRIP_LENGTH, 10)
print(f"Created rainbow twinkle: {rainbow_twinkle}")
print("This creates a rainbow twinkling effect with:")
print("- Rainbow color palette")
print("- Medium density (100/255)")
print("- Standard timing")

# Demo 5: Custom Twinkle Configuration
print("\n--- Demo 5: Custom Twinkle Configuration ---")
var custom_twinkle = animation.twinkle_animation(
  0xFFFFD700,  # Gold color
  150,         # High density
  8,           # 8 Hz speed
  200,         # Fast fade
  16,          # Very dim minimum
  240,         # Bright maximum
  STRIP_LENGTH,
  10, 0, true, "custom_gold"
)
print(f"Created custom twinkle: {custom_twinkle}")
print("This creates a custom gold twinkling effect with:")
print("- Gold color (0xFFFFD700)")
print("- High density (150/255)")
print("- 8 Hz twinkle speed")
print("- Fast fade (200/255)")
print("- Wide brightness range (16-240)")

# Demo 6: Animation Controller Integration
print("\n--- Demo 6: Animation Controller Integration ---")
print("Creating animation controller and adding multiple twinkle effects...")

# Create a mock LED strip (in real usage, this would be a real strip)
class MockStrip
  var pixels
  var strip_length
  
  def init(length)
    self.strip_length = length
    self.pixels = []
    self.pixels.resize(length)
    for i: 0..length-1
      self.pixels[i] = 0
    end
  end
  
  def set_pixel_color(index, color)
    if index >= 0 && index < self.strip_length
      self.pixels[index] = color
    end
  end
  
  def show()
    # In real usage, this would update the physical LEDs
    return true
  end
  
  def get_pixel_color(index)
    if index >= 0 && index < self.strip_length
      return self.pixels[index]
    end
    return 0
  end
  
  def length()
    return self.strip_length
  end
end

var strip = global.Leds(STRIP_LENGTH)
var controller = animation.animation_controller(strip)

# Add multiple twinkle animations with different priorities
controller.add_animation(classic_twinkle)
controller.add_animation(gentle_twinkle.set_priority(5))  # Lower priority

print("Added classic twinkle (priority 10) and gentle twinkle (priority 5)")
print("The classic twinkle will render on top of the gentle twinkle")

# Demo 7: Real-time Parameter Updates
print("\n--- Demo 7: Real-time Parameter Updates ---")
var dynamic_twinkle = animation.twinkle_animation.classic(64, STRIP_LENGTH, 10)
print(f"Created dynamic twinkle with initial density: {dynamic_twinkle.get_param('density')}")

# Simulate parameter changes over time
var densities = [64, 100, 150, 200, 255, 128, 80]
for i: 0..size(densities)-1
  var new_density = densities[i]
  dynamic_twinkle.set_density(new_density)
  print(f"Updated density to: {new_density}")
end

# Demo 8: Performance Considerations
print("\n--- Demo 8: Performance Considerations ---")
print("Twinkle animation performance tips:")
print("- Lower twinkle_speed reduces CPU usage (fewer updates per second)")
print("- Lower density reduces computation and random number generation")
print("- Smaller strip_length reduces memory usage and computation")
print("- Use solid colors instead of color providers for maximum performance")

# Create a performance-optimized twinkle
var perf_twinkle = animation.twinkle_animation.solid(0xFFFFFFFF, 80, STRIP_LENGTH, 10)
perf_twinkle.set_twinkle_speed(4)  # Slower updates
perf_twinkle.set_fade_speed(150)   # Moderate fade
print(f"Created performance-optimized twinkle: {perf_twinkle}")

# Demo 9: Simulation Test
print("\n--- Demo 9: Twinkle Simulation Test ---")
print("Running twinkle simulation for a few cycles...")

var test_twinkle = animation.twinkle_animation.classic(180, 10, 10)  # Smaller strip for easier visualization
test_twinkle.start()

var frame = animation.frame_buffer(10)
var start_time = 1000

for cycle: 0..4
  var current_time = start_time + (cycle * 167)  # 6 Hz = 167ms intervals
  test_twinkle.update(current_time)
  test_twinkle.render(frame, current_time)
  
  print(f"Cycle {cycle + 1}:")
  var twinkle_info = "  Brightness: "
  for i: 0..9
    var color = frame.get_pixel_color(i)
    var brightness = 0
    if color != 0xFF000000
      # Extract brightness from color (simple approximation)
      var r = (color >> 16) & 0xFF
      var g = (color >> 8) & 0xFF
      var b = color & 0xFF
      brightness = (r + g + b) / 3
    end
    twinkle_info += f"{brightness:3d} "
  end
  print(twinkle_info)
end

# Demo 10: Different Twinkle Styles Comparison
print("\n--- Demo 10: Different Twinkle Styles Comparison ---")
print("Comparing different twinkle styles:")

var styles = [
  ["Gentle", animation.twinkle_animation.gentle(0xFFFFFFFF, 10, 10)],
  ["Classic", animation.twinkle_animation.classic(128, 10, 10)],
  ["Intense", animation.twinkle_animation.intense(0xFFFFFFFF, 10, 10)]
]

for style_info: styles
  var style_name = style_info[0]
  var style_twinkle = style_info[1]
  
  print(f"\n{style_name} style:")
  print(f"  Density: {style_twinkle.get_param('density')}")
  print(f"  Speed: {style_twinkle.get_param('twinkle_speed')} Hz")
  print(f"  Fade: {style_twinkle.get_param('fade_speed')}")
  print(f"  Brightness: {style_twinkle.get_param('brightness_min')}-{style_twinkle.get_param('brightness_max')}")
end

print("\n=== Twinkle Animation Demo Complete ===")
print("The twinkle animation provides realistic twinkling effects with:")
print("- Random light placement and timing")
print("- Configurable density and fade characteristics")
print("- Support for custom colors and palettes")
print("- Multiple preset styles (gentle, classic, intense)")
print("- Real-time parameter updates")
print("- Integration with the animation framework")

return true