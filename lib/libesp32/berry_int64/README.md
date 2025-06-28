# Berry Int64 Library

A secure 64-bit integer implementation for Berry language on 32-bit architectures, specifically designed for embedded systems like ESP32.

## Overview

This library provides comprehensive 64-bit integer support for Berry applications running on 32-bit platforms where native 64-bit integer operations are not available or efficient. It integrates seamlessly with Berry's type system and memory management.

## Features

### Core Functionality
- **Complete 64-bit arithmetic**: Addition, subtraction, multiplication, division, modulo
- **Bitwise operations**: Left/right shifts with defined behavior
- **Comparison operations**: All standard comparison operators
- **Type conversions**: From/to strings, integers, floats, bytes
- **Memory efficient**: Optimized for embedded systems

### Security Features ✅
- **Secure string parsing**: Validates all string-to-integer conversions
- **Overflow detection**: Detects and reports arithmetic overflow
- **Memory safety**: Proper cleanup in all error paths
- **Defined behavior**: Eliminates undefined behavior in shift operations
- **Input validation**: Comprehensive validation of all inputs

## Installation

### PlatformIO
Add to your `platformio.ini`:
```ini
lib_deps = 
    https://github.com/your-repo/berry_int64
```

### Manual Installation
Copy the `src/` directory contents to your Berry library path.

## Usage

### Basic Operations
```berry
# Create int64 values
var a = int64(42)
var b = int64("1234567890123456789")
var c = int64.fromu32(0xFFFFFFFF, 0x12345678)

# Arithmetic operations
var sum = a + b
var product = a * b
var quotient = b / a

# Comparisons
if a > b
    print("a is greater")
end

# Type conversions
print(a.tostring())
print(a.toint())        # Convert to int32 (if in range)
print(a.tobool())       # Convert to boolean
```

### Advanced Features
```berry
# Bitwise operations
var shifted = a << 10
var masked = b >> 5

# Byte operations
var bytes_data = a.tobytes()
var from_bytes = int64.frombytes(bytes_data)

# Range checking
if a.isint()
    var safe_int = a.toint()
end

# Factory methods
var from_float = int64.fromfloat(3.14159)
var from_string = int64.fromstring("999999999999")
```

### Error Handling
```berry
# The library provides comprehensive error handling
try
    var invalid = int64("not_a_number")
except "value_error"
    print("Invalid string format")
end

try
    var overflow = int64.fromu32(0xFFFFFFFF, 0x7FFFFFFF) + int64(1)
except "overflow_error"
    print("Arithmetic overflow detected")
end

try
    var div_error = int64(10) / int64(0)
except "divzero_error"
    print("Division by zero")
end
```

## Security

This library has been thoroughly analyzed and hardened for security:

### ✅ **Security Features**
- **Input Validation**: All string inputs are validated using secure parsing
- **Overflow Detection**: Arithmetic operations detect and report overflow
- **Memory Safety**: No memory leaks, proper cleanup in all error paths
- **Defined Behavior**: Shift operations use wrapping to eliminate undefined behavior
- **Error Reporting**: Clear, specific error messages for debugging

### 🛡️ **Security Testing**
Run the security test suite:
```berry
load("security_tests.be")
```

The test suite validates:
- String parsing security (malformed inputs, overflow)
- Arithmetic overflow detection
- Shift operation defined behavior
- Division by zero protection
- Memory allocation robustness
- Buffer operation safety

## API Reference

### Constructors
```berry
int64()                    # Create int64 with value 0
int64(value)              # Create from int, real, string, bool, or int64
int64.fromu32(low, high)  # Create from two 32-bit values
int64.fromfloat(f)        # Create from float
int64.fromstring(s)       # Create from string (with validation)
int64.frombytes(bytes, idx) # Create from byte buffer
```

### Instance Methods
```berry
.tostring()               # Convert to string representation
.toint()                  # Convert to int32 (if in range)
.tobool()                 # Convert to boolean (non-zero = true)
.tobytes()                # Convert to byte representation
.isint()                  # Check if value fits in int32
.low32()                  # Get low 32 bits as int32
.high32()                 # Get high 32 bits as int32
```

### Operators
```berry
# Arithmetic
+, -, *, /, %             # Standard arithmetic operators
-* (unary minus)          # Negation

# Comparison  
==, !=, <, <=, >, >=     # All comparison operators

# Bitwise
<<, >>                    # Left and right shift (with wrapping)
```

## Performance

Optimized for embedded systems:
- **Memory efficient**: ~24-32 bytes per int64 instance
- **CPU optimized**: Minimal overhead for arithmetic operations
- **GC friendly**: Integrates with Berry's garbage collector
- **Cache efficient**: Compact data structures

## Compatibility

- **Berry Version**: Compatible with Berry mapping system
- **Platforms**: ESP32, ESP8266, and other 32-bit embedded platforms
- **Frameworks**: Arduino, ESP-IDF
- **Memory**: Minimum 4KB RAM recommended

## Error Types

The library defines specific error types for different failure modes:

- `"value_error"`: Invalid input values (malformed strings, out of range)
- `"overflow_error"`: Arithmetic overflow in operations
- `"divzero_error"`: Division or modulo by zero
- `"memory_error"`: Memory allocation failures

## Testing

### Basic Tests
```bash
# Run the original test suite
berry tests/int64.be
```

### Security Tests
```bash
# Run security validation tests
berry tests/security_tests.be
```

### Integration Tests
Test with your application to ensure proper integration with Berry's type system and garbage collector.

## Contributing

When contributing to this library:

1. **Security First**: All changes must maintain security properties
2. **Test Coverage**: Add tests for new functionality
3. **Documentation**: Update documentation for API changes
4. **Compatibility**: Maintain backward compatibility where possible

## License

MIT License - see LICENSE file for details.

## Security Disclosure

If you discover security vulnerabilities, please report them responsibly:
1. Do not create public issues for security vulnerabilities
2. Contact the maintainers directly
3. Provide detailed reproduction steps
4. Allow time for fixes before public disclosure

## Changelog

### Version 1.1 (Security Hardened)
- ✅ **SECURITY**: Fixed memory leaks in error paths
- ✅ **SECURITY**: Replaced unsafe `atoll()` with validated `strtoll()`
- ✅ **SECURITY**: Added arithmetic overflow detection
- ✅ **SECURITY**: Eliminated undefined behavior in shift operations
- ✅ **SECURITY**: Added comprehensive input validation
- ✅ **TESTING**: Added security test suite
- ✅ **DOCS**: Added security documentation

### Version 1.0 (Original)
- Basic 64-bit integer functionality
- Integration with Berry type system
- Comprehensive test suite
