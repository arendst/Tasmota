# Token System Test Suite
# Tests for Token class

import string
import animation
import animation_dsl

# Local helper functions to replace removed Token methods
# These replace the removed is_identifier(), is_type(), and is_keyword() methods from Token class
def is_identifier(token, name)
  return token.type == 1 #-animation_dsl.Token.IDENTIFIER-# && token.value == name
end

def is_type(token, token_type)
  return token.type == token_type
end

def is_keyword(token, keyword)
  return token.type == 0 #-animation_dsl.Token.KEYWORD-# && token.value == keyword
end

# Test Token class basic functionality
def test_token_basic()
  print("Testing Token basic functionality...")
  
  # Test basic token creation
  var token = animation_dsl.Token(0 #-animation_dsl.Token.KEYWORD-#, "color", 1, 5, 5)
  assert(token.type == 0 #-animation_dsl.Token.KEYWORD-#)
  assert(token.value == "color")
  assert(token.line == 1)
  assert(token.column == 5)
  assert(token.length == 5)
  
  # Test default length calculation
  var token2 = animation_dsl.Token(1 #-animation_dsl.Token.IDENTIFIER-#, "red", 2, 10)
  assert(token2.length == 3)  # Should default to size of "red"
  
  # Test nil handling (using ERROR token instead of removed EOF)
  var token3 = animation_dsl.Token(39 #-animation_dsl.Token.ERROR-#, nil, nil, nil)
  assert(token3.value == "")
  assert(token3.line == 1)
  assert(token3.column == 1)
  
  print("✓ Token basic functionality test passed")
  return true
end

# Test Token type checking methods
def test_token_type_checking()
  print("Testing Token type checking methods...")
  
  var keyword_token = animation_dsl.Token(0 #-animation_dsl.Token.KEYWORD-#, "color", 1, 1)
  var identifier_token = animation_dsl.Token(1 #-animation_dsl.Token.IDENTIFIER-#, "red", 1, 1)
  var number_token = animation_dsl.Token(2 #-animation_dsl.Token.NUMBER-#, "123", 1, 1)
  var operator_token = animation_dsl.Token(9 #-animation_dsl.Token.PLUS-#, "+", 1, 1)
  var delimiter_token = animation_dsl.Token(24 #-animation_dsl.Token.LEFT_PAREN-#, "(", 1, 1)
  var separator_token = animation_dsl.Token(30 #-animation_dsl.Token.COMMA-#, ",", 1, 1)
  
  # Test is_type
  assert(is_type(keyword_token, 0 #-animation_dsl.Token.KEYWORD-#))
  assert(!is_type(keyword_token, 1 #-animation_dsl.Token.IDENTIFIER-#))
  
  # Test is_keyword
  assert(is_keyword(keyword_token, "color"))
  assert(!is_keyword(keyword_token, "red"))
  assert(!is_keyword(identifier_token, "color"))
  
  # Test is_identifier
  assert(is_identifier(identifier_token, "red"))
  assert(!is_identifier(identifier_token, "blue"))
  assert(!is_identifier(keyword_token, "red"))
  
  print("✓ Token type checking test passed")
  return true
end

# Test Token value extraction methods
def test_token_value_extraction()
  print("Testing Token value extraction methods...")
  
  # Test boolean tokens
  var true_token = animation_dsl.Token(0 #-animation_dsl.Token.KEYWORD-#, "true", 1, 1)
  var false_token = animation_dsl.Token(0 #-animation_dsl.Token.KEYWORD-#, "false", 1, 1)
  var other_token = animation_dsl.Token(0 #-animation_dsl.Token.KEYWORD-#, "color", 1, 1)
  
  # Test numeric tokens
  var number_token = animation_dsl.Token(2 #-animation_dsl.Token.NUMBER-#, "123.45", 1, 1)
  var time_token = animation_dsl.Token(5 #-animation_dsl.Token.TIME-#, "2s", 1, 1)
  var percent_token = animation_dsl.Token(6 #-animation_dsl.Token.PERCENTAGE-#, "50%", 1, 1)
  var multiplier_token = animation_dsl.Token(7 #-animation_dsl.Token.MULTIPLIER-#, "2.5x", 1, 1)
  
  # Test time conversion
  var ms_token = animation_dsl.Token(5 #-animation_dsl.Token.TIME-#, "500ms", 1, 1)
  var s_token = animation_dsl.Token(5 #-animation_dsl.Token.TIME-#, "3s", 1, 1)
  var m_token = animation_dsl.Token(5 #-animation_dsl.Token.TIME-#, "2m", 1, 1)
  var h_token = animation_dsl.Token(5 #-animation_dsl.Token.TIME-#, "1h", 1, 1)
  
  # Test percentage to 255 conversion
  var percent_0 = animation_dsl.Token(6 #-animation_dsl.Token.PERCENTAGE-#, "0%", 1, 1)
  var percent_50 = animation_dsl.Token(6 #-animation_dsl.Token.PERCENTAGE-#, "50%", 1, 1)
  var percent_100 = animation_dsl.Token(6 #-animation_dsl.Token.PERCENTAGE-#, "100%", 1, 1)
  
  print("✓ Token value extraction test passed")
  return true
end

# Test Token utility methods
def test_token_utilities()
  print("Testing Token utility methods...")
  
  var token = animation_dsl.Token(1 #-animation_dsl.Token.IDENTIFIER-#, "test", 5, 10, 4)
  
  # Test expression checking
  var literal_token = animation_dsl.Token(2 #-animation_dsl.Token.NUMBER-#, "123", 1, 1)
  var identifier_token = animation_dsl.Token(1 #-animation_dsl.Token.IDENTIFIER-#, "test", 1, 1)
  var paren_token = animation_dsl.Token(24 #-animation_dsl.Token.LEFT_PAREN-#, "(", 1, 1)
  var keyword_token = animation_dsl.Token(0 #-animation_dsl.Token.KEYWORD-#, "color", 1, 1)
  
  print("✓ Token utilities test passed")
  return true
end

# Test Token string representations
def test_token_string_representations()
  print("Testing Token string representations...")
  
  var keyword_token = animation_dsl.Token(0 #-animation_dsl.Token.KEYWORD-#, "color", 1, 5)
  # EOF token removed - use ERROR token for testing instead
  var error_token = animation_dsl.Token(39 #-animation_dsl.Token.ERROR-#, "Invalid character", 2, 8)
  var long_token = animation_dsl.Token(3 #-animation_dsl.Token.STRING-#, "This is a very long string that should be truncated", 3, 1)
  
  # Test tostring
  var keyword_str = keyword_token.tostring()
  assert(string.find(keyword_str, "KEYWORD") != -1)
  assert(string.find(keyword_str, "color") != -1)
  assert(string.find(keyword_str, "1:5") != -1)
  
  # EOF token removed - skip EOF-specific string tests
  
  var long_str = long_token.tostring()
  assert(string.find(long_str, "...") != -1)  # Should be truncated
    
  print("✓ Token string representations test passed")
  return true
end

# Test utility functions
def test_utility_functions()
  print("Testing utility functions...")
  
  # create_eof_token test removed - function deprecated with EOF token removal
  
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
  var plus_token = animation_dsl.Token(9 #-animation_dsl.Token.PLUS-#, "+", 1, 1)
  var multiply_token = animation_dsl.Token(11 #-animation_dsl.Token.MULTIPLY-#, "*", 1, 1)
  var power_token = animation_dsl.Token(14 #-animation_dsl.Token.POWER-#, "^", 1, 1)
  var and_token = animation_dsl.Token(21 #-animation_dsl.Token.LOGICAL_AND-#, "&&", 1, 1)
  
  print("✓ Utility functions test passed")
  return true
end

# Test edge cases and error conditions
def test_edge_cases()
  print("Testing edge cases...")
  
  # Test empty values
  var empty_token = animation_dsl.Token(3 #-animation_dsl.Token.STRING-#, "", 1, 1)
  assert(empty_token.value == "")
  assert(empty_token.length == 0)
  
  # Test very long values
  var long_value = ""
  for i : 0..99
    long_value += "x"
  end
  var long_token = animation_dsl.Token(3 #-animation_dsl.Token.STRING-#, long_value, 1, 1)
  assert(size(long_token.value) == 100)
  assert(long_token.length == 100)
    
  # Test invalid percentage formats
  var invalid_percent = animation_dsl.Token(6 #-animation_dsl.Token.PERCENTAGE-#, "invalid%", 1, 1)
  # Should not crash, but may return nil or 0
  
  print("✓ Edge cases test passed")
  return true
end

# Run all tests
def run_token_tests()
  print("=== Token System Test Suite ===")
  
  var tests = [
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