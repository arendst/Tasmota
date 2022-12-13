#!/usr/bin/python3

"""
  espupload.py - for Tasmota

  Copyright (C) 2022  Theo Arends

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

Provides:
  Uploads binary file to OTA server.
  Usually initated from http-uploader.py

Requirements:
  - Python
  - pip install requests

Usage:
  ./espupload -u <Host_IP_address>:<Host_port>/<Host_path> -f <sketch.bin>
"""

import sys
import os
import shutil
import argparse
import requests

#HOST_URL = "domus1:80/api/upload-arduino.php"
HOST_URL = "otaserver/ota/upload-tasmota.php"

def main(args):
#  print(sys.argv[0:])

  # get arguments
  parser = argparse.ArgumentParser(
    usage = "%prog [arguments]",
    description = "Upload image to over the air Host server for the esp8266 or esp32 module with OTA support."
  )
  parser.add_argument("-u", "--host_url", dest = "host_url", action = "store", help = "Host url", default = HOST_URL)
  parser.add_argument("-f", "--file", dest = "image", help = "Image file.", metavar = "FILE", default = None)
  args = parser.parse_args()

  if (not args.host_url or not args.image):
    print("Not enough arguments.")
    return 1
  # end if

  if not os.path.exists(args.image):
    print('Sorry: the file %s does not exist', args.image)
    return 2
  # end if

  # copy firmware.bin to tasmota.bin or tasmota32.bin
  tname = os.path.normpath(os.path.dirname(args.image))
  new_filename = tname + os.sep + os.path.basename(tname) + '.bin'
  shutil.copy2(args.image, new_filename)

  url = 'http://%s' % (args.host_url)
  files = {'file': open(new_filename, 'rb')}
  req = requests.post(url, files=files)
  print(req.text)
# end main

if __name__ == '__main__':
  sys.exit(main(sys.argv))
# end if