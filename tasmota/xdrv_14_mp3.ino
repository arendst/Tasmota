/*
  xdrv_14_mp3.ino - MP3 support for Tasmota

  Copyright (C) 2020  gemu2015, mike2nl and Theo Arends

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

  --------------------------------------------------------------------------------------------
  Version yyyymmdd  Action    Description
  --------------------------------------------------------------------------------------------
  1.0.0.4 20181003  added   - MP3Reset command in case that the player do rare things
                            - and needs a reset, the default volume will be set again too
                    added   - MP3_CMD_RESET_VALUE for the player reset function
                    cleaned - some comments and added function text header
                    fixed   - missing void's in function calls
                    added   - MP3_CMD_DAC command to switch off/on the dac outputs
                    tested  - works with MP3Device 1 = USB STick, or MP3Device 2 = SD-Card
                            - after power and/or reset the SD-Card(2) is the default device
                            - DAC looks working too on a headset. Had no amplifier for test
  ---
  1.0.0.3 20180915  added   - select device for SD-Card or USB Stick, default will be SD-Card
                    tested  - works by MP3Device 1 = USB STick, or MP3Device 2 = SD-Card
                            - after power and/or reset the SD-Card(2) is the default device
  ---
  1.0.0.2 20180912  added   - again some if-commands to switch() because of new commands
  ---
  1.0.0.1 20180911  added   - command eq (equalizer 0..5)
                    tested  - works in console with MP3EQ 1, the value can be 0..5
                    added   - USB device selection via command in console
                    tested  - looks like it is working
                    erased  - code for USB device about some errors, will be added in a next release
  ---
  1.0.0.1 20180910  changed - command real MP3Stop in place of pause/stop used in the original version
                    changed - the command MP3Play e.g. 001 to MP3Track e.g. 001,
                    added   - new normal command MP3Play and MP3Pause
  ---
  1.0.0.0 20180907  merged  - by arendst
                    changed - the driver name from xdrv_91_mp3.ino to xdrv_14_mp3.ino
  ---
  0.9.0.3 20180906  request - Pull Request
                    changed - if-commands to switch() for faster response
  ---
  0.9.0.2 20180906  cleaned - source code for faster reading
  ---
  0.9.0.1 20180905  added   - #include <TasmotaSerial.h> because compiler error (Arduino IDE v1.8.5)
  ---
  0.9.0.0 20180901  started - further development by mike2nl  - https://github.com/mike2nl/Sonoff-Tasmota
                    base    - code base from gemu2015 ;-)     - https://github.com/gemu2015/Sonoff-Tasmota
                    forked  - from arendst/tasmota            - https://github.com/arendst/Tasmota

*/

#ifdef USE_MP3_PLAYER
/*********************************************************************************************\
 * MP3 control for RB-DFR-562 DFRobot mini MP3 player
 * https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299
\*********************************************************************************************/

#define XDRV_14             14

#include <TasmotaSerial.h>

TasmotaSerial *MP3Player;

/*********************************************************************************************\
 * constants
\*********************************************************************************************/

#define D_CMND_MP3 "MP3"

const char S_JSON_MP3_COMMAND_NVALUE[] PROGMEM = "{\"" D_CMND_MP3 "%s\":%d}";
const char S_JSON_MP3_COMMAND[] PROGMEM        = "{\"" D_CMND_MP3 "%s\"}";
const char kMP3_Commands[] PROGMEM             = "Track|Play|Pause|Stop|Volume|EQ|Device|Reset|DAC";

/*********************************************************************************************\
 * enumerationsines
\*********************************************************************************************/

enum MP3_Commands {                                 // commands useable in console or rules
  CMND_MP3_TRACK,                                   // MP3Track 001...255
  CMND_MP3_PLAY,                                    // MP3Play, after pause or normal start to play
  CMND_MP3_PAUSE,                                   // MP3Pause
  CMND_MP3_STOP,                                    // MP3Stop, real stop, original version was pause function
  CMND_MP3_VOLUME,                                  // MP3Volume 0..100
  CMND_MP3_EQ,                                      // MP3EQ 0..5
  CMND_MP3_DEVICE,                                  // sd-card: 02, usb-stick: 01
  CMND_MP3_RESET,                                   // MP3Reset, a fresh and default restart
  CMND_MP3_DAC };                                   // set dac, 1=off, 0=on, DAC is turned on (0) by default


/*********************************************************************************************\
 * command defines
\*********************************************************************************************/

#define MP3_CMD_RESET_VALUE 0                       // mp3 reset command value
// player commands
#define MP3_CMD_TRACK       0x03                    // specify playback of a track, e.g. MP3Track 003
#define MP3_CMD_PLAY        0x0d                    // Play, works as a normal play on a real MP3 Player, starts at 001.mp3 file on the selected device
#define MP3_CMD_PAUSE       0x0e                    // Pause, was original designed as stop, see data sheet
#define MP3_CMD_STOP        0x16                    // Stop, it's a real stop now, in the original version it was a pause command
#define MP3_CMD_VOLUME      0x06                    // specifies the volume and means a console input as 0..100
#define MP3_CMD_EQ          0x07                    // specify EQ(0/1/2/3/4/5), 0:Normal, 1:Pop, 2:Rock, 3:Jazz, 4:Classic, 5:Bass
#define MP3_CMD_DEVICE      0x09                    // specify playback device, USB=1, SD-Card=2, default is 2 also after reset or power down/up
#define MP3_CMD_RESET       0x0C                    // send a reset command to start fresh
#define MP3_CMD_DAC         0x1A                    // activate or deactivate the DAC output for an external amplifier, DAC is turned on by default

