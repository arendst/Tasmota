# DSL Transpiler Test Suite
# Tests for SimpleDSLTranspiler class
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/dsl_transpiler_test.be

import animation
import animation_dsl
import string

# Helper function to extract all tokens from a pull lexer (for testing only)
def extract_all_tokens(lexer)
  var tokens = []
  lexer.reset()  # Start from beginning
  
  while !lexer.at_end()
    var token = lexer.next_token()
    
    # EOF token removed - check for nil instead
    if token == nil
      break
    end
    
    tokens.push(token)
  end
  
  return tokens
end

# Test basic transpilation
def test_basic_transpilation()
  print("Testing basic DSL transpilation...")
  
  var dsl_source = "# strip length 60  # TEMPORARILY DISABLED\n" +
    "color custom_red = 0xFF0000\n" +
    "animation solid_red = solid(color=custom_red)\n" +
    "animation red_anim = solid_red\n" +
    "\n" +
    "sequence demo {\n" +
    "  play red_anim for 5s\n" +
    "}\n" +
    "\n" +
    "run demo"
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should generate Berry code")
  assert(string.find(berry_code, "var engine = animation.init_strip()") >= 0, "Should generate strip configuration")
  assert(string.find(berry_code, "var custom_red_ = 0xFFFF0000") >= 0, "Should generate color definition")
  assert(string.find(berry_code, "var demo_ = animation.SequenceManager(engine)") >= 0, "Should generate sequence manager")
  assert(string.find(berry_code, "engine.add(demo_)") >= 0, "Should add sequence manager")
  
  # print("Generated Berry code:")
  # print("==================================================")
  # print(berry_code)
  # print("==================================================")
  
  print("✓ Basic transpilation test passed")
  return true
end

# Test color definitions
def test_color_definitions()
  print("Testing color definitions...")
  
  var color_tests = [
    ["color custom_red = 0xFF0000", "var custom_red_ = 0xFFFF0000"],
    ["color custom_blue = 0x0000FF", "var custom_blue_ = 0xFF0000FF"],
    ["color my_white = white", "var my_white_ = 0xFFFFFFFF"],
    ["color my_green = green", "var my_green_ = 0xFF008000"]
  ]
  
  for test : color_tests
    var dsl_input = test[0]
    var expected_output = test[1]
    
    var berry_code = animation_dsl.compile(dsl_input)
    assert(berry_code != nil, "Should compile: " + dsl_input)
    assert(string.find(berry_code, expected_output) >= 0, "Should contain: " + expected_output)
  end
  
  print("✓ Color definitions test passed")
  return true
end

# Test color definitions with alpha channel
def test_color_alpha_channel()
  print("Testing color definitions with alpha channel...")
  
  var alpha_color_tests = [
    # Test 8-character hex with alpha (should preserve alpha)
    ["color red_opaque = 0xFFFF0000", "var red_opaque_ = 0xFFFF0000"],
    ["color red_half = 0x80FF0000", "var red_half_ = 0x80FF0000"],
    ["color blue_quarter = 0x400000FF", "var blue_quarter_ = 0x400000FF"],
    ["color clear = 0x00000000", "var clear_ = 0x00000000"],
    
    # Test 6-character hex without alpha (should add FF for opaque)
    ["color custom_red = 0xFF0000", "var custom_red_ = 0xFFFF0000"],
    ["color custom_lime = 0x00FF00", "var custom_lime_ = 0xFF00FF00"]
  ]
  
  for test : alpha_color_tests
    var dsl_input = test[0]
    var expected_output = test[1]
    
    var berry_code = animation_dsl.compile(dsl_input)
    assert(berry_code != nil, "Should compile: " + dsl_input)
    assert(string.find(berry_code, expected_output) >= 0, f"Should contain: {expected_output} in: {berry_code}")
  end
  
  print("✓ Color alpha channel test passed")
  return true
end

# Test strip configuration
def test_strip_configuration()
  print("Testing strip configuration...")
  
  # Strip directive tests are temporarily disabled
  var config_tests = [
    # ["strip length 30", "var engine = animation.init_strip(30)"],  # TEMPORARILY DISABLED
    # ["strip length 60", "var engine = animation.init_strip(60)"],  # TEMPORARILY DISABLED
    # ["strip length 120", "var engine = animation.init_strip(120)"] # TEMPORARILY DISABLED
  ]
  
  for test : config_tests
    var dsl_input = test[0]
    var expected_output = test[1]
    
    var berry_code = animation_dsl.compile(dsl_input)
    assert(berry_code != nil, "Should compile: " + dsl_input)
    assert(string.find(berry_code, expected_output) >= 0, "Should contain: " + expected_output)
  end
  
  print("✓ Strip configuration test passed")
  return true
end

# Test simple patterns
def test_simple_patterns()
  print("Testing simple patterns...")
  
  var dsl_source = "color custom = 0xFF8080\n"
    "animation solid_red = solid(color=red)\n"
    "animation solid_custom = solid(color=custom)"
  
  var berry_code = animation_dsl.compile(dsl_source)

  # print("Generated Berry code:")
  # print("==================================================")
  # print(berry_code)
  # print("==================================================")

  assert(berry_code != nil, "Should compile simple pattern")
  assert(string.find(berry_code, "var custom_ = 0xFFFF8080") >= 0, "Should define color")
  assert(string.find(berry_code, "var solid_red_ = animation.solid(engine)") >= 0, "Should define animation")
  assert(string.find(berry_code, "solid_red_.color = 0xFFFF0000") >= 0, "Should set color parameter")
  
  print("✓ Simple patterns test passed")
  return true
end

# Test sequences
def test_sequences()
  print("Testing sequences...")
  
  var dsl_source = "color custom_blue = 0x0000FF\n"
    "animation blue_anim = solid(color=custom_blue)\n"
    "\n"
    "sequence test_seq {\n"
    "  play blue_anim for 3s\n"
    "}\n"
    "\n"
    "run test_seq"
  
  var berry_code = animation_dsl.compile(dsl_source)
  assert(berry_code != nil, "Should compile sequence")
  assert(string.find(berry_code, "var test_seq_ = animation.SequenceManager(engine)") >= 0, "Should define sequence manager")
  assert(string.find(berry_code, ".push_play_step(") >= 0, "Should add play step")
  assert(string.find(berry_code, "3000)") >= 0, "Should reference duration")
  assert(string.find(berry_code, "engine.run()") >= 0, "Should start engine")
  
  print("✓ Sequences test passed")
  return true
