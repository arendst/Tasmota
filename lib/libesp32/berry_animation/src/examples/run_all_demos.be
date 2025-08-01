# Run all demos for the Berry Animation Framework
#
# This script runs all the demo files in the examples directory
# and provides a simple menu to select which demo to run.

import global
import tasmota
import animation

# Define a function to run a demo file
def run_demo_file(file_path)
  print("Running " + file_path + "...")
  
  # Load the file content
  var f = open(file_path, "r")
  if f == nil
    print("Error: Could not open file " + file_path)
    return false
  end
  
  var content = f.read()
  f.close()
  
  # Compile and execute the file content
  try
    var compiled = compile(content)
    var result = compiled()
    
    # If the demo returns a handler, store it for later use
    if type(result) == 'instance' && result.contains('handler')
      return result
    end
    
    return true
  except .. as e
    print("Error executing " + file_path + ": " + str(e))
    return false
  end
end

# Main function to run demos
def run_demos()
  print("=== Berry Animation Framework Demo Suite ===")
  print("")
  
  var demo_files = [
    "lib/libesp32/berry_animation/examples/sine_int_demo.be",
    "lib/libesp32/berry_animation/examples/frame_buffer_demo.be",
    "lib/libesp32/berry_animation/examples/renderer_demo.be",
    "lib/libesp32/berry_animation/examples/advanced_renderer_demo.be",
    "lib/libesp32/berry_animation/examples/parameter_demo.be",
    "lib/libesp32/berry_animation/examples/animation_manager_demo.be",
    "lib/libesp32/berry_animation/examples/layering_demo.be",
    "lib/libesp32/berry_animation/examples/animation_controller_demo.be",
    "lib/libesp32/berry_animation/examples/fast_loop_demo.be",
    "lib/libesp32/berry_animation/examples/solid_animation_demo.be",  # Unified solid() demo
    "lib/libesp32/berry_animation/examples/pulse_animation_demo.be",
    "lib/libesp32/berry_animation/examples/breathe_animation_demo.be",
    "lib/libesp32/berry_animation/examples/color_cycle_animation_demo.be",
    "lib/libesp32/berry_animation/examples/rich_palette_animation_demo.be",
    "lib/libesp32/berry_animation/examples/palette_pattern_demo.be"
  ]
  
  var demo_names = [
    "Fixed-point Sine Demo",
    "Frame Buffer Demo",
    "Basic Renderer Demo",
    "Advanced Renderer Demo",
    "Animation Parameter Demo",
    "Animation Manager Demo",
    "Animation Layering Demo",
    "Animation Controller Demo",
    "Fast Loop Integration Demo",
    "Solid Animation Demo",
    "Pulse Animation Demo",
    "Breathe Animation Demo",
    "Color Cycle Animation Demo",
    "Rich Palette Animation Demo",
    "Palette Pattern Demo"
  ]
  
  var demo_descriptions = [
    "Demonstrates the optimized fixed-point sine function",
    "Demonstrates basic frame buffer operations",
    "Shows basic rendering capabilities",
    "Demonstrates advanced rendering features",
    "Shows how to use animation parameters",
    "Demonstrates animation management features",
    "Shows how to layer multiple animations",
    "Demonstrates the complete animation controller",
    "Shows the fast_loop integration for efficient updates",
    "Demonstrates the Solid animation effect",
    "Demonstrates the Pulse animation effect with color pulsing",
    "Demonstrates the Breathe animation effect with natural breathing curve",
    "Demonstrates the Color Cycle animation with smooth transitions between colors",
    "Demonstrates the Rich Palette animation with compact palette formats",
    "Demonstrates using RichPalette as a color provider for patterns"
  ]
  
  # Print menu
  print("Available demos:")
  print("")
  
  var i = 0
  while i < size(demo_files)
    print(format("%d. %s - %s", i + 1, demo_names[i], demo_descriptions[i]))
    i += 1
  end
  
  print("0. Run all demos sequentially")
  print("q. Quit")
  print("")
  
  # Get user choice
  print("Enter your choice (0-" + str(size(demo_files)) + " or q): ")
  var choice = input()
  
  if choice == "q" || choice == "Q"
    print("Exiting demo suite.")
    return
  end
  
  var choice_num = number(choice)
  
  if choice_num == 0
    # Run all demos
    print("Running all demos sequentially...")
    print("")
    
    var demo_handlers = []
    
    i = 0
    while i < size(demo_files)
      print("=== " + demo_names[i] + " ===")
      var result = run_demo_file(demo_files[i])
      
      if type(result) == 'instance' && result.contains('handler')
        demo_handlers.push(result)
      end
      
      print("")
      print("Press Enter to continue to the next demo...")
      input()
      print("")
      
      i += 1
    end
    
    print("All demos completed!")
    
    # Return the handlers for the last demo
    if size(demo_handlers) > 0
      return demo_handlers[size(demo_handlers) - 1]
    end
  elif choice_num >= 1 && choice_num <= size(demo_files)
    # Run selected demo
    var index = choice_num - 1
    print("=== " + demo_names[index] + " ===")
    return run_demo_file(demo_files[index])
  else
    print("Invalid choice. Please run the script again and select a valid option.")
  end
  
  return nil
end

# Run demos and get the result
var demo_result = run_demos()

# If the demo returned a handler, return it for interactive use
if demo_result != nil
  print("Demo handler available for interactive use.")
  return demo_result
end

# Return success
return true