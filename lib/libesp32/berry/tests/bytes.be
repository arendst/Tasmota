def assert_error(f, error_type)
    try
        f()
        assert(false, 'unexpected execution flow')
    except .. as e, m
        assert(e == error_type)
    end
end

#- basic initialization -#
b=bytes()
assert(str(b) == "bytes('')")
b=bytes("")
assert(str(b) == "bytes('')")
b=bytes(0)
assert(str(b) == "bytes('')")
b=bytes(1)
assert(str(b) == "bytes('')")
b=bytes(-1)
assert(str(b) == "bytes(\'00\')")
assert(b.size() == 1)

b=bytes("a")
assert(str(b) == "bytes('')")
b=bytes(3.5)
assert(str(b) == "bytes('')")
b=bytes([])
assert(str(b) == "bytes('')")

b=bytes("1122AAaaBBbb")
assert(str(b) == "bytes('1122AAAABBBB')")
assert(b.size() == 6)
b=bytes("112")
assert(str(b) == "bytes('11')")
b=bytes("++")
assert(str(b) == "bytes('00')")

#- add -#
b=bytes()
b.add(0x22)
assert(str(b) == "bytes('22')")
b.add(0x12345678, 0)
assert(str(b) == "bytes('22')")
b.add(0x12345678, 1)
assert(str(b) == "bytes('2278')")
b.add(0x12345678, 2)
assert(str(b) == "bytes('22787856')")
b.add(0x12345678, 4)
assert(str(b) == "bytes('2278785678563412')")
b.add(0x12345678, -1) #- big endian -#
assert(str(b) == "bytes('227878567856341278')")
b.add(0x12345678, -2)
assert(str(b) == "bytes('2278785678563412785678')")
b.add(0x12345678, -4)
assert(str(b) == "bytes('227878567856341278567812345678')")

#- get -#
b=bytes("000102030405")
assert(b.get(0) == 0)
assert(b.get(-1) == 0x05)   #- last byte -#
assert(b.get(6) == 0)
assert(b.get(1) == 1)
assert(b.get(5) == 5)
assert(b.get(-1000) == 0)   # out of range, default to zero
assert(b.get(1000) == 0)    # out of range, default to zero

assert(b.get(1,1) == 0x01)
assert(b.get(1,2) == 0x0201)
assert(b.get(1,4) == 0x04030201)
assert(b.get(1,-1) == 0x01)
assert(b.get(1,-2) == 0x0102) #- big endian -#
assert(b.get(1,-4) == 0x01020304)
assert(b.get(1,0) == 0)     # size zero is invalid, returns zero
assert(b.get(-1000,1) == 0) # out of range, default to zero
assert(b.get(1000,1) == 0)  # out of range, default to zero

#- resize -#
assert(bytes().size() == 0)
b=bytes("112233")
b.resize(2)
assert(str(b) == "bytes('1122')")
assert(b.size() == 2)
b.resize(4)
assert(str(b) == "bytes('11220000')")
assert(b.size() == 4)
b.resize(20)
assert(str(b) == "bytes('1122000000000000000000000000000000000000')")
assert(b.size() == 20)
b.resize(0)
assert(str(b) == "bytes('')")
assert(b.size() == 0)
b=bytes("112233")
b.resize(-5)    # resize negative is equivalent to resize(0)
assert(str(b) == "bytes('')")
assert(b.size() == 0)

#- clear -#
b=bytes("aabb")
b.clear()
assert(str(b) == "bytes('')")

#- == != -#
assert(bytes() == bytes())
assert(bytes("11") == bytes("11"))
assert(bytes("11") == bytes()..0x11)
assert(! (bytes("11") == bytes(0x12)) )
assert(! (bytes("11") == 0x11) )
assert(! (bytes("11") != bytes("11")) )
assert(bytes("11") != bytes("1122"))
assert(bytes("11") != bytes("12"))
assert(bytes("11") != bytes())

#- + -#
b1 = bytes("1122")
b2 = bytes("334455")
b = b1 + b2
assert(str(b1) == "bytes('1122')")
assert(str(b2) == "bytes('334455')")
assert(str(b) == "bytes('1122334455')")
b = b2 + b1
assert(str(b) == "bytes('3344551122')")

