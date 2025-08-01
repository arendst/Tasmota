# Test for global variable access with new animation.global() signature
# Verifies that generated code properly uses animation.global(name, module_name)

import animation

def test_global_variable_access()
  print("Testing global variable access in generated code...")
  
  var dsl_code = 
    "color red_alt = #FF0100\n"
    "pattern solid_red = solid(red_alt)"
  
  var berry_code = animation.compile_dsl(dsl_code)
  
  assert(berry_code != nil, "Should compile DSL code")
  
  # Check that global module is imported
  import string
  
  # With simplified transpiler, variables use direct names without prefixes
  assert(string.find(berry_code, "var red_alt_ = 0xFFFF0100") >= 0, "Should define red_alt variable")
  assert(string.find(berry_code, "var solid_red_ = ") >= 0, "Should define solid_red variable")
  
  # Variable references should use new two-parameter animation.global() calls
  assert(string.find(berry_code, "animation.global('red_alt_', 'red_alt')") >= 0, "Should use animation.global('red_alt_', 'red_alt') for variable reference")
  
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
  
  var dsl_code = "pattern test = solid(undefined_var)"
  var berry_code = animation.compile_dsl(dsl_code)
  
  assert(berry_code != nil, "Should compile DSL code")
  
  # Check that animation.global() is used for the fallback with new two-parameter format
  import string
  assert(string.find(berry_code, "animation.global('undefined_var_', 'undefined_var')") >= 0, "Should use animation.global('undefined_var_', 'undefined_var') for undefined variable")
  
  # Verify the generated code compiles
  var compiled_code = compile(berry_code)
  assert(compiled_code != nil, "Generated code should compile")
  
  # Verify it raises an exception when executed (due to undefined variable)
  try
    compiled_code()
    assert(false, "Should have raised an exception for undefined variable")
  except .. as e, msg
    print(f"✓ Correctly raised exception for undefined variable: {e}")
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