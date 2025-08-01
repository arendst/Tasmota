# DSL Runtime Integration Demo
# Demonstrates complete DSL execution lifecycle with practical examples

import global
import tasmota
import animation

def demo_basic_dsl_runtime()
  print("=== Basic DSL Runtime Demo ===")
  
  # Create LED strip (30 pixels)
  var strip = global.Leds(30, 1)
  
  # Create DSL runtime with debug mode
  var runtime = animation.create_dsl_runtime(strip, true)
  
  # Define a simple animation DSL
  var campfire_dsl =
    "# Campfire animation DSL\n" +
    "strip length 30\n" +
    "\n" +
    "# Define fire colors\n" +
    "color red = #FF0000\n" +
    "color orange = #FF8000\n" +
    "\n" +
    "# Create fire animation\n" +
    "animation flames = solid(red)\n" +
    "\n" +
    "# Define the campfire sequence\n" +
    "sequence campfire {\n" +
    "  play flames for 5s\n" +
    "}\n" +
    "\n" +
    "# Start the campfire\n" +
    "run campfire"
  
  print("Loading campfire DSL...")
  print("Runtime created successfully")
  print("```")
  print(campfire_dsl)
  print("```")
  if runtime.load_dsl(campfire_dsl)
    print("✓ Campfire DSL loaded successfully!")
    
    # Show generated Berry code (first 200 characters)
    var generated_code = runtime.get_generated_code()
    if generated_code != nil
      print("Generated Berry code preview:")
      # var preview = size(generated_code) > 200 ? generated_code[0..199] + "..." : generated_code
      # print(generated_code)
      print(generated_code)
    end
    
    print("Campfire animation is now running!")
    print("(In real usage, this would continue until stopped)")
    
  else
    print("✗ Failed to load campfire DSL")
  end
end

def demo_dsl_hot_reloading()
  print("\n=== DSL Hot Reloading Demo ===")
  
  var strip = global.Leds(20, 1)
  var runtime = animation.create_dsl_runtime(strip, true)
  
  # Load initial animation
  var initial_dsl = "strip length 20\n" +
    "color blue = #0000FF\n" +
    "animation blue_anim = solid(blue)\n" +
    "sequence initial {\n" +
    "  play blue_anim for 3s\n" +
    "}\n" +
    "run initial"
  
  print("Loading initial blue animation...")
  if runtime.load_dsl(initial_dsl)
    print("✓ Initial animation loaded")
    
    # Simulate development workflow - modify and reload
    var updated_dsl = "strip length 20\n" +
      "color red = #FF0000\n" +
      "animation red_anim = solid(red)\n" +
      "sequence updated {\n" +
      "  play red_anim for 5s\n" +
      "}\n" +
      "run updated"
    
    print("Updating to red/white strobe animation...")
    if runtime.load_dsl(updated_dsl)
      print("✓ Animation updated successfully!")
      
    else
      print("✗ Failed to update animation")
    end
  else
    print("✗ Failed to load initial animation")
  end
end

def demo_dsl_error_handling()
  print("\n=== DSL Error Handling Demo ===")
  
  var strip = global.Leds(15, 1)
  var runtime = animation.create_dsl_runtime(strip, true)
  
  # Try to load DSL with various errors
  var error_cases = [
    {
      "name": "Syntax Error",
      "dsl": "color red = \n" +
        "pattern broken = solid(red)"
    },
    {
      "name": "Undefined Reference",
      "dsl": "strip length 15\n" +
        "pattern test = solid(undefined_color)\n" +
        "sequence demo { play test for 1s }\n" +
        "run demo"
    },
    {
      "name": "Invalid Color Format",
      "dsl": "strip length 15\n" +
        "color bad = #GGGGGG\n" +
        "sequence demo { play solid(bad) for 1s }\n" +
        "run demo"
    }
  ]
  
  for case : error_cases
    print(f"\nTesting {case['name']}:")
    if runtime.load_dsl(case["dsl"])
      print(f"✗ Expected {case['name']} to fail, but it succeeded")
    else
      print(f"✓ {case['name']} correctly rejected")
    end
  end
  
  # Show that runtime is still functional after errors
  var valid_dsl = "strip length 15\n" +
    "color green = #00FF00\n" +
    "animation green_anim = solid(green)\n" +
    "sequence recovery {\n" +
    "  play green_anim for 2s\n" +
    "}\n" +
    "run recovery"
  
  print("\nTesting recovery with valid DSL:")
  if runtime.load_dsl(valid_dsl)
    print("✓ Runtime recovered successfully after errors")
  else
    print("✗ Runtime failed to recover")
  end
