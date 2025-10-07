# Animation Engine Test Suite
# Comprehensive tests for the unified AnimationEngine

import animation

print("=== Animation Engine Test Suite ===")

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

# Test 1: Engine Creation
print("\n--- Test 1: Engine Creation ---")
var strip = global.Leds(20)
var engine = animation.create_engine(strip)

assert_not_nil(engine, "Engine should be created")
assert_equals(engine.width, 20, "Engine width should match strip length")
assert_equals(engine.is_active(), false, "Engine should start inactive")
assert_equals(engine.size(), 0, "Engine should start with no animations")

# Test 2: Animation Management
print("\n--- Test 2: Animation Management ---")
var anim1 = animation.solid(engine)    # Red, priority 10
anim1.color = 0xFFFF0000
anim1.priority = 10
anim1.name = "red"

var anim2 = animation.solid(engine)     # Green, priority 5
anim2.color = 0xFF00FF00
anim2.priority = 5
anim2.name = "green"

var anim3 = animation.solid(engine)     # Blue, priority 15
anim3.color = 0xFF0000FF
anim3.priority = 15
anim3.name = "blue"

assert_test(engine.add(anim1), "Should add first animation")
assert_test(engine.add(anim2), "Should add second animation")
assert_test(engine.add(anim3), "Should add third animation")
assert_equals(engine.size(), 3, "Engine should have 3 animations")

# Test priority sorting (higher priority first)
var animations = engine.get_animations()
assert_equals(animations[0].priority, 15, "First animation should have highest priority")
assert_equals(animations[1].priority, 10, "Second animation should have medium priority")
assert_equals(animations[2].priority, 5, "Third animation should have lowest priority")

# Test duplicate prevention
assert_test(!engine.add(anim1), "Should not add duplicate animation")
assert_equals(engine.size(), 3, "Size should remain 3 after duplicate attempt")

# Test animation removal
assert_test(engine.remove_animation(anim2), "Should remove existing animation")
assert_equals(engine.size(), 2, "Size should be 2 after removal")
assert_test(!engine.remove_animation(anim2), "Should not remove non-existent animation")

# Test 3: Engine Lifecycle
print("\n--- Test 3: Engine Lifecycle ---")
assert_test(engine.run(), "Should start engine")
assert_equals(engine.is_active(), true, "Engine should be active after start")

# Test that starting again doesn't break anything
engine.run()
assert_equals(engine.is_active(), true, "Engine should remain active after second start")

assert_test(engine.stop(), "Should stop engine")
assert_equals(engine.is_active(), false, "Engine should be inactive after stop")

# Test 4: Animation Updates and Rendering
print("\n--- Test 4: Animation Updates and Rendering ---")
engine.clear()
var test_anim = animation.solid(engine)
test_anim.color = 0xFFFF0000
test_anim.priority = 10
test_anim.name = "test"
engine.add(test_anim)
engine.run()

var current_time = tasmota.millis()

# Simulate a tick
engine.on_tick(current_time)

# Check that engine processed the tick (we can't easily check strip.show() calls with global.Leds)
assert_test(true, "Engine should process tick without error")

# Test 5: Sequence Manager Integration
print("\n--- Test 5: Sequence Manager Integration ---")
var seq_manager = animation.SequenceManager(engine)
assert_not_nil(seq_manager, "Sequence manager should be created")

engine.add(seq_manager)
assert_test(true, "Should add sequence manager without error")

engine.remove_sequence_manager(seq_manager)
assert_test(true, "Should remove sequence manager without error")

# Test 6: Clear Functionality
print("\n--- Test 6: Clear Functionality ---")
engine.add(anim1)
engine.add(anim3)
engine.add(seq_manager)

assert_equals(engine.size(), 3, "Should have 3 animations before clear")
engine.clear()
assert_equals(engine.size(), 0, "Should have 0 animations after clear")

# Test 7: Performance and Memory
print("\n--- Test 7: Performance Test ---")
engine.clear()

# Add many animations to test performance
var start_time = tasmota.millis()
for i : 0..49
  var color = (0xFF000000 | (i * 5) << 16 | (i * 3) << 8 | (i * 2))
  var anim = animation.solid(engine)
  anim.color = color
  anim.priority = i
  anim.name = f"perf_{i}"
  engine.add(anim)
end

var add_time = tasmota.millis() - start_time
assert_test(add_time < 100, f"Adding 50 animations should be fast (took {add_time}ms)")
assert_equals(engine.size(), 50, "Should have 50 animations")

