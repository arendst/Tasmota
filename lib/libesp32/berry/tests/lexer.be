import math

def check(a, b)
    assert(math.abs(a - b) < 1e-6)
end

def test_source(src, msg)
    try
        compile(src)
        assert(false, 'unexpected execution flow')
    except .. as e, m
        assert(e == 'syntax_error')
        assert(m == 'string:1: ' + msg)
    end
end

#----
  this is a
  mult-line comment
----#

compile('x = 5; 0..x')
assert('\x5a' == 'Z')
assert('\132' == 'Z')
assert('\a\b\f\n\r\t\v\\\'\"\?' == '\x07\x08\x0c\x0a\x0d\x09\x0b\x5c\x27\x22\x3f')
assert(.45 == 0.45)
assert(0X10 == 16)
assert(0x10 == 16)
assert(0X1A == 26)
assert(0x1a == 26)
check(45., 45)
check(45.e-1, 4.5)
check(45.E-1, 4.5)
check(45.1e-1, 4.51)
check(45.1e2, 4510)
check(45.e2, 4500)
check(45.e+2, 4500)

# unicode encoding from JSON
assert(bytes().fromstring("a").tohex() == "61")
assert(bytes().fromstring("\uF054").tohex() == "EF8194")
assert(bytes().fromstring("\uF054\uF055").tohex() == "EF8194EF8195")
assert(bytes().fromstring("a\uF054b").tohex() == "61EF819462")
# 1 byte
assert(bytes().fromstring("\u0061").tohex() == "61")
# 2 bytes
assert(bytes().fromstring("\u0088").tohex() == "C288")
assert(bytes().fromstring("\u0288").tohex() == "CA88")
# 3 bytes
assert(bytes().fromstring("\u1288").tohex() == "E18A88")

assert(bytes().fromstring("\uFFFF").tohex() == "EFBFBF")

# bad unicode encoding
test_source('"\\u"', "incorrect '\\u' encoding")
test_source('"\\u1"', "incorrect '\\u' encoding")
test_source('"\\u22"', "incorrect '\\u' encoding")
test_source('"\\u333"', "incorrect '\\u' encoding")

# Ensure pathologically long numbers don't crash the lexer (or cause an buffer overflow)
assert(000000000000000000000000000000000000E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000 == 0.0);


test_source('x = 5; 0...x;', 'unexpected symbol near \'.\'')
test_source('x = 5; 0...x;', 'unexpected symbol near \'.\'')
test_source('0xg', 'invalid hexadecimal number')
test_source('"\\x5g"', 'invalid hexadecimal number')
test_source('0x5g', 'malformed number')
test_source('"\\779"', 'invalid octal number')
test_source('"\n', 'unfinished string')

var malformed_numbers = [
    '45f',
    '45.f',
    '45.ef',
    '45.e-f',
    '45.e-1f',
    '45.e-1.',
    '45.5.',
    '0x45.',
    '0x45j'
]

for i : malformed_numbers
    test_source(i, 'malformed number')
end

#- ensure that string literal with NULL character is truncated -#
assert(size('aa\000bb\000cc') == 2)
