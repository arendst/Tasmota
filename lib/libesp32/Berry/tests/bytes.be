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
assert(str(b) == "bytes('')")
assert(b.size() == 0)

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
assert(b.get(-1) == 0)  #- could consider nil as well -#
assert(b.get(6) == 0)  #- could consider nil as well -#
assert(b.get(1) == 1)
assert(b.get(5) == 5)

assert(b.get(1,0) == nil)
assert(b.get(1,1) == 0x01)
assert(b.get(1,2) == 0x0201)
assert(b.get(1,4) == 0x04030201)
assert(b.get(1,-1) == 0x01)
assert(b.get(1,-2) == 0x0102) #- big endian -#
assert(b.get(1,-4) == 0x01020304)

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

#- .. -#
b1 = bytes("1122")
b2 = bytes("334455")
b = b1..b2
assert(str(b1) == "bytes('1122334455')")
assert(str(b2) == "bytes('334455')")
assert(str(b) == "bytes('1122334455')")

#- item -#
b = bytes("334455")
assert(b[0] == 0x33)
assert(b[1] == 0x44)
assert(b[2] == 0x55)

#- item range -#
b = bytes("00112233445566778899AABBCCDDEEFF")
assert(str(b[1..1]) =="bytes('11')")
assert(str(b[-11..1]) =="bytes('0011')")
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
assert(str(b) =="bytes('4C6F72656D20697073756D20646F6C6F722073697420616D65742C20636F6E73656374657475722061646970697363696E6720656C69742C2073656420646F20656975736D6F642074656D706F7220696E6369646964756E74207574206C61626F726520657420646F6C6F7265206D61676E6120616C697175612E')")
