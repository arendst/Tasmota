# Constraint Encoding Test Suite
#
# Comprehensive tests for encode_constraints() and ParameterizedObject static methods:
# - constraint_mask()
# - constraint_find()
#
# Tests all PARAMS patterns found in the Berry Animation Framework codebase.

import "./core/param_encoder" as encode_constraints

# Test counter
var test_count = 0
var pass_count = 0
var fail_count = 0

# Test helper function
def assert_equal(actual, expected, test_name)
  test_count += 1
  if actual == expected
    pass_count += 1
    print(f"✓ Test {test_count}: {test_name}")
    return true
  else
    fail_count += 1
    print(f"✗ Test {test_count}: {test_name}")
    print(f"  Expected: {expected}")
    print(f"  Actual:   {actual}")
    return false
  end
end

# Test helper for array equality
def assert_array_equal(actual, expected, test_name)
  test_count += 1
  if size(actual) != size(expected)
    fail_count += 1
    print(f"✗ Test {test_count}: {test_name}")
    print(f"  Expected size: {size(expected)}, Actual size: {size(actual)}")
    return false
  end
  var i = 0
  while i < size(actual)
    if actual[i] != expected[i]
      fail_count += 1
      print(f"✗ Test {test_count}: {test_name}")
      print(f"  Mismatch at index {i}: expected {expected[i]}, got {actual[i]}")
      return false
    end
    i += 1
  end
  pass_count += 1
  print(f"✓ Test {test_count}: {test_name}")
  return true
end

print("=" * 70)
print("CONSTRAINT ENCODING TEST SUITE")
print("=" * 70)

# ============================================================================
# TEST GROUP 1: Basic Integer Constraints (min/max/default)
# ============================================================================
print("\n--- Test Group 1: Basic Integer Constraints ---")

# Test 1.1: Simple min/max/default (int8 range)
var params_1_1 = {"min": 0, "max": 255, "default": 128}
var encoded_1_1 = encode_constraints({"test": params_1_1})["test"]
assert_equal(animation.parameterized_object.constraint_mask(encoded_1_1, "min"), 0x01, "1.1a: has min")
assert_equal(animation.parameterized_object.constraint_mask(encoded_1_1, "max"), 0x02, "1.1b: has max")
assert_equal(animation.parameterized_object.constraint_mask(encoded_1_1, "default"), 0x04, "1.1c: has default")
assert_equal(animation.parameterized_object.constraint_find(encoded_1_1, "min", nil), 0, "1.1d: min value")
assert_equal(animation.parameterized_object.constraint_find(encoded_1_1, "max", nil), 255, "1.1e: max value")
assert_equal(animation.parameterized_object.constraint_find(encoded_1_1, "default", nil), 128, "1.1f: default value")

# Test 1.2: Only default (no min/max)
var params_1_2 = {"default": 0xFFFFFFFF}
var encoded_1_2 = encode_constraints({"test": params_1_2})["test"]
assert_equal(animation.parameterized_object.constraint_mask(encoded_1_2, "min"), 0x00, "1.2a: no min")
assert_equal(animation.parameterized_object.constraint_mask(encoded_1_2, "max"), 0x00, "1.2b: no max")
assert_equal(animation.parameterized_object.constraint_mask(encoded_1_2, "default"), 0x04, "1.2c: has default")
assert_equal(animation.parameterized_object.constraint_find(encoded_1_2, "default", nil), 0xFFFFFFFF, "1.2d: default value")

# Test 1.3: Min only
var params_1_3 = {"min": 1, "default": 1000}
var encoded_1_3 = encode_constraints({"test": params_1_3})["test"]
assert_equal(animation.parameterized_object.constraint_mask(encoded_1_3, "min"), 0x01, "1.3a: has min")
assert_equal(animation.parameterized_object.constraint_mask(encoded_1_3, "max"), 0x00, "1.3b: no max")
assert_equal(animation.parameterized_object.constraint_find(encoded_1_3, "min", nil), 1, "1.3c: min value")
assert_equal(animation.parameterized_object.constraint_find(encoded_1_3, "default", nil), 1000, "1.3d: default value")

