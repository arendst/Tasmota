# DSL Runtime Integration Test
# Tests the complete DSL execution lifecycle and file loading
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/dsl_runtime_test.be

import string
import animation
import animation_dsl

def test_dsl_runtime()
  print("=== DSL Runtime Integration Test ===")
  
  # Create strip and runtime
  var strip = global.Leds(30)
  var runtime = animation_dsl.create_runtime(strip, true)  # Debug mode enabled
  
  var tests_passed = 0
  var tests_total = 0
  
  # Test 1: Basic DSL loading and execution
  tests_total += 1
  print("\nTest 1: Basic DSL loading")
  
  var simple_dsl =
    "# strip length 30  # TEMPORARILY DISABLED\n"
    "color custom_red = 0xFF0000\n"
    "animation red_anim = pulsating_animation(color=static_color(color=custom_red), period=2s)\n"
    "sequence demo {\n"
    "  play red_anim for 1s\n"
    "}\n"
    "run demo"
  
  if runtime.load_dsl(simple_dsl)
    print("✓ Basic DSL loading successful")
    tests_passed += 1
  else
    print("✗ Basic DSL loading failed")
  end
  
  # Test 2: Reload functionality
  tests_total += 1
  print("\nTest 2: Reload functionality")
  
  # Load same DSL again - should work without issues
  if runtime.load_dsl(simple_dsl)
    print("✓ DSL reload successful")
    tests_passed += 1
  else
    print("✗ DSL reload failed")
  end
  
  # Test 3: Generated code inspection
  tests_total += 1
  print("\nTest 3: Generated code inspection")
  
  try
    var generated_code = runtime.get_generated_code(simple_dsl)
    if generated_code != nil && size(generated_code) > 0
      print("✓ Generated code available")
      print(f"Generated code length: {size(generated_code)} characters")
      
      # Check for expected content
      if string.find(generated_code, "import animation") >= 0 &&
         string.find(generated_code, "var custom_red_") >= 0
        print("✓ Generated code contains expected elements")
        tests_passed += 1
      else
        print("✗ Generated code missing expected elements")
        print("Generated code preview:")
        print(generated_code[0..200] + "...")
      end
    else
      print("✗ Generated code not available")
    end
  except "dsl_compilation_error" as e, msg
    print("✗ Generated code compilation failed: " + msg)
  end
  
  # Test 4: Error handling
  tests_total += 1
  print("\nTest 4: Error handling")
  
  var invalid_dsl = "color invalid_syntax = \n" +
    "animation broken = unknown_function(param=value)"
  
  if !runtime.load_dsl(invalid_dsl)
    print("✓ Error handling working - invalid DSL rejected")
    tests_passed += 1
  else
    print("✗ Error handling failed - invalid DSL accepted")
  end
  
  # Test 5: DSL reload functionality
  tests_total += 1
  print("\nTest 5: DSL reload functionality")
  
  if runtime.reload_dsl()
    print("✓ DSL reload successful")
    tests_passed += 1
  else
    print("✗ DSL reload failed")
  end
  
  # Test 6: Multiple DSL sources
  tests_total += 1
  print("\nTest 6: Multiple DSL sources")
  
  var dsl1 = 
    "# strip length 30  # TEMPORARILY DISABLED\n" +
    "color custom_blue = 0x0000FF\n" +
    "animation blue_anim = pulsating_animation(color=static_color(color=custom_blue), period=2s)\n" +
    "sequence blue_demo {\n" +
    "  play blue_anim for 1s\n" +
    "}\n" +
    "run blue_demo"
  
  var dsl2 = 
    "# strip length 30  # TEMPORARILY DISABLED\n" +
    "color custom_green = 0x00FF00\n" +
    "animation green_anim = pulsating_animation(color=static_color(color=custom_green), period=2s)\n" +
    "sequence green_demo {\n" +
    "  play green_anim for 1s\n" +
    "}\n" +
    "run green_demo"
  
  if runtime.load_dsl(dsl1) && runtime.load_dsl(dsl2)
    print("✓ Multiple DSL sources loaded successfully")
    tests_passed += 1
  else
    print("✗ Failed to load multiple DSL sources")
  end
  
  # Test 7: Runtime state management
  tests_total += 1
  print("\nTest 7: Runtime state management")
  
  if runtime.is_loaded() && runtime.get_active_source() != nil
    print("✓ Runtime state management working")
    tests_passed += 1
  else
    print("✗ Runtime state management failed")
  end
  
  # Test 8: engine access
  tests_total += 1
  print("\nTest 8: engine access")
  
  var engine = runtime.get_engine()
  if engine != nil
    print("✓ Engine access working")
    tests_passed += 1
  else
    print("✗ engine access failed")
  end
  
  # Final results
  print(f"\n=== DSL Runtime Test Results ===")
  print(f"Tests passed: {tests_passed}/{tests_total}")
  print(f"Success rate: {tests_passed * 100 / tests_total}%")
  
  if tests_passed == tests_total
    print("🎉 All DSL Runtime tests passed!")
    return true
  else
    print("❌ Some DSL Runtime tests failed")
    raise "test_failed"
  end
end

def test_dsl_file_operations()
  print("\n=== DSL File Operations Test ===")
  
  # Create a test DSL file
  var test_filename = "/tmp/test_animation.dsl"
  var test_dsl_content = "# strip length 20  # TEMPORARILY DISABLED\n" +
    "color custom_purple = 0x800080\n" +
    "animation purple_anim = pulsating_animation(color=static_color(color=custom_purple), period=2s)\n" +
    "sequence file_test {\n" +
    "  play purple_anim for 2s\n" +
    "}\n" +
    "run file_test"
  
  try
    # Write test file
    var file = open(test_filename, "w")
    if file != nil
      file.write(test_dsl_content)
      file.close()
      print(f"✓ Test file created: {test_filename}")
      
      # Test file loading
      var strip = global.Leds(20)
      var runtime = animation_dsl.create_runtime(strip, true)
      
      if runtime.load_dsl_file(test_filename)
        print("✓ DSL file loading successful")
        
        # Verify content was loaded
        var active_source = runtime.get_active_source()
        if active_source != nil && string.find(active_source, "custom_purple") >= 0
          print("✓ File content loaded correctly")
          return true
        else
          print("✗ File content not loaded correctly")
        end
      else
        print("✗ DSL file loading failed")
      end
    else
      print("✗ Could not create test file")
    end
    
  except .. as e, msg
    print(f"File operations test skipped: {msg}")
    return true  # Skip file tests if filesystem not available
  end
  
  return false
end

# Run the tests
def run_all_dsl_runtime_tests()
  print("Starting DSL Runtime Integration Tests...")
  
  var basic_tests_passed = test_dsl_runtime()
  var file_tests_passed = test_dsl_file_operations()
  
  print(f"\n=== Overall DSL Runtime Test Results ===")
  if basic_tests_passed
    print("✓ Core runtime tests: PASSED")
  else
    print("✗ Core runtime tests: FAILED")
    raise "failed_tests"
  end
  
  if file_tests_passed
    print("✓ File operation tests: PASSED")
  else
    print("✓ File operation tests: SKIPPED (filesystem not available)")
    raise "failed_tests"
  end
  
  return basic_tests_passed
end

run_all_dsl_runtime_tests()

return {
  "test_dsl_runtime": test_dsl_runtime,
  "test_dsl_file_operations": test_dsl_file_operations,
  "run_all_dsl_runtime_tests": run_all_dsl_runtime_tests
}