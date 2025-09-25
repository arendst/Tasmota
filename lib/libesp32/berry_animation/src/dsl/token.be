# Token Types and Token Class for Animation DSL
# Defines all token types and the Token class with line/column tracking

#@ solidify:Token,weak
class Token
  # Basic token types
  static var KEYWORD = 0        # strip, color, animation, sequence, etc.
  static var IDENTIFIER = 1     # user-defined names
  static var NUMBER = 2         # 123, 3.14
  static var STRING = 3         # "hello", 'world'
  static var COLOR = 4          # #FF0000, rgb(255,0,0), hsv(240,100,100)
  static var TIME = 5           # 2s, 500ms, 1m, 2h
  static var PERCENTAGE = 6     # 50%, 100%
  static var MULTIPLIER = 7     # 2x, 0.5x
  
  # Static arrays for better solidification (moved from inline arrays)
  static var names = [
    "KEYWORD", "IDENTIFIER", "NUMBER", "STRING", "COLOR", "TIME", "PERCENTAGE", "MULTIPLIER",
    "ASSIGN", "PLUS", "MINUS", "MULTIPLY", "DIVIDE", "MODULO", "POWER",
    "EQUAL", "NOT_EQUAL", "LESS_THAN", "LESS_EQUAL", "GREATER_THAN", "GREATER_EQUAL",
    "LOGICAL_AND", "LOGICAL_OR", "LOGICAL_NOT",
    "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE", "LEFT_BRACKET", "RIGHT_BRACKET",
    "COMMA", "SEMICOLON", "COLON", "DOT", "ARROW",
    "NEWLINE", "VARIABLE_REF", "COMMENT", "EOF", "ERROR",
    "EVENT_ON", "EVENT_INTERRUPT", "EVENT_RESUME", "EVENT_AFTER"
  ]
  
  static var statement_keywords = [
    "strip", "set", "color", "palette", "animation", 
    "sequence", "function", "zone", "on", "run", "template", "param", "import", "berry"
  ]
  
  static var keywords = [
    # Configuration keywords
    "strip", "set", "import", "berry",
    
    # Definition keywords
    "color", "palette", "animation", "sequence", "function", "zone", "template", "param", "type",
    
    # Control flow keywords
    "play", "for", "with", "repeat", "times", "forever", "if", "else", "elif",
    "choose", "random", "on", "run", "wait", "goto", "interrupt", "resume",
    "while", "from", "to", "return", "reset", "restart",
    
    # Modifier keywords (only actual DSL syntax keywords)
    "at", "ease", "sync", "every", "stagger", "across", "pixels",
    
    # Core built-in functions (minimal set for essential DSL operations)
    "rgb", "hsv",
    
    # Spatial keywords
    "all", "even", "odd", "center", "edges", "left", "right", "top", "bottom",
    
    # Boolean and special values
    "true", "false", "nil", "transparent",
    
    # Event keywords
    "startup", "shutdown", "button_press", "button_hold", "motion_detected",
    "brightness_change", "timer", "time", "sound_peak", "network_message",
    
    # Time and measurement keywords
    "ms", "s", "m", "h", "bpm"
  ]
  
  static var color_names = [
    "red", "green", "blue", "white", "black", "yellow", "orange", "purple",
    "pink", "cyan", "magenta", "gray", "grey", "silver", "gold", "brown",
    "lime", "navy", "olive", "maroon", "teal", "aqua", "fuchsia", "indigo",
    "violet", "crimson", "coral", "salmon", "khaki", "plum", "orchid",
    "turquoise", "tan", "beige", "ivory", "snow", "transparent"
  ]
  
  # Operators
  static var ASSIGN = 8         # =
  static var PLUS = 9           # +
  static var MINUS = 10         # -
  static var MULTIPLY = 11      # *
  static var DIVIDE = 12        # /
  static var MODULO = 13        # %
  static var POWER = 14         # ^
  
  # Comparison operators
  static var EQUAL = 15         # ==
  static var NOT_EQUAL = 16     # !=
  static var LESS_THAN = 17     # <
  static var LESS_EQUAL = 18    # <=
  static var GREATER_THAN = 19  # >
  static var GREATER_EQUAL = 20 # >=
  
  # Logical operators
  static var LOGICAL_AND = 21   # &&
  static var LOGICAL_OR = 22    # ||
  static var LOGICAL_NOT = 23   # !
  
  # Delimiters
  static var LEFT_PAREN = 24    # (
  static var RIGHT_PAREN = 25   # )
  static var LEFT_BRACE = 26    # {
  static var RIGHT_BRACE = 27   # }
  static var LEFT_BRACKET = 28  # [
  static var RIGHT_BRACKET = 29 # ]
  
  # Separators
  static var COMMA = 30         # ,
  static var SEMICOLON = 31     # ;
  static var COLON = 32         # :
  static var DOT = 33           # .
  static var ARROW = 34         # ->
  
  # Special tokens
  static var NEWLINE = 35       # \n (significant in some contexts)
  static var VARIABLE_REF = 36  # $identifier
  static var COMMENT = 37       # # comment text
  static var EOF = 38           # End of file
  static var ERROR = 39         # Error token for invalid input
  
  # Event-related tokens
  static var EVENT_ON = 40      # on (event handler keyword)
  static var EVENT_INTERRUPT = 41  # interrupt
  static var EVENT_RESUME = 42  # resume
  static var EVENT_AFTER = 43   # after (for resume timing)
  
  # Convert token type to string for debugging
  static def to_string(token_type)
    if token_type >= 0 && token_type < size(_class.names)
      return _class.names[token_type]
    end
    return "UNKNOWN"
  end
  
  var type        # int - the type of this token (Token.KEYWORD, Token.IDENTIFIER, etc.)
  var value       # String - the actual text value of the token
  var line        # Integer - line number where token appears (1-based)
  var column      # Integer - column number where token starts (1-based)
  var length      # Integer - length of the token in characters
  
  # Initialize a new token
  #
  # @param type: int - Token type constant (Token.KEYWORD, Token.IDENTIFIER, etc.)
  # @param value: string - The actual text value
  # @param line: int - Line number (1-based)
  # @param column: int - Column number (1-based)
  # @param length: int - Length of token in characters (optional, defaults to value length)
  def init(typ, value, line, column, length)
    self.type = typ
    self.value = value != nil ? value : ""
    self.line = line != nil ? line : 1
    self.column = column != nil ? column : 1
    self.length = length != nil ? length : size(self.value)
  end
  
  # Check if this token is of a specific type
  #
  # @param token_type: int - Token type to check against
  # @return bool - True if token matches the type
  def is_type(token_type)
    return self.type == token_type
  end
  
  # Check if this token is a keyword with specific value
  #
  # @param keyword: string - Keyword to check for
  # @return bool - True if token is the specified keyword
  def is_keyword(keyword)
    return self.type == 0 #-self.KEYWORD-# && self.value == keyword
  end
  
  # Check if this token is an identifier with specific value
  #
  # @param name: string - Identifier name to check for
  # @return bool - True if token is the specified identifier
  def is_identifier(name)
    return self.type == 1 #-self.IDENTIFIER-# && self.value == name
  end
  
  # Check if this token is an operator
  #
  # @return bool - True if token is any operator type
  def is_operator()
    return self.type >= 8 #-self.ASSIGN-# && self.type <= 23 #-self.LOGICAL_NOT-#
  end
  
  # Check if this token is a delimiter
  #
  # @return bool - True if token is any delimiter type
  def is_delimiter()
    return self.type >= 24 #-self.LEFT_PAREN-# && self.type <= 29 #-self.RIGHT_BRACKET-#
  end
  
  # Check if this token is a separator
  #
  # @return bool - True if token is any separator type
  def is_separator()
    return self.type >= 30 #-self.COMMA-# && self.type <= 34 #-self.ARROW-#
  end
  
  # Check if this token is a literal value
  #
  # @return bool - True if token represents a literal value
  def is_literal()
    return self.type == 2 #-self.NUMBER-# || 
           self.type == 3 #-self.STRING-# || 
           self.type == 4 #-self.COLOR-# ||
           self.type == 5 #-self.TIME-# ||
           self.type == 6 #-self.PERCENTAGE-# ||
           self.type == 7 #-self.MULTIPLIER-#
  end
  
  # Get the end column of this token
  #
  # @return int - Column number where token ends
  def end_column()
    return self.column + self.length - 1
  end
  
  # Create a copy of this token with a different type
  #
  # @param new_type: int - New token type
  # @return Token - New token with same position but different type
  def with_type(new_type)
    import animation_dsl
    return animation_dsl.Token(new_type, self.value, self.line, self.column, self.length)
  end
  
  # Create a copy of this token with a different value
  #
  # @param new_value: string - New value
  # @return Token - New token with same position but different value
  def with_value(new_value)
    import animation_dsl
    return animation_dsl.Token(self.type, new_value, self.line, self.column, size(new_value))
  end
  
  # Get a string representation of the token for debugging
  #
  # @return string - Human-readable token description
  def tostring()
    var type_name = self.to_string(self.type)
    if self.type == 38 #-self.EOF-#
      return f"Token({type_name} at {self.line}:{self.column})"
    elif self.type == 35 #-self.NEWLINE-#
      return f"Token({type_name} at {self.line}:{self.column})"
    elif size(self.value) > 20
      var short_value = self.value[0..17] + "..."
      return f"Token({type_name}, '{short_value}' at {self.line}:{self.column})"
    else
      return f"Token({type_name}, '{self.value}' at {self.line}:{self.column})"
    end
  end
  
  # Get a compact string representation for error messages
  #
  # @return string - Compact token description
  def to_error_string()
    if self.type == 38 #-self.EOF-#
      return "end of file"
    elif self.type == 35 #-self.NEWLINE-#
      return "newline"
    elif self.type == 0 #-self.KEYWORD-#
      return f"keyword '{self.value}'"
    elif self.type == 1 #-self.IDENTIFIER-#
      return f"identifier '{self.value}'"
    elif self.type == 3 #-self.STRING-#
      return f"string '{self.value}'"
    elif self.type == 2 #-self.NUMBER-#
      return f"number '{self.value}'"
    elif self.type == 4 #-self.COLOR-#
      return f"color '{self.value}'"
    elif self.type == 5 #-self.TIME-#
      return f"time '{self.value}'"
    elif self.type == 6 #-self.PERCENTAGE-#
      return f"percentage '{self.value}'"
    elif self.type == 39 #-self.ERROR-#
      return f"invalid token '{self.value}'"
    else
      return f"'{self.value}'"
    end
  end
  
  # Check if this token represents a boolean value
  #
  # @return bool - True if token is "true" or "false" keyword
  def is_boolean()
    return self.type == 0 #-self.KEYWORD-# && (self.value == "true" || self.value == "false")
  end
  
  # Get boolean value if this token represents one
  #
  # @return bool - Boolean value, or nil if not a boolean token
  def get_boolean_value()
    if self.is_boolean()
      return self.value == "true"
    end
    return nil
  end
  
  # Check if this token represents a numeric value
  #
  # @return bool - True if token can be converted to a number
  def is_numeric()
    return self.type == 2 #-self.NUMBER-# || 
           self.type == 5 #-self.TIME-# ||
           self.type == 6 #-self.PERCENTAGE-# ||
           self.type == 7 #-self.MULTIPLIER-#
  end
  
  # Get numeric value from token (without units) - returns only integers
  #
  # @return int - Numeric value, or nil if not numeric
  # - time is in ms
  # - percentage is converted to 100% = 255
  # - times is converted to x256 (2x = 512)
  def get_numeric_value()
    import string
    import math

    if self.type == 2 #-self.NUMBER-#
      return math.round(real(self.value))
    elif self.type == 5 #-self.TIME-#
      # Remove time unit suffix and convert to milliseconds
      var value_str = self.value
      if string.endswith(value_str, "ms")
        return math.round(real(value_str[0..-3]))
      elif string.endswith(value_str, "s")
        return math.round(real(value_str[0..-2]) * 1000)
      elif string.endswith(value_str, "m")
        return math.round(real(value_str[0..-2]) * 60000)
      elif string.endswith(value_str, "h")
        return math.round(real(value_str[0..-2]) * 3600000)
      end
    elif self.type == 6 #-self.PERCENTAGE-#
      # Remove % and convert to 0-255 range (100% = 255)
      var percent = math.round(real(self.value[0..-2]))
      return tasmota.scale_uint(percent, 0, 100, 0, 255)
    elif self.type == 7 #-self.MULTIPLIER-#
      # Remove x suffix and convert to x256 scale (2x = 512)
      var multiplier = real(self.value[0..-2])
      return math.round(multiplier * 256)
    end
    return nil
  end
  

  
  # Check if this token can start an expression
  #
  # @return bool - True if token can begin an expression
  def can_start_expression()
    return self.is_literal() ||
           self.type == 1 #-self.IDENTIFIER-# ||
           self.type == 36 #-self.VARIABLE_REF-# ||
           self.type == 24 #-self.LEFT_PAREN-# ||
           self.type == 23 #-self.LOGICAL_NOT-# ||
           self.type == 10 #-self.MINUS-# ||
           self.type == 9 #-self.PLUS-#
  end
  
  # Check if this token can end an expression
  #
  # @return bool - True if token can end an expression
  def can_end_expression()
    return self.is_literal() ||
           self.type == 1 #-self.IDENTIFIER-# ||
           self.type == 36 #-self.VARIABLE_REF-# ||
           self.type == 25 #-self.RIGHT_PAREN-#
  end
  
  # Check if this token indicates the start of a new top-level statement
  # Useful for single-pass transpiler to know when to stop collecting expression tokens
  #
  # @return bool - True if token starts a new statement
  def is_statement_start()
    if self.type != 0 #-self.KEYWORD-#
      return false
    end
    
    for keyword : self.statement_keywords
      if self.value == keyword
        return true
      end
    end
    return false
  end
  
  # Check if this token is a DSL function name (for animation expressions)
  # Uses dynamic introspection to check if function exists in animation module
  #
  # @return bool - True if token is a DSL function name
  def is_dsl_function()
    if self.type != 0 #-self.KEYWORD-#
      return false
    end
    
    # Use dynamic introspection to check if function exists in animation module
    # This automatically supports any new functions added to the framework
    try
      import introspect
      var animation = global.animation
      if animation != nil
        var members = introspect.members(animation)
        return members.find(self.value) != nil
      end
    except .. as e, msg
      # Fallback to false if introspection fails
      return false
    end
    
    return false
  end
