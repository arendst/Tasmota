# Demo showing user-defined functions in DSL
#
# This example demonstrates how user-defined functions can be used in DSL
# after being registered in Berry code.

import animation

# Load user functions (this registers them with the animation module)
import "user_functions" as user_funcs

# Example DSL using user-defined functions
var dsl_code = 
  "strip length 60\n" +
  "\n" +
  "# Define colors\n" +
  "color red = #FF0000\n" +
  "color blue = #0000FF\n" +
  "color white = #FFFFFF\n" +
  "color orange = #FFA500\n" +
  "\n" +
  "# Use user-defined functions just like built-in functions\n" +
  "animation calm_breathing = breathing(blue, 4s)\n" +
  "animation emergency_lights = police_lights(200ms)\n" +
  "animation fire_effect = fire(200, 300ms)\n" +
  "animation sparkles = sparkle(white, blue, 10%)\n" +
  "animation rainbow = color_wheel(5s)\n" +
  "\n" +
  "# Nested calls with user functions\n" +
  "animation complex = fade(breathing(red, 2s), 3s)\n" +
  "\n" +
  "# Create a sequence using user-defined animations\n" +
  "sequence user_demo {\n" +
  "  play calm_breathing for 5s\n" +
  "  wait 1s\n" +
  "  play fire_effect for 8s\n" +
  "  wait 500ms\n" +
  "  play emergency_lights for 3s\n" +
  "  wait 1s\n" +
  "  play rainbow for 10s\n" +
  "  wait 500ms\n" +
  "  play complex for 6s\n" +
  "}\n" +
  "\n" +
  "run user_demo"

print("=== User Functions Demo ===")
print("Available user functions:")
var functions = animation.list_user_functions()
for func_name : functions
  print(f"  - {func_name}")
end
print()

print("DSL Code:")
print(dsl_code)
print()

print("Compiling DSL...")
var berry_code = animation.compile_dsl(dsl_code)

if berry_code != nil
  print("✓ DSL compiled successfully!")
  print()
  print("Generated Berry Code:")
  print(berry_code)
  print()
  
  print("Executing generated code...")
  try
    var compiled_func = compile(berry_code)
    if compiled_func != nil
      print("✓ Berry code compiled successfully!")
      # Note: We don't execute it here since we don't have actual LED strip
      print("✓ Demo completed successfully!")
    else
      print("✗ Failed to compile generated Berry code")
    end
  except .. as e, msg
    print(f"✗ Error executing generated code: {e} - {msg}")
  end
else
  print("✗ Failed to compile DSL")
end

return true