# Test 1.4: Negative values
var params_1_4 = {"min": -128, "max": 127, "default": 0}
var encoded_1_4 = encode_constraints({"test": params_1_4})["test"]
assert_equal(animation.parameterized_object.constraint_find(encoded_1_4, "min", nil), -128, "1.4a: negative min")
assert_equal(animation.parameterized_object.constraint_find(encoded_1_4, "max", nil), 127, "1.4b: positive max")
assert_equal(animation.parameterized_object.constraint_find(encoded_1_4, "default", nil), 0, "1.4c: zero default")

# Test 1.5: Large int32 values
var params_1_5 = {"min": 0, "max": 25600, "default": 2560}
var encoded_1_5 = encode_constraints({"test": params_1_5})["test"]
assert_equal(animation.parameterized_object.constraint_find(encoded_1_5, "max", nil), 25600, "1.5a: large max")
assert_equal(animation.parameterized_object.constraint_find(encoded_1_5, "default", nil), 2560, "1.5b: large default")

# ============================================================================
# TEST GROUP 2: Enum Constraints
# ============================================================================
print("\n--- Test Group 2: Enum Constraints ---")

# Test 2.1: Simple enum with positive values
var params_2_1 = {"enum": [1, 2, 3, 4, 5, 6, 7, 8, 9], "default": 1}
var encoded_2_1 = encode_constraints({"test": params_2_1})["test"]
assert_equal(animation.parameterized_object.constraint_mask(encoded_2_1, "enum"), 0x10, "2.1a: has enum")
assert_equal(animation.parameterized_object.constraint_find(encoded_2_1, "default", nil), 1, "2.1b: default value")
var enum_2_1 = animation.parameterized_object.constraint_find(encoded_2_1, "enum", nil)
assert_array_equal(enum_2_1, [1, 2, 3, 4, 5, 6, 7, 8, 9], "2.1c: enum values")

# Test 2.2: Enum with negative values
var params_2_2 = {"enum": [-1, 1], "default": 1}
var encoded_2_2 = encode_constraints({"test": params_2_2})["test"]
var enum_2_2 = animation.parameterized_object.constraint_find(encoded_2_2, "enum", nil)
assert_array_equal(enum_2_2, [-1, 1], "2.2a: enum with negative values")

# Test 2.3: Enum with min/max/default
var params_2_3 = {"min": 0, "max": 3, "enum": [0, 1, 2, 3], "default": 0}
var encoded_2_3 = encode_constraints({"test": params_2_3})["test"]
assert_equal(animation.parameterized_object.constraint_mask(encoded_2_3, "min"), 0x01, "2.3a: has min")
assert_equal(animation.parameterized_object.constraint_mask(encoded_2_3, "max"), 0x02, "2.3b: has max")
assert_equal(animation.parameterized_object.constraint_mask(encoded_2_3, "enum"), 0x10, "2.3c: has enum")
var enum_2_3 = animation.parameterized_object.constraint_find(encoded_2_3, "enum", nil)
assert_array_equal(enum_2_3, [0, 1, 2, 3], "2.3d: enum values")

# ============================================================================
# TEST GROUP 3: Type Annotations
# ============================================================================
print("\n--- Test Group 3: Type Annotations ---")

# Test 3.1: Bool type
var params_3_1 = {"type": "bool", "default": false}
var encoded_3_1 = encode_constraints({"test": params_3_1})["test"]
assert_equal(animation.parameterized_object.constraint_mask(encoded_3_1, "type"), 0x08, "3.1a: has type")
assert_equal(animation.parameterized_object.constraint_find(encoded_3_1, "type", nil), "bool", "3.1b: type is bool")
assert_equal(animation.parameterized_object.constraint_find(encoded_3_1, "default", nil), false, "3.1c: default is false")

