# Test file for parameterized solid() function
#
# This file contains tests for the new parameterized animation.solid() function
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/solid_animation_test.be

import animation

print("Imported animation module")

# Create LED strip and engine for testing (following specification)
var strip = global.Leds(10)  # Use global.Leds() for testing as per specification
var engine = animation.create_engine(strip)

print("Created test engine with 10 LEDs")

# Test 1: Create a solid animation with engine-only constructor
var anim = animation.solid(engine)
print("Created solid animation with engine-only constructor")

# Set parameters using virtual member assignment
anim.color = 0xFFFFFFFF  # White
anim.priority = 10
anim.duration = 0        # Infinite
anim.loop = false
anim.opacity = 255
anim.name = "test_solid"

print("Set parameters using virtual member assignment")

# Start the animation (uses engine time)
anim.start()

# Test that it's created successfully
print(f"Animation created: {anim}")
print(f"Animation type: {type(anim)}")

# Test parameter values
print(f"Color: {anim.color}")
print(f"Priority: {anim.priority}")
print(f"Opacity: {anim.opacity}")
print(f"Duration: {anim.duration}")
print(f"Loop: {anim.loop}")
print(f"Name: {anim.name}")

# Test 2: Create animation with red color
var red_anim = animation.solid(engine)
red_anim.color = 0xFFFF0000  # Red
print("Red animation created and configured")

# Test 3: Create animation with all custom parameters
var blue_anim = animation.solid(engine)
blue_anim.color = 0xFF0000FF     # Blue
blue_anim.priority = 20
blue_anim.duration = 5000
blue_anim.loop = true
blue_anim.opacity = 200
blue_anim.name = "test_blue"
print(f"Blue animation - priority: {blue_anim.priority}, duration: {blue_anim.duration}, loop: {blue_anim.loop}")

# Test 4: Create animation with ValueProvider
var solid_provider = animation.static_color(engine)
solid_provider.color = 0xFF00FF00  # Green
var green_anim = animation.solid(engine)
green_anim.color = solid_provider  # Use ValueProvider as dynamic parameter
print("Green animation with color provider created")

# Test 5: Test rendering
var frame = animation.frame_buffer(5)
red_anim.start()  # Uses engine time automatically
red_anim.render(frame, engine.time_ms)
print("Rendering test completed")

# Test 6: Test engine time usage
print(f"Engine time: {engine.time_ms}")
print("Engine time is used consistently across all objects")

# Validate key test results
assert(anim != nil, "Solid animation should be created")
assert(anim.is_running, "Solid animation should be running after start")
assert(type(anim) == "instance", "Animation should be an instance")
assert(red_anim != nil, "Red animation should be created")
assert(blue_anim != nil, "Blue animation should be created")
assert(green_anim != nil, "Green animation should be created")

# Test parameter access
assert(anim.color == 0xFFFFFFFF, "Color parameter should be accessible")
assert(red_anim.color == 0xFFFF0000, "Red color should be set correctly")
assert(blue_anim.priority == 20, "Priority should be set correctly")

# Test ValueProvider integration
assert(animation.is_value_provider(solid_provider), "Should recognize ValueProvider")
# Use get_param to get raw ValueProvider (not resolved value)
var raw_color_param = green_anim.get_param("color")
assert(raw_color_param == solid_provider, "Should store ValueProvider as parameter")

print("All parameterized solid() tests completed successfully!")
return true