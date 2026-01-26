# Test file for Leds JavaScript bridge functions
#
# This file contains tests for the JavaScript-specific Leds display functions

print("Testing Leds JavaScript bridge functions...")

# Create a Leds strip with 10 pixels
var strip = Leds(10)
assert(strip.leds == 10, "LED strip should have 10 pixels")

# Test show method (should not error even if not in browser)
print("Testing show method...")
strip.clear_to(0xFF0000)  # Fill with red
strip.show()  # Should not error
print("show method works (no error)")

# Test pixel_count method (should return leds when not in browser)
print("Testing pixel_count method...")
var pixel_count = strip.pixel_count()
assert(pixel_count == 10, f"Pixel count should be 10 when not in browser, got {pixel_count}")
print("pixel_count method works (returns 10 when not in browser)")

# Test that show works with different pixel patterns
print("Testing show with different patterns...")

# Pattern 1: All red
strip.clear_to(0xFF0000)
strip.show()
print("Pattern 1 (all red) sent to JS")

# Pattern 2: Mixed colors
strip.clear_to(0x000000)
strip.set_pixel_color(0, 0xFF0000)  # Red
strip.set_pixel_color(1, 0x00FF00)  # Green
strip.set_pixel_color(2, 0x0000FF)  # Blue
strip.set_pixel_color(3, 0xFFFF00)  # Yellow
strip.set_pixel_color(4, 0x00FFFF)  # Cyan
strip.show()
print("Pattern 2 (mixed colors) sent to JS")

# Pattern 3: Gradient-like pattern
strip.clear_to(0x000000)
var i = 0
while i < strip.leds
  var brightness = (i * 255) / strip.leds
  strip.set_pixel_color(i, (brightness << 16))  # Red gradient
  i += 1
end
strip.show()
print("Pattern 3 (gradient) sent to JS")

print("All Leds JavaScript bridge tests passed!")
return true