#- + for string -#
b1 = bytes("AA")
b = b1 + ''
assert(str(b) == "bytes('AA')")
b = b1 + '01'
assert(str(b) == "bytes('AA3031')")

#- .. and append as synonyms -#
b1 = bytes("1122")
b2 = bytes("334455")
b = b1..b2
assert(str(b1) == "bytes('1122334455')")
assert(str(b2) == "bytes('334455')")
assert(str(b) == "bytes('1122334455')")
#
b1 = bytes("1122")
b2 = bytes("334455")
b = b1.append(b2)
assert(str(b1) == "bytes('1122334455')")
assert(str(b2) == "bytes('334455')")
assert(str(b) == "bytes('1122334455')")

#- .. with string -#
b1 = bytes("AA")
b1 .. ''
assert(str(b1) == "bytes('AA')")
b1 .. '01'
assert(str(b1) == "bytes('AA3031')")
#
b1 = bytes("AA")
b1.append('')
assert(str(b1) == "bytes('AA')")
b1.append('01')
assert(str(b1) == "bytes('AA3031')")

#- appendhex -#
assert(bytes().appendhex(bytes("DEADBEEF")) == bytes("4445414442454546"))
assert(bytes("AABBCC").appendhex(bytes("DEADBEEF")) == bytes("AABBCC4445414442454546"))
assert(bytes("AABBCC").appendhex(bytes("")) == bytes("AABBCC"))

#- item -#
b = bytes("334455")
assert(b[0] == 0x33)
assert(b[1] == 0x44)
assert(b[2] == 0x55)
assert(b[-1] == 0x55)
assert(b[-2] == 0x44)
assert(b[-3] == 0x33)
# out of range raises "index_error" exceptions
assert_error(def () return b[-4] end, 'index_error')
assert_error(def () return b[4] end, 'index_error')

#- item range -#
b = bytes("00112233445566778899AABBCCDDEEFF")
assert(str(b[1..1]) =="bytes('11')")
assert(str(b[-1..1]) =="bytes('')")
assert(str(b[0..40]) =="bytes('00112233445566778899AABBCCDDEEFF')")
assert(str(b[1..0]) =="bytes('')")

#- copy -#
b=bytes("112233")
b2=b.copy()
assert(str(b) =="bytes('112233')")
assert(str(b2) =="bytes('112233')")
b2.clear()
assert(str(b) =="bytes('112233')")
assert(str(b2) =="bytes('')")

#- setitem -#
b=bytes("112233")
assert(str(b) =="bytes('112233')")
b[1]=0xAA
assert(str(b) =="bytes('11AA33')")
b[0]=0xBB
assert(str(b) =="bytes('BBAA33')")
b[2]=-1
assert(str(b) =="bytes('BBAAFF')")
# negative indices, counting from end
b[-1]=0xFE
assert(str(b) =="bytes('BBAAFE')")
b[-3]=0xBC
assert(str(b) =="bytes('BCAAFE')")
# out of range raises "index_error" exceptions
assert_error(def () b[-4]=0x11 end, 'index_error')
assert_error(def () b[4]=0x11 end, 'index_error')

#- resize -#
b=bytes()
b.resize(20)
assert(str(b) =="bytes('0000000000000000000000000000000000000000')")
b2=b.copy()
assert(str(b2) =="bytes('0000000000000000000000000000000000000000')")

#- asstring -#
b=bytes()
assert(b.asstring() == '')
b=bytes("334455")
assert(b.asstring() == '3DU')
b=bytes("33456502")
assert(b.asstring() == '3Ee\x02')

#- fromstring -#
b=bytes()
b.fromstring("Aa0")
assert(str(b) =="bytes('416130')")
b=bytes()
b.fromstring("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.")
assert(b.tostring(0) =="bytes('4C6F72656D20697073756D20646F6C6F722073697420616D65742C20636F6E73656374657475722061646970697363696E6720656C69742C2073656420646F20656975736D6F642074656D706F7220696E6369646964756E74207574206C61626F726520657420646F6C6F7265206D61676E6120616C697175612E')")
assert(size(bytes('4C6F72656D20697073756D20646F6C6F722073697420616D65742C20636F6E73656374657475722061646970697363696E6720656C69742C2073656420646F20656975736D6F642074656D706F7220696E6369646964756E74207574206C61626F726520657420646F6C6F7265206D61676E6120616C697175612E')) == 123)

