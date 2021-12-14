def assert_true(status)
    assert(status == true, 'assert(true) failed!')
end

def assert_false(status)
    assert(status == false, 'assert(false) failed!')
end

assert_true(0 == 0)
assert_false(0 != 0)
assert_true(0 != 1)
assert_false(0 == 1)


assert_true(0.0 == 0)
assert_false(0.0 != 0)
assert_true(0.0 != 1.0)
assert_false(0.0 == 1.0)

assert_true(nil == nil)
assert_false(nil != nil)
assert_true(true != nil)
assert_false(true == nil)
assert_true(nil != false)
assert_false(nil == false)

assert_true(list == list)
assert_false(list == map)

assert_true([] == [])
assert_true([true] == [true])
assert_true([[]] == [[]])
assert_false([[]] != [[]])
assert_false([0] == [])
assert_false([] != [])
assert_true([] != nil)
assert_false([] == nil)

assert_true({} != nil)
assert_false({} == nil)