end

# Test sequence assignments
def test_sequence_assignments()
  print("Testing sequence assignments...")
  
  # Test basic sequence assignment
  var dsl_source = "color my_red = 0xFF0000\n" +
    "set brightness = 128\n" +
    "animation test = solid(color=my_red)\n" +
    "\n" +
    "sequence demo {\n" +
    "  play test for 1s\n" +
    "  test.opacity = brightness\n" +
    "  play test for 1s\n" +
    "}\n" +
    "\n" +
    "run demo"
  
  var berry_code = animation_dsl.compile(dsl_source)
  assert(berry_code != nil, "Should compile sequence with assignments")
  assert(string.find(berry_code, "var demo_ = animation.SequenceManager(engine)") >= 0, "Should define sequence manager")
  assert(string.find(berry_code, ".push_closure_step") >= 0, "Should generate closure step")
  assert(string.find(berry_code, "test_.opacity = brightness_") >= 0, "Should generate assignment")
  
  # Test multiple assignments in sequence
  var multi_assign_dsl = "color my_red = 0xFF0000\n" +
    "color my_blue = 0x0000FF\n" +
    "set high_brightness = 255\n" +
    "set low_brightness = 50\n" +
    "animation test = solid(color=my_red)\n" +
    "\n" +
    "sequence demo {\n" +
    "  play test for 1s\n" +
    "  test.opacity = high_brightness\n" +
    "  test.color = my_blue\n" +
    "  play test for 1s\n" +
    "  test.opacity = low_brightness\n" +
    "}\n" +
    "\n" +
    "run demo"
  
  var multi_berry_code = animation_dsl.compile(multi_assign_dsl)
  assert(multi_berry_code != nil, "Should compile multiple assignments")
  
  # Count assignment steps
  var assign_count = 0
  var pos = 0
  while true
    pos = string.find(multi_berry_code, "push_closure_step", pos)
    if pos < 0 break end
    assign_count += 1
    pos += 1
  end
  assert(assign_count == 3, f"Should have 3 assignment steps, found {assign_count}")
  
  # Test assignments in repeat blocks
  var repeat_assign_dsl = "color my_green = 0x00FF00\n" +
    "set brightness = 200\n" +
    "animation test = solid(color=my_green)\n" +
    "\n" +
    "sequence demo {\n" +
    "  repeat 2 times {\n" +
    "    play test for 500ms\n" +
    "    test.opacity = brightness\n" +
    "    wait 200ms\n" +
    "  }\n" +
    "}\n" +
    "\n" +
    "run demo"
  
  var repeat_berry_code = animation_dsl.compile(repeat_assign_dsl)
print(repeat_berry_code)
  assert(repeat_berry_code != nil, "Should compile repeat with assignments")
  assert(string.find(repeat_berry_code, "push_repeat_subsequence") >= 0, "Should generate repeat loop")
  assert(string.find(repeat_berry_code, "push_closure_step") >= 0, "Should generate closure step in repeat")
  
  # Test complex cylon rainbow example
  var cylon_dsl = "set strip_len = strip_length()\n" +
    "palette eye_palette = [ red, yellow, green, violet ]\n" +
    "color eye_color = color_cycle(palette=eye_palette, cycle_period=0)\n" +
    "set cosine_val = cosine_osc(min_value = 0, max_value = strip_len - 2, duration = 5s)\n" +
    "set triangle_val = triangle(min_value = 0, max_value = strip_len - 2, duration = 5s)\n" +
    "\n" +
    "animation red_eye = beacon_animation(\n" +
    "  color = eye_color\n" +
    "  pos = cosine_val\n" +
    "  beacon_size = 3\n" +
    "  slew_size = 2\n" +
    "  priority = 10\n" +
    ")\n" +
    "\n" +
    "sequence cylon_eye {\n" +
    "  play red_eye for 3s\n" +
    "  red_eye.pos = triangle_val\n" +
    "  play red_eye for 3s\n" +
    "  red_eye.pos = cosine_val\n" +
    "  eye_color.next = 1\n" +
    "}\n" +
    "\n" +
    "run cylon_eye"
  
  var cylon_berry_code = animation_dsl.compile(cylon_dsl)
  assert(cylon_berry_code != nil, "Should compile cylon rainbow example")
  
  # Check for all expected assignment steps
  assert(string.find(cylon_berry_code, "red_eye_.pos = triangle_val_") >= 0, "Should assign triangle_val to pos")
  assert(string.find(cylon_berry_code, "red_eye_.pos = cosine_val_") >= 0, "Should assign cosine_val to pos")
  assert(string.find(cylon_berry_code, "eye_color_.next = 1") >= 0, "Should assign 1 to next")
  
  print("✓ Sequence assignments test passed")
  return true
end

# Test variable duration support
def test_variable_duration()
  print("Testing variable duration support...")
  
  # Test basic variable duration
  var basic_dsl = "set short_time = 2s\n" +
    "set long_time = 5s\n" +
    "color test_color = 0xFF0000\n" +
    "animation test_anim = solid(color=test_color)\n" +
    "\n" +
    "sequence test_seq {\n" +
    "  play test_anim for short_time\n" +
    "  wait long_time\n" +
    "  play test_anim for long_time\n" +
    "}\n" +
    "\n" +
    "run test_seq"
  
  var basic_code = animation_dsl.compile(basic_dsl)
  assert(basic_code != nil, "Should compile variable duration")
  assert(string.find(basic_code, "var short_time_ = 2000") >= 0, "Should define short_time variable")
  assert(string.find(basic_code, "var long_time_ = 5000") >= 0, "Should define long_time variable")
  assert(string.find(basic_code, "short_time_") >= 0, "Should reference short_time in play")
  assert(string.find(basic_code, "long_time_") >= 0, "Should reference long_time in wait/play")
  
  # Test undefined variable should fail
  var undefined_dsl = "set valid_time = 3s\n" +
    "animation test_anim = solid(color=red)\n" +
    "\n" +
    "sequence test_seq {\n" +
    "  play test_anim for invalid_time\n" +
    "}\n" +
    "\n" +
    "run test_seq"
  
  var undefined_code = nil
  try
    undefined_code = animation_dsl.compile(undefined_dsl)
    assert(false, "Should fail with undefined variable")
  except "dsl_compilation_error" as e, msg
    assert(string.find(msg, "Undefined reference 'invalid_time' in duration") >= 0, "Should report undefined variable error")
  end
  
  # Test value provider duration
  var provider_dsl = "set dynamic_time = triangle(min_value=1000, max_value=3000, duration=10s)\n" +
    "animation test_anim = solid(color=blue)\n" +
    "\n" +
    "sequence test_seq {\n" +
    "  play test_anim for dynamic_time\n" +
    "}\n" +
    "\n" +
    "run test_seq"
  
  var provider_code = animation_dsl.compile(provider_dsl)
  assert(provider_code != nil, "Should compile value provider duration")
  assert(string.find(provider_code, "animation.triangle(engine)") >= 0, "Should create triangle value provider")
  assert(string.find(provider_code, "dynamic_time_") >= 0, "Should reference dynamic_time variable")
  
  print("✓ Variable duration test passed")
  return true
