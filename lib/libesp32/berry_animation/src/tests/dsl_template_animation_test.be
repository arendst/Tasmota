# DSL Template Animation Test
# Tests the new template animation feature that generates classes extending engine_proxy
#
# This test suite covers:
# 1. Basic template animation compilation
# 2. Parameter constraints (min, max, default)
# 3. Parameter reference as self.param
# 4. Closure wrapping for parameters
# 5. PARAMS generation with encode_constraints
# 6. Multiple instances of template animations
# 7. Template animation with sequences
# 8. Parameter usage in different contexts

import animation
import animation_dsl
import string

# Test class to verify template animation functionality
class DSLTemplateAnimationTest
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
  
  # Test basic template animation compilation
  def test_basic_template_animation()
    var dsl_code = 
      "template animation simple_effect {\n" +
      "  param my_color type color\n" +
      "  \n" +
      "  animation test = solid(color=my_color)\n" +
      "  run test\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Basic template animation should compile successfully"
    end
    
    # Check that it generates a class, not a function
    if string.find(berry_code, "class simple_effect_animation : animation.engine_proxy") == -1
      raise "generation_error", "Should generate class extending engine_proxy"
    end
    
    # Check for PARAMS static variable
    if string.find(berry_code, "static var PARAMS = animation.enc_params({") == -1
      raise "generation_error", "Should generate PARAMS with encode_constraints"
    end
    
    # Check for setup_template method (replaces init method)
    if string.find(berry_code, "def setup_template()") == -1
      raise "generation_error", "Should generate setup_template method"
    end
    
    # Check for engine variable assignment
    if string.find(berry_code, "var engine = self ") == -1
      raise "generation_error", "Should assign engine from self.engine"
    end
    
    # Check for self.add instead of engine.add
    if string.find(berry_code, "self.add(test_)") == -1
      raise "generation_error", "Should use self.add instead of engine.add"
    end
  end
  
  # Test parameter constraints (min, max, default, nillable)
  def test_parameter_constraints()
    var dsl_code = 
      "template animation constrained_effect {\n" +
      "  param duration type time min 0 max 3600 default 5\n" +
      "  param intensity type number min 0 max 255 default 128 nillable true\n" +
      "  param colors type palette nillable false\n" +
      "  \n" +
      "  animation test = solid(color=red)\n" +
      "  run test\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Template animation with constraints should compile successfully"
    end
    
    # Check that constraints are in PARAMS
    if string.find(berry_code, '"duration": {"type": "time", "min": 0, "max": 3600, "default": 5}') == -1
      raise "generation_error", "Duration constraints should be in PARAMS"
    end
    
    if string.find(berry_code, '"intensity": {"type": "number", "min": 0, "max": 255, "default": 128, "nillable": true}') == -1
      raise "generation_error", "Intensity constraints with nillable should be in PARAMS"
    end
    
    if string.find(berry_code, '"colors": {"type": "palette", "nillable": false}') == -1
      raise "generation_error", "Colors with nillable false should be in PARAMS"
    end
  end
  
  # Test parameter reference as self.param
  def test_self_param_reference()
    var dsl_code = 
      "template animation param_ref_effect {\n" +
      "  param my_color type palette\n" +
      "  param my_duration type time\n" +
      "  \n" +
      "  color col = color_cycle(palette=my_color, cycle_period=0)\n" +
      "  animation test = pulsating_animation(color=col, period=my_duration)\n" +
      "  run test\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Template animation with parameter references should compile successfully"
    end
    
    # Check that palette parameter is wrapped with self.my_color
    if string.find(berry_code, "self.my_color") == -1
      raise "generation_error", "Palette parameter should be referenced as self.my_color"
    end
    
    # Check that it's wrapped in create_closure_value
    if string.find(berry_code, "animation.create_closure_value(engine, def (engine) return self.my_color end)") == -1
      raise "generation_error", "Palette parameter should be wrapped in create_closure_value"
    end
  end
  
  # Test parameter in computed expressions
  def test_param_in_computed_expression()
    var dsl_code = 
      "template animation computed_effect {\n" +
      "  param base_size type number\n" +
      "  \n" +
      "  set strip_len = strip_length()\n" +
      "  set computed_size = strip_len / 2\n" +
      "  \n" +
      "  animation test = beacon_animation(beacon_size=base_size)\n" +
      "  run test\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Template animation with computed expressions should compile successfully"
    end
    
    # Check that parameter is used correctly
    if string.find(berry_code, "self.base_size") == -1
      raise "generation_error", "Parameter should be referenced as self.base_size"
    end
  end
  
  # Test template animation with sequence
  def test_template_animation_with_sequence()
    var dsl_code = 
      "template animation sequence_effect {\n" +
      "  param colors type palette\n" +
      "  param duration type time\n" +
      "  \n" +
      "  color col = color_cycle(palette=colors, cycle_period=0)\n" +
      "  animation anim = solid(color=col)\n" +
      "  \n" +
      "  sequence seq repeat forever {\n" +
      "    play anim for duration\n" +
      "    col.next = 1\n" +
      "  }\n" +
      "  \n" +
      "  run seq\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Template animation with sequence should compile successfully"
    end
    
    # Check that duration parameter is used in play statement (wrapped in closure)
    if string.find(berry_code, "def (engine) return self.duration end") == -1
      raise "generation_error", "Duration parameter should be wrapped in closure for play statement"
    end
    
    # Check that sequence is added with self.add
    if string.find(berry_code, "self.add(seq_)") == -1
      raise "generation_error", "Sequence should be added with self.add"
    end
  end
  
  # Test template animation with value provider parameter
  def test_template_animation_with_value_provider()
    var dsl_code = 
      "template animation provider_effect {\n" +
      "  param duration type time\n" +
      "  \n" +
      "  set strip_len = strip_length()\n" +
      "  set oscillator = sawtooth(min_value=0, max_value=strip_len, duration=duration)\n" +
      "  \n" +
      "  animation test = beacon_animation(pos=oscillator)\n" +
      "  run test\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Template animation with value provider should compile successfully"
    end
    
    # Check that duration is used in the value provider
    if string.find(berry_code, "provider.duration = animation.create_closure_value(engine, def (engine) return self.duration end)") == -1
      raise "generation_error", "Duration parameter should be wrapped in closure for value provider"
    end
  end
  
  # Test template animation with multiple parameters
  def test_multiple_parameters()
    var dsl_code = 
      "template animation multi_param_effect {\n" +
      "  param colors type palette\n" +
      "  param duration type time min 1 max 10 default 5\n" +
      "  param intensity type number min 0 max 255 default 200\n" +
      "  \n" +
      "  color col = color_cycle(palette=colors, cycle_period=0)\n" +
      "  animation test = solid(color=col)\n" +
      "  test.opacity = intensity\n" +
      "  run test\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Template animation with multiple parameters should compile successfully"
    end
    
    # Check all parameters are in PARAMS
    if string.find(berry_code, '"colors": {"type": "palette"}') == -1
      raise "generation_error", "Colors parameter should be in PARAMS"
    end
    
    if string.find(berry_code, '"duration": {"type": "time", "min": 1, "max": 10, "default": 5}') == -1
      raise "generation_error", "Duration parameter with constraints should be in PARAMS"
    end
    
    if string.find(berry_code, '"intensity": {"type": "number", "min": 0, "max": 255, "default": 200}') == -1
      raise "generation_error", "Intensity parameter with constraints should be in PARAMS"
    end
    
    # Check that intensity parameter is used correctly
    if string.find(berry_code, "self.intensity") == -1
      raise "generation_error", "Intensity parameter should be referenced as self.intensity"
    end
  end
  
  # Test template animation with no parameters
  def test_no_parameters()
    var dsl_code = 
      "template animation no_param_effect {\n" +
      "  animation test = solid(color=red)\n" +
      "  run test\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Template animation with no parameters should compile successfully"
    end
    
    # Check that PARAMS is present with encode_constraints (may have whitespace)
    if string.find(berry_code, "static var PARAMS = animation.enc_params({") == -1
      raise "generation_error", "PARAMS should be present with encode_constraints"
    end
    
    # Check that PARAMS has closing brace (empty constraints)
    if string.find(berry_code, "animation.enc_params({") == -1
      raise "generation_error", "PARAMS should use encode_constraints with empty map"
    end
  end
  
  # Test parameter usage validation (should not warn for used parameters)
  def test_parameter_usage_validation()
    var dsl_code = 
      "template animation used_params_effect {\n" +
      "  param colors type palette\n" +
      "  param duration type time\n" +
      "  \n" +
      "  color col = color_cycle(palette=colors, cycle_period=0)\n" +
      "  animation test = solid(color=col)\n" +
      "  \n" +
      "  sequence seq repeat forever {\n" +
      "    play test for duration\n" +
      "  }\n" +
      "  \n" +
      "  run seq\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Template animation with used parameters should compile successfully"
    end
    
    # Check that there are no warnings about unused parameters
    # (warnings would appear as comments in the generated code)
    var has_unused_warning = string.find(berry_code, "parameter 'colors' is declared but never used") != -1 ||
                             string.find(berry_code, "parameter 'duration' is declared but never used") != -1
    
    if has_unused_warning
      raise "validation_error", "Should not warn about parameters that are actually used"
    end
  end
  
  # Test color defaults (named colors and hex colors)
  def test_color_defaults()
    var dsl_code = 
      "template animation color_defaults_effect {\n" +
      "  param color1 type color default red\n" +
      "  param color2 type color default 0xFF00FF00\n" +
      "  param color3 type color default transparent\n" +
      "  param color4 type color default blue\n" +
      "  \n" +
      "  animation test = solid(color=color1)\n" +
      "  run test\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Template animation with color defaults should compile successfully"
    end
    
    # Check that named colors are converted to ARGB hex values
    if string.find(berry_code, '"color1": {"type": "color", "default": 0xFFFF0000}') == -1
      raise "generation_error", "Named color 'red' should be converted to 0xFFFF0000"
    end
    
    # Check that hex colors are preserved
    if string.find(berry_code, '"color2": {"type": "color", "default": 0xFF00FF00}') == -1
      raise "generation_error", "Hex color should be preserved as 0xFF00FF00"
    end
    
    # Check that transparent is converted correctly
    if string.find(berry_code, '"color3": {"type": "color", "default": 0x00000000}') == -1
      raise "generation_error", "Named color 'transparent' should be converted to 0x00000000"
    end
    
    # Check that blue is converted correctly
    if string.find(berry_code, '"color4": {"type": "color", "default": 0xFF0000FF}') == -1
      raise "generation_error", "Named color 'blue' should be converted to 0xFF0000FF"
    end
  end
  
  # Test time defaults (seconds, minutes, hours)
  def test_time_defaults()
    var dsl_code = 
      "template animation time_defaults_effect {\n" +
      "  param duration1 type time default 5s\n" +
      "  param duration2 type time default 2m\n" +
      "  param duration3 type time default 1h\n" +
      "  param duration4 type time default 500ms\n" +
      "  \n" +
      "  animation test = solid(color=red)\n" +
      "  run test\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Template animation with time defaults should compile successfully"
    end
    
    # Check that time values are converted to milliseconds
    if string.find(berry_code, '"duration1": {"type": "time", "default": 5000}') == -1
      raise "generation_error", "Time '5s' should be converted to 5000 milliseconds"
    end
    
    if string.find(berry_code, '"duration2": {"type": "time", "default": 120000}') == -1
      raise "generation_error", "Time '2m' should be converted to 120000 milliseconds"
    end
    
    if string.find(berry_code, '"duration3": {"type": "time", "default": 3600000}') == -1
      raise "generation_error", "Time '1h' should be converted to 3600000 milliseconds"
    end
    
    if string.find(berry_code, '"duration4": {"type": "time", "default": 500}') == -1
      raise "generation_error", "Time '500ms' should be converted to 500 milliseconds"
    end
  end
  
  # Test time constraints for min/max
  def test_time_constraints()
    var dsl_code = 
      "template animation time_constraints_effect {\n" +
      "  param duration type time min 1s max 1h default 10s\n" +
      "  param speed type time min 100ms max 5s default 1s\n" +
      "  \n" +
      "  animation test = solid(color=red)\n" +
      "  run test\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Template animation with time constraints should compile successfully"
    end
    
    # Check that time constraints are converted to milliseconds
    if string.find(berry_code, '"duration": {"type": "time", "min": 1000, "max": 3600000, "default": 10000}') == -1
      raise "generation_error", "Time constraints should be converted to milliseconds (1s=1000, 1h=3600000, 10s=10000)"
    end
    
    if string.find(berry_code, '"speed": {"type": "time", "min": 100, "max": 5000, "default": 1000}') == -1
      raise "generation_error", "Time constraints should be converted to milliseconds (100ms=100, 5s=5000, 1s=1000)"
    end
  end
  
  # Test mixed constraint types (colors, times, numbers)
  def test_mixed_constraint_types()
    var dsl_code = 
      "template animation mixed_constraints_effect {\n" +
      "  param eye_color type color default red\n" +
      "  param back_color type color default transparent\n" +
      "  param duration type time min 0 max 1h default 10s\n" +
      "  param brightness type number min 0 max 255 default 200\n" +
      "  \n" +
      "  animation test = solid(color=eye_color)\n" +
      "  test.opacity = brightness\n" +
      "  run test\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Template animation with mixed constraint types should compile successfully"
    end
    
    # Check color defaults
    if string.find(berry_code, '"eye_color": {"type": "color", "default": 0xFFFF0000}') == -1
      raise "generation_error", "Color default 'red' should be converted to 0xFFFF0000"
    end
    
    if string.find(berry_code, '"back_color": {"type": "color", "default": 0x00000000}') == -1
      raise "generation_error", "Color default 'transparent' should be converted to 0x00000000"
    end
    
    # Check time constraints
    if string.find(berry_code, '"duration": {"type": "time", "min": 0, "max": 3600000, "default": 10000}') == -1
      raise "generation_error", "Time constraints should be converted to milliseconds"
    end
    
    # Check number constraints
    if string.find(berry_code, '"brightness": {"type": "number", "min": 0, "max": 255, "default": 200}') == -1
      raise "generation_error", "Number constraints should be preserved"
    end
  end
  
  # Test inherited parameters from engine_proxy class hierarchy
  def test_inherited_parameters()
    var dsl_code = 
      "template animation inherited_params_effect {\n" +
      "  param colors type palette\n" +
      "  param period type time default 5s\n" +
      "  \n" +
      "  set strip_len = strip_length()\n" +
      "  set shutter_size = sawtooth(min_value=0, max_value=strip_len, duration=duration)\n" +
      "  \n" +
      "  color col = color_cycle(palette=colors, cycle_period=0)\n" +
      "  animation test = beacon_animation(color=col, beacon_size=shutter_size)\n" +
      "  \n" +
      "  sequence seq repeat forever {\n" +
      "    play test for period\n" +
      "  }\n" +
      "  \n" +
      "  run seq\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Template animation with inherited parameters should compile successfully"
    end
    
    # Check that 'duration' parameter (inherited from engine_proxy) is recognized
    # It should be wrapped in a closure when used in the value provider
    if string.find(berry_code, "self.duration") == -1
      raise "generation_error", "Inherited parameter 'duration' should be referenced as self.duration"
    end
    
    # Check that duration is wrapped in create_closure_value
    if string.find(berry_code, "animation.create_closure_value(engine, def (engine) return self.duration end)") == -1
      raise "generation_error", "Inherited parameter 'duration' should be wrapped in closure"
    end
    
    # Verify that other inherited parameters would also work (name, priority, opacity, color, loop, is_running)
    # These are all valid parameters from the engine_proxy class hierarchy
  end
  
  # Test all inherited parameters from engine_proxy
  def test_all_inherited_parameters()
    var dsl_code = 
      "template animation all_inherited_effect {\n" +
      "  param my_color type color\n" +
      "  \n" +
      "  animation test = solid(color=my_color)\n" +
      "  test.opacity = opacity\n" +
      "  test.priority = priority\n" +
      "  test.duration = duration\n" +
      "  test.loop = loop\n" +
      "  \n" +
      "  run test\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Template animation using all inherited parameters should compile successfully"
    end
    
    # Check that all inherited parameters are recognized and referenced as self.param
    var inherited_params = ["opacity", "priority", "duration", "loop"]
    for param : inherited_params
      if string.find(berry_code, f"self.{param}") == -1
        raise "generation_error", f"Inherited parameter '{param}' should be referenced as self.{param}"
      end
    end
  end
  
  # Test complex template animation with all features
  def test_complex_template_animation()
    var dsl_code = 
      "template animation complex_shutter {\n" +
      "  param colors type palette\n" +
      "  param duration type time min 0 max 3600 default 5\n" +
      "  \n" +
      "  set strip_len = strip_length()\n" +
      "  set strip_len2 = (strip_len + 1) / 2\n" +
      "  set shutter_size = sawtooth(min_value=0, max_value=strip_len, duration=duration)\n" +
      "  \n" +
      "  color col1 = color_cycle(palette=colors, cycle_period=0)\n" +
      "  color col2 = color_cycle(palette=colors, cycle_period=0)\n" +
      "  col2.next = 1\n" +
      "  \n" +
      "  animation shutter = beacon_animation(\n" +
      "    color=col1\n" +
      "    back_color=col2\n" +
      "    pos=strip_len2\n" +
      "    beacon_size=shutter_size\n" +
      "  )\n" +
      "  \n" +
      "  sequence seq repeat forever {\n" +
      "    restart shutter_size\n" +
      "    play shutter for duration\n" +
      "    col1.next = 1\n" +
      "  }\n" +
      "  \n" +
      "  run seq\n" +
      "}\n"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Complex template animation should compile successfully"
    end
    
    # Check class structure
    if string.find(berry_code, "class complex_shutter_animation : animation.engine_proxy") == -1
      raise "generation_error", "Should generate class with correct name"
    end
    
    # Check PARAMS with constraints
    if string.find(berry_code, '"duration": {"type": "time", "min": 0, "max": 3600, "default": 5}') == -1
      raise "generation_error", "Duration constraints should be in PARAMS"
    end
    
    # Check parameter references
    if string.find(berry_code, "self.colors") == -1
      raise "generation_error", "Colors parameter should be referenced as self.colors"
    end
    
    if string.find(berry_code, "self.duration") == -1
      raise "generation_error", "Duration parameter should be referenced as self.duration"
    end
    
    # Check closure wrapping
    if string.find(berry_code, "animation.create_closure_value(engine, def (engine) return self.colors end)") == -1
      raise "generation_error", "Colors parameter should be wrapped in closure"
    end
    
    # Check closure wrapping in sequence for duration parameter
    if string.find(berry_code, "def (engine) return self.duration end") == -1
      raise "generation_error", "Duration should be wrapped in closure for play statement"
    end
    
    # Check self.add
    if string.find(berry_code, "self.add(seq_)") == -1
      raise "generation_error", "Should use self.add for sequence"
    end
  end
  
  # Run all tests
  def run_all_tests()
    print("Running DSL Template Animation Tests...")
    
    var total_tests = 0
    var passed_tests = 0
    
    # Test cases
    var tests = [
      ["Basic Template Animation", / -> self.test_basic_template_animation()],
      ["Parameter Constraints", / -> self.test_parameter_constraints()],
      ["Self Parameter Reference", / -> self.test_self_param_reference()],
      ["Parameter in Computed Expression", / -> self.test_param_in_computed_expression()],
      ["Template Animation with Sequence", / -> self.test_template_animation_with_sequence()],
      ["Template Animation with Value Provider", / -> self.test_template_animation_with_value_provider()],
      ["Multiple Parameters", / -> self.test_multiple_parameters()],
      ["No Parameters", / -> self.test_no_parameters()],
      ["Parameter Usage Validation", / -> self.test_parameter_usage_validation()],
      ["Color Defaults", / -> self.test_color_defaults()],
      ["Time Defaults", / -> self.test_time_defaults()],
      ["Time Constraints", / -> self.test_time_constraints()],
      ["Mixed Constraint Types", / -> self.test_mixed_constraint_types()],
      ["Inherited Parameters", / -> self.test_inherited_parameters()],
      ["All Inherited Parameters", / -> self.test_all_inherited_parameters()],
      ["Complex Template Animation", / -> self.test_complex_template_animation()]
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
      print("✓ All DSL template animation tests passed!")
      return true
    else
      print("✗ Some DSL template animation tests failed!")
      raise "test_failed"
    end
  end
end

# Run tests
var test_runner = DSLTemplateAnimationTest()
test_runner.run_all_tests()

# Export for use in other test files
return {
  "DSLTemplateAnimationTest": DSLTemplateAnimationTest
}
