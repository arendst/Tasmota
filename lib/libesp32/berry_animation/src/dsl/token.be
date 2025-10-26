# Token Types and Token Class for Animation DSL
# Defines all token types and the Token class with line/column tracking

#@ solidify:Token,weak
class Token
  # Basic token types
  # static var KEYWORD = 0        # strip, color, animation, sequence, etc.
  # static var IDENTIFIER = 1     # user-defined names
  # static var NUMBER = 2         # 123, 3.14
  # static var STRING = 3         # "hello", 'world'
  # static var COLOR = 4          # #FF0000, rgb(255,0,0), hsv(240,100,100)
  # static var TIME = 5           # 2s, 500ms, 1m, 2h
  # static var PERCENTAGE = 6     # 50%, 100%
  # static var MULTIPLIER = 7     # 2x, 0.5x
  
  # Human readable type name for each type value
  static var names = [
    "KEYWORD", "IDENTIFIER", "NUMBER", "STRING", "COLOR", "TIME", "PERCENTAGE", "MULTIPLIER",
    "ASSIGN", "PLUS", "MINUS", "MULTIPLY", "DIVIDE", "MODULO", "POWER",
    "EQUAL", "NOT_EQUAL", "LESS_THAN", "LESS_EQUAL", "GREATER_THAN", "GREATER_EQUAL",
    "LOGICAL_AND", "LOGICAL_OR", "LOGICAL_NOT",
    "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE", "LEFT_BRACKET", "RIGHT_BRACKET",
    "COMMA", "SEMICOLON", "COLON", "DOT", "ARROW",
    "NEWLINE", "VARIABLE_REF", "COMMENT", "" #-ex-EOF-#, "ERROR",
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
  
  # # Operators
  # static var ASSIGN = 8         # =
  # static var PLUS = 9           # +
  # static var MINUS = 10         # -
  # static var MULTIPLY = 11      # *
  # static var DIVIDE = 12        # /
  # static var MODULO = 13        # %
  # static var POWER = 14         # ^
  
  # # Comparison operators
  # static var EQUAL = 15         # ==
  # static var NOT_EQUAL = 16     # !=
  # static var LESS_THAN = 17     # <
  # static var LESS_EQUAL = 18    # <=
  # static var GREATER_THAN = 19  # >
  # static var GREATER_EQUAL = 20 # >=
  
  # # Logical operators
  # static var LOGICAL_AND = 21   # &&
  # static var LOGICAL_OR = 22    # ||
  # static var LOGICAL_NOT = 23   # !
  
  # # Delimiters
  # static var LEFT_PAREN = 24    # (
  # static var RIGHT_PAREN = 25   # )
  # static var LEFT_BRACE = 26    # {
  # static var RIGHT_BRACE = 27   # }
  # static var LEFT_BRACKET = 28  # [
  # static var RIGHT_BRACKET = 29 # ]
  
  # # Separators
  # static var COMMA = 30         # ,
  # static var SEMICOLON = 31     # ;
  # static var COLON = 32         # :
  # static var DOT = 33           # .
  # static var ARROW = 34         # ->
  
  # # Special tokens
  # static var NEWLINE = 35       # \n (significant in some contexts)
  # static var VARIABLE_REF = 36  # $identifier
  # static var COMMENT = 37       # # comment text
  # # static var EOF = 38           # End of file (REMOVED - reserved number)
  # static var ERROR = 39         # Error token for invalid input
  
  # # Event-related tokens
  # static var EVENT_ON = 40      # on (event handler keyword)
  # static var EVENT_INTERRUPT = 41  # interrupt
  # static var EVENT_RESUME = 42  # resume
  # static var EVENT_AFTER = 43   # after (for resume timing)
  
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
  
  # Get a string representation of the token for debugging
  #
  # @return string - Human-readable token description
  def tostring()
    var type_name = "UNKNOWN"
    if self.type >= 0 && self.type < size(self.names)
      type_name = self.names[self.type]
    end
    # if self.type == 38 #-self.EOF-#
    #   return f"Token({type_name} at {self.line}:{self.column})"
    if self.type == 35 #-self.NEWLINE-#
      return f"Token({type_name} at {self.line}:{self.column})"
    elif size(self.value) > 20
      var short_value = self.value[0..17] + "..."
      return f"Token({type_name}, '{short_value}' at {self.line}:{self.column})"
    else
      return f"Token({type_name}, '{self.value}' at {self.line}:{self.column})"
    end
  end
  
end

# Utility functions for token handling

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

return {
  "Token": Token,
  "is_keyword": is_keyword,
  "is_color_name": is_color_name
}