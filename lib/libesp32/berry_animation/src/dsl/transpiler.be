# Ultra-Simplified DSL Transpiler for Animation DSL
# Single-pass transpiler with minimal complexity
# Leverages Berry's runtime for symbol resolution

#@ solidify:SimpleDSLTranspiler,weak
class SimpleDSLTranspiler
  var tokens          # Token stream from lexer
  var pos             # Current token position
  var output          # Generated Berry code lines
  var errors          # Compilation errors
  var run_statements  # Collect all run statements for single engine.start()
  var first_statement # Track if we're processing the first statement
  var strip_initialized # Track if strip was initialized
  
  # Static color mapping for named colors (helps with solidification)
  static var named_colors = {
    "red": "0xFFFF0000", "green": "0xFF008000", "blue": "0xFF0000FF",
    "white": "0xFFFFFFFF", "black": "0xFF000000", "yellow": "0xFFFFFF00",
    "orange": "0xFFFFA500", "purple": "0xFF800080", "pink": "0xFFFFC0CB",
    "cyan": "0xFF00FFFF", "magenta": "0xFFFF00FF", "gray": "0xFF808080",
    "grey": "0xFF808080", "silver": "0xFFC0C0C0", "gold": "0xFFFFD700",
    "brown": "0xFFA52A2A", "lime": "0xFF00FF00", "navy": "0xFF000080",
    "olive": "0xFF808000", "maroon": "0xFF800000", "teal": "0xFF008080",
    "aqua": "0xFF00FFFF", "fuchsia": "0xFFFF00FF", "indigo": "0xFF4B0082",
    "violet": "0xFFEE82EE", "crimson": "0xFFDC143C", "coral": "0xFFFF7F50",
    "salmon": "0xFFFA8072", "khaki": "0xFFF0E68C", "plum": "0xFFDDA0DD",
    "orchid": "0xFFDA70D6", "turquoise": "0xFF40E0D0", "tan": "0xFFD2B48C",
    "beige": "0xFFF5F5DC", "ivory": "0xFFFFFFF0", "snow": "0xFFFFFAFA",
    "transparent": "0x00000000"
  }
  
  def init(tokens)
    self.tokens = tokens != nil ? tokens : []
    self.pos = 0
    self.output = []
    self.errors = []
    self.run_statements = []
    self.first_statement = true  # Track if we're processing the first statement
    self.strip_initialized = false  # Track if strip was initialized
  end
  
  # Main transpilation method - single pass
  def transpile()
    try
      self.add("import animation")
      self.add("")
      
      # Single pass: process all statements
      while !self.at_end()
        self.process_statement()
      end
      
      # Generate single engine.start() call after all run statements
      self.generate_engine_start()
      
      return size(self.errors) == 0 ? self.join_output() : nil
    except .. as e, msg
      self.error(f"Transpilation failed: {msg}")
      return nil
    end
  end
  
  # Process statements - simplified approach
  def process_statement()
    var tok = self.current()
    if tok == nil || tok.type == animation.Token.EOF
      return
    end
    
    # Handle comments - preserve them in generated code
    if tok.type == animation.Token.COMMENT
      self.add(tok.value)  # Add comment as-is to output
      self.next()
      return
    end
    
    # Skip whitespace (newlines)
    if tok.type == animation.Token.NEWLINE
      self.next()
      return
    end
    
    # Check if this is the first non-comment, non-whitespace statement
    var is_first_real_statement = self.first_statement
    if tok.type == animation.Token.KEYWORD || tok.type == animation.Token.IDENTIFIER
      self.first_statement = false
    end
    
    # Handle keywords
    if tok.type == animation.Token.KEYWORD
      if tok.value == "strip"
        if !is_first_real_statement
          self.error("'strip' declaration must be the first statement")
          self.skip_statement()
          return
        end
        self.process_strip()
      else
        # For any other statement, ensure strip is initialized
        if !self.strip_initialized
          self.generate_default_strip_initialization()
        end
        
        if tok.value == "color"
          self.process_color()
        elif tok.value == "palette"
          self.process_palette()
        elif tok.value == "pattern"  
          self.process_pattern()
        elif tok.value == "animation"
          self.process_animation()
        elif tok.value == "set"
          self.process_set()
        elif tok.value == "sequence"
          self.process_sequence()
        elif tok.value == "run"
          self.process_run()
        elif tok.value == "on"
          self.process_event_handler()
        else
          self.skip_statement()
        end
      end
    elif tok.type == animation.Token.IDENTIFIER
      # For property assignments, ensure strip is initialized
      if !self.strip_initialized
        self.generate_default_strip_initialization()
      end
      # Check if this is a property assignment (identifier.property = value)
      self.process_property_assignment()
    else
      self.skip_statement()
    end
  end
  
  # Process color definition: color red = #FF0000
  def process_color()
    self.next()  # skip 'color'
    var name = self.expect_identifier()
    
    # Validate that the color name is not reserved
    if !self.validate_user_name(name, "color")
      self.skip_statement()
      return
    end
    
    self.expect_assign()
    var value = self.process_value("color")
    var inline_comment = self.collect_inline_comment()
    self.add(f"var {name}_ = {value}{inline_comment}")
  end
  
  # Process palette definition: palette aurora_colors = [(0, #000022), (64, #004400), ...]
  def process_palette()
    self.next()  # skip 'palette'
    var name = self.expect_identifier()
    
    # Validate that the palette name is not reserved
    if !self.validate_user_name(name, "palette")
      self.skip_statement()
      return
    end
    
    self.expect_assign()
    
    # Expect array literal with tuples
    self.expect_left_bracket()
    var palette_entries = []
    
    while !self.at_end() && !self.check_right_bracket()
      self.skip_whitespace()
      
      if self.check_right_bracket()
        break
      end
      
      # Parse tuple (value, color)
      self.expect_left_paren()
      var value = self.expect_number()
      self.expect_comma()
      var color = self.process_value("color")  # Reuse existing color parsing
      self.expect_right_paren()
      
      # Convert to VRGB format entry
      var vrgb_entry = self.convert_to_vrgb(value, color)
      palette_entries.push(f'"{vrgb_entry}"')
      
      self.skip_whitespace()
      
      if self.current() != nil && self.current().type == animation.Token.COMMA
        self.next()  # skip comma
        self.skip_whitespace()
      elif !self.check_right_bracket()
        self.error("Expected ',' or ']' in palette definition")
        break
      end
    end
    
    self.expect_right_bracket()
    var inline_comment = self.collect_inline_comment()
    
    # Generate Berry bytes object
    var palette_data = ""
    for i : 0..size(palette_entries)-1
      if i > 0
        palette_data += " "
      end
      palette_data += palette_entries[i]
    end
    
    self.add(f"var {name}_ = bytes({palette_data}){inline_comment}")
  end
  
  # Process pattern definition: pattern solid_red = solid(red)
  def process_pattern()
    self.next()  # skip 'pattern'
    var name = self.expect_identifier()
    
    # Validate that the pattern name is not reserved
    if !self.validate_user_name(name, "pattern")
      self.skip_statement()
      return
    end
    
    self.expect_assign()
    var value = self.process_value("pattern")
    var inline_comment = self.collect_inline_comment()
    self.add(f"var {name}_ = {value}{inline_comment}")
  end
  
  # Process animation definition: animation pulse_red = pulse(solid_red, 2s)
  def process_animation()
    self.next()  # skip 'animation'
    var name = self.expect_identifier()
    
    # Validate that the animation name is not reserved
    if !self.validate_user_name(name, "animation")
      self.skip_statement()
      return
    end
    
    self.expect_assign()
    var value = self.process_value("animation")
    var inline_comment = self.collect_inline_comment()
    self.add(f"var {name}_ = {value}{inline_comment}")
  end
  
  # Process strip configuration: strip length 60
  def process_strip()
    self.next()  # skip 'strip'
    var prop = self.expect_identifier()
    if prop == "length"
      var length = self.expect_number()
      var inline_comment = self.collect_inline_comment()
      self.add(f"var strip = global.Leds({length}){inline_comment}")
      self.add(f"var engine = animation.create_engine(strip)")
      self.strip_initialized = true  # Mark that strip was initialized
    end
  end
  
  # Process variable assignment: set brightness = 80%
  def process_set()
    self.next()  # skip 'set'
    var name = self.expect_identifier()
    
    # Validate that the variable name is not reserved
    if !self.validate_user_name(name, "variable")
      self.skip_statement()
      return
    end
    
    self.expect_assign()
    var value = self.process_value("variable")
    var inline_comment = self.collect_inline_comment()
    self.add(f"var {name}_ = {value}{inline_comment}")
  end
  
  # Process sequence definition: sequence demo { ... }
  def process_sequence()
    self.next()  # skip 'sequence'
    var name = self.expect_identifier()
    
    # Validate that the sequence name is not reserved
    if !self.validate_user_name(name, "sequence")
      self.skip_statement()
      return
    end
    
    self.expect_left_brace()
    
    self.add(f"def sequence_{name}()")
    self.add(f"  var steps = []")
    
    # Process sequence body
    while !self.at_end() && !self.check_right_brace()
      self.process_sequence_statement()
    end
    
    self.add(f"  var seq_manager = animation.SequenceManager(engine)")
    self.add(f"  seq_manager.start_sequence(steps)")
    self.add(f"  return seq_manager")
    self.add("end")
    self.expect_right_brace()
  end
  
  # Process statements inside sequences
  def process_sequence_statement()
    var tok = self.current()
    if tok == nil || tok.type == animation.Token.EOF
      return
    end
    
    # Handle comments - preserve them in generated code with proper indentation
    if tok.type == animation.Token.COMMENT
      self.add("  " + tok.value)  # Add comment with sequence indentation
      self.next()
      return
    end
    
    # Skip whitespace (newlines)
    if tok.type == animation.Token.NEWLINE
      self.next()
      return
    end
    
    if tok.type == animation.Token.KEYWORD && tok.value == "play"
      self.next()  # skip 'play'
      var anim_name = self.expect_identifier()
      
      # Handle optional 'for duration'
      var duration = "0"
      if self.current() != nil && self.current().type == animation.Token.KEYWORD && self.current().value == "for"
        self.next()  # skip 'for'
        duration = str(self.process_time_value())
      end
      
      var inline_comment = self.collect_inline_comment()
      self.add(f"  steps.push(animation.create_play_step(animation.global('{anim_name}_'), {duration})){inline_comment}")
      
    elif tok.type == animation.Token.KEYWORD && tok.value == "wait"
      self.next()  # skip 'wait'
      var duration = self.process_time_value()
      var inline_comment = self.collect_inline_comment()
      self.add(f"  steps.push(animation.create_wait_step({duration})){inline_comment}")
      
    elif tok.type == animation.Token.KEYWORD && tok.value == "repeat"
      self.next()  # skip 'repeat'
      var count = self.expect_number()
      self.expect_keyword("times")
      self.expect_colon()
      
      self.add(f"  for repeat_i : 0..{count}-1")
      
      # Process repeat body
      while !self.at_end() && !self.check_right_brace()
        var inner_tok = self.current()
        if inner_tok == nil || inner_tok.type == animation.Token.EOF
          break
        end
        if inner_tok.type == animation.Token.COMMENT
          self.add("    " + inner_tok.value)  # Add comment with repeat body indentation
          self.next()
          continue
        end
        if inner_tok.type == animation.Token.NEWLINE
          self.next()
          continue
        end
        if inner_tok.type == animation.Token.KEYWORD && inner_tok.value == "play"
          self.next()  # skip 'play'
          var anim_name = self.expect_identifier()
          
          var duration = "0"
          if self.current() != nil && self.current().type == animation.Token.KEYWORD && self.current().value == "for"
            self.next()  # skip 'for'
            duration = str(self.process_time_value())
          end
          
          var inline_comment = self.collect_inline_comment()
          self.add(f"    steps.push(animation.create_play_step(animation.global('{anim_name}_'), {duration})){inline_comment}")
          
        elif inner_tok.type == animation.Token.KEYWORD && inner_tok.value == "wait"
          self.next()  # skip 'wait'
          var duration = self.process_time_value()
          var inline_comment = self.collect_inline_comment()
          self.add(f"    steps.push(animation.create_wait_step({duration})){inline_comment}")
        else
          break  # Exit repeat body
        end
      end
      
      self.add(f"  end")
    else
      self.skip_statement()
    end
  end
  
  # Process run statement: run demo
  def process_run()
    self.next()  # skip 'run'
    var name = self.expect_identifier()
    var inline_comment = self.collect_inline_comment()
    
    # Store run statement for later processing
    self.run_statements.push({
      "name": name,
      "comment": inline_comment
    })
  end
  
  # Process property assignment: animation_name.property = value
  def process_property_assignment()
    var object_name = self.expect_identifier()
    
    # Check if next token is a dot
    if self.current() != nil && self.current().type == animation.Token.DOT
      self.next()  # skip '.'
      var property_name = self.expect_identifier()
      self.expect_assign()
      var value = self.process_value("property")
      var inline_comment = self.collect_inline_comment()
      
      # Generate property assignment: animation.global('object_name_').property = value
      self.add(f"animation.global('{object_name}_').{property_name} = {value}{inline_comment}")
    else
      # Not a property assignment, skip this statement
      self.error(f"Expected property assignment for '{object_name}' but found no dot")
      self.skip_statement()
    end
  end
  
  # Process any value - unified approach
  def process_value(context)
    var tok = self.current()
    if tok == nil
      self.error("Expected value")
      return "nil"
    end
    
    # Handle unary minus for negative numbers
    if tok.type == animation.Token.MINUS
      self.next()  # consume the minus
      var next_tok = self.current()
      if next_tok != nil && next_tok.type == animation.Token.NUMBER
        var value = "-" + next_tok.value
        self.next()  # consume the number
        return value
      else
        self.error("Expected number after '-'")
        return "0"
      end
    end
    
    # Function call: identifier or easing keyword followed by '('
    if (tok.type == animation.Token.KEYWORD || tok.type == animation.Token.IDENTIFIER) && 
       self.peek() != nil && self.peek().type == animation.Token.LEFT_PAREN
      return self.process_function_call(context)
    end
    
    # Color value
    if tok.type == animation.Token.COLOR
      self.next()
      return self.convert_color(tok.value)
    end
    
    # Time value
    if tok.type == animation.Token.TIME
      return str(self.process_time_value())
    end
    
    # Percentage value
    if tok.type == animation.Token.PERCENTAGE
      return str(self.process_percentage_value())
    end
    
    # Number value
    if tok.type == animation.Token.NUMBER
      var value = tok.value
      self.next()
      return value
    end
    
    # String value
    if tok.type == animation.Token.STRING
      var value = tok.value
      self.next()
      return f'"{value}"'
    end
    
    # Array literal
    if tok.type == animation.Token.LEFT_BRACKET
      return self.process_array_literal()
    end
    
    # Identifier - could be color, pattern, animation, or variable
    if tok.type == animation.Token.IDENTIFIER
      var name = tok.value
      self.next()
      
      # Check for palette constants
      import string
      if string.startswith(name, "PALETTE_")
        return f"animation.{name}"
      end
      
      # Check if it's a named color
      if animation.is_color_name(name)
        return self.get_named_color_value(name)
      end
      
      # Use underscore suffix for DSL variables to avoid conflicts
      return f"animation.global('{name}_', '{name}')"
    end
    
    # Boolean keywords
    if tok.type == animation.Token.KEYWORD && (tok.value == "true" || tok.value == "false")
      var value = tok.value
      self.next()
      return value
    end
    
    # Handle keywords that should be treated as identifiers (not sure this actually happens)
    if tok.type == animation.Token.KEYWORD
      var name = tok.value
      self.next()
      return f"animation.{name}"
    end
    
    self.error(f"Unexpected value: {tok.value}")
    self.next()
    return "nil"
  end
  
  # Process function call
  def process_function_call(context)
    var tok = self.current()
    var func_name = ""
    
    # Handle both identifiers and keywords as function names
    if tok != nil && (tok.type == animation.Token.IDENTIFIER || tok.type == animation.Token.KEYWORD)
      func_name = tok.value
      self.next()
    else
      self.error("Expected function name")
      return "nil"
    end
    
    var args = self.process_function_arguments()
    
    # Check if it's a user-defined function first
    if animation.is_user_function(func_name)
      return f"animation.get_user_function('{func_name}')({args})"
    else
      # All functions are resolved from the animation module
      # No context-specific mapping needed with unified architecture
      return f"animation.{func_name}({args})"
    end
  end
  
  # Process time value - simplified
  def process_time_value()
    var tok = self.current()
    if tok != nil && tok.type == animation.Token.TIME
      var time_str = tok.value
      self.next()
      return self.convert_time_to_ms(time_str)
    elif tok != nil && tok.type == animation.Token.NUMBER
      var num = tok.value
      self.next()
      return int(real(num)) * 1000  # assume seconds
    else
      self.error("Expected time value")
      return 1000
    end
  end
  
  # Process percentage value - simplified
  def process_percentage_value()
    var tok = self.current()
    if tok != nil && tok.type == animation.Token.PERCENTAGE
      var percent_str = tok.value
      self.next()
      var percent = real(percent_str[0..-2])
      return int(percent * 255 / 100)
    elif tok != nil && tok.type == animation.Token.NUMBER
      var num = tok.value
      self.next()
      return int(real(num))
    else
      self.error("Expected percentage value")
      return 255
    end
  end
  
  # Helper methods
  def current()
    return self.pos < size(self.tokens) ? self.tokens[self.pos] : nil
  end
  
  def peek()
    return (self.pos + 1 < size(self.tokens)) ? self.tokens[self.pos + 1] : nil
  end
  
  def next()
    if self.pos < size(self.tokens)
      self.pos += 1
    end
  end
  
  def at_end()
    return self.pos >= size(self.tokens) || 
           (self.current() != nil && self.current().type == animation.Token.EOF)
  end
  
  def skip_whitespace()
    while !self.at_end()
      var tok = self.current()
      if tok != nil && (tok.type == animation.Token.NEWLINE || tok.type == animation.Token.COMMENT)
        self.next()
      else
        break
      end
    end
  end
  
  # Collect inline comment if present and return it formatted for Berry code
  def collect_inline_comment()
    var tok = self.current()
    if tok != nil && tok.type == animation.Token.COMMENT
      var comment = "  " + tok.value  # Add spacing before comment
      self.next()
      return comment
    end
    return ""  # No comment found
  end
  
  def expect_identifier()
    var tok = self.current()
    if tok != nil && (tok.type == animation.Token.IDENTIFIER || 
                      tok.type == animation.Token.COLOR ||
                      (tok.type == animation.Token.KEYWORD && self.can_use_as_identifier(tok.value)))
      var name = tok.value
      self.next()
      return name
    else
      self.error("Expected identifier")
      return "unknown"
    end
  end
  
  def can_use_as_identifier(keyword)
    # Keywords that can be used as identifiers in variable contexts
    var identifier_keywords = [
      "opacity", "offset", "speed", "weight", "brightness", "duration", 
      "count", "length", "width", "height", "size", "scale",
      # Event names that can be used as identifiers
      "startup", "shutdown", "button_press", "button_hold", "motion_detected",
      "brightness_change", "timer", "time", "sound_peak", "network_message"
    ]
    
    for kw : identifier_keywords
      if keyword == kw
        return true
      end
    end
    return false
  end
  
  # Process function arguments
  def process_function_arguments()
    self.expect_left_paren()
    var args = []
    
    while !self.at_end() && !self.check_right_paren()
      self.skip_whitespace()
      
      if self.check_right_paren()
        break
      end
      
      var arg = self.process_value("argument")
      args.push(arg)
      
      self.skip_whitespace()
      
      if self.current() != nil && self.current().type == animation.Token.COMMA
        self.next()  # skip comma
        self.skip_whitespace()
      elif !self.check_right_paren()
        self.error("Expected ',' or ')' in function arguments")
        break
      end
    end
    
    self.expect_right_paren()
    
    # Join arguments with commas
    var result = ""
    for i : 0..size(args)-1
      if i > 0
        result += ", "
      end
      result += args[i]
    end
    return result
  end
  
  def expect_assign()
    var tok = self.current()
    if tok != nil && tok.type == animation.Token.ASSIGN
      self.next()
    else
      self.error("Expected '='")
    end
  end
  
  def expect_left_paren()
    var tok = self.current()
    if tok != nil && tok.type == animation.Token.LEFT_PAREN
      self.next()
    else
      self.error("Expected '('")
    end
  end
  
  def expect_right_paren()
    var tok = self.current()
    if tok != nil && tok.type == animation.Token.RIGHT_PAREN
      self.next()
    else
      self.error("Expected ')'")
    end
  end
  
  def check_right_paren()
    var tok = self.current()
    return tok != nil && tok.type == animation.Token.RIGHT_PAREN
  end
  
  def expect_comma()
    var tok = self.current()
    if tok != nil && tok.type == animation.Token.COMMA
      self.next()
    else
      self.error("Expected ','")
    end
  end
  
  def expect_left_brace()
    var tok = self.current()
    if tok != nil && tok.type == animation.Token.LEFT_BRACE
      self.next()
    else
      self.error("Expected '{'")
    end
  end
  
  def expect_right_brace()
    var tok = self.current()
    if tok != nil && tok.type == animation.Token.RIGHT_BRACE
      self.next()
    else
      self.error("Expected '}'")
    end
  end
  
  def check_right_brace()
    var tok = self.current()
    return tok != nil && tok.type == animation.Token.RIGHT_BRACE
  end
  
  def expect_number()
    var tok = self.current()
    if tok != nil && tok.type == animation.Token.NUMBER
      var value = tok.value
      self.next()
      return value
    else
      self.error("Expected number")
      return "0"
    end
  end
  
  def expect_keyword(keyword)
    var tok = self.current()
    if tok != nil && tok.type == animation.Token.KEYWORD && tok.value == keyword
      self.next()
    else
      self.error(f"Expected '{keyword}'")
    end
  end
  
  def expect_colon()
    var tok = self.current()
    if tok != nil && tok.type == animation.Token.COLON
      self.next()
    else
      self.error("Expected ':'")
    end
  end
  
  def expect_left_bracket()
    var tok = self.current()
    if tok != nil && tok.type == animation.Token.LEFT_BRACKET
      self.next()
    else
      self.error("Expected '['")
    end
  end
  
  def expect_right_bracket()
    var tok = self.current()
    if tok != nil && tok.type == animation.Token.RIGHT_BRACKET
      self.next()
    else
      self.error("Expected ']'")
    end
  end
  
  def check_right_bracket()
    var tok = self.current()
    return tok != nil && tok.type == animation.Token.RIGHT_BRACKET
  end
  

  
  # Process array literal [item1, item2, item3]
  def process_array_literal()
    self.expect_left_bracket()
    var items = []
    
    while !self.at_end() && !self.check_right_bracket()
      # Process array element
      var item = self.process_value("array_element")
      items.push(item)
      
      if self.current() != nil && self.current().type == animation.Token.COMMA
        self.next()  # skip comma
      elif !self.check_right_bracket()
        self.error("Expected ',' or ']' in array literal")
        break
      end
    end
    
    self.expect_right_bracket()
    
    # Join items with commas and wrap in brackets
    var result = "["
    for i : 0..size(items)-1
      if i > 0
        result += ", "
      end
      result += items[i]
    end
    result += "]"
    return result
  end
  
  def skip_statement()
    # Skip to next statement (newline or EOF)
    while !self.at_end()
      var tok = self.current()
      if tok.type == animation.Token.NEWLINE || tok.type == animation.Token.EOF
        break
      end
      self.next()
    end
  end
  
  # Conversion helpers
  def convert_color(color_str)
    import string
    # Handle 0x hex colors (new format)
    if string.startswith(color_str, "0x")
      if size(color_str) == 10  # 0xAARRGGBB (with alpha channel)
        return color_str
      elif size(color_str) == 8   # 0xRRGGBB (without alpha channel - add opaque alpha)
        return f"0xFF{color_str[2..]}"
      end
    end
    
    # Handle legacy # hex colors (for backward compatibility during transition)
    if string.startswith(color_str, "#")
      if size(color_str) == 9  # #AARRGGBB (with alpha channel)
        return f"0x{color_str[1..]}"
      elif size(color_str) == 7  # #RRGGBB (without alpha channel - add opaque alpha)
        return f"0xFF{color_str[1..]}"
      elif size(color_str) == 5  # #ARGB (short form with alpha)
        var a = color_str[1]
        var r = color_str[2]
        var g = color_str[3] 
        var b = color_str[4]
        return f"0x{a}{a}{r}{r}{g}{g}{b}{b}"
      elif size(color_str) == 4  # #RGB (short form without alpha - add opaque alpha)
        var r = color_str[1]
        var g = color_str[2] 
        var b = color_str[3]
        return f"0xFF{r}{r}{g}{g}{b}{b}"
      end
    end
    
    # Handle named colors - use framework's color name system
    if animation.is_color_name(color_str)
      return self.get_named_color_value(color_str)
    end
    
    # Unknown color - return white as default
    return "0xFFFFFFFF"
  end
  
  # Get the ARGB value for a named color
  # This should match the colors supported by is_color_name()
  def get_named_color_value(color_name)
    return animation.SimpleDSLTranspiler.named_colors.find(color_name, "0xFFFFFFFF")
  end
  
  # Validate that a user-defined name is not a predefined color
  def validate_user_name(name, definition_type)
    # Check if it's a predefined color name
    for color : animation.Token.color_names
      if name == color
        self.error(f"Cannot redefine predefined color '{name}'. Use a different name like '{name}_custom' or 'my_{name}'")
        return false
      end
    end
    
    return true
  end
  
  # Convert palette entry to VRGB format (Value, Red, Green, Blue)
  # Used by palette definitions to create Berry bytes objects
  #
  # @param value: string - palette position value (0-255)
  # @param color: string - color value (hex format like "0xFFRRGGBB")
  # @return string - 8-character hex string in VRGB format
  def convert_to_vrgb(value, color)
    import string
    
    # Convert value to hex (2 digits)
    var val_int = int(real(value))
    if val_int < 0
      val_int = 0
    elif val_int > 255
      val_int = 255
    end
    var val_hex = string.format("%02X", val_int)
    
    # Extract RGB from color
    var color_str = str(color)
    var rgb_hex = "FFFFFF"  # Default to white
    
    if string.startswith(color_str, "0x") && size(color_str) >= 10
      # Extract RGB components (skip alpha channel)
      # Format is "0xAARRGGBB", we want "RRGGBB"
      rgb_hex = color_str[4..9]  # Skip "0xAA" to get "RRGGBB"
    elif string.startswith(color_str, "0x") && size(color_str) == 8
      # Format is "0xRRGGBB", we want "RRGGBB"  
      rgb_hex = color_str[2..7]  # Skip "0x" to get "RRGGBB"
    end
    
    return val_hex + rgb_hex  # VRRGGBB format
  end
  
  def convert_time_to_ms(time_str)
    import string
    if string.endswith(time_str, "ms")
      return int(real(time_str[0..-3]))
    elif string.endswith(time_str, "s")
      return int(real(time_str[0..-2]) * 1000)
    elif string.endswith(time_str, "m")
      return int(real(time_str[0..-2]) * 60000)
    elif string.endswith(time_str, "h")
      return int(real(time_str[0..-2]) * 3600000)
    end
    return 1000
  end
  
  def add(line)
    self.output.push(line)
  end
  
  def join_output()
    var result = ""
    for line : self.output
      result += line + "\n"
    end
    return result
  end
  
  def error(msg)
    var line = self.current() != nil ? self.current().line : 0
    self.errors.push(f"Line {line}: {msg}")
  end
  
  def get_errors()
    return self.errors
  end
  
  def has_errors()
    return size(self.errors) > 0
  end
  
  def get_error_report()
    if !self.has_errors()
      return "No compilation errors"
    end
    
    var report = "Compilation errors:\n"
    for error : self.errors
      report += "  " + error + "\n"
    end
    return report
  end
  
  # Generate single engine.start() call for all run statements
  def generate_engine_start()
    if size(self.run_statements) == 0
      return  # No run statements, no need to start engine
    end
    
    self.add("# Start all animations/sequences")
    
    # Add all animations/sequences to the engine
    for run_stmt : self.run_statements
      var name = run_stmt["name"]
      var comment = run_stmt["comment"]
      
      # Check what exists: sequence function or animation variable
      self.add(f"if global.contains('sequence_{name}'){comment}")
      self.add(f"  var seq_manager = global.sequence_{name}()")
      self.add(f"  engine.add_sequence_manager(seq_manager)")
      self.add(f"else")
      self.add(f"  engine.add_animation(animation.global('{name}_'))")
      self.add(f"end")
    end
    
    # Single engine.start() call
    self.add("engine.start()")
  end

  # Basic event handler processing
  def process_event_handler()
    self.next()  # skip 'on'
    var event_name = self.expect_identifier()
    var line = self.current() != nil ? self.current().line : 0
    
    # Check for event parameters (e.g., timer(5s))
    var event_params = "{}"
    if self.current() != nil && self.current().type == animation.Token.LEFT_PAREN
      event_params = self.process_event_parameters()
    end
    
    self.expect_colon()
    
    # Generate unique handler function name
    var handler_name = f"event_handler_{event_name}_{line}"
    
    # Start generating the event handler function
    self.add(f"def {handler_name}(event_data)")
    
    # Process the event action - simple function call or identifier
    var tok = self.current()
    if tok != nil
      if tok.type == animation.Token.KEYWORD && tok.value == "interrupt"
        self.next()  # skip 'interrupt'
        var target = self.expect_identifier()
        if target == "current"
          self.add("  engine.interrupt_current()")
        else
          self.add(f"  engine.interrupt_animation(\"{target}\")")
        end
      else
        # Assume it's an animation function call or reference
        var action = self.process_value("animation")
        self.add(f"  var temp_anim = {action}")
        self.add(f"  engine.add_animation(temp_anim)")
      end
    end
    
    self.add("end")
    
    # Register the event handler
    self.add(f"animation.register_event_handler(\"{event_name}\", {handler_name}, 0, nil, {event_params})")
  end
  
  # Process event parameters: timer(5s) -> {"interval": 5000}
  def process_event_parameters()
    self.expect_left_paren()
    var params = "{"
    
    # For timer events, convert time to milliseconds
    if !self.at_end() && !self.check_right_paren()
      var tok = self.current()
      if tok != nil && tok.type == animation.Token.TIME
        var time_ms = self.process_time_value()
        params += f"\"interval\": {time_ms}"
      else
        var value = self.process_value("event_param")
        params += f"\"value\": {value}"
      end
    end
    
    self.expect_right_paren()
    params += "}"
    return params
  end
  
  # Generate default strip initialization using Tasmota configuration
  def generate_default_strip_initialization()
    if self.strip_initialized
      return  # Already initialized, don't duplicate
    end
    
    self.add("# Auto-generated strip initialization (using Tasmota configuration)")
    self.add("var strip = global.Leds()  # Get strip length from Tasmota configuration")
    self.add("var engine = animation.create_engine(strip)")
    self.add("")
    self.strip_initialized = true
  end

end

# DSL compilation function
def compile_dsl(source)
  var lexer = animation.DSLLexer(source)
  var tokens = lexer.tokenize()
  
  if lexer.has_errors()
    var error_msg = "DSL Lexer errors:\n"
    for error : lexer.get_errors()
      error_msg += "  " + error + "\n"
    end
    raise "dsl_compilation_error", error_msg
  end
  
  var transpiler = animation.SimpleDSLTranspiler(tokens)
  var berry_code = transpiler.transpile()
  
  if transpiler.has_errors()
    var error_msg = "DSL Transpiler errors:\n"
    for error : transpiler.get_errors()
      error_msg += "  " + error + "\n"
    end
    raise "dsl_compilation_error", error_msg
  end
  
  return berry_code
end

# Return module exports
return {
  "SimpleDSLTranspiler": SimpleDSLTranspiler,
  "compile_dsl": compile_dsl,
}