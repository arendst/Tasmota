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

#ifndef __ARDUINOHEXPARSE_H__

#include <Arduino.h>

class ArduinoHexParse {
  public:
    ArduinoHexParse(void);
    void ParseLine(byte* data);
    byte* GetFlashPage(void);
    byte* GetLoadAddress(void);
    bool IsFlashPageReady(void);
  private:
    uint32_t address = 0;
    uint32_t len = 0;
    uint32_t nextAddress = 0;
    uint32_t PageMemIdx = 0;
    uint32_t recordType = 0;
    byte FlashPage[128];
    byte loadAddress[2];
    bool FlashPageReady = false;
    bool firstRun = true;
    uint32_t GetAddress(byte* hexline);
    uint16_t GetLength(byte* hexline);
    uint16_t GetRecordType(byte* hexline);
    byte* GetData(byte* hexline, uint32_t len);
    void GetLoadAddress(byte* hexline);
    void EndOfFile(void);
};

#endif // __ARDUINOHEXPARSE_H__