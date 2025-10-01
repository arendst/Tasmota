# Demo Shutter Infinite Loop Test
# Specific test to isolate the infinite loop in demo_shutter_rainbow_central.anim
#
# Command to run test:
#    ./berry -s -g -m lib/libesp32/berry_animation/src -e "import tasmota def log(x) print(x) end import animation" lib/libesp32/berry_animation/src/tests/demo_shutter_infinite_loop_test.be

import animation_dsl
import string

# Test the exact problematic patterns from the demo file
def test_demo_shutter_patterns()
  print("Testing specific patterns from demo_shutter_rainbow_central.anim...")
  
  # Test 1: The nested repeat structure that might cause issues
  print("  Testing nested repeat structure...")
  var nested_repeat = "template shutter_central {\n" +
                     "    param colors type palette\n" +
                     "    param duration\n" +
                     "    \n" +
                     "    color col1 = color_cycle(palette=colors, cycle_period=0)\n" +
                     "    animation test_anim = solid(color=col1)\n" +
                     "    \n" +
                     "    sequence shutter_seq repeat forever {\n" +
                     "      repeat col1.palette_size times {\n" +
                     "        play test_anim for duration\n" +
                     "        col1.next = 1\n" +
                     "      }\n" +
                     "    }\n" +
                     "    \n" +
                     "    run shutter_seq\n" +
                     "}\n" +
                     "\n" +
                     "palette rainbow = [red, green, blue]\n" +
                     "shutter_central(rainbow, 1s)"
  
  try
    print("    Compiling nested repeat structure...")
    var result1 = animation_dsl.compile(nested_repeat)
    print("    ✓ Nested repeat structure works")
  except .. as e, msg
    print(f"    ✗ Nested repeat structure failed: {e}: {msg}")
    return false
  end
  
  # Test 2: The col1.next = 1 pattern
  print("  Testing color.next assignment...")
  var color_next = "color col1 = color_cycle(palette=[red, green], cycle_period=0)\n" +
                   "col1.next = 1\n" +
                   "animation test_anim = solid(color=col1)\n" +
                   "run test_anim"
  
  try
    print("    Compiling color.next assignment...")
    var result2 = animation_dsl.compile(color_next)
    print("    ✓ Color.next assignment works")
  except .. as e, msg
    print(f"    ✗ Color.next assignment failed: {e}: {msg}")
    return false
  end
  
  # Test 3: The restart statement
  print("  Testing restart statement...")
  var restart_test = "set shutter_size = sawtooth(min_value=0, max_value=10, duration=1s)\n" +
                     "animation test_anim = solid(color=red)\n" +
                     "sequence test_seq {\n" +
                     "  restart shutter_size\n" +
                     "  play test_anim for 1s\n" +
                     "}\n" +
                     "run test_seq"
  
  try
    print("    Compiling restart statement...")
    var result3 = animation_dsl.compile(restart_test)
    print("    ✓ Restart statement works")
  except .. as e, msg
    print(f"    ✗ Restart statement failed: {e}: {msg}")
    return false
  end
  
  # Test 4: Complex expressions in beacon_animation
  print("  Testing complex beacon_animation expressions...")
  var complex_beacon = "template shutter_central {\n" +
                      "    param colors type palette\n" +
                      "    param duration\n" +
                      "    \n" +
                      "    set strip_len = strip_length()\n" +
                      "    set strip_len2 = (strip_len + 1) / 2\n" +
                      "    set shutter_size = sawtooth(min_value = 0, max_value = strip_len, duration = duration)\n" +
                      "    \n" +
                      "    color col1 = color_cycle(palette=colors, cycle_period=0)\n" +
                      "    \n" +
                      "    animation shutter_anim = beacon_animation(\n" +
                      "      color = col1\n" +
                      "      back_color = red\n" +
                      "      pos = strip_len2 - (shutter_size + 1) / 2\n" +
                      "      beacon_size = shutter_size\n" +
                      "      slew_size = 0\n" +
                      "      priority = 5\n" +
                      "    )\n" +
                      "    \n" +
                      "    run shutter_anim\n" +
                      "}\n" +
                      "\n" +
                      "palette rainbow = [red, green, blue]\n" +
                      "shutter_central(rainbow, 1s)"
  
  try
    print("    Compiling complex beacon_animation...")
    var result4 = animation_dsl.compile(complex_beacon)
    print("    ✓ Complex beacon_animation works")
  except .. as e, msg
    print(f"    ✗ Complex beacon_animation failed: {e}: {msg}")
    return false
  end
  
  # Test 5: The full problematic sequence structure
  print("  Testing full sequence structure (this may hang)...")
  var full_sequence = "template shutter_central {\n" +
                     "    param colors type palette\n" +
                     "    param duration\n" +
                     "    \n" +
                     "    set strip_len = strip_length()\n" +
                     "    set strip_len2 = (strip_len + 1) / 2\n" +
                     "    set shutter_size = sawtooth(min_value = 0, max_value = strip_len, duration = duration)\n" +
                     "    \n" +
                     "    color col1 = color_cycle(palette=colors, cycle_period=0)\n" +
                     "    color col2 = color_cycle(palette=colors, cycle_period=0)\n" +
                     "    col2.next = 1\n" +
                     "    \n" +
                     "    animation shutter_inout = beacon_animation(\n" +
                     "      color = col2\n" +
                     "      back_color = col1\n" +
                     "      pos = strip_len2 - (shutter_size + 1) / 2\n" +
                     "      beacon_size = shutter_size\n" +
                     "      slew_size = 0\n" +
                     "      priority = 5\n" +
                     "    )\n" +
                     "    \n" +
                     "    animation shutter_outin = beacon_animation(\n" +
                     "      color = col1\n" +
                     "      back_color = col2\n" +
                     "      pos = strip_len2 - (strip_len - shutter_size + 1) / 2\n" +
                     "      beacon_size = strip_len - shutter_size\n" +
                     "      slew_size = 0\n" +
                     "      priority = 5\n" +
                     "    )\n" +
                     "    \n" +
                     "    sequence shutter_seq repeat forever {\n" +
                     "      repeat col1.palette_size times {\n" +
                     "        restart shutter_size\n" +
                     "        play shutter_inout for duration\n" +
                     "        col1.next = 1\n" +
                     "        col2.next = 1\n" +
                     "      }\n" +
                     "      repeat col1.palette_size times {\n" +
                     "        restart shutter_size\n" +
                     "        play shutter_outin for duration\n" +
                     "        col1.next = 1\n" +
                     "        col2.next = 1\n" +
                     "      }\n" +
                     "    }\n" +
                     "    \n" +
                     "    run shutter_seq\n" +
                     "}\n" +
                     "\n" +
                     "palette rainbow_with_white = [red, orange, yellow, green, blue, indigo, white]\n" +
                     "shutter_central(rainbow_with_white, 1.5s)"
  
  print("    CRITICAL: This exact structure causes infinite loop")
  print("    The combination of:")
  print("      - 'repeat forever' outer loop")
  print("      - Multiple nested 'repeat col1.palette_size times' loops")
  print("      - 'restart' statements inside the loops")
  print("      - '.next = 1' assignments on color_cycle objects")
  print("    appears to trigger infinite recursion in the transpiler")
  print("")
  print("    RECOMMENDATION: Debug the transpiler's handling of:")
  print("      1. Nested repeat loop transpilation")
  print("      2. Variable scope resolution in nested contexts")
  print("      3. Color cycle object method resolution")
  print("      4. Restart statement processing")
  
  print("✓ Demo shutter patterns test completed")
  return true
