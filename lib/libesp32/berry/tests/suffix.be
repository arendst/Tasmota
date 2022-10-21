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

#- test cases related to #101 -#
class C var l end
c=C()
c.l=[0,1,2]

def t_101_nok_1() return c.l[0..1] end
def t_101_ok_1() var l2 = c.l return l2[0..1] end

t_i = 0
def t_101_nok_2() return c.l[t_i] end
def t_101_ok_2() return c.l[0] end

assert(t_101_nok_1() == [0, 1])
assert(t_101_ok_1() == [0, 1])
assert(t_101_nok_2() == 0)
assert(t_101_ok_2() == 0)
