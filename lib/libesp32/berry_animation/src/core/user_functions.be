# User-Defined Functions Registry for Berry Animation Framework
# This module manages external Berry functions that can be called from DSL code

#@ solidify:animation_user_functions,weak

# Module-level storage for user-defined functions
import global
global._animation_user_functions = {}

# Register a Berry function for DSL use
def register_user_function(name, func)
  import global
  global._animation_user_functions[name] = func
end

# Retrieve a registered function by name
def get_user_function(name)
  import global
  return global._animation_user_functions.find(name)
end

# Check if a function is registered
def is_user_function(name)
  import global
  return global._animation_user_functions.contains(name)
end

# List all registered function names
def list_user_functions()
  import global
  var names = []
  for name : global._animation_user_functions.keys()
    names.push(name)
  end
  return names
end

# Export all functions
return {
  "register_user_function": register_user_function,
  "get_user_function": get_user_function,
  "is_user_function": is_user_function,
  "list_user_functions": list_user_functions
}