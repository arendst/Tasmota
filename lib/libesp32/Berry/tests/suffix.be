var keys = [ 'key1', 'key2', 'key3', 'key4' ]
var pairs = {
    keys[0]: 'value1',
    keys[1]: 'value2',
    keys[2]: 'value3',
    keys[3]: 'value4'
}

for i : 0 .. keys.size() - 1
    assert(pairs[keys[i]] == 'value' .. i + 1)
end
