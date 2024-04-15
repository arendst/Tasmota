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
