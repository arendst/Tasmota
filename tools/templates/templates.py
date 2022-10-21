#!/usr/bin/env python3

"""
  templates.py - template beautify TEMPLATES.md for Tasmota

  Copyright (C) 2021  Theo Arends

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

Instructions:
    Execute command to produce file TEMPLATE.md as found in the Tasmota root folder

Usage:
    python templates.py

"""

import sys
import io
import requests
from io import BytesIO
from io import StringIO
from datetime import datetime
import json


LIST_URL = "https://templates.blakadder.com/list.json"
TEMPLATES = "../../TEMPLATES.md"

COLUMN = 27  # Start position of {"NAME":... in line

# Some values from support.ino and tasmota_templates.h
AGPIO = lambda x : x << 5
GPIO_NONE = 0
GPIO_USER = AGPIO(GPIO_NONE)+1
GPIO_ADC_INPUT = 147
GPIO_ADC_JOY = 104

# Conversion table extracted from Tasmota code
GPIO_CONVERT = [
  0x0000,
  0x04A0,
  0x04C0,
  0x04E0,
  0x0520,
  0x0260,
  0x0280,
  0x0560,
  0x0420,
  0x00A0,
  0x00A1,
  0x00A2,
  0x00A3,
  0x00A4,
  0x00A5,
  0x00A6,
  0x00A7,
  0x0020,
  0x0021,
  0x0022,
  0x0023,
  0x00E0,
  0x00E1,
  0x00E2,
  0x00E3,
  0x00E4,
  0x00E5,
  0x00E6,
  0x00E7,
  0x0100,
  0x0101,
  0x0102,
  0x0103,
  0x0104,
  0x0105,
  0x0106,
  0x0107,
  0x01A0,
  0x01A1,
  0x01A2,
  0x01A3,
  0x01A4,
  0x0160,
  0x0161,
  0x0162,
  0x0163,
  0x01C0,
  0x01C1,
  0x01C2,
  0x01C3,
  0x01C4,
  0x0440,
  0x0120,
  0x0121,
  0x0122,
  0x0123,
  0x0140,
  0x0141,
  0x0142,
  0x0143,
  0x0580,
  0x05A0,
  0x05C0,
  0x05E0,
  0x0640,
  0x0660,
  0x0300,
  0x0320,
  0x03E0,
  0x06A0,
  0x06E0,
  0x0700,
  0x0720,
  0x0740,
  0x0760,
  0x0780,
  0x07A0,
  0x07C0,
  0x07E0,
  0x0800,
  0x0820,
  0x0840,
  0x00C0,
  0x00C1,
  0x00C2,
  0x00C3,
  0x00C4,
  0x00C5,
  0x00C6,
  0x00C7,
  0x0040,
  0x0041,
  0x0042,
  0x0043,
  0x0180,
  0x0181,
  0x0182,
  0x0183,
  0x0600,
  0x0620,
  0x0860,
  0x06C0,
  0x0880,
  0x08A0,
  0x08C0,
  0x0460,
  0x0480,
  0x08E0,
  0x0900,
  0x0920,
  0x0940,
  0x0340,
  0x0360,
  0x0380,
  0x03A0,
  0x03C0,
  0x0960,
  0x0980,
  0x09A0,
  0x09C0,
  0x09E0,
  0x0A00,
  0x0060,
  0x0061,
  0x0062,
  0x0063,
  0x0080,
  0x0081,
  0x0082,
  0x0083,
  0x0A20,
  0x0A40,
  0x0A60,
  0x0A80,
  0x0AA0,
  0x0AC0,
  0x0AE0,
  0x0B00,
  0x0B20,
  0x0B40,
  0x0B60,
  0x0B80,
  0x0BA0,
  0x0BC0,
  0x0BE0,
  0x0C00,
  0x0C20,
  0x0C40,
  0x0C80,
  0x0CA0,
  0x0CC0,
  0x0CE0,
  0x0CC1,
  0x0CE1,
  0x0D40,
  0x0D60,
  0x0D80,
  0x0220,
  0x0240,
  0x0C60,
  0x01E0,
  0x0200,
  0x0400,
  0x0DA0,
  0x0DC0,
  0x0DE0,
  0x0E00,
  0x0E20,
  0x0E40,
  0x0E60,
  0x0E80,
  0x0EA0,
  0x0EC0,
  0x0EE0,
  0x0EE1,
  0x0EE2,
  0x0F40,
  0x0F60,
  0x0F80,
  0x0FA0,
  0x0FC0,
  0x0FE0,
  0x1000,
  0x1020,
  0x1040,
  0x1060,
  0x1080,
  0x10A0,
  0x10C0,
  0x10E0,
  0x1100,
  0x1120,
  0x0540,
  0x0500,
  0x1140,
  0x1160,
  0x1180,
  0x11A0,
  0x11C0,
  0x11E0,
  0x1200,
  0x1220,
  0x1240,
  0x0680,
  0x1340,
  0x1360,
  0x1380,
  0x13C0,
  0x13E0,
  0x1400,
  0x1420,
  0x1440,
  0x1460,
  0x1480,
  0x14A0,
  0x14C0,
  0x14E0
]


