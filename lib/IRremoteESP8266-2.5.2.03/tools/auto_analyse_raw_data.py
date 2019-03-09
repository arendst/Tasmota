#!/usr/bin/python
"""Attempt an automatic analysis of IRremoteESP8266's Raw data output.
   Makes suggestions on key values and tried to break down the message
   into likely chunks."""
#
# Copyright 2018 David Conran
import argparse
import sys


class RawIRMessage(object):
  """Basic analyse functions & structure for raw IR messages."""

  # pylint: disable=too-many-instance-attributes

  def __init__(self, margin, timings, output=sys.stdout, verbose=True):
    self.hdr_mark = None
    self.hdr_space = None
    self.bit_mark = None
    self.zero_space = None
    self.one_space = None
    self.gaps = []
    self.margin = margin
    self.marks = []
    self.mark_buckets = {}
    self.spaces = []
    self.space_buckets = {}
    self.output = output
    self.verbose = verbose
    if len(timings) <= 3:
      raise ValueError("Too few message timings supplied.")
    self.timings = timings
    self._generate_timing_candidates()
    self._calc_values()

  def _generate_timing_candidates(self):
    """Determine the likely values from the given data."""
    count = 0
    for usecs in self.timings:
      count = count + 1
      if count % 2:
        self.marks.append(usecs)
      else:
        self.spaces.append(usecs)
    self.marks, self.mark_buckets = self.reduce_list(self.marks)
    self.spaces, self.space_buckets = self.reduce_list(self.spaces)

  def reduce_list(self, items):
    """Reduce a list of numbers into buckets that are at least margin apart."""
    result = []
    last = -1
    buckets = {}
    for item in sorted(items, reverse=True):
      if last == -1 or item < last - self.margin:
        result.append(item)
        last = item
        buckets[last] = [item]
      else:
        buckets[last].append(item)
    return result, buckets

  def _usec_compare(self, seen, expected):
    """Compare two usec values and see if they match within a
       subtractive margin."""
    return seen <= expected and seen > expected - self.margin

  def _usec_compares(self, usecs, expecteds):
    """Compare a usec value to a list of values and return True
       if they are within a subtractive margin."""
    for expected in expecteds:
      if self._usec_compare(usecs, expected):
        return True
    return False

  def display_binary(self, binary_str):
    """Display common representations of the suppied binary string."""
    num = int(binary_str, 2)
    bits = len(binary_str)
    rev_binary_str = binary_str[::-1]
    rev_num = int(rev_binary_str, 2)
    self.output.write("\n  Bits: %d\n"
                      "  Hex:  %s (MSB first)\n"
                      "        %s (LSB first)\n"
                      "  Dec:  %s (MSB first)\n"
                      "        %s (LSB first)\n"
                      "  Bin:  0b%s (MSB first)\n"
                      "        0b%s (LSB first)\n" %
                      (bits, "0x{0:0{1}X}".format(num, bits / 4),
                       "0x{0:0{1}X}".format(rev_num, bits / 4), num, rev_num,
                       binary_str, rev_binary_str))

  def add_data_code(self, bin_str, footer=True):
    """Add the common "data" sequence of code to send the bulk of a message."""
    # pylint: disable=no-self-use
    code = []
    code.append("    // Data")
    code.append("    // e.g. data = 0x%X, nbits = %d" % (int(bin_str, 2),
                                                         len(bin_str)))
    code.append("    sendData(kBitMark, kOneSpace, kBitMark, kZeroSpace, data, "
                "nbits, true);")
    if footer:
      code.append("    // Footer")
      code.append("    mark(kBitMark);")
    return code

  def _calc_values(self):
    """Calculate the values which describe the standard timings
       for the protocol."""
    if self.verbose:
      self.output.write("Potential Mark Candidates:\n"
                        "%s\n"
                        "Potential Space Candidates:\n"
                        "%s\n" % (str(self.marks), str(self.spaces)))
    # Largest mark is likely the kHdrMark
    self.hdr_mark = self.marks[0]
    # The bit mark is likely to be the smallest mark.
    self.bit_mark = self.marks[-1]

    if self.is_space_encoded() and len(self.spaces) >= 3:
      if self.verbose and len(self.marks) > 2:
        self.output.write("DANGER: Unexpected and unused mark timings!")
      # We should have 3 space candidates at least.
      # They should be: zero_space (smallest), one_space, & hdr_space (largest)
      spaces = list(self.spaces)
      self.zero_space = spaces.pop()
      self.one_space = spaces.pop()
      self.hdr_space = spaces.pop()
      # Rest are probably message gaps
      self.gaps = spaces

  def is_space_encoded(self):
    """Make an educated guess if the message is space encoded."""
    return len(self.spaces) > len(self.marks)

  def is_hdr_mark(self, usec):
    """Is usec the header mark?"""
    return self._usec_compare(usec, self.hdr_mark)

  def is_hdr_space(self, usec):
    """Is usec the header space?"""
    return self._usec_compare(usec, self.hdr_space)

  def is_bit_mark(self, usec):
    """Is usec the bit mark?"""
    return self._usec_compare(usec, self.bit_mark)

  def is_one_space(self, usec):
    """Is usec the one space?"""
    return self._usec_compare(usec, self.one_space)

  def is_zero_space(self, usec):
    """Is usec the zero_space?"""
    return self._usec_compare(usec, self.zero_space)

  def is_gap(self, usec):
    """Is usec the a space gap?"""
    return self._usec_compares(usec, self.gaps)


