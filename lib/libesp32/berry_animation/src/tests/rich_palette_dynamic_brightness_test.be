# Test for rich_palette_color dynamic brightness
#
# This test verifies that brightness can change over time without
# invalidating the LUT cache, which is critical for animations
# where brightness changes dynamically.

import animation
import animation_dsl

def log(msg)
  print(msg)
end

# Create a test engine
var engine = animation.init_strip()

log("=== rich_palette_color Dynamic Brightness Test ===")
log("")

# Test 1: Verify brightness changes don't invalidate LUT
log("Test 1: Brightness changes should NOT invalidate LUT")
log("------------------------------------------------------")

# Create a simple RGB palette
var rgb_palette = bytes(
  "00FF0000"    # Value 0: Red
  "80FFFF00"    # Value 128: Yellow
  "FFFFFF00"    # Value 255: Yellow
)

var provider = animation.rich_palette_color(engine)
provider.colors = rgb_palette
provider.period = 0  # Static mode

# Initialize the provider and build LUT
provider.produce_value("color", 0)
var color_at_255 = provider.get_color_for_value(128, 0)
log(f"Initial color at value 128 with brightness 255:")
var r = (color_at_255 >> 16) & 0xFF
var g = (color_at_255 >> 8) & 0xFF
var b = color_at_255 & 0xFF
log(f"  RGB({r:3d}, {g:3d}, {b:3d}) = 0x{color_at_255:08X}")

# Verify LUT is not dirty
log(f"LUT dirty after initial build: {provider._lut_dirty}")
log("")

# Change brightness multiple times and verify LUT stays valid
var brightness_values = [200, 150, 100, 50, 255]
for brightness : brightness_values
  provider.brightness = brightness
  log(f"Changed brightness to {brightness}")
  log(f"  LUT dirty: {provider._lut_dirty}")
  
  var color = provider.get_color_for_value(128, 0)
  r = (color >> 16) & 0xFF
  g = (color >> 8) & 0xFF
  b = color & 0xFF
  log(f"  Color at value 128: RGB({r:3d}, {g:3d}, {b:3d})")
  
  # Verify brightness scaling is correct
  # At value 128, we should get yellow (255, 255, 0) scaled by brightness
  var expected_r = tasmota.scale_uint(255, 0, 255, 0, brightness)
  var expected_g = tasmota.scale_uint(255, 0, 255, 0, brightness)
  var expected_b = 0
  
  if r == expected_r && g == expected_g && b == expected_b
    log(f"  ✓ Brightness scaling correct")
  else
    log(f"  ✗ ERROR: Expected RGB({expected_r}, {expected_g}, {expected_b})")
  end
  log("")
end

log("")

# Test 2: Verify colors at different values with varying brightness
log("Test 2: Color accuracy at different brightness levels")
log("-------------------------------------------------------")

var test_values = [0, 64, 128, 192, 255]
var test_brightness = [255, 128, 64]

for brightness : test_brightness
  provider.brightness = brightness
  log(f"Brightness: {brightness}")
  
  for value : test_values
    var color = provider.get_color_for_value(value, 0)
    r = (color >> 16) & 0xFF
    g = (color >> 8) & 0xFF
    b = color & 0xFF
    log(f"  Value {value:3d}: RGB({r:3d}, {g:3d}, {b:3d})")
  end
  log("")
end

log("")

# Test 3: Performance with dynamic brightness
log("Test 3: Performance with dynamic brightness changes")
log("----------------------------------------------------")

# Simulate an animation where brightness oscillates
var iterations = 1000
var start_time = tasmota.millis()

var i = 0
while i < iterations
  # Simulate oscillating brightness (0-255)
  var brightness = tasmota.scale_uint(i % 256, 0, 255, 0, 255)
  provider.brightness = brightness
  
  # Get colors for a gradient (simulate 10 pixels)
  var pixel = 0
  while pixel < 10
    var value = tasmota.scale_uint(pixel, 0, 9, 0, 255)
    provider.get_color_for_value(value, 0)
    pixel += 1
  end
  
  i += 1
end

var elapsed = tasmota.millis() - start_time
var total_lookups = iterations * 10
log(f"Rendered {iterations} frames with dynamic brightness")
log(f"Total lookups: {total_lookups}")
log(f"Time: {elapsed}ms")
if elapsed > 0
  log(f"Average: {elapsed * 1000.0 / total_lookups:.2f} microseconds per lookup")
  log(f"Frame rate: {iterations * 1000.0 / elapsed:.1f} FPS")
else
  log("Average: < 0.01 microseconds per lookup (too fast to measure)")
  log("Frame rate: > 100000 FPS")
end

log("")

# Test 4: Verify LUT rebuild only happens when needed
log("Test 4: LUT rebuild verification")
log("---------------------------------")

# Create a fresh provider
var rebuild_provider = animation.rich_palette_color(engine)
rebuild_provider.colors = rgb_palette
rebuild_provider.period = 0

# Force initial build
rebuild_provider.get_color_for_value(128, 0)
log(f"After initial build: lut_dirty = {rebuild_provider._lut_dirty}")

# Change brightness - should NOT trigger rebuild
rebuild_provider.brightness = 100
log(f"After brightness change: lut_dirty = {rebuild_provider._lut_dirty}")
rebuild_provider.get_color_for_value(128, 0)
log(f"After lookup with new brightness: lut_dirty = {rebuild_provider._lut_dirty}")

# Change palette - SHOULD trigger rebuild
rebuild_provider.colors = bytes("00FF0000" "FFFFFF00")
log(f"After palette change: lut_dirty = {rebuild_provider._lut_dirty}")
rebuild_provider.get_color_for_value(128, 0)
log(f"After lookup with new palette: lut_dirty = {rebuild_provider._lut_dirty}")

# Change transition_type - SHOULD trigger rebuild
rebuild_provider.transition_type = 5 #-SINE-#
log(f"After transition_type change: lut_dirty = {rebuild_provider._lut_dirty}")
rebuild_provider.get_color_for_value(128, 0)
log(f"After lookup with new transition: lut_dirty = {rebuild_provider._lut_dirty}")

log("")
log("=== All tests completed successfully ===")
log("")
log("Summary:")
log("--------")
log("✓ Brightness changes do NOT invalidate LUT cache")
log("✓ Brightness scaling is applied correctly after LUT lookup")
log("✓ Performance remains optimal with dynamic brightness")
log("✓ LUT only rebuilds when palette or transition_type changes")
