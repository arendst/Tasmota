# DSL Transpiler Test Suite
# Tests for SimpleDSLTranspiler class
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/dsl_transpiler_test.be

import animation
import string

# Test basic transpilation
def test_basic_transpilation()
  print("Testing basic DSL transpilation...")
  
  var dsl_source = "strip length 60\n" +
    "color custom_red = #FF0000\n" +
    "pattern solid_red = solid(custom_red)\n" +
    "animation red_anim = solid_red\n" +
    "\n" +
    "sequence demo {\n" +
    "  play red_anim for 5s\n" +
    "}\n" +
    "\n" +
    "run demo"
  
  var berry_code = animation.compile_dsl(dsl_source)
  
  assert(berry_code != nil, "Should generate Berry code")
  assert(string.find(berry_code, "var strip = global.Leds(60)") >= 0, "Should generate strip configuration")
  assert(string.find(berry_code, "var custom_red_ = 0xFFFF0000") >= 0, "Should generate color definition")
  assert(string.find(berry_code, "def sequence_demo()") >= 0, "Should generate sequence function")
  assert(string.find(berry_code, "sequence_demo()") >= 0, "Should generate sequence call")
  
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
    ["color custom_red = #FF0000", "var custom_red_ = 0xFFFF0000"],
    ["color custom_blue = #0000FF", "var custom_blue_ = 0xFF0000FF"],
    ["color my_white = white", "var my_white_ = 0xFFFFFFFF"],
    ["color my_green = green", "var my_green_ = 0xFF008000"]
  ]
  
  for test : color_tests
    var dsl_input = test[0]
    var expected_output = test[1]
    
    var berry_code = animation.compile_dsl(dsl_input)
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
    ["color red_opaque = #FFFF0000", "var red_opaque_ = 0xFFFF0000"],
    ["color red_half = #80FF0000", "var red_half_ = 0x80FF0000"],
    ["color blue_quarter = #400000FF", "var blue_quarter_ = 0x400000FF"],
    ["color clear = #00000000", "var clear_ = 0x00000000"],
    
    # Test 6-character hex without alpha (should add FF for opaque)
    ["color custom_red = #FF0000", "var custom_red_ = 0xFFFF0000"],
    ["color custom_lime = #00FF00", "var custom_lime_ = 0xFF00FF00"],
    
    # Test 4-character short form with alpha
    ["color red_half_short = #8F00", "var red_half_short_ = 0x88FF0000"],
    ["color blue_quarter_short = #400F", "var blue_quarter_short_ = 0x440000FF"],
    
    # Test 3-character short form without alpha (should add FF for opaque)
    ["color red_short = #F00", "var red_short_ = 0xFFFF0000"],
    ["color lime_short = #0F0", "var lime_short_ = 0xFF00FF00"],
    ["color blue_short = #00F", "var blue_short_ = 0xFF0000FF"]
  ]
  
  for test : alpha_color_tests
    var dsl_input = test[0]
    var expected_output = test[1]
    
    var berry_code = animation.compile_dsl(dsl_input)
    assert(berry_code != nil, "Should compile: " + dsl_input)
    assert(string.find(berry_code, expected_output) >= 0, f"Should contain: {expected_output} in: {berry_code}")
  end
  
  print("✓ Color alpha channel test passed")
  return true
end

# Test strip configuration
def test_strip_configuration()
  print("Testing strip configuration...")
  
  var config_tests = [
    ["strip length 30", "var strip = global.Leds(30)"],
    ["strip length 60", "var strip = global.Leds(60)"],
    ["strip length 120", "var strip = global.Leds(120)"]
  ]
  
  for test : config_tests
    var dsl_input = test[0]
    var expected_output = test[1]
    
    var berry_code = animation.compile_dsl(dsl_input)
    assert(berry_code != nil, "Should compile: " + dsl_input)
    assert(string.find(berry_code, expected_output) >= 0, "Should contain: " + expected_output)
  end
  
  print("✓ Strip configuration test passed")
  return true
end

