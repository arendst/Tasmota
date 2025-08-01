#
# DSL Demo Runner
# Loads and runs DSL animation files (.anim extension)
# Usage: Modify the 'demo_file' variable to run different DSL demos
#

import animation

# Configuration - change this to run different DSL demos
var demo_file = "breathe_demo.anim"  # Options:
                                     # - breathe_demo.anim
                                     # - palette_background_demo.anim  
                                     # - pulse_demo.anim
                                     # - enhanced_breathe_showcase.anim
                                     # - advanced_palette_showcase.anim
                                     # - dynamic_pulse_showcase.anim

var leds = 30  # Adjust based on your LED strip

# Create LED strip
var strip = global.Leds(leds, gpio.pin(gpio.WS2812, 0))

# Create DSL runtime
var runtime = animation.create_dsl_runtime(strip)

# Load and run the DSL file
print("Loading DSL demo:", demo_file)
print("=" * 50)

var success = runtime.load_dsl_file("lib/libesp32/berry_animation/examples/dsl/" + demo_file)

if success
  print("DSL demo loaded and running successfully!")
  print("Visual effect should now be visible on your LED strip")
  print("\nTo try different demos, edit the 'demo_file' variable above")
  print("Available DSL demos:")
  print("- breathe_demo.anim (simple breathing effect)")
  print("- palette_background_demo.anim (rainbow background)")
  print("- pulse_demo.anim (moving pulse with color cycling)")
  print("- enhanced_breathe_showcase.anim (multi-layer breathing)")
  print("- advanced_palette_showcase.anim (complex palette effects)")
  print("- dynamic_pulse_showcase.anim (multiple pulse types)")
else
  print("Failed to load DSL demo!")
  print("Check that the file exists and the DSL syntax is correct")
end

print("\nPress Ctrl+C to stop")