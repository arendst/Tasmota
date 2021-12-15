#!/usr/bin/python3
"""Attempt an automatic analysis of IRremoteESP8266's Raw data output.
   Makes suggestions on key values and tried to break down the message
   into likely chunks."""
#
# Copyright 2018-2021 David Conran
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
    # pylint: disable=C0209
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
    # pylint: enable=C0209

  def add_data_code(self, bin_str, name="", footer=True):
    """Add the common "data" sequence of code to send the bulk of a message."""
    # pylint: disable=no-self-use
    code = []
    nbits = len(bin_str)
    code.append(f"    // Data Section #{self.section_count}")
    code.append(f"    // e.g. data = 0x{int(bin_str, 2):X}, nbits = {nbits}")
    code.append(f"    sendData(k{name}BitMark, k{name}OneSpace, k{name}BitMark,"
                f" k{name}ZeroSpace, send_data, {nbits}, true);")
    code.append(f"    send_data >>= {nbits};")
    if footer:
      code.append("    // Footer")
      code.append(f"    mark(k{name}BitMark);")
    return code

  def add_data_decode_code(self, bin_str, name="", footer=True):
    """Add the common "data" sequence code to decode the bulk of a message."""
    # pylint: disable=no-self-use
    code = []
    nbits = len(bin_str)
    code.extend([
        "",
        f"  // Data Section #{self.section_count}",
        f"  // e.g. data_result.data = 0x{int(bin_str, 2):X}, nbits = {nbits}",
        f"  data_result = matchData(&(results->rawbuf[offset]), {nbits},",
        f"                          k{name}BitMark, k{name}OneSpace,",
        f"                          k{name}BitMark, k{name}ZeroSpace);",
        "  offset += data_result.used;",
        "  if (data_result.success == false) return false;  // Fail",
        f"  data <<= {nbits};  // Make room for the new bits of data.",
        "  data |= data_result.data;"])
    if footer:
      code.extend([
          "",
          "  // Footer",
          f"  if (!matchMark(results->rawbuf[offset++], k{name}BitMark))",
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
    lastmark = ambles.get("lastmark", f"k{name}BitMark")
    lastspace = ambles.get("lastspace", "kDefaultMessageGap")
    code.append(f"    // Data Section #{self.section_count}")
    if nbits % 8:
      code.append("    // DANGER: Nr. of bits is not a multiple of 8. "
                  "This section won't work!")
    code.extend([
        "    // e.g.",
        f"    //   bits = {nbits}; bytes = {int(nbytes)};",
        # pylint: disable=C0209
        "    //   *(data + pos) = {0x%s};" % (
            ", 0x".join("%02X" % int(bin_str[i:i + 8], 2)
                        for i in range(0, len(bin_str), 8))),
        # pylint: enable=C0209
        f"    sendGeneric({firstmark}, {firstspace},",
        f"                k{name}BitMark, k{name}OneSpace,",
        f"                k{name}BitMark, k{name}ZeroSpace,",
        f"                {lastmark}, {lastspace},",
        f"                data + pos, {int(nbytes)},  // Bytes",
        f"                k{name}Freq, true, kNoRepeat, kDutyDefault);",
        f"    pos += {int(nbytes)};"
        f"  // Adjust by how many bytes of data we sent"])
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
    lastmark = ambles.get("lastmark", f"k{name}BitMark")
    lastspace = ambles.get("lastspace", "kDefaultMessageGap")

    code.extend([
        "",
        f"  // Data Section #{self.section_count}",
        "  // e.g.",
        f"  //   bits = {nbits}; bytes = {int(nbytes)};",
        # pylint: disable=C0209
        "  //   *(results->state + pos) = {0x%s};" % (
            ", 0x".join("%02X" % int(bin_str[i:i + 8], 2)
                        for i in range(0, len(bin_str), 8))),
        # pylint: enable=C0209
        "  used = matchGeneric(results->rawbuf + offset, results->state + pos,",
        f"                      results->rawlen - offset, {nbits},",
        f"                      {firstmark}, {firstspace},",
        f"                      k{name}BitMark, k{name}OneSpace,",
        f"                      k{name}BitMark, k{name}ZeroSpace,",
        f"                      {lastmark}, {lastspace}, true);",
        "  if (used == 0) return false;  // We failed to find any data.",
        "  offset += used;  // Adjust for how much of the message we read.",
        f"  pos += {int(nbytes)};"
        "  // Adjust by how many bytes of data we read"])
    return code

  def _calc_values(self):
    """Calculate the values which describe the standard timings
       for the protocol."""
    if self.verbose:
      self.output.write(f"Potential Mark Candidates:\n{self.marks}\n"
                        f"Potential Space Candidates:\n{self.spaces}\n")
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
    except ValueError as non_numeric:
      raise ValueError(
          f"Raw Data contains a non-numeric value of '{timing}'."
          ) from non_numeric
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
               f"k{name}HdrMark   = {hdr_mark}\n"
               f"k{name}HdrSpace  = {hdr_space}\n"
               f"k{name}BitMark   = {bit_mark}\n"
               f"k{name}OneSpace  = {one_space}\n"
               f"k{name}ZeroSpace = {zero_space}\n")
  defines.append(f"const uint16_t k{name}HdrMark = {hdr_mark};")
  defines.append(f"const uint16_t k{name}BitMark = {bit_mark};")
  defines.append(f"const uint16_t k{name}HdrSpace = {hdr_space};")
  defines.append(f"const uint16_t k{name}OneSpace = {one_space};")
  defines.append(f"const uint16_t k{name}ZeroSpace = {zero_space};")
  if ldr_mark:
    output.write(f"k{name}LdrMark   = {ldr_mark}\n")
    defines.append(f"const uint16_t k{name}LdrMark = {ldr_mark};")

  avg_gaps = [avg_list(message.space_buckets[x]) for x in message.gaps]
  if len(message.gaps) == 1:
    output.write(f"k{name}SpaceGap = {avg_gaps[0]}\n")
    defines.append(f"const uint16_t k{name}SpaceGap = {avg_gaps[0]};")
  else:
    count = 0
    for gap in avg_gaps:
      # We probably (still) have a gap in the protocol.
      count = count + 1
      output.write(f"k{name}SpaceGap{count} = {gap}\n")
      defines.append(f"const uint16_t k{name}SpaceGap{count} = {gap};")
  defines.append(f"const uint16_t k{name}Freq = 38000;  "
                 "// Hz. (Guessing the most common frequency.)")


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

  output.write(f"Found {len(rawdata)} timing entries.\n")

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
      f"#if SEND_{def_name.upper()}",
      SAFE64NOTE,
      f"/// Send a {name} formatted message.",
      "/// Status: ALPHA / Untested."])
  code["send"].extend([
      "/// @param[in] data containing the IR command.",
      f"/// @param[in] nbits Nr. of bits to send. usually k{name}Bits",
      "/// @param[in] repeat Nr. of times the message is to be repeated.",
      f"void IRsend::send{def_name}(const uint64_t data, const uint16_t"
      " nbits, const uint16_t repeat) {",
      f"  enableIROut(k{name}Freq);",
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
      f" (>=k{name}StateLength)",
      "/// @param[in] repeat Nr. of times the message is to be repeated.",
      f"void IRsend::send{def_name}(const uint8_t data[],"
      " const uint16_t nbytes, const uint16_t repeat) {",
      "  for (uint16_t r = 0; r <= repeat; r++) {",
      "    uint16_t pos = 0;"])
  code["sendcomfoot"].extend([
      "  }",
      "}",
      f"#endif  // SEND_{def_name.upper()}"])
  code["recvcomhead"].extend([
      "",
      f"#if DECODE_{def_name.upper()}",
      SAFE64NOTE,
      f"/// Decode the supplied {name} message.",
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
      f"bool IRrecv::decode{def_name}(decode_results *results, uint16_t offset,"
      " const uint16_t nbits, const bool strict) {",
      f"  if (results->rawlen < 2 * nbits + k{name}Overhead - offset)",
      "    return false;  // Too short a message to match.",
      f"  if (strict && nbits != k{name}Bits)",
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
      f"#endif  // DECODE_{def_name.upper()}"])

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
        code_info["lastmark"] = f"k{name}{mark_type}drMark"
        total_bits = total_bits + binary_value
      code_info["firstmark"] = f"k{name}{mark_type}drMark"
      binary_value = add_bit(binary_value, "reset")
      output.write(f"k{name}{mark_type}drMark+")
      code["send"].extend([f"    // {mark_type}eader",
                           f"    mark(k{name}{mark_type}drMark);"])
      code["recv"].extend([
          "",
          f"  // {mark_type}eader",
          "  if (!matchMark(results->rawbuf[offset++],"
          f" k{name}{mark_type}drMark))",
          "    return false;"])

    # Handle header spaces.
    elif message.is_hdr_space(usec) and not message.is_one_space(usec):
      if binary64_value:
        code_info["lastspace"] = f"k{name}HdrSpace"
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
          code_info["lastspace"] = f"k{name}HdrSpace"
          message.section_count = message.section_count + 1
        binary_value = binary64_value = add_bit(binary_value, "reset")
        output.write("UNEXPECTED->")
      state = "HS"
      output.write(f"k{name}HdrSpace+")
      code["send"].append(f"    space(k{name}HdrSpace);")
      code["recv"].extend([
          f"  if (!matchSpace(results->rawbuf[offset++], k{name}HdrSpace))",
          "    return false;"])
      code_info["firstspace"] = f"k{name}HdrSpace"
    # Handle bit marks.
    elif message.is_bit_mark(usec) and count % 2:
      if state not in ("HS", "BS"):
        output.write(f"k{name}BitMark(UNEXPECTED)")
      state = "BM"
    # Handle "zero" spaces
    elif message.is_zero_space(usec):
      if state != "BM":
        output.write(f"k{name}ZeroSpace(UNEXPECTED)")
      state = "BS"
      binary_value = binary64_value = add_bit(binary_value, 0, output)
    # Handle "one" spaces
    elif message.is_one_space(usec):
      if state != "BM":
        output.write(f"k{name}OneSpace(UNEXPECTED)")
      state = "BS"
      binary_value = binary64_value = add_bit(binary_value, 1, output)
    elif message.is_gap(usec):
      if state != "BM":
        output.write("UNEXPECTED->")
      output.write(f"GAP({usec})")
      code_info["lastspace"] = f"k{name}SpaceGap"
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
          code["send"].extend([f"    mark(k{name}BitMark);"])
          code["recv"].extend([
              f"  if (!matchMark(results->rawbuf[offset++], k{name}BitMark))",
              "    return false;"])
      code["send"].append(f"    space(k{name}SpaceGap);")
      code["recv"].extend([
          f"  if (!matchSpace(results->rawbuf[offset++], k{name}SpaceGap))",
          "    return false;"])
      total_bits = total_bits + binary_value
      binary_value = binary64_value = add_bit(binary_value, "reset")
      state = "GS"
    else:
      output.write(f"UNKNOWN({usec})")
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
      f"  results->decode_type = decode_type_t::{def_name.upper()};",
      "  results->bits = nbits;",
      "  results->value = data;",
      "  results->command = 0;",
      "  results->address = 0;"])
  code["recv64+"].extend([
      "",
      "  // Success",
      f"  results->decode_type = decode_type_t::{def_name.upper()};",
      "  results->bits = nbits;"])

  total_bits = total_bits + binary_value
  output.write(f"\nTotal Nr. of suspected bits: {len(total_bits)}\n")
  defines.append(f"const uint16_t k{name}Bits = {len(total_bits)};"
                 "  // Move to IRremoteESP8266.h")
  if len(total_bits) > 64:
    defines.append(f"const uint16_t k{name}StateLength = "
                   f"{int(len(total_bits) / 8)};  // Move to IRremoteESP8266.h")
  defines.append(f"const uint16_t k{name}Overhead = "
                 f"{message.rawlen - 2 * len(total_bits)};")
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
               f"/// @brief Support for {def_name} protocol\n\n"
               "// Supports:\n"
               f"//   Brand: {def_name},  Model: TODO add device and remote\n\n"
               '#include "IRrecv.h"\n'
               '#include "IRsend.h"\n'
               '#include "IRutils.h"\n\n'
               "// WARNING: This probably isn't directly usable."
               " It's a guide only.\n\n"
               "// See https://github.com/crankyoldgit/IRremoteESP8266/wiki/"
               "Adding-support-for-a-new-IR-protocol\n"
               "// for details of how to include this in the library."
               "\n")
  for line in defines:
    output.write(f"{line}\n")

  if len(bits_str) > 64:  # Will it fit in a uint64_t?
    output.write("// DANGER: More than 64 bits detected. A uint64_t for "
                 "'data' won't work!\n")
  # Display the "normal" version's send code incase there are some
  # oddities in it.
  for line in code["sendcomhead"] + code["send"] + code["sendcomfoot"]:
    if line == SAFE64NOTE:
      line = "// Function should be safe up to 64 bits."
    output.write(f"{line}\n")

  if len(bits_str) > 64:  # Will it fit in a uint64_t?
    for line in code["sendcomhead"] + code["send64+"] + code["sendcomfoot"]:
      if line == SAFE64NOTE:
        line = (f"// Alternative >64bit function to send {def_name.upper()}"
                " messages\n"
                "// Function should be safe over 64 bits.")
      elif line == CODEGEN:
        # pylint: disable=C0209
        line = "///   uint8_t data[k%sStateLength] = {0x%s};" % (
            name, ", 0x".join("%02X" % int(bits_str[i:i + 8], 2)
                              for i in range(0, len(bits_str), 8)))
        # pylint: enable=C0209
      output.write(f"{line}\n")
  if len(bits_str) > 64:  # Will it fit in a uint64_t?
    output.write("\n// DANGER: More than 64 bits detected. A uint64_t for "
                 "'data' won't work!")

  # Display the "normal" version's decode code incase there are some
  # oddities in it.
  for line in code["recvcomhead"] + code["recv"] + code["recvcomfoot"]:
    if line == SAFE64NOTE:
      line = "// Function should be safe up to 64 bits."
    output.write(f"{line}\n")

  # Display the > 64bit version's decode code
  if len(bits_str) > 64:  # Is it too big for a uint64_t?
    if len(bits_str) % 8:
      output.write("\n// WARNING: Data is not a multiple of bytes. "
                   "This won't work!\n")
    for line in code["recvcomhead"] + code["recv64+"] + code["recvcomfoot"]:
      if line == SAFE64NOTE:
        line = "// Function should be safe over 64 bits."
      output.write(f"{line}\n")

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
    with open(arg_options.file, encoding="utf8") as input_file:
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
