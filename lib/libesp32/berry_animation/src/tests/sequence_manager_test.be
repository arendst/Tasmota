# Unit tests for the SequenceManager class
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/sequence_manager_test.be

import string
import animation
import global
import tasmota

def test_sequence_manager_basic()
  print("=== SequenceManager Basic Tests ===")
  
  # Test SequenceManager class exists
  assert(animation.SequenceManager != nil, "SequenceManager class should be defined")
  
  # Create strip and engine for testing
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  
  # Test initialization
  var seq_manager = animation.SequenceManager(engine)
  assert(seq_manager.engine == engine, "Engine should be set correctly")
  assert(seq_manager.steps != nil, "Steps list should be initialized")
  assert(seq_manager.steps.size() == 0, "Steps list should be empty initially")
  assert(seq_manager.step_index == 0, "Step index should be 0 initially")
  assert(seq_manager.is_running == false, "Sequence should not be running initially")
  
  print("✓ Basic initialization tests passed")
end

def test_sequence_manager_step_creation()
  print("=== SequenceManager Step Creation Tests ===")
  
  # Create test animation using new parameterized API
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  var color_provider = animation.static_color(engine)
  color_provider.color = 0xFFFF0000
  var test_anim = animation.solid(engine)
  test_anim.color = color_provider
  test_anim.priority = 0
  test_anim.duration = 0
  test_anim.loop = true
  test_anim.name = "test"
  
  # Test fluent interface step creation
  var seq_manager = animation.SequenceManager(engine)
  
  # Test push_play_step
  seq_manager.push_play_step(test_anim, 5000)
  assert(seq_manager.steps.size() == 1, "Should have one step after push_play_step")
  var play_step = seq_manager.steps[0]
  assert(play_step["type"] == "play", "Play step should have correct type")
  assert(play_step["animation"] == test_anim, "Play step should have correct animation")
  assert(play_step["duration"] == 5000, "Play step should have correct duration")
  
  # Test push_wait_step
  seq_manager.push_wait_step(2000)
  assert(seq_manager.steps.size() == 2, "Should have two steps after push_wait_step")
  var wait_step = seq_manager.steps[1]
  assert(wait_step["type"] == "wait", "Wait step should have correct type")
  assert(wait_step["duration"] == 2000, "Wait step should have correct duration")
  
  # Test push_closure_step
  var test_closure = def (engine) test_anim.opacity = 128 end
  seq_manager.push_closure_step(test_closure)
  assert(seq_manager.steps.size() == 3, "Should have three steps after push_closure_step")
  var assign_step = seq_manager.steps[2]
  assert(assign_step["type"] == "closure", "Assign step should have correct type")
  assert(assign_step["closure"] == test_closure, "Assign step should have correct closure")
  
  print("✓ Step creation tests passed")
end

def test_sequence_manager_execution()
  print("=== SequenceManager Execution Tests ===")
  
  # Create strip and engine
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  var seq_manager = animation.SequenceManager(engine)
  
  # Create test animations using new parameterized API
  var color_provider1 = animation.static_color(engine)
  color_provider1.color = 0xFFFF0000
  var anim1 = animation.solid(engine)
  anim1.color = color_provider1
  anim1.priority = 0
  anim1.duration = 0
  anim1.loop = true
  anim1.name = "anim1"
  
  var color_provider2 = animation.static_color(engine)
  color_provider2.color = 0xFF00FF00
  var anim2 = animation.solid(engine)
  anim2.color = color_provider2
  anim2.priority = 0
  anim2.duration = 0
  anim2.loop = true
  anim2.name = "anim2"
  
  # Create sequence using fluent interface
  seq_manager.push_play_step(anim1, 1000)
              .push_wait_step(500)
              .push_play_step(anim2, 2000)
  
  # Test sequence start
  tasmota.set_millis(10000)
  engine.run()  # Start the engine
  engine.on_tick(10000)  # Update engine time
  seq_manager.start()
  
  assert(seq_manager.is_running == true, "Sequence should be running after start")
  assert(seq_manager.steps.size() == 3, "Sequence should have 3 steps")
  assert(seq_manager.step_index == 0, "Should start at step 0")
  
  # Check that first animation was started
  assert(engine.size() == 1, "Engine should have 1 animation")
  
  print("✓ Sequence execution start tests passed")
