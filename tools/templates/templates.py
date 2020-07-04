#!/usr/bin/env python3

"""
  templates.py - template beautify TEMPLATES.md for Tasmota

  Copyright (C) 2020  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

Requirements:
   - Python
   - pip pycurl certifi

Instructions:
    Execute command to produce file TEMPLATE.md as found in the Tasmota root folder

Usage:
    python templates.py

"""

import io
import pycurl
import certifi
from io import BytesIO
from io import StringIO
from datetime import datetime

column = 27  # Start position of {"NAME":... in line

def main():
  print ("\n*** templates.py v20200514 by Theo Arends ***")

  # Download from template website
  buffer = BytesIO()
  url = "https://templates.blakadder.com/list.json"
  c = pycurl.Curl()
  c.setopt(c.URL, url)
  c.setopt(c.WRITEDATA, buffer)
  c.setopt(c.CAINFO, certifi.where())
  c.perform()
  c.close()
  body = buffer.getvalue()
  fin = StringIO(body.decode('UTF-8'))

  now = datetime.now()
  month = now.strftime('%B')
  year = now.strftime('%Y')

  # Write to root/TEMPLATES.md
  fout = open("..\..\TEMPLATES.md","w+")

  fout.write("<img src=\"/tools/logo/TASMOTA_FullLogo_Vector.svg\" alt=\"Logo\" align=\"right\" height=\"76\"/>\n")
  fout.write("\n")
  fout.write("# Templates\n")
  fout.write("\n")
  fout.write("Find below the available templates as of " + month + " " + year + ". More template information can be found in the [Tasmota Device Templates Repository](http://blakadder.github.io/templates)\n")

  not_first = 0
  fline = fin.readlines()
  for line in fline:
    if line.strip():
      if line.startswith("##"):
        if not_first:
          fout.write('```\n')
        fout.write('\n')
        fout.write(line)
        fout.write('```\n')
        not_first = 1
      elif line.startswith("#"):
        noop = 0
      else:
        pos1 = line.find("{")
        if pos1 < column:
          a = column + 2 - pos1
          lout = line[0:pos1 - 4] + " "*a + line[pos1:len(line)]
        else:
          lout = line[0:pos1 - 4] + "  " + line[pos1:len(line)]
        fout.write(lout)

  fout.write('```\n')

  fout.close()
  fin.close()

if __name__ == "__main__":
  main()
