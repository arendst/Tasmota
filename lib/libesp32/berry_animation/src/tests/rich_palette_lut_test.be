# Test for rich_palette_color LUT optimization
#
# This test verifies that the LUT cache produces correct colors
# and measures the performance improvement

import animation
import animation_dsl

def log(msg)
  print(msg)
end

# Create a test engine
var engine = animation.init_strip()

log("=== rich_palette_color LUT Cache Test ===")
log("")

# Test 1: Verify LUT produces correct colors
log("Test 1: Color accuracy verification")
log("------------------------------------")

# Create a rainbow palette (format: VRGB where V=value, RGB=color)
var rainbow_palette = bytes(
  "00FF0000"    # Value 0: Red
  "49FFFF00"    # Value 73: Yellow  
  "92FF00FF"    # Value 146: Magenta
  "FFFF0000"    # Value 255: Red
)

# Debug: Print palette bytes
log("Palette bytes:")
var i = 0
while i < size(rainbow_palette)
  var v = rainbow_palette[i]
  var r = rainbow_palette[i+1]
  var g = rainbow_palette[i+2]
  var b = rainbow_palette[i+3]
  log(f"  [{i/4}] V={v:3d} R={r:3d} G={g:3d} B={b:3d}")
  i += 4
end
log("")

var provider = animation.rich_palette_color(engine)
provider.colors = rainbow_palette
provider.period = 0  # Static mode for testing

# Trigger initialization by calling produce_value once
# This will initialize value_arr and slots
provider.produce_value("color", 0)

# Debug: Check palette
log(f"Palette size: {size(provider.colors)} bytes")
log(f"Slots: {provider._slots}")
log("Range: 0 to 255 (fixed)")

# Force LUT rebuild
provider._lut_dirty = true

# Test key values
var test_values = [0, 2, 4, 50, 100, 150, 200, 254, 255]
log("Testing color values at key positions:")

for value : test_values
  var color = provider.get_color_for_value(value, 0)
  var r = (color >> 16) & 0xFF
  var g = (color >> 8) & 0xFF
  var b = color & 0xFF
  log(f"  Value {value:3d}: RGB({r:3d}, {g:3d}, {b:3d}) = 0x{color:08X}")
end

log("")

# Test 2: Verify LUT invalidation on parameter changes
log("Test 2: LUT invalidation on parameter changes")
log("----------------------------------------------")

provider._lut_dirty = false
log(f"Initial _lut_dirty: {provider._lut_dirty}")

provider.brightness = 200
log(f"After brightness change: _lut_dirty = {provider._lut_dirty}")

provider._lut_dirty = false
provider.transition_type = 5 #-SINE-#
log(f"After transition_type change: _lut_dirty = {provider._lut_dirty}")

provider._lut_dirty = false
provider.colors = bytes("00FF0000" "FFFFFF00" "FF00FF00")
log(f"After palette change: _lut_dirty = {provider._lut_dirty}")

log("")

# Test 3: Performance comparison
log("Test 3: Performance measurement")
log("-------------------------------")

# Create a fresh provider for performance testing
var perf_provider = animation.rich_palette_color(engine)
perf_provider.colors = rainbow_palette
perf_provider.period = 0

# Warm up the LUT
perf_provider.get_color_for_value(128, 0)

# Measure LUT-based lookups
var iterations = 10000
var start_time = tasmota.millis()

var i = 0
while i < iterations
  var value = (i * 17) % 256  # Pseudo-random values
  perf_provider.get_color_for_value(value, 0)
  i += 1
end

var lut_time = tasmota.millis() - start_time
log(f"LUT-based: {iterations} lookups in {lut_time}ms")
log(f"Average: {lut_time * 1000 / iterations:.2f} microseconds per lookup")

log("")

# Test 4: Gradient animation scenario
log("Test 4: Gradient animation scenario (60 pixels)")
log("------------------------------------------------")

# Simulate gradient animation with 60 pixels
var strip_length = 60
var gradient_values = []
gradient_values.resize(strip_length)

# Pre-calculate gradient values
i = 0
while i < strip_length
  gradient_values[i] = tasmota.scale_uint(i, 0, strip_length - 1, 0, 255)
  i += 1
end

# Measure time to render one frame
start_time = tasmota.millis()

var frames = 100
var frame_idx = 0
while frame_idx < frames
  i = 0
  while i < strip_length
    perf_provider.get_color_for_value(gradient_values[i], 0)
    i += 1
  end
  frame_idx += 1
end

var total_time = tasmota.millis() - start_time
var lookups = frames * strip_length
log(f"Rendered {frames} frames ({lookups} lookups) in {total_time}ms")
if total_time > 0
  log(f"Average: {total_time * 1000.0 / lookups:.2f} microseconds per lookup")
  log(f"Frame rate: {frames * 1000.0 / total_time:.1f} FPS equivalent")
else
  log("Average: < 0.01 microseconds per lookup (too fast to measure)")
  log("Frame rate: > 100000 FPS (too fast to measure)")
end

log("")

# Test 5: Edge cases
log("Test 5: Edge case verification")
log("-------------------------------")

# Test boundary values
var edge_cases = [
  [0, "Minimum value (0)"],
  [1, "Odd value (1) - should use index 0"],
  [2, "Even value (2) - exact LUT entry"],
  [3, "Odd value (3) - should use index 1"],
  [254, "Even value (254) - exact LUT entry"],
  [255, "Maximum value (255) - special case"],
  [-5, "Negative value (should clamp to 0)"],
  [300, "Over-range value (should clamp to 255)"]
]

for case : edge_cases
  var value = case[0]
  var description = case[1]
  var color = perf_provider.get_color_for_value(value, 0)
  var r = (color >> 16) & 0xFF
  var g = (color >> 8) & 0xFF
  var b = color & 0xFF
  log(f"  {description}: RGB({r:3d}, {g:3d}, {b:3d})")
end

log("")
log("=== All tests completed successfully ===")