end

# Utility functions for token handling

# Create an EOF token at a specific position
#
# @param line: int - Line number
# @param column: int - Column number
# @return Token - EOF token
def create_eof_token(line, column)
  import animation_dsl
  return animation_dsl.Token(38 #-animation_dsl.Token.EOF-#, "", line, column, 0)
end

# Create an error token with a message
#
# @param message: string - Error message
# @param line: int - Line number
# @param column: int - Column number
# @return Token - Error token
def create_error_token(message, line, column)
  import animation_dsl
  return animation_dsl.Token(39 #-animation_dsl.Token.ERROR-#, message, line, column, size(message))
end

# Create a newline token
#
# @param line: int - Line number
# @param column: int - Column number
# @return Token - Newline token
def create_newline_token(line, column)
  import animation_dsl
  return animation_dsl.Token(35 #-animation_dsl.Token.NEWLINE-#, "\n", line, column, 1)
end

# Check if a string is a reserved keyword
#
# @param word: string - Word to check
# @return bool - True if word is a reserved keyword
def is_keyword(word)
  import animation_dsl
  for keyword : animation_dsl.Token.keywords
    if word == keyword
      return true
    end
  end
  return false
end

# Check if a string is a predefined color name
#
# @param word: string - Word to check
# @return bool - True if word is a predefined color name
def is_color_name(word)
  import animation_dsl
  for color : animation_dsl.Token.color_names
    if word == color
      return true
    end
  end
  return false
end

# Get the precedence of an operator token
#
# @param token: Token - Operator token
# @return int - Precedence level (higher number = higher precedence)
def get_operator_precedence(token)
  if token.type == 22 #-animation_dsl.Token.LOGICAL_OR-#
    return 1
  elif token.type == 21 #-animation_dsl.Token.LOGICAL_AND-#
    return 2
  elif token.type == 15 #-animation_dsl.Token.EQUAL-# || token.type == 16 #-animation_dsl.Token.NOT_EQUAL-#
    return 3
  elif token.type == 17 #-animation_dsl.Token.LESS_THAN-# || token.type == 18 #-animation_dsl.Token.LESS_EQUAL-# ||
       token.type == 19 #-animation_dsl.Token.GREATER_THAN-# || token.type == 20 #-animation_dsl.Token.GREATER_EQUAL-#
    return 4
  elif token.type == 9 #-animation_dsl.Token.PLUS-# || token.type == 10 #-animation_dsl.Token.MINUS-#
    return 5
  elif token.type == 11 #-animation_dsl.Token.MULTIPLY-# || token.type == 12 #-animation_dsl.Token.DIVIDE-# || token.type == 13 #-animation_dsl.Token.MODULO-#
    return 6
  elif token.type == 14 #-animation_dsl.Token.POWER-#
    return 7
  end
  return 0  # Not an operator or unknown operator
end

# Check if an operator is right-associative
#
# @param token: Token - Operator token
# @return bool - True if operator is right-associative
def is_right_associative(token)
  return token.type == 14 #-animation_dsl.Token.POWER-#  # Only power operator is right-associative
end

return {
  "Token": Token,
  "create_eof_token": create_eof_token,
  "create_error_token": create_error_token,
  "create_newline_token": create_newline_token,
  "is_keyword": is_keyword,
  "is_color_name": is_color_name,
  "get_operator_precedence": get_operator_precedence,
  "is_right_associative": is_right_associative
}