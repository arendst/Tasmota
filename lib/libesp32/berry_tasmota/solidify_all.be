#! ../berry/berry -s -g
#
# Berry solidify files
#
# `../berry/berry -s -g`

import os
import global
import solidify
import string
import re

# import sys
# sys.path().push('src/embedded')   # allow to import from src/embedded

# globals that need to exist to make compilation succeed
var globs = "path,ctypes_bytes_dyn,tasmota,ccronexpr,gpio,light,webclient,load,MD5,lv,light_state"

var files = ['tasmota_class.be', 'leds.be', 'animate_module.be', 'autoconf_module.be','driver_class.be',
  'partition_core.be','i2c_driver.be','hue_bridge.be','persist.be','uuid.be']

for g:string.split(globs, ",")
  global.(g) = nil
end

var prefix_dir = "src/embedded/"
var prefix_out = "src/solidify/"

def clean_directory(dir)
  var file_list = os.listdir(dir)
  for f : file_list
    if f[0] == '.'  continue end    # ignore files starting with `.`
    os.remove(dir + f)
  end
end

var pattern = "#@\\s*solidify:([A-Za-z0-9_,]+)"

def parse_file(fname)
  print("Parsing: ", fname)
  var f = open(prefix_dir + fname)
  var src = f.read()
  f.close()
  # try to compile
  var compiled = compile(src)
  compiled()      # run the compile code to instanciate the classes and modules
  # output solidified
  var fname_h = string.split(fname, '.be')[0] + '.h'  # take whatever is before the first '.be'
  var fout = open(prefix_out + "solidified_" + fname_h, "w")
  fout.write(string.format("/* Solidification of %s */\n", fname_h))
  fout.write("/********************************************************************\\\n")
  fout.write("* Generated code, don't edit                                         *\n")
  fout.write("\\********************************************************************/\n")
  fout.write('#include "be_constobj.h"\n')

  var directives = re.searchall(pattern, src)
  # print(directives)

  for directive : directives
    var object_list = string.split(directive[1], ',')
    var object_name = object_list[0]
    var weak = (object_list.find('weak') != nil)          # do we solidify with weak strings?
    var o = global.(object_name)
    solidify.dump(o, weak, fout)
  end

  fout.write("/********************************************************************/\n")
  fout.write("/* End of solidification */\n")
  fout.close()
end

clean_directory(prefix_out)

var src_file_list = os.listdir(prefix_dir)
for src_file : src_file_list
  if src_file[0] == '.'  continue end
end

# manual
for f : files
  parse_file(f)
end