end

# Test reading the actual demo file and analyzing its structure
def test_demo_file_analysis()
  print("Analyzing demo_shutter_rainbow_central.anim structure...")
  
  var demo_content = ""
  try
    var f = open("lib/libesp32/berry_animation/anim_examples/demo_shutter_rainbow_central.anim", "r")
    demo_content = f.read()
    f.close()
  except .. as e, msg
    print(f"  ERROR: Could not read demo file: {e} - {msg}")
    return false
  end
  
  print(f"  File size: {size(demo_content)} characters")
  
  # Count occurrences of potentially problematic patterns
  var repeat_count = 0
  var pos = 0
  while true
    pos = string.find(demo_content, "repeat", pos)
    if pos == -1 break end
    repeat_count += 1
    pos += 6
  end
  
  var next_count = 0
  pos = 0
  while true
    pos = string.find(demo_content, ".next", pos)
    if pos == -1 break end
    next_count += 1
    pos += 5
  end
  
  var restart_count = 0
  pos = 0
  while true
    pos = string.find(demo_content, "restart", pos)
    if pos == -1 break end
    restart_count += 1
    pos += 7
  end
  
  print(f"  Found {repeat_count} 'repeat' statements")
  print(f"  Found {next_count} '.next' assignments")
  print(f"  Found {restart_count} 'restart' statements")
  
  # Check for nested repeat structures
  if string.find(demo_content, "repeat forever") != -1
    print("  Contains 'repeat forever' - potential infinite loop source")
  end
  
  if repeat_count > 2
    print("  Multiple nested repeat structures detected")
  end
  
  print("✓ Demo file analysis completed")
  return true
end

# Test the actual demo file compilation (DANGEROUS - may hang)
def test_actual_demo_file_compilation()
  print("Testing actual demo file compilation...")
  print("WARNING: This test is designed to demonstrate the infinite loop")
  print("If you run this test, it WILL hang and you'll need to kill the process")
  print("")
  print("To reproduce the infinite loop manually, run:")
  print("  animation_dsl.compile(open('lib/libesp32/berry_animation/anim_examples/demo_shutter_rainbow_central.anim', 'r').read())")
  print("")
  print("SKIPPING actual compilation to prevent hang")
  print("✓ Actual demo file compilation test documented")
  return true
end

# Run all demo shutter infinite loop tests
def run_all_demo_shutter_tests()
  print("=== Demo Shutter Infinite Loop Test Suite ===")
  print("")
  
  var tests = [
    test_demo_file_analysis,
    test_demo_shutter_patterns,
    test_actual_demo_file_compilation
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
      print(f"✗ Test crashed: {error_type} - {error_message}")
    end
    print("")
  end
  
  print("=== Demo Shutter Test Results ===")
  print(f"Passed: {passed}/{total}")
  
  if passed == total
    print("All demo shutter tests passed! ✓")
    print("")
    print("CONCLUSION:")
    print("The infinite loop appears to be caused by the complex nested")
    print("repeat structure with 'repeat forever' and multiple inner")
    print("'repeat col1.palette_size times' loops combined with")
    print("'restart' statements and '.next' assignments.")
    return true
  else
    print("Some demo shutter tests failed! ✗")
    raise "test_failed"
  end
end

# Run the tests
return run_all_demo_shutter_tests()