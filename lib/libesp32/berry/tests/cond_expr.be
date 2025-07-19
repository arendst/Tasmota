# Test conditional expressions (ternary operator)
assert("" != 0 ? true : false)
assert(false || !(true ? false : true) && true)
var t1 = 8, t2 = false
if t1 ? 7 + t1 : t2  # Test ternary in conditional
    var a = 'good'
    assert((a == 'good' ? a + '!' : a) == 'good!')
    assert((a == 'good?' ? a + '!' : a) != 'good!')
else
    assert('condition expression test failed')
end
