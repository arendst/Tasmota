# Symbol Registry Test Suite
# Tests for the simplified transpiler's runtime symbol resolution approach
# The simplified transpiler uses runtime resolution with new animation.global(name, module_name) signature
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/symbol_registry_test.be

import animation
import animation_dsl
import string

# Test basic symbol registration (simplified transpiler approach)
def test_basic_symbol_registration()
  print("Testing basic symbol registration...")
  
  var dsl_source = "color custom_red = 0xFF0000\n" +
    "animation solid_red = solid(color=custom_red)\n" +
    "animation red_anim = solid_red"
  
  var lexer = animation_dsl.DSLLexer(dsl_source)
  var tokens = lexer.tokenize()
  var transpiler = animation_dsl.SimpleDSLTranspiler(tokens)
  
  # Process the DSL
  var berry_code = transpiler.transpile()
  
  assert(berry_code != nil, "Should compile successfully")
  assert(!transpiler.has_errors(), "Should have no errors")
  
  # Check that definitions appear in generated code (with underscore suffix)
  assert(string.find(berry_code, "var custom_red_ = 0xFFFF0000") >= 0, "Should generate color definition")
  assert(string.find(berry_code, "var solid_red_ = animation.solid(engine)") >= 0, "Should generate animation definition")
  assert(string.find(berry_code, "solid_red_.color = custom_red_") >= 0, "Should set color parameter")
  assert(string.find(berry_code, "var red_anim_") >= 0, "Should generate animation reference")
  
  print("✓ Basic symbol registration test passed")
  return true
end

# Test forward reference resolution
def test_forward_reference_resolution()
  print("Testing forward reference resolution...")
  
  # DSL with forward reference: animation uses color defined later
  var dsl_source = "animation fire_pattern = solid(color=custom_red)\n" +
    "color custom_red = 0xFF0000"
  
  var lexer = animation_dsl.DSLLexer(dsl_source)
  var tokens = lexer.tokenize()
  var transpiler = animation_dsl.SimpleDSLTranspiler(tokens)
  
  var berry_code = transpiler.transpile()
  
  # Should resolve the forward reference successfully
  assert(berry_code != nil, "Should compile with forward reference")
  assert(!transpiler.has_errors(), "Should resolve forward reference without errors")
  
  # Check generated code contains both definitions (with underscore suffix)
  assert(string.find(berry_code, "var custom_red_ = 0xFFFF0000") >= 0, "Should define custom_red color")
  assert(string.find(berry_code, "var fire_pattern_ = animation.solid(engine)") >= 0, "Should define fire animation")
  assert(string.find(berry_code, "fire_pattern_.color = custom_red_") >= 0, "Should reference custom_red")
  
  print("✓ Forward reference resolution test passed")
  return true
end

# Test undefined reference handling (simplified transpiler uses runtime resolution)
def test_undefined_reference_handling()
  print("Testing undefined reference handling...")
  
  # DSL with undefined reference
  var dsl_source = "animation test_pattern = solid(color=undefined_color)"
  
  var lexer = animation_dsl.DSLLexer(dsl_source)
  var tokens = lexer.tokenize()
  var transpiler = animation_dsl.SimpleDSLTranspiler(tokens)
  
  var berry_code = transpiler.transpile()
  
  # New behavior: transpiler generates direct reference to undefined_color_
  assert(berry_code != nil, "Should compile with direct reference")
  assert(!transpiler.has_errors(), "Should have no compile-time errors")
  
  # Check that direct reference is generated (since undefined_color doesn't exist in animation module)
  assert(string.find(berry_code, "undefined_color_") >= 0, "Should generate runtime resolution")
  
  # With new behavior, Berry compilation will fail due to undefined variable
  # This is actually better than runtime errors as it catches issues earlier
  try
    var compiled_code = compile(berry_code)
    assert(false, "Should fail to compile due to undefined variable")
  except .. as e, msg
    print(f"✓ Correctly caught undefined variable at compile time: {e}")
    assert(string.find(str(msg), "undefined_color_") >= 0, "Error should mention undefined variable")
  end
  
  print("✓ Undefined reference handling test passed")
  return true
end

