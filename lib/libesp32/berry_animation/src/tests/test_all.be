# Run all tests for the Berry Animation Framework
#
# This script runs all the test files in the tests directory
# and reports the overall results.

import global
import tasmota

def log(x,l) tasmota.log(x,l) end

# Import the animation module
import animation
import user_functions

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
    "lib/libesp32/berry_animation/src/tests/sine_int_test.be",

    # Core framework tests
    "lib/libesp32/berry_animation/src/tests/frame_buffer_test.be",
    "lib/libesp32/berry_animation/src/tests/constraint_encoding_test.be",  # Tests parameter constraint encoding/decoding
    "lib/libesp32/berry_animation/src/tests/nillable_parameter_test.be",
    "lib/libesp32/berry_animation/src/tests/parameterized_object_test.be",  # Tests parameter management base class
    "lib/libesp32/berry_animation/src/tests/bytes_type_test.be",  # Tests bytes type validation in parameterized objects
    "lib/libesp32/berry_animation/src/tests/animation_test.be",
    "lib/libesp32/berry_animation/src/tests/animation_engine_test.be",
    "lib/libesp32/berry_animation/src/tests/animation_opacity_test.be",
    "lib/libesp32/berry_animation/src/tests/fast_loop_integration_test.be",
    "lib/libesp32/berry_animation/src/tests/solid_animation_test.be",  # Tests unified solid() function
    "lib/libesp32/berry_animation/src/tests/solid_unification_test.be",  # Tests solid unification
    
    # Animation effect tests
    "lib/libesp32/berry_animation/src/tests/filled_animation_test.be",
    # "lib/libesp32/berry_animation/src/tests/pulse_animation_test.be",
    # "lib/libesp32/berry_animation/src/tests/breathe_animation_test.be",
    "lib/libesp32/berry_animation/src/tests/color_cycle_animation_test.be",
    "lib/libesp32/berry_animation/src/tests/color_cycle_bytes_test.be",  # Tests ColorCycleColorProvider with bytes palette
    "lib/libesp32/berry_animation/src/tests/color_cycle_palette_size_test.be",  # Tests ColorCycleColorProvider palette_size read-only parameter
    "lib/libesp32/berry_animation/src/tests/rich_palette_animation_test.be",
    "lib/libesp32/berry_animation/src/tests/rich_palette_animation_class_test.be",
    "lib/libesp32/berry_animation/src/tests/comet_animation_test.be",
    "lib/libesp32/berry_animation/src/tests/fire_animation_test.be",
    "lib/libesp32/berry_animation/src/tests/twinkle_animation_test.be",
    "lib/libesp32/berry_animation/src/tests/crenel_position_animation_test.be",
    "lib/libesp32/berry_animation/src/tests/beacon_animation_test.be",
    "lib/libesp32/berry_animation/src/tests/gradient_animation_test.be",
    "lib/libesp32/berry_animation/src/tests/noise_animation_test.be",
    # "lib/libesp32/berry_animation/src/tests/plasma_animation_test.be",
    # "lib/libesp32/berry_animation/src/tests/sparkle_animation_test.be",
    "lib/libesp32/berry_animation/src/tests/wave_animation_test.be",
    "lib/libesp32/berry_animation/src/tests/palette_pattern_animation_test.be",
    
    # Motion effects tests
    # "lib/libesp32/berry_animation/src/tests/shift_animation_test.be",
    # "lib/libesp32/berry_animation/src/tests/bounce_animation_test.be",
    # "lib/libesp32/berry_animation/src/tests/scale_animation_test.be",
    # "lib/libesp32/berry_animation/src/tests/jitter_animation_test.be",
    # "lib/libesp32/berry_animation/src/tests/motion_effects_test.be",
    
    # Color and parameter tests
    "lib/libesp32/berry_animation/src/tests/crenel_position_color_test.be",
    "lib/libesp32/berry_animation/src/tests/get_param_value_test.be",
    "lib/libesp32/berry_animation/src/tests/parameter_validation_test.be",
    
    # Sequence and timing tests
    "lib/libesp32/berry_animation/src/tests/sequence_manager_test.be",
    "lib/libesp32/berry_animation/src/tests/sequence_manager_layering_test.be",
    "lib/libesp32/berry_animation/src/tests/black_frame_fix_test.be",
    
    # Value provider tests
    "lib/libesp32/berry_animation/src/tests/core_value_provider_test.be",
    "lib/libesp32/berry_animation/src/tests/test_time_ms_requirement.be",
    "lib/libesp32/berry_animation/src/tests/value_provider_test.be",
    "lib/libesp32/berry_animation/src/tests/oscillator_value_provider_test.be",
    "lib/libesp32/berry_animation/src/tests/oscillator_ease_test.be",
    "lib/libesp32/berry_animation/src/tests/oscillator_elastic_bounce_test.be",
    "lib/libesp32/berry_animation/src/tests/strip_length_provider_test.be",
    "lib/libesp32/berry_animation/src/tests/closure_value_provider_test.be",
    "lib/libesp32/berry_animation/src/tests/breathe_color_provider_test.be",
    
    # DSL tests
    "lib/libesp32/berry_animation/src/tests/dsl_lexer_test.be",
    "lib/libesp32/berry_animation/src/tests/pull_lexer_test.be",
    "lib/libesp32/berry_animation/src/tests/pull_lexer_transpiler_test.be",
    "lib/libesp32/berry_animation/src/tests/token_test.be",
    "lib/libesp32/berry_animation/src/tests/global_variable_test.be",
    "lib/libesp32/berry_animation/src/tests/dsl_transpiler_test.be",
    "lib/libesp32/berry_animation/src/tests/dsl_compilation_test.be",
    "lib/libesp32/berry_animation/src/tests/dsl_core_processing_test.be",
    "lib/libesp32/berry_animation/src/tests/simplified_transpiler_test.be",
    "lib/libesp32/berry_animation/src/tests/symbol_registry_test.be",
    "lib/libesp32/berry_animation/src/tests/nested_function_calls_test.be",
    "lib/libesp32/berry_animation/src/tests/user_functions_test.be",
    "lib/libesp32/berry_animation/src/tests/palette_dsl_test.be",
    "lib/libesp32/berry_animation/src/tests/dsl_parameter_validation_test.be",
    "lib/libesp32/berry_animation/src/tests/dsl_value_provider_validation_test.be",
    "lib/libesp32/berry_animation/src/tests/dsl_template_validation_test.be",
    "lib/libesp32/berry_animation/src/tests/dsl_undefined_identifier_test.be",
    "lib/libesp32/berry_animation/src/tests/dsl_newline_syntax_test.be",
    "lib/libesp32/berry_animation/src/tests/test_math_method_transpilation.be",
    "lib/libesp32/berry_animation/src/tests/test_user_functions_in_computed_parameters.be",
    "lib/libesp32/berry_animation/src/tests/dsl_berry_code_blocks_test.be",
    "lib/libesp32/berry_animation/src/tests/dsl_lexer_triple_quotes_test.be",
    "lib/libesp32/berry_animation/src/tests/dsl_berry_integration_test.be",
    "lib/libesp32/berry_animation/src/tests/dsl_restart_test.be",
    
    # Event system tests
    "lib/libesp32/berry_animation/src/tests/event_system_test.be"
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