# OscillatorValueProvider Demo
#
# This example demonstrates how to use the OscillatorValueProvider
# with different waveforms and parameters.

import global
import animation
import tasmota

print("=== OscillatorValueProvider Demo ===")

# Create different types of oscillators
print("\n1. Creating different oscillator types:")

# Sawtooth oscillator (ramp from 0 to 100 over 2 seconds)
var sawtooth = animation.ramp(0, 100, 2000)
print(f"Sawtooth: {sawtooth}")

# Triangle oscillator (0 to 100 and back over 3 seconds)
var triangle = animation.linear(0, 100, 3000)
print(f"Triangle: {triangle}")

# Smooth cosine oscillator (0 to 100 smoothly over 4 seconds)
var smooth = animation.smooth(0, 100, 4000)
print(f"Smooth: {smooth}")

# Square wave oscillator (0 to 100 with 30% duty cycle over 1 second)
var square = animation.square(0, 100, 1000, 30)
print(f"Square: {square}")

# Custom oscillator with phase shift and duty cycle (using direct constructor)
var custom = animation.oscillator_value_provider(10, 90, 1500, animation.TRIANGLE)
custom.set_phase(25).set_duty_cycle(75)
print(f"Custom: {custom}")

print("\n2. Demonstrating oscillator values over time:")

# Simulate time progression for sawtooth oscillator
var start_time = tasmota.millis()
sawtooth.origin = start_time

print("Sawtooth oscillator values:")
for i: 0..10
  var time_offset = i * 200  # Every 200ms
  var current_time = start_time + time_offset
  var value = sawtooth.get_value(current_time)
  var progress = (time_offset * 100) / 2000  # Percentage through cycle
  print(f"  t+{time_offset}ms ({progress}%): {value}")
end

print("\nTriangle oscillator values:")
triangle.origin = start_time
for i: 0..15
  var time_offset = i * 200  # Every 200ms
  var current_time = start_time + time_offset
  var value = triangle.get_value(current_time)
  var progress = (time_offset * 100) / 3000  # Percentage through cycle
  print(f"  t+{time_offset}ms ({progress:.1f}%): {value}")
end

print("\n3. Demonstrating phase shift:")

# Create two identical oscillators with different phase
var osc1 = animation.ramp(0, 100, 1000)
var osc2 = animation.ramp(0, 100, 1000)
osc2.set_phase(50)  # 50% phase shift

osc1.origin = start_time
osc2.origin = start_time

print("Phase shift comparison (0% vs 50% phase):")
for i: 0..8
  var time_offset = i * 125  # Every 125ms (1/8 of cycle)
  var current_time = start_time + time_offset
  var value1 = osc1.get_value(current_time)
  var value2 = osc2.get_value(current_time)
  var progress = (time_offset * 100) / 1000
  print(f"  t+{time_offset}ms ({progress}%): no phase={value1}, 50% phase={value2}")
end

print("\n4. Demonstrating duty cycle with square wave:")

# Create square waves with different duty cycles
var square25 = animation.square(0, 100, 1000, 25)
var square50 = animation.square(0, 100, 1000, 50)
var square75 = animation.square(0, 100, 1000, 75)

square25.origin = start_time
square50.origin = start_time
square75.origin = start_time

print("Duty cycle comparison (25%, 50%, 75%):")
for i: 0..10
  var time_offset = i * 100  # Every 100ms (10% of cycle)
  var current_time = start_time + time_offset
  var value25 = square25.get_value(current_time)
  var value50 = square50.get_value(current_time)
  var value75 = square75.get_value(current_time)
  var progress = (time_offset * 100) / 1000
  print(f"  t+{time_offset}ms ({progress}%): 25%={value25}, 50%={value50}, 75%={value75}")
end

print("\n5. Using oscillators with animations:")

# Example of how you might use an oscillator with an animation parameter
print("Example: Using oscillator for dynamic pulse size")

# Create a pulse size oscillator that varies from 1 to 8 pixels
var pulse_size_osc = animation.ramp(1, 8, 2000)
pulse_size_osc.origin = start_time

print("Dynamic pulse sizes over time:")
for i: 0..10
  var time_offset = i * 200
  var current_time = start_time + time_offset
  var pulse_size = pulse_size_osc.get_value(current_time)
  print(f"  t+{time_offset}ms: pulse size = {pulse_size} pixels")
end

print("\n6. Demonstrating update() method:")

# Show how update() method tracks value changes
var change_detector = animation.smooth(0, 255, 1000)
change_detector.origin = start_time

print("Value change detection:")
var last_time = start_time
for i: 1..5
  var time_offset = i * 100
  var current_time = start_time + time_offset
  var changed = change_detector.update(current_time)
  var value = change_detector.value
  print(f"  t+{time_offset}ms: value={value}, changed={changed}")
  
  # Update again with same time (should not change)
  var changed_again = change_detector.update(current_time)
  print(f"    (same time): changed={changed_again}")
end

print("\n=== Demo Complete ===")

# Practical usage example comment:
print("\n# Practical Usage Example:")
print("# var brightness_osc = animation.smooth(50, 255, 3000)")
print("# var animation = animation.pulse_position_animation(10, brightness_osc, 0xFF00FF00)")
print("# animation.set_pulse_size(brightness_osc)  # Dynamic brightness AND size!")

return "OscillatorValueProvider demo completed"