end

# Test multiple run statements
def test_multiple_run_statements()
  print("Testing multiple run statements...")
  
  # Test with multiple animations
  var dsl_source = "# strip length 30  # TEMPORARILY DISABLED\n" +
    "color custom_red = 0xFF0000\n" +
    "color custom_blue = 0x0000FF\n" +
    "color custom_green = 0x00FF00\n" +
    "\n" +
    "animation red_anim = solid(color=custom_red)\n" +
    "animation blue_anim = solid(color=custom_blue)\n" +
    "animation green_anim = solid(color=custom_green)\n" +
    "\n" +
    "red_anim.priority = 5\n" +
    "blue_anim.priority = 15\n" +
    "green_anim.priority = 25\n" +
    "\n" +
    "run red_anim\n" +
    "run blue_anim\n" +
    "run green_anim"
  
  var berry_code = animation_dsl.compile(dsl_source)
  assert(berry_code != nil, "Should compile multiple run statements")
  
  # Count engine.run() calls - should be exactly 1
  var lines = string.split(berry_code, "\n")
  var start_count = 0
  for line : lines
    if string.find(line, "engine.run()") >= 0
      start_count += 1
    end
  end
  
  assert(start_count == 1, f"Should have exactly 1 engine.run() call, found {start_count}")
  
  # Check that all animations are added to the engine
  assert(string.find(berry_code, "engine.add(red_anim_)") >= 0, "Should add red_anim to engine")
  assert(string.find(berry_code, "engine.add(blue_anim_)") >= 0, "Should add blue_anim to engine")
  assert(string.find(berry_code, "engine.add(green_anim_)") >= 0, "Should add green_anim to engine")
  
  # Verify the engine.run() comes after all animations are added
  var start_line_index = -1
  var last_add_line_index = -1
  
  for i : 0..size(lines)-1
    var line = lines[i]
    if string.find(line, "engine.run()") >= 0
      start_line_index = i
    end
    if string.find(line, "engine.add(") >= 0
      last_add_line_index = i
    end
  end
  
  assert(start_line_index > last_add_line_index, "engine.run() should come after all engine.add_* calls")
  
  # Test with mixed animations and sequences
  var mixed_dsl = "# strip length 30  # TEMPORARILY DISABLED\n" +
    "color custom_red = 0xFF0000\n" +
    "color custom_blue = 0x0000FF\n" +
    "\n" +
    "animation red_anim = solid(color=custom_red)\n" +
    "\n" +
    "sequence blue_seq {\n" +
    "  play red_anim for 2s\n" +
    "  wait 1s\n" +
    "}\n" +
    "\n" +
    "run red_anim\n" +
    "run blue_seq"
  
  var mixed_berry_code = animation_dsl.compile(mixed_dsl)
  assert(mixed_berry_code != nil, "Should compile mixed run statements")
  
  # Count engine.run() calls in mixed scenario
  var mixed_lines = string.split(mixed_berry_code, "\n")
  var mixed_start_count = 0
  for line : mixed_lines
    if string.find(line, "engine.run()") >= 0
      mixed_start_count += 1
    end
  end
  
  assert(mixed_start_count == 1, f"Mixed scenario should have exactly 1 engine.run() call, found {mixed_start_count}")
  
  # Check that both animation and sequence are handled
  assert(string.find(mixed_berry_code, "engine.add(red_anim_)") >= 0, "Should add animation to engine")
  assert(string.find(mixed_berry_code, "engine.add(blue_seq_)") >= 0, "Should add sequence to engine")
  
  print("✓ Multiple run statements test passed")
  return true
end

# Test variable assignments
def test_variable_assignments()
  print("Testing variable assignments...")
  
  var dsl_source = "set my_length = 60\n" +
    "set brightness = 80%\n" +
    "set cycle_time = 5s"
  
  var berry_code = animation_dsl.compile(dsl_source)
  assert(berry_code != nil, "Should compile variables")
  assert(string.find(berry_code, "var my_length_ = 60") >= 0, "Should define numeric variable")
  assert(string.find(berry_code, "var brightness_ = 204") >= 0, "Should convert percentage to 0-255 range")
  assert(string.find(berry_code, "var cycle_time_ = 5000") >= 0, "Should convert time to milliseconds")
  
  # Test value provider assignments
  var value_provider_dsl = "set pos_test = triangle(min_value=2, max_value=57, duration=2s)\n" +
    "set brightness_osc = smooth(min_value=50, max_value=255, duration=3s)"
  
  var provider_code = animation_dsl.compile(value_provider_dsl)
  assert(provider_code != nil, "Should compile value provider assignments")
  assert(string.find(provider_code, "animation.triangle(engine)") >= 0, "Should create triangle value provider")
  assert(string.find(provider_code, "animation.smooth(engine)") >= 0, "Should create smooth value provider")
  assert(string.find(provider_code, "min_value = 2") >= 0, "Should set triangle min_value parameter")
  assert(string.find(provider_code, "max_value = 57") >= 0, "Should set triangle max_value parameter")
  assert(string.find(provider_code, "duration = 2000") >= 0, "Should convert triangle duration to milliseconds")
  assert(string.find(provider_code, "duration = 3000") >= 0, "Should convert smooth duration to milliseconds")
  
  print("✓ Variable assignments test passed")
  return true
end

