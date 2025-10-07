# Test for nillable parameter attribute
import animation

import "./core/param_encoder" as encode_constraints

print("Testing nillable parameter attribute...")

# Create a test class with nillable and non-nillable parameters
class TestParameterizedClass : animation.parameterized_object
  static var PARAMS = encode_constraints({
    "nillable_param": {"type": "int", "nillable": true},
    "non_nillable_param": {"type": "int"}  # No default, no nillable
  })
  
  def init(engine)
    super(self).init(engine)
  end
end

# Create LED strip and engine
var strip = global.Leds(5)
var engine = animation.create_engine(strip)

# Test nillable parameter
var test_obj = TestParameterizedClass(engine)

# Test setting nil on a nillable parameter (should work)
test_obj.nillable_param = nil
assert(test_obj.nillable_param == nil, "Should accept nil for nillable parameter")

# Test setting a valid value on a nillable parameter (should work)
test_obj.nillable_param = 42
assert(test_obj.nillable_param == 42, "Should accept valid value for nillable parameter")

# Test setting nil back again (should work)
test_obj.nillable_param = nil
assert(test_obj.nillable_param == nil, "Should accept nil again for nillable parameter")

# Test that non-nillable parameter rejects nil
var success = test_obj.set_param("non_nillable_param", nil)
assert(success == false, "Should reject nil for non-nillable parameter")

# Test that non-nillable parameter accepts valid values
success = test_obj.set_param("non_nillable_param", 100)
assert(success == true, "Should accept valid value for non-nillable parameter")
assert(test_obj.non_nillable_param == 100, "Should store valid value for non-nillable parameter")

# Test gradient animation nillable color parameter
var gradient = animation.gradient_animation(engine)

# Test setting nil on gradient color (should work because it's nillable)
gradient.color = nil
assert(gradient.color == nil, "Should accept nil for nillable gradient color")

# Test setting a valid color (should work)
gradient.color = 0xFFFF0000
assert(gradient.color == 0xFFFF0000, "Should accept valid color for gradient")

print("✓ Nillable parameter attribute test passed!")

return true