def is_old_template(template):
  """find out it the template is old format"""
  # Old format <=> 13 gpios AND all gpios <= 255
#  print(f"Template {template}\n")
  if "GPIO" in template and len(template["GPIO"]) == 13:
    for g in template["GPIO"]:
      if g > 255:
        return False
    return True
  return False

def gpio_convert(gpio):
  """Conversion as per https://tasmota.github.io/docs/Components/#gpio-conversion"""
  if gpio >= len(GPIO_CONVERT):
    return GPIO_USER
  return GPIO_CONVERT[gpio]

def adc0_convert(adc0):
  """Conversion as per https://tasmota.github.io/docs/Components/#adc-conversion"""
  adc0 = adc0 & 0x0F
  if 0 == adc0:
    return GPIO_NONE
  elif 8 == adc0:
    return AGPIO(GPIO_ADC_JOY); # Joystick
  elif adc0 > 7:
    return GPIO_USER
  return AGPIO(GPIO_ADC_INPUT + adc0 -1)

def convert_gpios(gpios, flag):
  new_gpios = []
  for g in gpios:
    new_gpios.append(gpio_convert(g))
  new_gpios.append(adc0_convert(flag))
  return new_gpios

def convert_template(template):
  if not is_old_template(template):
    return template
  return { "NAME": template["NAME"],
           "GPIO": convert_gpios(template["GPIO"], template["FLAG"]),
           "FLAG":0,
           "BASE": template["BASE"] }

def get_templates_list():
  """Download templates list from template website"""
  response = requests.get(LIST_URL)
  if response.status_code != 200:
    printf(f"Error GET({URL}) returned code {response.status_code}\n")
    exit()
  return response.text.split('\n')

def main():
  print ("\n*** templates.py v20201121 by Theo Arends and Barbudor ***")

  now = datetime.now()
  month = now.strftime('%B')
  year = now.strftime('%Y')

  # if stdin is a file, read the file instead of downloading (usefull for testing)
  if sys.__stdin__.isatty():
    print(f"Get template list from {LIST_URL}\n")
    template_list = get_templates_list()
  else:
    print("Get template list from stdin\n")
    template_list = sys.__stdin__.readlines()

  # Write to root/TEMPLATES.md
  fout = open(TEMPLATES,"w+")

  fout.write("<img src=\"/tools/logo/TASMOTA_FullLogo_Vector.svg\" alt=\"Logo\" align=\"right\" height=\"76\"/>\n")
  fout.write("\n")
  fout.write("# Templates\n")
  fout.write("\n")
  fout.write("Find below the available templates as of " + month + " " + year + ". More template information can be found in the [Tasmota Device Templates Repository](http://blakadder.github.io/templates)\n")

  first = True
  for line in template_list:
    if line.strip():
      if line.startswith("##"):
        if not first:
          fout.write('```\n')
        fout.write('\n')
        fout.write(line+'\n')
        fout.write('```\n')
        first = False
      elif line.startswith("#"):
        pass
      else:
        json_start = line.find("{")
        if json_start > -1:
          name = line[:json_start].strip()
          try:
            template = json.loads(line[json_start:])
            template = convert_template(template)
          except json.decoder.JSONDecodeError as ex:
            template = "Not available"

          fout.write(f"{name:<{COLUMN}}  {json.dumps(template, separators=(',', ':'))}\n")
#        else:
#          fout.write(line+'\n')

  fout.write('```\n')

  fout.close()

if __name__ == "__main__":
  main()
