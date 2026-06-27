"""
Tests for the json module — ported from tests/json.be

Covers: json.load() parsing (primitives, strings, unicode, numbers,
arrays, objects, escapes, control chars, security), json.dump() serialization,
and the stack-size stress test from tests/json_test_stack_size.be.
"""

import sys
import subprocess
import pytest

BERRY_CMD = [sys.executable, "-m", "berry_port"]


def run_berry(*args, input_text=None, timeout=15):
    """Run berry_port CLI with given args, return CompletedProcess."""
    return subprocess.run(
        [*BERRY_CMD, *args],
        capture_output=True,
        text=True,
        input=input_text,
        timeout=timeout,
    )


def berry_eval(source, timeout=15):
    """Run Berry source, return (returncode, stdout, stderr)."""
    r = run_berry("-e", source, timeout=timeout)
    return r.returncode, r.stdout, r.stderr


# ============================================================================
# Helpers — assert_load / assert_load_failed as Berry source
# ============================================================================

ASSERT_LOAD_PRELUDE = (
    "import json\nimport string\n"
    "def assert_load(text, value)\n"
    "  var loaded_val = json.load(text)\n"
    "  var ok = loaded_val == value\n"
    "  if !ok\n"
    "    print(string.format('FAIL: for JSON \\'%s\\' expected %s [%s] but got %s [%s]',\n"
    "      text, str(value), type(value), str(loaded_val), type(loaded_val)))\n"
    "  end\n"
    "  assert(ok)\n"
    "end\n"
    "def assert_load_failed(text)\n"
    "  assert(json.load(text) == nil)\n"
    "end\n"
)


def run_json_test(body, timeout=15):
    """Run a Berry test body with the assert_load helpers pre-loaded."""
    source = ASSERT_LOAD_PRELUDE + body
    rc, stdout, stderr = berry_eval(source, timeout=timeout)
    assert rc == 0, f"Berry exited {rc}\nstdout: {stdout}\nstderr: {stderr}"
    return stdout


# ============================================================================
# json.load — primitives
# ============================================================================

class TestJsonLoadPrimitives:
    def test_null(self):
        run_json_test("assert_load('null', nil)")

    def test_true(self):
        run_json_test("assert_load('true', true)")

    def test_false(self):
        run_json_test("assert_load('false', false)")


# ============================================================================
# json.load — numbers
# ============================================================================

class TestJsonLoadNumbers:
    def test_integer(self):
        run_json_test("assert_load('123', 123)")

    def test_real(self):
        run_json_test("assert_load('12.3', 12.3)")

    def test_negative_real(self):
        run_json_test("assert_load('-0.1', -0.1)")

    def test_exponent_lower(self):
        run_json_test("assert_load('1e2', 1e2)")

    def test_exponent_plus(self):
        run_json_test("assert_load('1e+2', 1e+2)")

    def test_exponent_minus(self):
        run_json_test("assert_load('1e-2', 1e-2)")

    def test_exponent_upper(self):
        run_json_test("assert_load('1E2', 1e2)")

    def test_exponent_upper_plus(self):
        run_json_test("assert_load('1E+2', 1e+2)")

    def test_real_with_exponent(self):
        run_json_test("assert_load('1.2e7', 1.2e7)")


# ============================================================================
# json.load — strings
# ============================================================================

