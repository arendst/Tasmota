# Test suite for OscillatorValueProvider ELASTIC and BOUNCE functionality
#
# This test verifies that the new ELASTIC and BOUNCE waveforms work correctly
# and produce the expected spring-like and bouncing curves.

import animation
import string

# Test the ELASTIC waveform
def test_elastic_waveform()
  print("Testing ELASTIC waveform...")
  
  var provider = animation.oscillator_value_provider(0, 100, 1000, animation.ELASTIC)
  
  # Test at key points in the cycle
  # At t=0, should be at starting value (0)
  var value_0 = provider.get_value(provider.origin)
  assert(value_0 == 0, f"ELASTIC at t=0 should be 0, got {value_0}")
  
  # At t=1000ms (100% through), should be at end value (100)
  var value_100 = provider.get_value(provider.origin + 999)  # Just before wrap
  assert(value_100 == 100, f"ELASTIC at 100% should be 100, got {value_100}")
  
  # Test that elastic shows oscillation (overshoots and undershoots)
  var values = []
  for i: [100, 200, 300, 400, 500, 600, 700, 800, 900]
    values.push(provider.get_value(provider.origin + i))
  end
  
  # Check that we have some variation indicating oscillation
  var min_val = values[0]
  var max_val = values[0]
  for val: values
    if val < min_val  min_val = val  end
    if val > max_val  max_val = val  end
  end
  
  # Elastic should show more variation than a simple linear progression
  var range_variation = max_val - min_val
  assert(range_variation > 20, f"ELASTIC should show oscillation, range was {range_variation}")
  
  print("✓ ELASTIC waveform test passed")
end

# Test the BOUNCE waveform
def test_bounce_waveform()
  print("Testing BOUNCE waveform...")
  
  var provider = animation.oscillator_value_provider(0, 100, 1000, animation.BOUNCE)
  
  # Test at key points in the cycle
  # At t=0, should be at starting value (0)
  var value_0 = provider.get_value(provider.origin)
  assert(value_0 == 0, f"BOUNCE at t=0 should be 0, got {value_0}")
  
  # At t=1000ms (100% through), should be at end value (100)
  var value_100 = provider.get_value(provider.origin + 999)  # Just before wrap
  assert(value_100 >= 95 && value_100 <= 100, f"BOUNCE at 100% should be ~100, got {value_100}")
  
  # Test bounce characteristics - should have multiple peaks
  var values = []
  for i: [0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 999]
    values.push(provider.get_value(provider.origin + i))
  end
  
  # Check for bounce-like behavior - should have some variation and settle high
  var has_bounce_pattern = false
  var max_val = values[0]
  var min_val = values[0]
  
  for val: values
    if val > max_val  max_val = val  end
    if val < min_val  min_val = val  end
  end
  
  # Should have some variation indicating bouncing
  var variation = max_val - min_val
  assert(variation > 10, f"BOUNCE should show variation, got {variation}")
  
  # Should generally trend upward (bouncing towards target)
  var early_avg = (values[1] + values[2] + values[3]) / 3
  var late_avg = (values[-3] + values[-2] + values[-1]) / 3
  assert(late_avg > early_avg, "BOUNCE should trend upward over time")
  
  # Values should generally increase over time (settling higher)
  assert(values[-1] > values[2], "BOUNCE should settle at target value")
  
  print("✓ BOUNCE waveform test passed")
end

# Test the convenience constructor functions
def test_elastic_bounce_constructors()
  print("Testing elastic and bounce constructor functions...")
  
  # Test elastic constructor
  var elastic_provider = animation.elastic(10, 90, 2000)
  assert(elastic_provider.a == 10, "elastic should set correct start value")
  assert(elastic_provider.b == 90, "elastic should set correct end value")
  assert(elastic_provider.duration_ms == 2000, "elastic should set correct duration")
  assert(elastic_provider.form == animation.ELASTIC, "elastic should set ELASTIC form")
  
  # Test bounce constructor
  var bounce_provider = animation.bounce(20, 80, 1500)
  assert(bounce_provider.a == 20, "bounce should set correct start value")
  assert(bounce_provider.b == 80, "bounce should set correct end value")
  assert(bounce_provider.duration_ms == 1500, "bounce should set correct duration")
  assert(bounce_provider.form == animation.BOUNCE, "bounce should set BOUNCE form")
  
  print("✓ Elastic and bounce constructor functions test passed")
end

