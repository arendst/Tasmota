# Fire Animation Test
# Tests the FireAnimation class functionality

import animation

print("=== Fire Animation Test ===")

# Create engine and LED strip for testing
var strip = global.Leds(30)  # Use built-in LED strip for testing
var engine = animation.create_engine(strip)

# Test 1: Basic Fire Animation Creation
print("\n1. Testing basic fire animation creation...")
var fire = animation.fire_animation(engine)
# Set parameters using virtual member assignment
fire.intensity = 180
fire.flicker_speed = 8
fire.flicker_amount = 100
fire.cooling_rate = 55
fire.sparking_rate = 120
fire.priority = 255
fire.name = "test_fire"

print(f"Created fire animation: {fire}")
print(f"Initial state - running: {fire.is_running}, priority: {fire.priority}")

# Test 2: Parameter Validation
print("\n2. Testing parameter validation...")
var result1 = fire.set_param("intensity", 200)
var result2 = fire.set_param("intensity", 300)  # Should fail - out of range
var result3 = fire.set_param("flicker_speed", 15)
var result4 = fire.set_param("flicker_speed", 25)  # Should fail - out of range

print(f"Set intensity to 200: {result1}")
print(f"Set intensity to 300 (invalid): {result2}")
print(f"Set flicker_speed to 15: {result3}")
print(f"Set flicker_speed to 25 (invalid): {result4}")

# Test 3: Factory Methods (if they exist)
print("\n3. Testing direct class instantiation...")
var fire_classic = animation.fire_animation(engine)
fire_classic.intensity = 150
fire_classic.priority = 30

var fire_solid = animation.fire_animation(engine)
fire_solid.color = 0xFFFF4500  # Orange red
fire_solid.intensity = 180
fire_solid.priority = 30

var fire_palette = animation.fire_animation(engine)
fire_palette.intensity = 200
fire_palette.priority = 30

print(f"Classic fire: {fire_classic}")
print(f"Solid fire: {fire_solid}")
print(f"Palette fire: {fire_palette}")

# Test 4: Animation Lifecycle
print("\n4. Testing animation lifecycle...")
fire.start()
print(f"After start - running: {fire.is_running}")

# Simulate some time passing and updates
engine.time_ms = 1000  # Set engine time for testing
var start_time = engine.time_ms
var current_time = start_time

for i: 0..5
  current_time += 125  # 125ms intervals (8 Hz = 125ms period)
  engine.time_ms = current_time  # Update engine time
  var still_running = fire.update(current_time)
  print(f"Update {i+1} at {current_time}ms - still running: {still_running}")
end

# Test 5: Frame Buffer Rendering
print("\n5. Testing frame buffer rendering...")
var frame = animation.frame_buffer(30)
frame.clear()

# Render the fire animation
var rendered = fire.render(frame, engine.time_ms)
print(f"Rendered to frame buffer: {rendered}")

# Check that some pixels have been set (fire should create non-black pixels)
var non_black_pixels = 0
for i: 0..29
  var color = frame.get_pixel_color(i)
  if color != 0xFF000000  # Not black
    non_black_pixels += 1
  end
end
print(f"Non-black pixels after rendering: {non_black_pixels}")

# Test 6: Parameter Updates
print("\n6. Testing parameter updates...")
print(f"Original intensity: {fire.intensity}")
fire.intensity = 100
print(f"Updated intensity: {fire.intensity}")

print(f"Original flicker_amount: {fire.flicker_amount}")
fire.flicker_amount = 150
print(f"Updated flicker_amount: {fire.flicker_amount}")

# Test 7: Color Updates
print("\n7. Testing color updates...")
var original_color = fire.color
print(f"Original color type: {type(original_color)}")

# Set to solid color
fire.color = 0xFFFF0000  # Red
print("Set to solid red color")

# Set back to fire palette
var fire_palette = animation.rich_palette(engine)
fire_palette.palette = animation.PALETTE_FIRE
fire_palette.cycle_period = 5000
fire_palette.transition_type = 1  # Use sine transition (smooth)
fire_palette.brightness = 255
fire_palette.range_min = 0
fire_palette.range_max = 255
fire.color = fire_palette
print("Set back to fire palette")

# Test 9: Multiple Fire Animations
print("\n9. Testing multiple fire animations...")
var fire1 = animation.fire_animation(engine)
fire1.intensity = 180
fire1.priority = 15

var fire2 = animation.fire_animation(engine)
fire2.color = 0xFFFF4500
fire2.intensity = 150
fire2.priority = 15

fire1.start()
fire2.start()

# Update both animations
current_time += 125
fire1.update(current_time)
fire2.update(current_time)

print(f"Fire1 running: {fire1.is_running}")
print(f"Fire2 running: {fire2.is_running}")

# Test 10: Edge Cases
print("\n10. Testing edge cases...")

# Very small strip
var tiny_strip = global.Leds(1)
var tiny_engine = animation.create_engine(tiny_strip)
var tiny_fire = animation.fire_animation(tiny_engine)
tiny_fire.intensity = 180
tiny_fire.priority = 1
tiny_fire.start()
tiny_engine.time_ms = current_time + 125
tiny_fire.update(current_time + 125)
var tiny_frame = animation.frame_buffer(1)
tiny_fire.render(tiny_frame, tiny_engine.time_ms)
print("Tiny fire (1 pixel) created and rendered successfully")

# Zero intensity
var dim_strip = global.Leds(10)
var dim_engine = animation.create_engine(dim_strip)
var dim_fire = animation.fire_animation(dim_engine)
dim_fire.intensity = 0
dim_fire.priority = 10
dim_fire.start()
dim_engine.time_ms = current_time + 250
dim_fire.update(current_time + 250)
var dim_frame = animation.frame_buffer(10)
dim_fire.render(dim_frame, dim_engine.time_ms)
print("Dim fire (0 intensity) created and rendered successfully")

print("\n=== Fire Animation Test Complete ===")

# Validate key test results
assert(fire != nil, "Fire animation should be created")
assert(fire.is_running, "Fire animation should be running after start")
assert(result1 == true, "Valid intensity parameter should be accepted")
assert(result2 == false, "Invalid intensity parameter should be rejected")
assert(result3 == true, "Valid flicker_speed parameter should be accepted")
assert(result4 == false, "Invalid flicker_speed parameter should be rejected")
assert(rendered == true, "Render should return true when animation is running")
assert(fire.intensity == 100, "Intensity should be updated to 100")
assert(fire.flicker_amount == 150, "Flicker amount should be updated to 150")

print("All tests passed successfully!")
return true