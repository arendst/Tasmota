# Test for ClosureValueProvider
#
# This test verifies that the ClosureValueProvider correctly wraps
# closures and evaluates them when producing values.

import animation

def test_closure_value_provider()
  print("Testing ClosureValueProvider...")
  
  # Create a mock engine
  class MockEngine
    var time_ms
    def init()
      self.time_ms = 1000
    end
  end
  var engine = MockEngine()
  
  # Create a closure value provider
  var provider = animation.closure_value(engine)
  
  # Test 1: Provider without closure returns nil
  var result = provider.produce_value("test", 1000)
  assert(result == nil, "Provider without closure should return nil")
  print("✓ Provider without closure returns nil")
  
  # Test 2: Set a simple closure
  var f = def(self, name, time_ms) return time_ms / 100 end
  provider.closure = f
  result = provider.produce_value("brightness", 1000)
  assert(result == 10, f"Expected 10, got {result}")
  print("✓ Simple closure evaluation works")
  
  # Test 3: Closure receives correct parameters
  var captured_name = nil
  var captured_time = nil
  provider.closure = def(self, name, time_ms) 
    captured_name = name
    captured_time = time_ms
    return 42
  end
  
  result = provider.produce_value("color", 2000)
  assert(result == 42, f"Expected 42, got {result}")
  assert(captured_name == "color", f"Expected 'color', got '{captured_name}'")
  assert(captured_time == 2000, f"Expected 2000, got {captured_time}")
  print("✓ Closure receives correct parameters")
  
  # Test 4: Complex closure with calculations
  provider.closure = def(self, name, time_ms)
    if name == "brightness"
      return (time_ms % 2000) / 2000.0 * 255
    elif name == "hue"
      return (time_ms / 10) % 360
    else
      return 0
    end
  end
  
  var brightness = provider.produce_value("brightness", 1000)
  var hue = provider.produce_value("hue", 3600)
  var other = provider.produce_value("other", 1000)
  
  assert(brightness == 127.5, f"Expected 127.5, got {brightness}")
  assert(hue == 0, f"Expected 0, got {hue}")  # 3600 / 10 = 360, 360 % 360 = 0
  assert(other == 0, f"Expected 0, got {other}")
  print("✓ Complex closure with parameter-specific logic works")
  
  # Test 5: Test self.resolve helper method with actual value provider
  var static_provider = animation.static_value(engine)
  static_provider.value = 100
  
  provider.closure = def(self, name, time_ms)
    # Use animation.resolve to get value from another provider
    var base_value = animation.resolve(static_provider, name, time_ms)
    return base_value * 2
  end
  
  result = provider.produce_value("test", 2000)
  # static_provider returns 100, then multiply by 2 = 200
  assert(result == 200, f"Expected 200, got {result}")
  print("✓ animation.resolve helper method works with value providers")
  
  # Test 6: Test animation.resolve with static value and value provider
  provider.closure = def(self, name, time_ms)
    var static_value = animation.resolve(50, name, time_ms)  # Static value
    var dynamic_value = animation.resolve(static_provider, name, time_ms)  # Value provider
    return static_value + dynamic_value
  end
  
  result = provider.produce_value("test", 1000)
  # static: 50, dynamic: 100, total: 150
  assert(result == 150, f"Expected 150, got {result}")
  print("✓ animation.resolve works with both static values and value providers")
  
  # Test 7: Test the use case from documentation - arithmetic with another provider
  var oscillator = animation.oscillator_value(engine)
  oscillator.min_value = 10
  oscillator.max_value = 20
  oscillator.duration = 1000
  provider.closure = def(engine, name, time_ms)
    var osc_value = animation.resolve(oscillator, name, time_ms)
    return osc_value + 5  # Add 5 to oscillator value
  end
  
  result = provider.produce_value("position", 500)
  # Oscillator should return a value between 10-20, plus 5 = 15-25
  assert(result >= 15 && result <= 25, f"Expected result between 15-25, got {result}")
  print("✓ Documentation use case works - arithmetic with other providers")
  
  # Test 8: Test negative numbers and negative expressions
  provider.closure = def(self, name, time_ms)
    if name == "negative_literal"
      return -2
    elif name == "negative_expression"
      return -(time_ms / 100)
    elif name == "negative_with_addition"
      return -5 + 3
    else
      return 0
    end
  end
  
  var neg_literal = provider.produce_value("negative_literal", 1000)
  var neg_expr = provider.produce_value("negative_expression", 500)
  var neg_add = provider.produce_value("negative_with_addition", 1000)
  
  assert(neg_literal == -2, f"Expected -2, got {neg_literal}")
  assert(neg_expr == -5, f"Expected -5, got {neg_expr}")  # -(500/100) = -5
  assert(neg_add == -2, f"Expected -2, got {neg_add}")    # -5 + 3 = -2
  print("✓ Negative numbers and expressions work correctly")
  
  # Test 9: Complex expressions with multiple parameters and operators
  var param1 = animation.static_value(engine)
  param1.value = 10
  var param2 = animation.static_value(engine)
  param2.value = 3
  var param3 = animation.static_value(engine)
  param3.value = 2
  
  provider.closure = def(self, name, time_ms)
    var p1 = animation.resolve(param1, name, time_ms)
    var p2 = animation.resolve(param2, name, time_ms)
    var p3 = animation.resolve(param3, name, time_ms)
    
    if name == "arithmetic_complex"
      return (p1 + p2) * p3 - 5  # (10 + 3) * 2 - 5 = 26 - 5 = 21
    elif name == "division_modulo"
      return (p1 * p2) / p3 % 7  # (10 * 3) / 2 % 7 = 30 / 2 % 7 = 15 % 7 = 1
    elif name == "mixed_operations"
      return p1 - p2 + p3 * 4 / 2  # 10 - 3 + 2 * 4 / 2 = 10 - 3 + 8 / 2 = 10 - 3 + 4 = 11
    elif name == "power_and_comparison"
      var base = p1 + p2  # 13
      return base > 12 ? base * p3 : base / p3  # 13 > 12 ? 13 * 2 : 13 / 2 = 26
    else
      return 0
    end
  end
  
  var arith_result = provider.produce_value("arithmetic_complex", 1000)
  var div_mod_result = provider.produce_value("division_modulo", 1000)
  var mixed_result = provider.produce_value("mixed_operations", 1000)
  var power_result = provider.produce_value("power_and_comparison", 1000)
  
  assert(arith_result == 21, f"Expected 21, got {arith_result}")
  assert(div_mod_result == 1, f"Expected 1, got {div_mod_result}")
  assert(mixed_result == 11, f"Expected 11, got {mixed_result}")
  assert(power_result == 26, f"Expected 26, got {power_result}")
  print("✓ Complex expressions with multiple parameters and operators work")
  
  # Test 10: Time-based expressions with multiple variables
  provider.closure = def(self, name, time_ms)
    var base_freq = animation.resolve(param1, name, time_ms)  # 10
    var amplitude = animation.resolve(param2, name, time_ms)  # 3
    var offset = animation.resolve(param3, name, time_ms)     # 2
    
    if name == "sine_wave_simulation"
      # Simulate: amplitude * sin(time * base_freq / 1000) + offset
      # Simplified: just use modulo for wave-like behavior
      var wave = (time_ms * base_freq / 1000) % 360
      return amplitude * (wave > 180 ? -1 : 1) + offset
    elif name == "exponential_decay"
      # Simulate: base_freq * exp(-time/1000) + offset
      # Simplified: base_freq / (1 + time/1000) + offset
      return base_freq / (1 + time_ms / 1000) + offset
    elif name == "linear_interpolation"
      # Linear interpolation between amplitude and base_freq over time
      var t = (time_ms % 2000) / 2000.0  # 0 to 1 over 2 seconds
      return amplitude + t * (base_freq - amplitude)
    else
      return 0
    end
  end
  
  var sine_result = provider.produce_value("sine_wave_simulation", 1500)
  var decay_result = provider.produce_value("exponential_decay", 1000)
  var lerp_result = provider.produce_value("linear_interpolation", 1000)
  
  # sine_wave_simulation: (1500 * 10 / 1000) % 360 = 15 % 360 = 15, 15 <= 180, so 3 * 1 + 2 = 5
  assert(sine_result == 5, f"Expected 5, got {sine_result}")
  # exponential_decay: 10 / (1 + 1000/1000) + 2 = 10 / 2 + 2 = 5 + 2 = 7
  assert(decay_result == 7, f"Expected 7, got {decay_result}")
  # linear_interpolation: t = 1000/2000 = 0.5, result = 3 + 0.5 * (10 - 3) = 3 + 0.5 * 7 = 3 + 3.5 = 6.5
  assert(lerp_result == 6.5, f"Expected 6.5, got {lerp_result}")
  print("✓ Time-based expressions with multiple variables work")
  
  # Test 11: Edge cases with zero, negative, and boundary values
  provider.closure = def(self, name, time_ms)
    if name == "division_by_small"
      return 100 / 0.1  # Should be 1000
    elif name == "negative_modulo"
      return -17 % 5    # Should be -2 in Berry
    elif name == "zero_operations"
      return 0 * 999 + 0 / 1 - 0  # Should be 0
    elif name == "boundary_conditions"
      var val = time_ms % 1000
      return val == 0 ? -1 : val > 500 ? 1 : 0
    else
      return 0
    end
  end
  
  var div_small = provider.produce_value("division_by_small", 1000)
  var neg_mod = provider.produce_value("negative_modulo", 1000)
  var zero_ops = provider.produce_value("zero_operations", 1000)
  var boundary1 = provider.produce_value("boundary_conditions", 1000)  # 1000 % 1000 = 0, so -1
  var boundary2 = provider.produce_value("boundary_conditions", 1750)  # 1750 % 1000 = 750 > 500, so 1
  var boundary3 = provider.produce_value("boundary_conditions", 1250)  # 1250 % 1000 = 250 <= 500, so 0
  
  assert(div_small == 1000, f"Expected 1000, got {div_small}")
  assert(neg_mod == -2, f"Expected -2, got {neg_mod}")
  assert(zero_ops == 0, f"Expected 0, got {zero_ops}")
  assert(boundary1 == -1, f"Expected -1, got {boundary1}")
  assert(boundary2 == 1, f"Expected 1, got {boundary2}")
  assert(boundary3 == 0, f"Expected 0, got {boundary3}")
  print("✓ Edge cases with zero, negative, and boundary values work")
  
  print("All ClosureValueProvider tests passed!")
