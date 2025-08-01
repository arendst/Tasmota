# Demonstration of fully dynamic pulse_position animation
#
# This example shows how all parameters of the pulse_position animation
# can now be made dynamic using value providers.

import global
import tasmota
import animation

print("=== Dynamic Pulse Position Animation Demo ===")

print("\n1. Creating dynamic value providers for all parameters:")

# Create dynamic providers for all parameters
var dynamic_color = animation.solid_color_provider(0xFF0000FF)  # Blue color
var dynamic_back_color = animation.static_value_provider(0xFF000000)  # Transparent background
var dynamic_pos = animation.static_value_provider(2)  # Position 2
var dynamic_pulse_size = animation.static_value_provider(4)  # 4 pixels wide
var dynamic_slew_size = animation.static_value_provider(1)  # 1 pixel fade

print("   ✓ Dynamic color provider: " + str(dynamic_color))
print("   ✓ Dynamic background color provider: " + str(dynamic_back_color))
print("   ✓ Dynamic position provider: " + str(dynamic_pos))
print("   ✓ Dynamic pulse size provider: " + str(dynamic_pulse_size))
print("   ✓ Dynamic slew size provider: " + str(dynamic_slew_size))

print("\n2. Creating pulse animation with all dynamic parameters:")

# Create a pulse animation where ALL parameters are dynamic
var fully_dynamic_pulse = animation.pulse_position_animation(
  dynamic_color,      # Dynamic color
  dynamic_pos,        # Dynamic position
  dynamic_pulse_size, # Dynamic pulse size
  dynamic_slew_size,  # Dynamic slew size
  10,                 # Static priority
  0,                  # Infinite duration
  false,              # No loop
  "fully_dynamic_pulse"
)

print("   ✓ Fully dynamic pulse created: " + str(fully_dynamic_pulse))

print("\n3. Testing parameter resolution:")

# Test that resolve_value works for all parameters
var test_time = tasmota.millis()

print("   Testing resolve_value() on all parameters:")
var resolved_color = fully_dynamic_pulse.resolve_value(dynamic_color, "color", test_time)
var resolved_pos = fully_dynamic_pulse.resolve_value(dynamic_pos, "pos", test_time)
var resolved_pulse_size = fully_dynamic_pulse.resolve_value(dynamic_pulse_size, "pulse_size", test_time)
var resolved_slew_size = fully_dynamic_pulse.resolve_value(dynamic_slew_size, "slew_size", test_time)

print("   ✓ Color resolved to: 0x" + format("%08x", resolved_color))
print("   ✓ Position resolved to: " + str(resolved_pos))
print("   ✓ Pulse size resolved to: " + str(resolved_pulse_size))
print("   ✓ Slew size resolved to: " + str(resolved_slew_size))
print("   ✓ All parameters resolved successfully!")

print("\n4. Benefits of fully dynamic parameters:")
print("   ✓ Color can change over time (e.g., rainbow effects)")
print("   ✓ Position can animate (moving pulses)")
print("   ✓ Pulse size can vary (breathing effect)")
print("   ✓ Slew size can change (fade intensity variations)")
print("   ✓ Background color can fade or cycle")

print("\n5. Example use cases:")
print("   • Moving spotlight effects with animated positions")
print("   • Breathing effects with variable pulse sizes and fade")
print("   • Color-changing indicators with dynamic backgrounds")
print("   • Animated progress indicators with smooth edges")
print("   • Dynamic lighting that responds to music or sensors")

print("\n=== Demo Complete ===")
print("All parameters in pulse_position can now be dynamic!")

return true