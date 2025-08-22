# Token System Test Suite
# Tests for Token class

import string
import animation
import animation_dsl

# Test Token constants and utilities
def test_token_type_constants()
  print("Testing Token constants...")
  
  # Test that all constants are defined and unique
  var token_types = [
    animation_dsl.Token.KEYWORD, animation_dsl.Token.IDENTIFIER, animation_dsl.Token.NUMBER,
    animation_dsl.Token.STRING, animation_dsl.Token.COLOR, animation_dsl.Token.TIME,
    animation_dsl.Token.PERCENTAGE, animation_dsl.Token.MULTIPLIER, animation_dsl.Token.ASSIGN,
    animation_dsl.Token.PLUS, animation_dsl.Token.MINUS, animation_dsl.Token.MULTIPLY,
    animation_dsl.Token.DIVIDE, animation_dsl.Token.MODULO, animation_dsl.Token.POWER,
    animation_dsl.Token.EQUAL, animation_dsl.Token.NOT_EQUAL, animation_dsl.Token.LESS_THAN,
    animation_dsl.Token.LESS_EQUAL, animation_dsl.Token.GREATER_THAN, animation_dsl.Token.GREATER_EQUAL,
    animation_dsl.Token.LOGICAL_AND, animation_dsl.Token.LOGICAL_OR, animation_dsl.Token.LOGICAL_NOT,
    animation_dsl.Token.LEFT_PAREN, animation_dsl.Token.RIGHT_PAREN, animation_dsl.Token.LEFT_BRACE,
    animation_dsl.Token.RIGHT_BRACE, animation_dsl.Token.LEFT_BRACKET, animation_dsl.Token.RIGHT_BRACKET,
    animation_dsl.Token.COMMA, animation_dsl.Token.SEMICOLON, animation_dsl.Token.COLON,
    animation_dsl.Token.DOT, animation_dsl.Token.ARROW, animation_dsl.Token.NEWLINE,
    animation_dsl.Token.VARIABLE_REF, animation_dsl.Token.COMMENT, animation_dsl.Token.EOF,
    animation_dsl.Token.ERROR
  ]
  
  # Check that all values are different
  var seen = {}
  for token_type : token_types
    if seen.contains(token_type)
      print(f"ERROR: Duplicate token type value: {token_type}")
      return false
    end
    seen[token_type] = true
  end
  
  # Test to_string method
  assert(animation_dsl.Token.to_string(animation_dsl.Token.KEYWORD) == "KEYWORD")
  assert(animation_dsl.Token.to_string(animation_dsl.Token.IDENTIFIER) == "IDENTIFIER")
  assert(animation_dsl.Token.to_string(animation_dsl.Token.EOF) == "EOF")
  assert(animation_dsl.Token.to_string(999) == "UNKNOWN")
  
  print("✓ Token constants test passed")
  return true
end

# Test Token class basic functionality
def test_token_basic()
  print("Testing Token basic functionality...")
  
  # Test basic token creation
  var token = animation_dsl.Token(animation_dsl.Token.KEYWORD, "color", 1, 5, 5)
  assert(token.type == animation_dsl.Token.KEYWORD)
  assert(token.value == "color")
  assert(token.line == 1)
  assert(token.column == 5)
  assert(token.length == 5)
  
  # Test default length calculation
  var token2 = animation_dsl.Token(animation_dsl.Token.IDENTIFIER, "red", 2, 10)
  assert(token2.length == 3)  # Should default to size of "red"
  
  # Test nil handling
  var token3 = animation_dsl.Token(animation_dsl.Token.EOF, nil, nil, nil)
  assert(token3.value == "")
  assert(token3.line == 1)
  assert(token3.column == 1)
  
  print("✓ Token basic functionality test passed")
  return true
end

