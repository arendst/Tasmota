/*
  Copyright (C) 2019  Andre Thomas and Theo Arends

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
*/

#include <Arduino.h>
#include "ArduinoHexParse.h"

ArduinoHexParse::ArduinoHexParse(void)
{
  loadAddress[0] = 0;
  loadAddress[1] = 0;
}

void ArduinoHexParse::ParseLine(byte* hexline)
{
  recordType = GetRecordType(hexline);
  if (0 == recordType) {
    address = GetAddress(hexline);
    len = GetLength(hexline);
    GetData(hexline, len);
    if (128 == PageMemIdx) {
      if (!firstRun) {
        loadAddress[1] += 0x40;
        if (0 == loadAddress[1]) {
          loadAddress[0] += 1;
        }
      }
      firstRun = false;
      FlashPageReady = true;
      PageMemIdx = 0;
    }
    nextAddress = address + len;
  }
  if (1 == recordType) {
    EndOfFile();
    FlashPageReady = true;
  }
}

bool ArduinoHexParse::IsFlashPageReady(void)
{
  return FlashPageReady;
}

byte* ArduinoHexParse::GetFlashPage(void)
{
  FlashPageReady = false;
  return FlashPage;
}

byte* ArduinoHexParse::GetLoadAddress(void)
{
  return loadAddress;
}

void ArduinoHexParse::GetLoadAddress(byte* hexline)
{
  char buff[3];
  buff[2] = '\0';
  buff[0] = hexline[3];
  buff[1] = hexline[4];
  loadAddress[0] = strtol(buff, 0, 16); 
  buff[0] = hexline[5];
  buff[1] = hexline[6];
  loadAddress[1] = strtol(buff, 0, 16); 
}

byte* ArduinoHexParse::GetData(byte* hexline, uint32_t len)
{
  uint32_t start = 9;
  uint32_t end = (len * 2) + start;
  char buff[3];
  buff[2] = '\0';
  for (uint32_t x = start; x < end; x = x+2) {
    buff[0] = hexline[x];
    buff[1] = hexline[x+1];
    FlashPage[PageMemIdx] = strtol(buff, 0, 16);
    PageMemIdx++;
  }
}

void ArduinoHexParse::EndOfFile(void)
{
  loadAddress[1] += 0x40;
  if (0 == loadAddress[1]) {
    loadAddress[0] += 1;
  }
  while (128 > PageMemIdx) {          // Fill the remaing space in the memory page with 0xFF
    FlashPage[PageMemIdx] = 0xFF;
    PageMemIdx++;
  }
}

uint32_t ArduinoHexParse::GetAddress(byte* hexline)
{
  char buff[5];
  buff[0] = hexline[3];
  buff[1] = hexline[4];
  buff[2] = hexline[5];
  buff[3] = hexline[6];
  buff[4] = '\0';
  return strtol(buff, 0, 16); 
}

uint16_t ArduinoHexParse::GetLength(byte* hexline)
{
  char buff[3];
  buff[0] = hexline[1];
  buff[1] = hexline[2];
  buff[2] = '\0';
  return strtol(buff, 0, 16); 
}

uint16_t ArduinoHexParse::GetRecordType(byte* hexline)
{
  char buff[3];
  buff[0] = hexline[7];
  buff[1] = hexline[8];
  buff[2] = '\0';
  return strtol(buff, 0, 16); 
}