# Test built-in reference handling
def test_builtin_reference_handling()
  print("Testing built-in reference handling...")
  
  # DSL using built-in color names and animation functions
  var dsl_source = "animation red_pattern = solid(color=red)\n" +
    "animation pulse_anim = pulsating_animation(color=red, period=2000)"
  
  var lexer = animation_dsl.DSLLexer(dsl_source)
  var tokens = lexer.tokenize()
  var transpiler = animation_dsl.SimpleDSLTranspiler(tokens)
  
  var berry_code = transpiler.transpile()
  
  # Should compile successfully with built-in references
  assert(berry_code != nil, "Should compile with built-in references")
  assert(!transpiler.has_errors(), "Should handle built-in references without errors")
  
  # Check generated code
  assert(string.find(berry_code, "red_pattern_.color = 0xFFFF0000") >= 0, "Should use built-in red color")
  assert(string.find(berry_code, "animation.pulsating_animation(engine)") >= 0, "Should use built-in pulsating_animation function")
  
  print("✓ Built-in reference handling test passed")
  return true
end

# Test definition generation (simplified transpiler approach)
def test_definition_generation()
  print("Testing definition generation...")
  
  var dsl_source = "color custom_blue = 0x0000FF"
  
  var lexer = animation_dsl.DSLLexer(dsl_source)
  var tokens = lexer.tokenize()
  var transpiler = animation_dsl.SimpleDSLTranspiler(tokens)
  
  var berry_code = transpiler.transpile()
  
  # Check that definition is properly generated (with underscore suffix)
  assert(berry_code != nil, "Should compile successfully")
  assert(string.find(berry_code, "var custom_blue_ = 0xFF0000FF") >= 0, "Should generate correct color definition")
  
  # Verify the generated code compiles and executes
  var compiled_code = compile(berry_code)
  assert(compiled_code != nil, "Generated code should compile")
  
  print("✓ Definition generation test passed")
  return true
end

# Test complex forward references
def test_complex_forward_references()
  print("Testing complex forward references...")
  
  # Complex DSL with multiple forward references
  var dsl_source = "animation complex_anim = pulsating_animation(color=primary_color, period=3000)\n" +
    "animation gradient_pattern = solid(color=primary_color)\n" +
    "color primary_color = 0xFF8000\n" +
    "sequence demo {\n" +
    "  play complex_anim for 5s\n" +
    "}\n" +
    "run demo"
  
  var lexer = animation_dsl.DSLLexer(dsl_source)
  var tokens = lexer.tokenize()
  var transpiler = animation_dsl.SimpleDSLTranspiler(tokens)
  
  var berry_code = transpiler.transpile()
  
  # Should resolve all forward references
  assert(berry_code != nil, "Should compile complex forward references")
  assert(!transpiler.has_errors(), "Should resolve all forward references")
  
  # Check all definitions are present (with underscore suffix)
  assert(string.find(berry_code, "var primary_color_") >= 0, "Should define primary color")
  assert(string.find(berry_code, "var gradient_pattern_") >= 0, "Should define gradient pattern")
  assert(string.find(berry_code, "var complex_anim_") >= 0, "Should define complex animation")
  assert(string.find(berry_code, "var demo_ = (def (engine)") >= 0, "Should define sequence")
  
  print("✓ Complex forward references test passed")
  return true
end

# Run all symbol registry tests
def run_symbol_registry_tests()
  print("=== Symbol Registry Test Suite ===")
  
  var tests = [
    test_basic_symbol_registration,
    test_forward_reference_resolution,
    test_undefined_reference_handling,
    test_builtin_reference_handling,
    test_definition_generation,
    test_complex_forward_references
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
    except .. as error_type, error_message
      print("✗ Test crashed: " + str(error_type) + " - " + str(error_message))
    end
    print("")  # Add spacing between tests
  end
  
  print("=== Results: " + str(passed) + "/" + str(total) + " tests passed ===")
  
  if passed == total
    print("🎉 All symbol registry tests passed!")
    return true
  else
    print("❌ Some symbol registry tests failed")
    raise "test_failed"
  end
end

# Auto-run tests when file is executed
run_symbol_registry_tests()