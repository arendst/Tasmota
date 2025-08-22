# Test suite for Palette DSL support
# Tests the new palette syntax in the Animation DSL

import animation

# Test basic palette definition and compilation
def test_palette_definition()
  print("Testing palette definition...")
  
  var dsl_source = 
    "strip length 30\n" +
    "\n" +
    "# Define a simple palette\n" +
    "palette test_palette = [\n" +
    "  (0, #FF0000),    # Red at position 0\n" +
    "  (128, #00FF00),  # Green at position 128\n" +
    "  (255, #0000FF)   # Blue at position 255\n" +
    "]\n" +
    "\n" +
    "# Use the palette in an animation\n" +
    "animation test_anim = filled(\n" +
    "  rich_palette(test_palette, 5s, smooth, 255),\n" +
    "  loop\n" +
    ")\n" +
    "\n" +
    "sequence demo {\n" +
    "  play test_anim for 10s\n" +
    "}\n" +
    "\n" +
    "run demo"
  
  # Compile the DSL
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "DSL compilation should succeed")
  
  # Check that the generated code contains the palette definition
  import string
  assert(string.find(berry_code, "var palette_test_palette = bytes(") != -1, 
         "Generated code should contain palette definition")
  
  # Check that the palette data is in VRGB format
  assert(string.find(berry_code, '"00FF0000"') != -1, "Should contain red entry in VRGB format")
  assert(string.find(berry_code, '"8000FF00"') != -1, "Should contain green entry in VRGB format") 
  assert(string.find(berry_code, '"FF0000FF"') != -1, "Should contain blue entry in VRGB format")
  
  print("✓ Palette definition test passed")
  return berry_code
end

# Test palette with named colors
def test_palette_with_named_colors()
  print("Testing palette with named colors...")
  
  var dsl_source = 
    "strip length 30\n" +
    "\n" +
    "palette rainbow_palette = [\n" +
    "  (0, red),\n" +
    "  (64, orange),\n" +
    "  (128, yellow),\n" +
    "  (192, green),\n" +
    "  (255, blue)\n" +
    "]\n"
  
  var berry_code = animation_dsl.compile(dsl_source)
  assert(berry_code != nil, "DSL compilation with named colors should succeed")
  
  # Check that named colors are properly converted
  import string
  assert(string.find(berry_code, "var palette_rainbow_palette = bytes(") != -1,
         "Should contain palette definition")
  
  print("✓ Palette with named colors test passed")
end

# Test palette with custom colors
def test_palette_with_custom_colors()
  print("Testing palette with custom colors...")
  
  var dsl_source = 
    "strip length 30\n" +
    "\n" +
    "# Define custom colors first\n" +
    "color aurora_green = #00AA44\n" +
    "color aurora_purple = #8800AA\n" +
    "\n" +
    "palette aurora_palette = [\n" +
    "  (0, #000022),      # Dark night sky\n" +
    "  (64, aurora_green), # Custom green\n" +
    "  (192, aurora_purple), # Custom purple\n" +
    "  (255, #CCAAFF)     # Pale purple\n" +
    "]\n"
  
  var berry_code = animation_dsl.compile(dsl_source)
  assert(berry_code != nil, "DSL compilation with custom colors should succeed")
  
  print("✓ Palette with custom colors test passed")
end

# Test error handling for invalid palette syntax
def test_palette_error_handling()
  print("Testing palette error handling...")
  
  # Test missing bracket
  var invalid_dsl1 = 
    "palette bad_palette = (\n" +
    "  (0, #FF0000)\n" +
    "]\n"
  
  var result1 = animation_dsl.compile(invalid_dsl1)
  assert(result1 == nil, "Should fail with missing opening bracket")
  
  # Test missing comma in tuple
  var invalid_dsl2 = 
    "palette bad_palette = [\n" +
    "  (0 #FF0000)\n" +
    "]\n"
  
  var result2 = animation_dsl.compile(invalid_dsl2)
  assert(result2 == nil, "Should fail with missing comma in tuple")
  
  print("✓ Palette error handling test passed")
end

# Test that palettes work with the animation framework
def test_palette_integration()
  print("Testing palette integration with animation framework...")
  
  var dsl_source = 
    "strip length 10\n" +
    "\n" +
    "palette fire_palette = [\n" +
    "  (0, #000000),    # Black\n" +
    "  (64, #800000),   # Dark red\n" +
    "  (128, #FF0000),  # Red\n" +
    "  (192, #FF8000),  # Orange\n" +
    "  (255, #FFFF00)   # Yellow\n" +
    "]\n"
  
  var berry_code = animation_dsl.compile(dsl_source)
  assert(berry_code != nil, "DSL compilation should succeed")
  
  # Try to execute the compiled code
  try
    var compiled_func = compile(berry_code)
    assert(compiled_func != nil, "Berry code should compile successfully")
    
    # Execute to create the palette
    compiled_func()
    
    # Check that the palette was created
    assert(global.contains('palette_fire_palette'), "Palette should be created in global scope")
    
    var palette = global.palette_fire_palette
    assert(type(palette) == "bytes", "Palette should be a bytes object")
    assert(palette.size() == 20, "Palette should have 20 bytes (5 entries × 4 bytes each)")
    
    print("✓ Palette integration test passed")
  except .. as e, msg
    print(f"Integration test failed: {e} - {msg}")
    assert(false, "Palette integration should work")
  end
end

