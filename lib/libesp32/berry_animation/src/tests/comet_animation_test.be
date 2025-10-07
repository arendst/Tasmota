# Comet Animation Test Suite
# Comprehensive tests for the CometAnimation class following parameterized class specification
#
# Command to run:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/comet_animation_test.be

import animation

print("=== Comet Animation Test Suite ===")

var test_count = 0
var pass_count = 0

def assert_test(condition, message)
  test_count += 1
  if condition
    pass_count += 1
    print(f"✓ PASS: {message}")
  else
    print(f"✗ FAIL: {message}")
  end
end

def assert_equals(actual, expected, message)
  assert_test(actual == expected, f"{message} (expected: {expected}, actual: {actual})")
end

def assert_not_nil(value, message)
  assert_test(value != nil, f"{message} (value should not be nil)")
end

def assert_true(condition, message)
  assert_test(condition == true, message)
end

def assert_false(condition, message)
  assert_test(condition == false, message)
end

# Create LED strip and animation engine following specification
var strip = global.Leds(30)  # Use global.Leds() for testing as per specification
var engine = animation.create_engine(strip)
print("Created LED strip and animation engine")

# Test 1: Basic Construction
print("\n--- Test 1: Basic Construction ---")

var comet = animation.comet_animation(engine)
assert_not_nil(comet, "Comet animation should be created")
assert_equals(comet.engine, engine, "Animation should have correct engine reference")

# Test default values
assert_equals(comet.color, 0xFFFFFFFF, "Default color should be white")
assert_equals(comet.tail_length, 5, "Default tail length should be 5")
assert_equals(comet.speed, 2560, "Default speed should be 2560")
assert_equals(comet.direction, 1, "Default direction should be 1 (forward)")
assert_equals(comet.wrap_around, 1, "Default wrap around should be enabled")
assert_equals(comet.fade_factor, 179, "Default fade factor should be 179")

# Test parameter assignment using virtual members
comet.color = 0xFFFF0000
comet.tail_length = 8
comet.speed = 5120
comet.direction = -1
comet.wrap_around = 0
comet.fade_factor = 150
comet.priority = 15
comet.name = "test_comet"

assert_equals(comet.color, 0xFFFF0000, "Color should be set correctly")
assert_equals(comet.tail_length, 8, "Tail length should be set correctly")
assert_equals(comet.speed, 5120, "Speed should be set correctly")
assert_equals(comet.direction, -1, "Direction should be set correctly")
assert_equals(comet.wrap_around, 0, "Wrap around should be disabled")
assert_equals(comet.fade_factor, 150, "Fade factor should be set correctly")
assert_equals(comet.priority, 15, "Priority should be set correctly")
assert_equals(comet.name, "test_comet", "Name should be set correctly")

# Test 2: Multiple Comet Animations
print("\n--- Test 2: Multiple Comet Animations ---")

var comet2 = animation.comet_animation(engine)
comet2.color = 0xFF00FF00
comet2.tail_length = 8
comet2.speed = 3840
assert_not_nil(comet2, "Second comet should be created")
assert_equals(comet2.tail_length, 8, "Second comet tail length should be correct")
assert_equals(comet2.speed, 3840, "Second comet speed should be correct")

var comet3 = animation.comet_animation(engine)
comet3.color = 0xFF0000FF
comet3.tail_length = 6
comet3.speed = 3072
assert_not_nil(comet3, "Third comet should be created")
assert_equals(comet3.tail_length, 6, "Third comet tail length should be correct")

# Test 3: Parameter Validation
print("\n--- Test 3: Parameter Validation ---")

# Valid parameters using virtual member assignment
comet.tail_length = 10
assert_equals(comet.tail_length, 10, "Valid tail length should be accepted")

comet.speed = 1408
assert_equals(comet.speed, 1408, "Valid speed should be accepted")

comet.direction = -1
assert_equals(comet.direction, -1, "Valid direction should be accepted")

comet.fade_factor = 128
assert_equals(comet.fade_factor, 128, "Valid fade factor should be accepted")

