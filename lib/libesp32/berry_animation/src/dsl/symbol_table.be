# Symbol Table Classes for DSL Transpiler
# Enhanced symbol caching and management for the Animation DSL

# Symbol table entry class for enhanced symbol caching
#@ solidify:SymbolEntry,weak
class SymbolEntry
  # Type constants
  static var TYPE_PALETTE_CONSTANT = 1
  static var TYPE_PALETTE = 2
  static var TYPE_CONSTANT = 3
  static var TYPE_MATH_FUNCTION = 4
  static var TYPE_USER_FUNCTION = 5
  static var TYPE_VALUE_PROVIDER_CONSTRUCTOR = 6
  static var TYPE_VALUE_PROVIDER = 7
  static var TYPE_ANIMATION_CONSTRUCTOR = 8
  static var TYPE_ANIMATION = 9
  static var TYPE_COLOR_CONSTRUCTOR = 10
  static var TYPE_COLOR = 11
  static var TYPE_VARIABLE = 12
  static var TYPE_SEQUENCE = 13
  static var TYPE_TEMPLATE = 14
  
  var name           # Symbol name
  var type           # Symbol type (int constant)
  var instance       # Actual instance (for validation) or nil
  var takes_args     # Boolean: whether this symbol takes arguments
  var arg_type       # "positional", "named", or "none"
  var is_builtin     # Boolean: whether this is a built-in symbol from animation module
  var is_dangerous   # Boolean: whether calling this symbol creates a new instance (dangerous in computed expressions)
  var param_types    # Map of parameter names to types (for templates and user functions)
  
  def init(name, typ, instance, is_builtin)
    self.name = name
    self.type = typ
    self.instance = instance
    self.is_builtin = is_builtin != nil ? is_builtin : false
    self.takes_args = false
    self.arg_type = "none"
    self.is_dangerous = false
    self.param_types = {}
    
    # Auto-detect argument characteristics and danger level based on type
    self._detect_arg_characteristics()
    self._detect_danger_level()
  end
  
  # Detect if this symbol takes arguments and what type
  def _detect_arg_characteristics()
    if self.type == self.TYPE_PALETTE_CONSTANT || self.type == self.TYPE_PALETTE || self.type == self.TYPE_CONSTANT
      # Palette objects and constants don't take arguments
      self.takes_args = false
      self.arg_type = "none"
    elif self.type == self.TYPE_MATH_FUNCTION
      # Math functions like max, min take positional arguments
      self.takes_args = true
      self.arg_type = "positional"
    elif self.type == self.TYPE_USER_FUNCTION
      # User functions take positional arguments (engine + user args)
      self.takes_args = true
      self.arg_type = "positional"
    elif self.type == self.TYPE_VALUE_PROVIDER_CONSTRUCTOR || self.type == self.TYPE_ANIMATION_CONSTRUCTOR || self.type == self.TYPE_COLOR_CONSTRUCTOR
      # Constructor functions take named arguments
      self.takes_args = true
      self.arg_type = "named"
    else
      # Instances, variables, sequences, templates don't take arguments when referenced
      self.takes_args = false
      self.arg_type = "none"
    end
  end
  
  # Detect if this symbol is dangerous (creates new instances when called)
  def _detect_danger_level()
    if self.type == self.TYPE_VALUE_PROVIDER_CONSTRUCTOR
      # Value provider constructors create new instances - dangerous in computed expressions
      self.is_dangerous = true
    elif self.type == self.TYPE_ANIMATION_CONSTRUCTOR
      # Animation constructors create new instances - dangerous in computed expressions
      self.is_dangerous = true
    elif self.type == self.TYPE_COLOR_CONSTRUCTOR
      # Color provider constructors create new instances - dangerous in computed expressions
      self.is_dangerous = true
    else
      # Constants, math functions, variables, instances, user functions, etc. are safe
      self.is_dangerous = false
    end
  end
  
  # Check if this symbol is a bytes() instance (for palettes)
  def is_bytes_instance()
    return (self.type == self.TYPE_PALETTE_CONSTANT || self.type == self.TYPE_PALETTE) && self.instance != nil && isinstance(self.instance, bytes)
  end
  
  # Check if this symbol is a math function
  def is_math_function()
    return self.type == self.TYPE_MATH_FUNCTION
  end
  
  # Check if this symbol is a user function
  def is_user_function()
    return self.type == self.TYPE_USER_FUNCTION
  end
  
  
  # Check if this symbol is a value provider constructor
  def is_value_provider_constructor()
    return self.type == self.TYPE_VALUE_PROVIDER_CONSTRUCTOR
  end
  
  # Check if this symbol is a value provider instance
  def is_value_provider_instance()
    return self.type == self.TYPE_VALUE_PROVIDER
  end
  
  # Check if this symbol is an animation constructor
  def is_animation_constructor()
    return self.type == self.TYPE_ANIMATION_CONSTRUCTOR
  end
  
  # Check if this symbol is an animation instance
  def is_animation_instance()
    return self.type == self.TYPE_ANIMATION
  end
  
  # Check if this symbol is a color constructor
  def is_color_constructor()
    return self.type == self.TYPE_COLOR_CONSTRUCTOR
  end
  
  # Check if this symbol is a color instance
  def is_color_instance()
    return self.type == self.TYPE_COLOR
  end
  
  # Check if this symbol takes positional arguments
  def takes_positional_args()
    return self.takes_args && self.arg_type == "positional"
  end
  
  # Check if this symbol takes named arguments
  def takes_named_args()
    return self.takes_args && self.arg_type == "named"
  end
  
  # Check if this symbol is dangerous (creates new instances when called)
  def is_dangerous_call()
    return self.is_dangerous
  end
  
  # Set parameter types for templates and user functions
  def set_param_types(param_types)
    self.param_types = param_types != nil ? param_types : {}
  end
  
  # Get parameter types
  def get_param_types()
    return self.param_types
  end
  
  # Convert type constant to string for debugging
  def type_to_string()
    if self.type == self.TYPE_PALETTE_CONSTANT return "palette_constant"
    elif self.type == self.TYPE_PALETTE return "palette"
    elif self.type == self.TYPE_CONSTANT return "constant"
    elif self.type == self.TYPE_MATH_FUNCTION return "math_function"
    elif self.type == self.TYPE_USER_FUNCTION return "user_function"
    elif self.type == self.TYPE_VALUE_PROVIDER_CONSTRUCTOR return "value_provider_constructor"
    elif self.type == self.TYPE_VALUE_PROVIDER return "value_provider"
    elif self.type == self.TYPE_ANIMATION_CONSTRUCTOR return "animation_constructor"
    elif self.type == self.TYPE_ANIMATION return "animation"
    elif self.type == self.TYPE_COLOR_CONSTRUCTOR return "color_constructor"
    elif self.type == self.TYPE_COLOR return "color"
    elif self.type == self.TYPE_VARIABLE return "variable"
    elif self.type == self.TYPE_SEQUENCE return "sequence"
    elif self.type == self.TYPE_TEMPLATE return "template"
    else return f"unknown({self.type})"
    end
  end
  
  # Get the resolved symbol reference for code generation
  def get_reference()
    # Generate appropriate reference based on whether it's built-in
    if self.is_builtin
      # Special handling for math functions
      if self.type == self.TYPE_MATH_FUNCTION
        return f"animation._math.{self.name}"
      else
        return f"animation.{self.name}"
      end
    else
      # User-defined symbols get underscore suffix
      return f"{self.name}_"
    end
  end
  
  # String representation for debugging
  def tostring()
    import string
    
    var instance_str = "nil"
    if self.instance != nil
      var instance_type = type(self.instance)
      if instance_type == "instance"
        instance_str = f"<{classname(self.instance)}>"
      else
        instance_str = f"<{instance_type}:{str(self.instance)}>"
      end
    end
    
    var param_types_str = ""
    if size(self.param_types) > 0
      var params_list = ""
      var first = true
      for key : self.param_types.keys()
        if !first
          params_list += ","
        end
        params_list += f"{key}:{self.param_types[key]}"
        first = false
      end
      param_types_str = f" params=[{params_list}]"
    end
    
    return f"SymbolEntry(name='{self.name}', type='{self.type_to_string()}', instance={instance_str}, " +
           f"takes_args={self.takes_args}, arg_type='{self.arg_type}', " +
           f"is_builtin={self.is_builtin}, is_dangerous={self.is_dangerous}{param_types_str})"
  end
  
  # Create a symbol entry for a palette constant (built-in like PALETTE_RAINBOW)
  static def create_palette_constant(name, instance, is_builtin)
    return _class(name, _class.TYPE_PALETTE_CONSTANT, instance, is_builtin)
  end
  
  # Create a symbol entry for a palette instance (user-defined)
  static def create_palette_instance(name, instance, is_builtin)
    return _class(name, _class.TYPE_PALETTE, instance, is_builtin)
  end
  
  # Create a symbol entry for an integer constant
  static def create_constant(name, instance, is_builtin)
    return _class(name, _class.TYPE_CONSTANT, instance, is_builtin)
  end
  
  # Create a symbol entry for a math function
  static def create_math_function(name, is_builtin)
    return _class(name, _class.TYPE_MATH_FUNCTION, nil, is_builtin)
  end
  
  # Create a symbol entry for a user function
  static def create_user_function(name, is_builtin)
    return _class(name, _class.TYPE_USER_FUNCTION, nil, is_builtin)
  end
  
  
  # Create a symbol entry for a value provider constructor (built-in like triangle, smooth)
  static def create_value_provider_constructor(name, instance, is_builtin)
    return _class(name, _class.TYPE_VALUE_PROVIDER_CONSTRUCTOR, instance, is_builtin)
  end
  
  # Create a symbol entry for a value provider instance (user-defined)
  static def create_value_provider_instance(name, instance, is_builtin)
    return _class(name, _class.TYPE_VALUE_PROVIDER, instance, is_builtin)
  end
  
  # Create a symbol entry for an animation constructor (built-in like solid, pulsating_animation)
  static def create_animation_constructor(name, instance, is_builtin)
    return _class(name, _class.TYPE_ANIMATION_CONSTRUCTOR, instance, is_builtin)
  end
  
  # Create a symbol entry for an animation instance (user-defined)
  static def create_animation_instance(name, instance, is_builtin)
    return _class(name, _class.TYPE_ANIMATION, instance, is_builtin)
  end
  
  # Create a symbol entry for a color constructor (built-in like color_cycle, breathe_color)
  static def create_color_constructor(name, instance, is_builtin)
    return _class(name, _class.TYPE_COLOR_CONSTRUCTOR, instance, is_builtin)
  end
  
  # Create a symbol entry for a color instance (user-defined)
  static def create_color_instance(name, instance, is_builtin)
    return _class(name, _class.TYPE_COLOR, instance, is_builtin)
  end
  
  # Create a symbol entry for a variable
  static def create_variable(name, is_builtin)
    return _class(name, _class.TYPE_VARIABLE, nil, is_builtin)
  end
  
  # Create a symbol entry for a sequence
  static def create_sequence(name, is_builtin)
    return _class(name, _class.TYPE_SEQUENCE, nil, is_builtin)
  end
  
  # Create a symbol entry for a template
  static def create_template(name, is_builtin)
    return _class(name, _class.TYPE_TEMPLATE, nil, is_builtin)
  end
