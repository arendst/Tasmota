# Test suite for OscillatorValueProvider
#
# This test verifies that the OscillatorValueProvider works correctly
# with all waveform types and parameter configurations using the new parameterized API.

import animation

# Create a mock engine for testing
class MockEngine
  var time_ms
  
  def init()
    self.time_ms = 0
  end
end

var mock_engine = MockEngine()

# Test basic oscillator functionality
def test_oscillator_basic()
  print("Testing OscillatorValueProvider basic functionality...")
  
  # Create oscillator using new parameterized API
  var osc = animation.oscillator_value(mock_engine)
  
  # Set parameters using virtual member assignment
  osc.min_value = 0
  osc.max_value = 100
  osc.duration = 1000
  osc.form = animation.SAWTOOTH
  
  # Test parameter access
  assert(osc.min_value == 0, "Starting value should be 0")
  assert(osc.max_value == 100, "End value should be 100")
  assert(osc.duration == 1000, "Duration should be 1000ms")
  assert(osc.form == animation.SAWTOOTH, "Form should be SAWTOOTH")
  assert(osc.phase == 0, "Phase should default to 0")
  assert(osc.duty_cycle == 50, "Duty cycle should default to 50")
  
  # Test parameter modification
  osc.phase = 25
  osc.duty_cycle = 75
  osc.min_value = 10
  osc.max_value = 90
  
  assert(osc.phase == 25, "Phase should be set to 25")
  assert(osc.duty_cycle == 75, "Duty cycle should be set to 75")
  assert(osc.min_value == 10, "Starting value should be set to 10")
  assert(osc.max_value == 90, "End value should be set to 90")
  
  print("✓ OscillatorValueProvider basic functionality test passed")
end

# Test sawtooth waveform
def test_sawtooth_waveform()
  print("Testing SAWTOOTH waveform...")
  
  var osc = animation.oscillator_value(mock_engine)
  osc.min_value = 0
  osc.max_value = 100
  osc.duration = 1000
  osc.form = animation.SAWTOOTH
  
  var start_time = 1000
  osc.start(start_time)
  
  # Test at different points in the cycle using produce_value
  var value_0 = osc.produce_value("test", start_time)        # t=0
  var value_25 = osc.produce_value("test", start_time + 250) # t=250ms (25%)
  var value_50 = osc.produce_value("test", start_time + 500) # t=500ms (50%)
  var value_75 = osc.produce_value("test", start_time + 750) # t=750ms (75%)
  var value_100 = osc.produce_value("test", start_time + 999) # t=999ms (almost 100%)
  
  # Sawtooth should be linear progression from min_value to max_value
  assert(value_0 == 0, f"Value at 0% should be 0, got {value_0}")
  assert(value_25 >= 20 && value_25 <= 30, f"Value at 25% should be ~25, got {value_25}")
  assert(value_50 >= 45 && value_50 <= 55, f"Value at 50% should be ~50, got {value_50}")
  assert(value_75 >= 70 && value_75 <= 80, f"Value at 75% should be ~75, got {value_75}")
  assert(value_100 >= 95 && value_100 <= 100, f"Value at 99.9% should be ~100, got {value_100}")
  
  # Test cycle wrapping
  var value_next_cycle = osc.produce_value("test", start_time + 1000) # Next cycle should start over
  assert(value_next_cycle == 0, f"Next cycle should start at 0, got {value_next_cycle}")
  
  print("✓ SAWTOOTH waveform test passed")
end

