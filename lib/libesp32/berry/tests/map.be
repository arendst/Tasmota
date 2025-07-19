m = { 'a':1, 'b':3.5, 'c': "foo", 0:1}

assert(type(m) == 'instance')
assert(classname(m) == 'map')

# accessor
assert(m['a'] == 1)
assert(m['b'] == 3.5)
assert(m['c'] == 'foo')
assert(m[0] == 1)

# find
assert(m.find('a') == 1)
assert(m.find('z') == nil)
assert(m.find('z', 4) == 4)

# contains
assert(m.contains('a'))
assert(m.contains(0))
assert(!m.contains('z'))
assert(!m.contains())

# set
m['y'] = -1
assert(m['y'] == -1)

# remove
m={1:2}
m.remove(2)
assert(str(m) == '{1: 2}')
m.remove(1)
assert(str(m) == '{}')

# allow booleans to be used as keys
m={true:10, false:20}
assert(m.contains(true))
assert(m.contains(false))
assert(m[true] == 10)
assert(m[false] == 20)
