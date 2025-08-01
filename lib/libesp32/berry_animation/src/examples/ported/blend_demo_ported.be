#
# Direct port of leds_blend_demo.be to new animation framework
# Original: Uses old animate.frame() with manual blending
# Ported: Uses new FrameBuffer with integrated blending capabilities
#
# Visual effect: Demonstrates frame buffer blending with gradient overlay
#

import animation

var LEDS_LENGTH = 25

# Create LED strip (same as original)
var strip = global.Leds(LEDS_LENGTH, gpio.pin(gpio.WS2812, 0))
var bri = 70

# Create animation engine
var engine = animation.create_engine(strip)

# Create frame buffers (replaces animate.frame)
var back_frame = animation.frame_buffer(LEDS_LENGTH)
var front_frame = animation.frame_buffer(LEDS_LENGTH)

# Fill background frame with red color at 80 opacity (replaces back.fill_pixels)
back_frame.fill(0xFF, 0x22, 0x00, 80)  # Red with alpha 80

# Fill front frame with green gradient (replaces the for loop)
for i: 0..LEDS_LENGTH-1
  var alpha = int(tasmota.scale_uint(i, 0, LEDS_LENGTH-1, 0, 255))
  front_frame.set_pixel(i, 0x00, 0xFF, 0x00, alpha)  # Green with varying alpha
end

# Blend frames (replaces back.blend_pixels)
back_frame.blend(front_frame, 255)

# Display debug information (same as original)
print("front=", front_frame.pixels.tohex())
print("back =", back_frame.pixels.tohex())

# Get strip buffer and apply blended result
var pixels_buffer = strip.pixels_buffer()
print("pixs =", pixels_buffer.tohex())

# Apply brightness and paste to strip buffer (replaces back.paste_pixels)
# Note: In new framework, this is typically handled by the engine
# For demonstration, we'll manually copy the blended result
for i: 0..LEDS_LENGTH-1
  var pixel = back_frame.get_pixel(i)
  var r = (pixel >> 16) & 0xFF
  var g = (pixel >> 8) & 0xFF  
  var b = pixel & 0xFF
  var a = (pixel >> 24) & 0xFF
  
  # Apply brightness scaling
  r = int(tasmota.scale_uint(r * bri, 0, 255 * 100, 0, 255))
  g = int(tasmota.scale_uint(g * bri, 0, 255 * 100, 0, 255))
  b = int(tasmota.scale_uint(b * bri, 0, 255 * 100, 0, 255))
  
  # Set pixel in strip buffer (GRB format for WS2812)
  var offset = i * 3
  pixels_buffer[offset] = g      # Green
  pixels_buffer[offset + 1] = r  # Red  
  pixels_buffer[offset + 2] = b  # Blue
end

# Update strip
strip.dirty()
strip.show()

print("Blend demo ported - frame buffer blending demonstration")
print("Background: Red with alpha 80")
print("Foreground: Green gradient with varying alpha")
print("Result: Blended frames displayed on strip")

# Alternative: Using the animation engine approach
print("\n--- Alternative using Animation Engine ---")

# Create animations that demonstrate the same blending
var bg_anim = animation.solid(0x50FF2200, 0, 0, true, "red_background")  # Red with alpha
var fg_anim = animation.filled_animation(
  # Create a custom color provider that generates the green gradient
  def(time_ms) 
    # This would need a custom gradient provider - simplified for demo
    return 0x8000FF00  # Green with alpha
  end,
  10, 0, true, "green_overlay"
)

engine.add_animation(bg_anim)
engine.add_animation(fg_anim)
engine.start()

print("Animation engine version running - demonstrates same blending concept")
print("Press Ctrl+C to stop")