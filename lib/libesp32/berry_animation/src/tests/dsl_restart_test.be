# DSL Restart Test Suite
# Tests for restart functionality in sequences
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation/src -e "import tasmota def log(x,l) tasmota.log(x,l) end" lib/libesp32/berry_animation/src/tests/dsl_reset_restart_test.be

import animation
import animation_dsl
import string

# Test basic restart functionality
def test_restart_basic()
  print("Testing basic restart functionality...")
  
  var dsl_source = "set smooth_val = smooth(min_value=5, max_value=15, duration=3s)\n" +
    "animation test_anim = solid(color=blue)\n" +
    "\n" +
    "sequence demo {\n" +
    "  play test_anim for 1s\n" +
    "  restart smooth_val\n" +
    "  play test_anim for 1s\n" +
    "}\n" +
    "\n" +
    "run demo"
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should generate Berry code for restart")
  assert(string.find(berry_code, "animation.smooth(engine)") >= 0, "Should generate smooth oscillator")
  assert(string.find(berry_code, "push_closure_step") >= 0, "Should generate closure step for restart")
  assert(string.find(berry_code, "smooth_val_.start(engine.time_ms)") >= 0, "Should call start() method")
  
  print("✓ Basic restart test passed")
  return true
end

# Test restart with different value provider types
def test_restart_different_providers()
  print("Testing restart with different value provider types...")
  
  var dsl_source = "set triangle_val = triangle(min_value=0, max_value=29, duration=5s)\n" +
    "set cosine_val = cosine_osc(min_value=0, max_value=29, duration=5s)\n" +
    "set sine_val = sine_osc(min_value=0, max_value=255, duration=2s)\n" +
    "animation test_anim = solid(color=green)\n" +
    "\n" +
    "sequence demo {\n" +
    "  play test_anim for 500ms\n" +
    "  restart triangle_val\n" +
    "  wait 200ms\n" +
    "  restart cosine_val\n" +
    "  wait 200ms\n" +
    "  restart sine_val\n" +
    "  play test_anim for 500ms\n" +
    "}\n" +
    "\n" +
    "run demo"
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should generate Berry code for multiple providers")
  assert(string.find(berry_code, "triangle_val_.start(engine.time_ms)") >= 0, "Should restart triangle")
  assert(string.find(berry_code, "cosine_val_.start(engine.time_ms)") >= 0, "Should restart cosine")
  assert(string.find(berry_code, "sine_val_.start(engine.time_ms)") >= 0, "Should restart sine")
  
  # Count the number of closure steps - should be 3 (one for each restart)
  var closure_count = 0
  var pos = 0
  while true
    pos = string.find(berry_code, "push_closure_step", pos)
    if pos < 0 break end
    closure_count += 1
    pos += 1
  end
  assert(closure_count == 3, f"Should have 3 closure steps for estart, found {closure_count}")
  
  print("✓ Different providers test passed")
  return true
end

# Test restart with animations
def test_restart_animations()
  print("Testing restart with animations...")
  
  var dsl_source = "set osc_val = triangle(min_value=0, max_value=10, duration=2s)\n" +
    "animation pulse_anim = pulsating_animation(color=red, period=3s)\n" +
    "animation solid_anim = solid(color=blue)\n" +
    "\n" +
    "sequence demo {\n" +
    "  play pulse_anim for 1s\n" +
    "  restart pulse_anim\n" +
    "  play solid_anim for 1s\n" +
    "  restart solid_anim\n" +
    "  play pulse_anim for 1s\n" +
    "}\n" +
    "\n" +
    "run demo"
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should generate Berry code for animation restart")
  assert(string.find(berry_code, "pulse_anim_.start(engine.time_ms)") >= 0, "Should restart pulse animation")
  assert(string.find(berry_code, "solid_anim_.start(engine.time_ms)") >= 0, "Should restart solid animation")
  
  # Count the number of closure steps - should be 2 (one for each restart)
  var closure_count = 0
  var pos = 0
  while true
    pos = string.find(berry_code, "push_closure_step", pos)
    if pos < 0 break end
    closure_count += 1
    pos += 1
  end
  assert(closure_count == 2, f"Should have 2 closure steps for animation restart, found {closure_count}")
  
  print("✓ Animation restart test passed")
  return true
