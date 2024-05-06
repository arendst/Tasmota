#
# Matter_TLV.be - implements the encoding and decoding of Matter TLV structures (Tag/Lenght/Value) Appendix A.
#
# Copyright (C) 2023  Stephan Hadinger & Theo Arends
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# Support for Matter Protocol: TLV encoding and decoding

# Test
import matter

def test_TLV(b, s)
  var m = matter.TLV.parse(b)
  assert(m.tostring() == s)
  assert(m.tlv2raw() == b)
  assert(m.encode_len() == size(b))
end

test_TLV(bytes("2502054C"), "2 = 19461U")
test_TLV(bytes("0001"), "1")
test_TLV(bytes("08"), "false")
test_TLV(bytes("09"), "true")

var TLV = matter.TLV
assert(TLV.create_TLV(TLV.BOOL, 1).tlv2raw() == bytes("09"))
assert(TLV.create_TLV(TLV.BOOL, true).tlv2raw() == bytes("09"))
assert(TLV.create_TLV(TLV.BOOL, 0).tlv2raw() == bytes("08"))
assert(TLV.create_TLV(TLV.BOOL, false).tlv2raw() == bytes("08"))

test_TLV(bytes("00FF"), "-1")
test_TLV(bytes("05FFFF"), "65535U")

test_TLV(bytes("0A0000C03F"), "1.5")
test_TLV(bytes("0C06466f6f626172"), '"Foobar"')
test_TLV(bytes("1006466f6f626172"), "466F6F626172")
test_TLV(bytes("e4f1ffeddeedfe55aa2a"), "0xFFF1::0xDEED:0xAA55FEED = 42U")
test_TLV(bytes("300120D2DAEE8760C9B1D1B25E0E2E4DD6ECA8AEF6193C0203761356FCB06BBEDD7D66"), "1 = D2DAEE8760C9B1D1B25E0E2E4DD6ECA8AEF6193C0203761356FCB06BBEDD7D66")

# context specific
test_TLV(bytes("24012a"), "1 = 42U")
test_TLV(bytes("4401002a"), "Matter::0x00000001 = 42U")

# int64
test_TLV(bytes("030102000000000000"), "513")
test_TLV(bytes("070102000000000000"), "513U")
test_TLV(bytes("03FFFFFFFFFFFFFFFF"), "-1")
test_TLV(bytes("07FFFFFFFFFFFFFF7F"), "9223372036854775807U")

# structure
test_TLV(bytes("1518"), "{}")
test_TLV(bytes("15300120D2DAEE8760C9B1D1B25E0E2E4DD6ECA8AEF6193C0203761356FCB06BBEDD7D662502054C240300280418"), "{1 = D2DAEE8760C9B1D1B25E0E2E4DD6ECA8AEF6193C0203761356FCB06BBEDD7D66, 2 = 19461U, 3 = 0U, 4 = false}")
test_TLV(bytes("15300120D2DAEE8760C9B1D1B25E0E2E4DD6ECA8AEF6193C0203761356FCB06BBEDD7D662502054C240300280435052501881325022C011818"), "{1 = D2DAEE8760C9B1D1B25E0E2E4DD6ECA8AEF6193C0203761356FCB06BBEDD7D66, 2 = 19461U, 3 = 0U, 4 = false, 5 = {1 = 5000U, 2 = 300U}}")

# list
test_TLV(bytes("1718"), "[[]]")
test_TLV(bytes("17000120002a000200032000ef18"), "[[1, 0 = 42, 2, 3, 0 = -17]]")

# array
test_TLV(bytes("1618"), "[]")
test_TLV(bytes("160000000100020003000418"), "[0, 1, 2, 3, 4]")

# mix
test_TLV(bytes("16002a02f067fdff15180a33338f410c0648656c6c6f2118"), '[42, -170000, {}, 17.9, "Hello!"]')
test_TLV(bytes("153600172403312504FCFF18172402002403302404001817240200240330240401181724020024033024040218172402002403302404031817240200240328240402181724020024032824040418172403312404031818280324FF0118"), '{0 = [[[3 = 49U, 4 = 65532U]], [[2 = 0U, 3 = 48U, 4 = 0U]], [[2 = 0U, 3 = 48U, 4 = 1U]], [[2 = 0U, 3 = 48U, 4 = 2U]], [[2 = 0U, 3 = 48U, 4 = 3U]], [[2 = 0U, 3 = 40U, 4 = 2U]], [[2 = 0U, 3 = 40U, 4 = 4U]], [[3 = 49U, 4 = 3U]]], 3 = false, 255 = 1U}')
