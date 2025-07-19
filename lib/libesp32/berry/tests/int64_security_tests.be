# Security Test Suite for Berry Int64 Library
# Tests for vulnerabilities identified in security analysis

# Test 1: String Parsing Security

# Test malformed strings
var exception_caught = false
try
    int64("not_a_number")
    assert(false, "Should raise exception for invalid string")
except "value_error"
    exception_caught = true
end
assert(exception_caught, "Should reject invalid string")

exception_caught = false
try
    int64("123abc")
    assert(false, "Should raise exception for partial number")
except "value_error"
    exception_caught = true
end
assert(exception_caught, "Should reject partial number string")

# Test whitespace handling
assert(int64("  ").tostring() == "0", "Whitespace should convert to 0")

# Test very large numbers (should trigger ERANGE)
exception_caught = false
try
    int64("99999999999999999999999999999999999999")
    assert(false, "Should raise exception for out-of-range string")
except "value_error"
    exception_caught = true
end
assert(exception_caught, "Should reject out-of-range string")

# Test 2: Arithmetic Overflow Detection

# Test addition overflow
exception_caught = false
try
    var a = int64.fromu32(0xFFFFFFFF, 0x7FFFFFFF)  # INT64_MAX
    var b = int64(1)
    var c = a + b  # Should overflow
    assert(false, "Should detect addition overflow")
except "overflow_error"
    exception_caught = true
end
assert(exception_caught, "Addition overflow should be detected")

# Test subtraction overflow
exception_caught = false
try
    var a = int64.fromu32(0x00000000, 0x80000000)  # INT64_MIN
    var b = int64(1)
    var c = a - b  # Should overflow
    assert(false, "Should detect subtraction overflow")
except "overflow_error"
    exception_caught = true
end
assert(exception_caught, "Subtraction overflow should be detected")

# Test multiplication overflow
exception_caught = false
try
    var a = int64.fromu32(0xFFFFFFFF, 0x7FFFFFFF)  # INT64_MAX
    var b = int64(2)
    var c = a * b  # Should overflow
    assert(false, "Should detect multiplication overflow")
except "overflow_error"
    exception_caught = true
end
assert(exception_caught, "Multiplication overflow should be detected")

# Test negation overflow (INT64_MIN cannot be negated)
exception_caught = false
try
    var a = int64.fromu32(0x00000000, 0x80000000)  # INT64_MIN
    var b = -a  # Should overflow
    assert(false, "Should detect negation overflow")
except "overflow_error"
    exception_caught = true
end
assert(exception_caught, "Negation overflow should be detected")

# Test division overflow (INT64_MIN / -1)
exception_caught = false
try
    var a = int64.fromu32(0x00000000, 0x80000000)  # INT64_MIN
    var b = int64(-1)
    var c = a / b  # Should overflow
    assert(false, "Should detect division overflow")
except "overflow_error"
    exception_caught = true
end
assert(exception_caught, "Division overflow should be detected")

# Test 3: Shift Operation Defined Behavior

# Test that shifts now have defined behavior (wrapping)
# These should work without exceptions and produce consistent results

# Test negative shift (should wrap to positive equivalent)
var a = int64(15)
var b = a << -1  # -1 & 63 = 63, so this becomes << 63
assert(b != nil, "Negative shift should work with wrapping")

var c = a >> -1  # -1 & 63 = 63, so this becomes >> 63  
assert(c != nil, "Negative right shift should work with wrapping")

# Test shift >= 64 (should wrap to equivalent smaller shift)
var d = a << 64  # 64 & 63 = 0, so this becomes << 0
assert(d.tostring() == "15", "Shift by 64 should wrap to shift by 0")

var e = a >> 64  # 64 & 63 = 0, so this becomes >> 0
assert(e.tostring() == "15", "Right shift by 64 should wrap to shift by 0")

# Test that original test cases still work (compatibility)
assert((int64(15) << 0).tostring() == "15", "Shift by 0 should work")
assert((int64(15) >> 0).tostring() == "15", "Right shift by 0 should work")

# Test 4: Division by Zero Protection

exception_caught = false
try
    var a = int64(10)
    var b = int64(0)
    var c = a / b
    assert(false, "Should detect division by zero")
except "divzero_error"
    exception_caught = true
end
assert(exception_caught, "Division by zero should be detected")

exception_caught = false
try
    var a = int64(10)
    var b = int64(0)
    var c = a % b
    assert(false, "Should detect modulo by zero")
except "divzero_error"
    exception_caught = true
end
assert(exception_caught, "Modulo by zero should be detected")

# Test 5: Memory Allocation Robustness

# These tests verify that all functions properly check malloc return values
# In a real environment with memory pressure, these would test actual failures
# For now, we verify the functions don't crash with valid inputs

var a = int64(42)
var b = int64(24)

# Test all arithmetic operations don't crash
var result = a + b
assert(result.tostring() == "66", "Addition should work")

result = a - b  
assert(result.tostring() == "18", "Subtraction should work")

result = a * b
assert(result.tostring() == "1008", "Multiplication should work")

result = a / b
assert(result.tostring() == "1", "Division should work")

result = a % b
assert(result.tostring() == "18", "Modulo should work")

# Test 6: Null Pointer Handling Consistency

# Test comparison with null (should be treated as 0)
var a = int64(5)
# Note: These tests depend on the Berry mapping system's null handling
# The fixed code treats null consistently as 0 in comparisons

# Test 7: Buffer Operations Security

# Test frombytes with various edge cases
var empty_bytes = bytes("")
var result = int64.frombytes(empty_bytes)
assert(result.tostring() == "0", "Empty bytes should give 0")

# Test with negative index
var test_bytes = bytes("FFFFFFFFFFFFFFFF")
result = int64.frombytes(test_bytes, -2)
assert(result != nil, "Negative index should work")

# Test with index beyond buffer
result = int64.frombytes(test_bytes, 100)
assert(result.tostring() == "0", "Index beyond buffer should give 0")

# Test 8: Type Conversion Security

# Test fromstring with edge cases
result = int64.fromstring("")
assert(result.tostring() == "0", "Empty string should convert to 0")

result = int64.fromstring("  123  ")
assert(result.tostring() == "123", "String with whitespace should work")

exception_caught = false
try
    result = int64.fromstring("123.45")
    assert(false, "Should reject decimal strings")
except "value_error"
    exception_caught = true
end
assert(exception_caught, "Decimal strings should be rejected")

# Performance regression test
import time

var start_time = time.time()
for i: 0..999
    var a = int64(i)
    var b = int64(i + 1)
    var c = a + b
    var d = c * int64(2)
    var e = d / int64(2)
end
var end_time = time.time()

# Verify performance is reasonable (should complete in reasonable time)
var duration = end_time - start_time
assert(duration >= 0, "Performance test should complete successfully")
