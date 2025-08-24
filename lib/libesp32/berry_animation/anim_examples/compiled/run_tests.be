#!/usr/bin/env berry
# Test runner for compiled DSL examples
# Generated automatically by compile_dsl_examples.be

import os
import sys

# Add animation library path
sys.path().push("lib/libesp32/berry_animation/src")

# Import animation framework
import animation
import animation_dsl

def run_compiled_example(filename)
  print(f"Running {filename}...")
  try
    var f = open(f"lib/libesp32/berry_animation/anim_examples/compiled/{filename}", "r")
    var code = f.read()
    f.close()
    
    var compiled_func = compile(code)
    if compiled_func != nil
      compiled_func()
      print(f"  ✓ {filename} executed successfully")
      return true
    else
      print(f"  ✗ {filename} failed to compile")
      return false
    end
  except .. as e, msg
    print(f"  ✗ {filename} execution failed: {msg}")
    return false
  end
end

def run_all_examples()
  var files = os.listdir("lib/libesp32/berry_animation/anim_examples/compiled")
  var success_count = 0
  var total_count = 0
  
  for file : files
    if string.endswith(file, ".be")
      total_count += 1
      if run_compiled_example(file)
        success_count += 1
      end
    end
  end
  
  print(f"\nTest Results: {success_count}/{total_count} examples ran successfully")
end

# Run all examples if script is executed directly
run_all_examples()