end

def test_sequence_manager_timing()
  print("=== SequenceManager Timing Tests ===")
  
  # Create strip and engine
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  var seq_manager = animation.SequenceManager(engine)
  
  # Create test animation using new parameterized API
  var color_provider = animation.static_color(engine)
  color_provider.color = 0xFFFF0000
  var test_anim = animation.solid(engine)
  test_anim.color = color_provider
  test_anim.priority = 0
  test_anim.duration = 0
  test_anim.loop = true
  test_anim.name = "test"
  
  # Create simple sequence with timed steps using fluent interface
  seq_manager.push_play_step(test_anim, 1000)  # 1 second
              .push_wait_step(500)             # 0.5 seconds

  # Start sequence at time 20000
  tasmota.set_millis(20000)
  engine.add(seq_manager)
  engine.run()  # Start the engine
  engine.on_tick(20000)  # Update engine time
  
  # Update immediately - should still be on first step
  seq_manager.update(engine.time_ms)
  assert(seq_manager.step_index == 0, "Should still be on first step immediately")
  assert(seq_manager.is_running == true, "Sequence should still be running")

  # Update after 500ms - should still be on first step
  tasmota.set_millis(20500)
  engine.on_tick(20500)  # Update engine time
  seq_manager.update(engine.time_ms)
  assert(seq_manager.step_index == 0, "Should still be on first step after 500ms")

  # Update after 1000ms - should advance to second step (wait)
  tasmota.set_millis(21000)
  engine.on_tick(21000)  # Update engine time
  seq_manager.update(engine.time_ms)
  assert(seq_manager.step_index == 1, "Should advance to second step after 1000ms")
  
  # Update after additional 500ms - should complete sequence
  tasmota.set_millis(21500)
  engine.on_tick(21500)  # Update engine time
  seq_manager.update(engine.time_ms)
  assert(seq_manager.is_running == false, "Sequence should complete after all steps")
  
  print("✓ Timing tests passed")
end

def test_sequence_manager_step_info()
  print("=== SequenceManager Step Info Tests ===")
  
  # Create strip and engine
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  var seq_manager = animation.SequenceManager(engine)
  
  # Create test sequence using new parameterized API
  var color_provider = animation.static_color(engine)
  color_provider.color = 0xFFFF0000
  var test_anim = animation.solid(engine)
  test_anim.color = color_provider
  test_anim.priority = 0
  test_anim.duration = 0
  test_anim.loop = true
  test_anim.name = "test"
  # Create sequence using fluent interface
  seq_manager.push_play_step(test_anim, 2000)
              .push_wait_step(1000)
  
  # Start sequence
  tasmota.set_millis(30000)
  engine.add(seq_manager)
  engine.run()  # Start the engine
  engine.on_tick(30000)  # Update engine time
  
  print("✓ Step info tests passed")
end

def test_sequence_manager_stop()
  print("=== SequenceManager Stop Tests ===")
  
  # Create strip and engine
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  var seq_manager = animation.SequenceManager(engine)
  
  # Create test sequence using new parameterized API
  var color_provider = animation.static_color(engine)
  color_provider.color = 0xFFFF0000
  var test_anim = animation.solid(engine)
  test_anim.color = color_provider
  test_anim.priority = 0
  test_anim.duration = 0
  test_anim.loop = true
  test_anim.name = "test"
  # Create sequence using fluent interface
  seq_manager.push_play_step(test_anim, 5000)
  
  # Start sequence
  tasmota.set_millis(40000)
  engine.run()  # Start the engine
  engine.on_tick(40000)  # Update engine time
  seq_manager.start()
  assert(seq_manager.is_running == true, "Sequence should be running")
  
  # Stop sequence
  seq_manager.stop()
  assert(seq_manager.is_running == false, "Sequence should not be running after stop")
  assert(engine.size() == 0, "Engine should have no animations after stop")
  
  print("✓ Stop tests passed")