#- negative index -#
b=bytes("112233")
assert(b[0] == 0x11)
assert(b[-1] == 0x33)   #- last element -#
assert(b[-3] == 0x11)

#- ranges with negative indices -#
b = bytes("00112233445566778899AABBCCDDEEFF")
assert(b[0..-1] == b)
assert(b[1..-2] == bytes("112233445566778899AABBCCDDEE"))
assert(b[5..10] == bytes("5566778899AA"))
assert(b[-10..-5] == bytes("66778899AABB"))
assert(b[5..-10] == bytes("5566"))
assert(b[7..-12] == bytes())

#- float -#
b = bytes("00000000")
b.setfloat(0, 0.33)
assert(b == bytes('C3F5A83E'))
b = bytes("0000C03F")
assert(b.getfloat(0) == 1.5)
b.addfloat(0.33)
assert(b == bytes("0000C03FC3F5A83E"))
b.addfloat(0.33, true)      #- Big Endian -#
assert(b == bytes("0000C03FC3F5A83E3EA8F5C3"))
b = bytes("")
b.addfloat(42)      #- add integer -#
assert(b == bytes("00002842"))

#- fromhex -#
b = bytes("112233")
b.fromhex("FFFEAABBCC")
assert(b == bytes("FFFEAABBCC"))
b.fromhex("")
assert(b == bytes())

#- tohex -#
b = bytes("FFFEAABBCC")
assert(b.tohex() == "FFFEAABBCC")
assert(bytes().tohex() == "")

# assign buffer to bytes
var a0 = bytes("112233445566778899")
b = bytes("aabbccddeeff")

a = a0.copy()
a.setbytes(0, b)            # assign from start
assert(a == bytes('AABBCCDDEEFF778899'))
a = a0.copy()
a.setbytes(0, b, 0, 0)      # zero len
assert(a == a0)
a = a0.copy()
a.setbytes(100, b)          # index out of range
assert(a == a0)
a = a0.copy()
a.setbytes(6, b)          # entire buffer not fitting
assert(a == bytes('112233445566AABBCC'))
a = a0.copy()
a.setbytes(6, b, 2, 2)
assert(a == bytes('112233445566CCDD99'))
a = b.copy()
a.setbytes(0, a0)
assert(a == bytes('112233445566'))

# reverse
assert(bytes().reverse() == bytes())
assert(bytes("AA").reverse() == bytes("AA"))
assert(bytes("1122334455").reverse() == bytes("5544332211"))
assert(bytes("11223344").reverse() == bytes("44332211"))

assert(bytes("0011223344").reverse(1) == bytes("0044332211"))
assert(bytes("0011223344").reverse(3) == bytes("0011224433"))
assert(bytes("0011223344").reverse(4) == bytes("0011223344"))
assert(bytes("0011223344").reverse(5) == bytes("0011223344"))
assert(bytes("0011223344").reverse(15) == bytes("0011223344"))
assert(bytes("0011223344").reverse(-2) == bytes("0011224433"))  # reverse starting 2 from end

assert(bytes("0011223344").reverse(1,3) == bytes("0033221144"))
assert(bytes("0011223344").reverse(1,0) == bytes("0011223344"))
assert(bytes("0011223344").reverse(2,2) == bytes("0011332244"))
assert(bytes("0011223344").reverse(0,2) == bytes("1100223344"))
assert(bytes("0011223344").reverse(nil,2) == bytes("1100223344"))
assert(bytes("0011223344").reverse(1, nil) == bytes("0044332211"))

assert(bytes("0011223344").reverse(nil, nil, 2) == bytes("2233001144"))
assert(bytes("001122334455").reverse(nil, nil, 3) == bytes("334455001122"))

# tobool returns `true` is bytes() is not null, `false` if empty
assert(bool(bytes()) == false)
assert(bytes().tobool() == false)
assert(!bytes())

assert(bool(bytes("00")) == true)
assert(bytes("01").tobool() == true)
assert(bytes("02"))

