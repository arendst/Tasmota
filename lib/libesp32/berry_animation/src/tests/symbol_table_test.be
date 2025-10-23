# Symbol Table Test Suite
# Tests for the SymbolEntry and SymbolTable classes
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation/src -e "import tasmota def log(x,l) tasmota.log(x,l) end" lib/libesp32/berry_animation/src/tests/symbol_table_test.be

import animation
import animation_dsl
import string

# Test SymbolEntry creation and basic properties
def test_symbol_entry_creation()
  print("Testing SymbolEntry creation...")
  
  # Test palette constant entry
  var palette_entry = animation_dsl._symbol_entry.create_palette_constant("PALETTE_RAINBOW", nil, true)
  assert(palette_entry.name == "PALETTE_RAINBOW", "Should set name correctly")
  assert(palette_entry.type == "palette_constant", "Should set type correctly")
  assert(palette_entry.is_builtin == true, "Should be marked as builtin")
  assert(palette_entry.takes_args == false, "Palette constants don't take args")
  assert(palette_entry.is_dangerous_call() == false, "Palette constants are not dangerous")
  
  # Test animation constructor entry
  var anim_entry = animation_dsl._symbol_entry.create_animation_constructor("solid", nil, true)
  assert(anim_entry.type == "animation_constructor", "Should be animation constructor")
  assert(anim_entry.takes_named_args() == true, "Animation constructors take named args")
  assert(anim_entry.is_dangerous_call() == true, "Animation constructors are dangerous")
  
  # Test user function entry
  var func_entry = animation_dsl._symbol_entry.create_user_function("my_func", false)
  assert(func_entry.type == "user_function", "Should be user function")
  assert(func_entry.takes_positional_args() == true, "User functions take positional args")
  assert(func_entry.is_builtin == false, "Should not be builtin")
  
  print("✓ SymbolEntry creation test passed")
  return true
end

# Test SymbolEntry reference generation
def test_symbol_entry_references()
  print("Testing SymbolEntry reference generation...")
  
  # Test builtin reference
  var builtin_entry = animation_dsl._symbol_entry.create_animation_constructor("solid", nil, true)
  assert(builtin_entry.get_reference() == "animation.solid", "Should generate builtin reference")
  
  # Test math function reference
  var math_entry = animation_dsl._symbol_entry.create_math_function("max", true)
  assert(math_entry.get_reference() == "animation._math.max", "Should generate math function reference")
  
  # Test user-defined reference
  var user_entry = animation_dsl._symbol_entry.create_animation_instance("my_anim", nil, false)
  assert(user_entry.get_reference() == "my_anim_", "Should generate user reference with underscore")
  
  print("✓ SymbolEntry reference generation test passed")
  return true
end

# Test SymbolEntry argument detection
def test_symbol_entry_argument_detection()
  print("Testing SymbolEntry argument detection...")
  
  # Test different types and their argument characteristics
  var tests = [
    ["palette_constant", false, "none"],
    ["math_function", true, "positional"],
    ["user_function", true, "positional"],
    ["value_provider_constructor", true, "named"],
    ["animation_constructor", true, "named"],
    ["color_constructor", true, "named"],
    ["variable", false, "none"],
    ["sequence", false, "none"]
  ]
  
  for test : tests
    var typ = test[0]
    var expected_takes_args = test[1]
    var expected_arg_type = test[2]
    
    var entry = animation_dsl._symbol_entry("test_symbol", typ, nil, false)
    assert(entry.takes_args == expected_takes_args, f"Type {typ} should have takes_args={expected_takes_args}")
    assert(entry.arg_type == expected_arg_type, f"Type {typ} should have arg_type={expected_arg_type}")
  end
  
  print("✓ SymbolEntry argument detection test passed")
  return true
end

# Test SymbolEntry danger detection
def test_symbol_entry_danger_detection()
  print("Testing SymbolEntry danger detection...")
  
  # Test dangerous types (constructors)
  var dangerous_types = ["value_provider_constructor", "animation_constructor", "color_constructor"]
  for typ : dangerous_types
    var entry = animation_dsl._symbol_entry("test", typ, nil, true)
    assert(entry.is_dangerous_call() == true, f"Type {typ} should be dangerous")
  end
  
  # Test safe types
  var safe_types = ["palette_constant", "math_function", "user_function", "variable", "sequence"]
  for typ : safe_types
    var entry = animation_dsl._symbol_entry("test", typ, nil, true)
    assert(entry.is_dangerous_call() == false, f"Type {typ} should be safe")
  end
  
  print("✓ SymbolEntry danger detection test passed")
  return true
