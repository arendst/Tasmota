# ParameterizedObject - Base class for parameter management
#
# This class provides a common parameter management system that can be shared
# between Animation and ValueProvider classes. It handles parameter validation,
# storage, and retrieval with support for ValueProvider instances.
#
# Parameters are stored in a 'values' map and accessed via virtual instance variables
# through member() and setmember() methods. Subclasses should not declare instance
# variables for parameters, but use the PARAMS system only.

class ParameterizedObject
  var values          # Map storing all parameter values
  var engine          # Reference to the animation engine
  
  # Static parameter definitions - should be overridden by subclasses
  static var PARAMS = {}
  
  # Initialize parameter system
  #
  # @param engine: AnimationEngine - Reference to the animation engine (required)
  def init(engine)
    if engine == nil || type(engine) != "instance"
      raise "value_error", "ParameterizedObject requires an engine parameter"
    end
    
    self.engine = engine
    self.values = {}
    self._init_parameter_values()
  end
  
  # Private method to initialize parameter values from the class hierarchy
  def _init_parameter_values()
    import introspect
    
    # Walk up the class hierarchy to initialize parameters with defaults
    var current_class = classof(self)
    while current_class != nil
      # Check if this class has PARAMS
      if introspect.contains(current_class, "PARAMS")
        var class_params = current_class.PARAMS
        # Initialize parameters from this class with their default values
        for param_name : class_params.keys()
          # Only set if not already set (child class defaults take precedence)
          if !self.values.contains(param_name)
            var param_def = class_params[param_name]
            if param_def.contains("default")
              self.values[param_name] = param_def["default"]
            end
          end
        end
      end
      
      # Move to parent class
      current_class = super(current_class)
    end
  end
  
  # Private method to check if a parameter exists in the class hierarchy
  #
  # @param name: string - Parameter name to check
  # @return bool - True if parameter exists in any class in the hierarchy
  def _has_param(name)
    import introspect
    
    # Walk up the class hierarchy to find the parameter
    var current_class = classof(self)
    while current_class != nil
      # Check if this class has PARAMS
      if introspect.contains(current_class, "PARAMS")
        var class_params = current_class.PARAMS
        if class_params.contains(name)
          return true
        end
      end
      
      # Move to parent class
      current_class = super(current_class)
    end
    
    return false
  end
  
  # Private method to get parameter definition from the class hierarchy
  #
  # @param name: string - Parameter name
  # @return map - Parameter definition or nil if not found
  def _get_param_def(name)
    import introspect
    
    # Walk up the class hierarchy to find the parameter definition
    var current_class = classof(self)
    while current_class != nil
      # Check if this class has PARAMS
      if introspect.contains(current_class, "PARAMS")
        var class_params = current_class.PARAMS
        if class_params.contains(name)
          return class_params[name]
        end
      end
      
      # Move to parent class
      current_class = super(current_class)
    end
    
    return nil
  end
  
  # Virtual member access - allows obj.param_name syntax
  # This is called when accessing a member that doesn't exist as a real instance variable
  #
  # @param name: string - Parameter name being accessed
  # @return any - Resolved parameter value (ValueProvider resolved to actual value)
  def member(name)
    # Check if it's a parameter (either set in values or defined in PARAMS)
    if self.values.contains(name) || self._has_param(name)
      return self._resolve_parameter_value(name, self.engine.time_ms)
    end
    
    # Not a parameter, raise attribute error (consistent with setmember behavior)
    raise "attribute_error", f"'{classname(self)}' object has no attribute '{name}'"
  end
  
  # Virtual member assignment - allows obj.param_name = value syntax
  # This is called when setting a member that doesn't exist as a real instance variable
  #
  # @param name: string - Parameter name being set
  # @param value: any - Value to set (can be static value or ValueProvider)
  def setmember(name, value)
    # Check if it's a parameter in the class hierarchy and set it with validation
    if self._has_param(name)
      self._set_parameter_value(name, value)
    else
      # Not a parameter, this will cause an error in normal Berry behavior
      raise "attribute_error", f"'{classname(self)}' object has no attribute '{name}'"
    end
  end
  
  # Internal method to set a parameter value with validation
  #
  # @param name: string - Parameter name
  # @param value: any - Value to set (can be static value or ValueProvider)
  def _set_parameter_value(name, value)
    # Validate the value (skip validation for ValueProvider instances)
    if !animation.is_value_provider(value)
      value = self._validate_param(name, value)  # Get potentially converted value
    end
    
    # Store the value
    self.values[name] = value
    
    # Notify of parameter change
    self.on_param_changed(name, value)
  end
  
  # Internal method to resolve a parameter value (handles ValueProviders)
  #
  # @param name: string - Parameter name
  # @param time_ms: int - Current time in milliseconds for ValueProvider resolution
  # @return any - Resolved value (static or from ValueProvider)
  def _resolve_parameter_value(name, time_ms)
    if !self.values.contains(name)
      # Return default if available from class hierarchy
      var param_def = self._get_param_def(name)
      if param_def != nil && param_def.contains("default")
        return param_def["default"]
      end
      return nil
    end
    
    var value = self.values[name]
    
    # If it's a ValueProvider, resolve it using produce_value
    if animation.is_value_provider(value)
      return value.produce_value(name, time_ms)
    else
      # It's a static value, return as-is
      return value
    end
  end
  
  # Validate a parameter value against its constraints
  # Raises detailed exceptions for validation failures
  #
  # @param name: string - Parameter name
  # @param value: any - Value to validate (may be modified for real->int conversion)
  # @return any - Validated value (potentially converted from real to int)
  def _validate_param(name, value)
    var constraints = self._get_param_def(name)
    if constraints == nil
      raise "value_error", f"Parameter '{name}' is not defined for class '{classname(self)}'"
    end
    
    # Accept ValueProvider instances for all parameters
    if animation.is_value_provider(value)
      return value
    end
    
    # Handle nil values
    if value == nil
      # Check if nil is explicitly allowed via nillable attribute
      if constraints.contains("nillable") && constraints["nillable"] == true
        return value  # nil is allowed for this parameter
      end
      
      # Check if there's a default value (nil is acceptable if there's a default)
      if constraints.contains("default")
        return value  # nil is acceptable, will use default
      end
      
      # nil is not allowed for this parameter
      raise "value_error", f"Parameter '{name}' does not accept nil values"
    end
    
    # Type validation - default type is "int" if not specified
    var expected_type = "int"  # Default type
    if constraints.contains("type")
      expected_type = constraints["type"]
    end
    
    # Get actual type for validation
    var actual_type = type(value)
    
    # Skip type validation if expected type is "any"
    if expected_type != "any"
      # Special case: accept real values for int parameters and convert them
      if expected_type == "int" && actual_type == "real"
        import math
        value = int(math.round(value))
        actual_type = "int"
      # Special case: check for bytes type using isinstance()
      elif expected_type == "bytes"
        if actual_type == "instance" && isinstance(value, bytes)
          actual_type = "bytes"
        elif actual_type != "instance" || !isinstance(value, bytes)
          raise "value_error", f"Parameter '{name}' expects type '{expected_type}' but got '{actual_type}' (value: {value})"
        end
      elif expected_type != actual_type
        raise "value_error", f"Parameter '{name}' expects type '{expected_type}' but got '{actual_type}' (value: {value})"
      end
    end
    
    # Range validation for integer values only
    if actual_type == "int"
      if constraints.contains("min") && value < constraints["min"]
        raise "value_error", f"Parameter '{name}' value {value} is below minimum {constraints['min']}"
      end
      if constraints.contains("max") && value > constraints["max"]
        raise "value_error", f"Parameter '{name}' value {value} is above maximum {constraints['max']}"
      end
    end
    
    # Enum validation
    if constraints.contains("enum")
      var valid = false
      import introspect
      var enum_list = constraints["enum"]
      var list_size = enum_list.size()
      var i = 0
      while (i < list_size)
        var enum_value = enum_list[i]
        if value == enum_value
          valid = true
          break
        end
        i += 1
      end
      if !valid
        raise "value_error", f"Parameter '{name}' value {value} is not in allowed values {enum_list}"
      end
    end
    
    return value
  end
  
  # Set a parameter value with validation
  #
  # @param name: string - Parameter name
  # @param value: any - Value to set
  # @return bool - True if parameter was set, false if validation failed
  def set_param(name, value)
    # Check if parameter exists in class hierarchy
    if !self._has_param(name)
      return false
    end
    
    try
      self._set_parameter_value(name, value)
      return true
    except "value_error" as e
      # Validation failed - return false for method-based setting
      return false
    end
  end
  
  # Get a parameter value (returns raw stored value, not resolved)
  #
  # @param name: string - Parameter name
  # @param default_value: any - Default value if parameter not found
  # @return any - Parameter value or default (may be ValueProvider)
  def get_param(name, default_value)
    # Check stored values
    if self.values.contains(name)
      return self.values[name]
    end
    
    # Fall back to parameter default from class hierarchy
    var param_def = self._get_param_def(name)
    if param_def != nil && param_def.contains("default")
      return param_def["default"]
    end
    
    return default_value
  end
  
  # Helper method to resolve a value that can be either static or from a value provider
  #
  # @param value: any - Static value or value provider instance
  # @param param_name: string - Parameter name for specific produce_value() method lookup
  # @param time_ms: int - Current time in milliseconds
  # @return any - The resolved value (static or from provider)
  def resolve_value(value, param_name, time_ms)
    if value == nil
      return nil
    end
    
    if animation.is_value_provider(value)
      return value.produce_value(param_name, time_ms)
    else
      return value
    end
  end
  
  # Get parameter metadata
  #
  # @param name: string - Parameter name
  # @return map - Parameter metadata or nil if not found
  def get_param_metadata(name)
    return self._get_param_def(name)
  end
  
  # Get all parameter metadata from class hierarchy
  #
  # @return map - Map of all parameter metadata
  def get_params_metadata()
    import introspect
    var all_params = {}
    
    # Walk up the class hierarchy to collect all parameter definitions
    var current_class = classof(self)
    while current_class != nil
      # Check if this class has PARAMS
      if introspect.contains(current_class, "PARAMS")
        var class_params = current_class.PARAMS
        # Add parameters from this class (child class parameters override parent)
        for param_name : class_params.keys()
          if !all_params.contains(param_name)  # Don't override child class params
            all_params[param_name] = class_params[param_name]
          end
        end
      end
      
      # Move to parent class
      current_class = super(current_class)
    end
    
    return all_params
  end
  
  # Helper method to get a resolved value from either a static value or a value provider
  # This is the same as accessing obj.param_name but with explicit time
  #
  # @param param_name: string - Name of the parameter
  # @param time_ms: int - Current time in milliseconds
  # @return any - The resolved value (static or from provider)
  def get_param_value(param_name, time_ms)
    return self._resolve_parameter_value(param_name, time_ms)
  end
  
  # Start the object - placeholder for future implementation
  #
  # @return self for method chaining
  def start(time_ms)
    return self
  end
  
  # Method called when a parameter is changed
  # Subclasses should override this to handle parameter changes
  #
  # @param name: string - Parameter name
  # @param value: any - New parameter value
  def on_param_changed(name, value)
    # Default implementation does nothing
  end
  
  # Equality operator for object identity comparison
  # This prevents the member() method from being called during == comparisons
  #
  # @param other: any - Object to compare with
  # @return bool - True if objects are the same instance
  def ==(other)
    import introspect
    return introspect.toptr(self) == introspect.toptr(other)
  end
  
  # Inequality operator for object identity comparison
  # This prevents the member() method from being called during != comparisons
  #
  # @param other: any - Object to compare with
  # @return bool - True if objects are different instances
  def !=(other)
    return !(self == other)
  end
end

return {'parameterized_object': ParameterizedObject}