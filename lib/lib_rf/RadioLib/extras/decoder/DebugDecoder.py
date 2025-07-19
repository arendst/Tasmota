import re, sys, argparse
from pathlib import Path
from argparse import RawTextHelpFormatter


'''
TODO list:
  1. Parse macro values (the names of bits in all registers in header file)
  2. Failed SPI write handling
  3. SX126x/SX128x handling
'''


def get_macro_name(value, macros):
    for macro in macros:
        if macro[1] == value:
            return macro[0]
    return 'UNKNOWN_VALUE'


def get_macro_value(value):
    return '                         0x{0:02X}\n'.format(int(value, 16))


parser = argparse.ArgumentParser(formatter_class=RawTextHelpFormatter, description='''
    RadioLib debug output decoder script. Turns RadioLib Serial dumps into readable text.

    Step-by-step guid on how to use the decoder:
    1. Uncomment lines 312 (#define RADIOLIB_DEBUG) and 313 (#define RADIOLIB_VERBOSE) in RadioLib/src/BuildOpt.h
    2. Recompile and upload the failing Arduino sketch
    3. Open Arduino IDE Serial Monitor and enable timestamps
    4. Copy the Serial output and save it into a .txt file
    5. Run this script

    Output will be saved in the file specified by --out and printed to the terminal
''')
parser.add_argument('file', metavar='file', type=str, help='Text file of the debug output')
parser.add_argument('--out', metavar='out', default='./out.txt', type=str, help='Where to save the decoded file (defaults to ./out.txt)')
args = parser.parse_args()

# open the log file
log = open(args.file, 'r').readlines()

# find modules that are in use
used_modules = []
pattern_module = re.compile('(([01]?[0-9]|2[0-3]):[0-5][0-9](:[0-5][0-9])?.[0-9]{3} -> )?M\t')
for entry in log:
    m = pattern_module.search(entry)
    if m != None:
        used_modules.append(entry[m.end():].rstrip())

# get paths to all relevant header files
header_files = []
for path in Path('../../src').rglob('*.h'):
    for module in used_modules:
        if module in path.name:
            header_files.append(path)

# extract names of address macros from the header files
macro_addresses = []
pattern_define = re.compile('#define \w* +\w*(\n| +\/\/){1}')
for path in header_files:
    file = open(path, 'r').readlines()
    for line in file:
        m = pattern_define.search(line)
        if m != None:
            s = re.split(' +', m.group().rstrip())
            if (s.__len__() > 1) and ('_REG' in s[1]):
                macro_addresses.append([s[1], int(s[2], 0)])

'''
# extract names of value macros for each adddress macro
macro_values = []
for path in header_files:
    file = open(path, 'r').readlines()
    for line in file:
        for module in used_modules:
            pattern_addr_macro = re.compile('\/\/ SI443X_REG_\w+'.format(module.capitalize()))
'''

# parse every line in the log file
out = []
pattern_debug = re.compile('(([01]?[0-9]|2[0-3]):[0-5][0-9](:[0-5][0-9])?.[0-9]{3} -> )?[RWM]\t.+')
for entry in log:
    m = pattern_debug.search(entry)
    if m != None:
        s = re.split('( |\t)+', entry.rstrip())
        cmd_len = int((s.__len__() - 7)/2)
        new_entry = s[0] + s[1] + s[2] + s[3]
        if s[4] == 'W':
            macro_address = int(s[6], 16)
            new_entry += 'write {0:>2} 0x{1:02X} {2}\n'.format(cmd_len, macro_address, get_macro_name(macro_address, macro_addresses))
            for i in range(cmd_len):
                new_entry += get_macro_value(s[8 + 2*i]);
        elif s[4] == 'R':
            macro_address = int(s[6], 16)
            new_entry += 'read  {0:>2} 0x{1:02X} {2}\n'.format(cmd_len, macro_address, get_macro_name(macro_address, macro_addresses))
            for i in range(cmd_len):
                new_entry += get_macro_value(s[8 + 2*i]);
        elif s[4] == 'M':
            new_entry += 'module {}\n'.format(s[6])
        out.append(new_entry)
    else:
        out.append(entry)

# write the output file
out_file = open(args.out, 'w')
for line in out:
    print(line, end='')
    out_file.write(line)
out_file.close()
