# Test file for FrameBuffer class
#
# This file contains tests for the FrameBuffer class
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/frame_buffer_test.be

import animation

print("Testing FrameBuffer...")

# Create a frame buffer with 10 pixels
var fb = animation.frame_buffer(10)
assert(fb.width == 10, "Frame buffer width should be 10")

# Test clear method
fb.clear()
assert(fb.tohex() == '00000000000000000000000000000000000000000000000000000000000000000000000000000000', "Clear should set all pixels to transparent black")

# Test set_pixel_color and get_pixel_color methods
fb.set_pixel_color(0, 0xFFFF0000)  # Set first pixel to red
assert(fb.get_pixel_color(0) == 0xFFFF0000, f"First pixel should be red (0x{fb.get_pixel_color(0) :08x})")

fb.set_pixel_color(1, 0xFF00FF00)  # Set second pixel to green
assert(fb.get_pixel_color(1) == 0xFF00FF00, f"Second pixel should be green (0x{fb.get_pixel_color(1) :08x})")

fb.set_pixel_color(2, 0xFF0000FF)  # Set third pixel to blue
assert(fb.get_pixel_color(2) == 0xFF0000FF, f"Third pixel should be blue (0x{fb.get_pixel_color(2) :08x})")

fb.set_pixel_color(3, 0xFFFFFF00)  # Set fourth pixel to yellow
assert(fb.get_pixel_color(3) == 0xFFFFFF00, f"Fourth pixel should be yellow (0x{fb.get_pixel_color(3) :08x})")

fb.set_pixel_color(4, 0x80FF00FF)  # Set fifth pixel to purple with 50% alpha
assert(fb.get_pixel_color(4) == 0x80FF00FF, f"Fifth pixel should be purple with 50% alpha (0x{fb.get_pixel_color(4) :08x})")

# Test fill_pixels method
fb.fill_pixels(fb.pixels, 0xFFFFFFFF)  # Fill with white

var all_white = true
for i: 0..9
  if fb.get_pixel_color(i) != 0xFFFFFFFF
    all_white = false
    break
  end
end
assert(all_white, "All pixels should be white")

# Test fill_pixels with color components
fb.fill_pixels(fb.pixels, 0xFF00FF00)  # Fill with green

var all_green = true
for i: 0..9
  if fb.get_pixel_color(i) != 0xFF00FF00
    all_green = false
    break
  end
end
assert(all_green, "All pixels should be green")

# Test blend_pixels method
var fb1 = animation.frame_buffer(10)
var fb2 = animation.frame_buffer(10)

fb1.fill_pixels(fb1.pixels, 0xFF0000FF)  # Fill fb1 with red (fully opaque)
fb2.fill_pixels(fb2.pixels, 0x80FF0000)  # Fill fb2 with blue at 50% alpha

# Blend fb2 into fb1 using per-pixel alpha
fb1.blend_pixels(fb1.pixels, fb2.pixels)

var all_blended = true
for i: 0..9
  var color = fb1.get_pixel_color(i)
  # With 50% alpha blue over red, we should get a purple blend
  # The exact color depends on the blending algorithm
  var a = (color >> 24) & 0xFF
  var r = (color >> 16) & 0xFF
  var g = (color >> 8) & 0xFF
  var b = color & 0xFF
  
  # Check that we have some red and some blue (purple-ish)
  if r == 0 || b == 0
    all_blended = false
    break
  end
end
assert(all_blended, "All pixels should be blended to purple-ish color")

# Test copy method
var fb_copy = fb1.copy()
assert(fb_copy.width == fb1.width, "Copied buffer should have the same width")

var all_copied = true
for i: 0..9
  if fb_copy.get_pixel_color(i) != fb1.get_pixel_color(i)
    all_copied = false
    break
  end
end
assert(all_copied, "All pixels should be copied correctly")

# Test blend_color method
fb1.fill_pixels(fb1.pixels, 0xFF0000FF)  # Fill fb1 with red
fb1.blend_color(fb1.pixels, 0x8000FF00)  # Blend with green at 50% alpha

var still_red = true
for i: 0..9
  if fb1.get_pixel_color(i) != 0xFF0000FF  # Red
    still_red = false
    break
  end
end
assert(!still_red, "Pixels should be blended with green")

# Test apply_brightness method
print("Testing apply_brightness method...")

# Test reducing brightness (0-255 range)
var brightness_test = animation.frame_buffer(5)
brightness_test.fill_pixels(brightness_test.pixels, 0xFFFF0000)  # Red with full brightness (255)
brightness_test.apply_brightness(brightness_test.pixels, 128)  # Apply 50% brightness

var reduced_pixel = brightness_test.get_pixel_color(0)
var reduced_r = (reduced_pixel >> 16) & 0xFF
assert(reduced_r == 128, f"Red component should be reduced to 128, got {reduced_r}")

# Test increasing brightness (256-511 range)
var increase_test = animation.frame_buffer(5)
increase_test.fill_pixels(increase_test.pixels, 0xFF008000)  # Green with 50% brightness (128)
increase_test.apply_brightness(increase_test.pixels, 384)  # Apply 1.5x brightness (384 = 256 + 128)

var increased_pixel = increase_test.get_pixel_color(0)
var increased_g = (increased_pixel >> 8) & 0xFF
# Should increase from 128 towards 255, but exact value depends on scaling
assert(increased_g > 128, f"Green component should be increased from 128, got {increased_g}")
assert(increased_g == 192, f"Green component should be increased to 192, got {increased_g}")
assert(increased_g <= 255, f"Green component should not exceed 255, got {increased_g}")

