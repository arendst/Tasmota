# DSL Lexer Triple Quotes Test Suite
# Tests for triple-quoted string tokenization in create_lexer
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation/src -e "import tasmota" lib/libesp32/berry_animation/src/tests/dsl_lexer_triple_quotes_test.be

import animation_dsl
import string

# Helper function to extract all tokens from a pull lexer (for testing only)
def extract_all_tokens(lexer)
  var tokens = []
  lexer.reset()  # Start from beginning
  
  while !lexer.at_end()
    var token = lexer.next_token()
    
    # EOF token removed - check for nil instead
    if token == nil
      break
    end
    
    tokens.push(token)
  end
  
  return tokens
end

# Test basic triple-quoted string tokenization with double quotes
def test_triple_quotes_double()
  print("Testing triple-quoted string tokenization (triple quotes)...")
  
  var source = 'berry """\nHello World\n"""'
  var lexer = animation_dsl.create_lexer(source)
  var tokens = extract_all_tokens(lexer)
  
  assert(size(tokens) >= 2, "Should have at least 2 tokens (berry, string)")
  assert(tokens[0].type == 0 #-animation_dsl.Token.KEYWORD-#, "First token should be KEYWORD")
  assert(tokens[0].value == "berry", "First token should be 'berry'")
  assert(tokens[1].type == 3 #-animation_dsl.Token.STRING-#, "Second token should be STRING")
  assert(tokens[1].value == "\nHello World\n", "String content should be preserved")
  
  print("✓ Triple-quoted string (double quotes) tokenization test passed")
  return true
end

# Test basic triple-quoted string tokenization with single quotes
def test_triple_quotes_single()
  print("Testing triple-quoted string tokenization (single quotes)...")
  
  var source = "berry '''\nHello World\n'''"
  var lexer = animation_dsl.create_lexer(source)
  var tokens = extract_all_tokens(lexer)
  
  assert(size(tokens) >= 2, "Should have at least 2 tokens (berry, string)")
  assert(tokens[0].type == 0 #-animation_dsl.Token.KEYWORD-#, "First token should be KEYWORD")
  assert(tokens[0].value == "berry", "First token should be 'berry'")
  assert(tokens[1].type == 3 #-animation_dsl.Token.STRING-#, "Second token should be STRING")
  assert(tokens[1].value == "\nHello World\n", "String content should be preserved")
  
  print("✓ Triple-quoted string (single quotes) tokenization test passed")
  return true
end

# Test multiline triple-quoted string
def test_multiline_triple_quotes()
  print("Testing multiline triple-quoted string tokenization...")
  
  var source = 'berry """\nLine 1\nLine 2\nLine 3\n"""'
  var lexer = animation_dsl.create_lexer(source)
  var tokens = extract_all_tokens(lexer)
  
  assert(size(tokens) >= 2, "Should have at least 2 tokens")
  assert(tokens[1].type == 3 #-animation_dsl.Token.STRING-#, "Second token should be STRING")
  
  var expected_content = "\nLine 1\nLine 2\nLine 3\n"
  assert(tokens[1].value == expected_content, f"String content should be '{expected_content}', got '{tokens[1].value}'")
  
  print("✓ Multiline triple-quoted string tokenization test passed")
  return true
end

# Test triple-quoted string with embedded quotes
def test_embedded_quotes()
  print("Testing triple-quoted string with embedded quotes...")
  
  var source = 'berry """\nprint("Hello")\nvar x = \'world\'\n"""'
  var lexer = animation_dsl.create_lexer(source)
  var tokens = extract_all_tokens(lexer)
  
  assert(size(tokens) >= 2, "Should have at least 2 tokens")
  assert(tokens[1].type == 3 #-animation_dsl.Token.STRING-#, "Second token should be STRING")
  
  var expected_content = '\nprint("Hello")\nvar x = \'world\'\n'
  assert(tokens[1].value == expected_content, f"String content should preserve embedded quotes")
  
  print("✓ Embedded quotes in triple-quoted string test passed")
  return true
end

# Test empty triple-quoted string
def test_empty_triple_quotes()
  print("Testing empty triple-quoted string...")
  
  var source = 'berry """"""'
  var lexer = animation_dsl.create_lexer(source)
  var tokens = extract_all_tokens(lexer)
  
  assert(size(tokens) >= 2, "Should have at least 2 tokens")
  assert(tokens[1].type == 3 #-animation_dsl.Token.STRING-#, "Second token should be STRING")
  assert(tokens[1].value == "", "Empty string should have empty value")
  
  print("✓ Empty triple-quoted string test passed")
  return true
end

# Test unterminated triple-quoted string (error case)
def test_unterminated_triple_quotes()
  print("Testing unterminated triple-quoted string...")
  
  var source = 'berry """\nUnterminated string'
  
  # Should raise an exception when trying to extract tokens (pull-mode lexer)
  try
    var lexer = animation_dsl.create_lexer(source)
    var tokens = extract_all_tokens(lexer)  # This should trigger the error
    assert(false, "Should raise exception for unterminated triple-quoted string")
  except "lexical_error" as e, msg
    # Expected - unterminated string should raise lexical_error
    assert(size(msg) > 0, "Should have error message")
    assert(string.find(msg, "Unterminated") >= 0, "Error message should mention unterminated string")
  end
  
  print("✓ Unterminated triple-quoted string test passed")
  return true
end

# Test triple-quoted string with complex content
def test_complex_content()
  print("Testing triple-quoted string with complex content...")
  
  var source = 'berry """\n' +
    'import math\n' +
    'def func(x)\n' +
    '  return x * 2\n' +
    'end\n' +
    'var result = func(21)\n' +
    'print("Result:", result)\n' +
    '"""'
  
  var lexer = animation_dsl.create_lexer(source)
  var tokens = extract_all_tokens(lexer)
  
  assert(size(tokens) >= 2, "Should have at least 2 tokens")
  assert(tokens[1].type == 3 #-animation_dsl.Token.STRING-#, "Second token should be STRING")
  
  var content = tokens[1].value
  assert(string.find(content, "import math") >= 0, "Should contain import statement")
  assert(string.find(content, "def func(x)") >= 0, "Should contain function definition")
  assert(string.find(content, "return x * 2") >= 0, "Should contain function body")
  assert(string.find(content, 'print("Result:", result)') >= 0, "Should contain print statement")
  
  print("✓ Complex content triple-quoted string test passed")
  return true
end

# Test mixed quote types (should not interfere)
def test_mixed_quote_types()
  print("Testing that triple quotes don't interfere with regular quotes...")
  
  var source = 'color red = 0xFF0000\nberry """\ntest\n"""\nvar x = "normal string"'
  var lexer = animation_dsl.create_lexer(source)
  var tokens = extract_all_tokens(lexer)
  
  # Find the normal string token
  var normal_string_found = false
  for token : tokens
    if token.type == 3 #-animation_dsl.Token.STRING-# && token.value == "normal string"
      normal_string_found = true
      break
    end
  end
  
  assert(normal_string_found, "Should still tokenize normal strings correctly")
  
  print("✓ Mixed quote types test passed")
  return true
end

# Run all lexer triple quotes tests
def run_all_lexer_triple_quotes_tests()
  print("=== DSL Lexer Triple Quotes Test Suite ===")
  print("")
  
  var tests = [
    test_triple_quotes_double,
    test_triple_quotes_single,
    test_multiline_triple_quotes,
    test_embedded_quotes,
    test_empty_triple_quotes,
    test_unterminated_triple_quotes,
    test_complex_content,
    test_mixed_quote_types
  ]
  
  var passed = 0
  var total = size(tests)
  
  for test_func : tests
    try
      if test_func()
        passed += 1
      end
    except .. as e, m
      print("✗ Test failed with exception:", e, m)
    end
    print("")
  end
  
  print("=== Lexer Triple Quotes Test Results ===")
  print(f"Passed: {passed}/{total}")
  
  if passed == total
    print("All lexer triple quotes tests passed! ✓")
    return true
  else
    print("Some lexer triple quotes tests failed! ✗")
    raise "test_failed"
  end
end

# Run the tests
return run_all_lexer_triple_quotes_tests()