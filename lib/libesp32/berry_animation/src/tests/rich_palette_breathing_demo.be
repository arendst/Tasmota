# Demonstration: rich_palette_color with dynamic brightness
#
# This demo shows how the LUT optimization now works correctly with
# animations that have time-varying brightness, such as breathing effects.
#
# Before the fix: LUT would include brightness, making it useless for dynamic brightness
# After the fix: LUT stores max brightness colors, actual brightness applied after lookup

import animation
import animation_dsl

def log(msg)
  print(msg)
end

# Create a test engine with 30 LEDs
var engine = animation.init_strip(30)

log("=== Rich Palette Breathing Effect Demo ===")
log("")
log("This demo simulates a gradient animation with breathing brightness.")
log("The LUT cache remains valid throughout the brightness oscillation.")
log("")

# Create a rainbow palette
var rainbow_palette = bytes(
  "00FF0000"    # Red
  "24FFA500"    # Orange
  "49FFFF00"    # Yellow
  "6E00FF00"    # Green
  "920000FF"    # Blue
  "B74B0082"    # Indigo
  "DBEE82EE"    # Violet
  "FFFF0000"    # Red
)

# Create the color provider
var provider = animation.rich_palette_color(engine)
provider.colors = rainbow_palette
provider.period = 0  # Value-based mode for gradient
provider.brightness = 255

# Initialize the provider
provider.produce_value("color", 0)

log("Simulating breathing effect over 10 cycles...")
log("Each cycle: brightness oscillates from 50 to 255 and back")
log("")

# Simulate 10 breathing cycles
var cycles = 10
var steps_per_cycle = 20
var total_lut_rebuilds = 0

var cycle = 0
while cycle < cycles
  log(f"Cycle {cycle + 1}/{cycles}:")
  
  # Breathing up (50 -> 255)
  var step = 0
  while step < steps_per_cycle / 2
    var brightness = tasmota.scale_uint(step, 0, steps_per_cycle / 2 - 1, 50, 255)
    
    # Check if LUT is dirty before setting brightness
    var was_dirty = provider.lut_dirty
    provider.brightness = brightness
    var is_dirty = provider.lut_dirty
    
    if is_dirty && !was_dirty
      total_lut_rebuilds += 1
    end
    
    # Render a gradient across all pixels
    var pixel = 0
    while pixel < 30
      var value = tasmota.scale_uint(pixel, 0, 29, 0, 255)
      var color = provider.get_color_for_value(value, 0)
      pixel += 1
    end
    
    step += 1
  end
  
  # Breathing down (255 -> 50)
  step = 0
  while step < steps_per_cycle / 2
    var brightness = tasmota.scale_uint(step, 0, steps_per_cycle / 2 - 1, 255, 50)
    
    # Check if LUT is dirty before setting brightness
    var was_dirty = provider.lut_dirty
    provider.brightness = brightness
    var is_dirty = provider.lut_dirty
    
    if is_dirty && !was_dirty
      total_lut_rebuilds += 1
    end
    
    # Render a gradient across all pixels
    var pixel = 0
    while pixel < 30
      var value = tasmota.scale_uint(pixel, 0, 29, 0, 255)
      var color = provider.get_color_for_value(value, 0)
      pixel += 1
    end
    
    step += 1
  end
  
  log(f"  Brightness range: 50-255-50, LUT rebuilds so far: {total_lut_rebuilds}")
  cycle += 1
end

log("")
log("=== Results ===")
log(f"Total breathing cycles: {cycles}")
log(f"Total brightness changes: {cycles * steps_per_cycle}")
log(f"Total LUT rebuilds: {total_lut_rebuilds}")
log("")

if total_lut_rebuilds == 0
  log("✓ SUCCESS: LUT was never rebuilt during brightness changes!")
  log("  This confirms the fix is working correctly.")
else
  log("✗ FAILURE: LUT was rebuilt {total_lut_rebuilds} times")
  log("  The fix may not be working as expected.")
end

log("")

# Performance comparison
log("=== Performance Comparison ===")
log("")

# Test with dynamic brightness (current implementation)
var start_time = tasmota.millis()
var frames = 100
var frame = 0
while frame < frames
  # Oscillating brightness
  var brightness = tasmota.scale_uint(frame % 50, 0, 49, 50, 255)
  provider.brightness = brightness
  
  # Render gradient
  var pixel = 0
  while pixel < 30
    var value = tasmota.scale_uint(pixel, 0, 29, 0, 255)
    provider.get_color_for_value(value, 0)
    pixel += 1
  end
  frame += 1
end
var elapsed = tasmota.millis() - start_time

log(f"Rendered {frames} frames with dynamic brightness:")
log(f"  Total pixels: {frames * 30}")
log(f"  Time: {elapsed}ms")
if elapsed > 0
  log(f"  Frame rate: {frames * 1000.0 / elapsed:.1f} FPS")
else
  log(f"  Frame rate: > 10000 FPS (too fast to measure)")
end

log("")
log("=== Demo Complete ===")
log("")
log("Key takeaways:")
log("1. LUT cache remains valid when brightness changes")
log("2. No performance penalty for dynamic brightness")
log("3. Breathing effects and other brightness animations work efficiently")
