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
def split(st, lst)
    var res = os.path.split(st)
    assert(res[0] == lst[0] && res[1] == lst[1],
        'unexpected results: ' .. res .. ', reference value: ' .. lst)
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
def splitext(st, lst)
    var res = os.path.splitext(st)
    assert(res[0] == lst[0] && res[1] == lst[1],
        'unexpected results: ' .. res .. ', reference value: ' .. lst)
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
