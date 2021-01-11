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
from os import listdir
from datetime import datetime


files = listdir('..\\..\\tasmota\\html_uncompressed') 

totalIn = 0
totalSaved = 0

for file in files:
  f = open('..\\..\\tasmota\\html_uncompressed\\' + file, "r")
  text = f.read()
  f.close()

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
      line_el = line.rsplit("//")
      # print('Splitted line list by //' % line_el)
      # print(line_el[0])
      text = text + line_el[0]
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

  print("####### Parsing input from "+'..\\..\\tasmota\\html_uncompressed\\' + file)  
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
  print("  ####### Compression result:")
  print("  Compressed from {i} to {o}, -{p:.1f}%".format(i=in_len, o=out_len, p=(100-(float(out_len)/float(in_len)*100))))
  out_bytes = out_bytes[:out_len]     # truncate to right size

  #PROGMEM is growing in steps 0,8,24,40,56,... bytes of data resulting in size of 0,16,32,48,64,... bytes
  for in_real in range(8,in_len+16,16):
      if in_real>=in_len:
        print("  Old real PROGMEM-size:"+str(in_real+8)+"(unused bytes:"+str(in_real-in_len)+")")
        break
  for out_real in range(8,out_len+16,16):
      if out_real>=out_len:
        print("  New real PROGMEM-size:"+str(out_real+8)+"(unused bytes:"+str(out_real-out_len)+")")
        break
  print("  the optimal case would be raw bytes + 8, real difference: "+str(in_real - out_real)+ "bytes")
  # https://www.geeksforgeeks.org/break-list-chunks-size-n-python/
  def chunked(my_list, n):
      return [my_list[i * n:(i + 1) * n] for i in range((len(my_list) + n - 1) // n )]  

  # split in chunks of 20 characters
  chunks = chunked(out_bytes, 20)

  lines_raw = [ "\"\\x" + "\\x".join( [ '{:02X}'.format(b) for b in chunk ] ) + "\"" for chunk in chunks ]
  line_complete = "const char " + const_name + "_COMPRESSED" +"[] PROGMEM = " + ("\n" + " "*29).join(lines_raw) + ";"
  lines = "\nconst size_t " + const_name +"_SIZE = {size};\n{lines}\n\n".format(size=in_len, lines=line_complete)

  #print('####### Final output:')  
  #print(lines)

  definition = "#define  " + const_name +  "       Decompress(" + const_name + "_COMPRESSED" + "," + const_name +"_SIZE" + ").c_str()"
  #print(definition)

  now = datetime.now() # current date and time
  percent = int((float(out_real)/float(in_real))*100.0)
  saving = in_real - out_real
  totalIn = totalIn + in_real
  totalSaved = totalSaved + saving
  comment = "/////////////////////////////////////////////////////////////////////\n"
  comment = comment + "// compressed by tools/unishox/compress-html-uncompressed.py\n"
  comment = comment + "/////////////////////////////////////////////////////////////////////\n"

  f = open('..\\..\\tasmota\\html_compressed\\' + file, "w")
  f.write(comment + lines + definition)
  f.close()
  print("####### Wrote output to "+'..\\..\\tasmota\\html_compressed\\' + file)  

print("If all files are in use, total saving was "+str(totalSaved)+" out of "+str(totalIn))
