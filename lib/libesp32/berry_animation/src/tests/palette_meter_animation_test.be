# Test suite for palette_meter (palette_meter)
#
# Tests the VU meter style animation with gradient colors and peak hold.

import animation

# Test basic creation
def test_gradient_meter_creation()
  print("Testing palette_meter creation...")

  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)

  var meter = animation.palette_meter(engine)
  assert(meter != nil, "Should create gradient meter animation")
  assert(meter.level == 255, "Should default to level 255")
  assert(meter.peak_hold == 1000, "Should default to peak_hold 1000ms")
  assert(meter.shift_period == 0, "Should default to static gradient")

  print("✓ palette_meter creation test passed")
end

# Test level parameter
def test_gradient_meter_level()
  print("Testing palette_meter level...")

  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  var meter = animation.palette_meter(engine)

  # Test level setting
  meter.level = 128
  assert(meter.level == 128, "Should set level to 128")

  meter.level = 255
  assert(meter.level == 255, "Should set level to 255")

  meter.level = 0
  assert(meter.level == 0, "Should set level to 0")

  # Test validation
  assert(meter.set_param("level", 300) == false, "Should reject level > 255")
  assert(meter.set_param("level", -1) == false, "Should reject level < 0")

  print("✓ palette_meter level test passed")
end

# Test peak hold functionality
def test_gradient_meter_peak_hold()
  print("Testing palette_meter peak hold...")

  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  var meter = animation.palette_meter(engine)

  # Enable peak hold
  meter.peak_hold = 1000  # 1 second hold
  assert(meter.peak_hold == 1000, "Should set peak_hold to 1000ms")

  # Start animation
  meter.start_time = 1000
  meter.start(1000)

  # Set initial level and update at time 1000
  meter.level = 200
  meter.update(1000)
  assert(meter.peak_level == 200, "Peak should track level")
  # peak_time is now 1000

  # Lower level at time 1500 - peak should stay (500ms elapsed, within 1000ms hold)
  meter.level = 100
  meter.update(1500)
  assert(meter.peak_level == 200, "Peak should hold at 200")

  # At time 2100 (1100ms since peak was set at 1000), hold has expired
  meter.update(2100)
  assert(meter.peak_level == 100, "Peak should drop to current level after hold expires")

  print("✓ palette_meter peak hold test passed")
end

# Test rendering
def test_gradient_meter_rendering()
  print("Testing palette_meter rendering...")

  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  var meter = animation.palette_meter(engine)

  # Use a color provider (required for palette_gradient)
  var color_source = animation.rich_palette_color(engine)
  meter.color_source = color_source

  var frame = animation.frame_buffer(10, 1)

  # Start and update (color_source needs update to init LUT)
  meter.start_time = 0
  meter.start(0)
  color_source.update(0)
  meter.update(0)

  # Test 50% level (should fill ~5 pixels)
  meter.level = 128
  color_source.update(100)
  meter.update(100)
  meter.render(frame, 100, 10)

  # First pixels should have color, last pixels should be transparent/black
  var first_color = frame.get_pixel_color(0)
  assert(first_color != 0x00000000, "First pixel should have color")

  # Test 0% level (nothing filled)
  frame.fill_pixels(frame.pixels, 0x00000000)  # Clear frame
  meter.level = 0
  color_source.update(200)
  meter.update(200)
  meter.render(frame, 200, 10)

  print("✓ palette_meter rendering test passed")
end

# Test peak indicator rendering
def test_gradient_meter_peak_indicator()
  print("Testing palette_meter peak indicator...")

  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  var meter = animation.palette_meter(engine)

  # Use a color provider
  var color_source = animation.rich_palette_color(engine)
  meter.color_source = color_source
  meter.peak_hold = 2000

  var frame = animation.frame_buffer(10, 1)

  # Start animation
  meter.start_time = 0
  meter.start(0)
  color_source.update(0)

  # Set high level then lower it
  meter.level = 200
  meter.update(0)

  meter.level = 50
  color_source.update(100)
  meter.update(100)

  # Peak should still be at 200, current at 50
  assert(meter.peak_level == 200, "Peak should be at 200")
  assert(meter.level == 50, "Level should be at 50")

  # Render and check peak pixel is set
  meter.render(frame, 100, 10)

  # Peak pixel position (200/255 * 9 ≈ 7)
  var peak_pixel = tasmota.scale_uint(200, 0, 255, 0, 9)
  var peak_color = frame.get_pixel_color(peak_pixel)
  assert(peak_color != 0x00000000, "Peak indicator pixel should have color")

  print("✓ palette_meter peak indicator test passed")
end

# Test with color provider
def test_gradient_meter_with_color_provider()
  print("Testing palette_meter with color provider...")

  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  var meter = animation.palette_meter(engine)

  # Use a color cycle provider
  var color_cycle = animation.color_cycle(engine)
  meter.color_source = color_cycle

  meter.level = 200
  meter.start_time = 0
  meter.start(0)
  color_cycle.update(0)
  meter.update(0)

  var frame = animation.frame_buffer(10, 1)
  var result = meter.render(frame, 0, 10)
  assert(result == true, "Should render with color provider")

  print("✓ palette_meter with color provider test passed")
end

# Test tostring
def test_gradient_meter_tostring()
  print("Testing palette_meter tostring...")

  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  var meter = animation.palette_meter(engine)
  meter.level = 150
  meter.peak_hold = 500

  var s = str(meter)
  assert(s != nil, "Should have string representation")
  assert(type(s) == "string", "Should be a string")

  print("✓ palette_meter tostring test passed")
end

# Run all tests
def run_palette_meter_animation_tests()
  print("=== palette_meter Tests ===")

  try
    test_gradient_meter_creation()
    test_gradient_meter_level()
    test_gradient_meter_peak_hold()
    test_gradient_meter_rendering()
    test_gradient_meter_peak_indicator()
    test_gradient_meter_with_color_provider()
    test_gradient_meter_tostring()

    print("=== All palette_meter tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

animation.run_palette_meter_animation_tests = run_palette_meter_animation_tests

run_palette_meter_animation_tests()

return run_palette_meter_animation_tests