class TestJsonLoadStrings:
    def test_simple_string(self):
        run_json_test("assert_load('\"abc\"', 'abc')")

    def test_escape_sequences(self):
        # Test all JSON escape sequences: \" \\ \/ \b \f \n \r \t
        # Use the exact bytes from tests/json.be line 32 to avoid
        # Python interpreting \b \f as control characters.
        import tempfile, os
        assert_line = bytes.fromhex(
            '6173736572745f6c6f61642827225c5c225c5c5c5c5c5c2f'
            '5c5c625c5c665c5c6e5c5c725c5c7422272c20275c225c5c'
            '2f5c625c665c6e5c725c7427290a'
        )
        berry_src = (
            b"import json\nimport string\n"
            b"def assert_load(text, value)\n"
            b"  var loaded_val = json.load(text)\n"
            b"  assert(loaded_val == value)\n"
            b"end\n"
        ) + assert_line
        with tempfile.NamedTemporaryFile(suffix='.be', delete=False) as f:
            f.write(berry_src)
            tmppath = f.name
        try:
            r = run_berry(tmppath)
            assert r.returncode == 0, (
                f"Berry exited {r.returncode}\nstdout: {r.stdout}\nstderr: {r.stderr}"
            )
        finally:
            os.unlink(tmppath)
        with tempfile.NamedTemporaryFile(suffix='.be', delete=False) as f:
            f.write(berry_src)
            tmppath = f.name
        try:
            r = run_berry(tmppath)
            assert r.returncode == 0, (
                f"Berry exited {r.returncode}\nstdout: {r.stdout}\nstderr: {r.stderr}"
            )
        finally:
            os.unlink(tmppath)

    def test_escape_sequences_partial(self):
        """Test the escape sequences the Python port lexer can handle."""
        run_json_test(
            "import json\n"
            "var r = json.load('\"\\\\\\\\\\\\n\\\\r\\\\t\\\\\\\\\\\\\"\\\\/')\n"
        )
        # Test individual escapes that work
        run_json_test(r"""assert_load('"\\n"', "\n")""")
        run_json_test(r"""assert_load('"\\r"', "\r")""")
        run_json_test(r"""assert_load('"\\t"', "\t")""")
        run_json_test(r"""assert_load('"\\\\"', "\\")""")
        run_json_test(r"""assert_load('"\\/"', "/")""")

    def test_unicode_sequences(self):
        run_json_test(
            r"""assert_load('"\\u1234\\u2345\\u04aF\\u003A"', 'ሴ⍅ү:')"""
        )

    def test_invalid_unicode_hex(self):
        run_json_test(r"""assert_load_failed('"\\u3fr"')""")

    def test_invalid_escape_char(self):
        run_json_test(r"""assert_load_failed('"\\q"')""")

    def test_unterminated_string(self):
        run_json_test("""assert_load_failed('"123')""")

    def test_empty_string_in_object(self):
        run_json_test(
            "var o = json.load('{\"key\": \"\"}')\n"
            "assert(o['key'] == '' && o.size() == 1)"
        )


# ============================================================================
# json.load — arrays
# ============================================================================

class TestJsonLoadArrays:
    def test_array_with_null(self):
        run_json_test("assert_load('[1, null]', [1, nil])")

    def test_invalid_array_value(self):
        run_json_test("assert_load_failed('[x]')")

    def test_nil_not_valid_json(self):
        run_json_test("assert_load_failed('[1, nil]')")

    def test_unterminated_array(self):
        run_json_test("assert_load_failed('[1, null')")


# ============================================================================
# json.load — objects
# ============================================================================

class TestJsonLoadObjects:
    def test_simple_object(self):
        run_json_test(
            "var o = json.load('{\"key\": 1}')\n"
            "assert(o['key'] == 1 && o.size() == 1)"
        )

    def test_unterminated_key(self):
        run_json_test("assert_load_failed('{\"ke: 1}')")

    def test_trailing_garbage(self):
        run_json_test("assert_load_failed('{\"key\": 1x}')")

    def test_missing_value(self):
        run_json_test("assert_load_failed('{\"key\"}')")

    def test_trailing_comma(self):
        run_json_test("assert_load_failed('{\"key\": 1, }')")

    def test_deeply_nested_object(self):
        run_json_test(
            "var text = 'null'\n"
            "for i : 0 .. 200\n"
            "  text = '{\"nested\":' + text + ', \"num\": 1, \"bool\": true, "
            "\"str\": \"abc\", \"n\": null, \"arr\": [1, 2, 3]}'\n"
            "end\n"
            "json.load(text)",
            timeout=30,
        )


# ============================================================================
# json.dump
# ============================================================================

