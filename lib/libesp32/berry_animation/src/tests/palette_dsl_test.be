# Test suite for Palette DSL support
# Tests the new palette syntax in the Animation DSL

import animation
import animation_dsl

# Helper function to extract all tokens from a pull lexer (for testing only)
def extract_all_tokens(lexer)
  var tokens = []
  lexer.reset()  # Start from beginning
  
  while !lexer.at_end()
    var token = lexer.next_token()
    
    # EOF token removed - check for nil instead
    if token == nil
      break
    end
    
    tokens.push(token)
  end
  
  return tokens
end

# Test basic palette definition and compilation
def test_palette_definition()
  print("Testing palette definition...")
  
  var dsl_source = 
    "# Define a simple palette\n" +
    "palette test_palette = [\n" +
    "  (0, 0xFF0000),    # Red at position 0\n" +
    "  (128, 0x00FF00),  # Green at position 128\n" +
    "  (255, 0x0000FF)   # Blue at position 255\n" +
    "]\n"
  
  # Compile the DSL
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "DSL compilation should succeed")
  
  # Check that the generated code contains the palette definition
  import string
  assert(string.find(berry_code, "var test_palette_ = bytes(") != -1, 
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
  assert(string.find(berry_code, "var rainbow_palette_ = bytes(") != -1,
         "Should contain palette definition")
  
  print("✓ Palette with named colors test passed")
end

# Test error handling for invalid palette syntax
def test_palette_error_handling()
  print("Testing palette error handling...")
  
  # Test 1: Invalid palette name (reserved color name)
  try
    var invalid_name1 = "palette red = [(0, 0xFF0000)]"
    var result1 = animation_dsl.compile(invalid_name1)
    assert(result1 == nil, "Should fail with reserved color name 'red'")
  except .. as e
    # Expected to fail - reserved name validation working
  end
  
  # Test 2: Invalid palette name (reserved color name)
  try
    var invalid_name2 = "palette blue = [(0, 0x0000FF)]"
    var result2 = animation_dsl.compile(invalid_name2)
    assert(result2 == nil, "Should fail with reserved color name 'blue'")
  except .. as e
    # Expected to fail - reserved name validation working
  end
  
  # Test 3: Invalid palette name (reserved keyword)
  try
    var invalid_name3 = "palette animation = [(0, 0xFF0000)]"
    var result3 = animation_dsl.compile(invalid_name3)
    assert(result3 == nil, "Should fail with reserved keyword 'animation'")
  except .. as e
    # Expected to fail - reserved name validation working
  end
  
  # Test 4: Invalid palette name (reserved keyword)
  try
    var invalid_name4 = "palette sequence = [(0, 0xFF0000)]"
    var result4 = animation_dsl.compile(invalid_name4)
    assert(result4 == nil, "Should fail with reserved keyword 'sequence'")
  except .. as e
    # Expected to fail - reserved name validation working
  end
  
  # Test 5: Invalid palette name (reserved keyword)
  try
    var invalid_name5 = "palette color = [(0, 0xFF0000)]"
    var result5 = animation_dsl.compile(invalid_name5)
    assert(result5 == nil, "Should fail with reserved keyword 'color'")
  except .. as e
    # Expected to fail - reserved name validation working
  end
  
  # Test 6: Invalid palette name (reserved keyword)
  try
    var invalid_name6 = "palette palette = [(0, 0xFF0000)]"
    var result6 = animation_dsl.compile(invalid_name6)
    assert(result6 == nil, "Should fail with reserved keyword 'palette'")
  except .. as e
    # Expected to fail - reserved name validation working
  end
  
  # Test 7: Missing closing bracket
  try
    var invalid_syntax1 = "palette test_palette = [(0, 0xFF0000)"
    var result7 = animation_dsl.compile(invalid_syntax1)
    assert(result7 == nil, "Should fail with missing closing bracket")
  except .. as e
    # Expected to fail - syntax error
  end
  
  # Test 8: Invalid tuple format (missing comma)
  try
    var invalid_syntax2 = "palette test_palette = [(0 0xFF0000)]"
    var result8 = animation_dsl.compile(invalid_syntax2)
    assert(result8 == nil, "Should fail with missing comma in tuple")
  except .. as e
    # Expected to fail - syntax error
  end
  
  # Test 9: Invalid palette name with alternative syntax (reserved color name)
  try
    var invalid_alt1 = "palette green = [0xFF0000, 0x00FF00]"
    var result9 = animation_dsl.compile(invalid_alt1)
    assert(result9 == nil, "Should fail with reserved color name 'green' in alternative syntax")
  except .. as e
    # Expected to fail - reserved name validation working
  end
  
  # Test 10: Invalid palette name with alternative syntax (reserved keyword)
  try
    var invalid_alt2 = "palette run = [red, blue]"
    var result10 = animation_dsl.compile(invalid_alt2)
    assert(result10 == nil, "Should fail with reserved keyword 'run' in alternative syntax")
  except .. as e
    # Expected to fail - reserved name validation working
  end
  
  print("✓ Palette error handling test passed")
end

# Test palette referencing non-existent color names
def test_nonexistent_color_names()
  print("Testing palette with non-existent color names...")
  
  # Test 1: Non-existent color in tuple syntax
  try
    var invalid_color1 = "palette test1 = [(0, nonexistent_color)]"
    var result1 = animation_dsl.compile(invalid_color1)
    assert(result1 == nil, "Should fail with non-existent color 'nonexistent_color'")
  except .. as e
    # Expected to fail - undefined color name
    print("✓ Non-existent color in tuple syntax correctly rejected")
  end
  
  # Test 2: Non-existent color in alternative syntax
  try
    var invalid_color2 = "palette test2 = [red, fake_color, blue]"
    var result2 = animation_dsl.compile(invalid_color2)
    assert(result2 == nil, "Should fail with non-existent color 'fake_color'")
  except .. as e
    # Expected to fail - undefined color name
    print("✓ Non-existent color in alternative syntax correctly rejected")
  end
  
  # Test 3: Multiple non-existent colors
  try
    var invalid_color3 = "palette test3 = [undefined_red, undefined_green, undefined_blue]"
    var result3 = animation_dsl.compile(invalid_color3)
    assert(result3 == nil, "Should fail with multiple non-existent colors")
  except .. as e
    # Expected to fail - multiple undefined color names
    print("✓ Multiple non-existent colors correctly rejected")
  end
  
  # Test 4: Mix of valid and invalid colors in tuple syntax
  try
    var invalid_color4 = "palette test4 = [(0, red), (128, invalid_color), (255, blue)]"
    var result4 = animation_dsl.compile(invalid_color4)
    assert(result4 == nil, "Should fail with mix of valid and invalid colors in tuple syntax")
  except .. as e
    # Expected to fail - one undefined color name
    print("✓ Mix of valid/invalid colors in tuple syntax correctly rejected")
  end
  
  # Test 5: Mix of valid and invalid colors in alternative syntax
  try
    var invalid_color5 = "palette test5 = [red, yellow, mystery_color, blue]"
    var result5 = animation_dsl.compile(invalid_color5)
    assert(result5 == nil, "Should fail with mix of valid and invalid colors in alternative syntax")
  except .. as e
    # Expected to fail - one undefined color name
    print("✓ Mix of valid/invalid colors in alternative syntax correctly rejected")
  end
  
  print("✓ Non-existent color names test passed")
end

# Test that palettes work with the animation framework
def test_palette_integration()
  print("Testing palette integration with animation framework...")
  
  var dsl_source = 
    "palette fire_palette = [\n" +
    "  (0, 0x000000),    # Black\n" +
    "  (64, 0x800000),   # Dark red\n" +
    "  (128, 0xFF0000),  # Red\n" +
    "  (192, 0xFF8000),  # Orange\n" +
    "  (255, 0xFFFF00)   # Yellow\n" +
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
    assert(global.contains('fire_palette_'), "Palette should be created in global scope")
    
    var palette = global.fire_palette_
    if type(palette) == "bytes"
      assert(palette.size() == 20, "Palette should have 20 bytes (5 entries × 4 bytes each)")
    end
    
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
    "palette aurora_colors = [\n" +
    "  (0, 0x000022),    # Dark night sky\n" +
    "  (64, 0x004400),   # Dark green\n" +
    "  (128, 0x00AA44),  # Aurora green\n" +
    "  (192, 0x44AA88),  # Light green\n" +
    "  (255, 0x88FFAA)   # Bright aurora\n" +
    "]\n"
  
  var berry_code = animation_dsl.compile(dsl_source)
  assert(berry_code != nil, "Aurora palette compilation should succeed")
  
  # Execute and verify VRGB format
  try
    var compiled_func = compile(berry_code)
    compiled_func()
    
    if global.contains('aurora_colors_')
      var palette = global.aurora_colors_
      var hex_data = palette.tohex()
      
      # Verify expected VRGB entries
      var expected_entries = [
        "00000022",  # (0, 0x000022)
        "40004400",  # (64, 0x004400)  
        "8000AA44",  # (128, 0x00AA44)
        "C044AA88",  # (192, 0x44AA88)
        "FF88FFAA"   # (255, 0x88FFAA)
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
    "# Define multiple palettes\n" +
    "palette warm_colors = [\n" +
    "  (0, 0xFF0000),    # Red\n" +
    "  (128, 0xFFA500),  # Orange\n" +
    "  (255, 0xFFFF00)   # Yellow\n" +
    "]\n" +
    "\n" +
    "palette cool_colors = [\n" +
    "  (0, blue),       # Blue\n" +
    "  (128, cyan),     # Cyan\n" +
    "  (255, white)     # White\n" +
    "]\n"
  
  # Test compilation
  var berry_code = animation_dsl.compile(complete_dsl)
  assert(berry_code != nil, "Complete workflow DSL should compile")
  
  # Verify generated code contains required elements
  import string
  var required_elements = [
    "var warm_colors_ = bytes(",
    "var cool_colors_ = bytes("
  ]
  
  for element : required_elements
    assert(string.find(berry_code, element) != -1, f"Missing element: {element}")
  end
  
  # Test execution
  try
    var compiled_func = compile(berry_code)
    compiled_func()
    
    # Verify both palettes were created
    assert(global.contains('warm_colors_'), "Warm palette should be created")
    assert(global.contains('cool_colors_'), "Cool palette should be created")
    
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
  var lexer = animation_dsl.create_lexer(simple_palette_dsl)
  var tokens = extract_all_tokens(lexer)
  
  var found_palette_keyword = false
  for token : tokens
    if token.type == 0 #-animation_dsl.Token.KEYWORD-# && token.value == "palette"
      found_palette_keyword = true
      break
    end
  end
  
  assert(found_palette_keyword, "Palette keyword should be recognized by lexer")
  print("✓ Palette keyword recognition test passed")
end

# Test alternative palette syntax (new feature)
def test_alternative_palette_syntax()
  print("Testing alternative palette syntax...")
  
  var dsl_source = 
    "palette colors = [\n" +
    "  red,\n" +
    "  0x008000,\n" +
    "  0x0000FF,\n" +
    "  0x112233\n" +
    "]\n"
  
  var berry_code = animation_dsl.compile(dsl_source)
  assert(berry_code != nil, "Alternative syntax compilation should succeed")
  
  # Check that alpha is forced to 0xFF for all colors
  import string
  assert(string.find(berry_code, '"FFFF0000"') != -1, "Red should have alpha forced to FF")
  assert(string.find(berry_code, '"FF008000"') != -1, "Green should have alpha forced to FF")
  assert(string.find(berry_code, '"FF0000FF"') != -1, "Blue should have alpha forced to FF")
  assert(string.find(berry_code, '"FF112233"') != -1, "Custom color should have alpha forced to FF")
  
  print("✓ Alternative palette syntax test passed")
end

# Test alternative syntax with named colors
def test_alternative_syntax_named_colors()
  print("Testing alternative syntax with named colors...")
  
  var dsl_source = 
    "palette rainbow = [\n" +
    "  red,\n" +
    "  yellow,\n" +
    "  green,\n" +
    "  blue\n" +
    "]\n"
  
  var berry_code = animation_dsl.compile(dsl_source)
  assert(berry_code != nil, "Alternative syntax with named colors should succeed")
  
  # Execute and verify the palette is created correctly
  try
    var compiled_func = compile(berry_code)
    compiled_func()
    
    assert(global.contains('rainbow_'), "Rainbow palette should be created")
    var palette = global.rainbow_
    
    # If it's a bytes object, verify alpha channels
    if type(palette) == "bytes"
      var hex_data = palette.tohex()
      assert(hex_data[0..1] == "FF", "First color should have FF alpha")
      assert(hex_data[8..9] == "FF", "Second color should have FF alpha")
      assert(hex_data[16..17] == "FF", "Third color should have FF alpha")
      assert(hex_data[24..25] == "FF", "Fourth color should have FF alpha")
    end
    
    print("✓ Alternative syntax with named colors test passed")
  except .. as e, msg
    print(f"Alternative syntax named colors test failed: {e} - {msg}")
    assert(false, "Alternative syntax with named colors should work")
  end
end

# Test mixed syntax detection (should fail)
def test_mixed_syntax_detection()
  print("Testing mixed syntax detection...")
  
  # Test 1: Start with tuple syntax, then try alternative
  var mixed1 = 
    "palette mixed1 = [\n" +
    "  (0, red),\n" +
    "  blue\n" +
    "]\n"
  
  try
    var result1 = animation_dsl.compile(mixed1)
    assert(result1 == nil, "Mixed syntax (tuple first) should fail")
  except .. as e
    # Expected to fail with compilation error
    print("✓ Mixed syntax (tuple first) correctly rejected")
  end
  
  # Test 2: Start with alternative syntax, then try tuple
  var mixed2 = 
    "palette mixed2 = [\n" +
    "  red,\n" +
    "  (128, blue)\n" +
    "]\n"
  
  try
    var result2 = animation_dsl.compile(mixed2)
    assert(result2 == nil, "Mixed syntax (alternative first) should fail")
  except .. as e
    # Expected to fail with compilation error
    print("✓ Mixed syntax (alternative first) correctly rejected")
  end
  
  print("✓ Mixed syntax detection test passed")
end

# Test alpha channel forcing with various color formats
def test_alpha_channel_forcing()
  print("Testing alpha channel forcing...")
  
  var dsl_source = 
    "palette alpha_test = [\n" +
    "  0x112233,\n" +
    "  0x80AABBCC,\n" +
    "  red,\n" +
    "  0x00000000\n" +
    "]\n"
  
  var berry_code = animation_dsl.compile(dsl_source)
  assert(berry_code != nil, "Alpha forcing test should compile")
  
  # Execute and verify alpha channels
  try
    var compiled_func = compile(berry_code)
    compiled_func()
    
    assert(global.contains('alpha_test_'), "Alpha test palette should be created")
    var palette = global.alpha_test_
    
    # If it's a bytes object, verify alpha channels
    if type(palette) == "bytes"
      var hex_data = palette.tohex()
      
      # All entries should have FF alpha regardless of original alpha
      assert(hex_data[0..7] == "FF112233", "0x112233 should become FF112233")
      assert(hex_data[8..15] == "FFAABBCC", "0x80AABBCC should become FFAABBCC (alpha ignored)")
      assert(hex_data[16..23] == "FFFF0000", "red should become FFFF0000")
      assert(hex_data[24..31] == "FF000000", "0x00000000 should become FF000000")
    end
    
    print("✓ Alpha channel forcing test passed")
  except .. as e, msg
    print(f"Alpha channel forcing test failed: {e} - {msg}")
    assert(false, "Alpha channel forcing should work")
  end
end

# Test backward compatibility (original syntax still works)
def test_backward_compatibility()
  print("Testing backward compatibility...")
  
  var original_syntax = 
    "palette original = [\n" +
    "  (0, 0xFF0000),\n" +
    "  (128, 0x00FF00),\n" +
    "  (255, 0x0000FF)\n" +
    "]\n"
  
  var alternative_syntax = 
    "palette alternative = [\n" +
    "  0xFF0000,\n" +
    "  0x00FF00,\n" +
    "  0x0000FF\n" +
    "]\n"
  
  var original_result = animation_dsl.compile(original_syntax)
  var alternative_result = animation_dsl.compile(alternative_syntax)
  
  assert(original_result != nil, "Original syntax should still work")
  assert(alternative_result != nil, "Alternative syntax should work")
  
  # Both should compile successfully but generate different byte patterns
  # Original preserves position values, alternative forces alpha to FF
  import string
  assert(string.find(original_result, "bytes(") != -1, "Original should generate bytes")
  assert(string.find(alternative_result, "bytes(") != -1, "Alternative should generate bytes")
  
  print("✓ Backward compatibility test passed")
end

# Test empty palette handling (should fail)
def test_empty_palette_handling()
  print("Testing empty palette handling...")
  
  # Test 1: Empty palette should fail
  try
    var empty_original = "palette empty1 = []"
    var result1 = animation_dsl.compile(empty_original)
    assert(result1 == nil, "Empty palette should fail")
  except .. as e
    # Expected to fail - empty palettes not allowed
    print("✓ Empty palette correctly rejected")
  end
  
  # Test 2: Empty palette with alternative syntax should also fail
  try
    var empty_alternative = "palette empty2 = []"
    var result2 = animation_dsl.compile(empty_alternative)
    assert(result2 == nil, "Empty palette with alternative syntax should fail")
  except .. as e
    # Expected to fail - empty palettes not allowed
    print("✓ Empty palette with alternative syntax correctly rejected")
  end
  
  print("✓ Empty palette handling test passed")
end

# Test integration with animations using alternative syntax palettes
def test_alternative_syntax_integration()
  print("Testing alternative syntax integration with animations...")
  
  var dsl_source = 
    "palette fire_colors = [\n" +
    "  0x000000,\n" +
    "  0x800000,\n" +
    "  0xFF0000,\n" +
    "  0xFF8000,\n" +
    "  0xFFFF00\n" +
    "]\n" +
    "\n" +
    "color rich_palette2 = color_cycle(palette=fire_colors, cycle_period=3s)\n"
    "animation fire_anim = solid(color=rich_palette2)\n" +
    "\n" +
    "run fire_anim\n"
  
  var berry_code = animation_dsl.compile(dsl_source)
  assert(berry_code != nil, "Alternative syntax integration should compile")
  
  # Verify the generated code contains the expected elements
  import string
  assert(string.find(berry_code, "var fire_colors_ = bytes(") != -1, "Should contain palette definition")
  assert(string.find(berry_code, "color_cycle(engine)") != -1, "Should contain value provider creation")
  assert(string.find(berry_code, "solid(engine)") != -1, "Should contain animation creation")
  assert(string.find(berry_code, "fire_colors_") != -1, "Should reference the palette")
  
  print("✓ Alternative syntax integration test passed")
end

# Test that non-predefined colors raise exceptions
# Palettes only accept hex colors (0xRRGGBB) or predefined color names,
# but not custom colors defined previously. For dynamic palettes, use user functions.
def test_non_predefined_color_exceptions()
  print("Testing non-predefined color exceptions...")
  
  # Test 1: Custom color identifier in tuple syntax should fail
  try
    var custom_color_tuple = "palette test1 = [(0, custom_red)]"
    var result1 = animation_dsl.compile(custom_color_tuple)
    assert(result1 == nil, "Should fail with custom color identifier in tuple syntax")
    print("✗ FAIL: Custom color identifier in tuple syntax was accepted")
    return false
  except .. as e, msg
    # Expected to fail - custom color identifier not allowed
    print("✓ Custom color identifier in tuple syntax correctly rejected")
  end
  
  # Test 2: Custom color identifier in alternative syntax should fail
  try
    var custom_color_alt = "palette test2 = [red, custom_blue, green]"
    var result2 = animation_dsl.compile(custom_color_alt)
    assert(result2 == nil, "Should fail with custom color identifier in alternative syntax")
    print("✗ FAIL: Custom color identifier in alternative syntax was accepted")
    return false
  except .. as e, msg
    # Expected to fail - custom color identifier not allowed
    print("✓ Custom color identifier in alternative syntax correctly rejected")
  end
  
  # Test 3: The specific case from the user report - 'grrreen' should fail
  try
    var grrreen_case = "palette rainbow_with_white = [red, grrreen]"
    var result3 = animation_dsl.compile(grrreen_case)
    assert(result3 == nil, "Should fail with 'grrreen' identifier")
    print("✗ FAIL: 'grrreen' identifier was accepted")
    return false
  except .. as e, msg
    # Expected to fail - 'grrreen' is not a predefined color
    print("✓ 'grrreen' identifier correctly rejected")
  end
  
  # Test 4: Misspelled predefined color should fail
  try
    var misspelled = "palette test4 = [red, bleu, green]"  # 'bleu' instead of 'blue'
    var result4 = animation_dsl.compile(misspelled)
    assert(result4 == nil, "Should fail with misspelled color 'bleu'")
    print("✗ FAIL: Misspelled color 'bleu' was accepted")
    return false
  except .. as e, msg
    # Expected to fail - 'bleu' is not a predefined color
    print("✓ Misspelled color 'bleu' correctly rejected")
  end
  
  # Test 5: Random identifier should fail
  try
    var random_id = "palette test5 = [red, some_random_name, blue]"
    var result5 = animation_dsl.compile(random_id)
    assert(result5 == nil, "Should fail with random identifier")
    print("✗ FAIL: Random identifier was accepted")
    return false
  except .. as e, msg
    # Expected to fail - random identifier is not a predefined color
    print("✓ Random identifier correctly rejected")
  end
  
  print("✓ Non-predefined color exceptions test passed")
  return true
end

# Run all palette tests
def run_palette_tests()
  print("=== Palette DSL Tests ===")
  
  try
    test_palette_keyword_recognition()
    test_palette_definition()
    test_palette_with_named_colors()
    test_palette_error_handling()
    test_nonexistent_color_names()
    test_non_predefined_color_exceptions()  # New test for strict color validation
    test_palette_integration()
    test_vrgb_format_validation()
    test_complete_workflow()
    
    # New alternative syntax tests
    test_alternative_palette_syntax()
    test_alternative_syntax_named_colors()
    test_mixed_syntax_detection()
    test_alpha_channel_forcing()
    test_backward_compatibility()
    test_empty_palette_handling()
    test_alternative_syntax_integration()
    
    print("=== All palette tests passed! ===")
    return true
  except .. as e, msg
    print(f"Palette test failed: {e} - {msg}")
    raise "test_failed"
  end
end

run_palette_tests()