# Unit tests for the SequenceManager class
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/sequence_manager_test.be

import string
import animation

def test_sequence_manager_basic()
  print("=== SequenceManager Basic Tests ===")
  
  # Test SequenceManager class exists
  assert(animation.SequenceManager != nil, "SequenceManager class should be defined")
  
  # Create strip and engine for testing
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  
  # Test initialization
  var seq_manager = animation.SequenceManager(engine)
  assert(seq_manager.controller == engine, "Engine should be set correctly")
  assert(seq_manager.steps != nil, "Steps list should be initialized")
  assert(seq_manager.steps.size() == 0, "Steps list should be empty initially")
  assert(seq_manager.step_index == 0, "Step index should be 0 initially")
  assert(seq_manager.is_running == false, "Sequence should not be running initially")
  
  print("✓ Basic initialization tests passed")
end

def test_sequence_manager_step_creation()
  print("=== SequenceManager Step Creation Tests ===")
  
  # Test step creation helper functions
  assert(animation.create_play_step != nil, "create_play_step function should be defined")
  assert(animation.create_wait_step != nil, "create_wait_step function should be defined")
  assert(animation.create_stop_step != nil, "create_stop_step function should be defined")
  
  # Create test animation using new parameterized API
  var strip = global.Leds(30)
  var engine = animation.animation_engine(strip)
  var color_provider = animation.static_color(engine)
  color_provider.color = 0xFFFF0000
  var test_anim = animation.solid(engine)
  test_anim.color = color_provider
  test_anim.priority = 0
  test_anim.duration = 0
  test_anim.loop = true
  test_anim.name = "test"
  
  # Test play step creation
  var play_step = animation.create_play_step(test_anim, 5000)
  assert(play_step["type"] == "play", "Play step should have correct type")
  assert(play_step["animation"] == test_anim, "Play step should have correct animation")
  assert(play_step["duration"] == 5000, "Play step should have correct duration")
  
  # Test wait step creation
  var wait_step = animation.create_wait_step(2000)
  assert(wait_step["type"] == "wait", "Wait step should have correct type")
  assert(wait_step["duration"] == 2000, "Wait step should have correct duration")
  
  # Test stop step creation
  var stop_step = animation.create_stop_step(test_anim)
  assert(stop_step["type"] == "stop", "Stop step should have correct type")
  assert(stop_step["animation"] == test_anim, "Stop step should have correct animation")
  
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
  
  # Create sequence steps
  var steps = []
  steps.push(animation.create_play_step(anim1, 1000))
  steps.push(animation.create_wait_step(500))
  steps.push(animation.create_play_step(anim2, 2000))
  steps.push(animation.create_stop_step(anim1))
  
  # Test sequence start
  tasmota.set_millis(10000)
  engine.start()  # Start the engine
  engine.on_tick(10000)  # Update engine time
  seq_manager.start_sequence(steps)
  
  assert(seq_manager.is_running == true, "Sequence should be running after start")
  assert(seq_manager.steps.size() == 4, "Sequence should have 4 steps")
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
  
  # Create simple sequence with timed steps
  var steps = []
  steps.push(animation.create_play_step(test_anim, 1000))  # 1 second
  steps.push(animation.create_wait_step(500))             # 0.5 seconds
  
  # Start sequence at time 20000
  tasmota.set_millis(20000)
  engine.start()  # Start the engine
  engine.on_tick(20000)  # Update engine time
  seq_manager.start_sequence(steps)
  
  # Update immediately - should still be on first step
  seq_manager.update()
  assert(seq_manager.step_index == 0, "Should still be on first step immediately")
  assert(seq_manager.is_running == true, "Sequence should still be running")
  
  # Update after 500ms - should still be on first step
  tasmota.set_millis(20500)
  engine.on_tick(20500)  # Update engine time
  seq_manager.update()
  assert(seq_manager.step_index == 0, "Should still be on first step after 500ms")
  
  # Update after 1000ms - should advance to second step (wait)
  tasmota.set_millis(21000)
  engine.on_tick(21000)  # Update engine time
  seq_manager.update()
  assert(seq_manager.step_index == 1, "Should advance to second step after 1000ms")
  
  # Update after additional 500ms - should complete sequence
  tasmota.set_millis(21500)
  engine.on_tick(21500)  # Update engine time
  seq_manager.update()
  assert(seq_manager.is_running == false, "Sequence should complete after all steps")
  
  print("✓ Timing tests passed")
end

