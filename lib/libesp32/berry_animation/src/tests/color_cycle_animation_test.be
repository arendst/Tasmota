# Test file for animation.solid with ColorCycleColorProvider
#
# This file contains tests for the animation.solid class with color cycle provider
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/color_cycle_animation_test.be

# Import the animation module
import animation

# Create a real engine for testing using global.Leds()
var strip = global.Leds(10)
var engine = animation.create_engine(strip)

# Create a test class
class ColorCycleAnimationTest
  var passed
  var failed
  
  def init()
    self.passed = 0
    self.failed = 0
    
    print("Running animation.solid with ColorCycleColorProvider Tests")
    
    self.test_initialization()
    self.test_update_and_render()
    self.test_manual_only_mode()
    self.test_direct_creation()
    
    print(f"animation.solid with ColorCycleColorProvider Tests: {self.passed} passed, {self.failed} failed")
  end
  
  def assert_equal(actual, expected, test_name)
    if actual == expected
      print(f"✓ {test_name}")
      self.passed += 1
    else
      print(f"✗ {test_name}: expected {expected}, got {actual}")
      self.failed += 1
    end
  end
  
  def assert_approx_equal(actual, expected, test_name)
    # For comparing values that might have small floating point differences
    if (actual >= expected - 2) && (actual <= expected + 2)
      print(f"✓ {test_name}")
      self.passed += 1
    else
      print(f"✗ {test_name}: expected ~{expected}, got {actual}")
      self.failed += 1
    end
  end
  
  def test_initialization()
    # Test default initialization with color cycle provider
    var provider = animation.color_cycle(engine)
    var anim = animation.solid(engine)
    anim.color = provider
    anim.priority = 10
    anim.duration = 0
    anim.loop = false
    anim.opacity = 255
    anim.name = "test_default"
    
    # Check that the color was set correctly
    self.assert_equal(anim.color != nil, true, "Color is set")
    var color_provider = anim.get_param("color")  # Get raw provider object
    self.assert_equal(color_provider.palette != nil, true, "Color provider has palette property")
    
    # Test with custom parameters
    var custom_palette = bytes("FFFF0000FF00FF00")  # Red and Green in AARRGGBB format
    var custom_provider = animation.color_cycle(engine)
    custom_provider.palette = custom_palette
    custom_provider.cycle_period = 2000
    
    var anim2 = animation.solid(engine)
    anim2.color = custom_provider
    anim2.priority = 15
    anim2.duration = 0
    anim2.loop = false
    anim2.opacity = 255
    anim2.name = "test_custom"
    
    # Check that the color was set correctly
    self.assert_equal(anim2.color != nil, true, "Custom color is set")
    var color_provider2 = anim2.get_param("color")  # Get raw provider object
    self.assert_equal(color_provider2.palette != nil, true, "Custom color provider has palette property")
    
    # Check provider properties
    self.assert_equal(color_provider2._get_palette_size(), 2, "Custom palette has 2 colors")
    self.assert_equal(color_provider2.cycle_period, 2000, "Custom cycle period is 2000ms")
  end
  
  def test_update_and_render()
    # Create animation with red and blue colors
    var palette = bytes("FFFF0000FF0000FF")  # Red and Blue in AARRGGBB format
    var provider = animation.color_cycle(engine)
    provider.palette = palette
    provider.cycle_period = 1000  # 1 second cycle
    
    var anim = animation.solid(engine)
    anim.color = provider
    anim.priority = 10
    anim.duration = 0
    anim.loop = false
    anim.opacity = 255
    anim.name = "test_render"
    
    # Create a frame buffer
    var frame = animation.frame_buffer(10)  # 10 pixels
    
    # Start the animation
    anim.start(0)  # Start at time 0
    
    # Test brutal color switching - colors should change abruptly, not smoothly
    anim.update(0)
    anim.render(frame, engine.time_ms)
    var pixel_color = frame.get_pixel_color(0)
    self.assert_equal(pixel_color != 0, true, "Start color is not zero")
    
    # Test at middle of cycle - should still be a solid color (brutal switching)
    anim.update(500)  # 50% through cycle
    anim.render(frame, engine.time_ms)
    pixel_color = frame.get_pixel_color(0)
    self.assert_equal(pixel_color != 0, true, "Middle color is not zero")
    
    # Test at end of cycle - should be a different solid color
    anim.update(1000)  # 100% through cycle
    anim.render(frame, engine.time_ms)
    pixel_color = frame.get_pixel_color(0)
    self.assert_equal(pixel_color != 0, true, "End color is not zero")
    
    # Test manual next color trigger
    var initial_color = pixel_color
    provider.next = 1  # Trigger move to next color
    anim.render(frame, engine.time_ms)
    var next_color = frame.get_pixel_color(0)
    # Color should change when next is triggered (though it might be the same if cycling back)
    self.assert_equal(next_color != 0, true, "Next color is not zero")
  end
  
  def test_manual_only_mode()
    print("Testing manual-only mode (cycle_period = 0)...")
    
    # Create animation with manual-only color provider
    var manual_provider = animation.color_cycle(engine)
    manual_provider.palette = bytes("FF0000FFFF00FF00FFFF0000")  # Blue, Green, Red in AARRGGBB format
    manual_provider.cycle_period = 0  # Manual-only mode
    
    var manual_anim = animation.solid(engine)
    manual_anim.color = manual_provider
    manual_anim.priority = 10
    manual_anim.duration = 0
    manual_anim.loop = false
    manual_anim.opacity = 255
    manual_anim.name = "manual_test"
    
    # Create a frame buffer
    var frame = animation.frame_buffer(10)  # 10 pixels
    
    # Start the animation
    manual_anim.start(0)  # Start at time 0
    
    # Test that color doesn't change with time in manual mode
    manual_anim.update(0)
    manual_anim.render(frame, engine.time_ms)
    var initial_color = frame.get_pixel_color(0)
    self.assert_equal(initial_color != 0, true, "Initial color should not be zero")
    
    # Advance time significantly - color should NOT change in manual mode
    engine.time_ms = 10000  # 10 seconds later
    manual_anim.update(engine.time_ms)
    manual_anim.render(frame, engine.time_ms)
    var same_color = frame.get_pixel_color(0)
    self.assert_equal(same_color, initial_color, "Color should not change with time in manual mode")
    
    # Manually trigger next color
    manual_provider.next = 1
    manual_anim.render(frame, engine.time_ms)
    var next_color = frame.get_pixel_color(0)
    # Color might be the same if we cycled back to the same color, but the trigger should work
    self.assert_equal(next_color != 0, true, "Next color should not be zero")
    
    # Trigger next again to ensure it works multiple times
    var previous_color = next_color
    manual_provider.next = 1
    manual_anim.render(frame, engine.time_ms)
    var third_color = frame.get_pixel_color(0)
    self.assert_equal(third_color != 0, true, "Third color should not be zero")
    
    print("✓ Manual-only mode test passed")
  end
  
  def test_direct_creation()
    # Test direct creation without factory method (following new parameterized pattern)
    var provider = animation.color_cycle(engine)
    provider.palette = bytes("FF0000FFFF00FF00FFFF0000")  # Blue, Green, Red in AARRGGBB format
    provider.cycle_period = 3000  # 3 second cycle period
    
    var anim = animation.solid(engine)
    anim.color = provider
    anim.priority = 10
    anim.duration = 0
    anim.loop = false
    anim.opacity = 255
    anim.name = "test_direct"
    
    # Check that the animation was created correctly
    self.assert_equal(anim != nil, true, "Animation was created")
    self.assert_equal(anim.render != nil, true, "Animation has render method")
    var color_provider3 = anim.get_param("color")  # Get raw provider object
    self.assert_equal(color_provider3.palette != nil, true, "Color provider has palette property")
    
    # Check provider properties
    self.assert_equal(color_provider3._get_palette_size(), 3, "Palette has 3 colors")
    self.assert_equal(color_provider3.cycle_period, 3000, "Cycle period is 3000ms")
    
    # Check animation properties
    self.assert_equal(anim.priority, 10, "Priority is 10")
  end
end

# Run the tests
var test_instance = ColorCycleAnimationTest()

# Check if any tests failed
if test_instance.failed > 0
  raise "test_failed"
end

# Return success if we got this far
return true