# Test triangle waveform
def test_triangle_waveform()
  print("Testing TRIANGLE waveform...")
  
  var osc = animation.oscillator_value(mock_engine)
  osc.min_value = 0
  osc.max_value = 100
  osc.duration = 1000
  osc.form = animation.TRIANGLE
  
  var start_time = 2000
  osc.start(start_time)
  
  # Test at different points in the cycle
  var value_0 = osc.produce_value("test", start_time)        # t=0
  var value_25 = osc.produce_value("test", start_time + 250) # t=250ms (25%)
  var value_50 = osc.produce_value("test", start_time + 500) # t=500ms (50% - peak)
  var value_75 = osc.produce_value("test", start_time + 750) # t=750ms (75% - descending)
  var value_100 = osc.produce_value("test", start_time + 999) # t=999ms (back to start)
  
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
  
  var osc = animation.oscillator_value(mock_engine)
  osc.min_value = 0
  osc.max_value = 100
  osc.duration = 1000
  osc.form = animation.SQUARE
  
  var start_time = 3000
  osc.start(start_time)
  
  # Test at different points in the cycle (50% duty cycle)
  var value_0 = osc.produce_value("test", start_time)        # t=0
  var value_25 = osc.produce_value("test", start_time + 250) # t=250ms (25% - first half)
  var value_49 = osc.produce_value("test", start_time + 490) # t=490ms (49% - still first half)
  var value_51 = osc.produce_value("test", start_time + 510) # t=510ms (51% - second half)
  var value_75 = osc.produce_value("test", start_time + 750) # t=750ms (75% - second half)
  
  # Square wave should be constant in each half
  assert(value_0 == 0, f"Value at 0% should be 0, got {value_0}")
  assert(value_25 == 0, f"Value at 25% should be 0, got {value_25}")
  assert(value_49 == 0, f"Value at 49% should be 0, got {value_49}")
  assert(value_51 == 100, f"Value at 51% should be 100, got {value_51}")
  assert(value_75 == 100, f"Value at 75% should be 100, got {value_75}")
  
  # Test custom duty cycle (25%)
  osc.duty_cycle = 25
  var value_20 = osc.produce_value("test", start_time + 200) # t=200ms (20% - first quarter)
  var value_30 = osc.produce_value("test", start_time + 300) # t=300ms (30% - second quarter)
  
  assert(value_20 == 0, f"Value at 20% with 25% duty should be 0, got {value_20}")
  assert(value_30 == 100, f"Value at 30% with 25% duty should be 100, got {value_30}")
  
  print("✓ SQUARE waveform test passed")
end

# Test cosine waveform
def test_cosine_waveform()
  print("Testing COSINE waveform...")
  
  var osc = animation.oscillator_value(mock_engine)
  osc.min_value = 0
  osc.max_value = 100
  osc.duration = 1000
  osc.form = animation.COSINE
  
  var start_time = 4000
  osc.start(start_time)
  
  # Test at different points in the cycle
  var value_0 = osc.produce_value("test", start_time)        # t=0 (should be at minimum)
  var value_25 = osc.produce_value("test", start_time + 250) # t=250ms (25% - rising)
  var value_50 = osc.produce_value("test", start_time + 500) # t=500ms (50% - maximum)
  var value_75 = osc.produce_value("test", start_time + 750) # t=750ms (75% - falling)
  var value_100 = osc.produce_value("test", start_time + 999) # t=999ms (back to minimum)
  
  # Print actual values for debugging
  print(f"  COSINE values: t=0%: {value_0}, t=25%: {value_25}, t=50%: {value_50}, t=75%: {value_75}, t=99.9%: {value_100}")
  
  # Cosine should be smooth curve from min to max and back
  # Note: The cosine implementation uses sine with phase shift, so values may differ from pure cosine
  assert(value_0 >= 0 && value_0 <= 10, f"Value at 0% should be ~0, got {value_0}")
  assert(value_25 >= 40 && value_25 <= 60, f"Value at 25% should be ~50, got {value_25}")
  assert(value_50 >= 90 && value_50 <= 100, f"Value at 50% should be ~100, got {value_50}")
  assert(value_75 >= 40 && value_75 <= 60, f"Value at 75% should be ~50, got {value_75}")
  assert(value_100 >= 0 && value_100 <= 10, f"Value at 99.9% should be ~0, got {value_100}")
  
  # Verify values are actually changing over time
  assert(value_0 != value_25, "COSINE values should change between 0% and 25%")
  assert(value_25 != value_50, "COSINE values should change between 25% and 50%")
  assert(value_50 != value_75, "COSINE values should change between 50% and 75%")
  assert(value_75 != value_100, "COSINE values should change between 75% and 99.9%")
  
  print("✓ COSINE waveform test passed")
end

