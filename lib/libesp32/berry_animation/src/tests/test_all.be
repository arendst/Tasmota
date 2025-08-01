# Run all tests for the Berry Animation Framework
#
# This script runs all the test files in the tests directory
# and reports the overall results.

import global
import tasmota

# Import the animation module
import animation

# Define a function to run a test file
def run_test_file(file_path)
  print(f"Running {file_path}...")
  
  # Load the file content
  var f = open(file_path, "r")
  if f == nil
    print(f"Error: Could not open file {file_path}")
    return false
  end
  
  var content = f.read()
  f.close()
  
  # Compile and execute the file content
  try
    var compiled = compile(content)
    compiled()
    return true
  except .. as e
    print(f"Error executing {file_path}: {e}")
    return false
  end
end

# Main function to run all tests
def run_all_tests()
  print("=== Berry Animation Framework Test Suite ===")
  print("")
  
  var test_files = [
    "lib/libesp32/berry_animation/tests/sine_int_test.be",

    # Core framework tests
    "lib/libesp32/berry_animation/tests/frame_buffer_test.be",
    "lib/libesp32/berry_animation/tests/animation_test.be",
    "lib/libesp32/berry_animation/tests/animation_engine_test.be",
    "lib/libesp32/berry_animation/tests/fast_loop_integration_test.be",
    "lib/libesp32/berry_animation/tests/solid_animation_test.be",  # Tests unified solid() function
    "lib/libesp32/berry_animation/tests/solid_unification_test.be",  # Tests solid unification
    
    # Animation effect tests
    "lib/libesp32/berry_animation/tests/filled_animation_test.be",
    "lib/libesp32/berry_animation/tests/pulse_animation_test.be",
    "lib/libesp32/berry_animation/tests/breathe_animation_test.be",
    "lib/libesp32/berry_animation/tests/color_cycle_animation_test.be",
    "lib/libesp32/berry_animation/tests/rich_palette_animation_test.be",
    "lib/libesp32/berry_animation/tests/comet_animation_test.be",
    "lib/libesp32/berry_animation/tests/fire_animation_test.be",
    "lib/libesp32/berry_animation/tests/twinkle_animation_test.be",
    "lib/libesp32/berry_animation/tests/crenel_position_animation_test.be",
    "lib/libesp32/berry_animation/tests/pulse_position_animation_test.be",
    "lib/libesp32/berry_animation/tests/gradient_animation_test.be",
    "lib/libesp32/berry_animation/tests/noise_animation_test.be",
    "lib/libesp32/berry_animation/tests/plasma_animation_test.be",
    "lib/libesp32/berry_animation/tests/sparkle_animation_test.be",
    "lib/libesp32/berry_animation/tests/wave_animation_test.be",
    
    # Motion effects tests
    "lib/libesp32/berry_animation/tests/shift_animation_test.be",
    "lib/libesp32/berry_animation/tests/bounce_animation_test.be",
    "lib/libesp32/berry_animation/tests/scale_animation_test.be",
    "lib/libesp32/berry_animation/tests/jitter_animation_test.be",
    "lib/libesp32/berry_animation/tests/motion_effects_test.be",
    
    # Color and parameter tests
    "lib/libesp32/berry_animation/tests/crenel_position_color_test.be",
    "lib/libesp32/berry_animation/tests/get_param_value_test.be",
    "lib/libesp32/berry_animation/tests/resolve_value_test.be",
    "lib/libesp32/berry_animation/tests/parameter_validation_test.be",
    "lib/libesp32/berry_animation/tests/pattern_animation_distinction_test.be",
    
    # Sequence and timing tests
    "lib/libesp32/berry_animation/tests/sequence_manager_test.be",
    "lib/libesp32/berry_animation/tests/sequence_manager_layering_test.be",
    
    # Value provider tests
    "lib/libesp32/berry_animation/tests/core_value_provider_test.be",
    "lib/libesp32/berry_animation/tests/test_time_ms_requirement.be",
    "lib/libesp32/berry_animation/tests/value_provider_test.be",
    "lib/libesp32/berry_animation/tests/oscillator_value_provider_test.be",
    "lib/libesp32/berry_animation/tests/oscillator_ease_test.be",
    "lib/libesp32/berry_animation/tests/oscillator_elastic_bounce_test.be",
    
    # DSL tests
    "lib/libesp32/berry_animation/tests/dsl_lexer_test.be",
    "lib/libesp32/berry_animation/tests/token_test.be",
    "lib/libesp32/berry_animation/tests/global_variable_test.be",
    "lib/libesp32/berry_animation/tests/dsl_transpiler_test.be",
    "lib/libesp32/berry_animation/tests/dsl_core_processing_test.be",
    "lib/libesp32/berry_animation/tests/simplified_transpiler_test.be",
    "lib/libesp32/berry_animation/tests/symbol_registry_test.be",
    "lib/libesp32/berry_animation/tests/dsl_runtime_test.be",
    "lib/libesp32/berry_animation/tests/nested_function_calls_test.be",
    "lib/libesp32/berry_animation/tests/user_functions_test.be",
    "lib/libesp32/berry_animation/tests/palette_dsl_test.be",
    
    # Event system tests
    "lib/libesp32/berry_animation/tests/event_system_test.be"
  ]
  
  var total_tests = size(test_files)
  var passed_tests = 0
  var failed_tests = []
  
  # Run each test file
  for file_path : test_files
    if run_test_file(file_path)
      passed_tests += 1
    else
      failed_tests.push(file_path)
    end
    print("")  # Add a blank line between test files
  end
  
  # Print summary
  print("=== Test Summary ===")
  print(f"Total test files: {total_tests}")
  print(f"Passed: {passed_tests}")
  print(f"Failed: {total_tests - passed_tests}")
  
  if size(failed_tests) > 0
    print("Failed test files:")
    for file_path : failed_tests
      print(f"  - {file_path}")
    end
    return false
  else
    print("All tests passed successfully!")
    return true
  end
end

# Run all tests
var success = run_all_tests()

# Return success status
return success