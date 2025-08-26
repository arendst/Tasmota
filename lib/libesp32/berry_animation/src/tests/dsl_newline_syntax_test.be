# DSL Newline Syntax Test Suite
# Tests for the new flexible parameter syntax (commas optional on separate lines)
#
# Command to run test:
#    ./berry -s -g -m lib/libesp32/berry_animation/src -e "import tasmota" lib/libesp32/berry_animation/src/tests/dsl_newline_syntax_test.be

import animation
import animation_dsl
import string

# Test animation parameters with newline syntax
def test_animation_newline_parameters()
  print("Testing animation parameters with newlines...")
  
  var dsl_source = 
    "color custom_red = 0xFF0000\n" +
    "animation stream1 = comet_animation(\n" +
    "  color=custom_red\n" +
    "  tail_length=15\n" +
    "  speed=1.5s\n" +
    "  priority=10\n" +
    ")\n" +
    "run stream1"
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should compile DSL with newline parameters")
  assert(string.find(berry_code, "var stream1_ = animation.comet_animation(engine)") >= 0, "Should generate animation creation")
  assert(string.find(berry_code, "stream1_.color = custom_red_") >= 0, "Should generate color assignment")
  assert(string.find(berry_code, "stream1_.tail_length = 15") >= 0, "Should generate tail_length assignment")
  assert(string.find(berry_code, "stream1_.speed = 1500") >= 0, "Should generate speed assignment")
  assert(string.find(berry_code, "stream1_.priority = 10") >= 0, "Should generate priority assignment")
  
  print("✓ Animation newline parameters test passed")
  return true
end

# Test palette entries with newline syntax
def test_palette_newline_entries()
  print("Testing palette entries with newlines...")
  
  var dsl_source = 
    "palette matrix_greens = [\n" +
    "  (0, 0x000000)\n" +
    "  (64, 0x003300)\n" +
    "  (128, 0x006600)\n" +
    "  (192, 0x00AA00)\n" +
    "  (255, 0x00FF00)\n" +
    "]\n" +
    "color stream_color = rich_palette(palette=matrix_greens, cycle_period=2s)\n" +
    "animation stream = solid(color=stream_color)\n" +
    "run stream"
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should compile DSL with newline palette entries")
  assert(string.find(berry_code, "var matrix_greens_ = bytes(") >= 0, "Should generate palette bytes")
  
  print("✓ Palette newline entries test passed")
  return true
end

# Test mixed comma and newline syntax
def test_mixed_syntax()
  print("Testing mixed comma and newline syntax...")
  
  var dsl_source = 
    "color custom_red = 0xFF0000\n" +
    "animation mixed = comet_animation(\n" +
    "  color=custom_red, tail_length=15\n" +
    "  speed=1.5s\n" +
    "  priority=10, direction=1\n" +
    ")\n" +
    "run mixed"
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should compile DSL with mixed syntax")
  assert(string.find(berry_code, "mixed_.color = custom_red_") >= 0, "Should generate color assignment")
  assert(string.find(berry_code, "mixed_.tail_length = 15") >= 0, "Should generate tail_length assignment")
  assert(string.find(berry_code, "mixed_.speed = 1500") >= 0, "Should generate speed assignment")
  assert(string.find(berry_code, "mixed_.priority = 10") >= 0, "Should generate priority assignment")
  assert(string.find(berry_code, "mixed_.direction = 1") >= 0, "Should generate direction assignment")
  
  print("✓ Mixed syntax test passed")
  return true
end

# Test that traditional comma syntax still works
def test_traditional_comma_syntax()
  print("Testing traditional comma syntax still works...")
  
  var dsl_source = 
    "color custom_red = 0xFF0000\n" +
    "animation traditional = comet_animation(color=custom_red, tail_length=15, speed=1.5s, priority=10)\n" +
    "run traditional"
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should compile DSL with traditional comma syntax")
  assert(string.find(berry_code, "traditional_.color = custom_red_") >= 0, "Should generate color assignment")
  assert(string.find(berry_code, "traditional_.tail_length = 15") >= 0, "Should generate tail_length assignment")
  
  print("✓ Traditional comma syntax test passed")
  return true
end

