# Test suite for ParameterizedObject
#
# This test verifies that the ParameterizedObject base class works correctly
# and provides proper virtual parameter management.

import animation

import "./core/param_encoder" as encode_constraints

# Create a mock engine for testing
class MockEngine
  var time_ms
  
  def init()
    self.time_ms = 1000  # Fixed time for testing
  end
end

var mock_engine = MockEngine()

# Test basic ParameterizedObject functionality
def test_parameterized_object_basic()
  print("Testing basic ParameterizedObject functionality...")
  
  # Create a simple test class that extends ParameterizedObject
  class TestObject : animation.parameterized_object
    # No instance variables for parameters - they're handled by the virtual system
    
    static var PARAMS = encode_constraints({
      "test_value": {"min": 0, "max": 100, "default": 50},
      "test_name": {"type": "string", "default": "test"},
      "test_enum": {"enum": [1, 2, 3], "default": 1}
    })
    
    def init(engine, value, name)
      super(self).init(engine)  # This initializes parameters with defaults
      # Override defaults if provided
      if value != nil
        self.test_value = value  # Uses virtual setmember
      end
      if name != nil
        self.test_name = name    # Uses virtual setmember
      end
    end
    
    def set_test_value(value)
      self.test_value = value    # Uses virtual setmember
      return self
    end
  end
  
  var obj = TestObject(mock_engine, 25, "my_test")
  
  # Test basic parameter access
  assert(obj.get_param("test_value") == 25, "Should get parameter value")
  assert(obj.get_param("test_name") == "my_test", "Should get parameter value")
  assert(obj.get_param("test_enum") == 1, "Should get default enum value")
  
  # Test virtual member access
  assert(obj.test_value == 25, "Should access parameter via virtual member")
  assert(obj.test_name == "my_test", "Should access parameter via virtual member")
  assert(obj.test_enum == 1, "Should access enum parameter via virtual member")
  
  # Test parameter setting
  assert(obj.set_param("test_value", 75) == true, "Should set valid parameter")
  assert(obj.test_value == 75, "Virtual member should be updated")
  
  # Test validation
  assert(obj.set_param("test_value", 150) == false, "Should reject out-of-range value")
  assert(obj.test_value == 75, "Virtual member should not change on invalid value")
  
  # Test enum validation
  assert(obj.set_param("test_enum", 2) == true, "Should accept valid enum value")
  assert(obj.test_enum == 2, "Enum should be updated")
  assert(obj.set_param("test_enum", 5) == false, "Should reject invalid enum value")
  assert(obj.test_enum == 2, "Enum should not change on invalid value")
  
  # Test virtual member assignment
  obj.test_value = 30  # Uses virtual setmember
  assert(obj.test_value == 30, "Virtual member assignment should work")
  
  # Test using setter method
  obj.set_test_value(40)
  assert(obj.test_value == 40, "Setter method should work")
  
  # Test non-existent parameter
  assert(obj.set_param("invalid_param", 42) == false, "Should reject unknown parameter")
  
  print("✓ Basic ParameterizedObject test passed")
end

# Test class hierarchy parameter inheritance
def test_parameter_hierarchy()
  print("Testing parameter hierarchy...")
  
  # Create a base class with some parameters
  class BaseClass : animation.parameterized_object
    static var PARAMS = encode_constraints({
      "base_param": {"type": "string", "default": "base_value"},
      "shared_param": {"type": "string", "default": "base_default"}
    })
    
    def init(engine)
      super(self).init(engine)
    end
  end
  
  # Create a child class with additional parameters
  class ChildClass : BaseClass
    static var PARAMS = encode_constraints({
      "child_param": {"min": 0, "max": 10, "default": 5},
      "shared_param": {"type": "string", "default": "child_default"}  # Override parent default
    })
    
    def init(engine)
      super(self).init(engine)
    end
  end
  
  var child = ChildClass(mock_engine)
  
  # Test that child has access to both parent and child parameters
  assert(child.base_param == "base_value", "Should inherit parent parameter")
  assert(child.child_param == 5, "Should have child parameter")
  assert(child.shared_param == "child_default", "Child should override parent default")
  
  # Test setting parameters from both levels
  assert(child.set_param("base_param", "new_base") == true, "Should set parent parameter")
  assert(child.base_param == "new_base", "Parent parameter should be updated")
  
  assert(child.set_param("child_param", 8) == true, "Should set child parameter")
  assert(child.child_param == 8, "Child parameter should be updated")
  
  # Test validation works for child parameters
  assert(child.set_param("child_param", 15) == false, "Should validate child parameter")
  assert(child.child_param == 8, "Child parameter should not change on validation error")
  
  print("✓ Parameter hierarchy test passed")
