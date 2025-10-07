#!/usr/bin/env berry

# Test for bytes type validation in parameterized_object.be
import animation
import animation_dsl

import "./core/param_encoder" as encode_constraints

# Test class that uses bytes parameter
class BytesTestClass : animation.parameterized_object
  static var PARAMS = encode_constraints({
    "data": {"type": "bytes", "default": nil, "nillable": true},
    "required_data": {"type": "bytes"},
    "name": {"type": "string", "default": "test"}
  })
  
  def init(engine)
    super(self).init(engine)
  end
end

# Mock engine for testing
class MockEngine
  var time_ms
  def init()
    self.time_ms = 1000
  end
end

def test_bytes_type_validation()
  print("Testing bytes type validation...")
  
  var mock_engine = MockEngine()
  var obj = BytesTestClass(mock_engine)
  
  # Test 1: Valid bytes objects
  var hex_bytes = bytes("AABBCC")
  obj.data = hex_bytes
  assert(obj.data.tohex() == "AABBCC", "Hex bytes should be accepted")
  
  var empty_bytes = bytes()
  obj.data = empty_bytes
  assert(obj.data != nil, "Empty bytes should be accepted")
  
  var sized_bytes = bytes(5)
  obj.data = sized_bytes
  assert(obj.data != nil, "Sized bytes should be accepted")
  
  # Test 2: Invalid types should be rejected
  var invalid_types = ["string", 123, 3.14, true, [], {}]
  
  for invalid_val : invalid_types
    var caught_error = false
    try
      obj.data = invalid_val
    except "value_error"
      caught_error = true
    end
    assert(caught_error, f"Should reject {type(invalid_val)}: {invalid_val}")
  end
  
  # Test 3: Nil handling
  obj.data = nil  # Should work for nillable parameter
  assert(obj.data == nil, "Nil should be accepted for nillable parameter")
  
  var nil_error = false
  try
    obj.required_data = nil  # Should fail for non-nillable parameter
  except "value_error"
    nil_error = true
  end
  assert(nil_error, "Should reject nil for non-nillable parameter")
  
  # Test 4: Method-based setting
  var success = obj.set_param("data", bytes("112233"))
  assert(success == true, "Method setting with valid bytes should succeed")
  
  success = obj.set_param("data", "invalid")
  assert(success == false, "Method setting with invalid type should fail")
  
  # Test 5: Parameter definition
  assert(obj._has_param("data") == true, "data parameter should exist")
  var param_def = obj._get_param_def("data")
  assert(obj.constraint_find(param_def, "type", nil) == "bytes", "Data parameter should have bytes type")
  assert(obj.constraint_mask(param_def, "nillable") == 0x20, "Data parameter should be nillable")
  
  assert(obj._has_param("required_data") == true, "required_data parameter should exist")
  var req_param_def = obj._get_param_def("required_data")
  assert(obj.constraint_find(req_param_def, "type", nil) == "bytes", "Required data should have bytes type")
  assert(obj.constraint_mask(req_param_def, "nillable") == 0x00, "Required data should not be nillable")
  
  print("✓ All bytes type validation tests passed!")
end

# Run the test
test_bytes_type_validation()