class TestJsonDump:
    def test_dump_nil(self):
        run_json_test(
            "assert(json.dump(nil) == 'null')"
        )

    def test_dump_true(self):
        run_json_test("assert(json.dump(true) == 'true')")

    def test_dump_false(self):
        run_json_test("assert(json.dump(false) == 'false')")

    def test_dump_real(self):
        run_json_test("assert(json.dump(1.23) == '1.23')")

    def test_dump_string(self):
        run_json_test("assert(json.dump('String') == '\"String\"')")

    def test_dump_list(self):
        run_json_test("assert(json.dump([1, 'x']) == '[1,\"x\"]')")

    def test_dump_map(self):
        run_json_test("assert(json.dump({1: 'x'}) == '{\"1\":\"x\"}')")

    def test_dump_list_format(self):
        run_json_test(
            r"assert(json.dump([1, 'x'], 'format') == '[\n  1,\n  \"x\"\n]')"
        )

    def test_dump_map_format(self):
        run_json_test(
            r"assert(json.dump({1: 'x'}, 'format') == '{\n  \"1\": \"x\"\n}')"
        )

    def test_dump_map_multiple_keys(self):
        run_json_test(
            "assert(json.dump({1: 'x', 'k': 'v'}) == '{\"k\":\"v\",\"1\":\"x\"}')"
        )

    def test_dump_subclass_map(self):
        run_json_test(
            "class map2 : map def init() super(self).init() end end\n"
            "var m = map2()\n"
            "m['key'] = 1\n"
            "assert(json.dump(m) == '{\"key\":1}')"
        )

    def test_dump_nested_arrays_sweep(self):
        """Sweep dumping nested arrays of different sizes — tests stack."""
        run_json_test(
            "for count : 10..200\n"
            "  var arr = [[]]\n"
            "  var last_arr = arr\n"
            "  for i : 0..count\n"
            "    var pushed = [i]\n"
            "    last_arr.push(pushed)\n"
            "    last_arr = pushed\n"
            "  end\n"
            "  json.dump(arr)\n"
            "end",
            timeout=30,
        )


# ============================================================================
# Security tests — ported from the new tests in tests/json.be
# ============================================================================

class TestJsonSecurityUnicodeExpansion:
    """Test 1: Unicode expansion buffer overflow protection."""

    def test_unicode_1byte(self):
        run_json_test(r"""assert_load('"\\u0048"', 'H')""")

    def test_unicode_2byte(self):
        run_json_test(r"""assert_load('"\\u00E9"', 'é')""")

    def test_unicode_3byte(self):
        run_json_test(r"""assert_load('"\\u0800"', 'ࠀ')""")

    def test_many_unicode_sequences(self):
        run_json_test(
            "var many_unicode = '\"'\n"
            "for i: 0..49\n"
            "  many_unicode += '\\\\u0800'\n"
            "end\n"
            "many_unicode += '\"'\n"
            "var result = json.load('{\"test\": ' + many_unicode + '}')\n"
            "assert(result != nil)\n"
            "assert(size(result['test']) == 150)"
        )


class TestJsonSecurityInvalidUnicode:
    """Test 2: Invalid Unicode sequence rejection."""

    def test_non_hex_chars(self):
        run_json_test(r"""assert_load_failed('"\\uXXXX"')""")

    def test_mixed_hex(self):
        run_json_test(r"""assert_load_failed('"\\u12XY"')""")

    def test_incomplete_u(self):
        run_json_test(r"""assert_load_failed('"\\u"')""")

    def test_too_short(self):
        run_json_test(r"""assert_load_failed('"\\u123"')""")

    def test_invalid_hex_digit(self):
        run_json_test(r"""assert_load_failed('"\\u123G"')""")


class TestJsonSecurityControlChars:
    """Test 3: Control character validation."""

    def test_unescaped_newline(self):
        run_json_test(r"""assert_load_failed('{"test": "hello\x0Aworld"}')""")

    def test_unescaped_tab(self):
        run_json_test(r"""assert_load_failed('{"test": "hello\x09world"}')""")

    def test_unescaped_cr(self):
        run_json_test(r"""assert_load_failed('{"test": "hello\x0Dworld"}')""")

    def test_unescaped_control_01(self):
        run_json_test(r"""assert_load_failed('{"test": "hello\x01world"}')""")

    def test_escaped_newline(self):
        run_json_test(
            r"""var r = json.load('{"test": "hello\\nworld"}')"""
            "\n"
            r"""assert(r != nil && r['test'] == "hello\nworld")"""
        )

    def test_escaped_tab(self):
        run_json_test(
            r"""var r = json.load('{"test": "hello\\tworld"}')"""
            "\n"
            r"""assert(r != nil && r['test'] == "hello\tworld")"""
        )

    def test_escaped_cr(self):
        run_json_test(
            r"""var r = json.load('{"test": "hello\\rworld"}')"""
            "\n"
            r"""assert(r != nil && r['test'] == "hello\rworld")"""
        )


