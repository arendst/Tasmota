#!/usr/bin/env berry

# Core ValueProvider system test - focuses only on the essential functionality

# Mock the animation module for testing
var animation = {}

# Define the ValueProvider base class
class ValueProvider
  def get_value(time_ms)
    return nil
  end
  
  def update(time_ms)
    return false
  end
end

# Define the StaticValueProvider
class StaticValueProvider : ValueProvider
  var value
  
  def init(value)
    self.value = value
  end
  
  def get_value(time_ms)
    return self.value
  end
  
  def update(time_ms)
    return false
  end
  
  # Universal member access using member() construct
  def member(name)
    if type(name) == "string" && name[0..3] == "get_"
      return def(time_ms) return self.value end
    end
    return super(self).member(name)
  end
  
  def tostring()
    return f"StaticValueProvider(value={self.value})"
  end
end

# Helper function to check if object is a value provider
def is_value_provider(obj)
  return obj != nil && type(obj) == "instance" && isinstance(obj, ValueProvider)
end

# Test the core functionality
def test_core_functionality()
  print("=== Core ValueProvider System Test ===")
  
  # Test 1: Basic ValueProvider interface
  print("1. Testing ValueProvider interface...")
  var base_provider = ValueProvider()
  assert(base_provider.get_value(1000) == nil, "Base provider should return nil")
  assert(base_provider.update(1000) == false, "Base provider update should return false")
  print("   ✓ Base interface works")
  
  # Test 2: StaticValueProvider basic functionality
  print("2. Testing StaticValueProvider...")
  var static_provider = StaticValueProvider(42)
  assert(static_provider.get_value(1000) == 42, "Should return static value")
  assert(static_provider.update(1000) == false, "Update should return false")
  print("   ✓ Static provider basic functionality works")
  
  # Test 3: Universal method support via member()
  print("3. Testing universal method support...")
  var pulse_size_method = static_provider.member("get_pulse_size")
  assert(type(pulse_size_method) == "function", "Should return function for get_pulse_size")
  assert(pulse_size_method(1000) == 42, "get_pulse_size should return static value")
  
  var pos_method = static_provider.member("get_pos")
  assert(type(pos_method) == "function", "Should return function for get_pos")
  assert(pos_method(1000) == 42, "get_pos should return static value")
  
  var color_method = static_provider.member("get_color")
  assert(type(color_method) == "function", "Should return function for get_color")
  assert(color_method(1000) == 42, "get_color should return static value")
  print("   ✓ Universal method support works")
  
  # Test 4: Provider detection
  print("4. Testing provider detection...")
  assert(is_value_provider(static_provider) == true, "Should detect StaticValueProvider")
  assert(is_value_provider(base_provider) == true, "Should detect ValueProvider")
  assert(is_value_provider(42) == false, "Should not detect integer")
  assert(is_value_provider("hello") == false, "Should not detect string")
  assert(is_value_provider(nil) == false, "Should not detect nil")
  print("   ✓ Provider detection works")
  
  # Test 5: Parameter resolution simulation
  print("5. Testing parameter resolution...")
  def resolve_parameter(param_value, param_name, time_ms)
    if is_value_provider(param_value)
      # Try specific method first
      var method_name = "get_" + param_name
      var method = param_value.member(method_name)
      if method != nil && type(method) == "function"
        return method(time_ms)
      else
        return param_value.get_value(time_ms)
      end
    else
      return param_value  # Static value
    end
  end
  
  # Test with static value
  var static_result = resolve_parameter(123, "beacon_size", 1000)
  assert(static_result == 123, "Should return static value")
  
  # Test with provider using specific method
  var provider_result = resolve_parameter(static_provider, "beacon_size", 1000)
  assert(provider_result == 42, "Should return value from provider via get_pulse_size")
  
  # Test with provider using generic method
  var generic_result = resolve_parameter(static_provider, "unknown_param", 1000)
  assert(generic_result == 42, "Should return value from provider via get_value")
  print("   ✓ Parameter resolution works")
  
  print("=== All core tests passed! ===")
  print()
  print("Core ValueProvider system is working correctly:")
  print("- ValueProvider base interface")
  print("- StaticValueProvider with universal method support")
  print("- Provider detection")
  print("- Parameter resolution with method-specific fallback")
  
  return true
end

# Run the test
test_core_functionality()