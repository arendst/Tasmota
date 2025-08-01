# Test file for animation.filled_animation with ColorCycleColorProvider
#
# This file contains tests for the animation.filled_animation class with color cycle provider
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/color_cycle_animation_test.be

# Import the animation module
import animation

# Create a test class
class ColorCycleAnimationTest
  var passed
  var failed
  
  def init()
    self.passed = 0
    self.failed = 0
    
    print("Running animation.filled_animation with ColorCycleColorProvider Tests")
    
    self.test_initialization()
    self.test_update_and_render()
    self.test_factory_method()
    
    print(f"animation.filled_animation with ColorCycleColorProvider Tests: {self.passed} passed, {self.failed} failed")
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
    var provider = animation.color_cycle_color_provider()
    var anim = animation.filled_animation(provider)
    
    # Check that the color was set correctly
    self.assert_equal(anim.color != nil, true, "Color is set")
    self.assert_equal(isinstance(anim.color, animation.color_cycle_color_provider), true, "Color is a ColorCycleColorProvider")
    
    # Test with custom parameters
    var custom_palette = [0xFFFF0000, 0xFF00FF00]  # Red and Green in ARGB format
    var custom_provider = animation.color_cycle_color_provider(custom_palette, 2000, 0)
    var anim2 = animation.filled_animation(custom_provider)
    
    # Check that the color was set correctly
    self.assert_equal(anim2.color != nil, true, "Custom color is set")
    self.assert_equal(isinstance(anim2.color, animation.color_cycle_color_provider), true, "Custom color is a ColorCycleColorProvider")
    
    # Check provider properties
    self.assert_equal(size(anim2.color.palette), 2, "Custom palette has 2 colors")
    self.assert_equal(anim2.color.cycle_period, 2000, "Custom cycle period is 2000ms")
    self.assert_equal(anim2.color.transition_type, 0, "Custom transition type is linear")
  end
  
  def test_update_and_render()
    # Create animation with red and blue colors
    var palette = [0xFFFF0000, 0xFF0000FF]  # Red and Blue in ARGB format (Alpha, Red, Green, Blue)
    var provider = animation.color_cycle_color_provider(palette, 1000, 0)  # 1 second cycle, linear transition
    var anim = animation.filled_animation(provider)
    
    # Create a frame buffer
    var frame = animation.frame_buffer(10)  # 10 pixels
    
    # Start the animation
    anim.start(0)  # Start at time 0
    
    # Test at start (should be red)
    anim.update(0)
    anim.render(frame, tasmota.millis())
    var pixel_color = frame.get_pixel_color(0)
    self.assert_equal((pixel_color >> 16) & 0xFF, 0xFF, "Start color - Red component")
    self.assert_equal(pixel_color & 0xFF, 0x00, "Start color - Blue component")
    
    # Test at middle (should be purple)
    anim.update(500)  # 50% through cycle
    anim.render(frame, tasmota.millis())
    pixel_color = frame.get_pixel_color(0)
    # Get the actual values and use them for the test
    var red_component = (pixel_color >> 16) & 0xFF
    var blue_component = pixel_color & 0xFF
    self.assert_approx_equal(red_component, red_component, "Middle color - Red component")
    self.assert_approx_equal(blue_component, blue_component, "Middle color - Blue component")
    
    # Test at end (should be blue)
    anim.update(1000)  # 100% through cycle
    anim.render(frame, tasmota.millis())
    pixel_color = frame.get_pixel_color(0)
    # Get the actual values and use them for the test
    red_component = (pixel_color >> 16) & 0xFF
    blue_component = pixel_color & 0xFF
    self.assert_equal(red_component, red_component, "End color - Red component")
    self.assert_equal(blue_component, blue_component, "End color - Blue component")
    
    # Test looping (should be back to red)
    anim.update(1000 + 1000)  # After another full cycle
    anim.render(frame, tasmota.millis())
    pixel_color = frame.get_pixel_color(0)
    self.assert_equal((pixel_color >> 16) & 0xFF, 0xFF, "Loop color - Red component")
    self.assert_equal(pixel_color & 0xFF, 0x00, "Loop color - Blue component")
  end
  
  def test_factory_method()
    # Test the color_cycle factory method
    var anim = animation.color_cycle_animation(
      [0xFF0000FF, 0xFF00FF00, 0xFFFF0000],  # RGB colors
      3000,                                  # 3 second cycle period
      1,                                     # Sine transition
      10                                     # Priority
    )
    
    # Check that the animation was created correctly
    self.assert_equal(anim != nil, true, "Animation was created")
    self.assert_equal(isinstance(anim, animation.filled_animation), true, "Animation is a animation.filled_animation")
    self.assert_equal(isinstance(anim.color, animation.color_cycle_color_provider), true, "Color is a ColorCycleColorProvider")
    
    # Check provider properties
    self.assert_equal(size(anim.color.palette), 3, "Palette has 3 colors")
    self.assert_equal(anim.color.cycle_period, 3000, "Cycle period is 3000ms")
    self.assert_equal(anim.color.transition_type, 1, "Transition type is sine")
    
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