end

# Test ValueProvider as parameter
def test_value_provider_as_parameter()
  print("Testing ValueProvider as parameter...")
  
  # Create a simple test class
  class TestClass : animation.parameterized_object
    static var PARAMS = encode_constraints({
      "dynamic_value": {"min": 0, "max": 100, "default": 50}
    })
    
    def init(engine)
      super(self).init(engine)
    end
  end
  
  var obj = TestClass(mock_engine)
  
  # Create a mock ValueProvider
  class MockValueProvider : animation.value_provider
    var test_value
    def init(engine, value)
      super(self).init(engine)
      self.test_value = value
    end
    def produce_value(name, time_ms)
      return self.test_value
    end
  end
  
  var provider = MockValueProvider(mock_engine, 75)
  
  # Set ValueProvider as parameter (should bypass validation)
  assert(obj.set_param("dynamic_value", provider) == true, "Should accept ValueProvider as parameter")
  
  # Test that get_param returns the provider itself
  var returned_provider = obj.get_param("dynamic_value")
  assert(returned_provider != nil, "get_param should return a value")
  assert(type(returned_provider) == "instance", "get_param should return an instance")
  # We can't directly compare instances, so we'll test that it produces the expected value
  assert(returned_provider.produce_value("test", 1000) == 75, "Returned provider should produce expected value")
  
  # Test that virtual member access resolves the provider
  assert(obj.dynamic_value == 75, "Virtual member should resolve ValueProvider")
  
  # Test get_param_value explicitly
  assert(obj.get_param_value("dynamic_value", 1000) == 75, "Should resolve ValueProvider value")
  
  print("✓ ValueProvider as parameter test passed")
end

# Test parameter metadata
def test_parameter_metadata()
  print("Testing parameter metadata...")
  
  class TestClass : animation.parameterized_object
    static var PARAMS = encode_constraints({
      "range_param": {"min": 0, "max": 100, "default": 50},
      "enum_param": {"enum": [1, 2, 3], "default": 1},
      "simple_param": {"type": "string", "default": "test"}
    })
    
    def init(engine)
      super(self).init(engine)
    end
  end
  
  var obj = TestClass(mock_engine)
  
  # Test getting single parameter definition
  assert(obj._has_param("range_param") == true, "range_param should exist")
  var range_def = obj._get_param_def("range_param")
  assert(range_def != nil, "Should get range parameter definition")
  assert(obj.constraint_find(range_def, "min", nil) == 0, "Should have min constraint")
  assert(obj.constraint_find(range_def, "max", nil) == 100, "Should have max constraint")
  assert(obj.constraint_find(range_def, "default", nil) == 50, "Should have default value")
  
  assert(obj._has_param("enum_param") == true, "enum_param should exist")
  var enum_def = obj._get_param_def("enum_param")
  assert(enum_def != nil, "Should get enum parameter definition")
  assert(obj.constraint_mask(enum_def, "enum") == 0x10, "Should have enum constraint")
  assert(obj.constraint_find(enum_def, "default", nil) == 1, "Should have default value")
  
  print("✓ Parameter metadata test passed")
end