# Test Token type checking methods
def test_token_type_checking()
  print("Testing Token type checking methods...")
  
  var keyword_token = animation_dsl.Token(animation_dsl.Token.KEYWORD, "color", 1, 1)
  var identifier_token = animation_dsl.Token(animation_dsl.Token.IDENTIFIER, "red", 1, 1)
  var number_token = animation_dsl.Token(animation_dsl.Token.NUMBER, "123", 1, 1)
  var operator_token = animation_dsl.Token(animation_dsl.Token.PLUS, "+", 1, 1)
  var delimiter_token = animation_dsl.Token(animation_dsl.Token.LEFT_PAREN, "(", 1, 1)
  var separator_token = animation_dsl.Token(animation_dsl.Token.COMMA, ",", 1, 1)
  
  # Test is_type
  assert(keyword_token.is_type(animation_dsl.Token.KEYWORD))
  assert(!keyword_token.is_type(animation_dsl.Token.IDENTIFIER))
  
  # Test is_keyword
  assert(keyword_token.is_keyword("color"))
  assert(!keyword_token.is_keyword("red"))
  assert(!identifier_token.is_keyword("color"))
  
  # Test is_identifier
  assert(identifier_token.is_identifier("red"))
  assert(!identifier_token.is_identifier("blue"))
  assert(!keyword_token.is_identifier("red"))
  
  # Test is_operator
  assert(operator_token.is_operator())
  assert(!keyword_token.is_operator())
  
  # Test is_delimiter
  assert(delimiter_token.is_delimiter())
  assert(!keyword_token.is_delimiter())
  
  # Test is_separator
  assert(separator_token.is_separator())
  assert(!keyword_token.is_separator())
  
  # Test is_literal
  assert(number_token.is_literal())
  assert(!keyword_token.is_literal())
  
  print("✓ Token type checking test passed")
  return true
end

# Test Token value extraction methods
def test_token_value_extraction()
  print("Testing Token value extraction methods...")
  
  # Test boolean tokens
  var true_token = animation_dsl.Token(animation_dsl.Token.KEYWORD, "true", 1, 1)
  var false_token = animation_dsl.Token(animation_dsl.Token.KEYWORD, "false", 1, 1)
  var other_token = animation_dsl.Token(animation_dsl.Token.KEYWORD, "color", 1, 1)
  
  assert(true_token.is_boolean())
  assert(false_token.is_boolean())
  assert(!other_token.is_boolean())
  
  assert(true_token.get_boolean_value() == true)
  assert(false_token.get_boolean_value() == false)
  assert(other_token.get_boolean_value() == nil)
  
  # Test numeric tokens
  var number_token = animation_dsl.Token(animation_dsl.Token.NUMBER, "123.45", 1, 1)
  var time_token = animation_dsl.Token(animation_dsl.Token.TIME, "2s", 1, 1)
  var percent_token = animation_dsl.Token(animation_dsl.Token.PERCENTAGE, "50%", 1, 1)
  var multiplier_token = animation_dsl.Token(animation_dsl.Token.MULTIPLIER, "2.5x", 1, 1)
  
  assert(number_token.is_numeric())
  assert(time_token.is_numeric())
  assert(percent_token.is_numeric())
  assert(multiplier_token.is_numeric())
  
  assert(number_token.get_numeric_value() == 123)  # Converted to int
  assert(time_token.get_numeric_value() == 2000)  # 2s = 2000ms (already int)
  assert(percent_token.get_numeric_value() == 127 || percent_token.get_numeric_value() == 128)  # 50% = ~127-128 in 0-255 range
  assert(multiplier_token.get_numeric_value() == 640)  # 2.5x = 2.5 * 256 = 640
  
  # Test time conversion
  var ms_token = animation_dsl.Token(animation_dsl.Token.TIME, "500ms", 1, 1)
  var s_token = animation_dsl.Token(animation_dsl.Token.TIME, "3s", 1, 1)
  var m_token = animation_dsl.Token(animation_dsl.Token.TIME, "2m", 1, 1)
  var h_token = animation_dsl.Token(animation_dsl.Token.TIME, "1h", 1, 1)
  
  assert(ms_token.get_numeric_value() == 500)
  assert(s_token.get_numeric_value() == 3000)
  assert(m_token.get_numeric_value() == 120000)
  assert(h_token.get_numeric_value() == 3600000)
  
  # Test percentage to 255 conversion
  var percent_0 = animation_dsl.Token(animation_dsl.Token.PERCENTAGE, "0%", 1, 1)
  var percent_50 = animation_dsl.Token(animation_dsl.Token.PERCENTAGE, "50%", 1, 1)
  var percent_100 = animation_dsl.Token(animation_dsl.Token.PERCENTAGE, "100%", 1, 1)
  
  assert(percent_0.get_numeric_value() == 0)
  assert(percent_50.get_numeric_value() == 127 || percent_50.get_numeric_value() == 128)  # Allow rounding
  assert(percent_100.get_numeric_value() == 255)
  
  print("✓ Token value extraction test passed")
  return true
