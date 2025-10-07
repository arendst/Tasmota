# Test suite for ValueProvider base class
#
# This test verifies that the base ValueProvider class works correctly
# and follows the parameterized class specification with produce_value() API.

import animation

import "./core/param_encoder" as encode_constraints

# Test the basic ValueProvider interface
def test_value_provider_interface()
  print("Testing ValueProvider interface...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
  var provider = animation.value_provider(engine)
  
  # Test default produce_value method
  var result = provider.produce_value("test_param", 1000)
  assert(type(result) == "module", "Default produce_value should return module('undefined')")
  
  # Test that it has the engine reference
  assert(provider.engine != nil, "Provider should have engine reference")
  assert(provider.engine == engine, "Provider should have correct engine reference")
  
  print("✓ ValueProvider interface test passed")
end

# Test with a custom value provider
def test_custom_value_provider()
  print("Testing custom ValueProvider...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
  # Create a simple time-based provider using new API
  class TimeBasedProvider : animation.value_provider
    # Parameter definitions
    static var PARAMS = encode_constraints({
      "multiplier": {"default": 1}
    })
    
    def init(engine)
      super(self).init(engine)
    end
    
    def produce_value(name, time_ms)
      var multiplier = self.multiplier
      return (time_ms / 100) * multiplier  # Changes every 100ms
    end
  end
  
  var provider = TimeBasedProvider(engine)
  provider.multiplier = 2  # Set parameter using virtual member assignment
  
  # Test at different times
  assert(provider.produce_value("test", 0) == 0, "Should return 0 at time 0")
  assert(provider.produce_value("test", 100) == 2, "Should return 2 at time 100")
  assert(provider.produce_value("test", 500) == 10, "Should return 10 at time 500")
  
  # Test parameter access
  assert(provider.multiplier == 2, "Should access parameter via virtual member")
  
  print("✓ Custom ValueProvider test passed")
end

# Test is_value_provider function
def test_is_value_provider()
  print("Testing is_value_provider function...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
  var base_provider = animation.value_provider(engine)
  
  assert(animation.is_value_provider(base_provider) == true, "ValueProvider should be detected")
  assert(animation.is_value_provider(42) == false, "Integer should not be detected")
  assert(animation.is_value_provider("hello") == false, "String should not be detected")
  assert(animation.is_value_provider(nil) == false, "nil should not be detected")
  
  print("✓ is_value_provider test passed")
end

# Test parameterized object integration
def test_parameterized_object_integration()
  print("Testing ParameterizedObject integration...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
  var provider = animation.value_provider(engine)
  
  # Test that it has the engine reference
  assert(provider.engine != nil, "Provider should have engine reference")
  assert(provider.engine == engine, "Provider should have correct engine reference")
  
  # Test parameter system methods exist
  assert(type(provider.set_param) == "function", "Should have set_param method")
  assert(type(provider.get_param) == "function", "Should have get_param method")
  
  # Test lifecycle method exists
  assert(type(provider.start) == "function", "Should have start method")
  
  print("✓ ParameterizedObject integration test passed")
end

# Test lifecycle methods
def test_lifecycle_methods()
  print("Testing lifecycle methods...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
  # Create a provider that tracks start calls
  class LifecycleProvider : animation.value_provider
    var start_called
    var start_time
    
    def init(engine)
      super(self).init(engine)
      self.start_called = false
      self.start_time = 0
    end
    
    def start(time_ms)
      self.start_called = true
      if time_ms == nil
        time_ms = self.engine.time_ms
      end
      self.start_time = time_ms
      return self
    end
    
    def produce_value(name, time_ms)
      return self.start_time
    end
  end
  
  var provider = LifecycleProvider(engine)
  engine.time_ms = 1000
  
  # Test start method
  provider.start(500)
  assert(provider.start_called == true, "start() should be called")
  assert(provider.start_time == 500, "start_time should be set to provided value")
  
  # Test start with nil (should use engine time)
  provider.start_called = false
  provider.start(nil)
  assert(provider.start_called == true, "start() should be called with nil")
  assert(provider.start_time == 1000, "start_time should use engine time when nil")
  
  print("✓ Lifecycle methods test passed")
end

# Run all tests
def run_value_provider_tests()
  print("=== ValueProvider Base Class Tests ===")
  
  try
    test_value_provider_interface()
    test_custom_value_provider()
    test_is_value_provider()
    test_parameterized_object_integration()
    test_lifecycle_methods()
    
    print("=== All ValueProvider base class tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_value_provider_tests = run_value_provider_tests

run_value_provider_tests()

return run_value_provider_tests