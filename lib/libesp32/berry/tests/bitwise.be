# and, or, xor
a = 11
assert(a & 0xFE == 10)
assert(a | 32 == 43)
assert(a ^ 33 == 42)

# same with literal
assert(11 & 0xFE == 10)
assert(11 | 32 == 43)
assert(11 ^ 33 == 42)

# flip
assert(~a == -12)
assert(~11 == -12)
