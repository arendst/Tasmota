# Pull Lexer Test Suite
# Comprehensive tests for the pull-mode lexer interface

import string
import tasmota
def log(m,l) tasmota.log(m,l) end
import animation
import animation_dsl

# Import the pull lexer module
import "dsl/lexer.be" as pull_lexer_module
var Lexer = pull_lexer_module["create_lexer"]
var create_lexer = pull_lexer_module["create_lexer"]

# Local helper functions to replace removed Token methods
# These replace the removed is_identifier(), is_type(), and is_keyword() methods from Token class
def is_identifier(token, name)
  return token.type == 1 #-animation_dsl.Token.IDENTIFIER-# && token.value == name
end

def is_type(token, token_type)
  return token.type == token_type
end

def is_keyword(token, keyword)
  return token.type == 0 #-animation_dsl.Token.KEYWORD-# && token.value == keyword
end

# Local helper function to count tokens (since token_count() was removed from Lexer)
# This creates a separate lexer instance to avoid state conflicts
def count_tokens(lexer)
  # Create a separate lexer instance to count tokens
  var counting_lexer = Lexer(lexer.source)
  
  var count = 0
  while !counting_lexer.at_end()
    var token = counting_lexer.next_token()
    if token == nil break end
    count += 1
  end
  
  return count
end

# Test basic pull lexer creation and initialization
def test_pull_lexer_creation()
  print("Testing Lexer creation and initialization...")
  
  var simple_dsl = "color red_custom = 0xFF0000"
  var lexer = Lexer(simple_dsl)
  
  # Test basic properties
  assert(lexer != nil, "Lexer should be created")
  assert(lexer.position == 0, "Initial position should be 0")
  assert(!lexer.at_end(), "Should not be at end initially")
  assert(count_tokens(lexer) > 0, "Should have tokens")
  
  # Test factory function
  var factory_lexer = create_lexer(simple_dsl)
  assert(factory_lexer != nil, "Factory function should create lexer")
  
  print("✓ Lexer creation test passed")
  return true
end

