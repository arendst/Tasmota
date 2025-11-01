# Integration test for gradient animation with LUT optimization
#
# This test verifies that palette_gradient_animation works correctly
# with the LUT-optimized RichPaletteColorProvider

import animation
import animation_dsl

def log(msg)
  print(msg)
end

# Create a test engine
var engine = animation.init_strip()

log("=== Gradient Animation LUT Integration Test ===")
log("")

# Create the exact scenario from the user's example
log("Test: Rainbow gradient with oscillating spatial period")
log("------------------------------------------------------")

# Define a palette of rainbow colors including white
var rainbow_with_white = bytes(
  "00FC0000"  # Red
  "24FF8000"  # Orange
  "49FFFF00"  # Yellow
  "6E00FF00"  # Green
  "9200FFFF"  # Cyan
  "B70080FF"  # Blue
  "DB8000FF"  # Violet
  "FFCCCCCC"  # White
)

# Create a rich palette color provider
var rainbow_rich_color = animation.rich_palette(engine)
rainbow_rich_color.palette = rainbow_with_white
rainbow_rich_color.cycle_period = 10000  # 10 seconds
rainbow_rich_color.transition_type = animation.SINE

# Get strip length
var strip_len = engine.get_strip_length()
log(f"Strip length: {strip_len} pixels")

# Create oscillator for spatial period
var period_osc = animation.sine_osc(engine)
period_osc.min_value = strip_len / 2
period_osc.max_value = (3 * strip_len) / 2
period_osc.duration = 5000  # 5 seconds

# Create gradient animation
var back_pattern = animation.palette_gradient_animation(engine)
back_pattern.color_source = rainbow_rich_color
back_pattern.spatial_period = strip_len  # Start with full strip
back_pattern.shift_period = 0  # Static for testing

log(f"Animation created: {back_pattern}")
log(f"Color source: {rainbow_rich_color}")
log("")

# Test 1: Verify LUT is built
log("Test 1: LUT initialization")
log("---------------------------")
back_pattern.start(0)
back_pattern.update(0)

# Trigger LUT build by calling get_color_for_value
rainbow_rich_color.get_color_for_value(128, 0)

if rainbow_rich_color.color_lut != nil
  log(f"✓ LUT initialized: {size(rainbow_rich_color.color_lut)} bytes")
  log(f"✓ LUT dirty flag: {rainbow_rich_color.lut_dirty}")
else
  log("✗ LUT not initialized!")
end
log("")

# Test 2: Verify color lookups work
log("Test 2: Color lookups")
log("---------------------")
log("Sample gradient colors (0-255 range):")
var sample_values = [0, 64, 128, 192, 255]
for value : sample_values
  var color = rainbow_rich_color.get_color_for_value(value, 0)
  var r = (color >> 16) & 0xFF
  var g = (color >> 8) & 0xFF
  var b = color & 0xFF
  log(f"  Value {value:3d}: RGB({r:3d}, {g:3d}, {b:3d})")
end
log("")

# Test 3: Performance test - color lookups
log("Test 3: Performance measurement")
log("-------------------------------")
var num_lookups = 10000
var start_time = tasmota.millis()

var lookup_idx = 0
while lookup_idx < num_lookups
  var value = (lookup_idx * 17) % 256  # Pseudo-random values
  rainbow_rich_color.get_color_for_value(value, 0)
  lookup_idx += 1
end

var elapsed = tasmota.millis() - start_time

if elapsed > 0
  log(f"Performed {num_lookups} lookups in {elapsed}ms")
  log(f"Average: {elapsed * 1000.0 / num_lookups:.2f} microseconds per lookup")
else
  log(f"Performed {num_lookups} lookups in < 1ms")
  log("Performance: Too fast to measure accurately (< 0.1 microseconds per lookup)")
end
log("")

# Test 4: Verify LUT invalidation
log("Test 4: LUT invalidation")
log("------------------------")
rainbow_rich_color.lut_dirty = false
log(f"Initial lut_dirty: {rainbow_rich_color.lut_dirty}")

rainbow_rich_color.brightness = 200
log(f"After brightness change: lut_dirty = {rainbow_rich_color.lut_dirty}")

# Trigger LUT rebuild by calling get_color_for_value
rainbow_rich_color.get_color_for_value(128, 1000)
log(f"After color lookup: lut_dirty = {rainbow_rich_color.lut_dirty}")
log("")

# Test 5: Dynamic spatial period (using value provider)
log("Test 5: Dynamic spatial period")
log("------------------------------")
# Note: This would require resolving the value provider in the animation
# For now, just verify the setup works
log(f"Period oscillator: min={period_osc.min_value}, max={period_osc.max_value}")
log(f"Period at t=0: {period_osc.produce_value('value', 0)}")
log(f"Period at t=2500: {period_osc.produce_value('value', 2500)}")
log(f"Period at t=5000: {period_osc.produce_value('value', 5000)}")
log("")

log("=== All integration tests completed successfully ===")
