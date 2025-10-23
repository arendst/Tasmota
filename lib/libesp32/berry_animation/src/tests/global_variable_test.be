# Test for global variable access with new transpiler symbol resolution
# Verifies that generated code uses animation.symbol for animation module symbols
# and symbol_ for user-defined variables calls)
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/global_variable_test.be

import animation
import animation_dsl

def test_global_variable_access()
  print("Testing global variable access in generated code...")
  
  var dsl_code = 
    "color red_alt = 0xFF0100\n"
    "animation solid_red = solid(color=red_alt)"
  
  var berry_code = animation_dsl.compile(dsl_code)
  
  assert(berry_code != nil, "Should compile DSL code")
  
  # Check that global module is imported
  import string
  
  # With simplified transpiler, variables use direct names without prefixes
  assert(string.find(berry_code, "var red_alt_ = 0xFFFF0100") >= 0, "Should define red_alt variable")
  assert(string.find(berry_code, "var solid_red_ = animation.solid(engine)") >= 0, "Should define solid_red variable with new pattern")
  
  # Variable references should now use direct underscore notation
  assert(string.find(berry_code, "solid_red_.color = red_alt_") >= 0, "Should use red_alt_ directly for variable reference")
  
  # Verify the generated code actually compiles by executing it
  try
    compile(berry_code)
    print("✓ Generated code compiles successfully")
  except .. as e, msg
    print(f"✗ Generated code compilation failed: {msg}")
    assert(false, "Generated code should compile")
  end
  
  print("✓ Global variable access test passed")
  return true
end

def test_undefined_variable_exception()
  print("Testing undefined variable exception behavior...")
  
  var dsl_code = "animation test = solid(color=undefined_var)"
  
  # The new transpiler behavior is to catch undefined variables at DSL compile time
  # This is better than the old behavior - we catch errors earlier!
  try
    var berry_code = animation_dsl.compile(dsl_code)
    assert(false, "Should have failed to compile DSL due to undefined variable")
  except "dsl_compilation_error" as e, msg
    print(f"✓ Correctly failed to compile DSL due to undefined variable: {e}")
    # Verify the error message mentions the undefined variable
    import string
    assert(string.find(msg, "undefined_var") >= 0, "Error message should mention undefined_var")
  end
  
  print("✓ Undefined variable exception test passed")
  return true
end

def run_global_variable_tests()
  print("=== Global Variable Access Tests ===")
  
  try
    test_global_variable_access()
    test_undefined_variable_exception()
    print("🎉 All global variable tests passed!")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Auto-run tests when file is executed
run_global_variable_tests()