# Test suite for OscillatorValueProvider
#
# This test verifies that the OscillatorValueProvider works correctly
# with all waveform types and parameter configurations.

import animation

# Test basic oscillator functionality
def test_oscillator_basic()
  print("Testing OscillatorValueProvider basic functionality...")
  
  # Create a simple sawtooth oscillator from 0 to 100 over 1000ms
  var osc = animation.oscillator_value_provider(0, 100, 1000, animation.SAWTOOTH)
  
  # Test initial state
  assert(osc.a == 0, "Starting value should be 0")
  assert(osc.b == 100, "End value should be 100")
  assert(osc.duration_ms == 1000, "Duration should be 1000ms")
  assert(osc.form == animation.SAWTOOTH, "Form should be SAWTOOTH")
  assert(osc.phase == 0, "Phase should default to 0")
  assert(osc.duty_cycle == 50, "Duty cycle should default to 50")
  
  # Test parameter setters
  osc.set_phase(25).set_duty_cycle(75).set_a(10).set_b(90)
  assert(osc.phase == 25, "Phase should be set to 25")
  assert(osc.duty_cycle == 75, "Duty cycle should be set to 75")
  assert(osc.a == 10, "Starting value should be set to 10")
  assert(osc.b == 90, "End value should be set to 90")
  
  print("✓ OscillatorValueProvider basic functionality test passed")
end

# Test sawtooth waveform
def test_sawtooth_waveform()
  print("Testing SAWTOOTH waveform...")
  
  var osc = animation.oscillator_value_provider(0, 100, 1000, animation.SAWTOOTH)
  var start_time = 1000
  osc.origin = start_time
  
  # Test at different points in the cycle
  var value_0 = osc.get_value(start_time)        # t=0
  var value_25 = osc.get_value(start_time + 250) # t=250ms (25%)
  var value_50 = osc.get_value(start_time + 500) # t=500ms (50%)
  var value_75 = osc.get_value(start_time + 750) # t=750ms (75%)
  var value_100 = osc.get_value(start_time + 999) # t=999ms (almost 100%)
  
  # Sawtooth should be linear progression from a to b
  assert(value_0 == 0, f"Value at 0% should be 0, got {value_0}")
  assert(value_25 >= 20 && value_25 <= 30, f"Value at 25% should be ~25, got {value_25}")
  assert(value_50 >= 45 && value_50 <= 55, f"Value at 50% should be ~50, got {value_50}")
  assert(value_75 >= 70 && value_75 <= 80, f"Value at 75% should be ~75, got {value_75}")
  assert(value_100 >= 95 && value_100 <= 100, f"Value at 99.9% should be ~100, got {value_100}")
  
  # Test cycle wrapping
  var value_next_cycle = osc.get_value(start_time + 1000) # Next cycle should start over
  assert(value_next_cycle == 0, f"Next cycle should start at 0, got {value_next_cycle}")
  
  print("✓ SAWTOOTH waveform test passed")
end

# Test triangle waveform
def test_triangle_waveform()
  print("Testing TRIANGLE waveform...")
  
  var osc = animation.oscillator_value_provider(0, 100, 1000, animation.TRIANGLE)
  var start_time = 2000
  osc.origin = start_time
  
  # Test at different points in the cycle
  var value_0 = osc.get_value(start_time)        # t=0
  var value_25 = osc.get_value(start_time + 250) # t=250ms (25%)
  var value_50 = osc.get_value(start_time + 500) # t=500ms (50% - peak)
  var value_75 = osc.get_value(start_time + 750) # t=750ms (75% - descending)
  var value_100 = osc.get_value(start_time + 999) # t=999ms (back to start)
  
  # Triangle should go up to peak at 50%, then back down
  assert(value_0 == 0, f"Value at 0% should be 0, got {value_0}")
  assert(value_25 >= 45 && value_25 <= 55, f"Value at 25% should be ~50, got {value_25}")
  assert(value_50 >= 95 && value_50 <= 100, f"Value at 50% should be ~100, got {value_50}")
  assert(value_75 >= 45 && value_75 <= 55, f"Value at 75% should be ~50, got {value_75}")
  assert(value_100 >= 0 && value_100 <= 5, f"Value at 99.9% should be ~0, got {value_100}")
  
  print("✓ TRIANGLE waveform test passed")
end

