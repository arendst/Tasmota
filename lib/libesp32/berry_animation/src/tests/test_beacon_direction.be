# Test for beacon animation right_edge parameter
# Tests that right_edge=1 positions the beacon using pos as the right edge of the beacon

import animation
import global

# Test counter
var test_count = 0
var passed_count = 0

# Create LED strip and engine for testing
var strip = global.Leds(10)  # Use built-in LED strip for testing
var engine = animation.create_engine(strip)

def test_assert(condition, message)
  test_count += 1
  if condition
    passed_count += 1
    print(f"✓ Test {test_count}: {message}")
  else
    print(f"✗ Test {test_count}: {message}")
  end
end

def run_tests()
  print("Running Beacon Animation right_edge Tests...")
  print("==================================================")
  
  var strip_length = 10
  var frame = animation.frame_buffer(strip_length)
  
  # Test 1: right_edge=0 (default, left edge), pos=0, beacon_size=1
  # Should show 1 pixel at position 0 (left edge)
  var beacon1 = animation.beacon(engine)
  beacon1.color = 0xFFFF0000  # Red
  beacon1.back_color = 0xFF000000  # Black
  beacon1.pos = 0
  beacon1.beacon_size = 1
  beacon1.slew_size = 0
  beacon1.right_edge = 0
  beacon1.start()
  
  frame.fill_pixels(frame.pixels, 0xFF000000)  # Clear to black
  beacon1.render(frame, 0, strip_length)
  
  # Check pixel 0 is red, others are black
  test_assert(frame.get_pixel_color(0) == 0xFFFF0000, "right_edge=0, pos=0: pixel 0 is red")
  test_assert(frame.get_pixel_color(1) == 0xFF000000, "right_edge=0, pos=0: pixel 1 is black")
  test_assert(frame.get_pixel_color(9) == 0xFF000000, "right_edge=0, pos=0: pixel 9 is black")
  
  # Test 2: right_edge=1 (right edge), pos=0, beacon_size=1
  # With right_edge=1: effective_pos = pos - beacon_size + 1 = 0 - 1 + 1 = 0
  # So pixel 0 should be lit
  var beacon2 = animation.beacon(engine)
  beacon2.color = 0xFF00FF00  # Green
  beacon2.back_color = 0xFF000000  # Black
  beacon2.pos = 0
  beacon2.beacon_size = 1
  beacon2.slew_size = 0
  beacon2.right_edge = 1
  beacon2.start()
  
  frame.fill_pixels(frame.pixels, 0xFF000000)  # Clear to black
  beacon2.render(frame, 0, strip_length)
  
  # Check pixel 0 is green (right edge of beacon at pos=0)
  test_assert(frame.get_pixel_color(0) == 0xFF00FF00, "right_edge=1, pos=0: pixel 0 is green")
  test_assert(frame.get_pixel_color(1) == 0xFF000000, "right_edge=1, pos=0: pixel 1 is black")
  
  # Test 3: right_edge=1, pos=5, beacon_size=3
  # With right_edge=1: effective_pos = pos - beacon_size + 1 = 5 - 3 + 1 = 3
  # So pixels 3,4,5 should be lit (beacon from 3 to 5, with right edge at 5)
  var beacon3 = animation.beacon(engine)
  beacon3.color = 0xFF0000FF  # Blue
  beacon3.back_color = 0xFF000000  # Black
  beacon3.pos = 5
  beacon3.beacon_size = 3
  beacon3.slew_size = 0
  beacon3.right_edge = 1
  beacon3.start()
  
  frame.fill_pixels(frame.pixels, 0xFF000000)  # Clear to black
  beacon3.render(frame, 0, strip_length)
  
  test_assert(frame.get_pixel_color(2) == 0xFF000000, "right_edge=1, pos=5, size=3: pixel 2 is black")
  test_assert(frame.get_pixel_color(3) == 0xFF0000FF, "right_edge=1, pos=5, size=3: pixel 3 is blue")
  test_assert(frame.get_pixel_color(4) == 0xFF0000FF, "right_edge=1, pos=5, size=3: pixel 4 is blue")
  test_assert(frame.get_pixel_color(5) == 0xFF0000FF, "right_edge=1, pos=5, size=3: pixel 5 is blue")
  test_assert(frame.get_pixel_color(6) == 0xFF000000, "right_edge=1, pos=5, size=3: pixel 6 is black")
  
  # Test 4: right_edge=0, pos=2, beacon_size=3 (same params, different right_edge)
  # Should show pixels 2,3,4 lit
  var beacon4 = animation.beacon(engine)
  beacon4.color = 0xFFFFFF00  # Yellow
  beacon4.back_color = 0xFF000000  # Black
  beacon4.pos = 2
  beacon4.beacon_size = 3
  beacon4.slew_size = 0
  beacon4.right_edge = 0
  beacon4.start()
  
  frame.fill_pixels(frame.pixels, 0xFF000000)  # Clear to black
  beacon4.render(frame, 0, strip_length)
  
  test_assert(frame.get_pixel_color(1) == 0xFF000000, "right_edge=0, pos=2, size=3: pixel 1 is black")
  test_assert(frame.get_pixel_color(2) == 0xFFFFFF00, "right_edge=0, pos=2, size=3: pixel 2 is yellow")
  test_assert(frame.get_pixel_color(3) == 0xFFFFFF00, "right_edge=0, pos=2, size=3: pixel 3 is yellow")
  test_assert(frame.get_pixel_color(4) == 0xFFFFFF00, "right_edge=0, pos=2, size=3: pixel 4 is yellow")
  test_assert(frame.get_pixel_color(5) == 0xFF000000, "right_edge=0, pos=2, size=3: pixel 5 is black")
  
  # Test 5: Default right_edge should be 0
  var beacon5 = animation.beacon(engine)
  test_assert(beacon5.right_edge == 0, "Default right_edge is 0")
  
  # Test 6: Invalid right_edge value should be rejected
  try
    beacon5.right_edge = 2
    test_assert(false, "Invalid right_edge value should be rejected")
  except "value_error"
    test_assert(true, "Invalid right_edge value properly rejected")
  end
  
  print("==================================================")
  print(f"Tests completed: {passed_count}/{test_count} passed")
  
  if passed_count == test_count
    print("🎉 All tests passed!")
    return true
  else
    print(f"❌ {test_count - passed_count} tests failed")
    raise "test_failed"
  end
end

# Run the tests
var success = run_tests()

return success
