# Test int() conversion function
class Test_int
    def toint()  # Custom conversion method
        return 42
    end
end
t = Test_int()
assert(int(t) == 42)  # Test custom toint() method

# Test hex string parsing
assert(int("0x00") == 0)
assert(int("0X1") == 1)
assert(int("0x000000F") == 15)
assert(int("0x1000") == 0x1000)
assert(int("0xFF00FF00") == 0xFF00FF00)