end

def test_sequence_manager_is_running()
  print("=== SequenceManager Running State Tests ===")
  
  # Create strip and engine
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  var seq_manager = animation.SequenceManager(engine)
  
  # Test initial state
  assert(seq_manager.is_sequence_running() == false, "Sequence should not be running initially")
  
  # Create and start sequence using new parameterized API
  var color_provider = animation.static_color(engine)
  color_provider.color = 0xFFFF0000
  var test_anim = animation.solid(engine)
  test_anim.color = color_provider
  test_anim.priority = 0
  test_anim.duration = 0
  test_anim.loop = true
  test_anim.name = "test"
  # Create sequence using fluent interface
  seq_manager.push_play_step(test_anim, 1000)
  
  tasmota.set_millis(50000)
  engine.add(seq_manager)
  engine.run()  # Start the engine
  engine.on_tick(50000)  # Update engine time
  assert(seq_manager.is_sequence_running() == true, "Sequence should be running after start")
  
  # Complete sequence
  tasmota.set_millis(51000)
  engine.on_tick(51000)  # Update engine time
  seq_manager.update(engine.time_ms)
  assert(seq_manager.is_sequence_running() == false, "Sequence should not be running after completion")
  
  print("✓ Running state tests passed")
end

def test_sequence_manager_assignment_steps()
  print("=== SequenceManager Assignment Steps Tests ===")
  
  # Create strip and engine
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  var seq_manager = animation.SequenceManager(engine)
  
  # Create test animation using new parameterized API
  var color_provider = animation.static_color(engine)
  color_provider.color = 0xFFFF0000
  var test_anim = animation.solid(engine)
  test_anim.color = color_provider
  test_anim.priority = 0
  test_anim.duration = 0
  test_anim.loop = true
  test_anim.name = "test"
  test_anim.opacity = 255  # Initial opacity
  
  # Create brightness value provider for assignment
  var brightness_provider = animation.static_value(engine)
  brightness_provider.value = 128
  
  # Create assignment closure that changes animation opacity
  var assignment_closure = def (engine) test_anim.opacity = brightness_provider.produce_value("value", engine.time_ms) end
  
  # Create sequence with assignment step using fluent interface
  seq_manager.push_play_step(test_anim, 500)           # Play for 0.5s
              .push_closure_step(assignment_closure)    # Assign new opacity
              .push_play_step(test_anim, 500)          # Play for another 0.5s
  
  # Start sequence
  tasmota.set_millis(80000)
  engine.add(seq_manager)
  engine.run()  # Start the engine
  engine.on_tick(80000)  # Update engine time
  
  # Verify initial state
  assert(seq_manager.is_running == true, "Sequence should be running")
  assert(seq_manager.step_index == 0, "Should start at step 0")
  assert(test_anim.opacity == 255, "Animation should have initial opacity")
  
  # Advance past assignment step (after 500ms)
  # Assignment steps are executed atomically and advance immediately
  tasmota.set_millis(80502)
  engine.on_tick(80502)  # Update engine time
  seq_manager.update(80502)
  assert(seq_manager.step_index == 2, "Should advance past assignment step immediately")
  assert(test_anim.opacity == 128, "Animation opacity should be changed by assignment")
  
  # Complete sequence (second play step should finish after 500ms more)
  tasmota.set_millis(81002)  # 80502 + 500ms = 81002
  engine.on_tick(81002)  # Update engine time
  seq_manager.update(81002)
  assert(seq_manager.is_running == false, "Sequence should complete")
  
  print("✓ Assignment steps tests passed")
end

