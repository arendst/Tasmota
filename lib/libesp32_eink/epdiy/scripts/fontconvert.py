#!python3
import sys

# inclusive unicode code point intervals
# must not overlap and be in ascending order
# modify intervals here
# however if the "string" command line argument is used these are ignored

intervals = [
    (32, 126),
    (160, 255),
    # punctuation
    (0x2010, 0x205F),
    # arrows
    (0x2190, 0x21FF),
    # math
    #(0x2200, 0x22FF),
    # symbols
    (0x2300, 0x23FF),
    # box drawing
    #(0x2500, 0x259F),
    # geometric shapes
    (0x25A0, 0x25FF),
    # misc symbols
    (0x2600, 0x26F0),
    (0x2700, 0x27BF),
    # powerline symbols
    #(0xE0A0, 0xE0A2),
    #(0xE0B0, 0xE0B3),
    #(0x1F600, 0x1F680),
]



try:
    import freetype
except ImportError as error:
    sys.exit("To run this script the freetype module needs to be installed.\nThis can be done using:\npip install freetype-py")
import zlib
import sys
import re
import math
import argparse
from collections import namedtuple
#see https://freetype-py.readthedocs.io/en/latest/ for documentation
parser = argparse.ArgumentParser(description="Generate a header file from a font to be used with epdiy.")
parser.add_argument("name", action="store", help="name of the font.")
parser.add_argument("size", type=int, help="font size to use.")
parser.add_argument("fontstack", action="store", nargs='+', help="list of font files, ordered by descending priority. This is not actually implemented please just use one file for now.")
parser.add_argument("--compress", dest="compress", action="store_true", help="compress glyph bitmaps.")
parser.add_argument("--additional-intervals", dest="additional_intervals", action="append", help="Additional code point intervals to export as min,max. This argument can be repeated.")
parser.add_argument("--string", action="store", help="A string of all required characters. intervals are made up of this" )

args = parser.parse_args()
command_line = ""
prev_arg = ""
for arg in sys.argv:
    # ~ if prev_arg == "--string":
        # ~ command_line =  command_line + " '" + arg +"'"
    # ~ else:
        command_line =  command_line + " " + arg
    # ~ prev_arg = arg

# ~ print (command_line)
GlyphProps = namedtuple("GlyphProps", ["width", "height", "advance_x", "left", "top", "compressed_size", "data_offset", "code_point"])

font_stack = [freetype.Face(f) for f in args.fontstack]
font_files =  args.fontstack
face_index = 0
font_file =  font_files[face_index]
compress = args.compress
size = args.size
font_name = args.name

for face in font_stack:
    # shift by 6 bytes, because sizes are given as 6-bit fractions
    # the display has about 150 dpi.
    face.set_char_size(size << 6, size << 6, 150, 150)


# assign intervals from argument parrameters  ie. handle the string arg

if args.string != None:
    font_file =  font_files[face_index]
    string = " " + args.string # always add space to the string it is easily forgotten
    chars = sorted(set(string))
    #make array of code pointscode_ponts.append(ord(char))
    code_points = list()
    intervals = []   # empty the intevals array NB. if you want to allways add default characters comment out this line
    # go through the sorted characters and make the intervals
    for char in chars:
        if( face.get_char_index(ord(char)) != 0 ):
            # this character is in the font file so add it to the new string.
            code_points.append(ord(char))
        else:
            print("The character ", char, " is not available in ", font_file, file=sys.stderr)
    lower = code_points[0]
    len_x = len(code_points)
    x = 0
    while x < len_x:
        # ~ print ("loop value x = ", x , file=sys.stderr)
        a =  code_points[x];
        b = a;
        if( x < len_x - 1):
            b = code_points[x + 1];

        if( a == b - 1 ):
            # ~ print("sequential", a, b, file=sys.stderr)
            if( lower == -1):
                lower = a
        else:
            # ~ print("non sequential", a, b , file=sys.stderr)
            if( lower == -1):
                # ~ print("single character")
                interval = (a , a)
            else:
                interval = (lower, a)
            # ~ print("interval", interval , file=sys.stderr)
            intervals.append(interval)
            lower = -1
        x = x + 1


# base intervals are assigned dditional intervals from arguments
add_ints = []
if args.additional_intervals != None:
    add_ints = [tuple([int(n, base=0) for n in i.split(",")]) for i in args.additional_intervals]

intervals = sorted(intervals + add_ints)

# ~ print("Intervals are now: ", intervals, file=sys.stderr)


def norm_floor(val):
    return int(math.floor(val / (1 << 6)))

def norm_ceil(val):
    return int(math.ceil(val / (1 << 6)))

for face in font_stack:
    # shift by 6 bytes, because sizes are given as 6-bit fractions
    # the display has about 150 dpi.
    face.set_char_size(size << 6, size << 6, 150, 150)

def chunks(l, n):
    for i in range(0, len(l), n):
        yield l[i:i + n]

total_size = 0
total_packed = 0
all_glyphs = []

# new globals
total_chars = 0
ascender = 0
descender = 100
f_height = 0

