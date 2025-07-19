#- toint() converts any instance to int -#
class Test_int
    def toint()
        return 42
    end
end
t=Test_int()
assert(int(t) == 42)

#- int can parse hex strings -#
assert(int("0x00") == 0)
assert(int("0X1") == 1)
assert(int("0x000000F") == 15)
assert(int("0x1000") == 0x1000)