def avg_list(items):
  """Return the average of a list of numbers."""
  if items:
    return sum(items) / len(items)
  return 0


def add_bit(so_far, bit, output=sys.stdout):
  """Add a bit to the end of the bits collected so far."""
  if bit == "reset":
    return ""
  output.write(str(bit))  # This effectively displays in LSB first order.
  return so_far + str(bit)  # Storing it in MSB first order.


def convert_rawdata(data_str):
  """Parse a C++ rawdata declaration into a list of values."""
  start = data_str.find('{')
  end = data_str.find('}')
  if end == -1:
    end = len(data_str)
  if start > end:
    raise ValueError("Raw Data not parsible due to parentheses placement.")
  data_str = data_str[start + 1:end]
  results = []
  for timing in [x.strip() for x in data_str.split(',')]:
    try:
      results.append(int(timing))
    except ValueError:
      raise ValueError(
          "Raw Data contains a non-numeric value of '%s'." % timing)
  return results


def dump_constants(message, defines, output=sys.stdout):
  """Dump the key constants and generate the C++ #defines."""
  hdr_mark = avg_list(message.mark_buckets[message.hdr_mark])
  bit_mark = avg_list(message.mark_buckets[message.bit_mark])
  hdr_space = avg_list(message.space_buckets[message.hdr_space])
  one_space = avg_list(message.space_buckets[message.one_space])
  zero_space = avg_list(message.space_buckets[message.zero_space])

  output.write("Guessing key value:\n"
               "kHdrMark   = %d\n"
               "kHdrSpace  = %d\n"
               "kBitMark   = %d\n"
               "kOneSpace  = %d\n"
               "kZeroSpace = %d\n" % (hdr_mark, hdr_space, bit_mark, one_space,
                                      zero_space))
  defines.append("const uint16_t kHdrMark = %d;" % hdr_mark)
  defines.append("const uint16_t kBitMark = %d;" % bit_mark)
  defines.append("const uint16_t kHdrSpace = %d;" % hdr_space)
  defines.append("const uint16_t kOneSpace = %d;" % one_space)
  defines.append("const uint16_t kZeroSpace = %d;" % zero_space)

  avg_gaps = [avg_list(message.space_buckets[x]) for x in message.gaps]
  if len(message.gaps) == 1:
    output.write("kSpaceGap = %d\n" % avg_gaps[0])
    defines.append("const uint16_t kSpaceGap = %d;" % avg_gaps[0])
  else:
    count = 0
    for gap in avg_gaps:
      # We probably (still) have a gap in the protocol.
      count = count + 1
      output.write("kSpaceGap%d = %d\n" % (count, gap))
      defines.append("const uint16_t kSpaceGap%d = %d;" % (count, gap))


