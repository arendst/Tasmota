#!/usr/bin/env -S ../../libesp32/berry/berry -s -g
#
# Berry solidify files

import os
import global
import solidify
import string as string2
import re

import sys
sys.path().push('src/embedded')   # allow to import from src/embedded

# globals that need to exist to make compilation succeed
var globs = "path,ctypes_bytes_dyn,tasmota,ccronexpr,gpio,light,webclient,load,MD5,lv,light_state,udp,tcpclientasync,log,"
            "lv_clock,lv_clock_icon,lv_signal_arcs,lv_signal_bars,lv_wifi_arcs_icon,lv_wifi_arcs,"
            "lv_wifi_bars_icon,lv_wifi_bars,"
            "_lvgl,"
            "int64"

for g:string2.split(globs, ",")
  global.(g) = nil
end

var prefix_dir = "src/embedded/"
var prefix_out = "src/solidify/"

def sort(l)
  # insertion sort
  for i:1..size(l)-1
    var k = l[i]
    var j = i
    while (j > 0) && (l[j-1] > k)
      l[j] = l[j-1]
      j -= 1
    end
    l[j] = k
  end
  return l
end

def clean_directory(dir)
  var file_list = os.listdir(dir)
  for f : file_list
    if f[0] == '.'  continue end    # ignore files starting with `.`
    os.remove(dir + f)
  end
end

var pattern = "#@\\s*solidify:([A-Za-z0-9_.,]+)"

def parse_file(fname, prefix_out)
  print("Parsing: ", fname)
  var f = open(prefix_dir + fname)
  var src = f.read()
  f.close()
  # try to compile
  var compiled = compile(src)
  compiled()      # run the compile code to instanciate the classes and modules
  # output solidified
  var fname_h = string2.split(fname, '.be')[0] + '.h'  # take whatever is before the first '.be'
  var fout = open(prefix_out + "solidified_" + fname_h, "w")
  fout.write(f"/* Solidification of {fname_h} */\n")
  fout.write("/********************************************************************\\\n")
  fout.write("* Generated code, don't edit                                         *\n")
  fout.write("\\********************************************************************/\n")
  fout.write('#include "be_constobj.h"\n')

  var directives = re.searchall(pattern, src)
  # print(directives)

  for directive : directives
    var object_list = string2.split(directive[1], ',')
    var object_name = object_list[0]
    var weak = (object_list.find('weak') != nil)          # do we solidify with weak strings?
    var o = global
    var cl_name = nil
    var obj_name = nil
    for subname : string2.split(object_name, '.')
      o = o.(subname)
      cl_name = obj_name
      obj_name = subname
      if   (type(o) == 'class')
        obj_name = 'class_' + obj_name
      elif (type(o) == 'module')
        obj_name = 'module_' + obj_name
      end
    end
    solidify.dump(o, weak, fout, cl_name)
  end

  fout.write("/********************************************************************/\n")
  fout.write("/* End of solidification */\n")
  fout.close()
end

clean_directory(prefix_out)

var src_file_list = os.listdir(prefix_dir)
src_file_list = sort(src_file_list)
for src_file : src_file_list
  if src_file[0] == '.'  continue end
  parse_file(src_file, prefix_out)
end
