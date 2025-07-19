import json
import string
# load tests

def assert_load(text, value)
    var loaded_val = json.load(text)
    var ok = loaded_val == value
    if !ok 
        print(string.format('for JSON \'%s\' expected %s [%s] but got %s [%s]', text, str(value), type(value), str(loaded_val), type(loaded_val)))
    end
    assert(ok)
end

def assert_load_failed(text)
    assert(json.load(text) == nil)
end

assert_load('null', nil)
assert_load('true', true)
assert_load('false', false)
assert_load('123', 123)
assert_load('12.3', 12.3)
assert_load('-0.1', -0.1)
assert_load('1e2', 1e2)
assert_load('1e+2', 1e+2)
assert_load('1e-2', 1e-2)
assert_load('1E2', 1e2)
assert_load('1E+2', 1e+2)
assert_load('1.2e7', 1.2e7)
assert_load('"abc"', 'abc')
# strings
assert_load('"\\"\\\\\\/\\b\\f\\n\\r\\t"', '\"\\/\b\f\n\r\t')
assert_load('"\\u1234\\u2345\\u04aF\\u003A"', 'ሴ⍅ү:')
assert_load_failed('"\\u3fr"');
assert_load_failed('"\\q"');
assert_load_failed('"123');
# list
assert_load('[1, null]', [1, nil])
assert_load_failed('[x]')
assert_load_failed('[1, nil]')
assert_load_failed('[1, null')
# object
var o = json.load('{"key": 1}')
assert(o['key'] == 1 && o.size() == 1)

# parsing an empty string used to cause berry to pass a NULL to strncmp
# make sure we catch this
o = json.load('{"key": ""}')
assert(o['key'] == '' && o.size() == 1)

assert_load_failed('{"ke: 1}')
assert_load_failed('{"key": 1x}')
assert_load_failed('{"key"}')
assert_load_failed('{"key": 1, }')
# insanely long, nested object
var text = 'null'
for i : 0 .. 200
    text = '{"nested":' + text + ', "num": 1, "bool": true, "str": "abc", "n": null, "arr": [1, 2, 3]}'
end
json.load(text) # do nothing, just check that it doesn't crash

# dump tests

def assert_dump(value, text, format)
    assert(json.dump(value, format) == text)
end

assert_dump(nil, 'null');
assert_dump(true, 'true');
assert_dump(false, 'false');
assert_dump(1.23, '1.23');
assert_dump('String', '"String"');
assert_dump([1, 'x'], '[1,"x"]');
assert_dump({1: 'x'}, '{"1":"x"}');
assert_dump([1, 'x'], '[\n  1,\n  "x"\n]', 'format');
assert_dump({1: 'x'}, '{\n  "1": "x"\n}', 'format');
assert_dump({1: 'x', 'k': 'v'}, '{"k":"v","1":"x"}');

class map2 : map def init() super(self).init() end end
var m = map2()
m['key'] = 1
assert_dump(m, '{"key":1}')

# sweep dumping nested arrays of diffrent sizes
# this tests for any unexpanded stack conditions
for count : 10..200
    var arr = [[]]
    var last_arr = arr
    for i : 0..count
        var pushed = [i]
        last_arr.push(pushed)
        last_arr = pushed
    end
    json.dump(arr)
end

# Security tests for JSON parsing fixes

# Test 1: Unicode expansion buffer overflow protection
# Each \u0800 sequence (6 chars in JSON) becomes 3 UTF-8 bytes
# Old code would allocate only 1 byte per sequence, causing buffer overflow
def test_unicode_expansion()
    # Test single Unicode sequences of different byte lengths
    assert_load('"\\u0048"', 'H')        # 1 UTF-8 byte (ASCII)
    assert_load('"\\u00E9"', 'é')        # 2 UTF-8 bytes (Latin)
    assert_load('"\\u0800"', 'ࠀ')        # 3 UTF-8 bytes (Samaritan)
    
    # Test multiple Unicode sequences that would cause buffer overflow in old code
    var many_unicode = '"'
    for i: 0..49  # 50 sequences (0-49 inclusive), each \u0800 -> 3 bytes (150 bytes total vs 50 bytes old allocation)
        many_unicode += '\\u0800'
    end
    many_unicode += '"'
    
    var result = json.load('{"test": ' + many_unicode + '}')
    assert(result != nil, "Unicode expansion test should succeed")
    assert(size(result['test']) == 150, "Unicode expansion should produce 150 UTF-8 bytes")  # 50 * 3 bytes
end

# Test 2: Invalid Unicode sequence rejection
def test_invalid_unicode()
    # Invalid hex digits in Unicode sequences should be rejected
    assert_load_failed('"\\uXXXX"')      # Non-hex characters
    assert_load_failed('"\\u12XY"')      # Mixed valid/invalid hex
    assert_load_failed('"\\u"')          # Incomplete sequence
    assert_load_failed('"\\u123"')       # Too short
    assert_load_failed('"\\u123G"')      # Invalid hex digit
end