def test_sequence_manager_complex_sequence()
  print("=== SequenceManager Complex Sequence Tests ===")
  
  # Create strip and engine
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  var seq_manager = animation.SequenceManager(engine)
  
  # Create multiple test animations using new parameterized API
  var red_provider = animation.static_color(engine)
  red_provider.color = 0xFFFF0000
  var red_anim = animation.solid(engine)
  red_anim.color = red_provider
  red_anim.priority = 0
  red_anim.duration = 0
  red_anim.loop = true
  red_anim.name = "red"
  
  var green_provider = animation.static_color(engine)
  green_provider.color = 0xFF00FF00
  var green_anim = animation.solid(engine)
  green_anim.color = green_provider
  green_anim.priority = 0
  green_anim.duration = 0
  green_anim.loop = true
  green_anim.name = "green"
  
  var blue_provider = animation.static_color(engine)
  blue_provider.color = 0xFF0000FF
  var blue_anim = animation.solid(engine)
  blue_anim.color = blue_provider
  blue_anim.priority = 0
  blue_anim.duration = 0
  blue_anim.loop = true
  blue_anim.name = "blue"
  
  # Create complex sequence using fluent interface
  seq_manager.push_play_step(red_anim, 1000)    # Play red for 1s
              .push_play_step(green_anim, 800)   # Play green for 0.8s
              .push_wait_step(200)              # Wait 0.2s
              .push_play_step(blue_anim, 1500)  # Play blue for 1.5s
  
  # Start sequence
  tasmota.set_millis(60000)
  engine.add(seq_manager)
  engine.run()  # Start the engine
  engine.on_tick(60000)  # Update engine time
  
  # Test sequence progression step by step
  
  # After 1000ms: red completes, should advance to green (step 1)
  tasmota.set_millis(61000)
  engine.on_tick(61000)  # Update engine time
  seq_manager.update(61000)
  assert(seq_manager.step_index == 1, "Should advance to step 1 (green) after red completes")
  assert(seq_manager.is_running == true, "Sequence should still be running")
  
  # After 1800ms: green completes, should advance to wait (step 2)
  tasmota.set_millis(61800)
  engine.on_tick(61800)  # Update engine time
  seq_manager.update(61800)
  assert(seq_manager.step_index == 2, "Should advance to step 2 (wait) after green completes")
  assert(seq_manager.is_running == true, "Sequence should still be running")
  
  # After 2000ms: wait completes, should advance to blue (step 3)
  tasmota.set_millis(62000)
  engine.on_tick(62000)  # Update engine time
  seq_manager.update(62000)
  assert(seq_manager.step_index == 3, "Should advance to step 3 (blue) after wait completes")
  assert(seq_manager.is_running == true, "Sequence should still be running")
  
  # After 3500ms: blue completes, sequence should complete (we removed stop steps)
  tasmota.set_millis(63500)
  engine.on_tick(63500)  # Update engine time
  seq_manager.update(63500)
  assert(seq_manager.is_running == false, "Complex sequence should complete after blue step")
  
  print("✓ Complex sequence tests passed")
end

def test_sequence_manager_integration()
  print("=== SequenceManager Integration Tests ===")
  
  # Create strip and engine
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  
  # Test engine integration
  var seq_manager = animation.SequenceManager(engine)
  engine.add(seq_manager)
  
  # Create test sequence using new parameterized API
  var color_provider = animation.static_color(engine)
  color_provider.color = 0xFFFF0000
  var test_anim = animation.solid(engine)
  test_anim.color = color_provider
  test_anim.priority = 0
  test_anim.duration = 0
  test_anim.loop = true
  test_anim.name = "test"
  # Create sequence using fluent interface
  seq_manager.push_play_step(test_anim, 1000)
  
  # Start sequence
  tasmota.set_millis(70000)
  engine.run()  # Start the engine
  engine.on_tick(70000)  # Update engine time
  
  # The engine should automatically start the sequence manager when engine.run() is called
  assert(seq_manager.is_running == true, "Sequence should be running after engine start")
  
  # Test that engine's on_tick calls sequence manager update
  # After 1 second, the sequence should complete
  tasmota.set_millis(71005)  # Add 5ms buffer for engine's minimum delta check
  engine.on_tick(71005)  # This should call seq_manager.update()
  
  # The sequence should complete after the 1-second duration
  assert(seq_manager.is_running == false, "Sequence should complete after 1 second duration")
  
  # Test engine cleanup
  engine.clear()
  assert(engine.sequence_managers.size() == 0, "Engine should clear sequence managers")
  
  print("✓ Integration tests passed")