end

# Test SymbolTable basic operations
def test_symbol_table_basic_operations()
  print("Testing SymbolTable basic operations...")
  
  var table = animation_dsl._symbol_table()
  
  # Test adding a symbol
  var entry = animation_dsl._symbol_entry.create_variable("test_var", false)
  var added_entry = table.add("test_var", entry)
  assert(added_entry == entry, "Should return the added entry")
  
  # Test checking if symbol exists
  assert(table.contains("test_var") == true, "Should contain added symbol")
  assert(table.contains("nonexistent") == false, "Should not contain nonexistent symbol")
  
  # Test getting symbol
  var retrieved = table.get("test_var")
  assert(retrieved == entry, "Should retrieve the same entry")
  
  # Test getting reference
  var ref = table.get_reference("test_var")
  assert(ref == "test_var_", "Should generate correct reference")
  
  print("✓ SymbolTable basic operations test passed")
  return true
end

# Test SymbolTable dynamic detection
def test_symbol_table_dynamic_detection()
  print("Testing SymbolTable dynamic detection...")
  
  var table = animation_dsl._symbol_table()
  
  # Test detection of built-in animation constructor
  assert(table.contains("solid") == true, "Should detect built-in solid function")
  var solid_entry = table.get("solid")
  assert(solid_entry != nil, "Should get solid entry")
  assert(solid_entry.type == "animation_constructor", "Should detect as animation constructor")
  assert(solid_entry.is_builtin == true, "Should be marked as builtin")
  
  # Test detection of built-in math function
  assert(table.contains("max") == true, "Should detect built-in max function")
  var max_entry = table.get("max")
  assert(max_entry != nil, "Should get max entry")
  assert(max_entry.type == "math_function", "Should detect as math function")
  
  # Test detection of built-in palette
  assert(table.contains("PALETTE_RAINBOW") == true, "Should detect built-in palette")
  var palette_entry = table.get("PALETTE_RAINBOW")
  assert(palette_entry != nil, "Should get palette entry")
  assert(palette_entry.type == "palette_constant", "Should detect as palette constant")
  
  print("✓ SymbolTable dynamic detection test passed")
  return true
end

# Test SymbolTable conflict detection
def test_symbol_table_conflict_detection()
  print("Testing SymbolTable conflict detection...")
  
  var table = animation_dsl._symbol_table()
  
  # Try to redefine a built-in symbol with different type
  var user_entry = animation_dsl._symbol_entry.create_variable("solid", false)
  
  try
    table.add("solid", user_entry)
    assert(false, "Should have thrown conflict error")
  except "symbol_redefinition_error" as e, msg
    assert(string.find(str(msg), "solid") >= 0, "Error should mention symbol name")
    assert(string.find(str(msg), "animation_constructor") >= 0, "Error should mention built-in type")
  end
  
  # Test that same type redefinition is allowed
  var anim_entry1 = animation_dsl._symbol_entry.create_animation_instance("my_anim", nil, false)
  var anim_entry2 = animation_dsl._symbol_entry.create_animation_instance("my_anim", nil, false)
  
  table.add("my_anim", anim_entry1)
  table.add("my_anim", anim_entry2)  # Should not throw
  
  print("✓ SymbolTable conflict detection test passed")
  return true
end

# Test SymbolTable creation methods
def test_symbol_table_creation_methods()
  print("Testing SymbolTable creation methods...")
  
  var table = animation_dsl._symbol_table()
  
  # Test create_palette
  var palette_entry = table.create_palette("my_palette", nil)
  assert(palette_entry.type == "palette", "Should create palette entry")
  assert(table.contains("my_palette") == true, "Should add to table")
  
  # Test create_color
  var color_entry = table.create_color("my_color", nil)
  assert(color_entry.type == "color", "Should create color entry")
  assert(table.contains("my_color") == true, "Should add to table")
  
  # Test create_animation
  var anim_entry = table.create_animation("my_anim", nil)
  assert(anim_entry.type == "animation", "Should create animation entry")
  assert(table.contains("my_anim") == true, "Should add to table")
  
  # Test create_value_provider
  var vp_entry = table.create_value_provider("my_vp", nil)
  assert(vp_entry.type == "value_provider", "Should create value provider entry")
  assert(table.contains("my_vp") == true, "Should add to table")
  
  # Test create_variable
  var var_entry = table.create_variable("my_var")
  assert(var_entry.type == "variable", "Should create variable entry")
  assert(table.contains("my_var") == true, "Should add to table")
  
  # Test create_sequence
  var seq_entry = table.create_sequence("my_seq")
  assert(seq_entry.type == "sequence", "Should create sequence entry")
  assert(table.contains("my_seq") == true, "Should add to table")
  
  # Test create_template
  var template_entry = table.create_template("my_template", {"param1": "int", "param2": "string"})
  assert(template_entry.type == "template", "Should create template entry")
  assert(template_entry.get_param_types()["param1"] == "int", "Should set parameter types")
  assert(table.contains("my_template") == true, "Should add to table")
  
  print("✓ SymbolTable creation methods test passed")
  return true