# Test VRGB format validation
def test_vrgb_format_validation()
  print("Testing VRGB format validation...")
  
  var dsl_source = 
    "strip length 30\n" +
    "palette aurora_colors = [\n" +
    "  (0, #000022),    # Dark night sky\n" +
    "  (64, #004400),   # Dark green\n" +
    "  (128, #00AA44),  # Aurora green\n" +
    "  (192, #44AA88),  # Light green\n" +
    "  (255, #88FFAA)   # Bright aurora\n" +
    "]\n"
  
  var berry_code = animation_dsl.compile(dsl_source)
  assert(berry_code != nil, "Aurora palette compilation should succeed")
  
  # Execute and verify VRGB format
  try
    var compiled_func = compile(berry_code)
    compiled_func()
    
    if global.contains('palette_aurora_colors')
      var palette = global.palette_aurora_colors
      var hex_data = palette.tohex()
      
      # Verify expected VRGB entries
      var expected_entries = [
        "00000022",  # (0, #000022)
        "40004400",  # (64, #004400)  
        "8000AA44",  # (128, #00AA44)
        "C044AA88",  # (192, #44AA88)
        "FF88FFAA"   # (255, #88FFAA)
      ]
      
      for i : 0..size(expected_entries)-1
        var expected = expected_entries[i]
        var start_pos = i * 8
        var actual = hex_data[start_pos..start_pos+7]
        assert(actual == expected, f"Entry {i}: expected {expected}, got {actual}")
      end
      
      print("✓ VRGB format validation test passed")
    else
      assert(false, "Aurora palette not found in global scope")
    end
    
  except .. as e, msg
    print(f"VRGB validation failed: {e} - {msg}")
    assert(false, "VRGB format validation should work")
  end
end

# Test complete workflow with multiple palettes
def test_complete_workflow()
  print("Testing complete workflow with multiple palettes...")
  
  var complete_dsl = 
    "strip length 20\n" +
    "\n" +
    "# Define multiple palettes\n" +
    "palette warm_colors = [\n" +
    "  (0, #FF0000),    # Red\n" +
    "  (128, #FFA500),  # Orange\n" +
    "  (255, #FFFF00)   # Yellow\n" +
    "]\n" +
    "\n" +
    "palette cool_colors = [\n" +
    "  (0, blue),       # Blue\n" +
    "  (128, cyan),     # Cyan\n" +
    "  (255, white)     # White\n" +
    "]\n" +
    "\n" +
    "# Create animations\n" +
    "animation warm_glow = filled(\n" +
    "  rich_palette(warm_colors, 4s, smooth, 255),\n" +
    "  loop\n" +
    ")\n" +
    "\n" +
    "animation cool_flow = filled(\n" +
    "  rich_palette(cool_colors, 6s, smooth, 200),\n" +
    "  loop\n" +
    ")\n" +
    "\n" +
    "# Sequence with both palettes\n" +
    "sequence color_demo {\n" +
    "  play warm_glow for 5s\n" +
    "  wait 500ms\n" +
    "  play cool_flow for 5s\n" +
    "}\n" +
    "\n" +
    "run color_demo"
  
  # Test compilation
  var berry_code = animation_dsl.compile(complete_dsl)
  assert(berry_code != nil, "Complete workflow DSL should compile")
  
  # Verify generated code contains required elements
  import string
  var required_elements = [
    "var palette_warm_colors = bytes(",
    "var palette_cool_colors = bytes(",
    "rich_palette(warm_colors",
    "rich_palette(cool_colors",
    "def sequence_color_demo()",
    "engine.start()"
  ]
  
  for element : required_elements
    assert(string.find(berry_code, element) != -1, f"Missing element: {element}")
  end
  
  # Test execution
  try
    var compiled_func = compile(berry_code)
    compiled_func()
    
    # Verify both palettes were created
    assert(global.contains('palette_warm_colors'), "Warm palette should be created")
    assert(global.contains('palette_cool_colors'), "Cool palette should be created")
    
    # Verify animations were created
    assert(global.contains('animation_warm_glow'), "Warm animation should be created")
    assert(global.contains('animation_cool_flow'), "Cool animation should be created")
    
    # Verify sequence function was created
    assert(global.contains('sequence_color_demo'), "Sequence function should be created")
    
    print("✓ Complete workflow test passed")
    
  except .. as e, msg
    print(f"Complete workflow execution failed: {e} - {msg}")
    assert(false, "Complete workflow should execute successfully")
  end
end

# Test palette keyword recognition
def test_palette_keyword_recognition()
  print("Testing palette keyword recognition...")
  
  var simple_palette_dsl = "palette test = [(0, #FF0000)]"
  var lexer = animation_dsl.DSLLexer(simple_palette_dsl)
  var tokens = lexer.tokenize()
  
  var found_palette_keyword = false
  for token : tokens
    if token.type == animation_dsl.Token.KEYWORD && token.value == "palette"
      found_palette_keyword = true
      break
    end
  end
  
  assert(found_palette_keyword, "Palette keyword should be recognized by lexer")
  print("✓ Palette keyword recognition test passed")
end

# Run all palette tests
def run_palette_tests()
  print("=== Palette DSL Tests ===")
  
  try
    test_palette_keyword_recognition()
    test_palette_definition()
    test_palette_with_named_colors()
    test_palette_with_custom_colors()
    test_palette_error_handling()
    test_palette_integration()
    test_vrgb_format_validation()
    test_complete_workflow()
    
    print("=== All palette tests passed! ===")
    return true
  except .. as e, msg
    print(f"Palette test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_palette_tests = run_palette_tests

return run_palette_tests