# FrameBuffer Demo
#
# This example demonstrates the basic usage of the FrameBuffer class.

import global
import tasmota
import animation

# Create a frame buffer with 10 pixels
var fb = animation.frame_buffer(10)

# Clear the buffer (set all pixels to transparent black)
fb.clear()
print(f"After clear: {fb.tohex()}")

# Set individual pixels
fb.set_pixel_color(0, 0xFFFF0000)    # Red
fb.set_pixel_color(1, 0xFF00FF00)    # Green
fb.set_pixel_color(2, 0xFF0000FF)    # Blue
fb.set_pixel_color(3, 0xFFFFFF00)    # Yellow
fb.set_pixel_color(4, 0xFFFF00FF)    # Purple
print(f"After setting pixels: {fb.tohex()}")

# Fill the buffer with a single color
fb.fill_pixels(0xFF00FFFF)  # Cyan
print(f"After fill with cyan: {fb.tohex()}")

# Create a second buffer for blending
var fb2 = animation.frame_buffer(10)
fb2.fill_pixels(0x80FF0000)  # Red with 50% alpha

# Blend the two buffers using per-pixel alpha
fb.blend_pixels(fb2)  # Blend using per-pixel alpha with default normal blend mode
print(f"After blending with red using per-pixel alpha (normal mode): {fb.tohex()}")

# Create a gradient fill
fb.gradient_fill(0xFFFF0000, 0xFF0000FF, 0, 9)  # Red to blue gradient
print(f"After gradient fill: {fb.tohex()}")

# Demonstrate region-specific blending
var region_buffer = animation.frame_buffer(10)
region_buffer.fill_pixels(0xC800FF00)  # Green with ~78% alpha (200/255)
fb.blend_pixels(region_buffer, fb.BLEND_MODE_NORMAL, 0, 4)  # Blend green into first half
print(f"After blending green into first half: {fb.tohex()}")

# Using array-like access
fb[9] = 0xFFFF0000  # Set last pixel to red
print(f"After setting last pixel to red: {fb.tohex()}")

# Demonstrate apply_opacity method
print("\nDemonstrating apply_opacity method:")

# Create a test buffer with various alpha values
var opacity_demo = animation.frame_buffer(5)
opacity_demo.set_pixel_color(0, 0xFF0000FF)  # Red, full alpha
opacity_demo.set_pixel_color(1, 0x800000FF)  # Red, 50% alpha
opacity_demo.set_pixel_color(2, 0x400000FF)  # Red, 25% alpha
opacity_demo.set_pixel_color(3, 0x200000FF)  # Red, 12.5% alpha
opacity_demo.set_pixel_color(4, 0x100000FF)  # Red, 6.25% alpha
print(f"Original alpha values: {opacity_demo.dump()}")

# Reduce opacity by 50%
var reduced_demo = opacity_demo.copy()
reduced_demo.apply_opacity(128)  # 50% opacity
print(f"After 50% opacity reduction: {reduced_demo.dump()}")

# Increase opacity by 50%
var increased_demo = opacity_demo.copy()
increased_demo.apply_opacity(384)  # 150% opacity (256 + 128)
print(f"After 50% opacity increase: {increased_demo.dump()}")

# Make fully transparent
var transparent_demo = opacity_demo.copy()
transparent_demo.apply_opacity(0)  # 0% opacity
print(f"After making fully transparent: {transparent_demo.dump()}")

# Make maximum opaque
var max_demo = opacity_demo.copy()
max_demo.apply_opacity(511)  # Maximum opacity
print(f"After maximum opacity: {max_demo.dump()}")

print("Frame buffer demo completed!")