# Test zero brightness (fully black)
var black_test = animation.frame_buffer(5)
black_test.fill_pixels(black_test.pixels, 0xFFFF0000)  # Red with full brightness
black_test.apply_brightness(black_test.pixels, 0)  # Make fully black

var black_pixel = black_test.get_pixel_color(0)
var black_r = (black_pixel >> 16) & 0xFF
var black_g = (black_pixel >> 8) & 0xFF
var black_b = black_pixel & 0xFF
assert(black_r == 0, f"Red component should be 0 (black), got {black_r}")
assert(black_g == 0, f"Green component should be 0 (black), got {black_g}")
assert(black_b == 0, f"Blue component should be 0 (black), got {black_b}")

# Test maximum brightness (should cap at 255)
var max_test = animation.frame_buffer(5)
max_test.fill_pixels(max_test.pixels, 0xFF008000)  # Green with 50% brightness
max_test.apply_brightness(max_test.pixels, 511)  # Apply maximum brightness

var max_pixel = max_test.get_pixel_color(0)
var max_g = (max_pixel >> 8) & 0xFF
assert(max_g == 255, f"Green component should be capped at 255, got {max_g}")

# Test that alpha channel is preserved
var alpha_test = animation.frame_buffer(5)
alpha_test.fill_pixels(alpha_test.pixels, 0x80FF0000)  # Red with 50% alpha
alpha_test.apply_brightness(alpha_test.pixels, 128)  # Apply 50% brightness

var alpha_pixel = alpha_test.get_pixel_color(0)
var alpha_a = (alpha_pixel >> 24) & 0xFF
var alpha_r = (alpha_pixel >> 16) & 0xFF
assert(alpha_a == 128, f"Alpha should be preserved at 128, got {alpha_a}")
assert(alpha_r == 128, f"Red should be reduced to 128, got {alpha_r}")

# Test blend_pixels with region
fb1.fill_pixels(fb1.pixels, 0xFF0000FF)  # Fill fb1 with red (fully opaque)
fb2.fill_pixels(fb2.pixels, 0x8000FF00)  # Fill fb2 with green at 50% alpha

# Blend fb2 into fb1 using per-pixel alpha, but only for the first half
fb1.blend_pixels(fb1.pixels, fb2.pixels, 0, 4)

var first_half_blended = true
var second_half_original = true

for i: 0..4
  if fb1.get_pixel_color(i) == 0xFF0000FF  # Still red
    first_half_blended = false
    break
  end
end

for i: 5..9
  if fb1.get_pixel_color(i) != 0xFF0000FF  # Should be red
    second_half_original = false
    break
  end
end

assert(first_half_blended, "First half should be blended")
assert(second_half_original, "Second half should remain original")

# Test gradient_fill method
fb1.clear()
fb1.gradient_fill(fb1.pixels, 0xFFFF0000, 0xFF00FF00)  # Red to green gradient

var first_pixel_color = fb1.get_pixel_color(0)
var last_pixel_color = fb1.get_pixel_color(9)

assert(first_pixel_color == 0xFFFF0000, f"First pixel should be red (0x{first_pixel_color :08x})")
assert(last_pixel_color == 0xFF00FF00, f"Last pixel should be green (0x{last_pixel_color :08x})")

# Test apply_opacity method
print("Testing apply_opacity method...")

# Test reducing opacity (0-255 range)
var opacity_test = animation.frame_buffer(5)
opacity_test.fill_pixels(opacity_test.pixels, 0xFF0000FF)  # Red with full alpha (255)
opacity_test.apply_opacity(opacity_test.pixels, 128)  # Apply 50% opacity

var reduced_pixel = opacity_test.get_pixel_color(0)
var reduced_alpha = (reduced_pixel >> 24) & 0xFF
assert(reduced_alpha == 128, f"Alpha should be reduced to 128, got {reduced_alpha}")

# Test increasing opacity (256-511 range)
var increase_test = animation.frame_buffer(5)
increase_test.fill_pixels(increase_test.pixels, 0x800000FF)  # Red with 50% alpha (128)
increase_test.apply_opacity(increase_test.pixels, 384)  # Apply 1.5x opacity (384 = 256 + 128)


var increased_pixel = increase_test.get_pixel_color(0)
var increased_alpha = (increased_pixel >> 24) & 0xFF
# Should increase from 128 towards 255, but exact value depends on scaling
assert(increased_alpha > 128, f"Alpha should be increased from 128, got {increased_alpha}")
assert(increased_alpha == 193, f"Alpha should be increased to 193, got {increased_alpha}")
assert(increased_alpha <= 255, f"Alpha should not exceed 255, got {increased_alpha}")

# Test zero opacity (fully transparent)
var transparent_test = animation.frame_buffer(5)
transparent_test.fill_pixels(transparent_test.pixels, 0xFF0000FF)  # Red with full alpha
transparent_test.apply_opacity(transparent_test.pixels, 0)  # Make fully transparent

var transparent_pixel = transparent_test.get_pixel_color(0)
var transparent_alpha = (transparent_pixel >> 24) & 0xFF
assert(transparent_alpha == 0, f"Alpha should be 0 (transparent), got {transparent_alpha}")

# Test maximum opacity (should cap at 255)
var max_test = animation.frame_buffer(5)
max_test.fill_pixels(max_test.pixels, 0x800000FF)  # Red with 50% alpha
max_test.apply_opacity(max_test.pixels, 511)  # Apply maximum opacity

var max_pixel = max_test.get_pixel_color(0)
var max_alpha = (max_pixel >> 24) & 0xFF
assert(max_alpha == 255, f"Alpha should be capped at 255, got {max_alpha}")

print("All FrameBuffer tests passed!")
return true