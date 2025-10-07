# Animation Engine Test Suite
# Comprehensive tests for the unified AnimationEngine

import animation

print("=== Animation Engine Opcaity Test Suite ===")

# Test utilities
var test_count = 0
var passed_count = 0

def assert_test(condition, message)
  test_count += 1
  if condition
    passed_count += 1
    print(f"✓ PASS: {message}")
  else
    print(f"✗ FAIL: {message}")
  end
end

def assert_equals(actual, expected, message)
  assert_test(actual == expected, f"{message} (expected: {expected}, actual: {actual})")
end

def assert_not_nil(value, message)
  assert_test(value != nil, f"{message} (value was nil)")
end

# Test 11: Animation Opacity with Animation Masks
print("\n--- Test 11: Animation Opacity with Animation Masks ---")

# Create a fresh engine for opacity tests
var opacity_strip = global.Leds(10)
var opacity_engine = animation.create_engine(opacity_strip)

# Test 11a: Basic numeric opacity
print("\n--- Test 11a: Basic numeric opacity ---")
var base_anim = animation.solid(opacity_engine)
base_anim.color = 0xFFFF0000  # Red
base_anim.opacity = 128       # 50% opacity
base_anim.priority = 10
base_anim.name = "base_red"

opacity_engine.add(base_anim)
opacity_engine.run()

# Create frame buffer and test rendering
var opacity_frame = animation.frame_buffer(10)
base_anim.start()
var render_result = base_anim.render(opacity_frame, opacity_engine.time_ms)
base_anim.post_render(opacity_frame, opacity_engine.time_ms)

assert_test(render_result, "Animation with numeric opacity should render successfully")
assert_equals(base_anim.opacity, 128, "Numeric opacity should be preserved")

# Test 11b: Animation as opacity mask - basic setup
print("\n--- Test 11b: Animation as opacity mask - basic setup ---")

# Create opacity mask animation (pulsing from 0 to 255)
var opacity_mask = animation.solid(opacity_engine)
opacity_mask.color = 0xFF808080  # Gray (128 brightness)
opacity_mask.priority = 5
opacity_mask.name = "opacity_mask"

# Create main animation with animation opacity
var masked_anim = animation.solid(opacity_engine)
masked_anim.color = 0xFF00FF00  # Green
masked_anim.opacity = opacity_mask  # Use animation as opacity
masked_anim.priority = 15
masked_anim.name = "masked_green"

assert_test(isinstance(masked_anim.opacity, animation.animation), "Opacity should be an animation instance")
assert_equals(masked_anim.opacity.name, "opacity_mask", "Opacity animation should be correctly assigned")

# Test 11c: Animation opacity rendering
print("\n--- Test 11c: Animation opacity rendering ---")

opacity_engine.clear()
opacity_engine.add(masked_anim)
opacity_engine.run()

# Start both animations
masked_anim.start()
opacity_mask.start()

# Test rendering with animation opacity
var masked_frame = animation.frame_buffer(10)
render_result = masked_anim.render(masked_frame, opacity_engine.time_ms)
masked_anim.post_render(masked_frame, opacity_engine.time_ms)

assert_test(render_result, "Animation with animation opacity should render successfully")
assert_not_nil(masked_anim.opacity_frame, "Opacity frame buffer should be created")
assert_equals(masked_anim.opacity_frame.width, 10, "Opacity frame buffer should match main frame width")

# Test 11e: Complex opacity animation scenarios
print("\n--- Test 11e: Complex opacity animation scenarios ---")

# Create a pulsing opacity mask
var pulsing_opacity = animation.solid(opacity_engine)
pulsing_opacity.color = 0xFF000000  # Start with black (0 opacity)
pulsing_opacity.priority = 1
pulsing_opacity.name = "pulsing_opacity"

# Create animated color base
var rainbow_base = animation.solid(opacity_engine)
rainbow_base.color = 0xFFFF0000  # Red base
rainbow_base.opacity = pulsing_opacity  # Pulsing opacity
rainbow_base.priority = 20
rainbow_base.name = "rainbow_with_pulse"

# Test multiple renders with changing opacity
opacity_engine.clear()
opacity_engine.add(rainbow_base)

rainbow_base.start()
pulsing_opacity.start()

# Test simple opacity changes
var test_frame = animation.frame_buffer(10)
var base_time = 2000

# Update opacity animation color to simulate pulsing
pulsing_opacity.color = 0xFF808080  # Gray (50% opacity)

render_result = rainbow_base.render(test_frame, base_time)
rainbow_base.post_render(test_frame, base_time)
assert_test(render_result, "Complex opacity animation should render successfully")