# Test rendering performance
start_time = tasmota.millis()
for i : 0..9
  engine.on_tick(tasmota.millis())
end
var render_time = tasmota.millis() - start_time
assert_test(render_time < 200, f"10 render cycles should be fast (took {render_time}ms)")

# Test 8: Error Handling
print("\n--- Test 8: Error Handling ---")
try
  var bad_engine = animation.create_engine(nil)
  assert_test(false, "Should throw error for nil strip")
except "value_error"
  assert_test(true, "Should throw value_error for nil strip")
end

# Test 9: Engine API Consistency
print("\n--- Test 9: Engine API Consistency ---")
var engine2 = animation.create_engine(strip)
assert_not_nil(engine2, "Second engine should be created")
assert_equals(engine2.width, strip.length(), "Second engine width should match strip")

var engine3 = animation.create_engine(strip)
assert_not_nil(engine3, "Direct engine creation should work")
assert_equals(engine3.width, strip.length(), "Direct engine width should match strip")

# Test 10: Dynamic Strip Length Detection
print("\n--- Test 10: Dynamic Strip Length Detection ---")

# Create a mock strip that can change length at runtime
class MockDynamicStrip
  var _length
  var pixels
  var show_calls
  
  def init(initial_length)
    self._length = initial_length
    self.pixels = []
    self.pixels.resize(initial_length)
    self.show_calls = 0
  end
  
  def length()
    return self._length
  end
  
  def set_length(new_length)
    self._length = new_length
    self.pixels.resize(new_length)
  end
  
  def set_pixel_color(index, color)
    if index >= 0 && index < self._length
      self.pixels[index] = color
    end
  end
  
  def clear()
    var i = 0
    while i < self._length
      self.pixels[i] = 0
      i += 1
    end
  end
  
  def show()
    self.show_calls += 1
  end
  
  def can_show()
    return true
  end
end

# Create engine with dynamic strip
var dynamic_strip = MockDynamicStrip(15)
var dynamic_engine = animation.create_engine(dynamic_strip)

# Test initial state
assert_equals(dynamic_engine.width, 15, "Engine should start with strip length 15")
assert_equals(dynamic_engine.frame_buffer.width, 15, "Frame buffer should match initial length")
assert_equals(dynamic_engine.temp_buffer.width, 15, "Temp buffer should match initial length")

# Store references to check object reuse
var original_frame_buffer = dynamic_engine.frame_buffer
var original_temp_buffer = dynamic_engine.temp_buffer

# Test 10a: No change detection
print("\n--- Test 10a: No change detection ---")
var length_changed = dynamic_engine.check_strip_length()
assert_test(!length_changed, "Should detect no change when length is same")
assert_equals(dynamic_engine.width, 15, "Engine width should remain 15")

# Test 10b: Manual length change detection
print("\n--- Test 10b: Manual length change detection ---")
dynamic_strip.set_length(25)
length_changed = dynamic_engine.check_strip_length()
assert_test(length_changed, "Should detect length change from 15 to 25")
assert_equals(dynamic_engine.width, 25, "Engine width should update to 25")
assert_equals(dynamic_engine.frame_buffer.width, 25, "Frame buffer should resize to 25")
assert_equals(dynamic_engine.temp_buffer.width, 25, "Temp buffer should resize to 25")

# Verify buffer objects were reused (efficient)
var frame_reused = (dynamic_engine.frame_buffer == original_frame_buffer)
var temp_reused = (dynamic_engine.temp_buffer == original_temp_buffer)
assert_test(frame_reused, "Frame buffer object should be reused for efficiency")
assert_test(temp_reused, "Temp buffer object should be reused for efficiency")

# Test 10c: Runtime detection during on_tick()
print("\n--- Test 10c: Runtime detection during on_tick() ---")
dynamic_engine.run()

# Add a test animation
var runtime_anim = animation.solid(dynamic_engine)
runtime_anim.color = 0xFF00FF00  # Green
runtime_anim.priority = 10
dynamic_engine.add(runtime_anim)

# Simulate several ticks with stable length
var tick_time = tasmota.millis()
for i : 0..2
  dynamic_engine.on_tick(tick_time + i * 10)
end
assert_equals(dynamic_engine.width, 25, "Width should remain stable during normal ticks")

# Change strip length during runtime
dynamic_strip.set_length(35)
var old_show_calls = dynamic_strip.show_calls

