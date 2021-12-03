l = [1, 2, 3, 4, 5]
assert(l[0] == 1)
assert(l[1] == 2)
assert(l[2] == 3)
assert(l[3] == 4)
assert(l[4] == 5)
assert(str(l) == '[1, 2, 3, 4, 5]')

it = l.iter()
assert(it() == 1)
assert(it() == 2)
assert(it() == 3)
assert(it() == 4)
assert(it() == 5)

l.insert(0, 10)
assert(l[0] == 10)
assert(l.size() == 6)
l.remove(0)
assert(l.size() == 5)
assert(l[0] == 1)
l.setitem(0, 42)
assert(l[0] == 42)
assert(l.item(2) == 3)
l.resize(10)
assert(l.size() == 10)
assert(l.tostring() == '[42, 2, 3, 4, 5, nil, nil, nil, nil, nil]')

assert(([] == []) == true)
assert(([] != []) == false)
assert(([1] == [1]) == true)
assert(([1] != [1]) == false)
assert(([1] == [0]) == false)
assert(([1] != [0]) == true)
assert(([1, 2, 3] == [1, 2, 3]) == true)
assert(([1, 2, 3] != [1, 2, 3]) == false)
assert(([1, 2, 3] == [1, 2, 4]) == false)
assert(([1, 2, 3] != [1, 2, 4]) == true)
assert(([1, 2, ['w']] == [1, 2, ['w']]) == true)
assert(([1, 2, ['w']] != [1, 2, ['w']]) == false)
assert(([1, 2, ['w']] == [1, 2, ['z']]) == false)
assert(([1, 2, ['w']] != [1, 2, ['z']]) == true)
assert(([1, 2, ['w']] == [1, 2, []]) == false)
assert(([1, 2, ['w']] != [1, 2, []]) == true)

var l = [0, 1, 2, 3]
assert(l[-1] == 3)
assert(l[-2] == 2)
var t = l.copy()
l.insert(-2, 4)
assert(t == [0, 1, 2, 3] && t != l)
assert(l == [0, 1, 4, 2, 3])
l.remove(-2)
assert(l == [0, 1, 4, 3])
assert(l.reverse() == [3, 4, 1, 0])
assert(l + [5, 6] == [3, 4, 1, 0, 5, 6])
l = [0]
assert(l .. '3' == [0, '3'])
l.push(1)
assert(l == [0, '3', 1])
assert(l.concat() == '031')
l.pop()
assert(l == [0, '3'])
l.pop(0)
assert(l == ['3'])

l1 = [0, 1]
l2 = [2, 3]
assert(l1+l2==[0, 1, 2, 3])
assert(l1 == [0, 1])
assert(l2 == [2, 3])
assert(l1+[2] == [0, 1, 2])
assert([-1]+l1 == [-1, 0, 1])
assert(l1 == [0, 1])

#- find -#
#- if no argument return nil -#
assert([].find() == nil)
assert([1,2].find() == nil)
assert([1,1,nil,2].find() == nil)

#- nil if not found -#
assert([1,2].find(3) == nil)
assert([1,2].find(true) == nil)
assert([1,2].find('foo') == nil)

#- if found -#
assert([1,2,3,4].find(1) == 0)
assert([1,2,3,4].find(2) == 1)
assert([1,2,3,4].find(3) == 2)
assert([1,2,3,4].find(4) == 3)
assert([1,2,"foo",4].find('foo') == 2)

#- if multiple occurrences -#
assert([1,1,2,2].find(1) == 0)
assert([1,1,2,2].find(2) == 2)

#- look for nil -#
assert([1,1,nil,2].find(nil) == 2)

#- sub-structure -#
assert([1,[1,nil,2],3,[3]].find(3) == 2)
assert([1,[1,nil,2],3,[3]].find([3]) == 3)
assert([1,[1,nil,2],3,[3]].find([1,nil,2]) == 1)

#- keys() -#
assert(str(["a",'b',0].keys()) == "(0..2)")
assert(str([nil].keys()) == "(0..0)")
assert(str([].keys()) == "(0..-1)")

#- concat with delimiter -#
assert(["foo","bar",0].concat() == "foobar0")
assert([1,2,3].concat() == "123")
assert(["foo","bar",0].concat('') == "foobar0")
assert([1,2,3].concat('') == "123")

assert(["foo","bar",0].concat('-') == "foo-bar-0")
assert([].concat('<->') == "")
assert(["foo"].concat('<->') == "foo")
assert(["foo","bar",0].concat('<->') == "foo<->bar<->0")

assert(["","foo","bar",0].concat('<->') == "<->foo<->bar<->0")
assert(["","",1,"bar",0].concat('<->') == "<-><->1<->bar<->0")
assert(["","",1,"bar",0].concat('') == "1bar0")

assert([1,2,3].concat('-') == "1-2-3")
assert([1,"2",3].concat('-') == "1-2-3")

assert(["",2,3].concat('-') == "-2-3")

#- negative indices -#
assert([1,2,3,4][0] == 1)
assert([1,2,3,4][-1] == 4)
assert([1,2,3,4][-2] == 3)

assert([1,2,3,4][1..10] == [2,3,4])
assert([1,2,3,4][1..-1] == [2,3,4])
assert([1,2,3,4][1..-2] == [2,3])
assert([1,2,3,4][3..2] == [])
assert([1,2,3,4][2..-3] == [])