def parse_and_report(rawdata_str, margin, gen_code=False, output=sys.stdout):
  """Analyse the rawdata c++ definition of a IR message."""
  defines = []
  function_code = []

  # Parse the input.
  rawdata = convert_rawdata(rawdata_str)

  output.write("Found %d timing entries.\n" % len(rawdata))

  message = RawIRMessage(margin, rawdata, output)
  output.write("\nGuessing encoding type:\n")
  if message.is_space_encoded():
    output.write("Looks like it uses space encoding. Yay!\n\n")
    dump_constants(message, defines, output)
  else:
    output.write("Sorry, it looks like it is Mark encoded. "
                 "I can't do that yet. Exiting.\n")
    sys.exit(1)
  total_bits = decode_data(message, defines, function_code, output)
  if gen_code:
    generate_irsend_code(defines, function_code, total_bits, output)


def decode_data(message, defines, function_code, output=sys.stdout):
  """Decode the data sequence with the given values in mind."""
  # pylint: disable=too-many-branches,too-many-statements

  # Now we have likely candidates for the key values, go through the original
  # sequence and break it up and indicate accordingly.

  output.write("\nDecoding protocol based on analysis so far:\n\n")
  state = ""
  count = 1
  total_bits = ""
  binary_value = add_bit("", "reset")

  function_code.extend([
      "// Function should be safe up to 64 bits.",
      "void IRsend::sendXyz(const uint64_t data, const uint16_t"
      " nbits, const uint16_t repeat) {",
      "  enableIROut(38);  // A guess. Most common frequency.",
      "  for (uint16_t r = 0; r <= repeat; r++) {"
  ])

  for usec in message.timings:
    if (message.is_hdr_mark(usec) and count % 2 and
        not message.is_bit_mark(usec)):
      state = "HM"
      if binary_value:
        message.display_binary(binary_value)
        function_code.extend(message.add_data_code(binary_value, False))
        total_bits = total_bits + binary_value
      binary_value = add_bit(binary_value, "reset")
      output.write("kHdrMark+")
      function_code.extend(["    // Header", "    mark(kHdrMark);"])
    elif message.is_hdr_space(usec) and not message.is_one_space(usec):
      if state != "HM":
        if binary_value:
          message.display_binary(binary_value)
          total_bits = total_bits + binary_value
          function_code.extend(message.add_data_code(binary_value))
        binary_value = add_bit(binary_value, "reset")
        output.write("UNEXPECTED->")
      state = "HS"
      output.write("kHdrSpace+")
      function_code.append("    space(kHdrSpace);")
    elif message.is_bit_mark(usec) and count % 2:
      if state != "HS" and state != "BS":
        output.write("kBitMark(UNEXPECTED)")
      state = "BM"
    elif message.is_zero_space(usec):
      if state != "BM":
        output.write("kZeroSpace(UNEXPECTED)")
      state = "BS"
      binary_value = add_bit(binary_value, 0, output)
    elif message.is_one_space(usec):
      if state != "BM":
        output.write("kOneSpace(UNEXPECTED)")
      state = "BS"
      binary_value = add_bit(binary_value, 1, output)
    elif message.is_gap(usec):
      if state != "BM":
        output.write("UNEXPECTED->")
      state = "GS"
      output.write("GAP(%d)" % usec)
      if binary_value:
        message.display_binary(binary_value)
        function_code.extend(message.add_data_code(binary_value))
      else:
        function_code.extend(["    // Gap", "    mark(kBitMark);"])
      function_code.append("    space(kSpaceGap);")
      total_bits = total_bits + binary_value
      binary_value = add_bit(binary_value, "reset")
    else:
      output.write("UNKNOWN(%d)" % usec)
      state = "UNK"
    count = count + 1
  if binary_value:
    message.display_binary(binary_value)
    function_code.extend(message.add_data_code(binary_value))
  function_code.extend([
      "    space(100000);  // A 100% made up guess of the gap"
      " between messages.", "  }", "}"
  ])

  total_bits = total_bits + binary_value
  output.write("\nTotal Nr. of suspected bits: %d\n" % len(total_bits))
  defines.append("const uint16_t kXyzBits = %d;" % len(total_bits))
  if len(total_bits) > 64:
    defines.append("const uint16_t kXyzStateLength = %d;" %
                   (len(total_bits) / 8))
  return total_bits


