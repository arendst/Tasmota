# Pattern base class - The root of the animation hierarchy
# 
# A Pattern defines WHAT should be displayed - it can generate colors for any pixel
# at any time. Patterns have priority for layering and can be rendered directly.
# 
# This is the base class for both simple patterns and complex animations.

#@ solidify:Pattern,weak
class Pattern
  # Core pattern properties
  var priority        # Rendering priority (higher = on top) (int)
  var opacity         # Pattern opacity (0-255) (int)
  var name            # Optional name for the pattern (string)
  var is_running      # Whether the pattern is active (bool)
  var params          # Map of pattern parameters with their constraints (map)
  var param_values    # Map of current parameter values (map)
  
  # Initialize a new pattern
  #
  # @param priority: int - Rendering priority (higher = on top), defaults to 10 if nil
  # @param opacity: int - Pattern opacity (0-255), defaults to 255 if nil
  # @param name: string - Optional name for the pattern, defaults to "pattern" if nil
  def init(priority, opacity, name)
    self.priority = priority != nil ? priority : 10
    self.opacity = opacity != nil ? opacity : 255
    self.name = name != nil ? name : "pattern"
    self.is_running = false
    self.params = {}
    self.param_values = {}
    
    # Register common parameters with validation
    self._register_param("priority", {"min": 0, "default": 10})
    self._register_param("opacity", {"min": 0, "max": 255, "default": 255})
    
    # Set initial values for common parameters
    self.set_param("priority", self.priority)
    self.set_param("opacity", self.opacity)
  end
  
  # Start the pattern (make it active)
  #
  # @return self for method chaining
  def start()
    self.is_running = true
    return self
  end
  
  # Stop the pattern (make it inactive)
  #
  # @return self for method chaining
  def stop()
    self.is_running = false
    return self
  end
  
  # Update pattern state based on current time
  # Base patterns are typically stateless, but this allows for time-varying patterns
  #
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if pattern is still active, false if completed
  def update(time_ms)
    return self.is_running
  end
  
  # Render the pattern to the provided frame buffer
  # This is an abstract method that must be implemented by subclasses
  #
  # @param frame: FrameBuffer - The frame buffer to render to
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms)
    # This is an abstract method that should be overridden by subclasses
    # The base implementation does nothing
    return false
  end
  
  # Get a color for a specific pixel position and time
  # This is the core method that defines what a pattern looks like
  #
  # @param pixel: int - Pixel index (0-based)
  # @param time_ms: int - Current time in milliseconds
  # @return int - Color in ARGB format (0xAARRGGBB)
  def get_color_at(pixel, time_ms)
    # Base implementation returns white
    # Subclasses should override this to provide actual pattern logic
    return 0xFFFFFFFF
  end
  
  # Set the pattern priority
  #
  # @param priority: int - New priority value
  # @return self for method chaining
  def set_priority(priority)
    self.set_param("priority", priority)
    return self
  end
  
  # Set the pattern opacity
  #
  # @param opacity: int - New opacity value (0-255)
  # @return self for method chaining
  def set_opacity(opacity)
    self.set_param("opacity", opacity)
    return self
  end
  
  # Register a parameter with validation constraints
  #
  # @param name: string - Parameter name
  # @param constraints: map - Validation constraints for the parameter
  # @return self for method chaining
  def _register_param(name, constraints)
    if constraints == nil
      constraints = {}
    end
    
    self.params[name] = constraints
    return self
  end
  
  # Register a new parameter with validation constraints
  #
  # @param name: string - Parameter name
  # @param constraints: map - Validation constraints for the parameter
  # @return self for method chaining
  def register_param(name, constraints)
    return self._register_param(name, constraints)
  end
  
  # Validate a parameter value against its constraints
  #
  # @param name: string - Parameter name
  # @param value: any - Value to validate
  # @return bool - True if valid, false otherwise
  def _validate_param(name, value)
    if !self.params.contains(name)
      return false  # Parameter not registered
    end
    
    var constraints = self.params[name]
    
    # Check if value is nil and there's a default
    if value == nil && constraints.contains("default")
      value = constraints["default"]
    end
    
    # Accept ValueProvider instances for all parameters
    if animation.is_value_provider(value)
      return true
    end
    
    # Only accept integer values
    if type(value) != "int"
      return false
    end
    
    # Range validation for integer values
    if constraints.contains("min") && value < constraints["min"]
      return false
    end
    if constraints.contains("max") && value > constraints["max"]
      return false
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
        return false
      end
    end
    
    return true
  end
  
  # Set a parameter value with validation
  #
  # @param name: string - Parameter name
  # @param value: any - Value to set
  # @return bool - True if parameter was set, false if validation failed
  def set_param(name, value)
    import introspect

    # Check if parameter exists
    if !self.params.contains(name)
      return false
    end
    
    # Validate the value
    if !animation.is_value_provider(value)
      if !self._validate_param(name, value)
        return false
      end

      if introspect.contains(self, name)
        self.(name) = value
      else
        self.param_values[name] = value
      end
      
      self.on_param_changed(name, value)
    else
      if introspect.contains(self, name)
        self.(name) = value
      else
        self.param_values[name] = value
      end
    end
    
    return true
  end
  
  # Get a parameter value
  #
  # @param name: string - Parameter name
  # @param default_value: any - Default value if parameter not found
  # @return any - Parameter value or default
  def get_param(name, default_value)
    import introspect
    var method_name = "get_" + name
    if introspect.contains(self, method_name)
      var method = self.(method_name)
      return method(self)      # since it's not a method call, we need to pass the self as first parameter
    end

    if self.param_values.contains(name)
      return self.param_values[name]
    end
    
    if introspect.contains(self, name)
      return self.(name)
    end
    
    if self.params.contains(name) && self.params[name].contains("default")
      return self.params[name]["default"]
    end
    
    return default_value
  end
  
  # Helper method to resolve a value that can be either static or from a value provider
  #
  # @param value: any - Static value or value provider instance
  # @param param_name: string - Parameter name for specific get_XXX() method lookup
  # @param time_ms: int - Current time in milliseconds
  # @return any - The resolved value (static or from provider)
  def resolve_value(value, param_name, time_ms)
    if value == nil
      return nil
    end
    
    if animation.is_value_provider(value)
      if animation.is_color_provider(value)
        return value.get_color(time_ms)
      end
      
      var method_name = "get_" + param_name
      import introspect
      var method = introspect.get(value, method_name)
      
      if type(method) == "function"
        return method(value, time_ms)
      else
        return value.get_value(time_ms)
      end
    else
      return value
    end
  end
  
  # Get parameter metadata
  #
  # @param name: string - Parameter name
  # @return map - Parameter metadata or nil if not found
  def get_param_metadata(name)
    if self.params.contains(name)
      return self.params[name]
    end
    return nil
  end
  
  # Get all parameter metadata
  #
  # @return map - Map of all parameter metadata
  def get_params_metadata()
    return self.params
  end
  
  # Get all parameter values
  #
  # @return map - Map of all parameter values
  def get_params()
    return self.param_values
  end
  
  # Helper method to get a value from either a static value or a value provider
  # This method checks if the parameter contains a value provider instance,
  # and if so, calls the appropriate get_XXX() method on it.
  #
  # @param param_name: string - Name of the parameter
  # @param time_ms: int - Current time in milliseconds
  # @return any - The resolved value (static or from provider)
  def get_param_value(param_name, time_ms)
    var param_value = self.get_param(param_name, nil)
    
    if param_value == nil
      return nil
    end
    
    # Check if it's a value provider instance
    if animation.is_value_provider(param_value)
      # Check for ColorProvider first for optimal color handling
      if animation.is_color_provider(param_value)
        return param_value.get_color(time_ms)
      end
      
      # Try to call the specific get_XXX method for this parameter
      var method_name = "get_" + param_name
      
      # Use introspect to check if the method exists
      import introspect
      var method = introspect.get(param_value, method_name)
      
      if type(method) == "function"
        # Call the specific method (e.g., get_pulse_size())
        return method(param_value, time_ms)   # Pass the instance as first argument (self)
      else
        # Fall back to generic get_value method
        return param_value.get_value(time_ms)
      end
    else
      # It's a static value, return as-is
      return param_value
    end
  end
  
  # Helper method to set a parameter that can be either a static value or a value provider
  # This method automatically wraps static values in a StaticValueProvider if needed
  #
  # @param param_name: string - Name of the parameter
  # @param value: any - Static value or value provider instance
  # @return bool - True if parameter was set successfully
  def set_param_value(param_name, value)
    # If it's already a value provider, use it directly
    if animation.is_value_provider(value)
      return self.set_param(param_name, value)
    else
      # It's a static value, wrap it in a StaticValueProvider
      var static_provider = animation.static_value_provider(value)
      return self.set_param(param_name, static_provider)
    end
  end
  
  # Method called when a parameter is changed
  # Subclasses should override this to handle parameter changes
  #
  # @param name: string - Parameter name
  # @param value: any - New parameter value
  def on_param_changed(name, value)
    # Base implementation does nothing
  end
  
  # String representation of the pattern
  def tostring()
    return f"Pattern({self.name}, priority={self.priority}, opacity={self.opacity}, running={self.is_running})"
  end
end

return {'pattern': Pattern}