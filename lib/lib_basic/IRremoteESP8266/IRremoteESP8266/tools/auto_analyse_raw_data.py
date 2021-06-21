#!/usr/bin/python
"""Attempt an automatic analysis of IRremoteESP8266's Raw data output.
   Makes suggestions on key values and tried to break down the message
   into likely chunks."""
#
# Copyright 2018 David Conran
import argparse
import sys

SAFE64NOTE = "--safe64note--"
CODEGEN = "--codegen--"

class RawIRMessage():
  """Basic analyse functions & structure for raw IR messages."""

  # pylint: disable=too-many-instance-attributes

  def __init__(self, margin, timings, output=sys.stdout, verbose=True):
    self.ldr_mark = None
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
    self.section_count = 1
    self.rawlen = len(timings)
    if self.rawlen <= 3:
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
    if expected is None:
      return False
    return expected - self.margin < seen <= expected

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
                      (bits, ("0x{0:0%dX}" % (bits / 4)).format(num),
                       ("0x{0:0%dX}" % (bits / 4)).format(rev_num), num,
                       rev_num, binary_str, rev_binary_str))

  def add_data_code(self, bin_str, name="", footer=True):
    """Add the common "data" sequence of code to send the bulk of a message."""
    # pylint: disable=no-self-use
    code = []
    nbits = len(bin_str)
    code.append("    // Data Section #%d" % self.section_count)
    code.append("    // e.g. data = 0x%X, nbits = %d" % (int(bin_str, 2),
                                                         nbits))
    code.append("    sendData(k%sBitMark, k%sOneSpace, k%sBitMark, "
                "k%sZeroSpace, send_data, %d, true);" %
                (name, name, name, name, nbits))
    code.append("    send_data >>= %d;" % nbits)
    if footer:
      code.append("    // Footer")
      code.append("    mark(k%sBitMark);" % name)
    return code

  def add_data_decode_code(self, bin_str, name="", footer=True):
    """Add the common "data" sequence code to decode the bulk of a message."""
    # pylint: disable=no-self-use
    code = []
    nbits = len(bin_str)
    code.extend([
        "",
        "  // Data Section #%d" % self.section_count,
        "  // e.g. data_result.data = 0x%X, nbits = %d" % (int(bin_str, 2),
                                                           nbits),
        "  data_result = matchData(&(results->rawbuf[offset]), %s," % nbits,
        "                          k%sBitMark, k%sOneSpace," % (name, name),
        "                          k%sBitMark, k%sZeroSpace);" % (name, name),
        "  offset += data_result.used;",
        "  if (data_result.success == false) return false;  // Fail",
        "  data <<= %s;  // Make room for the new bits of data." % nbits,
        "  data |= data_result.data;"])
    if footer:
      code.extend([
          "",
          "  // Footer",
          "  if (!matchMark(results->rawbuf[offset++], k%sBitMark))" % name,
          "    return false;"])
    return code

  def add_data_byte_code(self, bin_str, name="", ambles=None):
    """Add the code to send the data from an array."""
    # pylint: disable=no-self-use
    code = []
    nbits = len(bin_str)
    nbytes = nbits / 8
    if ambles is None:
      ambles = {}
    firstmark = ambles.get("firstmark", 0)
    firstspace = ambles.get("firstspace", 0)
    lastmark = ambles.get("lastmark", "k%sBitMark" % name)
    lastspace = ambles.get("lastspace", "kDefaultMessageGap")
    code.append(
        "    // Data Section #%d" % self.section_count)
    if nbits % 8:
      code.append("    // DANGER: Nr. of bits is not a multiple of 8. "
                  "This section won't work!")
    code.extend([
        "    // e.g.",
        "    //   bits = %d; bytes = %d;" % (nbits, nbytes),
        "    //   *(data + pos) = {0x%s};" % (
            ", 0x".join("%02X" % int(bin_str[i:i + 8], 2)
                        for i in range(0, len(bin_str), 8))),
        "    sendGeneric(%s, %s," % (firstmark, firstspace),
        "                k%sBitMark, k%sOneSpace," % (name, name),
        "                k%sBitMark, k%sZeroSpace," % (name, name),
        "                %s, %s," % (lastmark, lastspace),
        "                data + pos, %d,  // Bytes" % nbytes,
        "                k%sFreq, true, kNoRepeat, kDutyDefault);" % name,
        "    pos += %d;  // Adjust by how many bytes of data we sent" % nbytes])
    return code

  def add_data_byte_decode_code(self, bin_str, name="", ambles=None):
    """Add the common byte-wise "data" sequence decode code."""
    # pylint: disable=no-self-use
    code = []
    nbits = len(bin_str)
    nbytes = nbits / 8
    if nbits % 8:
      code.append("  // WARNING: Nr. of bits is not a multiple of 8. "
                  "This section won't work!")
    if ambles is None:
      ambles = {}
    firstmark = ambles.get("firstmark", 0)
    firstspace = ambles.get("firstspace", 0)
    lastmark = ambles.get("lastmark", "k%sBitMark" % name)
    lastspace = ambles.get("lastspace", "kDefaultMessageGap")

    code.extend([
        "",
        "  // Data Section #%d" % self.section_count,
        "  // e.g.",
        "  //   bits = %d; bytes = %d;" % (nbits, nbytes),
        "  //   *(results->state + pos) = {0x%s};" % (
            ", 0x".join("%02X" % int(bin_str[i:i + 8], 2)
                        for i in range(0, len(bin_str), 8))),
        "  used = matchGeneric(results->rawbuf + offset, results->state + pos,",
        "                      results->rawlen - offset, %d," % nbits,
        "                      %s, %s," % (firstmark, firstspace),
        "                      k%sBitMark, k%sOneSpace," % (name, name),
        "                      k%sBitMark, k%sZeroSpace," % (name, name),
        "                      %s, %s, true);" % (lastmark, lastspace),
        "  if (used == 0) return false;  // We failed to find any data.",
        "  offset += used;  // Adjust for how much of the message we read.",
        "  pos += %d;  // Adjust by how many bytes of data we read" % nbytes])
    return code

  def _calc_values(self):
    """Calculate the values which describe the standard timings
       for the protocol."""
    if self.verbose:
      self.output.write("Potential Mark Candidates:\n"
                        "%s\n"
                        "Potential Space Candidates:\n"
                        "%s\n" % (str(self.marks), str(self.spaces)))
    # The bit mark is likely to be the smallest mark.
    self.bit_mark = self.marks[-1]
    if len(self.marks) > 2:  # Possible leader mark?
      self.ldr_mark = self.marks[0]
      self.hdr_mark = self.marks[1]
    elif len(self.marks) > 1:  # At least two marks
      # Largest mark is likely the kHdrMark
      self.hdr_mark = self.marks[0]
    else:
      # Probably no header mark.
      self.hdr_mark = 0

    if self.is_space_encoded() and len(self.spaces) >= 2:
      if self.verbose and len(self.marks) > 2:
        self.output.write("DANGER: Unusual number of mark timings!")
      # We should have 3 space candidates at least.
      # They should be: zero_space (smallest), one_space, & hdr_space (largest)
      spaces = list(self.spaces)
      if spaces:
        self.zero_space = spaces.pop()
      if spaces:
        self.one_space = spaces.pop()
      if spaces:
        self.hdr_space = spaces.pop()
      # Rest are probably message gaps
      self.gaps = spaces

  def is_space_encoded(self):
    """Make an educated guess if the message is space encoded."""
    return len(self.spaces) > len(self.marks)

  def is_ldr_mark(self, usec):
    """Is usec the leader mark?"""
    if self.ldr_mark is None:
      return False
    return self._usec_compare(usec, self.ldr_mark)

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
    return int(sum(items) / len(items))
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


