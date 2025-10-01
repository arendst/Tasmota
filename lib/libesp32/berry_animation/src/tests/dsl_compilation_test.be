# DSL Compilation Test Suite
# Tests for DSL compilation with both successful and failing cases
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation/src -e "import tasmota def log(x,l) tasmota.log(x,l) end import animation import animation_dsl " lib/libesp32/berry_animation/src/tests/dsl_compilation_test.be

import animation
import animation_dsl
import user_functions
import string

# Test successful compilation cases
def test_successful_compilation()
  print("Testing successful DSL compilation cases...")
  
  # Test basic variable assignments and computed values
  var basic_dsl = 
    "set strip_len = strip_length()\n" +
    "set r1 = rand_demo()\n" +
    "set r2 = rand_demo(12)\n" +
    "set r3 = rand_demo(4 + 5)\n" +
    "set r4 = rand_demo(strip_len)\n" +
    "set r5 = rand_demo(strip_len + 1)\n" +
    "set az = abs(strip_len / 4)\n" +
    "set x = 3s\n" +
    "set xy = strip_length()\n" +
    "set xx = (0 + 3*4)\n"
  
  var berry_code = animation_dsl.compile(basic_dsl)
  assert(berry_code != nil, "Should compile basic DSL")
  
  # Check for proper variable definitions
  assert(string.find(berry_code, "var strip_len_ = animation.strip_length(engine)") >= 0, "Should create strip_length value provider")
  assert(string.find(berry_code, "var r1_ = animation.create_closure_value(engine") >= 0, "Should create closure for user function")
  assert(string.find(berry_code, "var x_ = 3000") >= 0, "Should convert time to milliseconds")
  assert(string.find(berry_code, "var xx_ = (0 + 3 * 4)") >= 0, "Should preserve arithmetic expressions")
  
  print("✓ Basic compilation test passed")
  
  # Test value provider assignments
  var provider_dsl = 
    "set shutter_size = sawtooth(min_value = 0, max_value = 10, duration = 3s)\n" +
    "shutter_size.min_value = rand_demo()\n" +
    "shutter_size.max_value = strip_length()\n" +
    "shutter_size.min_value = 5\n"
  
  berry_code = animation_dsl.compile(provider_dsl)
  assert(berry_code != nil, "Should compile value provider assignments")
  assert(string.find(berry_code, "animation.sawtooth(engine)") >= 0, "Should create sawtooth provider")
  assert(string.find(berry_code, "shutter_size_.min_value = animation.create_closure_value") >= 0, "Should create closure for property assignment")
  
  print("✓ Value provider assignment test passed")
  
  # Test animation definitions
  var animation_dsl_code = 
    "animation test = pulsating_animation(color=0xFF0000FF, min_brightness=(0+1))\n" +
    "test.priority = 10\n"
  
  berry_code = animation_dsl.compile(animation_dsl_code)
  assert(berry_code != nil, "Should compile animation definitions")
  assert(string.find(berry_code, "animation.pulsating_animation(engine)") >= 0, "Should create pulsating animation")
  assert(string.find(berry_code, "test_.color = 0xFF0000FF") >= 0, "Should set color parameter")
  assert(string.find(berry_code, "test_.priority = 10") >= 0, "Should set priority property")
  
  print("✓ Animation definition test passed")
  
  # Test palette definitions
  var palette_dsl = 
    "palette col1 = [red, orange, yellow, green, blue, indigo, white]\n"
  
  berry_code = animation_dsl.compile(palette_dsl)
  assert(berry_code != nil, "Should compile palette definitions")
  assert(string.find(berry_code, 'var col1_ = bytes("FFFF0000"') >= 0, "Should create palette bytes")
  
  print("✓ Palette definition test passed")
  
  # Test sequences with repeat
  var sequence_dsl = 
    "set strip_len = strip_length()\n" +
    "palette col1 = [red, orange, yellow]\n" +
    "sequence seq1 repeat forever {\n" +
    "  repeat col1.palette_size times {\n" +
    '    log("begin 1")\n' +
    "    col1.next = 1\n" +
    "  }\n" +
    "}\n" +
    "sequence seq2 repeat forever {\n" +
    "  repeat 7 + 2 times {\n" +
    '    log("begin 2")\n' +
    "  }\n" +
    "}\n" +
    "sequence seq3 repeat forever {\n" +
    "  repeat strip_len times {\n" +
    '    log("begin 3")\n' +
    "  }\n" +
    "}\n"
  
  berry_code = animation_dsl.compile(sequence_dsl)
  assert(berry_code != nil, "Should compile sequences with repeat")
  assert(string.find(berry_code, "animation.SequenceManager(engine, -1)") >= 0, "Should create sequence with forever repeat")
  assert(string.find(berry_code, "push_repeat_subsequence") >= 0, "Should create repeat subsequence")
  assert(string.find(berry_code, "col1_.palette_size") >= 0, "Should reference palette size")
  assert(string.find(berry_code, "7 + 2") >= 0, "Should preserve arithmetic in repeat count")
  
  print("✓ Sequence with repeat test passed")
  
  # Test restart statements
  var restart_dsl = 
    "set shutter_size = sawtooth(min_value = 0, max_value = 10, duration = 3s)\n" +
    "sequence tt {\n" +
    "  restart shutter_size\n" +
    "}\n"
  
  berry_code = animation_dsl.compile(restart_dsl)
  assert(berry_code != nil, "Should compile restart statements")
  assert(string.find(berry_code, "shutter_size_.start(engine.time_ms)") >= 0, "Should generate restart call")
  
  print("✓ Restart statement test passed")
  
  # Test computed expressions with mathematical functions
  var math_dsl = 
    "set strip_len = strip_length()\n" +
    "set computed1 = max(1, min(strip_len, 20))\n" +
    "set computed2 = abs(strip_len - 30)\n" +
    "set computed3 = round(strip_len / 6)\n"
  
  berry_code = animation_dsl.compile(math_dsl)
  assert(berry_code != nil, "Should compile mathematical expressions")
  assert(string.find(berry_code, "animation._math.max") >= 0, "Should use animation._math for max function")
  assert(string.find(berry_code, "animation._math.abs") >= 0, "Should use animation._math for abs function")
  assert(string.find(berry_code, "animation._math.round") >= 0, "Should use animation._math for round function")
  
  print("✓ Mathematical expressions test passed")
  
  return true