end

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

# Enhanced symbol table class for holistic symbol management and caching
#@ solidify:SymbolTable,weak
class SymbolTable
  var entries        # Map of name -> SymbolEntry
  var mock_engine    # MockEngine for validation
  
  def init()
    import animation_dsl
    self.entries = {}
    self.mock_engine = animation_dsl.MockEngine()
  end
  
  # Dynamically detect and cache symbol type when first encountered
  def _detect_and_cache_symbol(name)
    import animation_dsl
    if self.entries.contains(name)
      return self.entries[name]  # Already cached
    end
    
    try
      import introspect
      
      # Check for named colors first (from animation_dsl.named_colors)
      if animation_dsl.named_colors.contains(name)
        var entry = animation_dsl._symbol_entry.create_color_instance(name, nil, true)  # true = is_builtin
        self.entries[name] = entry
        return entry
      end
      
      # Check for special built-in functions like 'log'
      if name == "log"
        var entry = animation_dsl._symbol_entry.create_user_function("log", true)  # true = is_builtin
        self.entries[name] = entry
        return entry
      end
      
      
      # Check for user functions (they might not be in animation module directly)
      if animation.is_user_function(name)
        var entry = animation_dsl._symbol_entry.create_user_function(name, true)
        self.entries[name] = entry
        return entry
      end
      
      # Check for math functions (they are in animation._math, not directly in animation)
      if introspect.contains(animation._math, name)
        var entry = animation_dsl._symbol_entry.create_math_function(name, true)
        self.entries[name] = entry
        return entry
      end
      
      # Check if it exists in animation module
      if introspect.contains(animation, name)
        var obj = animation.(name)
        var obj_type = type(obj)

        # Detect palette objects (bytes() instances)
        if isinstance(obj, bytes)
          var entry = animation_dsl._symbol_entry.create_palette_constant(name, obj, true)
          self.entries[name] = entry
          return entry
        end
        
        # Detect integer constants (like LINEAR, SINE, COSINE, etc.)
        if obj_type == "int"
          var entry = animation_dsl._symbol_entry.create_constant(name, obj, true)
          self.entries[name] = entry
          return entry
        end
        
        # Detect constructors (functions/classes that create instances)
        if obj_type == "function" || obj_type == "class"
          try
            var instance = obj(self.mock_engine)
            if isinstance(instance, animation.color_provider)
              # Color providers are a subclass of value providers, check them first
              var entry = animation_dsl._symbol_entry.create_color_constructor(name, instance, true)
              self.entries[name] = entry
              return entry
            elif isinstance(instance, animation.value_provider)
              var entry = animation_dsl._symbol_entry.create_value_provider_constructor(name, instance, true)
              self.entries[name] = entry
              return entry
            elif isinstance(instance, animation.animation)
              var entry = animation_dsl._symbol_entry.create_animation_constructor(name, instance, true)
              self.entries[name] = entry
              return entry
            end
          except .. as e, msg
            # If instance creation fails, it might still be a valid function
            # but not a constructor we can validate
          end
        end
      end
      
      # If not found in animation module, return nil (will be handled as user-defined)
      return nil
      
    except .. as e, msg
      # If detection fails, return nil
      return nil
    end
  end
  
  # Add a symbol entry to the table (with conflict detection) - returns the entry
  def add(name, entry)
    # First check if there's a built-in symbol with this name
    var builtin_entry = self._detect_and_cache_symbol(name)
    if builtin_entry != nil && builtin_entry.type != entry.type
      raise "symbol_redefinition_error", f"Cannot define '{name}' as {entry.type_to_string()} - it conflicts with built-in {builtin_entry.type_to_string()}"
    end
    
    # Check existing user-defined symbols
    var existing = self.entries.find(name)
    if existing != nil
      # Check if it's the same type
      if existing.type != entry.type
        raise "symbol_redefinition_error", f"Cannot redefine symbol '{name}' as {entry.type_to_string()} - it's already defined as {existing.type_to_string()}"
      end
      # If same type, allow update (for cases like reassignment)
    end
    
    self.entries[name] = entry
    return entry
  end
  
  # Check if a symbol exists (with dynamic detection)
  def contains(name)
    if self.entries.contains(name)
      return true
    end
    
    # Try to detect and cache it
    var entry = self._detect_and_cache_symbol(name)
    return entry != nil
  end
  
  # Get a symbol entry (with dynamic detection)
  def get(name)
    var entry = self.entries.find(name)
    if entry != nil
      return entry
    end
    
    # Try to detect and cache it
    return self._detect_and_cache_symbol(name)
  end
  
  # Get symbol reference for code generation (with dynamic detection)
  def get_reference(name)
    import animation_dsl
    # Try to get from cache or detect dynamically (includes named colors)
    var entry = self.get(name)
    if entry != nil
      # For builtin color entries, return the actual color value directly
      if entry.is_builtin && entry.type == 11 #-animation_dsl._symbol_entry.TYPE_COLOR-#
        var color_value = animation_dsl.named_colors[name]
        # Convert integer to hex string format for transpiler
        return f"0x{color_value:08X}"
      end
      return entry.get_reference()
    end
    
    # Default to user-defined format
    return f"{name}_"
  end
  
  # Check if symbol exists (including named colors, with dynamic detection)
  def symbol_exists(name)
    # Use proper discovery through _detect_and_cache_symbol via contains()
    return self.contains(name)
  end
  
  # Create and register a palette instance symbol (user-defined)
  def create_palette(name, instance)
    import animation_dsl
    var entry = animation_dsl._symbol_entry.create_palette_instance(name, instance, false)
    return self.add(name, entry)
  end
  
  # Create and register a color instance symbol (user-defined)
  def create_color(name, instance)
    import animation_dsl
    var entry = animation_dsl._symbol_entry.create_color_instance(name, instance, false)
    return self.add(name, entry)
  end
  
  # Create and register an animation instance symbol (user-defined)
  def create_animation(name, instance)
    import animation_dsl
    var entry = animation_dsl._symbol_entry.create_animation_instance(name, instance, false)
    return self.add(name, entry)
  end
  
  # Create and register a value provider instance symbol (user-defined)
  def create_value_provider(name, instance)
    import animation_dsl
    var entry = animation_dsl._symbol_entry.create_value_provider_instance(name, instance, false)
    return self.add(name, entry)
  end
  
  # Create and register a variable symbol (user-defined)
  def create_variable(name)
    import animation_dsl
    var entry = animation_dsl._symbol_entry.create_variable(name, false)
    return self.add(name, entry)
  end
  
  # Create and register a sequence symbol (user-defined)
  def create_sequence(name)
    import animation_dsl
    var entry = animation_dsl._symbol_entry.create_sequence(name, false)
    return self.add(name, entry)
  end
  
  # Create and register a template symbol (user-defined)
  def create_template(name, param_types)
    import animation_dsl
    var entry = animation_dsl._symbol_entry.create_template(name, false)
    entry.set_param_types(param_types != nil ? param_types : {})
    return self.add(name, entry)
  end
  

  # Register a user function (detected at runtime)
  def register_user_function(name)
    import animation_dsl
    if !self.contains(name)
      var entry = animation_dsl._symbol_entry.create_user_function(name, false)
      self.add(name, entry)
    end
  end
  
  # Generic create function that can specify name/type/instance/builtin directly
  def create_generic(name, typ, instance, is_builtin)
    import animation_dsl
    var entry = animation_dsl._symbol_entry(name, typ, instance, is_builtin != nil ? is_builtin : false)
    return self.add(name, entry)
  end
  
  # Get the type of a symbol
  def get_type(name)
    var entry = self.get(name)
    return entry != nil ? entry.type_to_string() : nil
  end
  
  # Check if symbol takes arguments
  def takes_args(name)
    var entry = self.get(name)
    return entry != nil ? entry.takes_args : false
  end
  
  # Check if symbol takes positional arguments
  def takes_positional_args(name)
    var entry = self.get(name)
    return entry != nil ? entry.takes_positional_args() : false
  end
  
  # Check if symbol takes named arguments
  def takes_named_args(name)
    var entry = self.get(name)
    return entry != nil ? entry.takes_named_args() : false
  end
  
  # Get instance for validation
  def get_instance(name)
    var entry = self.get(name)
    return entry != nil ? entry.instance : nil
  end
  
  # Check if symbol is dangerous (creates new instances when called)
  def is_dangerous(name)
    var entry = self.get(name)
    return entry != nil ? entry.is_dangerous_call() : false
  end
  
  # Helper method to get named color value (uses proper discovery)
  def _get_named_color_value(color_name)
    import animation_dsl
    var entry = self.get(color_name)  # This will trigger _detect_and_cache_symbol if needed
    if entry != nil && entry.is_builtin && entry.type == 11 #-animation_dsl._symbol_entry.TYPE_COLOR-#
      var color_value = animation_dsl.named_colors[color_name]
      # Convert integer to hex string format for transpiler
      return f"0x{color_value:08X}"
    end
    return "0xFFFFFFFF"  # Default fallback
  end
  
  # Debug method to list all symbols
  def list_symbols()
    var result = []
    for name : self.entries.keys()
      var entry = self.entries[name]
      result.push(f"{name}: {entry.type_to_string()}")
    end
    return result
  end
end

# Return module exports
return {
  "_symbol_entry": SymbolEntry,
  "_symbol_table": SymbolTable,
  "MockEngine": MockEngine
}