def dump_constants(message, defines, name="", output=sys.stdout):
  """Dump the key constants and generate the C++ #defines."""
  ldr_mark = None
  hdr_mark = 0
  hdr_space = 0
  if message.ldr_mark is not None:
    ldr_mark = avg_list(message.mark_buckets[message.ldr_mark])
  if message.hdr_mark != 0:
    hdr_mark = avg_list(message.mark_buckets[message.hdr_mark])
  bit_mark = avg_list(message.mark_buckets[message.bit_mark])
  if message.hdr_space is not None:
    hdr_space = avg_list(message.space_buckets[message.hdr_space])
  one_space = avg_list(message.space_buckets[message.one_space])
  zero_space = avg_list(message.space_buckets[message.zero_space])

  output.write("Guessing key value:\n"
               "k%sHdrMark   = %d\n"
               "k%sHdrSpace  = %d\n"
               "k%sBitMark   = %d\n"
               "k%sOneSpace  = %d\n"
               "k%sZeroSpace = %d\n" % (name, hdr_mark, name, hdr_space,
                                        name, bit_mark, name, one_space,
                                        name, zero_space))
  defines.append("const uint16_t k%sHdrMark = %d;" % (name, hdr_mark))
  defines.append("const uint16_t k%sBitMark = %d;" % (name, bit_mark))
  defines.append("const uint16_t k%sHdrSpace = %d;" % (name, hdr_space))
  defines.append("const uint16_t k%sOneSpace = %d;" % (name, one_space))
  defines.append("const uint16_t k%sZeroSpace = %d;" % (name, zero_space))
  if ldr_mark:
    output.write("k%sLdrMark   = %d\n" % (name, ldr_mark))
    defines.append("const uint16_t k%sLdrMark = %d;" % (name, ldr_mark))

  avg_gaps = [avg_list(message.space_buckets[x]) for x in message.gaps]
  if len(message.gaps) == 1:
    output.write("k%sSpaceGap = %d\n" % (name, avg_gaps[0]))
    defines.append("const uint16_t k%sSpaceGap = %d;" % (name, avg_gaps[0]))
  else:
    count = 0
    for gap in avg_gaps:
      # We probably (still) have a gap in the protocol.
      count = count + 1
      output.write("k%sSpaceGap%d = %d\n" % (name, count, gap))
      defines.append("const uint16_t k%sSpaceGap%d = %d;" % (name, count, gap))
  defines.append("const uint16_t k%sFreq = 38000;  "
                 "// Hz. (Guessing the most common frequency.)" % name)


