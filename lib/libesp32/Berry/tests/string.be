import string as s

assert(s.find('012345', '23') == 2)
assert(s.find('012345', '23', 1) == 2)
assert(s.find('012345', '23', 1, 3) == -1)
assert(s.find('012345', '23', 2, 4) == 2)
assert(s.find('012345', '23', 3) == -1)

assert(s.find('012345', '') == 0)
assert(s.find('012345', '', 0, 0) == 0)
assert(s.find('012345', '', 1) == 1)
assert(s.find('012345', '', 1, 1) == 1)
assert(s.find('012345', '', 1, 0) == -1)
assert(s.find('012345', '', 6) == 6)
assert(s.find('012345', '', 7) == -1)

assert(s.count('012345', '') == 7)
assert(s.count('012345', '', 2) == 5)
assert(s.count('012345', '', 6) == 1)

assert(s.count('121314', '1') == 3)
assert(s.count('121314', '1', 1) == 2)
assert(s.count('121314', '1', 2) == 2)
assert(s.count('121314', '1', 1, 2) == 0)
assert(s.count('121314', '1', 1, 3) == 1)

assert(s.split('a b c d e f', '1') == ['a b c d e f'])
assert(s.split('a b c d e f', ' ') == ['a', 'b', 'c', 'd', 'e', 'f'])
assert(s.split('a b c d e f', ' ', 2) == ['a', 'b', 'c d e f'])
assert(s.split('a b c d e f', '') == ['a b c d e f'])
