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
import "dsl/runtime.be" as dsl_runtime
register_to_dsl(dsl_runtime)
import "dsl/named_colors.be" as dsl_named_colors
register_to_dsl(dsl_named_colors)

# Main DSL compilation function
# Compiles DSL source code to Berry code
#
# @param source: string - DSL source code
# @return string - Generated Berry code
def compile_dsl_source(source)
  return animation_dsl.compile_dsl(source)
end
animation_dsl.compile = compile_dsl_source

# Execute DSL source code
# Compiles and executes DSL source in one step
#
# @param source: string - DSL source code
# @return any - Result of execution
def execute(source)
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
  var f = open(filename, "r")
  if f == nil
    raise "io_error", f"Cannot open DSL file: {filename}"
  end
  
  var source = f.read()
  f.close()
  
  return animation_dsl.execute(source)
end
animation_dsl.load_file = load_file

# Create a DSL runtime instance
#
# @return DSLRuntime - New runtime instance
def create_runtime(strip, debug_mode)
  var engine = animation.create_engine(strip)
  return animation_dsl.DSLRuntime(engine, debug_mode)
end
animation_dsl.create_runtime = create_runtime

return animation_dsl
