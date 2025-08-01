# Test file for animation.filled_animation with RichPaletteColorProvider
#
# This file contains tests for the animation.filled_animation class with rich palette provider
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/rich_palette_animation_test.be

# Import the animation module
import animation

# Create a test class
class RichPaletteAnimationTest
  var passed
  var failed
  
  def init()
    self.passed = 0
    self.failed = 0
    
    print("Running animation.filled_animation with RichPaletteColorProvider Tests")
    
    self.test_initialization()
    self.test_update_and_render()
    self.test_factory_method()
    self.test_palette_properties()
    self.test_css_gradient()
    
    print(f"animation.filled_animation with RichPaletteColorProvider Tests: {self.passed} passed, {self.failed} failed")
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
    # Test default initialization with rich palette provider
    var provider = animation.rich_palette_color_provider()
    var anim = animation.filled_animation(provider)
    
    # Check that the color was set correctly
    self.assert_equal(anim.color != nil, true, "Color is set")
    self.assert_equal(isinstance(anim.color, animation.rich_palette_color_provider), true, "Color is a RichPaletteColorProvider")
    
    # Test with custom parameters
    var custom_palette = bytes("00FF0000" "FFFFFF00")
    var custom_provider = animation.rich_palette_color_provider(custom_palette, 2000, 0, 128)
    var anim2 = animation.filled_animation(custom_provider)
    
    # Check that the color was set correctly
    self.assert_equal(anim2.color != nil, true, "Custom color is set")
    self.assert_equal(isinstance(anim2.color, animation.rich_palette_color_provider), true, "Custom color is a RichPaletteColorProvider")
    
    # Check provider properties
    self.assert_equal(anim2.color.slots, 2, "Custom palette has 2 slots")
    self.assert_equal(anim2.color.cycle_period, 2000, "Custom cycle period is 2000ms")
    self.assert_equal(anim2.color.transition_type, 0, "Custom transition type is linear")
    self.assert_equal(anim2.color.brightness, 128, "Custom brightness is 128")
  end
  
  def test_update_and_render()
    # Create animation with red and blue colors
    var palette = bytes("00FF0000" "FF0000FF")  # Red to Blue in VRGB format
    var provider = animation.rich_palette_color_provider(palette, 1000, 0)  # 1 second cycle, linear transition
    var anim = animation.filled_animation(provider)
    
    # Create a frame buffer
    var frame = animation.frame_buffer(10)  # 10 pixels
    
    # Start the animation
    anim.start(0)  # Start at time 0
    
    # Test at start - just check that we get a valid color
    anim.update(0)
    anim.render(frame, tasmota.millis())
    var pixel_color = frame.get_pixel_color(0)
    self.assert_equal(pixel_color != 0, true, "Start color is not zero")
    
    # Test at middle - check that color changes
    anim.update(500)  # 50% through cycle
    anim.render(frame, tasmota.millis())
    var middle_color = frame.get_pixel_color(0)
    self.assert_equal(middle_color != 0, true, "Middle color is not zero")
    
    # Test at end - check that color changes again
    anim.update(1000)  # 100% through cycle
    anim.render(frame, tasmota.millis())
    var end_color = frame.get_pixel_color(0)
    self.assert_equal(end_color != 0, true, "End color is not zero")
    
    # Test looping - should be back to start color
    anim.update(2000)  # After another full cycle
    anim.render(frame, tasmota.millis())
    var loop_color = frame.get_pixel_color(0)
    self.assert_equal(loop_color, pixel_color, "Loop color matches start color")
    
    # Test that colors are different at different times
    self.assert_equal(pixel_color != middle_color, true, "Start and middle colors are different")
    self.assert_equal(middle_color != end_color, true, "Middle and end colors are different")
  end
  
  def test_factory_method()
    # Test the rainbow factory method
    var provider = animation.rich_palette_color_provider.rainbow(5000, 1, 255)
    var anim = animation.filled_animation(provider, 10)  # Priority 10
    
    # Check that the animation was created correctly
    self.assert_equal(anim != nil, true, "Animation was created")
    self.assert_equal(isinstance(anim, animation.filled_animation), true, "Animation is a animation.filled_animation")
    self.assert_equal(isinstance(anim.color, animation.rich_palette_color_provider), true, "Color is a RichPaletteColorProvider")
    
    # Check provider properties
    self.assert_equal(anim.color.cycle_period, 5000, "Cycle period is 5000ms")
    self.assert_equal(anim.color.transition_type, 1, "Transition type is sine")
    self.assert_equal(anim.color.brightness, 255, "Brightness is 255")
    
    # Check animation properties
    self.assert_equal(anim.priority, 10, "Priority is 10")
  end
  
  def test_palette_properties()
    # Test palette properties and value-based color generation
    var palette = bytes("00FF0000" "80FFFF00" "FF0000FF")  # Red to Yellow to Blue
    var provider = animation.rich_palette_color_provider(palette, 1000)
    
    # Check basic properties
    self.assert_equal(provider.slots, 3, "Palette has 3 slots")
    self.assert_equal(provider.cycle_period, 1000, "Cycle period is 1000ms")
    
    # Test range setting and value-based colors
    provider.set_range(0, 100)
    self.assert_equal(provider.range_min, 0, "Range min is 0")
    self.assert_equal(provider.range_max, 100, "Range max is 100")
    
    # Test value-based color generation
    var color_0 = provider.get_color_for_value(0, 0)
    var color_50 = provider.get_color_for_value(50, 0)
    var color_100 = provider.get_color_for_value(100, 0)
    
    self.assert_equal(color_0 != nil, true, "Color at value 0 is not nil")
    self.assert_equal(color_50 != nil, true, "Color at value 50 is not nil")
    self.assert_equal(color_100 != nil, true, "Color at value 100 is not nil")
    
    # Colors should be different
    self.assert_equal(color_0 != color_50, true, "Color at 0 differs from color at 50")
    self.assert_equal(color_50 != color_100, true, "Color at 50 differs from color at 100")
  end
  
  def test_css_gradient()
    # Test CSS gradient generation
    var palette = bytes("00FF0000" "80FFFF00" "FF0000FF")  # Red to Yellow to Blue
    var provider = animation.rich_palette_color_provider(palette, 1000)
    
    var css = provider.to_css_gradient()
    
    # Check that the CSS is not empty
    self.assert_equal(css != nil, true, "CSS gradient is not nil")
    self.assert_equal(size(css) > 0, true, "CSS gradient is not empty")
    
    # Check if the CSS string starts with the expected prefix
    var prefix = "background:linear-gradient"
    var prefix_len = size(prefix)
    var css_prefix = css[0..prefix_len-1]
    self.assert_equal(css_prefix == prefix, true, "CSS starts with correct prefix")
  end
end

# Run the tests
RichPaletteAnimationTest()

# Return success if we got this far
return true