end

def test_sequence_manager_parametric_repeat_counts()
  print("=== SequenceManager Parametric Repeat Count Tests ===")
  
  # Create strip and engine
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  
  # Test 1: Static repeat count (baseline)
  var static_repeat_count = 3
  var seq_manager1 = animation.SequenceManager(engine, static_repeat_count)
  
  # Test get_resolved_repeat_count with static number
  var resolved_count = seq_manager1.get_resolved_repeat_count()
  assert(resolved_count == 3, f"Static repeat count should resolve to 3, got {resolved_count}")
  
  # Test 2: Function-based repeat count (simulating col1.palette_size)
  var palette_size_function = def (engine) return 5 end  # Simulates a palette with 5 colors
  var seq_manager2 = animation.SequenceManager(engine, palette_size_function)
  
  # Test get_resolved_repeat_count with function
  resolved_count = seq_manager2.get_resolved_repeat_count()
  assert(resolved_count == 5, f"Function repeat count should resolve to 5, got {resolved_count}")
  
  # Test 3: Dynamic repeat count that changes over time
  var dynamic_counter = 0
  var dynamic_function = def (engine) 
    dynamic_counter += 1
    return dynamic_counter <= 1 ? 2 : 4  # First call returns 2, subsequent calls return 4
  end
  
  var seq_manager3 = animation.SequenceManager(engine, dynamic_function)
  var first_resolved = seq_manager3.get_resolved_repeat_count()
  var second_resolved = seq_manager3.get_resolved_repeat_count()
  assert(first_resolved == 2, f"First dynamic call should return 2, got {first_resolved}")
  assert(second_resolved == 4, f"Second dynamic call should return 4, got {second_resolved}")
  
  print("✓ Parametric repeat count tests passed")
end

def test_sequence_manager_repeat_execution_with_functions()
  print("=== SequenceManager Repeat Execution with Functions Tests ===")
  
  # Create strip and engine
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  
  # Create test animation
  var color_provider = animation.static_color(engine)
  color_provider.color = 0xFF00FF00
  var test_anim = animation.solid(engine)
  test_anim.color = color_provider
  test_anim.priority = 0
  test_anim.duration = 0
  test_anim.loop = true
  test_anim.name = "test_repeat"
  
  # Create a function that returns repeat count (simulating palette_size)
  var repeat_count_func = def (engine) return 3 end
  
  # Create sequence manager with function-based repeat count
  var seq_manager = animation.SequenceManager(engine, repeat_count_func)
  seq_manager.push_play_step(test_anim, 50)  # Short duration for testing
  
  # Verify repeat count is resolved correctly
  var resolved_count = seq_manager.get_resolved_repeat_count()
  assert(resolved_count == 3, f"Repeat count should resolve to 3, got {resolved_count}")
  
  # Test that the sequence manager accepts function-based repeat counts
  assert(type(seq_manager.repeat_count) == "function", "Repeat count should be stored as function")
  
  # Test that multiple calls to get_resolved_repeat_count work
  var second_resolved = seq_manager.get_resolved_repeat_count()
  assert(second_resolved == 3, f"Second resolution should also return 3, got {second_resolved}")
  
  # Test sequence execution with function-based repeat count
  tasmota.set_millis(90000)
  seq_manager.start(90000)
  assert(seq_manager.is_running == true, "Sequence should start running")
  assert(seq_manager.current_iteration == 0, "Should start at iteration 0")
  
  print("✓ Repeat execution with functions tests passed")
end

