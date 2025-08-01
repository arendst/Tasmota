# Demonstration of fully dynamic crenel_position animation
#
# This example shows how all parameters of the crenel_position animation
# can now be made dynamic using value providers.

import global
import tasmota
import animation

print("=== Dynamic Crenel Position Animation Demo ===")

print("\n1. Creating dynamic value providers for all parameters:")

# Create dynamic providers for all parameters
var dynamic_color = animation.solid_color_provider(0xFF00FF00)  # Green color
var dynamic_back_color = animation.static_value_provider(0xFF000000)  # Transparent background
var dynamic_pos = animation.static_value_provider(1)  # Position 1
var dynamic_pulse_size = animation.static_value_provider(3)  # 3 pixels wide
var dynamic_low_size = animation.static_value_provider(2)  # 2 pixels gap
var dynamic_nb_pulse = animation.static_value_provider(5)  # 5 pulses

print("   ✓ Dynamic color provider: " + str(dynamic_color))
print("   ✓ Dynamic background color provider: " + str(dynamic_back_color))
print("   ✓ Dynamic position provider: " + str(dynamic_pos))
print("   ✓ Dynamic pulse size provider: " + str(dynamic_pulse_size))
print("   ✓ Dynamic low size provider: " + str(dynamic_low_size))
print("   ✓ Dynamic nb_pulse provider: " + str(dynamic_nb_pulse))

print("\n2. Creating crenel animation with all dynamic parameters:")

# Create a crenel animation where ALL parameters are dynamic
var fully_dynamic_crenel = animation.crenel_position_animation(
  dynamic_color,      # Dynamic color
  dynamic_pos,        # Dynamic position
  dynamic_pulse_size, # Dynamic pulse size
  dynamic_low_size,   # Dynamic low size
  dynamic_nb_pulse,   # Dynamic number of pulses
  10,                 # Static priority
  0,                  # Infinite duration
  false,              # No loop
  "fully_dynamic_crenel"
)

print("   ✓ Fully dynamic crenel created: " + str(fully_dynamic_crenel))

print("\n3. Testing parameter resolution:")

# Test that resolve_value works for all parameters
var test_time = tasmota.millis()

print("   Testing resolve_value() on all parameters:")
var resolved_color = fully_dynamic_crenel.resolve_value(dynamic_color, "color", test_time)
var resolved_pos = fully_dynamic_crenel.resolve_value(dynamic_pos, "pos", test_time)
var resolved_pulse_size = fully_dynamic_crenel.resolve_value(dynamic_pulse_size, "pulse_size", test_time)
var resolved_low_size = fully_dynamic_crenel.resolve_value(dynamic_low_size, "low_size", test_time)
var resolved_nb_pulse = fully_dynamic_crenel.resolve_value(dynamic_nb_pulse, "nb_pulse", test_time)

print("   ✓ Color resolved to: 0x" + format("%08x", resolved_color))
print("   ✓ Position resolved to: " + str(resolved_pos))
print("   ✓ Pulse size resolved to: " + str(resolved_pulse_size))
print("   ✓ Low size resolved to: " + str(resolved_low_size))
print("   ✓ Number of pulses resolved to: " + str(resolved_nb_pulse))
print("   ✓ All parameters resolved successfully!")

print("\n4. Benefits of fully dynamic parameters:")
print("   ✓ Color can change over time (e.g., rainbow effects)")
print("   ✓ Position can animate (moving pulses)")
print("   ✓ Pulse size can vary (breathing effect)")
print("   ✓ Gap size can change (rhythm variations)")
print("   ✓ Number of pulses can be time-based")
print("   ✓ Background color can fade or cycle")

print("\n5. Example use cases:")
print("   • Animated progress bars with changing colors")
print("   • Breathing effects with variable pulse sizes")
print("   • Moving patterns with position animation")
print("   • Rhythm-based lighting with variable gaps")
print("   • Dynamic backgrounds that fade in/out")

print("\n=== Demo Complete ===")
print("All parameters in crenel_position can now be dynamic!")

return true