def test_sequence_manager_step_info()
  print("=== SequenceManager Step Info Tests ===")
  
  # Create strip and engine
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  var seq_manager = animation.SequenceManager(engine)
  
  # Test step info when not running
  var step_info = seq_manager.get_current_step_info()
  assert(step_info == nil, "Step info should be nil when not running")
  
  # Create test sequence using new parameterized API
  var color_provider = animation.static_color(engine)
  color_provider.color = 0xFFFF0000
  var test_anim = animation.solid(engine)
  test_anim.color = color_provider
  test_anim.priority = 0
  test_anim.duration = 0
  test_anim.loop = true
  test_anim.name = "test"
  var steps = []
  steps.push(animation.create_play_step(test_anim, 2000))
  steps.push(animation.create_wait_step(1000))
  
  # Start sequence
  tasmota.set_millis(30000)
  engine.start()  # Start the engine
  engine.on_tick(30000)  # Update engine time
  seq_manager.start_sequence(steps)
  
  # Get step info
  step_info = seq_manager.get_current_step_info()
  assert(step_info != nil, "Step info should not be nil when running")
  assert(step_info["step_index"] == 0, "Step info should show correct step index")
  assert(step_info["total_steps"] == 2, "Step info should show correct total steps")
  assert(step_info["current_step"]["type"] == "play", "Step info should show correct step type")
  assert(step_info["elapsed_ms"] >= 0, "Step info should show elapsed time")
  
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
  var steps = []
  steps.push(animation.create_play_step(test_anim, 5000))
  
  # Start sequence
  tasmota.set_millis(40000)
  engine.start()  # Start the engine
  engine.on_tick(40000)  # Update engine time
  seq_manager.start_sequence(steps)
  assert(seq_manager.is_running == true, "Sequence should be running")
  
  # Stop sequence
  seq_manager.stop_sequence()
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
  var steps = []
  steps.push(animation.create_play_step(test_anim, 1000))
  
  tasmota.set_millis(50000)
  engine.start()  # Start the engine
  engine.on_tick(50000)  # Update engine time
  seq_manager.start_sequence(steps)
  assert(seq_manager.is_sequence_running() == true, "Sequence should be running after start")
  
  # Complete sequence
  tasmota.set_millis(51000)
  engine.on_tick(51000)  # Update engine time
  seq_manager.update()
  assert(seq_manager.is_sequence_running() == false, "Sequence should not be running after completion")
  
  print("✓ Running state tests passed")
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
  
  # Create complex sequence
  var steps = []
  steps.push(animation.create_play_step(red_anim, 1000))    # Play red for 1s
  steps.push(animation.create_play_step(green_anim, 800))   # Play green for 0.8s
  steps.push(animation.create_wait_step(200))              # Wait 0.2s
  steps.push(animation.create_play_step(blue_anim, 1500))  # Play blue for 1.5s
  steps.push(animation.create_stop_step(red_anim))         # Stop red
  steps.push(animation.create_stop_step(green_anim))       # Stop green
  
  # Start sequence
  tasmota.set_millis(60000)
  engine.start()  # Start the engine
  engine.on_tick(60000)  # Update engine time
  seq_manager.start_sequence(steps)
  
  # Test sequence progression step by step
  
  # After 1000ms: red completes, should advance to green (step 1)
  tasmota.set_millis(61000)
  engine.on_tick(61000)  # Update engine time
  seq_manager.update()
  assert(seq_manager.step_index == 1, "Should advance to step 1 (green) after red completes")
  assert(seq_manager.is_running == true, "Sequence should still be running")
  
  # After 1800ms: green completes, should advance to wait (step 2)
  tasmota.set_millis(61800)
  engine.on_tick(61800)  # Update engine time
  seq_manager.update()
  assert(seq_manager.step_index == 2, "Should advance to step 2 (wait) after green completes")
  assert(seq_manager.is_running == true, "Sequence should still be running")
  
  # After 2000ms: wait completes, should advance to blue (step 3)
  tasmota.set_millis(62000)
  engine.on_tick(62000)  # Update engine time
  seq_manager.update()
  assert(seq_manager.step_index == 3, "Should advance to step 3 (blue) after wait completes")
  assert(seq_manager.is_running == true, "Sequence should still be running")
  
  # After 3500ms: blue completes, should advance to stop red (step 4)
  tasmota.set_millis(63500)
  engine.on_tick(63500)  # Update engine time
  seq_manager.update()
  assert(seq_manager.step_index == 4, "Should advance to step 4 (stop red) after blue completes")
  assert(seq_manager.is_running == true, "Sequence should still be running")
  
  # Stop steps execute immediately, so another update should advance to step 5 and then complete
  seq_manager.update()
  
  # The sequence should complete when step_index reaches the end
  if seq_manager.is_running
    # If still running, do one more update to complete
    seq_manager.update()
  end
  
  assert(seq_manager.is_running == false, "Complex sequence should complete after all stop steps")
  
  print("✓ Complex sequence tests passed")
end

def test_sequence_manager_integration()
  print("=== SequenceManager Integration Tests ===")
  
  # Create strip and engine
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  
  # Test engine integration
  var seq_manager = animation.SequenceManager(engine)
  engine.add_sequence_manager(seq_manager)
  
  # Create test sequence using new parameterized API
  var color_provider = animation.static_color(engine)
  color_provider.color = 0xFFFF0000
  var test_anim = animation.solid(engine)
  test_anim.color = color_provider
  test_anim.priority = 0
  test_anim.duration = 0
  test_anim.loop = true
  test_anim.name = "test"
  var steps = []
  steps.push(animation.create_play_step(test_anim, 1000))
  
  # Start sequence
  tasmota.set_millis(70000)
  engine.start()  # Start the engine
  engine.on_tick(70000)  # Update engine time
  seq_manager.start_sequence(steps)
  
  # Test that engine's on_tick calls sequence manager update
  # The engine has a 5ms minimum delta check, so we need to account for that
  tasmota.set_millis(71000)
  
  # Start the engine to initialize last_update
  engine.start()
  engine.on_tick(70000)  # Initialize last_update
  
  # Now call on_tick after the sequence should complete
  engine.on_tick(71000)  # This should call seq_manager.update()
  
  # The sequence should complete after the 1-second duration
  assert(seq_manager.is_running == false, "Sequence should complete after 1 second duration")
  
  # Test engine cleanup
  engine.clear()
  assert(engine.sequence_managers.size() == 0, "Engine should clear sequence managers")
  
  print("✓ Integration tests passed")
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
  test_sequence_manager_complex_sequence()
  test_sequence_manager_integration()
  
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
  "test_sequence_manager_complex_sequence": test_sequence_manager_complex_sequence,
  "test_sequence_manager_integration": test_sequence_manager_integration
}