class Test
    var maximum
    def init(maximum)
        self.maximum = maximum
    end
    def iter() # method closure upvalues test
        var i = -1, maximum = self.maximum
        return def ()
            i += 1
            if i > maximum
                raise 'stop_iteration'
            end
            return i
        end
    end
end

var sum = 0
for i : Test(10)
    sum += i
end
assert(sum == 55, 'iteraion sum is ' + str(sum) + ' (expected 55).')
