# Test function closures and variable capture
var gbl
def func1()
    var a = 'func1_a'
    def func2()
        return a  # Capture variable from outer scope
    end
    gbl = func2
    return 400000 + 500
end
assert(func1() == 400500)
assert(gbl() == 'func1_a')  # Test closure still has access to captured variable