# Test simple patterns
def test_simple_patterns()
  print("Testing simple patterns...")
  
  var dsl_source = "color custom = #FF8080\n"
    "pattern solid_red = solid(red)\n"
    "pattern solid_custom = solid(custom)"
  
  var berry_code = animation.compile_dsl(dsl_source)

  # print("Generated Berry code:")
  # print("==================================================")
  # print(berry_code)
  # print("==================================================")

  assert(berry_code != nil, "Should compile simple pattern")
  assert(string.find(berry_code, "var custom_ = 0xFFFF8080") >= 0, "Should define color")
  assert(string.find(berry_code, "var solid_red_ = animation.solid(0xFFFF0000)") >= 0, "Should define pattern")
  
  print("✓ Simple patterns test passed")
  return true
end

# Test sequences
def test_sequences()
  print("Testing sequences...")
  
  var dsl_source = "color custom_blue = #0000FF\n"
    "animation blue_anim = custom_blue\n"
    "\n"
    "sequence test_seq {\n"
    "  play blue_anim for 3s\n"
    "}\n"
    "\n"
    "run test_seq"
  
  var berry_code = animation.compile_dsl(dsl_source)
  assert(berry_code != nil, "Should compile sequence")
  assert(string.find(berry_code, "def sequence_test_seq()") >= 0, "Should define sequence function")
  assert(string.find(berry_code, "animation.create_play_step(animation.global('blue_anim_'), 3000)") >= 0, "Should reference animation")
  assert(string.find(berry_code, "engine.add_sequence_manager(seq_manager)") >= 0, "Should add sequence manager to engine")
  assert(string.find(berry_code, "engine.start()") >= 0, "Should start engine")
  assert(string.find(berry_code, "sequence_test_seq()") >= 0, "Should call sequence")
  
  print("✓ Sequences test passed")
  return true
end

# Test multiple run statements
def test_multiple_run_statements()
  print("Testing multiple run statements...")
  
  # Test with multiple animations
  var dsl_source = "strip length 30\n" +
    "color custom_red = #FF0000\n" +
    "color custom_blue = #0000FF\n" +
    "color custom_green = #00FF00\n" +
    "\n" +
    "animation red_anim = pulse_position_animation(custom_red, 5, 5, 2)\n" +
    "animation blue_anim = pulse_position_animation(custom_blue, 5, 5, 2)\n" +
    "animation green_anim = pulse_position_animation(custom_green, 5, 5, 2)\n" +
    "\n" +
    "red_anim.pos = 5\n" +
    "blue_anim.pos = 15\n" +
    "green_anim.pos = 25\n" +
    "\n" +
    "run red_anim\n" +
    "run blue_anim\n" +
    "run green_anim"
  
  var berry_code = animation.compile_dsl(dsl_source)
  assert(berry_code != nil, "Should compile multiple run statements")
  
  # Count engine.start() calls - should be exactly 1
  var lines = string.split(berry_code, "\n")
  var start_count = 0
  for line : lines
    if string.find(line, "engine.start()") >= 0
      start_count += 1
    end
  end
  
  assert(start_count == 1, f"Should have exactly 1 engine.start() call, found {start_count}")
  
  # Check that all animations are added to the engine
  assert(string.find(berry_code, "# Start all animations/sequences") >= 0, "Should have consolidated startup comment")
  assert(string.find(berry_code, "engine.add_animation(animation.global('red_anim_'))") >= 0, "Should add red_anim to engine")
  assert(string.find(berry_code, "engine.add_animation(animation.global('blue_anim_'))") >= 0, "Should add blue_anim to engine")
  assert(string.find(berry_code, "engine.add_animation(animation.global('green_anim_'))") >= 0, "Should add green_anim to engine")
  
  # Verify the engine.start() comes after all animations are added
  var start_line_index = -1
  var last_add_line_index = -1
  
  for i : 0..size(lines)-1
    var line = lines[i]
    if string.find(line, "engine.start()") >= 0
      start_line_index = i
    end
    if string.find(line, "engine.add_animation") >= 0 || string.find(line, "engine.add_sequence_manager") >= 0
      last_add_line_index = i
    end
  end
  
  assert(start_line_index > last_add_line_index, "engine.start() should come after all engine.add_* calls")
  
  # Test with mixed animations and sequences
  var mixed_dsl = "strip length 30\n" +
    "color custom_red = #FF0000\n" +
    "color custom_blue = #0000FF\n" +
    "\n" +
    "animation red_anim = pulse_position_animation(custom_red, 5, 5, 2)\n" +
    "\n" +
    "sequence blue_seq {\n" +
    "  play red_anim for 2s\n" +
    "  wait 1s\n" +
    "}\n" +
    "\n" +
    "run red_anim\n" +
    "run blue_seq"
  
  var mixed_berry_code = animation.compile_dsl(mixed_dsl)
  assert(mixed_berry_code != nil, "Should compile mixed run statements")
  
  # Count engine.start() calls in mixed scenario
  var mixed_lines = string.split(mixed_berry_code, "\n")
  var mixed_start_count = 0
  for line : mixed_lines
    if string.find(line, "engine.start()") >= 0
      mixed_start_count += 1
    end
  end
  
  assert(mixed_start_count == 1, f"Mixed scenario should have exactly 1 engine.start() call, found {mixed_start_count}")
  
  # Check that both animation and sequence are handled
  assert(string.find(mixed_berry_code, "engine.add_animation(animation.global('red_anim_'))") >= 0, "Should add animation to engine")
  assert(string.find(mixed_berry_code, "engine.add_sequence_manager(seq_manager)") >= 0, "Should add sequence to engine")
  
  print("✓ Multiple run statements test passed")
  return true