def parse_and_report(rawdata_str, margin, gen_code=False, name="",
                     output=sys.stdout):
  """Analyse the rawdata c++ definition of a IR message."""
  defines = []
  code = {}
  code["sendcomhead"] = []
  code["send"] = []
  code["send64+"] = []
  code["sendcomfoot"] = []
  code["recvcomhead"] = []
  code["recv"] = []
  code["recv64+"] = []
  code["recvcomfoot"] = []

  # Parse the input.
  rawdata = convert_rawdata(rawdata_str)

  output.write("Found %d timing entries.\n" % len(rawdata))

  message = RawIRMessage(margin, rawdata, output)
  output.write("\nGuessing encoding type:\n")
  if message.is_space_encoded():
    output.write("Looks like it uses space encoding. Yay!\n\n")
    dump_constants(message, defines, name, output)
  else:
    output.write("Sorry, it looks like it is Mark encoded. "
                 "I can't do that yet. Exiting.\n")
    sys.exit(1)
  total_bits = decode_data(message, defines, code, name, output)
  if gen_code:
    generate_code(defines, code, total_bits, name, output)


def decode_data(message, defines, code, name="", output=sys.stdout):
  """Decode the data sequence with the given values in mind."""
  # pylint: disable=too-many-branches,too-many-statements

  # Now we have likely candidates for the key values, go through the original
  # sequence and break it up and indicate accordingly.

  output.write("\nDecoding protocol based on analysis so far:\n\n")
  state = ""
  code_info = {}
  count = 1
  total_bits = ""
  binary_value = binary64_value = add_bit("", "reset")
  if name:
    def_name = name
  else:
    def_name = "TBD"

  code["sendcomhead"].extend([
      "",
      "#if SEND_%s" % def_name.upper(),
      SAFE64NOTE,
      "/// Send a %s formatted message." % name,
      "/// Status: ALPHA / Untested."])
  code["send"].extend([
      "/// @param[in] data containing the IR command.",
      "/// @param[in] nbits Nr. of bits to send. usually k%sBits" % name,
      "/// @param[in] repeat Nr. of times the message is to be repeated.",
      "void IRsend::send%s(const uint64_t data, const uint16_t"
      " nbits, const uint16_t repeat) {" % def_name,
      "  enableIROut(k%sFreq);" % name,
      "  for (uint16_t r = 0; r <= repeat; r++) {",
      "    uint64_t send_data = data;"])
  code["send64+"].extend([
      "/// @param[in] data An array of bytes containing the IR command.",
      "///                 It is assumed to be in MSB order for this code.",
      "/// e.g.",
      "/// @code",
      CODEGEN,
      "/// @endcode",
      "/// @param[in] nbytes Nr. of bytes of data in the array."
      " (>=k%sStateLength)" % name,
      "/// @param[in] repeat Nr. of times the message is to be repeated.",
      "void IRsend::send%s(const uint8_t data[], const uint16_t nbytes,"
      " const uint16_t repeat) {" % def_name,
      "  for (uint16_t r = 0; r <= repeat; r++) {",
      "    uint16_t pos = 0;"])
  code["sendcomfoot"].extend([
      "  }",
      "}",
      "#endif  // SEND_%s" % def_name.upper()])
  code["recvcomhead"].extend([
      "",
      "#if DECODE_%s" % def_name.upper(),
      SAFE64NOTE,
      "/// Decode the supplied %s message." % name,
      "/// Status: ALPHA / Untested.",
      "/// @param[in,out] results Ptr to the data to decode &"
      " where to store the decode",
      "/// @param[in] offset The starting index to use when"
      " attempting to decode the",
      "///   raw data. Typically/Defaults to kStartOffset.",
      "/// @param[in] nbits The number of data bits to expect.",
      "/// @param[in] strict Flag indicating if we should perform strict"
      " matching.",
      "/// @return A boolean. True if it can decode it, false if it can't.",
      "bool IRrecv::decode%s(decode_results *results, uint16_t offset,"
      " const uint16_t nbits, const bool strict) {" % def_name,
      "  if (results->rawlen < 2 * nbits + k%sOverhead - offset)" % name,
      "    return false;  // Too short a message to match.",
      "  if (strict && nbits != k%sBits)" % name,
      "    return false;",
      ""])
  code["recv"].extend([
      "  uint64_t data = 0;",
      "  match_result_t data_result;"])
  code["recv64+"].extend([
      "  uint16_t pos = 0;",
      "  uint16_t used = 0;"])
  code["recvcomfoot"].extend([
      "  return true;",
      "}",
      "#endif  // DECODE_%s" % def_name.upper()])

  # states are:
  #  HM:  Header/Leader mark
  #  HS:  Header space
  #  BM:  Bit mark
  #  BS:  Bit space
  #  GS:  Gap space
  #  UNK: Unknown state.
  for usec in message.timings:
    # Handle header/leader marks.
    if ((message.is_hdr_mark(usec) or message.is_ldr_mark(usec)) and
        count % 2 and not message.is_bit_mark(usec)):
      state = "HM"
      if message.is_hdr_mark(usec):
        mark_type = "H"  # Header
      else:
        mark_type = "L"  # Leader
      if binary_value:
        message.display_binary(binary_value)
        code["send"].extend(message.add_data_code(binary_value, name, False))
        code["recv"].extend(message.add_data_decode_code(binary_value, name,
                                                         False))
        message.section_count = message.section_count + 1
        code_info["lastmark"] = "k%s%sdrMark" % (name, mark_type)
        total_bits = total_bits + binary_value
      code_info["firstmark"] = "k%s%sdrMark" % (name, mark_type)
      binary_value = add_bit(binary_value, "reset")
      output.write("k%s%sdrMark+" % (name, mark_type))
      code["send"].extend(["    // %seader" % mark_type,
                           "    mark(k%s%sdrMark);" % (name, mark_type)])
      code["recv"].extend([
          "",
          "  // %seader" % mark_type,
          "  if (!matchMark(results->rawbuf[offset++], k%s%sdrMark))" % (
              name, mark_type),
          "    return false;"])

    # Handle header spaces.
    elif message.is_hdr_space(usec) and not message.is_one_space(usec):
      if binary64_value:
        code_info["lastspace"] = "k%sHdrSpace" % name
        message.section_count = message.section_count - 1
        code["send64+"].extend(message.add_data_byte_code(binary64_value, name,
                                                          code_info))
        code["recv64+"].extend(message.add_data_byte_decode_code(binary64_value,
                                                                 name,
                                                                 code_info))
        code_info.clear()
        binary64_value = binary_value
        message.section_count = message.section_count + 1
      if state != "HM":
        if binary_value:  # If we we are in a header and we have data, add it.
          message.display_binary(binary_value)
          total_bits = total_bits + binary_value
          code["send"].extend(message.add_data_code(binary_value, name))
          code["recv"].extend(message.add_data_decode_code(binary_value, name))
          code_info["lastspace"] = "k%sHdrSpace" % name
          message.section_count = message.section_count + 1
        binary_value = binary64_value = add_bit(binary_value, "reset")
        output.write("UNEXPECTED->")
      state = "HS"
      output.write("k%sHdrSpace+" % name)
      code["send"].append("    space(k%sHdrSpace);" % name)
      code["recv"].extend([
          "  if (!matchSpace(results->rawbuf[offset++], k%sHdrSpace))" % name,
          "    return false;"])
      code_info["firstspace"] = "k%sHdrSpace" % name
    # Handle bit marks.
    elif message.is_bit_mark(usec) and count % 2:
      if state not in ("HS", "BS"):
        output.write("k%sBitMark(UNEXPECTED)" % name)
      state = "BM"
    # Handle "zero" spaces
    elif message.is_zero_space(usec):
      if state != "BM":
        output.write("k%sZeroSpace(UNEXPECTED)" % name)
      state = "BS"
      binary_value = binary64_value = add_bit(binary_value, 0, output)
    # Handle "one" spaces
    elif message.is_one_space(usec):
      if state != "BM":
        output.write("k%sOneSpace(UNEXPECTED)" % name)
      state = "BS"
      binary_value = binary64_value = add_bit(binary_value, 1, output)
    elif message.is_gap(usec):
      if state != "BM":
        output.write("UNEXPECTED->")
      output.write("GAP(%d)" % usec)
      code_info["lastspace"] = "k%sSpaceGap" % name
      if binary64_value:
        code["send64+"].extend(message.add_data_byte_code(binary64_value, name,
                                                          code_info))
        code["recv64+"].extend(message.add_data_byte_decode_code(binary64_value,
                                                                 name,
                                                                 code_info))
        code_info.clear()
      if binary_value:
        message.display_binary(binary_value)
        code["send"].extend(message.add_data_code(binary_value, name))
        code["recv"].extend(message.add_data_decode_code(binary_value, name))
        message.section_count = message.section_count + 1
      else:
        code["recv"].extend(["",
                             "  // Gap"])
        code["send"].extend(["    // Gap"])
        if state == "BM":
          code["send"].extend(["    mark(k%sBitMark);" % name])
          code["recv"].extend([
              "  if (!matchMark(results->rawbuf[offset++], k%sBitMark))" % name,
              "    return false;"])
      code["send"].append("    space(k%sSpaceGap);" % name)
      code["recv"].extend([
          "  if (!matchSpace(results->rawbuf[offset++], k%sSpaceGap))" % name,
          "    return false;"])
      total_bits = total_bits + binary_value
      binary_value = binary64_value = add_bit(binary_value, "reset")
      state = "GS"
    else:
      output.write("UNKNOWN(%d)" % usec)
      state = "UNK"
    count = count + 1
  if binary64_value:
    code["send64+"].extend(message.add_data_byte_code(binary64_value, name,
                                                      code_info))
    code["recv64+"].extend(message.add_data_byte_decode_code(binary64_value,
                                                             name, code_info))
    code_info.clear()
  if binary_value:
    message.display_binary(binary_value)
    code["send"].extend(message.add_data_code(binary_value, name))
    code["recv"].extend(message.add_data_decode_code(binary_value, name))
    message.section_count = message.section_count + 1
  code["send"].extend([
      "    space(kDefaultMessageGap);  // A 100% made up guess of the gap"
      " between messages."])
  code["recv"].extend([
      "",
      "  // Success",
      "  results->decode_type = decode_type_t::%s;" % def_name.upper(),
      "  results->bits = nbits;",
      "  results->value = data;",
      "  results->command = 0;",
      "  results->address = 0;"])
  code["recv64+"].extend([
      "",
      "  // Success",
      "  results->decode_type = decode_type_t::%s;" % def_name.upper(),
      "  results->bits = nbits;"])

  total_bits = total_bits + binary_value
  output.write("\nTotal Nr. of suspected bits: %d\n" % len(total_bits))
  defines.append("const uint16_t k%sBits = %d;"
                 "  // Move to IRremoteESP8266.h" % (name, len(total_bits)))
  if len(total_bits) > 64:
    defines.append("const uint16_t k%sStateLength = %d;"
                   "  // Move to IRremoteESP8266.h" %
                   (name, len(total_bits) / 8))
  defines.append("const uint16_t k%sOverhead = %d;" %
                 (name, message.rawlen - 2 * len(total_bits)))
  return total_bits