# Test virtual member error handling
def test_virtual_member_errors()
  print("Testing virtual member error handling...")
  
  class TestClass : animation.parameterized_object
    static var PARAMS = encode_constraints({
      "valid_param": {"min": 0, "max": 100, "default": 50}
    })
    
    def init(engine)
      super(self).init(engine)
    end
  end
  
  var obj = TestClass(mock_engine)
  
  # Test accessing non-existent parameter should raise attribute_error
  try
    var result = obj.member("non_existent_param")
    assert(false, "Should have raised attribute_error for non-existent parameter")
  except "attribute_error" as e, msg
    # Expected behavior
  except .. as e, msg
    assert(false, f"Should have raised attribute_error, got {e}")
  end
  
  # Test setting non-existent parameter should raise error
  try
    obj.setmember("non_existent_param", 42)
    assert(false, "Should have raised attribute error")
  except .. as e
    # Expected
  end
  
  # Test validation error in setmember
  try
    obj.setmember("valid_param", 150)  # Out of range
    assert(false, "Should have raised validation error")
  except .. as e
    # Expected
  end
  
  print("✓ Virtual member error handling test passed")
end

# Test undefined parameter behavior
# This test verifies what happens when reading/writing parameters that are not defined in PARAMS:
# - Reading via virtual member access (obj.undefined_param) raises "attribute_error"
# - Reading via get_param("undefined_param") returns nil
# - Writing via virtual member assignment (obj.undefined_param = value) raises "attribute_error"
# - Writing via set_param("undefined_param", value) returns false
def test_undefined_parameter_behavior()
  print("Testing undefined parameter behavior...")
  import string  # Import once at the top of the function
  
  class TestClass : animation.parameterized_object
    static var PARAMS = encode_constraints({
      "defined_param": {"min": 0, "max": 100, "default": 50}
    })
    
    def init(engine)
      super(self).init(engine)
    end
  end
  
  var obj = TestClass(mock_engine)
  
  # Test reading undefined parameter via virtual member access
  print("  Testing reading undefined parameter...")
  var read_exception_caught = false
  var read_exception_type = nil
  var read_exception_msg = nil
  
  try
    var undefined_value = obj.undefined_param
    assert(false, "Reading undefined parameter should raise an exception")
  except .. as e, msg
    read_exception_caught = true
    read_exception_type = e
    read_exception_msg = msg
    print(f"    Exception type: {e}")
    print(f"    Exception message: {msg}")
  end
  
  assert(read_exception_caught, "Should have caught an exception when reading undefined parameter")
  assert(read_exception_type == "attribute_error", "Should raise attribute_error for undefined parameter read")
  
  # Check that the error message contains the expected text
  var read_msg_contains_attribute = string.find(read_exception_msg, "has no attribute") >= 0
  var read_msg_contains_param_name = string.find(read_exception_msg, "undefined_param") >= 0
  assert(read_msg_contains_attribute && read_msg_contains_param_name, "Should mention missing attribute in read error message")
  
  # Test reading undefined parameter via get_param method
  var undefined_via_method = obj.get_param("undefined_param")
  assert(undefined_via_method == nil, "get_param for undefined parameter should return nil")
  
  # Test writing undefined parameter via virtual member assignment
  print("  Testing writing undefined parameter...")
  var exception_caught = false
  var actual_exception_type = nil
  var actual_exception_msg = nil
  
  try
    obj.undefined_param = 42
    assert(false, "Writing to undefined parameter should raise an exception")
  except .. as e, msg
    exception_caught = true
    actual_exception_type = e
    actual_exception_msg = msg
    print(f"    Exception type: {e}")
    print(f"    Exception message: {msg}")
  end
  
  assert(exception_caught, "Should have caught an exception when setting undefined parameter")
  assert(actual_exception_type == "attribute_error", "Should raise attribute_error for undefined parameter")
  # Check that the error message contains the expected text (using string module)
  var msg_contains_attribute = string.find(actual_exception_msg, "has no attribute") >= 0
  var msg_contains_param_name = string.find(actual_exception_msg, "undefined_param") >= 0
  assert(msg_contains_attribute && msg_contains_param_name, "Should mention missing attribute in error message")
  
  # Test writing undefined parameter via set_param method
  print("  Testing set_param with undefined parameter...")
  var set_result = obj.set_param("undefined_param", 42)
  assert(set_result == false, "set_param for undefined parameter should return false")
  
  # Verify that the undefined parameter was not actually set
  var still_undefined = obj.get_param("undefined_param")
  assert(still_undefined == nil, "Undefined parameter should still be nil after failed set")
  
  # Test that defined parameters still work correctly
  print("  Verifying defined parameters still work...")
  assert(obj.defined_param == 50, "Defined parameter should still work")
  obj.defined_param = 75
  assert(obj.defined_param == 75, "Defined parameter assignment should still work")
  
  # Test _has_param and _get_param_def for undefined parameter
  print("  Testing parameter definition for undefined parameter...")
  assert(obj._has_param("undefined_param") == false, "_has_param for undefined parameter should return false")
  var undefined_def = obj._get_param_def("undefined_param")
  assert(undefined_def == nil, "_get_param_def for undefined parameter should be nil")
  
  # Test get_param_value for undefined parameter
  print("  Testing get_param_value for undefined parameter...")
  var undefined_param_value = obj.get_param_value("undefined_param", 1000)
  assert(undefined_param_value == nil, "get_param_value for undefined parameter should return nil")
  
  print("✓ Undefined parameter behavior test passed")
