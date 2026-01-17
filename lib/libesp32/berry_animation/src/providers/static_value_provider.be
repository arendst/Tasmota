# static_value for Berry Animation Framework
#
# This value provider returns a single, static value for any parameter type.
# It's a dummy implementation that serves as a wrapper for static values,
# providing the same interface as dynamic value providers.
#
# This provider uses the member() construct to respond to any get_XXX() method
# call with the same static value, making it a universal static provider.
#
# Follows the parameterized class specification:
# - Constructor takes only 'engine' parameter
# - Value is set via virtual member assignment after creation

import "./core/param_encoder" as encode_constraints

class static_value : animation.parameterized_object
  static var VALUE_PROVIDER = true
  # Parameter definitions
  static var PARAMS = animation.enc_params({
    "value": {"default": nil, "type": "any"}
  })
  

  # Produce the static value for any parameter name
  #
  # @param name: string - Parameter name being requested (ignored)
  # @param time_ms: int - Current time in milliseconds (ignored)
  # @return any - The static value
  def produce_value(name, time_ms)
    return self.value
  end
end

return {'static_value': static_value}