
/*
  xdrv_38_somfy.ino - Somfy RF Rolling Code Support
  Based on https://github.com/Nickduino/Somfy_Remote 

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

#ifdef USE_SOMFY

#define XDRV_38 38

/*********************************************************************************************\
 * Send Somfy RTS commands via a connected 433MHz transmitter module
\*********************************************************************************************/
const char kSomfyCommands[] PROGMEM = "Somfy|" // prefix
  "Up|Down|Stop|Prog|Set";

void (* const somfyCommand[])(void) PROGMEM = {
  &CmdUp, &CmdDown, &CmdStop, &CmdProg, &CmdSomfySet};

#define SYMBOL 640
#define SOMFY_UP 0x2
#define SOMFY_STOP 0x1
#define SOMFY_DOWN 0x4
#define SOMFY_PROG 0x8

byte frame[7];
byte checksum;

void BuildFrame(byte *frame, byte button);
void SendCommand(byte *frame, byte sync);

void somfysetup() {
  pinMode(pin[GPIO_SOMFY_RTS_TX], OUTPUT);
  digitalWrite(pin[GPIO_SOMFY_RTS_TX], LOW);
}

void BuildFrame(byte *frame, byte button) {
  unsigned int code, remote;
  code = Settings.rolling_code_count;
  remote = Settings.rolling_code_serial;
  frame[0] = 0xA7; // Encryption key. Doesn't matter much
  frame[1] = button << 4;  // Which button did you press? The 4 LSB will be the checksum
  frame[2] = code >> 8;    // Rolling code (big endian)
  frame[3] = code;         // Rolling code
  frame[4] = remote >> 16; // Remote address
  frame[5] = remote >>  8; // Remote address
  frame[6] = remote;       // Remote address
// Checksum calculation: a XOR of all the nibbles
  checksum = 0;
  for(byte i = 0; i < 7; i++) {
    checksum = checksum ^ frame[i] ^ (frame[i] >> 4);
  }
  checksum &= 0b1111; // We keep the last 4 bits only
//Checksum integration
  frame[1] |= checksum; //  If a XOR of all the nibbles is equal to 0, the blinds will
                        // consider the checksum ok.
// Obfuscation: a XOR of all the bytes
  for(byte i = 1; i < 7; i++) {
    frame[i] ^= frame[i-1];
  }

  code++;
  Settings.rolling_code_count = code;
}

void SendCommand(byte *frame, byte sync) {
  if(sync == 2) { // Only with the first frame.
  //Wake-up pulse & Silence
  digitalWrite(pin[GPIO_SOMFY_RTS_TX], HIGH);
    delayMicroseconds(9415);
  digitalWrite(pin[GPIO_SOMFY_RTS_TX], LOW);
    delayMicroseconds(89565);
  }
// Hardware sync: two sync for the first frame, seven for the following ones.
  for (int i = 0; i < sync; i++) {
  digitalWrite(pin[GPIO_SOMFY_RTS_TX], HIGH);
    delayMicroseconds(4*SYMBOL);
  digitalWrite(pin[GPIO_SOMFY_RTS_TX], LOW);
    delayMicroseconds(4*SYMBOL);
  }
// Software sync
  digitalWrite(pin[GPIO_SOMFY_RTS_TX], HIGH);
  delayMicroseconds(4550);
  digitalWrite(pin[GPIO_SOMFY_RTS_TX], LOW);
  delayMicroseconds(SYMBOL);

//Data: bits are sent one by one, starting with the MSB.
  for(byte i = 0; i < 56; i++) {
    if(((frame[i/8] >> (7 - (i%8))) & 1) == 1) {
  digitalWrite(pin[GPIO_SOMFY_RTS_TX], LOW);
      delayMicroseconds(SYMBOL);
  digitalWrite(pin[GPIO_SOMFY_RTS_TX], HIGH);
      delayMicroseconds(SYMBOL);
    }
    else {
  digitalWrite(pin[GPIO_SOMFY_RTS_TX], HIGH);
      delayMicroseconds(SYMBOL);
  digitalWrite(pin[GPIO_SOMFY_RTS_TX], LOW);
      delayMicroseconds(SYMBOL);
    }
  }

  digitalWrite(pin[GPIO_SOMFY_RTS_TX], LOW);

  delayMicroseconds(30415); // Inter-frame silence
}

void CmdUp()
{
  Cmd(SOMFY_UP);
}

void CmdDown()
{
  Cmd(SOMFY_DOWN);
}

void CmdStop()
{
  Cmd(SOMFY_STOP);
}

void CmdProg()
{
  Cmd(SOMFY_PROG);
}

void Cmd(byte cmdCode)
{
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Sending using remote %08x. Current Somfy Rolling Code: %u"), Settings.rolling_code_serial, Settings.rolling_code_count);
  somfysetup();
  BuildFrame(frame, cmdCode);
  SendCommand(frame, 2);
  for(int i = 0; i<2; i++) {
    SendCommand(frame, 7);
  }
  ResponseCmndDone();
}

void CmdSomfySet(void)
{
  if (XdrvMailbox.data_len > 0) {
    if (XdrvMailbox.payload > 0) {
      char *p;
      uint32_t i = 0;
      uint32_t param[2] = { 0 };
      for (char *str = strtok_r(XdrvMailbox.data, ", ", &p); str && i < 2; str = strtok_r(nullptr, ", ", &p)) {
        param[i] = strtoul(str, nullptr, 0);
        i++;
      }
      
      Settings.rolling_code_serial = param[0];
      Settings.rolling_code_count = param[1];

      ResponseCmndDone();
    } else {
      DEBUG_DRIVER_LOG(LOG_LEVEL_DEBUG_MORE, PSTR("no payload"));
    }
  } else {
    DEBUG_DRIVER_LOG(LOG_LEVEL_DEBUG_MORE, PSTR("no param"));
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdrv38(uint8_t function)
{
  bool result = false;

  if(99 == pin[GPIO_SOMFY_RTS_TX]) return result;

  switch (function) {
    case FUNC_COMMAND:
      AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("calling command"));
      result = DecodeCommand(kSomfyCommands, somfyCommand);
      break;
    case FUNC_INIT:
      DEBUG_DRIVER_LOG(LOG_LEVEL_DEBUG_MORE, PSTR("init done."));
      break;
  }

  return result;
}
#endif  // USE_SOMFY
