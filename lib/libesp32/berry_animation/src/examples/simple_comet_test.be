# Simple Comet Test
# Basic test to verify comet animation works

import animation

print("=== Simple Comet Test ===")

# Create a frame buffer
var frame = animation.frame_buffer(10)
print("Created frame buffer:", frame)

# Create a simple comet
var comet = animation.comet.solid(0xFFFF0000, 3, 256, 10, 1)  # Red, 3-pixel tail, slow
print("Created comet:", comet)

# Start the animation
comet.start()
print("Started comet")

# Update and render
comet.update(0)
print(f"Updated comet - head_position: {comet.head_position}")

# Clear frame and render
frame.clear()
var rendered = comet.render(frame, tasmota.millis())
print("Rendered:", rendered)

# Check pixels
print("Frame buffer contents:")
for i: 0..9
  var color = frame.get_pixel_color(i)
  if color != 0
    print(f"  Pixel {i}: 0x{color:08X}")
  end
end

print("=== Simple Comet Test Complete ===")