# Ultra-Simplified DSL Transpiler for Animation DSL
# Single-pass transpiler with minimal complexity
# Leverages Berry's runtime for symbol resolution

# Mock engine class for parameter validation during transpilation
class MockEngine
  var time_ms
  
  def init()
    self.time_ms = 0
  end
  
  def get_strip_length()
    return 30  # Default strip length for validation
  end
end

#@ solidify:SimpleDSLTranspiler,weak
class SimpleDSLTranspiler
  var tokens          # Token stream from lexer
  var pos             # Current token position
  var output          # Generated Berry code lines
  var errors          # Compilation errors
  var run_statements  # Collect all run statements for single engine.run()
  var first_statement # Track if we're processing the first statement
  var strip_initialized # Track if strip was initialized
  var sequence_names  # Track which names are sequences
  var symbol_table    # Track created objects: name -> instance
  var indent_level    # Track current indentation level for nested sequences
  var template_definitions  # Track template definitions: name -> {params, body}
  var has_template_calls    # Track if we have template calls to trigger engine.run()
  
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
    self.sequence_names = {}  # Track which names are sequences
    self.symbol_table = {}  # Track created objects: name -> instance
    self.indent_level = 0  # Track current indentation level
    self.template_definitions = {}  # Track template definitions
    self.has_template_calls = false  # Track if we have template calls
  end
  
  # Get current indentation string
  def get_indent()
    return "  " * (self.indent_level + 1)  # Base indentation is 2 spaces
  end
  
  # Helper method to process user function calls (user.function_name())
  def _process_user_function_call(func_name)
    # Check if this is a function call (user.function_name())
    if self.current() != nil && self.current().type == animation_dsl.Token.LEFT_PAREN
      # This is a user function call: user.function_name()
      # Don't check for existence during transpilation - trust that function will be available at runtime
      
      # User functions use positional parameters with engine as first argument
      # In closure context, use engine parameter directly
      var args = self.process_function_arguments(true)
      var full_args = args != "" ? f"engine, {args}" : "engine"
      return f"animation.get_user_function('{func_name}')({full_args})"
    else
      self.error("User functions must be called with parentheses: user.function_name()")
      return "nil"
    end
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
      
      # Generate single engine.run() call after all run statements
      self.generate_engine_run()
      
      return size(self.errors) == 0 ? self.join_output() : nil
    except .. as e, msg
      self.error(f"Transpilation failed: {msg}")
      return nil
    end
  end
  
  # Transpile template body (similar to main transpile but without imports/engine start)
  def transpile_template_body()
    try
      # Process all statements in template body
      while !self.at_end()
        self.process_statement()
      end
      
      # For templates, process run statements immediately instead of collecting them
      if size(self.run_statements) > 0
        for run_stmt : self.run_statements
          var obj_name = run_stmt["name"]
          var comment = run_stmt["comment"]
          # In templates, use underscore suffix for local variables
          self.add(f"engine.add({obj_name}_){comment}")
        end
      end
      
      return size(self.errors) == 0 ? self.join_output() : nil
    except .. as e, msg
      self.error(f"Template body transpilation failed: {msg}")
      return nil
    end
  end
  
  # Process statements - simplified approach
  def process_statement()
    var tok = self.current()
    if tok == nil || tok.type == animation_dsl.Token.EOF
      return
    end
    
    # Handle comments - preserve them in generated code
    if tok.type == animation_dsl.Token.COMMENT
      self.add(tok.value)  # Add comment as-is to output
      self.next()
      return
    end
    
    # Skip whitespace (newlines)
    if tok.type == animation_dsl.Token.NEWLINE
      self.next()
      return
    end
    
    # Check if this is the first non-comment, non-whitespace statement
    var is_first_real_statement = self.first_statement
    if tok.type == animation_dsl.Token.KEYWORD || tok.type == animation_dsl.Token.IDENTIFIER
      self.first_statement = false
    end
    
    # Handle keywords
    if tok.type == animation_dsl.Token.KEYWORD
      if tok.value == "strip"
        # Strip directive is temporarily disabled but remains a reserved keyword
        self.error("'strip' directive is temporarily disabled. Strip configuration is handled automatically.")
        self.skip_statement()
        return
      else
        # For any other statement, ensure strip is initialized
        if !self.strip_initialized
          self.generate_default_strip_initialization()
        end
        
        if tok.value == "color"
          self.process_color()
        elif tok.value == "palette"
          self.process_palette()
        elif tok.value == "animation"
          self.process_animation()
        elif tok.value == "set"
          self.process_set()
        elif tok.value == "sequence"
          self.process_sequence()
        elif tok.value == "template"
          self.process_template()
        elif tok.value == "run"
          self.process_run()
        elif tok.value == "import"
          self.process_import()
        elif tok.value == "on"
          self.process_event_handler()
        else
          self.skip_statement()
        end
      end
    elif tok.type == animation_dsl.Token.IDENTIFIER
      # For property assignments, ensure strip is initialized
      if !self.strip_initialized
        self.generate_default_strip_initialization()
      end
      
      # Check if this is a log function call
      if tok.value == "log" && self.peek() != nil && self.peek().type == animation_dsl.Token.LEFT_PAREN
        self.process_standalone_log()
      else
        # Check if this is a property assignment (identifier.property = value)
        self.process_property_assignment()
      end
    else
      self.skip_statement()
    end
  end
  
  # Process color definition: color red = #FF0000 or color cycle_red = color_cycle(palette=[red, blue])
  def process_color()
    self.next()  # skip 'color'
    var name = self.expect_identifier()
    
    # Validate that the color name is not reserved
    if !self.validate_user_name(name, "color")
      self.skip_statement()
      return
    end
    
    self.expect_assign()
    
    # Check if this is a function call with named arguments (color provider)
    var tok = self.current()
    if (tok.type == animation_dsl.Token.KEYWORD || tok.type == animation_dsl.Token.IDENTIFIER) && 
       self.peek() != nil && self.peek().type == animation_dsl.Token.LEFT_PAREN
      
      # This is a function call - check if it's a user function or built-in color provider
      var func_name = tok.value
      self.next()  # consume function name
      
      var inline_comment = ""
      # Check for inline comment before opening paren
      if self.current() != nil && self.current().type == animation_dsl.Token.COMMENT
        inline_comment = "  " + self.current().value
        self.next()
      end
      
      # Check if this is a template call first
      if self.template_definitions.contains(func_name)
        # This is a template call - treat like user function
        var args = self.process_function_arguments(false)
        var full_args = args != "" ? f"engine, {args}" : "engine"
        self.add(f"{func_name}_template({full_args}){inline_comment}")
      else
        # Built-in functions use the new engine-first + named parameters pattern
        # Validate that the factory function exists at transpilation time
        if !self._validate_color_provider_factory_exists(func_name)
          self.error(f"Color provider factory function '{func_name}' does not exist. Check the function name and ensure it's available in the animation module.")
          self.skip_statement()
          return
        end
        
        # Generate the base function call immediately
        self.add(f"var {name}_ = animation.{func_name}(engine){inline_comment}")
        
        # Track this symbol in our symbol table
        var instance = self._create_instance_for_validation(func_name)
        if instance != nil
          self.symbol_table[name] = instance
        end
        
        # Process named arguments with validation
        self._process_named_arguments_for_color_provider(f"{name}_", func_name)
      end
    else
      # Check if this is a simple identifier reference before processing
      var current_tok = self.current()
      var is_simple_identifier = (current_tok != nil && current_tok.type == animation_dsl.Token.IDENTIFIER && 
                                  (self.peek() == nil || self.peek().type != animation_dsl.Token.LEFT_PAREN))
      var ref_name = is_simple_identifier ? current_tok.value : nil
      
      # Regular value assignment (simple color value)
      var value = self.process_value("color")
      var inline_comment = self.collect_inline_comment()
      self.add(f"var {name}_ = {value}{inline_comment}")
      
      # If this is an identifier reference to another color provider in our symbol table,
      # add this name to the symbol table as well for compile-time validation
      if is_simple_identifier && ref_name != nil && self.symbol_table.contains(ref_name)
        var ref_instance = self.symbol_table[ref_name]
        # Only copy instances, not sequence markers
        if type(ref_instance) != "string"
          self.symbol_table[name] = ref_instance
        end
      else
        # Add simple color to symbol table with a marker
        self.symbol_table[name] = "color"
      end
    end
  end
  
  # Process palette definition: palette aurora_colors = [(0, #000022), (64, #004400), ...] or [red, 0x008000, blue, 0x112233]
  def process_palette()
    self.next()  # skip 'palette'
    var name = self.expect_identifier()
    
    # Validate that the palette name is not reserved
    if !self.validate_user_name(name, "palette")
      self.skip_statement()
      return
    end
    
    self.expect_assign()
    
    # Expect array literal
    self.expect_left_bracket()
    var palette_entries = []
    var palette_comments = []  # Store comments for each entry
    
    # Detect syntax type by looking at the first entry
    self.skip_whitespace_including_newlines()
    
    if self.check_right_bracket()
      # Empty palette - not allowed
      self.error("Empty palettes are not allowed. A palette must contain at least one color entry.")
      self.skip_statement()
      return
    end
    
    # Check if first entry starts with '(' (tuple syntax) or not (alternative syntax)
    var is_tuple_syntax = self.current() != nil && self.current().type == animation_dsl.Token.LEFT_PAREN
    
    while !self.at_end() && !self.check_right_bracket()
      self.skip_whitespace_including_newlines()
      
      if self.check_right_bracket()
        break
      end
      
      if is_tuple_syntax
        # Parse tuple (value, color) - original syntax
        # Check if we accidentally have alternative syntax in tuple mode
        if self.current() != nil && self.current().type != animation_dsl.Token.LEFT_PAREN
          self.error("Cannot mix alternative syntax [color1, color2, ...] with tuple syntax (value, color). Use only one syntax per palette.")
          self.skip_statement()
          return
        end
        
        self.expect_left_paren()
        var value = self.expect_number()
        self.expect_comma()
        var color = self.process_palette_color()  # Use specialized palette color processing
        self.expect_right_paren()
        
        # Convert to VRGB format entry and store as integer
        var vrgb_entry = self.convert_to_vrgb(value, color)
        var vrgb_int = int(f"0x{vrgb_entry}")
        palette_entries.push(vrgb_int)
      else
        # Parse color only - alternative syntax
        # Check if we accidentally have a tuple in alternative syntax mode
        if self.current() != nil && self.current().type == animation_dsl.Token.LEFT_PAREN
          self.error("Cannot mix tuple syntax (value, color) with alternative syntax [color1, color2, ...]. Use only one syntax per palette.")
          self.skip_statement()
          return
        end
        
        var color = self.process_palette_color()  # Use specialized palette color processing

        # Convert to VRGB format entry and store as integer after setting alpha to 0xFF
        var vrgb_entry = self.convert_to_vrgb(0xFF, color)
        var vrgb_int = int(f"0x{vrgb_entry}")
        palette_entries.push(vrgb_int)
      end
      
      # Check for entry separator: comma OR newline OR end of palette
      # Also collect any comment that comes after the separator
      var entry_comment = ""
      
      if self.current() != nil && self.current().type == animation_dsl.Token.COMMA
        self.next()  # skip comma
        
        # Check for comment immediately after comma
        if self.current() != nil && self.current().type == animation_dsl.Token.COMMENT
          entry_comment = self.current().value
          self.next()
        end
        
        # Skip remaining whitespace/newlines
        while !self.at_end()
          var tok = self.current()
          if tok != nil && tok.type == animation_dsl.Token.NEWLINE
            self.next()
          else
            break
          end
        end
      elif self.current() != nil && self.current().type == animation_dsl.Token.NEWLINE
        # Newline acts as entry separator - skip it and continue
        self.next()  # skip newline
        self.skip_whitespace_including_newlines()
      elif !self.check_right_bracket()
        # For the last entry, check if there's a comment before the closing bracket
        if self.current() != nil && self.current().type == animation_dsl.Token.COMMENT
          entry_comment = self.current().value
          self.next()
        elif !self.check_right_bracket()
          self.error("Expected ',' or ']' in palette definition")
          break
        end
      end
      
      palette_comments.push(entry_comment)  # Store comment (empty string if no comment)
    end
    
    self.expect_right_bracket()
    var inline_comment = self.collect_inline_comment()
    
    # Generate Berry bytes object with comments preserved
    # Check if we have any comments to preserve
    var has_comments = false
    for comment : palette_comments
      if comment != ""
        has_comments = true
        break
      end
    end
    
    if has_comments
      # Multi-line format with comments
      self.add(f"var {name}_ = bytes({inline_comment}")
      for i : 0..size(palette_entries)-1
        var hex_str = format("%08X", palette_entries[i])
        var comment = palette_comments[i]
        var comment_suffix = comment != "" ? f"  {comment}" : ""
        self.add(f"  \"{hex_str}\"{comment_suffix}")
      end
      self.add(")")
    else
      # Single-line format (original behavior when no comments)
      var palette_data = ""
      for i : 0..size(palette_entries)-1
        if i > 0
          palette_data += " "
        end
        # Convert integer back to hex string for bytes() constructor
        var hex_str = format("%08X", palette_entries[i])
        palette_data += f'"{hex_str}"'
      end
      
      self.add(f"var {name}_ = bytes({palette_data}){inline_comment}")
    end
  end
  
  # Process animation definition: animation pulse_red = pulse(color=red, period=2s)
  def process_animation()
    self.next()  # skip 'animation'
    var name = self.expect_identifier()
    
    # Validate that the animation name is not reserved
    if !self.validate_user_name(name, "animation")
      self.skip_statement()
      return
    end
    
    self.expect_assign()
    
    # Check if this is a function call with named arguments
    var tok = self.current()
    if (tok.type == animation_dsl.Token.KEYWORD || tok.type == animation_dsl.Token.IDENTIFIER) && 
       self.peek() != nil && self.peek().type == animation_dsl.Token.LEFT_PAREN
      
      # This is a function call - check if it's a user function or built-in
      var func_name = tok.value
      self.next()  # consume function name
      
      var inline_comment = ""
      # Check for inline comment before opening paren
      if self.current() != nil && self.current().type == animation_dsl.Token.COMMENT
        inline_comment = "  " + self.current().value
        self.next()
      end
      
      # Check if this is a template call first
      if self.template_definitions.contains(func_name)
        # This is a template call - treat like user function
        var args = self.process_function_arguments(false)
        var full_args = args != "" ? f"engine, {args}" : "engine"
        self.add(f"{func_name}_template({full_args}){inline_comment}")
      else
        # Built-in functions use the new engine-first + named parameters pattern
        # Validate that the factory function creates an animation instance at transpile time
        if !self._validate_animation_factory_creates_animation(func_name)
          self.error(f"Animation factory function '{func_name}' does not exist or does not create an instance of animation.animation class. Check the function name and ensure it returns an animation object.")
          self.skip_statement()
          return
        end
        
        # Generate the base function call immediately
        self.add(f"var {name}_ = animation.{func_name}(engine){inline_comment}")
        
        # Track this symbol in our symbol table
        var instance = self._create_instance_for_validation(func_name)
        if instance != nil
          self.symbol_table[name] = instance
        end
        
        # Process named arguments with validation
        self._process_named_arguments_for_animation(f"{name}_", func_name)
      end
    else
      # Check if this is a simple identifier reference before processing
      var current_tok = self.current()
      var is_simple_identifier = (current_tok != nil && current_tok.type == animation_dsl.Token.IDENTIFIER && 
                                  (self.peek() == nil || self.peek().type != animation_dsl.Token.LEFT_PAREN))
      var ref_name = is_simple_identifier ? current_tok.value : nil
      
      # Regular value assignment (identifier, color, etc.)
      var value = self.process_value("animation")
      var inline_comment = self.collect_inline_comment()
      self.add(f"var {name}_ = {value}{inline_comment}")
      
      # If this is an identifier reference to another animation in our symbol table,
      # add this name to the symbol table as well for compile-time validation
      if is_simple_identifier && ref_name != nil && self.symbol_table.contains(ref_name)
        var ref_instance = self.symbol_table[ref_name]
        # Only copy instances, not sequence markers
        if type(ref_instance) != "string"
          self.symbol_table[name] = ref_instance
        end
      else
        # Add simple animation to symbol table with a marker
        self.symbol_table[name] = "animation"
      end
    end
  end
  
  # Process strip configuration: strip length 60
  # Temporarily disabled
  # def process_strip()
  #   self.next()  # skip 'strip'
  #   var prop = self.expect_identifier()
  #   if prop == "length"
  #     var length = self.expect_number()
  #     var inline_comment = self.collect_inline_comment()
  #     self.add(f"var engine = animation.init_strip({length}){inline_comment}")
  #     self.strip_initialized = true  # Mark that strip was initialized
  #   end
  # end
  
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
    
    # Check if this is a value provider function call
    var is_value_provider = false
    var tok = self.current()
    if (tok.type == animation_dsl.Token.KEYWORD || tok.type == animation_dsl.Token.IDENTIFIER) && 
       self.peek() != nil && self.peek().type == animation_dsl.Token.LEFT_PAREN
      
      var func_name = tok.value
      # Check if this is a value provider factory function
      if self._validate_value_provider_factory_exists(func_name)
        is_value_provider = true
      end
    end
    
    var value = self.process_value("variable")
    var inline_comment = self.collect_inline_comment()
    self.add(f"var {name}_ = {value}{inline_comment}")
    
    # Add to symbol table with appropriate marker
    if is_value_provider
      self.symbol_table[name] = "value_provider"
    else
      # Use a string marker to indicate this is a variable (not an instance)
      self.symbol_table[name] = "variable"
    end
  end
  
  # Process template definition: template name { param ... }
  def process_template()
    self.next()  # skip 'template'
    var name = self.expect_identifier()
    
    # Validate that the template name is not reserved
    if !self.validate_user_name(name, "template")
      self.skip_statement()
      return
    end
    
    self.expect_left_brace()
    
    # First pass: collect all parameters
    var params = []
    var param_types = {}
    
    while !self.at_end() && !self.check_right_brace()
      self.skip_whitespace_including_newlines()
      
      if self.check_right_brace()
        break
      end
      
      var tok = self.current()
      
      if tok != nil && tok.type == animation_dsl.Token.KEYWORD && tok.value == "param"
        # Process parameter declaration
        self.next()  # skip 'param'
        var param_name = self.expect_identifier()
        
        # Check for optional type annotation
        var param_type = nil
        if self.current() != nil && self.current().type == animation_dsl.Token.KEYWORD && self.current().value == "type"
          self.next()  # skip 'type'
          param_type = self.expect_identifier()
        end
        
        params.push(param_name)
        if param_type != nil
          param_types[param_name] = param_type
        end
        
        # Skip optional newline after parameter
        if self.current() != nil && self.current().type == animation_dsl.Token.NEWLINE
          self.next()
        end
      else
        # Found non-param statement, break to collect body
        break
      end
    end
    
    # Second pass: collect body tokens (everything until closing brace)
    var body_tokens = []
    var brace_depth = 0
    
    while !self.at_end()
      var tok = self.current()
      
      if tok == nil || tok.type == animation_dsl.Token.EOF
        break
      end
      
      if tok.type == animation_dsl.Token.LEFT_BRACE
        brace_depth += 1
        body_tokens.push(tok)
      elif tok.type == animation_dsl.Token.RIGHT_BRACE
        if brace_depth == 0
          break  # This is our closing brace
        else
          brace_depth -= 1
          body_tokens.push(tok)
        end
      else
        body_tokens.push(tok)
      end
      
      self.next()
    end
    
    self.expect_right_brace()
    
    # Store template definition
    self.template_definitions[name] = {
      'params': params,
      'param_types': param_types,
      'body_tokens': body_tokens
    }
    
    # Generate Berry function for this template
    self.generate_template_function(name, params, param_types, body_tokens)
    
    # Add template to symbol table as a special marker
    self.symbol_table[name] = "template"
  end
  
  # Process sequence definition: sequence demo { ... } or sequence demo repeat N times { ... }
  def process_sequence()
    self.next()  # skip 'sequence'
    var name = self.expect_identifier()
    
    # Validate that the sequence name is not reserved
    if !self.validate_user_name(name, "sequence")
      self.skip_statement()
      return
    end
    
    # Track that this name is a sequence
    self.sequence_names[name] = true
    
    # Also add to symbol table with a special marker for sequences
    # We use a string marker since sequences don't have real instances
    self.symbol_table[name] = "sequence"
    
    # Check for second syntax: sequence name repeat N times { ... } or sequence name forever { ... }
    var is_repeat_syntax = false
    var repeat_count = "1"
    
    var current_tok = self.current()
    if current_tok != nil && current_tok.type == animation_dsl.Token.KEYWORD
      if current_tok.value == "repeat"
        is_repeat_syntax = true
        self.next()  # skip 'repeat'
        
        # Parse repeat count: either number or "forever"
        var tok_after_repeat = self.current()
        if tok_after_repeat != nil && tok_after_repeat.type == animation_dsl.Token.KEYWORD && tok_after_repeat.value == "forever"
          self.next()  # skip 'forever'
          repeat_count = "-1"  # -1 means forever
        else
          var count_expr = self.process_value("repeat_count")
          self.expect_keyword("times")
          repeat_count = count_expr
        end
      elif current_tok.value == "forever"
        # New syntax: sequence name forever { ... } (repeat is optional)
        is_repeat_syntax = true
        self.next()  # skip 'forever'
        repeat_count = "-1"  # -1 means forever
      end
    elif current_tok != nil && current_tok.type == animation_dsl.Token.NUMBER
      # New syntax: sequence name N times { ... } (repeat is optional)
      is_repeat_syntax = true
      var count_expr = self.process_value("repeat_count")
      self.expect_keyword("times")
      repeat_count = count_expr
    end
    
    self.expect_left_brace()
    
    if is_repeat_syntax
      # Second syntax: sequence name repeat N times { ... }
      # Create a single SequenceManager with fluent interface
      self.add(f"var {name}_ = animation.SequenceManager(engine, {repeat_count})")
      
      # Process sequence body - add steps using fluent interface
      while !self.at_end() && !self.check_right_brace()
        self.process_sequence_statement()
      end
    else
      # First syntax: sequence demo { ... }
      # Use fluent interface for regular sequences too (no repeat count = default)
      self.add(f"var {name}_ = animation.SequenceManager(engine)")
      
      # Process sequence body - add steps using fluent interface
      while !self.at_end() && !self.check_right_brace()
        self.process_sequence_statement()
      end
    end
    
    self.expect_right_brace()
  end
  
  # Process statements inside sequences using fluent interface
  def process_sequence_statement()
    var tok = self.current()
    if tok == nil || tok.type == animation_dsl.Token.EOF
      return
    end
    
    # Handle comments - preserve them in generated code with proper indentation
    if tok.type == animation_dsl.Token.COMMENT
      self.add(self.get_indent() + tok.value)  # Add comment with fluent indentation
      self.next()
      return
    end
    
    # Skip whitespace (newlines)
    if tok.type == animation_dsl.Token.NEWLINE
      self.next()
      return
    end
    
    if tok.type == animation_dsl.Token.KEYWORD && tok.value == "play"
      self.process_play_statement_fluent()
      
    elif tok.type == animation_dsl.Token.KEYWORD && tok.value == "wait"
      self.process_wait_statement_fluent()
      
    elif tok.type == animation_dsl.Token.IDENTIFIER && tok.value == "log"
      self.process_log_statement_fluent()
      
    elif tok.type == animation_dsl.Token.KEYWORD && tok.value == "restart"
      self.process_restart_statement_fluent()
      
    elif tok.type == animation_dsl.Token.KEYWORD && tok.value == "reset"
      self.error("'reset' command is no longer supported. Use 'restart' instead.")
      self.skip_statement()
      
    elif tok.type == animation_dsl.Token.KEYWORD && tok.value == "repeat"
      self.next()  # skip 'repeat'
      
      # Parse repeat count: either number or "forever"
      var repeat_count = "1"
      var tok_after_repeat = self.current()
      if tok_after_repeat != nil && tok_after_repeat.type == animation_dsl.Token.KEYWORD && tok_after_repeat.value == "forever"
        self.next()  # skip 'forever'
        repeat_count = "-1"  # -1 means forever
      else
        var count_expr = self.process_value("repeat_count")
        self.expect_keyword("times")
        repeat_count = count_expr
      end
      
      self.expect_left_brace()
      
      # Create a nested sub-sequence using recursive processing
      self.add(f"{self.get_indent()}.push_repeat_subsequence(animation.SequenceManager(engine, {repeat_count})")
      
      # Increase indentation level for nested content
      self.indent_level += 1
      
      # Process repeat body recursively - just call the same method
      while !self.at_end() && !self.check_right_brace()
        self.process_sequence_statement()
      end
      
      self.expect_right_brace()
      
      # Decrease indentation level and close the sub-sequence
      self.add(f"{self.get_indent()})")
      self.indent_level -= 1
      
    elif tok.type == animation_dsl.Token.IDENTIFIER
      # Check if this is a property assignment (identifier.property = value)
      if self.peek() != nil && self.peek().type == animation_dsl.Token.DOT
        self.process_sequence_assignment_fluent()
      else
        # Unknown identifier in sequence - this is an error
        self.error(f"Unknown command '{tok.value}' in sequence. Valid sequence commands are: play, wait, repeat, restart, log, or property assignments (object.property = value)")
        self.skip_statement()
      end
    else
      # Unknown token type in sequence - this is an error
      self.error(f"Invalid statement in sequence. Expected: play, wait, repeat, restart, log, or property assignments")
      self.skip_statement()
    end
  end
  
  # Process property assignment using fluent style
  def process_sequence_assignment_fluent()
    var object_name = self.expect_identifier()
    self.expect_dot()
    var property_name = self.expect_identifier()
    self.expect_assign()
    var value = self.process_value("property")
    var inline_comment = self.collect_inline_comment()
    
    # Create assignment step using fluent style
    var closure_code = f"def (engine) {object_name}_.{property_name} = {value} end"
    self.add(f"{self.get_indent()}.push_closure_step({closure_code}){inline_comment}")
  end
  
  # Process property assignment inside sequences: object.property = value (legacy)
  def process_sequence_assignment(indent)
    self.process_sequence_assignment_generic(indent, "steps")
  end
  
  # Generic method to process sequence assignment with configurable target array
  def process_sequence_assignment_generic(indent, target_array)
    var object_name = self.expect_identifier()
    
    # Check if next token is a dot
    if self.current() != nil && self.current().type == animation_dsl.Token.DOT
      self.next()  # skip '.'
      var property_name = self.expect_identifier()
      
      # Validate parameter if we have this object in our symbol table
      if self.symbol_table.contains(object_name)
        var instance = self.symbol_table[object_name]
        
        # Only validate parameters for actual instances, not sequence markers
        if type(instance) != "string"
          var class_name = classname(instance)
          
          # Use the existing parameter validation logic
          self._validate_single_parameter(class_name, property_name, instance)
        else
          # This is a sequence marker - sequences don't have properties
          self.error(f"Sequences like '{object_name}' do not have properties. Property assignments are only valid for animations and color providers.")
          return
        end
      end
      
      self.expect_assign()
      var value = self.process_value("property")
      var inline_comment = self.collect_inline_comment()
      
      # Generate assignment step with closure
      # The closure receives the engine as parameter and performs the assignment
      var object_ref = self.resolve_symbol_reference(object_name)
      
      # Create closure that performs the assignment
      var closure_code = f"def (engine) {object_ref}.{property_name} = {value} end"
      self.add(f"{indent}{target_array}.push(animation.create_assign_step({closure_code})){inline_comment}")
    else
      # Not a property assignment, this shouldn't happen since we checked for dot
      self.error(f"Expected property assignment for '{object_name}' but found no dot")
      self.skip_statement()
    end
  end
  

  
  # Helper method to process play statement using fluent style
  def process_play_statement_fluent()
    self.next()  # skip 'play'
    
    # Check if this is a function call or an identifier
    var anim_ref = ""
    var current_tok = self.current()
    if current_tok != nil && (current_tok.type == animation_dsl.Token.IDENTIFIER || current_tok.type == animation_dsl.Token.KEYWORD) &&
       self.peek() != nil && self.peek().type == animation_dsl.Token.LEFT_PAREN
      # This is a function call - process it as a nested function call
      anim_ref = self.process_nested_function_call()
    else
      # This is an identifier reference
      var anim_name = self.expect_identifier()
      
      # Validate that the referenced object exists
      self._validate_object_reference(anim_name, "sequence play")
      
      anim_ref = f"{anim_name}_"
    end
    
    # Handle optional 'for duration'
    var duration = "nil"
    if self.current() != nil && self.current().type == animation_dsl.Token.KEYWORD && self.current().value == "for"
      self.next()  # skip 'for'
      duration = str(self.process_time_value())
    end
    
    var inline_comment = self.collect_inline_comment()
    self.add(f"{self.get_indent()}.push_play_step({anim_ref}, {duration}){inline_comment}")
  end
  
  # Helper method to process wait statement using fluent style
  def process_wait_statement_fluent()
    self.next()  # skip 'wait'
    var duration = self.process_time_value()
    var inline_comment = self.collect_inline_comment()
    self.add(f"{self.get_indent()}.push_wait_step({duration}){inline_comment}")
  end
  
  # Unified log processing method - handles all log contexts
  def process_log_call(args_str, context_type, inline_comment)
    # Convert DSL log("message") to Berry log(f"message", 3)
    if context_type == "fluent"
      # For sequence context - wrap in closure
      var closure_code = f"def (engine) log(f\"{args_str}\", 3) end"
      return f"{self.get_indent()}.push_closure_step({closure_code}){inline_comment}"
    elif context_type == "expression"
      # For expression context - return just the call (no inline comment)
      return f"log(f\"{args_str}\", 3)"
    else
      # For standalone context - direct call with comment
      return f"log(f\"{args_str}\", 3){inline_comment}"
    end
  end

  # Helper method to process log statement using fluent style
  def process_log_statement_fluent()
    self.next()  # skip 'log'
    self.expect_left_paren()
    
    # Process the message string
    var message_tok = self.current()
    if message_tok == nil || message_tok.type != animation_dsl.Token.STRING
      self.error("log() function requires a string message")
      self.skip_statement()
      return
    end
    
    var message = message_tok.value
    self.next()  # consume string
    self.expect_right_paren()
    
    var inline_comment = self.collect_inline_comment()
    # Use unified log processing
    var log_code = self.process_log_call(message, "fluent", inline_comment)
    self.add(log_code)
  end

  # Helper method to process restart statement using fluent style
  def process_restart_statement_fluent()
    var keyword = self.current().value  # "restart"
    self.next()  # skip 'restart'
    
    # Expect the value provider identifier
    var val_name = self.expect_identifier()
    
    # Validate that the value is a value_provider at transpile time
    if !self._validate_value_provider_reference(val_name, keyword)
      self.skip_statement()
      return
    end
    
    var inline_comment = self.collect_inline_comment()
    
    # Generate closure step that calls val.start(engine.time_ms)
    var closure_code = f"def (engine) {val_name}_.start(engine.time_ms) end"
    self.add(f"{self.get_indent()}.push_closure_step({closure_code}){inline_comment}")
  end

  # Process import statement: import user_functions or import module_name
  def process_import()
    self.next()  # skip 'import'
    var module_name = self.expect_identifier()
    
    var inline_comment = self.collect_inline_comment()
    
    # Generate Berry import statement with quoted module name
    self.add(f'import {module_name} {inline_comment}')
  end
  
  # Process standalone log statement: log("message")
  def process_standalone_log()
    self.next()  # skip 'log'
    self.expect_left_paren()
    
    # Process the message string
    var message_tok = self.current()
    if message_tok == nil || message_tok.type != animation_dsl.Token.STRING
      self.error("log() function requires a string message")
      self.skip_statement()
      return
    end
    
    var message = message_tok.value
    self.next()  # consume string
    self.expect_right_paren()
    
    var inline_comment = self.collect_inline_comment()
    # Use unified log processing
    var log_code = self.process_log_call(message, "standalone", inline_comment)
    self.add(log_code)
  end
  
  # Process run statement: run demo
  def process_run()
    self.next()  # skip 'run'
    var name = self.expect_identifier()
    
    # Validate that the referenced object exists
    self._validate_object_reference(name, "run")
    
    var inline_comment = self.collect_inline_comment()
    
    # Store run statement for later processing
    self.run_statements.push({
      "name": name,
      "comment": inline_comment
    })
  end
  
  # Process property assignment or standalone function call: animation_name.property = value OR template_call(args)
  def process_property_assignment()
    var object_name = self.expect_identifier()
    
    # Check if this is a function call (template call or special function)
    if self.current() != nil && self.current().type == animation_dsl.Token.LEFT_PAREN
      # Special case for log function - allow as standalone
      if object_name == "log"
        var args = self.process_function_arguments(false)
        var inline_comment = self.collect_inline_comment()
        # Use unified log processing
        var log_code = self.process_log_call(args, "standalone", inline_comment)
        self.add(log_code)
        return
      end
      
      # This is a standalone function call - check if it's a template
      if self.template_definitions.contains(object_name)
        var args = self.process_function_arguments(false)
        var full_args = args != "" ? f"engine, {args}" : "engine"
        var inline_comment = self.collect_inline_comment()
        self.add(f"{object_name}_template({full_args}){inline_comment}")
        
        # Track that we have template calls to trigger engine.run()
        self.has_template_calls = true
      else
        self.error(f"Standalone function calls are only supported for templates. '{object_name}' is not a template.")
        self.skip_statement()
      end
      return
    end
    
    # Check if next token is a dot (property assignment)
    if self.current() != nil && self.current().type == animation_dsl.Token.DOT
      self.next()  # skip '.'
      var property_name = self.expect_identifier()
      
      # Validate parameter if we have this object in our symbol table
      if self.symbol_table.contains(object_name)
        var instance = self.symbol_table[object_name]
        
        # Only validate parameters for actual instances, not sequence markers
        if type(instance) != "string"
          var class_name = classname(instance)
          
          # Use the existing parameter validation logic
          self._validate_single_parameter(class_name, property_name, instance)
        else
          # This is a sequence marker - sequences don't have properties
          self.error(f"Sequences like '{object_name}' do not have properties. Property assignments are only valid for animations and color providers.")
        end
      end
      
      self.expect_assign()
      var value = self.process_value("property")
      var inline_comment = self.collect_inline_comment()
      
      # Use consolidated symbol resolution for property assignments
      var object_ref = self.resolve_symbol_reference(object_name)
      
      # Generate property assignment
      self.add(f"{object_ref}.{property_name} = {value}{inline_comment}")
    else
      # Not a property assignment, skip this statement
      self.error(f"Expected property assignment for '{object_name}' but found no dot")
      self.skip_statement()
    end
  end
  
  # Process any value - unified approach
  def process_value(context)
    return self.process_additive_expression(context, true, false)  # true = top-level, false = not raw mode
  end
  
  # Process palette color with strict validation
  # Only accepts predefined color names or hex color literals
  def process_palette_color()
    var tok = self.current()
    if tok == nil
      self.error("Expected color value in palette")
      return "0xFFFFFFFF"
    end
    
    # Handle hex color literals
    if tok.type == animation_dsl.Token.COLOR
      self.next()
      return self.convert_color(tok.value)
    end
    
    # Handle identifiers (color names)
    if tok.type == animation_dsl.Token.IDENTIFIER
      var name = tok.value
      self.next()
      
      # Only accept predefined color names
      if animation_dsl.is_color_name(name)
        return self.get_named_color_value(name)
      end
      
      # Reject any other identifier
      self.error(f"Unknown color '{name}'. Palettes only accept hex colors (0xRRGGBB) or predefined color names (like 'red', 'blue', 'green'), but not custom colors defined previously. For dynamic palettes with custom colors, use user functions instead.")
      return "0xFFFFFFFF"
    end
    
    self.error("Expected color value in palette. Use hex colors (0xRRGGBB) or predefined color names (like 'red', 'blue', 'green').")
    return "0xFFFFFFFF"
  end
  
  # Process additive expressions (+ and -) - unified method
  def process_additive_expression(context, is_top_level, raw_mode)
    var left = self.process_multiplicative_expression(context, is_top_level, raw_mode)
    
    while !self.at_end()
      var tok = self.current()
      if tok != nil && (tok.type == animation_dsl.Token.PLUS || tok.type == animation_dsl.Token.MINUS)
        var op = tok.value
        self.next()  # consume operator
        var right = self.process_multiplicative_expression(context, false, raw_mode)  # sub-expressions are not top-level
        left = f"{left} {op} {right}"
      else
        break
      end
    end
    
    # Skip closure wrapping in raw mode
    if raw_mode
      return left
    end
    
    # Only create closures at the top level, but not for anonymous functions
    if is_top_level && !self.is_anonymous_function(left)
      # Special handling for repeat_count context - always create simple function for property access
      if context == "repeat_count"
        import string
        if self.is_computed_expression_string(left) || string.find(left, ".") >= 0
          return self.create_simple_function_from_string(left)
        else
          return left
        end
      elif self.is_computed_expression_string(left)
        return self.create_computation_closure_from_string(left)
      else
        return left
      end
    else
      return left
    end
  end
  
  # Process multiplicative expressions (* and /) - unified method
  def process_multiplicative_expression(context, is_top_level, raw_mode)
    var left = self.process_unary_expression(context, is_top_level, raw_mode)
    
    while !self.at_end()
      var tok = self.current()
      if tok != nil && (tok.type == animation_dsl.Token.MULTIPLY || tok.type == animation_dsl.Token.DIVIDE)
        var op = tok.value
        self.next()  # consume operator
        var right = self.process_unary_expression(context, false, raw_mode)  # sub-expressions are not top-level
        left = f"{left} {op} {right}"
      else
        break
      end
    end
    
    return left
  end
  
  # Process unary expressions (- and +) - unified method
  def process_unary_expression(context, is_top_level, raw_mode)
    var tok = self.current()
    if tok == nil
      self.error("Expected value")
      return "nil"
    end
    
    # Handle unary minus for negative numbers
    if tok.type == animation_dsl.Token.MINUS
      self.next()  # consume the minus
      var expr = self.process_unary_expression(context, false, raw_mode)  # sub-expressions are not top-level
      return f"(-{expr})"
    end
    
    # Handle unary plus (optional)
    if tok.type == animation_dsl.Token.PLUS
      self.next()  # consume the plus
      return self.process_unary_expression(context, false, raw_mode)  # sub-expressions are not top-level
    end
    
    return self.process_primary_expression(context, is_top_level, raw_mode)
  end
  
  # Process primary expressions (literals, identifiers, function calls, parentheses) - unified method
  def process_primary_expression(context, is_top_level, raw_mode)
    var tok = self.current()
    if tok == nil
      self.error("Expected value")
      return "nil"
    end
    
    # Parenthesized expression
    if tok.type == animation_dsl.Token.LEFT_PAREN
      self.next()  # consume '('
      var expr = self.process_additive_expression(context, false, raw_mode)  # parenthesized expressions are not top-level
      self.expect_right_paren()
      return f"({expr})"
    end
    
    # Function call: identifier or easing keyword followed by '('
    if (tok.type == animation_dsl.Token.KEYWORD || tok.type == animation_dsl.Token.IDENTIFIER) && 
       self.peek() != nil && self.peek().type == animation_dsl.Token.LEFT_PAREN
      
      var func_name = tok.value
      
      # In raw mode, handle function calls differently
      if raw_mode
        self.next()
        
        # Check if this is a mathematical function
        if self.is_math_method(func_name)
          var args = self.process_function_arguments(true)
          return f"animation._math.{func_name}({args})"
        end
        
        # Special case for log function in expressions
        if func_name == "log"
          var args = self.process_function_arguments(true)
          return self.process_log_call(args, "expression", "")
        end
        
        # Check if this is a template call
        if self.template_definitions.contains(func_name)
          var args = self.process_function_arguments(true)
          var full_args = args != "" ? f"engine, {args}" : "engine"
          return f"{func_name}_template({full_args})"
        end
        
        # For other functions, this shouldn't happen in expression context
        self.error(f"Function '{func_name}' not supported in expression context")
        return "nil"
      else
        # Regular mode - existing logic
        # Check if this is a simple function call first
        if self._is_simple_function_call(func_name)
          return self.process_function_call(context)
        # Check if this is a nested function call or a variable assignment with named parameters
        elif context == "argument" || context == "property" || context == "variable"
          return self.process_nested_function_call()
        else
          return self.process_function_call(context)
        end
      end
    end
    
    # Color value
    if tok.type == animation_dsl.Token.COLOR
      self.next()
      return self.convert_color(tok.value)
    end
    
    # Time value
    if tok.type == animation_dsl.Token.TIME
      return str(self.process_time_value())
    end
    
    # Percentage value
    if tok.type == animation_dsl.Token.PERCENTAGE
      return str(self.process_percentage_value())
    end
    
    # Number value
    if tok.type == animation_dsl.Token.NUMBER
      var value = tok.value
      self.next()
      return value
    end
    
    # String value
    if tok.type == animation_dsl.Token.STRING
      var value = tok.value
      self.next()
      return f'"{value}"'
    end
    
    # Array literal (not supported in raw mode)
    if tok.type == animation_dsl.Token.LEFT_BRACKET && !raw_mode
      return self.process_array_literal()
    end
    
    # Identifier - could be color, animation, variable, or object property reference
    if tok.type == animation_dsl.Token.IDENTIFIER
      var name = tok.value
      self.next()
      
      # Check if this is an object property reference (identifier.property)
      if self.current() != nil && self.current().type == animation_dsl.Token.DOT
        self.next()  # consume '.'
        var property_name = self.expect_identifier()
        
        # Special handling for user.function_name() calls
        if name == "user"
          return self._process_user_function_call(property_name)
        end
        
        # Validate that the property exists on the referenced object (skip in raw mode)
        if !raw_mode && self.symbol_table.contains(name)
          var instance = self.symbol_table[name]
          # Only validate parameters for actual instances, not sequence markers
          if type(instance) != "string"
            var class_name = classname(instance)
            self._validate_single_parameter(class_name, property_name, instance)
          else
            # This is a sequence marker - sequences don't have properties
            self.error(f"Sequences like '{name}' do not have properties. Property references are only valid for animations and color providers.")
            return "nil"
          end
        end
        
        # Use consolidated symbol resolution for the object reference
        var object_ref = self.resolve_symbol_reference(name)
        
        # In raw mode or repeat_count context, generate simple property access
        if raw_mode || context == "repeat_count"
          return f"{object_ref}.{property_name}"
        else
          # Return a closure expression that will be wrapped by the caller if needed
          return f"animation.resolve({object_ref}, '{property_name}')"
        end
      end
      
      # Use consolidated symbol resolution
      return self.resolve_symbol_reference(name)
    end
    
    # Boolean keywords
    if tok.type == animation_dsl.Token.KEYWORD && (tok.value == "true" || tok.value == "false")
      var value = tok.value
      self.next()
      return value
    end
    
    # Handle keywords that should be treated as identifiers (not sure this actually happens)
    if tok.type == animation_dsl.Token.KEYWORD
      var name = tok.value
      self.next()
      return f"animation.{name}"
    end
    
    self.error(f"Unexpected value: {tok.value}")
    self.next()
    return "nil"
  end
  
  # Check if an expression is already an anonymous function
  def is_anonymous_function(expr_str)
    import string
    # Check if the expression starts with "(def " and ends with ")(engine)"
    return string.find(expr_str, "(def ") == 0 && string.find(expr_str, ")(engine)") >= 0
  end

  # Check if an expression string contains computed values that need a closure
  def is_computed_expression_string(expr_str)
    import string
    
    # Check if the expression contains operators that make it a computation
    var has_operators = (
      string.find(expr_str, " + ") >= 0 ||                                     # Addition
      string.find(expr_str, " - ") >= 0 ||                                     # Subtraction  
      string.find(expr_str, " * ") >= 0 ||                                     # Multiplication
      string.find(expr_str, " / ") >= 0                                        # Division
    )
    
    # Check for function calls (parentheses with identifiers before them)
    # This excludes simple parenthesized literals like (-1) and simple function calls
    var has_function_calls = false
    var paren_pos = string.find(expr_str, "(")
    if paren_pos > 0
      # Check if there's an identifier before the parenthesis (indicating a function call)
      var char_before = expr_str[paren_pos-1]
      if self.is_identifier_char(char_before)
        # Extract the function name to check if it's a simple function
        var func_start = paren_pos - 1
        while func_start >= 0 && self.is_identifier_char(expr_str[func_start])
          func_start -= 1
        end
        func_start += 1
        var func_name = expr_str[func_start..paren_pos-1]
        
        # Only mark as needing closure if it's NOT a simple function
        if !self._is_simple_function_call(func_name)
          has_function_calls = true
        end
      end
    end
    
    # Only create closures for expressions that actually involve computation
    return has_operators || has_function_calls
  end
  
  # Check if an expression contains computed values that need a closure (legacy method)
  def is_computed_expression(left, op, right)
    import string
    
    # Check if either operand contains a function call, variable reference, or user variable
    # We're permissive here - any expression with these patterns gets a closure
    var has_dynamic_content = (
      string.find(left, "(") >= 0 || string.find(right, "(") >= 0 ||           # Function calls
      string.find(left, "animation.") >= 0 || string.find(right, "animation.") >= 0 ||  # Animation module calls
      string.find(left, "_") >= 0 || string.find(right, "_") >= 0              # User variables (might be ValueProviders)
    )
    
    return has_dynamic_content
  end
  

  
  # Create a closure for computed expressions from a complete expression string
  def create_computation_closure_from_string(expr_str)
    import string
    
    # Transform the entire expression to handle ValueProvider instances
    var transformed_expr = self.transform_expression_for_closure(expr_str)
    
    # Clean up spacing in the expression - remove extra spaces
    while string.find(transformed_expr, "  ") >= 0
      transformed_expr = string.replace(transformed_expr, "  ", " ")
    end
    
    var closure_code = f"def (engine) return {transformed_expr} end"
    
    # Return a closure value provider instance
    return f"animation.create_closure_value(engine, {closure_code})"
  end
  
  # Create a closure for computed expressions (legacy method)
  def create_computation_closure(left, op, right)
    import string
    
    # Create a closure value provider that wraps the computation
    # This replaces the old DSL computed value wrapper approach
    
    # Transform operands to handle ValueProvider instances
    var left_expr = self.transform_operand_for_closure(left)
    var right_expr = self.transform_operand_for_closure(right)
    
    # Clean up spacing in the expression - remove extra spaces
    while string.find(left_expr, "  ") >= 0
      left_expr = string.replace(left_expr, "  ", " ")
    end
    while string.find(right_expr, "  ") >= 0
      right_expr = string.replace(right_expr, "  ", " ")
    end
    
    var closure_code = f"def (engine) return {left_expr} {op} {right_expr} end"
    
    # Return a closure value provider instance
    return f"animation.create_closure_value(engine, {closure_code})"
  end
  
  # Create a simple function for repeat counts (no closure wrapper)
  def create_simple_function_from_string(expr_str)
    # For repeat counts, create a simple function that takes engine and returns the value
    # The expression should already be in simple form like "col1_.palette_size"
    return f"def (engine) return {expr_str} end"
  end
  
  # Transform a complete expression for use in a closure, handling ValueProvider instances
  def transform_expression_for_closure(expr_str)
    import string
    
    var result = expr_str
    var pos = 0
    
    # First pass: Replace value provider function calls with resolve calls
    # Handle functions like animation.strip_length(engine) that return value providers
    var value_provider_functions = ["strip_length"]
    
    for func_name : value_provider_functions
      var pattern = f"animation.{func_name}(engine)"
      var replacement = f"animation.resolve(animation.{func_name}(engine))"
      
      # Replace all occurrences of this pattern
      pos = 0
      while pos < size(result)
        var found_pos = string.find(result, pattern, pos)
        if found_pos < 0
          break
        end
        
        # Check if it's already wrapped in animation.resolve()
        var check_start = found_pos >= 18 ? found_pos - 18 : 0
        var prefix = result[check_start..found_pos-1]
        if string.find(prefix, "animation.resolve(") >= 0
          # Already wrapped, skip this occurrence
          pos = found_pos + size(pattern)
          continue
        end
        
        # Replace the pattern
        var before = found_pos > 0 ? result[0..found_pos-1] : ""
        var after = found_pos + size(pattern) < size(result) ? result[found_pos + size(pattern)..] : ""
        result = before + replacement + after
        pos = found_pos + size(replacement)
      end
    end
    
    # Second pass: Replace all user variables (ending with _) with resolve calls
    pos = 0
    while pos < size(result)
      var underscore_pos = string.find(result, "_", pos)
      if underscore_pos < 0
        break
      end
      
      # Find the start of the identifier
      var start_pos = underscore_pos
      while start_pos > 0 && self.is_identifier_char(result[start_pos-1])
        start_pos -= 1
      end
      
      # Check if this is a user variable (not preceded by "animation." or already inside a resolve call)
      var is_user_var = true
      if start_pos >= 18
        var check_start = start_pos >= 18 ? start_pos - 18 : 0
        var prefix = result[check_start..start_pos-1]
        if string.find(prefix, "animation.resolve(") >= 0
          is_user_var = false
        end
      end
      if is_user_var && start_pos >= 10
        var check_start = start_pos >= 10 ? start_pos - 10 : 0
        var prefix = result[check_start..start_pos-1]
        if string.find(prefix, "animation.") >= 0
          is_user_var = false
        end
      end
      
      if is_user_var && start_pos < underscore_pos
        # Extract the variable name
        var var_name = result[start_pos..underscore_pos]
        
        # Check if it's followed by non-identifier characters
        var end_pos = underscore_pos + 1
        if end_pos >= size(result) || !self.is_identifier_char(result[end_pos])
          # Replace the variable with the resolve call
          var replacement = f"animation.resolve({var_name})"
          var before = start_pos > 0 ? result[0..start_pos-1] : ""
          var after = end_pos < size(result) ? result[end_pos..] : ""
          result = before + replacement + after
          pos = start_pos + size(replacement)
        else
          pos = underscore_pos + 1
        end
      else
        pos = underscore_pos + 1
      end
    end
    
    return result
  end
  
  # Helper method to check if a character is part of an identifier
  def is_identifier_char(ch)
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_'
  end
  
  # Helper method to check if a function name is a mathematical function
  def is_math_method(func_name)
    try
      import introspect
      # Check if the function is registered in the animation._math map
      return introspect.contains(animation._math, func_name)
    except .. as e, msg
      # If _math map doesn't exist or access fails, return false to be safe
      return false
    end
  end
  
  # Transform an operand for use in a closure, handling ValueProvider instances
  def transform_operand_for_closure(operand)
    import string
    
    # If the operand is already a closure (contains create_closure_value), extract its inner expression
    if string.find(operand, "animation.create_closure_value") >= 0
      # Extract the inner expression from the closure
      var start_pos = string.find(operand, "return (") + 8
      var end_pos = size(operand) - 5  # Remove " end)"
      var inner_expr = operand[start_pos..end_pos]
      # Clean up any extra spaces
      while string.find(inner_expr, "  ") >= 0
        inner_expr = string.replace(inner_expr, "  ", " ")
      end
      return inner_expr
    end
    
    # Check if this is a simple user variable (identifier ending with _ and no operators/parentheses)
    var has_underscore = string.find(operand, "_") >= 0
    var has_operators = string.find(operand, " ") >= 0  # Simple check for operators (they have spaces)
    var has_paren = string.find(operand, "(") >= 0
    var has_animation_prefix = string.find(operand, "animation.") >= 0
    
    if has_underscore && !has_operators && !has_paren && !has_animation_prefix
      # This looks like a simple user variable that might be a ValueProvider
      return f"animation.resolve({operand})"
    else
      # For other expressions (literals, animation module calls, complex expressions), use as-is
      return operand
    end
  end
  
  # Process function call (legacy - for non-animation contexts)
  def process_function_call(context)
    var tok = self.current()
    var func_name = ""
    
    # Handle both identifiers and keywords as function names
    if tok != nil && (tok.type == animation_dsl.Token.IDENTIFIER || tok.type == animation_dsl.Token.KEYWORD)
      func_name = tok.value
      self.next()
    else
      self.error("Expected function name")
      return "nil"
    end
    
    # Check if this is a mathematical function - handle with positional arguments
    if self.is_math_method(func_name)
      # Mathematical functions use positional arguments, not named parameters
      var args = self.process_function_arguments(false)
      return f"{func_name}({args})"  # Return as-is for transformation in closure
    end
    
    # Special case for log function - call global log function directly
    if func_name == "log"
      var args = self.process_function_arguments(false)
      # Use unified log processing (return expression for use in contexts)
      return self.process_log_call(args, "expression", "")
    end
    
    var args = self.process_function_arguments(false)
    
    # Check if it's a template call first
    if self.template_definitions.contains(func_name)
      # This is a template call - treat like user function
      var full_args = args != "" ? f"engine, {args}" : "engine"
      return f"{func_name}_template({full_args})"
    else
      # All functions are resolved from the animation module and need engine as first parameter
      if args != ""
        return f"animation.{func_name}(engine, {args})"
      else
        return f"animation.{func_name}(engine)"
      end
    end
  end
  
  # Process time value - simplified
  def process_time_value()
    var tok = self.current()
    if tok != nil && tok.type == animation_dsl.Token.TIME
      var time_str = tok.value
      self.next()
      return self.convert_time_to_ms(time_str)
    elif tok != nil && tok.type == animation_dsl.Token.NUMBER
      var num = tok.value
      self.next()
      return int(real(num)) * 1000  # assume seconds
    elif tok != nil && tok.type == animation_dsl.Token.IDENTIFIER
      # Handle variable references for time values
      var var_name = tok.value
      
      # Validate that the variable exists before processing
      self._validate_object_reference(var_name, "duration")
      
      var expr = self.process_primary_expression("time", true, false)
      return expr
    else
      self.error("Expected time value")
      return 1000
    end
  end
  
  # Process percentage value - simplified
  def process_percentage_value()
    var tok = self.current()
    if tok != nil && tok.type == animation_dsl.Token.PERCENTAGE
      var percent_str = tok.value
      self.next()
      var percent = real(percent_str[0..-2])
      return int(percent * 255 / 100)
    elif tok != nil && tok.type == animation_dsl.Token.NUMBER
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
           (self.current() != nil && self.current().type == animation_dsl.Token.EOF)
  end
  
  def skip_whitespace()
    while !self.at_end()
      var tok = self.current()
      if tok != nil && (tok.type == animation_dsl.Token.NEWLINE || tok.type == animation_dsl.Token.COMMENT)
        self.next()
      else
        break
      end
    end
  end
  
  # Skip whitespace including newlines (for parameter parsing contexts)
  def skip_whitespace_including_newlines()
    while !self.at_end()
      var tok = self.current()
      if tok != nil && (tok.type == animation_dsl.Token.COMMENT || tok.type == animation_dsl.Token.NEWLINE)
        self.next()
      else
        break
      end
    end
  end
  
  # Collect inline comment if present and return it formatted for Berry code
  def collect_inline_comment()
    var tok = self.current()
    if tok != nil && tok.type == animation_dsl.Token.COMMENT
      var comment = "  " + tok.value  # Add spacing before comment
      self.next()
      return comment
    end
    return ""  # No comment found
  end
  
  def expect_identifier()
    var tok = self.current()
    if tok != nil && (tok.type == animation_dsl.Token.IDENTIFIER || 
                      tok.type == animation_dsl.Token.COLOR ||
                      (tok.type == animation_dsl.Token.KEYWORD && self.can_use_as_identifier(tok.value)))
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
      # DSL keywords that might be used as parameter names or variable names
      "color", "animation", "palette",
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
  
  # Process function arguments with unified implementation
  # @param raw_mode: boolean - If true, returns raw expressions without closures (for expression contexts)
  #                           If false, processes values normally with closure wrapping (for statement contexts)
  def process_function_arguments(raw_mode)
    self.expect_left_paren()
    var args = []
    
    while !self.at_end() && !self.check_right_paren()
      self.skip_whitespace()
      
      if self.check_right_paren()
        break
      end
      
      var arg
      if raw_mode
        # For expression contexts - use raw mode to avoid closure wrapping
        arg = self.process_additive_expression("argument", true, true)  # raw_mode = true
      else
        # For statement contexts - use normal processing with closure wrapping
        arg = self.process_value("argument")
      end
      args.push(arg)
      
      self.skip_whitespace()
      
      if self.current() != nil && self.current().type == animation_dsl.Token.COMMA
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




  
  # Process nested function call (generates temporary variable or raw expression)
  def process_nested_function_call()
    var tok = self.current()
    var func_name = ""
    
    # Handle both identifiers and keywords as function names
    if tok != nil && (tok.type == animation_dsl.Token.IDENTIFIER || tok.type == animation_dsl.Token.KEYWORD)
      func_name = tok.value
      self.next()
    else
      self.error("Expected function name")
      return "nil"
    end
    
    # Check if this is a mathematical function - handle with positional arguments
    if self.is_math_method(func_name)
      # Mathematical functions use positional arguments, not named parameters
      var args = self.process_function_arguments(true)
      return f"animation._math.{func_name}({args})"  # Math functions are under _math namespace
    end
    
    # Special case for log function in nested calls
    if func_name == "log"
      var args = self.process_function_arguments(true)
      # Use unified log processing
      return self.process_log_call(args, "expression", "")
    end
    
    # Check if this is a template call
    if self.template_definitions.contains(func_name)
      # This is a template call - treat like user function
      var args = self.process_function_arguments(true)
      var full_args = args != "" ? f"engine, {args}" : "engine"
      return f"{func_name}_template({full_args})"
    else
      # Check if this is a simple function call without named parameters
      if self._is_simple_function_call(func_name)
        # For simple functions like strip_length(), do direct assignment
        var args = self.process_function_arguments(false)
        if args != ""
          return f"animation.{func_name}(engine, {args})"
        else
          return f"animation.{func_name}(engine)"
        end
      else
        # Built-in functions use the new engine-first + named parameters pattern
        # Validate that the factory function exists at transpilation time
        if !self._validate_animation_factory_exists(func_name)
          self.error(f"Animation factory function '{func_name}' does not exist. Check the function name and ensure it's available in the animation module.")
          self.skip_function_arguments()  # Skip the arguments to avoid parsing errors
          return "nil"
        end
        
        # Generate anonymous function that creates and configures the provider
        return self._generate_anonymous_function_call(func_name)
      end
    end
  end
  
  # Process named arguments for a variable (new simpler pattern with parameter validation)
  def process_named_arguments_for_variable(var_name)
    self._process_named_arguments_unified(var_name, "", "variable")
  end
  
  def expect_assign()
    var tok = self.current()
    if tok != nil && tok.type == animation_dsl.Token.ASSIGN
      self.next()
    else
      self.error("Expected '='")
    end
  end
  
  def expect_left_paren()
    var tok = self.current()
    if tok != nil && tok.type == animation_dsl.Token.LEFT_PAREN
      self.next()
    else
      self.error("Expected '('")
    end
  end
  
  def expect_right_paren()
    var tok = self.current()
    if tok != nil && tok.type == animation_dsl.Token.RIGHT_PAREN
      self.next()
    else
      self.error("Expected ')'")
    end
  end
  
  def check_right_paren()
    var tok = self.current()
    return tok != nil && tok.type == animation_dsl.Token.RIGHT_PAREN
  end
  
  def expect_comma()
    var tok = self.current()
    if tok != nil && tok.type == animation_dsl.Token.COMMA
      self.next()
    else
      self.error("Expected ','")
    end
  end
  
  def expect_left_brace()
    var tok = self.current()
    if tok != nil && tok.type == animation_dsl.Token.LEFT_BRACE
      self.next()
    else
      self.error("Expected '{'")
    end
  end
  
  def expect_right_brace()
    var tok = self.current()
    if tok != nil && tok.type == animation_dsl.Token.RIGHT_BRACE
      self.next()
    else
      self.error("Expected '}'")
    end
  end
  
  def check_right_brace()
    var tok = self.current()
    return tok != nil && tok.type == animation_dsl.Token.RIGHT_BRACE
  end
  
  def expect_number()
    var tok = self.current()
    if tok != nil && tok.type == animation_dsl.Token.NUMBER
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
    if tok != nil && tok.type == animation_dsl.Token.KEYWORD && tok.value == keyword
      self.next()
    else
      self.error(f"Expected '{keyword}'")
    end
  end
  
  def expect_colon()
    var tok = self.current()
    if tok != nil && tok.type == animation_dsl.Token.COLON
      self.next()
    else
      self.error("Expected ':'")
    end
  end
  
  def expect_dot()
    var tok = self.current()
    if tok != nil && tok.type == animation_dsl.Token.DOT
      self.next()
    else
      self.error("Expected '.'")
    end
  end
  
  def expect_left_bracket()
    var tok = self.current()
    if tok != nil && tok.type == animation_dsl.Token.LEFT_BRACKET
      self.next()
    else
      self.error("Expected '['")
    end
  end
  
  def expect_right_bracket()
    var tok = self.current()
    if tok != nil && tok.type == animation_dsl.Token.RIGHT_BRACKET
      self.next()
    else
      self.error("Expected ']'")
    end
  end
  
  def check_right_bracket()
    var tok = self.current()
    return tok != nil && tok.type == animation_dsl.Token.RIGHT_BRACKET
  end
  

  
  # Process array literal [item1, item2, item3]
  def process_array_literal()
    self.expect_left_bracket()
    var items = []
    
    while !self.at_end() && !self.check_right_bracket()
      # Process array element
      var item = self.process_value("array_element")
      items.push(item)
      
      if self.current() != nil && self.current().type == animation_dsl.Token.COMMA
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
      if tok.type == animation_dsl.Token.NEWLINE || tok.type == animation_dsl.Token.EOF
        break
      end
      self.next()
    end
  end
  
  # Skip function arguments when validation fails
  def skip_function_arguments()
    if self.current() != nil && self.current().type == animation_dsl.Token.LEFT_PAREN
      self.next()  # consume '('
      var paren_count = 1
      
      while !self.at_end() && paren_count > 0
        var tok = self.current()
        if tok.type == animation_dsl.Token.LEFT_PAREN
          paren_count += 1
        elif tok.type == animation_dsl.Token.RIGHT_PAREN
          paren_count -= 1
        end
        self.next()
      end
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
    if animation_dsl.is_color_name(color_str)
      return self.get_named_color_value(color_str)
    end
    
    # Unknown color - return white as default
    return "0xFFFFFFFF"
  end
  
  # Get the ARGB value for a named color
  # This should match the colors supported by is_color_name()
  def get_named_color_value(color_name)
    return animation_dsl.SimpleDSLTranspiler.named_colors.find(color_name, "0xFFFFFFFF")
  end
  
  # Validate that a user-defined name is not a predefined color or DSL keyword
  def validate_user_name(name, definition_type)
    # Check if it's a predefined color name
    for color : animation_dsl.Token.color_names
      if name == color
        self.error(f"Cannot redefine predefined color '{name}'. Use a different name like '{name}_custom' or 'my_{name}'")
        return false
      end
    end
    
    # Check if it's a DSL statement keyword
    for keyword : animation_dsl.Token.statement_keywords
      if name == keyword
        self.error(f"Cannot use DSL keyword '{name}' as {definition_type} name. Use a different name like '{name}_custom' or 'my_{name}'")
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
  
  # Generate single engine.run() call for all run statements
  def generate_engine_run()
    if size(self.run_statements) == 0 && !self.has_template_calls
      return  # No run statements or template calls, no need to start engine
    end
    
    # Add all animations/sequences to the engine
    for run_stmt : self.run_statements
      var name = run_stmt["name"]
      var comment = run_stmt["comment"]
      
      # Check if this is a sequence or regular animation
      # Use unified add() method - it will detect the type automatically
      self.add(f"engine.add({name}_){comment}")
    end
    
    # Single engine.run() call
    self.add("engine.run()")
  end

  # Basic event handler processing
  def process_event_handler()
    self.next()  # skip 'on'
    var event_name = self.expect_identifier()
    var line = self.current() != nil ? self.current().line : 0
    
    # Check for event parameters (e.g., timer(5s))
    var event_params = "{}"
    if self.current() != nil && self.current().type == animation_dsl.Token.LEFT_PAREN
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
      if tok.type == animation_dsl.Token.KEYWORD && tok.value == "interrupt"
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
        self.add(f"  engine.add(temp_anim)")
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
      if tok != nil && tok.type == animation_dsl.Token.TIME
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
    self.add("var engine = animation.init_strip()")
    self.add("")
    self.strip_initialized = true
  end
  
  # Generate Berry function for template definition
  def generate_template_function(name, params, param_types, body_tokens)
    import string
    
    # Generate function signature with engine as first parameter
    var param_list = "engine"
    for param : params
      param_list += f", {param}_"
    end
    
    self.add(f"# Template function: {name}")
    self.add(f"def {name}_template({param_list})")
    
    # Create a new transpiler instance for the template body
    var template_transpiler = animation_dsl.SimpleDSLTranspiler(body_tokens)
    template_transpiler.symbol_table = {}  # Fresh symbol table for template
    template_transpiler.strip_initialized = true  # Templates assume engine exists
    
    # Add parameters to template's symbol table
    for param : params
      template_transpiler.symbol_table[param] = "parameter"
    end
    
    # Transpile the template body
    var template_body = template_transpiler.transpile_template_body()
    
    if template_body != nil
      # Add the transpiled body with proper indentation
      var body_lines = string.split(template_body, "\n")
      for line : body_lines
        if size(line) > 0
          self.add(f"  {line}")  # Add 2-space indentation
        end
      end
    else
      # Error in template body transpilation
      for error : template_transpiler.errors
        self.error(f"Template '{name}' body error: {error}")
      end
    end
    
    self.add("end")
    self.add("")
    
    # Register the template as a user function
    self.add(f"animation.register_user_function('{name}', {name}_template)")
    self.add("")
  end
  
  # Process named arguments for animation declarations with parameter validation
  #
  # @param var_name: string - Variable name to assign parameters to
  # @param func_name: string - Animation function name for validation
  def _process_named_arguments_for_animation(var_name, func_name)
    self._process_named_arguments_unified(var_name, func_name, "animation")
  end
  
  # Create instance for parameter validation at transpile time
  def _create_instance_for_validation(func_name)
    try
      var mock_engine = animation_dsl.MockEngine()
      import introspect
      
      if introspect.contains(animation, func_name)
        var factory_func = animation.(func_name)
        if type(factory_func) == "class" || type(factory_func) == "function"
          return factory_func(mock_engine)
        end
      end
      return nil
    except .. as e, msg
      return nil
    end
  end
  
  # Validate a single parameter immediately as it's parsed
  #
  # @param func_name: string - Name of the animation function
  # @param param_name: string - Name of the parameter being validated
  # @param animation_instance: instance - Pre-created animation instance for validation
  def _validate_single_parameter(func_name, param_name, animation_instance)
    try
      import introspect
      
      # Validate parameter using the _has_param method
      if animation_instance != nil && introspect.contains(animation_instance, "_has_param")
        if !animation_instance._has_param(param_name)
          var line = self.current() != nil ? self.current().line : 0
          self.error(f"Animation '{func_name}' does not have parameter '{param_name}'. Check the animation documentation for valid parameters.")
        end
      end
      
    except .. as e, msg
      # If validation fails for any reason, just continue
      # This ensures the transpiler is robust even if validation has issues
    end
  end
  
  # Validate that a referenced object exists in the symbol table or animation module
  #
  # @param object_name: string - Name of the object being referenced
  # @param context: string - Context where the reference occurs (for error messages)
  def _validate_object_reference(object_name, context)
    try
      # Use consolidated symbol resolution
      self.validate_symbol_reference(object_name, context)
    except .. as e, msg
      # If validation fails for any reason, just continue
      # This ensures the transpiler is robust even if validation has issues
    end
  end
  
  # Validate factory function at transpile time by creating instance and checking type
  def _validate_factory_function(func_name, expected_base_class)
    try
      import introspect
      
      # Check if the animation module has the function
      if !introspect.contains(animation, func_name)
        return false
      end
      
      var factory_func = animation.(func_name)
      
      # Check if it's callable (function or class)
      var func_type = type(factory_func)
      if func_type != "function" && func_type != "class"
        return false
      end
      
      # If no type checking needed, just return true
      if expected_base_class == nil
        return true
      end
      
      # Create instance at transpile time to validate type
      var mock_engine = animation_dsl.MockEngine()
      try
        var instance = factory_func(mock_engine)
        return isinstance(instance, expected_base_class)
      except .. as e, msg
        return false
      end
      
    except .. as e, msg
      return false
    end
  end
  
  # Validate animation factory exists and creates animation.animation instance
  def _validate_animation_factory_exists(func_name)
    # Skip validation for mathematical functions - they will be handled by closure transformation
    if self.is_math_method(func_name)
      return true  # Skip validation for mathematical functions
    end
    
    return self._validate_factory_function(func_name, nil)
  end
  
  def _validate_animation_factory_creates_animation(func_name)
    return self._validate_factory_function(func_name, animation.animation)
  end
  
  # Validate color provider factory exists and creates animation.color_provider instance  
  def _validate_color_provider_factory_exists(func_name)
    return self._validate_factory_function(func_name, animation.color_provider)
  end
  
  # Validate value provider factory exists and creates animation.value_provider instance  
  def _validate_value_provider_factory_exists(func_name)
    return self._validate_factory_function(func_name, animation.value_provider)
  end
  
  # Consolidated symbol resolution - returns the symbol reference string
  def resolve_symbol_reference(name)
    import introspect
    
    # Check if it's a named color
    if animation_dsl.is_color_name(name)
      return self.get_named_color_value(name)
    end
    
    # Check symbol table, sequences, then animation module
    if self.symbol_table.contains(name) || self.sequence_names.contains(name)
      return f"{name}_"
    elif introspect.contains(animation, name)
      return f"animation.{name}"
    else
      return f"{name}_"  # Assume user-defined if not found
    end
  end
  
  # Check if a symbol exists (for validation)
  def symbol_exists(name)
    import introspect
    
    return (
      animation_dsl.is_color_name(name) ||
      self.symbol_table.contains(name) ||
      self.sequence_names.contains(name) ||
      introspect.contains(animation, name)
    )
  end
  
  # Validate symbol reference with error reporting
  def validate_symbol_reference(name, context)
    if !self.symbol_exists(name)
      self.error(f"Undefined reference '{name}' in {context}. Make sure the object is defined before use.")
      return false
    end
    return true
  end

  # Validate that a referenced object is a value provider or animation
  def _validate_value_provider_reference(object_name, context)
    try
      # First check if symbol exists
      if !self.symbol_exists(object_name)
        self.error(f"Undefined reference '{object_name}' in {context} statement. Make sure the value provider or animation is defined before use.")
        return false
      end
      
      # Check if it's a user-defined object in symbol table
      if self.symbol_table.contains(object_name)
        var marker = self.symbol_table[object_name]
        
        # Check if it's marked as a value provider or animation
        if type(marker) == "string" && (marker == "value_provider" || marker == "animation")
          return true  # Valid value provider or animation
        elif type(marker) == "string"
          # It's some other type (variable, color, sequence, etc.)
          self.error(f"'{object_name}' in {context} statement is not a value provider or animation. Only value providers (like oscillators) and animations can be restarted.")
          return false
        else
          # It's an actual instance - check if it's a value provider or animation
          if isinstance(marker, animation.value_provider) || isinstance(marker, animation.animation)
            return true  # Valid value provider or animation
          else
            self.error(f"'{object_name}' in {context} statement is not a value provider or animation. Only value providers (like oscillators) and animations can be restarted.")
            return false
          end
        end
      end
      
      # For built-in symbols or sequences, assume they're valid (can't validate at compile time)
      return true
      
    except .. as e, msg
      # If validation fails for any reason, report error but continue
      self.error(f"Could not validate '{object_name}' in {context} statement: {msg}")
      return false
    end
  end
  
  # Core parameter processing logic that can be used by different contexts
  # @param func_name: string - Function name for validation (can be empty for variable mode)
  # @param validation_type: string - Type of validation: "animation", "color_provider", "value_provider", "variable", or "generic"
  # @param assignment_callback: function - Callback to handle parameter assignments, receives (param_name, param_value, inline_comment)
  def _process_parameters_core(func_name, validation_type, assignment_callback)
    # Create instance once for parameter validation based on validation type
    var instance = nil
    var effective_func_name = func_name
    
    if validation_type == "variable"
      # Extract function name from variable name for validation
      import string
      if string.find(func_name, "temp_") == 0
        # Extract function name from temp variable: temp_breathe_123 -> breathe
        var parts = string.split(func_name, "_")
        if size(parts) >= 2
          effective_func_name = parts[1]
        end
      end
    end
    
    # Create validation instance if we have a function name
    if effective_func_name != ""
      instance = self._create_instance_for_validation(effective_func_name)
    end
    
    while !self.at_end() && !self.check_right_paren()
      self.skip_whitespace_including_newlines()
      
      if self.check_right_paren()
        break
      end
      
      # Parse named argument: param_name=value
      var param_name = self.expect_identifier()
      
      # Validate parameter immediately as it's parsed
      if instance != nil && effective_func_name != ""
        self._validate_single_parameter(effective_func_name, param_name, instance)
      end
      
      self.expect_assign()
      var param_value = self.process_value("argument")
      var inline_comment = self.collect_inline_comment()
      
      # Call the assignment callback to handle the parameter
      assignment_callback(param_name, param_value, inline_comment)
      
      # Skip whitespace but preserve newlines for separator detection
      while !self.at_end()
        var tok = self.current()
        if tok != nil && tok.type == animation_dsl.Token.COMMENT
          self.next()
        else
          break
        end
      end
      
      # Check for parameter separator: comma OR newline OR end of parameters
      if self.current() != nil && self.current().type == animation_dsl.Token.COMMA
        self.next()  # skip comma
        self.skip_whitespace_including_newlines()
      elif self.current() != nil && self.current().type == animation_dsl.Token.NEWLINE
        # Newline acts as parameter separator - skip it and continue
        self.next()  # skip newline
        self.skip_whitespace_including_newlines()
      elif !self.check_right_paren()
        self.error("Expected ',' or ')' in function arguments")
        break
      end
    end
  end
  
  # Unified parameter processing method with validation type parameter
  # @param var_name: string - Variable name to assign parameters to
  # @param func_name: string - Function name for validation (can be empty for variable mode)
  # @param validation_type: string - Type of validation: "animation", "color_provider", "value_provider", "variable", or "generic"
  def _process_named_arguments_unified(var_name, func_name, validation_type)
    self.expect_left_paren()
    
    # Use the core processing logic with a callback for standard assignments
    var assignment_callback = def (param_name, param_value, inline_comment)
      self.add(f"{var_name}.{param_name} = {param_value}{inline_comment}")
    end
    
    self._process_parameters_core(func_name, validation_type, assignment_callback)
    self.expect_right_paren()
  end
  
  # Legacy wrapper methods for backward compatibility
  def _process_named_arguments_generic(var_name, func_name)
    self._process_named_arguments_unified(var_name, func_name, "generic")
  end
  
  def _process_named_arguments_for_color_provider(var_name, func_name)
    self._process_named_arguments_unified(var_name, func_name, "color_provider")
  end

  # Check if this is a simple function call that doesn't need anonymous function treatment
  def _is_simple_function_call(func_name)
    # Functions that return simple values and don't use named parameters
    # Note: log is handled by unified process_log_call method
    var simple_functions = [
      "strip_length",
      "static_value"
    ]
    
    for simple_func : simple_functions
      if func_name == simple_func
        return true
      end
    end
    return false
  end

  # Generate anonymous function call that creates and configures a provider without temporary variables
  def _generate_anonymous_function_call(func_name)
    # Start building the anonymous function
    var lines = []
    lines.push("(def (engine)")
    lines.push(f"  var provider = animation.{func_name}(engine)")
    
    # Process named arguments and collect them
    self.expect_left_paren()
    
    # Use the core processing logic with a callback for anonymous function assignments
    var assignment_callback = def (param_name, param_value, inline_comment)
      lines.push(f"  provider.{param_name} = {param_value}{inline_comment}")
    end
    
    self._process_parameters_core(func_name, "generic", assignment_callback)
    self.expect_right_paren()
    
    # Complete the anonymous function
    lines.push("  return provider")
    lines.push("end)(engine)")
    
    # Join all lines into a single expression
    var result = ""
    for i : 0..size(lines)-1
      if i > 0
        result += "\n"
      end
      result += lines[i]
    end
    return result
  end

end

# DSL compilation function
def compile_dsl(source)
  var lexer = animation_dsl.DSLLexer(source)
  var tokens = lexer.tokenize()
  
  if lexer.has_errors()
    var error_msg = "DSL Lexer errors:\n"
    for error : lexer.get_errors()
      error_msg += "  " + error + "\n"
    end
    raise "dsl_compilation_error", error_msg
  end
  
  var transpiler = animation_dsl.SimpleDSLTranspiler(tokens)
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
  "MockEngine": MockEngine
}