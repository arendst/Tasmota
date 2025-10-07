# Test file for RichPaletteAnimation class
#
# This file contains tests for the new RichPaletteAnimation class with parameter forwarding
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/rich_palette_animation_class_test.be

import animation

print("Imported animation module")

# Create LED strip and engine for testing (following specification)
var strip = global.Leds(10)  # Use global.Leds() for testing as per specification
var engine = animation.create_engine(strip)

print("Created test engine with 10 LEDs")

# Test 1: Create a rich palette animation with engine-only constructor
var anim = animation.rich_palette_animation(engine)
print("Created rich palette animation with engine-only constructor")

# Test that it's created successfully
print(f"Animation created: {anim}")
print(f"Animation type: {type(anim)}")
print(f"Animation name: {anim.name}")

# Test 2: Set parameters using virtual member assignment (parameter forwarding)
anim.palette = bytes("00FF0000" "80FFFF00" "FF0000FF")  # Red to Yellow to Blue
anim.cycle_period = 3000
anim.transition_type = 1  # sine
anim.brightness = 200
anim.range_min = 0
anim.range_max = 100

# Set Animation base parameters
anim.priority = 15
anim.duration = 10000
anim.loop = true
anim.opacity = 255

print("Set parameters using virtual member assignment")

# Test parameter values (should be forwarded to internal provider)
print(f"Palette: {bool(anim.palette)}")
print(f"Cycle period: {anim.cycle_period}")
print(f"Transition type: {anim.transition_type}")
print(f"Brightness: {anim.brightness}")
print(f"Range min: {anim.range_min}")
print(f"Range max: {anim.range_max}")

# Test Animation base parameters
print(f"Priority: {anim.priority}")
print(f"Duration: {anim.duration}")
print(f"Loop: {anim.loop}")
print(f"Opacity: {anim.opacity}")

# Test 3: Verify parameter forwarding to internal color provider
print(f"Internal provider cycle period: {anim.color_provider.cycle_period}")
print(f"Internal provider brightness: {anim.color_provider.brightness}")
print(f"Internal provider transition type: {anim.color_provider.transition_type}")

# Test 4: Start the animation (uses engine time)
anim.start()
print(f"Animation running: {anim.is_running}")

# Test 5: Test rendering
var frame = animation.frame_buffer(5)
anim.render(frame, engine.time_ms)
var pixel_color = frame.get_pixel_color(0)
print(f"Rendered pixel color: {pixel_color}")

# Test 6: Test color changes over time
engine.time_ms = 0
anim.start(0)
anim.render(frame, 0)
var color_t0 = frame.get_pixel_color(0)

engine.time_ms = 1500  # Half cycle
anim.render(frame, 1500)
var color_t1500 = frame.get_pixel_color(0)

engine.time_ms = 3000  # Full cycle
anim.render(frame, 3000)
var color_t3000 = frame.get_pixel_color(0)

print(f"Color at t=0: {color_t0}")
print(f"Color at t=1500: {color_t1500}")
print(f"Color at t=3000: {color_t3000}")

# Test 7: Test parameter change propagation
anim.cycle_period = 2000  # Change cycle period
print(f"Changed cycle period to: {anim.cycle_period}")
print(f"Internal provider cycle period: {anim.color_provider.cycle_period}")

anim.brightness = 100  # Change brightness
print(f"Changed brightness to: {anim.brightness}")
print(f"Internal provider brightness: {anim.color_provider.brightness}")

# Test 8: Test with different palette
var rainbow_palette = bytes(
  "00FF0000"    # Red (value 0)
  "24FFA500"    # Orange (value 36)
  "49FFFF00"    # Yellow (value 73)
  "6E00FF00"    # Green (value 110)
  "920000FF"    # Blue (value 146)
  "B74B0082"    # Indigo (value 183)
  "DBEE82EE"    # Violet (value 219)
  "FFFF0000"    # Red (value 255)
)

var rainbow_anim = animation.rich_palette_animation(engine)
rainbow_anim.palette = rainbow_palette
rainbow_anim.cycle_period = 5000
rainbow_anim.brightness = 255
print("Created rainbow animation with custom palette")

# Test 9: Test static mode (cycle_period = 0)
var static_anim = animation.rich_palette_animation(engine)
static_anim.palette = rainbow_palette
static_anim.cycle_period = 0  # Static mode
static_anim.brightness = 150
print("Created static animation (cycle_period = 0)")

# Test 10: Test access to internal color provider methods
var css_gradient = anim.color_provider.to_css_gradient()
print(f"CSS gradient available: {bool(css_gradient)}")

# Validate key test results
assert(anim != nil, "Rich palette animation should be created")
assert(type(anim) == "instance", "Animation should be an instance")
assert(anim.name == "rich_palette", "Animation should have correct default name")

# Test parameter forwarding
assert(anim.cycle_period == 2000, "Cycle period should be forwarded")
assert(anim.brightness == 100, "Brightness should be forwarded")
assert(anim.color_provider.cycle_period == 2000, "Internal provider should receive forwarded cycle period")
assert(anim.color_provider.brightness == 100, "Internal provider should receive forwarded brightness")

# Test Animation base parameters
assert(anim.priority == 15, "Priority should be set correctly")
assert(anim.duration == 10000, "Duration should be set correctly")
assert(anim.loop == true, "Loop should be set correctly")
assert(anim.opacity == 255, "Opacity should be set correctly")

# Test color provider integration
assert(anim.color_provider != nil, "Internal color provider should exist")
assert(animation.is_value_provider(anim.color_provider), "Internal provider should be a ValueProvider")

# Test that color parameter is set to internal provider
var raw_color_param = anim.get_param("color")
assert(raw_color_param == anim.color_provider, "Color parameter should be internal provider")

# Test rendering produces valid colors
assert(color_t0 != 0, "Should produce valid colors at t=0")
assert(color_t1500 != 0, "Should produce valid colors at t=1500")
assert(color_t3000 != 0, "Should produce valid colors at t=3000")

# Test different animations are independent
assert(rainbow_anim != nil, "Rainbow animation should be created")
assert(static_anim != nil, "Static animation should be created")
assert(rainbow_anim.cycle_period == 5000, "Rainbow animation should have correct cycle period")
assert(static_anim.cycle_period == 0, "Static animation should have cycle period 0")

print("All RichPaletteAnimation class tests completed successfully!")
return true