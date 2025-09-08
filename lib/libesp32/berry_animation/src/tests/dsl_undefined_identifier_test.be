# DSL Undefined Identifier Test
# Tests that undefined identifiers in function calls are properly caught and reported
#
# This test covers the fix for process_primary_expression() where undefined identifiers
# in function call contexts should raise appropriate error messages.

import animation
import animation_dsl
import string

# Test class to verify undefined identifier error handling
class DSLUndefinedIdentifierTest
  var test_results
  
  def init()
    self.test_results = []
  end
  
  # Helper method to run a test case
  def run_test(test_name, test_func)
    try
      test_func()
      self.test_results.push(f"✓ {test_name}")
      return true
    except .. as e, msg
      self.test_results.push(f"✗ {test_name}: {msg}")
      return false
    end
  end
  
  # Test undefined function in animation definition
  # Note: This will be caught by animation factory validation, not our new check
  def test_undefined_function_in_animation()
    var dsl_code = 
      "animation test = undefined_function(color=red)\n"
      "run test"
    
    try
      var berry_code = animation_dsl.compile_dsl(dsl_code)
      raise "compilation_error", "Should have failed with undefined function error"
    except "dsl_compilation_error" as e, msg
      # This will be caught by animation factory validation
      if string.find(msg, "Animation factory function 'undefined_function' does not exist") == -1
        raise "wrong_error", f"Expected animation factory error, got: {msg}"
      end
    end
  end
  
  # Test undefined function in color definition
  # Note: This will be caught by color provider validation, not our new check
  def test_undefined_function_in_color()
    var dsl_code = 
      "color test_color = undefined_color_provider(period=2s)\n"
      "animation test = solid(color=test_color)\n"
      "run test"
    
    try
      var berry_code = animation_dsl.compile_dsl(dsl_code)
      raise "compilation_error", "Should have failed with undefined function error"
    except "dsl_compilation_error" as e, msg
      # This will be caught by color provider validation
      if string.find(msg, "Color provider factory") == -1 && string.find(msg, "does not exist") == -1
        raise "wrong_error", f"Expected color provider factory error, got: {msg}"
      end
    end
  end
  
  # Test undefined function in property assignment
  def test_undefined_function_in_property()
    var dsl_code = 
      "animation test = solid(color=red)\n"
      "test.opacity = undefined_value_provider(min_value=0, max_value=255)\n"
      "run test"
    
    try
      var berry_code = animation_dsl.compile_dsl(dsl_code)
      raise "compilation_error", "Should have failed with undefined function error"
    except "dsl_compilation_error" as e, msg
      if string.find(msg, "Unknown function or identifier 'undefined_value_provider'") == -1
        raise "wrong_error", f"Expected undefined function error, got: {msg}"
      end
    end
  end
  
  # Test undefined function in computed expression
  def test_undefined_function_in_computed_expression()
    var dsl_code = 
      "animation test = solid(color=red)\n"
      "test.opacity = undefined_function() + 100\n"
      "run test"
    
    try
      var berry_code = animation_dsl.compile_dsl(dsl_code)
      raise "compilation_error", "Should have failed with undefined function error"
    except "dsl_compilation_error" as e, msg
      if string.find(msg, "Unknown function or identifier 'undefined_function'") == -1
        raise "wrong_error", f"Expected undefined function error, got: {msg}"
      end
    end
  end
  
  # Test that valid functions still work (regression test)
  def test_valid_functions_still_work()
    var dsl_code = 
      "set osc = triangle(min_value=50, max_value=255, duration=2s)\n"
      "animation test = solid(color=red)\n"
      "test.opacity = osc\n"
      "run test"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Valid functions should compile successfully"
    end
    
    # Check that the generated code contains expected elements
    if string.find(berry_code, "animation.triangle") == -1
      raise "generation_error", "Generated code should contain animation.triangle"
    end
  end
  
  # Test undefined identifier in regular context (not function call)
  def test_undefined_identifier_regular_context()
    var dsl_code = 
      "animation test = solid(color=undefined_color)\n"
      "run test"
    
    try
      var berry_code = animation_dsl.compile_dsl(dsl_code)
      raise "compilation_error", "Should have failed with undefined identifier error"
    except "dsl_compilation_error" as e, msg
      if string.find(msg, "Unknown identifier 'undefined_color'") == -1
        raise "wrong_error", f"Expected undefined identifier error, got: {msg}"
      end
    end
  end
  
  # Run all tests
  def run_all_tests()
    print("=== DSL Undefined Identifier Test Suite ===")
    
    var tests = [
      ["Undefined function in animation definition", / -> self.test_undefined_function_in_animation()],
      ["Undefined function in color definition", / -> self.test_undefined_function_in_color()],
      ["Undefined function in property assignment", / -> self.test_undefined_function_in_property()],
      ["Undefined function in computed expression", / -> self.test_undefined_function_in_computed_expression()],
      ["Valid functions still work (regression)", / -> self.test_valid_functions_still_work()],
      ["Undefined identifier in regular context", / -> self.test_undefined_identifier_regular_context()]
    ]
    
    var passed = 0
    var total = size(tests)
    
    for test_info : tests
      var test_name = test_info[0]
      var test_func = test_info[1]
      
      try
        if self.run_test(test_name, test_func)
          passed += 1
        end
      except .. as error_type, error_message
        print(f"✗ Test crashed: {error_type} - {error_message}")
      end
    end
    
    print(f"\n=== Test Results: {passed}/{total} passed ===")
    
    for result : self.test_results
      print(result)
    end
    
    return passed == total
  end
end

# Run the tests
var test_suite = DSLUndefinedIdentifierTest()
var success = test_suite.run_all_tests()

if success
  print("\n🎉 All undefined identifier tests passed!")
else
  print("\n❌ Some undefined identifier tests failed!")
  raise "test_failed"
end

return success