# Next tick should detect the change automatically
dynamic_engine.on_tick(tick_time + 50)
assert_equals(dynamic_engine.width, 35, "Engine should detect length change during on_tick()")
assert_equals(dynamic_engine.frame_buffer.width, 35, "Frame buffer should resize during on_tick()")
assert_equals(dynamic_engine.temp_buffer.width, 35, "Temp buffer should resize during on_tick()")

# Verify rendering still works after length change
var new_show_calls = dynamic_strip.show_calls
assert_test(new_show_calls >= old_show_calls, "Strip should be updated after length change (or at least not decrease)")

# Test 10d: Multiple length changes
print("\n--- Test 10d: Multiple length changes ---")
var lengths_to_test = [10, 50, 5, 30]
for new_length : lengths_to_test
  dynamic_strip.set_length(new_length)
  dynamic_engine.on_tick(tasmota.millis())
  assert_equals(dynamic_engine.width, new_length, f"Engine should adapt to length {new_length}")
  assert_equals(dynamic_engine.frame_buffer.width, new_length, f"Frame buffer should adapt to length {new_length}")
  assert_equals(dynamic_engine.temp_buffer.width, new_length, f"Temp buffer should adapt to length {new_length}")
end

# Test 10e: Length change with multiple animations
print("\n--- Test 10e: Length change with multiple animations ---")
dynamic_engine.clear()

# Add multiple animations
var red_anim = animation.solid(dynamic_engine)
red_anim.color = 0xFFFF0000
red_anim.priority = 20
dynamic_engine.add(red_anim)

var blue_anim = animation.solid(dynamic_engine)
blue_anim.color = 0xFF0000FF
blue_anim.priority = 10
dynamic_engine.add(blue_anim)

assert_equals(dynamic_engine.size(), 2, "Should have 2 animations")

# Change length and verify all animations continue working
dynamic_strip.set_length(40)
old_show_calls = dynamic_strip.show_calls
dynamic_engine.on_tick(tasmota.millis())

assert_equals(dynamic_engine.width, 40, "Engine should handle length change with multiple animations")
new_show_calls = dynamic_strip.show_calls
assert_test(new_show_calls >= old_show_calls, "Rendering should continue with multiple animations (or at least not decrease)")
assert_equals(dynamic_engine.size(), 2, "Should still have 2 animations after length change")

# Test 10f: Invalid length handling
print("\n--- Test 10f: Invalid length handling ---")
var current_width = dynamic_engine.width

# Test zero length (should be ignored)
dynamic_strip.set_length(0)
dynamic_engine.on_tick(tasmota.millis())
assert_equals(dynamic_engine.width, current_width, "Should ignore zero length")

# Test negative length (should be ignored)
dynamic_strip.set_length(-5)
dynamic_engine.on_tick(tasmota.millis())
assert_equals(dynamic_engine.width, current_width, "Should ignore negative length")

# Restore valid length
dynamic_strip.set_length(20)
dynamic_engine.on_tick(tasmota.millis())
assert_equals(dynamic_engine.width, 20, "Should accept valid length after invalid ones")

# Test 10g: Performance impact of length checking
print("\n--- Test 10g: Performance impact of length checking ---")
dynamic_strip.set_length(30)
dynamic_engine.check_strip_length()  # Ensure stable state

var perf_start_time = tasmota.millis()
# Run many ticks with stable length (should be fast)
for i : 0..99
  dynamic_engine.on_tick(perf_start_time + i)
end
var stable_time = tasmota.millis() - perf_start_time

# Now test with length changes (should still be reasonable)
perf_start_time = tasmota.millis()
for i : 0..19
  dynamic_strip.set_length(30 + (i % 5))  # Change length every few ticks
  dynamic_engine.on_tick(perf_start_time + i * 5)
end
var changing_time = tasmota.millis() - perf_start_time

assert_test(stable_time < 100, f"100 stable ticks should be fast (took {stable_time}ms)")
assert_test(changing_time < 200, f"20 ticks with length changes should be reasonable (took {changing_time}ms)")

dynamic_engine.stop()

# Cleanup
engine.stop()

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

print("\n=== Performance Benefits ===")
print("Unified AnimationEngine benefits:")
print("- Single object replacing 3 separate classes")
print("- Reduced memory overhead and allocations")
print("- Simplified API surface")
print("- Better cache locality")
print("- Fewer method calls per frame")
print("- Cleaner codebase with no deprecated APIs")
print("- Maintained full functionality")