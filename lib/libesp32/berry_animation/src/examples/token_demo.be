# Token System Demo
# Demonstrates the DSL token system functionality

import "animation" as animation

print("=== Animation DSL Token System Demo ===")

# Demo 1: Basic token creation and inspection
print("\n--- Demo 1: Basic Token Creation ---")

var keyword_token = animation.Token(animation.Token.KEYWORD, "color", 1, 5)
var identifier_token = animation.Token(animation.Token.IDENTIFIER, "red", 1, 11)
var color_token = animation.Token(animation.Token.COLOR, "#FF0000", 1, 15)
var time_token = animation.Token(animation.Token.TIME, "2s", 2, 1)
var number_token = animation.Token(animation.Token.NUMBER, "123.45", 2, 5)

print(f"Keyword token: {keyword_token}")
print(f"Identifier token: {identifier_token}")
print(f"Color token: {color_token}")
print(f"Time token: {time_token}")
print(f"Number token: {number_token}")

# Demo 2: Token type checking
print("\n--- Demo 2: Token Type Checking ---")

print(f"Is '{keyword_token.value}' a keyword? {keyword_token.is_type(animation.Token.KEYWORD)}")
print(f"Is '{keyword_token.value}' the 'color' keyword? {keyword_token.is_keyword('color')}")
print(f"Is '{identifier_token.value}' an identifier? {identifier_token.is_type(animation.Token.IDENTIFIER)}")
print(f"Is '{identifier_token.value}' the 'red' identifier? {identifier_token.is_identifier('red')}")

print(f"Is '{color_token.value}' a literal? {color_token.is_literal()}")
print(f"Is '{time_token.value}' numeric? {time_token.is_numeric()}")

# Demo 3: Value extraction
print("\n--- Demo 3: Value Extraction ---")

var true_token = animation.Token(animation.Token.KEYWORD, "true", 3, 1)
var false_token = animation.Token(animation.Token.KEYWORD, "false", 3, 6)
var percent_token = animation.Token(animation.Token.PERCENTAGE, "75%", 3, 12)
var multiplier_token = animation.Token(animation.Token.MULTIPLIER, "2.5x", 3, 17)

print(f"Boolean value of 'true': {true_token.get_boolean_value()}")
print(f"Boolean value of 'false': {false_token.get_boolean_value()}")
print(f"Numeric value of '2s': {time_token.get_numeric_value()}ms")
print(f"Time in ms of '2s': {time_token.get_numeric_value()}ms")
print(f"Percentage as 0-255: {percent_token.get_numeric_value()}")
print(f"Multiplier value: {multiplier_token.get_numeric_value()}")

# Demo 4: Different time units
print("\n--- Demo 4: Time Unit Conversion ---")

var time_tokens = [
  animation.Token(animation.Token.TIME, "500ms", 4, 1),
  animation.Token(animation.Token.TIME, "3s", 4, 7),
  animation.Token(animation.Token.TIME, "2m", 4, 11),
  animation.Token(animation.Token.TIME, "1h", 4, 15)
]

for token : time_tokens
  print(f"Time '{token.value}' = {token.get_numeric_value()}ms")
end

# Demo 5: Operator precedence
print("\n--- Demo 5: Operator Precedence ---")

var operators = [
  animation.Token(animation.Token.LOGICAL_OR, "||", 5, 1),
  animation.Token(animation.Token.LOGICAL_AND, "&&", 5, 4),
  animation.Token(animation.Token.EQUAL, "==", 5, 7),
  animation.Token(animation.Token.PLUS, "+", 5, 10),
  animation.Token(animation.Token.MULTIPLY, "*", 5, 12),
  animation.Token(animation.Token.POWER, "^", 5, 14)
]

print("Operator precedence (higher number = higher precedence):")
for op : operators
  var precedence = animation.get_operator_precedence(op)
  var associativity = animation.is_right_associative(op) ? "right" : "left"
  print(f"  {op.value} : precedence={precedence}, associativity={associativity}")
end

# Demo 6: Keyword and color name checking
print("\n--- Demo 6: Keyword and Color Name Checking ---")

var test_words = ["color", "pattern", "red", "blue", "my_variable", "gradient", "transparent"]

for word : test_words
  var is_kw = animation.is_keyword(word)
  var is_color = animation.is_color_name(word)
  var type_str = ""
  if is_kw && is_color
    type_str = "keyword + color"
  elif is_kw
    type_str = "keyword"
  elif is_color
    type_str = "color"
  else
    type_str = "identifier"
  end
  print(f"  '{word}' -> {type_str}")
end

# Demo 7: Token utilities
print("\n--- Demo 7: Token Utilities ---")

var original_token = animation.Token(animation.Token.IDENTIFIER, "test", 6, 10, 4)
print(f"Original token: {original_token}")
print(f"End column: {original_token.end_column()}")

var new_type_token = original_token.with_type(animation.Token.KEYWORD)
print(f"With new type: {new_type_token}")

var new_value_token = original_token.with_value("newname")
print(f"With new value: {new_value_token}")

# Demo 8: Expression checking
print("\n--- Demo 8: Expression Checking ---")

var expression_tokens = [
  animation.Token(animation.Token.NUMBER, "123", 7, 1),
  animation.Token(animation.Token.IDENTIFIER, "red", 7, 5),
  animation.Token(animation.Token.LEFT_PAREN, "(", 7, 9),
  animation.Token(animation.Token.RIGHT_PAREN, ")", 7, 10),
  animation.Token(animation.Token.KEYWORD, "color", 7, 12),
  animation.Token(animation.Token.PLUS, "+", 7, 18)
]

print("Expression analysis:")
for token : expression_tokens
  var can_start = token.can_start_expression()
  var can_end = token.can_end_expression()
  print(f"  {token.to_error_string()}: can_start={can_start}, can_end={can_end}")
end

# Demo 9: Error handling and edge cases
print("\n--- Demo 9: Error Handling ---")

var eof_token = animation.create_eof_token(10, 1)
var error_token = animation.create_error_token("Unexpected character '&'", 8, 15)
var newline_token = animation.create_newline_token(9, 20)

print(f"EOF token: {eof_token}")
print(f"Error token: {error_token}")
print(f"Newline token: {newline_token}")

print(f"EOF error string: {eof_token.to_error_string()}")
print(f"Error error string: {error_token.to_error_string()}")

# Demo 10: Token type string conversion
print("\n--- Demo 10: Token Type Names ---")

var sample_types = [
  animation.Token.KEYWORD,
  animation.Token.IDENTIFIER,
  animation.Token.COLOR,
  animation.Token.TIME,
  animation.Token.LOGICAL_AND,
  animation.Token.LEFT_PAREN,
  animation.Token.EOF
]

print("Token type names:")
for token_type : sample_types
  print(f"  {token_type} -> {animation.Token.to_string(token_type)}")
end

print("\n=== Token System Demo Complete ===")
print("The token system provides comprehensive support for:")
print("- All DSL token types with proper classification")
print("- Line and column tracking for error reporting")
print("- Value extraction with type conversion")
print("- Operator precedence and associativity")
print("- Keyword and color name recognition")
print("- Expression boundary detection")
print("- Comprehensive string representations")

return true