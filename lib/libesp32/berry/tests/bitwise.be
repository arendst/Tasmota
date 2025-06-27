# Test bitwise operations
a = 11
assert(a & 0xFE == 10)  # AND operation
assert(a | 32 == 43)    # OR operation  
assert(a ^ 33 == 42)    # XOR operation

# Test with literals
assert(11 & 0xFE == 10)
assert(11 | 32 == 43)
assert(11 ^ 33 == 42)

# Test bitwise NOT
assert(~a == -12)
assert(~11 == -12)