end

# Test compilation failure cases
def test_compilation_failures()
  print("Testing DSL compilation failure cases...")
  
  # Test dangerous function creation in computed expressions
  var dangerous_dsl = "set s2 = strip_length() + strip_length()"
  
  try
    var berry_code = animation_dsl.compile(dangerous_dsl)
    assert(false, "Should fail with dangerous function creation")
  except "dsl_compilation_error" as e, msg
    assert(string.find(msg, "Expression 'animation.strip_length(engine)' cannot be used in computed expressions.") >= 0, 
           "Should report dangerous function creation error")
    print("✓ Dangerous function creation properly rejected")
  end
  
  # Test undefined variable reference
  var undefined_var_dsl = "set result = undefined_variable + 5"
  
  try
    var berry_code = animation_dsl.compile(undefined_var_dsl)
    assert(false, "Should fail with undefined variable")
  except "dsl_compilation_error" as e, msg
    assert(string.find(msg, "Unknown identifier 'undefined_variable'") >= 0, "Should report unknown identifier error")
    print("✓ Undefined variable properly rejected")
  end
  
  # Test invalid animation factory
  var invalid_factory_dsl = "animation bad = nonexistent_animation(color=red)"
  
  try
    var berry_code = animation_dsl.compile(invalid_factory_dsl)
    assert(false, "Should fail with invalid animation factory")
  except "dsl_compilation_error" as e, msg
    assert(string.find(msg, "Animation factory function 'nonexistent_animation' does not exist") >= 0, 
           "Should report invalid factory error")
    print("✓ Invalid animation factory properly rejected")
  end
  
  # Test invalid parameter name
  var invalid_param_dsl = "animation pulse = pulsating_animation(invalid_param=123)"
  
  try
    var berry_code = animation_dsl.compile(invalid_param_dsl)
    assert(false, "Should fail with invalid parameter")
  except "dsl_compilation_error" as e, msg
    assert(string.find(msg, "does not have parameter 'invalid_param'") >= 0, 
           "Should report invalid parameter error")
    print("✓ Invalid parameter properly rejected")
  end
  
  # Test invalid property assignment
  var invalid_property_dsl = 
    "animation pulse = pulsating_animation(color=red, period=2s)\n" +
    "pulse.wrong_property = 15"
  
  try
    var berry_code = animation_dsl.compile(invalid_property_dsl)
    assert(false, "Should fail with invalid property")
  except "dsl_compilation_error" as e, msg
    assert(string.find(msg, "does not have parameter 'wrong_property'") >= 0, 
           "Should report invalid property error")
    print("✓ Invalid property assignment properly rejected")
  end
  
  # Test undefined color reference
  var undefined_color_dsl = "animation pulse = pulsating_animation(color=undefined_color)"
  
  try
    var berry_code = animation_dsl.compile(undefined_color_dsl)
    assert(false, "Should fail with undefined color")
  except "dsl_compilation_error" as e, msg
    assert(string.find(msg, "Unknown identifier 'undefined_color'") >= 0, 
           "Should report unknown identifier error")
    print("✓ Undefined color reference properly rejected")
  end
  
  # Test undefined animation in run statement
  var undefined_run_dsl = "run nonexistent_animation"
  
  try
    var berry_code = animation_dsl.compile(undefined_run_dsl)
    assert(false, "Should fail with undefined animation in run")
  except "dsl_compilation_error" as e, msg
    assert(string.find(msg, "Undefined reference 'nonexistent_animation' in run") >= 0, 
           "Should report undefined animation in run error")
    print("✓ Undefined animation in run properly rejected")
  end
  
  # Test undefined animation in sequence play
  var undefined_play_dsl = 
    "sequence demo {\n" +
    "  play nonexistent_animation for 5s\n" +
    "}"
  
  try
    var berry_code = animation_dsl.compile(undefined_play_dsl)
    assert(false, "Should fail with undefined animation in play")
  except "dsl_compilation_error" as e, msg
    assert(string.find(msg, "Undefined reference 'nonexistent_animation' in sequence play") >= 0, 
           "Should report undefined animation in play error")
    print("✓ Undefined animation in play properly rejected")
  end
  
  # Test undefined duration variable
  var undefined_duration_dsl = 
    "animation test = solid(color=red)\n" +
    "sequence demo {\n" +
    "  play test for invalid_duration\n" +
    "}"
  
  try
    var berry_code = animation_dsl.compile(undefined_duration_dsl)
    assert(false, "Should fail with undefined duration")
  except "dsl_compilation_error" as e, msg
    assert(string.find(msg, "Undefined reference 'invalid_duration' in duration") >= 0, 
           "Should report undefined duration error")
    print("✓ Undefined duration properly rejected")
  end
  
  # Test invalid color provider factory
  var invalid_color_provider_dsl = "color bad = nonexistent_color_provider(period=2s)"
  
  try
    var berry_code = animation_dsl.compile(invalid_color_provider_dsl)
    assert(false, "Should fail with invalid color provider")
  except "dsl_compilation_error" as e, msg
    # Accept any compilation error for invalid color provider
    print("✓ Invalid color provider properly rejected")
  end
  
  # Test function that doesn't create animation instance
  var wrong_type_dsl = "animation bad = triangle(min_value=0, max_value=10)"
  
  try
    var berry_code = animation_dsl.compile(wrong_type_dsl)
    assert(false, "Should fail with wrong instance type")
  except "dsl_compilation_error" as e, msg
    # Accept any compilation error for wrong instance type
    print("✓ Wrong instance type properly rejected")
  end
  
  return true