# Test 3.2: String type
var params_3_2 = {"type": "string", "default": "animation"}
var encoded_3_2 = encode_constraints({"test": params_3_2})["test"]
assert_equal(animation.parameterized_object.constraint_mask(encoded_3_2, "type"), 0x08, "3.2a: has type")
assert_equal(animation.parameterized_object.constraint_find(encoded_3_2, "type", nil), "string", "3.2b: type is string")
assert_equal(animation.parameterized_object.constraint_find(encoded_3_2, "default", nil), "animation", "3.2c: default string")

# Test 3.3: Int type (explicit)
var params_3_3 = {"type": "int", "default": 3}
var encoded_3_3 = encode_constraints({"test": params_3_3})["test"]
assert_equal(animation.parameterized_object.constraint_mask(encoded_3_3, "type"), 0x08, "3.3a: has type")
assert_equal(animation.parameterized_object.constraint_find(encoded_3_3, "type", nil), "int", "3.3b: type is int")
assert_equal(animation.parameterized_object.constraint_find(encoded_3_3, "default", nil), 3, "3.3c: default int")

# Test 3.4: Any type
var params_3_4 = {"type": "any", "default": 255}
var encoded_3_4 = encode_constraints({"test": params_3_4})["test"]
assert_equal(animation.parameterized_object.constraint_mask(encoded_3_4, "type"), 0x08, "3.4a: has type")
assert_equal(animation.parameterized_object.constraint_find(encoded_3_4, "type", nil), "any", "3.4b: type is any")

# Test 3.5: Instance type
var params_3_5 = {"type": "instance", "default": nil}
var encoded_3_5 = encode_constraints({"test": params_3_5})["test"]
assert_equal(animation.parameterized_object.constraint_mask(encoded_3_5, "type"), 0x08, "3.5a: has type")
assert_equal(animation.parameterized_object.constraint_find(encoded_3_5, "type", nil), "instance", "3.5b: type is instance")

# Test 3.6: Function type
var params_3_6 = {"type": "function"}
var encoded_3_6 = encode_constraints({"test": params_3_6})["test"]
assert_equal(animation.parameterized_object.constraint_mask(encoded_3_6, "type"), 0x08, "3.6a: has type")
assert_equal(animation.parameterized_object.constraint_find(encoded_3_6, "type", nil), "function", "3.6b: type is function")

# Test 3.7: Bytes type
var params_3_7 = {"type": "bytes", "default": bytes("FF0000FF")}
var encoded_3_7 = encode_constraints({"test": params_3_7})["test"]
assert_equal(animation.parameterized_object.constraint_mask(encoded_3_7, "type"), 0x08, "3.7a: has type")
assert_equal(animation.parameterized_object.constraint_find(encoded_3_7, "type", nil), "bytes", "3.7b: type is bytes")
# Note: bytes comparison would need special handling

# ============================================================================
# TEST GROUP 4: Nillable Constraints
# ============================================================================
print("\n--- Test Group 4: Nillable Constraints ---")

# Test 4.1: Nillable with nil default
var params_4_1 = {"default": nil, "nillable": true}
var encoded_4_1 = encode_constraints({"test": params_4_1})["test"]
assert_equal(animation.parameterized_object.constraint_mask(encoded_4_1, "nillable"), 0x20, "4.1a: has nillable")
assert_equal(animation.parameterized_object.constraint_mask(encoded_4_1, "default"), 0x04, "4.1b: has default")
assert_equal(animation.parameterized_object.constraint_find(encoded_4_1, "nillable", false), true, "4.1c: nillable is true")
assert_equal(animation.parameterized_object.constraint_find(encoded_4_1, "default", 999), nil, "4.1d: default is nil")

# Test 4.2: Nillable without explicit default
var params_4_2 = {"nillable": true}
var encoded_4_2 = encode_constraints({"test": params_4_2})["test"]
assert_equal(animation.parameterized_object.constraint_mask(encoded_4_2, "nillable"), 0x20, "4.2a: has nillable")
assert_equal(animation.parameterized_object.constraint_find(encoded_4_2, "nillable", false), true, "4.2b: nillable is true")

