import os

# os.path.join test
assert(os.path.join('') == '')
assert(os.path.join('abc', 'de') == 'abc/de')
assert(os.path.join('abc', '/de') == '/de')
assert(os.path.join('a', 'de') == 'a/de')
assert(os.path.join('abc/', 'de') == 'abc/de')
assert(os.path.join('abc', 'de', '') == 'abc/de/')
assert(os.path.join('abc', '', '', 'de') == 'abc/de')
assert(os.path.join('abc', '/de', 'fghij') == '/de/fghij')
assert(os.path.join('abc', 'xyz', '/de', 'fghij') == '/de/fghij')

# os.path.split test
def split(str, list)
    var res = os.path.split(str)
    assert(res[0] == list[0] && res[1] == list[1],
        'unexpected results: ' .. res .. ', reference value: ' .. list)
end

split('/', ['/', ''])
split('//', ['//', ''])
split('///', ['///', ''])
split('a/', ['a', ''])
split('a//', ['a', ''])
split('a/b/c', ['a/b', 'c'])
split('a/b/', ['a/b', ''])
split('a//b//', ['a//b', ''])
split('a/../b', ['a/..', 'b'])
split('abcd////ef/////', ['abcd////ef', ''])
split('abcd////ef', ['abcd', 'ef'])

# os.path.splitext test
def splitext(str, list)
    var res = os.path.splitext(str)
    assert(res[0] == list[0] && res[1] == list[1],
        'unexpected results: ' .. res .. ', reference value: ' .. list)
end

splitext('a.b', ['a', '.b'])
splitext('a..b', ['a.', '.b'])
splitext('/a..b', ['/a.', '.b'])
splitext('/.b', ['/.b', ''])
splitext('/..b', ['/..b', ''])
splitext('..b', ['..b', ''])
splitext('...b', ['...b', ''])
splitext('.b', ['.b', ''])
splitext('ac..b', ['ac.', '.b'])
splitext('ac.b', ['ac', '.b'])
splitext('ac/.b', ['ac/.b', ''])
splitext('ac/..b', ['ac/..b', ''])
