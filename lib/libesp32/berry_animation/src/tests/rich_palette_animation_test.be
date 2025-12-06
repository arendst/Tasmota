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
  var strip_length
  
  def init()
    self.time_ms = 0  # Start at time 0
    self.strip_length = 10  # Mock strip length
  end
  
  def get_strip_length()
    return self.strip_length
  end
  
  def set_time(time)
    self.time_ms = time
  end
  
  # Fake add() method for value provider auto-registration
  def add(obj)
    return true
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
    self.test_sine_mode()
    self.test_sine_mode_value_based()
    
    print(f"animation.solid with RichPaletteColorProvider Tests: {self.passed} passed, {self.failed} failed")
    if self.failed > 0
      raise "test_failed"
    end
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
    custom_provider.transition_type = animation.SINE
    custom_provider.brightness = 128
    
    var anim2 = animation.solid(mock_engine)
    anim2.color = custom_provider
    
    # Check that the color was set correctly
    self.assert_equal(anim2.color != nil, true, "Custom color is set")
    self.assert_equal(type(anim2.color) == 'int', true, "Custom color is resolved to integer")
    
    # Check provider properties directly on the provider object
    self.assert_equal(custom_provider.cycle_period, 2000, "Custom cycle period is 2000ms")
    self.assert_equal(custom_provider.transition_type, animation.SINE, "Custom transition type is sine")
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
    anim.render(frame, 0, mock_engine.strip_length)
    var pixel_color = frame.get_pixel_color(0)
    self.assert_equal(pixel_color != 0, true, "Start color is not zero")
    
    # Test at middle - update engine time and get color
    mock_engine.set_time(500)
    anim.update(500)  # 50% through cycle
    anim.render(frame, 500, mock_engine.strip_length)
    var middle_color = frame.get_pixel_color(0)
    self.assert_equal(middle_color != 0, true, "Middle color is not zero")
    
    # Test at end - update engine time and get color
    mock_engine.set_time(1000)
    anim.update(1000)  # 100% through cycle
    anim.render(frame, 1000, mock_engine.strip_length)
    var end_color = frame.get_pixel_color(0)
    self.assert_equal(end_color != 0, true, "End color is not zero")
    
    # Test looping - should be back to start color
    mock_engine.set_time(2000)
    anim.update(2000)  # After another full cycle
    anim.render(frame, 2000, mock_engine.strip_length)
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
    self.assert_equal(provider.transition_type, animation.LINEAR, "Default transition type is linear")
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
    
    # Value-based colors now always use 0-255 range
    
    # Test value-based color generation (now always 0-255 range)
    provider.start()
    provider.update()
    print(f"{provider._slots_arr=} {provider._value_arr=}")
    var color_0 = provider.get_color_for_value(0, 0)
    var color_128 = provider.get_color_for_value(128, 0)
    var color_255 = provider.get_color_for_value(255, 0)
    
    self.assert_equal(color_0 != nil, true, "Color at value 0 is not nil")
    self.assert_equal(color_128 != nil, true, "Color at value 128 is not nil")
    self.assert_equal(color_255 != nil, true, "Color at value 255 is not nil")
    
    # Colors should be different
    self.assert_equal(color_0 != color_128, true, "Color at 0 differs from color at 128")
    self.assert_equal(color_128 != color_255, true, "Color at 128 differs from color at 255")
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
    provider.start()
    provider.update()
    
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
    provider.update(0)
    
    # Now colors should change over time again
    var time_color_0 = provider.produce_value("color", 0)
    var time_color_500 = provider.produce_value("color", 500)
    # Note: Colors may be the same depending on palette resolution, so just check they're valid
    self.assert_equal(time_color_0 != 0, true, "Time-based color at 0 is valid")
  end
  
  def test_sine_mode()
    # Test SINE mode for smooth ease-in/ease-out transitions
    var palette = bytes("00FF0000" "FF0000FF")  # Red at 0, Blue at 255
    
    # Create LINEAR provider
    var provider_linear = animation.rich_palette(mock_engine)
    provider_linear.palette = palette
    provider_linear.cycle_period = 1000
    provider_linear.transition_type = animation.LINEAR
    provider_linear.brightness = 255
    provider_linear.start(0)
    
    # Create SINE provider
    var provider_sine = animation.rich_palette(mock_engine)
    provider_sine.palette = palette
    provider_sine.cycle_period = 1000
    provider_sine.transition_type = animation.SINE
    provider_sine.brightness = 255
    provider_sine.start(0)
    
    # Test that transition_type is set correctly
    self.assert_equal(provider_linear.transition_type, animation.LINEAR, "LINEAR mode is set")
    self.assert_equal(provider_sine.transition_type, animation.SINE, "SINE mode is set")
    
    # Test at 0% - should be identical (Red)
    var linear_0 = provider_linear.produce_value("color", 0)
    var sine_0 = provider_sine.produce_value("color", 0)
    self.assert_equal(linear_0, sine_0, "LINEAR and SINE identical at 0%")
    
    # Test at 10% - SINE should have less blue (slower start)
    var linear_100 = provider_linear.produce_value("color", 100)
    var sine_100 = provider_sine.produce_value("color", 100)
    var linear_blue_100 = linear_100 & 0xFF
    var sine_blue_100 = sine_100 & 0xFF
    
    self.assert_equal(linear_blue_100 > sine_blue_100, true, "SINE slower than LINEAR at 10%")
    
    # Test at 25% - SINE should still be slower
    var linear_250 = provider_linear.produce_value("color", 250)
    var sine_250 = provider_sine.produce_value("color", 250)
    var linear_blue_250 = linear_250 & 0xFF
    var sine_blue_250 = sine_250 & 0xFF
    
    self.assert_equal(linear_blue_250 > sine_blue_250, true, "SINE slower than LINEAR at 25%")
    
    # Test at 50% - both should be at midpoint (approximately)
    var linear_500 = provider_linear.produce_value("color", 500)
    var sine_500 = provider_sine.produce_value("color", 500)
    
    # Colors should be very close at midpoint
    self.assert_approx_equal(linear_500, sine_500, "LINEAR and SINE similar at 50%")
    
    # Test ease-in behavior: change from 0-100ms should be smaller than 100-250ms for SINE
    var sine_blue_0 = sine_0 & 0xFF
    
    var change_0_100 = sine_blue_100 - sine_blue_0
    var change_100_250 = sine_blue_250 - sine_blue_100
    
    self.assert_equal(change_0_100 < change_100_250, true, "SINE has ease-in (slow start)")
  end
  
  def test_sine_mode_value_based()
    # Test SINE mode with value-based interpolation
    var palette = bytes("00FF0000" "FF0000FF")  # Red to Blue
    var provider = animation.rich_palette(mock_engine)
    provider.palette = palette
    provider.cycle_period = 0  # Value-based mode
    provider.transition_type = animation.SINE
    provider.start()
    provider.update()
    
    # Get colors at different values (now using 0-255 range)
    var color_0 = provider.get_color_for_value(0, 0)
    var color_64 = provider.get_color_for_value(64, 0)
    var color_128 = provider.get_color_for_value(128, 0)
    var color_192 = provider.get_color_for_value(192, 0)
    var color_255 = provider.get_color_for_value(255, 0)
    
    # Extract blue channel
    var blue_0 = color_0 & 0xFF
    var blue_64 = color_64 & 0xFF
    var blue_128 = color_128 & 0xFF
    var blue_192 = color_192 & 0xFF
    var blue_255 = color_255 & 0xFF
    
    # Test that we have a smooth S-curve
    # Change from 0-64 should be smaller than 64-128 (ease-in)
    var change_0_64 = blue_64 - blue_0
    var change_64_128 = blue_128 - blue_64
    self.assert_equal(change_0_64 < change_64_128, true, "Value-based SINE has ease-in")
    
    # Change from 128-192 should be larger than 192-255 (ease-out)
    var change_128_192 = blue_192 - blue_128
    var change_192_255 = blue_255 - blue_192
    self.assert_equal(change_128_192 > change_192_255, true, "Value-based SINE has ease-out")
    
    # Midpoint should be approximately 128
    self.assert_approx_equal(blue_128, 128, "Value-based SINE midpoint is ~128")
  end
end

# Run the tests
RichPaletteAnimationTest()

# Return success if we got this far
return true