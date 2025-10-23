# DSL Berry Code Blocks Integration Test Suite
# Tests for real-world integration scenarios with berry code blocks
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation/src -e "import tasmota" lib/libesp32/berry_animation/src/tests/dsl_berry_integration_test.be

import animation
import animation_dsl
import string

# Test berry code blocks with mathematical calculations
def test_mathematical_integration()
  print("Testing berry code blocks with mathematical calculations...")
  
  var dsl_source = 'berry """\n' +
    'import math\n' +
    'def calculate_period(base_period, frequency)\n' +
    '  return int(base_period / frequency)\n' +
    'end\n' +
    'def calculate_opacity(brightness_percent)\n' +
    '  return int(brightness_percent * 2.55)\n' +
    'end\n' +
    '"""\n' +
    'color wave_color = 0x0080FF\n' +
    'animation wave1 = pulsating_animation(color=wave_color, period=2s)\n' +
    'animation wave2 = pulsating_animation(color=wave_color, period=3s)\n' +
    'berry """\n' +
    'wave1_.period = calculate_period(4000, 2.0)  # 2000ms\n' +
    'wave1_.opacity = calculate_opacity(80)       # 204\n' +
    'wave2_.period = calculate_period(6000, 1.5)  # 4000ms\n' +
    'wave2_.opacity = calculate_opacity(60)       # 153\n' +
    '"""\n' +
    'run wave1\n' +
    'run wave2'
  
  # Test compilation
  var berry_code = animation_dsl.compile(dsl_source)
  assert(berry_code != nil, "Should compile successfully")
  
  # Verify mathematical functions are included
  assert(string.find(berry_code, "def calculate_period(base_period, frequency)") >= 0, "Should include period calculation function")
  assert(string.find(berry_code, "def calculate_opacity(brightness_percent)") >= 0, "Should include opacity calculation function")
  
  # Verify function calls are included
  assert(string.find(berry_code, "wave1_.period = calculate_period(4000, 2.0)") >= 0, "Should include period calculation call")
  assert(string.find(berry_code, "wave1_.opacity = calculate_opacity(80)") >= 0, "Should include opacity calculation call")
  
  # Test execution
  try
    animation_dsl.execute(dsl_source)
    print("✓ Mathematical integration test passed")
    return true
  except .. as e
    print("✗ Mathematical integration test failed:", e)
    return false
  end
end

# Test berry code blocks with configuration management
def test_configuration_management()
  print("Testing berry code blocks with configuration management...")
  
  var dsl_source = 'berry """\n' +
    '# Global configuration object\n' +
    'var config = {\n' +
    '  "brightness": 200,\n' +
    '  "speed_multiplier": 1.5,\n' +
    '  "color_intensity": 0.8,\n' +
    '  "debug": true\n' +
    '}\n' +
    'def log_debug(message)\n' +
    '  if config["debug"]\n' +
    '    print("[DEBUG]", message)\n' +
    '  end\n' +
    'end\n' +
    'def apply_config_to_animation(anim, name)\n' +
    '  anim.opacity = config["brightness"]\n' +
    '  log_debug("Applied config to " + name)\n' +
    'end\n' +
    '"""\n' +
    'color fire_red = 0xFF4500\n' +
    'color ocean_blue = 0x006994\n' +
    'animation fire_anim = pulsating_animation(color=fire_red, period=2s)\n' +
    'animation ocean_anim = pulsating_animation(color=ocean_blue, period=3s)\n' +
    'berry """\n' +
    'apply_config_to_animation(fire_anim_, "fire_animation")\n' +
    'apply_config_to_animation(ocean_anim_, "ocean_animation")\n' +
    'log_debug("Configuration applied to all animations")\n' +
    '"""\n' +
    'run fire_anim\n' +
    'run ocean_anim'
  
  # Test compilation
  var berry_code = animation_dsl.compile(dsl_source)
  assert(berry_code != nil, "Should compile successfully")
  
  # Verify configuration object is included
  assert(string.find(berry_code, 'var config = {') >= 0, "Should include config object")
  assert(string.find(berry_code, '"brightness": 200,') >= 0, "Should include brightness setting")
  
  # Verify helper functions are included
  assert(string.find(berry_code, "def log_debug(message)") >= 0, "Should include debug function")
  assert(string.find(berry_code, "def apply_config_to_animation(anim, name)") >= 0, "Should include config application function")
  
  # Test execution
  try
    animation_dsl.execute(dsl_source)
    print("✓ Configuration management test passed")
    return true
  except .. as e
    print("✗ Configuration management test failed:", e)
    return false
  end
end

# Test berry code blocks with dynamic animation creation
def test_dynamic_animation_creation()
  print("Testing berry code blocks with dynamic animation creation...")
  
  var dsl_source = 'berry """\n' +
    'var animation_counter = 0\n' +
    'def create_numbered_animation(base_color, period_ms)\n' +
    '  animation_counter += 1\n' +
    '  var anim = animation.pulsating_animation(engine)\n' +
    '  anim.color = base_color\n' +
    '  anim.period = period_ms\n' +
    '  anim.priority = animation_counter\n' +
    '  engine.add(anim)\n' +
    '  print("Created animation #" + str(animation_counter))\n' +
    '  return anim\n' +
    'end\n' +
    '"""\n' +
    'color red_custom = 0xFF0000\n' +
    'color green_custom = 0x00FF00\n' +
    'color blue_custom = 0x0000FF\n' +
    'berry """\n' +
    'create_numbered_animation(0xFFFF0000, 2000)  # Red\n' +
    'create_numbered_animation(0xFF00FF00, 2500)  # Green\n' +
    'create_numbered_animation(0xFF0000FF, 3000)  # Blue\n' +
    'print("Created " + str(animation_counter) + " dynamic animations")\n' +
    '"""'
  
  # Test compilation
  var berry_code = animation_dsl.compile(dsl_source)
  assert(berry_code != nil, "Should compile successfully")
  
  # Verify dynamic creation function is included
  assert(string.find(berry_code, "def create_numbered_animation(base_color, period_ms)") >= 0, "Should include dynamic creation function")
  assert(string.find(berry_code, "var anim = animation.pulsating_animation(engine)") >= 0, "Should create animations dynamically")
  assert(string.find(berry_code, "engine.add(anim)") >= 0, "Should add animations to engine")
  
  # Test execution
  try
    animation_dsl.execute(dsl_source)
    print("✓ Dynamic animation creation test passed")
    return true
  except .. as e
    print("✗ Dynamic animation creation test failed:", e)
    return false
  end