# retrieving 3-bytes little/big endian
a = bytes("01020304")
assert(a.get(1, 3) == 0x040302)
assert(a.get(1, -3) == 0x020304)

# append base64
b = bytes("AABBCC")
c = bytes("001122")
assert(bytes().fromstring(bytes("001122").tob64()) == bytes('41424569'))
assert(b.appendb64(c) == bytes("AABBCC41424569"))
assert(b.appendb64(bytes()) == bytes("AABBCC41424569"))

b = bytes("AABBCC")
assert(bytes().fromstring(bytes("1122").tob64()) == bytes('4553493D'))
assert(b.appendb64(c, 1) == bytes("AABBCC4553493D"))

b = bytes("AABBCC")
assert(bytes().fromstring(bytes("22").tob64()) == bytes('49673D3D'))
assert(b.appendb64(c, 2) == bytes("AABBCC49673D3D"))

b = bytes("AABBCC")
assert(bytes().fromstring(bytes("11").tob64()) == bytes('45513D3D'))
assert(b.appendb64(c, 1, 1) == bytes("AABBCC45513D3D"))

# bytes assign 3-byte values
b = bytes("1122")
assert(b.add(0x334455, 3) == bytes('1122554433'))
assert(b.add(0x334455, -3) == bytes('1122554433334455'))

# new type testing in set/add methods
b = bytes("0000000000")

assert_error(def () b.set(0, 0.5, 4) end, 'type_error')
assert_error(def () b.set(0, nil, 4) end, 'type_error')
assert_error(def () b.set(0, 1, nil) end, 'type_error')
assert_error(def () b.set(0, 1, 3.5) end, 'type_error')
assert_error(def () b.set(0, 'foo', 4) end, 'type_error')
assert_error(def () b.set(0, 4, 'foo') end, 'type_error')
assert_error(def () b.set(0, 0.5) end, 'type_error')
assert_error(def () b.set(0, nil) end, 'type_error')
assert_error(def () b.set(0, 'foo') end, 'type_error')
assert_error(def () b.set() end, 'type_error')

assert_error(def () b.add(0.5, 4) end, 'type_error')
assert_error(def () b.add(nil, 4) end, 'type_error')
assert_error(def () b.add(5, 1.5) end, 'type_error')
assert_error(def () b.add(5, nil) end, 'type_error')
assert_error(def () b.add('foo', 4) end, 'type_error')
assert_error(def () b.add(5, 'foo') end, 'type_error')
assert_error(def () b.add() end, 'type_error')

assert_error(def () b.addfloat(true) end, 'type_error')
assert_error(def () b.addfloat(nil) end, 'type_error')
assert_error(def () b.addfloat('foo') end, 'type_error')
assert_error(def () b.addfloat() end, 'type_error')

#- ------------------------------------------------------------ -#
#- getbits / setbits                                            -#
#- ------------------------------------------------------------ -#

#- getbits: basic reads within a single byte (LSB = bit 0) -#
b = bytes("AF000000")
assert(b.getbits(0, 4) == 0x0F)
assert(b.getbits(4, 4) == 0x0A)
assert(b.getbits(0, 8) == 0xAF)
assert(b.getbits(0, 1) == 1)
assert(b.getbits(7, 1) == 1)
assert(b.getbits(4, 1) == 0)        #- 0xAF = 1010_1111, bit4 = 0 -#
assert(b.getbits(5, 1) == 1)

#- getbits: reading zero bytes returns 0 -#
b = bytes("00000000")
assert(b.getbits(0, 8) == 0)
assert(b.getbits(0, 32) == 0)

#- getbits: spanning byte boundaries (byte0 is least significant) -#
b = bytes("3412")            #- bytes[0]=0x34, bytes[1]=0x12 -#
assert(b.getbits(0, 16) == 0x1234)
assert(b.getbits(0, 4) == 0x4)      #- low nibble of byte0 -#
assert(b.getbits(4, 8) == 0x23)     #- high nibble of byte0 + low nibble of byte1 -#
assert(b.getbits(8, 8) == 0x12)     #- byte1 -#
assert(b.getbits(12, 4) == 0x1)     #- high nibble of byte1 -#

