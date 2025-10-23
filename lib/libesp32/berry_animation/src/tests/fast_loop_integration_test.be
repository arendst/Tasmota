# Unit tests for the fast_loop integration in AnimationEngine
#
# This file contains tests to verify that the AnimationEngine
# properly integrates with Tasmota's fast_loop system.
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/fast_loop_integration_test.be

# Import the animation module
import animation

# Using global.Leds instead of MockStrip
import global

# Test animation that tracks method calls
class TestAnimation : animation.animation
  var render_called
  var update_called
  var update_time
  
  def init(engine)
    super(self).init(engine)
    self.render_called = false
    self.update_called = false
    self.update_time = 0
  end
  
  def render(frame)
    self.render_called = true
    # Fill frame with red for testing
    if frame != nil
      frame.fill_pixels(frame.pixels, 0xFF0000FF)
    end
    return true
  end
  
  def update(time_ms)
    self.update_called = true
    self.update_time = time_ms
    return super(self).update(time_ms)
  end
  
  def reset_test_state()
    self.render_called = false
    self.update_called = false
    self.update_time = 0
  end
end

# Test fast_loop registration and removal
def test_fast_loop_registration()
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  # Check that fast_loop_closure is initially nil
  assert(engine.fast_loop_closure == nil)
  
  # Start the engine
  engine.run()
  
  # Check that fast_loop_closure is now set
  assert(engine.fast_loop_closure != nil)
  
  # Stop the engine
  engine.stop()
  
  # Check that fast_loop_closure is still set (but not used)
  assert(engine.fast_loop_closure != nil)
  
  print("✓ test_fast_loop_registration passed")
end

# Test on_tick performance optimization
def test_on_tick_performance()
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  # Add a test animation
  var anim = TestAnimation(engine)
  anim.priority = 1
  engine.add(anim)
  anim.start(tasmota.millis())
  
  # Start the engine
  engine.run()
  
  # Set initial time
  var initial_time = 1000
  tasmota.set_millis(initial_time)
  engine.last_update = initial_time
  
  # Call on_tick with less than 5ms elapsed
  tasmota.set_millis(initial_time + 3)
  var result = engine.on_tick()
  
  # Check that on_tick returned true but didn't render
  assert(result == true)
  assert(anim.render_called == false)
  
  # Call on_tick with more than 5ms elapsed
  tasmota.set_millis(initial_time + 10)
  result = engine.on_tick()
  
  # Check that on_tick rendered the animation
  assert(result == true)
  assert(anim.render_called == true)
  
  # Reset test state
  anim.reset_test_state()
  strip.clear()
  
  # Note: We can't test can_show functionality with global.Leds as it always returns true
  
  # Stop the engine
  engine.stop()
  
  print("✓ test_on_tick_performance passed")
end

# Test animation update timing
def test_animation_update_timing()
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  # Add a test animation
  var anim = TestAnimation(engine)
  anim.priority = 1
  engine.add(anim)
  
  # Start the animation and engine
  var start_time = 2000
  tasmota.set_millis(start_time)
  anim.start(start_time)
  engine.run()
  
  # Call on_tick with a specific time
  var update_time = start_time + 100
  tasmota.set_millis(update_time)
  engine.on_tick()
  
  # Check that the animation was updated with the correct time
  assert(anim.update_called == true)
  assert(anim.update_time == update_time)
  
  # Stop the engine
  engine.stop()
  
  print("✓ test_animation_update_timing passed")
end

# Run all tests
def run_tests()
  print("Running fast_loop integration tests...")
  test_fast_loop_registration()
  test_on_tick_performance()
  test_animation_update_timing()
  print("All fast_loop integration tests passed!")
end

# Run the tests
run_tests()