# Test 11f: Opacity animation lifecycle management
print("\n--- Test 11f: Opacity animation lifecycle management ---")

# Test that opacity animation starts automatically when main animation renders
var auto_start_opacity = animation.solid(opacity_engine)
auto_start_opacity.color = 0xFF808080  # Gray
auto_start_opacity.priority = 1
auto_start_opacity.name = "auto_start_opacity"
auto_start_opacity.is_running = false  # Start stopped

var auto_start_main = animation.solid(opacity_engine)
auto_start_main.color = 0xFFFFFF00  # Yellow
auto_start_main.opacity = auto_start_opacity
auto_start_main.priority = 10
auto_start_main.name = "auto_start_main"

# Opacity animation should not be running initially
assert_test(!auto_start_opacity.is_running, "Opacity animation should start stopped")

# Start main animation and render
auto_start_main.start()
var auto_frame = animation.frame_buffer(10)
render_result = auto_start_main.render(auto_frame, opacity_engine.time_ms)
auto_start_main.post_render(auto_frame, opacity_engine.time_ms)

# Opacity animation should now be running
assert_test(auto_start_opacity.is_running, "Opacity animation should auto-start when main animation renders")
assert_test(render_result, "Main animation with auto-started opacity should render successfully")

# Test 11g: Nested animation opacity (animation with animation opacity)
print("\n--- Test 11g: Nested animation opacity ---")

# Create a chain: base -> opacity1 -> opacity2
var base_nested = animation.solid(opacity_engine)
base_nested.color = 0xFF00FFFF  # Cyan
base_nested.priority = 30
base_nested.name = "base_nested"

var opacity1 = animation.solid(opacity_engine)
opacity1.color = 0xFF808080  # 50% gray
opacity1.priority = 25
opacity1.name = "opacity1"

var opacity2 = animation.solid(opacity_engine)
opacity2.color = 0xFFC0C0C0  # 75% gray
opacity2.priority = 20
opacity2.name = "opacity2"

# Chain the opacities: base uses opacity1, opacity1 uses opacity2
opacity1.opacity = opacity2
base_nested.opacity = opacity1

# Test rendering with nested opacity
opacity_engine.clear()
opacity_engine.add(base_nested)

base_nested.start()
opacity1.start()
opacity2.start()

var nested_frame = animation.frame_buffer(10)
render_result = base_nested.render(nested_frame, opacity_engine.time_ms)
base_nested.post_render(nested_frame, opacity_engine.time_ms)
opacity1.post_render(nested_frame, opacity_engine.time_ms)

assert_test(render_result, "Nested animation opacity should render successfully")
assert_not_nil(base_nested.opacity_frame, "Base animation should have opacity frame buffer")
assert_not_nil(opacity1.opacity_frame, "First opacity animation should have opacity frame buffer")

# Test 11h: Opacity animation parameter changes
print("\n--- Test 11h: Opacity animation parameter changes ---")

var param_base = animation.solid(opacity_engine)
param_base.color = 0xFFFF00FF  # Magenta
param_base.priority = 10
param_base.name = "param_base"

var param_opacity = animation.solid(opacity_engine)
param_opacity.color = 0xFF404040  # Dark gray
param_opacity.priority = 5
param_opacity.name = "param_opacity"

param_base.opacity = param_opacity

# Test changing opacity animation parameters
param_base.start()
param_opacity.start()

var param_frame = animation.frame_buffer(10)
render_result = param_base.render(param_frame, opacity_engine.time_ms)
param_base.post_render(param_frame, opacity_engine.time_ms)
assert_test(render_result, "Animation should render before opacity parameter change")

# Change opacity animation color
param_opacity.color = 0xFFFFFFFF  # White (full opacity)
render_result = param_base.render(param_frame, opacity_engine.time_ms + 100)
param_base.post_render(param_frame, opacity_engine.time_ms + 100)
assert_test(render_result, "Animation should render after opacity parameter change")

# Change opacity animation to numeric value
param_base.opacity = 64  # 25% opacity
render_result = param_base.render(param_frame, opacity_engine.time_ms + 200)
param_base.post_render(param_frame, opacity_engine.time_ms + 200)
assert_test(render_result, "Animation should render after changing from animation to numeric opacity")

# Change back to animation opacity
param_base.opacity = param_opacity
render_result = param_base.render(param_frame, opacity_engine.time_ms + 300)
param_base.post_render(param_frame, opacity_engine.time_ms + 300)
assert_test(render_result, "Animation should render after changing from numeric to animation opacity")