# Test 4.3: Non-nillable (default behavior)
var params_4_3 = {"default": 0}
var encoded_4_3 = encode_constraints({"test": params_4_3})["test"]
assert_equal(animation.parameterized_object.constraint_mask(encoded_4_3, "nillable"), 0x00, "4.3a: no nillable flag")

# ============================================================================
# TEST GROUP 5: Real-World PARAMS from Codebase
# ============================================================================
print("\n--- Test Group 5: Real-World PARAMS ---")

# Test 5.1: BeaconAnimation PARAMS
var beacon_params = {
  "color": {"default": 0xFFFFFFFF},
  "back_color": {"default": 0xFF000000},
  "pos": {"default": 0},
  "beacon_size": {"min": 0, "default": 1},
  "slew_size": {"min": 0, "default": 0}
}
var beacon_encoded = encode_constraints(beacon_params)
assert_equal(animation.parameterized_object.constraint_find(beacon_encoded["color"], "default", nil), 0xFFFFFFFF, "5.1a: beacon color")
assert_equal(animation.parameterized_object.constraint_find(beacon_encoded["beacon_size"], "min", nil), 0, "5.1b: beacon_size min")

# Test 5.2: CometAnimation PARAMS
var comet_params = {
  "tail_length": {"min": 1, "max": 50, "default": 5},
  "speed": {"min": 1, "max": 25600, "default": 2560},
  "direction": {"enum": [-1, 1], "default": 1},
  "wrap_around": {"min": 0, "max": 1, "default": 1},
  "fade_factor": {"min": 0, "max": 255, "default": 179}
}
var comet_encoded = encode_constraints(comet_params)
assert_equal(animation.parameterized_object.constraint_find(comet_encoded["tail_length"], "max", nil), 50, "5.2a: tail_length max")
assert_equal(animation.parameterized_object.constraint_find(comet_encoded["speed"], "max", nil), 25600, "5.2b: speed max")
var direction_enum = animation.parameterized_object.constraint_find(comet_encoded["direction"], "enum", nil)
assert_array_equal(direction_enum, [-1, 1], "5.2c: direction enum")

# Test 5.3: Animation base class PARAMS
var animation_params = {
  "name": {"type": "string", "default": "animation"},
  "priority": {"min": 0, "default": 10},
  "duration": {"min": 0, "default": 0},
  "loop": {"type": "bool", "default": false},
  "opacity": {"type": "any", "default": 255},
  "color": {"default": 0xFFFFFFFF}
}
var animation_encoded = encode_constraints(animation_params)
assert_equal(animation.parameterized_object.constraint_find(animation_encoded["name"], "type", nil), "string", "5.3a: name type")
assert_equal(animation.parameterized_object.constraint_find(animation_encoded["name"], "default", nil), "animation", "5.3b: name default")
assert_equal(animation.parameterized_object.constraint_find(animation_encoded["loop"], "type", nil), "bool", "5.3c: loop type")
assert_equal(animation.parameterized_object.constraint_find(animation_encoded["opacity"], "type", nil), "any", "5.3d: opacity type")

# Test 5.4: GradientAnimation PARAMS (with nillable)
var gradient_params = {
  "color": {"default": nil, "nillable": true},
  "gradient_type": {"min": 0, "max": 1, "default": 0},
  "direction": {"min": 0, "max": 255, "default": 0}
}
var gradient_encoded = encode_constraints(gradient_params)
assert_equal(animation.parameterized_object.constraint_mask(gradient_encoded["color"], "nillable"), 0x20, "5.4a: color nillable")
assert_equal(animation.parameterized_object.constraint_find(gradient_encoded["color"], "default", 999), nil, "5.4b: color default nil")

