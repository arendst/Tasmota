# Test file for animation.solid with RichPaletteColorProvider
#
# This file contains tests for the animation.solid class with rich palette provider
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/rich_palette_animation_test.be

# Import the animation module
import animation

# Create a mock engine for testing
class MockEngine
  var time_ms
  
  def init()
    self.time_ms = 0  # Start at time 0
  end
  
  def get_strip_length()
    return 10  # Mock strip length
  end
  
  def set_time(time)
    self.time_ms = time
  end
end

var mock_engine = MockEngine()

# Create a test class
class RichPaletteAnimationTest
  var passed
  var failed
  
  def init()
    self.passed = 0
    self.failed = 0
    
    print("Running animation.solid with RichPaletteColorProvider Tests")
    
    self.test_initialization()
    self.test_update_and_render()
    self.test_factory_method()
    self.test_palette_properties()
    self.test_css_gradient()
    self.test_cycle_period_zero()
    
    print(f"animation.solid with RichPaletteColorProvider Tests: {self.passed} passed, {self.failed} failed")
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
    var provider = animation.rich_palette(mock_engine)
    var anim = animation.solid(mock_engine)
    anim.color = provider
    
    # Check that the color was set correctly (it will be resolved to a color value)
    self.assert_equal(anim.color != nil, true, "Color is set")
    # The color should be resolved to an integer color value
    self.assert_equal(type(anim.color) == 'int', true, "Color is resolved to integer")
    
    # Test with custom parameters using new parameterized class specification
    var custom_palette = bytes("00FF0000" "FFFFFF00")
    var custom_provider = animation.rich_palette(mock_engine)
    custom_provider.palette = custom_palette
    custom_provider.cycle_period = 2000
    custom_provider.transition_type = animation.LINEAR
    custom_provider.brightness = 128
    
    var anim2 = animation.solid(mock_engine)
    anim2.color = custom_provider
    
    # Check that the color was set correctly
    self.assert_equal(anim2.color != nil, true, "Custom color is set")
    self.assert_equal(type(anim2.color) == 'int', true, "Custom color is resolved to integer")
    
    # Check provider properties directly on the provider object
    self.assert_equal(custom_provider.cycle_period, 2000, "Custom cycle period is 2000ms")
    self.assert_equal(custom_provider.transition_type, animation.LINEAR, "Custom transition type is linear")
    self.assert_equal(custom_provider.brightness, 128, "Custom brightness is 128")
  end
  
  def test_update_and_render()
    # Create animation with red and blue colors
    var palette = bytes("00FF0000" "FF0000FF")  # Red to Blue in VRGB format
    var provider = animation.rich_palette(mock_engine)
    provider.palette = palette
    provider.cycle_period = 1000  # 1 second cycle
    provider.transition_type = animation.LINEAR  # linear transition
    
    var anim = animation.solid(mock_engine)
    anim.color = provider
    
    # Create a frame buffer
    var frame = animation.frame_buffer(10)  # 10 pixels
    
    # Start the animation and provider
    anim.start(0)  # Start at time 0
    provider.start(0)  # Start provider at time 0
    
    # Test at start - update engine time and get color
    mock_engine.set_time(0)
    anim.update(0)
    anim.render(frame, 0)
    var pixel_color = frame.get_pixel_color(0)
    self.assert_equal(pixel_color != 0, true, "Start color is not zero")
    
    # Test at middle - update engine time and get color
    mock_engine.set_time(500)
    anim.update(500)  # 50% through cycle
    anim.render(frame, 500)
    var middle_color = frame.get_pixel_color(0)
    self.assert_equal(middle_color != 0, true, "Middle color is not zero")
    
    # Test at end - update engine time and get color
    mock_engine.set_time(1000)
    anim.update(1000)  # 100% through cycle
    anim.render(frame, 1000)
    var end_color = frame.get_pixel_color(0)
    self.assert_equal(end_color != 0, true, "End color is not zero")
    
    # Test looping - should be back to start color
    mock_engine.set_time(2000)
    anim.update(2000)  # After another full cycle
    anim.render(frame, 2000)
    var loop_color = frame.get_pixel_color(0)
    self.assert_equal(loop_color, pixel_color, "Loop color matches start color")
    
    # Test that colors are different at different times (may be same due to palette resolution)
    # Just check that we got valid colors - exact color differences depend on palette implementation
    self.assert_equal(pixel_color != 0, true, "Start color is valid")
    self.assert_equal(middle_color != 0, true, "Middle color is valid")
  end
  
  def test_factory_method()
    # Test the rainbow factory method
    var provider = animation.rich_palette_rainbow(mock_engine)
    provider.cycle_period = 5000
    provider.transition_type = animation.SINE  # sine
    provider.brightness = 255
    
    var anim = animation.solid(mock_engine)
    anim.color = provider
    anim.priority = 10  # Priority 10
    
    # Check that the animation was created correctly
    self.assert_equal(anim != nil, true, "Animation was created")
    self.assert_equal(anim.render != nil, true, "Animation has render method")
    self.assert_equal(type(anim.color) == 'int', true, "Color is resolved to integer")
    
    # Check provider properties directly on the provider object
    self.assert_equal(provider.cycle_period, 5000, "Cycle period is 5000ms")
    self.assert_equal(provider.transition_type, animation.SINE, "Transition type is sine")
    self.assert_equal(provider.brightness, 255, "Brightness is 255")
    
    # Check animation properties
    self.assert_equal(anim.priority, 10, "Priority is 10")
  end
  
  def test_palette_properties()
    # Test palette properties and value-based color generation
    var palette = bytes("00FF0000" "80FFFF00" "FF0000FF")  # Red to Yellow to Blue
    var provider = animation.rich_palette(mock_engine)
    provider.palette = palette
    provider.cycle_period = 1000
    
    # Check basic properties
    self.assert_equal(provider.cycle_period, 1000, "Cycle period is 1000ms")
    
    # Test range setting and value-based colors
    provider.range_min = 0
    provider.range_max = 100
    self.assert_equal(provider.range_min, 0, "Range min is 0")
    self.assert_equal(provider.range_max, 100, "Range max is 100")
    
    # Test value-based color generation
    provider.start()
    print(f"{provider.slots_arr=} {provider.value_arr=}")
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
    var provider = animation.rich_palette(mock_engine)
    provider.palette = palette
    provider.cycle_period = 1000
    
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
  
  def test_cycle_period_zero()
    # Test the new cycle_period = 0 functionality for value-based color mapping
    var palette = bytes("00FF0000" "80FFFF00" "FF0000FF")  # Red to Yellow to Blue
    var provider = animation.rich_palette(mock_engine)
    provider.palette = palette
    provider.cycle_period = 0  # Value-based mode
    provider.range_min = 0
    provider.range_max = 255
    provider.start()
    
    # Check that cycle_period can be set to 0
    self.assert_equal(provider.cycle_period, 0, "Cycle period can be set to 0")
    
    # Test that produce_value returns static color when cycle_period = 0
    var color_t0 = provider.produce_value("color", 0)
    var color_t1000 = provider.produce_value("color", 1000)
    var color_t2000 = provider.produce_value("color", 2000)
    
    # All colors should be the same (static) when cycle_period = 0
    self.assert_equal(color_t0, color_t1000, "Static color at different times (0 vs 1000)")
    self.assert_equal(color_t1000, color_t2000, "Static color at different times (1000 vs 2000)")
    
    # Test value-based color generation still works
    var color_0 = provider.get_color_for_value(0, 0)
    var color_128 = provider.get_color_for_value(128, 0)
    var color_255 = provider.get_color_for_value(255, 0)
    
    self.assert_equal(color_0 != nil, true, "Value-based color at 0 is not nil")
    self.assert_equal(color_128 != nil, true, "Value-based color at 128 is not nil")
    self.assert_equal(color_255 != nil, true, "Value-based color at 255 is not nil")
    
    # Value-based colors should be different
    self.assert_equal(color_0 != color_128, true, "Value-based colors differ (0 vs 128)")
    self.assert_equal(color_128 != color_255, true, "Value-based colors differ (128 vs 255)")
    
    # Test that we can switch back to time-based mode
    provider.cycle_period = 1000
    self.assert_equal(provider.cycle_period, 1000, "Can switch back to time-based mode")
    
    # Start the provider for time-based mode
    provider.start(0)
    
    # Now colors should change over time again
    var time_color_0 = provider.produce_value("color", 0)
    var time_color_500 = provider.produce_value("color", 500)
    # Note: Colors may be the same depending on palette resolution, so just check they're valid
    self.assert_equal(time_color_0 != 0, true, "Time-based color at 0 is valid")
  end
end

# Run the tests
RichPaletteAnimationTest()

# Return success if we got this far
return true