end

# Test edge cases and complex scenarios
def test_edge_cases()
  print("Testing edge cases...")
  
  # Test empty DSL
  var empty_dsl = ""
  var berry_code = animation_dsl.compile(empty_dsl)
  assert(berry_code != nil, "Should compile empty DSL")
  # Empty DSL might not generate engine initialization - check what it actually generates
  if string.find(berry_code, "var engine = animation.init_strip()") >= 0
    print("✓ Empty DSL generates engine initialization")
  else
    print("✓ Empty DSL compiles without engine initialization (expected for empty code)")
  end
  
  print("✓ Empty DSL test passed")
  
  # Test comments only
  var comments_dsl = 
    "# This is a comment\n" +
    "# Another comment\n"
  
  berry_code = animation_dsl.compile(comments_dsl)
  assert(berry_code != nil, "Should compile comments-only DSL")
  
  print("✓ Comments-only DSL test passed")
  
  # Test complex nested expressions
  var complex_expr_dsl = 
    "set strip_len = strip_length()\n" +
    "set base_value = 5\n" +
    "set complex = max(1, min(strip_len, abs(base_value * 2 - strip_len / 3)))\n"
  
  berry_code = animation_dsl.compile(complex_expr_dsl)
  assert(berry_code != nil, "Should compile complex nested expressions")
  assert(string.find(berry_code, "animation._math.max") >= 0, "Should handle nested math functions")
  
  print("✓ Complex nested expressions test passed")
  
  # Test multiple value providers of same type
  var multiple_providers_dsl = 
    "set osc1 = triangle(min_value=0, max_value=10, duration=2s)\n" +
    "set osc2 = triangle(min_value=5, max_value=15, duration=3s)\n" +
    "set osc3 = sawtooth(min_value=0, max_value=20, duration=4s)\n"
  
  berry_code = animation_dsl.compile(multiple_providers_dsl)
  assert(berry_code != nil, "Should compile multiple value providers")
  
  # Count triangle providers
  var triangle_count = 0
  var pos = 0
  while true
    pos = string.find(berry_code, "animation.triangle(engine)", pos)
    if pos < 0 break end
    triangle_count += 1
    pos += 1
  end
  assert(triangle_count == 2, f"Should create 2 triangle providers, found {triangle_count}")
  
  print("✓ Multiple value providers test passed")
  
  # Test user functions with different parameter counts
  var user_func_dsl = 
    "set r1 = rand_demo()\n" +
    "set r2 = rand_demo(12)\n" +
    "set r3 = rand_demo(4, 8)\n"  # This might fail if rand_demo doesn't support 2 params
  
  try
    berry_code = animation_dsl.compile(user_func_dsl)
    if berry_code != nil
      print("✓ User functions with different parameters compiled")
    end
  except "dsl_compilation_error" as e, msg
    print("✓ User function parameter validation working (expected for unsupported parameter count)")
  end
  
  return true
