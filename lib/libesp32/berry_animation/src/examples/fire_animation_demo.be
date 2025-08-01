# Fire Animation Demo
# Demonstrates the FireAnimation class with various configurations

import "animation" as animation

print("=== Fire Animation Demo ===")

# Demo configuration
var STRIP_LENGTH = 30
var DEMO_DURATION = 10000  # 10 seconds per demo

print(f"Strip length: {STRIP_LENGTH} pixels")
print(f"Demo duration: {DEMO_DURATION}ms per animation")

# Demo 1: Classic Fire Effect
print("\n--- Demo 1: Classic Fire Effect ---")
var classic_fire = animation.fire_animation.classic(180, STRIP_LENGTH, 10)
print(f"Created classic fire: {classic_fire}")
print("This creates a realistic fire effect with warm colors")
print("- Base intensity: 180/255")
print("- Flicker speed: 8 Hz")
print("- Uses built-in fire palette (black -> dark red -> red -> orange -> yellow)")

# Demo 2: Intense Fire
print("\n--- Demo 2: Intense Fire ---")
var intense_fire = animation.fire_animation(nil, 255, 12, 150, 40, 180, STRIP_LENGTH, 10, 0, true, "intense_fire")
print(f"Created intense fire: {intense_fire}")
print("This creates a more intense fire with:")
print("- Maximum intensity: 255/255")
print("- Faster flicker: 12 Hz")
print("- More flicker variation: 150/255")
print("- Faster cooling: 40/255")
print("- More sparks: 180/255")

# Demo 3: Gentle Fire
print("\n--- Demo 3: Gentle Fire ---")
var gentle_fire = animation.fire_animation(nil, 120, 4, 50, 80, 80, STRIP_LENGTH, 10, 0, true, "gentle_fire")
print(f"Created gentle fire: {gentle_fire}")
print("This creates a gentler fire with:")
print("- Lower intensity: 120/255")
print("- Slower flicker: 4 Hz")
print("- Less flicker variation: 50/255")
print("- Slower cooling: 80/255")
print("- Fewer sparks: 80/255")

# Demo 4: Single Color Fire
print("\n--- Demo 4: Single Color Fire (Blue Flame) ---")
var blue_fire = animation.fire_animation.solid(0xFF0080FF, 200, STRIP_LENGTH, 10)
print(f"Created blue fire: {blue_fire}")
print("This creates a blue flame effect using a single color")
print("- Color: Blue (0xFF0080FF)")
print("- Intensity: 200/255")

# Demo 5: Custom Palette Fire
print("\n--- Demo 5: Custom Palette Fire (Green Flame) ---")
# Create a green fire palette
var green_fire_palette = bytes(
  "00000000"    # Black (value 0)
  "40004000"    # Dark green (value 64)
  "8000FF00"    # Green (value 128)
  "C080FF80"    # Light green (value 192)
  "FFFFFF80"    # Very light green (value 255)
)
var green_fire = animation.fire_animation.palette(green_fire_palette, 180, STRIP_LENGTH, 10)
print(f"Created green fire: {green_fire}")
print("This creates a green flame effect using a custom palette")

# Demo 6: Animation Controller Integration
print("\n--- Demo 6: Animation Controller Integration ---")
print("Creating animation controller and adding multiple fire effects...")

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

var mock_strip = MockStrip(STRIP_LENGTH)
var controller = animation.animation_controller(mock_strip, 255)

# Add multiple fire animations with different priorities
controller.add_animation(classic_fire)
controller.add_animation(blue_fire.set_priority(5))  # Lower priority

print("Added classic fire (priority 10) and blue fire (priority 5)")
print("The classic fire will render on top of the blue fire")

# Demo 7: Real-time Parameter Updates
print("\n--- Demo 7: Real-time Parameter Updates ---")
var dynamic_fire = animation.fire_animation.classic(100, STRIP_LENGTH, 10)
print(f"Created dynamic fire with initial intensity: {dynamic_fire.get_param('intensity')}")

# Simulate parameter changes over time
var intensities = [100, 150, 200, 255, 180, 120]
for i: 0..size(intensities)-1
  var new_intensity = intensities[i]
  dynamic_fire.set_intensity(new_intensity)
  print(f"Updated intensity to: {new_intensity}")
end

# Demo 8: Performance Considerations
print("\n--- Demo 8: Performance Considerations ---")
print("Fire animation performance tips:")
print("- Lower flicker_speed reduces CPU usage (fewer updates per second)")
print("- Smaller strip_length reduces memory usage and computation")
print("- Lower flicker_amount reduces random number generation overhead")
print("- Use solid colors instead of palettes for maximum performance")

# Create a performance-optimized fire
var perf_fire = animation.fire_animation.solid(0xFFFF4500, 150, STRIP_LENGTH, 10)
perf_fire.set_flicker_speed(4)  # Slower updates
perf_fire.set_flicker_amount(30)  # Less randomness
print(f"Created performance-optimized fire: {perf_fire}")

# Demo 9: Simulation Test
print("\n--- Demo 9: Fire Simulation Test ---")
print("Running fire simulation for a few cycles...")

var test_fire = animation.fire_animation.classic(180, 10, 10)  # Smaller strip for easier visualization
test_fire.start()

var frame = animation.frame_buffer(10)
var start_time = 1000

for cycle: 0..4
  var current_time = start_time + (cycle * 125)  # 8 Hz = 125ms intervals
  test_fire.update(current_time)
  test_fire.render(frame, current_time)
  
  print(f"Cycle {cycle + 1}:")
  var heat_info = "  Heat pattern: "
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
    heat_info += f"{brightness:3d} "
  end
  print(heat_info)
end

print("\n=== Fire Animation Demo Complete ===")
print("The fire animation provides realistic flame effects with:")
print("- Heat-based simulation with cooling and sparking")
print("- Configurable intensity, flicker, and timing")
print("- Support for custom colors and palettes")
print("- Real-time parameter updates")
print("- Integration with the animation framework")

return true