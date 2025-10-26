import animation
import animation_dsl
import string

# Load user functions
import "user_functions" as user_funcs

# Test to verify that user functions work correctly in computed parameters

def test_transpilation_case(dsl_code, expected_user_function, test_name)
  print(f"\n  Testing: {test_name}")
  
  var lexer = animation_dsl.create_lexer(dsl_code)
  var transpiler = animation_dsl.SimpleDSLTranspiler(lexer)
  var generated_code = transpiler.transpile()
  
  if generated_code == nil
    print("    ❌ Transpilation failed:")
    return false
  end
  
  print(f"    Generated code:\n{generated_code}")
  
  # Check that user function is called with engine parameter
  var expected_call = f"animation.get_user_function('{expected_user_function}')(engine"
  if string.find(generated_code, expected_call) < 0
    print(f"    ❌ Expected to find '{expected_call}' in generated code")
    return false
  else
    print(f"    ✅ Found user function call with engine: '{expected_call}'")
  end
  
  # Verify the code compiles
  try
    var compiled_func = compile(generated_code, test_name)
    print("    ✅ Generated code compiles successfully")
    return true
  except .. as e, msg
    print(f"    ❌ Generated code compilation failed: {msg}")
    return false
  end
end

# Test that user functions are correctly detected
def test_user_function_detection()
  print("\nTesting user function detection...")
  
  # Check that user functions are registered
  var user_functions = ["rand_demo"]
  
  for func_name : user_functions
    if !animation.is_user_function(func_name)
      print(f"    ❌ {func_name} should be detected as a user function")
      return false
    else
      print(f"    ✅ {func_name} correctly detected as user function")
    end
  end
  
  # Check that non-user functions are not detected as user functions
  var non_user_functions = ["pulsating_animation", "solid", "abs", "min", "max", "breathing", "fire", "sparkle"]
  
  for func_name : non_user_functions
    if animation.is_user_function(func_name)
      print(f"    ❌ {func_name} should NOT be detected as a user function")
      return false
    else
      print(f"    ✅ {func_name} correctly NOT detected as user function")
    end
  end
  
  return true
end

def test_user_function_in_computed_parameter()
  print("Testing user functions in computed parameters...")
  
  # Test case 1: Simple user function in computed parameter
  var dsl_code1 = 
    "import user_functions\n"
    "set strip_len = strip_length()\n"
    "animation test = pulsating_animation(color=red, period=2s)\n"
    "test.opacity = rand_demo()\n"
    "run test"
  
  var result1 = test_transpilation_case(dsl_code1, "rand_demo", "Simple user function in computed parameter")
  if !result1
    return false
  end
  
  # Test case 2: User function with mathematical functions
  var dsl_code2 = 
    "import user_functions\n"
    "set strip_len = strip_length()\n"
    "animation test = solid(color=red)\n"
    "test.opacity = max(100, rand_demo())\n"
    "run test"
  
  var result2 = test_transpilation_case(dsl_code2, "rand_demo", "User function with mathematical functions")
  if !result2
    return false
  end
  
  # Test case 3: User function in arithmetic expressions
  var dsl_code3 = 
    "import user_functions\n"
    "set strip_len = strip_length()\n"
    "animation test = solid(color=green)\n"
    "test.opacity = abs(rand_demo() - 128) + 64\n"
    "run test"
  
  var result3 = test_transpilation_case(dsl_code3, "rand_demo", "User function in arithmetic expressions")
  if !result3
    return false
  end
  
  print("\n✅ All user function computed parameter tests passed!")
  return true
end

# Run all tests
print("🧪 Testing User Functions in Computed Parameters")
print("==================================================")

var test1_result = test_user_function_detection()
var test2_result = test_user_function_in_computed_parameter()

if test1_result && test2_result
  print("\n🎉 All tests passed!")
  print("✅ User functions are correctly detected")
  print("✅ User functions work correctly in computed parameters")
  print("✅ User functions are called with engine in closure context")
else
  print("\n❌ Some tests failed!")
  raise "test_failed"
end