end

# Test SymbolTable named color handling
def test_symbol_table_named_colors()
  print("Testing SymbolTable named color handling...")
  
  var table = animation_dsl._symbol_table()
  
  # Test named color detection
  assert(table.symbol_exists("red") == true, "Should recognize named color 'red'")
  assert(table.symbol_exists("blue") == true, "Should recognize named color 'blue'")
  assert(table.symbol_exists("nonexistent_color") == false, "Should not recognize invalid color")
  
  # Test named color reference generation
  var red_ref = table.get_reference("red")
  assert(red_ref == "0xFFFF0000", "Should generate correct hex value for red")
  
  var blue_ref = table.get_reference("blue")
  assert(blue_ref == "0xFF0000FF", "Should generate correct hex value for blue")
  
  print("✓ SymbolTable named color handling test passed")
  return true
end

# Test SymbolTable utility methods
def test_symbol_table_utility_methods()
  print("Testing SymbolTable utility methods...")
  
  var table = animation_dsl._symbol_table()
  
  # Add some test symbols
  table.create_animation("test_anim", nil)
  table.create_variable("test_var")
  
  # Test get_type
  assert(table.get_type("test_anim") == "animation", "Should return correct type")
  assert(table.get_type("solid") == "animation_constructor", "Should return builtin type")
  assert(table.get_type("nonexistent") == nil, "Should return nil for nonexistent")
  
  # Test takes_args
  assert(table.takes_args("solid") == true, "Solid should take args")
  assert(table.takes_args("test_var") == false, "Variables don't take args")
  
  # Test takes_named_args
  assert(table.takes_named_args("solid") == true, "Solid takes named args")
  assert(table.takes_named_args("max") == false, "Max takes positional args")
  
  # Test takes_positional_args
  assert(table.takes_positional_args("max") == true, "Max takes positional args")
  assert(table.takes_positional_args("solid") == false, "Solid doesn't take positional args")
  
  # Test is_dangerous
  assert(table.is_dangerous("solid") == true, "Solid is dangerous (constructor)")
  assert(table.is_dangerous("max") == false, "Max is not dangerous")
  assert(table.is_dangerous("test_var") == false, "Variables are not dangerous")
  
  print("✓ SymbolTable utility methods test passed")
  return true
end

# Test MockEngine functionality
def test_mock_engine()
  print("Testing MockEngine functionality...")
  
  var mock = animation_dsl.MockEngine()
  assert(mock.time_ms == 0, "Should initialize time to 0")
  assert(mock.get_strip_length() == 30, "Should return default strip length")
  
  print("✓ MockEngine test passed")
  return true
end

# Run all symbol table tests
def run_symbol_table_tests()
  print("=== Symbol Table Test Suite ===")
  
  var tests = [
    test_symbol_entry_creation,
    test_symbol_entry_references,
    test_symbol_entry_argument_detection,
    test_symbol_entry_danger_detection,
    test_symbol_table_basic_operations,
    test_symbol_table_dynamic_detection,
    test_symbol_table_conflict_detection,
    test_symbol_table_creation_methods,
    test_symbol_table_named_colors,
    test_symbol_table_utility_methods,
    test_mock_engine
  ]
  
  var passed = 0
  var total = size(tests)
  
  for test_func : tests
    try
      if test_func()
        passed += 1
      else
        print("✗ Test failed")
      end
    except .. as error_type, error_message
      print("✗ Test crashed: " + str(error_type) + " - " + str(error_message))
    end
    print("")  # Add spacing between tests
  end
  
  print("=== Results: " + str(passed) + "/" + str(total) + " tests passed ===")
  
  if passed == total
    print("🎉 All symbol table tests passed!")
    return true
  else
    print("❌ Some symbol table tests failed")
    raise "test_failed"
  end
end

# Auto-run tests when file is executed
run_symbol_table_tests()