#- getbits: full 32-bit read -#
b = bytes("78563412")
assert(b.getbits(0, 32) == 0x12345678)

#- getbits: reading past the end of the buffer yields zero bits -#
b = bytes("FF")
assert(b.getbits(0, 8) == 0xFF)
assert(b.getbits(0, 16) == 0x00FF)  #- second byte is out of range -> 0 -#
assert(b.getbits(8, 8) == 0)

#- getbits: length validation -#
assert(bytes("00").getbits(0, 0) == nil)        #- zero length returns nil (like get()) -#
assert_error(def () bytes("00").getbits(0, 33) end, 'value_error')
assert_error(def () bytes("00").getbits(0, -33) end, 'value_error')

#- getbits: argument type validation -#
assert_error(def () bytes("00").getbits() end, 'type_error')
assert_error(def () bytes("00").getbits(0) end, 'type_error')
assert_error(def () bytes("00").getbits('a', 4) end, 'type_error')
assert_error(def () bytes("00").getbits(0, 'a') end, 'type_error')
assert_error(def () bytes("00").getbits(0.5, 4) end, 'type_error')

#- setbits: basic writes within a single byte -#
b = bytes("00000000")
assert(b.setbits(0, 4, 0x0F) == bytes("0F000000"))
assert(b.setbits(4, 4, 0x0A) == bytes("AF000000"))

#- setbits: overwriting existing bits clears the old value -#
b = bytes("FF000000")
b.setbits(0, 4, 0x0)
assert(b == bytes("F0000000"))
b.setbits(4, 4, 0x0)
assert(b == bytes("00000000"))

#- setbits: spanning byte boundaries (little-endian byte order) -#
b = bytes("00000000")
b.setbits(4, 8, 0xAB)
assert(b == bytes("B00A0000"))      #- low nibble in byte0, high nibble in byte1 -#
assert(b.getbits(4, 8) == 0xAB)

b = bytes("00000000")
b.setbits(0, 16, 0x1234)
assert(b == bytes("34120000"))

#- setbits: full 32-bit write -#
b = bytes("00000000")
b.setbits(0, 32, 0x12345678)
assert(b == bytes("78563412"))
assert(b.getbits(0, 32) == 0x12345678)

#- setbits: only the targeted bits are modified, neighbours are preserved -#
b = bytes("FFFFFFFF")
b.setbits(4, 8, 0x00)
assert(b == bytes("0FF0FFFF"))
assert(b.getbits(0, 4) == 0x0F)     #- low nibble preserved -#
assert(b.getbits(4, 8) == 0x00)     #- cleared field -#

#- setbits: value larger than len_bits is masked to len_bits -#
b = bytes("00")
b.setbits(0, 4, 0xFF)               #- only low 4 bits should be written -#
assert(b == bytes("0F"))

#- setbits: writing zero bits leaves the buffer unchanged and returns self -#
b = bytes("1234")
assert(b.setbits(0, 0, 0xFF) == bytes("1234"))

#- setbits: accepts bool values (converted to int) -#
b = bytes("00")
b.setbits(0, 1, true)
assert(b.getbits(0, 1) == 1)
b.setbits(1, 1, false)
assert(b.getbits(1, 1) == 0)

#- setbits: returns self so calls can be chained -#
b = bytes("00000000")
assert(b.setbits(0, 4, 1).setbits(4, 4, 2) == bytes("21000000"))

#- setbits / getbits round-trip across a range of offsets and lengths -#
b = bytes("0000000000")
b.setbits(3, 12, 0xABC)
assert(b.getbits(3, 12) == 0xABC)
b.setbits(15, 17, 0x1FFFF)
assert(b.getbits(15, 17) == 0x1FFFF)
assert(b.getbits(3, 12) == 0xABC)   #- previous field untouched -#

#- setbits: length validation -#
assert(bytes("FF").setbits(0, 0, 0) == bytes("FF"))   #- zero length is a no-op -#
assert_error(def () bytes("00").setbits(0, 33, 0) end, 'value_error')
assert_error(def () bytes("00").setbits(0, -33, 0) end, 'value_error')

