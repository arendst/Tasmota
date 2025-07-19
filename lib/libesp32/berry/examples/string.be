s = "This is a long string test. 0123456789 abcdefg ABCDEFG"
print(s)

a = .5
print(a)

import string as s

print(s.hex(0x45678ABCD, 16))

def bin(x, num)
    assert(type(x) == 'int', 'the type of \'x\' must be integer')
    # test the 'x' bits
    var bits = 1
    for i : 0 .. 62
        if x & (1 << 63 - i)
            bits = 64 - i
            break
        end
    end
    if type(num) == 'int' && num > 0 && num <= 64
        bits = bits < num ? num : bits
    end
    var result = ''
    bits -= 1
    for i : 0 .. bits
        result += x & (1 << (bits - i)) ? '1' : '0'
    end
    return result
end

print(bin(33))
