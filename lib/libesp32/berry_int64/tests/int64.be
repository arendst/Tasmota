# tests for int64 support (for int32 internal int representation)

assert(int(int64()) == 0)
assert(str(int64()) == "0")
assert(int(int64(10)) == 10)
assert(str(int64(10)) == "10")
assert(int(int64(-5)) == -5)
assert(str(int64(-5)) == "-5")

# testing large numbers
a = int64()
a.set(0x7FFFFFFF,0xFFFFFFFF)     # max positive number
assert(str(a) == "9223372036854775807")

a.set(0x80000000,0x00000000)
assert(str(a) == "-9223372036854775808")

a.set(10,10)
assert(str(a) == "42949672970")

# addition
assert(str(int64(10) + int64(20)) == "30")
assert(str(int64(10) + int64(-20)) == "-10")
assert(str(int64() + int64()) == "0")
assert(str(int64(10) + 0) == "10")      # special case, do not crash if null

# substraction
assert(str(int64(10) - int64(20)) == "-10")
assert(str(int64(10) - int64(-20)) == "30")
assert(str(int64() - int64()) == "0")
assert(str(int64(10) - 0) == "10")      # special case, do not crash if null

# neg
assert(str(-int64(10)) == "-10")
assert(str(-int64(-10)) == "10")
assert(str(-int64()) == "0")

# multiplication
assert(str(int64(10) * int64(20)) == "200")
assert(str(int64(10) * int64(-20)) == "-200")
assert(str(int64() * int64()) == "0")
assert(str(int64(10) * 0) == "0")      # special case, do not crash if null

# division
assert(str(int64(100) / int64(20)) == "5")
assert(str(int64(100) / int64(-20)) == "-5")

# modulus
assert(str(int64(102) % int64(10)) == "2")

# equals
assert((int64(10) == int64(10)) == true)
assert((int64(10) == int64(20)) == false)
assert((int64(10) == int64()) == false)
assert((int64(0) == int64()) == true)
assert((int64(0) == 0) == true)
assert((int64(10) == 0) == false)

# unequals
assert((int64(10) != int64(10)) == false)
assert((int64(10) != int64(20)) == true)
assert((int64(10) != int64()) == true)
assert((int64(0) != int64()) == false)
assert((int64(0) != 0) == false)
assert((int64(10) != 0) == true)

# >
assert((int64(10) > int64(10)) == false)
assert((int64(10) > int64(20)) == false)
assert((int64(20) > int64(10)) == true)
assert((int64(10) > 0) == true)
# >=
assert((int64(10) >= int64(10)) == true)
assert((int64(10) >= int64(20)) == false)
assert((int64(20) >= int64(10)) == true)
assert((int64(10) >= 0) == true)

# <
assert((int64(10) < int64(10)) == false)
assert((int64(10) < int64(20)) == true)
assert((int64(20) < int64(10)) == false)
assert((int64(10) < 0) == false)
# <=
assert((int64(10) <= int64(10)) == true)
assert((int64(10) <= int64(20)) == true)
assert((int64(20) <= int64(10)) == false)
assert((int64(10) <= 0) == false)

# tobytes
assert(int64().tobytes() == bytes("0000000000000000"))
assert(int64(10).tobytes() == bytes("0A00000000000000"))
a = int64()
a.set(0x7FFFFFFF,0xFFFFFFFF)     # max positive number
assert(a.tobytes() == bytes("FFFFFFFFFFFFFF7F"))
a.set(0x80000000,0x00000000)
assert(a.tobytes() == bytes("0000000000000080"))
assert(int64(-1).tobytes() == bytes("FFFFFFFFFFFFFFFF"))

# frombytes
a = int64()
a.frombytes(bytes("0A00000000000000"), 0)
assert(a.tobytes() == bytes("0A00000000000000"))
a.frombytes(bytes("0A00000000000000"))                  # with implicit index 0
assert(a.tobytes() == bytes("0A00000000000000"))
a.frombytes(bytes("0A00000000000000"), 1)               # index 1 and incomplete (7 bytes)
assert(a.tobytes() == bytes("0000000000000000"))

a.frombytes(bytes("00FFFFFFFFFFFFFFFF"), 1)             # index 1 and incomplete (7 bytes)
assert(a.tobytes() == bytes("FFFFFFFFFFFFFFFF"))
a.frombytes(bytes("00FFFFFFFFFFFFFFFF"), -2)            # from end
assert(a.tobytes() == bytes("FFFF000000000000"))
a.frombytes(bytes(""))                                  # empty
assert(a.tobytes() == bytes("0000000000000000"))
a.frombytes(bytes(""),4)                                # empty with wrong index
assert(a.tobytes() == bytes("0000000000000000"))
