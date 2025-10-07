# Parameter Constraint Encoder for Berry Animation Framework
#
# This module provides functions to encode parameter constraints into a compact
# bytes() format with type-prefixed values for maximum flexibility and correctness.
#
# Encoding Format:
# ----------------
# Byte 0: Constraint mask (bit field)
#   Bit 0 (0x01): has_min
#   Bit 1 (0x02): has_max
#   Bit 2 (0x04): has_default
#   Bit 3 (0x08): has_explicit_type
#   Bit 4 (0x10): has_enum
#   Bit 5 (0x20): is_nillable
#   Bits 6-7: reserved
#
# Bytes 1+: Values in order (min, max, default, enum)
#   Each value is prefixed with its own type byte, followed by the value data.
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
# Value Encoding (each value has: type_byte + data):
#   - min: [type_byte][value_data]
#   - max: [type_byte][value_data]
#   - default: [type_byte][value_data]
#   - enum: [count_byte][type_byte][value_data][type_byte][value_data]...
#   - explicit_type: [type_code] (only if has_explicit_type bit is set)
#
# Explicit Type Codes (semantic types for validation) (1 byte):
#   0x00 = int
#   0x01 = string
#   0x02 = bytes
#   0x03 = bool
#   0x04 = any
#   0x05 = instance
#   0x06 = function

# Encode a full PARAMS map into a map of encoded constraints
#
# @param params_map: map - Map of parameter names to constraint definitions
# @return map - Map of parameter names to encoded bytes() objects
#
# Example:
#   encode_constraints({"color": {"default": 0xFFFFFFFF}, "size": {"min": 0, "max": 255, "default": 128}})
#   => {"color": bytes("04 02 FFFFFFFF"), "size": bytes("07 00 00 FF 80")}
def encode_constraints(params_map)
  # Nested function: Encode a single constraint map into bytes() format
  def encode_single_constraint(constraint_map)
    # Nested helper: Determine the appropriate type code for a value
    def get_type_code(value)
      var value_type = type(value)
      if value == nil  return 0x06 #-NIL-#
      elif value_type == "bool"  return 0x05 #-BOOL-#
      elif value_type == "string"  return 0x03 #-STRING-#
      elif value_type == "instance" && isinstance(value, bytes)  return 0x04 #-BYTES-#
      elif value_type == "int"
        # Use signed ranges: int8 for -128 to 127, int16 for larger values
        if value >= -128 && value <= 127  return 0x00 #-INT8-#
        elif value >= -32768 && value <= 32767  return 0x01 #-INT16-#
        else  return 0x02 #-INT32-#  end
      else  return 0x02 #-INT32-#  end
    end
    
    # Nested helper: Encode a single value with its type prefix
    def encode_value_with_type(value, result)
      var type_code = get_type_code(value)
      result.add(type_code, 1)  # Add type byte prefix
      
      if type_code == 0x06 #-NIL-#  return
      elif type_code == 0x05 #-BOOL-#  result.add(value ? 1 : 0, 1)
      elif type_code == 0x00 #-INT8-#  result.add(value & 0xFF, 1)
      elif type_code == 0x01 #-INT16-#  result.add(value & 0xFFFF, 2)
      elif type_code == 0x02 #-INT32-#  result.add(value, 4)
      elif type_code == 0x03 #-STRING-#
        var str_bytes = bytes().fromstring(value)
        result.add(size(str_bytes), 1)
        result .. str_bytes
      elif type_code == 0x04 #-BYTES-#
        result.add(size(value), 2)
        result .. value
      end
    end
    
    var mask = 0
    var result = bytes()
    
    # Reserve space for mask only (will be set at the end)
    result.resize(1)
    
    # Helper: Convert explicit type string to type code
    def get_explicit_type_code(type_str)
      if type_str == "int"  return 0x00
      elif type_str == "string"  return 0x01
      elif type_str == "bytes"  return 0x02
      elif type_str == "bool"  return 0x03
      elif type_str == "any"  return 0x04
      elif type_str == "instance"  return 0x05
      elif type_str == "function"  return 0x06
      end
      return 0x04  # Default to "any"
    end
    
    # Check if explicit type is specified
    var explicit_type_code = nil
    if constraint_map.contains("type")
      explicit_type_code = get_explicit_type_code(constraint_map["type"])
    end
    
    # Encode min value (with type prefix)
    if constraint_map.contains("min")
      mask |= 0x01 #-HAS_MIN-#
      encode_value_with_type(constraint_map["min"], result)
    end
    
    # Encode max value (with type prefix)
    if constraint_map.contains("max")
      mask |= 0x02 #-HAS_MAX-#
      encode_value_with_type(constraint_map["max"], result)
    end
    
    # Encode default value (with type prefix)
    if constraint_map.contains("default")
      mask |= 0x04 #-HAS_DEFAULT-#
      encode_value_with_type(constraint_map["default"], result)
    end
    
    # Encode explicit type code if present (1 byte)
    if explicit_type_code != nil
      mask |= 0x08 #-HAS_EXPLICIT_TYPE-#
      result.add(explicit_type_code, 1)
    end
    
    # Encode enum values (each with type prefix)
    if constraint_map.contains("enum")
      mask |= 0x10 #-HAS_ENUM-#
      var enum_list = constraint_map["enum"]
      result.add(size(enum_list), 1)  # Enum count
      for val : enum_list
        encode_value_with_type(val, result)
      end
    end
    
    # Set nillable flag
    if constraint_map.contains("nillable") && constraint_map["nillable"]
      mask |= 0x20 #-IS_NILLABLE-#
    end
    
    # Write mask at the beginning
    result.set(0, mask, 1)
    
    return result
  end
  
  # Encode each parameter constraint
  var result = {}
  for param_name : params_map.keys()
    result[param_name] = encode_single_constraint(params_map[param_name])
  end
  return result