#- setbits: argument type validation -#
assert_error(def () bytes("00").setbits() end, 'type_error')
assert_error(def () bytes("00").setbits(0) end, 'type_error')
assert_error(def () bytes("00").setbits(0, 4) end, 'type_error')
assert_error(def () bytes("00").setbits('a', 4, 0) end, 'type_error')
assert_error(def () bytes("00").setbits(0, 'a', 0) end, 'type_error')
assert_error(def () bytes("00").setbits(0, 4, 'a') end, 'type_error')
assert_error(def () bytes("00").setbits(0, 4, nil) end, 'type_error')


#- ------------------------------------------------------------ -#
#- getbits / setbits — Big Endian support (PR berry-lang#537)   -#
#- ------------------------------------------------------------ -#

#- Single byte - endiannes does't matter -#
def getbit_and_setbit_roundtrip(byte, offset, length, val)
    # check for expected value within a predefined byte
    assert(byte.getbits(offset, length) == val)

    # set the value at a position, roundtrip and should return same value
    assert(bytes(-2).setbits(offset, length, val).getbits(offset, length) == val)
end

b = bytes("ABCD") # bytes constructor uses big endian, btw
assert(b.get(0) == 0xAB)    
assert(b.get(1) == 0xCD)
assert(b.get(0,-2) == 0xABCD)
# little endian
v=0x3;    o=0;   l= 2;   getbit_and_setbit_roundtrip(b, o, l, v)
v=0xB;    o=0;   l= 4;   getbit_and_setbit_roundtrip(b, o, l, v)
v=0xA;    o=4;   l= 4;   getbit_and_setbit_roundtrip(b, o, l, v)
v=0xAB;   o=0;   l= 8;   getbit_and_setbit_roundtrip(b, o, l, v)
v=0xDA;   o=4;   l= 8;   getbit_and_setbit_roundtrip(b, o, l, v)
v=0xDAB;  o=0;   l= 12;  getbit_and_setbit_roundtrip(b, o, l, v)
v=0xC;    o=12;  l= 4;   getbit_and_setbit_roundtrip(b, o, l, v)
v=0xCDAB; o=0;   l= 16;  getbit_and_setbit_roundtrip(b, o, l, v)
# big endian
v=0xB;    o=4;   l= -4;  getbit_and_setbit_roundtrip(b, o, l, v)
v=0xA;    o=0;   l= -4;  getbit_and_setbit_roundtrip(b, o, l, v)
v=0xAB;   o=0;   l= -8;  getbit_and_setbit_roundtrip(b, o, l, v)
v=0xBC;   o=4;   l= -8;  getbit_and_setbit_roundtrip(b, o, l, v)
v=0xABC;  o=0;   l= -12; getbit_and_setbit_roundtrip(b, o, l, v)
v=0xD;    o=12;  l= -4;  getbit_and_setbit_roundtrip(b, o, l, v)
v=0xABCD; o=0;   l= -16; getbit_and_setbit_roundtrip(b, o, l, v)


#- LE: read from known pattern -#
b = bytes("A55A")
assert(b.getbits(0, 16) == 0x5AA5)

#- BE: read from known pattern -#
b = bytes("A55A")
assert(b.getbits(0, -16) == 0xA55A)


#- setbit - input value read/write -#
# we always truncate input value according to big endian - so in both cases we will see alteration of A and B nibbles in the output
b = bytes(-3)
b.setbits(0, 8, 0xDEADAB)
assert(b == bytes("AB0000"))
# endiannes doesn't matter for a single contiguous byte:
b = bytes(-3)
b.setbits(0, -8, 0xBEEFAB)
assert(b == bytes("AB0000"))
# endiannes and offset only influences the output
b = bytes(-3)
b.setbits(4, 8, 0xBEEFAB)
assert(b == bytes("B00A00")) # write down bytes vertically if it doesn't make sense ;)
b = bytes(-3)
b.setbits(4, -8, 0xBEEFAB)
assert(b == bytes("0AB000"))

#- setbit - input longer than length - partial byte-#
b = bytes("0000")
b.setbits(0, 4, 0xFFFF)
assert(b == bytes("0F00"))
assert(b.getbits(0, 4) == 0xF)


