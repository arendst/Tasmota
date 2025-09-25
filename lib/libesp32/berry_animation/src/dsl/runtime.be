# DSL Runtime Integration
# Provides complete DSL execution lifecycle management

#@ solidify:DSLRuntime,weak
class DSLRuntime
  var engine         # Animation engine instance
  var active_source  # Currently loaded DSL source
  var debug_mode     # Enable debug output
  
  def init(engine, debug_mode)
    self.engine = engine
    self.active_source = nil
    self.debug_mode = debug_mode != nil ? debug_mode : false
  end
  
  # Load and execute DSL from string
  def load_dsl(source_code)
    import animation_dsl
    if source_code == nil || size(source_code) == 0
      if self.debug_mode
        print("DSL: Empty source code")
      end
      return false
    end
    
    # Compile DSL with exception handling
    if self.debug_mode
      print("DSL: Compiling source...")
    end
    
    try
      var berry_code = animation_dsl.compile(source_code)
      # Execute the compiled Berry code
      return self.execute_berry_code(berry_code, source_code)
    except "dsl_compilation_error" as e, msg
      if self.debug_mode
        print("DSL: Compilation failed - " + msg)
      end
      return false
    end
  end
  
  # Load DSL from file
  def load_dsl_file(filename)
    try
      var file = open(filename, "r")
      if file == nil
        if self.debug_mode
          print(f"DSL: Cannot open file {filename}")
        end
        return false
      end
      
      var source_code = file.read()
      file.close()
      
      if self.debug_mode
        print(f"DSL: Loaded {size(source_code)} characters from {filename}")
      end
      
      return self.load_dsl(source_code)
      
    except .. as e, msg
      if self.debug_mode
        print(f"DSL: File loading error: {msg}")
      end
      return false
    end
  end
  
  # Reload current DSL (useful for development)
  def reload_dsl()
    if self.active_source == nil
      if self.debug_mode
        print("DSL: No active DSL to reload")
      end
      return false
    end
    
    if self.debug_mode
      print("DSL: Reloading current DSL...")
    end
    
    # Stop current animations
    self.engine.stop()
    self.engine.clear()
    
    # Reload with fresh compilation
    return self.load_dsl(self.active_source)
  end
  
  # Get generated Berry code for inspection (debugging)
  def get_generated_code(source_code)
    import animation_dsl
    if source_code == nil
      source_code = self.active_source
    end
    
    if source_code == nil
      return nil
    end
    
    # Generate code with exception handling
    try
      return animation_dsl.compile(source_code)
    except "dsl_compilation_error" as e, msg
      if self.debug_mode
        print("DSL: Code generation failed - " + msg)
      end
      return nil
    end
  end
  
  # Execute Berry code with proper error handling
  def execute_berry_code(berry_code, source_code)
    try
      # Stop current animations before starting new ones
      self.engine.stop()
      self.engine.clear()
      
      # Compile and execute the Berry code
      var compiled_func = compile(berry_code)
      if compiled_func == nil
        if self.debug_mode
          print("DSL: Berry compilation failed")
        end
        return false
      end
      
      # Execute in controlled environment
      compiled_func()
      
      # Store as active source
      self.active_source = source_code
      
      if self.debug_mode
        print("DSL: Execution successful")
      end
      
      return true
      
    except .. as e, msg
      if self.debug_mode
        print(f"DSL: Execution error: {msg}")
      end
      return false
    end
  end
  

  
  # Get current engine for external access
  def get_engine()
    return self.engine
  end
  
  # Check if DSL is currently loaded
  def is_loaded()
    return self.active_source != nil
  end
  
  # Get current DSL source
  def get_active_source()
    return self.active_source
  end
end

# Factory function for easy creation
def create_dsl_runtime(strip, debug_mode)
  import animation_dsl
  var engine = animation.create_engine(strip)
  return animation_dsl.DSLRuntime(engine, debug_mode)
end

# Return module exports
return {
  "DSLRuntime": DSLRuntime,
  "create_dsl_runtime": create_dsl_runtime
}