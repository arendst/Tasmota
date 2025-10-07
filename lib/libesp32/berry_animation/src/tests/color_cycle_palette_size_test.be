#!/usr/bin/env berry

# Test for ColorCycleColorProvider palette_size read-only parameter
import animation

# Mock engine for testing
class MockEngine
  var time_ms
  def init()
    self.time_ms = 1000
  end
end

def test_palette_size_parameter_access()
  print("Testing palette_size parameter access...")
  
  var engine = MockEngine()
  var provider = animation.color_cycle(engine)
  
  # Test 1: Default palette_size should be 3
  var default_size = provider.palette_size
  assert(default_size == 3, f"Default palette_size should be 3, got {default_size}")
  
  # Test 2: palette_size should match _get_palette_size()
  var internal_size = provider._get_palette_size()
  assert(default_size == internal_size, f"palette_size ({default_size}) should match _get_palette_size() ({internal_size})")
  
  print("✓ palette_size parameter access tests passed!")
end

def test_palette_size_read_only()
  print("Testing palette_size is read-only...")
  
  var engine = MockEngine()
  var provider = animation.color_cycle(engine)
  
  var original_size = provider.palette_size
  
  # Test 1: Direct assignment should raise exception
  var caught_exception = false
  try
    provider.palette_size = 10
  except "value_error" as e
    caught_exception = true
  end
  assert(caught_exception, "Direct assignment to palette_size should raise value_error")
  
  # Test 2: Value should remain unchanged after failed write
  var size_after_write = provider.palette_size
  assert(size_after_write == original_size, f"palette_size should remain {original_size} after failed write, got {size_after_write}")
  
  # Test 3: set_param method should return false and not change value
  var set_success = provider.set_param("palette_size", 99)
  assert(set_success == false, "set_param should return false for read-only parameter")
  
  var size_after_set_param = provider.palette_size
  assert(size_after_set_param == original_size, f"palette_size should remain {original_size} after set_param, got {size_after_set_param}")
  
  # Test 4: get_param should return the actual value, not the attempted write
  var raw_value = provider.get_param("palette_size")
  assert(raw_value == original_size, f"get_param should return actual value {original_size}, got {raw_value}")
  
  print("✓ palette_size read-only tests passed!")
end

def test_palette_size_updates_with_palette_changes()
  print("Testing palette_size updates when palette changes...")
  
  var engine = MockEngine()
  var provider = animation.color_cycle(engine)
  
  # Test 1: 2-color palette
  var palette_2 = bytes("FFFF0000" "FF00FF00")
  provider.palette = palette_2
  var size_2 = provider.palette_size
  assert(size_2 == 2, f"palette_size should be 2 for 2-color palette, got {size_2}")
  
  # Test 2: 5-color palette
  var palette_5 = bytes("FFFF0000" "FF00FF00" "FF0000FF" "FFFFFF00" "FFFF00FF")
  provider.palette = palette_5
  var size_5 = provider.palette_size
  assert(size_5 == 5, f"palette_size should be 5 for 5-color palette, got {size_5}")
  
  # Test 3: 1-color palette
  var palette_1 = bytes("FFFF0000")
  provider.palette = palette_1
  var size_1 = provider.palette_size
  assert(size_1 == 1, f"palette_size should be 1 for 1-color palette, got {size_1}")
  
  # Test 4: Empty palette
  var empty_palette = bytes()
  provider.palette = empty_palette
  var size_0 = provider.palette_size
  assert(size_0 == 0, f"palette_size should be 0 for empty palette, got {size_0}")
  
  # Test 5: Large palette (10 colors)
  var palette_10 = bytes(
    "FFFF0000" "FF00FF00" "FF0000FF" "FFFFFF00" "FFFF00FF"
    "FF800000" "FF008000" "FF000080" "FF808000" "FF800080"
  )
  provider.palette = palette_10
  var size_10 = provider.palette_size
  assert(size_10 == 10, f"palette_size should be 10 for 10-color palette, got {size_10}")
  
  # Test 6: Verify palette_size is still read-only after palette changes
  var caught_exception = false
  try
    provider.palette_size = 15
  except "value_error"
    caught_exception = true
  end
  assert(caught_exception, "palette_size should still be read-only after palette changes")
  
  var final_size = provider.palette_size
  assert(final_size == 10, f"palette_size should remain 10 after failed write, got {final_size}")
  
  print("✓ palette_size update tests passed!")
end

def test_palette_size_with_new_instances()
  print("Testing palette_size with new provider instances...")
  
  var engine = MockEngine()
  
  # Test 1: Multiple instances should have correct default palette_size
  var provider1 = animation.color_cycle(engine)
  var provider2 = animation.color_cycle(engine)
  
  assert(provider1.palette_size == 3, "First provider should have default palette_size of 3")
  assert(provider2.palette_size == 3, "Second provider should have default palette_size of 3")
  
  # Test 2: Changing one instance shouldn't affect the other
  var custom_palette = bytes("FFFF0000" "FF00FF00")
  provider1.palette = custom_palette
  
  assert(provider1.palette_size == 2, "First provider should have palette_size of 2 after change")
  assert(provider2.palette_size == 3, "Second provider should still have palette_size of 3")
  
  # Test 3: Both instances should maintain read-only behavior
  var caught_exception_1 = false
  var caught_exception_2 = false
  
  try
    provider1.palette_size = 5
  except "value_error"
    caught_exception_1 = true
  end
  
  try
    provider2.palette_size = 7
  except "value_error"
    caught_exception_2 = true
  end
  
  assert(caught_exception_1, "First provider should reject palette_size writes")
  assert(caught_exception_2, "Second provider should reject palette_size writes")
  
  assert(provider1.palette_size == 2, "First provider palette_size should remain 2")
  assert(provider2.palette_size == 3, "Second provider palette_size should remain 3")
  
  print("✓ Multiple instance tests passed!")
end

def test_palette_size_parameter_metadata()
  print("Testing palette_size parameter metadata...")
  
  var engine = MockEngine()
  var provider = animation.color_cycle(engine)
  
  # Test 1: Parameter should exist
  assert(provider._has_param("palette_size") == true, "palette_size parameter should exist")
  var param_def = provider._get_param_def("palette_size")
  assert(param_def != nil, "palette_size should have parameter definition")
  
  # Test 2: Check parameter definition using static methods
  assert(provider.constraint_mask(param_def, "type") == 0x08, "palette_size definition should have type")
  assert(provider.constraint_find(param_def, "type", nil) == "int", f"palette_size type should be 'int', got '{provider.constraint_find(param_def, 'type', nil)}'")
  
  assert(provider.constraint_mask(param_def, "default") == 0x04, "palette_size definition should have default")
  assert(provider.constraint_find(param_def, "default", nil) == 3, f"palette_size default should be 3, got {provider.constraint_find(param_def, 'default', nil)}")
    
  print("✓ Parameter metadata tests passed!")
end

# Run all tests
test_palette_size_parameter_access()
test_palette_size_read_only()
test_palette_size_updates_with_palette_changes()
test_palette_size_with_new_instances()
test_palette_size_parameter_metadata()

print("✓ All ColorCycleColorProvider palette_size tests completed successfully!")