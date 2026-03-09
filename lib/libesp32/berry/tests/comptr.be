# test about comptr
import introspect

var p = introspect.toptr(1024)
assert(str(p) == '<ptr: 0x400>')

p += 1
assert(p == introspect.toptr(1025))

p -= 2
assert(p == introspect.toptr(1023))

# use comptr[idx] to read or write bytes
var b = bytes("11223344")
p = b._buffer()                 # p is comptr
assert(p[0] == 0x11)
assert(p[1] == 0x22)
assert(p[2] == 0x33)
assert(p[3] == 0x44)

p[0] = 0xFF
p[1] = 0x55
p[2] = 0xFEBC                   # shoud truncate to 0xBC
assert(b == bytes("FF55BC44"))
assert(p[0] == 255)             # check it's unsigned