# Test 11i: Opacity with full transparency and full opacity
print("\n--- Test 11i: Opacity edge cases ---")

var edge_base = animation.solid(opacity_engine)
edge_base.color = 0xFF0080FF  # Blue
edge_base.priority = 10
edge_base.name = "edge_base"

# Test full transparency (should still render but with no visible effect)
edge_base.opacity = 0
edge_base.start()
var edge_frame = animation.frame_buffer(10)
render_result = edge_base.render(edge_frame, opacity_engine.time_ms)
edge_base.post_render(edge_frame, opacity_engine.time_ms)
assert_test(render_result, "Animation with 0 opacity should still render")

# Test full opacity (should render normally)
edge_base.opacity = 255
render_result = edge_base.render(edge_frame, opacity_engine.time_ms + 100)
edge_base.post_render(edge_frame, opacity_engine.time_ms + 100)
assert_test(render_result, "Animation with full opacity should render normally")

# Test transparent animation as opacity
var transparent_opacity = animation.solid(opacity_engine)
transparent_opacity.color = 0x00000000  # Fully transparent
transparent_opacity.priority = 5
transparent_opacity.name = "transparent_opacity"

edge_base.opacity = transparent_opacity
transparent_opacity.start()
render_result = edge_base.render(edge_frame, opacity_engine.time_ms + 200)
edge_base.post_render(edge_frame, opacity_engine.time_ms + 200)
assert_test(render_result, "Animation with transparent animation opacity should render")

# Test 11j: Performance with animation opacity
print("\n--- Test 11j: Performance with animation opacity ---")

# Create multiple animations with animation opacity for performance testing
opacity_engine.clear()

var perf_animations = []
var perf_opacities = []

for i : 0..9
  var perf_base = animation.solid(opacity_engine)
  perf_base.color = 0xFF000000 | ((i * 25) << 16) | ((i * 15) << 8) | (i * 10)
  perf_base.priority = 50 + i
  perf_base.name = f"perf_base_{i}"
  
  var perf_opacity = animation.solid(opacity_engine)
  perf_opacity.color = 0xFF808080  # 50% gray
  perf_opacity.priority = 40 + i
  perf_opacity.name = f"perf_opacity_{i}"
  
  perf_base.opacity = perf_opacity
  
  perf_animations.push(perf_base)
  perf_opacities.push(perf_opacity)
  
  opacity_engine.add(perf_base)
end

# Start all animations
for anim : perf_animations
  anim.start()
end
for opacity : perf_opacities
  opacity.start()
end

# Performance test: render multiple times
var perf_start_time = tasmota.millis()
for i : 0..19
  opacity_engine.on_tick(perf_start_time + i * 10)
end
var perf_time = tasmota.millis() - perf_start_time

assert_test(perf_time < 300, f"20 render cycles with 10 animation opacities should be reasonable (took {perf_time}ms)")
assert_equals(opacity_engine.size(), 10, "Should have 10 animations with animation opacity")

# Verify all opacity frame buffers were created
var opacity_frames_created = 0
for anim : perf_animations
  if anim.opacity_frame != nil
    opacity_frames_created += 1
  end
end
assert_test(opacity_frames_created >= 5, f"Most animations should have opacity frame buffers created (found {opacity_frames_created})")

opacity_engine.stop()

# Test Results
print(f"\n=== Test Results ===")
print(f"Tests run: {test_count}")
print(f"Tests passed: {passed_count}")
print(f"Tests failed: {test_count - passed_count}")
print(f"Success rate: {tasmota.scale_uint(passed_count, 0, test_count, 0, 100)}%")

if passed_count == test_count
  print("🎉 All tests passed!")
else
  print("❌ Some tests failed")
  raise "test_failed"
end

print("\n=== Animation Opacity Features ===")
print("Animation opacity system supports:")
print("- Numeric opacity values (0-255)")
print("- Animation instances as opacity masks")
print("- Automatic opacity animation lifecycle management")
print("- Efficient opacity frame buffer reuse")
print("- Dynamic frame buffer resizing")
print("- Nested animation opacity chains")
print("- Real-time opacity parameter changes")
print("- High performance with multiple opacity animations")

print("\n=== Performance Benefits ===")
print("Unified AnimationEngine benefits:")
print("- Single object replacing 3 separate classes")
print("- Reduced memory overhead and allocations")
print("- Simplified API surface")
print("- Better cache locality")
print("- Fewer method calls per frame")
print("- Cleaner codebase with no deprecated APIs")
print("- Maintained full functionality")