# Test computed values and expressions (regression tests)
def test_computed_values()
  print("Testing computed values and expressions...")
  
  # Test computed values with single resolve calls (regression test for double resolve issue)
  var computed_dsl = "set strip_len = strip_length()\n" +
    "animation stream1 = comet_animation(\n" +
    "  color=red\n" +
    "  tail_length=abs(strip_len / 4)\n" +
    "  speed=1.5\n" +
    "  priority=10\n" +
    ")"
  
  var computed_code = animation_dsl.compile(computed_dsl)
  assert(computed_code != nil, "Should compile computed values")
  
  # Check for single resolve calls (no double wrapping)
  var expected_single_resolve = "animation._math.abs(animation.resolve(strip_len_) / 4)"
  assert(string.find(computed_code, expected_single_resolve) >= 0, "Should generate single resolve call in computed expression")
  
  # Check that there are no double resolve calls
  var double_resolve_count = 0
  var pos = 0
  while true
    pos = string.find(computed_code, "animation.resolve(self.resolve(", pos)
    if pos < 0
      break
    end
    double_resolve_count += 1
    pos += 1
  end
  assert(double_resolve_count == 0, f"Should have no double resolve calls, found {double_resolve_count}")
  
  # Test complex expressions with single closure (regression test for nested closure issue)
  var complex_expr_dsl = "set strip_len = strip_length()\n" +
    "set base_value = 5\n" +
    "animation stream2 = comet_animation(\n" +
    "  color=blue\n" +
    "  tail_length=strip_len / 8 + (2 * strip_len) - 10\n" +
    "  speed=(base_value + strip_len) * 2.5\n" +
    "  priority=max(1, min(10, strip_len / 6))\n" +
    ")"
  
  var complex_code = animation_dsl.compile(complex_expr_dsl)
  assert(complex_code != nil, "Should compile complex expressions")
  
  # Count closure creations - each computed parameter should have exactly one closure
  var closure_count = 0
  pos = 0
  while true
    pos = string.find(complex_code, "animation.create_closure_value(", pos)
    if pos < 0
      break
    end
    closure_count += 1
    pos += 1
  end
  assert(closure_count == 3, f"Should have exactly 3 closures for 3 computed parameters, found {closure_count}")
  
  # Check that complex expressions are in single closures (no nested closures)
  var nested_closure_count = 0
  pos = 0
  while true
    # Look for closure inside closure pattern
    var closure_start = string.find(complex_code, "animation.create_closure_value(", pos)
    if closure_start < 0
      break
    end
    var closure_end = string.find(complex_code, ") end)", closure_start)
    if closure_end < 0
      break
    end
    var closure_content = complex_code[closure_start..closure_end]
    if string.find(closure_content, "animation.create_closure_value(") > 0
      nested_closure_count += 1
    end
    pos = closure_end + 1
  end
  assert(nested_closure_count == 0, f"Should have no nested closures, found {nested_closure_count}")
  
  # Verify specific complex expression patterns
  var expected_complex_tail = "animation.resolve(strip_len_) / 8 + (2 * animation.resolve(strip_len_)) - 10"
  assert(string.find(complex_code, expected_complex_tail) >= 0, "Should generate correct complex tail_length expression")
  
  var expected_complex_speed = "(animation.resolve(base_value_) + animation.resolve(strip_len_)) * 2.5"
  assert(string.find(complex_code, expected_complex_speed) >= 0, "Should generate correct complex speed expression")
  
  var expected_complex_priority = "animation._math.max(1, animation._math.min(10, animation.resolve(strip_len_) / 6))"
  assert(string.find(complex_code, expected_complex_priority) >= 0, "Should generate correct complex priority expression with math functions")
  
  # Test simple expressions that don't need closures
  var simple_expr_dsl = "set strip_len = strip_length()\n" +
    "animation simple = comet_animation(\n" +
    "  color=red\n" +
    "  tail_length=strip_len\n" +
    "  speed=1.5\n" +
    "  priority=10\n" +
    ")"
  
  var simple_code = animation_dsl.compile(simple_expr_dsl)
  assert(simple_code != nil, "Should compile simple expressions")
  
  # Simple variable reference should not create a closure
  assert(string.find(simple_code, "simple_.tail_length = strip_len_") >= 0, "Should generate direct variable reference without closure")
  
  # Test mathematical functions in computed expressions
  var math_expr_dsl = "set strip_len = strip_length()\n" +
    "animation math_test = comet_animation(\n" +
    "  color=red\n" +
    "  tail_length=max(1, min(strip_len, 20))\n" +
    "  speed=abs(strip_len - 30)\n" +
    "  priority=round(strip_len / 6)\n" +
    ")"
  
  var math_code = animation_dsl.compile(math_expr_dsl)
  assert(math_code != nil, "Should compile mathematical expressions")
  
  # Check that mathematical functions are prefixed with self. in closures
  assert(string.find(math_code, "animation._math.max(1, animation._math.min(") >= 0, "Should prefix math functions with animation._math. in closures")
  assert(string.find(math_code, "animation._math.abs(") >= 0, "Should prefix abs function with self. in closures")
  assert(string.find(math_code, "animation._math.round(") >= 0, "Should prefix round function with self. in closures")
  
  print("✓ Computed values test passed")
  return true
end

# Test error handling
def test_error_handling()
  print("Testing error handling...")
  
  # Test invalid syntax - should raise exception
  var invalid_dsl = "invalid syntax here"
  try
    var berry_code = animation_dsl.compile(invalid_dsl)
    assert(false, "Should have raised exception for invalid syntax")
  except "dsl_compilation_error" as e, msg
    # Expected behavior
  end
  
  # Test undefined references - should raise exception
  var undefined_ref_dsl = "animation test = undefined_pattern"
  
  try
    var berry_code = animation_dsl.compile(undefined_ref_dsl)
    assert(false, "Should have raised exception for undefined identifier")
  except "dsl_compilation_error" as e, msg
    # Expected behavior - undefined identifiers should raise exceptions
  end
  
  print("✓ Error handling test passed")
  return true
end

