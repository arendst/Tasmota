# DSL Lexer (Tokenizer) for Animation DSL
# Converts DSL source code into a stream of tokens for the single-pass transpiler

# Get reference to animation module (avoid circular import)

#@ solidify:DSLLexer,weak
class DSLLexer
  var source          # String - DSL source code
  var position        # Integer - current character position
  var line            # Integer - current line number (1-based)
  var column          # Integer - current column number (1-based)
  var tokens          # List - generated tokens
  var errors          # List - lexical errors encountered
  
  # Initialize lexer with source code
  #
  # @param source: string - DSL source code to tokenize
  def init(source)
    self.source = source != nil ? source : ""
    self.position = 0
    self.line = 1
    self.column = 1
    self.tokens = []
    self.errors = []
  end
  
  # Tokenize the entire source code
  #
  # @return list - Array of Token objects
  def tokenize()
    self.tokens = []
    self.errors = []
    self.position = 0
    self.line = 1
    self.column = 1
    
    while !self.at_end()
      self.scan_token()
    end
    
    # Add EOF token
    self.add_token(38 #-animation.Token.EOF-#, "", 0)
    
    return self.tokens
  end
  
  # Scan and create the next token
  def scan_token()
    var start_column = self.column
    var ch = self.advance()
    
    if ch == ' ' || ch == '\t' || ch == '\r'
      # Skip whitespace (but not newlines - they can be significant)
      return
    elif ch == '\n'
      self.add_token(35 #-animation.Token.NEWLINE-#, "\n", 1)
      self.line += 1
      self.column = 1
      return
    elif ch == '#'
      self.scan_comment_or_color()
    elif self.is_alpha(ch) || ch == '_'
      self.scan_identifier_or_keyword()
    elif self.is_digit(ch)
      self.scan_number()
    elif ch == '"' || ch == "'"
      self.scan_string(ch)
    elif ch == '$'
      self.scan_variable_reference()
    else
      self.scan_operator_or_delimiter(ch)
    end
  end
  
  # Scan comment or hex color (both start with #)
  def scan_comment_or_color()
    var start_pos = self.position - 1
    var start_column = self.column - 1
    
    # Look ahead to see if this is a hex color
    if self.position < size(self.source) && self.is_hex_digit(self.source[self.position])
      # This is a hex color
      self.scan_hex_color()
    else
      # This is a comment - consume until end of line
      while !self.at_end() && self.peek() != '\n'
        self.advance()
      end
      
      var comment_text = self.source[start_pos..self.position-1]
      self.add_token(37 #-animation.Token.COMMENT-#, comment_text, self.position - start_pos)
    end
  end
  
  # Scan hex color (#RRGGBB, #RGB, #AARRGGBB, or #ARGB)
  def scan_hex_color()
    var start_pos = self.position - 1  # Include the #
    var start_column = self.column - 1
    var hex_digits = 0
    
    # Count hex digits
    while !self.at_end() && self.is_hex_digit(self.peek())
      self.advance()
      hex_digits += 1
    end
    
    var color_value = self.source[start_pos..self.position-1]
    
    # Validate hex color format - support alpha channel
    if hex_digits == 3 || hex_digits == 4 || hex_digits == 6 || hex_digits == 8
      self.add_token(4 #-animation.Token.COLOR-#, color_value, size(color_value))
    else
      self.add_error("Invalid hex color format: " + color_value)
      self.add_token(39 #-animation.Token.ERROR-#, color_value, size(color_value))
    end
  end
  
  # Scan identifier or keyword
  def scan_identifier_or_keyword()
    var start_pos = self.position - 1
    var start_column = self.column - 1
    
    # Continue while alphanumeric or underscore
    while !self.at_end() && (self.is_alnum(self.peek()) || self.peek() == '_')
      self.advance()
    end
    
    var text = self.source[start_pos..self.position-1]
    var token_type
    
    # Check for color names first (they take precedence over keywords)
    if animation.is_color_name(text)
      token_type = 4 #-animation.Token.COLOR-#
    elif animation.is_keyword(text)
      token_type = 0 #-animation.Token.KEYWORD-#
    else
      token_type = 1 #-animation.Token.IDENTIFIER-#
    end
    
    self.add_token(token_type, text, size(text))
  end
  
  # Scan numeric literal (with optional time/percentage/multiplier suffix)
  def scan_number()
    var start_pos = self.position - 1
    var start_column = self.column - 1
    var has_dot = false
    
    # Scan integer part
    while !self.at_end() && self.is_digit(self.peek())
      self.advance()
    end
    
    # Check for decimal point
    if !self.at_end() && self.peek() == '.' && 
       self.position + 1 < size(self.source) && self.is_digit(self.source[self.position + 1])
      has_dot = true
      self.advance()  # consume '.'
      
      # Scan fractional part
      while !self.at_end() && self.is_digit(self.peek())
        self.advance()
      end
    end
    
    var number_text = self.source[start_pos..self.position-1]
    
    # Check for time unit suffixes
    if self.check_time_suffix()
      var suffix = self.scan_time_suffix()
      self.add_token(5 #-animation.Token.TIME-#, number_text + suffix, size(number_text + suffix))
    # Check for percentage suffix
    elif !self.at_end() && self.peek() == '%'
      self.advance()
      self.add_token(6 #-animation.Token.PERCENTAGE-#, number_text + "%", size(number_text) + 1)
    # Check for multiplier suffix
    elif !self.at_end() && self.peek() == 'x'
      self.advance()
      self.add_token(7 #-animation.Token.MULTIPLIER-#, number_text + "x", size(number_text) + 1)
    else
      # Plain number
      self.add_token(2 #-animation.Token.NUMBER-#, number_text, size(number_text))
    end
  end
  
  # Check if current position has a time suffix
  def check_time_suffix()
    import string
    if self.at_end()
      return false
    end
    
    var remaining = self.source[self.position..]
    return string.startswith(remaining, "ms") ||
           string.startswith(remaining, "s") ||
           string.startswith(remaining, "m") ||
           string.startswith(remaining, "h")
  end
  
  # Scan time suffix and return it
  def scan_time_suffix()
    import string
    if string.startswith(self.source[self.position..], "ms")
      self.advance()
      self.advance()
      return "ms"
    elif self.peek() == 's'
      self.advance()
      return "s"
    elif self.peek() == 'm'
      self.advance()
      return "m"
    elif self.peek() == 'h'
      self.advance()
      return "h"
    end
    return ""
  end
  
  # Scan string literal
  def scan_string(quote_char)
    var start_pos = self.position - 1  # Include opening quote
    var start_column = self.column - 1
    var value = ""
    
    while !self.at_end() && self.peek() != quote_char
      var ch = self.advance()
      
      if ch == '\\'
        # Handle escape sequences
        if !self.at_end()
          var escaped = self.advance()
          if escaped == 'n'
            value += '\n'
          elif escaped == 't'
            value += '\t'
          elif escaped == 'r'
            value += '\r'
          elif escaped == '\\'
            value += '\\'
          elif escaped == quote_char
            value += quote_char
          else
            # Unknown escape sequence - include as-is
            value += '\\'
            value += escaped
          end
        else
          value += '\\'
        end
      elif ch == '\n'
        self.line += 1
        self.column = 1
        value += ch
      else
        value += ch
      end
    end
    
    if self.at_end()
      self.add_error("Unterminated string literal")
      self.add_token(39 #-animation.Token.ERROR-#, value, self.position - start_pos)
    else
      # Consume closing quote
      self.advance()
      self.add_token(3 #-animation.Token.STRING-#, value, self.position - start_pos)
    end
  end
  
  # Scan variable reference ($identifier)
  def scan_variable_reference()
    var start_pos = self.position - 1  # Include $
    var start_column = self.column - 1
    
    if self.at_end() || !(self.is_alpha(self.peek()) || self.peek() == '_')
      self.add_error("Invalid variable reference: $ must be followed by identifier")
      self.add_token(39 #-animation.Token.ERROR-#, "$", 1)
      return
    end
    
    # Scan identifier part
    while !self.at_end() && (self.is_alnum(self.peek()) || self.peek() == '_')
      self.advance()
    end
    
    var var_ref = self.source[start_pos..self.position-1]
    self.add_token(36 #-animation.Token.VARIABLE_REF-#, var_ref, size(var_ref))
  end
  
  # Scan operator or delimiter
  def scan_operator_or_delimiter(ch)
    var start_column = self.column - 1
    
    if ch == '='
      if self.match('=')
        self.add_token(15 #-animation.Token.EQUAL-#, "==", 2)
      else
        self.add_token(8 #-animation.Token.ASSIGN-#, "=", 1)
      end
    elif ch == '!'
      if self.match('=')
        self.add_token(16 #-animation.Token.NOT_EQUAL-#, "!=", 2)
      else
        self.add_token(23 #-animation.Token.LOGICAL_NOT-#, "!", 1)
      end
    elif ch == '<'
      if self.match('=')
        self.add_token(18 #-animation.Token.LESS_EQUAL-#, "<=", 2)
      elif self.match('<')
        # Left shift - not used in DSL but included for completeness
        self.add_token(39 #-animation.Token.ERROR-#, "<<", 2)
      else
        self.add_token(17 #-animation.Token.LESS_THAN-#, "<", 1)
      end
    elif ch == '>'
      if self.match('=')
        self.add_token(20 #-animation.Token.GREATER_EQUAL-#, ">=", 2)
      elif self.match('>')
        # Right shift - not used in DSL but included for completeness
        self.add_token(39 #-animation.Token.ERROR-#, ">>", 2)
      else
        self.add_token(19 #-animation.Token.GREATER_THAN-#, ">", 1)
      end
    elif ch == '&'
      if self.match('&')
        self.add_token(21 #-animation.Token.LOGICAL_AND-#, "&&", 2)
      else
        self.add_error("Single '&' not supported in DSL")
        self.add_token(39 #-animation.Token.ERROR-#, "&", 1)
      end
    elif ch == '|'
      if self.match('|')
        self.add_token(22 #-animation.Token.LOGICAL_OR-#, "||", 2)
      else
        self.add_error("Single '|' not supported in DSL")
        self.add_token(39 #-animation.Token.ERROR-#, "|", 1)
      end
    elif ch == '-'
      if self.match('>')
        self.add_token(34 #-animation.Token.ARROW-#, "->", 2)
      else
        self.add_token(10 #-animation.Token.MINUS-#, "-", 1)
      end
    elif ch == '+'
      self.add_token(9 #-animation.Token.PLUS-#, "+", 1)
    elif ch == '*'
      self.add_token(11 #-animation.Token.MULTIPLY-#, "*", 1)
    elif ch == '/'
      self.add_token(12 #-animation.Token.DIVIDE-#, "/", 1)
    elif ch == '%'
      self.add_token(13 #-animation.Token.MODULO-#, "%", 1)
    elif ch == '^'
      self.add_token(14 #-animation.Token.POWER-#, "^", 1)
    elif ch == '('
      self.add_token(24 #-animation.Token.LEFT_PAREN-#, "(", 1)
    elif ch == ')'
      self.add_token(25 #-animation.Token.RIGHT_PAREN-#, ")", 1)
    elif ch == '{'
      self.add_token(26 #-animation.Token.LEFT_BRACE-#, "{", 1)
    elif ch == '}'
      self.add_token(27 #-animation.Token.RIGHT_BRACE-#, "}", 1)
    elif ch == '['
      self.add_token(28 #-animation.Token.LEFT_BRACKET-#, "[", 1)
    elif ch == ']'
      self.add_token(29 #-animation.Token.RIGHT_BRACKET-#, "]", 1)
    elif ch == ','
      self.add_token(30 #-animation.Token.COMMA-#, ",", 1)
    elif ch == ';'
      self.add_token(31 #-animation.Token.SEMICOLON-#, ";", 1)
    elif ch == ':'
      self.add_token(32 #-animation.Token.COLON-#, ":", 1)
    elif ch == '.'
      if self.match('.')
        # Range operator (..) - treat as two dots for now
        self.add_token(33 #-animation.Token.DOT-#, ".", 1)
        self.add_token(33 #-animation.Token.DOT-#, ".", 1)
      else
        self.add_token(33 #-animation.Token.DOT-#, ".", 1)
      end
    else
      self.add_error("Unexpected character: '" + ch + "'")
      self.add_token(39 #-animation.Token.ERROR-#, ch, 1)
    end
  end
  
  # Helper methods
  
  # Check if at end of source
  def at_end()
    return self.position >= size(self.source)
  end
  
  # Advance position and return current character
  def advance()
    if self.at_end()
      return ""
    end
    
    var ch = self.source[self.position]
    self.position += 1
    self.column += 1
    return ch
  end
  
  # Peek at current character without advancing
  def peek()
    if self.at_end()
      return ""
    end
    return self.source[self.position]
  end
  
  # Peek at next character without advancing
  def peek_next()
    if self.position + 1 >= size(self.source)
      return ""
    end
    return self.source[self.position + 1]
  end
  
  # Check if current character matches expected and advance if so
  def match(expected)
    if self.at_end() || self.source[self.position] != expected
      return false
    end
    
    self.position += 1
    self.column += 1
    return true
  end
  
  # Character classification helpers
  def is_alpha(ch)
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
  end
  
  def is_digit(ch)
    return ch >= '0' && ch <= '9'
  end
  
  def is_alnum(ch)
    return self.is_alpha(ch) || self.is_digit(ch)
  end
  
  def is_hex_digit(ch)
    return self.is_digit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F')
  end
  
  # Add token to tokens list
  def add_token(token_type, value, length)
    var token = animation.Token(token_type, value, self.line, self.column - length, length)
    self.tokens.push(token)
  end
  
  # Add error to errors list
  def add_error(message)
    self.errors.push({
      "message": message,
      "line": self.line,
      "column": self.column,
      "position": self.position
    })
  end
  
  # Get all errors encountered during tokenization
  def get_errors()
    return self.errors
  end
  
  # Check if any errors were encountered
  def has_errors()
    return size(self.errors) > 0
  end
  
  # Get a formatted error report
  def get_error_report()
    if !self.has_errors()
      return "No lexical errors"
    end
    
    var report = "Lexical errors (" + str(size(self.errors)) + "):\n"
    for error : self.errors
      report += "  Line " + str(error["line"]) + ":" + str(error["column"]) + ": " + error["message"] + "\n"
    end
    return report
  end
  
  # Reset lexer state for reuse
  def reset(new_source)
    self.source = new_source != nil ? new_source : ""
    self.position = 0
    self.line = 1
    self.column = 1
    self.tokens = []
    self.errors = []
  end
  
  # Get current position info for debugging
  def get_position_info()
    return {
      "position": self.position,
      "line": self.line,
      "column": self.column,
      "at_end": self.at_end()
    }
  end
  
  # Tokenize and return both tokens and errors
  def tokenize_with_errors()
    var tokens = self.tokenize()
    var result = {
      "tokens": tokens,
      "errors": self.errors,
      "success": !self.has_errors()
    }
    return result
  end
end

# Utility function to tokenize DSL source code
#
# @param source: string - DSL source code
# @return list - Array of Token objects
def tokenize_dsl(source)
  var lexer = animation.DSLLexer(source)
  return lexer.tokenize()
end

# Utility function to tokenize with error handling
#
# @param source: string - DSL source code
# @return map - {tokens: list, errors: list, success: bool}
def tokenize_dsl_with_errors(source)
  var lexer = animation.DSLLexer(source)
  return lexer.tokenize_with_errors()
end

return {
  "DSLLexer": DSLLexer,
  "tokenize_dsl": tokenize_dsl,
  "tokenize_dsl_with_errors": tokenize_dsl_with_errors
}