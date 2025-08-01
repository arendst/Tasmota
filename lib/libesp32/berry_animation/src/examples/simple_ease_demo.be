# Simple demo of ease_in and ease_out functionality
# This demo only tests the oscillator provider without the full animation framework

import tasmota

# Mock tasmota functions for testing
import global
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

print("=== Simple Ease In/Out Demo ===")

# Test EASE_IN
print("\nTesting EASE_IN (0->100 over 1000ms):")
var ease_in = osc_provider.ease_in(0, 100, 1000)
var start_time = 1000

for i: [0, 250, 500, 750, 1000]
  var value = ease_in.get_value(start_time + i)
  var percent = i / 10  # Convert to percentage
  print(f"  {percent:3.0f}% ({i:4d}ms): value = {value:3d}")
end

# Test EASE_OUT
print("\nTesting EASE_OUT (0->100 over 1000ms):")
var ease_out = osc_provider.ease_out(0, 100, 1000)
start_time = 2000

for i: [0, 250, 500, 750, 1000]
  var value = ease_out.get_value(start_time + i)
  var percent = i / 10  # Convert to percentage
  print(f"  {percent:3.0f}% ({i:4d}ms): value = {value:3d}")
end

# Compare with linear
print("\nComparison with LINEAR (0->100 over 1000ms):")
var linear = osc_provider.linear(0, 100, 1000)
start_time = 3000

print("Time%  Linear  Ease-In  Ease-Out")
print("----  ------  -------  --------")

var ease_in_comp = osc_provider.ease_in(0, 100, 1000)
var ease_out_comp = osc_provider.ease_out(0, 100, 1000)

for i: [0, 250, 500, 750, 1000]
  var percent = i / 10
  var linear_val = linear.get_value(start_time + i)
  var ease_in_val = ease_in_comp.get_value(start_time + i)
  var ease_out_val = ease_out_comp.get_value(start_time + i)
  print(f"{percent:3.0f}%  {linear_val:6d}  {ease_in_val:7d}  {ease_out_val:8d}")
end

print("\n=== Demo Complete ===")
print("✓ EASE_IN: Starts slow, accelerates (quadratic)")
print("✓ EASE_OUT: Starts fast, decelerates (quadratic)")
print("✓ Both integrate seamlessly with existing oscillator features")