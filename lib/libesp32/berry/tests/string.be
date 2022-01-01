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

assert(s.format("%%") == "%")
assert(s.format("%i%%", 12) == "12%")
assert(s.format("%i%%%i", 12, 13) == "12%13")
assert(s.format("%s%%", "foo") == "foo%")
assert(s.format("%.1f%%", 3.5) == "3.5%")

s="azerty"
assert(s[1..2] == "ze")
assert(s[1..] == "zerty")
assert(s[1..-1] == "zerty")

#- string ranges -#
s="azertyuiop"
assert(s[0] == "a")
assert(s[0..1] == "az")
assert(s[0..2] == "aze")
assert(s[0..10] == s)
assert(s[0..size(s)] == s)
assert(s[0..50] == s)       #- upper limit is allowed to be out of range -#

#- negative indices start from the end -#
s="azertyuiop"
assert(s[-1] == "p")
assert(s[-2] == "o")
assert(s[0..-2] == "azertyuio")
assert(s[-4..-2] == "uio")
assert(s[-2..-4] == "")  #- if range is in wrong order, returns empty string -#
assert(s[-40..-2] == "azertyuio")  #- borders are allowed to be out of range -#

# escape
import string
assert(string.escape("A") == '"A"')
assert(string.escape("A", true) == "'A'")
assert(string.escape("\"") == '"\\""')
assert(string.escape("\"", true) == '\'"\'')

var s ='"a\'b"\''
assert(string.escape(s) == '"\\"a\'b\\"\'"')
assert(string.escape(s, true) == '\'"a\\\'b"\\\'\'')

# tr
assert(string.tr("azer", "abcde", "ABCDE") == 'AzEr')
assert(string.tr("azer", "", "") == 'azer')
assert(string.tr("azer", "aaa", "ABC") == 'Azer')  # only first match works
assert(string.tr("A_b", "_", " ") == 'A b')
# tr used to remove characters
assert(string.tr("qwerty", "qwe", "_") == '_rty')

# the following should not crash
var s1 = 'A string of more than 128 bytes 012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789'
var s2 = string.format("%i, %s", 1, s1)
