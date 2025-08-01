# Pattern vs Animation Example
# This example demonstrates the correct usage of patterns and animations

import animation

# Example: Creating patterns (ColorProviders)
def create_patterns()
  print("Creating patterns...")
  
  # Patterns define WHAT the strip should look like (spatial)
  var red_pattern = animation.solid(0xFFFF0000)      # Solid red pattern
  var blue_pattern = animation.solid(0xFF0000FF)     # Solid blue pattern
  var white_pattern = animation.solid(0xFFFFFFFF)    # Solid white pattern
  
  print("✅ Created solid color patterns")
  return [red_pattern, blue_pattern, white_pattern]
end

# Example: Creating animations from patterns
def create_animations(patterns)
  print("Creating animations...")
  
  var red_pattern = patterns[0]
  var blue_pattern = patterns[1] 
  var white_pattern = patterns[2]
  
  # Animations define HOW patterns change over time (temporal)
  var red_fill = animation.filled(red_pattern, 0)     # Fill strip with red pattern
  var blue_fill = animation.filled(blue_pattern, 5)   # Fill strip with blue pattern (higher priority)
  var white_fill = animation.filled(white_pattern, 10) # Fill strip with white pattern (highest priority)
  
  print("✅ Created filled animations")
  return [red_fill, blue_fill, white_fill]
end

# Example: Demonstrating reusability
def demonstrate_reusability()
  print("Demonstrating pattern reusability...")
  
  # One pattern, multiple animations
  var red_pattern = animation.solid(0xFFFF0000)
  
  # Use the same pattern in different ways
  var static_red = animation.filled(red_pattern, 0, 0, true, "static")
  var priority_red = animation.filled(red_pattern, 10, 0, true, "priority")
  var timed_red = animation.filled(red_pattern, 0, 5000, false, "timed")  # 5 second duration
  
  print("✅ One pattern used in multiple animations:")
  print(f"  - Static: {static_red.name}, priority={static_red.priority}")
  print(f"  - Priority: {priority_red.name}, priority={priority_red.priority}")
  print(f"  - Timed: {timed_red.name}, duration={timed_red.duration}")
end

# Example: DSL-style composition
def demonstrate_dsl_style()
  print("Demonstrating DSL-style composition...")
  
  # This is how the DSL would work:
  # pattern solid_red = solid(red)
  var solid_red = animation.solid(0xFFFF0000)
  
  # animation red_anim = filled(solid_red)
  var red_anim = animation.filled(solid_red)
  
  # The animation can be used with an engine
  print(f"✅ Created DSL-style animation: {red_anim.name}")
  print(f"  - Uses pattern: {red_anim.color_provider}")
  print(f"  - Priority: {red_anim.priority}")
  print(f"  - Loop: {red_anim.loop}")
end

# Example: Testing with a mock strip
def test_with_mock_strip()
  print("Testing with mock strip...")
  
  try
    # Create a simple LED strip simulation
    var strip_length = 10
    var mock_strip = global.Leds(strip_length, 1)
    
    # Create pattern and animation
    var green_pattern = animation.solid(0xFF00FF00)
    var green_anim = animation.filled(green_pattern)
    
    # Create engine and add animation
    var engine = animation.create_engine(mock_strip)
    engine.add_animation(green_anim)
    
    # Start the engine (this would normally be called by Tasmota's fast_loop)
    engine.start()
    
    # Simulate one update cycle
    engine.on_tick(0)
    
    print("✅ Successfully tested with mock strip")
    
  except .. as e, msg
    print(f"⚠️  Mock strip test failed (expected in test environment): {msg}")
  end
end

# Main example function
def run_example()
  print("Pattern vs Animation Example")
  print("=" * 40)
  
  # Demonstrate the concepts
  var patterns = create_patterns()
  var animations = create_animations(patterns)
  
  demonstrate_reusability()
  demonstrate_dsl_style()
  test_with_mock_strip()
  
  print("=" * 40)
  print("Example completed!")
  
  print("\nKey Takeaways:")
  print("- Patterns define WHAT (spatial color distribution)")
  print("- Animations define HOW (temporal behavior)")
  print("- Patterns are reusable across multiple animations")
  print("- This separation enables powerful composition")
end

# Export the example function
return {'run_example': run_example}