def load_glyph(code_point):
    global face_index
    face_index = 0
    while face_index < len(font_stack):
        face = font_stack[face_index]
        glyph_index = face.get_char_index(code_point)
        if glyph_index > 0:
            face.load_glyph(glyph_index, freetype.FT_LOAD_RENDER)
            #count characters found and find bounds of characters
            global ascender
            if ascender < face.size.ascender:
                ascender = face.size.ascender
            global descender
            if descender > face.size.descender:
                descender = face.size.descender
            global f_height
            if f_height < face.size.height:
                f_height = face.size.height
            global total_chars
            total_chars += 1
            return face
            break
        face_index += 1
        # this needs work
        # this needs to be handled better to show failed character and continue not just die a questionable death
        # this appears to have been designed to combine several font files
        # but that is not clear to the end user and this then looks like a bug
        print (f"falling back to font {face_index} for {chr(code_point)}.", file=sys.stderr)
    raise ValueError(f"code point {code_point} not found in font stack!")

for i_start, i_end in intervals:
    for code_point in range(i_start, i_end + 1):
        # handle missing characters in font file
        if( face.get_char_index(code_point) == 0 ):
            print("Character ", chr(code_point), "(", code_point, ") is not in ", font_file, file=sys.stderr)
            continue
        face = load_glyph(code_point)
        bitmap = face.glyph.bitmap
        pixels = []
        px = 0
        for i, v in enumerate(bitmap.buffer):
            y = i / bitmap.width
            x = i % bitmap.width
            if x % 2 == 0:
                px = (v >> 4)
            else:
                px = px | (v & 0xF0)
                pixels.append(px);
                px = 0
            # eol
            if x == bitmap.width - 1 and bitmap.width % 2 > 0:
                pixels.append(px)
                px = 0

        packed = bytes(pixels);
        total_packed += len(packed)
        compressed = packed
        if compress:
            compressed = zlib.compress(packed)

        glyph = GlyphProps(
            width = bitmap.width,
            height = bitmap.rows,
            advance_x = norm_floor(face.glyph.advance.x),
            left = face.glyph.bitmap_left,
            top = face.glyph.bitmap_top,
            compressed_size = len(compressed),
            data_offset = total_size,
            code_point = code_point,
        )
        total_size += len(compressed)
        all_glyphs.append((glyph, compressed))

# pipe seems to be a good heuristic for the "real" descender
# face = load_glyph(ord('|'))
# removed as max descender and assender are handled above

glyph_data = []
glyph_props = []
for index, glyph in enumerate(all_glyphs):
    props, compressed = glyph
    glyph_data.extend([b for b in compressed])
    glyph_props.append(props)
print("", file=sys.stderr)
print(f"Original font file {font_file} as {font_name} using {total_chars} characters", file=sys.stderr)

print("total", total_packed, file=sys.stderr)
print("compressed", total_size, file=sys.stderr)

print("#pragma once")
print("#include \"epdiy.h\"")

# add font file origin and characters at the head of the output file
print("/*")
print ( "Created with")
print(command_line)
print(f"As '{font_name}' with available {total_chars} characters")
for i, g in enumerate(glyph_props):
    print (f"{chr(g.code_point)}", end ="" )
print("")
print("*/")

print(f"const uint8_t {font_name}_Bitmaps[{len(glyph_data)}] = {{")
for c in chunks(glyph_data, 16):
    print ("    " + " ".join(f"0x{b:02X}," for b in c))
print ("};");


print ('// GlyphProps[width, height, advance_x, left, top, compressed_size, data_offset, code_point]')
print(f"const EpdGlyph {font_name}_Glyphs[] = {{")
for i, g in enumerate(glyph_props):
    print ("    { " + ", ".join([f"{a}" for a in list(g[:-1])]),"},", f"// '{chr(g.code_point) if g.code_point != 92 else '<backslash>'}'")
print ("};");

print(f"const EpdUnicodeInterval {font_name}_Intervals[] = {{")
offset = 0
for i_start, i_end in intervals:
    print (f"    {{ 0x{i_start:X}, 0x{i_end:X}, 0x{offset:X} }},")
    offset += i_end - i_start + 1
print ("};");

print(f"const EpdFont {font_name} = {{")
print(f"    {font_name}_Bitmaps, // (*bitmap) Glyph bitmap pointer, all concatenated together")
print(f"    {font_name}_Glyphs, // glyphs Glyph array")
print(f"    {font_name}_Intervals, // intervals Valid unicode intervals for this font")
print(f"    {len(intervals)},   // interval_count Number of unicode intervals.intervals")
print(f"    {1 if compress else 0}, // compressed Does this font use compressed glyph bitmaps?")
print(f"    {norm_ceil(f_height)}, // advance_y Newline distance (y axis)")
print(f"    {norm_ceil(ascender)}, // ascender Maximal height of a glyph above the base line")
print(f"    {norm_floor(descender)}, // descender Maximal height of a glyph below the base line")
print("};")
print("/*")
print("Included intervals")
for i_start, i_end in intervals:
    print (f"    ( {i_start}, {i_end}), ie. '{chr(i_start)}' -  '{chr(i_end)}'")
print("Included intervals", file=sys.stderr)
for i_start, i_end in intervals:
    print (f"    ( {i_start}, {i_end}), ie. '{chr(i_start)}' -  '{chr(i_end)}'", file=sys.stderr)
print("")
print("*/")

