import animation
import animation_dsl
import string

# Test to verify that mathematical methods in computed parameters are correctly transpiled to animation._math.<func>()

def test_transpilation_case(dsl_code, expected_methods, test_name)
  print(f"\n  Testing: {test_name}")
  
  var lexer = animation_dsl.create_lexer(dsl_code)
  var transpiler = animation_dsl.SimpleDSLTranspiler(lexer)
  var generated_code = transpiler.transpile()
  
  if generated_code == nil
    print("    ❌ Transpilation failed:")
    return false
  end
  
  print(f"    Generated code:\n{generated_code}")
  

  
  # Check that mathematical methods are prefixed with animation._math.
  var methods_to_check = []
  if type(expected_methods) == "instance"  # Berry lists are of type "instance"
    methods_to_check = expected_methods
  else
    methods_to_check = [expected_methods]
  end
  
  for method : methods_to_check
    var self_method = f"animation._math.{method}("
    if string.find(generated_code, self_method) < 0
      print(f"    ❌ Expected to find 'animation._math.{method}(' in generated code")
      return false
    else
      print(f"    ✅ Found 'animation._math.{method}(' in generated code")
    end
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

def test_non_math_functions(dsl_code)
  print("\n  Testing: Non-math functions should NOT be prefixed with animation._math.")
  
  var lexer = animation_dsl.create_lexer(dsl_code)
  var transpiler = animation_dsl.SimpleDSLTranspiler(lexer)
  var generated_code = transpiler.transpile()
  
  if generated_code == nil
    print("    ❌ Transpilation failed:")
    return false
  end
  
  print(f"    Generated code:\n{generated_code}")
  
  # Check that 'scale' is prefixed with animation._math. (it's a math method)
  if string.find(generated_code, "animation._math.scale(") < 0
    print("    ❌ Expected to find 'animation._math.scale(' in generated code")
    return false
  else
    print("    ✅ Found 'animation._math.scale(' in generated code")
  end
  
  # Check that animation functions like 'pulsating_animation' are NOT prefixed with animation._math.
  if string.find(generated_code, "animation._math.pulsating_animation") >= 0
    print("    ❌ Found 'animation._math.pulsating_animation' - animation functions should NOT be prefixed")
    return false
  else
    print("    ✅ Animation functions correctly NOT prefixed with animation._math.")
  end
  
  return true
end

# Test the dynamic introspection method directly
def test_is_math_method_function()
  print("\nTesting is_math_method() function directly...")
  
  var dummy_lexer = animation_dsl.create_lexer("")
  var transpiler = animation_dsl.SimpleDSLTranspiler(dummy_lexer)
  
  # Test mathematical methods
  var math_methods = ["min", "max", "abs", "round", "sqrt", "scale", "sin", "cos"]
  for method : math_methods
    var entry = transpiler.symbol_table.get(method)
    if entry == nil || entry.type != animation_dsl._symbol_entry.TYPE_MATH_FUNCTION
      print(f"    ❌ {method} should be detected as a math method")
      return false
    else
      print(f"    ✅ {method} correctly detected as math method")
    end
  end
  
  # Test non-mathematical methods
  var non_math_methods = ["pulsating_animation", "solid", "color_cycle", "unknown_method"]
  for method : non_math_methods
    var entry = transpiler.symbol_table.get(method)
    if entry != nil && entry.type == animation_dsl._symbol_entry.TYPE_MATH_FUNCTION
      print(f"    ❌ {method} should NOT be detected as a math method")
      return false
    else
      print(f"    ✅ {method} correctly NOT detected as math method")
    end
  end
  
  return true
end

def test_math_method_transpilation()
  print("Testing mathematical method transpilation in computed parameters...")
  
  # Test case 1: Simple mathematical function in computed parameter
  var dsl_code1 = 
    "set value = 50\n"
    "animation test = pulsating_animation(color=red, period=2s)\n"
    "test.opacity = abs(value - 100)\n"
    "run test"
  
  var result1 = test_transpilation_case(dsl_code1, "abs", "Simple abs() function")
  if !result1
    return false
  end
  
  # Test case 2: Multiple mathematical functions
  var dsl_code2 = 
    "set x = 10\n"
    "set y = 20\n"
    "animation wave = pulsating_animation(color=blue, period=3s)\n"
    "wave.min_brightness = max(min(x, y), sqrt(abs(x - y)))\n"
    "run wave"
  
  var result2 = test_transpilation_case(dsl_code2, ["max", "min", "sqrt", "abs"], "Multiple math functions")
  if !result2
    return false
  end
  
  # Test case 3: Mathematical functions with complex expressions
  var dsl_code3 = 
    "set angle = 45\n"
    "animation rotate = pulsating_animation(color=green, period=2s)\n"
    "rotate.min_brightness = round(sin(angle) * 180 + cos(angle) * 90)\n"
    "run rotate"
  
  var result3 = test_transpilation_case(dsl_code3, ["round", "sin", "cos"], "Complex math expressions")
  if !result3
    return false
  end
  
  # Test case 4: Ensure non-math functions are NOT prefixed with animation._math.
  var dsl_code4 = 
    "animation pulse = pulsating_animation(color=red, period=2s)\n"
    "pulse.min_brightness = scale(50, 0, 100)\n"
    "run pulse"
  
  var result4 = test_non_math_functions(dsl_code4)
  if !result4
    return false
  end
  
  print("\n✅ All mathematical method transpilation tests passed!")
  return true
end

# Run all tests
print("🧪 Testing Mathematical Method Transpilation")
print("==================================================")

var test1_result = test_is_math_method_function()
var test2_result = test_math_method_transpilation()

if test1_result && test2_result
  print("\n🎉 All tests passed!")
  print("✅ Mathematical methods are correctly transpiled to animation._math.<method>() calls")
  print("✅ Non-mathematical functions are correctly left unchanged")
  print("✅ Dynamic introspection is working properly at transpile time")
else
  print("\n❌ Some tests failed!")
  raise "test_failed"
end