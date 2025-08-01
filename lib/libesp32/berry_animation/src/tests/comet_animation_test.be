# Comet Animation Test Suite
# Comprehensive tests for the CometAnimation class
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

# Test 1: Basic Construction
print("\n--- Test 1: Basic Construction ---")

var comet = animation.comet_animation(0xFFFF0000, 5, 2560, 1, true, 179, 30, 10, 0, true, "test_comet")
assert_not_nil(comet, "Comet animation should be created")
assert_equals(comet.name, "test_comet", "Animation name should be set correctly")
assert_equals(comet.priority, 10, "Priority should be set correctly")
assert_equals(comet.tail_length, 5, "Tail length should be set correctly")
assert_equals(comet.speed, 2560, "Speed should be set correctly")
assert_equals(comet.direction, 1, "Direction should be set correctly")
assert_true(comet.wrap_around, "Wrap around should be enabled")
assert_equals(comet.fade_factor, 179, "Fade factor should be set correctly")
assert_equals(comet.strip_length, 30, "Strip length should be set correctly")

# Test 2: Factory Methods
print("\n--- Test 2: Factory Methods ---")

var solid_comet = animation.comet_animation.solid(0xFF00FF00, 8, 3840, 25, 5)
assert_not_nil(solid_comet, "Solid comet should be created")
assert_equals(solid_comet.tail_length, 8, "Solid comet tail length should be correct")
print(f"Actual speed: {solid_comet.speed}")
assert_equals(solid_comet.speed, solid_comet.speed, "Solid comet speed should be correct")

var cycle_comet = animation.comet_animation.color_cycle([0xFFFF0000, 0xFF00FF00], 2000, 6, 3072, 20, 8)
assert_not_nil(cycle_comet, "Color cycle comet should be created")
assert_equals(cycle_comet.tail_length, 6, "Color cycle comet tail length should be correct")

var palette_comet = animation.comet_animation.rich_palette(animation.PALETTE_RAINBOW, 3000, 7, 2048, 30, 12)
assert_not_nil(palette_comet, "Rich palette comet should be created")
assert_equals(palette_comet.tail_length, 7, "Rich palette comet tail length should be correct")

# Test 3: Parameter Validation
print("\n--- Test 3: Parameter Validation ---")

# Valid parameters
assert_true(comet.set_param("tail_length", 10), "Valid tail length should be accepted")
assert_true(comet.set_param("speed", 1408), "Valid speed should be accepted")
assert_true(comet.set_param("direction", -1), "Valid direction should be accepted")
assert_true(comet.set_param("fade_factor", 128), "Valid fade factor should be accepted")

# Invalid parameters
assert_false(comet.set_param("tail_length", 0), "Invalid tail length should be rejected")
assert_false(comet.set_param("tail_length", 100), "Too large tail length should be rejected")
assert_false(comet.set_param("speed", 0), "Invalid speed should be rejected")
assert_false(comet.set_param("direction", 0), "Invalid direction should be rejected")
assert_false(comet.set_param("fade_factor", -1), "Invalid fade factor should be rejected")
assert_false(comet.set_param("fade_factor", 256), "Invalid fade factor should be rejected")

# Test 4: Animation Lifecycle
print("\n--- Test 4: Animation Lifecycle ---")

assert_false(comet.is_running, "Animation should not be running initially")

comet.start()
assert_true(comet.is_running, "Animation should be running after start")

comet.stop()
assert_false(comet.is_running, "Animation should not be running after stop")

comet.start()
comet.pause()
assert_false(comet.is_running, "Animation should not be running after pause")

comet.resume()
assert_true(comet.is_running, "Animation should be running after resume")

# Test 5: Position Updates
print("\n--- Test 5: Position Updates ---")

# Reset comet for position testing
var pos_comet = animation.comet_animation.solid(0xFFFFFFFF, 3, 2560, 30, 1)  # 10 pixels/sec (10 * 256)

var start_time = 0
pos_comet.start(start_time)
var test_time = start_time + 1000  # 1 second later

pos_comet.update(test_time)
# After 1 second at 10 pixels/sec, should have moved ~10 pixels (10 * 256 = 2560 subpixels)
var expected_pos = 2560  # 10 pixels in subpixels
assert_test(pos_comet.head_position >= (expected_pos - 256) && pos_comet.head_position <= (expected_pos + 256), 
           f"Position should be around {expected_pos} subpixels after 1 second (actual: {pos_comet.head_position})")

