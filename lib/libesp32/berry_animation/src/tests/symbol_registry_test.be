# Symbol Registry Test Suite
# Tests for the simplified transpiler's runtime symbol resolution approach
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
  
  var lexer = animation_dsl.create_lexer(dsl_source)
  var transpiler = animation_dsl.SimpleDSLTranspiler(lexer)
  
  # Process the DSL
  var berry_code = transpiler.transpile()
  
  assert(berry_code != nil, "Should compile successfully")
  # No error check needed - transpiler would have raised exception if there were errors
  
  # Check that definitions appear in generated code (with underscore suffix)
  assert(string.find(berry_code, "var custom_red_ = 0xFFFF0000") >= 0, "Should generate color definition")
  assert(string.find(berry_code, "var solid_red_ = animation.solid(engine)") >= 0, "Should generate animation definition")
  assert(string.find(berry_code, "solid_red_.color = custom_red_") >= 0, "Should set color parameter")
  assert(string.find(berry_code, "var red_anim_") >= 0, "Should generate animation reference")
  
  print("✓ Basic symbol registration test passed")
  return true
end

# Test proper symbol ordering (no forward references)
def test_proper_symbol_ordering()
  print("Testing proper symbol ordering...")
  
  # DSL with proper ordering: color defined before animation uses it
  var dsl_source = "color custom_red = 0xFF0000\n" +
    "animation fire_pattern = solid(color=custom_red)"
  
  var lexer = animation_dsl.create_lexer(dsl_source)
  var transpiler = animation_dsl.SimpleDSLTranspiler(lexer)
  
  var berry_code = transpiler.transpile()
  
  # Should compile successfully with proper ordering
  assert(berry_code != nil, "Should compile with proper symbol ordering")
  # No error check needed - transpiler would have raised exception if there were errors
  
  # Check generated code contains both definitions (with underscore suffix)
  assert(string.find(berry_code, "var custom_red_ = 0xFFFF0000") >= 0, "Should define custom_red color")
  assert(string.find(berry_code, "var fire_pattern_ = animation.solid(engine)") >= 0, "Should define fire animation")
  assert(string.find(berry_code, "fire_pattern_.color = custom_red_") >= 0, "Should reference custom_red")
  
  print("✓ Proper symbol ordering test passed")
  return true
end

# Test undefined reference handling (should fail at transpile time)
def test_undefined_reference_handling()
  print("Testing undefined reference handling...")
  
  # DSL with undefined reference
  var dsl_source = "animation test_pattern = solid(color=undefined_color)"
  
  var lexer = animation_dsl.create_lexer(dsl_source)
  var transpiler = animation_dsl.SimpleDSLTranspiler(lexer)
  
  # Should detect undefined reference at transpile time and raise exception
  try
    var berry_code = transpiler.transpile()
    assert(false, "Should raise exception for undefined reference")
  except "dsl_compilation_error" as e, msg
    # Check that error message mentions the undefined symbol
    assert(string.find(msg, "undefined_color") >= 0, "Error should mention undefined_color")
    assert(string.find(msg, "Unknown identifier") >= 0, "Should be an unknown identifier error")
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
  
  var lexer = animation_dsl.create_lexer(dsl_source)
  var transpiler = animation_dsl.SimpleDSLTranspiler(lexer)
  
  var berry_code = transpiler.transpile()
  
  # Should compile successfully with built-in references
  assert(berry_code != nil, "Should compile with built-in references")
  # No error check needed - transpiler would have raised exception if there were errors
  
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
  
  var lexer = animation_dsl.create_lexer(dsl_source)
  var transpiler = animation_dsl.SimpleDSLTranspiler(lexer)
  
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

# Test complex symbol dependencies with proper ordering
def test_complex_symbol_dependencies()
  print("Testing complex symbol dependencies...")
  
  # Complex DSL with proper symbol ordering (no forward references)
  var dsl_source = "color primary_color = 0xFF8000\n" +
    "animation complex_anim = pulsating_animation(color=primary_color, period=3000)\n" +
    "animation gradient_pattern = solid(color=primary_color)\n" +
    "sequence demo {\n" +
    "  play complex_anim for 5s\n" +
    "}\n" +
    "run demo"
  
  var lexer = animation_dsl.create_lexer(dsl_source)
  var transpiler = animation_dsl.SimpleDSLTranspiler(lexer)
  
  var berry_code = transpiler.transpile()
  
  # Should compile successfully with proper ordering
  assert(berry_code != nil, "Should compile complex dependencies")
  # No error check needed - transpiler would have raised exception if there were errors
  
  # Check all definitions are present (with underscore suffix)
  assert(string.find(berry_code, "var primary_color_") >= 0, "Should define primary color")
  assert(string.find(berry_code, "var gradient_pattern_") >= 0, "Should define gradient pattern")
  assert(string.find(berry_code, "var complex_anim_") >= 0, "Should define complex animation")
  assert(string.find(berry_code, "var demo_ = animation.SequenceManager(engine)") >= 0, "Should define sequence")
  
  print("✓ Complex symbol dependencies test passed")
  return true
end

# Run all symbol registry tests
def run_symbol_registry_tests()
  print("=== Symbol Registry Test Suite ===")
  
  var tests = [
    test_basic_symbol_registration,
    test_proper_symbol_ordering,
    test_undefined_reference_handling,
    test_builtin_reference_handling,
    test_definition_generation,
    test_complex_symbol_dependencies
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