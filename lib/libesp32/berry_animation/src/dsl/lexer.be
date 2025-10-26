# Pull-Mode Lexer v2 for Animation DSL
# Combines pull-mode interface with original lexer.be implementation
# Reuses most of the code from lexer.be while providing pull-based token access

# Import token functions and Token class
import "dsl/token.be" as token_module
var Token = token_module["Token"]

#@ solidify:Lexer,weak
class Lexer
  var source          # String - DSL source code
  var position        # Integer - current character position
  var line            # Integer - current line number (1-based)
  var column          # Integer - current column number (1-based)
  var token_position  # Integer - current token position (for compatibility)
  
  # Initialize pull lexer with source code
  #
  # @param source: string - DSL source code to tokenize
  def init(source)
    self.source = source != nil ? source : ""
    self.position = 0
    self.line = 1
    self.column = 1
    self.token_position = 0
  end
  
  # Pull the next token from the stream
  # This is the main pull-mode interface - generates tokens on demand
  #
  # @return Token - Next token, or nil if at end
  def next_token()
    # Skip whitespace and comments until we find a meaningful token or reach end
    while !self.at_end()
      var start_column = self.column
      var ch = self.advance()
      
      if ch == ' ' || ch == '\t' || ch == '\r'
        # Skip whitespace (but not newlines - they can be significant)
        continue
      elif ch == '\n'
        var token = self.create_token(35 #-animation_dsl.Token.NEWLINE-#, "\n", 1)
        self.line += 1
        self.column = 1
        self.token_position += 1
        return token
      elif ch == '#'
        var token = self.scan_comment()
        self.token_position += 1
        return token
      elif ch == '0' && self.peek() == 'x'
        var token = self.scan_hex_color_0x()
        self.token_position += 1
        return token
      elif self.is_alpha(ch) || ch == '_'
        var token = self.scan_identifier_or_keyword()
        self.token_position += 1
        return token
      elif self.is_digit(ch)
        var token = self.scan_number()
        self.token_position += 1
        return token
      elif ch == '"' || ch == "'"
        # Check for triple quotes
        if (ch == '"' && self.peek() == '"' && self.peek_char_ahead(1) == '"') ||
           (ch == "'" && self.peek() == "'" && self.peek_char_ahead(1) == "'")
          var token = self.scan_triple_quoted_string(ch)
          self.token_position += 1
          return token
        else
          var token = self.scan_string(ch)
          self.token_position += 1
          return token
        end
      elif ch == '$'
        var token = self.scan_variable_reference()
        self.token_position += 1
        return token
      else
        var token = self.scan_operator_or_delimiter(ch)
        self.token_position += 1
        return token
      end
    end
    
    # Reached end of source
    return nil
  end
  
  # Peek at the next token without consuming it
  # Uses position saving/restoring to implement peek
  #
  # @return Token - Next token, or nil if at end
  def peek_token()
    # Save current state
    var saved_position = self.position
    var saved_line = self.line
    var saved_column = self.column
    var saved_token_position = self.token_position
    
    # Get next token
    var token = self.next_token()
    if (token != nil)
      # We haven't reached the end of the file    
      # Restore state
      self.position = saved_position
      self.line = saved_line
      self.column = saved_column
      self.token_position = saved_token_position
    end
    
    return token
  end
  
  # Peek ahead by n tokens without consuming them
  # Note: This is less efficient than the array-based version but maintains simplicity
  #
  # @param n: int - Number of tokens to look ahead (1-based)
  # @return Token - Token at position + n, or nil if beyond end
  def peek_ahead(n)
    if n <= 0 return nil end
    
    # Save current state
    var saved_position = self.position
    var saved_line = self.line
    var saved_column = self.column
    var saved_token_position = self.token_position
    
    # Advance n tokens
    var token = nil
    for i : 1..n
      token = self.next_token()
      if token == nil break end
    end
    
    # Restore state
    self.position = saved_position
    self.line = saved_line
    self.column = saved_column
    self.token_position = saved_token_position
    
    return token
  end
  
  # Check if we're at the end of the source
  #
  # @return bool - True if no more characters available
  def at_end()
    return self.position >= size(self.source)
  end
  
  # Reset to beginning of source
  def reset()
    self.position = 0
    self.line = 1
    self.column = 1
    self.token_position = 0
  end
  
  
  # Get current position in token stream (for compatibility with array-based version)
  #
  # @return int - Current token position
  def get_position()
    return self.token_position
  end
  
  # Set position in token stream (for compatibility with array-based version)
  # Note: This is a simplified implementation that resets to beginning and advances
  #
  # @param pos: int - New token position
  def set_position(pos)
    if pos < 0 return end
    
    # Save current state in case we need to restore it
    var saved_position = self.position
    var saved_line = self.line
    var saved_column = self.column
    var saved_token_position = self.token_position
    
    # Reset to beginning
    self.position = 0
    self.line = 1
    self.column = 1
    self.token_position = 0
    
    # Advance to desired token position
    while self.token_position < pos && !self.at_end()
      self.next_token()
    end
    
    # If we didn't reach the desired position, it was invalid - restore state
    if self.token_position != pos
      self.position = saved_position
      self.line = saved_line
      self.column = saved_column
      self.token_position = saved_token_position
    end
  end
  
  # Create a sub-lexer (for compatibility with array-based version)
  # Note: This converts token positions to character positions
  #
  # @param start_token_pos: int - Starting token position
  # @param end_token_pos: int - Ending token position (exclusive)
  # @return Lexer - New pull lexer with subset of source
  def create_sub_lexer(start_token_pos, end_token_pos)
    import animation_dsl
    # Check for invalid ranges
    if start_token_pos < 0 || end_token_pos <= start_token_pos
      # Invalid range - return empty sub-lexer
      return animation_dsl.create_lexer("")
    end
    
    # Save current state
    var saved_position = self.position
    var saved_line = self.line
    var saved_column = self.column
    var saved_token_position = self.token_position
    
    # Reset to beginning and find character positions for token positions
    self.position = 0
    self.line = 1
    self.column = 1
    self.token_position = 0
    
    var start_char_pos = 0
    var end_char_pos = size(self.source)
    var found_start = false
    var found_end = false
    
    # Find start position
    while self.token_position < start_token_pos && !self.at_end()
      start_char_pos = self.position
      self.next_token()
    end
    if self.token_position == start_token_pos
      start_char_pos = self.position
      found_start = true
    end
    
    # Find end position
    while self.token_position < end_token_pos && !self.at_end()
      self.next_token()
    end
    if self.token_position == end_token_pos
      end_char_pos = self.position
      found_end = true
    end
    
    # Restore state
    self.position = saved_position
    self.line = saved_line
    self.column = saved_column
    self.token_position = saved_token_position
    
    # Create sub-lexer with character range
    if !found_start
      return animation_dsl.create_lexer("")
    end
    
    # Clamp end position
    if end_char_pos > size(self.source) end_char_pos = size(self.source) end
    if start_char_pos >= end_char_pos
      return animation_dsl.create_lexer("")
    end
    
    # Extract subset of source
    var sub_source = self.source[start_char_pos..end_char_pos-1]
    var sub_lexer = animation_dsl.create_lexer(sub_source)
    # Ensure sub-lexer starts at position 0 (should already be 0 from init, but make sure)
    sub_lexer.position = 0
    sub_lexer.line = 1
    sub_lexer.column = 1
    sub_lexer.token_position = 0
    return sub_lexer
  end
  
  # === TOKEN SCANNING METHODS (from original lexer.be) ===
  
  # Scan comment (now unambiguous - only starts with #)
  def scan_comment()
    var start_pos = self.position - 1
    var start_column = self.column - 1
    
    # This is a comment - consume until end of line
    while !self.at_end() && self.peek() != '\n'
      self.advance()
    end
    
    var comment_text = self.source[start_pos..self.position-1]
    
    # Trim trailing whitespace from comment text manually
    # Find the last non-whitespace character in the comment content
    var trimmed_text = comment_text
    var end_pos = size(comment_text) - 1
    while end_pos >= 0 && (comment_text[end_pos] == ' ' || comment_text[end_pos] == '\t' || comment_text[end_pos] == '\r')
      end_pos -= 1
    end
    
    # Extract trimmed comment text
    if end_pos >= 0
      trimmed_text = comment_text[0 .. end_pos]
    else
      trimmed_text = "#"  # Keep at least the # character for empty comments
    end
    
    # Use trimmed text but keep original position tracking
    return self.create_token(37 #-animation_dsl.Token.COMMENT-#, trimmed_text, self.position - start_pos)
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
      return self.create_token(4 #-animation_dsl.Token.COLOR-#, color_value, size(color_value))
    else
      self.error("Invalid hex color format: " + color_value + " (expected 0xRRGGBB or 0xAARRGGBB)")
    end
  end
  
  # Scan identifier or keyword
  def scan_identifier_or_keyword()
    import animation_dsl
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
    
    return self.create_token(token_type, text, size(text))
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
      return self.create_token(5 #-animation_dsl.Token.TIME-#, number_text + suffix, size(number_text + suffix))
    # Check for percentage suffix
    elif !self.at_end() && self.peek() == '%'
      self.advance()
      return self.create_token(6 #-animation_dsl.Token.PERCENTAGE-#, number_text + "%", size(number_text) + 1)
    # Check for multiplier suffix
    elif !self.at_end() && self.peek() == 'x'
      self.advance()
      return self.create_token(7 #-animation_dsl.Token.MULTIPLIER-#, number_text + "x", size(number_text) + 1)
    else
      # Plain number
      return self.create_token(2 #-animation_dsl.Token.NUMBER-#, number_text, size(number_text))
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
      return self.create_token(3 #-animation_dsl.Token.STRING-#, value, self.position - start_pos)
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
         self.peek_char_ahead(1) == quote_char && 
         self.peek_char_ahead(2) == quote_char
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
      return self.create_token(3 #-animation_dsl.Token.STRING-#, value, self.position - start_pos)
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
    return self.create_token(36 #-animation_dsl.Token.VARIABLE_REF-#, var_ref, size(var_ref))
  end
  
  # Scan operator or delimiter
  def scan_operator_or_delimiter(ch)
    var start_column = self.column - 1
    
    if ch == '='
      if self.match('=')
        return self.create_token(15 #-animation_dsl.Token.EQUAL-#, "==", 2)
      else
        return self.create_token(8 #-animation_dsl.Token.ASSIGN-#, "=", 1)
      end
    elif ch == '!'
      if self.match('=')
        return self.create_token(16 #-animation_dsl.Token.NOT_EQUAL-#, "!=", 2)
      else
        return self.create_token(23 #-animation_dsl.Token.LOGICAL_NOT-#, "!", 1)
      end
    elif ch == '<'
      if self.match('=')
        return self.create_token(18 #-animation_dsl.Token.LESS_EQUAL-#, "<=", 2)
      elif self.match('<')
        # Left shift - not used in DSL but included for completeness
        self.error("Left shift operator '<<' not supported in DSL")
      else
        return self.create_token(17 #-animation_dsl.Token.LESS_THAN-#, "<", 1)
      end
    elif ch == '>'
      if self.match('=')
        return self.create_token(20 #-animation_dsl.Token.GREATER_EQUAL-#, ">=", 2)
      elif self.match('>')
        # Right shift - not used in DSL but included for completeness
        self.error("Right shift operator '>>' not supported in DSL")
      else
        return self.create_token(19 #-animation_dsl.Token.GREATER_THAN-#, ">", 1)
      end
    elif ch == '&'
      if self.match('&')
        return self.create_token(21 #-animation_dsl.Token.LOGICAL_AND-#, "&&", 2)
      else
        self.error("Single '&' not supported in DSL")
      end
    elif ch == '|'
      if self.match('|')
        return self.create_token(22 #-animation_dsl.Token.LOGICAL_OR-#, "||", 2)
      else
        self.error("Single '|' not supported in DSL")
      end
    elif ch == '-'
      if self.match('>')
        return self.create_token(34 #-animation_dsl.Token.ARROW-#, "->", 2)
      else
        return self.create_token(10 #-animation_dsl.Token.MINUS-#, "-", 1)
      end
    elif ch == '+'
      return self.create_token(9 #-animation_dsl.Token.PLUS-#, "+", 1)
    elif ch == '*'
      return self.create_token(11 #-animation_dsl.Token.MULTIPLY-#, "*", 1)
    elif ch == '/'
      return self.create_token(12 #-animation_dsl.Token.DIVIDE-#, "/", 1)
    elif ch == '%'
      return self.create_token(13 #-animation_dsl.Token.MODULO-#, "%", 1)
    elif ch == '^'
      return self.create_token(14 #-animation_dsl.Token.POWER-#, "^", 1)
    elif ch == '('
      return self.create_token(24 #-animation_dsl.Token.LEFT_PAREN-#, "(", 1)
    elif ch == ')'
      return self.create_token(25 #-animation_dsl.Token.RIGHT_PAREN-#, ")", 1)
    elif ch == '{'
      return self.create_token(26 #-animation_dsl.Token.LEFT_BRACE-#, "{", 1)
    elif ch == '}'
      return self.create_token(27 #-animation_dsl.Token.RIGHT_BRACE-#, "}", 1)
    elif ch == '['
      return self.create_token(28 #-animation_dsl.Token.LEFT_BRACKET-#, "[", 1)
    elif ch == ']'
      return self.create_token(29 #-animation_dsl.Token.RIGHT_BRACKET-#, "]", 1)
    elif ch == ','
      return self.create_token(30 #-animation_dsl.Token.COMMA-#, ",", 1)
    elif ch == ';'
      return self.create_token(31 #-animation_dsl.Token.SEMICOLON-#, ";", 1)
    elif ch == ':'
      return self.create_token(32 #-animation_dsl.Token.COLON-#, ":", 1)
    elif ch == '.'
      # For now, just handle single dots - range operators can be added later if needed
      return self.create_token(33 #-animation_dsl.Token.DOT-#, ".", 1)
    else
      self.error("Unexpected character: '" + ch + "'")
    end
  end
  
  # === HELPER METHODS (from original lexer.be) ===
  
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
  
  # Peek ahead by n characters without advancing
  def peek_char_ahead(n)
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
  
  # Create token with proper position tracking
  def create_token(token_type, value, length)
    import animation_dsl
    return animation_dsl.Token(token_type, value, self.line, self.column - length, length)
  end
  
  # Raise lexical error immediately
  def error(message)
    var error_msg = "Line " + str(self.line) + ":" + str(self.column) + ": " + message
    raise "lexical_error", error_msg
  end
end

return {
  "create_lexer": Lexer
}