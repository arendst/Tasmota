# ParameterizedObject - Base class for parameter management
#
# This class provides a common parameter management system that can be shared
# between Animation and ValueProvider classes. It handles parameter validation,
# storage, and retrieval with support for ValueProvider instances.
#
# Parameters are stored in a 'values' map and accessed via virtual instance variables
# through member() and setmember() methods. Subclasses should not declare instance
# variables for parameters, but use the PARAMS system only.

import "./core/param_encoder" as encode_constraints

class ParameterizedObject
  var values          # Map storing all parameter values
  var engine          # Reference to the animation engine
  var start_time      # Time when object started (ms) (int), value is set at first call to update() or render()
  
  # Static parameter definitions - should be overridden by subclasses
  static var PARAMS = encode_constraints(
    {"is_running": {"type": "bool", "default": false}
  })   # Whether the object is active
  
  # Initialize parameter system
  #
  # @param engine: AnimationEngine - Reference to the animation engine (required)
  def init(engine)
    if engine == nil || type(engine) != "instance"
      raise "value_error", "missing engine parameter"
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
            var encoded_constraints = class_params[param_name]
            # Use static method to check for default value
            if self.constraint_mask(encoded_constraints, "default")
              self.values[param_name] = self.constraint_find(encoded_constraints, "default")
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
  # @return bytes - Encoded parameter constraints or nil if not found
  def _get_param_def(name)
    import introspect
    
    # Walk up the class hierarchy to find the parameter definition
    var current_class = classof(self)
    while current_class != nil
      # Check if this class has PARAMS
      if introspect.contains(current_class, "PARAMS")
        var class_params = current_class.PARAMS
        if class_params.contains(name)
          return class_params[name]  # Returns encoded bytes
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
      var encoded_constraints = self._get_param_def(name)
      if encoded_constraints != nil && self.constraint_mask(encoded_constraints, "default")
        return self.constraint_find(encoded_constraints, "default")
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
    var encoded_constraints = self._get_param_def(name)
    if encoded_constraints == nil
      raise "attribute_error", f"'{classname(self)}' object has no attribute '{name}'"
    end
    
    # Accept ValueProvider instances for all parameters
    if animation.is_value_provider(value)
      return value
    end
    
    # Handle nil values
    if value == nil
      # Check if nil is explicitly allowed via nillable attribute
      if self.constraint_mask(encoded_constraints, "nillable")
        return value  # nil is allowed for this parameter
      end
      
      # Check if there's a default value (nil is acceptable if there's a default)
      if self.constraint_mask(encoded_constraints, "default")
        return value  # nil is acceptable, will use default
      end
      
      # nil is not allowed for this parameter
      raise "value_error", f"'{name}' does not accept nil values"
    end
    
    # Type validation - default type is "int" if not specified
    var expected_type = self.constraint_find(encoded_constraints, "type", "int")
    
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
          raise "value_error", f"'{name}' expects type '{expected_type}' but got '{actual_type}' (value: {value})"
        end
      elif expected_type != actual_type
        raise "value_error", f"'{name}' expects type '{expected_type}' but got '{actual_type}' (value: {value})"
      end
    end
    
    # Range validation for integer values only
    if actual_type == "int"
      if self.constraint_mask(encoded_constraints, "min")
        var min_val = self.constraint_find(encoded_constraints, "min")
        if value < min_val
          raise "value_error", f"'{name}' value {value} is below minimum {min_val}"
        end
      end
      if self.constraint_mask(encoded_constraints, "max")
        var max_val = self.constraint_find(encoded_constraints, "max")
        if value > max_val
          raise "value_error", f"'{name}' value {value} is above maximum {max_val}"
        end
      end
    end
    
    # Enum validation
    if self.constraint_mask(encoded_constraints, "enum")
      var valid = false
      var enum_list = self.constraint_find(encoded_constraints, "enum")
      var list_size = size(enum_list)
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
        raise "value_error", f"'{name}' value {value} is not in allowed values {enum_list}"
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
    var encoded_constraints = self._get_param_def(name)
    if encoded_constraints != nil && self.constraint_mask(encoded_constraints, "default")
      return self.constraint_find(encoded_constraints, "default", default_value)
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
    if animation.is_value_provider(value)             # this also captures 'nil'
      return value.produce_value(param_name, time_ms)
    else
      return value
    end
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
  
  # Helper function to make sure both self.start_time and time_ms are valid
  #
  # If time_ms is nil, replace with time_ms from engine
  # Then initialize the value for self.start_time if not set already
  #
  # @param time_ms: int or nil - Current time in milliseconds
  # @return time_ms: int (guaranteed)
  def _fix_time_ms(time_ms)
    if time_ms == nil
      time_ms = self.engine.time_ms
    end
    # if time_ms == nil
    #   raise "value_error", "engine.time_ms should not be 'nil'"
    # end
    if self.start_time == nil
      self.start_time = time_ms
    end
    return time_ms
  end

  # Start the object - base implementation
  #
  # `start(time_ms)` is called whenever an animation is about to be run
  # by the animation engine directly or via a sequence manager.
  # For value providers, start is typically not called because instances
  # can be embedded in closures. So value providers must consider the first
  # call to `produce_value()` as a start of their internal time reference.
  # @param start_time: int - Optional start time in milliseconds
  # @return self for method chaining
  def start(time_ms)
    if time_ms == nil
      time_ms = self.engine.time_ms
    end
    # if time_ms == nil
    #   raise "value_error", "engine.time_ms should not be 'nil'"
    # end
    if self.start_time != nil   # reset time only if it was already started
      self.start_time = time_ms
    end
    # Set is_running directly in values map to avoid infinite loop
    self.values["is_running"] = true
    return self
  end
  
  # Method called when a parameter is changed
  # Subclasses should override this to handle parameter changes
  #
  # @param name: string - Parameter name
  # @param value: any - New parameter value
  def on_param_changed(name, value)
    if name == "is_running"
      if value == true
        # Start the object (but avoid infinite loop by not setting is_running again)
        # Call start method to handle start_time
        self.start(nil)
      elif value == false
        # Stop the object - just set the internal state
        # (is_running is already set to false by the parameter system)
      end
    end
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
  
  # ============================================================================
  # STATIC METHODS FOR ENCODED CONSTRAINT ACCESS
  # ============================================================================
  # PARAMETER CONSTRAINT ENCODING
  # ==============================
  #
  # Parameter constraints are encoded into a compact bytes() format for efficient
  # storage and transmission. Each value is prefixed with its own type byte for
  # maximum flexibility and correctness.
  #
  # Byte 0: Constraint mask (bit field)
  #   Bit 0 (0x01): has_min
  #   Bit 1 (0x02): has_max
  #   Bit 2 (0x04): has_default
  #   Bit 3 (0x08): has_explicit_type
  #   Bit 4 (0x10): has_enum
  #   Bit 5 (0x20): is_nillable
  #   Bits 6-7: reserved
  #
  # Bytes 1+: Type-prefixed values in order (min, max, default, enum)
  #   Each value consists of: [type_byte][value_data]
  #
  # Value Type Codes:
  #   0x00 = int8 (1 byte, signed -128 to 127)
  #   0x01 = int16 (2 bytes, signed -32768 to 32767)
  #   0x02 = int32 (4 bytes, signed integer)
  #   0x03 = string (1-byte length prefix + string bytes)
  #   0x04 = bytes (2-byte length prefix + byte data)
  #   0x05 = bool (1 byte, 0 or 1)
  #   0x06 = nil (0 bytes)
  #
  # Explicit Type Codes (semantic types for validation) (1 byte):
  #   0x00 = int
  #   0x01 = string
  #   0x02 = bytes
  #   0x03 = bool
  #   0x04 = any
  #   0x05 = instance
  #   0x06 = function
  #
  # ENCODING EXAMPLES:
  #
  # {"min": 0, "max": 255, "default": 128}
  #   => bytes("07 00 00 01 00FF 00 0080")  # 8 bytes
  #   Breakdown:
  #     07 = mask (has_min|has_max|has_default)
  #     00 00 = min (type=int8, value=0)
  #     01 00FF = max (type=int16, value=255)
  #     00 0080 = default (type=int8, value=128)
  #
  # {"enum": [1, 2, 3], "default": 1}
  #   => bytes("0C 00 01 03 00 01 00 02 00 03")  # 10 bytes
  #   Breakdown:
  #     0C = mask (has_enum|has_default)
  #     00 01 = default (type=int8, value=1)
  #     03 = enum count (3 values)
  #     00 01 = enum[0] (type=int8, value=1)
  #     00 02 = enum[1] (type=int8, value=2)
  #     00 03 = enum[2] (type=int8, value=3)
  #
  # {"default": nil, "nillable": true}
  #   => bytes("14 06")  # 2 bytes
  #   Breakdown:
  #     14 = mask (has_default|is_nillable)
  #     06 = default (type=nil, no value data)
  #
  # USAGE:
  #
  # Encoding constraints (see param_encoder.be):
  #   import param_encoder
  #   var encoded = param_encoder.encode_constraints({"min": 0, "max": 255, "default": 128})
  #
  # Checking if constraint contains a field:
  #   if ParameterizedObject.constraint_mask(encoded, "min")
  #     print("Has min constraint")
  #   end
  #
  # Getting constraint field value:
  #   var min_val = ParameterizedObject.constraint_find(encoded, "min", 0)
  #   var max_val = ParameterizedObject.constraint_find(encoded, "max", 255)
  # ============================================================================
  # Check if an encoded constraint contains a specific field (monolithic, no sub-calls)
  #
  # This static method provides fast access to encoded constraint metadata without
  # decoding the entire constraint. It directly checks the mask byte to determine
  # if a field is present.
  #
  # @param encoded_bytes: bytes - Encoded constraint in Hybrid format
  # @param name: string - Field name ("min", "max", "default", "enum", "nillable", "type")
  # @return bool - True if field exists, false otherwise
  #
  # Example:
  #   var encoded = bytes("07 00 00 FF 80")  # min=0, max=255, default=128
  #   ParameterizedObject.constraint_mask(encoded, "min")      # => true
  #   ParameterizedObject.constraint_mask(encoded, "enum")     # => false
  static var _MASK = [
    "min",      #- 0x01 HAS_MIN-#
    "max",      #- 0x02, HAS_MAX-#
    "default",  #- 0x04, HAS_DEFAULT-#
    "type",     #- 0x08, HAS_EXPLICIT_TYPE-#
    "enum",     #- 0x10, HAS_ENUM-#
    "nillable", #- 0x20, IS_NILLABLE-#
  ]
  static var _TYPES = [
    "int",        # 0x00
    "string",     # 0x01
    "bytes",      # 0x02
    "bool",       # 0x03
    "any",        # 0x04
    "instance",   # 0x05
    "function"    # 0x06
  ]
  static def constraint_mask(encoded_bytes, name)
    if size(encoded_bytes) > 0
      var index_mask = _class._MASK.find(name)
      if (index_mask != nil)
        return (encoded_bytes[0] & (1 << index_mask))
      end
    end
    return 0
  end
  
  # Find and return an encoded constraint field value (monolithic, no sub-calls)
  #
  # This static method extracts a specific field value from an encoded constraint
  # without decoding the entire structure. It performs direct byte reading with
  # inline type handling for maximum efficiency.
  #
  # @param encoded_bytes: bytes - Encoded constraint in Hybrid format
  # @param name: string - Field name ("min", "max", "default", "enum", "nillable", "type")
  # @param default: any - Default value if field not found
  # @return any - Field value or default
  #
  # Supported field names:
  #   - "min": Minimum value constraint (int)
  #   - "max": Maximum value constraint (int)
  #   - "default": Default value (any type)
  #   - "enum": List of allowed values (array)
  #   - "nillable": Whether nil is allowed (bool)
  #   - "type": Explicit type string ("int", "string", "bytes", "bool", "any", "instance", "function")
  #
  # Example:
  #   var encoded = bytes("07 00 00 FF 80")  # min=0, max=255, default=128
  #   ParameterizedObject.constraint_find(encoded, "min", 0)       # => 0
  #   ParameterizedObject.constraint_find(encoded, "max", 255)     # => 255
  #   ParameterizedObject.constraint_find(encoded, "default", 100) # => 128
  #   ParameterizedObject.constraint_find(encoded, "enum", nil)    # => nil (not present)
  
  static def constraint_find(encoded_bytes, name, default)

    # Helper: Skip a value with type prefix and return new offset
    def _skip_typed_value(encoded_bytes, offset)
      if offset >= size(encoded_bytes)  return 0  end
      var type_code = encoded_bytes[offset]
      
      if type_code == 0x06 #-NIL-#  return 1
      elif type_code == 0x05 #-BOOL-#  return 2
      elif type_code == 0x00 #-INT8-#  return 2
      elif type_code == 0x01 #-INT16-#  return 3
      elif type_code == 0x02 #-INT32-#  return 5
      elif type_code == 0x03 #-STRING-#  return 2 + encoded_bytes[offset + 1]
      elif type_code == 0x04 #-BYTES-#  return 3 + encoded_bytes.get(offset + 1, 2)
      end
      return 0
    end

    # Helper: Read a value with type prefix and return [value, new_offset]
    def _read_typed_value(encoded_bytes, offset)
      if offset >= size(encoded_bytes)  return nil  end
      var type_code = encoded_bytes[offset]
      offset += 1  # Skip type byte
      
      if type_code == 0x06 #-NIL-#  return nil
      elif type_code == 0x05 #-BOOL-#
        return encoded_bytes[offset] != 0
      elif type_code == 0x00 #-INT8-# 
        var v = encoded_bytes[offset]
        return v > 127 ? v - 256 : v
      elif type_code == 0x01 #-INT16-#
        var v = encoded_bytes.get(offset, 2)
        return v > 32767 ? v - 65536 : v
      elif type_code == 0x02 #-INT32-#
        return encoded_bytes.get(offset, 4)
      elif type_code == 0x03 #-STRING-#
        var len = encoded_bytes[offset]
        return encoded_bytes[offset + 1 .. offset + len].asstring()
      elif type_code == 0x04 #-BYTES-#
        var len = encoded_bytes.get(offset, 2)
        return encoded_bytes[offset + 2 .. offset + len + 1]
      end
      return nil
    end

    if size(encoded_bytes) < 1  return default  end
    var mask = encoded_bytes[0]
    var offset = 1
    
    # Quick check if field exists
    var target_mask = _class._MASK.find(name)   # nil or 0..5
    if (target_mask == nil) return default  end
    target_mask = (1 << target_mask)

    # If no match, quick fail
    if !(mask & target_mask)  return default  end

    # Easy check if 'nillable'
    if target_mask == 0x20 #-IS_NILLABLE-#
      return true           # since 'mask & target_mask' is true, we know we should return true
    end

    # Skip fields before target
    if target_mask > 0x01 #-HAS_MIN-# && (mask & 0x01 #-HAS_MIN-#)
      offset += _skip_typed_value(encoded_bytes, offset)
    end
    if target_mask > 0x02 #-HAS_MAX-# && (mask & 0x02 #-HAS_MAX-#)
      offset += _skip_typed_value(encoded_bytes, offset)
    end
    if target_mask > 0x04 #-HAS_DEFAULT-# && (mask & 0x04 #-HAS_DEFAULT-#)
      offset += _skip_typed_value(encoded_bytes, offset)
    end
    if target_mask > 0x08 #-HAS_EXPLICIT_TYPE-# && (mask & 0x08 #-HAS_EXPLICIT_TYPE-#)
      offset += 1
    end
    if offset >= size(encoded_bytes)  return default  end   # sanity check

    # Special case for explicit_type
    if target_mask == 0x08 #-HAS_EXPLICIT_TYPE-#
      # Read explicit type code and convert to string
      var type_byte = encoded_bytes[offset]                 # sanity check above guarantees that index is correct
      if type_byte < size(_class._TYPES)
        return _class._TYPES[type_byte]
      end
      return default
    end    
    
    # Read target value
    if target_mask == 0x10 #-HAS_ENUM-#
      var count = encoded_bytes[offset]
      offset += 1
      var result = []
      var i = 0
      while i < count
        var val_and_offset = 
        result.push(_read_typed_value(encoded_bytes, offset))
        offset += _skip_typed_value(encoded_bytes, offset)
        i += 1
      end
      return result
    end

    # All other cases
    return _read_typed_value(encoded_bytes, offset)
  end
end

return {'parameterized_object': ParameterizedObject}