# test regex from re1.5
import re

# standard use of lib
assert(re.search("a.*?b(z+)", "zaaaabbbccbbzzzee")  ==  ['aaaabbbccbbzzz', 'zzz'])
assert(re.searchall('<([a-zA-Z]+)>', '<abc> yeah <xyz>')  ==  [['<abc>', 'abc'], ['<xyz>', 'xyz']])

assert(re.match("a.*?b(z+)", "aaaabbbccbbzzzee")  ==  ['aaaabbbccbbzzz', 'zzz'])
assert(re.match2("a.*?b(z+)", "aaaabbbccbbzzzee")  ==  [14, 'zzz'])
assert(re.matchall('<([a-zA-Z]+)>', '<abc> yeah <xyz>')  ==  [['<abc>', 'abc']])
assert(re.matchall('<([a-zA-Z]+)>', '<abc><xyz>')  ==  [['<abc>', 'abc'], ['<xyz>', 'xyz']])
assert(re.split('/', "foo/bar//baz")  ==  ['foo', 'bar', '', 'baz'])

# pre-compile
var rr
rr = re.compile("a.*?b(z+)")
assert(rr.search("zaaaabbbccbbzzzee")  ==  ['aaaabbbccbbzzz', 'zzz'])
rr = re.compile('<([a-zA-Z]+)>')
assert(rr.searchall('<abc> yeah <xyz>')  ==  [['<abc>', 'abc'], ['<xyz>', 'xyz']])

rr = re.compile("a.*?b(z+)")
assert(rr.match("aaaabbbccbbzzzee")  ==  ['aaaabbbccbbzzz', 'zzz'])
assert(rr.match2("aaaabbbccbbzzzee")  ==  [14, 'zzz'])
rr = re.compile('<([a-zA-Z]+)>')
assert(rr.matchall('<abc> yeah <xyz>')  ==  [['<abc>', 'abc']])
assert(rr.matchall('<abc><xyz>')  ==  [['<abc>', 'abc'], ['<xyz>', 'xyz']])
rr = re.compile('/')
assert(rr.split("foo/bar//baz")  ==  ['foo', 'bar', '', 'baz'])

# compile to bytes
var rb
rb = re.compilebytes("a.*?b(z+)")
assert(re.search(rb, "zaaaabbbccbbzzzee")  ==  ['aaaabbbccbbzzz', 'zzz'])
assert(rb == bytes('1B0000000F0000000100000062030260FB7E00016162030260FB01627E02017A62FC7E037E017F'))

rb = re.compilebytes('<([a-zA-Z]+)>')
assert(re.searchall(rb, '<abc> yeah <xyz>')  ==  [['<abc>', 'abc'], ['<xyz>', 'xyz']])
assert(rb == bytes('1A0000000C0000000100000062030260FB7E00013C7E020302617A415A62F87E03013E7E017F'))

rb = re.compilebytes("a.*?b(z+)")
assert(re.match(rb, "aaaabbbccbbzzzee")  ==  ['aaaabbbccbbzzz', 'zzz'])
assert(re.match2(rb, "aaaabbbccbbzzzee")  ==  [14, 'zzz'])
assert(rb == bytes('1B0000000F0000000100000062030260FB7E00016162030260FB01627E02017A62FC7E037E017F'))

rb = re.compilebytes('<([a-zA-Z]+)>')
assert(re.matchall(rb, '<abc> yeah <xyz>')  ==  [['<abc>', 'abc']])
assert(re.matchall(rb, '<abc><xyz>')  ==  [['<abc>', 'abc'], ['<xyz>', 'xyz']])
assert(rb == bytes('1A0000000C0000000100000062030260FB7E00013C7E020302617A415A62F87E03013E7E017F'))

rb = re.compilebytes('/')
assert(re.split(rb, "foo/bar//baz")  ==  ['foo', 'bar', '', 'baz'])
assert(rb == bytes('0C000000070000000000000062030260FB7E00012F7E017F'))
