# Test suite for the simplified DSL transpiler
# Verifies that the simplified version produces the same results as the original

import animation
import animation_dsl

def test_basic_transpilation()
  print("Testing basic DSL transpilation...")
  
  # Create a simple DSL program with custom color names (not predefined ones)
  var dsl_code = 
    "# strip length 30  # TEMPORARILY DISABLED\n"
    "color my_red = 0xFF0000\n"
    "color my_blue = 0x0000FF\n"
    "animation solid_red = solid(color=my_red)\n"
    "animation pulse_red = pulsating_animation(color=my_red, period=2000)\n"
    "sequence demo {\n"
    "  play pulse_red for 3s\n"
    "  wait 1s\n"
    "}\n"
    "run demo"
  
  # Compile the DSL
  var berry_code = animation_dsl.compile(dsl_code)
  
  if berry_code == nil
    print("✗ Compilation failed")
    return false
  end
  
  print("✓ Basic transpilation test passed")
  return true
end

def test_color_resolution()
  print("Testing color resolution...")
  
  # Test that named colors work
  var dsl_code = 
    "# strip length 10  # TEMPORARILY DISABLED\n"
    "animation red_pattern = solid(color=red)\n"
    "animation blue_pattern = solid(color=blue)\n"
    "run red_pattern"
  
  var berry_code = animation_dsl.compile(dsl_code)
  
  if berry_code == nil
    print("✗ Color resolution test failed")
    return false
  end
  
  # Check that named colors are properly resolved
  import string
  if string.find(berry_code, "0xFFFF0000") == -1
    print("✗ Red color not properly resolved")
    return false
  end
  
  print("✓ Color resolution test passed")
  return true
end

def test_function_calls()
  print("Testing function calls...")
  
  var dsl_code = 
    "# strip length 20  # TEMPORARILY DISABLED\n"
    "animation solid_red = solid(color=red)\n"
    "animation test_anim = pulsating_animation(color=red, period=1000)\n"
    "run test_anim"
  
  var berry_code = animation_dsl.compile(dsl_code)
  
  if berry_code == nil
    print("✗ Function call test failed")
    return false
  end
  
  # Check that function calls are properly generated
  import string
  if string.find(berry_code, "animation.pulsating_animation(engine)") == -1
    print("✗ Function call not properly generated")
    return false
  end
  
  print("✓ Function call test passed")
  return true
end

def test_error_handling()
  print("Testing error handling...")
  
  # Test with syntax that should cause transpiler errors
  var dsl_code = "color = 0xFF0000"  # Missing color name
  
  try
    var berry_code = animation_dsl.compile(dsl_code)
    # Should not reach here - should throw exception for invalid syntax
    print("✗ Error handling test failed - should have thrown exception")
    return false
  except "dsl_compilation_error" as e, msg
    # This is expected - the transpiler should reject invalid syntax
    print("✓ Error handling test passed - correctly rejected invalid syntax")
    return true
  except .. as e, msg
    print(f"✗ Unexpected error type: {e} - {msg}")
    return false
  end
end

def run_simplified_transpiler_tests()
  print("=== Simplified Transpiler Tests ===")
  
  var tests = [
    test_basic_transpilation,
    test_color_resolution,
    test_function_calls,
    test_error_handling
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
    except "dsl_compilation_error" as e, msg
      # DSL compilation errors are expected in some tests
      print("✗ Test failed with DSL error (this may be expected)")
    except .. as error_type, error_message
      print(f"✗ Test crashed: {error_type} - {error_message}")
    end
  end
  
  print(f"\n=== Results: {passed}/{total} tests passed ===")
  
  if passed == total
    print("🎉 All simplified transpiler tests passed!")
    return true
  else
    print("❌ Some tests failed")
    raise "test_failed"
  end
end

# Auto-run tests when file is executed
run_simplified_transpiler_tests()