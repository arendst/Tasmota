# Test suite for user-defined functions in DSL
#
# This test verifies that user-defined functions can be registered and called from DSL

import animation
import animation_dsl

# Load user functions
import "user_functions" as user_funcs

# Test basic user function registration
def test_user_function_registration()
  print("Testing user function registration...")
  
  # Check that functions are registered
  assert(animation.is_user_function("breathing"), "breathing function should be registered")
  assert(animation.is_user_function("fire"), "fire function should be registered")
  assert(!animation.is_user_function("nonexistent"), "nonexistent function should not be registered")
  
  # Check that we can get functions
  var breathing_func = animation.get_user_function("breathing")
  assert(breathing_func != nil, "Should be able to get breathing function")
  
  var nonexistent_func = animation.get_user_function("nonexistent")
  assert(nonexistent_func == nil, "Should return nil for nonexistent function")
  
  print("✓ User function registration test passed")
end

# Test user function call in DSL
def test_user_function_in_dsl()
  print("Testing user function call in DSL...")
  
  var dsl_code = 
    "strip length 30\n"
    "color custom_red = 0xFF0000\n"
    "animation red_breathing = breathing(custom_red, 4s)\n"
    "run red_breathing"
  
  try
    var berry_code = animation_dsl.compile(dsl_code)
    assert(berry_code != nil, "Generated Berry code should not be nil")
  
    # Check that the generated code contains the user function call
    import string
    assert(string.find(berry_code, "animation.get_user_function('breathing')") >= 0, 
           "Generated code should contain user function call")
    
    print("✓ User function in DSL test passed")
  except "dsl_compilation_error" as e, msg
    assert(false, f"DSL compilation should not fail: {msg}")
  end
end

# Test nested user function calls
def test_nested_user_function_calls()
  print("Testing nested user function calls...")
  
  # Register a function that calls another user function
  def complex_effect(base_color, speed)
    return animation.get_user_function("breathing")(base_color, speed)
  end
  animation.register_user_function("complex", complex_effect)
  
  var dsl_code = 
    "strip length 30\n"
    "color custom_blue = 0x0000FF\n"
    "animation complex_blue = complex(custom_blue, 2s)\n"
    "run complex_blue"
  
  try
    var berry_code = animation_dsl.compile(dsl_code)
    assert(berry_code != nil, "Generated Berry code should not be nil")
  
    # Check that the generated code contains the user function call
    import string
    assert(string.find(berry_code, "animation.get_user_function('complex')") >= 0, 
           "Generated code should contain nested user function call")
    
    print("✓ Nested user function calls test passed")
  except "dsl_compilation_error" as e, msg
    assert(false, f"DSL compilation should not fail: {msg}")
  end
end

# Test user function with multiple parameters
def test_user_function_multiple_parameters()
  print("Testing user function with multiple parameters...")
  
  var dsl_code = 
    "strip length 30\n"
    "animation sparkles = sparkle(red, white, 15%)\n"
    "run sparkles"
  
  try
    var berry_code = animation_dsl.compile(dsl_code)
    assert(berry_code != nil, "Generated Berry code should not be nil")
  
    # Check that the generated code contains the user function call with parameters
    import string
    assert(string.find(berry_code, "animation.get_user_function('sparkle')") >= 0, 
           "Generated code should contain user function call")
    assert(string.find(berry_code, "0xFFFF0000, 0xFFFFFFFF") >= 0, 
           "Generated code should contain color parameters")
    
    print("✓ User function multiple parameters test passed")
  except "dsl_compilation_error" as e, msg
    assert(false, f"DSL compilation should not fail: {msg}")
  end
end

# Test user function in nested calls
def test_user_function_in_nested_calls()
  print("Testing user function in nested calls...")
  
  var dsl_code = 
    "strip length 30\n"
    "color custom_red = 0xFF0000\n"
    "animation complex = pulsating_animation(color=breathing(custom_red, 3s), period=2s)\n"
    "run complex"
  
  try
    var berry_code = animation_dsl.compile(dsl_code)
    assert(berry_code != nil, "Generated Berry code should not be nil")
  
    # Check that both user and built-in functions are handled correctly
    import string
    assert(string.find(berry_code, "animation.get_user_function('breathing')") >= 0, 
           "Generated code should contain user function call")
    assert(string.find(berry_code, "animation.pulsating_animation(") >= 0, 
           "Generated code should contain built-in function call")
    
    print("✓ User function in nested calls test passed")
  except "dsl_compilation_error" as e, msg
    assert(false, f"DSL compilation should not fail: {msg}")
  end
end

# Test that generated code is valid Berry syntax
def test_generated_code_validity()
  print("Testing generated code validity with user functions...")
  
  var dsl_code = 
    "strip length 30\n"
    "color custom_red = 0xFF0000\n"
    "animation red_fire = fire(200, 500ms)\n"
    "run red_fire"
  
  try
    var berry_code = animation_dsl.compile(dsl_code)
    assert(berry_code != nil, "Generated Berry code should not be nil")
  
    # Try to compile the generated Berry code (basic syntax check)
    try
      compile(berry_code)
      print("✓ Generated code compiles successfully")
    except .. as e, msg
      print(f"Generated code compilation failed: {e} - {msg}")
      print("Generated code:")
      print(berry_code)
      assert(false, "Generated code should be valid Berry syntax")
    end
    
    print("✓ Generated code validity test passed")
  except "dsl_compilation_error" as e, msg
    assert(false, f"DSL compilation should not fail: {msg}")
  end
end

# Run all tests
def run_user_functions_tests()
  print("=== User Functions Tests ===")
  
  try
    test_user_function_registration()
    test_user_function_in_dsl()
    test_nested_user_function_calls()
    test_user_function_multiple_parameters()
    test_user_function_in_nested_calls()
    test_generated_code_validity()
    
    print("=== All user functions tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_user_functions_tests = run_user_functions_tests

run_user_functions_tests()

return run_user_functions_tests