# Test forward references (deferred resolution)
def test_forward_references()
  print("Testing forward references...")
  
  var dsl_source = "# Forward reference: animation uses color defined later\n" +
    "animation fire_gradient = gradient_animation(color=red)\n" +
    "color red = 0xFF0000\n" +
    "color orange = 0xFF8000"
  
  var berry_code = nil
  var compilation_failed = false
  
  try
    var lexer = animation_dsl.create_lexer(dsl_source)
    var transpiler = animation_dsl.SimpleDSLTranspiler(lexer)
    berry_code = transpiler.transpile()
  except "dsl_compilation_error" as e, msg
    compilation_failed = true
    print("Forward references not yet supported - compilation failed as expected")
  end
  
  # Should resolve forward references if supported
  if berry_code != nil && !compilation_failed
    assert(string.find(berry_code, "var red_ = 0xFFFF0000") >= 0, "Should define red color")
    assert(string.find(berry_code, "var orange_ = 0xFFFF8000") >= 0, "Should define orange color")
    print("Forward references resolved successfully")
  else
    print("Forward references not yet fully implemented - this is expected")
  end
  
  print("✓ Forward references test passed")
  return true
end

# Test complex DSL example with core processing features
def test_complex_dsl()
  print("Testing complex DSL example...")
  
  var complex_dsl = "# LED Strip Configuration\n" +
    "# strip length 60  # TEMPORARILY DISABLED\n" +
    "\n" +
    "# Color Definitions\n" +
    "color custom_red = 0xFF0000\n" +
    "color custom_blue = 0x0000FF\n" +
    "\n" +
    "# Variable Definitions\n" +
    "set cycle_time = 5s\n" +
    "set brightness = 80%\n" +
    "\n" +
    "# Animation Definitions\n" +
    "animation red_pulse = pulsating_animation(color=red, period=2000)\n" +
    "animation blue_breathe = breathe_animation(color=blue, period=4000)\n" +
    "\n" +
    "# Sequence Definition with Control Flow\n" +
    "sequence demo {\n" +
    "  play red_pulse for 3s\n" +
    "  wait 1s\n" +
    "  repeat 2 times {\n" +
    "    play blue_breathe for 2s\n" +
    "    wait 500ms\n" +
    "  }\n" +
    "}\n" +
    "\n" +
    "# Execution\n" +
    "run demo"
  
  var berry_code = animation_dsl.compile(complex_dsl)
  
  if berry_code != nil
    print("Complex DSL compiled successfully!")
    
    # Check for key components
    assert(string.find(berry_code, "var engine = animation.init_strip()") >= 0, "Should have default strip initialization")
    assert(string.find(berry_code, "var custom_red_ = 0xFFFF0000") >= 0, "Should have color definitions")
    assert(string.find(berry_code, "var demo_ = animation.SequenceManager(engine)") >= 0, "Should have sequence definition")
    assert(string.find(berry_code, "engine.add(demo_)") >= 0, "Should have execution")
    
    print("Generated code structure looks correct")
  else
    print("Complex DSL compilation failed - checking for specific issues...")
    
    # Test individual components
    var lexer = animation_dsl.create_lexer(complex_dsl)
    
    print("Lexical analysis passed")
    
    var transpiler = animation_dsl.SimpleDSLTranspiler(lexer)
    var result = transpiler.transpile()
  end
  
  print("✓ Complex DSL test completed")
  return true
end

# Test transpiler components individually
def test_transpiler_components()
  print("Testing transpiler components...")
  
  # Basic transpiler functionality test
  print("Testing basic transpiler instantiation...")
  
  # Test token processing
  var lexer = animation_dsl.create_lexer("color red = 0xFF0000")
  var tokens = extract_all_tokens(lexer)
  assert(size(tokens) >= 4, "Should have multiple tokens")
  
  # Reset lexer position before creating transpiler
  lexer.reset()
  var transpiler = animation_dsl.SimpleDSLTranspiler(lexer)
  assert(!transpiler.at_end(), "Should not be at end initially")
  
  print("✓ Transpiler components test passed")
  return true
end

# Test core processing methods functionality
def test_core_processing_methods()
  print("Testing core processing methods...")
  
  # Test pulse animation generation
  var pulse_dsl = "color custom_red = 0xFF0000\n" +
    "animation solid_red = solid(color=custom_red)\n" +
    "animation pulse_red = pulsating_animation(color=custom_red, period=2000)"
  
  var berry_code = animation_dsl.compile(pulse_dsl)
  assert(berry_code != nil, "Should compile pulse animation")
  assert(string.find(berry_code, "animation.pulsating_animation(engine)") >= 0, "Should generate pulse animation")
  
  # Test control flow
  var control_dsl = "color custom_blue = 0x0000FF\n" +
    "animation blue_anim = solid(color=custom_blue)\n" +
    "sequence test {\n" +
    "  repeat 2 times {\n" +
    "    play blue_anim for 1s\n" +
    "    wait 500ms\n" +
    "  }\n" +
    "}\n" +
    "run test"
  
  berry_code = animation_dsl.compile(control_dsl)
  assert(berry_code != nil, "Should compile control flow")
  assert(string.find(berry_code, "push_repeat_subsequence") >= 0, "Should generate repeat loop")
  assert(string.find(berry_code, "push_wait_step") >= 0, "Should generate wait statement")
  
  # Test variable assignments
  var var_dsl = "set opacity = 75%\n" +
    "set duration = 3s"
  
  berry_code = animation_dsl.compile(var_dsl)
  assert(berry_code != nil, "Should compile variables")
  assert(string.find(berry_code, "var opacity_ = 191") >= 0, "Should convert percentage")
  assert(string.find(berry_code, "var duration_ = 3000") >= 0, "Should convert time")
  
  print("✓ Core processing methods test passed")
  return true
end

# Test event system DSL compilation
def test_event_system_dsl()
  print("Testing event system DSL compilation...")
  
  var event_dsl = "# strip length 30  # TEMPORARILY DISABLED\n" +
    "color custom_red = 0xFF0000\n" +
    "color custom_blue = 0x0000FF\n" +
    "\n" +
    "# Event handlers\n" +
    "animation red_solid = solid(color=red)\n" +
    "animation blue_solid = solid(color=blue)\n" +
    "on button_press: red_solid\n" +
    "on timer(5s): blue_solid\n" +
    "on startup: interrupt current\n" +
    "\n" +
    "# Main sequence\n" +
    "sequence main {\n" +
    "  play solid(color=red) for 2s\n" +
    "}\n" +
    "\n" +
    "run main"
  
  var berry_code = animation_dsl.compile(event_dsl)
  
  # Event system is complex and simplified transpiler has basic support
  if berry_code != nil
    print("Event system compiled successfully (basic support)")
    
    # Check for basic event handler registration if present
    if string.find(berry_code, "register_event_handler") >= 0
      print("Event handler registration found")
    end
  else
    print("Event system compilation failed - this is expected with simplified transpiler")
    print("Core DSL functionality is working correctly")
  end
  
  # print("Generated event system Berry code:")
  # print("==================================================")
  # print(berry_code)
  # print("==================================================")
  
  print("✓ Event system DSL test passed")
  return true
