#!/usr/bin/env berry

# Test for ColorCycleColorProvider with bytes palette in AARRGGBB format
import animation
import animation_dsl

# Mock engine for testing
class MockEngine
  var time_ms
  def init()
    self.time_ms = 1000
  end
end

def test_color_cycle_bytes_format()
  print("Testing ColorCycleColorProvider with bytes palette (AARRGGBB format)...")
  
  var engine = MockEngine()
  
  # Test 1: Create provider with default palette
  var provider = animation.color_cycle(engine)
  assert(provider != nil, "Provider should be created")
  
  # Test 2: Check default palette
  var default_size = provider._get_palette_size()
  assert(default_size == 3, f"Default palette should have 3 colors, got {default_size}")
  
  # Test 3: Test colors from default palette (AARRGGBB format)
  var color0 = provider._get_color_at_index(0)  # Should be FF0000FF (blue)
  var color1 = provider._get_color_at_index(1)  # Should be FF00FF00 (green)
  var color2 = provider._get_color_at_index(2)  # Should be FFFF0000 (red)
  
  assert(color0 == 0xFF0000FF, f"First color should be blue (0xFF0000FF), got 0x{color0:08X}")
  assert(color1 == 0xFF00FF00, f"Second color should be green (0xFF00FF00), got 0x{color1:08X}")
  assert(color2 == 0xFFFF0000, f"Third color should be red (0xFFFF0000), got 0x{color2:08X}")
  
  # Test 4: Set custom bytes palette
  var custom_palette = bytes(
    "80FF0000"    # Semi-transparent red (alpha=0x80)
    "FF00FF00"    # Opaque green (alpha=0xFF)
    "C00000FF"    # Semi-transparent blue (alpha=0xC0)
    "FFFFFF00"    # Opaque yellow (alpha=0xFF)
  )
  
  provider.palette = custom_palette
  var custom_size = provider._get_palette_size()
  assert(custom_size == 4, f"Custom palette should have 4 colors, got {custom_size}")
  
  # Test 5: Verify custom palette colors (alpha channel forced to 0xFF)
  var custom_color0 = provider._get_color_at_index(0)  # Red with forced full alpha
  var custom_color1 = provider._get_color_at_index(1)  # Green with forced full alpha
  var custom_color2 = provider._get_color_at_index(2)  # Blue with forced full alpha
  var custom_color3 = provider._get_color_at_index(3)  # Yellow with forced full alpha
  
  assert(custom_color0 == 0xFFFF0000, f"Custom color 0 should be 0xFFFF0000 (alpha forced), got 0x{custom_color0:08X}")
  assert(custom_color1 == 0xFF00FF00, f"Custom color 1 should be 0xFF00FF00 (alpha forced), got 0x{custom_color1:08X}")
  assert(custom_color2 == 0xFF0000FF, f"Custom color 2 should be 0xFF0000FF (alpha forced), got 0x{custom_color2:08X}")
  assert(custom_color3 == 0xFFFFFF00, f"Custom color 3 should be 0xFFFFFF00 (alpha forced), got 0x{custom_color3:08X}")
  
  # Test 6: Test auto-cycle mode
  provider.cycle_period = 4000  # 4 seconds for 4 colors = 1 second per color
  
  # At time 0, should be first color
  engine.time_ms = 0
  var cycle_color0 = provider.produce_value("color", 0)
  assert(cycle_color0 == custom_color0, f"Cycle color at t=0 should match first color")
  
  # At time 1000 (1/4 of cycle), should be second color
  var cycle_color1 = provider.produce_value("color", 1000)
  assert(cycle_color1 == custom_color1, f"Cycle color at t=1000 should match second color")
  
  # At time 2000 (2/4 of cycle), should be third color
  var cycle_color2 = provider.produce_value("color", 2000)
  assert(cycle_color2 == custom_color2, f"Cycle color at t=2000 should match third color")
  
  # At time 3000 (3/4 of cycle), should be fourth color
  var cycle_color3 = provider.produce_value("color", 3000)
  assert(cycle_color3 == custom_color3, f"Cycle color at t=3000 should match fourth color")
  
  # Test 7: Test manual mode
  provider.cycle_period = 0  # Manual mode
  provider.current_index = 1
  provider.current_color = custom_color1
  
  var manual_color = provider.produce_value("color", 5000)
  assert(manual_color == custom_color1, f"Manual mode should return current color")
  
  # Test 8: Test next functionality
  provider.next = 1  # Should trigger move to next color
  var next_color = provider.current_color
  assert(next_color == custom_color2, f"Next should move to third color")
  assert(provider.current_index == 2, f"Current index should be 2")
  
  # Test 9: Test value-based color selection
  var value_color_0 = provider.get_color_for_value(0, 0)    # Should be first color
  var value_color_50 = provider.get_color_for_value(50, 0)  # Should be middle color
  var value_color_100 = provider.get_color_for_value(100, 0) # Should be last color
  
  assert(value_color_0 == custom_color0, f"Value 0 should return first color")
  assert(value_color_100 == custom_color3, f"Value 100 should return last color")
  
  # Test 10: Test edge cases
  var invalid_color = provider._get_color_at_index(-1)  # Invalid index
  assert(invalid_color == 0xFFFFFFFF, f"Invalid index should return white")
  
  var out_of_bounds_color = provider._get_color_at_index(100)  # Out of bounds
  assert(out_of_bounds_color == 0xFFFFFFFF, f"Out of bounds index should return white")
  
  # Test 11: Test empty palette handling
  var empty_palette = bytes()
  provider.palette = empty_palette
  var empty_size = provider._get_palette_size()
  assert(empty_size == 0, f"Empty palette should have 0 colors")
  
  var empty_color = provider.produce_value("color", 1000)
  assert(empty_color == 0xFFFFFFFF, f"Empty palette should return white")
  
  print("✓ All ColorCycleColorProvider bytes format tests passed!")
end

def test_bytes_parameter_validation()
  print("Testing bytes parameter validation...")
  
  var engine = MockEngine()
  var provider = animation.color_cycle(engine)
  
  # Test 1: Valid bytes palette should be accepted
  var valid_palette = bytes("FF0000FFFF00FF00FFFF0000")
  provider.palette = valid_palette
  assert(provider._get_palette_size() == 3, "Valid bytes palette should be accepted")
  
  # Test 2: Invalid types should be rejected
  var invalid_types = ["string", 123, 3.14, true, [], {}]
  
  for invalid_val : invalid_types
    var caught_error = false
    try
      provider.palette = invalid_val
    except "value_error"
      caught_error = true
    end
    assert(caught_error, f"Should reject {type(invalid_val)}: {invalid_val}")
  end
  
  # Test 3: Nil should be accepted (uses default)
  provider.palette = nil
  assert(provider._get_palette_size() == 3, "Nil should use default palette")
  
  print("✓ All bytes parameter validation tests passed!")
end

# Run the tests
test_color_cycle_bytes_format()
test_bytes_parameter_validation()
print("✓ All ColorCycleColorProvider tests completed successfully!")