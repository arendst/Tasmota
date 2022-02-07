import json

# load tests

def assert_load(text, value)
    assert(json.load(text) == value)
end

def assert_load_failed(text)
    assert(json.load(text) == nil)
end

assert_load('null', nil)
assert_load('true', true)
assert_load('false', false)
assert_load('123', 123)
assert_load('12.3', 12.3)
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
assert_load_failed('{"ke: 1}')
assert_load_failed('{"key": 1x}')
assert_load_failed('{"key"}')
assert_load_failed('{"key": 1, }')

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