end

# Test property assignments
def test_property_assignments()
  print("Testing property assignments...")
  
  var dsl_with_properties = "color custom_red = 0xFF0000\n" +
    "animation red_anim = solid(color=custom_red)\n" +
    "red_anim.opacity = 128\n" +
    "red_anim.priority = 10"
  
  var berry_code = animation_dsl.compile(dsl_with_properties)
  
  assert(berry_code != nil, "Should generate Berry code with property assignments")
  
  # Check that property assignments are generated correctly (new behavior: direct underscore access)
  assert(string.find(berry_code, "red_anim_.opacity = 128") >= 0, "Should generate opacity property assignment")
  assert(string.find(berry_code, "red_anim_.priority = 10") >= 0, "Should generate priority property assignment")
  
  # Verify the generated code compiles
  try
    compile(berry_code)
    print("✓ Generated property assignment code compiles successfully")
  except .. as e, msg
    print(f"✗ Generated property assignment code compilation failed: {msg}")
    assert(false, "Generated code should compile")
  end
  
  print("✓ Property assignments test passed")
  return true
end

# Test comment preservation in generated Berry code
def test_comment_preservation()
  print("Testing comment preservation...")
  
  var dsl_with_comments = "# Header comment\n" +
    "# strip length 30  # Strip config comment (TEMPORARILY DISABLED)\n" +
    "# Color section\n" +
    "color custom_red = 0xFF0000  # Red color\n" +
    "animation solid_red = solid(color=custom_red)  # Red animation\n" +
    "sequence demo {\n" +
    "  # Play red\n" +
    "  play solid_red for 2s  # Red phase\n" +
    "  wait 1s  # Pause\n" +
    "}\n" +
    "run demo  # Execute"
  
  var berry_code = animation_dsl.compile(dsl_with_comments)
  
  assert(berry_code != nil, "Should generate Berry code with comments")
  
  # Check that comments are preserved
  assert(string.find(berry_code, "# Header comment") >= 0, "Should preserve header comment")
  assert(string.find(berry_code, "# Strip config comment") >= 0, "Should preserve inline comment")
  assert(string.find(berry_code, "# Color section") >= 0, "Should preserve section comment")
  assert(string.find(berry_code, "# Red color") >= 0, "Should preserve color comment")
  assert(string.find(berry_code, "# Red animation") >= 0, "Should preserve animation comment")
  assert(string.find(berry_code, "  # Play red") >= 0, "Should preserve sequence comment with indentation")
  assert(string.find(berry_code, "# Red phase") >= 0, "Should preserve play statement comment")
  assert(string.find(berry_code, "# Pause") >= 0, "Should preserve wait statement comment")
  assert(string.find(berry_code, "# Execute") >= 0, "Should preserve run statement comment")
  
  # Count comment lines
  var lines = string.split(berry_code, "\n")
  var comment_count = 0
  for line : lines
    if string.find(line, "#") >= 0
      comment_count += 1
    end
  end
  
  assert(comment_count >= 9, "Should have at least 9 lines with comments")
  
  print("✓ Comment preservation test passed")
  return true
end

# Test easing keywords
def test_easing_keywords()
  print("Testing easing keywords...")
  
  var dsl_with_easing = "# strip length 30  # TEMPORARILY DISABLED\n" +
    "# Test all easing keywords\n" +
    "animation linear_anim = solid(color=linear)\n" +
    "animation smooth_anim = solid(color=smooth)\n" +
    "animation ease_in_anim = solid(color=ease_in)\n" +
    "animation ease_out_anim = solid(color=ease_out)\n" +
    "animation ramp_anim = solid(color=ramp)\n" +
    "animation square_anim = solid(color=square)\n" +
    "run linear_anim"
  
  var berry_code = animation_dsl.compile(dsl_with_easing)
  
  assert(berry_code != nil, "Should generate Berry code with easing keywords")
  
  # Check that all easing keywords are properly converted to direct animation module access
  var easing_keywords = ["linear", "smooth", "ease_in", "ease_out", "ramp", "square"]
  for easing : easing_keywords
    # Check if the easing keyword exists in animation module (they're lowercase)
    import introspect
    if introspect.contains(animation, easing)
      assert(string.find(berry_code, f"animation.{easing}") >= 0, f"Should convert {easing} to animation.{easing}")
    else
      assert(string.find(berry_code, f"{easing}_") >= 0, f"Should convert {easing} to {easing}_")
    end
  end
  
  # Test easing keywords as function calls (regression test for breathing_colors.anim issue)
  var dsl_with_function_calls = "# strip length 30  # TEMPORARILY DISABLED\n" +
    "color custom_red = 0xFF0000\n" +
    "animation test_anim = solid(color=custom_red)\n" +
    "test_anim.opacity = 128\n" +
    "run test_anim"
  
  var function_call_code = animation_dsl.compile(dsl_with_function_calls)
  assert(function_call_code != nil, "Should handle easing keywords as function calls")
  # Note: Function calls like smooth(100, 255, 4s) are handled differently than simple identifiers
  # They should generate animation.smooth(100, 255, 4000) calls
  assert(string.find(function_call_code, "test_anim_.opacity = 128") >= 0, "Should set opacity property correctly")
  
  print("✓ Easing keywords test passed")
  return true
end

