# Demo of ease_in and ease_out functionality in OscillatorValueProvider
#
# This demo shows how to use the new EASE_IN and EASE_OUT waveforms
# for creating smooth acceleration and deceleration effects.

import animation
import tasmota

print("=== Ease In/Out Demo ===")

# Create different easing providers
var ease_in_brightness = animation.ease_in(0, 255, 3000)      # 3-second ease-in from 0 to 255
var ease_out_brightness = animation.ease_out(255, 0, 2000)    # 2-second ease-out from 255 to 0
var ease_in_position = animation.ease_in(0, 30, 4000)         # 4-second ease-in position
var ease_out_size = animation.ease_out(1, 10, 1500)           # 1.5-second ease-out size

print("Created easing providers:")
print(f"  Ease-in brightness: {ease_in_brightness}")
print(f"  Ease-out brightness: {ease_out_brightness}")
print(f"  Ease-in position: {ease_in_position}")
print(f"  Ease-out size: {ease_out_size}")

# Simulate time progression and show values
print("\nEase-in brightness progression (0->255 over 3 seconds):")
var start_time = tasmota.millis()
for i: [0, 750, 1500, 2250, 3000]
  var value = ease_in_brightness.get_value(start_time + i)
  var percent = (i * 100) / 3000
  print(f"  {percent:3.0f}% ({i:4d}ms): brightness = {value:3d}")
end

print("\nEase-out brightness progression (255->0 over 2 seconds):")
start_time = tasmota.millis()
for i: [0, 500, 1000, 1500, 2000]
  var value = ease_out_brightness.get_value(start_time + i)
  var percent = (i * 100) / 2000
  print(f"  {percent:3.0f}% ({i:4d}ms): brightness = {value:3d}")
end

# Show comparison with linear interpolation
print("\nComparison: Ease-in vs Linear vs Ease-out (0->100 over 2 seconds):")
var linear_provider = animation.linear(0, 100, 2000)
var ease_in_demo = animation.ease_in(0, 100, 2000)
var ease_out_demo = animation.ease_out(0, 100, 2000)

start_time = tasmota.millis()
print("Time%  Linear  Ease-In  Ease-Out")
print("----  ------  -------  --------")
for i: [0, 500, 1000, 1500, 2000]
  var percent = (i * 100) / 2000
  var linear_val = linear_provider.get_value(start_time + i)
  var ease_in_val = ease_in_demo.get_value(start_time + i)
  var ease_out_val = ease_out_demo.get_value(start_time + i)
  print(f"{percent:3.0f}%  {linear_val:6d}  {ease_in_val:7d}  {ease_out_val:8d}")
end

# Show how to use with phase shifts
print("\nEase-in with 25% phase shift:")
var phased_ease = animation.ease_in(0, 100, 2000)
phased_ease.set_phase(25)

start_time = tasmota.millis()
for i: [0, 500, 1000, 1500, 2000]
  var value = phased_ease.get_value(start_time + i)
  var percent = (i * 100) / 2000
  print(f"  {percent:3.0f}% ({i:4d}ms): value = {value:3d}")
end

# Example usage in animation context
print("\nExample: Using ease providers in animations")
print("# Create a breathing effect with ease-in/out")
print("var breathing_brightness = animation.ease_in(50, 255, 2000)")
print("var pulse_anim = animation.pulse(0xFF0000FF, breathing_brightness, 0, 0, true, 'breathing')")
print("")
print("# Create a smooth position animation")
print("var smooth_position = animation.ease_out(0, 29, 3000)")
print("var comet_anim = animation.comet(0xFF00FF00, smooth_position, 5, 128, true, 'smooth_comet')")

print("\n=== Demo Complete ===")
print("The new ease_in and ease_out functions provide:")
print("• EASE_IN: Quadratic acceleration (starts slow, speeds up)")
print("• EASE_OUT: Quadratic deceleration (starts fast, slows down)")
print("• Both work with all existing OscillatorValueProvider features")
print("• Phase shifts, duty cycles, and method chaining all supported")
print("• Use animation.ease_in(start, end, duration_ms) and animation.ease_out(start, end, duration_ms)")