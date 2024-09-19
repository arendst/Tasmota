# tests for int64 support (for int32 internal int representation)

assert(int(int64()) == 0)
assert(str(int64()) == "0")
assert(int(int64(10)) == 10)
assert(str(int64(10)) == "10")
assert(int(int64(-5)) == -5)
assert(str(int64(-5)) == "-5")

# extended constructor
assert(int64(35).tostring() == "35")
assert(int64(3.5).tostring() == "3")
assert(int64("35").tostring() == "35")
assert(int64("20000000000").tostring() == "20000000000")
assert(int64(true).tostring() == "1")
assert(int64(false).tostring() == "0")
assert(int64(int64(42)).tostring() == "42")
# invalid
assert(int64("").tostring() == "0")
assert(int64(nil).tostring() == "0")

# testing large numbers
assert(str(int64.fromu32(0xFFFFFFFF, 0x7FFFFFFF)) == "9223372036854775807")     # max positive number
assert(str(int64.fromu32(0x00000000, 0x80000000)) == "-9223372036854775808")
assert(str(int64.fromu32(10,10)) == "42949672970")

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
a = int64.fromu32(0xFFFFFFFF, 0x7FFFFFFF)     # max positive number
assert(a.tobytes() == bytes("FFFFFFFFFFFFFF7F"))
a = int64.fromu32(0x00000000, 0x80000000)
assert(a.tobytes() == bytes("0000000000000080"))
assert(int64(-1).tobytes() == bytes("FFFFFFFFFFFFFFFF"))

# frombytes
assert(int64.frombytes(bytes("0A00000000000000"), 0) == bytes("0A00000000000000"))          # with implicit index 0
assert(int64.frombytes(bytes("0A00000000000000"))    == bytes("0A00000000000000"))
assert(int64.frombytes(bytes("0A00000000000000"), 1) == bytes("0000000000000000"))          # index 1 and incomplete (7 bytes)

assert(int64.frombytes(bytes("00FFFFFFFFFFFFFFFF"), 1) == bytes("FFFFFFFFFFFFFFFF"))        # index 1 and incomplete (7 bytes)
assert(int64.frombytes(bytes("00FFFFFFFFFFFFFFFF"), -2) == bytes("FFFF000000000000"))       # from end
assert(int64.frombytes(bytes("")) == bytes("0000000000000000"))                             # empty
assert(int64.frombytes(bytes(""),4) == bytes("0000000000000000"))                           # empty with wrong index

# fromu32
assert(int64.fromu32(0).tobytes() == bytes("0000000000000000"))
assert(int64.fromu32(0xFFFFFFFF).tobytes() == bytes("FFFFFFFF00000000"))
assert(int64.fromu32(0xFFFFFFFF, 1).tobytes() == bytes("FFFFFFFF01000000"))
assert(int64.fromu32(-1, 1).tobytes() == bytes("FFFFFFFF01000000"))
assert(int64.fromu32(-1, -1).tobytes() == bytes("FFFFFFFFFFFFFFFF"))

# fromfloat
assert(int64.fromfloat(3.5).tostring() == "3")
assert(int64.fromfloat(-3.5).tostring() == "-3")
assert(int64.fromfloat(2e10).tostring() == "20000000000")   # 20000000000 does not fit in 32 bits

# toint64, supports int, float, bool, string, int64
assert(int64.toint64(35).tostring() == "35")
assert(int64.toint64(3.5).tostring() == "3")
assert(int64.toint64("35").tostring() == "35")
assert(int64.toint64("20000000000").tostring() == "20000000000")
assert(int64.toint64(true).tostring() == "1")
assert(int64.toint64(false).tostring() == "0")
assert(int64.toint64(int64(42)).tostring() == "42")
# invalid
assert(int64.toint64("").tostring() == "0")
assert(int64.toint64(nil) == nil)
