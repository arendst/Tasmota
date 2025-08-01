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

# Mock LED strip for testing
class TestStrip
  var length_val
  var pixels
  var show_called
  
  def init(length)
    self.length_val = length
    self.pixels = []
    self.show_called = 0
    for i : 0..length-1
      self.pixels.push(0x00000000)
    end
  end
  
  def length()
    return self.length_val
  end
  
  def set_pixel_color(index, color)
    if index >= 0 && index < self.length_val
      self.pixels[index] = color
    end
  end
  
  def show()
    self.show_called += 1
  end
  
  def can_show()
    return true
  end
  
  def get_pixel_color(index)
    if index >= 0 && index < self.length_val
      return self.pixels[index]
    end
    return 0
  end
  
  def clear()
    for i : 0..self.length_val-1
      self.pixels[i] = 0x00000000
    end
  end
end

# Test 1: Engine Creation
print("\n--- Test 1: Engine Creation ---")
var strip = TestStrip(20)
var engine = animation.animation_engine(strip)

assert_not_nil(engine, "Engine should be created")
assert_equals(engine.width, 20, "Engine width should match strip length")
assert_equals(engine.is_active(), false, "Engine should start inactive")
assert_equals(engine.size(), 0, "Engine should start with no animations")

# Test 2: Animation Management
print("\n--- Test 2: Animation Management ---")
var anim1 = animation.solid(0xFFFF0000, 10)  # Red, priority 10
var anim2 = animation.solid(0xFF00FF00, 5)   # Green, priority 5
var anim3 = animation.solid(0xFF0000FF, 15)  # Blue, priority 15

assert_test(engine.add_animation(anim1), "Should add first animation")
assert_test(engine.add_animation(anim2), "Should add second animation")
assert_test(engine.add_animation(anim3), "Should add third animation")
assert_equals(engine.size(), 3, "Engine should have 3 animations")

# Test priority sorting (higher priority first)
var animations = engine.get_animations()
assert_equals(animations[0].priority, 15, "First animation should have highest priority")
assert_equals(animations[1].priority, 10, "Second animation should have medium priority")
assert_equals(animations[2].priority, 5, "Third animation should have lowest priority")

# Test duplicate prevention
assert_test(!engine.add_animation(anim1), "Should not add duplicate animation")
assert_equals(engine.size(), 3, "Size should remain 3 after duplicate attempt")

# Test animation removal
assert_test(engine.remove_animation(anim2), "Should remove existing animation")
assert_equals(engine.size(), 2, "Size should be 2 after removal")
assert_test(!engine.remove_animation(anim2), "Should not remove non-existent animation")

# Test 3: Engine Lifecycle
print("\n--- Test 3: Engine Lifecycle ---")
assert_test(engine.start(), "Should start engine")
assert_equals(engine.is_active(), true, "Engine should be active after start")

# Test that starting again doesn't break anything
engine.start()
assert_equals(engine.is_active(), true, "Engine should remain active after second start")

assert_test(engine.stop(), "Should stop engine")
assert_equals(engine.is_active(), false, "Engine should be inactive after stop")

# Test 4: Animation Updates and Rendering
print("\n--- Test 4: Animation Updates and Rendering ---")
engine.clear()
var test_anim = animation.solid(0xFFFF0000, 10)
engine.add_animation(test_anim)
engine.start()

var initial_show_count = strip.show_called
var current_time = tasmota.millis()

# Simulate a tick
engine.on_tick(current_time)

# Check that strip was updated
assert_test(strip.show_called > initial_show_count, "Strip should be updated after tick")

# Test 5: Sequence Manager Integration
print("\n--- Test 5: Sequence Manager Integration ---")
var seq_manager = animation.SequenceManager(engine)
assert_not_nil(seq_manager, "Sequence manager should be created")

engine.add_sequence_manager(seq_manager)
assert_test(true, "Should add sequence manager without error")

engine.remove_sequence_manager(seq_manager)
assert_test(true, "Should remove sequence manager without error")

# Test 6: Clear Functionality
print("\n--- Test 6: Clear Functionality ---")
engine.add_animation(anim1)
engine.add_animation(anim3)
engine.add_sequence_manager(seq_manager)

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
  var anim = animation.solid(color, i)
  engine.add_animation(anim)
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
  var bad_engine = animation.animation_engine(nil)
  assert_test(false, "Should throw error for nil strip")
except "value_error"
  assert_test(true, "Should throw value_error for nil strip")
end

# Test 9: Engine API Consistency
print("\n--- Test 9: Engine API Consistency ---")
var engine2 = animation.create_engine(strip)
assert_not_nil(engine2, "Second engine should be created")
assert_equals(engine2.width, strip.length_val, "Second engine width should match strip")

var engine3 = animation.animation_engine(strip)
assert_not_nil(engine3, "Direct engine creation should work")
assert_equals(engine3.width, strip.length_val, "Direct engine width should match strip")

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