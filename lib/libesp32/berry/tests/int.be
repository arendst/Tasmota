#- toint() converts any instance to int -#
class Test_int
    def toint()
        return 42
    end
end
t=Test_int()
assert(int(t) == 42)
