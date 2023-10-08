#!python3

from PIL import Image, ImageOps
from argparse import ArgumentParser
import sys
import math

parser = ArgumentParser()
parser.add_argument('-i', action="store", dest="inputfile", required=True)
parser.add_argument('-n', action="store", dest="name", required=True)
parser.add_argument('-o', action="store", dest="outputfile", required=True)
parser.add_argument('-maxw', action="store", dest="max_width", default=1200, type=int)
parser.add_argument('-maxh', action="store", dest="max_height", default=825, type=int)

args = parser.parse_args()

im = Image.open(args.inputfile)
# convert to grayscale
im = im.convert(mode='L')
im.thumbnail((args.max_width, args.max_height), Image.ANTIALIAS)

# Write out the output file.
with open(args.outputfile, 'w') as f:
    f.write("const uint32_t {}_width = {};\n".format(args.name, im.size[0]))
    f.write("const uint32_t {}_height = {};\n".format(args.name, im.size[1]))
    f.write(
        "const uint8_t {}_data[({}*{})/2] = {{\n".format(args.name, math.ceil(im.size[0] / 2) * 2, im.size[1])
    )
    for y in range(0, im.size[1]):
        byte = 0
        done = True
        for x in range(0, im.size[0]):
            l = im.getpixel((x, y))
            if x % 2 == 0:
                byte = l >> 4
                done = False;
            else:
                byte |= l & 0xF0
                f.write("0x{:02X}, ".format(byte))
                done = True
        if not done:
            f.write("0x{:02X}, ".format(byte))
        f.write("\n\t");
    f.write("};\n")