# Test parameter validation with invalid values
try
  comet.tail_length = 0  # Should fail validation (min is 1)
  assert_test(false, "Should have failed validation for tail_length = 0")
except "value_error"
  assert_test(true, "Parameter validation correctly rejected tail_length = 0")
end

try
  comet.tail_length = 100  # Should fail validation (max is 50)
  assert_test(false, "Should have failed validation for tail_length = 100")
except "value_error"
  assert_test(true, "Parameter validation correctly rejected tail_length = 100")
end

try
  comet.direction = 0  # Should fail validation (enum is [-1, 1])
  assert_test(false, "Should have failed validation for direction = 0")
except "value_error"
  assert_test(true, "Parameter validation correctly rejected direction = 0")
end

try
  comet.fade_factor = 300  # Should fail validation (max is 255)
  assert_test(false, "Should have failed validation for fade_factor = 300")
except "value_error"
  assert_test(true, "Parameter validation correctly rejected fade_factor = 300")
end

# Test 4: Position Updates
print("\n--- Test 4: Position Updates ---")

# Create comet for position testing
var pos_comet = animation.comet_animation(engine)
pos_comet.color = 0xFFFFFFFF
pos_comet.tail_length = 3
pos_comet.speed = 2560  # 10 pixels/sec (10 * 256)

# Use engine time for testing
engine.time_ms = 1000
var start_time = engine.time_ms
pos_comet.start(start_time)
pos_comet.update(start_time)

engine.time_ms = start_time + 1000  # 1 second later
pos_comet.update(engine.time_ms)

# After 1 second at 10 pixels/sec, should have moved ~10 pixels (10 * 256 = 2560 subpixels)
var expected_pos = 2560  # 10 pixels in subpixels
assert_test(pos_comet.head_position >= (expected_pos - 256) && pos_comet.head_position <= (expected_pos + 256), 
           f"Position should be around {expected_pos} subpixels after 1 second (actual: {pos_comet.head_position})")

# Test 5: Direction Changes
print("\n--- Test 5: Direction Changes ---")

var dir_comet = animation.comet_animation(engine)
dir_comet.color = 0xFFFFFFFF
dir_comet.tail_length = 3
dir_comet.speed = 2560  # 10 pixels/sec
dir_comet.direction = -1  # Backward

engine.time_ms = 2000
start_time = engine.time_ms
dir_comet.start(start_time)
dir_comet.update(start_time)
var initial_pos = dir_comet.head_position

engine.time_ms = start_time + 500  # 0.5 seconds later
dir_comet.update(engine.time_ms)
# Should have moved backward (position should decrease)
assert_test(dir_comet.head_position < initial_pos, 
           f"Position should decrease with backward direction (initial: {initial_pos}, current: {dir_comet.head_position})")

# Test 6: Wrap Around vs Bounce
print("\n--- Test 6: Wrap Around vs Bounce ---")

# Create smaller strip for faster testing
var small_strip = global.Leds(10)
var small_engine = animation.create_engine(small_strip)

# Test wrap around
var wrap_comet = animation.comet_animation(small_engine)
wrap_comet.color = 0xFFFFFFFF
wrap_comet.tail_length = 3
wrap_comet.speed = 25600  # Very fast (100 pixels/sec)
wrap_comet.wrap_around = 1  # Enable wrapping

small_engine.time_ms = 3000
start_time = small_engine.time_ms
wrap_comet.start(start_time)
wrap_comet.update(start_time)
small_engine.time_ms = start_time + 2000  # 2 seconds - should wrap multiple times
wrap_comet.update(small_engine.time_ms)
var strip_length_subpixels = 10 * 256
assert_test(wrap_comet.head_position >= 0 && wrap_comet.head_position < strip_length_subpixels, 
           f"Wrapped position should be within strip bounds (position: {wrap_comet.head_position})")