def generate_code(defines, code, bits_str, name="", output=sys.stdout):
  """Output the estimated C++ code to reproduce & decode the IR message."""
  # pylint: disable=too-many-branches
  if name:
    def_name = name
  else:
    def_name = "TBD"
  output.write("\nGenerating a VERY rough code outline:\n\n"
               "// Copyright 2020 David Conran (crankyoldgit)\n"
               "/// @file\n"
               "/// @brief Support for %s protocol\n\n"
               "// Supports:\n"
               "//   Brand: %s,  Model: TODO add device and remote\n\n"
               '#include "IRrecv.h"\n'
               '#include "IRsend.h"\n'
               '#include "IRutils.h"\n\n'
               "// WARNING: This probably isn't directly usable."
               " It's a guide only.\n\n"
               "// See https://github.com/crankyoldgit/IRremoteESP8266/wiki/"
               "Adding-support-for-a-new-IR-protocol\n"
               "// for details of how to include this in the library."
               "\n" % (def_name, def_name))
  for line in defines:
    output.write("%s\n" % line)

  if len(bits_str) > 64:  # Will it fit in a uint64_t?
    output.write("// DANGER: More than 64 bits detected. A uint64_t for "
                 "'data' won't work!\n")
  # Display the "normal" version's send code incase there are some
  # oddities in it.
  for line in code["sendcomhead"] + code["send"] + code["sendcomfoot"]:
    if line == SAFE64NOTE:
      line = "// Function should be safe up to 64 bits."
    output.write("%s\n" % line)

  if len(bits_str) > 64:  # Will it fit in a uint64_t?
    for line in code["sendcomhead"] + code["send64+"] + code["sendcomfoot"]:
      if line == SAFE64NOTE:
        line = "// Alternative >64bit function to send %s messages\n" % \
            def_name.upper() + "// Function should be safe over 64 bits."
      elif line == CODEGEN:
        line = "///   uint8_t data[k%sStateLength] = {0x%s};" % (
            name, ", 0x".join("%02X" % int(bits_str[i:i + 8], 2)
                              for i in range(0, len(bits_str), 8)))
      output.write("%s\n" % line)
  if len(bits_str) > 64:  # Will it fit in a uint64_t?
    output.write("\n// DANGER: More than 64 bits detected. A uint64_t for "
                 "'data' won't work!")

  # Display the "normal" version's decode code incase there are some
  # oddities in it.
  for line in code["recvcomhead"] + code["recv"] + code["recvcomfoot"]:
    if line == SAFE64NOTE:
      line = "// Function should be safe up to 64 bits."
    output.write("%s\n" % line)

  # Display the > 64bit version's decode code
  if len(bits_str) > 64:  # Is it too big for a uint64_t?
    if len(bits_str) % 8:
      output.write("\n// WARNING: Data is not a multiple of bytes. "
                   "This won't work!\n")
    for line in code["recvcomhead"] + code["recv64+"] + code["recvcomfoot"]:
      if line == SAFE64NOTE:
        line = "// Function should be safe over 64 bits."
      output.write("%s\n" % line)