end

def demo_dsl_caching_performance()
  print("\n=== DSL Caching Performance Demo ===")
  
  var strip = global.Leds(25, 1)
  var runtime = animation.create_dsl_runtime(strip, false)  # Disable debug for cleaner output
  
  var test_dsl = "strip length 25\n" +
    "color cyan = #00FFFF\n" +
    "animation wave = solid(cyan)\n" +
    "sequence performance_test {\n" +
    "  play wave for 5s\n" +
    "}\n" +
    "run performance_test"
  
  print("Testing compilation performance...")
  
  # First load (compilation)
  var start_time = tasmota.millis()
  var success1 = runtime.load_dsl(test_dsl)
  var first_load_time = tasmota.millis() - start_time
  
  if success1
    print(f"✓ DSL compilation: {first_load_time}ms")
  else
    print("✗ Performance test failed")
  end
end

def demo_advanced_dsl_features()
  print("\n=== Advanced DSL Features Demo ===")
  
  var strip = global.Leds(40, 1)
  var runtime = animation.create_dsl_runtime(strip, true)
  
  # Complex DSL with multiple patterns and sequences
  var advanced_dsl = "# Advanced LED Animation Demo\n" +
    "strip length 40\n" +
    "\n" +
    "# Color palette\n" +
    "color blue = #0000FF\n" +
    "color white = #FFFFFF\n" +
    "color gold = #FFD700\n" +
    "\n" +
    "# Multiple animations\n" +
    "animation ocean_waves = solid(blue)\n" +
    "animation sunset_glow = solid(gold)\n" +
    "animation lighthouse = solid(white)\n" +
    "\n" +
    "# Complex sequence with multiple phases\n" +
    "sequence coastal_scene {\n" +
    "  play ocean_waves for 3s\n" +
    "  play sunset_glow for 3s\n" +
    "  play lighthouse for 2s\n" +
    "}\n" +
    "\n" +
    "run coastal_scene"
  
  print("Loading advanced coastal scene DSL...")
  if runtime.load_dsl(advanced_dsl)
    print("✓ Advanced DSL loaded successfully!")
    print("Coastal scene animation with multiple phases is now running")
    
    # Show the complexity of generated code
    var generated_code = runtime.get_generated_code()
    if generated_code != nil
      var lines = 0
      for i : 0..size(generated_code)-1
        if generated_code[i] == '\n'
          lines += 1
        end
      end
      print(f"Generated {lines} lines of Berry code from DSL")
    end
    
  else
    print("✗ Failed to load advanced DSL")
  end
end

# Main demo function
def run_dsl_runtime_demos()
  print("🌟 DSL Runtime Integration Demos 🌟")
  print("Demonstrating complete DSL execution lifecycle\n")
  
  demo_basic_dsl_runtime()
  demo_dsl_hot_reloading()
  demo_dsl_error_handling()
  demo_dsl_caching_performance()
  demo_advanced_dsl_features()
  
  print("\n🎉 DSL Runtime demos completed!")
  print("The DSL Runtime provides:")
  print("  • Complete DSL-to-animation execution")
  print("  • Intelligent compilation caching")
  print("  • Hot reloading for development")
  print("  • Robust error handling")
  print("  • File-based DSL loading")
  print("  • Performance optimization")
end

run_dsl_runtime_demos()

return {
  "run_dsl_runtime_demos": run_dsl_runtime_demos,
  "demo_basic_dsl_runtime": demo_basic_dsl_runtime,
  "demo_dsl_hot_reloading": demo_dsl_hot_reloading,
  "demo_dsl_error_handling": demo_dsl_error_handling,
  "demo_dsl_caching_performance": demo_dsl_caching_performance,
  "demo_advanced_dsl_features": demo_advanced_dsl_features
}