#- LE: single-bit set -#
b = bytes("5A")
b.setbits(0, 1, 1)
assert(b == bytes("5B"))        #- LSB 0→1 -#
b = bytes("5A")
b.setbits(7, 1, 1)
assert(b == bytes("DA"))        #- MSB 0→1 -#
b.setbits(7, 1, 0)
assert(b == bytes("5A"))        #- MSB 1→0 -#

#- BE: single-bit -#
b = bytes("5A")
b.setbits(0, -1, 1)
assert(b == bytes("DA"))        # MSB-0 bit 0 (phys 7) 0→1
b = bytes("5A")
b.setbits(7, -1, 1)
assert(b == bytes("5B"))        # MSB-0 bit 7 (phys 0) 0→1
b.setbits(7, -1, 0)
assert(b == bytes("5A"))


#- LE: partial middle of byte -#
b = bytes("00")
b.setbits(2, 3, 5)              #- 5 = 101b at bits 2,3,4 -#
assert(b == bytes("14"))        #- 0001_0100 -#
assert(b.getbits(2, 3) == 5)

b = bytes("FF")
b.setbits(1, 2, 0)
assert(b == bytes("F9"))        #- clear bits 1,2 → 1111_1001 -#
assert(b.getbits(1, 2) == 0)

#- BE: partial middle of byte -#
b = bytes("00")
b.setbits(1, -3, 5)
assert(b == bytes("50"))        # 5 = 101b at bits 1,2,3 → phys 6,5,4 = 0101_0000
assert(b.getbits(1, -3) == 5)

b = bytes("FF")
b.setbits(1, -2, 0)
assert(b == bytes("9F"))        # clear bits 1,2 (phys 6,5) → 1001_1111
assert(b.getbits(1, -2) == 0)


#- LE: full byte -#
b = bytes("00")
b.setbits(0, 8, 0x5A)
assert(b == bytes("5A"))
b.setbits(0, 8, 0xA5)
assert(b == bytes("A5"))

#- BE: full byte -#
b = bytes("00")
b.setbits(0, -8, 0x5A)
assert(b == bytes("5A"))
b.setbits(0, -8, 0xA5)
assert(b == bytes("A5"))


#- LE: multi-byte -#
b = bytes("0000")
b.setbits(0, 16, 0xA55A)
assert(b == bytes("5AA5"))      #- LE wire order: LSB first -#
b = bytes("0000")
b.setbits(8, 8, 0x5A)
assert(b == bytes("005A"))

#- BE multi-byte = MSB-first byte order (opposite of LE) -#
b = bytes("0000")
b.setbits(0, -16, 0xA55A)
assert(b == bytes("A55A"))      # BE wire order: MSB first 
b = bytes("0000")
b.setbits(8, -8, 0x5A)
assert(b == bytes("005A"))


#- LE: cross-byte partial  -#
b = bytes("0000")
b.setbits(4, 10, 0xFFF)
assert(b == bytes("F03F"))      #- byte0 bits 4-7 = 0xF, byte1 bits 0-5 = 0x3F -#
assert(b.getbits(4, 10) == 0x3FF)


#- BE: cross-byte partial -#
b = bytes("0000")
b.setbits(4, -10, 0xFFF)
assert(b == bytes("0FFC"))      # byte0 bits 4-7 = 0x0F, byte1 bits 0-5 = 0xFC
assert(b.getbits(4, -10) == 0x3FF)

#- mixing LE/BE -#
b = bytes(-3)
b.setbits(0, -12, 0xABC)
b.setbits(12, 12, 0xDEF) # F corrupts C nibble, one nibble empty
assert(b == bytes("ABF0DE")) 
b = bytes(-4) # let's try again
b.setbits(0, -12, 0xABC)
b.setbits(16, 12, 0xDEF) # we need to start in byte 2
assert(b == bytes("ABC0EF0D"))


# length check
assert(bytes("FF").getbits(0, 0) == nil)
assert(bytes("FF").setbits(0, 0, 0) == bytes("FF"))
assert_error(def () bytes("00").getbits(0, 33) end, 'value_error')
assert_error(def () bytes("00").setbits(0, 33, 0) end, 'value_error')
assert_error(def () bytes("00").getbits(0, -33) end, 'value_error')
assert_error(def () bytes("00").setbits(0, -33, 0) end, 'value_error')
