# Ultra-Simplified DSL Transpiler for Animation DSL
# Single-pass transpiler with minimal complexity
# Leverages Berry's runtime for symbol resolution

#@ solidify:SimpleDSLTranspiler,weak
class SimpleDSLTranspiler
  var pull_lexer      # Pull lexer instance
  var output          # Generated Berry code lines
  var warnings        # Compilation warnings
  var run_statements  # Collect all run statements for single engine.run()
  var strip_initialized # Track if strip was initialized
  var symbol_table    # Enhanced symbol cache: name -> {type, instance, class_obj}
  var indent_level    # Track current indentation level for nested sequences
  var has_template_calls    # Track if we have template calls to trigger engine.run()
  
  # Context constants for process_value calls
  static var CONTEXT_VARIABLE = 1
  static var CONTEXT_COLOR = 2
  static var CONTEXT_ANIMATION = 3
  static var CONTEXT_ARGUMENT = 4
  static var CONTEXT_PROPERTY = 5
  static var CONTEXT_REPEAT_COUNT = 6
  static var CONTEXT_ARRAY_ELEMENT = 7
  static var CONTEXT_TIME = 8
  static var CONTEXT_EXPRESSION = 9
  static var CONTEXT_GENERIC = 10
  static var CONTEXT_COLOR_PROVIDER = 11
  
  # Helper class to track expression metadata for closure detection
  #@ solidify:ExpressionResult,weak
  static class ExpressionResult
    var expr              # The expression string
    var has_dynamic       # Boolean: true if contains dynamic content that may change over time, hence needs to wrap into a closure
    var has_dangerous     # Boolean: true if contains dangerous code, i.e. code that creates new instances so it shouldn't be called at each tick but only at initialization
    var has_computation   # Boolean: true if contains operators (computation)
    var return_type       # Int: result type number from SymbolEntry constants
    var instance_for_validation  # Instance object for validation (nil by default)
    
    def init(expr, has_dynamic, has_dangerous, has_computation, return_type, instance_for_validation)
      self.expr = (expr != nil) ? expr : ""
      self.has_dynamic = bool(has_dynamic)
      self.has_dangerous = bool(has_dangerous)
      self.has_computation = bool(has_computation)
      self.return_type = (return_type != nil) ? return_type : 12 #-animation_dsl._symbol_entry.TYPE_VARIABLE-#
      self.instance_for_validation = instance_for_validation  # nil by default
    end
    
    # Check if this expression needs closure wrapping
    def needs_closure()
      return self.has_dynamic
    end

    # Check if this expression needs function wrapping
    def needs_function()
      return self.has_dynamic
    end
    
    # String representation for debugging
    def tostring()
      var instance_str = (self.instance_for_validation != nil) ? f"instance={classname(self.instance_for_validation)}" : "instance=nil"
      # var type_str = self._type_to_string(self.return_type)
      # return f"ExpressionResult(expr='{self.expr}', dynamic={self.has_dynamic}, dangerous={self.has_dangerous}, comp={self.has_computation}, type={type_str}, {instance_str})"
      return f"ExpressionResult(expr='{self.expr}', dynamic={self.has_dynamic}, dangerous={self.has_dangerous}, comp={self.has_computation}, type={self.return_type}, {instance_str})"
    end
    
    # # Helper method to convert type number to string for debugging
    # def _type_to_string(type_num)
    #   if type_num == 1 #-animation_dsl._symbol_entry.TYPE_PALETTE_CONSTANT-# return "palette_constant"
    #   elif type_num == 2 #-animation_dsl._symbol_entry.TYPE_PALETTE-# return "palette"
    #   elif type_num == 3 #-animation_dsl._symbol_entry.TYPE_CONSTANT-# return "constant"
    #   elif type_num == 4 #-animation_dsl._symbol_entry.TYPE_MATH_FUNCTION-# return "math_function"
    #   elif type_num == 5 #-animation_dsl._symbol_entry.TYPE_USER_FUNCTION-# return "user_function"
    #   elif type_num == 6 #-animation_dsl._symbol_entry.TYPE_VALUE_PROVIDER_CONSTRUCTOR-# return "value_provider_constructor"
    #   elif type_num == 7 #-animation_dsl._symbol_entry.TYPE_VALUE_PROVIDER-# return "value_provider"
    #   elif type_num == 8 #-animation_dsl._symbol_entry.TYPE_ANIMATION_CONSTRUCTOR-# return "animation_constructor"
    #   elif type_num == 9 #-animation_dsl._symbol_entry.TYPE_ANIMATION-# return "animation"
    #   elif type_num == 10 #-animation_dsl._symbol_entry.TYPE_COLOR_CONSTRUCTOR-# return "color_constructor"
    #   elif type_num == 11 #-animation_dsl._symbol_entry.TYPE_COLOR-# return "color"
    #   elif type_num == 12 #-animation_dsl._symbol_entry.TYPE_VARIABLE-# return "variable"
    #   elif type_num == 13 #-animation_dsl._symbol_entry.TYPE_SEQUENCE-# return "sequence"
    #   elif type_num == 14 #-animation_dsl._symbol_entry.TYPE_TEMPLATE-# return "template"
    #   else return f"unknown({type_num})"
    #   end
    # end
    
    # Static method to combine expression results
    # Takes an expression string and 1-2 ExpressionResult parameters (checks for nil)
    static def combine(expr_str, result1, result2)
      var has_dynamic = false
      var has_dangerous = false
      var has_computation = true  # If we're combining, it means there's an operator
      var return_type = 12 #-animation_dsl._symbol_entry.TYPE_VARIABLE-#  # Default to variable for composite expressions
      
      # Combine flags from all non-nil results
      if result1 != nil
        has_dynamic = has_dynamic || result1.has_dynamic
        has_dangerous = has_dangerous || result1.has_dangerous
        has_computation = has_computation || result1.has_computation
      end
      
      if result2 != nil
        has_dynamic = has_dynamic || result2.has_dynamic
        has_dangerous = has_dangerous || result2.has_dangerous
        has_computation = has_computation || result2.has_computation
      end
      
      # Compute the new return type
      # For composite expressions (combining two results), typically revert to TYPE_VARIABLE
      # unless both operands are the same specific type
      if result1 != nil && result2 != nil
        # If both operands have the same specific type, preserve it
        if result1.return_type == result2.return_type && result1.return_type != 12 #-animation_dsl._symbol_entry.TYPE_VARIABLE-#
          return_type = result1.return_type
        else
          # Different types or one is variable -> result is variable
          return_type = 12 #-animation_dsl._symbol_entry.TYPE_VARIABLE-#
        end
      elif result1 != nil
        # Only one operand (unary operation) - preserve its type unless it's composite
        return_type = has_computation ? 12 #-animation_dsl._symbol_entry.TYPE_VARIABLE-# : result1.return_type
      elif result2 != nil
        # Only one operand (unary operation) - preserve its type unless it's composite
        return_type = has_computation ? 12 #-animation_dsl._symbol_entry.TYPE_VARIABLE-# : result2.return_type
      end
      
      return _class(expr_str, has_dynamic #-has_dynamic-#, has_dangerous #-has_dangerous-#, has_computation #-has_computation-#, return_type, nil)
    end
    
    # Create a simple literal result (no dynamic elements)
    static def literal(expr, return_type, instance_for_validation)
      return _class(expr, false #-has_dynamic-#, false #-has_dangerous-#, false #-has_computation-#, return_type, instance_for_validation)
    end
    
    # Create a function call result (dynamic=true, dangerous=true)
    static def function_call(expr, return_type, instance_for_validation)
      return _class(expr, true #-has_dynamic-#, false #-has_dangerous-#, false #-has_computation-#, return_type, instance_for_validation)
    end
    
    # Create a constructor call result (dynamic=false, dangerous=true)
    static def constructor_call(expr, return_type, instance_for_validation)
      return _class(expr, false #-has_dynamic-#, true #-has_dangerous-#, false #-has_computation-#, return_type, instance_for_validation)
    end
    
    # Create a variable reference result (dynamic=true, dangerous=false)
    static def variable_ref(expr, return_type, instance_for_validation)
      return _class(expr, true #-has_dynamic-#, false #-has_dangerous-#, false #-has_computation-#, return_type, instance_for_validation)
    end
    
    # Create a property access result (dynamic=true, dangerous=false)
    static def property_access(expr, return_type, instance_for_validation)
      return _class(expr, true #-has_dynamic-#, false #-has_dangerous-#, false #-has_computation-#, return_type, instance_for_validation)
    end
  end
  
  def init(pull_lexer)
    import animation_dsl
    
    # Only support pull lexer interface now
    self.pull_lexer = pull_lexer
    self.output = []
    self.warnings = []  # Separate array for warnings
    self.run_statements = []
    self.strip_initialized = false  # Track if strip was initialized
    self.symbol_table = animation_dsl._symbol_table()  # Enhanced symbol cache with built-in detection
    self.indent_level = 0  # Track current indentation level
    self.has_template_calls = false  # Track if we have template calls
    
    # Note: Special functions like 'log' are now auto-discovered dynamically by the symbol table
  end
  
  # Get current indentation string
  def get_indent()
    return "  " * (self.indent_level + 1)  # Base indentation is 2 spaces
  end
  
  # Helper method to process simple value assignments with symbol table tracking
  # Consolidates duplicate code from process_color and process_animation
  def _process_simple_value_assignment(name, context, symbol_create_method)
    # Check if this is a simple identifier reference before processing
    var current_tok = self.current()
    var is_simple_identifier = (current_tok != nil && current_tok.type == 1 #-animation_dsl.Token.IDENTIFIER-# && 
                                (self.peek() == nil || self.peek().type != 24 #-animation_dsl.Token.LEFT_PAREN-#))
    var ref_name = is_simple_identifier ? current_tok.value : nil
    
    # Regular value assignment
    var value_result = self.process_value(context)
    var inline_comment = self.collect_inline_comment()
    self.add(f"var {name}_ = {value_result.expr}{inline_comment}")
    
    # If this is an identifier reference to another object in our symbol table,
    # add this name to the symbol table as well for compile-time validation
    if is_simple_identifier && ref_name != nil && self.symbol_table.contains(ref_name)
      var ref_entry = self.symbol_table.get(ref_name)
      # Only copy actual instances, not just markers
      if ref_entry != nil && ref_entry.instance != nil
        symbol_create_method(name, ref_entry.instance)
      else
        symbol_create_method(name, nil)
      end
    else
      # Add simple object to symbol table with a marker
      symbol_create_method(name, nil)
    end
  end
  
  # Helper method to process user function calls (user.function_name())
  def _process_user_function_call(func_name)
    # Check if this is a function call (user.function_name())
    if self.current() != nil && self.current().type == 24 #-LEFT_PAREN-#
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
  
  # Helper method to unwrap animation.resolve() calls
  # Takes an expression like "animation.resolve(strip_len_)" and returns "strip_len_"
  # Returns nil if the expression doesn't match the pattern or if the unwrapped part isn't a valid identifier
  def _unwrap_resolve(expr)
    import string
    
    # Check if expression starts with "animation.resolve(" and ends with ")"
    if string.find(expr, "animation.resolve(") == 0 && expr[-1] == ')'
      # Extract the content between parentheses
      var start_pos = size("animation.resolve(")
      var end_pos = size(expr) - 1  # Position of the closing parenthesis
      var inner_expr = expr[start_pos..end_pos-1]
      
      # Check if the inner expression looks like a valid identifier
      # It should contain only letters, digits, and underscores, and not be empty
      if size(inner_expr) > 0 && self._is_valid_identifier(inner_expr)
        return inner_expr
      end
    end
    
    return nil
  end
  
  # Helper method to check if a string is a valid identifier
  def _is_valid_identifier(text)
    import string
    
    if size(text) == 0
      return false
    end
    
    # First character must be letter or underscore
    var first_char = text[0]
    if !((first_char >= 'a' && first_char <= 'z') || 
         (first_char >= 'A' && first_char <= 'Z') || 
         first_char == '_')
      return false
    end
    
    # Remaining characters must be letters, digits, or underscores
    for i: 1..size(text)-1
      var ch = text[i]
      if !((ch >= 'a' && ch <= 'z') || 
           (ch >= 'A' && ch <= 'Z') || 
           (ch >= '0' && ch <= '9') || 
           ch == '_')
        return false
      end
    end
    
    return true
  end
  
  # Helper method to determine the return type of a function call
  def _determine_function_return_type(entry)
    if entry != nil
      if entry.type == 8 #-animation_dsl._symbol_entry.TYPE_ANIMATION_CONSTRUCTOR-# || entry.type == 9 #-animation_dsl._symbol_entry.TYPE_ANIMATION-#
        return 9 #-animation_dsl._symbol_entry.TYPE_ANIMATION-#
      elif entry.type == 10 #-animation_dsl._symbol_entry.TYPE_COLOR_CONSTRUCTOR-# || entry.type == 11 #-animation_dsl._symbol_entry.TYPE_COLOR-#
        return 11 #-animation_dsl._symbol_entry.TYPE_COLOR-#
      elif entry.type == 6 #-animation_dsl._symbol_entry.TYPE_VALUE_PROVIDER_CONSTRUCTOR-# || entry.type == 7 #-animation_dsl._symbol_entry.TYPE_VALUE_PROVIDER-#
        return 7 #-animation_dsl._symbol_entry.TYPE_VALUE_PROVIDER-#
      elif entry.type == 1 #-animation_dsl._symbol_entry.TYPE_PALETTE_CONSTANT-# || entry.type == 2 #-animation_dsl._symbol_entry.TYPE_PALETTE-#
        return 2 #-animation_dsl._symbol_entry.TYPE_PALETTE-#
      elif entry.type == 4 #-animation_dsl._symbol_entry.TYPE_MATH_FUNCTION-#
        return 12 #-animation_dsl._symbol_entry.TYPE_VARIABLE-#  # Math functions return numeric values
      elif entry.type == 5 #-animation_dsl._symbol_entry.TYPE_USER_FUNCTION-# || entry.type == 14 #-animation_dsl._symbol_entry.TYPE_TEMPLATE-#
        return 12 #-animation_dsl._symbol_entry.TYPE_VARIABLE-#  # User functions and templates can return anything
      end
    end
    return 12 #-animation_dsl._symbol_entry.TYPE_VARIABLE-#  # Default fallback
  end
  
  # Helper method to create symbol entry based on return type number
  def _create_symbol_by_return_type(name, return_type, instance)
    if return_type == 9 #-animation_dsl._symbol_entry.TYPE_ANIMATION-#
      return self.symbol_table.create_animation(name, instance)
    elif return_type == 11 #-animation_dsl._symbol_entry.TYPE_COLOR-#
      return self.symbol_table.create_color(name, instance)
    elif return_type == 7 #-animation_dsl._symbol_entry.TYPE_VALUE_PROVIDER-#
      return self.symbol_table.create_value_provider(name, instance)
    elif return_type == 2 #-animation_dsl._symbol_entry.TYPE_PALETTE-#
      return self.symbol_table.create_palette(name, instance)
    elif return_type == 13 #-animation_dsl._symbol_entry.TYPE_SEQUENCE-#
      return self.symbol_table.create_sequence(name)
    elif return_type == 14 #-animation_dsl._symbol_entry.TYPE_TEMPLATE-#
      return self.symbol_table.create_template(name, nil)
    else  # TYPE_VARIABLE or any other type
      return self.symbol_table.create_variable(name)
    end
  end
  
  # Helper method to determine the return type of a symbol reference
  def _determine_symbol_return_type(entry)
    if entry.type == 9 #-animation_dsl._symbol_entry.TYPE_ANIMATION-# || entry.type == 8 #-animation_dsl._symbol_entry.TYPE_ANIMATION_CONSTRUCTOR-#
      return 9 #-animation_dsl._symbol_entry.TYPE_ANIMATION-#
    elif entry.type == 11 #-animation_dsl._symbol_entry.TYPE_COLOR-# || entry.type == 10 #-animation_dsl._symbol_entry.TYPE_COLOR_CONSTRUCTOR-#
      return 11 #-animation_dsl._symbol_entry.TYPE_COLOR-#
    elif entry.type == 7 #-animation_dsl._symbol_entry.TYPE_VALUE_PROVIDER-# || entry.type == 6 #-animation_dsl._symbol_entry.TYPE_VALUE_PROVIDER_CONSTRUCTOR-#
      return 7 #-animation_dsl._symbol_entry.TYPE_VALUE_PROVIDER-#
    elif entry.type == 2 #-animation_dsl._symbol_entry.TYPE_PALETTE-# || entry.type == 1 #-animation_dsl._symbol_entry.TYPE_PALETTE_CONSTANT-#
      return 2 #-animation_dsl._symbol_entry.TYPE_PALETTE-#
    elif entry.type == 3 #-animation_dsl._symbol_entry.TYPE_CONSTANT-#
      return 12 #-animation_dsl._symbol_entry.TYPE_VARIABLE-#  # Constants are numeric values
    elif entry.type == 12 #-animation_dsl._symbol_entry.TYPE_VARIABLE-#
      return 12 #-animation_dsl._symbol_entry.TYPE_VARIABLE-#
    elif entry.type == 13 #-animation_dsl._symbol_entry.TYPE_SEQUENCE-#
      return 13 #-animation_dsl._symbol_entry.TYPE_SEQUENCE-#
    elif entry.type == 14 #-animation_dsl._symbol_entry.TYPE_TEMPLATE-#
      return 14 #-animation_dsl._symbol_entry.TYPE_TEMPLATE-#
    else
      return 12 #-animation_dsl._symbol_entry.TYPE_VARIABLE-#  # Default fallback
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
      
      # Add warnings as comments if any exist
      if self.has_warnings()
        self.add("")
        self.add("# Compilation warnings:")
        for warning : self.warnings
          self.add(f"# {warning}")
        end
      end
      
      return self.join_output()
    except .. as e, msg
      self.error(f"Transpilation failed: {msg}")
    end
  end
  
  # Transpile template body (similar to main transpile but without imports/engine start)
  def transpile_template_body()
    try
      # Process all statements in template body until we hit the closing brace
      var brace_depth = 0
      while !self.at_end()
        var tok = self.current()
        
        # Check for template end condition
        if tok != nil && tok.type == 27 #-animation_dsl.Token.RIGHT_BRACE-# && brace_depth == 0
          # This is the closing brace of the template - stop processing
          break
        end
        
        # Track brace depth for nested braces
        if tok != nil && tok.type == 26 #-animation_dsl.Token.LEFT_BRACE-#
          brace_depth += 1
        elif tok != nil && tok.type == 27 #-animation_dsl.Token.RIGHT_BRACE-#
          brace_depth -= 1
        end
        
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
      
      return self.join_output()
    except .. as e, msg
      self.error(f"Template body transpilation failed: {msg}")
    end
  end
  
  # Process statements - simplified approach
  def process_statement()
    var tok = self.current()
    if tok == nil  # EOF token removed - nil indicates end of file
      return
    end
    
    # Handle comments - preserve them in generated code
    if tok.type == 37 #-animation_dsl.Token.COMMENT-#
      self.add(tok.value)  # Add comment as-is to output
      self.next()
      return
    end
    
    # Skip whitespace (newlines)
    if tok.type == 35 #-animation_dsl.Token.NEWLINE-#
      self.next()
      return
    end
    
    # Handle keywords
    if tok.type == 0 #-animation_dsl.Token.KEYWORD-#
      if tok.value == "strip"
        # Strip directive is temporarily disabled but remains a reserved keyword
        self.error("'strip' directive is temporarily disabled. Strip configuration is handled automatically.")
        self.skip_statement()
        return
      elif tok.value == "template"
        self.process_template()
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
        elif tok.value == "run"
          self.process_run()
        elif tok.value == "import"
          self.process_import()
        elif tok.value == "on"
          self.process_event_handler()
        elif tok.value == "berry"
          self.process_berry_code_block()
        else
          self.error(f"Unknown keyword '{tok.value}'.")
          self.skip_statement()
        end
      end
    elif tok.type == 1 #-animation_dsl.Token.IDENTIFIER-#
      # For property assignments, ensure strip is initialized
      if !self.strip_initialized
        self.generate_default_strip_initialization()
      end
      
      # Check if this is a log function call
      if tok.value == "log" && self.peek() != nil && self.peek().type == 24 #-animation_dsl.Token.LEFT_PAREN-#
        self.process_standalone_log()
      else
        # Check if this is a property assignment (identifier.property = value)
        self.process_property_assignment()
      end
    else
      self.error(f"Unexpected token '{tok.value}'.")
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
    if (tok.type == 0 #-animation_dsl.Token.KEYWORD-# || tok.type == 1 #-animation_dsl.Token.IDENTIFIER-#) && 
       self.peek() != nil && self.peek().type == 24 #-animation_dsl.Token.LEFT_PAREN-#
      
      # This is a function call - check if it's a user function or built-in color provider
      var func_name = tok.value
      self.next()  # consume function name
      
      var inline_comment = ""
      # Check for inline comment before opening paren
      if self.current() != nil && self.current().type == 37 #-animation_dsl.Token.COMMENT-#
        inline_comment = "  " + self.current().value
        self.next()
      end
      
      # Get symbol table entry for this function
      var entry = self.symbol_table.get(func_name)
      
      # Check if this is a template call first
      if entry != nil && entry.type == 14 #-animation_dsl._symbol_entry.TYPE_TEMPLATE-#
        # This is a template call - validate and process
        var args_str = self.process_function_arguments(false)
        
        # Validate template call arguments
        var provided_args = args_str != "" ? self._split_function_arguments(args_str) : []
        var template_info = entry.instance  # This should contain parameter info
        if template_info != nil && template_info.contains("params")
          var expected_params = template_info["params"]
          var param_types = template_info.find("param_types", {})
          
          if !self._validate_template_call_arguments(func_name, provided_args, expected_params, param_types)
            self.skip_statement()
            return
          end
        end
        
        var full_args = args_str != "" ? f"engine, {args_str}" : "engine"
        self.add(f"var {name}_ = {func_name}_template({full_args}){inline_comment}")
        
        # Register in symbol table as color instance
        self.symbol_table.create_color(name, nil)
      elif entry != nil && entry.type == 5 #-animation_dsl._symbol_entry.TYPE_USER_FUNCTION-#
        # This is a user function call - use positional parameters with engine as first argument
        var args = self.process_function_arguments(false)
        var full_args = args != "" ? f"engine, {args}" : "engine"
        self.add(f"var {name}_ = animation.get_user_function('{func_name}')({full_args}){inline_comment}")
        
        # Track this symbol in our symbol table as a color instance (user function result)
        self.symbol_table.create_color(name, nil)
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
          self.symbol_table.create_color(name, instance)
        end
        
        # Process named arguments with validation
        self._process_named_arguments_for_color_provider(f"{name}_", func_name)
      end
    else
      # Use helper method to process simple value assignment
      self._process_simple_value_assignment(name, self.CONTEXT_COLOR, / name, instance -> self.symbol_table.create_color(name, instance))
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
    var is_tuple_syntax = self.current() != nil && self.current().type == 24 #-animation_dsl.Token.LEFT_PAREN-#
    
    while !self.at_end() && !self.check_right_bracket()
      self.skip_whitespace_including_newlines()
      
      if self.check_right_bracket()
        break
      end
      
      if is_tuple_syntax
        # Parse tuple (value, color) - original syntax
        # Check if we accidentally have alternative syntax in tuple mode
        if self.current() != nil && self.current().type != 24 #-animation_dsl.Token.LEFT_PAREN-#
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
        if self.current() != nil && self.current().type == 24 #-animation_dsl.Token.LEFT_PAREN-#
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
      
      if self.current() != nil && self.current().type == 30 #-animation_dsl.Token.COMMA-#
        self.next()  # skip comma
        
        # Check for comment immediately after comma
        if self.current() != nil && self.current().type == 37 #-animation_dsl.Token.COMMENT-#
          entry_comment = self.current().value
          self.next()
        end
        
        # Skip remaining whitespace/newlines
        while !self.at_end()
          var tok = self.current()
          if tok != nil && tok.type == 35 #-animation_dsl.Token.NEWLINE-#
            self.next()
          else
            break
          end
        end
      elif self.current() != nil && self.current().type == 35 #-animation_dsl.Token.NEWLINE-#
        # Newline acts as entry separator - skip it and continue
        self.next()  # skip newline
        self.skip_whitespace_including_newlines()
      elif !self.check_right_bracket()
        # For the last entry, check if there's a comment before the closing bracket
        if self.current() != nil && self.current().type == 37 #-animation_dsl.Token.COMMENT-#
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
    
    # Register palette in symbol table
    self.symbol_table.create_palette(name, nil)
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
    if (tok.type == 0 #-animation_dsl.Token.KEYWORD-# || tok.type == 1 #-animation_dsl.Token.IDENTIFIER-#) && 
       self.peek() != nil && self.peek().type == 24 #-animation_dsl.Token.LEFT_PAREN-#
      
      # This is a function call - check if it's a user function or built-in
      var func_name = tok.value
      self.next()  # consume function name
      
      var inline_comment = ""
      # Check for inline comment before opening paren
      if self.current() != nil && self.current().type == 37 #-animation_dsl.Token.COMMENT-#
        inline_comment = "  " + self.current().value
        self.next()
      end
      
      # Get symbol table entry for this function
      var entry = self.symbol_table.get(func_name)
      
      # Check if this is a template call first
      if entry != nil && entry.type == 14 #-animation_dsl._symbol_entry.TYPE_TEMPLATE-#
        # This is a template call - treat like user function
        var args = self.process_function_arguments(false)
        var full_args = args != "" ? f"engine, {args}" : "engine"
        self.add(f"var {name}_ = {func_name}_template({full_args}){inline_comment}")
        
        # Register in symbol table as animation instance
        self.symbol_table.create_animation(name, nil)
      elif entry != nil && entry.type == 5 #-animation_dsl._symbol_entry.TYPE_USER_FUNCTION-#
        # This is a user function call - use positional parameters with engine as first argument
        var args = self.process_function_arguments(false)
        var full_args = args != "" ? f"engine, {args}" : "engine"
        self.add(f"var {name}_ = animation.get_user_function('{func_name}')({full_args}){inline_comment}")
        
        # Track this symbol in our symbol table as animation instance (user function result)
        self.symbol_table.create_animation(name, nil)
      else
        # Built-in functions use the new engine-first + named parameters pattern
        # Validate that the factory function creates an animation instance at transpile time
        # Use symbol table's dynamic detection with type checking for animation constructors only
        if entry == nil || entry.type != 8 #-animation_dsl._symbol_entry.TYPE_ANIMATION_CONSTRUCTOR-#
          self.error(f"Animation factory function '{func_name}' does not exist or does not create an instance of animation.animation class. Check the function name and ensure it returns an animation object.")
          self.skip_statement()
          return
        end
        
        # Generate the base function call immediately
        self.add(f"var {name}_ = animation.{func_name}(engine){inline_comment}")
        
        # Track this symbol in our symbol table
        var instance = self._create_instance_for_validation(func_name)
        if instance != nil
          self.symbol_table.create_animation(name, instance)
        end
        
        # Process named arguments with validation
        self._process_named_arguments_for_animation(f"{name}_", func_name)
      end
    else
      # Use helper method to process simple value assignment
      self._process_simple_value_assignment(name, self.CONTEXT_ANIMATION, / name, instance -> self.symbol_table.create_animation(name, instance))
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
    
    var value_result = self.process_value(self.CONTEXT_VARIABLE)
    var inline_comment = self.collect_inline_comment()
    # Add to symbol table using appropriate method based on return type
    var local_entry = self._create_symbol_by_return_type(name, value_result.return_type, value_result.instance_for_validation)
    var local_ref = (local_entry != nil) ? local_entry.get_reference() : f"{name}_"
    self.add(f"var {local_ref} = {value_result.expr}{inline_comment}")
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
    
    # First pass: collect all parameters with validation
    var params = []
    var param_types = {}
    var param_names_seen = {}  # Track duplicate parameter names
    
    while !self.at_end() && !self.check_right_brace()
      self.skip_whitespace_including_newlines()
      
      if self.check_right_brace()
        break
      end
      
      var tok = self.current()
      
      if tok != nil && tok.type == 0 #-animation_dsl.Token.KEYWORD-# && tok.value == "param"
        # Process parameter declaration
        self.next()  # skip 'param'
        var param_name = self.expect_identifier()
        
        # Validate parameter name
        if !self._validate_template_parameter_name(param_name, param_names_seen)
          self.skip_statement()
          return
        end
        
        # Check for optional type annotation
        var param_type = nil
        if self.current() != nil && self.current().type == 0 #-animation_dsl.Token.KEYWORD-# && self.current().value == "type"
          self.next()  # skip 'type'
          param_type = self.expect_identifier()
          
          # Validate type annotation
          if !self._validate_template_parameter_type(param_type)
            self.skip_statement()
            return
          end
        end
        
        # Add parameter to collections
        params.push(param_name)
        param_names_seen[param_name] = true
        if param_type != nil
          param_types[param_name] = param_type
        end
        
        # Skip optional newline after parameter
        if self.current() != nil && self.current().type == 35 #-animation_dsl.Token.NEWLINE-#
          self.next()
        end
      else
        # Found non-param statement, break to collect body
        break
      end
    end
    
    # Generate Berry function for this template using direct pull-lexer approach
    self.generate_template_function_direct(name, params, param_types)
    
    # Add template to symbol table with parameter information
    var template_info = {
      "params": params,
      "param_types": param_types
    }
    self.symbol_table.create_template(name, template_info)
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
    
    # Track sequence in symbol table
    self.symbol_table.create_sequence(name)
    
    # Check for second syntax: sequence name repeat N times { ... } or sequence name forever { ... }
    var is_repeat_syntax = false
    var repeat_count = "1"
    
    var current_tok = self.current()
    if current_tok != nil && current_tok.type == 0 #-animation_dsl.Token.KEYWORD-#
      if current_tok.value == "repeat"
        is_repeat_syntax = true
        self.next()  # skip 'repeat'
        
        # Parse repeat count: either number or "forever"
        var tok_after_repeat = self.current()
        if tok_after_repeat != nil && tok_after_repeat.type == 0 #-animation_dsl.Token.KEYWORD-# && tok_after_repeat.value == "forever"
          self.next()  # skip 'forever'
          repeat_count = "-1"  # -1 means forever
        else
          var count_result = self.process_value(self.CONTEXT_REPEAT_COUNT)
          self.expect_keyword("times")
          repeat_count = count_result.expr
        end
      elif current_tok.value == "forever"
        # New syntax: sequence name forever { ... } (repeat is optional)
        is_repeat_syntax = true
        self.next()  # skip 'forever'
        repeat_count = "-1"  # -1 means forever
      end
    elif current_tok != nil && current_tok.type == 2 #-animation_dsl.Token.NUMBER-#
      # New syntax: sequence name N times { ... } (repeat is optional)
      is_repeat_syntax = true
      var count_result = self.process_value(self.CONTEXT_REPEAT_COUNT)
      self.expect_keyword("times")
      repeat_count = count_result.expr
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
    if tok == nil  # EOF token removed - nil indicates end of file
      return
    end
    
    # Handle comments - preserve them in generated code with proper indentation
    if tok.type == 37 #-animation_dsl.Token.COMMENT-#
      self.add(self.get_indent() + tok.value)  # Add comment with fluent indentation
      self.next()
      return
    end
    
    # Skip whitespace (newlines) - we specifically don't call skip_whitespace_including_newlines()
    if tok.type == 35 #-animation_dsl.Token.NEWLINE-#
      self.next()
      return
    end
    
    if tok.type == 0 #-animation_dsl.Token.KEYWORD-# && tok.value == "play"
      self.process_play_statement_fluent()
      
    elif tok.type == 0 #-animation_dsl.Token.KEYWORD-# && tok.value == "wait"
      self.process_wait_statement_fluent()
      
    elif tok.type == 1 #-animation_dsl.Token.IDENTIFIER-# && tok.value == "log"
      self.process_log_statement_fluent()
      
    elif tok.type == 0 #-animation_dsl.Token.KEYWORD-# && tok.value == "restart"
      self.process_restart_statement_fluent()
      
    elif tok.type == 0 #-animation_dsl.Token.KEYWORD-# && tok.value == "repeat"
      self.process_repeat_statement_fluent()
      
    elif tok.type == 1 #-animation_dsl.Token.IDENTIFIER-#
      # Check if this is a property assignment (identifier.property = value)
      if self.peek() != nil && self.peek().type == 33 #-animation_dsl.Token.DOT-#
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
    var value_result = self.process_value(self.CONTEXT_PROPERTY)
    var inline_comment = self.collect_inline_comment()
    
    # Create assignment step using fluent style
    var closure_code = f"def (engine) {object_name}_.{property_name} = {value_result.expr} end"
    self.add(f"{self.get_indent()}.push_closure_step({closure_code}){inline_comment}")
  end
  
  # Generic method to process sequence assignment with configurable target array
  def process_sequence_assignment_generic(indent, target_array)
    var object_name = self.expect_identifier()
    
    # Check if next token is a dot
    if self.current() != nil && self.current().type == 33 #-animation_dsl.Token.DOT-#
      self.next()  # skip '.'
      var property_name = self.expect_identifier()
      
      # Validate parameter if we have this object in our symbol table
      if self.symbol_table.contains(object_name)
        var entry = self.symbol_table.get(object_name)
        
        # Only validate parameters for actual instances, not sequence markers
        if entry != nil && entry.instance != nil
          var class_name = classname(entry.instance)
          
          # Use the existing parameter validation logic
          self._validate_single_parameter(class_name, property_name, entry.instance)
        elif entry != nil && entry.type == 13 #-animation_dsl._symbol_entry.TYPE_SEQUENCE-#
          # This is a sequence marker - sequences don't have properties
          self.error(f"Sequences like '{object_name}' do not have properties. Property assignments are only valid for animations and color providers.")
          return
        end
      end
      
      self.expect_assign()
      var value_result = self.process_value(self.CONTEXT_PROPERTY)
      var inline_comment = self.collect_inline_comment()
      
      # Generate assignment step with closure
      # The closure receives the engine as parameter and performs the assignment
      var object_ref = self.symbol_table.get_reference(object_name)
      
      # Create closure that performs the assignment
      var closure_code = f"def (engine) {object_ref}.{property_name} = {value_result.expr} end"
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
    if current_tok != nil && (current_tok.type == 1 #-animation_dsl.Token.IDENTIFIER-# || current_tok.type == 0 #-animation_dsl.Token.KEYWORD-#) &&
       self.peek() != nil && self.peek().type == 24 #-animation_dsl.Token.LEFT_PAREN-#
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
    if self.current() != nil && self.current().type == 0 #-animation_dsl.Token.KEYWORD-# && self.current().value == "for"
      self.next()  # skip 'for'
      duration = self.process_time_value()
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
    elif context_type == self.CONTEXT_EXPRESSION
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
    if message_tok == nil || message_tok.type != 3 #-animation_dsl.Token.STRING-#
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

  # Helper method to process repeat statement using fluent style
  def process_repeat_statement_fluent()
    self.next()  # skip 'repeat'
    
    # Parse repeat count: either number or "forever"
    var repeat_count = "1"
    var tok_after_repeat = self.current()
    if tok_after_repeat != nil && tok_after_repeat.type == 0 #-animation_dsl.Token.KEYWORD-# && tok_after_repeat.value == "forever"
      self.next()  # skip 'forever'
      repeat_count = "-1"  # -1 means forever
    else
      var count_result = self.process_value(self.CONTEXT_REPEAT_COUNT)
      self.expect_keyword("times")
      repeat_count = count_result.expr
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
    if message_tok == nil || message_tok.type != 3 #-animation_dsl.Token.STRING-#
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
    if self.current() != nil && self.current().type == 24 #-animation_dsl.Token.LEFT_PAREN-#
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
      var entry = self.symbol_table.get(object_name)
      if entry != nil && entry.type == 14 #-animation_dsl._symbol_entry.TYPE_TEMPLATE-#
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
    if self.current() != nil && self.current().type == 33 #-animation_dsl.Token.DOT-#
      self.next()  # skip '.'
      var property_name = self.expect_identifier()
      
      # Validate parameter if we have this object in our symbol table
      if self.symbol_table.contains(object_name)
        var entry = self.symbol_table.get(object_name)
        
        # Only validate parameters for actual instances, not sequence markers
        if entry != nil && entry.instance != nil
          var class_name = classname(entry.instance)
          
          # Use the existing parameter validation logic
          self._validate_single_parameter(class_name, property_name, entry.instance)
        elif entry != nil && entry.type == 13 #-animation_dsl._symbol_entry.TYPE_SEQUENCE-#
          # This is a sequence marker - sequences don't have properties
          self.error(f"Sequences like '{object_name}' do not have properties. Property assignments are only valid for animations and color providers.")
        end
      end
      
      self.expect_assign()
      var value_result = self.process_value(self.CONTEXT_PROPERTY)
      var inline_comment = self.collect_inline_comment()
      
      # Use consolidated symbol resolution for property assignments
      var object_ref = self.symbol_table.get_reference(object_name)
      
      # Generate property assignment
      self.add(f"{object_ref}.{property_name} = {value_result.expr}{inline_comment}")
    else
      # Not a property assignment, skip this statement
      self.error(f"Expected property assignment for '{object_name}' but found no dot")
      self.skip_statement()
    end
  end
  
  # Process any value - unified approach
  def process_value(context)
    var result = self.process_additive_expression(context, true, false)  # true = top-level, false = not raw mode
    # Handle closure wrapping for top-level expressions (not in raw mode) only if there is computation needed
    # print(f"> process_value {context=} {result.needs_function()=} {result=}")
    if    (((context == self.CONTEXT_VARIABLE) || (context == self.CONTEXT_PROPERTY)) && result.needs_closure())
       || ((context == self.CONTEXT_REPEAT_COUNT) && result.needs_function())
      # Special handling for repeat_count context - always create simple function for property access
      if context == self.CONTEXT_REPEAT_COUNT
        # print(f">>> CONTEXT_REPEAT_COUNT")
        var closure_expr = f"def (engine) return {result.expr} end"
        # Return new ExpressionResult with closure expression but preserve return type
        return self.ExpressionResult.function_call(closure_expr, result.return_type)
      else
        # Default behavior is to wrap into `animation.create_closure_value(engine, def (engine) return <>  end)`
        var expr = f"animation.create_closure_value(engine, def (engine) return {result.expr} end)"

        if result.return_type == 9 #-animation_dsl._symbol_entry.TYPE_ANIMATION-# && !result.has_computation
          # Special case of a reference to another variable containing an animation, in such case no need for wrapping
          expr = result.expr
        end

        # Simple optimization, unwrap a single `animation.resolve()` instead of wrapping in a closure
        var unwrapped_expr = self._unwrap_resolve(result.expr)
        # print(f"{unwrapped_expr=}")
        if unwrapped_expr != nil
          expr = unwrapped_expr     # override expr
        end

        # var closure_expr = self.create_computation_closure_from_string(result.expr)
        var entry_for_closure_value = self.symbol_table.get("closure_value")
        return self.ExpressionResult.function_call(expr, entry_for_closure_value.type, entry_for_closure_value.instance)
      end
    else
      # Return the original result unchanged
      return result
    end
  end
  
  # Process palette color with strict validation
  # Only accepts predefined color names or hex color literals
  def process_palette_color()
    import animation_dsl
    var tok = self.current()
    if tok == nil
      self.error("Expected color value in palette")
      return "0xFFFFFFFF"
    end
    
    # Handle hex color literals
    if tok.type == 4 #-animation_dsl.Token.COLOR-#
      self.next()
      return self.convert_color(tok.value)
    end
    
    # Handle identifiers (color names)
    if tok.type == 1 #-animation_dsl.Token.IDENTIFIER-#
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
    var left_result = self.process_multiplicative_expression(context, is_top_level, raw_mode)
    
    while !self.at_end()
      var tok = self.current()
      if tok != nil && (tok.type == 9 #-animation_dsl.Token.PLUS-# || tok.type == 10 #-animation_dsl.Token.MINUS-#)
        var op = tok.value
        self.next()  # consume operator
        var right_result = self.process_multiplicative_expression(context, false, raw_mode)  # sub-expressions are not top-level

        # Check if either of left or right are dangerous calls, if so raise an error
        if left_result.has_dangerous || right_result.has_dangerous
          var dangerous_expr = left_result.has_dangerous ? left_result.expr : right_result.expr
          self.error(f"Expression '{dangerous_expr}' cannot be used in computed expressions. This creates a new instance at each evaluation. Use either:\n  set var_name = {dangerous_expr}()  # Single function call\n  set computed = (existing_var + 1) / 2  # Computation with existing values")
          self.skip_statement()
          return self.ExpressionResult.literal("nil")
        end

        left_result = self.ExpressionResult.combine(f"{left_result.expr} {op} {right_result.expr}", left_result, right_result)

      else
        break
      end
    end
    
    return left_result
  end
  
  # Process multiplicative expressions (* and /) - unified method
  def process_multiplicative_expression(context, is_top_level, raw_mode)
    var left_result = self.process_unary_expression(context, is_top_level, raw_mode)
    
    while !self.at_end()
      var tok = self.current()
      if tok != nil && (tok.type == 11 #-animation_dsl.Token.MULTIPLY-# || tok.type == 12 #-animation_dsl.Token.DIVIDE-#)
        var op = tok.value
        self.next()  # consume operator
        var right_result = self.process_unary_expression(context, false, raw_mode)  # sub-expressions are not top-level

        # Check if either of left or right are dangerous calls, if so raise an error
        if left_result.has_dangerous || right_result.has_dangerous
          var dangerous_expr = left_result.has_dangerous ? left_result.expr : right_result.expr
          self.error(f"Expression '{dangerous_expr}' cannot be used in computed expressions. This creates a new instance at each evaluation. Use either:\n  set var_name = {dangerous_expr}()  # Single function call\n  set computed = (existing_var + 1) / 2  # Computation with existing values")
          self.skip_statement()
          return self.ExpressionResult.literal("nil")
        end


        left_result = self.ExpressionResult.combine(f"{left_result.expr} {op} {right_result.expr}", left_result, right_result)
      else
        break
      end
    end
    
    return left_result
  end
  
  # Process unary expressions (- and +) - unified method
  def process_unary_expression(context, is_top_level, raw_mode)
    var tok = self.current()
    if tok == nil
      self.error("Expected value")
      return self.ExpressionResult.literal("nil")
    end
    
    # Handle unary minus for negative numbers
    if tok.type == 10 #-animation_dsl.Token.MINUS-#
      self.next()  # consume the minus
      var expr_result = self.process_unary_expression(context, false, raw_mode)  # sub-expressions are not top-level
      return self.ExpressionResult(f"(-{expr_result.expr})", expr_result.has_dynamic, expr_result.has_dangerous, true #-force has_computation-#, expr_result.return_type, expr_result.instance_for_validation)
    end
    
    # Handle unary plus (optional)
    if tok.type == 9 #-animation_dsl.Token.PLUS-#
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
      return self.ExpressionResult.literal("nil")
    end
    
    # Parenthesized expression
    if tok.type == 24 #-animation_dsl.Token.LEFT_PAREN-#
      self.next()  # consume '('
      var expr_result = self.process_additive_expression(context, false, raw_mode)  # parenthesized expressions are not top-level
      self.expect_right_paren()
      return self.ExpressionResult(f"({expr_result.expr})", expr_result.has_dynamic, expr_result.has_dangerous, expr_result.has_computation, expr_result.return_type, expr_result.instance_for_validation)
    end
    
    # Color value
    if tok.type == 4 #-animation_dsl.Token.COLOR-#
      self.next()
      return self.ExpressionResult.literal(self.convert_color(tok.value), 11 #-animation_dsl._symbol_entry.TYPE_COLOR-#)
    end
    
    # Time value
    if tok.type == 5 #-animation_dsl.Token.TIME-#
      return self.ExpressionResult.literal(self.process_time_value())
    end
    
    # Percentage value
    if tok.type == 6 #-animation_dsl.Token.PERCENTAGE-#
      return self.ExpressionResult.literal(str(self.process_percentage_value()))
    end
    
    # Number value
    if tok.type == 2 #-animation_dsl.Token.NUMBER-#
      var value = tok.value
      self.next()
      return self.ExpressionResult.literal(value)
    end
    
    # Boolean keywords
    if tok.type == 0 #-animation_dsl.Token.KEYWORD-# && (tok.value == "true" || tok.value == "false")
      var value = tok.value
      self.next()
      return self.ExpressionResult.literal(value)
    end
    
    # String value
    if tok.type == 3 #-animation_dsl.Token.STRING-#
      var value = tok.value
      self.next()
      return self.ExpressionResult.literal(f'"{value}"')
    end
    
    # Array literal (not supported in raw mode)
    if tok.type == 28 #-animation_dsl.Token.LEFT_BRACKET-# && !raw_mode
      var result = self.process_array_literal()
      return self.ExpressionResult.literal(result)
    end
    
    # Anthing that looks like a function call
    if (tok.type == 0 #-animation_dsl.Token.KEYWORD-# || tok.type == 1 #-animation_dsl.Token.IDENTIFIER-#) && 
       self.peek() != nil && self.peek().type == 24 #-animation_dsl.Token.LEFT_PAREN-#
      var func_name = tok.value
      var entry = self.symbol_table.get(func_name)
      
      # Check if the identifier exists
      if entry == nil
        self.error(f"Unknown function or identifier '{func_name}'. Make sure it's defined before use.")
        self.skip_statement()
        return self.ExpressionResult.literal("nil")
      end
      
      # Special handling for user fonction function_name() calls (without 'user.' prefix)
      if entry.is_user_function()
        self.next()
        var result = self._process_user_function_call(func_name)
        return self.ExpressionResult.function_call(result)
      end

      # In raw mode, handle function calls differently
      if raw_mode
        self.next()
        
        # Check if this is a mathematical function
        if entry != nil && entry.type == 4 #-animation_dsl._symbol_entry.TYPE_MATH_FUNCTION-#
          var args = self.process_function_arguments(true)
          var result = self.ExpressionResult.function_call(f"{entry.get_reference()}({args})")
        end
        
        # Check if this is a template call
        if entry != nil && entry.type == 14 #-animation_dsl._symbol_entry.TYPE_TEMPLATE-#
          var args = self.process_function_arguments(true)
          var full_args = args != "" ? f"engine, {args}" : "engine"
          return self.ExpressionResult.function_call(f"{func_name}_template({full_args})")
        end
        
        # For other functions, this shouldn't happen in expression context
        self.error(f"Function '{func_name}' not supported in expression context")
        return self.ExpressionResult.literal("nil")
      else
        # Regular mode - function calls are marked as having functions
        # Check if this is a simple function call first
        if !entry.takes_named_args()
          var result = self.process_function_call(context)
          var return_type = self._determine_function_return_type(entry)
          return self.ExpressionResult.function_call(result, return_type, entry.instance)
        # Check if this is a nested function call or a variable assignment with named parameters
        elif context == self.CONTEXT_ARGUMENT || context == self.CONTEXT_PROPERTY || context == self.CONTEXT_VARIABLE
          var result = self.process_nested_function_call()
          var return_type = self._determine_function_return_type(entry)
          return self.ExpressionResult.constructor_call(result, return_type, entry.instance)
        else
          var result = self.process_function_call(context)
          var return_type = self._determine_function_return_type(entry)
          return self.ExpressionResult.constructor_call(result, return_type, entry.instance)
        end
      end
    end
    
    # Identifier - could be color, animation, variable, or object property reference
    if tok.type == 1 #-animation_dsl.Token.IDENTIFIER-#
      var name = tok.value
      var entry = self.symbol_table.get(name)

      if entry == nil
        self.error(f"Unknown identifier '{name}'. Make sure it's defined before use.")
        self.skip_statement()
        return self.ExpressionResult.literal("nil")
      end
      self.next()

      # Check if this is an object property reference (identifier.property)
      if self.current() != nil && self.current().type == 33 #-animation_dsl.Token.DOT-#
        self.next()  # consume '.'
        var property_name = self.expect_identifier()
        
        # Property access - mark as having properties
        var property_expr = f"{name}.{property_name}"
        
        # Validate that the property exists on the referenced object (skip in raw mode)
        if !raw_mode && self.symbol_table.contains(name)
          # Only validate parameters for actual instances, not sequence markers
          if entry != nil && entry.instance != nil
            var class_name = classname(entry.instance)
            self._validate_single_parameter(class_name, property_name, entry.instance)
          elif entry != nil && entry.type == 13 #-animation_dsl._symbol_entry.TYPE_SEQUENCE-#
            # This is a sequence marker - sequences don't have properties
            self.error(f"Sequences like '{name}' do not have properties. Property references are only valid for animations and color providers.")
            return self.ExpressionResult.literal("nil")
          end
        end
        
        # Use consolidated symbol resolution for the object reference
        var object_ref = self.symbol_table.get_reference(name)
        
        return self.ExpressionResult.property_access(f"{object_ref}.{property_name}", "variable")
      end
      
      if  entry.type == 11 #-animation_dsl._symbol_entry.TYPE_COLOR-# ||
          entry.type == 2 #-animation_dsl._symbol_entry.TYPE_PALETTE-# ||
          entry.type == 1 #-animation_dsl._symbol_entry.TYPE_PALETTE_CONSTANT-# ||
          entry.type == 3 #-animation_dsl._symbol_entry.TYPE_CONSTANT-#
        return self.ExpressionResult.literal(entry.get_reference(), 11 #-animation_dsl._symbol_entry.TYPE_COLOR-#)
      end

      # Regular identifier - check if it's a variable reference
      var ref = self.symbol_table.get_reference(name)
      var return_type = self._determine_symbol_return_type(entry)    # compute the return type based on entry
      if entry.type == 7 #-animation_dsl._symbol_entry.TYPE_VALUE_PROVIDER-# ||
         entry.type == 12 #-animation_dsl._symbol_entry.TYPE_VARIABLE-#
        # Special case for simple value providers, wrap in animation.resolve()
        return self.ExpressionResult.function_call(f"animation.resolve({ref})", return_type)
      end
      return self.ExpressionResult.variable_ref(ref, return_type)
    end
    
    # Handle keywords that should be treated as identifiers (not sure this actually happens), 'run'
    if tok.type == 0 #-animation_dsl.Token.KEYWORD-#
      var name = tok.value
      self.next()
      return self.ExpressionResult.literal(f"animation.{name}")
    end
    
    self.error(f"Unexpected value: {tok.value}")
    self.skip_statement()
    return self.ExpressionResult.literal("nil")
  end
  
  # Process function call (legacy - for non-animation contexts)
  def process_function_call(context)
    var tok = self.current()
    var func_name = ""
    
    # Handle both identifiers and keywords as function names
    if tok != nil && (tok.type == 1 #-animation_dsl.Token.IDENTIFIER-# || tok.type == 0 #-animation_dsl.Token.KEYWORD-#)
      func_name = tok.value
      self.next()
    else
      self.error("Expected function name")
      return "nil"
    end
    
    # Check if this is a mathematical function - handle with positional arguments
    var entry = self.symbol_table.get(func_name)
    if entry != nil && entry.type == 4 #-animation_dsl._symbol_entry.TYPE_MATH_FUNCTION-#
      # Mathematical functions use positional arguments, not named parameters
      var args = self.process_function_arguments(false)
      return f"{entry.get_reference()}({args})"
    end
    
    # Special case for log function - call global log function directly
    if func_name == "log"
      var args = self.process_function_arguments(false)
      # Use unified log processing (return expression for use in contexts)
      return self.process_log_call(args, self.CONTEXT_EXPRESSION, "")
    end
    
    var args = self.process_function_arguments(false)
    
    # Check if it's a template call first
    if entry != nil && entry.type == 14 #-animation_dsl._symbol_entry.TYPE_TEMPLATE-#
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
  #
  # @Return string
  def process_time_value()
    var tok = self.current()
    if tok != nil && tok.type == 5 #-animation_dsl.Token.TIME-#
      var time_str = tok.value
      self.next()
      return str(self.convert_time_to_ms(time_str))
    elif tok != nil && tok.type == 2 #-animation_dsl.Token.NUMBER-#
      var num = tok.value
      self.next()
      return str(int(real(num)) * 1000)  # assume seconds
    elif tok != nil && tok.type == 1 #-animation_dsl.Token.IDENTIFIER-#
      # Handle variable references for time values
      var var_name = tok.value
      
      # Validate that the variable exists before processing
      self._validate_object_reference(var_name, "duration")
      
      var result = self.process_primary_expression(self.CONTEXT_TIME, true, false)
      return result.expr
    else
      self.error("Expected time value")
      return "1000"
    end
  end
  
  # Process percentage value - simplified
  def process_percentage_value()
    var tok = self.current()
    if tok != nil && tok.type == 6 #-animation_dsl.Token.PERCENTAGE-#
      var percent_str = tok.value
      self.next()
      var percent = real(percent_str[0..-2])
      return int(percent * 255 / 100)
    elif tok != nil && tok.type == 2 #-animation_dsl.Token.NUMBER-#
      var num = tok.value
      self.next()
      return int(real(num))
    else
      self.error("Expected percentage value")
      return 255
    end
  end
  
  # Helper methods - pull lexer only
  def current()
    return self.pull_lexer.peek_token()
  end
  
  def peek()
    return self.pull_lexer.peek_ahead(2)  # Look ahead by 2 (next token after current)
  end
  
  def next()
    return self.pull_lexer.next_token()
  end
  
  def at_end()
    return self.pull_lexer.at_end()
  end
  
  def skip_whitespace()
    while !self.at_end()
      var tok = self.current()
      if tok != nil && (tok.type == 35 #-animation_dsl.Token.NEWLINE-# || tok.type == 37 #-animation_dsl.Token.COMMENT-#)
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
      if tok != nil && (tok.type == 37 #-animation_dsl.Token.COMMENT-# || tok.type == 35 #-animation_dsl.Token.NEWLINE-#)
        self.next()
      else
        break
      end
    end
  end
  
  # Collect inline comment if present and return it formatted for Berry code
  def collect_inline_comment()
    var tok = self.current()
    if tok != nil && tok.type == 37 #-animation_dsl.Token.COMMENT-#
      var comment = "  " + tok.value  # Add spacing before comment
      self.next()
      return comment
    end
    return ""  # No comment found
  end
  
  def expect_identifier()
    var tok = self.current()
    if tok != nil && (tok.type == 1 #-animation_dsl.Token.IDENTIFIER-# || 
                      tok.type == 4 #-animation_dsl.Token.COLOR-# ||
                      (tok.type == 0 #-animation_dsl.Token.KEYWORD-# && self.can_use_as_identifier(tok.value)))
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
        var arg_result = self.process_additive_expression(self.CONTEXT_ARGUMENT, true, true)  # raw_mode = true
        arg = arg_result.expr  # Extract the expression string
      else
        # For statement contexts - use normal processing with closure wrapping
        var arg_result = self.process_value(self.CONTEXT_ARGUMENT)
        arg = arg_result.expr
      end
      args.push(arg)
      
      self.skip_whitespace()
      
      if self.current() != nil && self.current().type == 30 #-animation_dsl.Token.COMMA-#
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
    if tok != nil && (tok.type == 1 #-animation_dsl.Token.IDENTIFIER-# || tok.type == 0 #-animation_dsl.Token.KEYWORD-#)
      func_name = tok.value
      self.next()
    else
      self.error("Expected function name")
      return "nil"
    end
    var entry = self.symbol_table.get(func_name)
    
    # Check if this is a mathematical function - handle with positional arguments
    if entry != nil && entry.type == 4 #-animation_dsl._symbol_entry.TYPE_MATH_FUNCTION-#
      # Mathematical functions use positional arguments, not named parameters
      var args = self.process_function_arguments(true)
      return f"{entry.get_reference()}({args})"  # Math functions are under _math namespace
    end
    
    # Special case for log function in nested calls
    if func_name == "log"
      var args = self.process_function_arguments(true)
      # Use unified log processing
      return self.process_log_call(args, self.CONTEXT_EXPRESSION, "")
    end
    
    # Check if this is a template call
    if entry != nil && entry.type == 14 #-animation_dsl._symbol_entry.TYPE_TEMPLATE-#
      # This is a template call - treat like user function
      var args = self.process_function_arguments(true)
      var full_args = args != "" ? f"engine, {args}" : "engine"
      return f"{func_name}_template({full_args})"
    else
      # TODO not sure we can go that far with a symbol not in animation
      if !self._validate_animation_factory_exists(func_name)
        self.error(f"Animation factory function '{func_name}' does not exist. Check the function name and ensure it's available in the animation module.")
        self.skip_function_arguments()  # Skip the arguments to avoid parsing errors
        return "nil"
      end

      self.expect_left_paren()

      # lines contains the attriute assignments to put in the closure
      var lines = []
      
      # Use the core processing logic with a callback for anonymous function assignments
      var assignment_callback = def (param_name, param_value, inline_comment)
        lines.push(f"  provider.{param_name} = {param_value}{inline_comment}")
      end
      
      self._process_parameters_core(func_name, "generic", assignment_callback)
      self.expect_right_paren()

      if size(lines) > 0
        # Join all lines into a single expression
        var result = ""
        for i : 0..size(lines)-1
          if i > 0
            result += "\n"
          end
          result += lines[i]
        end

        return f"(def (engine)\n"
                "  var provider = animation.{func_name}(engine)\n"
                "{result}\n"
                "  return provider\n"
                "end)(engine)"
      else
        return f"animation.{func_name}(engine)"
      end
    end
  end
  
  def expect_assign()
    var tok = self.current()
    if tok != nil && tok.type == 8 #-animation_dsl.Token.ASSIGN-#
      self.next()
    else
      self.error("Expected '='")
    end
  end
  
  def expect_left_paren()
    var tok = self.current()
    if tok != nil && tok.type == 24 #-animation_dsl.Token.LEFT_PAREN-#
      self.next()
    else
      self.error("Expected '('")
    end
  end
  
  def expect_right_paren()
    var tok = self.current()
    if tok != nil && tok.type == 25 #-animation_dsl.Token.RIGHT_PAREN-#
      self.next()
    else
      self.error("Expected ')'")
    end
  end
  
  def check_right_paren()
    var tok = self.current()
    return tok != nil && tok.type == 25 #-animation_dsl.Token.RIGHT_PAREN-#
  end
  
  def expect_comma()
    var tok = self.current()
    if tok != nil && tok.type == 30 #-animation_dsl.Token.COMMA-#
      self.next()
    else
      self.error("Expected ','")
    end
  end
  
  def expect_left_brace()
    var tok = self.current()
    if tok != nil && tok.type == 26 #-animation_dsl.Token.LEFT_BRACE-#
      self.next()
    else
      self.error("Expected '{'")
    end
  end
  
  def expect_right_brace()
    var tok = self.current()
    if tok != nil && tok.type == 27 #-animation_dsl.Token.RIGHT_BRACE-#
      self.next()
    else
      self.error("Expected '}'")
    end
  end
  
  def check_right_brace()
    var tok = self.current()
    return tok != nil && tok.type == 27 #-animation_dsl.Token.RIGHT_BRACE-#
  end
  
  def expect_number()
    var tok = self.current()
    if tok != nil && tok.type == 2 #-animation_dsl.Token.NUMBER-#
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
    if tok != nil && tok.type == 0 #-animation_dsl.Token.KEYWORD-# && tok.value == keyword
      self.next()
    else
      self.error(f"Expected '{keyword}'")
    end
  end
  
  def expect_colon()
    var tok = self.current()
    if tok != nil && tok.type == 32 #-animation_dsl.Token.COLON-#
      self.next()
    else
      self.error("Expected ':'")
    end
  end
  
  def expect_dot()
    var tok = self.current()
    if tok != nil && tok.type == 33 #-animation_dsl.Token.DOT-#
      self.next()
    else
      self.error("Expected '.'")
    end
  end
  
  def expect_left_bracket()
    var tok = self.current()
    if tok != nil && tok.type == 28 #-animation_dsl.Token.LEFT_BRACKET-#
      self.next()
    else
      self.error("Expected '['")
    end
  end
  
  def expect_right_bracket()
    var tok = self.current()
    if tok != nil && tok.type == 29 #-animation_dsl.Token.RIGHT_BRACKET-#
      self.next()
    else
      self.error("Expected ']'")
    end
  end
  
  def check_right_bracket()
    var tok = self.current()
    return tok != nil && tok.type == 29 #-animation_dsl.Token.RIGHT_BRACKET-#
  end
  

  
  # Process array literal [item1, item2, item3]
  def process_array_literal()
    self.expect_left_bracket()
    var items = []
    
    while !self.at_end() && !self.check_right_bracket()
      # Process array element
      var item_result = self.process_value(self.CONTEXT_ARRAY_ELEMENT)
      items.push(item_result.expr)
      
      if self.current() != nil && self.current().type == 30 #-animation_dsl.Token.COMMA-#
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
      if tok == nil || tok.type == 35 #-animation_dsl.Token.NEWLINE-#  # EOF token removed - check nil
        break
      end
      self.next()
    end
  end
  
  # Skip function arguments when validation fails
  def skip_function_arguments()
    if self.current() != nil && self.current().type == 24 #-animation_dsl.Token.LEFT_PAREN-#
      self.next()  # consume '('
      var paren_count = 1
      
      while !self.at_end() && paren_count > 0
        var tok = self.current()
        if tok.type == 24 #-animation_dsl.Token.LEFT_PAREN-#
          paren_count += 1
        elif tok.type == 25 #-animation_dsl.Token.RIGHT_PAREN-#
          paren_count -= 1
        end
        self.next()
      end
    end
  end
  
  # Conversion helpers
  def convert_color(color_str)
    import animation_dsl
    import string
    # Handle 0x hex colors (new format)
    if string.startswith(color_str, "0x")
      if size(color_str) == 10  # 0xAARRGGBB (with alpha channel)
        return color_str
      elif size(color_str) == 8   # 0xRRGGBB (without alpha channel - add opaque alpha)
        return f"0xFF{color_str[2..]}"
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
    return self.symbol_table.get_reference(color_name)
  end
  
  # Validate that a user-defined name is not a predefined color or DSL keyword
  def validate_user_name(name, definition_type)
    import animation_dsl
    # Check if the name already exists in the symbol table
    var entry = self.symbol_table.get(name)
    if entry == nil
      # Name is available - continue with other checks
    elif entry.is_builtin && entry.type == 11 #-animation_dsl._symbol_entry.TYPE_COLOR-#
      self.error(f"Cannot redefine predefined color '{name}'. Use a different name like '{name}_custom' or 'my_{name}'")
      return false
    elif entry.is_builtin
      self.error(f"Cannot redefine built-in symbol '{name}' (type: {entry.type}). Use a different name like '{name}_custom' or 'my_{name}'")
      return false
    else
      # User-defined symbol already exists - this is a redefinition error
      self.error(f"Symbol '{name}' is already defined as {entry.type}. Cannot redefine as {definition_type}.")
      return false
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
    var error_msg = f"Line {line}: {msg}"
    raise "dsl_compilation_error", error_msg
  end
  
  def warning(msg)
    var line = self.current() != nil ? self.current().line : 0
    self.warnings.push(f"Line {line}: {msg}")
  end
  
  def get_warnings()
    return self.warnings
  end
  
  def has_warnings()
    return size(self.warnings) > 0
  end
  
  def get_symbol_table_report()
    import string
    
    var report = "## Symbol Table\n\n"
    
    var symbols = self.symbol_table.list_symbols()
    if size(symbols) == 0
      report += "No symbols defined\n\n"
      return report
    end
    
    # Helper function to calculate display width (accounting for Unicode characters)
    def display_width(s)
      # Common Unicode symbols and their display widths
      var unicode_widths = {
        "✓": 1,    # Check mark
        "⚠️": 2,   # Warning sign (emoji) - actually displays as 2 characters wide
        "⚠": 1     # Warning sign (text)
      }
      
      var width = 0
      var i = 0
      while i < size(s)
        var found_unicode = false
        # Check for known Unicode symbols
        for symbol : unicode_widths.keys()
          if i + size(symbol) <= size(s) && s[i..i+size(symbol)-1] == symbol
            width += unicode_widths[symbol]
            i += size(symbol)
            found_unicode = true
            break
          end
        end
        
        if !found_unicode
          # Regular ASCII character
          width += 1
          i += 1
        end
      end
      
      return width
    end
    
    # Collect all symbol data first to calculate column widths
    var symbol_data = []
    var max_name_len = 6  # "Symbol"
    var max_type_len = 4  # "Type"
    var max_builtin_len = 7  # "Builtin"
    var max_dangerous_len = 9  # "Dangerous"
    var max_takes_args_len = 10  # "Takes Args"
    
    for symbol_info : symbols
      var parts = string.split(symbol_info, ": ")
      if size(parts) >= 2
        var name = parts[0]
        var typ = parts[1]
        var entry = self.symbol_table.get(name)
        
        # Filter out built-in colors to reduce noise
        if entry != nil
          var builtin = entry.is_builtin ? "✓" : ""
          var dangerous = entry.is_dangerous_call() ? "⚠️" : ""
          var takes_args = entry.takes_args ? " ✓ " : ""
          
          # Calculate max widths using display width
          var name_with_backticks = f"`{name}`"
          if display_width(name_with_backticks) > max_name_len
            max_name_len = display_width(name_with_backticks)
          end
          if display_width(typ) > max_type_len
            max_type_len = display_width(typ)
          end
          if display_width(builtin) > max_builtin_len
            max_builtin_len = display_width(builtin)
          end
          if display_width(dangerous) > max_dangerous_len
            max_dangerous_len = display_width(dangerous)
          end
          if display_width(takes_args) > max_takes_args_len
            max_takes_args_len = display_width(takes_args)
          end
          
          symbol_data.push({
            "name": name_with_backticks,
            "typ": typ,
            "builtin": builtin,
            "dangerous": dangerous,
            "takes_args": takes_args
          })
        end
      end
    end
    
    # Sort symbol_data by name (case-insensitive)
    def _sort_symbol_data()
      var n = size(symbol_data)
      if n <= 1
        return
      end
      
      # Insertion sort for small lists
      var i = 1
      while i < n
        var key = symbol_data[i]
        var key_name = key['name']
        var j = i
        while j > 0 && symbol_data[j-1]['name'] > key_name
          symbol_data[j] = symbol_data[j-1]
          j -= 1
        end
        symbol_data[j] = key
        i += 1
      end
    end
    
    _sort_symbol_data()
    
    # Helper function to pad strings to specific width (using display width)
    def pad_string(s, width)
      var padding = width - display_width(s)
      if padding <= 0
        return s
      end
      return s + (" " * padding)
    end
    
    def center_string(s, width)
      var padding = width - display_width(s)
      if padding <= 0
        return s
      end
      var left_pad = padding / 2
      var right_pad = padding - left_pad
      return (" " * left_pad) + s + (" " * right_pad)
    end
    
    # Create properly formatted table header
    var header = f"| {pad_string('Symbol', max_name_len)} | {pad_string('Type', max_type_len)} | {pad_string('Builtin', max_builtin_len)} | {pad_string('Dangerous', max_dangerous_len)} | {pad_string('Takes Args', max_takes_args_len)} |\n"
    var separator = f"|{'-' * (max_name_len + 2)}|{'-' * (max_type_len + 2)}|{'-' * (max_builtin_len + 2)}|{'-' * (max_dangerous_len + 2)}|{'-' * (max_takes_args_len + 2)}|\n"
    
    report += header
    report += separator
    
    # Add formatted rows
    for data : symbol_data
      var row = f"| {pad_string(data['name'], max_name_len)} | {pad_string(data['typ'], max_type_len)} | {center_string(data['builtin'], max_builtin_len)} | {center_string(data['dangerous'], max_dangerous_len)} | {center_string(data['takes_args'], max_takes_args_len)} |\n"
      report += row
    end
    
    report += "\n"
    return report
  end

  def get_error_report()
    var report = ""
    
    if self.has_warnings()
      report += "Compilation warnings:\n"
      for warning : self.warnings
        report += "  " + warning + "\n"
      end
    end
    
    if report == ""
      return "No compilation warnings"
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
    if self.current() != nil && self.current().type == 24 #-animation_dsl.Token.LEFT_PAREN-#
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
      if tok.type == 0 #-animation_dsl.Token.KEYWORD-# && tok.value == "interrupt"
        self.next()  # skip 'interrupt'
        var target = self.expect_identifier()
        if target == "current"
          self.add("  engine.interrupt_current()")
        else
          self.add(f"  engine.interrupt_animation(\"{target}\")")
        end
      else
        # Assume it's an animation function call or reference
        var action_result = self.process_value(self.CONTEXT_ANIMATION)
        self.add(f"  engine.add({action_result.expr})")
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
      if tok != nil && tok.type == 5 #-animation_dsl.Token.TIME-#
        var time_ms = self.process_time_value()
        params += f"\"interval\": {time_ms}"
      else
        var value_result = self.process_value("event_param")
        params += f"\"value\": {value_result.expr}"
      end
    end
    
    self.expect_right_paren()
    params += "}"
    return params
  end
  
  # Process berry code block: berry """<berry code>""" or berry '''<berry code>'''
  def process_berry_code_block()
    self.next()  # skip 'berry'
    
    # Expect a string token containing the berry code
    var tok = self.current()
    if tok == nil || tok.type != 3 #-animation_dsl.Token.STRING-#
      self.error("Expected string literal after 'berry' keyword. Use berry \"\"\"<code>\"\"\" or berry '''<code>'''")
      self.skip_statement()
      return
    end
    
    var berry_code = tok.value
    self.next()  # consume string token
    
    var inline_comment = self.collect_inline_comment()
    
    # Add the berry code verbatim to the output
    self.add(f"# Berry code block{inline_comment}")
    
    # Split the berry code into lines and add each line
    import string
    var lines = string.split(berry_code, '\n')
    for line : lines
      self.add(line)
    end
    
    self.add("# End berry code block")
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
  


  # Generate Berry function for template definition using direct pull-lexer approach
  def generate_template_function_direct(name, params, param_types)
    import animation_dsl
    import string
    
    # Generate function signature with engine as first parameter
    var param_list = "engine"
    for param : params
      param_list += f", {param}_"
    end
    
    self.add(f"# Template function: {name}")
    self.add(f"def {name}_template({param_list})")
    
    # Create a new transpiler that shares the same pull lexer
    # It will consume tokens from the current position until the template ends
    var template_transpiler = animation_dsl.SimpleDSLTranspiler(self.pull_lexer)
    template_transpiler.symbol_table = animation_dsl._symbol_table()  # Fresh symbol table for template
    template_transpiler.strip_initialized = true  # Templates assume engine exists
    
    # Add parameters to template's symbol table with proper types
    for param : params
      var param_type = param_types.find(param)
      if param_type != nil
        # Create typed parameter based on type annotation
        self._add_typed_parameter_to_symbol_table(template_transpiler.symbol_table, param, param_type)
      else
        # Default to variable type for untyped parameters
        template_transpiler.symbol_table.create_variable(param)
      end
    end
    
    # Transpile the template body - it will consume tokens until the closing brace
    var template_body = template_transpiler.transpile_template_body()
    
    if template_body != nil
      # Add the transpiled body with proper indentation
      var body_lines = string.split(template_body, "\n")
      for line : body_lines
        if size(line) > 0
          self.add(f"  {line}")  # Add 2-space indentation
        end
      end
      
      # Validate parameter usage in template body (post-transpilation check)
      self._validate_template_parameter_usage(name, params, template_body)
    else
      # Error in template body transpilation
      for error : template_transpiler.errors
        self.error(f"Template '{name}' body error: {error}")
      end
    end
    
    # Expect the closing brace (template_transpiler should have left us at this position)
    self.expect_right_brace()
    
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
    self._process_named_arguments_unified(var_name, func_name, self.CONTEXT_ANIMATION)
  end
  
  # Create instance for parameter validation at transpile time - simplified using symbol_table
  def _create_instance_for_validation(func_name)
    # Use symbol_table's dynamic detection to get instance
    var entry = self.symbol_table.get(func_name)
    return entry != nil ? entry.instance : nil
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
      
    except "dsl_compilation_error" as e, msg
      # Re-raise DSL compilation errors (these are intentional validation failures)
      raise e, msg
    except .. as e, msg
      # If validation fails for any other reason, just continue
      # This ensures the transpiler is robust even if validation has issues
    end
  end
  
  # Validate that a referenced object exists in the symbol table or animation module
  #
  # @param object_name: string - Name of the object being referenced
  # @param context: string - Context where the reference occurs (for error messages)
  # @return bool: true if exists, false if not found
  def _validate_object_reference(object_name, context)
    if !self.symbol_table.symbol_exists(object_name)
      self.error(f"Undefined reference '{object_name}' in {context}. Make sure the object is defined before use.")
      return false
    end
    return true
  end

  # Validate animation factory exists - simplified using symbol_table
  def _validate_animation_factory_exists(func_name)
    # Use symbol table's dynamic detection - any callable function is valid
    var entry = self.symbol_table.get(func_name)
    return entry != nil
  end
  
  # Validate color provider factory exists - simplified using symbol_table  
  def _validate_color_provider_factory_exists(func_name)
    # Use symbol table's dynamic detection - any callable function is valid
    var entry = self.symbol_table.get(func_name)
    return entry != nil && entry.type == 10 #-animation_dsl._symbol_entry.TYPE_COLOR_CONSTRUCTOR-#
  end
  
  # Validate that a referenced object is a value provider or animation - simplified using symbol_table
  def _validate_value_provider_reference(object_name, context)
    try
      # First check if symbol exists using symbol_table
      if !self.symbol_table.symbol_exists(object_name)
        self.error(f"Undefined reference '{object_name}' in {context} statement. Make sure the value provider or animation is defined before use.")
        return false
      end
      
      # Use symbol_table to get type information
      var entry = self.symbol_table.get(object_name)
      if entry != nil
        # Check if it's a value provider or animation instance (not constructor)
        if entry.type == 7 #-animation_dsl._symbol_entry.TYPE_VALUE_PROVIDER-# || entry.type == 9 #-animation_dsl._symbol_entry.TYPE_ANIMATION-#
          return true  # Valid value provider or animation instance
        else
          # It's some other type (variable, color, sequence, constructor, etc.)
          self.error(f"'{object_name}' in {context} statement is not a value provider or animation instance. Only value provider instances (like oscillators) and animation instances can be restarted.")
          return false
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
      var param_value_result = self.process_value(self.CONTEXT_VARIABLE)
      var inline_comment = self.collect_inline_comment()
      
      # Call the assignment callback to handle the parameter
      assignment_callback(param_name, param_value_result.expr, inline_comment)
      
      # Skip whitespace but preserve newlines for separator detection
      while !self.at_end()
        var tok = self.current()
        if tok != nil && tok.type == 37 #-animation_dsl.Token.COMMENT-#
          self.next()
        else
          break
        end
      end
      
      # Check for parameter separator: comma OR newline OR end of parameters
      if self.current() != nil && self.current().type == 30 #-animation_dsl.Token.COMMA-#
        self.next()  # skip comma
        self.skip_whitespace_including_newlines()
      elif self.current() != nil && self.current().type == 35 #-animation_dsl.Token.NEWLINE-#
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
  
  def _process_named_arguments_for_color_provider(var_name, func_name)
    self._process_named_arguments_unified(var_name, func_name, self.CONTEXT_COLOR_PROVIDER)
  end

  # Template parameter validation methods
  
  # Validate template parameter name
  def _validate_template_parameter_name(param_name, param_names_seen)
    import animation_dsl
    # Check for duplicate parameter names
    if param_names_seen.contains(param_name)
      self.error(f"Duplicate parameter name '{param_name}' in template. Each parameter must have a unique name.")
      return false
    end
    
    # Check if parameter name conflicts with reserved keywords
    var reserved_keywords = [
      "engine", "self", "animation", "color", "palette", "sequence", "template",
      "import", "def", "end", "class", "var", "if", "else", "while", "for",
      "true", "false", "nil", "return", "break", "continue"
    ]
    
    for keyword : reserved_keywords
      if param_name == keyword
        self.error(f"Parameter name '{param_name}' conflicts with reserved keyword. Use a different name like '{param_name}_param' or 'my_{param_name}'.")
        return false
      end
    end
    
    # Check if parameter name conflicts with built-in color names
    if animation_dsl.is_color_name(param_name)
      self.error(f"Parameter name '{param_name}' conflicts with built-in color name. Use a different name like '{param_name}_param' or 'my_{param_name}'.")
      return false
    end
    
    return true
  end
  
  # Validate template parameter type annotation
  def _validate_template_parameter_type(param_type)
    var valid_types = [
      "color", "palette", "animation", "number", "string", "boolean", 
      "time", "percentage", "variable", "value_provider"
    ]
    
    for valid_type : valid_types
      if param_type == valid_type
        return true
      end
    end
    
    self.error(f"Invalid parameter type '{param_type}'. Valid types are: {valid_types}")
    return false
  end
  
  # Add typed parameter to symbol table based on type annotation
  def _add_typed_parameter_to_symbol_table(symbol_table, param_name, param_type)
    if param_type == "color"
      symbol_table.create_color(param_name, nil)
    elif param_type == "palette"
      symbol_table.create_palette(param_name, nil)
    elif param_type == "animation"
      symbol_table.create_animation(param_name, nil)
    elif param_type == "value_provider"
      symbol_table.create_value_provider(param_name, nil)
    else
      # Default to variable for number, string, boolean, time, percentage, variable
      symbol_table.create_variable(param_name)
    end
  end
  
  # Validate template parameter usage in generated body
  def _validate_template_parameter_usage(template_name, params, template_body)
    import string
    
    # Check if each parameter is actually used in the template body
    for param : params
      var param_ref = f"{param}_"  # Parameters are referenced with underscore suffix
      
      if string.find(template_body, param_ref) == -1
        # Parameter not found in body - this is a warning, not an error
        self.warning(f"Template '{template_name}' parameter '{param}' is declared but never used in the template body.")
      end
    end
  end
  
  # Validate template call arguments (called when processing template calls)
  def _validate_template_call_arguments(template_name, provided_args, expected_params, param_types)
    # Check argument count
    if size(provided_args) != size(expected_params)
      self.error(f"Template '{template_name}' expects {size(expected_params)} arguments but {size(provided_args)} were provided. Expected parameters: {expected_params}")
      return false
    end
    
    # TODO: Add type checking for arguments based on param_types
    # This would require more sophisticated type inference for the provided arguments
    
    return true
  end
  
  # Helper method to split function arguments string into array
  def _split_function_arguments(args_str)
    import string
    
    if args_str == "" || args_str == nil
      return []
    end
    
    # Simple split by comma - this is a basic implementation
    # A more sophisticated version would handle nested parentheses and quotes
    var args = string.split(args_str, ",")
    var result = []
    
    for arg : args
      # Trim whitespace
      var trimmed = string.strip(arg)
      if size(trimmed) > 0
        result.push(trimmed)
      end
    end
    
    return result
  end

end

# DSL compilation function
def compile_dsl(source)
  import animation_dsl
  var lexer = animation_dsl.create_lexer(source)
  var transpiler = animation_dsl.SimpleDSLTranspiler(lexer)
  var berry_code = transpiler.transpile()
  
  return berry_code
end

# Return module exports
return {
  "SimpleDSLTranspiler": SimpleDSLTranspiler,
  "compile_dsl": compile_dsl,
}