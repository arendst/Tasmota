# Unit tests for Crenel Position Animation
#
# This file contains comprehensive tests for the CrenelPositionAnimation class
# to ensure it works correctly with various parameters and edge cases.
#
# Command to run tests:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/crenel_position_animation_test.be

import animation

# Test counter
var test_count = 0
var passed_count = 0

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
  print("Running Crenel Position Animation Tests...")
  print("==================================================")
  
  # Create engine and strip for testing
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  # Test 1: Basic construction with new parameterized pattern
  var crenel = animation.crenel_position_animation(engine)
  test_assert(crenel != nil, "Crenel position animation creation")
  
  # Set parameters via virtual member assignment
  crenel.color = 0xFFFF0000
  crenel.back_color = 0xFF000000
  crenel.pos = 4
  crenel.pulse_size = 2
  crenel.low_size = 3
  crenel.nb_pulse = -1
  crenel.priority = 1
  crenel.duration = 0
  crenel.loop = true
  crenel.opacity = 255
  crenel.name = "test_crenel"
  
  test_assert(crenel.color == 0xFFFF0000, "Initial color setting")
  test_assert(crenel.pos == 4, "Initial position setting")
  test_assert(crenel.pulse_size == 2, "Initial pulse size setting")
  test_assert(crenel.low_size == 3, "Initial low size setting")
  test_assert(crenel.nb_pulse == -1, "Initial nb_pulse setting (infinite)")
  
  # Test 2: Parameter validation and updates via direct assignment
  crenel.color = 0xFF00FF00
  test_assert(crenel.color == 0xFF00FF00, "Color update via direct assignment")
  
  crenel.pos = 5
  test_assert(crenel.pos == 5, "Position update via direct assignment")
  
  crenel.pulse_size = 4
  test_assert(crenel.pulse_size == 4, "Pulse size update via direct assignment")
  
  crenel.low_size = 2
  test_assert(crenel.low_size == 2, "Low size update via direct assignment")
  
  crenel.nb_pulse = 3
  test_assert(crenel.nb_pulse == 3, "Nb pulse update via direct assignment")
  
  crenel.back_color = 0xFF000080
  test_assert(crenel.back_color == 0xFF000080, "Background color update via direct assignment")
  
  # Test 3: Negative value handling via parameter validation
  var old_pulse_size = crenel.pulse_size
  try
    crenel.pulse_size = -1
    test_assert(false, "Negative pulse size should raise error")
  except .. as e
    test_assert(crenel.pulse_size == old_pulse_size, "Negative pulse size rejected, keeps old value")
  end
  
  var old_low_size = crenel.low_size
  try
    crenel.low_size = -2
    test_assert(false, "Negative low size should raise error")
  except .. as e
    test_assert(crenel.low_size == old_low_size, "Negative low size rejected, keeps old value")
  end
  
  # Test 5: Frame rendering - basic crenel pattern
  var frame = animation.frame_buffer(10)
  crenel.color = 0xFFFF0000  # Red
  crenel.pos = 0
  crenel.pulse_size = 2
  crenel.low_size = 3
  crenel.nb_pulse = -1  # Infinite
  crenel.back_color = 0xFF000000  # Transparent
  crenel.start()
  
  var rendered = crenel.render(frame, engine.time_ms)
  test_assert(rendered, "Render returns true when running")
  
  # Check pattern: 2 on, 3 off, 2 on, 3 off...
  # Positions: 0,1 = red, 2,3,4 = transparent, 5,6 = red, 7,8,9 = transparent
  test_assert(frame.get_pixel_color(0) == 0xFFFF0000, "First pulse pixel 1 is red")
  test_assert(frame.get_pixel_color(1) == 0xFFFF0000, "First pulse pixel 2 is red")
  test_assert(frame.get_pixel_color(2) == 0x00000000, "First gap pixel 1 is transparent")
  test_assert(frame.get_pixel_color(3) == 0x00000000, "First gap pixel 2 is transparent")
  test_assert(frame.get_pixel_color(4) == 0x00000000, "First gap pixel 3 is transparent")
  test_assert(frame.get_pixel_color(5) == 0xFFFF0000, "Second pulse pixel 1 is red")
  test_assert(frame.get_pixel_color(6) == 0xFFFF0000, "Second pulse pixel 2 is red")
  test_assert(frame.get_pixel_color(7) == 0x00000000, "Second gap pixel 1 is transparent")
  test_assert(frame.get_pixel_color(8) == 0x00000000, "Second gap pixel 2 is transparent")
  test_assert(frame.get_pixel_color(9) == 0x00000000, "Second gap pixel 3 is transparent")
  
  # Test 6: Frame rendering with background
  frame.clear()
  crenel.back_color = 0xFF000080  # Dark blue background
  crenel.render(frame, engine.time_ms)
  
  test_assert(frame.get_pixel_color(2) == 0xFF000080, "Gap pixel has background color")
  test_assert(frame.get_pixel_color(0) == 0xFFFF0000, "Pulse pixel overrides background")
  
  # Test 7: Limited number of pulses
  frame.clear()
  crenel.back_color = 0xFF000000  # Transparent background
  crenel.nb_pulse = 2  # Only 2 pulses
  crenel.render(frame, engine.time_ms)
  
  # Should have 2 pulses: positions 0,1 and 5,6
  test_assert(frame.get_pixel_color(0) == 0xFFFF0000, "Limited pulse 1 pixel 1 is red")
  test_assert(frame.get_pixel_color(1) == 0xFFFF0000, "Limited pulse 1 pixel 2 is red")
  test_assert(frame.get_pixel_color(5) == 0xFFFF0000, "Limited pulse 2 pixel 1 is red")
  test_assert(frame.get_pixel_color(6) == 0xFFFF0000, "Limited pulse 2 pixel 2 is red")
  # No third pulse should exist
  test_assert(frame.get_pixel_color(7) == 0x00000000, "No third pulse - gap is transparent")
  test_assert(frame.get_pixel_color(8) == 0x00000000, "No third pulse - gap is transparent")
  test_assert(frame.get_pixel_color(9) == 0x00000000, "No third pulse - gap is transparent")
  
  # Test 8: Position offset
  frame.clear()
  crenel.pos = 2  # Start at position 2
  crenel.nb_pulse = -1  # Back to infinite
  crenel.render(frame, engine.time_ms)
  
  # Pattern should start at position 2: positions 2,3 and 7,8
  test_assert(frame.get_pixel_color(0) == 0x00000000, "Offset pattern - position 0 is transparent")
  test_assert(frame.get_pixel_color(1) == 0x00000000, "Offset pattern - position 1 is transparent")
  test_assert(frame.get_pixel_color(2) == 0xFFFF0000, "Offset pattern - first pulse pixel 1 is red")
  test_assert(frame.get_pixel_color(3) == 0xFFFF0000, "Offset pattern - first pulse pixel 2 is red")
  test_assert(frame.get_pixel_color(4) == 0x00000000, "Offset pattern - gap pixel is transparent")
  test_assert(frame.get_pixel_color(7) == 0xFFFF0000, "Offset pattern - second pulse pixel 1 is red")
  test_assert(frame.get_pixel_color(8) == 0xFFFF0000, "Offset pattern - second pulse pixel 2 is red")
  
  # Test 9: Zero pulse size (should render nothing)
  frame.clear()
  crenel.pos = 0
  crenel.pulse_size = 0
  crenel.render(frame, engine.time_ms)
  
  # All pixels should remain transparent
  for i:0..9
    test_assert(frame.get_pixel_color(i) == 0x00000000, f"Zero pulse size - pixel {i} is transparent")
  end
  
  # Test 10: Single pixel pulses
  frame.clear()
  crenel.pulse_size = 1
  crenel.low_size = 2
  crenel.render(frame, engine.time_ms)
  
  # Pattern: 1 on, 2 off, 1 on, 2 off...
  # Positions: 0 = red, 1,2 = transparent, 3 = red, 4,5 = transparent, 6 = red, 7,8 = transparent, 9 = red
  test_assert(frame.get_pixel_color(0) == 0xFFFF0000, "Single pixel pulse at position 0")
  test_assert(frame.get_pixel_color(1) == 0x00000000, "Gap after single pixel pulse")
  test_assert(frame.get_pixel_color(2) == 0x00000000, "Gap after single pixel pulse")
  test_assert(frame.get_pixel_color(3) == 0xFFFF0000, "Single pixel pulse at position 3")
  test_assert(frame.get_pixel_color(6) == 0xFFFF0000, "Single pixel pulse at position 6")
  test_assert(frame.get_pixel_color(9) == 0xFFFF0000, "Single pixel pulse at position 9")
  
  # Test 11: Negative position handling
  frame.clear()
  crenel.pulse_size = 2
  crenel.low_size = 3
  crenel.pos = -1
  crenel.render(frame, engine.time_ms)
  
  # With period = 5 and pos = -1, the pattern should be shifted
  # The algorithm should handle negative positions correctly
  var has_red_pixels = false
  for i:0..9
    if frame.get_pixel_color(i) == 0xFFFF0000
      has_red_pixels = true
      break
    end
  end
  test_assert(has_red_pixels, "Negative position produces visible pattern")
  
  # Test 12: Zero nb_pulse (should render nothing)
  frame.clear()
  crenel.pos = 0
  crenel.nb_pulse = 0
  crenel.render(frame, engine.time_ms)
  
  # All pixels should remain transparent
  for i:0..9
    test_assert(frame.get_pixel_color(i) == 0x00000000, f"Zero nb_pulse - pixel {i} is transparent")
  end
  
  # Test 14: Parameter constraints via direct assignment
  crenel.start()  # Restart for parameter tests
  crenel.pos = 15
  test_assert(crenel.pos == 15, "Position parameter updated")
  
  crenel.pulse_size = 5
  test_assert(crenel.pulse_size == 5, "Pulse size parameter updated")
  
  crenel.low_size = 4
  test_assert(crenel.low_size == 4, "Low size parameter updated")
  
  crenel.nb_pulse = 10
  test_assert(crenel.nb_pulse == 10, "Nb_pulse parameter updated")
  
  # Test 15: String representation
  var str_repr = crenel.tostring()
  test_assert(type(str_repr) == "string", "String representation returns string")
  import string
  test_assert(string.find(str_repr, "CrenelPositionAnimation") >= 0, "String representation contains class name")
  
  # Test 16: Edge case - very large frame
  var large_frame = animation.frame_buffer(100)
  crenel.pos = 0
  crenel.pulse_size = 10
  crenel.low_size = 5
  crenel.nb_pulse = 3  # 3 pulses
  crenel.render(large_frame)
  
  # Should have 3 pulses of 10 pixels each, separated by 5 pixels
  # Pulse 1: 0-9, Gap: 10-14, Pulse 2: 15-24, Gap: 25-29, Pulse 3: 30-39
  test_assert(large_frame.get_pixel_color(0) == 0xFFFF0000, "Large frame - first pulse start")
  test_assert(large_frame.get_pixel_color(9) == 0xFFFF0000, "Large frame - first pulse end")
  test_assert(large_frame.get_pixel_color(10) == 0x00000000, "Large frame - first gap start")
  test_assert(large_frame.get_pixel_color(14) == 0x00000000, "Large frame - first gap end")
  test_assert(large_frame.get_pixel_color(15) == 0xFFFF0000, "Large frame - second pulse start")
  test_assert(large_frame.get_pixel_color(24) == 0xFFFF0000, "Large frame - second pulse end")
  test_assert(large_frame.get_pixel_color(30) == 0xFFFF0000, "Large frame - third pulse start")
  test_assert(large_frame.get_pixel_color(39) == 0xFFFF0000, "Large frame - third pulse end")
  test_assert(large_frame.get_pixel_color(40) == 0x00000000, "Large frame - after third pulse")
  
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