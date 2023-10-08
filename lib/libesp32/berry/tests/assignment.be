class Test
    var a
end

# continuous assignment of global suffix expressions
o = Test()
o.a = 100
assert(o.a == 100)
o.a += 10
assert(o.a == 110)

p = Test()
p.a = Test()
p.a.a = 50
assert(p.a.a == 50)
p.a.a += 10
assert(p.a.a == 60)

# continuous assignment of local suffix expressions
def test_func()
    var o = Test()
    o.a = 100
    assert(o.a == 100)
    o.a += 10
    assert(o.a == 110)

    var p = Test()
    p.a = Test()
    p.a.a = 50
    assert(p.a.a == 50)
    p.a.a += 10
    assert(p.a.a == 60)
end
test_func()
