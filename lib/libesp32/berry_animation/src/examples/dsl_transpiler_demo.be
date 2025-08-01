# DSL Transpiler Demo
# Demonstrates the single-pass DSL transpiler functionality
#
# Command to run demo is:
#    ./berry -s -g -m lib/libesp32/berry_animation lib/libesp32/berry_animation/examples/dsl_transpiler_demo.be

import tasmota
import animation

print("=== Animation DSL Transpiler Demo ===")

# Example 1: Simple color and strip configuration
print("\n--- Example 1: Basic Configuration ---")
var basic_dsl = "strip length 30\n" +
  "color red = #FF0000\n" +
  "color blue = #0000FF\n"

print("DSL Input:")
print(basic_dsl)

var basic_berry = animation.compile_dsl(basic_dsl)
if basic_berry != nil
  print("\nGenerated Berry Code:")
  print(basic_berry)
else
  print("❌ Compilation failed")
end

# Example 2: Pattern and animation definitions
print("\n--- Example 2: Patterns and Animations ---")
var pattern_dsl = "color green = #00FF00\n" +
  "pattern solid_green = solid(green)\n"

print("DSL Input:")
print(pattern_dsl)

var pattern_berry = animation.compile_dsl(pattern_dsl)
if pattern_berry != nil
  print("\nGenerated Berry Code:")
  print(pattern_berry)
else
  print("❌ Compilation failed")
end

# Example 3: Complete sequence with execution
print("\n--- Example 3: Complete Animation Sequence ---")
var sequence_dsl = "# LED Strip Setup\n" +
  "strip length 60\n" +
  "\n" +
  "# Colors\n" +
  "color red = #FF0000\n" +
  "color white = white\n" +
  "\n" +
  "# Patterns\n" +
  "pattern red_solid = solid(red)\n" +
  "\n" +
  "# Variables\n" +
  "set duration = 3s\n" +
  "\n" +
  "# Sequence\n" +
  "sequence blink {\n" +
  "  play red_solid for 2s\n" +
  "}\n" +
  "\n" +
  "# Run the sequence\n" +
  "run blink\n"

print("DSL Input:")
print(sequence_dsl)

var sequence_berry = animation.compile_dsl(sequence_dsl)
if sequence_berry != nil
  print("\nGenerated Berry Code:")
  print(sequence_berry)
else
  print("❌ Compilation failed")
end

# Example 4: Variable assignments and time conversions
print("\n--- Example 4: Variables and Time Conversions ---")
var variables_dsl = "set strip_length = 60\n" +
  "set brightness = 75%\n" +
  "set short_time = 500ms\n" +
  "set long_time = 2s\n" +
  "set very_long = 1m\n"

print("DSL Input:")
print(variables_dsl)

var variables_berry = animation.compile_dsl(variables_dsl)
if variables_berry != nil
  print("\nGenerated Berry Code:")
  print(variables_berry)
else
  print("❌ Compilation failed")
end

# Example 5: Error handling demonstration
print("\n--- Example 5: Error Handling ---")
var error_dsl = "# This should produce errors\n" +
  "invalid_keyword test = value\n" +
  "color incomplete = \n" +
  "strip length\n"

print("DSL Input (with intentional errors):")
print(error_dsl)

var lexer = animation.DSLLexer(error_dsl)
var tokens = lexer.tokenize()

if lexer.has_errors()
  print("\nLexical Errors:")
  print(lexer.get_error_report())
end

var transpiler = animation.SimpleDSLTranspiler(tokens)
var error_berry = transpiler.transpile()

if transpiler.has_errors()
  print("\nTranspilation Errors:")
  print(transpiler.get_error_report())
end

if error_berry != nil
  print("\nGenerated Berry Code (despite errors):")
  print(error_berry)
else
  print("❌ Compilation failed due to errors")
end

# Example 6: Demonstrate deferred resolution (forward references)
print("\n--- Example 6: Forward References ---")
var forward_ref_dsl = "# This pattern references colors defined later\n" +
  "animation test_anim = red_pattern\n" +
  "pattern red_pattern = solid(red)\n" +
  "color red = #FF0000\n"

print("DSL Input (with forward references):")
print(forward_ref_dsl)

var forward_berry = animation.compile_dsl(forward_ref_dsl)
if forward_berry != nil
  print("\nGenerated Berry Code:")
  print(forward_berry)
else
  print("❌ Forward reference resolution not yet fully implemented")
  
  # Show what the transpiler attempted
  var lexer2 = animation.DSLLexer(forward_ref_dsl)
  var tokens2 = lexer2.tokenize()
  var transpiler2 = animation.SimpleDSLTranspiler(tokens2)
  var result = transpiler2.transpile()
  
  if transpiler2.has_errors()
    print("\nTranspilation Errors:")
    print(transpiler2.get_error_report())
  end
end

# Example 7: Practical LED animation
print("\n--- Example 7: Practical LED Animation ---")
var practical_dsl = "# Campfire effect setup\n" +
  "strip length 60\n" +
  "\n" +
  "# Fire colors\n" +
  "color deep_red = #8B0000\n" +
  "color red = #FF0000\n" +
  "color orange = #FF4500\n" +
  "color yellow = #FFD700\n" +
  "\n" +
  "# Simple solid patterns for now\n" +
  "pattern fire_base = solid(red)\n" +
  "\n" +
  "# Animation\n" +
  "animation campfire = fire_base\n" +
  "\n" +
  "# Show the fire\n" +
  "sequence fire_show {\n" +
  "  play campfire for 10s\n" +
  "}\n" +
  "\n" +
  "run fire_show\n"

print("DSL Input:")
print(practical_dsl)

var practical_berry = animation.compile_dsl(practical_dsl)
if practical_berry != nil
  print("\nGenerated Berry Code:")
  print(practical_berry)
  
  print("\n--- Compilation Summary ---")
  print("✅ DSL successfully compiled to Berry code")
  print("✅ Strip configuration generated")
  print("✅ Color definitions converted")
  print("✅ Pattern definitions created")
  print("✅ Animation definitions prepared")
  print("✅ Sequence function generated")
  print("✅ Execution code added")
else
  print("❌ Compilation failed")
end

print("\n=== Demo Complete ===")
print("The DSL transpiler can now handle:")
print("• Strip configuration (length)")
print("• Color definitions (hex colors and named colors)")
print("• Simple pattern definitions (solid patterns)")
print("• Variable assignments with type conversion")
print("• Sequence definitions with play statements")
print("• Run statements for execution")
print("• Basic error detection and reporting")
print("")
print("Next steps for full DSL implementation:")
print("• Complex pattern functions (gradient, sparkle, etc.)")
print("• Advanced animation functions (shift, pulse, etc.)")
print("• Control flow statements (repeat, if, etc.)")
print("• Function definitions and calls")
print("• Event handlers")
print("• Full forward reference resolution")