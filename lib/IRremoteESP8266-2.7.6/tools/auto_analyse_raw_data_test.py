#!/usr/bin/python3
"""Unit tests for auto_analyse_raw_data.py"""
from io import StringIO
import unittest
import auto_analyse_raw_data as analyse

# pylint: disable=too-many-lines

class TestRawIRMessage(unittest.TestCase):
  """Unit tests for the RawIRMessage class."""

  # pylint: disable=too-many-public-methods

  def test_display_binary(self):
    """Test the display_binary() method."""
    output = StringIO()
    message = analyse.RawIRMessage(100, [8000, 4000, 500, 500, 500], output,
                                   False)
    self.assertEqual(output.getvalue(), '')
    message.display_binary("10101010")
    message.display_binary("0000000000000000")
    message.display_binary("00010010001101000101011001111000")
    self.assertEqual(
        output.getvalue(), '\n'
        '  Bits: 8\n'
        '  Hex:  0xAA (MSB first)\n'
        '        0x55 (LSB first)\n'
        '  Dec:  170 (MSB first)\n'
        '        85 (LSB first)\n'
        '  Bin:  0b10101010 (MSB first)\n'
        '        0b01010101 (LSB first)\n'
        '\n'
        '  Bits: 16\n'
        '  Hex:  0x0000 (MSB first)\n'
        '        0x0000 (LSB first)\n'
        '  Dec:  0 (MSB first)\n'
        '        0 (LSB first)\n'
        '  Bin:  0b0000000000000000 (MSB first)\n'
        '        0b0000000000000000 (LSB first)\n'
        '\n'
        '  Bits: 32\n'
        '  Hex:  0x12345678 (MSB first)\n'
        '        0x1E6A2C48 (LSB first)\n'
        '  Dec:  305419896 (MSB first)\n'
        '        510274632 (LSB first)\n'
        '  Bin:  0b00010010001101000101011001111000 (MSB first)\n'
        '        0b00011110011010100010110001001000 (LSB first)\n')