end

# Test restart in repeat blocks
def test_restart_in_repeat()
  print("Testing restart in repeat blocks...")
  
  var dsl_source = "set osc_val = triangle(min_value=0, max_value=10, duration=1s)\n" +
    "animation test_anim = solid(color=yellow)\n" +
    "\n" +
    "sequence demo {\n" +
    "  repeat 3 times {\n" +
    "    play test_anim for 500ms\n" +
    "    restart osc_val\n" +
    "    wait 200ms\n" +
    "  }\n" +
    "}\n" +
    "\n" +
    "run demo"
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should generate Berry code for restart in repeat")
  assert(string.find(berry_code, "push_repeat_subsequence") >= 0, "Should generate repeat block")
  assert(string.find(berry_code, "osc_val_.start(engine.time_ms)") >= 0, "Should restart in repeat block")
  
  print("✓ Restart in repeat test passed")
  return true
end

# Test error handling - undefined value provider
def test_error_undefined_provider()
  print("Testing error handling for undefined value provider...")
  
  var dsl_source = "animation test_anim = solid(color=red)\n" +
    "\n" +
    "sequence demo {\n" +
    "  play test_anim for 1s\n" +
    "  restart undefined_provider\n" +
    "}\n" +
    "\n" +
    "run demo"
  
  var berry_code = nil
  try
    berry_code = animation_dsl.compile(dsl_source)
    assert(false, "Should fail with undefined provider error")
  except "dsl_compilation_error" as e, msg
    assert(string.find(msg, "Undefined reference 'undefined_provider'") >= 0, "Should report undefined reference error")
  end
  
  print("✓ Undefined provider error test passed")
  return true
end

# Test error handling - non-value provider
def test_error_non_value_provider()
  print("Testing error handling for non-value provider...")
  
  var dsl_source = "color my_color = 0xFF0000\n" +
    "animation test_anim = solid(color=red)\n" +
    "\n" +
    "sequence demo {\n" +
    "  play test_anim for 1s\n" +
    "  restart my_color\n" +
    "}\n" +
    "\n" +
    "run demo"
  
  var berry_code = nil
  try
    berry_code = animation_dsl.compile(dsl_source)
    assert(false, "Should fail with non-value provider error")
  except "dsl_compilation_error" as e, msg
    assert(string.find(msg, "is not a value provider") >= 0, "Should report non-value provider error")
  end
  
  print("✓ Non-value provider error test passed")
  return true
end

# Test error handling - animation instead of value provider
def test_error_animation_not_provider()
  print("Testing error handling for animation instead of value provider...")
  
  var dsl_source = "animation my_anim = solid(color=blue)\n" +
    "\n" +
    "set myvar = 2\n"
    "sequence demo {\n" +
    "  play my_anim for 1s\n" +
    "  restart myvar\n" +
    "}\n" +
    "\n" +
    "run demo"
  
  var berry_code = nil
  try
    berry_code = animation_dsl.compile(dsl_source)
    assert(false, "Should fail with animation not value provider error")
  except "dsl_compilation_error" as e, msg
    assert(string.find(msg, "is not a value provider") >= 0, "Should report animation not value provider error")
  end
  
  print("✓ Animation not provider error test passed")
  return true
end

# Test error handling - variable instead of value provider
def test_error_variable_not_provider()
  print("Testing error handling for variable instead of value provider...")
  
  var dsl_source = "set my_var = 100\n" +
    "animation test_anim = solid(color=green)\n" +
    "\n" +
    "sequence demo {\n" +
    "  play test_anim for 1s\n" +
    "  restart my_var\n" +
    "}\n" +
    "\n" +
    "run demo"
  
  var berry_code = nil
  try
    berry_code = animation_dsl.compile(dsl_source)
    assert(false, "Should fail with variable not value provider error")
  except "dsl_compilation_error" as e, msg
    assert(string.find(msg, "is not a value provider") >= 0, "Should report variable not value provider error")
  end
  
  print("✓ Variable not provider error test passed")
  return true