# Test sine waveform
def test_sine_waveform()
  print("Testing SINE waveform...")
  
  var osc = animation.oscillator_value(mock_engine)
  osc.min_value = 0
  osc.max_value = 100
  osc.duration = 1000
  osc.form = animation.SINE
  
  var start_time = 4500
  osc.start(start_time)
  
  # Test at different points in the cycle
  var value_0 = osc.produce_value("test", start_time)        # t=0 (should be at middle)
  var value_25 = osc.produce_value("test", start_time + 250) # t=250ms (25% - maximum)
  var value_50 = osc.produce_value("test", start_time + 500) # t=500ms (50% - middle)
  var value_75 = osc.produce_value("test", start_time + 750) # t=750ms (75% - minimum)
  var value_100 = osc.produce_value("test", start_time + 999) # t=999ms (back to middle)
  
  # Print actual values for debugging
  print(f"  SINE values: t=0%: {value_0}, t=25%: {value_25}, t=50%: {value_50}, t=75%: {value_75}, t=99.9%: {value_100}")
  
  # Sine should be smooth curve starting at middle, going to max, middle, min, middle
  # At t=0: sine(0) = 0, which maps to middle value (50)
  # At t=25%: sine(π/2) = 1, which maps to max value (100)
  # At t=50%: sine(π) = 0, which maps to middle value (50)
  # At t=75%: sine(3π/2) = -1, which maps to min value (0)
  assert(value_0 >= 45 && value_0 <= 55, f"Value at 0% should be ~50 (middle), got {value_0}")
  assert(value_25 >= 90 && value_25 <= 100, f"Value at 25% should be ~100 (max), got {value_25}")
  assert(value_50 >= 45 && value_50 <= 55, f"Value at 50% should be ~50 (middle), got {value_50}")
  assert(value_75 >= 0 && value_75 <= 10, f"Value at 75% should be ~0 (min), got {value_75}")
  assert(value_100 >= 45 && value_100 <= 55, f"Value at 99.9% should be ~50 (back to middle), got {value_100}")
  
  # Verify values are actually changing over time
  assert(value_0 != value_25, "SINE values should change between 0% and 25%")
  assert(value_25 != value_50, "SINE values should change between 25% and 50%")
  assert(value_50 != value_75, "SINE values should change between 50% and 75%")
  assert(value_75 != value_100, "SINE values should change between 75% and 99.9%")
  
  print("✓ SINE waveform test passed")
end

# Test phase shift
def test_phase_shift()
  print("Testing phase shift...")
  
  var osc = animation.oscillator_value(mock_engine)
  osc.min_value = 0
  osc.max_value = 100
  osc.duration = 1000
  osc.form = animation.SAWTOOTH
  
  var start_time = 5000
  osc.start(start_time)
  
  # Test without phase shift
  osc.phase = 0
  var value_no_phase = osc.produce_value("test", start_time)
  
  # Test with 25% phase shift (should be like starting at 25% of cycle)
  osc.phase = 25
  var value_with_phase = osc.produce_value("test", start_time)
  
  # Values should be different due to phase shift
  assert(value_no_phase != value_with_phase, "Phase shift should change the value")
  assert(value_with_phase >= 20 && value_with_phase <= 30, f"25% phase shift should give ~25 value, got {value_with_phase}")
  
  print("✓ Phase shift test passed")
end

# Test static constructor functions
def test_static_constructors()
  print("Testing static constructor functions...")
  
  # Test ramp() constructor (replaces oscillator functionality)
  var ramp1 = animation.ramp(mock_engine)
  assert(ramp1.form == animation.SAWTOOTH, "ramp() should use SAWTOOTH")
  
  # Test sawtooth() constructor (alias for ramp)
  var sawtooth1 = animation.sawtooth(mock_engine)
  sawtooth1.min_value = 0
  sawtooth1.max_value = 255
  sawtooth1.duration = 1500
  assert(sawtooth1.form == animation.SAWTOOTH, "sawtooth() should use SAWTOOTH")
  assert(sawtooth1.min_value == 0, "sawtooth() should set min_value")
  assert(sawtooth1.max_value == 255, "sawtooth() should set max_value")
  assert(sawtooth1.duration == 1500, "sawtooth() should set duration")
  
  # Test linear() constructor
  var linear1 = animation.linear(mock_engine)
  assert(linear1.form == animation.TRIANGLE, "linear() should use TRIANGLE")
  
  # Test triangle() constructor (alias for linear)
  var triangle1 = animation.triangle(mock_engine)
  triangle1.min_value = 50
  triangle1.max_value = 150
  triangle1.duration = 3000
  assert(triangle1.form == animation.TRIANGLE, "triangle() should use TRIANGLE")
  assert(triangle1.min_value == 50, "triangle() should set min_value")
  assert(triangle1.max_value == 150, "triangle() should set max_value")
  assert(triangle1.duration == 3000, "triangle() should set duration")
  
  # Test smooth() constructor
  var smooth1 = animation.smooth(mock_engine)
  assert(smooth1.form == animation.COSINE, "smooth() should use COSINE")
  
  # Test sine_osc() constructor
  var sine1 = animation.sine_osc(mock_engine)
  sine1.min_value = 0
  sine1.max_value = 255
  sine1.duration = 2000
  assert(sine1.form == animation.SINE, "sine_osc() should use SINE")
  assert(sine1.min_value == 0, "sine_osc() should set min_value")
  assert(sine1.max_value == 255, "sine_osc() should set max_value")
  assert(sine1.duration == 2000, "sine_osc() should set duration")
  
  # Test cosine_osc() constructor (alias for smooth)
  var cosine1 = animation.cosine_osc(mock_engine)
  cosine1.min_value = 25
  cosine1.max_value = 200
  cosine1.duration = 1800
  assert(cosine1.form == animation.COSINE, "cosine_osc() should use COSINE")
  assert(cosine1.min_value == 25, "cosine_osc() should set min_value")
  assert(cosine1.max_value == 200, "cosine_osc() should set max_value")
  assert(cosine1.duration == 1800, "cosine_osc() should set duration")
  
  # Test square() constructor
  var square1 = animation.square(mock_engine)
  square1.min_value = 0
  square1.max_value = 1
  square1.duration = 500
  square1.duty_cycle = 30
  assert(square1.form == animation.SQUARE, "square() should use SQUARE")
  assert(square1.duty_cycle == 30, "square() should set duty cycle to 30")
  
  # Test square() with default duty cycle
  var square2 = animation.square(mock_engine)
  square2.min_value = 0
  square2.max_value = 1
  square2.duration = 500
  assert(square2.duty_cycle == 50, "square() should default duty cycle to 50")
  
  print("✓ Static constructor functions test passed")
