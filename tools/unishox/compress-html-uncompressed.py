###############################################################
# compresses all files found in ..\..\tasmota\html_uncompressed
# write compressed C code to    ..\..\tasmota\html_compressed
# Instructions:
# open a console, e.g. in vscode, open a 'terminal'
# cd .\tools\unishox
# run:
# python compress-html-uncompressed.py
#
# The intent it to commit both uncompressed and compressed to the repo
# else this script would need to be run at build.
#
# Example Tasmota code:
# #ifdef USE_UNISHOX_COMPRESSION
#   #include "./html_compressed/HTTP_SCRIPT_CONSOL.h"
# #else
#   #include "./html_uncompressed/HTTP_SCRIPT_CONSOL.h"
# #endif
#
###############################################################

import unishox
from sys import argv
from datetime import datetime
from pathlib import Path
from hashlib import sha256

self_dir = Path(__file__).absolute().parent
base_dir = self_dir.parent.parent

def extract_c_string(s: str) -> str:
  state = 0
  escape = False
  out = ""
  for c in s:
    if state == 0:    # before string
      if c == '"':      # entering string
        out = '"'
        state = 1
      elif c == '/':    # start of comment before entering string
        state = 99      # we're done
    elif state == 1:  # in string
      if escape:        # escaped char
        out += '\\' + c
        escape = False
      elif c == '\\':   # escaped char
        escape = True
      elif c == '"':    # end of string
        out += '"'
        state = 99      # we're done
      else:
        out += c
  return out

def compress_html(source, target, argv=None, verbose=False):
  if argv is None: argv = []

  with open(source, "r") as f:
    text = f.read()

  src_sha, old_sha = sha256(text.encode()).hexdigest(), None

  if not ('--force' in argv):
    with open(target, "r") as f:
      for line in f:
        prefix = line[:17]
        if prefix == '// input sha256: ':
          old_sha = line[17:17+64]
          break

    if src_sha == old_sha:
      return (0, 0)

  #text = Tk().clipboard_get()
  # print(text)

  # parsing and cleaning
  text_list = text.splitlines()
  text = '' #just reuse the string
  const_name = '<var>' #default if no name will be found

  line_number = 0
  for line in text_list:
    pos = line.find("const char")
    # print(pos, line)
    if pos > -1:
      line_list = line.rsplit(" ")
      for el in line_list:
        if el.find('[]') > -1:
          const_name = el[:-2] #extract the "const char" variable name
          line_list.pop(line_number)
    else: # remove line comments
      line_el = extract_c_string(line)
      # print(line_el)
      text = text + line_el
    line_number = line_number +1

  # print const_name
  # print text

  #remove unwanted quotation marks
  qm = []
  pos  =0
  last_char = ""
  for char in text:
      if char == "\"":
          if last_char != "\\":
            qm.append(pos) #find all quotation marks without preceding backslash
      last_char = char
      pos = pos + 1
  # print(qm)
  lastel = 0
  input = ""
  for pos in qm:
      sub = text[lastel+1:pos:]
      if not sub.isspace() and pos-lastel > 1:
          # print(lastel, pos)
          input = input + sub #only copy substrings that are not whitespace
          # print(text[lastel+1:pos:])
      lastel = pos

  if verbose:
    print("####### Parsing input from " + str(source.relative_to(base_dir)))
    print("  Const char name: "+const_name)
  #print('####### Cleaned input:')
  #print(input)

  #construct output (taken from shadinger)
  input = input.replace("\\t", "\t")
  input = input.replace("\\n", "\n")
  input = input.replace("\\r", "\r")
  input = input.replace("\\f", "\f")
  input = input.replace("\\b", "\b")
  input = input.replace("\\\"", u"\u0022")

  in_bytes = bytearray(input, 'utf-8')
  in_len = len(in_bytes)
  out_bytes = bytearray(in_len * 2)

  UNISHOX = unishox.Unishox()
  out_len = UNISHOX.compress(in_bytes, len(in_bytes), out_bytes, len(out_bytes))
  if verbose:
    print("  ####### Compression result:")
    reduction = 100-(float(out_len)/float(in_len)*100)
    print(f"  Compressed from {in_len} to {out_len}, -{reduction:.1f}%")
  out_bytes = out_bytes[:out_len]     # truncate to right size

  #PROGMEM is growing in steps 0,8,24,40,56,... bytes of data resulting in size of 0,16,32,48,64,... bytes
  for in_real in range(8,in_len+16,16):
      if in_real>=in_len:
        if verbose:
          print(f"  Old real PROGMEM-size:{in_real+8}(unused bytes:{in_real-in_len})")
        break
  for out_real in range(8,out_len+16,16):
      if out_real>=out_len:
        if verbose:
          print(f"  New real PROGMEM-size:{out_real+8}(unused bytes:{out_real-out_len})")
        break

  if verbose:
    print(f"  the optimal case would be raw bytes + 8, real difference: {in_real - out_real}bytes")

  # https://www.geeksforgeeks.org/break-list-chunks-size-n-python/
  def chunked(my_list, n):
      return [my_list[i * n:(i + 1) * n] for i in range((len(my_list) + n - 1) // n )]

  # split in chunks of 20 characters
  chunks = chunked(out_bytes, 20)

  lines_raw = [ "\"\\x" + "\\x".join( [ '{:02X}'.format(b) for b in chunk ] ) + "\"" for chunk in chunks ]
  line_complete = f"const char {const_name}_COMPRESSED[] PROGMEM = " + ("\n" + " "*29).join(lines_raw) + ";"
  lines = f"\nconst size_t {const_name}_SIZE = {in_len};    // compressed size {out_len} bytes\n{line_complete}\n\n"

  #print('####### Final output:')
  #print(lines)

  definition = f"#define  {const_name}       Decompress({const_name}_COMPRESSED,{const_name}_SIZE).c_str()"
  #print(definition)

  now = datetime.now() # current date and time
  percent = int((float(out_real)/float(in_real))*100.0)
  saving = in_real - out_real
  #totalIn = totalIn + in_real
  #totalSaved = totalSaved + saving
  comment  = "/////////////////////////////////////////////////////////////////////\n"
  comment += "// compressed by tools/unishox/compress-html-uncompressed.py\n"
  comment += f"// input sha256: {src_sha}\n"
  comment += "/////////////////////////////////////////////////////////////////////\n"

  with open(target, "w") as f:
      f.write(comment + lines + definition)

  return (in_real, saving)

  if verbose:
    print("####### Wrote output to " + str(target.relative_to(base_dir)))

def compress_dir(source_dir, target_dir, argv=None, verbose=False):
  totalIn, totalSaved = 0, 0

  for source in source_dir.iterdir():
    target = Path(target_dir, source.stem + ".h")
    bytesIn, bytesSaved = compress_html(source, target, argv, verbose)
    totalIn += bytesIn
    totalSaved += bytesSaved

  return (totalIn, totalSaved)

if __name__ == '__main__':
  path_uncompressed = Path(base_dir, 'tasmota', 'html_uncompressed')
  path_compressed   = Path(base_dir, 'tasmota', 'html_compressed')

  totalIn, totalSaved = compress_dir(path_uncompressed, path_compressed, argv, True)

  if totalSaved > 0:
    print(f"If all files are in use, total saving was {totalSaved} out of {totalIn}")