def add_rawdata_args(parser):
  """Add the arguments for feeding in the rawdata string(s)."""
  arg_group = parser.add_mutually_exclusive_group(required=True)
  arg_group.add_argument(
      "rawdata",
      help="A rawData line from IRrecvDumpV2+. e.g. 'uint16_t rawbuf[37] = {"
      "7930, 3952, 494, 1482, 520, 1482, 494, 1508, 494, 520, 494, 1482, 494, "
      "520, 494, 1482, 494, 1482, 494, 3978, 494, 520, 494, 520, 494, 520, "
      "494, 520, 520, 520, 494, 520, 494, 520, 494, 520, 494};'",
      nargs="?")
  arg_group.add_argument(
      "-f", "--file", help="Read in a rawData line from the file.")
  arg_group.add_argument(
      "--stdin",
      help="Read in a rawData line from STDIN.",
      action="store_true",
      default=False)

def get_rawdata(arg_options):
  """Return the rawdata string(s) as per the options."""
  if arg_options.stdin:
    return sys.stdin.read()
  if arg_options.file:
    with open(arg_options.file) as input_file:
      return input_file.read()
  else:
    return arg_options.rawdata


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
  arg_parser.add_argument(
      "-n",
      "--name",
      help="Name of the protocol/device to use in code generation. E.g. Onkyo",
      dest="name",
      default="")
  arg_parser.add_argument(
      "-r",
      "--range",
      type=int,
      help="Max number of micro-seconds difference between values to consider"
      " it the same value.",
      dest="margin",
      default=200)
  add_rawdata_args(arg_parser)
  arg_options = arg_parser.parse_args()

  raw_data = get_rawdata(arg_options).strip()
  if not raw_data:
    arg_parser.print_help(sys.stderr)
    sys.stderr.write("error: no rawdata content\n")
    sys.exit(1)

  parse_and_report(raw_data, arg_options.margin,
                   arg_options.gen_code, arg_options.name)


if __name__ == '__main__':
  main()