end

# Test produce_value method
def test_produce_value_method()
  print("Testing produce_value() method...")
  
  var osc = animation.oscillator_value(mock_engine)
  osc.min_value = 0
  osc.max_value = 100
  osc.duration = 1000
  osc.form = animation.SAWTOOTH
  
  var start_time = 6000
  osc.start(start_time)
  
  # Test that produce_value returns consistent values for same time
  var value1 = osc.produce_value("test", start_time + 100)
  var value2 = osc.produce_value("test", start_time + 100)
  assert(value1 == value2, "produce_value should return same value for same time")
  
  # Test that produce_value returns different values for different times
  var value3 = osc.produce_value("test", start_time + 200)
  assert(value1 != value3, "produce_value should return different values for different times")
  
  # Test that parameter name is ignored (same value regardless of name)
  var value4 = osc.produce_value("different_name", start_time + 100)
  assert(value1 == value4, "produce_value should ignore parameter name")
  
  print("✓ produce_value method test passed")
end

# Test ValueProvider interface compliance
def test_value_provider_interface()
  print("Testing ValueProvider interface compliance...")
  
  var osc = animation.oscillator_value(mock_engine)
  
  # Test that it's recognized as a value provider
  assert(animation.is_value_provider(osc) == true, "OscillatorValueProvider should be recognized as ValueProvider")
  
  # Test that produce_value() works with time parameter
  var value = osc.produce_value("test", mock_engine.time_ms)
  assert(type(value) == "int", "produce_value() should return integer")
  
  # Test start method
  var result = osc.start(mock_engine.time_ms)
  assert(result == osc, "start() should return self for method chaining")
  
  print("✓ ValueProvider interface compliance test passed")
end

# Test edge cases and error handling
def test_edge_cases()
  print("Testing edge cases...")
  
  # Test with default parameters
  var osc1 = animation.oscillator_value(mock_engine)
  assert(osc1.min_value == 0, "Default min_value should be 0")
  assert(osc1.max_value == 100, "Default max_value should be 100")
  assert(osc1.duration == 1000, "Default duration should be 1000")
  assert(osc1.form == animation.SAWTOOTH, "Default form should be SAWTOOTH")
  
  # Test with minimum duration (1ms)
  var osc2 = animation.oscillator_value(mock_engine)
  osc2.min_value = 0
  osc2.max_value = 100
  osc2.duration = 1  # Minimum allowed duration
  osc2.form = animation.SAWTOOTH
  
  var value = osc2.produce_value("test", mock_engine.time_ms)
  assert(type(value) == "int", "Minimum duration should return valid integer value")
  
  # Test phase and duty cycle bounds (parameter validation should handle this)
  var osc3 = animation.oscillator_value(mock_engine)
  osc3.form = animation.SQUARE
  
  # Test valid bounds
  osc3.phase = 0
  osc3.duty_cycle = 50
  assert(osc3.phase == 0, "Phase 0 should be valid")
  assert(osc3.duty_cycle == 50, "Duty cycle 50 should be valid")
  
  osc3.phase = 100
  osc3.duty_cycle = 100
  assert(osc3.phase == 100, "Phase 100 should be valid")
  assert(osc3.duty_cycle == 100, "Duty cycle 100 should be valid")
  
  print("✓ Edge cases test passed")