# Test square waveform
def test_square_waveform()
  print("Testing SQUARE waveform...")
  
  var osc = animation.oscillator_value_provider(0, 100, 1000, animation.SQUARE)
  var start_time = 3000
  osc.origin = start_time
  
  # Test at different points in the cycle (50% duty cycle)
  var value_0 = osc.get_value(start_time)        # t=0
  var value_25 = osc.get_value(start_time + 250) # t=250ms (25% - first half)
  var value_49 = osc.get_value(start_time + 490) # t=490ms (49% - still first half)
  var value_51 = osc.get_value(start_time + 510) # t=510ms (51% - second half)
  var value_75 = osc.get_value(start_time + 750) # t=750ms (75% - second half)
  
  # Square wave should be constant in each half
  assert(value_0 == 0, f"Value at 0% should be 0, got {value_0}")
  assert(value_25 == 0, f"Value at 25% should be 0, got {value_25}")
  assert(value_49 == 0, f"Value at 49% should be 0, got {value_49}")
  assert(value_51 == 100, f"Value at 51% should be 100, got {value_51}")
  assert(value_75 == 100, f"Value at 75% should be 100, got {value_75}")
  
  # Test custom duty cycle (25%)
  osc.set_duty_cycle(25)
  var value_20 = osc.get_value(start_time + 200) # t=200ms (20% - first quarter)
  var value_30 = osc.get_value(start_time + 300) # t=300ms (30% - second quarter)
  
  assert(value_20 == 0, f"Value at 20% with 25% duty should be 0, got {value_20}")
  assert(value_30 == 100, f"Value at 30% with 25% duty should be 100, got {value_30}")
  
  print("✓ SQUARE waveform test passed")
end

# Test cosine waveform
def test_cosine_waveform()
  print("Testing COSINE waveform...")
  
  var osc = animation.oscillator_value_provider(0, 100, 1000, animation.COSINE)
  var start_time = 4000
  osc.origin = start_time
  
  # Test at different points in the cycle
  var value_0 = osc.get_value(start_time)        # t=0 (should be at minimum)
  var value_25 = osc.get_value(start_time + 250) # t=250ms (25% - rising)
  var value_50 = osc.get_value(start_time + 500) # t=500ms (50% - maximum)
  var value_75 = osc.get_value(start_time + 750) # t=750ms (75% - falling)
  var value_100 = osc.get_value(start_time + 999) # t=999ms (back to minimum)
  
  # Cosine should be smooth curve from min to max and back
  # Note: The cosine implementation uses sine with phase shift, so values may differ from pure cosine
  assert(value_0 >= 0 && value_0 <= 10, f"Value at 0% should be ~0, got {value_0}")
  assert(value_25 >= 40 && value_25 <= 60, f"Value at 25% should be ~50, got {value_25}")
  assert(value_50 >= 90 && value_50 <= 100, f"Value at 50% should be ~100, got {value_50}")
  assert(value_75 >= 40 && value_75 <= 60, f"Value at 75% should be ~50, got {value_75}")
  assert(value_100 >= 0 && value_100 <= 10, f"Value at 99.9% should be ~0, got {value_100}")
  
  print("✓ COSINE waveform test passed")
end

# Test phase shift
def test_phase_shift()
  print("Testing phase shift...")
  
  var osc = animation.oscillator_value_provider(0, 100, 1000, animation.SAWTOOTH)
  var start_time = 5000
  osc.origin = start_time
  
  # Test without phase shift
  var value_no_phase = osc.get_value(start_time)
  
  # Test with 25% phase shift (should be like starting at 25% of cycle)
  osc.set_phase(25)
  var value_with_phase = osc.get_value(start_time)
  
  # With 25% phase shift, value at t=0 should be like value at t=25% without phase
  var expected_value = osc.get_value(start_time + 250)
  osc.set_phase(25)  # Reset phase
  var actual_value = osc.get_value(start_time)
  
  # Values should be different due to phase shift
  assert(value_no_phase != value_with_phase, "Phase shift should change the value")
  assert(value_with_phase >= 20 && value_with_phase <= 30, f"25% phase shift should give ~25 value, got {value_with_phase}")
  
  print("✓ Phase shift test passed")
end

# Test static constructor functions
def test_static_constructors()
  print("Testing static constructor functions...")
  
  # Note: oscillator() function removed since easing keyword is now 'ramp'
  # Test ramp() constructor (replaces oscillator functionality)
  var ramp1 = animation.ramp(0, 255, 1500)
  assert(ramp1.form == animation.SAWTOOTH, "ramp() should use SAWTOOTH")
  
  # Test sawtooth() constructor (alias for ramp)
  var sawtooth1 = animation.sawtooth(0, 255, 1500)
  assert(sawtooth1.form == animation.SAWTOOTH, "sawtooth() should use SAWTOOTH")
  
  # Test linear() constructor
  var linear1 = animation.linear(50, 150, 3000)
  assert(linear1.form == animation.TRIANGLE, "linear() should use TRIANGLE")
  
  # Test triangle() constructor (alias for linear)
  var triangle1 = animation.triangle(50, 150, 3000)
  assert(triangle1.form == animation.TRIANGLE, "triangle() should use TRIANGLE")
  
  # Test smooth() constructor
  var smooth1 = animation.smooth(0, 100, 4000)
  assert(smooth1.form == animation.COSINE, "smooth() should use COSINE")
  
  # Test square() constructor
  var square1 = animation.square(0, 1, 500, 30)
  assert(square1.form == animation.SQUARE, "square() should use SQUARE")
  assert(square1.duty_cycle == 30, "square() should set duty cycle to 30")
  
  # Test square() without duty cycle
  var square2 = animation.square(0, 1, 500)
  assert(square2.duty_cycle == 50, "square() should default duty cycle to 50")
  
  print("✓ Static constructor functions test passed")