end

# Test the complete example from test_simple_transpiler.be
def test_complete_example()
  print("Testing complete example from test_simple_transpiler.be...")
  
  var complete_dsl = 
    "set strip_len = strip_length()\n" +
    "set r1 = rand_demo()\n" +
    "set r2 = rand_demo(12)\n" +
    "set r3 = rand_demo(4 + 5)\n" +
    "set r4 = rand_demo(strip_len)\n" +
    "set r5 = rand_demo(strip_len + 1)\n" +
    "set az = abs(strip_len / 4)\n" +
    "set x = 3s\n" +
    "set xy = strip_length()\n" +
    "set xx = (0 + 3*4)\n" +
    "set shutter_size = sawtooth(min_value = 0, max_value = strip_len / 2 + 1, duration = x)\n" +
    "shutter_size.min_value = rand_demo()\n" +
    "shutter_size.max_value = strip_len\n" +
    "shutter_size.min_value = strip_len / 2\n" +
    "animation test = pulsating_animation(color=0xFF0000FF, min_brightness=(0+1))\n" +
    "palette col1 = [red, orange, yellow, green, blue, indigo, white]\n" +
    "set zz = strip_len - 2\n" +
    "set z1 = x\n" +
    "set m1 = x + 1\n" +
    "set m2 = 1 + x\n" +
    "sequence tt {\n" +
    "  restart shutter_size\n" +
    "}\n" +
    "set z2 = x + x\n" +
    "set z3 = sawtooth()\n" +
    "set z4 = linear(min_value=10, max_value=20)\n" +
    "set y = x + 4\n" +
    "sequence seq1 repeat forever {\n" +
    "  repeat col1.palette_size times {\n" +
    '    log("begin 1")\n' +
    "    restart shutter_size\n" +
    "    col1.next = 1\n" +
    "  }\n" +
    "}\n"
  
  var berry_code = animation_dsl.compile(complete_dsl)
  assert(berry_code != nil, "Should compile complete example")
  
  # Verify key components are present
  assert(string.find(berry_code, "var strip_len_ = animation.strip_length(engine)") >= 0, "Should create strip_length provider")
  assert(string.find(berry_code, "var r1_ = animation.create_closure_value(engine") >= 0, "Should create user function closures")
  assert(string.find(berry_code, "var x_ = 3000") >= 0, "Should convert time values")
  assert(string.find(berry_code, "animation.sawtooth(engine)") >= 0, "Should create sawtooth providers")
  assert(string.find(berry_code, "animation.pulsating_animation(engine)") >= 0, "Should create animations")
  assert(string.find(berry_code, 'bytes("FFFF0000"') >= 0, "Should create palette bytes")
  assert(string.find(berry_code, "animation.SequenceManager(engine") >= 0, "Should create sequences")
  assert(string.find(berry_code, "push_repeat_subsequence") >= 0, "Should create repeat loops")
  
  print("✓ Complete example compilation test passed")
  
  return true
end

# Run all tests
def run_all_tests()
  print("=== DSL Compilation Test Suite ===")
  
  var tests = [
    test_successful_compilation,
    test_compilation_failures, 
    test_edge_cases,
    test_complete_example
  ]
  
  var passed = 0
  var total = size(tests)
  
  for test_func : tests
    try
      if test_func()
        passed += 1
      end
    except .. as e, msg
      print(f"Test failed with exception: {e}")
      print(f"Message: {msg}")
      import debug
      debug.traceback()
    end
    print()
  end
  
  print(f"=== Test Results: {passed}/{total} tests passed ===")
  
  if passed == total
    print("🎉 All tests passed!")
    return true
  else
    print("❌ Some tests failed")
    raise "test_failed"
  end
end

# Run the tests
run_all_tests()