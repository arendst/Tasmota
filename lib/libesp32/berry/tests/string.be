import string as s

assert(s.find('012345', '23') == 2)
assert(s.find('012345', '23', 1) == 2)
assert(s.find('012345', '23', 1, 3) == -1)
assert(s.find('012345', '23', 2, 4) == 2)
assert(s.find('012345', '23', 3) == -1)

assert(s.find('012345', '') == 0)
assert(s.find('012345', '', 0, 0) == 0)
assert(s.find('012345', '', 1) == 1)
assert(s.find('012345', '', 1, 1) == 1)
assert(s.find('012345', '', 1, 0) == -1)
assert(s.find('012345', '', 6) == 6)
assert(s.find('012345', '', 7) == -1)

assert(s.count('012345', '') == 7)
assert(s.count('012345', '', 2) == 5)
assert(s.count('012345', '', 6) == 1)

assert(s.count('121314', '1') == 3)
assert(s.count('121314', '1', 1) == 2)
assert(s.count('121314', '1', 2) == 2)
assert(s.count('121314', '1', 1, 2) == 0)
assert(s.count('121314', '1', 1, 3) == 1)

assert(s.split('a b c d e f', '1') == ['a b c d e f'])
assert(s.split('a b c d e f', ' ') == ['a', 'b', 'c', 'd', 'e', 'f'])
assert(s.split('a b c d e f', ' ', 2) == ['a', 'b', 'c d e f'])
assert(s.split('a b c d e f', '') == ['a b c d e f'])

assert(s.format("%%") == "%")
assert(s.format("%i%%", 12) == "12%")
assert(s.format("%i%%%i", 12, 13) == "12%13")
assert(s.format("%s%%", "foo") == "foo%")
assert(s.format("%.1f%%", 3.5) == "3.5%")

s="azerty"
assert(s[1..2] == "ze")
assert(s[1..] == "zerty")
assert(s[1..-1] == "zerty")

#- string ranges -#
s="azertyuiop"
assert(s[0] == "a")
assert(s[0..1] == "az")
assert(s[0..2] == "aze")
assert(s[0..10] == s)
assert(s[0..size(s)] == s)
assert(s[0..50] == s)       #- upper limit is allowed to be out of range -#

#- negative indices start from the end -#
s="azertyuiop"
assert(s[-1] == "p")
assert(s[-2] == "o")
assert(s[0..-2] == "azertyuio")
assert(s[-4..-2] == "uio")
assert(s[-2..-4] == "")  #- if range is in wrong order, returns empty string -#
assert(s[-40..-2] == "azertyuio")  #- borders are allowed to be out of range -#

# escape
import string
assert(string.escape("A") == '"A"')
assert(string.escape("A", true) == "'A'")
assert(string.escape("\"") == '"\\""')
assert(string.escape("\"", true) == '\'"\'')

var s ='"a\'b"\''
assert(string.escape(s) == '"\\"a\'b\\"\'"')
assert(string.escape(s, true) == '\'"a\\\'b"\\\'\'')

# tr
assert(string.tr("azer", "abcde", "ABCDE") == 'AzEr')
assert(string.tr("azer", "", "") == 'azer')
assert(string.tr("azer", "aaa", "ABC") == 'Azer')  # only first match works
assert(string.tr("A_b", "_", " ") == 'A b')
# tr used to remove characters
assert(string.tr("qwerty", "qwe", "_") == '_rty')

# the following should not crash
var s1 = 'A string of more than 128 bytes 012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789'
var s2 = string.format("%i, %s", 1, s1)

# replace
assert(string.replace("hello", "ll", "xx") == "hexxo")
assert(string.replace("hellollo", "ll", "xx") == "hexxoxxo")
assert(string.replace("hellollo", "aa", "xx") == "hellollo")
assert(string.replace("hello", "ll", "") == "heo")
assert(string.replace("hello", "", "xx") == "hello")
assert(string.replace("hello", "", "") == "hello")

# multi-line strings
var s = "a" "b""c"
assert(s == 'abc')