# Test 3: Control character validation
def test_control_characters()
    # Unescaped control characters (0x00-0x1F) should be rejected
    # Note: We need to create JSON strings with actual unescaped control characters
    assert_load_failed('{"test": "hello\x0Aworld"}')     # Unescaped newline (0x0A)
    assert_load_failed('{"test": "hello\x09world"}')     # Unescaped tab (0x09)
    assert_load_failed('{"test": "hello\x0Dworld"}')     # Unescaped carriage return (0x0D)
    assert_load_failed('{"test": "hello\x01world"}')     # Unescaped control char (0x01)
    
    # Properly escaped control characters should work
    var escaped_newline = json.load('{"test": "hello\\nworld"}')
    assert(escaped_newline != nil && escaped_newline['test'] == "hello\nworld", "Escaped newline should work")
    
    var escaped_tab = json.load('{"test": "hello\\tworld"}')
    assert(escaped_tab != nil && escaped_tab['test'] == "hello\tworld", "Escaped tab should work")
    
    var escaped_cr = json.load('{"test": "hello\\rworld"}')
    assert(escaped_cr != nil && escaped_cr['test'] == "hello\rworld", "Escaped carriage return should work")
end

# Test 4: Invalid escape sequence rejection
def test_invalid_escapes()
    # Invalid escape sequences should be rejected
    assert_load_failed('"\\q"')          # Invalid escape character
    assert_load_failed('"\\x"')          # Invalid escape character
    assert_load_failed('"\\z"')          # Invalid escape character
    assert_load_failed('"\\"')           # Incomplete escape at end
end

# Test 5: String length limits
def test_string_length_limits()
    # Test very long strings (should work up to limit)
    var long_str = '"'
    for i: 0..999  # 1000 character string (0-999 inclusive)
        long_str += 'a'
    end
    long_str += '"'
    
    var result = json.load('{"test": ' + long_str + '}')
    assert(result != nil, "Long string within limits should work")
    assert(size(result['test']) == 1000, "Long string should have correct length")
end

# Test 6: Mixed Unicode and ASCII (realistic scenario)
def test_mixed_content()
    # Test realistic mixed content that could trigger the vulnerability
    var mixed = '{"message": "Hello \\u4E16\\u754C! Welcome to \\u0048\\u0065\\u006C\\u006C\\u006F world."}'
    var result = json.load(mixed)
    assert(result != nil, "Mixed Unicode/ASCII should work")
    assert(result['message'] == "Hello 世界! Welcome to Hello world.", "Mixed content should decode correctly")
end

# Test 7: Edge cases
def test_edge_cases()
    # Empty string
    var empty_result = json.load('{"empty": ""}')
    assert(empty_result != nil && empty_result['empty'] == "", "Empty string should work")
    
    # String with only Unicode
    var unicode_result = json.load('{"unicode": "\\u0048\\u0065\\u006C\\u006C\\u006F"}')
    assert(unicode_result != nil && unicode_result['unicode'] == "Hello", "Unicode-only string should work")
    
    # String with only escapes
    var escapes_result = json.load('{"escapes": "\\n\\t\\r\\\\\\\""}')
    assert(escapes_result != nil && escapes_result['escapes'] == "\n\t\r\\\"", "Escape-only string should work")
    
    # Maximum valid Unicode value
    var max_unicode_result = json.load('{"max_unicode": "\\uFFFF"}')
    assert(max_unicode_result != nil, "Maximum Unicode value should work")
end

# Test 8: Malformed JSON strings
def test_malformed_strings()
    # Unterminated strings
    assert_load_failed('{"test": "unterminated')
    assert_load_failed('{"test": "unterminated\\')
    
    # Invalid JSON structure with string issues
    assert_load_failed('{"test": "valid"x}')
    assert_load_failed('{"test": "\\uXXXX", "other": "valid"}')
end

# Test 9: Nested objects with Unicode (stress test)
def test_nested_unicode_stress()
    # Create nested structure with Unicode to test memory management
    var nested = '{"level0": {"unicode": "\\u0800\\u0801\\u0802", "level1": {"unicode": "\\u0800\\u0801\\u0802", "final": "\\u4E16\\u754C"}}}'
    
    var result = json.load(nested)
    assert(result != nil, "Nested Unicode structure should parse successfully")
end

# Test 10: Security regression test
def test_security_regression()
    # This specific pattern would cause buffer overflow in the original code
    # \u0800 sequences: 6 chars in JSON -> 3 bytes in UTF-8 (50% expansion)
    var attack_pattern = '{"payload": "'
    for i: 0..99  # 100 sequences (0-99 inclusive) = 600 chars in JSON, 300 bytes needed, but old code allocated only 100 bytes
        attack_pattern += '\\u0800'
    end
    attack_pattern += '"}'
    
    var result = json.load(attack_pattern)
    assert(result != nil, "Security regression test should not crash")
    assert(size(result['payload']) == 300, "Should produce exactly 300 UTF-8 bytes")  # 100 * 3 bytes
end

# Run all security tests
test_unicode_expansion()
test_invalid_unicode()
test_control_characters()
test_invalid_escapes()
test_string_length_limits()
test_mixed_content()
test_edge_cases()
test_malformed_strings()
test_nested_unicode_stress()
test_security_regression()
