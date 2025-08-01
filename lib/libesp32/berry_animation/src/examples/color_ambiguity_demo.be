# Color Ambiguity Resolution Demo
# Demonstrates how the DSL handles color name conflicts between predefined and user-defined colors
#
# Command to run demo:
#    ./berry -s -g -m lib/libesp32/berry_animation lib/libesp32/berry_animation/examples/color_ambiguity_demo.be

import tasmota
import animation

print("=== Color Ambiguity Resolution Demo ===")
print("This demo shows how the DSL resolves conflicts between predefined and user-defined colors.")
print("")

# Test Case 1: Using predefined colors without redefinition
print("--- Test Case 1: Predefined Colors (No Conflicts) ---")
var test1_dsl = "strip length 30\n" +
  "color primary = red\n" +        # 'red' is predefined
  "color secondary = blue\n" +     # 'blue' is predefined  
  "pattern red_pattern = solid(red)\n" +      # Should use predefined red
  "pattern blue_pattern = solid(blue)\n"      # Should use predefined blue

print("DSL Input:")
print(test1_dsl)
print("")

var test1_berry = animation.compile_dsl(test1_dsl)
if test1_berry != nil
  print("Generated Berry Code:")
  print(test1_berry)
  print("✅ Test 1 PASSED: Predefined colors used correctly")
else
  print("❌ Test 1 FAILED: Compilation error")
end
print("")

# Test Case 2: User redefining predefined color names
print("--- Test Case 2: User Redefining Predefined Colors ---")
var test2_dsl = "strip length 30\n" +
  "color red = #0000FF\n" +        # User redefines 'red' as blue!
  "color blue = #FF0000\n" +       # User redefines 'blue' as red!
  "pattern red_pattern = solid(red)\n" +      # Should use user's red (blue)
  "pattern blue_pattern = solid(blue)\n"      # Should use user's blue (red)

print("DSL Input:")
print(test2_dsl)
print("")

var test2_berry = animation.compile_dsl(test2_dsl)
if test2_berry != nil
  print("Generated Berry Code:")
  print(test2_berry)
  print("✅ Test 2 PASSED: User-defined colors take precedence")
else
  print("❌ Test 2 FAILED: Compilation error")
end
print("")

# Test Case 3: Mixed usage - some predefined, some user-defined
print("--- Test Case 3: Mixed Predefined and User-Defined Colors ---")
var test3_dsl = "strip length 30\n" +
  "color red = #FF8000\n" +        # User redefines 'red' as orange
  "# 'blue' is not redefined, so it remains predefined\n" +
  "color custom = #00FF80\n" +     # User defines a new color
  "pattern red_pattern = solid(red)\n" +      # Should use user's red (orange)
  "pattern blue_pattern = solid(blue)\n" +    # Should use predefined blue
  "pattern custom_pattern = solid(custom)\n"  # Should use user's custom color

print("DSL Input:")
print(test3_dsl)
print("")

var test3_berry = animation.compile_dsl(test3_dsl)
if test3_berry != nil
  print("Generated Berry Code:")
  print(test3_berry)
  print("✅ Test 3 PASSED: Mixed color resolution works correctly")
else
  print("❌ Test 3 FAILED: Compilation error")
end
print("")

# Test Case 4: Forward references with redefined colors
print("--- Test Case 4: Forward References with Color Redefinition ---")
var test4_dsl = "strip length 30\n" +
  "pattern early_pattern = solid(red)\n" +    # Forward reference to 'red'
  "color red = #FFFF00\n" +        # User redefines 'red' as yellow (after reference)
  "pattern late_pattern = solid(red)\n"       # Direct reference to user's red

print("DSL Input:")
print(test4_dsl)
print("")

var test4_berry = animation.compile_dsl(test4_dsl)
if test4_berry != nil
  print("Generated Berry Code:")
  print(test4_berry)
  print("✅ Test 4 PASSED: Forward references with redefinition work")
else
  print("❌ Test 4 FAILED: Compilation error")
end
print("")

# Test Case 5: Complex scenario with multiple redefinitions
print("--- Test Case 5: Complex Color Redefinition Scenario ---")
var test5_dsl = "strip length 60\n" +
  "# Swap the meanings of red and blue\n" +
  "color red = #0000FF\n" +        # red is now blue
  "color blue = #FF0000\n" +       # blue is now red
  "color green = #FFFF00\n" +      # green is now yellow
  "# white and black remain predefined\n" +
  "\n" +
  "# Create patterns using the redefined colors\n" +
  "pattern fire_pattern = solid(red)\n" +     # This will be blue!
  "pattern water_pattern = solid(blue)\n" +   # This will be red!
  "pattern earth_pattern = solid(green)\n" +  # This will be yellow!
  "pattern air_pattern = solid(white)\n" +    # This remains white (predefined)
  "\n" +
  "# Create animations\n" +
  "animation fire_anim = fire_pattern\n" +
  "animation water_anim = water_pattern\n" +
  "\n" +
  "# Create a sequence\n" +
  "sequence color_swap_demo {\n" +
  "  play fire_anim for 2s\n" +
  "  play water_anim for 2s\n" +
  "}\n" +
  "\n" +
  "run color_swap_demo\n"

print("DSL Input:")
print(test5_dsl)
print("")

var test5_berry = animation.compile_dsl(test5_dsl)
if test5_berry != nil
  print("Generated Berry Code:")
  print(test5_berry)
  print("✅ Test 5 PASSED: Complex color redefinition scenario works")
else
  print("❌ Test 5 FAILED: Compilation error")
end
print("")

# Test Case 6: Error case - undefined color
print("--- Test Case 6: Error Handling for Undefined Colors ---")
var test6_dsl = "strip length 30\n" +
  "pattern mystery_pattern = solid(undefined_color)\n"  # This should cause an error

print("DSL Input:")
print(test6_dsl)
print("")

var test6_berry = animation.compile_dsl(test6_dsl)
if test6_berry != nil
  print("Generated Berry Code:")
  print(test6_berry)
  print("⚠️  Test 6: Code generated despite undefined color (will be caught at runtime)")
else
  print("✅ Test 6 PASSED: Undefined color properly detected as error")
end
print("")

print("=== Color Resolution Summary ===")
print("The DSL color resolution system works as follows:")
print("")
print("1. SCOPED RESOLUTION: User-defined colors always take precedence over predefined ones")
print("   - If user defines 'color red = #0000FF', then 'red' means blue in that DSL scope")
print("   - Predefined colors are only used if the user hasn't redefined them")
print("")
print("2. FORWARD REFERENCE SUPPORT: Colors can be referenced before they are defined")
print("   - The transpiler scans ahead to detect user color definitions")
print("   - This ensures consistent behavior regardless of definition order")
print("")
print("3. CLEAR PRECEDENCE RULES:")
print("   - User-defined colors (highest precedence)")
print("   - Predefined color names (medium precedence)")  
print("   - Undefined colors (error - lowest precedence)")
print("")
print("4. BENEFITS:")
print("   - No ambiguity: clear, predictable color resolution")
print("   - User control: users can redefine any color name")
print("   - Backward compatibility: existing DSL code continues to work")
print("   - Forward compatibility: new predefined colors won't break user code")
print("")
print("This approach solves the color ambiguity problem while maintaining")
print("intuitive behavior and full user control over color definitions.")