# Test that elastic and bounce work with different value ranges
def test_elastic_bounce_value_ranges()
  print("Testing elastic and bounce with different value ranges...")
  
  # Test with negative values
  var neg_elastic = animation.elastic(-50, 50, 1000)
  var neg_value_0 = neg_elastic.get_value(neg_elastic.origin)
  var neg_value_100 = neg_elastic.get_value(neg_elastic.origin + 999)
  
  assert(neg_value_0 == -50, "Negative range elastic should start at -50")
  assert(neg_value_100 == 50, "Negative range elastic should end at 50")
  
  # Test with small ranges
  var small_bounce = animation.bounce(100, 110, 1000)
  var small_value_0 = small_bounce.get_value(small_bounce.origin)
  var small_value_100 = small_bounce.get_value(small_bounce.origin + 999)
  
  assert(small_value_0 == 100, "Small range bounce should start at 100")
  assert(small_value_100 >= 108 && small_value_100 <= 110, "Small range bounce should end at ~110")
  
  print("✓ Elastic and bounce value ranges test passed")
end

# Test string representation includes new waveforms
def test_elastic_bounce_tostring()
  print("Testing elastic and bounce tostring representation...")
  
  var elastic_provider = animation.elastic(0, 255, 3000)
  var bounce_provider = animation.bounce(10, 200, 2500)
  
  var elastic_str = elastic_provider.tostring()
  var bounce_str = bounce_provider.tostring()
  
  assert(string.find(elastic_str, "ELASTIC") >= 0, "ELASTIC tostring should contain 'ELASTIC'")
  assert(string.find(bounce_str, "BOUNCE") >= 0, "BOUNCE tostring should contain 'BOUNCE'")
  
  print("✓ Elastic and bounce tostring test passed")
end

# Test that constants are properly exported
def test_elastic_bounce_constants()
  print("Testing elastic and bounce constants export...")
  
  assert(animation.ELASTIC == 7, "ELASTIC constant should be 7")
  assert(animation.BOUNCE == 8, "BOUNCE constant should be 8")
  
  # Test that constants work with direct constructor
  var direct_elastic = animation.oscillator_value_provider(0, 100, 1000, animation.ELASTIC)
  var direct_bounce = animation.oscillator_value_provider(0, 100, 1000, animation.BOUNCE)
  
  assert(direct_elastic.form == animation.ELASTIC, "Direct ELASTIC should work")
  assert(direct_bounce.form == animation.BOUNCE, "Direct BOUNCE should work")
  
  print("✓ Elastic and bounce constants test passed")
end

# Test behavior characteristics specific to elastic and bounce
def test_elastic_bounce_characteristics()
  print("Testing elastic and bounce specific characteristics...")
  
  # Test elastic overshoot behavior
  var elastic = animation.elastic(0, 100, 2000)
  var mid_values = []
  for i: [800, 900, 1000, 1100, 1200]  # Around middle of animation
    mid_values.push(elastic.get_value(elastic.origin + i))
  end
  
  # Elastic should show some overshoot (values outside 0-100 range or rapid changes)
  var has_variation = false
  for i: 1..(mid_values.size() - 1)
    if (mid_values[i] - mid_values[i-1]) * (mid_values[i+1] - mid_values[i]) < 0
      has_variation = true  # Found a direction change (oscillation)
      break
    end
  end
  assert(has_variation, "ELASTIC should show oscillation/direction changes")
  
  # Test bounce settling behavior
  var bounce = animation.bounce(0, 100, 2000)
  var early_val = bounce.get_value(bounce.origin + 400)   # 20% through
  var late_val = bounce.get_value(bounce.origin + 1600)   # 80% through
  var final_val = bounce.get_value(bounce.origin + 1999)  # 99.95% through
  
  # Bounce should show decreasing amplitude over time
  assert(final_val > late_val, "BOUNCE should settle higher over time")
  assert(final_val >= 95, "BOUNCE should settle close to target value")
  
  print("✓ Elastic and bounce characteristics test passed")
end

# Run all tests
def run_oscillator_elastic_bounce_tests()
  print("=== OscillatorValueProvider Elastic & Bounce Tests ===")
  
  try
    test_elastic_waveform()
    test_bounce_waveform()
    test_elastic_bounce_constructors()
    test_elastic_bounce_value_ranges()
    test_elastic_bounce_tostring()
    test_elastic_bounce_constants()
    test_elastic_bounce_characteristics()
    
    print("=== All OscillatorValueProvider elastic & bounce tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_oscillator_elastic_bounce_tests = run_oscillator_elastic_bounce_tests

run_oscillator_elastic_bounce_tests()

return run_oscillator_elastic_bounce_tests