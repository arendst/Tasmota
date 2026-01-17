# Test suite for value_provider functionality
#
# This test verifies that value providers work correctly
# and follow the parameterized class specification with produce_value() API.
# Note: value_provider class has been removed; providers now inherit from parameterized_object
# with VALUE_PROVIDER = true.

import animation

import "./core/param_encoder" as encode_constraints

# Simple test provider class (since value_provider class was removed)
class TestValueProvider : animation.parameterized_object
  static var VALUE_PROVIDER = true
  
  def produce_value(name, time_ms)
    return module("undefined")  # Default behavior
  end
end

# Test the basic value_provider interface
def test_value_provider_interface()
  print("Testing value_provider interface...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
  var provider = TestValueProvider(engine)
  
  # Test default produce_value method
  var result = provider.produce_value("test_param", 1000)
  assert(type(result) == "module", "Default produce_value should return module('undefined')")
  
  # Test that it has the engine reference
  assert(provider.engine != nil, "Provider should have engine reference")
  assert(provider.engine == engine, "Provider should have correct engine reference")
  
  print("✓ value_provider interface test passed")
end

# Test with a custom value provider
def test_custom_value_provider()
  print("Testing custom value_provider...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
  # Create a simple time-based provider using new API
  class TimeBasedProvider : animation.parameterized_object
    static var VALUE_PROVIDER = true
    # Parameter definitions
    static var PARAMS = animation.enc_params({
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
  
  print("✓ Custom value_provider test passed")
end

# Test is_value_provider function
def test_is_value_provider()
  print("Testing is_value_provider function...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
  var base_provider = TestValueProvider(engine)
  
  assert(animation.is_value_provider(base_provider) == true, "value_provider should be detected")
  assert(animation.is_value_provider(42) == false, "Integer should not be detected")
  assert(animation.is_value_provider("hello") == false, "String should not be detected")
  assert(animation.is_value_provider(nil) == false, "nil should not be detected")
  
  print("✓ is_value_provider test passed")
end

# Test parameterized object integration
def test_parameterized_object_integration()
  print("Testing parameterized_object integration...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
  var provider = TestValueProvider(engine)
  
  # Test that it has the engine reference
  assert(provider.engine != nil, "Provider should have engine reference")
  assert(provider.engine == engine, "Provider should have correct engine reference")
  
  # Test parameter system methods exist
  assert(type(provider.set_param) == "function", "Should have set_param method")
  assert(type(provider.get_param) == "function", "Should have get_param method")
  
  # Test lifecycle method exists
  assert(type(provider.start) == "function", "Should have start method")
  
  print("✓ parameterized_object integration test passed")
end

# Test lifecycle methods
def test_lifecycle_methods()
  print("Testing lifecycle methods...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
  # Create a provider that tracks start calls
  class LifecycleProvider : animation.parameterized_object
    static var VALUE_PROVIDER = true
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

# Test value provider registration in engine_proxy
def test_value_provider_registration()
  print("Testing value_provider registration in engine_proxy...")
  
  # Create a mock LED strip
  var strip = Leds(30)
  var engine = animation.create_engine(strip)
  
  # Get the root proxy (engine delegates to this)
  var proxy = engine.root_animation
  
  # Create a simple value provider (oscillator)
  # It should auto-register with the engine (which delegates to root_animation)
  var oscillator = animation.triangle(engine)
  oscillator.min_value = 0
  oscillator.max_value = 255
  oscillator.duration = 2000
  
  # Test: Start proxy (should NOT auto-start value provider)
  var start_time = 1000
  proxy.start(start_time)
  assert(proxy.is_running == true, "Proxy should be running")
  assert(oscillator.is_running == false, "Value provider should NOT be auto-started by proxy")
  
  # Test: Manually start value provider and update proxy
  oscillator.start(start_time)
  assert(oscillator.is_running == true, "Value provider should be running after manual start")
  
  var update_time = 2000
  proxy.update(update_time)
  
  # Value provider should have been updated
  var value = oscillator.produce_value("test", update_time)
  assert(value != nil, "Value provider should produce a value")
  
  # Test: Stop proxy (should NOT auto-stop value provider)
  proxy.stop()
  assert(proxy.is_running == false, "Proxy should be stopped")
  assert(oscillator.is_running == true, "Value provider should still be running after proxy stop")
  
  # Manually stop the value provider
  oscillator.stop()
  assert(oscillator.is_running == false, "Value provider should be stopped after manual stop")
  
  # Test: Clear proxy (should clear value providers)
  proxy.clear()
  assert(size(proxy.value_providers) == 0, "Proxy should have no value providers after clear")
  
  # Test: Remove value provider
  var oscillator2 = animation.triangle(engine)
  proxy.add(oscillator2)
  assert(size(proxy.value_providers) == 1, "Should have 1 provider after add")
  
  var removed = proxy.remove(oscillator2)
  assert(removed == true, "Value provider should be removed successfully")
  assert(size(proxy.value_providers) == 0, "Proxy should have no value providers after remove")
  
  print("✓ value_provider registration test passed")
end

# Test multiple value providers
def test_multiple_value_providers()
  print("Testing multiple value_providers in engine_proxy...")
  
  var strip = Leds(30)
  var engine = animation.create_engine(strip)
  var proxy = animation.engine_proxy(engine)
  
  var osc1 = animation.triangle(engine)
  var osc2 = animation.smooth(engine)
  var osc3 = animation.sine_osc(engine)
  
  proxy.add(osc1)
  proxy.add(osc2)
  proxy.add(osc3)
  
  assert(size(proxy.value_providers) == 3, "Should have 3 value providers")
  
  # Manually start all value providers (simulating what animations would do)
  osc1.start(3000)
  osc2.start(3000)
  osc3.start(3000)
  
  proxy.start(3000)
  assert(osc1.is_running == true, "Oscillator 1 should be running")
  assert(osc2.is_running == true, "Oscillator 2 should be running")
  assert(osc3.is_running == true, "Oscillator 3 should be running")
  
  proxy.update(4000)
  # All should be updated (we can't directly verify, but no errors means success)
  
  proxy.stop()
  # Value providers should still be running (not auto-stopped by proxy)
  assert(osc1.is_running == true, "Oscillator 1 should still be running")
  assert(osc2.is_running == true, "Oscillator 2 should still be running")
  assert(osc3.is_running == true, "Oscillator 3 should still be running")
  
  # Manually stop them
  osc1.stop()
  osc2.stop()
  osc3.stop()
  
  print("✓ Multiple value_providers test passed")
end

# Test is_empty() includes value_providers
def test_is_empty_with_value_providers()
  print("Testing is_empty() with value_providers...")
  
  var strip = Leds(30)
  var engine = animation.create_engine(strip)
  var proxy = animation.engine_proxy(engine)
  
  assert(proxy.is_empty() == true, "Proxy should be empty initially")
  
  proxy.add(animation.triangle(engine))
  assert(proxy.is_empty() == false, "Proxy should not be empty with value provider")
  
  proxy.clear()
  assert(proxy.is_empty() == true, "Proxy should be empty after clear")
  
  print("✓ is_empty() with value_providers test passed")
end

# Run all tests
def run_value_provider_tests()
  print("=== value_provider Base Class Tests ===")
  
  try
    test_value_provider_interface()
    test_custom_value_provider()
    test_is_value_provider()
    test_parameterized_object_integration()
    test_lifecycle_methods()
    
    print("\n=== value_provider Registration Tests ===")
    test_value_provider_registration()
    test_multiple_value_providers()
    test_is_empty_with_value_providers()
    
    print("\n=== All value_provider tests passed! ===")
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