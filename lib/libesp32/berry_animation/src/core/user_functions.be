# User-Defined Functions Registry for Berry Animation Framework
# This module manages external Berry functions that can be called from DSL code

# Register a Berry function for DSL use
def register_user_function(name, func)
  animation._user_functions[name] = func
end

# Retrieve a registered function by name
def get_user_function(name)
  return animation._user_functions.find(name)
end

# Check if a function is registered
def is_user_function(name)
  return animation._user_functions.contains(name)
end

# List all registered function names
def list_user_functions()
  var names = []
  for name : animation.user_functions.keys()
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