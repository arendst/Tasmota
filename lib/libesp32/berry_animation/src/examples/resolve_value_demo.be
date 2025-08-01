# Demonstration of the new resolve_value() method
#
# This example shows how the new resolve_value() method simplifies
# parameter handling compared to the old get_param_value() approach.

import global
import tasmota
import animation

print("=== resolve_value() Method Demonstration ===")

# Note: In a real Tasmota environment, you would create an LED strip like:
# var strip = global.Leds(10, 1)
# var engine = animation.create_engine(strip)

print("\n1. Creating animations with static values:")

# Create a crenel animation with static values
var static_crenel = animation.crenel_position_animation(
  0xFF00FF00,  # Green color
  0,           # Position
  2,           # Pulse size
  3,           # Low size
  -1,          # Infinite pulses
  10,          # Priority
  0,           # Infinite duration
  false,       # No loop
  "static_crenel"
)

print("   Static crenel: " + str(static_crenel))

print("\n2. Creating animations with dynamic values (ValueProviders):")

# Create dynamic parameters using value providers
print("   Creating dynamic color provider...")
var dynamic_color = animation.solid_color_provider(0xFF0000FF)  # Blue
print("   Creating dynamic position provider...")
var dynamic_pos = animation.static_value_provider(1)            # Position 1
print("   Creating dynamic pulse size provider...")
var dynamic_pulse_size = animation.static_value_provider(3)     # 3 pixels

print("   Creating crenel animation with dynamic values...")
# Create a crenel animation with dynamic values
var dynamic_crenel = animation.crenel_position_animation(
  dynamic_color,     # Dynamic color
  dynamic_pos,       # Dynamic position (wrapped in provider)
  dynamic_pulse_size, # Dynamic pulse size
  2,                 # Static low size
  5,                 # 5 pulses
  20,                # Higher priority
  0,                 # Infinite duration
  false,             # No loop
  "dynamic_crenel"
)

print("   Dynamic crenel: " + str(dynamic_crenel))

print("\n3. Demonstrating resolve_value() usage:")

# Show how resolve_value() works with both static and dynamic values
var test_anim = animation.animation(10, 0, false, "test")

var static_value = 42
var dynamic_value = animation.static_value_provider(84)
var time_ms = tasmota.millis()

print("   Static value (42): " + str(test_anim.resolve_value(static_value, "test_param", time_ms)))
print("   Dynamic value (84): " + str(test_anim.resolve_value(dynamic_value, "test_param", time_ms)))

print("\n4. Comparison with old approach:")

# Old approach (still works but more complex)
test_anim.register_param("test_param", {"default": 0})
test_anim.set_param("test_param", dynamic_value)
var old_result = test_anim.get_param_value("test_param", time_ms)

# New approach (simpler and more direct)
var new_result = test_anim.resolve_value(dynamic_value, "test_param", time_ms)

print("   Old approach result: " + str(old_result))
print("   New approach result: " + str(new_result))
print("   Results match: " + str(old_result == new_result))

print("\n5. Benefits of resolve_value():")
print("   ✓ No string parameter names required")
print("   ✓ Direct access to parameter values")
print("   ✓ Better performance (no string lookup)")
print("   ✓ Type safety (direct parameter access)")
print("   ✓ Simpler API (one method for static and dynamic)")
print("   ✓ Works with any value type")

print("\n6. Usage in animation render() methods:")
print("   Old: var color = self.get_param_value(\"color\", time_ms)")
print("   New: var color = self.resolve_value(self.color, \"color\", time_ms)")
print("")
print("   The new approach is:")
print("   - More efficient (no parameter registration/lookup)")
print("   - Supports specific get_XXX() methods on providers")
print("   - More readable (direct parameter access)")
print("   - Allows providers to have specialized methods")

print("\n=== Demonstration Complete ===")

return true