end

# Test engine parameter requirement
def test_engine_requirement()
  print("Testing engine parameter requirement...")
  
  class TestClass : animation.parameterized_object
    static var PARAMS = encode_constraints({
      "test_param": {"default": 42}
    })
  end
  
  # Test that nil engine raises error
  try
    var obj = TestClass(nil)
    assert(false, "Should have raised error for nil engine")
  except .. as e
    # Expected
  end
  
  # Test that valid engine works
  var obj = TestClass(mock_engine)
  assert(obj.engine == mock_engine, "Should store engine reference")
  assert(obj.test_param == 42, "Should initialize parameters correctly")
  
  print("✓ Engine requirement test passed")
end

# Test equality operator
def test_equality_operator()
  print("Testing equality operator...")
  
  class TestClass : animation.parameterized_object
    static var PARAMS = encode_constraints({
      "test_param": {"default": 42}
    })
    
    def init(engine)
      super(self).init(engine)
    end
  end
  
  var obj1 = TestClass(mock_engine)
  var obj2 = TestClass(mock_engine)
  
  # Test that objects are equal to themselves
  assert(obj1 == obj1, "Object should be equal to itself")
  assert(obj2 == obj2, "Object should be equal to itself")
  
  # Test that different objects are not equal
  assert(!(obj1 == obj2), "Different objects should not be equal")
  assert(!(obj2 == obj1), "Different objects should not be equal")
  
  # Test inequality operator
  assert(obj1 != obj2, "Different objects should be not equal via != operator")
  assert(obj2 != obj1, "Different objects should be not equal via != operator")
  assert(!(obj1 != obj1), "Object should not be not equal to itself")
  assert(!(obj2 != obj2), "Object should not be not equal to itself")
  
  # Test that comparison doesn't trigger member() exceptions
  var comparison_worked = true
  try
    var result1 = obj1 == obj2
    var result2 = obj1 != obj2
    # If we get here, both comparisons worked without exceptions
  except .. as e, msg
    comparison_worked = false
    print(f"    Unexpected exception during comparison: {e} - {msg}")
  end
  
  assert(comparison_worked, "Both == and != comparisons should work without exceptions")
  
  print("✓ Equality operator test passed")
end

# Run all tests
def run_parameterized_object_tests()
  print("=== ParameterizedObject Tests ===")
  
  try
    test_parameterized_object_basic()
    test_parameter_hierarchy()
    test_value_provider_as_parameter()
    test_parameter_metadata()
    test_virtual_member_errors()
    test_undefined_parameter_behavior()
    test_engine_requirement()
    test_equality_operator()
    
    print("=== All ParameterizedObject tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_parameterized_object_tests = run_parameterized_object_tests

run_parameterized_object_tests()

return run_parameterized_object_tests