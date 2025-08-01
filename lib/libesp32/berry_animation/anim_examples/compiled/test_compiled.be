#!/usr/bin/env berry
# Simple test runner for working DSL examples

import os
import sys
sys.path().push("lib/libesp32/berry_animation")
import animation

def test_compiled_file(filename)
  print(f"Testing {filename}...")
  try
    var f = open(f"anim_examples/compiled/{filename}", "r")
    var code = f.read()
    f.close()
    
    # Try to compile the Berry code
    var compiled_func = compile(code)
    if compiled_func != nil
      print(f"  ✓ {filename} compiles successfully")
      return true
    else
      print(f"  ✗ {filename} failed to compile")
      return false
    end
  except .. as e, msg
    print(f"  ✗ {filename} test failed: {msg}")
    return false
  end
end

# Test all .be files in compiled directory
var files = os.listdir("compiled")
var success_count = 0
var total_count = 0

for file : files
  import string
  if string.endswith(file, ".be")
    total_count += 1
    if test_compiled_file(file)
      success_count += 1
    end
  end
end

print(f"\nResults: {success_count}/{total_count} files compiled successfully")