# Test color provider with newline syntax
def test_color_provider_newline_syntax()
  print("Testing color provider with newline syntax...")
  
  var dsl_source = 
    "palette test_palette = [(0, 0x000000), (255, 0xFFFFFF)]\n" +
    "color dynamic_color = rich_palette(\n" +
    "  palette=test_palette\n" +
    "  cycle_period=2s\n" +
    "  transition_type=LINEAR\n" +
    "  brightness=255\n" +
    ")\n" +
    "animation test = solid(color=dynamic_color)\n" +
    "run test"
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should compile color provider with newline syntax")
  assert(string.find(berry_code, "var dynamic_color_ = animation.rich_palette(engine)") >= 0, "Should generate color provider creation")
  assert(string.find(berry_code, "dynamic_color_.palette = test_palette_") >= 0, "Should generate palette assignment")
  assert(string.find(berry_code, "dynamic_color_.cycle_period = 2000") >= 0, "Should generate cycle_period assignment")
  
  print("✓ Color provider newline syntax test passed")
  return true
end

# Test nested function calls with newline syntax
def test_nested_function_calls()
  print("Testing nested function calls with newline syntax...")
  
  var dsl_source = 
    "animation nested = pulsating_animation(\n" +
    "  color=solid(color=red)\n" +
    "  period=triangle(\n" +
    "    min_value=1000\n" +
    "    max_value=3000\n" +
    "    duration=10s\n" +
    "  )\n" +
    ")\n" +
    "run nested"
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should compile nested function calls with newline syntax")
  assert(string.find(berry_code, "var nested_ = animation.pulsating_animation(engine)") >= 0, "Should generate main animation")
  assert(string.find(berry_code, "nested_.color = (def (engine)") >= 0, "Should generate nested solid call as anonymous function")
  assert(string.find(berry_code, "nested_.period = (def (engine)") >= 0, "Should generate nested triangle call as anonymous function")
  assert(string.find(berry_code, "var provider = animation.solid(engine)") >= 0, "Should generate solid provider in anonymous function")
  assert(string.find(berry_code, "var provider = animation.triangle(engine)") >= 0, "Should generate triangle provider in anonymous function")
  
  print("✓ Nested function calls test passed")
  return true
end

# Test complex real-world example
def test_complex_example()
  print("Testing complex real-world example...")
  
  var dsl_source = 
    "# Matrix rain effect with newline syntax\n" +
    "palette matrix_greens = [\n" +
    "  (0, 0x000000)     # Black\n" +
    "  (64, 0x003300)    # Dark green\n" +
    "  (128, 0x006600)   # Medium green\n" +
    "  (255, 0x00FF00)   # Neon green\n" +
    "]\n" +
    "\n" +
    "color stream_pattern = rich_palette(\n" +
    "  palette=matrix_greens\n" +
    "  cycle_period=2s\n" +
    "  transition_type=LINEAR\n" +
    "  brightness=255\n" +
    ")\n" +
    "\n" +
    "animation stream = comet_animation(\n" +
    "  color=stream_pattern  # color source\n" +
    "  tail_length=15        # long tail\n" +
    "  speed=1.5s            # speed\n" +
    "  priority=10\n" +
    ")\n" +
    "\n" +
    "run stream"
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should compile complex real-world example")
  assert(string.find(berry_code, "var matrix_greens_ = bytes(") >= 0, "Should generate palette")
  assert(string.find(berry_code, "var stream_pattern_ = animation.rich_palette(engine)") >= 0, "Should generate color provider")
  assert(string.find(berry_code, "var stream_ = animation.comet_animation(engine)") >= 0, "Should generate animation")
  
  print("✓ Complex example test passed")
  return true
end

# Run all tests
def run_all_tests()
  print("=== DSL Newline Syntax Test Suite ===")
  
  var tests = [
    test_animation_newline_parameters,
    test_palette_newline_entries,
    test_mixed_syntax,
    test_traditional_comma_syntax,
    test_color_provider_newline_syntax,
    test_nested_function_calls,
    test_complex_example
  ]
  
  var passed = 0
  var total = size(tests)
  
  for test : tests
    try
      if test()
        passed += 1
      end
    except .. as e, msg
      print(f"❌ Test failed with exception: {msg}")
    end
    print("")
  end
  
  print(f"=== Results: {passed}/{total} tests passed ===")
  
  if passed == total
    print("🎉 All newline syntax tests passed!")
    return true
  else
    print("❌ Some tests failed")
    raise "failed_test"
  end
end

# Run the tests
run_all_tests()