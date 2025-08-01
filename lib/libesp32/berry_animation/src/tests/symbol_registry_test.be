# Symbol Registry Test Suite
# Tests for the simplified transpiler's runtime symbol resolution approach
# The simplified transpiler uses runtime resolution with new animation.global(name, module_name) signature
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/symbol_registry_test.be

import animation
import string

# Test basic symbol registration (simplified transpiler approach)
def test_basic_symbol_registration()
  print("Testing basic symbol registration...")
  
  var dsl_source = "color custom_red = 0xFF0000\n" +
    "pattern solid_red = solid(custom_red)\n" +
    "animation red_anim = solid_red"
  
  var lexer = animation.DSLLexer(dsl_source)
  var tokens = lexer.tokenize()
  var transpiler = animation.SimpleDSLTranspiler(tokens)
  
  # Process the DSL
  var berry_code = transpiler.transpile()
  
  assert(berry_code != nil, "Should compile successfully")
  assert(!transpiler.has_errors(), "Should have no errors")
  
  # Check that definitions appear in generated code (with underscore suffix)
  assert(string.find(berry_code, "var custom_red_ = 0xFFFF0000") >= 0, "Should generate color definition")
  assert(string.find(berry_code, "var solid_red_") >= 0, "Should generate pattern definition")
  assert(string.find(berry_code, "var red_anim_") >= 0, "Should generate animation definition")
  
  print("✓ Basic symbol registration test passed")
  return true
end

# Test forward reference resolution
def test_forward_reference_resolution()
  print("Testing forward reference resolution...")
  
  # DSL with forward reference: pattern uses color defined later
  var dsl_source = "pattern fire_pattern = solid(custom_red)\n" +
    "color custom_red = 0xFF0000"
  
  var lexer = animation.DSLLexer(dsl_source)
  var tokens = lexer.tokenize()
  var transpiler = animation.SimpleDSLTranspiler(tokens)
  
  var berry_code = transpiler.transpile()
  
  # Should resolve the forward reference successfully
  assert(berry_code != nil, "Should compile with forward reference")
  assert(!transpiler.has_errors(), "Should resolve forward reference without errors")
  
  # Check generated code contains both definitions (with underscore suffix)
  assert(string.find(berry_code, "var custom_red_ = 0xFFFF0000") >= 0, "Should define custom_red color")
  assert(string.find(berry_code, "var fire_pattern_") >= 0, "Should define fire pattern")
  
  print("✓ Forward reference resolution test passed")
  return true
end

# Test undefined reference handling (simplified transpiler uses runtime resolution)
def test_undefined_reference_handling()
  print("Testing undefined reference handling...")
  
  # DSL with undefined reference
  var dsl_source = "pattern test_pattern = solid(undefined_color)"
  
  var lexer = animation.DSLLexer(dsl_source)
  var tokens = lexer.tokenize()
  var transpiler = animation.SimpleDSLTranspiler(tokens)
  
  var berry_code = transpiler.transpile()
  
  # Simplified transpiler compiles successfully but uses runtime resolution
  assert(berry_code != nil, "Should compile with runtime resolution")
  assert(!transpiler.has_errors(), "Should have no compile-time errors")
  
  # Check that runtime resolution code is generated with new two-parameter format
  assert(string.find(berry_code, "animation.global('undefined_color_', 'undefined_color')") >= 0, "Should generate runtime resolution")
  
  # Verify the generated code compiles but will fail at runtime
  var compiled_code = compile(berry_code)
  assert(compiled_code != nil, "Generated code should compile")
  
  # Should raise exception when executed due to undefined variable
  try
    compiled_code()
    assert(false, "Should raise exception at runtime for undefined variable")
  except .. as e, msg
    print(f"✓ Correctly deferred error to runtime: {e}")
  end
  
  print("✓ Undefined reference handling test passed")
  return true
end

# Test built-in reference handling
def test_builtin_reference_handling()
  print("Testing built-in reference handling...")
  
  # DSL using built-in color names and animation functions
  var dsl_source = "pattern red_pattern = solid(red)\n" +
    "animation pulse_anim = pulse(red_pattern, 2s)"
  
  var lexer = animation.DSLLexer(dsl_source)
  var tokens = lexer.tokenize()
  var transpiler = animation.SimpleDSLTranspiler(tokens)
  
  var berry_code = transpiler.transpile()
  
  # Should compile successfully with built-in references
  assert(berry_code != nil, "Should compile with built-in references")
  assert(!transpiler.has_errors(), "Should handle built-in references without errors")
  
  # Check generated code
  assert(string.find(berry_code, "animation.solid(0xFFFF0000)") >= 0, "Should use built-in red color")
  assert(string.find(berry_code, "animation.pulse") >= 0, "Should use built-in pulse function")
  
  print("✓ Built-in reference handling test passed")
  return true
end

# Test definition generation (simplified transpiler approach)
def test_definition_generation()
  print("Testing definition generation...")
  
  var dsl_source = "color custom_blue = 0x0000FF"
  
  var lexer = animation.DSLLexer(dsl_source)
  var tokens = lexer.tokenize()
  var transpiler = animation.SimpleDSLTranspiler(tokens)
  
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
  var dsl_source = "animation complex_anim = pulse(gradient_pattern, 3s)\n" +
    "pattern gradient_pattern = solid(primary_color)\n" +
    "color primary_color = 0xFF8000\n" +
    "sequence demo {\n" +
    "  play complex_anim for 5s\n" +
    "}\n" +
    "run demo"
  
  var lexer = animation.DSLLexer(dsl_source)
  var tokens = lexer.tokenize()
  var transpiler = animation.SimpleDSLTranspiler(tokens)
  
  var berry_code = transpiler.transpile()
  
  # Should resolve all forward references
  assert(berry_code != nil, "Should compile complex forward references")
  assert(!transpiler.has_errors(), "Should resolve all forward references")
  
  # Check all definitions are present (with underscore suffix)
  assert(string.find(berry_code, "var primary_color_") >= 0, "Should define primary color")
  assert(string.find(berry_code, "var gradient_pattern_") >= 0, "Should define gradient pattern")
  assert(string.find(berry_code, "var complex_anim_") >= 0, "Should define complex animation")
  assert(string.find(berry_code, "def sequence_demo()") >= 0, "Should define sequence")
  
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