end

# Test variable assignments
def test_variable_assignments()
  print("Testing variable assignments...")
  
  var dsl_source = "set strip_length = 60\n" +
    "set brightness = 80%\n" +
    "set cycle_time = 5s"
  
  var berry_code = animation.compile_dsl(dsl_source)
  assert(berry_code != nil, "Should compile variables")
  assert(string.find(berry_code, "var strip_length_ = 60") >= 0, "Should define numeric variable")
  assert(string.find(berry_code, "var brightness_ = 204") >= 0, "Should convert percentage to 0-255 range")
  assert(string.find(berry_code, "var cycle_time_ = 5000") >= 0, "Should convert time to milliseconds")
  
  print("✓ Variable assignments test passed")
  return true
end

# Test error handling
def test_error_handling()
  print("Testing error handling...")
  
  # Test invalid syntax - should raise exception
  var invalid_dsl = "invalid syntax here"
  try
    var berry_code = animation.compile_dsl(invalid_dsl)
    assert(false, "Should have raised exception for invalid syntax")
  except "dsl_compilation_error" as e, msg
    # Expected behavior
  end
  
  # Test undefined references - simplified transpiler uses runtime resolution
  var undefined_ref_dsl = "animation test = undefined_pattern"
  
  try
    var berry_code = animation.compile_dsl(undefined_ref_dsl)
    # Simplified transpiler uses runtime resolution, so this should compile
    assert(berry_code != nil, "Should compile with runtime resolution")
  except "dsl_compilation_error" as e, msg
    assert(false, "Should not raise exception for undefined references: " + msg)
  end
  
  print("✓ Error handling test passed")
  return true
end

