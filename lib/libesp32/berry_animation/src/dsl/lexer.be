# DSL Lexer (Tokenizer) for Animation DSL
# Converts DSL source code into a stream of tokens for the single-pass transpiler

# Import token functions and Token class
import "dsl/token.be" as token_module
var Token = token_module["Token"]

#@ solidify:DSLLexer,weak
class DSLLexer
  var source          # String - DSL source code
  var position        # Integer - current character position
  var line            # Integer - current line number (1-based)
  var column          # Integer - current column number (1-based)
  var tokens          # List - generated tokens
  
  # Initialize lexer with source code
  #
  # @param source: string - DSL source code to tokenize
  def init(source)
    self.source = source != nil ? source : ""
    self.position = 0
    self.line = 1
    self.column = 1
    self.tokens = []
  end
  
  # Tokenize the entire source code
  #
  # @return list - Array of Token objects
  def tokenize()
    self.tokens = []
    self.position = 0
    self.line = 1
    self.column = 1
    
    while !self.at_end()
      self.scan_token()
    end
    
    # Add EOF token
    self.add_token(38 #-animation_dsl.Token.EOF-#, "", 0)
    
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
      self.add_token(35 #-animation_dsl.Token.NEWLINE-#, "\n", 1)
      self.line += 1
      self.column = 1
      return
    elif ch == '#'
      self.scan_comment()
    elif ch == '0' && self.peek() == 'x'
      self.scan_hex_color_0x()
    elif self.is_alpha(ch) || ch == '_'
      self.scan_identifier_or_keyword()
    elif self.is_digit(ch)
      self.scan_number()
    elif ch == '"' || ch == "'"
      # Check for triple quotes
      if (ch == '"' && self.peek() == '"' && self.peek_ahead(1) == '"') ||
         (ch == "'" && self.peek() == "'" && self.peek_ahead(1) == "'")
        self.scan_triple_quoted_string(ch)
      else
        self.scan_string(ch)
      end
    elif ch == '$'
      self.scan_variable_reference()
    else
      self.scan_operator_or_delimiter(ch)
    end
  end
  
  # Scan comment (now unambiguous - only starts with #)
  def scan_comment()
    var start_pos = self.position - 1
    var start_column = self.column - 1
    
    # This is a comment - consume until end of line
    while !self.at_end() && self.peek() != '\n'
      self.advance()
    end
    
    var comment_text = self.source[start_pos..self.position-1]
    self.add_token(37 #-animation_dsl.Token.COMMENT-#, comment_text, self.position - start_pos)
  end
  
  # Scan hex color (0xRRGGBB, 0xAARRGGBB)
  def scan_hex_color_0x()
    var start_pos = self.position - 1  # Include the '0'
    var start_column = self.column - 1
    
    # Advance past 'x'
    self.advance()
    var hex_digits = 0
    
    # Count hex digits
    while !self.at_end() && self.is_hex_digit(self.peek())
      self.advance()
      hex_digits += 1
    end
    
    var color_value = self.source[start_pos..self.position-1]
    
    # Validate hex color format - support 6 (RGB) or 8 (ARGB) digits
    if hex_digits == 6 || hex_digits == 8
      self.add_token(4 #-animation_dsl.Token.COLOR-#, color_value, size(color_value))
    else
      self.error("Invalid hex color format: " + color_value + " (expected 0xRRGGBB or 0xAARRGGBB)")
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
    if animation_dsl.is_color_name(text)
      token_type = 4 #-animation_dsl.Token.COLOR-#
    elif animation_dsl.is_keyword(text)
      token_type = 0 #-animation_dsl.Token.KEYWORD-#
    else
      token_type = 1 #-animation_dsl.Token.IDENTIFIER-#
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
      self.add_token(5 #-animation_dsl.Token.TIME-#, number_text + suffix, size(number_text + suffix))
    # Check for percentage suffix
    elif !self.at_end() && self.peek() == '%'
      self.advance()
      self.add_token(6 #-animation_dsl.Token.PERCENTAGE-#, number_text + "%", size(number_text) + 1)
    # Check for multiplier suffix
    elif !self.at_end() && self.peek() == 'x'
      self.advance()
      self.add_token(7 #-animation_dsl.Token.MULTIPLIER-#, number_text + "x", size(number_text) + 1)
    else
      # Plain number
      self.add_token(2 #-animation_dsl.Token.NUMBER-#, number_text, size(number_text))
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
      self.error("Unterminated string literal")
    else
      # Consume closing quote
      self.advance()
      self.add_token(3 #-animation_dsl.Token.STRING-#, value, self.position - start_pos)
    end
  end
  
  # Scan triple-quoted string literal (for berry code blocks)
  def scan_triple_quoted_string(quote_char)
    var start_pos = self.position - 1  # Include first opening quote
    var start_column = self.column - 1
    var value = ""
    
    # Consume the two remaining opening quotes
    self.advance()  # second quote
    self.advance()  # third quote
    
    # Look for the closing triple quotes
    while !self.at_end()
      var ch = self.peek()
      
      # Check for closing triple quotes
      if ch == quote_char && 
         self.peek_ahead(1) == quote_char && 
         self.peek_ahead(2) == quote_char
        # Found closing triple quotes - consume them
        self.advance()  # first closing quote
        self.advance()  # second closing quote
        self.advance()  # third closing quote
        break
      end
      
      # Regular character - add to value
      ch = self.advance()
      if ch == '\n'
        self.line += 1
        self.column = 1
      end
      value += ch
    end
    
    # Check if we reached end without finding closing quotes
    if self.at_end() && !(self.source[self.position-3..self.position-1] == quote_char + quote_char + quote_char)
      self.error("Unterminated triple-quoted string literal")
    else
      self.add_token(3 #-animation_dsl.Token.STRING-#, value, self.position - start_pos)
    end
  end
  
  # Scan variable reference ($identifier)
  def scan_variable_reference()
    var start_pos = self.position - 1  # Include $
    var start_column = self.column - 1
    
    if self.at_end() || !(self.is_alpha(self.peek()) || self.peek() == '_')
      self.error("Invalid variable reference: $ must be followed by identifier")
    end
    
    # Scan identifier part
    while !self.at_end() && (self.is_alnum(self.peek()) || self.peek() == '_')
      self.advance()
    end
    
    var var_ref = self.source[start_pos..self.position-1]
    self.add_token(36 #-animation_dsl.Token.VARIABLE_REF-#, var_ref, size(var_ref))
  end
  
  # Scan operator or delimiter
  def scan_operator_or_delimiter(ch)
    var start_column = self.column - 1
    
    if ch == '='
      if self.match('=')
        self.add_token(15 #-animation_dsl.Token.EQUAL-#, "==", 2)
      else
        self.add_token(8 #-animation_dsl.Token.ASSIGN-#, "=", 1)
      end
    elif ch == '!'
      if self.match('=')
        self.add_token(16 #-animation_dsl.Token.NOT_EQUAL-#, "!=", 2)
      else
        self.add_token(23 #-animation_dsl.Token.LOGICAL_NOT-#, "!", 1)
      end
    elif ch == '<'
      if self.match('=')
        self.add_token(18 #-animation_dsl.Token.LESS_EQUAL-#, "<=", 2)
      elif self.match('<')
        # Left shift - not used in DSL but included for completeness
        self.error("Left shift operator '<<' not supported in DSL")
      else
        self.add_token(17 #-animation_dsl.Token.LESS_THAN-#, "<", 1)
      end
    elif ch == '>'
      if self.match('=')
        self.add_token(20 #-animation_dsl.Token.GREATER_EQUAL-#, ">=", 2)
      elif self.match('>')
        # Right shift - not used in DSL but included for completeness
        self.error("Right shift operator '>>' not supported in DSL")
      else
        self.add_token(19 #-animation_dsl.Token.GREATER_THAN-#, ">", 1)
      end
    elif ch == '&'
      if self.match('&')
        self.add_token(21 #-animation_dsl.Token.LOGICAL_AND-#, "&&", 2)
      else
        self.error("Single '&' not supported in DSL")
      end
    elif ch == '|'
      if self.match('|')
        self.add_token(22 #-animation_dsl.Token.LOGICAL_OR-#, "||", 2)
      else
        self.error("Single '|' not supported in DSL")
      end
    elif ch == '-'
      if self.match('>')
        self.add_token(34 #-animation_dsl.Token.ARROW-#, "->", 2)
      else
        self.add_token(10 #-animation_dsl.Token.MINUS-#, "-", 1)
      end
    elif ch == '+'
      self.add_token(9 #-animation_dsl.Token.PLUS-#, "+", 1)
    elif ch == '*'
      self.add_token(11 #-animation_dsl.Token.MULTIPLY-#, "*", 1)
    elif ch == '/'
      self.add_token(12 #-animation_dsl.Token.DIVIDE-#, "/", 1)
    elif ch == '%'
      self.add_token(13 #-animation_dsl.Token.MODULO-#, "%", 1)
    elif ch == '^'
      self.add_token(14 #-animation_dsl.Token.POWER-#, "^", 1)
    elif ch == '('
      self.add_token(24 #-animation_dsl.Token.LEFT_PAREN-#, "(", 1)
    elif ch == ')'
      self.add_token(25 #-animation_dsl.Token.RIGHT_PAREN-#, ")", 1)
    elif ch == '{'
      self.add_token(26 #-animation_dsl.Token.LEFT_BRACE-#, "{", 1)
    elif ch == '}'
      self.add_token(27 #-animation_dsl.Token.RIGHT_BRACE-#, "}", 1)
    elif ch == '['
      self.add_token(28 #-animation_dsl.Token.LEFT_BRACKET-#, "[", 1)
    elif ch == ']'
      self.add_token(29 #-animation_dsl.Token.RIGHT_BRACKET-#, "]", 1)
    elif ch == ','
      self.add_token(30 #-animation_dsl.Token.COMMA-#, ",", 1)
    elif ch == ';'
      self.add_token(31 #-animation_dsl.Token.SEMICOLON-#, ";", 1)
    elif ch == ':'
      self.add_token(32 #-animation_dsl.Token.COLON-#, ":", 1)
    elif ch == '.'
      if self.match('.')
        # Range operator (..) - treat as two dots for now
        self.add_token(33 #-animation_dsl.Token.DOT-#, ".", 1)
        self.add_token(33 #-animation_dsl.Token.DOT-#, ".", 1)
      else
        self.add_token(33 #-animation_dsl.Token.DOT-#, ".", 1)
      end
    else
      self.error("Unexpected character: '" + ch + "'")
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
  
  # Peek ahead by n characters without advancing
  def peek_ahead(n)
    if self.position + n >= size(self.source)
      return ""
    end
    return self.source[self.position + n]
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
    var token = animation_dsl.Token(token_type, value, self.line, self.column - length, length)
    self.tokens.push(token)
  end
  
  # Raise lexical error immediately
  def error(message)
    var error_msg = "Line " + str(self.line) + ":" + str(self.column) + ": " + message
    raise "lexical_error", error_msg
  end
  
  # Reset lexer state for reuse
  def reset(new_source)
    self.source = new_source != nil ? new_source : ""
    self.position = 0
    self.line = 1
    self.column = 1
    self.tokens = []
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
  

end

# Utility function to tokenize DSL source code
#
# @param source: string - DSL source code
# @return list - Array of Token objects
def tokenize_dsl(source)
  var lexer = animation_dsl.DSLLexer(source)
  return lexer.tokenize()
end

return {
  "DSLLexer": DSLLexer,
  "tokenize_dsl": tokenize_dsl
}