class TestAutoAnalyseRawData(unittest.TestCase):
  """Unit tests for the functions in AutoAnalyseRawData."""

  # pylint: disable=too-many-public-methods

  def test_avg_list(self):
    """Tests for the avg_list method."""

    self.assertEqual(0, analyse.avg_list([]))
    self.assertEqual(23, analyse.avg_list([10, 20, 40]))

  def test_dump_constants_simple(self):
    """Simple tests for the dump_constants() function."""
    ignore = StringIO()
    output = StringIO()
    defs = []
    message = analyse.RawIRMessage(200, [
        7930, 3952, 494, 1482, 520, 1482, 494, 1508, 494, 520, 494, 1482, 494,
        520, 494, 1482, 494, 1482, 494, 3978, 494, 520, 494, 520, 494, 520, 494,
        520, 520, 520, 494, 520, 494, 520, 494, 1482, 494
    ], ignore)
    analyse.dump_constants(message, defs, "BAR", output)
    self.assertEqual(defs, [
        'const uint16_t kBARHdrMark = 7930;',
        'const uint16_t kBARBitMark = 496;',
        'const uint16_t kBARHdrSpace = 3965;',
        'const uint16_t kBAROneSpace = 1485;',
        'const uint16_t kBARZeroSpace = 520;',
        'const uint16_t kBARFreq = 38000;  // Hz. (Guessing the most common '
        'frequency.)'
    ])
    self.assertEqual(
        output.getvalue(), 'Guessing key value:\n'
        'kBARHdrMark   = 7930\n'
        'kBARHdrSpace  = 3965\n'
        'kBARBitMark   = 496\n'
        'kBAROneSpace  = 1485\n'
        'kBARZeroSpace = 520\n')

  def test_dump_constants_aircon(self):
    """More complex tests for the dump_constants() function."""
    ignore = StringIO()
    output = StringIO()
    defs = []
    message = analyse.RawIRMessage(200, [
        9008, 4496, 644, 1660, 676, 530, 648, 558, 672, 1636, 646, 1660, 644,
        556, 650, 584, 626, 560, 644, 580, 628, 1680, 624, 560, 648, 1662, 644,
        582, 648, 536, 674, 530, 646, 580, 628, 560, 670, 532, 646, 562, 644,
        556, 672, 536, 648, 1662, 646, 1660, 652, 554, 644, 558, 672, 538, 644,
        560, 668, 560, 648, 1638, 668, 536, 644, 1660, 668, 532, 648, 560, 648,
        1660, 674, 554, 622, 19990, 646, 580, 624, 1660, 648, 556, 648, 558,
        674, 556, 622, 560, 644, 564, 668, 536, 646, 1662, 646, 1658, 672, 534,
        648, 558, 644, 562, 648, 1662, 644, 584, 622, 558, 648, 562, 668, 534,
        670, 536, 670, 532, 672, 536, 646, 560, 646, 558, 648, 558, 670, 534,
        650, 558, 646, 560, 646, 560, 668, 1638, 646, 1662, 646, 1660, 646,
        1660, 648
    ], ignore)
    analyse.dump_constants(message, defs, "TEST", output)
    self.assertEqual(defs, [
        'const uint16_t kTESTHdrMark = 9008;',
        'const uint16_t kTESTBitMark = 650;',
        'const uint16_t kTESTHdrSpace = 4496;',
        'const uint16_t kTESTOneSpace = 1657;',
        'const uint16_t kTESTZeroSpace = 554;',
        'const uint16_t kTESTSpaceGap = 19990;',
        'const uint16_t kTESTFreq = 38000;  // Hz. (Guessing the most common '
        'frequency.)'
    ])
    self.assertEqual(
        output.getvalue(), 'Guessing key value:\n'
        'kTESTHdrMark   = 9008\n'
        'kTESTHdrSpace  = 4496\n'
        'kTESTBitMark   = 650\n'
        'kTESTOneSpace  = 1657\n'
        'kTESTZeroSpace = 554\n'
        'kTESTSpaceGap = 19990\n')

  def test_convert_rawdata(self):
    """Tests for the convert_rawdata() function."""
    # trivial cases
    self.assertEqual(analyse.convert_rawdata("0"), [0])
    with self.assertRaises(ValueError) as context:
      analyse.convert_rawdata("")
    self.assertEqual(
        str(context.exception), "Raw Data contains a non-numeric value of ''.")

    # Single parenthesis
    self.assertEqual(analyse.convert_rawdata("foo {10"), [10])
    self.assertEqual(analyse.convert_rawdata("20} bar"), [20])

    # No parentheses
    self.assertEqual(analyse.convert_rawdata("10,20 ,  30"), [10, 20, 30])

    # Dual parentheses
    self.assertEqual(analyse.convert_rawdata("{10,20 ,  30}"), [10, 20, 30])
    self.assertEqual(analyse.convert_rawdata("foo{10,20}bar"), [10, 20])

    # Many parentheses
    self.assertEqual(analyse.convert_rawdata("foo{10,20}{bar}"), [10, 20])
    self.assertEqual(analyse.convert_rawdata("foo{10,20}{bar}}{"), [10, 20])

    # Bad parentheses
    with self.assertRaises(ValueError) as context:
      analyse.convert_rawdata("}10{")
    self.assertEqual(
        str(context.exception),
        "Raw Data not parsible due to parentheses placement.")

    # Non base-10 values
    with self.assertRaises(ValueError) as context:
      analyse.convert_rawdata("10, 20, foo, bar, 30")
    self.assertEqual(
        str(context.exception),
        "Raw Data contains a non-numeric value of 'foo'.")

    # A messy usual "good" case.
    input_str = """uint16_t rawbuf[6] = {
    9008, 4496, 644,
    1660, 676,

    530}
    ;"""
    self.assertEqual(
        analyse.convert_rawdata(input_str), [9008, 4496, 644, 1660, 676, 530])

  def test_parse_and_report(self):
    """Tests for the parse_and_report() function."""

    # Without code generation.
    output = StringIO()
    input_str = """
        uint16_t rawbuf[139] = {9008, 4496, 644, 1660, 676, 530, 648, 558, 672,
            1636, 646, 1660, 644, 556, 650, 584, 626, 560, 644, 580, 628, 1680,
            624, 560, 648, 1662, 644, 582, 648, 536, 674, 530, 646, 580, 628,
            560, 670, 532, 646, 562, 644, 556, 672, 536, 648, 1662, 646, 1660,
            652, 554, 644, 558, 672, 538, 644, 560, 668, 560, 648, 1638, 668,
            536, 644, 1660, 668, 532, 648, 560, 648, 1660, 674, 554, 622, 19990,
            646, 580, 624, 1660, 648, 556, 648, 558, 674, 556, 622, 560, 644,
            564, 668, 536, 646, 1662, 646, 1658, 672, 534, 648, 558, 644, 562,
            648, 1662, 644, 584, 622, 558, 648, 562, 668, 534, 670, 536, 670,
            532, 672, 536, 646, 560, 646, 558, 648, 558, 670, 534, 650, 558,
            646, 560, 646, 560, 668, 1638, 646, 1662, 646, 1660, 646, 1660,
            648};"""
    analyse.parse_and_report(input_str, 200, False, "FOO", output)
    self.assertEqual(
        output.getvalue(), 'Found 139 timing entries.\n'
        'Potential Mark Candidates:\n'
        '[9008, 676]\n'
        'Potential Space Candidates:\n'
        '[19990, 4496, 1680, 584]\n'
        '\n'
        'Guessing encoding type:\n'
        'Looks like it uses space encoding. Yay!\n'
        '\n'
        'Guessing key value:\n'
        'kFOOHdrMark   = 9008\n'
        'kFOOHdrSpace  = 4496\n'
        'kFOOBitMark   = 650\n'
        'kFOOOneSpace  = 1657\n'
        'kFOOZeroSpace = 554\n'
        'kFOOSpaceGap = 19990\n'
        '\n'
        'Decoding protocol based on analysis so far:\n'
        '\n'
        'kFOOHdrMark+kFOOHdrSpace+10011000010100000000011000001010010GAP(19990)'
        '\n'
        '  Bits: 35\n'
        '  Hex:  0x4C2803052 (MSB first)\n'
        '        0x250600A19 (LSB first)\n'
        '  Dec:  20443050066 (MSB first)\n'
        '        9938405913 (LSB first)\n'
        '  Bin:  0b10011000010100000000011000001010010 (MSB first)\n'
        '        0b01001010000011000000000101000011001 (LSB first)\n'
        'kFOOBitMark(UNEXPECTED)01000000110001000000000000001111\n'
        '  Bits: 32\n'
        '  Hex:  0x40C4000F (MSB first)\n'
        '        0xF0002302 (LSB first)\n'
        '  Dec:  1086586895 (MSB first)\n'
        '        4026540802 (LSB first)\n'
        '  Bin:  0b01000000110001000000000000001111 (MSB first)\n'
        '        0b11110000000000000010001100000010 (LSB first)\n'
        '\n'
        'Total Nr. of suspected bits: 67\n')

    # With code generation.
    output = StringIO()
    input_str = """
        uint16_t rawbuf[37] = {7930, 3952, 494, 1482, 520, 1482, 494,
            1508, 494, 520, 494, 1482, 494, 520, 494, 1482, 494, 1482, 494,
            3978, 494, 520, 494, 520, 494, 520, 494, 520, 520, 520, 494, 520,
            494, 520, 494, 1482, 494};"""
    analyse.parse_and_report(input_str, 200, True, "FOO", output)
    self.assertEqual(
        output.getvalue(), 'Found 37 timing entries.\n'
        'Potential Mark Candidates:\n'
        '[7930, 520]\n'
        'Potential Space Candidates:\n'
        '[3978, 1508, 520]\n'
        '\n'
        'Guessing encoding type:\n'
        'Looks like it uses space encoding. Yay!\n'
        '\n'
        'Guessing key value:\n'
        'kFOOHdrMark   = 7930\n'
        'kFOOHdrSpace  = 3965\n'
        'kFOOBitMark   = 496\n'
        'kFOOOneSpace  = 1485\n'
        'kFOOZeroSpace = 520\n'
        '\n'
        'Decoding protocol based on analysis so far:\n'
        '\n'
        'kFOOHdrMark+kFOOHdrSpace+11101011\n'
        '  Bits: 8\n'
        '  Hex:  0xEB (MSB first)\n'
        '        0xD7 (LSB first)\n'
        '  Dec:  235 (MSB first)\n'
        '        215 (LSB first)\n'
        '  Bin:  0b11101011 (MSB first)\n'
        '        0b11010111 (LSB first)\n'
        'UNEXPECTED->kFOOHdrSpace+00000001\n'
        '  Bits: 8\n'
        '  Hex:  0x01 (MSB first)\n'
        '        0x80 (LSB first)\n'
        '  Dec:  1 (MSB first)\n'
        '        128 (LSB first)\n'
        '  Bin:  0b00000001 (MSB first)\n'
        '        0b10000000 (LSB first)\n'
        '\n'
        'Total Nr. of suspected bits: 16\n'
        '\n'
        'Generating a VERY rough code outline:\n'
        '\n'
        '// Copyright 2019 David Conran (crankyoldgit)\n'
        '// Support for FOO protocol\n'
        '\n'
        '#include "IRrecv.h"\n'
        '#include "IRsend.h"\n'
        '#include "IRutils.h"\n'
        '\n'
        "// WARNING: This probably isn't directly usable. It's a guide only.\n"
        '\n'
        '// See https://github.com/crankyoldgit/IRremoteESP8266/wiki/'
        'Adding-support-for-a-new-IR-protocol\n'
        '// for details of how to include this in the library.\n'
        'const uint16_t kFOOHdrMark = 7930;\n'
        'const uint16_t kFOOBitMark = 496;\n'
        'const uint16_t kFOOHdrSpace = 3965;\n'
        'const uint16_t kFOOOneSpace = 1485;\n'
        'const uint16_t kFOOZeroSpace = 520;\n'
        'const uint16_t kFOOFreq = 38000;  // Hz. (Guessing the most common'
        ' frequency.)\n'
        'const uint16_t kFOOBits = 16;  // Move to IRremoteESP8266.h\n'
        'const uint16_t kFOOOverhead = 5;\n'
        '#if SEND_FOO\n'
        '// Function should be safe up to 64 bits.\n'
        'void IRsend::sendFOO(const uint64_t data, const uint16_t nbits,'
        ' const uint16_t repeat) {\n'
        '  enableIROut(kFOOFreq);\n'
        '  for (uint16_t r = 0; r <= repeat; r++) {\n'
        '    uint64_t send_data = data;\n'
        '    // Header\n'
        '    mark(kFOOHdrMark);\n'
        '    space(kFOOHdrSpace);\n'
        '    // Data Section #1\n'
        '    // e.g. data = 0xEB, nbits = 8\n'
        '    sendData(kFOOBitMark, kFOOOneSpace, kFOOBitMark, kFOOZeroSpace,'
        ' send_data, 8, true);\n'
        '    send_data >>= 8;\n'
        '    // Footer\n'
        '    mark(kFOOBitMark);\n'
        '    space(kFOOHdrSpace);\n'
        '    // Data Section #2\n'
        '    // e.g. data = 0x1, nbits = 8\n'
        '    sendData(kFOOBitMark, kFOOOneSpace, kFOOBitMark, kFOOZeroSpace,'
        ' send_data, 8, true);\n'
        '    send_data >>= 8;\n'
        '    // Footer\n'
        '    mark(kFOOBitMark);\n'
        '    space(kDefaultMessageGap);  // A 100% made up guess of the gap'
        ' between messages.\n'
        '  }\n'
        '}\n'
        '#endif  // SEND_FOO\n'
        '\n'
        '#if DECODE_FOO\n'
        '// Function should be safe up to 64 bits.\n'
        'bool IRrecv::decodeFOO(decode_results *results, const uint16_t nbits,'
        ' const bool strict) {\n'
        '  if (results->rawlen < 2 * nbits + kFOOOverhead)\n'
        '    return false;  // Too short a message to match.\n'
        '  if (strict && nbits != kFOOBits)\n'
        '    return false;\n'
        '\n'
        '  uint16_t offset = kStartOffset;\n'
        '  uint64_t data = 0;\n'
        '  match_result_t data_result;\n'
        '\n'
        '  // Header\n'
        '  if (!matchMark(results->rawbuf[offset++], kFOOHdrMark))\n'
        '    return false;\n'
        '  if (!matchSpace(results->rawbuf[offset++], kFOOHdrSpace))\n'
        '    return false;\n'
        '\n'
        '  // Data Section #1\n'
        '  // e.g. data_result.data = 0xEB, nbits = 8\n'
        '  data_result = matchData(&(results->rawbuf[offset]), 8,\n'
        '                          kFOOBitMark, kFOOOneSpace,\n'
        '                          kFOOBitMark, kFOOZeroSpace);\n'
        '  offset += data_result.used;\n'
        '  if (data_result.success == false) return false;  // Fail\n'
        '  data <<= 8;  // Make room for the new bits of data.\n'
        '  data |= data_result.data;\n'
        '\n'
        '  // Footer\n'
        '  if (!matchMark(results->rawbuf[offset++], kFOOBitMark))\n'
        '    return false;\n'
        '  if (!matchSpace(results->rawbuf[offset++], kFOOHdrSpace))\n'
        '    return false;\n'
        '\n'
        '  // Data Section #2\n'
        '  // e.g. data_result.data = 0x1, nbits = 8\n'
        '  data_result = matchData(&(results->rawbuf[offset]), 8,\n'
        '                          kFOOBitMark, kFOOOneSpace,\n'
        '                          kFOOBitMark, kFOOZeroSpace);\n'
        '  offset += data_result.used;\n'
        '  if (data_result.success == false) return false;  // Fail\n'
        '  data <<= 8;  // Make room for the new bits of data.\n'
        '  data |= data_result.data;\n'
        '\n'
        '  // Footer\n'
        '  if (!matchMark(results->rawbuf[offset++], kFOOBitMark))\n'
        '    return false;\n'
        '\n'
        '  // Success\n'
        '  results->decode_type = decode_type_t::FOO;\n'
        '  results->bits = nbits;\n'
        '  results->value = data;\n'
        '  results->command = 0;\n'
        '  results->address = 0;\n'
        '  return true;\n'
        '}\n'
        '#endif  // DECODE_FOO\n')

  def test_reduce_list(self):
    """Tests for the reduce_list method."""

    ignore = StringIO()
    message = analyse.RawIRMessage(200, [
        7930, 3952, 494, 1482, 520, 1482, 494, 1508, 494, 520, 494, 1482, 494,
        520, 494, 1482, 494, 1482, 494, 3978, 494, 520, 494, 520, 494, 520, 494,
        520, 520, 520, 494, 520, 494, 520, 494, 1482, 494
    ], ignore)
    test_space_data = [4496, 1660, 530, 558, 1636, 1660, 556]
    result_list, result_dict = message.reduce_list(test_space_data)
    self.assertEqual([4496, 1660, 558], result_list)
    self.assertEqual({
        558: [558, 556, 530],
        1660: [1660, 1660, 1636],
        4496: [4496]
    }, result_dict)

  def test_leader_marks(self):
    """Tests for leader-type marks in parse_and_report() function."""

    # Ref Issue #973
    output = StringIO()
    input_str = """
        uint16_t rawData[853] = {
        29784, 49290, 3416, 1604, 464, 1210, 468, 372, 460, 374, 462, 374,
        466, 368, 464, 372, 462, 374, 464, 374, 464, 368, 464, 370, 464, 370,
        466, 370, 464, 1208, 464, 374, 462, 372, 466, 374, 464, 370, 462, 372,
        464, 370, 466, 370, 464, 372, 462, 374, 462, 374, 462, 378, 460, 370,
        460, 374, 464, 372, 462, 372, 464, 374, 466, 368, 464, 1210, 464, 374,
        466, 1202, 464, 1206, 464, 1210, 466, 1206, 468, 1204, 464, 1210, 466,
        370, 462, 1214, 460, 1208, 464, 1206, 464, 1208, 466, 1208, 464, 1206,
        466, 1208, 464, 1206, 466, 1212, 464, 370, 464, 370, 462, 374, 462,
        374, 462, 374, 462, 374, 462, 372, 464, 376, 460, 372, 462, 374, 466,
        1204, 464, 1210, 464, 372, 460, 374, 462, 1208, 464, 1212, 464, 1202,
        468, 1204, 464, 374, 460, 374, 466, 1208, 462, 1210, 462, 374, 462,
        376, 464, 368, 466, 1204, 462, 374, 466, 372, 464, 1206, 462, 376,
        460, 376, 464, 1210, 462, 1208, 462, 372, 466, 1206, 464, 1208, 466,
        372, 462, 1210, 462, 1210, 466, 374, 468, 1202, 464, 1206, 466, 374,
        462, 372, 464, 1208, 464, 374, 464, 372, 464, 376, 462, 370, 466, 368,
        464, 1208, 462, 1210, 460, 374, 464, 1208, 466, 1206, 464, 1214, 464,
        368, 462, 374, 462, 1212, 460, 1210, 466, 1206, 466, 370, 462, 1210,
        464, 416, 424, 1202, 466, 1220, 448, 376, 464, 372, 462, 372, 462,
        1212, 462, 374, 460, 1214, 468, 364, 468, 370, 462, 372, 462, 376,
        458, 374, 464, 372, 462, 376, 464, 376, 462, 1204, 464, 1210, 462,
        1210, 464, 1208, 466, 1208, 464, 1206, 462, 1210, 464, 1212, 464, 368,
        462, 372, 464, 372, 464, 372, 464, 372, 466, 370, 466, 370, 464, 376,
        464, 1202, 464, 1212, 464, 1204, 464, 1210, 462, 1208, 464, 1212, 462,
        1210, 464, 1212, 460, 372, 462, 374, 462, 374, 466, 370, 462, 374, 462,
        372, 464, 372, 462, 376, 462, 1206, 464, 1206, 466, 1210, 462, 1208,
        464, 1210, 466, 1204, 464, 1210, 462, 1214, 462, 368, 462, 374, 466,
        370, 462, 376, 466, 368, 466, 370, 462, 414, 424, 374, 464, 1206, 464,
        1206, 464, 1206, 468, 1206, 466, 1206, 466, 1210, 462, 1206, 464, 1214,
        468, 364, 466, 372, 466, 370, 462, 372, 462, 374, 464, 372, 462, 374,
        460, 376, 466, 1204, 464, 1208, 462, 1210, 464, 1206, 464, 1210, 464,
        1208, 464, 1208, 466, 1210, 462, 1206, 466, 1206, 466, 372, 462, 374,
        466, 1206, 466, 370, 464, 1206, 466, 376, 464, 368, 462, 372, 466,
        1206, 464, 1206, 464, 374, 466, 1204, 464, 374, 466, 1206, 466, 1204,
        468, 368, 466, 370, 466, 370, 462, 1212, 462, 1210, 462, 1210, 462,
        1214, 464, 368, 464, 1206, 466, 1206, 466, 1206, 464, 374, 464, 370,
        466, 370, 462, 378, 466, 366, 464, 372, 466, 368, 466, 370, 464, 370,
        462, 372, 462, 374, 464, 374, 464, 1202, 466, 1206, 462, 1208, 466,
        1208, 466, 1208, 464, 1210, 462, 1206, 464, 1212, 464, 368, 464, 372,
        464, 370, 468, 368, 462, 376, 462, 372, 466, 370, 464, 376, 462, 1206,
        464, 1210, 462, 1212, 462, 1208, 464, 1208, 462, 1212, 466, 1246, 424,
        1212, 464, 368, 464, 372, 466, 370, 464, 372, 462, 374, 464, 372, 464,
        370, 462, 1212, 466, 1206, 462, 1206, 464, 1210, 466, 1206, 462, 1208,
        464, 1250, 422, 1208, 468, 372, 464, 1204, 466, 1206, 466, 370, 462,
        374, 462, 376, 460, 374, 466, 370, 462, 376, 464, 368, 462, 376, 462,
        1210, 462, 1208, 464, 1206, 466, 1206, 464, 1208, 468, 1212, 460, 1206,
        464, 372, 464, 372, 466, 370, 462, 374, 466, 370, 466, 370, 466, 374,
        464, 368, 462, 1210, 462, 1210, 464, 1210, 462, 1208, 462, 1212, 464,
        1206, 466, 1208, 466, 366, 464, 374, 460, 374, 462, 1208, 466, 372,
        462, 374, 462, 374, 464, 1212, 468, 1202, 464, 1208, 466, 1204, 464,
        376, 460, 1208, 468, 1208, 462, 1208, 464, 378, 460, 372, 460, 372,
        462, 376, 464, 372, 462, 374, 460, 374, 464, 370, 462, 378, 464, 1202,
        468, 1204, 468, 1204, 466, 1208, 466, 1208, 464, 1210, 460, 1212, 462,
        1212, 464, 366, 466, 370, 464, 372, 466, 370, 464, 372, 462, 414, 424,
        372, 466, 372, 460, 1206, 466, 1206, 466, 1206, 466, 1208, 466, 1206,
        464, 1208, 466, 1208, 462, 1212, 468, 1202, 466, 1204, 470, 1204, 468,
        1204, 466, 1206, 466, 1206, 464, 1210, 462, 1212, 468, 366, 464, 372,
        462, 374, 460, 374, 460, 374, 466, 410, 424, 372, 460, 378, 466, 1200,
        464, 1212, 462, 1210, 464, 1210, 466, 1206, 462, 1208, 464, 1210, 464,
        1210, 464, 366, 462, 376, 462, 374, 460, 376, 462, 372, 466, 374, 460,
        372, 462, 378, 462, 1202, 468, 1206, 464, 1208, 466, 1208, 462, 1208,
        464, 1208, 468, 1204, 464, 1212, 466, 368, 462, 374, 466, 372, 464,
        370, 462, 374, 464, 370, 462, 376, 464, 374, 462, 1206, 464, 1208, 462,
        1210, 466, 1208, 460, 1210, 468, 1206, 462, 1210, 464, 1212, 466, 366,
        464, 374, 462, 372, 466, 370, 462, 374, 464, 372, 464, 370, 464, 374,
        462};"""
    analyse.parse_and_report(input_str, 200, True, "Hitachi", output)
    self.assertEqual(
        output.getvalue(), 'Found 853 timing entries.\n'
        'Potential Mark Candidates:\n'
        '[29784, 3416, 470]\n'
        'Potential Space Candidates:\n'
        '[49290, 1604, 1250, 416]\n'
        'DANGER: Unusual number of mark timings!\n'
        'Guessing encoding type:\n'
        'Looks like it uses space encoding. Yay!\n'
        '\n'
        'Guessing key value:\n'
        'kHitachiHdrMark   = 3416\n'
        'kHitachiHdrSpace  = 1604\n'
        'kHitachiBitMark   = 463\n'
        'kHitachiOneSpace  = 1208\n'
        'kHitachiZeroSpace = 372\n'
        'kHitachiLdrMark   = 29784\n'
        'kHitachiSpaceGap = 49290\n'
        '\n'
        'Decoding protocol based on analysis so far:\n'
        '\n'
        'kHitachiLdrMark+UNEXPECTED->GAP(49290)kHitachiHdrMark+'
        'kHitachiHdrSpace+100000000000100000000000000000101111110111111111'
        '0000000000110011110011000100100110110110110010000011011100111010110001'
        '010000000011111111000000001111111100000000111111110000000011111111'
        '0000000011111111110010100011010110001111011100000000000011111111'
        '0000000011111111000000011111111011000000001111111000000001111111'
        '0001000111101110000000001111111100000000111111111111111100000000'
        '111111110000000011111111000000001111111100000000\n'
        '  Bits: 424\n'
        '  Hex:  0x80080002FDFF0033CC49B6C8373AC500FF00FF00FF00FF00FFCA358F7000'
        'FF00FF01FEC03F807F11EE00FF00FFFF00FF00FF00FF00 (MSB first)\n'
        '        0x00FF00FF00FF00FFFF00FF007788FE01FC037F80FF00FF000EF1AC53FF00'
        'FF00FF00FF00FF00A35CEC136D9233CC00FFBF40001001 (LSB first)\n'
        '  Dec:  21666770463250971033249250747302630158357464218891161163035832'
        '525825434564377831675503794869126268735511944198247894513495375616'
        ' (MSB first)\n'
        '        16857184424372658669179408622645041920057617761062334415219526'
        '0950249734545225589859643087812860908833344117446370839379316737'
        ' (LSB first)\n'
        '  Bin:  0b100000000000100000000000000000101111110111111111000000000011'
        '00111100110001001001101101101100100000110111001110101100010100000000'
        '1111111100000000111111110000000011111111000000001111111100000000'
        '1111111111001010001101011000111101110000000000001111111100000000'
        '1111111100000001111111101100000000111111100000000111111100010001'
        '1110111000000000111111110000000011111111111111110000000011111111'
        '0000000011111111000000001111111100000000 (MSB first)\n'
        '        0b000000001111111100000000111111110000000011111111000000001111'
        '11111111111100000000111111110000000001110111100010001111111000000001'
        '1111110000000011011111111000000011111111000000001111111100000000'
        '0000111011110001101011000101001111111111000000001111111100000000'
        '1111111100000000111111110000000011111111000000001010001101011100'
        '1110110000010011011011011001001000110011110011000000000011111111'
        '1011111101000000000000000001000000000001 (LSB first)\n'
        '\n'
        'Total Nr. of suspected bits: 424\n'
        '\n'
        'Generating a VERY rough code outline:\n'
        '\n'
        '// Copyright 2019 David Conran (crankyoldgit)\n'
        '// Support for Hitachi protocol\n'
        '\n'
        '#include "IRrecv.h"\n'
        '#include "IRsend.h"\n'
        '#include "IRutils.h"\n'
        '\n'
        "// WARNING: This probably isn't directly usable. It's a guide only.\n"
        '\n'
        '// See https://github.com/crankyoldgit/IRremoteESP8266/wiki/'
        'Adding-support-for-a-new-IR-protocol\n'
        '// for details of how to include this in the library.\n'
        'const uint16_t kHitachiHdrMark = 3416;\n'
        'const uint16_t kHitachiBitMark = 463;\n'
        'const uint16_t kHitachiHdrSpace = 1604;\n'
        'const uint16_t kHitachiOneSpace = 1208;\n'
        'const uint16_t kHitachiZeroSpace = 372;\n'
        'const uint16_t kHitachiLdrMark = 29784;\n'
        'const uint16_t kHitachiSpaceGap = 49290;\n'
        'const uint16_t kHitachiFreq = 38000;  // Hz. (Guessing the most'
        ' common frequency.)\n'
        'const uint16_t kHitachiBits = 424;  // Move to IRremoteESP8266.h\n'
        'const uint16_t kHitachiStateLength = 53;  // Move to IRremoteESP8266.h'
        '\n'
        'const uint16_t kHitachiOverhead = 5;\n'
        "// DANGER: More than 64 bits detected. A uint64_t for 'data' won't"
        ' work!\n'
        '#if SEND_HITACHI\n'
        '// Function should be safe up to 64 bits.\n'
        'void IRsend::sendHitachi(const uint64_t data, const uint16_t nbits,'
        ' const uint16_t repeat) {\n'
        '  enableIROut(kHitachiFreq);\n'
        '  for (uint16_t r = 0; r <= repeat; r++) {\n'
        '    uint64_t send_data = data;\n'
        '    // Leader\n'
        '    mark(kHitachiLdrMark);\n'
        '    // Gap\n'
        '    space(kHitachiSpaceGap);\n'
        '    // Header\n'
        '    mark(kHitachiHdrMark);\n'
        '    space(kHitachiHdrSpace);\n'
        '    // Data Section #1\n'
        '    // e.g. data = 0x80080002FDFF0033CC49B6C8373AC500FF00FF00FF00FF00'
        'FFCA358F7000FF00FF01FEC03F807F11EE00FF00FFFF00FF00FF00FF00,'
        ' nbits = 424\n'
        '    sendData(kHitachiBitMark, kHitachiOneSpace, kHitachiBitMark,'
        ' kHitachiZeroSpace, send_data, 424, true);\n'
        '    send_data >>= 424;\n'
        '    // Footer\n'
        '    mark(kHitachiBitMark);\n'
        '    space(kDefaultMessageGap);  // A 100% made up guess of the gap'
        ' between messages.\n'
        '  }\n'
        '}\n'
        '#endif  // SEND_HITACHI\n'
        '\n'
        '#if SEND_HITACHI\n'
        '// Alternative >64bit function to send HITACHI messages\n'
        '// Where data is:\n'
        '//   uint8_t data[kHitachiStateLength] = {0x80, 0x08, 0x00, 0x02,'
        ' 0xFD, 0xFF, 0x00, 0x33, 0xCC, 0x49, 0xB6, 0xC8, 0x37, 0x3A, 0xC5,'
        ' 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xCA,'
        ' 0x35, 0x8F, 0x70, 0x00, 0xFF, 0x00, 0xFF, 0x01, 0xFE, 0xC0, 0x3F,'
        ' 0x80, 0x7F, 0x11, 0xEE, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF,'
        ' 0x00, 0xFF, 0x00, 0xFF, 0x00};\n'
        '//\n'
        '// Args:\n'
        '//   data: An array of bytes containing the IR command.\n'
        '//         It is assumed to be in MSB order for this code.\n'
        '//   nbytes: Nr. of bytes of data in the array.'
        ' (>=kHitachiStateLength)\n'
        '//   repeat: Nr. of times the message is to be repeated.\n'
        '//\n'
        '// Status: ALPHA / Untested.\n'
        'void IRsend::sendHitachi(const uint8_t data[], const uint16_t nbytes,'
        ' const uint16_t repeat) {\n'
        '  for (uint16_t r = 0; r <= repeat; r++) {\n'
        '    uint16_t pos = 0;\n'
        '    // Data Section #1\n'
        '    // e.g.\n'
        '    //   bits = 424; bytes = 53;\n'
        '    //   *(data + pos) = {0x80, 0x08, 0x00, 0x02, 0xFD, 0xFF, 0x00,'
        ' 0x33, 0xCC, 0x49, 0xB6, 0xC8, 0x37, 0x3A, 0xC5, 0x00, 0xFF, 0x00,'
        ' 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xCA, 0x35, 0x8F, 0x70,'
        ' 0x00, 0xFF, 0x00, 0xFF, 0x01, 0xFE, 0xC0, 0x3F, 0x80, 0x7F, 0x11,'
        ' 0xEE, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,'
        ' 0xFF, 0x00};\n'
        '    sendGeneric(kHitachiHdrMark, kHitachiHdrSpace,\n'
        '                kHitachiBitMark, kHitachiOneSpace,\n'
        '                kHitachiBitMark, kHitachiZeroSpace,\n'
        '                kHitachiBitMark, kHitachiSpaceGap,\n'
        '                data + pos, 53,  // Bytes\n'
        '                kHitachiFreq, true, kNoRepeat, kDutyDefault);\n'
        '    pos += 53;  // Adjust by how many bytes of data we sent\n'
        '  }\n'
        '}\n'
        '#endif  // SEND_HITACHI\n'
        '\n'
        "// DANGER: More than 64 bits detected. A uint64_t for 'data' won't"
        ' work!\n'
        '#if DECODE_HITACHI\n'
        '// Function should be safe up to 64 bits.\n'
        'bool IRrecv::decodeHitachi(decode_results *results,'
        ' const uint16_t nbits, const bool strict) {\n'
        '  if (results->rawlen < 2 * nbits + kHitachiOverhead)\n'
        '    return false;  // Too short a message to match.\n'
        '  if (strict && nbits != kHitachiBits)\n'
        '    return false;\n'
        '\n'
        '  uint16_t offset = kStartOffset;\n'
        '  uint64_t data = 0;\n'
        '  match_result_t data_result;\n'
        '\n'
        '  // Leader\n'
        '  if (!matchMark(results->rawbuf[offset++], kHitachiLdrMark))\n'
        '    return false;\n'
        '\n'
        '  // Gap\n'
        '  if (!matchSpace(results->rawbuf[offset++], kHitachiSpaceGap))\n'
        '    return false;\n'
        '\n'
        '  // Header\n'
        '  if (!matchMark(results->rawbuf[offset++], kHitachiHdrMark))\n'
        '    return false;\n'
        '  if (!matchSpace(results->rawbuf[offset++], kHitachiHdrSpace))\n'
        '    return false;\n'
        '\n'
        '  // Data Section #1\n'
        '  // e.g. data_result.data = 0x80080002FDFF0033CC49B6C8373AC500FF00FF'
        '00FF00FF00FFCA358F7000FF00FF01FEC03F807F11EE00FF00FFFF00FF00FF00FF00,'
        ' nbits = 424\n'
        '  data_result = matchData(&(results->rawbuf[offset]), 424,\n'
        '                          kHitachiBitMark, kHitachiOneSpace,\n'
        '                          kHitachiBitMark, kHitachiZeroSpace);\n'
        '  offset += data_result.used;\n'
        '  if (data_result.success == false) return false;  // Fail\n'
        '  data <<= 424;  // Make room for the new bits of data.\n'
        '  data |= data_result.data;\n'
        '\n'
        '  // Footer\n'
        '  if (!matchMark(results->rawbuf[offset++], kHitachiBitMark))\n'
        '    return false;\n'
        '\n'
        '  // Success\n'
        '  results->decode_type = decode_type_t::HITACHI;\n'
        '  results->bits = nbits;\n'
        '  results->value = data;\n'
        '  results->command = 0;\n'
        '  results->address = 0;\n'
        '  return true;\n'
        '}\n'
        '#endif  // DECODE_HITACHI\n'
        '\n'
        '// Note: This should be 64+ bit safe.\n'
        '#if DECODE_HITACHI\n'
        '// Function should be safe over 64 bits.\n'
        'bool IRrecv::decodeHitachi(decode_results *results,'
        ' const uint16_t nbits, const bool strict) {\n'
        '  if (results->rawlen < 2 * nbits + kHitachiOverhead)\n'
        '    return false;  // Too short a message to match.\n'
        '  if (strict && nbits != kHitachiBits)\n'
        '    return false;\n'
        '\n'
        '  uint16_t offset = kStartOffset;\n'
        '  uint16_t pos = 0;\n'
        '  uint16_t used = 0;\n'
        '\n'
        '  // Data Section #1\n'
        '  // e.g.\n'
        '  //   bits = 424; bytes = 53;\n'
        '  //   *(results->state + pos) = {0x80, 0x08, 0x00, 0x02, 0xFD, 0xFF,'
        ' 0x00, 0x33, 0xCC, 0x49, 0xB6, 0xC8, 0x37, 0x3A, 0xC5, 0x00, 0xFF,'
        ' 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xCA, 0x35, 0x8F,'
        ' 0x70, 0x00, 0xFF, 0x00, 0xFF, 0x01, 0xFE, 0xC0, 0x3F, 0x80, 0x7F,'
        ' 0x11, 0xEE, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xFF,'
        ' 0x00, 0xFF, 0x00};\n'
        '  used = matchGeneric(results->rawbuf + offset,'
        ' results->state + pos,\n'
        '                      results->rawlen - offset, 424,\n'
        '                      kHitachiHdrMark, kHitachiHdrSpace,\n'
        '                      kHitachiBitMark, kHitachiOneSpace,\n'
        '                      kHitachiBitMark, kHitachiZeroSpace,\n'
        '                      kHitachiBitMark, kHitachiSpaceGap, true);\n'
        '  if (used == 0) return false;  // We failed to find any data.\n'
        '  offset += used;  // Adjust for how much of the message we read.\n'
        '  pos += 53;  // Adjust by how many bytes of data we read\n'
        '\n'
        '  // Success\n'
        '  results->decode_type = decode_type_t::HITACHI;\n'
        '  results->bits = nbits;\n'
        '  return true;\n'
        '}\n'
        '#endif  // DECODE_HITACHI\n')

  def test_unusual_gaps(self):
    """Tests for unusual Space Gaps in parse_and_report() function."""

    # Tests for unusual Gaps. (Issue #482)
    output = StringIO()
    input_str = """
        uint16_t rawbuf[272] = {3485, 3512, 864, 864, 864, 2620, 864, 864,
            864, 2620, 864, 2620, 864, 2620, 864, 2620, 864, 2620, 864, 864,
            864, 2620, 864, 864, 864, 2620, 864, 2620, 864, 2620, 864, 2620,
            864, 2620, 864, 864, 864, 2620, 864, 864, 864, 864, 864, 864,
            864, 864, 864, 864, 864, 864, 864, 864, 864, 2620, 864, 864,
            864, 864, 864, 864, 864, 864, 864, 864, 864, 864,
            3485, 3512, 864, 864, 864, 2620, 864, 864, 864, 2620, 864, 2620,
            864, 2620, 864, 2620, 864, 2620, 864, 864, 864, 2620, 864, 864,
            864, 2620, 864, 2620, 864, 2620, 864, 2620, 864, 2620, 864, 864,
            864, 2620, 864, 864, 864, 864, 864, 864, 864, 864, 864, 864, 864,
            864, 864, 864, 864, 2620, 864, 864, 864, 864, 864, 864, 864, 864,
            864, 864, 864, 864,
            3485, 3512, 864, 13996,
            3485, 3512, 864, 864, 864, 864, 864, 2620, 864, 864, 864, 2620,
            864, 2620, 864, 2620, 864, 2620, 864, 864, 864, 864, 864, 2620,
            864, 864, 864, 2620, 864, 2620, 864, 2620, 864, 2620, 864, 864,
            864, 2620, 864, 2620, 864, 864, 864, 2620, 864, 2620, 864, 864,
            864, 864, 864, 864, 864, 2620, 864, 2620, 864, 864, 864, 2620,
            864, 2620, 864, 864, 864, 864,
            3485, 3512, 864, 864, 864, 864, 864, 2620, 864, 864, 864, 2620,
            864, 2620, 864, 2620, 864, 2620, 864, 864, 864, 864, 864, 2620,
            864, 864, 864, 2620, 864, 2620, 864, 2620, 864, 2620, 864, 864,
            864, 2620, 864, 2620, 864, 864, 864, 2620, 864, 2620, 864, 864,
            864, 864, 864, 864, 864, 2620, 864, 2620, 864, 864, 864, 2620,
            864, 2620, 864, 864, 864, 864, 3485, 3512, 864, 13996};"""
    analyse.parse_and_report(input_str, 200, True, "FOO", output)
    self.assertEqual(
        output.getvalue(), 'Found 272 timing entries.\n'
        'Potential Mark Candidates:\n'
        '[3485, 864]\n'
        'Potential Space Candidates:\n'
        '[13996, 3512, 2620, 864]\n'
        '\n'
        'Guessing encoding type:\n'
        'Looks like it uses space encoding. Yay!\n'
        '\n'
        'Guessing key value:\n'
        'kFOOHdrMark   = 3485\n'
        'kFOOHdrSpace  = 3512\n'
        'kFOOBitMark   = 864\n'
        'kFOOOneSpace  = 2620\n'
        'kFOOZeroSpace = 864\n'
        'kFOOSpaceGap = 13996\n'
        '\n'
        'Decoding protocol based on analysis so far:\n'
        '\n'
        'kFOOHdrMark+kFOOHdrSpace+01011111010111110100000001000000\n'
        '  Bits: 32\n'
        '  Hex:  0x5F5F4040 (MSB first)\n'
        '        0x0202FAFA (LSB first)\n'
        '  Dec:  1600077888 (MSB first)\n'
        '        33749754 (LSB first)\n'
        '  Bin:  0b01011111010111110100000001000000 (MSB first)\n'
        '        0b00000010000000101111101011111010 (LSB first)\n'
        'kFOOHdrMark+kFOOHdrSpace+01011111010111110100000001000000\n'
        '  Bits: 32\n'
        '  Hex:  0x5F5F4040 (MSB first)\n'
        '        0x0202FAFA (LSB first)\n'
        '  Dec:  1600077888 (MSB first)\n'
        '        33749754 (LSB first)\n'
        '  Bin:  0b01011111010111110100000001000000 (MSB first)\n'
        '        0b00000010000000101111101011111010 (LSB first)\n'
        'kFOOHdrMark+kFOOHdrSpace+GAP(13996)kFOOHdrMark+kFOOHdrSpace+0010111100'
        '1011110110110001101100\n'
        '  Bits: 32\n'
        '  Hex:  0x2F2F6C6C (MSB first)\n'
        '        0x3636F4F4 (LSB first)\n'
        '  Dec:  791637100 (MSB first)\n'
        '        909571316 (LSB first)\n'
        '  Bin:  0b00101111001011110110110001101100 (MSB first)\n'
        '        0b00110110001101101111010011110100 (LSB first)\n'
        'kFOOHdrMark+kFOOHdrSpace+00101111001011110110110001101100\n'
        '  Bits: 32\n'
        '  Hex:  0x2F2F6C6C (MSB first)\n'
        '        0x3636F4F4 (LSB first)\n'
        '  Dec:  791637100 (MSB first)\n'
        '        909571316 (LSB first)\n'
        '  Bin:  0b00101111001011110110110001101100 (MSB first)\n'
        '        0b00110110001101101111010011110100 (LSB first)\n'
        'kFOOHdrMark+kFOOHdrSpace+GAP(13996)\n'
        'Total Nr. of suspected bits: 128\n'
        '\n'
        'Generating a VERY rough code outline:\n'
        '\n'
        '// Copyright 2019 David Conran (crankyoldgit)\n'
        '// Support for FOO protocol\n'
        '\n'
        '#include "IRrecv.h"\n'
        '#include "IRsend.h"\n'
        '#include "IRutils.h"\n'
        '\n'
        "// WARNING: This probably isn't directly usable. It's a guide only.\n"
        '\n'
        '// See https://github.com/crankyoldgit/IRremoteESP8266/wiki/'
        'Adding-support-for-a-new-IR-protocol\n'
        '// for details of how to include this in the library.\n'
        'const uint16_t kFOOHdrMark = 3485;\n'
        'const uint16_t kFOOBitMark = 864;\n'
        'const uint16_t kFOOHdrSpace = 3512;\n'
        'const uint16_t kFOOOneSpace = 2620;\n'
        'const uint16_t kFOOZeroSpace = 864;\n'
        'const uint16_t kFOOSpaceGap = 13996;\n'
        'const uint16_t kFOOFreq = 38000;  // Hz. (Guessing the most common'
        ' frequency.)\n'
        'const uint16_t kFOOBits = 128;  // Move to IRremoteESP8266.h\n'
        'const uint16_t kFOOStateLength = 16;  // Move to IRremoteESP8266.h\n'
        'const uint16_t kFOOOverhead = 16;\n'
        "// DANGER: More than 64 bits detected. A uint64_t for 'data' won't"
        ' work!\n'
        '#if SEND_FOO\n'
        '// Function should be safe up to 64 bits.\n'
        'void IRsend::sendFOO(const uint64_t data, const uint16_t nbits,'
        ' const uint16_t repeat) {\n'
        '  enableIROut(kFOOFreq);\n'
        '  for (uint16_t r = 0; r <= repeat; r++) {\n'
        '    uint64_t send_data = data;\n'
        '    // Header\n'
        '    mark(kFOOHdrMark);\n'
        '    space(kFOOHdrSpace);\n'
        '    // Data Section #1\n'
        '    // e.g. data = 0x5F5F4040, nbits = 32\n'
        '    sendData(kFOOBitMark, kFOOOneSpace, kFOOBitMark, kFOOZeroSpace,'
        ' send_data, 32, true);\n'
        '    send_data >>= 32;\n'
        '    // Header\n'
        '    mark(kFOOHdrMark);\n'
        '    space(kFOOHdrSpace);\n'
        '    // Data Section #2\n'
        '    // e.g. data = 0x5F5F4040, nbits = 32\n'
        '    sendData(kFOOBitMark, kFOOOneSpace, kFOOBitMark, kFOOZeroSpace,'
        ' send_data, 32, true);\n'
        '    send_data >>= 32;\n'
        '    // Header\n'
        '    mark(kFOOHdrMark);\n'
        '    space(kFOOHdrSpace);\n'
        '    // Gap\n'
        '    mark(kFOOBitMark);\n'
        '    space(kFOOSpaceGap);\n'
        '    // Header\n'
        '    mark(kFOOHdrMark);\n'
        '    space(kFOOHdrSpace);\n'
        '    // Data Section #3\n'
        '    // e.g. data = 0x2F2F6C6C, nbits = 32\n'
        '    sendData(kFOOBitMark, kFOOOneSpace, kFOOBitMark, kFOOZeroSpace,'
        ' send_data, 32, true);\n'
        '    send_data >>= 32;\n'
        '    // Header\n'
        '    mark(kFOOHdrMark);\n'
        '    space(kFOOHdrSpace);\n'
        '    // Data Section #4\n'
        '    // e.g. data = 0x2F2F6C6C, nbits = 32\n'
        '    sendData(kFOOBitMark, kFOOOneSpace, kFOOBitMark, kFOOZeroSpace,'
        ' send_data, 32, true);\n'
        '    send_data >>= 32;\n'
        '    // Header\n'
        '    mark(kFOOHdrMark);\n'
        '    space(kFOOHdrSpace);\n'
        '    // Gap\n'
        '    mark(kFOOBitMark);\n'
        '    space(kFOOSpaceGap);\n'
        '    space(kDefaultMessageGap);  // A 100% made up guess of the gap'
        ' between messages.\n'
        '  }\n'
        '}\n'
        '#endif  // SEND_FOO\n'
        '\n'
        '#if SEND_FOO\n'
        '// Alternative >64bit function to send FOO messages\n'
        '// Where data is:\n'
        '//   uint8_t data[kFOOStateLength] = {0x5F, 0x5F, 0x40, 0x40, 0x5F,'
        ' 0x5F, 0x40, 0x40, 0x2F, 0x2F, 0x6C, 0x6C, 0x2F, 0x2F, 0x6C, 0x6C};\n'
        '//\n'
        '// Args:\n'
        '//   data: An array of bytes containing the IR command.\n'
        '//         It is assumed to be in MSB order for this code.\n'
        '//   nbytes: Nr. of bytes of data in the array. (>=kFOOStateLength)\n'
        '//   repeat: Nr. of times the message is to be repeated.\n'
        '//\n'
        '// Status: ALPHA / Untested.\n'
        'void IRsend::sendFOO(const uint8_t data[], const uint16_t nbytes,'
        ' const uint16_t repeat) {\n'
        '  for (uint16_t r = 0; r <= repeat; r++) {\n'
        '    uint16_t pos = 0;\n'
        '    // Data Section #1\n'
        '    // e.g.\n'
        '    //   bits = 32; bytes = 4;\n'
        '    //   *(data + pos) = {0x5F, 0x5F, 0x40, 0x40};\n'
        '    sendGeneric(kFOOHdrMark, kFOOHdrSpace,\n'
        '                kFOOBitMark, kFOOOneSpace,\n'
        '                kFOOBitMark, kFOOZeroSpace,\n'
        '                kFOOHdrMark, kFOOHdrSpace,\n'
        '                data + pos, 4,  // Bytes\n'
        '                kFOOFreq, true, kNoRepeat, kDutyDefault);\n'
        '    pos += 4;  // Adjust by how many bytes of data we sent\n'
        '    // Data Section #2\n'
        '    // e.g.\n'
        '    //   bits = 32; bytes = 4;\n'
        '    //   *(data + pos) = {0x5F, 0x5F, 0x40, 0x40};\n'
        '    sendGeneric(kFOOHdrMark, kFOOHdrSpace,\n'
        '                kFOOBitMark, kFOOOneSpace,\n'
        '                kFOOBitMark, kFOOZeroSpace,\n'
        '                kFOOHdrMark, kFOOHdrSpace,\n'
        '                data + pos, 4,  // Bytes\n'
        '                kFOOFreq, true, kNoRepeat, kDutyDefault);\n'
        '    pos += 4;  // Adjust by how many bytes of data we sent\n'
        '    // Data Section #3\n'
        '    // e.g.\n'
        '    //   bits = 32; bytes = 4;\n'
        '    //   *(data + pos) = {0x2F, 0x2F, 0x6C, 0x6C};\n'
        '    sendGeneric(kFOOHdrMark, kFOOHdrSpace,\n'
        '                kFOOBitMark, kFOOOneSpace,\n'
        '                kFOOBitMark, kFOOZeroSpace,\n'
        '                kFOOHdrMark, kFOOHdrSpace,\n'
        '                data + pos, 4,  // Bytes\n'
        '                kFOOFreq, true, kNoRepeat, kDutyDefault);\n'
        '    pos += 4;  // Adjust by how many bytes of data we sent\n'
        '    // Data Section #4\n'
        '    // e.g.\n'
        '    //   bits = 32; bytes = 4;\n'
        '    //   *(data + pos) = {0x2F, 0x2F, 0x6C, 0x6C};\n'
        '    sendGeneric(kFOOHdrMark, kFOOHdrSpace,\n'
        '                kFOOBitMark, kFOOOneSpace,\n'
        '                kFOOBitMark, kFOOZeroSpace,\n'
        '                kFOOHdrMark, kFOOHdrSpace,\n'
        '                data + pos, 4,  // Bytes\n'
        '                kFOOFreq, true, kNoRepeat, kDutyDefault);\n'
        '    pos += 4;  // Adjust by how many bytes of data we sent\n'
        '  }\n'
        '}\n'
        '#endif  // SEND_FOO\n'
        '\n'
        "// DANGER: More than 64 bits detected. A uint64_t for 'data' won't "
        'work!\n'
        '#if DECODE_FOO\n'
        '// Function should be safe up to 64 bits.\n'
        'bool IRrecv::decodeFOO(decode_results *results, const uint16_t nbits,'
        ' const bool strict) {\n'
        '  if (results->rawlen < 2 * nbits + kFOOOverhead)\n'
        '    return false;  // Too short a message to match.\n'
        '  if (strict && nbits != kFOOBits)\n'
        '    return false;\n'
        '\n'
        '  uint16_t offset = kStartOffset;\n'
        '  uint64_t data = 0;\n'
        '  match_result_t data_result;\n'
        '\n'
        '  // Header\n'
        '  if (!matchMark(results->rawbuf[offset++], kFOOHdrMark))\n'
        '    return false;\n'
        '  if (!matchSpace(results->rawbuf[offset++], kFOOHdrSpace))\n'
        '    return false;\n'
        '\n'
        '  // Data Section #1\n'
        '  // e.g. data_result.data = 0x5F5F4040, nbits = 32\n'
        '  data_result = matchData(&(results->rawbuf[offset]), 32,\n'
        '                          kFOOBitMark, kFOOOneSpace,\n'
        '                          kFOOBitMark, kFOOZeroSpace);\n'
        '  offset += data_result.used;\n'
        '  if (data_result.success == false) return false;  // Fail\n'
        '  data <<= 32;  // Make room for the new bits of data.\n'
        '  data |= data_result.data;\n'
        '\n'
        '  // Header\n'
        '  if (!matchMark(results->rawbuf[offset++], kFOOHdrMark))\n'
        '    return false;\n'
        '  if (!matchSpace(results->rawbuf[offset++], kFOOHdrSpace))\n'
        '    return false;\n'
        '\n'
        '  // Data Section #2\n'
        '  // e.g. data_result.data = 0x5F5F4040, nbits = 32\n'
        '  data_result = matchData(&(results->rawbuf[offset]), 32,\n'
        '                          kFOOBitMark, kFOOOneSpace,\n'
        '                          kFOOBitMark, kFOOZeroSpace);\n'
        '  offset += data_result.used;\n'
        '  if (data_result.success == false) return false;  // Fail\n'
        '  data <<= 32;  // Make room for the new bits of data.\n'
        '  data |= data_result.data;\n'
        '\n'
        '  // Header\n'
        '  if (!matchMark(results->rawbuf[offset++], kFOOHdrMark))\n'
        '    return false;\n'
        '  if (!matchSpace(results->rawbuf[offset++], kFOOHdrSpace))\n'
        '    return false;\n'
        '\n'
        '  // Gap\n'
        '  if (!matchMark(results->rawbuf[offset++], kFOOBitMark))\n'
        '    return false;\n'
        '  if (!matchSpace(results->rawbuf[offset++], kFOOSpaceGap))\n'
        '    return false;\n'
        '\n'
        '  // Header\n'
        '  if (!matchMark(results->rawbuf[offset++], kFOOHdrMark))\n'
        '    return false;\n'
        '  if (!matchSpace(results->rawbuf[offset++], kFOOHdrSpace))\n'
        '    return false;\n'
        '\n'
        '  // Data Section #3\n'
        '  // e.g. data_result.data = 0x2F2F6C6C, nbits = 32\n'
        '  data_result = matchData(&(results->rawbuf[offset]), 32,\n'
        '                          kFOOBitMark, kFOOOneSpace,\n'
        '                          kFOOBitMark, kFOOZeroSpace);\n'
        '  offset += data_result.used;\n'
        '  if (data_result.success == false) return false;  // Fail\n'
        '  data <<= 32;  // Make room for the new bits of data.\n'
        '  data |= data_result.data;\n'
        '\n'
        '  // Header\n'
        '  if (!matchMark(results->rawbuf[offset++], kFOOHdrMark))\n'
        '    return false;\n'
        '  if (!matchSpace(results->rawbuf[offset++], kFOOHdrSpace))\n'
        '    return false;\n'
        '\n'
        '  // Data Section #4\n'
        '  // e.g. data_result.data = 0x2F2F6C6C, nbits = 32\n'
        '  data_result = matchData(&(results->rawbuf[offset]), 32,\n'
        '                          kFOOBitMark, kFOOOneSpace,\n'
        '                          kFOOBitMark, kFOOZeroSpace);\n'
        '  offset += data_result.used;\n'
        '  if (data_result.success == false) return false;  // Fail\n'
        '  data <<= 32;  // Make room for the new bits of data.\n'
        '  data |= data_result.data;\n'
        '\n'
        '  // Header\n'
        '  if (!matchMark(results->rawbuf[offset++], kFOOHdrMark))\n'
        '    return false;\n'
        '  if (!matchSpace(results->rawbuf[offset++], kFOOHdrSpace))\n'
        '    return false;\n'
        '\n'
        '  // Gap\n'
        '  if (!matchMark(results->rawbuf[offset++], kFOOBitMark))\n'
        '    return false;\n'
        '  if (!matchSpace(results->rawbuf[offset++], kFOOSpaceGap))\n'
        '    return false;\n'
        '\n'
        '  // Success\n'
        '  results->decode_type = decode_type_t::FOO;\n'
        '  results->bits = nbits;\n'
        '  results->value = data;\n'
        '  results->command = 0;\n'
        '  results->address = 0;\n'
        '  return true;\n'
        '}\n'
        '#endif  // DECODE_FOO\n'
        '\n'
        '// Note: This should be 64+ bit safe.\n'
        '#if DECODE_FOO\n'
        '// Function should be safe over 64 bits.\n'
        'bool IRrecv::decodeFOO(decode_results *results, const uint16_t nbits,'
        ' const bool strict) {\n'
        '  if (results->rawlen < 2 * nbits + kFOOOverhead)\n'
        '    return false;  // Too short a message to match.\n'
        '  if (strict && nbits != kFOOBits)\n'
        '    return false;\n'
        '\n'
        '  uint16_t offset = kStartOffset;\n'
        '  uint16_t pos = 0;\n'
        '  uint16_t used = 0;\n'
        '\n'
        '  // Data Section #1\n'
        '  // e.g.\n'
        '  //   bits = 32; bytes = 4;\n'
        '  //   *(results->state + pos) = {0x5F, 0x5F, 0x40, 0x40};\n'
        '  used = matchGeneric(results->rawbuf + offset, results->state + pos,'
        '\n'
        '                      results->rawlen - offset, 32,\n'
        '                      kFOOHdrMark, kFOOHdrSpace,\n'
        '                      kFOOBitMark, kFOOOneSpace,\n'
        '                      kFOOBitMark, kFOOZeroSpace,\n'
        '                      kFOOHdrMark, kFOOHdrSpace, true);\n'
        '  if (used == 0) return false;  // We failed to find any data.\n'
        '  offset += used;  // Adjust for how much of the message we read.\n'
        '  pos += 4;  // Adjust by how many bytes of data we read\n'
        '\n'
        '  // Data Section #2\n'
        '  // e.g.\n'
        '  //   bits = 32; bytes = 4;\n'
        '  //   *(results->state + pos) = {0x5F, 0x5F, 0x40, 0x40};\n'
        '  used = matchGeneric(results->rawbuf + offset, results->state + pos,'
        '\n'
        '                      results->rawlen - offset, 32,\n'
        '                      kFOOHdrMark, kFOOHdrSpace,\n'
        '                      kFOOBitMark, kFOOOneSpace,\n'
        '                      kFOOBitMark, kFOOZeroSpace,\n'
        '                      kFOOHdrMark, kFOOHdrSpace, true);\n'
        '  if (used == 0) return false;  // We failed to find any data.\n'
        '  offset += used;  // Adjust for how much of the message we read.\n'
        '  pos += 4;  // Adjust by how many bytes of data we read\n'
        '\n'
        '  // Data Section #3\n'
        '  // e.g.\n'
        '  //   bits = 32; bytes = 4;\n'
        '  //   *(results->state + pos) = {0x2F, 0x2F, 0x6C, 0x6C};\n'
        '  used = matchGeneric(results->rawbuf + offset, results->state + pos,'
        '\n'
        '                      results->rawlen - offset, 32,\n'
        '                      kFOOHdrMark, kFOOHdrSpace,\n'
        '                      kFOOBitMark, kFOOOneSpace,\n'
        '                      kFOOBitMark, kFOOZeroSpace,\n'
        '                      kFOOHdrMark, kFOOHdrSpace, true);\n'
        '  if (used == 0) return false;  // We failed to find any data.\n'
        '  offset += used;  // Adjust for how much of the message we read.\n'
        '  pos += 4;  // Adjust by how many bytes of data we read\n'
        '\n'
        '  // Data Section #4\n'
        '  // e.g.\n'
        '  //   bits = 32; bytes = 4;\n'
        '  //   *(results->state + pos) = {0x2F, 0x2F, 0x6C, 0x6C};\n'
        '  used = matchGeneric(results->rawbuf + offset, results->state + pos,'
        '\n'
        '                      results->rawlen - offset, 32,\n'
        '                      kFOOHdrMark, kFOOHdrSpace,\n'
        '                      kFOOBitMark, kFOOOneSpace,\n'
        '                      kFOOBitMark, kFOOZeroSpace,\n'
        '                      kFOOHdrMark, kFOOHdrSpace, true);\n'
        '  if (used == 0) return false;  // We failed to find any data.\n'
        '  offset += used;  // Adjust for how much of the message we read.\n'
        '  pos += 4;  // Adjust by how many bytes of data we read\n'
        '\n'
        '  // Success\n'
        '  results->decode_type = decode_type_t::FOO;\n'
        '  results->bits = nbits;\n'
        '  return true;\n'
        '}\n'
        '#endif  // DECODE_FOO\n')

  def test_no_headers(self):
    """Tests for no space or mark headers in parse_and_report() function."""

    # Tests for mark or space headers. (Issue #1014)
    output = StringIO()
    input_str = """
      uint16_t rawData[257] = {472, 1016, 490, 536, 446, 1038, 464, 544, 490,
        516, 492, 1008, 418, 592, 462, 1042, 476, 532, 444, 1062, 474, 532, 470,
        1014, 492, 1010, 446, 562, 460, 1046, 474, 532, 472, 534, 416, 590, 458,
        548, 486, 520, 490, 516, 490, 534, 470, 534, 470, 534, 470, 536, 416,
        590, 460, 546, 488, 518, 490, 536, 468, 536, 470, 534, 470, 536, 442,
        564, 414, 1092, 470, 536, 468, 536, 416, 590, 414, 592, 486, 520, 490,
        536, 470, 534, 470, 534, 468, 536, 416, 590, 432, 574, 486, 520, 490,
        536, 470, 534, 468, 536, 468, 536, 468, 538, 420, 590, 454, 546, 488,
        518, 488, 536, 468, 536, 468, 536, 468, 536, 440, 566, 414, 592, 462,
        546, 490, 536, 468, 536, 468, 538, 468, 536, 468, 538, 414, 592, 460,
        546, 488, 518, 490, 536, 468, 536, 470, 536, 468, 536, 442, 564, 414,
        592, 462, 546, 490, 518, 488, 536, 470, 534, 470, 536, 470, 536, 416,
        590, 460, 548, 488, 518, 490, 536, 470, 534, 470, 534, 470, 536, 468,
        536, 414, 592, 462, 546, 490, 518, 488, 534, 470, 536, 468, 536, 468,
        536, 414, 590, 462, 546, 488, 518, 466, 560, 444, 560, 446, 560, 446,
        560, 444, 562, 416, 592, 462, 546, 464, 542, 464, 560, 444, 560, 446,
        560, 446, 560, 416, 590, 460, 546, 464, 544, 464, 562, 444, 560, 446,
        560, 446, 560, 444, 560, 416, 592, 462, 1042, 446, 560, 444, 560, 416,
        592, 462, 544, 488, 520, 466, 558, 446, 560, 446};"""
    analyse.parse_and_report(input_str, 200, True, "", output)
    self.assertEqual(
        output.getvalue(), 'Found 257 timing entries.\n'
        'Potential Mark Candidates:\n'
        '[492]\n'
        'Potential Space Candidates:\n'
        '[1092, 592]\n'
        '\n'
        'Guessing encoding type:\n'
        'Looks like it uses space encoding. Yay!\n'
        '\n'
        'Guessing key value:\n'
        'kHdrMark   = 0\n'
        'kHdrSpace  = 0\n'
        'kBitMark   = 460\n'
        'kOneSpace  = 1037\n'
        'kZeroSpace = 547\n'
        '\n'
        'Decoding protocol based on analysis so far:\n'
        '\n'
        'kBitMark(UNEXPECTED)10100101010110100000000000000000010000000000000000'
        '0000000000000000000000000000000000000000000000000000000000000000000000'
        '10000000\n'
        '  Bits: 128\n'
        '  Hex:  0xA55A0000400000000000000000000080 (MSB first)\n'
        '        0x01000000000000000000000200005AA5 (LSB first)\n'
        '  Dec:  219789926041586294144261994014272651392 (MSB first)\n'
        '        1329227995784915872903807068870302373 (LSB first)\n'
        '  Bin:  0b101001010101101000000000000000000100000000000000000000000000'
        '00000000000000000000000000000000000000000000000000000000000010000000'
        ' (MSB first)\n'
        '        0b000000010000000000000000000000000000000000000000000000000000'
        '00000000000000000000000000000000001000000000000000000101101010100101'
        ' (LSB first)\n'
        '\n'
        'Total Nr. of suspected bits: 128\n'
        '\n'
        'Generating a VERY rough code outline:\n'
        '\n'
        '// Copyright 2019 David Conran (crankyoldgit)\n'
        '// Support for TBD protocol\n'
        '\n'
        '#include "IRrecv.h"\n'
        '#include "IRsend.h"\n'
        '#include "IRutils.h"\n'
        '\n'
        "// WARNING: This probably isn't directly usable. It's a guide only.\n"
        '\n'
        '// See https://github.com/crankyoldgit/IRremoteESP8266/wiki/'
        'Adding-support-for-a-new-IR-protocol\n'
        '// for details of how to include this in the library.\n'
        'const uint16_t kHdrMark = 0;\n'
        'const uint16_t kBitMark = 460;\n'
        'const uint16_t kHdrSpace = 0;\n'
        'const uint16_t kOneSpace = 1037;\n'
        'const uint16_t kZeroSpace = 547;\n'
        'const uint16_t kFreq = 38000;  // Hz. (Guessing the most common'
        ' frequency.)\n'
        'const uint16_t kBits = 128;  // Move to IRremoteESP8266.h\n'
        'const uint16_t kStateLength = 16;  // Move to IRremoteESP8266.h\n'
        'const uint16_t kOverhead = 1;\n'
        "// DANGER: More than 64 bits detected. A uint64_t for 'data' won't"
        ' work!\n'
        '#if SEND_TBD\n'
        '// Function should be safe up to 64 bits.\n'
        'void IRsend::sendTBD(const uint64_t data, const uint16_t nbits, const'
        ' uint16_t repeat) {\n'
        '  enableIROut(kFreq);\n'
        '  for (uint16_t r = 0; r <= repeat; r++) {\n'
        '    uint64_t send_data = data;\n'
        '    // Data Section #1\n'
        '    // e.g. data = 0xA55A0000400000000000000000000080, nbits = 128\n'
        '    sendData(kBitMark, kOneSpace, kBitMark, kZeroSpace, send_data,'
        ' 128, true);\n'
        '    send_data >>= 128;\n'
        '    // Footer\n'
        '    mark(kBitMark);\n'
        '    space(kDefaultMessageGap);  // A 100% made up guess of the gap'
        ' between messages.\n'
        '  }\n'
        '}\n'
        '#endif  // SEND_TBD\n'
        '\n'
        '#if SEND_TBD\n'
        '// Alternative >64bit function to send TBD messages\n'
        '// Where data is:\n'
        '//   uint8_t data[kStateLength] = {0xA5, 0x5A, 0x00, 0x00, 0x40, 0x00,'
        ' 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80};\n'
        '//\n'
        '// Args:\n'
        '//   data: An array of bytes containing the IR command.\n'
        '//         It is assumed to be in MSB order for this code.\n'
        '//   nbytes: Nr. of bytes of data in the array. (>=kStateLength)\n'
        '//   repeat: Nr. of times the message is to be repeated.\n'
        '//\n'
        '// Status: ALPHA / Untested.\n'
        'void IRsend::sendTBD(const uint8_t data[], const uint16_t nbytes,'
        ' const uint16_t repeat) {\n'
        '  for (uint16_t r = 0; r <= repeat; r++) {\n'
        '    uint16_t pos = 0;\n'
        '    // Data Section #1\n'
        '    // e.g.\n'
        '    //   bits = 128; bytes = 16;\n'
        '    //   *(data + pos) = {0xA5, 0x5A, 0x00, 0x00, 0x40, 0x00, 0x00,'
        ' 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80};\n'
        '    sendGeneric(0, 0,\n'
        '                kBitMark, kOneSpace,\n'
        '                kBitMark, kZeroSpace,\n'
        '                kBitMark, kDefaultMessageGap,\n'
        '                data + pos, 16,  // Bytes\n'
        '                kFreq, true, kNoRepeat, kDutyDefault);\n'
        '    pos += 16;  // Adjust by how many bytes of data we sent\n'
        '  }\n'
        '}\n'
        '#endif  // SEND_TBD\n'
        '\n'
        "// DANGER: More than 64 bits detected. A uint64_t for 'data' won't"
        ' work!\n'
        '#if DECODE_TBD\n'
        '// Function should be safe up to 64 bits.\n'
        'bool IRrecv::decodeTBD(decode_results *results, const uint16_t nbits,'
        ' const bool strict) {\n'
        '  if (results->rawlen < 2 * nbits + kOverhead)\n'
        '    return false;  // Too short a message to match.\n'
        '  if (strict && nbits != kBits)\n'
        '    return false;\n'
        '\n'
        '  uint16_t offset = kStartOffset;\n'
        '  uint64_t data = 0;\n'
        '  match_result_t data_result;\n'
        '\n'
        '  // Data Section #1\n'
        '  // e.g. data_result.data = 0xA55A0000400000000000000000000080, nbits'
        ' = 128\n'
        '  data_result = matchData(&(results->rawbuf[offset]), 128,\n'
        '                          kBitMark, kOneSpace,\n'
        '                          kBitMark, kZeroSpace);\n'
        '  offset += data_result.used;\n'
        '  if (data_result.success == false) return false;  // Fail\n'
        '  data <<= 128;  // Make room for the new bits of data.\n'
        '  data |= data_result.data;\n'
        '\n'
        '  // Footer\n'
        '  if (!matchMark(results->rawbuf[offset++], kBitMark))\n'
        '    return false;\n'
        '\n'
        '  // Success\n'
        '  results->decode_type = decode_type_t::TBD;\n'
        '  results->bits = nbits;\n'
        '  results->value = data;\n'
        '  results->command = 0;\n'
        '  results->address = 0;\n'
        '  return true;\n'
        '}\n'
        '#endif  // DECODE_TBD\n'
        '\n'
        '// Note: This should be 64+ bit safe.\n'
        '#if DECODE_TBD\n'
        '// Function should be safe over 64 bits.\n'
        'bool IRrecv::decodeTBD(decode_results *results, const uint16_t nbits,'
        ' const bool strict) {\n'
        '  if (results->rawlen < 2 * nbits + kOverhead)\n'
        '    return false;  // Too short a message to match.\n'
        '  if (strict && nbits != kBits)\n'
        '    return false;\n'
        '\n'
        '  uint16_t offset = kStartOffset;\n'
        '  uint16_t pos = 0;\n'
        '  uint16_t used = 0;\n'
        '\n'
        '  // Data Section #1\n'
        '  // e.g.\n'
        '  //   bits = 128; bytes = 16;\n'
        '  //   *(results->state + pos) = {0xA5, 0x5A, 0x00, 0x00, 0x40, 0x00,'
        ' 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80};\n'
        '  used = matchGeneric(results->rawbuf + offset, results->state + pos,'
        '\n'
        '                      results->rawlen - offset, 128,\n'
        '                      0, 0,\n'
        '                      kBitMark, kOneSpace,\n'
        '                      kBitMark, kZeroSpace,\n'
        '                      kBitMark, kDefaultMessageGap, true);\n'
        '  if (used == 0) return false;  // We failed to find any data.\n'
        '  offset += used;  // Adjust for how much of the message we read.\n'
        '  pos += 16;  // Adjust by how many bytes of data we read\n'
        '\n'
        '  // Success\n'
        '  results->decode_type = decode_type_t::TBD;\n'
        '  results->bits = nbits;\n'
        '  return true;\n'
        '}\n'
        '#endif  // DECODE_TBD\n')


if __name__ == '__main__':
  unittest.main(verbosity=2)