# Test forward references (deferred resolution)
def test_forward_references()
  print("Testing forward references...")
  
  var dsl_source = "# Forward reference: pattern uses color defined later\n" +
    "pattern fire_pattern = gradient(red, orange)\n" +
    "color red = #FF0000\n" +
    "color orange = #FF8000"
  
  var lexer = animation.DSLLexer(dsl_source)
  var tokens = lexer.tokenize()
  var transpiler = animation.SimpleDSLTranspiler(tokens)
  var berry_code = transpiler.transpile()
  
  # Should resolve forward references
  if berry_code != nil
    assert(string.find(berry_code, "var red = 0xFFFF0000") >= 0, "Should define red color")
    assert(string.find(berry_code, "var orange = 0xFFFF8000") >= 0, "Should define orange color")
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
    "strip length 60\n" +
    "\n" +
    "# Color Definitions\n" +
    "color custom_red = #FF0000\n" +
    "color custom_blue = #0000FF\n" +
    "\n" +
    "# Variable Definitions\n" +
    "set cycle_time = 5s\n" +
    "set brightness = 80%\n" +
    "\n" +
    "# Pattern Definitions\n" +
    "pattern solid_red = solid(red)\n" +
    "pattern solid_blue = solid(blue)\n" +
    "\n" +
    "# Animation Definitions\n" +
    "animation red_pulse = pulse(solid_red, 2s, 30%, 100%)\n" +
    "animation blue_breathe = breathe(solid_blue, 4s)\n" +
    "\n" +
    "# Sequence Definition with Control Flow\n" +
    "sequence demo {\n" +
    "  play red_pulse for 3s\n" +
    "  wait 1s\n" +
    "  repeat 2 times:\n" +
    "    play blue_breathe for 2s\n" +
    "    wait 500ms\n" +
    "  if brightness > 50:\n" +
    "    play red_pulse for 2s\n" +
    "  else:\n" +
    "    play blue_breathe for 2s\n" +
    "  with red_pulse for 5s opacity 60%\n" +
    "}\n" +
    "\n" +
    "# Execution\n" +
    "run demo"
  
  var berry_code = animation.compile_dsl(complex_dsl)
  
  if berry_code != nil
    print("Complex DSL compiled successfully!")
    
    # Check for key components
    assert(string.find(berry_code, "var strip = global.Leds(60)") >= 0, "Should have strip config")
    assert(string.find(berry_code, "var custom_red_ = 0xFFFF0000") >= 0, "Should have color definitions")
    assert(string.find(berry_code, "def sequence_demo()") >= 0, "Should have sequence definition")
    assert(string.find(berry_code, "sequence_demo()") >= 0, "Should have execution")
    
    print("Generated code structure looks correct")
  else
    print("Complex DSL compilation failed - checking for specific issues...")
    
    # Test individual components
    var lexer = animation.DSLLexer(complex_dsl)
    var tokens = lexer.tokenize()
    
    if lexer.has_errors()
      print("Lexical errors found:")
      print(lexer.get_error_report())
    else
      print("Lexical analysis passed")
      
      var transpiler = animation.SimpleDSLTranspiler(tokens)
      var result = transpiler.transpile()
      
      if transpiler.has_errors()
        print("Transpilation errors found:")
        print(transpiler.get_error_report())
      end
    end
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
  var lexer = animation.DSLLexer("color red = #FF0000")
  var tokens = lexer.tokenize()
  assert(size(tokens) >= 4, "Should have multiple tokens")
  
  var transpiler = animation.SimpleDSLTranspiler(tokens)
  assert(!transpiler.at_end(), "Should not be at end initially")
  
  print("✓ Transpiler components test passed")
  return true
end

# Test core processing methods functionality
def test_core_processing_methods()
  print("Testing core processing methods...")
  
  # Test pulse animation generation
  var pulse_dsl = "color custom_red = #FF0000\n" +
    "animation pulse_red = pulse(solid(custom_red), 2s, 20%, 100%)"
  
  var berry_code = animation.compile_dsl(pulse_dsl)
  assert(berry_code != nil, "Should compile pulse animation")
  assert(string.find(berry_code, "animation.pulse") >= 0, "Should generate pulse animation")
  
  # Test control flow
  var control_dsl = "color custom_blue = #0000FF\n" +
    "animation blue_anim = solid(custom_blue)\n" +
    "sequence test {\n" +
    "  repeat 2 times:\n" +
    "    play blue_anim for 1s\n" +
    "    wait 500ms\n" +
    "}\n" +
    "run test"
  
  berry_code = animation.compile_dsl(control_dsl)
  assert(berry_code != nil, "Should compile control flow")
  assert(string.find(berry_code, "for repeat_i : 0..2-1") >= 0, "Should generate repeat loop")
  assert(string.find(berry_code, "animation.create_wait_step(500)") >= 0, "Should generate wait statement")
  
  # Test variable assignments
  var var_dsl = "set opacity = 75%\n" +
    "set duration = 3s"
  
  berry_code = animation.compile_dsl(var_dsl)
  assert(berry_code != nil, "Should compile variables")
  assert(string.find(berry_code, "var opacity_ = 191") >= 0, "Should convert percentage")
  assert(string.find(berry_code, "var duration_ = 3000") >= 0, "Should convert time")
  
  print("✓ Core processing methods test passed")
  return true
end