/*********************************************************************************************\
 * calculate the checksum
 * starts with cmd[1] with a length of 6 bytes
\*********************************************************************************************/

uint16_t MP3_Checksum(uint8_t *array)
{
  uint16_t checksum = 0;
  for (uint32_t i = 0; i < 6; i++) {
    checksum += array[i];
  }
  checksum = checksum^0xffff;
  return (checksum+1);
}

/*********************************************************************************************\
 * init player
 * define serial tx port fixed with 9600 baud
\*********************************************************************************************/

void MP3PlayerInit(void) {
  MP3Player = new TasmotaSerial(-1, Pin(GPIO_MP3_DFR562));
  // start serial communication fixed to 9600 baud
  if (MP3Player->begin(9600)) {
    MP3Player->flush();
    delay(1000);
    MP3_CMD(MP3_CMD_RESET, MP3_CMD_RESET_VALUE);    // reset the player to defaults
    delay(3000);
    MP3_CMD(MP3_CMD_VOLUME, MP3_VOLUME);            // after reset set volume depending on the entry in the my_user_config.h
  }
  return;
}

/*********************************************************************************************\
 * create the MP3 commands payload, and send it via serial interface to the MP3 player
 * data length is 6 = 6 bytes [FF 06 09 00 00 00] but not counting the start, end, and verification.
 * {start byte, version, length, command, feedback, para MSB, para LSB, chks MSB, chks LSB, end byte};
 * {cmd[0]    , cmd[1] , cmd[2], cmd[3] , cmd[4]  , cmd[5]  , cmd[6]  , cmd[7]  , cmd[8]  , cmd[9]  };
 * {0x7e      , 0xff   , 6     , 0      , 0/1     , 0       , 0       , 0       , 0       , 0xef    };
\*********************************************************************************************/

void MP3_CMD(uint8_t mp3cmd,uint16_t val) {
  uint8_t i       = 0;
  uint8_t cmd[10] = {0x7e,0xff,6,0,0,0,0,0,0,0xef}; // fill array
  cmd[3]          = mp3cmd;                         // mp3 command value
  cmd[4]          = 0;                              // feedback, 1=yes, 0=no, yet not use
  cmd[5]          = val>>8;                         // data value, shift 8 byte right
  cmd[6]          = val;                            // data value low byte
  uint16_t chks   = MP3_Checksum(&cmd[1]);          // see calculate the checksum
  cmd[7]          = chks>>8;                        // checksum. shift 8 byte right
  cmd[8]          = chks;                           // checksum low byte
  MP3Player->write(cmd, sizeof(cmd));               // write mp3 data array to player
  delay(1000);
  if (mp3cmd == MP3_CMD_RESET) {
    MP3_CMD(MP3_CMD_VOLUME, MP3_VOLUME);            // after reset set volume depending on the entry in the my_user_config.h
  }
  return;
}

/*********************************************************************************************\
 * check the MP3 commands
\*********************************************************************************************/

bool MP3PlayerCmd(void) {
  char command[CMDSZ];
  bool serviced = true;
  uint8_t disp_len = strlen(D_CMND_MP3);

  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_MP3), disp_len)) {  // prefix
    int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + disp_len, kMP3_Commands);

    switch (command_code) {
      case CMND_MP3_TRACK:
      case CMND_MP3_VOLUME:
      case CMND_MP3_EQ:
      case CMND_MP3_DEVICE:
      case CMND_MP3_DAC:
        // play a track, set volume, select EQ, sepcify file device
        if (XdrvMailbox.data_len > 0) {
          if (command_code == CMND_MP3_TRACK)  { MP3_CMD(MP3_CMD_TRACK,  XdrvMailbox.payload); }
          if (command_code == CMND_MP3_VOLUME) { MP3_CMD(MP3_CMD_VOLUME, XdrvMailbox.payload * 30 / 100); }
          if (command_code == CMND_MP3_EQ)     { MP3_CMD(MP3_CMD_EQ,     XdrvMailbox.payload); }
          if (command_code == CMND_MP3_DEVICE) { MP3_CMD(MP3_CMD_DEVICE, XdrvMailbox.payload); }
          if (command_code == CMND_MP3_DAC)    { MP3_CMD(MP3_CMD_DAC,    XdrvMailbox.payload); }
        }
        Response_P(S_JSON_MP3_COMMAND_NVALUE, command, XdrvMailbox.payload);
        break;
      case CMND_MP3_PLAY:
      case CMND_MP3_PAUSE:
      case CMND_MP3_STOP:
      case CMND_MP3_RESET:
        // play or re-play after pause, pause, stop,
        if (command_code == CMND_MP3_PLAY)     { MP3_CMD(MP3_CMD_PLAY,   0); }
        if (command_code == CMND_MP3_PAUSE)    { MP3_CMD(MP3_CMD_PAUSE,  0); }
        if (command_code == CMND_MP3_STOP)     { MP3_CMD(MP3_CMD_STOP,   0); }
        if (command_code == CMND_MP3_RESET)    { MP3_CMD(MP3_CMD_RESET,  0); }
        Response_P(S_JSON_MP3_COMMAND, command, XdrvMailbox.payload);
        break;
      default:
    	  // else for Unknown command
    	  serviced = false;
    	break;
    }
  } else {
    return false;
  }
  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv14(uint8_t function)
{
  bool result = false;

  if (PinUsed(GPIO_MP3_DFR562)) {
    switch (function) {
      case FUNC_PRE_INIT:
        MP3PlayerInit();                              // init and start communication
        break;
      case FUNC_COMMAND:
        result = MP3PlayerCmd();                      // return result from mp3 player command
        break;
    }
  }
  return result;
}

#endif  // USE_MP3_PLAYER