end

# Test Token utility methods
def test_token_utilities()
  print("Testing Token utility methods...")
  
  var token = animation_dsl.Token(animation_dsl.Token.IDENTIFIER, "test", 5, 10, 4)
  
  # Test end_column
  assert(token.end_column() == 13)  # 10 + 4 - 1
  
  # Test with_type
  var new_token = token.with_type(animation_dsl.Token.KEYWORD)
  assert(new_token.type == animation_dsl.Token.KEYWORD)
  assert(new_token.value == "test")
  assert(new_token.line == 5)
  assert(new_token.column == 10)
  
  # Test with_value
  var new_token2 = token.with_value("newvalue")
  assert(new_token2.type == animation_dsl.Token.IDENTIFIER)
  assert(new_token2.value == "newvalue")
  assert(new_token2.length == 8)  # size of "newvalue"
  
  # Test expression checking
  var literal_token = animation_dsl.Token(animation_dsl.Token.NUMBER, "123", 1, 1)
  var identifier_token = animation_dsl.Token(animation_dsl.Token.IDENTIFIER, "test", 1, 1)
  var paren_token = animation_dsl.Token(animation_dsl.Token.LEFT_PAREN, "(", 1, 1)
  var keyword_token = animation_dsl.Token(animation_dsl.Token.KEYWORD, "color", 1, 1)
  
  assert(literal_token.can_start_expression())
  assert(identifier_token.can_start_expression())
  assert(paren_token.can_start_expression())
  assert(!keyword_token.can_start_expression())
  
  assert(literal_token.can_end_expression())
  assert(identifier_token.can_end_expression())
  assert(!paren_token.can_end_expression())
  
  print("✓ Token utilities test passed")
  return true
end

# Test Token string representations
def test_token_string_representations()
  print("Testing Token string representations...")
  
  var keyword_token = animation_dsl.Token(animation_dsl.Token.KEYWORD, "color", 1, 5)
  var eof_token = animation_dsl.Token(animation_dsl.Token.EOF, "", 10, 1)
  var error_token = animation_dsl.Token(animation_dsl.Token.ERROR, "Invalid character", 2, 8)
  var long_token = animation_dsl.Token(animation_dsl.Token.STRING, "This is a very long string that should be truncated", 3, 1)
  
  # Test tostring
  var keyword_str = keyword_token.tostring()
  assert(string.find(keyword_str, "KEYWORD") != -1)
  assert(string.find(keyword_str, "color") != -1)
  assert(string.find(keyword_str, "1:5") != -1)
  
  var eof_str = eof_token.tostring()
  assert(string.find(eof_str, "EOF") != -1)
  assert(string.find(eof_str, "10:1") != -1)
  
  var long_str = long_token.tostring()
  assert(string.find(long_str, "...") != -1)  # Should be truncated
  
  # Test to_error_string
  assert(keyword_token.to_error_string() == "keyword 'color'")
  assert(eof_token.to_error_string() == "end of file")
  assert(error_token.to_error_string() == "invalid token 'Invalid character'")
  
  print("✓ Token string representations test passed")
  return true
end