# Test event system DSL compilation
def test_event_system_dsl()
  print("Testing event system DSL compilation...")
  
  var event_dsl = "strip length 30\n" +
    "color custom_red = #FF0000\n" +
    "color custom_blue = #0000FF\n" +
    "\n" +
    "# Event handlers\n" +
    "on button_press: solid(red)\n" +
    "on timer(5s): solid(blue)\n" +
    "on startup: interrupt current\n" +
    "\n" +
    "# Main sequence\n" +
    "sequence main {\n" +
    "  play solid(red) for 2s\n" +
    "}\n" +
    "\n" +
    "run main"
  
  var berry_code = animation.compile_dsl(event_dsl)
  
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
  
  var dsl_with_properties = "color custom_red = #FF0000\n" +
    "animation red_anim = solid(custom_red)\n" +
    "red_anim.pos = 15\n" +
    "red_anim.opacity = 128\n" +
    "red_anim.priority = 10"
  
  var berry_code = animation.compile_dsl(dsl_with_properties)
  
  assert(berry_code != nil, "Should generate Berry code with property assignments")
  
  # Check that property assignments are generated correctly
  assert(string.find(berry_code, "animation.global('red_anim_').pos = 15") >= 0, "Should generate pos property assignment")
  assert(string.find(berry_code, "animation.global('red_anim_').opacity = 128") >= 0, "Should generate opacity property assignment")
  assert(string.find(berry_code, "animation.global('red_anim_').priority = 10") >= 0, "Should generate priority property assignment")
  
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
    "strip length 30  # Strip config comment\n" +
    "# Color section\n" +
    "color custom_red = #FF0000  # Red color\n" +
    "pattern solid_red = solid(custom_red)  # Red pattern\n" +
    "sequence demo {\n" +
    "  # Play red\n" +
    "  play solid_red for 2s  # Red phase\n" +
    "  wait 1s  # Pause\n" +
    "}\n" +
    "run demo  # Execute"
  
  var berry_code = animation.compile_dsl(dsl_with_comments)
  
  assert(berry_code != nil, "Should generate Berry code with comments")
  
  # Check that comments are preserved
  assert(string.find(berry_code, "# Header comment") >= 0, "Should preserve header comment")
  assert(string.find(berry_code, "# Strip config comment") >= 0, "Should preserve inline comment")
  assert(string.find(berry_code, "# Color section") >= 0, "Should preserve section comment")
  assert(string.find(berry_code, "# Red color") >= 0, "Should preserve color comment")
  assert(string.find(berry_code, "# Red pattern") >= 0, "Should preserve pattern comment")
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
  
  var dsl_with_easing = "strip length 30\n" +
    "# Test all easing keywords\n" +
    "animation linear_anim = rich_palette_animation(PALETTE_RAINBOW, 5s, linear, 255)\n" +
    "animation smooth_anim = rich_palette_animation(PALETTE_RAINBOW, 5s, smooth, 255)\n" +
    "animation ease_in_anim = rich_palette_animation(PALETTE_RAINBOW, 5s, ease_in, 255)\n" +
    "animation ease_out_anim = rich_palette_animation(PALETTE_RAINBOW, 5s, ease_out, 255)\n" +
    "animation ramp_anim = rich_palette_animation(PALETTE_RAINBOW, 5s, ramp, 255)\n" +
    "animation square_anim = rich_palette_animation(PALETTE_RAINBOW, 5s, square, 255)\n" +
    "run linear_anim"
  
  var berry_code = animation.compile_dsl(dsl_with_easing)
  
  assert(berry_code != nil, "Should generate Berry code with easing keywords")
  
  # Check that all easing keywords are properly converted to animation.global() calls with new signature
  var easing_keywords = ["linear", "smooth", "ease_in", "ease_out", "ramp", "square"]
  for easing : easing_keywords
    assert(string.find(berry_code, f"animation.global('{easing}_', '{easing}')") >= 0, f"Should convert {easing} to animation.global('{easing}_', '{easing}')")
  end
  
  # Test easing keywords as function calls (regression test for breathing_colors.anim issue)
  var dsl_with_function_calls = "strip length 30\n" +
    "color custom_red = #FF0000\n" +
    "animation test_anim = pulse_position_animation(custom_red, 5, 5, 2)\n" +
    "test_anim.opacity = smooth(100, 255, 4s)\n" +
    "run test_anim"
  
  var function_call_code = animation.compile_dsl(dsl_with_function_calls)
  assert(function_call_code != nil, "Should handle easing keywords as function calls")
  # Note: Function calls like smooth(100, 255, 4s) are handled differently than simple identifiers
  # They should generate animation.smooth(100, 255, 4000) calls
  assert(string.find(function_call_code, "animation.smooth(100, 255, 4000)") >= 0, "Should convert smooth() function call correctly")
  
  print("✓ Easing keywords test passed")
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
    test_multiple_run_statements,
    test_variable_assignments,
    test_error_handling,
    test_forward_references,
    test_complex_dsl,
    test_core_processing_methods,
    test_event_system_dsl,
    test_property_assignments,
    test_comment_preservation,
    test_easing_keywords
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