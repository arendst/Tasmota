#- test for bytes with fixed sizes -#
def assert_attribute_error(f)
    try
        f()
        assert(false, 'unexpected execution flow')
    except .. as e, m
        assert(e == 'attribute_error')
    end
end

#- simple initialization -#
b=bytes(-1)
assert(str(b) == "bytes('00')")
b=bytes(-5)
assert(str(b) == "bytes('0000000000')")

#- initialization with hex -#
b=bytes("11", -1)
assert(str(b) == "bytes('11')")
b=bytes("11", -5)
assert(str(b) == "bytes('1100000000')")
b=bytes("11223344", -4)
assert(str(b) == "bytes('11223344')")
assert(str(bytes(-3).fromb64('RFVm')) == "bytes('445566')")

#- check that get and set still works -#
b=bytes("11223344",-4)
assert(b.get(0,2) == 0x2211)
b.set(0,0x5566,2)
assert(b.get(0,2) == 0x5566)
assert(b[0] == 0x66)
b[0]=12
assert(b[0] == 12)

#- resize -#
b=bytes("11223344",-4)
assert(b.resize(4) == bytes('11223344'))

#- check that changing size raises an exception -#
b=bytes("112233", -3)
assert_attribute_error(/-> b.add(1))
assert_attribute_error(/-> b.add(2,2))
assert_attribute_error(/-> b.resize(4))
assert_attribute_error(/-> b.fromstring("aaaaa"))
assert_attribute_error(/-> b.fromb64('RFVmdw=='))
assert_attribute_error(/-> b.clear())
assert_attribute_error(/-> b.bytes())

#- add -#
b=bytes("112233", -3)
assert(b+b == bytes("112233112233"))

#- bytes mapped -#
b1=bytes("112233445566")
ptr=b1._buffer()
b2=bytes(ptr, -4)
assert(b2 == bytes("11223344"))
b2=bytes(ptr, 4)
assert(b2 == bytes("11223344"))
b2=bytes(ptr, 6)
assert(b2 == bytes("112233445566"))
assert(b1 == b2)

b2.set(0,0xAABB,2)
assert(b2 == bytes("BBAA33445566"))
assert(b1 == bytes("BBAA33445566"))
assert(b1 == b2)
assert(b1.ismapped() == false)