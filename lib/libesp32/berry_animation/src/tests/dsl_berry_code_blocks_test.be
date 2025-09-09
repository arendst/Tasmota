# DSL Berry Code Blocks Test Suite
# Tests for berry code block functionality in SimpleDSLTranspiler
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation/src -e "import tasmota" lib/libesp32/berry_animation/src/tests/dsl_berry_code_blocks_test.be

import animation
import animation_dsl
import string

# Test basic berry code block with triple braces
def test_basic_berry_block_triple_braces()
  print("Testing basic berry code block with triple braces...")
  
  var dsl_source = 'berry """\n' +
    'var test_var = 42\n' +
    'print("Hello from berry block")\n' +
    '"""\n' +
    'color red_custom = 0xFF0000\n'
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should generate Berry code")
  assert(string.find(berry_code, "# Berry code block") >= 0, "Should have berry code block comment")
  assert(string.find(berry_code, "var test_var = 42") >= 0, "Should include berry code verbatim")
  assert(string.find(berry_code, 'print("Hello from berry block")') >= 0, "Should include print statement")
  assert(string.find(berry_code, "# End berry code block") >= 0, "Should have end comment")
  assert(string.find(berry_code, "var red_custom_ = 0xFFFF0000") >= 0, "Should continue with DSL after berry block")
  
  print("✓ Basic berry code block (triple braces) test passed")
  return true
end

# Test basic berry code block with braces inside
def test_basic_berry_block_with_braces()
  print("Testing basic berry code block with braces inside...")
  
  var dsl_source = "berry '''\n" +
    "var test_var = 100\n" +
    "if test_var > 50\n" +
    "  print('Value is greater than 50')\n" +
    "end\n" +
    "'''\n" +
    "color blue_custom = 0x0000FF\n"
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should generate Berry code")
  assert(string.find(berry_code, "# Berry code block") >= 0, "Should have berry code block comment")
  assert(string.find(berry_code, "var test_var = 100") >= 0, "Should include berry code verbatim")
  assert(string.find(berry_code, "print('Value is greater than 50')") >= 0, "Should include print statement")
  assert(string.find(berry_code, "# End berry code block") >= 0, "Should have end comment")
  
  print("✓ Basic berry code block (single quotes) test passed")
  return true
end

# Test multiple berry code blocks
def test_multiple_berry_blocks()
  print("Testing multiple berry code blocks...")
  
  var dsl_source = 'berry """\n' +
    'var global_var = 50\n' +
    'def helper_function(x)\n' +
    '  return x * 2\n' +
    'end\n' +
    '"""\n' +
    'color green_custom = 0x00FF00\n' +
    "berry '''\n" +
    "var result = helper_function(global_var)\n" +
    "print('Result:', result)\n" +
    "'''\n" +
    "set col = 0xFF8800"
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should generate Berry code")
  
  # Check first berry block
  var first_block_pos = string.find(berry_code, "var global_var = 50")
  assert(first_block_pos >= 0, "Should include first berry block")
  
  # Check second berry block
  var second_block_pos = string.find(berry_code, "var result = helper_function(global_var)")
  assert(second_block_pos >= 0, "Should include second berry block")
  assert(second_block_pos > first_block_pos, "Second block should come after first block")
  
  # Check DSL continues to work
  assert(string.find(berry_code, "var green_custom_ = 0xFF00FF00") >= 0, "Should process DSL after berry blocks")
  
  print("✓ Multiple berry code blocks test passed")
  return true
end

# Test berry code block with complex content
def test_complex_berry_content()
  print("Testing berry code block with complex content...")
  
  var dsl_source = 'berry """\n' +
    'import math\n' +
    'import string\n' +
    '\n' +
    '# Complex function with multiple features\n' +
    'def calculate_dynamic_value(base, factor)\n' +
    '  var result = math.sin(factor) * base\n' +
    '  return int(result + 100)\n' +
    'end\n' +
    '\n' +
    'var config = {\n' +
    '  "brightness": 200,\n' +
    '  "speed": 1.5\n' +
    '}\n' +
    '\n' +
    'print("Complex berry block initialized")\n' +
    '"""\n' +
    'color purple_custom = 0x800080\n' +
    'animation pulse = pulsating_animation(color=purple_custom, period=2s)\n' +
    'run pulse'
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should generate Berry code")
  assert(string.find(berry_code, "import math") >= 0, "Should include import statements")
  assert(string.find(berry_code, "import string") >= 0, "Should include multiple imports")
  assert(string.find(berry_code, "def calculate_dynamic_value(base, factor)") >= 0, "Should include function definition")
  assert(string.find(berry_code, 'var config = {') >= 0, "Should include complex data structures")
  assert(string.find(berry_code, '"brightness": 200,') >= 0, "Should preserve map syntax")
  assert(string.find(berry_code, 'print("Complex berry block initialized")') >= 0, "Should include print statement")
  
  print("✓ Complex berry content test passed")
  return true
end

# Test berry code block interacting with DSL objects
def test_berry_dsl_interaction()
  print("Testing berry code block interaction with DSL objects...")
  
  var dsl_source = 'color red_custom = 0xFF0000\n' +
    'animation test_anim = pulsating_animation(color=red_custom, period=3s)\n' +
    'berry """\n' +
    '# Modify DSL-generated animation\n' +
    'test_anim_.opacity = 150\n' +
    'test_anim_.priority = 10\n' +
    'print("Animation modified via berry code")\n' +
    '"""\n' +
    'run test_anim'
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should generate Berry code")
  assert(string.find(berry_code, "var test_anim_ = animation.pulsating_animation(engine)") >= 0, "Should create animation")
  assert(string.find(berry_code, "test_anim_.opacity = 150") >= 0, "Should modify animation opacity")
  assert(string.find(berry_code, "test_anim_.priority = 10") >= 0, "Should modify animation priority")
  assert(string.find(berry_code, 'print("Animation modified via berry code")') >= 0, "Should include print statement")
  
  print("✓ Berry-DSL interaction test passed")
  return true
