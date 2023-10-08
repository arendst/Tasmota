from tkinter import Tk
import unishox

# get text from clipboard expecting something like that:
# const char HTTP_SCRIPT_WIFI[] PROGMEM =
#   "function c(l){"  // comments
#     "eb('s1').value=l.innerText||l.textContent;"  // comments
#     "eb('p1').focus();"  // comments
#   // comments
#   "}";

text = Tk().clipboard_get()
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

print("####### Parsing intput:")  
print("Const char name: ",const_name)
print('####### Cleaned input:')  
print(input)

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
print("####### Compression result:")
print("Compressed from {i} to {o}, -{p:.1f}%".format(i=in_len, o=out_len, p=(100-out_len/in_len*100)))
out_bytes = out_bytes[:out_len]     # truncate to right size

#PROGMEM is growing in steps 0,8,24,40,56,... bytes of data resulting in size of 0,16,32,48,64,... bytes
for in_real in range(8,in_len+16,16):
    if in_real>=in_len:
      print("Old real PROGMEM-size:",in_real+8,"(unused bytes:",in_real-in_len,")")
      break
for out_real in range(8,out_len+16,16):
    if out_real>=out_len:
      print("New real PROGMEM-size:",out_real+8,"(unused bytes:",out_real-out_len,")")
      break
print("the optimal case would be raw bytes + 8, real difference: ", in_real - out_real, "bytes")
# https://www.geeksforgeeks.org/break-list-chunks-size-n-python/
def chunked(my_list, n):
    return [my_list[i * n:(i + 1) * n] for i in range((len(my_list) + n - 1) // n )]  

# split in chunks of 20 characters
chunks = chunked(out_bytes, 20)

lines_raw = [ "\"\\x" + "\\x".join( [ '{:02X}'.format(b) for b in chunk ] ) + "\"" for chunk in chunks ]
line_complete = "const char " + const_name + "_COMPRESSED" +"[] PROGMEM = " + ("\n" + " "*29).join(lines_raw) + ";"
lines = "const size_t " + const_name +"_SIZE = {size};\n{lines}".format(size=in_len, lines=line_complete)

print('####### Final output:')  
print(lines)

definition = "#define  " + const_name +  "       Decompress(" + const_name + "_COMPRESSED" + "," + const_name +"_SIZE" + ").c_str()"
print(definition)


# maybe add export to clipboard for later ...