# Test bounce
var bounce_comet = animation.comet_animation(small_engine)
bounce_comet.color = 0xFFFFFFFF
bounce_comet.tail_length = 3
bounce_comet.speed = 25600  # Very fast
bounce_comet.wrap_around = 0  # Disable wrapping (enable bouncing)

small_engine.time_ms = 4000
start_time = small_engine.time_ms
bounce_comet.start(start_time)
bounce_comet.update(small_engine.time_ms)
small_engine.time_ms = start_time + 200  # Should hit the end and bounce
bounce_comet.update(small_engine.time_ms)
# Direction should have changed due to bouncing
assert_test(bounce_comet.direction == -1, 
           f"Direction should change to -1 after bouncing (direction: {bounce_comet.direction})")

# Test 7: Frame Buffer Rendering
print("\n--- Test 7: Frame Buffer Rendering ---")

var frame = animation.frame_buffer(10)
var render_comet = animation.comet_animation(small_engine)
render_comet.color = 0xFFFF0000  # Red
render_comet.tail_length = 3
render_comet.speed = 256  # Slow (1 pixel/sec)

small_engine.time_ms = 5000
render_comet.start(small_engine.time_ms)
render_comet.update(small_engine.time_ms)

# Clear frame and render
frame.clear()
var rendered = render_comet.render(frame, small_engine.time_ms)
assert_true(rendered, "Render should return true when successful")

# Check that pixels were set (comet should be at position 0 with tail)
var head_color = frame.get_pixel_color(0)  # Head at position 0
assert_test(head_color != 0, "Head pixel should have color")

# Check tail pixels have lower brightness (tail wraps around to end of strip)
var tail_color = frame.get_pixel_color(9)  # Tail pixel
assert_test(tail_color != 0, "Tail pixel should have some color")

# Extract alpha components to compare transparency (alpha-based fading)
var head_alpha = (head_color >> 24) & 0xFF
var tail_alpha = (tail_color >> 24) & 0xFF
assert_test(head_alpha > tail_alpha, f"Head should be less transparent than tail (head alpha: {head_alpha}, tail alpha: {tail_alpha})")

# Test 8: Color Provider Integration
print("\n--- Test 8: Color Provider Integration ---")

# Test with solid color provider
var solid_provider = animation.static_color(engine)
solid_provider.color = 0xFF00FFFF
var provider_comet = animation.comet_animation(engine)
provider_comet.color = solid_provider
provider_comet.tail_length = 4
provider_comet.speed = 1280

assert_not_nil(provider_comet, "Comet with color provider should be created")

engine.time_ms = 6000
provider_comet.start(engine.time_ms)
provider_comet.update(engine.time_ms)

# Test that the color can be resolved properly through virtual member access
var resolved_color = provider_comet.color
assert_test(resolved_color != 0, "Color should be resolved from provider")
assert_equals(resolved_color, 0xFF00FFFF, "Resolved color should match provider color")

# Test 9: Engine Integration
print("\n--- Test 9: Engine Integration ---")

var engine_comet = animation.comet_animation(engine)
engine_comet.color = 0xFFFFFFFF
engine_comet.tail_length = 5
engine_comet.speed = 2560

# Test adding to engine
engine.add(engine_comet)
assert_test(true, "Animation should be added to engine successfully")

# Test strip length from engine
var strip_length = engine_comet.engine.get_strip_length()
assert_equals(strip_length, 30, "Strip length should come from engine")

# Test engine time usage
engine.time_ms = 7000
engine_comet.start(engine.time_ms)
engine_comet.update(engine.time_ms)
assert_equals(engine_comet.start_time, 7000, "Animation should use engine time for start")

# Test Results
print(f"\n=== Test Results ===")
print(f"Tests run: {test_count}")
print(f"Tests passed: {pass_count}")
print(f"Tests failed: {test_count - pass_count}")
print(f"Success rate: {(pass_count * 100) / test_count:.1f}%")

if pass_count == test_count
  print("🎉 All tests passed!")
else
  print("❌ Some tests failed. Please review the implementation.")
  raise "test_failed"
end

print("=== Comet Animation Test Suite Complete ===")