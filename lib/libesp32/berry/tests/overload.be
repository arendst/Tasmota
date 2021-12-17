class test
    def init()
        self._a = 123
    end
    def +()
        return self._a
    end
    def ()()
        return self._a
    end
    var _a
end

print(test() + test())