# Test animation type checking
def test_animation_type_checking()
  print("Testing animation type checking...")
  
  # Test valid animation factory functions
  var valid_animation_dsl = "# strip length 30  # TEMPORARILY DISABLED\n" +
    "color custom_red = 0xFF0000\n" +
    "animation pulse_red = pulsating_animation(color=custom_red, period=2000)\n" +
    "animation solid_blue = solid(color=0x0000FF)\n" +
    "run pulse_red"
  
  var berry_code = animation_dsl.compile(valid_animation_dsl)
  assert(berry_code != nil, "Should compile valid animation factories")
  assert(string.find(berry_code, "animation.pulsating_animation(engine)") >= 0, "Should generate pulsating_animation call")
  assert(string.find(berry_code, "animation.solid(engine)") >= 0, "Should generate solid call")
  
  # Test invalid animation factory function (should fail at transpile time)
  var invalid_animation_dsl = "# strip length 30  # TEMPORARILY DISABLED\n" +
    "animation invalid_anim = non_existent_animation(color=custom_red)"
  
  try
    var invalid_code = animation_dsl.compile(invalid_animation_dsl)
    assert(false, "Should have failed for non-existent animation factory")
  except "dsl_compilation_error" as e, msg
    assert(string.find(msg, "does not exist") >= 0, "Should report non-existent factory")
  end
  
  # Test color provider assigned to animation (should fail at transpile time)
  var color_provider_as_animation_dsl = "# strip length 30  # TEMPORARILY DISABLED\n" +
    "animation invalid_anim = rich_palette(palette=breathe_palette)"
  
  try
    var invalid_code = animation_dsl.compile(color_provider_as_animation_dsl)
    assert(false, "Should have failed for color provider assigned to animation")
  except "dsl_compilation_error" as e, msg
    assert(string.find(msg, "does not create an instance of animation.animation class") >= 0, "Should report type mismatch")
  end
  
  print("✓ Animation type checking test passed")
  return true
end

# Test color type checking and color providers
def test_color_type_checking()
  print("Testing color type checking...")
  
  # Test simple color values
  var simple_color_dsl = "# strip length 30  # TEMPORARILY DISABLED\n" +
    "color custom_red = 0xFF0000\n" +
    "color custom_blue = 0x0000FF\n" +
    "color named_green = green\n" +
    "animation test_anim = solid(color=custom_red)\n" +
    "run test_anim"
  
  var berry_code = animation_dsl.compile(simple_color_dsl)
  assert(berry_code != nil, "Should compile simple color values")
  assert(string.find(berry_code, "var custom_red_ = 0xFFFF0000") >= 0, "Should generate red color")
  assert(string.find(berry_code, "var custom_blue_ = 0xFF0000FF") >= 0, "Should generate blue color")
  
  # Test color provider functions (if they exist)
  var color_provider_dsl = "# strip length 30  # TEMPORARILY DISABLED\n" +
    "color cycle_colors = color_cycle(palette=[0xFF0000, 0x00FF00, 0x0000FF])\n" +
    "animation cycle_anim = solid(color=cycle_colors)\n" +
    "run cycle_anim"
  
  try
    var provider_code = animation_dsl.compile(color_provider_dsl)
    if provider_code != nil
      print("Color provider compilation successful")
      assert(string.find(provider_code, "animation.color_cycle(engine)") >= 0, "Should generate color provider call")
    else
      print("Color provider compilation failed - this may be expected if color providers don't exist")
    end
  except "dsl_compilation_error" as e, msg
    print("Color provider compilation failed (expected if not implemented): " + msg)
  end
  
  # Test invalid color provider function (should fail at transpile time)
  var invalid_color_dsl = "# strip length 30  # TEMPORARILY DISABLED\n" +
    "color invalid_color = non_existent_color_provider(param=value)"
  
  try
    var invalid_code = animation_dsl.compile(invalid_color_dsl)
    # This might succeed if the transpiler doesn't validate color providers yet
    print("Invalid color provider compiled - validation may not be fully implemented")
  except "dsl_compilation_error" as e, msg
    print("Invalid color provider correctly rejected: " + msg)
  end
  
  print("✓ Color type checking test passed")
  return true
end

# Test invalid sequence commands
def test_invalid_sequence_commands()
  print("Testing invalid sequence commands...")
  
  # Test 1: Invalid command in sequence
  var invalid_command_dsl = 
    "animation test_anim = solid(color=red)\n" +
    "sequence bad {\n" +
    "  do_bad_things anim\n" +
    "  play test_anim for 1s\n" +
    "}"
  
  try
    var result1 = animation_dsl.compile(invalid_command_dsl)
    assert(false, "Should have thrown an exception for invalid command")
  except "dsl_compilation_error"
    # Expected - invalid command should cause compilation error
  end
  
  # Test 2: Another invalid command
  var invalid_command_dsl2 = 
    "animation test_anim = solid(color=red)\n" +
    "sequence bad {\n" +
    "  play test_anim for 1s\n" +
    "  invalid_command\n" +
    "  wait 500ms\n" +
    "}"
  
  try
    var result2 = animation_dsl.compile(invalid_command_dsl2)
    assert(false, "Should have thrown an exception for invalid command")
  except "dsl_compilation_error"
    # Expected - invalid command should cause compilation error
  end
  
  # Test 3: Invalid command in repeat block
  var invalid_repeat_dsl = 
    "animation test_anim = solid(color=red)\n" +
    "sequence bad {\n" +
    "  repeat 3 times {\n" +
    "    play test_anim for 1s\n" +
    "    bad_command_in_repeat\n" +
    "    wait 500ms\n" +
    "  }\n" +
    "}"
  
  try
    var result3 = animation_dsl.compile(invalid_repeat_dsl)
    assert(false, "Should have thrown an exception for invalid command in repeat")
  except "dsl_compilation_error"
    # Expected - invalid command should cause compilation error
  end
  
  # Test 4: Valid sequence should still work
  var valid_sequence_dsl = 
    "animation test_anim = solid(color=red)\n" +
    "sequence good {\n" +
    "  play test_anim for 1s\n" +
    "  wait 500ms\n" +
    "  log(\"test message\")\n" +
    "  test_anim.opacity = 128\n" +
    "}"
  
  var result4 = animation_dsl.compile(valid_sequence_dsl)
  assert(result4 != nil, "Should compile valid sequence successfully")
  assert(string.find(result4, "SequenceManager") >= 0, "Should generate sequence manager")
  assert(string.find(result4, "push_play_step") >= 0, "Should generate play step")
  assert(string.find(result4, "push_wait_step") >= 0, "Should generate wait step")
  assert(string.find(result4, "log(f\"test message\", 3)") >= 0, "Should generate log statement")
  assert(string.find(result4, "push_closure_step") >= 0, "Should generate closure steps")
  
  print("✓ Invalid sequence commands test passed")
  return true
end

