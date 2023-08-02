import json
import string
# load tests

def assert_load(text, value)
    var loaded_val = json.load(text)
    var ok = loaded_val == value
    if !ok 
        print(string.format('for JSON \'%s\' expected %s [%s] but got %s [%s]', text, str(value), type(value), str(loaded_val), type(loaded_val)))
    end
    assert(ok)
end

def assert_load_failed(text)
    assert(json.load(text) == nil)
end

assert_load('null', nil)
assert_load('true', true)
assert_load('false', false)
assert_load('123', 123)
assert_load('12.3', 12.3)
assert_load('-0.1', -0.1)
assert_load('1e2', 1e2)
assert_load('1e+2', 1e+2)
assert_load('1e-2', 1e-2)
assert_load('1E2', 1e2)
assert_load('1E+2', 1e+2)
assert_load('1.2e7', 1.2e7)
assert_load('"abc"', 'abc')
# strings
assert_load('"\\"\\\\\\/\\b\\f\\n\\r\\t"', '\"\\/\b\f\n\r\t')
assert_load('"\\u1234\\u2345\\u04aF\\u003A"', 'ሴ⍅ү:')
assert_load_failed('"\\u3fr"');
assert_load_failed('"\\q"');
assert_load_failed('"123');
# list
assert_load('[1, null]', [1, nil])
assert_load_failed('[x]')
assert_load_failed('[1, nil]')
assert_load_failed('[1, null')
# object
var o = json.load('{"key": 1}')
assert(o['key'] == 1 && o.size() == 1)

# parsing an empty string used to cause berry to pass a NULL to strncmp
# make sure we catch this
o = json.load('{"key": ""}')
assert(o['key'] == '' && o.size() == 1)

assert_load_failed('{"ke: 1}')
assert_load_failed('{"key": 1x}')
assert_load_failed('{"key"}')
assert_load_failed('{"key": 1, }')
# insanely long, nested object
var text = 'null'
for i : 0 .. 200
    text = '{"nested":' + text + ', "num": 1, "bool": true, "str": "abc", "n": null, "arr": [1, 2, 3]}'
end
json.load(text) # do nothing, just check that it doesn't crash

# dump tests

def assert_dump(value, text, format)
    assert(json.dump(value, format) == text)
end

assert_dump(nil, 'null');
assert_dump(true, 'true');
assert_dump(false, 'false');
assert_dump(1.23, '1.23');
assert_dump('String', '"String"');
assert_dump([1, 'x'], '[1,"x"]');
assert_dump({1: 'x'}, '{"1":"x"}');
assert_dump([1, 'x'], '[\n  1,\n  "x"\n]', 'format');
assert_dump({1: 'x'}, '{\n  "1": "x"\n}', 'format');
assert_dump({1: 'x', 'k': 'v'}, '{"k":"v","1":"x"}');

class map2 : map def init() super(self).init() end end
var m = map2()
m['key'] = 1
assert_dump(m, '{"key":1}')

# sweep dumping nested arrays of diffrent sizes
# this tests for any unexpanded stack conditions
for count : 10..200
    var arr = [[]]
    var last_arr = arr
    for i : 0..count
        var pushed = [i]
        last_arr.push(pushed)
        last_arr = pushed
    end
    json.dump(arr)
end
