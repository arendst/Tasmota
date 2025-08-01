# Comprehensive demo of all easing types in OscillatorValueProvider
#
# This demo shows all available easing waveforms:
# EASE_IN, EASE_OUT, ELASTIC, and BOUNCE

import global
import tasmota

# Mock tasmota functions for testing
if !global.contains("tasmota")
  tasmota = {}
  tasmota.millis = def() return 1000 end
  tasmota.scale_uint = def(value, from_min, from_max, to_min, to_max)
    if from_max == from_min return to_min end
    return to_min + ((value - from_min) * (to_max - to_min)) / (from_max - from_min)
  end
  tasmota.sine_int = def(angle)
    import math
    return int(4096 * math.sin(angle * math.pi / 16384))
  end
end

# Load just the oscillator provider
import "providers/oscillator_value_provider.be" as osc_provider

print("=== Complete Easing Types Demo ===")
print("Showing all available easing waveforms (0->100 over 1000ms)")

# Create all easing types
var ease_in = osc_provider.ease_in(0, 100, 1000)
var ease_out = osc_provider.ease_out(0, 100, 1000)
var elastic = osc_provider.elastic(0, 100, 1000)
var bounce = osc_provider.bounce(0, 100, 1000)
var linear = osc_provider.linear(0, 100, 1000)  # For comparison

print("\nTime%  Linear  Ease-In  Ease-Out  Elastic  Bounce")
print("----  ------  -------  --------  -------  ------")

var start_time = 1000
for i: [0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000]
  var percent = i / 10  # Convert to percentage
  var linear_val = linear.get_value(start_time + i)
  var ease_in_val = ease_in.get_value(start_time + i)
  var ease_out_val = ease_out.get_value(start_time + i)
  var elastic_val = elastic.get_value(start_time + i)
  var bounce_val = bounce.get_value(start_time + i)
  
  print(f"{percent:3.0f}%  {linear_val:6d}  {ease_in_val:7d}  {ease_out_val:8d}  {elastic_val:7d}  {bounce_val:6d}")
end

print("\n=== Easing Type Characteristics ===")

print("\n1. EASE_IN (Quadratic Acceleration)")
print("   • Starts slow, accelerates")
print("   • Formula: f(t) = t²")
print("   • Use: Smooth start-up animations")
print("   • Constructor: animation.ease_in(start, end, duration)")

print("\n2. EASE_OUT (Quadratic Deceleration)")
print("   • Starts fast, decelerates")
print("   • Formula: f(t) = 1 - (1-t)²")
print("   • Use: Smooth stop animations")
print("   • Constructor: animation.ease_out(start, end, duration)")

print("\n3. ELASTIC (Spring-like)")
print("   • Overshoots and oscillates like a spring")
print("   • Shows spring-back behavior")
print("   • Use: Playful, attention-grabbing effects")
print("   • Constructor: animation.elastic(start, end, duration)")

print("\n4. BOUNCE (Ball-like)")
print("   • Bounces like a ball with decreasing amplitude")
print("   • Multiple bounces that settle")
print("   • Use: Drop animations, impact effects")
print("   • Constructor: animation.bounce(start, end, duration)")

print("\n=== Usage Examples ===")

print("\n# Smooth brightness fade-in")
print("var brightness = animation.ease_in(0, 255, 3000)")
print("var fade_anim = animation.filled(0xFF0000FF, brightness, 0, false, 'fade')")

print("\n# Gentle position movement")
print("var position = animation.ease_out(0, 29, 2000)")
print("var move_anim = animation.pulse_position(0xFF00FF00, 5, position, 1, 10, 0, true, 'move')")

print("\n# Playful spring effect")
print("var spring_size = animation.elastic(1, 10, 2500)")
print("var spring_anim = animation.pulse(0xFFFF0000, spring_size, 0, 0, true, 'spring')")

print("\n# Bouncing comet")
print("var bounce_pos = animation.bounce(0, 29, 3000)")
print("var comet_anim = animation.comet(0xFF00FFFF, bounce_pos, 8, 200, true, 'bouncy')")

print("\n# DSL Integration")
print("# animation smooth_pulse = pulse(red, ease_in(50, 255, 2s), 0, 0)")
print("# animation spring_move = comet(blue, elastic(0, 29, 3s), 5, 128)")
print("# animation bouncy_breathe = breathe(green, bounce(30, 255, 4s), 8000)")

print("\n=== Advanced Features ===")

print("\n• All easing types support phase shifts:")
var phased_elastic = osc_provider.elastic(0, 100, 2000)
phased_elastic.set_phase(25)
print(f"  Elastic with 25% phase: {phased_elastic}")

print("\n• Method chaining works with all types:")
var chained = osc_provider.bounce(10, 90, 3000).set_phase(10)
print(f"  Chained bounce: {chained}")

print("\n• All types work with any value range:")
print("  animation.elastic(-50, 150, 2000)  # Negative to positive")
print("  animation.bounce(100, 105, 1000)   # Small range")
print("  animation.ease_in(1000, 0, 3000)   # Reverse direction")

print("\n=== Performance Notes ===")
print("• All calculations use integer arithmetic")
print("• Uses tasmota.scale_uint for efficient scaling")
print("• Minimal memory overhead")
print("• Suitable for embedded systems")

print("\n=== Demo Complete ===")
print("All easing types are now available in the Berry Animation Framework!")