# Test utility functions
def test_utility_functions()
  print("Testing utility functions...")
  
  # Test create_eof_token
  var eof_token = animation_dsl.create_eof_token(5, 10)
  assert(eof_token.type == animation_dsl.Token.EOF)
  assert(eof_token.line == 5)
  assert(eof_token.column == 10)
  
  # Test create_error_token
  var error_token = animation_dsl.create_error_token("Test error", 3, 7)
  assert(error_token.type == animation_dsl.Token.ERROR)
  assert(error_token.value == "Test error")
  assert(error_token.line == 3)
  assert(error_token.column == 7)
  
  # Test create_newline_token
  var newline_token = animation_dsl.create_newline_token(2, 15)
  assert(newline_token.type == animation_dsl.Token.NEWLINE)
  assert(newline_token.value == "\n")
  assert(newline_token.line == 2)
  assert(newline_token.column == 15)
  
  # Test is_keyword
  assert(animation_dsl.is_keyword("color"))
  assert(animation_dsl.is_keyword("animation"))
  assert(animation_dsl.is_keyword("sequence"))
  assert(animation_dsl.is_keyword("true"))
  assert(animation_dsl.is_keyword("false"))
  assert(!animation_dsl.is_keyword("red"))
  assert(!animation_dsl.is_keyword("my_pattern"))
  assert(!animation_dsl.is_keyword("pattern"))  # "pattern" is not a DSL keyword
  
  # Test is_color_name
  assert(animation_dsl.is_color_name("red"))
  assert(animation_dsl.is_color_name("blue"))
  assert(animation_dsl.is_color_name("white"))
  assert(animation_dsl.is_color_name("transparent"))
  assert(!animation_dsl.is_color_name("color"))
  assert(!animation_dsl.is_color_name("my_color"))
  
  # Test operator precedence
  var plus_token = animation_dsl.Token(animation_dsl.Token.PLUS, "+", 1, 1)
  var multiply_token = animation_dsl.Token(animation_dsl.Token.MULTIPLY, "*", 1, 1)
  var power_token = animation_dsl.Token(animation_dsl.Token.POWER, "^", 1, 1)
  var and_token = animation_dsl.Token(animation_dsl.Token.LOGICAL_AND, "&&", 1, 1)
  
  assert(animation_dsl.get_operator_precedence(multiply_token) > animation_dsl.get_operator_precedence(plus_token))
  assert(animation_dsl.get_operator_precedence(power_token) > animation_dsl.get_operator_precedence(multiply_token))
  assert(animation_dsl.get_operator_precedence(plus_token) > animation_dsl.get_operator_precedence(and_token))
  
  # Test associativity
  assert(animation_dsl.is_right_associative(power_token))
  assert(!animation_dsl.is_right_associative(plus_token))
  assert(!animation_dsl.is_right_associative(multiply_token))
  
  print("✓ Utility functions test passed")
  return true
end

# Test edge cases and error conditions
def test_edge_cases()
  print("Testing edge cases...")
  
  # Test empty values
  var empty_token = animation_dsl.Token(animation_dsl.Token.STRING, "", 1, 1)
  assert(empty_token.value == "")
  assert(empty_token.length == 0)
  
  # Test very long values
  var long_value = ""
  for i : 0..99
    long_value += "x"
  end
  var long_token = animation_dsl.Token(animation_dsl.Token.STRING, long_value, 1, 1)
  assert(size(long_token.value) == 100)
  assert(long_token.length == 100)
  
  # Test invalid time formats (should not crash)
  var invalid_time = animation_dsl.Token(animation_dsl.Token.TIME, "invalid", 1, 1)
  assert(invalid_time.get_numeric_value() == nil)
  
  # Test invalid percentage formats
  var invalid_percent = animation_dsl.Token(animation_dsl.Token.PERCENTAGE, "invalid%", 1, 1)
  # Should not crash, but may return nil or 0
  
  # Test boundary values
  var zero_percent = animation_dsl.Token(animation_dsl.Token.PERCENTAGE, "0%", 1, 1)
  var max_percent = animation_dsl.Token(animation_dsl.Token.PERCENTAGE, "100%", 1, 1)
  assert(zero_percent.get_numeric_value() == 0)
  assert(max_percent.get_numeric_value() == 255)
  
  print("✓ Edge cases test passed")
  return true
end

# Run all tests
def run_token_tests()
  print("=== Token System Test Suite ===")
  
  var tests = [
    test_token_type_constants,
    test_token_basic,
    test_token_type_checking,
    test_token_value_extraction,
    test_token_utilities,
    test_token_string_representations,
    test_utility_functions,
    test_edge_cases
  ]
  
  var passed = 0
  var total = size(tests)
  
  for test_func : tests
    try
      if test_func()
        passed += 1
      else
        print("✗ Test failed")
      end
    except .. as error_type, error_message
      print(f"✗ Test crashed: {error_type} - {error_message}")
    end
  end
  
  print(f"\n=== Results: {passed}/{total} tests passed ===")
  
  if passed == total
    print("🎉 All token tests passed!")
    return true
  else
    print("❌ Some token tests failed")
    raise "test_failed"
  end
end

# Auto-run tests when file is executed
run_token_tests()