s = 'a'"b"'''c'
assert(s == 'abc')

s = "a"
'b'
    ""
    "c"
assert(s == 'abc')

s = "a" #- b -# "b"  #--# "c"
assert(s == 'abc')

s = "a"#
    # "z"
    "b"  # zz
    "c"
assert(s == 'abc')

# string.format with automatic conversion
import string

assert(string.format("%i", 3) == '3')
assert(string.format("%i", "3") == '3')
assert(string.format("%i", "03") == '3')
assert(string.format("%i", nil) == '')

class A def toint() return 42 end end
a=A()
class B end
b=B()

assert(string.format("%i", a) == '42')
assert(string.format("%i", b) == '')

assert(string.format("%i", nil) == '')
assert(string.format("%i", true) == '1')
assert(string.format("%i", false) == '0')

assert(string.format("%c", a) == '*')

assert(string.format("%f", 3.5) == '3.500000')
assert(string.format("%f", 3) == '3.000000')
assert(string.format("%.1f", 3) == '3.0')
assert(string.format("%.1f", nil) == '')
assert(string.format("%.1f", true) == '')
assert(string.format("%.1f", false) == '')
assert(string.format("%.1f", a) == '')

assert(string.format("%s", a) == '<instance: A()>')
assert(string.format("%s", 0) == '0')
assert(string.format("%s", nil) == 'nil')
assert(string.format("%s", true) == 'true')
assert(string.format("%s", false) == 'false')

assert(string.format("%q", "\ntest") == '\'\\ntest\'')

# corrupt format string should not crash the VM
string.format("%0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000f", 3.5)

# format is now synonym to string.format
assert(format == string.format)
assert(format("%.1f", 3) == '3.0')

# f-strings
assert(f"" == '')
assert(f'' == '')
assert(f"abc\n\r\t" == 'abc\n\r\t')
assert(f'{{a}}' == '{a}')
assert(f'\\\\' == '\\\\')

assert(f"A = {1+1}" == 'A = 2')
assert(f"A = {1+1:s}" == 'A = 2')
assert(f"A = {1+1:i}" == 'A = 2')
assert(f"A = {1+1:04i}" == 'A = 0002')

assert(f"P = {3.1415:.2f}" == 'P = 3.14')

var a = 'foobar{0}'
assert(f"S = {a}" == 'S = foobar{0}')
assert(f"S = {a:i}" == 'S = 0')
assert(f"{a=}" == 'a=foobar{0}')

# startswith case sensitive
assert(string.startswith("", "") == true)
assert(string.startswith("qwerty", "qw") == true)
assert(string.startswith("qwerty", "qwerty") == true)
assert(string.startswith("qwerty", "") == true)
assert(string.startswith("qwerty", "qW") == false)
assert(string.startswith("qwerty", "QW") == false)
assert(string.startswith("qwerty", "qz") == false)
assert(string.startswith("qwerty", "qwertyw") == false)

# startswith case insensitive
assert(string.startswith("qwerty", "qw", true) == true)
assert(string.startswith("qwerty", "qwerty", true) == true)
assert(string.startswith("qwerty", "", true) == true)
assert(string.startswith("qwerty", "qW", true) == true)
assert(string.startswith("qwerty", "QW", true) == true)
assert(string.startswith("qwerty", "qz", true) == false)
assert(string.startswith("qwerty", "qwertyw", true) == false)

# endswith case sensitive
assert(string.endswith("", "") == true)
assert(string.endswith("qwerty", "ty") == true)
assert(string.endswith("qwerty", "qwerty") == true)
assert(string.endswith("qwerty", "") == true)
assert(string.endswith("qwerty", "tY") == false)
assert(string.endswith("qwerty", "TY") == false)
assert(string.endswith("qwerty", "tr") == false)
assert(string.endswith("qwerty", "qwertyw") == false)

# endswith case insensitive
assert(string.endswith("", "", true) == true)
assert(string.endswith("qwerty", "ty", true) == true)
assert(string.endswith("qwerty", "qwerty", true) == true)
assert(string.endswith("qwerty", "", true) == true)
assert(string.endswith("qwerty", "tY", true) == true)
assert(string.endswith("qwerty", "TY", true) == true)
assert(string.endswith("qwerty", "tr", true) == false)
assert(string.endswith("qwerty", "qwertyw", true) == false)

# unicode literals
assert("\uF014" == "\xEF\x80\x94")

# string multiplication tests
# Basic integer multiplication
assert("aze" * 3 == "azeazeaze")
assert("ab" * 5 == "ababababab")
assert("x" * 1 == "x")
assert("hello" * 2 == "hellohello")

# Zero and negative multiplication
assert("aze" * 0 == "")
assert("hello" * -1 == "")
assert("test" * -5 == "")

# Boolean multiplication
assert("aze" * true == "aze")
assert("aze" * false == "")
assert("hello" * true == "hello")
assert("world" * false == "")

# Empty string multiplication
assert("" * 0 == "")
assert("" * 1 == "")
assert("" * 5 == "")
assert("" * 100 == "")
assert("" * true == "")
assert("" * false == "")

# Single character multiplication
assert("a" * 10 == "aaaaaaaaaa")
assert("z" * 0 == "")
assert("!" * 3 == "!!!")

# Large multiplication (testing performance and memory)
var large_result = "abc" * 20
assert(size(large_result) == 60)
assert(large_result == "abcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabc")

# Edge cases with special characters
assert("\n" * 3 == "\n\n\n")
assert("\t" * 2 == "\t\t")
assert("\"" * 4 == "\"\"\"\"")
assert("\\" * 3 == "\\\\\\")

# Verify that regular multiplication still works
assert(3 * 4 == 12)
assert(2.5 * 4 == 10)
assert(5 * 3 == 15)
assert(0 * 10 == 0)

# Test that invalid combinations still throw errors
try
    var result = "hello" * "world"
    assert(false, "Should have thrown an error")
except 'type_error'
    # Expected error
end

try
    var result = "hello" * 3.14
    assert(false, "Should have thrown an error")
except 'type_error'
    # Expected error
end

try
    var result = "hello" * nil
    assert(false, "Should have thrown an error")
except 'type_error'
    # Expected error
end

# Test with variables
var s = "test"
var count = 3
assert(s * count == "testtesttest")

var bool_true = true
var bool_false = false
assert(s * bool_true == "test")
assert(s * bool_false == "")

# Test chaining and expressions
assert(("a" * 2) + ("b" * 3) == "aabbb")
assert("x" * (1 + 2) == "xxx")
assert("y" * (true && true) == "y")
assert("z" * (false || false) == "")

# Test with longer strings
var long_str = "Hello, World!"
assert(long_str * 0 == "")
assert(long_str * 1 == "Hello, World!")
assert(long_str * 2 == "Hello, World!Hello, World!")

# Test boundary conditions
assert("a" * 64 == "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")  # SHORT_STR_MAX_LEN
var very_long = "a" * 100  # Should use long string path
assert(size(very_long) == 100)
