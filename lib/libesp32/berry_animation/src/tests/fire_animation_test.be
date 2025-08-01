# Fire Animation Test
# Tests the FireAnimation class functionality

import animation

print("=== Fire Animation Test ===")

# Test 1: Basic Fire Animation Creation
print("\n1. Testing basic fire animation creation...")
var fire = animation.fire_animation(nil, 180, 8, 100, 55, 120, 30, 10, 0, true, "test_fire")
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

# Test 3: Factory Methods
print("\n3. Testing factory methods...")
var fire_classic = animation.fire_animation.classic(150, 30, 5)
var fire_solid = animation.fire_animation.solid(0xFFFF4500, 180, 30, 5)  # Orange red
var fire_palette = animation.fire_animation.palette(animation.PALETTE_FIRE, 200, 30, 5)

print(f"Classic fire: {fire_classic}")
print(f"Solid fire: {fire_solid}")
print(f"Palette fire: {fire_palette}")

# Test 4: Animation Lifecycle
print("\n4. Testing animation lifecycle...")
fire.start()
print(f"After start - running: {fire.is_running}")

# Simulate some time passing and updates
var start_time = 1000
var current_time = start_time

for i: 0..5
  current_time += 125  # 125ms intervals (8 Hz = 125ms period)
  var still_running = fire.update(current_time)
  print(f"Update {i+1} at {current_time}ms - still running: {still_running}")
end

# Test 5: Frame Buffer Rendering
print("\n5. Testing frame buffer rendering...")
var frame = animation.frame_buffer(30)
frame.clear()

# Render the fire animation
var rendered = fire.render(frame, tasmota.millis())
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
print(f"Original intensity: {fire.get_param('intensity')}")
fire.set_intensity(100)
print(f"Updated intensity: {fire.get_param('intensity')}")

print(f"Original flicker_amount: {fire.get_param('flicker_amount')}")
fire.set_flicker_amount(150)
print(f"Updated flicker_amount: {fire.get_param('flicker_amount')}")

# Test 7: Color Updates
print("\n7. Testing color updates...")
var original_color = fire.color
print(f"Original color type: {type(original_color)}")

# Set to solid color
fire.set_color(0xFFFF0000)  # Red
print("Set to solid red color")

# Set back to fire palette
var fire_palette = animation.rich_palette_color_provider(animation.PALETTE_FIRE, 5000, 1, 255)
fire_palette.set_range(0, 255)
fire.set_color(fire_palette)
print("Set back to fire palette")

# Test 8: Animation Stop/Start
print("\n8. Testing stop/start...")
fire.stop()
print(f"After stop - running: {fire.is_running}")

fire.start()
print(f"After restart - running: {fire.is_running}")

# Test 9: Multiple Fire Animations
print("\n9. Testing multiple fire animations...")
var fire1 = animation.fire_animation.classic(180, 15, 10)
var fire2 = animation.fire_animation.solid(0xFFFF4500, 150, 15, 5)

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
var tiny_fire = animation.fire_animation.classic(180, 1, 5)
tiny_fire.start()
tiny_fire.update(current_time + 125)
var tiny_frame = animation.frame_buffer(1)
tiny_fire.render(tiny_frame)
print("Tiny fire (1 pixel) created and rendered successfully")

# Zero intensity
var dim_fire = animation.fire_animation.classic(0, 10, 5)
dim_fire.start()
dim_fire.update(current_time + 250)
var dim_frame = animation.frame_buffer(10)
dim_fire.render(dim_frame)
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
assert(fire.get_param('intensity') == 100, "Intensity should be updated to 100")
assert(fire.get_param('flicker_amount') == 150, "Flicker amount should be updated to 150")

print("All tests passed successfully!")
return true