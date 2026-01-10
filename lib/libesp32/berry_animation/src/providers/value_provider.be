# value_provider utilities for Berry Animation Framework
#
# Value providers are parameterized_object subclasses with VALUE_PROVIDER = true.
# They generate values based on time, which can be used by animations
# for any parameter that needs to be dynamic over time.
#
# The value_provider class has been removed from the hierarchy.
# All value providers now inherit directly from parameterized_object
# and set `static var VALUE_PROVIDER = true`.

# Check if an object is a value provider
# Returns true if obj is a parameterized_object with VALUE_PROVIDER = true
def is_value_provider(obj)
  return obj != nil && type(obj) == "instance" && isinstance(obj, animation.parameterized_object) && obj.VALUE_PROVIDER
end

return {'is_value_provider': is_value_provider}