# Test basic token pulling functionality
def test_basic_token_pulling()
  print("Testing basic token pulling...")
  
  var dsl_code = "color red_custom = 0xFF0000"
  var lexer = Lexer(dsl_code)
  
  # Debug: Print all tokens to understand the structure
  print("  Debug: Tokens in stream:")
  var debug_lexer = Lexer(dsl_code)
  var token_index = 0
  
  # Pull tokens one by one, skipping whitespace/newlines
  var tokens = []
  while !lexer.at_end()
    var token = lexer.next_token()
    if token == nil break end
    print(f"    [{token_index}] {token.tostring()}")
    # Skip whitespace and newline tokens
    if token.type != 35 #-animation_dsl.Token.NEWLINE-#
      tokens.push(token)
      token_index += 1
    end
  end
  
  assert(size(tokens) >= 4, f"Should have at least 4 non-whitespace tokens, got {size(tokens)}")
  
  # Check the meaningful tokens
  assert(is_keyword(tokens[0], "color"), f"First token should be 'color' keyword, got {tokens[0].tostring()}")
  assert(is_identifier(tokens[1], "red_custom"), f"Second token should be 'red_custom' identifier, got {tokens[1].tostring()}")
  assert(is_type(tokens[2], 8 #-animation_dsl.Token.ASSIGN-#), f"Third token should be assignment, got {tokens[2].tostring()}")
  assert(is_type(tokens[3], 4 #-animation_dsl.Token.COLOR-#), f"Fourth token should be color literal, got {tokens[3].tostring()}")
  assert(tokens[3].value == "0xFF0000", f"Color value should match, got '{tokens[3].value}'")
  
  print("✓ Basic token pulling test passed")
  return true
end

# Test peek functionality
def test_peek_functionality()
  print("Testing peek functionality...")
  
  var dsl_code = "animation pulse = pulsating_animation(color=red_custom, period=2s)"
  var lexer = Lexer(dsl_code)
  
  # Test peek without consuming
  var peek1 = lexer.peek_token()
  var peek2 = lexer.peek_token()
  assert(peek1 != nil && peek2 != nil, "Peek should return tokens")
  assert(peek1.value == peek2.value, "Multiple peeks should return same token")
  assert(lexer.position == 0, "Peek should not advance position")
  
  # Test peek ahead
  var peek_ahead_1 = lexer.peek_ahead(1)
  var peek_ahead_2 = lexer.peek_ahead(2)
  var peek_ahead_3 = lexer.peek_ahead(3)
  
  assert(is_keyword(peek_ahead_1, "animation"), "Peek ahead 1 should be 'animation'")
  assert(is_identifier(peek_ahead_2, "pulse"), "Peek ahead 2 should be 'pulse'")
  assert(is_type(peek_ahead_3, 8 #-animation_dsl.Token.ASSIGN-#), "Peek ahead 3 should be assignment")
  
  # Consume one token and test peek again
  var consumed = lexer.next_token()
  assert(is_keyword(consumed, "animation"), "Consumed token should match peek")
  
  var new_peek = lexer.peek_token()
  assert(is_identifier(new_peek, "pulse"), "New peek should be next token")
  
  # Test peek beyond end
  var initial_pos = lexer.position
  while !lexer.at_end()
    lexer.next_token()
  end
  
  var end_peek = lexer.peek_token()
  assert(end_peek == nil, "Peek at end should return nil")
  
  var beyond_peek = lexer.peek_ahead(5)
  assert(beyond_peek == nil, "Peek beyond end should return nil")
  
  print("✓ Peek functionality test passed")
  return true
end

# Test position management
def test_position_management()
  print("Testing position management...")
  
  var dsl_code = "color red_custom = 0xFF0000\ncolor blue_custom = 0x0000FF"
  var lexer = Lexer(dsl_code)
  
  # Test initial position
  assert(lexer.get_position() == 0, "Initial position should be 0")
  
  # Advance and check position
  lexer.next_token()  # color
  assert(lexer.get_position() == 1, "Position should advance")
  
  lexer.next_token()  # red_custom
  lexer.next_token()  # =
  assert(lexer.get_position() == 3, "Position should continue advancing")
  
  # Test set_position
  lexer.set_position(1)
  assert(lexer.get_position() == 1, "Position should be set to 1")
  
  var token = lexer.peek_token()
  assert(is_identifier(token, "red_custom"), "Should be back at 'red_custom' token")
  
  # Test reset
  lexer.reset()
  assert(lexer.get_position() == 0, "Reset should return to position 0")
  
  var first_token = lexer.peek_token()
  assert(is_keyword(first_token, "color"), "Should be back at first token")
  
  # Test invalid position setting
  var original_pos = lexer.get_position()
  lexer.set_position(-1)
  assert(lexer.get_position() == original_pos, "Invalid negative position should be ignored")
  
  lexer.set_position(1000)
  assert(lexer.get_position() == original_pos, "Invalid large position should be ignored")
  
  print("✓ Position management test passed")
  return true
end

# Test position information for error reporting
def test_position_information()
  print("Testing position information...")
  
  var dsl_code = "color red_custom = 0xFF0000\n" +
                 "animation pulse = pulsating_animation(color=red_custom)"
  var lexer = Lexer(dsl_code)
  
  
  # Advance to second line and test
  var found_line_2 = false
  while !lexer.at_end()
    var token = lexer.next_token()
    if token != nil && token.line == 2
      found_line_2 = true
      break
    end
  end
  
  # Test at end
  while !lexer.at_end()
    lexer.next_token()
  end

  
  print("✓ Position information test passed")
  return true
end

# Test sub-lexer creation
def test_sub_lexer_creation()
  print("Testing sub-lexer creation...")
  
  var dsl_code = "color red_custom = 0xFF0000\ncolor blue_custom = 0x0000FF\nanimation test = solid(color=red_custom)"
  var lexer = Lexer(dsl_code)
  
  # Get total token count
  var total_tokens = count_tokens(lexer)
  assert(total_tokens > 6, "Should have multiple tokens")
  
  # Create sub-lexer for middle portion
  var sub_lexer = lexer.create_sub_lexer(2, 6)
  assert(sub_lexer != nil, "Sub-lexer should be created")
  assert(count_tokens(sub_lexer) == 4, "Sub-lexer should have 4 tokens (6-2)")
  assert(sub_lexer.get_position() == 0, "Sub-lexer should start at position 0")
  
  # Test sub-lexer tokens
  var sub_token1 = sub_lexer.next_token()
  assert(sub_token1 != nil, "Sub-lexer should have tokens")
  assert(is_type(sub_token1, 8 #-animation_dsl.Token.ASSIGN-#), "First sub-token should be assignment")
  
  # Test invalid sub-lexer ranges
  var invalid_sub1 = lexer.create_sub_lexer(-1, 5)
  assert(count_tokens(invalid_sub1) == 0, "Invalid start should create empty sub-lexer")
  
  var invalid_sub2 = lexer.create_sub_lexer(5, 2)
  assert(count_tokens(invalid_sub2) == 0, "Invalid range should create empty sub-lexer")
  
  var invalid_sub3 = lexer.create_sub_lexer(0, total_tokens + 10)
  assert(count_tokens(invalid_sub3) == total_tokens, "Should clamp to valid range")
  
  print("✓ Sub-lexer creation test passed")
  return true
end

# Test complex DSL parsing scenarios
def test_complex_dsl_scenarios()
  print("Testing complex DSL scenarios...")
  
  # Test sequence with nested structures
  var complex_dsl = "color red_custom = 0xFF0000\n" +
                    "color blue_custom = 0x0000FF\n" +
                    "\n" +
                    "animation pulse_red = pulsating_animation(\n" +
                    "  color=red_custom,\n" +
                    "  period=2s,\n" +
                    "  min_brightness=50%,\n" +
                    "  max_brightness=100%\n" +
                    ")\n" +
                    "\n" +
                    "sequence demo {\n" +
                    "  play pulse_red for 3s\n" +
                    "  wait 1s\n" +
                    "  repeat 3 times {\n" +
                    "    play pulse_red for 500ms\n" +
                    "    wait 200ms\n" +
                    "  }\n" +
                    "}\n" +
                    "\n" +
                    "run demo"
  
  var lexer = Lexer(complex_dsl)
  
  # Count different token types
  var keyword_count = 0
  var identifier_count = 0
  var color_count = 0
  var time_count = 0
  var percentage_count = 0
  var brace_count = 0
  
  var token_index = 0
  while !lexer.at_end()
    var token = lexer.next_token()
    if token == nil break end
    print(f"    [{token_index}] {token.tostring()}")
    token_index += 1
    
    if token.type == 0 #-animation_dsl.Token.KEYWORD-#
      keyword_count += 1
    elif token.type == 1 #-animation_dsl.Token.IDENTIFIER-#
      identifier_count += 1
    elif token.type == 4 #-animation_dsl.Token.COLOR-#
      color_count += 1
    elif token.type == 5 #-animation_dsl.Token.TIME-#
      time_count += 1
    elif token.type == 6 #-animation_dsl.Token.PERCENTAGE-#
      percentage_count += 1
    elif token.type == 26 #-animation_dsl.Token.LEFT_BRACE-# || token.type == 27 #-animation_dsl.Token.RIGHT_BRACE-#
      brace_count += 1
    end
  end
  
  assert(keyword_count > 5, "Should have multiple keywords")
  assert(identifier_count > 5, "Should have multiple identifiers")
  assert(color_count == 2, "Should have 2 color literals")
  assert(time_count > 3, "Should have multiple time values")
  assert(percentage_count == 2, "Should have 2 percentage values")
  assert(brace_count > 2, "Should have multiple braces")
  
  print("✓ Complex DSL scenarios test passed")
  return true
end

# Test error handling and edge cases
def test_error_handling()
  print("Testing error handling and edge cases...")
  
  # Test empty source
  var empty_lexer = Lexer("")
  assert(empty_lexer.at_end(), "Empty lexer should be at end")
  assert(count_tokens(empty_lexer) == 0, "Empty lexer should have no tokens")
  assert(empty_lexer.next_token() == nil, "Empty lexer should return nil")
  assert(empty_lexer.peek_token() == nil, "Empty lexer peek should return nil")
  
  # Test whitespace-only source
  var whitespace_lexer = Lexer("   \n\t  \n  ")
  # Whitespace tokens might be present depending on lexer implementation
  
  # Test single token
  var single_lexer = Lexer("red_custom")
  assert(!single_lexer.at_end(), "Single token lexer should not be at end initially")
  var single_token = single_lexer.next_token()
  assert(single_token != nil, "Should get the single token")
  assert(is_identifier(single_token, "red_custom"), "Should be 'red_custom' identifier")
  assert(single_lexer.at_end(), "Should be at end after consuming single token")
  
  # Test malformed DSL (lexer should still tokenize what it can)
  var malformed_dsl = "color red_custom = #invalid_color @#$%"
  var malformed_lexer = Lexer(malformed_dsl)
  
  var tokens_found = 0
  while !malformed_lexer.at_end()
    var token = malformed_lexer.next_token()
    if token == nil break end
    tokens_found += 1
    # Should not crash, even with malformed input
  end
  
  assert(tokens_found > 0, "Should find some tokens even in malformed DSL")
  
  print("✓ Error handling test passed")
  return true
end

# Test integration with existing DSL components
def test_dsl_integration()
  print("Testing DSL integration...")
  
  var dsl_code = "color red_custom = 0xFF0000\n" +
                 "animation pulse = pulsating_animation(color=red_custom, period=2s)\n" +
                 "run pulse"
  
  var lexer = Lexer(dsl_code)
  
  # Simulate transpiler-like usage
  var tokens_processed = []
  
  while !lexer.at_end()
    var token = lexer.next_token()
    if token == nil break end
    
    tokens_processed.push(token)
    
    # Test token properties that transpiler would use
    # Note: We need to be more careful about context - "color" can be both a statement keyword
    # and a parameter name in function calls
    if is_keyword(token, "color")
      # Skip whitespace/newlines to find the next meaningful token
      var name_token = lexer.peek_token()
      while name_token != nil && name_token.type == 35 #-animation_dsl.Token.NEWLINE-#
        lexer.next_token()  # consume the newline
        name_token = lexer.peek_token()
      end
      
      # Only check for identifier if this looks like a color definition (not a parameter)
      if name_token != nil && name_token.type != 8 #-animation_dsl.Token.ASSIGN-#
        assert(name_token.type == 1 #-animation_dsl.Token.IDENTIFIER-#, 
               "Color definition should be followed by identifier")
      end
    elif is_keyword(token, "animation")
      # Skip whitespace/newlines to find the next meaningful token
      var name_token = lexer.peek_token()
      while name_token != nil && name_token.type == 35 #-animation_dsl.Token.NEWLINE-#
        lexer.next_token()  # consume the newline
        name_token = lexer.peek_token()
      end
      
      if name_token != nil
        assert(name_token.type == 1 #-animation_dsl.Token.IDENTIFIER-#,
               "Animation keyword should be followed by identifier")
      end
    elif is_keyword(token, "run")
      # Skip whitespace/newlines to find the next meaningful token
      var name_token = lexer.peek_token()
      while name_token != nil && name_token.type == 35 #-animation_dsl.Token.NEWLINE-#
        lexer.next_token()  # consume the newline
        name_token = lexer.peek_token()
      end
      
      if name_token != nil
        assert(name_token.type == 1 #-animation_dsl.Token.IDENTIFIER-#,
               "Run keyword should be followed by identifier")
      end
    end
  end
  
  assert(size(tokens_processed) > 10, "Should have processed multiple tokens")
  
  # Test that we can create multiple lexers for the same source
  var lexer2 = Lexer(dsl_code)
  var first_token1 = lexer2.next_token()
  
  var lexer3 = Lexer(dsl_code)
  var first_token2 = lexer3.next_token()
  
  assert(first_token1.value == first_token2.value, "Multiple lexers should produce same tokens")
  
  print("✓ DSL integration test passed")
  return true
end

# Test interleaved lexer operations
def test_interleaved_lexer_operations()
  print("Testing interleaved lexer operations...")
  
  var dsl_code = "color red_custom = 0xFF0000\n" +
                 "color blue_custom = 0x0000FF\n" +
                 "animation pulse = pulsating_animation(\n" +
                 "  color=red_custom,\n" +
                 "  period=2s\n" +
                 ")\n" +
                 "run pulse"
  
  var lexer = Lexer(dsl_code)
  
  # Test 1: Interleaved peek and next operations
  print("  Testing interleaved peek and next operations...")
  
  # Start at position 0
  assert(lexer.get_position() == 0, "Should start at position 0")
  
  # Peek at first token multiple times
  var peek1 = lexer.peek_token()
  var peek2 = lexer.peek_token()
  assert(peek1.value == peek2.value, "Multiple peeks should return same token")
  assert(is_keyword(peek1, "color"), "First token should be 'color'")
  assert(lexer.get_position() == 0, "Peek should not advance position")
  
  # Consume first token and verify it matches peek
  var consumed1 = lexer.next_token()
  assert(consumed1.value == peek1.value, "Consumed token should match peek")
  assert(lexer.get_position() == 1, "Position should advance after next_token")
  
  # Test 2: Peek ahead operations
  print("  Testing peek ahead operations...")
  
  var peek_ahead_1 = lexer.peek_ahead(1)  # Should be identifier "red_custom"
  var peek_ahead_2 = lexer.peek_ahead(2)  # Should be assignment "="
  var peek_ahead_3 = lexer.peek_ahead(3)  # Should be color "0xFF0000"
  
  assert(is_identifier(peek_ahead_1, "red_custom"), "Peek ahead 1 should be 'red_custom'")
  assert(is_type(peek_ahead_2, 8 #-animation_dsl.Token.ASSIGN-#), "Peek ahead 2 should be assignment")
  assert(is_type(peek_ahead_3, 4 #-animation_dsl.Token.COLOR-#), "Peek ahead 3 should be color")
  assert(peek_ahead_3.value == "0xFF0000", "Color value should match")
  
  # Position should still be 1
  assert(lexer.get_position() == 1, "Peek ahead should not change position")
  
  # Test 3: Position manipulation with set_position
  print("  Testing position manipulation...")
  
  # Save current position
  var saved_pos = lexer.get_position()
  
  # Jump to position 3 (should be the color token)
  lexer.set_position(3)
  assert(lexer.get_position() == 3, "Position should be set to 3")
  
  var token_at_3 = lexer.peek_token()
  assert(is_type(token_at_3, 4 #-animation_dsl.Token.COLOR-#), "Token at position 3 should be color")
  assert(token_at_3.value == "0xFF0000", "Color value should match")
  
  # Jump back to saved position
  lexer.set_position(saved_pos)
  assert(lexer.get_position() == saved_pos, "Should return to saved position")
  
  var token_at_saved = lexer.peek_token()
  assert(is_identifier(token_at_saved, "red_custom"), "Should be back at 'red_custom'")
  
  # Test 4: Complex interleaved operations
  print("  Testing complex interleaved operations...")
  
  # Reset to beginning
  lexer.reset()
  assert(lexer.get_position() == 0, "Reset should return to position 0")
  
  # Simulate a complex parsing scenario with backtracking
  var checkpoint_positions = []
  var checkpoint_tokens = []
  
  # Consume first few tokens while saving checkpoints
  for i : 0..4
    checkpoint_positions.push(lexer.get_position())
    var token = lexer.next_token()
    if token != nil
      checkpoint_tokens.push(token)
    end
  end
  
  # Current position should be 5
  assert(lexer.get_position() == 5, "Should be at position 5 after consuming 5 tokens")
  
  # Backtrack to checkpoint 2
  lexer.set_position(checkpoint_positions[2])
  var backtrack_token = lexer.peek_token()
  assert(backtrack_token.value == checkpoint_tokens[2].value, "Backtracked token should match checkpoint")
  
  # Test peek ahead from backtracked position
  var peek_from_backtrack = lexer.peek_ahead(2)
  assert(peek_from_backtrack.value == checkpoint_tokens[3].value, "Peek ahead from backtrack should match")
  
  # Test 5: Boundary conditions with position manipulation
  print("  Testing boundary conditions...")
  
  # Try to set position beyond end
  var total_tokens = count_tokens(lexer)
  var original_pos = lexer.get_position()
  
  lexer.set_position(total_tokens + 10)  # Beyond end
  assert(lexer.get_position() == original_pos, "Invalid position should be ignored")
  
  # Try to set negative position
  lexer.set_position(-5)
  assert(lexer.get_position() == original_pos, "Negative position should be ignored")
  
  # Set to last valid position
  lexer.set_position(total_tokens - 1)
  assert(lexer.get_position() == total_tokens - 1, "Should accept last valid position")
  
  var last_token = lexer.peek_token()
  assert(last_token != nil, "Should have token at last position")
  
  # Try to peek beyond end
  var beyond_peek = lexer.peek_ahead(5)
  assert(beyond_peek == nil, "Peek beyond end should return nil")
  
  # Test 6: Interleaved operations with newlines and whitespace
  print("  Testing operations with newlines and whitespace...")
  
  lexer.reset()
  var meaningful_tokens = []
  var all_tokens = []
  
  # Collect all tokens and filter meaningful ones
  while !lexer.at_end()
    var pos_before = lexer.get_position()
    var token = lexer.next_token()
    if token == nil break end
    
    all_tokens.push(token)
    
    # Skip newlines for meaningful token collection
    if token.type != 35 #-animation_dsl.Token.NEWLINE-#
      meaningful_tokens.push({
        "token": token,
        "position": pos_before
      })
    end
  end
  
  # Test jumping between meaningful tokens
  for i : 0..(size(meaningful_tokens) - 1)
    var entry = meaningful_tokens[i]
    lexer.set_position(entry["position"])
    
    var retrieved_token = lexer.peek_token()
    assert(retrieved_token.value == entry["token"].value, 
           f"Token at position {entry['position']} should match stored token")
    
    # Test peek ahead to next meaningful token if it exists
    if i < size(meaningful_tokens) - 1
      var next_entry = meaningful_tokens[i + 1]
      var distance = next_entry["position"] - entry["position"]
      
      if distance <= 5  # Only test reasonable distances
        var peek_next = lexer.peek_ahead(distance + 1)
        if peek_next != nil
          assert(peek_next.value == next_entry["token"].value,
                 "Peek ahead should reach next meaningful token")
        end
      end
    end
  end
  
  print("✓ Interleaved lexer operations test passed")
  return true
end

# Test full template parsing with complex DSL
def test_full_template_parsing()
  print("Testing full template parsing...")
  
  # Full template example similar to cylon_generic.anim
  var template_dsl = "# Cylon Red Eye Template\n" +
                     "# Automatically adapts to the length of the strip\n" +
                     "\n" +
                     "template cylon_effect {\n" +
                     "  param eye_color type color\n" +
                     "  param back_color type color\n" +
                     "  param duration\n" +
                     "  \n" +
                     "  set strip_len = strip_length()\n" +
                     "\n" +
                     "  animation eye_animation = beacon_animation(\n" +
                     "    color = eye_color\n" +
                     "    back_color = back_color\n" +
                     "    pos = cosine_osc(min_value = -1, max_value = strip_len - 2, duration = duration)\n" +
                     "    beacon_size = 3\n" +
                     "    slew_size = 2\n" +
                     "    priority = 5\n" +
                     "  )\n" +
                     "\n" +
                     "  run eye_animation\n" +
                     "}\n" +
                     "\n" +
                     "cylon_effect(red, transparent, 3s)\n"
  
  var lexer = Lexer(template_dsl)
  
  print("  Testing template structure parsing...")
  
  # Test 1: Navigate to template definition with interleaved operations
  var template_found = false
  var template_start_pos = -1
  
  while !lexer.at_end()
    var current_pos = lexer.get_position()
    var token = lexer.peek_token()
    
    if token != nil && is_keyword(token, "template")
      template_found = true
      template_start_pos = current_pos
      break
    end
    
    lexer.next_token()  # Consume token
  end
  
  assert(template_found, "Should find template keyword")
  assert(template_start_pos >= 0, "Should have valid template start position")
  
  # Test 2: Parse template header with position manipulation
  print("  Testing template header parsing...")
  
  lexer.set_position(template_start_pos)
  
  # Verify template keyword
  var template_token = lexer.next_token()
  assert(is_keyword(template_token, "template"), "Should be template keyword")
  
  # Get template name
  var name_token = lexer.next_token()
  assert(is_identifier(name_token, "cylon_effect"), "Template name should be 'cylon_effect'")
  
  # Expect opening brace
  var brace_token = lexer.next_token()
  assert(is_type(brace_token, 26 #-animation_dsl.Token.LEFT_BRACE-#), "Should have opening brace")
  
  # Test 3: Parse template parameters with peek ahead
  print("  Testing template parameter parsing...")
  
  var param_count = 0
  var expected_params = ["eye_color", "back_color", "duration"]
  var found_params = []
  
  # Look for param keywords and collect parameter names
  while !lexer.at_end()
    var token = lexer.peek_token()
    if token == nil break end
    
    if is_keyword(token, "param")
      lexer.next_token()  # consume 'param'
      
      var param_name_token = lexer.peek_token()
      if param_name_token != nil && param_name_token.type == 1 #-animation_dsl.Token.IDENTIFIER-#
        found_params.push(param_name_token.value)
        param_count += 1
        
        # Skip to next line or statement
        while !lexer.at_end()
          var skip_token = lexer.next_token()
          if skip_token == nil || skip_token.type == 35 #-animation_dsl.Token.NEWLINE-#
            break
          end
        end
      else
        lexer.next_token()  # consume whatever token this is
      end
    elif is_keyword(token, "set") || is_keyword(token, "animation") || is_keyword(token, "run")
      # We've reached the template body
      break
    else
      lexer.next_token()  # consume token
    end
  end
  
  assert(param_count == 3, f"Should find 3 parameters, found {param_count}")
  
  for expected_param : expected_params
    var found = false
    for found_param : found_params
      if found_param == expected_param
        found = true
        break
      end
    end
    assert(found, f"Should find parameter '{expected_param}'")
  end
  
  # Test 4: Parse template body with complex expressions
  print("  Testing template body parsing...")
  
  # Look for the set statement
  var set_found = false
  var set_position = -1
  
  while !lexer.at_end()
    var current_pos = lexer.get_position()
    var token = lexer.peek_token()
    
    if token != nil && is_keyword(token, "set")
      set_found = true
      set_position = current_pos
      break
    end
    
    lexer.next_token()
  end
  
  assert(set_found, "Should find 'set' statement in template body")
  
  # Parse the set statement: set strip_len = strip_length()
  lexer.set_position(set_position)
  
  var set_token = lexer.next_token()
  assert(is_keyword(set_token, "set"), "Should be 'set' keyword")
  
  var var_name_token = lexer.next_token()
  assert(is_identifier(var_name_token, "strip_len"), "Variable name should be 'strip_len'")
  
  var assign_token = lexer.next_token()
  assert(is_type(assign_token, 8 #-animation_dsl.Token.ASSIGN-#), "Should have assignment operator")
  
  var func_name_token = lexer.next_token()
  assert(is_identifier(func_name_token, "strip_length"), "Function name should be 'strip_length'")
  
  # Test 5: Parse complex animation definition with interleaved operations
  print("  Testing complex animation definition...")
  
  # Look for animation keyword
  var animation_found = false
  var animation_position = -1
  
  while !lexer.at_end()
    var current_pos = lexer.get_position()
    var token = lexer.peek_token()
    
    if token != nil && is_keyword(token, "animation")
      animation_found = true
      animation_position = current_pos
      break
    end
    
    lexer.next_token()
  end
  
  assert(animation_found, "Should find 'animation' keyword in template body")
  
  # Parse animation definition with position manipulation
  lexer.set_position(animation_position)
  
  var anim_keyword = lexer.next_token()
  assert(is_keyword(anim_keyword, "animation"), "Should be 'animation' keyword")
  
  var anim_name = lexer.next_token()
  assert(is_identifier(anim_name, "eye_animation"), "Animation name should be 'eye_animation'")
  
  var anim_assign = lexer.next_token()
  assert(is_type(anim_assign, 8 #-animation_dsl.Token.ASSIGN-#), "Should have assignment")
  
  var anim_func = lexer.next_token()
  assert(is_identifier(anim_func, "beacon_animation"), "Should be 'beacon_animation' function")
  
  # Test 6: Parse function parameters with peek ahead
  print("  Testing function parameter parsing...")
  
  var left_paren = lexer.next_token()
  assert(is_type(left_paren, 24 #-animation_dsl.Token.LEFT_PAREN-#), "Should have opening parenthesis")
  
  # Count parameters by looking for assignment operators within the function call
  var param_assignments = 0
  var paren_depth = 1
  
  while !lexer.at_end() && paren_depth > 0
    var token = lexer.next_token()
    if token == nil break end
    
    if is_type(token, 24 #-animation_dsl.Token.LEFT_PAREN-#)
      paren_depth += 1
    elif is_type(token, 25 #-animation_dsl.Token.RIGHT_PAREN-#)
      paren_depth -= 1
    elif is_type(token, 8 #-animation_dsl.Token.ASSIGN-#) && paren_depth == 1
      param_assignments += 1
    end
  end
  
  assert(param_assignments >= 5, f"Should find at least 5 parameter assignments, found {param_assignments}")
  
  # Test 7: Parse template call with position manipulation
  print("  Testing template call parsing...")
  
  # Look for template call: cylon_effect(red, transparent, 3s)
  lexer.reset()
  var call_found = false
  var call_position = -1
  
  while !lexer.at_end()
    var current_pos = lexer.get_position()
    var token = lexer.peek_token()
    
    if token != nil && is_identifier(token, "cylon_effect")
      # Check if next token is opening parenthesis (indicating a call)
      var next_token = lexer.peek_ahead(2)
      if next_token != nil && is_type(next_token, 24 #-animation_dsl.Token.LEFT_PAREN-#)
        call_found = true
        call_position = current_pos
        break
      end
    end
    
    lexer.next_token()
  end
  
  assert(call_found, "Should find template call")
  
  # Parse template call arguments
  lexer.set_position(call_position)
  
  var call_name = lexer.next_token()
  assert(is_identifier(call_name, "cylon_effect"), "Call name should be 'cylon_effect'")
  
  var call_paren = lexer.next_token()
  assert(is_type(call_paren, 24 #-animation_dsl.Token.LEFT_PAREN-#), "Should have opening parenthesis")
  
  # Parse arguments: red, transparent, 3s (skip whitespace/newlines)
  var arg1 = lexer.next_token()
  while arg1 != nil && arg1.type == 35 #-animation_dsl.Token.NEWLINE-#
    arg1 = lexer.next_token()
  end
  var arg1_desc = arg1 != nil ? arg1.tostring() : "nil"
  # "red" is a predefined color, so it's tokenized as COLOR, not IDENTIFIER
  assert(arg1 != nil && (is_identifier(arg1, "red") || (is_type(arg1, 4 #-animation_dsl.Token.COLOR-#) && arg1.value == "red")), f"First argument should be 'red', got {arg1_desc}")
  
  var comma1 = lexer.next_token()
  while comma1 != nil && comma1.type == 35 #-animation_dsl.Token.NEWLINE-#
    comma1 = lexer.next_token()
  end
  var comma1_desc = comma1 != nil ? comma1.tostring() : "nil"
  assert(comma1 != nil && is_type(comma1, 30 #-animation_dsl.Token.COMMA-#), f"Should have comma, got {comma1_desc}")
  
  var arg2 = lexer.next_token()
  while arg2 != nil && arg2.type == 35 #-animation_dsl.Token.NEWLINE-#
    arg2 = lexer.next_token()
  end
  var arg2_desc = arg2 != nil ? arg2.tostring() : "nil"
  # "transparent" is also a predefined color
  assert(arg2 != nil && (is_identifier(arg2, "transparent") || (is_type(arg2, 4 #-animation_dsl.Token.COLOR-#) && arg2.value == "transparent")), f"Second argument should be 'transparent', got {arg2_desc}")
  
  var comma2 = lexer.next_token()
  while comma2 != nil && comma2.type == 35 #-animation_dsl.Token.NEWLINE-#
    comma2 = lexer.next_token()
  end
  var comma2_desc = comma2 != nil ? comma2.tostring() : "nil"
  assert(comma2 != nil && is_type(comma2, 30 #-animation_dsl.Token.COMMA-#), f"Should have comma, got {comma2_desc}")
  
  var arg3 = lexer.next_token()
  while arg3 != nil && arg3.type == 35 #-animation_dsl.Token.NEWLINE-#
    arg3 = lexer.next_token()
  end
  var arg3_desc = arg3 != nil ? arg3.tostring() : "nil"
  assert(arg3 != nil && is_type(arg3, 5 #-animation_dsl.Token.TIME-#), f"Third argument should be time value, got {arg3_desc}")
  assert(arg3.value == "3s", f"Time value should be '3s', got '{arg3.value}'")
  
  var close_paren = lexer.next_token()
  while close_paren != nil && close_paren.type == 35 #-animation_dsl.Token.NEWLINE-#
    close_paren = lexer.next_token()
  end
  var close_paren_desc = close_paren != nil ? close_paren.tostring() : "nil"
  assert(close_paren != nil && is_type(close_paren, 25 #-animation_dsl.Token.RIGHT_PAREN-#), f"Should have closing parenthesis, got {close_paren_desc}")
  
  # Test 8: Verify token count and structure
  print("  Testing overall token structure...")
  
  lexer.reset()
  var total_tokens = count_tokens(lexer)
  assert(total_tokens > 50, f"Should have substantial number of tokens, got {total_tokens}")
  
  # Count different token types
  var keyword_count = 0
  var identifier_count = 0
  var brace_count = 0
  var paren_count = 0
  var comment_count = 0
  
  while !lexer.at_end()
    var token = lexer.next_token()
    if token == nil break end
    
    if token.type == 0 #-animation_dsl.Token.KEYWORD-#
      keyword_count += 1
    elif token.type == 1 #-animation_dsl.Token.IDENTIFIER-#
      identifier_count += 1
    elif token.type == 26 #-animation_dsl.Token.LEFT_BRACE-# || token.type == 27 #-animation_dsl.Token.RIGHT_BRACE-#
      brace_count += 1
    elif token.type == 24 #-animation_dsl.Token.LEFT_PAREN-# || token.type == 25 #-animation_dsl.Token.RIGHT_PAREN-#
      paren_count += 1
    elif token.type == 37 #-animation_dsl.Token.COMMENT-#
      comment_count += 1
    end
  end
  
  assert(keyword_count >= 8, f"Should have multiple keywords, got {keyword_count}")
  assert(identifier_count >= 15, f"Should have many identifiers, got {identifier_count}")
  assert(brace_count >= 2, f"Should have braces for template, got {brace_count}")
  assert(paren_count >= 4, f"Should have parentheses for function calls, got {paren_count}")
  
  print("✓ Full template parsing test passed")
  return true
end

# Run all tests
def run_pull_lexer_tests()
  print("=== Pull Lexer Test Suite ===")
  
  var tests = [
    test_pull_lexer_creation,
    test_basic_token_pulling,
    test_peek_functionality,
    test_position_management,
    test_position_information,
    test_sub_lexer_creation,
    test_complex_dsl_scenarios,
    test_error_handling,
    test_dsl_integration,
    test_interleaved_lexer_operations,
    test_full_template_parsing,
  ]
  
  var passed = 0
  var total = size(tests)
  
  for test_func : tests
    try
      if test_func()
        passed += 1
      else
        print("✗ Test failed")
      end
    except .. as error_type, error_message
      print(f"✗ Test crashed: {error_type} - {error_message}")
      import string
      import debug
      var stack_trace = string.format("Stack trace: %s", debug.traceback())
      print(stack_trace)
    end
    print()  # Add spacing between tests
  end
  
  print(f"=== Results: {passed}/{total} tests passed ===")
  
  if passed == total
    print("🎉 All pull lexer tests passed!")
    return true
  else
    print("❌ Some pull lexer tests failed")
    raise "test_failed"
  end
end

# Auto-run tests when file is executed
run_pull_lexer_tests()