end

# Test update() method
def test_update_method()
  print("Testing update() method...")
  
  var osc = animation.oscillator_value_provider(0, 100, 1000, animation.SAWTOOTH)
  var start_time = 6000
  osc.origin = start_time
  
  # First update should change value
  var changed1 = osc.update(start_time + 100)
  assert(changed1 == true, "First update should return true (value changed)")
  
  # Update with same time should not change value
  var changed2 = osc.update(start_time + 100)
  assert(changed2 == false, "Update with same time should return false")
  
  # Update with different time should change value
  var changed3 = osc.update(start_time + 200)
  assert(changed3 == true, "Update with different time should return true")
  
  print("✓ Update method test passed")
end

# Test ValueProvider interface compliance
def test_value_provider_interface()
  print("Testing ValueProvider interface compliance...")
  
  var osc = animation.oscillator_value_provider(0, 100, 1000, animation.SAWTOOTH)
  
  # Test that it's recognized as a value provider
  assert(animation.is_value_provider(osc) == true, "OscillatorValueProvider should be recognized as ValueProvider")
  
  # Test that get_value() works with time parameter
  var value = osc.get_value(tasmota.millis())
  assert(type(value) == "int", "get_value() should return integer")
  
  # Test that update() works with time parameter
  var updated = osc.update(tasmota.millis())
  assert(type(updated) == "bool", "update() should return boolean")
  
  print("✓ ValueProvider interface compliance test passed")
end

# Test edge cases and error handling
def test_edge_cases()
  print("Testing edge cases...")
  
  # Test with nil parameters
  var osc1 = animation.oscillator_value_provider(nil, nil, nil, nil)
  assert(osc1.a == 0, "nil a should default to 0")
  assert(osc1.b == 100, "nil b should default to 100")
  assert(osc1.duration_ms == 1000, "nil duration should default to 1000")
  assert(osc1.form == animation.SAWTOOTH, "nil form should default to SAWTOOTH")
  
  # Test with zero duration
  var osc2 = animation.oscillator_value_provider(0, 100, 0, animation.SAWTOOTH)
  var value = osc2.get_value(tasmota.millis())
  assert(value == 0, "Zero duration should return starting value")
  
  # Test phase and duty cycle bounds
  var osc3 = animation.oscillator_value_provider(0, 100, 1000, animation.SQUARE)
  osc3.set_phase(-10)  # Should clamp to 0
  osc3.set_duty_cycle(150)  # Should clamp to 100
  assert(osc3.phase == 0, "Negative phase should clamp to 0")
  assert(osc3.duty_cycle == 100, "Duty cycle > 100 should clamp to 100")
  
  print("✓ Edge cases test passed")
end

# Test tostring() method
def test_tostring()
  print("Testing tostring() method...")
  
  var osc = animation.oscillator_value_provider(10, 90, 2000, animation.TRIANGLE)
  var str_repr = osc.tostring()
  
  # Should contain key information
  import string
  assert(string.find(str_repr, "OscillatorValueProvider") >= 0, "String should contain class name")
  assert(string.find(str_repr, "10") >= 0, "String should contain a value")
  assert(string.find(str_repr, "90") >= 0, "String should contain b value")
  assert(string.find(str_repr, "2000") >= 0, "String should contain duration")
  assert(string.find(str_repr, "TRIANGLE") >= 0, "String should contain waveform name")
  
  print("✓ tostring() method test passed")
end

# Run all tests
def run_oscillator_value_provider_tests()
  print("=== OscillatorValueProvider Tests ===")
  
  try
    test_oscillator_basic()
    test_sawtooth_waveform()
    test_triangle_waveform()
    test_square_waveform()
    test_cosine_waveform()
    test_phase_shift()
    test_static_constructors()
    test_update_method()
    test_value_provider_interface()
    test_edge_cases()
    test_tostring()
    
    print("=== All OscillatorValueProvider tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_oscillator_value_provider_tests = run_oscillator_value_provider_tests

run_oscillator_value_provider_tests()

return run_oscillator_value_provider_tests