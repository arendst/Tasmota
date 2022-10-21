class mylist : classof([]) end

assert(issubclass(mylist, list) == true)
assert(issubclass(mylist, []) == true)
assert(issubclass(mylist(), list) == false)
assert(issubclass(mylist(), []) == false)

assert(isinstance(mylist, list) == false)
assert(isinstance(mylist, []) == false)
assert(isinstance(mylist(), list) == true)
assert(isinstance(mylist(), []) == true)

assert(issubclass(list, list) == true)
assert(issubclass(list, []) == true)
assert(issubclass(list(), list) == false)
assert(issubclass(list(), []) == false)

assert(isinstance(list, list) == false)
assert(isinstance(list, []) == false)
assert(isinstance(list(), list) == true)
assert(isinstance(list(), []) == true)

assert(issubclass(list, list) == true)
assert(issubclass(list, []) == true)
assert(issubclass(list(), list) == false)
assert(issubclass(list(), []) == false)

assert(issubclass(list, mylist) == false)
assert(isinstance([], mylist) == false)
