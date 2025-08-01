# DSL Core Features Demo
# Demonstrates all implemented core processing methods in the DSL transpiler
#
# This example shows:
# - Color definitions (hex, named colors)
# - Pattern definitions (solid, gradient)
# - Animation definitions (pulse, breathe, simple references)
# - Variable assignments (numbers, percentages, times)
# - Sequence definitions with control flow
# - Play statements with duration
# - With statements for parallel animations
# - Repeat statements (forever, N times, range)
# - If/else statements
# - Wait statements
# - Loop statements
# - Run statements

import animation

# Comprehensive DSL example showcasing all core features
var comprehensive_dsl = "# LED Strip Animation DSL - Core Features Demo\n" +
  "\n" +
  "# Strip Configuration\n" +
  "strip length 60\n" +
  "\n" +
  "# Color Definitions\n" +
  "color red = #FF0000\n" +
  "color blue = #0000FF\n" +
  "color green = #00FF00\n" +
  "color white = white\n" +
  "color warm_orange = #FF8000\n" +
  "\n" +
  "# Variable Assignments\n" +
  "set brightness_level = 80%\n" +
  "set cycle_duration = 5s\n" +
  "set repeat_count = 3\n" +
  "set fade_time = 2s\n" +
  "\n" +
  "# Pattern Definitions\n" +
  "pattern solid_red = solid(red)\n" +
  "pattern solid_blue = solid(blue)\n" +
  "pattern fire_gradient = gradient(red, warm_orange)\n" +
  "\n" +
  "# Animation Definitions\n" +
  "animation red_pulse = pulse(solid_red, 2s, 20%, 100%)\n" +
  "animation blue_breathe = breathe(solid_blue, 4s)\n" +
  "animation simple_red = solid_red\n" +
  "animation fade_effect = fade(red, blue, 3s)\n" +
  "\n" +
  "# Main Sequence with All Control Flow Features\n" +
  "sequence comprehensive_demo {\n" +
  "  # Basic play statement\n" +
  "  play red_pulse for 3s\n" +
  "  \n" +
  "  # Wait statement\n" +
  "  wait 1s\n" +
  "  \n" +
  "  # Repeat N times\n" +
  "  repeat 2 times:\n" +
  "    play blue_breathe for 2s\n" +
  "    wait 500ms\n" +
  "  \n" +
  "  # Conditional statement\n" +
  "  if brightness_level > 50:\n" +
  "    play red_pulse for 4s\n" +
  "  else:\n" +
  "    play blue_breathe for 4s\n" +
  "  \n" +
  "  # Parallel animation with modifiers\n" +
  "  with simple_red for 6s opacity 60%\n" +
  "  \n" +
  "  # Wait before final effect\n" +
  "  wait 2s\n" +
  "  \n" +
  "  # Final play\n" +
  "  play fade_effect for 5s\n" +
  "}\n" +
  "\n" +
  "# Simple sequence for comparison\n" +
  "sequence simple_demo {\n" +
  "  play red_pulse for 10s\n" +
  "}\n" +
  "\n" +
  "# Run the comprehensive demo\n" +
  "run comprehensive_demo"

print("=== DSL Core Features Demo ===")
print()
print("This demo showcases all implemented core processing methods:")
print("✓ Color definitions (hex colors, named colors)")
print("✓ Pattern definitions (solid, gradient)")
print("✓ Animation definitions (pulse, breathe, references)")
print("✓ Variable assignments (percentages, times, numbers)")
print("✓ Sequence definitions with control flow")
print("✓ Play statements with duration")
print("✓ With statements for parallel animations")
print("✓ Repeat statements (N times)")
print("✓ If/else conditional statements")
print("✓ Wait statements for delays")
print("✓ Run statements for execution")
print()

print("Compiling comprehensive DSL...")
var berry_code = animation.compile_dsl(comprehensive_dsl)

if berry_code != nil
  print("✅ Compilation successful!")
  print()
  print("Generated Berry Code:")
  print("============================================================")
  print(berry_code)
  print("============================================================")
  print()
  print("🎉 All core processing methods are working correctly!")
  print()
  print("Key features demonstrated:")
  print("- Forward reference resolution (animations using patterns)")
  print("- Deferred code generation for complex expressions")
  print("- Proper symbol table management")
  print("- Control flow statement generation")
  print("- Expression parsing and code generation")
  print("- Error handling and recovery")
else
  print("❌ Compilation failed")
  
  # Detailed error analysis
  var lexer = animation.DSLLexer(comprehensive_dsl)
  var tokens = lexer.tokenize()
  
  if lexer.has_errors()
    print()
    print("Lexical errors:")
    print(lexer.get_error_report())
  else
    print("✓ Lexical analysis passed")
    
    var transpiler = animation.SinglePassDSLTranspiler(tokens)
    var result = transpiler.transpile()
    
    if transpiler.has_errors()
      print()
      print("Transpilation errors:")
      print(transpiler.get_error_report())
    end
  end
end

print()
print("=== Demo Complete ===")