def test_sequence_manager_palette_size_simulation()
  print("=== SequenceManager Palette Size Simulation Tests ===")
  
  # Create strip and engine
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  
  # Simulate a color cycle with palette_size property (like col1.palette_size)
  var mock_color_cycle = {
    "palette_size": 5,  # Use smaller palette for simpler testing
    "current_index": 0
  }
  
  # Create function that accesses palette_size (simulating col1.palette_size)
  var palette_size_func = def (engine) return mock_color_cycle["palette_size"] end
  
  # Create closure that advances color cycle (simulating col1.next = 1)
  var advance_color_func = def (engine) 
    mock_color_cycle["current_index"] = (mock_color_cycle["current_index"] + 1) % mock_color_cycle["palette_size"]
  end
  
  # Create sequence similar to demo_shutter_rainbow.anim:
  # sequence shutter_seq repeat col1.palette_size times {
  #   play shutter_animation for duration
  #   col1.next = 1
  # }
  var seq_manager = animation.SequenceManager(engine, palette_size_func)
  seq_manager.push_closure_step(advance_color_func)  # Just test the closure execution
  
  # Test that repeat count is resolved correctly
  var resolved_count = seq_manager.get_resolved_repeat_count()
  assert(resolved_count == 5, f"Should resolve to palette size 5, got {resolved_count}")
  
  # Test that the closure function works
  var initial_index = mock_color_cycle["current_index"]
  advance_color_func(engine)
  assert(mock_color_cycle["current_index"] == (initial_index + 1) % 5, "Color should advance when closure is called")
  
  # Test that the function can be called multiple times
  var second_resolved = seq_manager.get_resolved_repeat_count()
  assert(second_resolved == 5, f"Second resolution should also return 5, got {second_resolved}")
  
  print("✓ Palette size simulation tests passed")
end

def test_sequence_manager_dynamic_repeat_changes()
  print("=== SequenceManager Dynamic Repeat Changes Tests ===")
  
  # Create strip and engine
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  
  # Create test animation
  var color_provider = animation.static_color(engine)
  color_provider.color = 0xFF0080FF
  var test_anim = animation.solid(engine)
  test_anim.color = color_provider
  test_anim.priority = 0
  test_anim.duration = 0
  test_anim.loop = true
  test_anim.name = "dynamic_test"
  
  # Create dynamic repeat count that changes based on external state
  var external_state = {"multiplier": 2}
  var dynamic_repeat_func = def (engine) 
    return external_state["multiplier"] * 2  # Returns 4 initially, can change
  end
  
  # Create sequence with dynamic repeat count
  var seq_manager = animation.SequenceManager(engine, dynamic_repeat_func)
  seq_manager.push_play_step(test_anim, 250)
  
  # Start sequence
  tasmota.set_millis(120000)
  engine.add(seq_manager)
  engine.run()
  engine.on_tick(120000)
  seq_manager.start(120000)
  
  # Test initial repeat count resolution
  var initial_count = seq_manager.get_resolved_repeat_count()
  assert(initial_count == 4, f"Initial repeat count should be 4, got {initial_count}")
  
  # Change external state mid-execution (simulating dynamic conditions)
  external_state["multiplier"] = 3
  
  # Test that new calls get updated count
  var updated_count = seq_manager.get_resolved_repeat_count()
  assert(updated_count == 6, f"Updated repeat count should be 6, got {updated_count}")
  
  # Test with function that depends on engine state
  var engine_dependent_func = def (engine) 
    # Simulating a function that depends on strip length or other engine properties
    return engine.strip != nil ? 3 : 1
  end
  
  var seq_manager2 = animation.SequenceManager(engine, engine_dependent_func)
  var engine_count = seq_manager2.get_resolved_repeat_count()
  assert(engine_count == 3, f"Engine-dependent count should be 3, got {engine_count}")
  
  print("✓ Dynamic repeat changes tests passed")
end

