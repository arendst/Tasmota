# Nested Function Calls Demo
#
# This example demonstrates the new nested function calls feature in the DSL transpiler.
# It shows how complex animations can be expressed more concisely using nested syntax.

import global
import string
import animation

# Example DSL code using nested function calls
var dsl_code = 
  "strip length 60\n"
  "\n"
  "# Define colors\n"
  "color sunset_red = #FF4500\n"
  "color ocean_blue = #0077AA\n"
  "color star_white = #FFFFFF\n"
  "color deep_blue = #000080\n"
  "\n"
  "# Simple nested call\n"
  "animation breathing_red = pulse(solid(sunset_red), 4s)\n"
  "\n"
  "# Complex nested animation with multiple levels\n"
  "animation evening_sky = fade(\n"
  "  overlay(\n"
  "    shift_right(gradient(sunset_red, orange, yellow), 400ms),\n"
  "    sparkle(star_white, deep_blue, 8%)\n"
  "  ),\n"
  "  10s\n"
  ")\n"
  "\n"
  "# Nested calls in array parameters\n"
  "animation color_wheel = color_cycle(\n"
  "  [solid(sunset_red), solid(ocean_blue), solid(star_white)],\n"
  "  3s\n"
  ")\n"
  "\n"
  "# Run the complex animation\n"
  "run evening_sky"

def demo_nested_calls()
  print("=== Nested Function Calls Demo ===")
  print()
  print("DSL Code:")
  print(dsl_code)
  print()
  
  # Compile the DSL
  var compiler = animation.SimpleDSLTranspiler()
  var lexer = animation.DSLLexer(dsl_code)
  var tokens = lexer.tokenize()
  
  if lexer.has_errors()
    print("Lexer errors:")
    print(lexer.get_error_report())
    return false
  end
  
  compiler.tokens = tokens
  var berry_code = compiler.transpile()
  
  if compiler.has_errors()
    print("Compiler errors:")
    print(compiler.get_error_report())
    return false
  end
  
  print("Generated Berry Code:")
  print("============================================================")
  print(berry_code)
  print("============================================================")
  print()
  
  # Verify the code compiles
  try
    var compiled = compile(berry_code)
    print("✓ Generated code compiles successfully!")
    print()
    
    # Show the benefits of nested syntax
    print("Benefits of Nested Function Calls:")
    print("- More concise: No need for intermediate pattern variables")
    print("- More readable: Natural expression of complex animations")
    print("- Less verbose: Reduces DSL code length significantly")
    print("- Better composition: Easy to build complex effects from simple parts")
    
    return true
  except .. as e, msg
    print(f"✗ Generated code compilation failed: {e} - {msg}")
    return false
  end
end

# Run the demo
demo_nested_calls()

return demo_nested_calls