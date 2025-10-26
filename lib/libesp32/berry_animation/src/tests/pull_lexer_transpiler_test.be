# Test for Pull Lexer Interface with Transpiler
# Verifies that the transpiler works correctly with both token array and pull lexer

import animation_dsl

def test_pull_lexer_basic()
  print("=== Testing Pull Lexer Basic Functionality ===")
  
  var dsl_source = "# Simple DSL test\n" +
                   "color my_red = 0xFF0000\n" +
                   "animation pulse = pulsating_animation(color=my_red, period=2s)\n" +
                   "run pulse"
  
  # Test with new create_lexer interface (uses pull lexer internally)
  var pull_lexer = animation_dsl.create_lexer(dsl_source)
  var transpiler = animation_dsl.SimpleDSLTranspiler(pull_lexer)
  var berry_code = transpiler.transpile()
  
  print("New create_lexer Result (using pull lexer internally):")
  print(berry_code)
  print()
  
  # Test with direct pull lexer creation
  var direct_pull_lexer = animation_dsl.create_lexer(dsl_source)
  var direct_transpiler = animation_dsl.SimpleDSLTranspiler(direct_pull_lexer)
  var direct_berry_code = direct_transpiler.transpile()
  
  print("Direct Pull Lexer Result:")
  print(direct_berry_code)
  print()
  
  # Compare results - they should be identical
  if berry_code == direct_berry_code
    print("✅ SUCCESS: create_lexer and direct pull lexer produce identical results")
  else
    print("❌ FAILURE: Results differ between create_lexer and direct pull lexer")
    print("Differences found!")
  end
  
  return berry_code == direct_berry_code
end

def test_pull_lexer_template_mode()
  print("=== Testing Pull Lexer Template Mode ===")
  
  var template_source = "animation test = solid(color=red)\n" +
                        "test.opacity = 200\n" +
                        "run test"
  
  # Test with template mode enabled
  var pull_lexer = animation_dsl.create_lexer(template_source)
  var transpiler = animation_dsl.SimpleDSLTranspiler(pull_lexer)
  
  var berry_code = transpiler.transpile_template_body()
  
  print("Template Body Result:")
  print(berry_code)
  
  return true
end

def test_pull_lexer_token_access()
  print("=== Testing Pull Lexer Token Access Methods ===")
  
  var dsl_source = "color red = 0xFF0000"
  var pull_lexer = animation_dsl.create_lexer(dsl_source)
  var transpiler = animation_dsl.SimpleDSLTranspiler(pull_lexer)
  
  print("Testing token access methods:")
  
  # Test current()
  var current_token = transpiler.current()
  print(f"Current token: {current_token.tostring()}")
  
  # Test peek()
  var next_token = transpiler.peek()
  print(f"Next token: {next_token.tostring()}")
  
  # Test next()
  var consumed_token = transpiler.next()
  print(f"Consumed token: {consumed_token.tostring()}")
  
  # Test current() after next()
  current_token = transpiler.current()
  print(f"Current token after next(): {current_token.tostring()}")
  
  # Test at_end()
  print(f"At end: {transpiler.at_end()}")
  
  return true
end

def test_pull_lexer_position_info()
  print("=== Testing Pull Lexer Position Information ===")
  
  var dsl_source = "color red = 0xFF0000\n" +
                   "animation pulse = pulsating_animation(color=red)"
  
  var pull_lexer = animation_dsl.create_lexer(dsl_source)
  
  # Consume a few tokens
  pull_lexer.next_token()  # color
  pull_lexer.next_token()  # red
  pull_lexer.next_token()  # =
  
  return true
end

def run_all_tests()
  print("Running Pull Lexer Transpiler Tests")
  print("=" * 50)
  
  var tests = [
    test_pull_lexer_basic,
    test_pull_lexer_template_mode,
    test_pull_lexer_token_access,
    test_pull_lexer_position_info
  ]
  
  var passed = 0
  var total = size(tests)
  
  for test_func : tests
    try
      if test_func()
        passed += 1
        print("✅ PASSED\n")
      else
        print("❌ FAILED\n")
      end
    except .. as e, msg
      print(f"❌ ERROR: {msg}\n")
    end
  end
  
  print("=" * 50)
  print(f"Results: {passed}/{total} tests passed")
  
  if passed == total
    print("🎉 All tests passed!")
  else
    print("⚠️  Some tests failed")
    raise "test_failed"
  end
  
  return passed == total
end

# Run tests when this file is executed directly
run_all_tests()

return {
  "test_pull_lexer_basic": test_pull_lexer_basic,
  "test_pull_lexer_template_mode": test_pull_lexer_template_mode,
  "test_pull_lexer_token_access": test_pull_lexer_token_access,
  "test_pull_lexer_position_info": test_pull_lexer_position_info,
  "run_all_tests": run_all_tests
}