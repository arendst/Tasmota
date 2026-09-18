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

import string
# m_item: key not found raises key_error
try
    var x = {'a': 1}['missing']
    assert(false)
except .. as e, m
    assert(e == 'key_error')
end

# m_insert: only inserts if key doesn't exist, returns bool
var mi = {'a': 1, 'b': 2}
assert(mi.insert('c', 3) == true)
assert(mi['c'] == 3)
assert(mi.insert('a', 99) == false)
assert(mi['a'] == 1)

# tostring with multiple entries (exercises comma branch in push_value)
var ms = str({'x': 1, 'y': 2})
assert(string.find(ms, ',') > 0)

# map.iter() iterates values
var vals = []
for v : {'p': 10}.iter()
    vals .. v
end
assert(vals == [10])

# real as map key (exercises hashreal)
var mr = {}
mr[1.5] = 'real_key'
assert(mr.contains(1.5))
assert(mr[1.5] == 'real_key')

# many entries to trigger collision chains, then bulk removal
var mb = {}
for i : 0..19
    mb[i] = i * 2
end
assert(mb.size() == 20)
mb.remove(0)
assert(!mb.contains(0))
assert(mb.size() == 19)
for i : 1..19
    mb.remove(i)
end
assert(mb.size() == 0)

# instance as map key - exercises hashins() in be_map.c
# case 1: instance with hash() method (custom hash)
class KeyWithHash
    var val
    def init(v) self.val = v end
    def hash() return self.val end
    def ==(other) return self.val == other.val end
end

var k1 = KeyWithHash(10)
var k2 = KeyWithHash(20)
var mh = {}
mh[k1] = 'ten'
mh[k2] = 'twenty'
assert(mh[k1] == 'ten')
assert(mh[k2] == 'twenty')
assert(mh.contains(k1))
assert(mh.size() == 2)

# case 2: instance without hash() method (falls back to hashptr)
class KeyNoHash
    var val
    def init(v) self.val = v end
end

var k3 = KeyNoHash(1)
var k4 = KeyNoHash(2)
var mnh = {}
mnh[k3] = 'a'
mnh[k4] = 'b'
assert(mnh[k3] == 'a')
assert(mnh[k4] == 'b')
assert(mnh.contains(k3))
assert(!mnh.contains(KeyNoHash(1)))  # different instance, different ptr hash

# instance with hash() returning non-int raises runtime_error (be_map.c lines 73-76)
class BadHash
    def hash() return "not an int" end
end

try
    var mbad = {}
    mbad[BadHash()] = 1
    assert(false)
except .. as e, m
    assert(e == 'runtime_error')
end