def test_sequence_manager_complex_parametric_scenario()
  print("=== SequenceManager Complex Parametric Scenario Tests ===")
  
  # Create strip and engine
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  
  # Simulate complex scenario with multiple parametric elements
  # Similar to a more complex version of demo_shutter_rainbow.anim
  
  # Mock palette and color cycle objects
  var rainbow_palette = {
    "colors": [0xFFFF0000, 0xFFFF8000, 0xFFFFFF00],  # Smaller palette for testing
    "size": 3
  }
  
  var color_cycle1 = {
    "palette": rainbow_palette,
    "current_index": 0,
    "palette_size": rainbow_palette["size"]
  }
  
  # Functions for parametric behavior
  var palette_size_func = def (engine) return color_cycle1["palette_size"] end
  var advance_colors_func = def (engine)
    color_cycle1["current_index"] = (color_cycle1["current_index"] + 1) % color_cycle1["palette_size"]
  end
  
  # Create sequence with parametric repeat
  var seq_manager = animation.SequenceManager(engine, palette_size_func)
  seq_manager.push_closure_step(advance_colors_func)
  
  # Verify sequence setup
  var resolved_count = seq_manager.get_resolved_repeat_count()
  assert(resolved_count == 3, f"Complex sequence should repeat 3 times, got {resolved_count}")
  
  # Test that the functions work correctly
  var initial_color_index = color_cycle1["current_index"]
  
  # Test closure execution
  advance_colors_func(engine)
  assert(color_cycle1["current_index"] == (initial_color_index + 1) % 3, "Color should advance")
  
  # Test multiple function calls
  var second_resolved = seq_manager.get_resolved_repeat_count()
  assert(second_resolved == 3, f"Second resolution should still return 3, got {second_resolved}")
  
  # Test that palette size function works with different values
  color_cycle1["palette_size"] = 5
  var updated_resolved = seq_manager.get_resolved_repeat_count()
  assert(updated_resolved == 5, f"Updated resolution should return 5, got {updated_resolved}")
  
  print("✓ Complex parametric scenario tests passed")
end

# Run all tests
def run_all_sequence_manager_tests()
  print("Starting SequenceManager Unit Tests...")
  
  test_sequence_manager_basic()
  test_sequence_manager_step_creation()
  test_sequence_manager_execution()
  test_sequence_manager_timing()
  test_sequence_manager_step_info()
  test_sequence_manager_stop()
  test_sequence_manager_is_running()
  test_sequence_manager_assignment_steps()
  test_sequence_manager_complex_sequence()
  test_sequence_manager_integration()
  test_sequence_manager_parametric_repeat_counts()
  test_sequence_manager_repeat_execution_with_functions()
  test_sequence_manager_palette_size_simulation()
  test_sequence_manager_dynamic_repeat_changes()
  test_sequence_manager_complex_parametric_scenario()
  
  print("\n🎉 All SequenceManager tests passed!")
  return true
end

# Execute tests
run_all_sequence_manager_tests()

return {
  "run_all_sequence_manager_tests": run_all_sequence_manager_tests,
  "test_sequence_manager_basic": test_sequence_manager_basic,
  "test_sequence_manager_step_creation": test_sequence_manager_step_creation,
  "test_sequence_manager_execution": test_sequence_manager_execution,
  "test_sequence_manager_timing": test_sequence_manager_timing,
  "test_sequence_manager_step_info": test_sequence_manager_step_info,
  "test_sequence_manager_stop": test_sequence_manager_stop,
  "test_sequence_manager_is_running": test_sequence_manager_is_running,
  "test_sequence_manager_assignment_steps": test_sequence_manager_assignment_steps,
  "test_sequence_manager_complex_sequence": test_sequence_manager_complex_sequence,
  "test_sequence_manager_integration": test_sequence_manager_integration,
  "test_sequence_manager_parametric_repeat_counts": test_sequence_manager_parametric_repeat_counts,
  "test_sequence_manager_repeat_execution_with_functions": test_sequence_manager_repeat_execution_with_functions,
  "test_sequence_manager_palette_size_simulation": test_sequence_manager_palette_size_simulation,
  "test_sequence_manager_dynamic_repeat_changes": test_sequence_manager_dynamic_repeat_changes,
  "test_sequence_manager_complex_parametric_scenario": test_sequence_manager_complex_parametric_scenario
}