end

# Test mathematical helper methods
def test_closure_math_methods()
  print("Testing ClosureValueProvider mathematical methods...")
  
  # Create a mock engine
  class MockEngine
    var time_ms
    def init()
      self.time_ms = 1000
    end
  end
  var engine = MockEngine()
  
  # Create a closure value provider
  var provider = animation.closure_value(engine)
  
  # Test 1: min/max functions
  provider.closure = def(self, name, time_ms)
    print(f">> {name=} {animation._math=}")
    if name == "min_test"
      return animation._math.min(5, 3, 8, 1, 9)  # Should return 1
    elif name == "max_test"
      return animation._math.max(5, 3, 8, 1, 9)  # Should return 9
    elif name == "min_two"
      return animation._math.min(10, 7)          # Should return 7
    elif name == "max_two"
      return animation._math.max(10, 7)          # Should return 10
    else
      return 0
    end
  end
  
  var min_result = provider.produce_value("min_test", 1000)
  var max_result = provider.produce_value("max_test", 1000)
  var min_two = provider.produce_value("min_two", 1000)
  var max_two = provider.produce_value("max_two", 1000)
  
  assert(min_result == 1, f"Expected min=1, got {min_result}")
  assert(max_result == 9, f"Expected max=9, got {max_result}")
  assert(min_two == 7, f"Expected min=7, got {min_two}")
  assert(max_two == 10, f"Expected max=10, got {max_two}")
  print("✓ min/max functions work correctly")
  
  # Test 2: abs function
  provider.closure = def(self, name, time_ms)
    if name == "abs_positive"
      return animation._math.abs(42)      # Should return 42
    elif name == "abs_negative"
      return animation._math.abs(-17)     # Should return 17
    elif name == "abs_zero"
      return animation._math.abs(0)       # Should return 0
    elif name == "abs_float"
      return animation._math.abs(-3.14)   # Should return 3.14
    else
      return 0
    end
  end
  
  var abs_pos = provider.produce_value("abs_positive", 1000)
  var abs_neg = provider.produce_value("abs_negative", 1000)
  var abs_zero = provider.produce_value("abs_zero", 1000)
  var abs_float = provider.produce_value("abs_float", 1000)
  
  assert(abs_pos == 42, f"Expected abs(42)=42, got {abs_pos}")
  assert(abs_neg == 17, f"Expected abs(-17)=17, got {abs_neg}")
  assert(abs_zero == 0, f"Expected abs(0)=0, got {abs_zero}")
  assert(abs_float == 3.14, f"Expected abs(-3.14)=3.14, got {abs_float}")
  print("✓ abs function works correctly")
  
  # Test 3: round function
  provider.closure = def(self, name, time_ms)
    if name == "round_up"
      return animation._math.round(3.7)     # Should return 4
    elif name == "round_down"
      return animation._math.round(3.2)     # Should return 3
    elif name == "round_half"
      return animation._math.round(3.5)     # Should return 4
    elif name == "round_negative"
      return animation._math.round(-2.8)    # Should return -3
    else
      return 0
    end
  end
  
  var round_up = provider.produce_value("round_up", 1000)
  var round_down = provider.produce_value("round_down", 1000)
  var round_half = provider.produce_value("round_half", 1000)
  var round_neg = provider.produce_value("round_negative", 1000)
  
  assert(round_up == 4, f"Expected round(3.7)=4, got {round_up}")
  assert(round_down == 3, f"Expected round(3.2)=3, got {round_down}")
  assert(round_half == 4, f"Expected round(3.5)=4, got {round_half}")
  assert(round_neg == -3, f"Expected round(-2.8)=-3, got {round_neg}")
  print("✓ round function works correctly")
  
  # Test 4: sqrt function with integer handling
  provider.closure = def(self, name, time_ms)
    if name == "sqrt_integer_255"
      return animation._math.sqrt(255)      # Should return 255 (full scale)
    elif name == "sqrt_integer_64"
      return animation._math.sqrt(64)       # Should return ~127 (sqrt(64/255)*255)
    elif name == "sqrt_integer_0"
      return animation._math.sqrt(0)        # Should return 0
    elif name == "sqrt_float"
      return animation._math.sqrt(16.0)     # Should return 4.0
    else
      return 0
    end
  end
  
  var sqrt_255 = provider.produce_value("sqrt_integer_255", 1000)
  var sqrt_64 = provider.produce_value("sqrt_integer_64", 1000)
  var sqrt_0 = provider.produce_value("sqrt_integer_0", 1000)
  var sqrt_float = provider.produce_value("sqrt_float", 1000)
  
  assert(sqrt_255 == 255, f"Expected sqrt(255)=255, got {sqrt_255}")
  assert(sqrt_64 >= 127 && sqrt_64 <= 129, f"Expected sqrt(64)~128, got {sqrt_64}")
  assert(sqrt_0 == 0, f"Expected sqrt(0)=0, got {sqrt_0}")
  assert(sqrt_float == 4.0, f"Expected sqrt(16.0)=4.0, got {sqrt_float}")
  print("✓ sqrt function works correctly")
  
  # Test 5: scale function
  provider.closure = def(self, name, time_ms)
    if name == "scale_basic"
      return animation._math.scale(50, 0, 100, 0, 255)    # Should return ~127
    elif name == "scale_reverse"
      return animation._math.scale(25, 0, 100, 255, 0)    # Should return ~191
    elif name == "scale_negative"
      return animation._math.scale(0, -50, 50, -100, 100) # Should return 0
    else
      return 0
    end
  end
  
  var scale_basic = provider.produce_value("scale_basic", 1000)
  var scale_reverse = provider.produce_value("scale_reverse", 1000)
  var scale_neg = provider.produce_value("scale_negative", 1000)
  
  assert(scale_basic >= 127 && scale_basic <= 128, f"Expected scale(50,0,100,0,255)~127, got {scale_basic}")
  assert(scale_reverse >= 191 && scale_reverse <= 192, f"Expected scale(25,0,100,255,0)~191, got {scale_reverse}")
  assert(scale_neg == 0, f"Expected scale(0,-50,50,-100,100)=0, got {scale_neg}")
  print("✓ scale function works correctly")
  
  # Test 6: sin function
  provider.closure = def(self, name, time_ms)
    if name == "sin_0"
      return animation._math.sin(0)        # sin(0°) = 0
    elif name == "sin_64"
      return animation._math.sin(64)       # sin(90°) = 1 -> 255
    elif name == "sin_128"
      return animation._math.sin(128)      # sin(180°) = 0
    elif name == "sin_192"
      return animation._math.sin(192)      # sin(270°) = -1 -> -255
    else
      return 0
    end
  end
  
  var sin_0 = provider.produce_value("sin_0", 1000)
  var sin_64 = provider.produce_value("sin_64", 1000)
  var sin_128 = provider.produce_value("sin_128", 1000)
  var sin_192 = provider.produce_value("sin_192", 1000)
  
  assert(sin_0 >= -5 && sin_0 <= 5, f"Expected sin(0)~0, got {sin_0}")
  assert(sin_64 >= 250 && sin_64 <= 255, f"Expected sin(64)~255, got {sin_64}")
  assert(sin_128 >= -5 && sin_128 <= 5, f"Expected sin(128)~0, got {sin_128}")
  assert(sin_192 >= -255 && sin_192 <= -250, f"Expected sin(192)~-255, got {sin_192}")
  print("✓ sin function works correctly")
  
  # Test 7: cos function (matches oscillator COSINE behavior)
  provider.closure = def(self, name, time_ms)
    if name == "cos_0"
      return animation._math.cos(0)      # Oscillator cosine at 0° = minimum -> -255
    elif name == "cos_64"
      return animation._math.cos(64)     # Oscillator cosine at 90° = ~0
    elif name == "cos_128"
      return animation._math.cos(128)    # Oscillator cosine at 180° = maximum -> 255
    elif name == "cos_192"
      return animation._math.cos(192)    # Oscillator cosine at 270° = ~0
    else
      return 0
    end
  end
  
  var cos_0 = provider.produce_value("cos_0", 1000)
  var cos_64 = provider.produce_value("cos_64", 1000)
  var cos_128 = provider.produce_value("cos_128", 1000)
  var cos_192 = provider.produce_value("cos_192", 1000)
  
  assert(cos_0 >= -255 && cos_0 <= -250, f"Expected cos(0)~-255, got {cos_0}")
  assert(cos_64 >= -5 && cos_64 <= 5, f"Expected cos(64)~0, got {cos_64}")
  assert(cos_128 >= 250 && cos_128 <= 255, f"Expected cos(128)~255, got {cos_128}")
  assert(cos_192 >= -5 && cos_192 <= 5, f"Expected cos(192)~0, got {cos_192}")
  print("✓ cos function works correctly")
  
  # Test 8: Complex expression using multiple math functions
  provider.closure = def(self, name, time_ms)
    if name == "complex_math"
      var angle = time_ms % 256  # 0-255 angle based on time
      var sine_val = animation._math.abs(animation._math.sin(angle))  # Absolute sine value
      var scaled = animation._math.scale(sine_val, 0, 255, 50, 200)  # Scale to 50-200 range
      return animation._math.min(animation._math.max(scaled, 75), 175)  # Clamp to 75-175 range
    else
      return 0
    end
  end
  
  var complex_result = provider.produce_value("complex_math", 1064)  # 1064 % 256 = 40
  # sine(40) should be positive, abs() keeps it positive, scale to 50-200, clamp to 75-175
  assert(complex_result >= 75 && complex_result <= 175, f"Expected complex result in 75-175 range, got {complex_result}")
  print("✓ Complex mathematical expressions work correctly")
  
  print("All mathematical method tests passed!")
end

# Run the tests
test_closure_value_provider()
test_closure_math_methods()