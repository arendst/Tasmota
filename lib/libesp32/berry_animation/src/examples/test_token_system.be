# Test Token System
# Quick test to verify the token system is working correctly

import "animation" as animation

print("Testing Animation DSL Token System...")

# Test 1: Basic token creation
print("\n1. Testing basic token creation...")
try
  var token = animation.Token(animation.Token.KEYWORD, "color", 1, 5)
  print(f"✓ Created token: {token}")
except .. as error_type, error_message
  print(f"✗ Failed to create token: {error_type} - {error_message}")
  return false
end

# Test 2: Token type constants
print("\n2. Testing token type constants...")
try
  assert(animation.Token.KEYWORD == 0)
  assert(animation.Token.IDENTIFIER == 1)
  assert(animation.Token.EOF == 38)
  print("✓ Token type constants are correct")
except .. as error_type, error_message
  print(f"✗ Token type constants failed: {error_type} - {error_message}")
  return false
end

# Test 3: Token methods
print("\n3. Testing token methods...")
try
  var token = animation.Token(animation.Token.TIME, "2s", 1, 1)
  assert(token.is_type(animation.Token.TIME))
  assert(token.is_numeric())
  assert(token.get_numeric_value() == 2000)
  print("✓ Token methods work correctly")
except .. as error_type, error_message
  print(f"✗ Token methods failed: {error_type} - {error_message}")
  return false
end

# Test 4: Utility functions
print("\n4. Testing utility functions...")
try
  assert(animation.is_keyword("color"))
  assert(!animation.is_keyword("my_color"))
  assert(animation.is_color_name("red"))
  assert(!animation.is_color_name("my_red"))
  print("✓ Utility functions work correctly")
except .. as error_type, error_message
  print(f"✗ Utility functions failed: {error_type} - {error_message}")
  return false
end

# Test 5: Token creation utilities
print("\n5. Testing token creation utilities...")
try
  var eof_token = animation.create_eof_token(10, 1)
  var error_token = animation.create_error_token("Test error", 5, 3)
  var newline_token = animation.create_newline_token(2, 15)
  
  assert(eof_token.type == animation.Token.EOF)
  assert(error_token.type == animation.Token.ERROR)
  assert(newline_token.type == animation.Token.NEWLINE)
  print("✓ Token creation utilities work correctly")
except .. as error_type, error_message
  print(f"✗ Token creation utilities failed: {error_type} - {error_message}")
  return false
end

# Test 6: Operator precedence
print("\n6. Testing operator precedence...")
try
  var plus_token = animation.Token(animation.Token.PLUS, "+", 1, 1)
  var mult_token = animation.Token(animation.Token.MULTIPLY, "*", 1, 1)
  
  var plus_prec = animation.get_operator_precedence(plus_token)
  var mult_prec = animation.get_operator_precedence(mult_token)
  
  assert(mult_prec > plus_prec)
  print("✓ Operator precedence works correctly")
except .. as error_type, error_message
  print(f"✗ Operator precedence failed: {error_type} - {error_message}")
  return false
end

print("\n🎉 All token system tests passed!")
print("The DSL token system is ready for use.")

# Using built-in assert function

return true