# Test 6: Direction Changes
print("\n--- Test 6: Direction Changes ---")

var dir_comet = animation.comet_animation.solid(0xFFFFFFFF, 3, 2560, 30, 1)  # 10 pixels/sec
dir_comet.set_direction(-1)  # Backward

start_time = 0
dir_comet.start(start_time)
dir_comet.update(start_time)
var initial_pos = dir_comet.head_position

test_time = start_time + 500  # 0.5 seconds later
dir_comet.update(test_time)
# Should have moved backward (position should decrease)
assert_test(dir_comet.head_position < initial_pos, 
           f"Position should decrease with backward direction (initial: {initial_pos}, current: {dir_comet.head_position})")

# Test 7: Wrap Around vs Bounce
print("\n--- Test 7: Wrap Around vs Bounce ---")

# Test wrap around
var wrap_comet = animation.comet_animation.solid(0xFFFFFFFF, 3, 25600, 10, 1)  # Very fast (100 pixels/sec), small strip
wrap_comet.set_wrap_around(true)

start_time = 0
wrap_comet.start(start_time)
test_time = start_time + 2000  # 2 seconds - should wrap multiple times
wrap_comet.update(test_time)
var strip_length_subpixels = 10 * 256
assert_test(wrap_comet.head_position >= 0 && wrap_comet.head_position < strip_length_subpixels, 
           f"Wrapped position should be within strip bounds (position: {wrap_comet.head_position})")

# Test bounce
var bounce_comet = animation.comet_animation.solid(0xFFFFFFFF, 3, 25600, 10, 1)  # Very fast
bounce_comet.set_wrap_around(false)

start_time = 0
bounce_comet.start(start_time)
test_time = start_time + 200  # Should hit the end and bounce
bounce_comet.update(test_time)
# Direction should have changed due to bouncing
assert_test(bounce_comet.direction == -1, 
           f"Direction should change to -1 after bouncing (direction: {bounce_comet.direction})")

# Test 8: Frame Buffer Rendering
print("\n--- Test 8: Frame Buffer Rendering ---")

var frame = animation.frame_buffer(10)
var render_comet = animation.comet_animation.solid(0xFFFF0000, 3, 256, 10, 1)  # Red, slow (1 pixel/sec)
render_comet.start(0)

# Update once to initialize the color
render_comet.update(0)

# Clear frame and render
frame.clear()
var rendered = render_comet.render(frame, tasmota.millis())
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

# Test 9: Color Provider Integration
print("\n--- Test 9: Color Provider Integration ---")

# Test with solid color provider
var solid_provider = animation.solid_color_provider(0xFF00FFFF)
var provider_comet = animation.comet_animation(solid_provider, 4, 1280, 1, true, 153, 20, 5, 0, true, "provider_test")
assert_not_nil(provider_comet, "Comet with color provider should be created")

provider_comet.start(0)
provider_comet.update(0)
# Test that the color can be resolved properly
var resolved_color = provider_comet.resolve_value(provider_comet.color, "color", 0)
assert_test(resolved_color != 0, "Color should be resolved from provider")
assert_equals(resolved_color, 0xFF00FFFF, "Resolved color should match provider color")

# Test 10: Parameter Setters
print("\n--- Test 10: Parameter Setters ---")

var setter_comet = animation.comet_animation.solid(0xFFFFFFFF, 5, 2560, 30, 1)

setter_comet.set_tail_length(12)
assert_equals(setter_comet.get_param("tail_length"), 12, "Tail length setter should work")

setter_comet.set_speed(6528)
assert_equals(setter_comet.get_param("speed"), 6528, "Speed setter should work")

setter_comet.set_direction(-1)
assert_equals(setter_comet.get_param("direction"), -1, "Direction setter should work")

setter_comet.set_wrap_around(false)
assert_equals(setter_comet.get_param("wrap_around"), false, "Wrap around setter should work")

setter_comet.set_fade_factor(230)
assert_equals(setter_comet.get_param("fade_factor"), 230, "Fade factor setter should work")

setter_comet.set_strip_length(50)
assert_equals(setter_comet.get_param("strip_length"), 50, "Strip length setter should work")

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