class TestJsonSecurityInvalidEscapes:
    """Test 4: Invalid escape sequence rejection."""

    def test_backslash_q(self):
        run_json_test(r"""assert_load_failed('"\\q"')""")

    def test_backslash_x(self):
        run_json_test(r"""assert_load_failed('"\\x"')""")

    def test_backslash_z(self):
        run_json_test(r"""assert_load_failed('"\\z"')""")

    def test_incomplete_escape(self):
        run_json_test(r"""assert_load_failed('"\\"')""")


class TestJsonSecurityStringLimits:
    """Test 5: String length limits."""

    def test_long_string(self):
        run_json_test(
            "var long_str = '\"'\n"
            "for i: 0..999\n"
            "  long_str += 'a'\n"
            "end\n"
            "long_str += '\"'\n"
            "var result = json.load('{\"test\": ' + long_str + '}')\n"
            "assert(result != nil)\n"
            "assert(size(result['test']) == 1000)"
        )


class TestJsonSecurityMixedContent:
    """Test 6: Mixed Unicode and ASCII."""

    def test_mixed_unicode_ascii(self):
        run_json_test(
            r"""var mixed = '{"message": "Hello \\u4E16\\u754C! Welcome to \\u0048\\u0065\\u006C\\u006C\\u006F world."}'"""
            "\n"
            "var result = json.load(mixed)\n"
            "assert(result != nil)\n"
            r"""assert(result['message'] == "Hello 世界! Welcome to Hello world.")"""
        )


class TestJsonSecurityEdgeCases:
    """Test 7: Edge cases."""

    def test_empty_string(self):
        run_json_test(
            "var r = json.load('{\"empty\": \"\"}')\n"
            "assert(r != nil && r['empty'] == '')"
        )

    def test_unicode_only_string(self):
        run_json_test(
            r"""var r = json.load('{"unicode": "\\u0048\\u0065\\u006C\\u006C\\u006F"}')"""
            "\n"
            "assert(r != nil && r['unicode'] == 'Hello')"
        )

    def test_escape_only_string(self):
        run_json_test(
            r"""var r = json.load('{"escapes": "\\n\\t\\r\\\\\\""}')"""
            "\n"
            r"""assert(r != nil && r['escapes'] == "\n\t\r\\\"")"""
        )

    def test_max_unicode_value(self):
        run_json_test(
            r"""var r = json.load('{"max_unicode": "\\uFFFF"}')"""
            "\n"
            "assert(r != nil)"
        )


class TestJsonSecurityMalformed:
    """Test 8: Malformed JSON strings."""

    def test_unterminated_string(self):
        run_json_test("""assert_load_failed('{"test": "unterminated')""")

    def test_unterminated_escape(self):
        run_json_test(r"""assert_load_failed('{"test": "unterminated\\')""")

    def test_trailing_garbage_after_string(self):
        run_json_test("""assert_load_failed('{"test": "valid"x}')""")

    def test_invalid_unicode_in_multi_field(self):
        run_json_test(
            r"""assert_load_failed('{"test": "\\uXXXX", "other": "valid"}')"""
        )


class TestJsonSecurityNestedUnicode:
    """Test 9: Nested objects with Unicode (stress test)."""

    def test_nested_unicode_structure(self):
        run_json_test(
            r"""var nested = '{"level0": {"unicode": "\\u0800\\u0801\\u0802", "level1": {"unicode": "\\u0800\\u0801\\u0802", "final": "\\u4E16\\u754C"}}}'"""
            "\n"
            "var result = json.load(nested)\n"
            "assert(result != nil)"
        )


class TestJsonSecurityRegression:
    """Test 10: Security regression — buffer overflow pattern."""

    def test_many_3byte_unicode(self):
        run_json_test(
            "var attack_pattern = '{\"payload\": \"'\n"
            "for i: 0..99\n"
            "  attack_pattern += '\\\\u0800'\n"
            "end\n"
            "attack_pattern += '\"}'\n"
            "var result = json.load(attack_pattern)\n"
            "assert(result != nil)\n"
            "assert(size(result['payload']) == 300)"
        )


# ============================================================================
# Stack size test — ported from tests/json_test_stack_size.be
# ============================================================================

class TestJsonStackSize:
    def test_large_object_stack(self):
        """Parsing a large JSON object should not cause stack overflow."""
        run_json_test(
            "var arr = '{'\n"
            "for i : 0..1000\n"
            "  arr += '\"k' + str(i) + '\": \"v' + str(i) + '\",'\n"
            "end\n"
            "arr += '}'\n"
            "json.load(arr)",
            timeout=30,
        )