end

# Test berry code block with inline comments
def test_berry_block_with_inline_comment()
  print("Testing berry code block with inline comment...")
  
  var dsl_source = 'berry """  # This is an inline comment\n' +
    'var test_value = 123   # Berry code block  # This is an inline comment\n' +
    '"""\n' +
    'color yellow_custom = 0xFFFF00'
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should generate Berry code")
  assert(string.find(berry_code, "# Berry code block  # This is an inline comment") >= 0, "Should preserve inline comment")
  assert(string.find(berry_code, "var test_value = 123") >= 0, "Should include berry code")
  
  print("✓ Berry block with inline comment test passed")
  return true
end

# Test error handling - missing string after berry keyword
def test_error_missing_string()
  print("Testing error handling for missing string after berry keyword...")
  
  var dsl_source = 'berry\n' +
    'print("This should cause an error")\n' +
    'color test_color = 0xFF0000'
  
  try
    var berry_code = animation_dsl.compile(dsl_source)
    assert(false, "Should raise compilation error")
  except "dsl_compilation_error"
    # ok
  end
  
  print("✓ Error handling (missing string) test passed")
  return true
end

# Test error handling - invalid token after berry keyword
def test_error_invalid_token()
  print("Testing error handling for invalid token after berry keyword...")
  
  var dsl_source = 'berry 123\n' +
    'color test_color = 0xFF0000'
  
  try
    var berry_code = animation_dsl.compile(dsl_source)
    assert(false, "Should raise compilation error")
  except "dsl_compilation_error"
    # ok
  end
  
  print("✓ Error handling (invalid token) test passed")
  return true
end

# Test execution of berry code blocks
def test_berry_block_execution()
  print("Testing execution of berry code blocks...")
  
  var dsl_source = 'berry """\n' +
    'var execution_test = "Berry code executed successfully"\n' +
    'print(execution_test)\n' +
    '"""\n' +
    'color cyan_custom = 0x00FFFF\n' +
    'animation my_anim = solid(color=cyan_custom)'
    'run my_anim'
  
  # Test compilation
  var berry_code = animation_dsl.compile(dsl_source)
  assert(berry_code != nil, "Should compile successfully")
  
  # Test execution (this will actually run the berry code)
  try
    animation_dsl.execute(dsl_source)
    print("✓ Berry block execution test passed")
    return true
  except .. as e, m
    print("✗ Berry block execution failed:", e, m)
    return false
  end
end

# Test berry code blocks with multiline strings and complex syntax
def test_multiline_complex_syntax()
  print("Testing berry code blocks with multiline strings and complex syntax...")
  
  var dsl_source = 'berry """\n' +
    '# Test multiline strings and complex syntax\n' +
    'var multiline_string = "This is a\\n" +\n' +
    '                       "multiline string\\n" +\n' +
    '                       "with multiple lines"\n' +
    '\n' +
    'def complex_function(a, b, c)\n' +
    '  if a > b\n' +
    '    return a + c\n' +
    '  else\n' +
    '    return b + c\n' +
    '  end\n' +
    'end\n' +
    '\n' +
    'var result = complex_function(10, 5, 3)\n' +
    'print("Complex function result:", result)\n' +
    '"""\n' +
    'color magenta_custom = 0xFF00FF\n'
  
  var berry_code = animation_dsl.compile(dsl_source)
  
  assert(berry_code != nil, "Should generate Berry code")
  assert(string.find(berry_code, 'var multiline_string = "This is a\\n"') >= 0, "Should preserve multiline string syntax")
  assert(string.find(berry_code, "def complex_function(a, b, c)") >= 0, "Should include function definition")
  assert(string.find(berry_code, "if a > b") >= 0, "Should include conditional logic")
  assert(string.find(berry_code, "var result = complex_function(10, 5, 3)") >= 0, "Should include function call")
  
  print("✓ Multiline complex syntax test passed")
  return true
end

# Run all tests
def run_all_berry_block_tests()
  print("=== DSL Berry Code Blocks Test Suite ===")
  print("")
  
  var tests = [
    test_basic_berry_block_triple_braces,
    test_basic_berry_block_with_braces,
    test_multiple_berry_blocks,
    test_complex_berry_content,
    test_berry_dsl_interaction,
    test_berry_block_with_inline_comment,
    test_error_missing_string,
    test_error_invalid_token,
    test_berry_block_execution,
    test_multiline_complex_syntax
  ]
  
  var passed = 0
  var total = size(tests)
  
  for test_func : tests
    try
      if test_func()
        passed += 1
      end
    except .. as e, m
      print("✗ Test failed with exception:", e, m)
    end
    print("")
  end
  
  print("=== Berry Code Blocks Test Results ===")
  print(f"Passed: {passed}/{total}")
  
  if passed == total
    print("All berry code block tests passed! ✓")
    return true
  else
    print("Some berry code block tests failed! ✗")
    raise "test_failed"
  end
end

# Run the tests
return run_all_berry_block_tests()