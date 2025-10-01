# Berry Animation Framework - DSL Module
# 
# This module provides Domain-Specific Language (DSL) functionality for the
# Berry Animation Framework. It allows users to write animations using a
# declarative syntax that gets transpiled to Berry code.
#
# The DSL provides:
# - Declarative animation definitions with intuitive syntax
# - Color and palette definitions
# - Animation sequences and timing control
# - Property assignments and dynamic parameters
# - Event system integration
# - User-defined functions
#
# Usage:
#   import animation_dsl
#   var berry_code = animation_dsl.compile(dsl_source)
#   animation_dsl.execute(berry_code)
#

import global
import animation

# Requires to first `import animation`
# We don't include it to not create a closure, but use the global instead

# Create the DSL module and make it globally accessible
#@ solidify:animation_dsl.SimpleDSLTranspiler.ExpressionResult,weak
#@ solidify:animation_dsl,weak
var animation_dsl = module("animation_dsl")
global.animation_dsl = animation_dsl

# Version information for compatibility tracking
animation_dsl.VERSION = animation.VERSION

# Helper function to register all exports from imported modules into the DSL module
def register_to_dsl(m)
  for k: m.keys()
    animation_dsl.(k) = m[k]
  end
end

# Import DSL components
import "dsl/token.be" as dsl_token
register_to_dsl(dsl_token)
import "dsl/lexer.be" as dsl_lexer
register_to_dsl(dsl_lexer)
import "dsl/transpiler.be" as dsl_transpiler
register_to_dsl(dsl_transpiler)
import "dsl/symbol_table.be" as dsl_symbol_table
register_to_dsl(dsl_symbol_table)
import "dsl/named_colors.be" as dsl_named_colors
register_to_dsl(dsl_named_colors)

# Import Web UI components
import "webui/animation_web_ui.be" as animation_web_ui
register_to_dsl(animation_web_ui)

# Main DSL compilation function
# Compiles DSL source code to Berry code
#
# @param source: string - DSL source code
# @return string - Generated Berry code
def compile_dsl_source(source)
  import animation_dsl
  return animation_dsl.compile_dsl(source)
end
animation_dsl.compile = compile_dsl_source

# Execute DSL source code
# Compiles and executes DSL source in one step
#
# @param source: string - DSL source code
# @return any - Result of execution
def execute(source)
  import animation_dsl
  var berry_code = animation_dsl.compile(source)
  var compiled_fn = compile(berry_code)
  return compiled_fn()
end
animation_dsl.execute = execute

# Load and execute DSL from file
#
# @param filename: string - Path to DSL file
# @return any - Result of execution
def load_file(filename)
  import animation_dsl
  var f = open(filename, "r")
  if f == nil
    raise "io_error", f"Cannot open DSL file: {filename}"
  end
  
  var source = f.read()
  f.close()
  
  return animation_dsl.execute(source)
end
animation_dsl.load_file = load_file

# Compile .anim file to .be file
# Takes a filename with .anim suffix and compiles to same prefix with .be suffix
#
# @param filename: string - Path to .anim file
# @return bool - True if compilation successful
# @raises "io_error" - If file cannot be read or written
# @raises "dsl_compilation_error" - If DSL compilation fails
# @raises "invalid_filename" - If filename doesn't have .anim extension
def compile_file(filename)
  import string
  import animation_dsl
  
  # Validate input filename
  if !string.endswith(filename, ".anim")
    raise "invalid_filename", f"Input file must have .anim extension: {filename}"
  end
  
  # Generate output filename
  var base_name = filename[0..-6]  # Remove .anim extension (5 chars + 1 for 0-based)
  var output_filename = base_name + ".be"
  
  # Read DSL source
  var f = open(filename, "r")
  if f == nil
    raise "io_error", f"Cannot open input file: {filename}"
  end
  
  var dsl_source = f.read()
  f.close()
  
  # Compile DSL to Berry code
  var berry_code = animation_dsl.compile(dsl_source)
  if berry_code == nil
    raise "dsl_compilation_error", f"DSL compilation failed for: {filename}"
  end
  
  # Generate header with metadata (no original source for compile_file)
  var header = "# Generated Berry code from Animation DSL\n" +
               f"# Source: {filename}\n" +
               "# Generated automatically by animation_dsl.compile_file()\n" +
               "# \n" +
               "# Do not edit manually - changes will be overwritten\n" +
               "\n"
  
  # Write complete Berry file (no footer with original source)
  var output_f = open(output_filename, "w")
  if output_f == nil
    raise "io_error", f"Cannot create output file: {output_filename}"
  end
  
  output_f.write(header + berry_code)
  output_f.close()
  
  return true
end
animation_dsl.compile_file = compile_file

# this function is called when the module is loaded
def animation_dsl_init(m)
  import animation
  # load the Web UI component
  var animation_web_ui = m.animation_web_ui
  animation.web_ui = animation_web_ui()     # create an instance and store in "animation.web_ui"

  return m    # return the module unchanged
end
animation_dsl.init = animation_dsl_init

return animation_dsl