end

# Test berry code blocks with state management
def test_state_management()
  print("Testing berry code blocks with state management...")
  
  var dsl_source = 'berry """\n' +
    '# State management system\n' +
    'var state = {\n' +
    '  "current_mode": "normal",\n' +
    '  "brightness_level": 255,\n' +
    '  "animation_count": 0\n' +
    '}\n' +
    'def set_mode(mode)\n' +
    '  state["current_mode"] = mode\n' +
    '  print("Mode changed to:", mode)\n' +
    'end\n' +
    'def get_brightness_for_mode()\n' +
    '  if state["current_mode"] == "dim"\n' +
    '    return 100\n' +
    '  elif state["current_mode"] == "bright"\n' +
    '    return 255\n' +
    '  else\n' +
    '    return 180\n' +
    '  end\n' +
    'end\n' +
    '"""\n' +
    'color status_color = 0x00FFFF\n' +
    'animation status_anim = pulsating_animation(color=status_color, period=2s)\n' +
    'berry """\n' +
    'set_mode("bright")\n' +
    'status_anim_.opacity = get_brightness_for_mode()\n' +
    'state["animation_count"] = 1\n' +
    'print("State:", state)\n' +
    '"""\n' +
    'run status_anim'
  
  # Test compilation
  var berry_code = animation_dsl.compile(dsl_source)
  assert(berry_code != nil, "Should compile successfully")
  
  # Verify state management is included
  assert(string.find(berry_code, 'var state = {') >= 0, "Should include state object")
  assert(string.find(berry_code, "def set_mode(mode)") >= 0, "Should include mode setter")
  assert(string.find(berry_code, "def get_brightness_for_mode()") >= 0, "Should include brightness getter")
  
  # Test execution
  try
    animation_dsl.execute(dsl_source)
    print("✓ State management test passed")
    return true
  except .. as e
    print("✗ State management test failed:", e)
    return false
  end
end

# Test berry code blocks with error handling
def test_error_handling_integration()
  print("Testing berry code blocks with error handling...")
  
  var dsl_source = 'berry """\n' +
    'def safe_divide(a, b)\n' +
    '  try\n' +
    '    return a / b\n' +
    '  except .. as e\n' +
    '    print("Division error:", e)\n' +
    '    return 1.0\n' +
    '  end\n' +
    'end\n' +
    'def safe_set_period(anim, period)\n' +
    '  try\n' +
    '    if period > 0\n' +
    '      anim.period = period\n' +
    '      print("Period set to:", period)\n' +
    '    else\n' +
    '      print("Invalid period, using default")\n' +
    '      anim.period = 2000\n' +
    '    end\n' +
    '  except .. as e\n' +
    '    print("Error setting period:", e)\n' +
    '  end\n' +
    'end\n' +
    '"""\n' +
    'color safe_color = 0xFF8000\n' +
    'animation safe_anim = pulsating_animation(color=safe_color, period=1s)\n' +
    'berry """\n' +
    'var calculated_period = safe_divide(4000, 2)  # Should work\n' +
    'safe_set_period(safe_anim_, int(calculated_period * 1000))\n' +
    '"""\n' +
    'run safe_anim'
  
  # Test compilation
  var berry_code = animation_dsl.compile(dsl_source)
  assert(berry_code != nil, "Should compile successfully")
  
  # Verify error handling functions are included
  assert(string.find(berry_code, "def safe_divide(a, b)") >= 0, "Should include safe division function")
  assert(string.find(berry_code, "def safe_set_period(anim, period)") >= 0, "Should include safe period setter")
  assert(string.find(berry_code, "try") >= 0, "Should include try-catch blocks")
  
  # Test execution
  try
    animation_dsl.execute(dsl_source)
    print("✓ Error handling integration test passed")
    return true
  except .. as e
    print("✗ Error handling integration test failed:", e)
    return false
  end
end

# Run all integration tests
def run_all_berry_integration_tests()
  print("=== DSL Berry Code Blocks Integration Test Suite ===")
  print("")
  
  var tests = [
    test_mathematical_integration,
    test_configuration_management,
    test_dynamic_animation_creation,
    test_state_management,
    test_error_handling_integration
  ]
  
  var passed = 0
  var total = size(tests)
  
  for test_func : tests
    try
      if test_func()
        passed += 1
      end
    except .. as e
      print("✗ Test failed with exception:", e)
    end
    print("")
  end
  
  print("=== Berry Integration Test Results ===")
  print(f"Passed: {passed}/{total}")
  
  if passed == total
    print("All berry integration tests passed! ✓")
    return true
  else
    print("Some berry integration tests failed! ✗")
    return false
  end
end

# Run the tests
return run_all_berry_integration_tests()