def generate_irsend_code(defines, normal, bits_str, output=sys.stdout):
  """Output the estimated C++ code to reproduce the IR message."""
  output.write("\nGenerating a VERY rough code outline:\n\n"
               "// WARNING: This probably isn't directly usable."
               " It's a guide only.\n")
  for line in defines:
    output.write("%s\n" % line)

  if len(bits_str) > 64:  # Will it fit in a uint64_t?
    output.write("// DANGER: More than 64 bits detected. A uint64_t for "
                 "'data' won't work!\n")
  # Display the "normal" version's code incase there are some
  # oddities in it.
  for line in normal:
    output.write("%s\n" % line)

  if len(bits_str) > 64:  # Will it fit in a uint64_t?
    output.write("\n\n// Alternative >64 bit Function\n"
                 "void IRsend::sendXyz(uint8_t data[], uint16_t nbytes,"
                 " uint16_t repeat) {\n"
                 "  // nbytes should typically be kXyzStateLength\n"
                 "  // data should typically be:\n"
                 "  //   uint8_t data[kXyzStateLength] = {0x%s};\n"
                 "  // data[] is assumed to be in MSB order for this code.\n"
                 "  for (uint16_t r = 0; r <= repeat; r++) {\n"
                 "    sendGeneric(kHdrMark, kHdrSpace,\n"
                 "                kBitMark, kOneSpace,\n"
                 "                kBitMark, kZeroSpace,\n"
                 "                kBitMark,\n"
                 "                100000, // 100%% made-up guess at the"
                 " message gap.\n"
                 "                data, nbytes,\n"
                 "                38000, // Complete guess of the modulation"
                 " frequency.\n"
                 "                true, 0, 50);\n"
                 "  }\n"
                 "}\n" % ", 0x".join("%02X" % int(bits_str[i:i + 8], 2)
                                     for i in range(0, len(bits_str), 8)))


def main():
  """Parse the commandline arguments and call the method."""
  arg_parser = argparse.ArgumentParser(
      description="Read an IRremoteESP8266 rawData declaration and tries to "
      "analyse it.",
      formatter_class=argparse.ArgumentDefaultsHelpFormatter)
  arg_parser.add_argument(
      "-g",
      "--code",
      action="store_true",
      default=False,
      dest="gen_code",
      help="Generate a C++ code outline to aid making an IRsend function.")
  arg_group = arg_parser.add_mutually_exclusive_group(required=True)
  arg_group.add_argument(
      "rawdata",
      help="A rawData line from IRrecvDumpV2. e.g. 'uint16_t rawbuf[37] = {"
      "7930, 3952, 494, 1482, 520, 1482, 494, 1508, 494, 520, 494, 1482, 494, "
      "520, 494, 1482, 494, 1482, 494, 3978, 494, 520, 494, 520, 494, 520, "
      "494, 520, 520, 520, 494, 520, 494, 520, 494, 520, 494};'",
      nargs="?")
  arg_group.add_argument(
      "-f", "--file", help="Read in a rawData line from the file.")
  arg_parser.add_argument(
      "-r",
      "--range",
      type=int,
      help="Max number of micro-seconds difference between values to consider"
      " it the same value.",
      dest="margin",
      default=200)
  arg_group.add_argument(
      "--stdin",
      help="Read in a rawData line from STDIN.",
      action="store_true",
      default=False)
  arg_options = arg_parser.parse_args()

  if arg_options.stdin:
    data = sys.stdin.read()
  elif arg_options.file:
    with open(arg_options.file) as input_file:
      data = input_file.read()
  else:
    data = arg_options.rawdata
  parse_and_report(data, arg_options.margin, arg_options.gen_code)


if __name__ == '__main__':
  main()
