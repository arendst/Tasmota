# DSL Template Parameter Validation Test
# Tests that template parameters are properly validated during DSL transpilation
#
# This test suite covers:
# 1. Template parameter name validation (duplicates, reserved keywords, color names)
# 2. Template parameter type annotation validation
# 3. Template parameter usage validation (unused parameters)
# 4. Template call argument validation
# 5. Templates with no parameters (should be allowed)
# 6. Templates with proper parameters and type annotations
# 7. Edge cases and error message validation

import animation
import animation_dsl
import string

# Test class to verify template parameter validation
class DSLTemplateValidationTest
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
  
  # Test valid template with proper parameters
  def test_valid_template_with_parameters()
    var dsl_code = 
      "template pulse_effect {\n" +
      "  param base_color type color\n" +
      "  param duration type time\n" +
      "  param intensity type number\n" +
      "  \n" +
      "  animation pulse = pulsating_animation(color=base_color, period=duration)\n" +
      "  pulse.opacity = intensity\n" +
      "  run pulse\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Valid template with parameters should compile successfully"
    end
    
    # Check that the generated code contains the expected template function
    if string.find(berry_code, "def pulse_effect_template(engine, base_color_, duration_, intensity_)") == -1
      raise "generation_error", "Generated code should contain template function with correct parameters"
    end
    
    # Check that template is registered as user function
    if string.find(berry_code, "animation.register_user_function('pulse_effect', pulse_effect_template)") == -1
      raise "generation_error", "Template should be registered as user function"
    end
  end
  
  # Test template with no parameters (should be allowed)
  def test_template_with_no_parameters()
    var dsl_code = 
      "template simple_effect {\n" +
      "  animation test = solid(color=red)\n" +
      "  run test\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Template with no parameters should compile successfully"
    end
    
    # Check that the generated code contains the expected template function with only engine parameter
    if string.find(berry_code, "def simple_effect_template(engine)") == -1
      raise "generation_error", "Generated code should contain template function with only engine parameter"
    end
  end
  
  # Test duplicate parameter names
  def test_duplicate_parameter_names()
    var dsl_code = 
      "template bad_template {\n" +
      "  param my_color type color\n" +
      "  param my_color type number\n" +
      "  \n" +
      "  animation test = solid(color=red)\n" +
      "  run test\n" +
      "}\n"
    
    var compilation_failed = false
    var error_message = ""
    
    try
      var berry_code = animation_dsl.compile_dsl(dsl_code)
      if berry_code == nil
        compilation_failed = true
      end
    except "dsl_compilation_error" as e, msg
      compilation_failed = true
      error_message = msg
    end
    
    if !compilation_failed
      raise "validation_error", "Duplicate parameter names should cause compilation to fail"
    end
    
    # Check that the error message mentions duplicate parameter
    if string.find(error_message, "Duplicate parameter name 'my_color'") == -1
      raise "error_message_error", f"Error message should mention duplicate parameter, got: {error_message}"
    end
  end
  
  # Test reserved keyword as parameter name
  def test_reserved_keyword_parameter_name()
    var dsl_code = 
      "template reserved_template {\n" +
      "  param animation type color\n" +
      "  \n" +
      "  animation test = solid(color=red)\n" +
      "  run test\n" +
      "}\n"
    
    var compilation_failed = false
    var error_message = ""
    
    try
      var berry_code = animation_dsl.compile_dsl(dsl_code)
      if berry_code == nil
        compilation_failed = true
      end
    except "dsl_compilation_error" as e, msg
      compilation_failed = true
      error_message = msg
    end
    
    if !compilation_failed
      raise "validation_error", "Reserved keyword as parameter name should cause compilation to fail"
    end
    
    # Check that the error message mentions reserved keyword conflict
    if string.find(error_message, "Parameter name 'animation' conflicts with reserved keyword") == -1
      raise "error_message_error", f"Error message should mention reserved keyword conflict, got: {error_message}"
    end
  end
  
  # Test built-in color name as parameter name
  def test_builtin_color_parameter_name()
    var dsl_code = 
      "template color_template {\n" +
      "  param red type number\n" +
      "  \n" +
      "  animation test = solid(color=blue)\n" +
      "  run test\n" +
      "}\n"
    
    var compilation_failed = false
    var error_message = ""
    
    try
      var berry_code = animation_dsl.compile_dsl(dsl_code)
      if berry_code == nil
        compilation_failed = true
      end
    except "dsl_compilation_error" as e, msg
      compilation_failed = true
      error_message = msg
    end
    
    if !compilation_failed
      raise "validation_error", "Built-in color name as parameter should cause compilation to fail"
    end
    
    # Check that the error message mentions color name conflict
    if string.find(error_message, "Parameter name 'red' conflicts with built-in color name") == -1
      raise "error_message_error", f"Error message should mention color name conflict, got: {error_message}"
    end
  end
  
  # Test invalid type annotation
  def test_invalid_type_annotation()
    var dsl_code = 
      "template type_template {\n" +
      "  param value type invalid_type\n" +
      "  \n" +
      "  animation test = solid(color=red)\n" +
      "  run test\n" +
      "}\n"
    
    var compilation_failed = false
    var error_message = ""
    
    try
      var berry_code = animation_dsl.compile_dsl(dsl_code)
      if berry_code == nil
        compilation_failed = true
      end
    except "dsl_compilation_error" as e, msg
      compilation_failed = true
      error_message = msg
    end
    
    if !compilation_failed
      raise "validation_error", "Invalid type annotation should cause compilation to fail"
    end
    
    # Check that the error message mentions invalid type and shows valid types
    if string.find(error_message, "Invalid parameter type 'invalid_type'") == -1
      raise "error_message_error", f"Error message should mention invalid type, got: {error_message}"
    end
    
    if string.find(error_message, "Valid types are:") == -1
      raise "error_message_error", f"Error message should show valid types, got: {error_message}"
    end
  end
  
  # Test all valid type annotations
  def test_valid_type_annotations()
    var dsl_code = 
      "template all_types_template {\n" +
      "  param my_color type color\n" +
      "  param my_number type number\n" +
      "  param my_time type time\n" +
      "  \n" +
      "  animation test = pulsating_animation(color=my_color, period=my_time)\n" +
      "  test.opacity = my_number\n" +
      "  run test\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Template with all valid type annotations should compile successfully"
    end
    
    # Check that the main parameters are included in function signature
    if string.find(berry_code, "def all_types_template_template(engine, my_color_, my_number_, my_time_)") == -1
      raise "generation_error", "Generated function should include the used parameters"
    end
  end
  
  # Test unused parameter warning
  def test_unused_parameter_warning()
    var dsl_code = 
      "template unused_template {\n" +
      "  param used_color type color\n" +
      "  param unused_param type number\n" +
      "  \n" +
      "  animation test = solid(color=used_color)\n" +
      "  run test\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Template with unused parameter should compile successfully (warnings don't prevent compilation)"
    end
    
    # Check that the generated code contains the warning as a comment
    if string.find(berry_code, "# Line") == -1 || string.find(berry_code, "unused_param") == -1
      raise "warning_error", f"Generated code should contain warning about unused parameter as comment, got: {berry_code}"
    end
    
    # Check that the template function is still generated correctly
    if string.find(berry_code, "def unused_template_template(engine, used_color_, unused_param_)") == -1
      raise "generation_error", "Template function should be generated with all parameters even if some are unused"
    end
  end
  
  # Test template with mixed typed and untyped parameters
  def test_mixed_typed_untyped_parameters()
    var dsl_code = 
      "template mixed_template {\n" +
      "  param typed_color type color\n" +
      "  param untyped_param\n" +
      "  param typed_number type number\n" +
      "  \n" +
      "  animation test = solid(color=typed_color)\n" +
      "  test.opacity = typed_number\n" +
      "  test.priority = untyped_param\n" +
      "  run test\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Template with mixed typed/untyped parameters should compile successfully"
    end
    
    # Check that function signature includes all parameters
    if string.find(berry_code, "def mixed_template_template(engine, typed_color_, untyped_param_, typed_number_)") == -1
      raise "generation_error", "Generated function should include all parameters in correct order"
    end
  end
  
  # Test template parameter validation with edge case names
  def test_edge_case_parameter_names()
    var dsl_code = 
      "template edge_template {\n" +
      "  param _valid_name type color\n" +
      "  param valid123 type number\n" +
      "  \n" +
      "  animation test = solid(color=_valid_name)\n" +
      "  test.opacity = valid123\n" +
      "  run test\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Template with edge case parameter names should compile successfully"
    end
    
    # Check that function signature includes the used parameters
    if string.find(berry_code, "def edge_template_template(engine, _valid_name_, valid123_)") == -1
      raise "generation_error", "Generated function should handle edge case parameter names correctly"
    end
  end
  
  # Test template with complex body using parameters
  def test_complex_template_body()
    var dsl_code = 
      "template complex_template {\n" +
      "  param base_color type color\n" +
      "  param speed type time\n" +
      "  param intensity type number\n" +
      "  \n" +
      "  color dynamic_color = color_cycle(palette=[base_color, white], cycle_period=speed)\n" +
      "  animation main = pulsating_animation(color=dynamic_color, period=speed)\n" +
      "  main.opacity = intensity\n" +
      "  main.priority = 10\n" +
      "  \n" +
      "  animation background = solid(color=black)\n" +
      "  background.priority = 1\n" +
      "  \n" +
      "  run background\n" +
      "  run main\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Template with complex body should compile successfully"
    end
    
    # Check that all parameters are used in the generated code
    if string.find(berry_code, "base_color_") == -1 || 
       string.find(berry_code, "speed_") == -1 || 
       string.find(berry_code, "intensity_") == -1
      raise "generation_error", "All parameters should be used in generated template body"
    end
    
    # Check that template creates multiple animations
    if string.find(berry_code, "engine.add(background_)") == -1 || 
       string.find(berry_code, "engine.add(main_)") == -1
      raise "generation_error", "Template should add all animations to engine"
    end
  end
  
  # Run all tests
  def run_all_tests()
    print("Running DSL Template Parameter Validation Tests...")
    
    var total_tests = 0
    var passed_tests = 0
    
    # Test cases
    var tests = [
      ["Valid Template with Parameters", / -> self.test_valid_template_with_parameters()],
      ["Template with No Parameters", / -> self.test_template_with_no_parameters()],
      ["Duplicate Parameter Names", / -> self.test_duplicate_parameter_names()],
      ["Reserved Keyword Parameter Name", / -> self.test_reserved_keyword_parameter_name()],
      ["Built-in Color Parameter Name", / -> self.test_builtin_color_parameter_name()],
      ["Invalid Type Annotation", / -> self.test_invalid_type_annotation()],
      ["Valid Type Annotations", / -> self.test_valid_type_annotations()],
      ["Unused Parameter Warning", / -> self.test_unused_parameter_warning()],
      ["Mixed Typed/Untyped Parameters", / -> self.test_mixed_typed_untyped_parameters()],
      ["Edge Case Parameter Names", / -> self.test_edge_case_parameter_names()],
      ["Complex Template Body", / -> self.test_complex_template_body()]
    ]
    
    for test : tests
      total_tests += 1
      if self.run_test(test[0], test[1])
        passed_tests += 1
      end
    end
    
    # Print results
    print(f"\nTest Results:")
    for result : self.test_results
      print(f"  {result}")
    end
    
    print(f"\nSummary: {passed_tests}/{total_tests} tests passed")
    
    if passed_tests == total_tests
      print("✓ All DSL template parameter validation tests passed!")
      return true
    else
      print("✗ Some DSL template parameter validation tests failed!")
      raise "test_failed"
    end
  end
end

# Run tests
var test_runner = DSLTemplateValidationTest()
test_runner.run_all_tests()

# Export for use in other test files
return {
  "DSLTemplateValidationTest": DSLTemplateValidationTest
}