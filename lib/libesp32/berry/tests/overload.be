# Test operator overloading
class test
    def init()
        self._a = 123
    end
    def +()  # Overload unary + operator
        return self._a
    end
    def ()()  # Overload function call operator
        return self._a
    end
    var _a
end

print(test() + test())  # Should print 246 (123 + 123)