# Test 5.5: OscillatorValueProvider PARAMS (large enum)
var oscillator_params = {
  "min_value": {"default": 0},
  "max_value": {"default": 100},
  "duration": {"min": 1, "default": 1000},
  "form": {"enum": [1, 2, 3, 4, 5, 6, 7, 8, 9], "default": 1},
  "phase": {"min": 0, "max": 100, "default": 0}
}
var oscillator_encoded = encode_constraints(oscillator_params)
var form_enum = animation.parameterized_object.constraint_find(oscillator_encoded["form"], "enum", nil)
assert_array_equal(form_enum, [1, 2, 3, 4, 5, 6, 7, 8, 9], "5.5a: form enum")

# Test 5.6: BreatheAnimation PARAMS
var breathe_params = {
  "base_color": {"default": 0xFFFFFFFF},
  "min_brightness": {"min": 0, "max": 255, "default": 0},
  "max_brightness": {"min": 0, "max": 255, "default": 255},
  "period": {"min": 100, "default": 3000},
  "curve_factor": {"min": 1, "max": 5, "default": 2}
}
var breathe_encoded = encode_constraints(breathe_params)
assert_equal(animation.parameterized_object.constraint_find(breathe_encoded["period"], "min", nil), 100, "5.6a: period min")
assert_equal(animation.parameterized_object.constraint_find(breathe_encoded["curve_factor"], "max", nil), 5, "5.6b: curve_factor max")

# ============================================================================
# TEST GROUP 6: Edge Cases and Special Scenarios
# ============================================================================
print("\n--- Test Group 6: Edge Cases ---")

# Test 6.1: Empty constraints (only default)
var params_6_1 = {"default": 42}
var encoded_6_1 = encode_constraints({"test": params_6_1})["test"]
assert_equal(animation.parameterized_object.constraint_mask(encoded_6_1, "min"), 0x00, "6.1a: no min")
assert_equal(animation.parameterized_object.constraint_mask(encoded_6_1, "max"), 0x00, "6.1b: no max")
assert_equal(animation.parameterized_object.constraint_mask(encoded_6_1, "enum"), 0x00, "6.1c: no enum")
assert_equal(animation.parameterized_object.constraint_find(encoded_6_1, "default", nil), 42, "6.1d: default value")

# Test 6.2: Zero values
var params_6_2 = {"min": 0, "max": 0, "default": 0}
var encoded_6_2 = encode_constraints({"test": params_6_2})["test"]
assert_equal(animation.parameterized_object.constraint_find(encoded_6_2, "min", nil), 0, "6.2a: zero min")
assert_equal(animation.parameterized_object.constraint_find(encoded_6_2, "max", nil), 0, "6.2b: zero max")
assert_equal(animation.parameterized_object.constraint_find(encoded_6_2, "default", nil), 0, "6.2c: zero default")

# Test 6.3: Single-element enum
var params_6_3 = {"enum": [42], "default": 42}
var encoded_6_3 = encode_constraints({"test": params_6_3})["test"]
var enum_6_3 = animation.parameterized_object.constraint_find(encoded_6_3, "enum", nil)
assert_array_equal(enum_6_3, [42], "6.3a: single-element enum")

# Test 6.4: Default not found (should return provided default)
var params_6_4 = {"min": 0, "max": 100}
var encoded_6_4 = encode_constraints({"test": params_6_4})["test"]
assert_equal(animation.parameterized_object.constraint_find(encoded_6_4, "default", 999), 999, "6.4a: missing default returns fallback")

# Test 6.5: Field not found (should return provided default)
assert_equal(animation.parameterized_object.constraint_find(encoded_6_4, "nonexistent", 777), 777, "6.5a: nonexistent field returns fallback")

# ============================================================================
# SUMMARY
# ============================================================================
print("\n" + "=" * 70)
print("TEST SUMMARY")
print("=" * 70)
print(f"Total tests: {test_count}")
print(f"Passed: {pass_count}")
print(f"Failed: {fail_count}")
if fail_count == 0
  print("\n✓ ALL TESTS PASSED!")
else
  print(f"\n✗ {fail_count} TEST(S) FAILED")
  raise "test_failed"
end
print("=" * 70)