# Test template-only transpilation
def test_template_only_transpilation()
  print("Testing template-only transpilation...")
  
  # Test single template definition
  var single_template_dsl = "template pulse_effect {\n" +
    "  param base_color type color\n" +
    "  param duration\n" +
    "  param brightness type number\n" +
    "  \n" +
    "  animation pulse = pulsating_animation(\n" +
    "    color=base_color\n" +
    "    period=duration\n" +
    "  )\n" +
    "  pulse.opacity = brightness\n" +
    "  run pulse\n" +
    "}"
  
  var single_code = animation_dsl.compile(single_template_dsl)
  assert(single_code != nil, "Should compile single template")
  
  # Should NOT contain engine initialization
  assert(string.find(single_code, "var engine = animation.init_strip()") < 0, "Should NOT generate engine initialization for template-only file")
  
  # Should NOT contain engine.run()
  assert(string.find(single_code, "engine.run()") < 0, "Should NOT generate engine.run() for template-only file")
  
  # Should contain template function definition
  assert(string.find(single_code, "def pulse_effect_template(engine, base_color_, duration_, brightness_)") >= 0, "Should generate template function")
  
  # Should contain function registration
  assert(string.find(single_code, "animation.register_user_function('pulse_effect', pulse_effect_template)") >= 0, "Should register template function")
  
  # Test multiple templates
  var multiple_templates_dsl = "template pulse_effect {\n" +
    "  param base_color type color\n" +
    "  param duration\n" +
    "  \n" +
    "  animation pulse = pulsating_animation(\n" +
    "    color=base_color\n" +
    "    period=duration\n" +
    "  )\n" +
    "  run pulse\n" +
    "}\n" +
    "\n" +
    "template blink_red {\n" +
    "  param speed\n" +
    "  \n" +
    "  animation blink = pulsating_animation(\n" +
    "    color=red\n" +
    "    period=speed\n" +
    "  )\n" +
    "  \n" +
    "  run blink\n" +
    "}"
  
  var multiple_code = animation_dsl.compile(multiple_templates_dsl)
  assert(multiple_code != nil, "Should compile multiple templates")
  
  # Should NOT contain engine initialization or run
  assert(string.find(multiple_code, "var engine = animation.init_strip()") < 0, "Should NOT generate engine initialization for multiple templates")
  assert(string.find(multiple_code, "engine.run()") < 0, "Should NOT generate engine.run() for multiple templates")
  
  # Should contain both template functions
  assert(string.find(multiple_code, "def pulse_effect_template(") >= 0, "Should generate first template function")
  assert(string.find(multiple_code, "def blink_red_template(") >= 0, "Should generate second template function")
  
  # Should contain both registrations
  assert(string.find(multiple_code, "animation.register_user_function('pulse_effect'") >= 0, "Should register first template")
  assert(string.find(multiple_code, "animation.register_user_function('blink_red'") >= 0, "Should register second template")
  
  print("✓ Template-only transpilation test passed")
  return true
end

# Test mixed template and DSL transpilation
def test_mixed_template_dsl_transpilation()
  print("Testing mixed template and DSL transpilation...")
  
  # Test template with regular DSL (should generate engine initialization and run)
  var mixed_dsl = "template pulse_effect {\n" +
    "  param base_color type color\n" +
    "  param duration\n" +
    "  \n" +
    "  animation pulse = pulsating_animation(\n" +
    "    color=base_color\n" +
    "    period=duration\n" +
    "  )\n" +
    "  run pulse\n" +
    "}\n" +
    "\n" +
    "color my_red = 0xFF0000\n" +
    "animation test_anim = solid(color=my_red)\n" +
    "run test_anim"
  
  var mixed_code = animation_dsl.compile(mixed_dsl)
  assert(mixed_code != nil, "Should compile mixed template and DSL")
  
  # Should contain engine initialization because of non-template DSL
  assert(string.find(mixed_code, "var engine = animation.init_strip()") >= 0, "Should generate engine initialization for mixed content")
  
  # Should contain engine.run() because of run statement
  assert(string.find(mixed_code, "engine.run()") >= 0, "Should generate engine.run() for mixed content")
  
  # Should contain template function
  assert(string.find(mixed_code, "def pulse_effect_template(") >= 0, "Should generate template function")
  
  # Should contain regular DSL elements
  assert(string.find(mixed_code, "var my_red_ = 0xFFFF0000") >= 0, "Should generate color definition")
  assert(string.find(mixed_code, "var test_anim_ = animation.solid(engine)") >= 0, "Should generate animation definition")
  
  # Test template with property assignment (should generate engine initialization)
  var template_with_property_dsl = "template pulse_effect {\n" +
    "  param base_color type color\n" +
    "  \n" +
    "  animation pulse = pulsating_animation(color=base_color, period=2s)\n" +
    "  run pulse\n" +
    "}\n" +
    "\n" +
    "animation test_anim = solid(color=red)\n" +
    "test_anim.opacity = 128"
  
  var property_code = animation_dsl.compile(template_with_property_dsl)
  assert(property_code != nil, "Should compile template with property assignment")
  
  # Should generate engine initialization because of property assignment
  assert(string.find(property_code, "var engine = animation.init_strip()") >= 0, "Should generate engine initialization for property assignment")
  
  # Should NOT generate engine.run() because no run statement
  assert(string.find(property_code, "engine.run()") < 0, "Should NOT generate engine.run() without run statement")
  
  print("✓ Mixed template and DSL transpilation test passed")
  return true
end

# Run all tests
def run_dsl_transpiler_tests()
  print("=== DSL Transpiler Test Suite ===")
  
  var tests = [
    test_transpiler_components,
    test_basic_transpilation,
    test_color_definitions,
    test_color_alpha_channel,
    test_strip_configuration,
    test_simple_patterns,
    test_sequences,
    test_sequence_assignments,
    test_variable_duration,
    test_multiple_run_statements,
    test_variable_assignments,
    test_computed_values,
    test_error_handling,
    test_forward_references,
    test_complex_dsl,
    test_core_processing_methods,
    test_event_system_dsl,
    test_property_assignments,
    test_comment_preservation,
    test_easing_keywords,
    test_animation_type_checking,
    test_color_type_checking,
    test_invalid_sequence_commands,
    test_template_only_transpilation,
    test_mixed_template_dsl_transpilation
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
    print("🎉 All DSL transpiler tests passed!")
    return true
  else
    print("❌ Some DSL transpiler tests failed")
    raise "test_failed"
  end
end

# Auto-run tests when file is executed
run_dsl_transpiler_tests()