end

# Test time evolution of COSINE and SINE waveforms
def test_cosine_sine_time_evolution()
  print("Testing COSINE and SINE time evolution...")
  
  # Test COSINE evolution over time
  var cosine_osc = animation.oscillator_value(mock_engine)
  cosine_osc.min_value = 0
  cosine_osc.max_value = 255
  cosine_osc.duration = 5000  # 5 second cycle
  cosine_osc.form = animation.COSINE
  
  var start_time = 10000
  cosine_osc.start(start_time)
  
  print("  COSINE waveform evolution (0-255 range, 5000ms duration):")
  var cosine_values = []
  for i: 0..10
    var time_offset = i * 500  # Every 500ms (10% of cycle)
    var value = cosine_osc.produce_value("test", start_time + time_offset)
    cosine_values.push(value)
    var percentage = (i * 10)
    print(f"    t={percentage}% ({time_offset}ms): {value}")
  end
  
  # Test SINE evolution over time
  var sine_osc = animation.oscillator_value(mock_engine)
  sine_osc.min_value = 0
  sine_osc.max_value = 255
  sine_osc.duration = 5000  # 5 second cycle
  sine_osc.form = animation.SINE
  
  sine_osc.start(start_time)
  
  print("  SINE waveform evolution (0-255 range, 5000ms duration):")
  var sine_values = []
  for i: 0..10
    var time_offset = i * 500  # Every 500ms (10% of cycle)
    var value = sine_osc.produce_value("test", start_time + time_offset)
    sine_values.push(value)
    var percentage = (i * 10)
    print(f"    t={percentage}% ({time_offset}ms): {value}")
  end
  
  # Verify that values are actually changing for both waveforms
  var cosine_changes = 0
  var sine_changes = 0
  
  for i: 1..10
    if cosine_values[i] != cosine_values[i-1]
      cosine_changes += 1
    end
    if sine_values[i] != sine_values[i-1]
      sine_changes += 1
    end
  end
  
  assert(cosine_changes >= 8, f"COSINE should change values at least 8 times out of 10 steps, got {cosine_changes}")
  assert(sine_changes >= 8, f"SINE should change values at least 8 times out of 10 steps, got {sine_changes}")
  
  # Verify COSINE starts at minimum and reaches maximum at 50%
  assert(cosine_values[0] <= 10, f"COSINE should start near minimum (0), got {cosine_values[0]}")
  assert(cosine_values[5] >= 245, f"COSINE should reach near maximum (255) at 50%, got {cosine_values[5]}")
  assert(cosine_values[10] <= 10, f"COSINE should return near minimum (0) at 100%, got {cosine_values[10]}")
  
  # Verify SINE starts at middle and follows expected pattern
  assert(sine_values[0] >= 120 && sine_values[0] <= 135, f"SINE should start near middle (127), got {sine_values[0]}")
  assert(sine_values[2] >= 240, f"SINE should reach near maximum around 25%, got {sine_values[2]} at 20%")
  assert(sine_values[5] >= 120 && sine_values[5] <= 135, f"SINE should return to middle at 50%, got {sine_values[5]}")
  assert(sine_values[7] <= 15, f"SINE should reach near minimum around 75%, got {sine_values[7]} at 70%")
  
  print("✓ COSINE and SINE time evolution test passed")
end

# Test tostring() method
def test_tostring()
  print("Testing tostring() method...")
  
  var osc = animation.oscillator_value(mock_engine)
  osc.min_value = 10
  osc.max_value = 90
  osc.duration = 2000
  osc.form = animation.TRIANGLE
  
  var str_repr = osc.tostring()
  
  # Should contain key information
  import string
  assert(string.find(str_repr, "OscillatorValueProvider") >= 0, "String should contain class name")
  assert(string.find(str_repr, "10") >= 0, "String should contain min_value")
  assert(string.find(str_repr, "90") >= 0, "String should contain max_value")
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
    test_sine_waveform()
    test_cosine_sine_time_evolution()
    test_phase_shift()
    test_static_constructors()
    test_produce_value_method()
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