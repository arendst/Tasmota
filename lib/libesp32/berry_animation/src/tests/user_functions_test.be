# Test suite for user-defined functions in DSL
#
# This test verifies that user-defined functions can be registered and called from DSL

import animation
import animation_dsl
import user_functions

# Load user functions
import "user_functions" as user_funcs

# Test basic user function registration
def test_user_function_registration()
  print("Testing user function registration...")
  
  # Check that functions are registered
  assert(animation.is_user_function("rand_demo"), "rand_demo function should be registered")
  assert(!animation.is_user_function("nonexistent"), "nonexistent function should not be registered")
  
  # Check that we can get functions
  var rand_demo_func = animation.get_user_function("rand_demo")
  assert(rand_demo_func != nil, "Should be able to get rand_demo function")
  
  var nonexistent_func = animation.get_user_function("nonexistent")
  assert(nonexistent_func == nil, "Should return nil for nonexistent function")
  
  print("✓ User function registration test passed")
end

# Test user function call in computed parameters
def test_user_function_in_computed_parameters()
  print("Testing user function in computed parameters...")
  
  var dsl_code = 
    "animation random_base = solid(color=blue, priority=10)\n"
    "random_base.opacity = rand_demo()\n"
    "run random_base"
  
  try
    var berry_code = animation_dsl.compile(dsl_code)
    assert(berry_code != nil, "Generated Berry code should not be nil")
  
    # Check that the generated code contains the user function call
    import string
    assert(string.find(berry_code, "animation.get_user_function('rand_demo')") >= 0, 
           "Generated code should contain user function call")
    
    print("✓ User function in computed parameters test passed")
  except "dsl_compilation_error" as e, msg
    assert(false, f"DSL compilation should not fail: {msg}")
  end
end

# Test user function with mathematical operations
def test_user_function_with_math()
  print("Testing user function with mathematical operations...")
  
  var dsl_code = 
    "animation random_bounded = solid(color=orange, priority=8)\n"
    "random_bounded.opacity = max(50, min(255, rand_demo() + 100))\n"
    "run random_bounded"
  
  try
    var berry_code = animation_dsl.compile(dsl_code)
    assert(berry_code != nil, "Generated Berry code should not be nil")
  
    # Check that the generated code contains both user function and math functions
    import string
    assert(string.find(berry_code, "animation.get_user_function('rand_demo')") >= 0, 
           "Generated code should contain user function call")
    assert(string.find(berry_code, "animation._math.max(") >= 0, 
           "Generated code should contain math function call")
    
    print("✓ User function with math test passed")
  except "dsl_compilation_error" as e, msg
    assert(false, f"DSL compilation should not fail: {msg}")
  end
end

# Test user function in arithmetic expressions
def test_user_function_in_arithmetic()
  print("Testing user function in arithmetic expressions...")
  
  var dsl_code = 
    "animation random_variation = solid(color=purple, priority=15)\n"
    "random_variation.opacity = abs(rand_demo() - 128) + 64\n"
    "run random_variation"
  
  try
    var berry_code = animation_dsl.compile(dsl_code)
    assert(berry_code != nil, "Generated Berry code should not be nil")
  
    # Check that the generated code contains the user function call in arithmetic
    import string
    assert(string.find(berry_code, "animation.get_user_function('rand_demo')") >= 0, 
           "Generated code should contain user function call")
    assert(string.find(berry_code, "animation._math.abs(") >= 0, 
           "Generated code should contain abs function call")
    
    print("✓ User function in arithmetic test passed")
  except "dsl_compilation_error" as e, msg
    assert(false, f"DSL compilation should not fail: {msg}")
  end
end

# Test complex expressions with user functions
def test_complex_user_function_expressions()
  print("Testing complex expressions with user functions...")
  
  var dsl_code = 
    "animation random_complex = solid(color=white, priority=20)\n"
    "random_complex.opacity = round((rand_demo() + 128) / 2 + abs(rand_demo() - 100))\n"
    "run random_complex"
  
  try
    var berry_code = animation_dsl.compile(dsl_code)
    assert(berry_code != nil, "Generated Berry code should not be nil")
  
    # Check that the generated code contains multiple user function calls
    import string
    var rand_demo_count = 0
    var pos = 0
    while true
      pos = string.find(berry_code, "animation.get_user_function('rand_demo')", pos)
      if pos < 0 break end
      rand_demo_count += 1
      pos += 1
    end
    assert(rand_demo_count >= 2, "Generated code should contain multiple rand_demo calls")
    
    print("✓ Complex user function expressions test passed")
  except "dsl_compilation_error" as e, msg
    assert(false, f"DSL compilation should not fail: {msg}")
  end
end

# Test that generated code is valid Berry syntax
def test_generated_code_validity()
  print("Testing generated code validity with user functions...")
  
  var dsl_code = 
    "animation random_multi = solid(color=cyan, priority=12)\n"
    "random_multi.opacity = rand_demo()\n"
    "random_multi.duration = max(100, rand_demo())\n"
    "run random_multi"
  
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
    test_user_function_in_computed_parameters()
    test_user_function_with_math()
    test_user_function_in_arithmetic()
    test_complex_user_function_expressions()
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