end

# # Decode a single value from bytes according to type code
# #
# # @param encoded_bytes: bytes - bytes() object to read from
# # @param offset: int - Offset to start reading from
# # @param type_code: int - Type code for decoding
# # @return [value, new_offset] - Decoded value and new offset
# def decode_value(encoded_bytes, offset, type_code)
#   if type_code == 0x06 #-NIL-#
#     return [nil, offset]
#   elif type_code == 0x05 #-BOOL-#
#     return [encoded_bytes[offset] != 0, offset + 1]
#   elif type_code == 0x00 #-INT8-#
#     var val = encoded_bytes[offset]
#     # Handle signed int8
#     if val > 127
#       val = val - 256
#     end
#     return [val, offset + 1]
#   elif type_code == 0x01 #-INT16-#
#     var val = encoded_bytes.get(offset, 2)
#     # Handle signed int16
#     if val > 32767
#       val = val - 65536
#     end
#     return [val, offset + 2]
#   elif type_code == 0x02 #-INT32-#
#     return [encoded_bytes.get(offset, 4), offset + 4]
#   elif type_code == 0x03 #-STRING-#
#     var length = encoded_bytes[offset]
#     var str_bytes = encoded_bytes[offset + 1 .. offset + length]
#     return [str_bytes.asstring(), offset + 1 + length]
#   elif type_code == 0x04 #-BYTES-#
#     var length = encoded_bytes.get(offset, 2)
#     var byte_data = encoded_bytes[offset + 2 .. offset + 2 + length - 1]
#     return [byte_data, offset + 2 + length]
#   end
#   
#   return [nil, offset]
# end

# # Decode an encoded constraint bytes() back into a map
# #
# # @param encoded_bytes: bytes - Encoded constraint as bytes() object
# # @return map - Decoded constraint map
# #
# # Example:
# #   decode_constraint(bytes("07 00 00 FF 80"))
# #   => {"min": 0, "max": 255, "default": 128}
# def decode_constraint(encoded_bytes)
#   if size(encoded_bytes) < 2
#     return {}
#   end
#   
#   var mask = encoded_bytes[0]
#   var type_code = encoded_bytes[1]
#   var offset = 2
#   var result = {}
#   
#   # Decode min value
#   if mask & 0x01 #-HAS_MIN-#
#     var decoded = decode_value(encoded_bytes, offset, type_code)
#     result["min"] = decoded[0]
#     offset = decoded[1]
#   end
#   
#   # Decode max value
#   if mask & 0x02 #-HAS_MAX-#
#     var decoded = decode_value(encoded_bytes, offset, type_code)
#     result["max"] = decoded[0]
#     offset = decoded[1]
#   end
#   
#   # Decode default value
#   if mask & 0x04 #-HAS_DEFAULT-#
#     var decoded = decode_value(encoded_bytes, offset, type_code)
#     result["default"] = decoded[0]
#     offset = decoded[1]
#   end
#   
#   # Decode enum values
#   if mask & 0x10 #-HAS_ENUM-#
#     var count = encoded_bytes[offset]
#     offset += 1
#     result["enum"] = []
#     var i = 0
#     while i < count
#       var decoded = decode_value(encoded_bytes, offset, type_code)
#       result["enum"].push(decoded[0])
#       offset = decoded[1]
#       i += 1
#     end
#   end
#   
#   # Set nillable flag
#   if mask & 0x20 #-IS_NILLABLE-#
#     result["nillable"] = true
#   end
#   
#   # Add type annotation if not default int32
#   if type_code == 0x03 #-STRING-#
#     result["type"] = "string"
#   elif type_code == 0x04 #-BYTES-#
#     result["type"] = "bytes"
#   elif type_code == 0x05 #-BOOL-#
#     result["type"] = "bool"
#   elif type_code == 0x06 #-NIL-#
#     result["type"] = "nil"
#   end
#   
#   return result
# end

# Export only the encode function (decode not needed - use constraint_mask/constraint_find instead)
# Note: constraint_mask() and constraint_find() are static methods
# in ParameterizedObject class for accessing encoded constraints
return encode_constraints
