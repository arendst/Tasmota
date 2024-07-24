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
