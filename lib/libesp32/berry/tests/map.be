# Test map (dictionary) operations
m = { 'a':1, 'b':3.5, 'c': "foo", 0:1}

assert(type(m) == 'instance')
assert(classname(m) == 'map')

# Test element access
assert(m['a'] == 1)
assert(m['b'] == 3.5)
assert(m['c'] == 'foo')
assert(m[0] == 1)

# Test find method
assert(m.find('a') == 1)
assert(m.find('z') == nil)
assert(m.find('z', 4) == 4)  # With default value

# Test contains method
assert(m.contains('a'))
assert(m.contains(0))
assert(!m.contains('z'))
assert(!m.contains())

# Test assignment
m['y'] = -1
assert(m['y'] == -1)

# Test remove method
m = {1:2}
m.remove(2)  # Remove non-existent key
assert(str(m) == '{1: 2}')
m.remove(1)  # Remove existing key
assert(str(m) == '{}')

# Test boolean keys
m = {true:10, false:20}
assert(m.contains(true))
assert(m.contains(false))
assert(m[true] == 10)
assert(m[false] == 20)