end

# Test complex scenario with multiple restarts
def test_complex_scenario()
  print("Testing complex scenario with multiple restarts...")
  
  var dsl_source = "# Complex cylon eye with restart functionality\n" +
    "set strip_len = strip_length()\n" +
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
    "  restart triangle_val\n" +
    "  play red_eye for 3s\n" +
    "  red_eye.pos = cosine_val\n" +
    "  restart cosine_val\n" +
    "  eye_color.next = 1\n" +
    "}\n" +
    "\n" +
    "run cylon_eye"
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should compile complex scenario")
  assert(string.find(berry_code, "triangle_val_.start(engine.time_ms)") >= 0, "Should restart triangle_val")
  assert(string.find(berry_code, "cosine_val_.start(engine.time_ms)") >= 0, "Should restart cosine_val")
  
  # Should have multiple closure steps: 2 assignments + 2 restarts + 1 color advance = 5 total
  var closure_count = 0
  var pos = 0
  while true
    pos = string.find(berry_code, "push_closure_step", pos)
    if pos < 0 break end
    closure_count += 1
    pos += 1
  end
  assert(closure_count == 5, f"Should have 5 closure steps in complex scenario, found {closure_count}")
  
  print("✓ Complex scenario test passed")
  return true
end

# Test that restart works with comments
def test_restart_with_comments()
  print("Testing restart with comments...")
  
  var dsl_source = "set osc_val = triangle(min_value=0, max_value=10, duration=2s)  # Triangle oscillator\n" +
    "animation test_anim = solid(color=red)\n" +
    "\n" +
    "sequence demo {\n" +
    "  play test_anim for 1s\n" +
    "  restart osc_val  # Restart the oscillator\n" +
    "  restart osc_val  # Restart it again\n" +
    "  play test_anim for 1s\n" +
    "}\n" +
    "\n" +
    "run demo"
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should generate Berry code with comments")
  assert(string.find(berry_code, "# Restart the oscillator") >= 0, "Should preserve restart comment")
  assert(string.find(berry_code, "# Restart it again") >= 0, "Should preserve restart comment")
  
  # Should have 2 closure steps for restart
  var closure_count = 0
  var pos = 0
  while true
    pos = string.find(berry_code, "push_closure_step", pos)
    if pos < 0 break end
    closure_count += 1
    pos += 1
  end
  assert(closure_count == 2, f"Should have 2 closure steps, found {closure_count}")
  
  print("✓ Restart with comments test passed")
  return true
end

# Run all tests
def run_all_restart_tests()
  print("Starting DSL Restart Tests...")
  
  test_restart_basic()
  test_restart_different_providers()
  test_restart_in_repeat()
  test_error_undefined_provider()
  test_error_non_value_provider()
  test_error_animation_not_provider()
  test_error_variable_not_provider()
  test_complex_scenario()
  test_restart_with_comments()
  
  print("\n🎉 All DSL Restart tests passed!")
  return true
end

# Execute tests
run_all_restart_tests()

return {
  "run_all_estart_tests": run_all_restart_tests,
  "test_restart_basic": test_restart_basic,
  "test_restart_different_providers": test_restart_different_providers,
  "test_restart_in_repeat": test_restart_in_repeat,
  "test_error_undefined_provider": test_error_undefined_provider,
  "test_error_non_value_provider": test_error_non_value_provider,
  "test_error_animation_not_provider": test_error_animation_not_provider,
  "test_error_variable_not_provider": test_error_variable_not_provider,
  "test_complex_scenario": test_complex_scenario,
  "test_restart_with_comments": test_restart_with_comments
}