    #!/usr/bin/python
"""Convert IRremoteESP8266's Raw data output into Pronto Code."""
#
# Copyright 2020 David Conran
import argparse
import sys
from auto_analyse_raw_data import convert_rawdata, add_rawdata_args, get_rawdata


# pylint: disable=too-many-arguments
def parse_and_report(rawdata_str, hertz=38000, end_usecs=100000,
                     use_initial=False, generate_code=False, verbose=False,
                     output=sys.stdout):
  """Analyse the rawdata c++ definition of a IR message."""

  # Parse the input.
  rawdata = convert_rawdata(rawdata_str)
  if verbose:
    output.write(f"Found {len(rawdata)} timing entries.\n")

  # Do we need to pad out the rawdata to make it even in length?
  if end_usecs > 0 and len(rawdata) % 2 == 1:
    rawdata.append(end_usecs)

  result = ["0000"]
  # Work out the frequency code.
  pronto_freq = int(1000000.0 / (hertz * 0.241246))
  if verbose:
    output.write(f"Pronto frequency is {pronto_freq:X} ({hertz} Hz).\n")
  result.append(f"{pronto_freq:04X}")
  period = 1000000.0 / max(1, hertz)
  if verbose:
    output.write(f"Pronto period is {period} uSecs.\n")
  # Add the lengths to the code.
  if use_initial:
    result.append(f"{int(len(rawdata) / 2):04x}")  # Initial burst code length
    result.append("0000")  # No Repeat code length
  else:
    result.append("0000")  # No Initial burst code length
    result.append(f"{int(len(rawdata) / 2):04x}")  # Repeat code length

  # Add the data.
  if verbose:
    output.write(f"Raw data: {rawdata} ")
  for i in rawdata:
    result.append(f"{int(i / period):04x}")
  if generate_code:
    output.write(f"uint16_t pronto[{len(result)}] = "
                 f"{{0x{', 0x'.join(result)}}};\n")
  else:
    output.write(f"Pronto code = '{' '.join(result)}'\n")
# pylint: enable=too-many-arguments


def main():
  """Parse the commandline arguments and call the method."""
  arg_parser = argparse.ArgumentParser(
      description="Read an IRremoteESP8266 rawData declaration and tries to "
      "convert it in to a Pronto code.",
      formatter_class=argparse.ArgumentDefaultsHelpFormatter)
  arg_parser.add_argument(
      "--hz",
      "--hertz",
      type=int,
      help="Frequency of the protocol to use in code generation. E.g. 38000Hz",
      dest="hertz",
      required=True)
  arg_parser.add_argument(
      "-c",
      "--code",
      action='store_true',
      help="Output C/C++ code instead of human-readable.",
      dest="generate_code")
  arg_parser.add_argument(
      "-g",
      "--gap",
      "--endgap",
      type=int,
      help="Nr. of uSeconds of gap to add to the end of the message.",
      dest="usecs",
      default=100000)
  arg_parser.add_argument(
      "-i",
      "--initial_burst",
      action='store_true',
      help="Send using only the 'inital burst' section of the pronto code.",
      dest="use_initial")
  arg_parser.add_argument(
      "-v",
      "--verbose",
      help="Increase output verbosity",
      action="store_true",
      dest="verbose",
      default=False)
  add_rawdata_args(arg_parser)
  arg_options = arg_parser.parse_args()
  parse_and_report(get_rawdata(arg_options), arg_options.hertz,
                   arg_options.usecs, arg_options.use_initial,
                   arg_options.generate_code, arg_options.verbose)


if __name__ == '__main__':
  main()
