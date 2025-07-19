# try to exercise bug in reference

class failable
    var fail        # if 'true', tostring() raises an exception

    def tostring()
        if self.fail
            raise "internal_error", "FAIL"
            return "FAIL"
        else
            return "SUCCESS"
        end
    end
end
f = failable()

l1 = [1, 2, f]
l2 = ["foo", l1]
l1.push(l1)

assert(str(l2) == "['foo', [1, 2, SUCCESS, [...]]]")
assert(str(l1) == "[1, 2, SUCCESS, [...]]")

f.fail = true
try
    print(str(l1))
except ..
end

f.fail = false
assert(str(l1) == "[1, 2, SUCCESS, [...]]")  # FAILS
