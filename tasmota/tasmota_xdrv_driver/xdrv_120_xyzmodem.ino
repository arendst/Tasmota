/*
  xdrv_120_xyzmodem.ino - XModem support for Tasmota

  SPDX-FileCopyrightText: 2025 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_XYZMODEM
/*********************************************************************************************\
 * XModem, XModem/CRC, Xmodem-1k file transfer protocol
 *
 * Usage:
 *  Use a tool able supporting XModem file transfer and to connect either using serial or telnet with Tasmota.
 *   TeraTerm (When using telnet enable non-bare CR support with Tasmota command `Teraterm 1`)
 *   SyncTerm
 *   SecureCRT (When using Telnet set all Telnet Advanced Options OFF EXCEPT 'Server requires bare CR`)
 *
 *  To start XModem file transfer first execute Tasmota command XSend or XReceive,
 *   then start tool XModem receive or send.
 *
 * Commands:
 *  TeraTerm                                    - Show current selection
 *  TeraTerm 0                                  - Disable adding zero after carriage return (0x0D)
 *  TeraTerm 1                                  - Enable adding zero after carriage return (0x0D)
 *  XSend <filename>                            - Send filename
 *  XSend Settings                              - Send active settings
 *  XSend autoexec.bat                          - Send file autoexec.bat
 *  XReceive[<checksum>] <filename>[,<trim1a>]  - Receive filename with checksum (default CRC) and trim CP/M SUB from end of file (default trim)
 *  XReceive Settings                           - Receive and activate settings using CRC (No need for trim1a as it's size is on 128/1024 bytes boundary)
 *  XReceive autoexec.bat                       - Receive file autoexec.bat using CRC and trim CP/M SUB from end of file
 *  XReceive autoexec.bat,0                     - Receive file autoexec.bat using CRC and do not trim CP/M SUB from end of file
 *  XReceive autoexec.bat,1                     - Receive file autoexec.bat using CRC and trim CP/M SUB from end of file (default)
 *  XReceive autoexec.bat,2                     - Receive file autoexec.bat using CRC and auto trim CP/M SUB from end of text file
 *  XReceive2 autoexec.bat                      - Receive file autoexec.bat using checksum (less secure)
 *
 * Resource: https://gallium.inria.fr/~doligez/zmodem/ymodem.txt
\*********************************************************************************************/

#define XDRV_120     120

//#define XYZM_DEBUG

#define XYZM_SOH     0x01    // Start of 128 byte data
#define XYZM_STX     0x02    // Start of 1024 byte data
#define XYZM_EOT     0x04
#define XYZM_ACK     0x06
#define XYZM_BS      0x08
#define XYZM_LF      0x0a
#define XYZM_CR      0x0d
#define XYZM_NAK     0x15
#define XYZM_CAN     0x18
#define XYZM_SUB     0x1a

// Number of seconds until giving up hope of receiving sync packets from host.
const uint8_t  XYZMODEM_SYNC_TIMEOUT = 30;
const uint8_t  XYZMODEM_RECV_TIMEOUT_SHORT = 1;       // Protocol = 10 seconds
const uint8_t  XYZMODEM_RECV_TIMEOUT_LONG = 3;        // Protocol = 60 seconds

// Number of times we try to send a packet to the host until we give up sending..
const uint8_t  XYZMODEM_MAX_RETRY = 4;                // Protocol = 10 for total packets to be send. Here retry per packet
// Packet size
const uint8_t  XYZMODEM_SOH_PACKET_SIZE = 128;
const uint16_t XYZMODEM_STX_PACKET_SIZE = 1024;
const uint16_t XYZMODEM_FILE_SECTOR_SIZE = 2048;

enum XTrim1aModes { XYZT_NONE, XYZT_TRIM, XYZT_AUTO };

enum XReceiveModes { XYZD_NONE, XYZD_SOH, XYZD_BLK1, XYZD_BLK2, XYZD_DATA };

enum XYZFileSteps { XYZM_IDLE, 
                    XYZM_SEND, XYZM_SND_ACK,
                    XYZM_RECEIVE, XYZM_RCV_START, XYZM_RCV_EOT,
                    XYZM_ERROR, XYZM_DONE };

enum XReceiveStates { XYZS_OK, XYZS_TIMEOUT, XYZS_EOT, XYZS_CAN, XYZS_OTHER, XYZS_CHECKSUM, XYZS_PACKET, XYZS_FILE };

#ifdef USE_UFILESYS
extern FS *ffsp;
#endif

#include <TasmotaSerial.h>

struct {
  TasmotaSerial *serial;
  WiFiClient    *client;
  uint32_t       timeout;
  uint32_t       filepos;
  uint32_t       packet_no;
  uint16_t       packet_size;
  uint16_t       crcBuf;
  uint8_t        receive_timeout;
  uint8_t        can_count;
  uint8_t        nak_count;
  uint8_t        checksumBuf;
  uint8_t        protocol;
  uint8_t        enabled;
  uint8_t        mode;
  uint8_t        retry;
  bool           oldChecksum;
  bool           teraterm;
} XYZModem;

struct {
  uint32_t       size;
  int32_t        sector_counter;
  uint32_t       byte_counter;
  uint8_t       *buffer;
  char           file_name[48];
  uint8_t        step;
  uint8_t        state;
  uint8_t        trim1a;
  bool           file;
} XYZFile;

/*********************************************************************************************\
 * Low level read and write
\*********************************************************************************************/

int XYZModemAvailable(void) {
  switch (XYZModem.protocol) {
    case TXMP_SERIAL:
      return XYZModem.serial->available();   // TasmotaSerial
    case TXMP_TASCONSOLE:
      return TasConsole.available();         // Serial or TASCONSOLE
    case TXMP_TELNET:
      return XYZModem.client->available();   // WiFiClient
  }
  return -1;
}

int XYZModemRead(void) {
  switch (XYZModem.protocol) {
    case TXMP_SERIAL:
      return XYZModem.serial->read();
    case TXMP_TASCONSOLE:
      return TasConsole.read();
    case TXMP_TELNET:
      return XYZModem.client->read();
  }
  return -1;
}

size_t XYZModemWrite(uint8_t data) {
  switch (XYZModem.protocol) {
    case TXMP_SERIAL:
      return XYZModem.serial->write(data);
    case TXMP_TASCONSOLE:
      return TasConsole.write(data);
    case TXMP_TELNET:
      return XYZModem.client->write(data);
  }
  return 0;
}

size_t XYZModemWriteBuf(const uint8_t *buf, size_t size) {
  switch (XYZModem.protocol) {
    case TXMP_SERIAL:
      return XYZModem.serial->write(buf, size);
    case TXMP_TASCONSOLE:
      return TasConsole.write(buf, size);
    case TXMP_TELNET:
      return XYZModem.client->write(buf, size);
  }
  return 0;
}

/*********************************************************************************************\
 * File or Settings read and write
\*********************************************************************************************/

bool XYZModemBufferAlloc(void) {
  if (nullptr == XYZFile.buffer) {
    // Tradeoff between memory usage and speed (file access is slow)
    if (!(XYZFile.buffer = (uint8_t *)malloc(XYZMODEM_FILE_SECTOR_SIZE))) {
      return false;                                  // Not enough (memory) space
    }
  }
  return true;
}

void XYZModemBufferFree(void) {
  if (XYZFile.buffer) { 
    free(XYZFile.buffer);
    XYZFile.buffer = nullptr;
  }
}

uint32_t XYZModemFileAvailable(void) {
  int available = XYZFile.size - XYZFile.byte_counter;
  if (available < 0) { available = 0; }
  return available;
}

int XYZModemFileRead(void) {
  // When the source device reaches the last XModem data block, it should be padded to 128 bytes
  // of data using SUB (ASCII 0x1A) characters.
  int data = XYZM_SUB;

  if (XYZModemFileAvailable()) {
    if (XYZFile.file) {
#ifdef USE_UFILESYS
      if (!XYZModemBufferAlloc()) {
        return -1;                                     // Not enough (memory) space
      }

      uint32_t index = XYZFile.byte_counter % XYZMODEM_FILE_SECTOR_SIZE;
      int32_t sector = XYZFile.byte_counter / XYZMODEM_FILE_SECTOR_SIZE;
      if (sector != XYZFile.sector_counter) {
        XYZFile.sector_counter = sector;
        File file = ffsp->open(XYZFile.file_name, "r");
        if (file && file.seek(XYZFile.byte_counter)) {
          file.read(XYZFile.buffer, XYZMODEM_FILE_SECTOR_SIZE);
          file.close();
        }
      }
      if (XYZFile.byte_counter <= XYZFile.size) {
        data = XYZFile.buffer[index];
      }
      if (XYZFile.byte_counter == XYZFile.size) {
        XYZModemBufferFree();
      }
#endif  // USE_UFILESYS
    } else {
      if (0 == XYZFile.byte_counter) {
        XYZFile.size = SettingsConfigBackup();
        if (0 == XYZFile.size) {
          return -1;                                   // Not enough (memory) space
        }
      }
      if (XYZFile.byte_counter <= XYZFile.size) {
        data = settings_buffer[XYZFile.byte_counter];
      }
      if (XYZFile.byte_counter == XYZFile.size) {
        SettingsBufferFree();
      }
    }
  }
  XYZFile.byte_counter++;
  return data;
}

/*********************************************************************************************/

bool XYZModemFileWrite(const uint8_t *buffer) {
  if (XYZFile.file) {                                // File
#ifdef USE_UFILESYS
    if (!XYZModemBufferAlloc()) {
      return false;                                  // Not enough (memory) space
    }

    // Keep space for mixed 128 and 1k packages
    uint32_t max_size = XYZMODEM_FILE_SECTOR_SIZE - XYZMODEM_STX_PACKET_SIZE;
    if (nullptr == buffer) {                         // At EOT
      if (XYZFile.trim1a) {
        uint32_t eot = XYZFile.sector_counter;       // (Auto) trim SUB from text files
        while ((eot > 0) && (XYZM_SUB == XYZFile.buffer[--eot])) {}
        if (XYZT_AUTO == XYZFile.trim1a) {
          if ((XYZM_LF == XYZFile.buffer[eot]) || (XYZM_CR == XYZFile.buffer[eot])) {
            eot++;
          }
        } else {
          eot++;
        }
        XYZFile.byte_counter -= (XYZFile.sector_counter - eot);  // Final file size
        XYZFile.sector_counter = eot;
      }
      max_size = 0;                                  // Force flush
    }

    if (XYZFile.sector_counter > max_size) {
//      AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Save file size %d,%d"), XYZFile.sector_counter, max_size);

      File file;
      if ((XYZFile.byte_counter - XYZFile.sector_counter) <= XYZFile.sector_counter) {
        ffsp->remove(XYZFile.file_name);
        file = ffsp->open(XYZFile.file_name, "w");
      } else {
        file = ffsp->open(XYZFile.file_name, "a");
      }
      if (file) {
        size_t size = 0;
        if (UfsFree() > 7) {                         // Fix ESP32 file.write() not detecting filesystem full
          size = file.write(XYZFile.buffer, XYZFile.sector_counter);
        }
        file.close();
        if (size != XYZFile.sector_counter) {
          return false;                              // File write error - disk full
        }
      } else {
        return false;                                // Unable to open file
      }
      XYZFile.sector_counter = 0;
    }

    if (buffer) {                                    // Add receive buffer to file save buffer
      memcpy(XYZFile.buffer + XYZFile.sector_counter, buffer, XYZModem.packet_size);
      XYZFile.sector_counter += XYZModem.packet_size;
    }
#endif  // USE_UFILESYS
  } else {                                           // Settings
    uint32_t index = XYZFile.byte_counter - XYZModem.packet_size;
    if (0 == index) {                                // First package
      uint32_t set_size = sizeof(TSettings);
#ifdef USE_UFILESYS
      if (('s' == buffer[2]) && ('e' == buffer[3])) {  // /.settings
        set_size = buffer[14] + (buffer[15] << 8);
      }
#endif  // USE_UFILESYS
      if (!SettingsBufferAlloc(set_size)) {
        return false;                                // Not enough (memory) space
      }
    }

    if (nullptr == buffer) {                         // Flush receive buffer
      return true;                                   // Already flushed
    }

    uint32_t package_size = XYZModem.packet_size;
    if (XYZFile.byte_counter > settings_size) {
      package_size -= (XYZFile.byte_counter - settings_size);
    }
    memcpy(settings_buffer + index, buffer, package_size);
  }
  return true;
}

void XYZModemFileWriteEot(bool state) {
  if (XYZFile.file) {
#ifdef USE_UFILESYS
    XYZModemBufferFree();                            // Release buffer
    if (0 == state) {
      ffsp->remove(XYZFile.file_name);               // Remove corrupted file
    }
#endif  // USE_UFILESYS
  } else {
    if (1 == state) {
      if (SettingsConfigRestore()) {                 // Process new settings and release buffer
        TasmotaGlobal.restart_flag = 2;              // Restart on valid settings
      }
    } else {
      SettingsBufferFree();                          // Release buffer
    }
  }
}

/*********************************************************************************************\
 * Xmodem read
\*********************************************************************************************/

bool XYZModemReadAvailable(uint32_t timeout) {
  int i = 0;
  while (!XYZModemAvailable()) {
    delayMicroseconds(100);
    i++;
    if (i > timeout * 10000) {                       // 10000 * 100 uS = 1 Sec (Protocol Initial 40 Sec, then 20 Sec)
      return false;
    }
  }
  return true;
}

int XYZModemReadByte(void) {
  uint8_t telnet_buffer[3];
  while (true) {
    if (!XYZModemReadAvailable(XYZModem.receive_timeout)) {
      return -1;
    }
    int in_char = XYZModemRead();
#ifdef XYZM_DEBUG
    Serial.printf("%02X",in_char);
#endif
    if (in_char >= 0) {
      if (TXMP_TELNET == XYZModem.protocol) {
        if (0xFF == in_char) {                       // Telnet IAC - Fix XModem over Telnet escape 
          telnet_buffer[0] = in_char;
          int in_char2 = XYZModemRead();
#ifdef XYZM_DEBUG
          Serial.printf("[%02X]",in_char2);
#endif
          if ((in_char2 >= 251) && (in_char2 <= 254)) {  // Telnet IAC - WILL, DO, WONT, DONT
            if (251 == in_char2) {                   // 251 = 0xFB = WILL
              telnet_buffer[1] = 254;                // 254 = 0xFE = DONT
            }
            else if (253 == in_char2) {              // 253 = 0xFD = DO
              telnet_buffer[1] = 252;                // 252 = 0xFC = WONT
            }
            int in_char3 = XYZModemRead();
            telnet_buffer[2] = in_char3;
#ifdef XYZM_DEBUG
            Serial.printf("(%02X)",in_char3);
#endif
            XYZModemWriteBuf(telnet_buffer, 3);      // Send not supported telnet functions reponse(s)
            continue;
          }
        }
        if (XYZModem.teraterm && (0x0D == in_char)) {  // Fix TeraTerm
          int in_char2 = XYZModemRead();
#ifdef XYZM_DEBUG
          Serial.printf("[%02X]",in_char2);
#endif
        }
      }
      return in_char;
    }
    break;
  }
  return -1;
}

/*********************************************************************************************\
 * Xmodem Send
\*********************************************************************************************/

// Wait for the remote to acknowledge or cancel.
// Returns the received char if no timeout occured or a CAN was received. In this cases, it returns -1.
int XYZModemWaitACK(void) {
  int in_char;
  do {
    in_char = XYZModemReadByte();
    if (in_char < 0) { return -1; }                  // Timeout
    if (XYZM_CAN == in_char) { return XYZM_CAN; }    // Cancel
  } while ((in_char != XYZM_NAK) && (in_char != XYZM_ACK) && (in_char != 'C'));
  return in_char;
}

// Calculate checksum
void XYZModemChecksum(uint8_t out_char) {
  XYZModem.checksumBuf += out_char;

  XYZModem.crcBuf = XYZModem.crcBuf ^ (uint16_t)out_char << 8;
  for (uint32_t i = 0; i < 8; i++) {
    if (XYZModem.crcBuf & 0x8000) {
      XYZModem.crcBuf = XYZModem.crcBuf << 1 ^ 0x1021;
    } else {
      XYZModem.crcBuf = XYZModem.crcBuf << 1;
    }
  }
}

void XYZModemSendEscaped(uint8_t *xmodem_buffer, uint32_t *xmodem_buffer_ptr, uint8_t in_char) {
  uint32_t buffer_ptr = *xmodem_buffer_ptr;
  xmodem_buffer[buffer_ptr++] = in_char;
  if (TXMP_TELNET == XYZModem.protocol) {
    if (0xFF == in_char) {                           // Fix XModem over Telnet escape 
      xmodem_buffer[buffer_ptr++] = in_char;
    }
    if (XYZModem.teraterm && (0x0D == in_char)) {    // Fix TeraTerm
      xmodem_buffer[buffer_ptr++] = 0x00;
    }
  }
  *xmodem_buffer_ptr = buffer_ptr;
}

bool XYZModemSend(uint32_t packet_no) {
  XYZModem.filepos = XYZFile.byte_counter;

  // Try to send packet, so header first. Use a buffer to reduce TCP/IP header overhead
  uint8_t xmodem_buffer[2 * (3 + XYZMODEM_SOH_PACKET_SIZE + 2)];  // Need double packet size to fix Telnet escape

  // Sending a packet will be retried
  uint32_t retries = 0;
  int in_char;
  do {
    // Seek to start of current data block,
    // will advance through the file as block will be acked..
    XYZFile.byte_counter = XYZModem.filepos;

    // Reset checksum stuff
    XYZModem.checksumBuf = 0x00;
    XYZModem.crcBuf = 0x00;

    uint8_t packet_num = packet_no;

//    AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Packet %d, retries %d, counter %d"), packet_no, retries, XYZFile.byte_counter);

    uint32_t xmodem_buffer_ptr = 0;
    XYZModemSendEscaped(xmodem_buffer, &xmodem_buffer_ptr, XYZM_SOH);
    XYZModemSendEscaped(xmodem_buffer, &xmodem_buffer_ptr, packet_num);
    XYZModemSendEscaped(xmodem_buffer, &xmodem_buffer_ptr, ~packet_num);
    for (uint32_t i = 0; i < XYZMODEM_SOH_PACKET_SIZE; i++) {
      in_char = XYZModemFileRead();
      if (in_char < 0) {
        return false;                                // No input
      }
      XYZModemChecksum(in_char);
      XYZModemSendEscaped(xmodem_buffer, &xmodem_buffer_ptr, in_char);
    }
    // Send out checksum, either CRC-16 CCITT or classical inverse of sum of bytes.
    // Depending on how the receiver introduced itself
    if (XYZModem.oldChecksum) {
      XYZModemSendEscaped(xmodem_buffer, &xmodem_buffer_ptr, XYZModem.checksumBuf);
    } else {
      XYZModemSendEscaped(xmodem_buffer, &xmodem_buffer_ptr, XYZModem.crcBuf >> 8);
      XYZModemSendEscaped(xmodem_buffer, &xmodem_buffer_ptr, XYZModem.crcBuf & 0xFF);
    }
    XYZModemWriteBuf(xmodem_buffer, xmodem_buffer_ptr);

    in_char = XYZModemWaitACK();

//    AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Ack %02X, Send %*_H"), in_char, xmodem_buffer_ptr, xmodem_buffer);
//    if (in_char != XYZM_ACK) {
//      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("XMD: Send packet %d, Size %d, Ack %d"), packet_no, xmodem_buffer_ptr, in_char);
//    }

    if (XYZM_CAN == in_char) { 
      return false;                                  // CAN
    }
    retries++;
    XYZModem.teraterm = 1;                           // Fix teraterm TELNET issue
    if (retries > XYZMODEM_MAX_RETRY) { 
      return false;                                  // Max retries
    }
  } while (in_char != XYZM_ACK);
  return true;
}

/*********************************************************************************************\
 * Xmodem Receive
\*********************************************************************************************/

void XYZModemCancel(void) {
	// Five cancels & five backspaces per spec
	const uint8_t cancel[] = { XYZM_CAN, XYZM_CAN, XYZM_CAN, XYZM_CAN, XYZM_CAN, XYZM_BS, XYZM_BS, XYZM_BS, XYZM_BS, XYZM_BS };
  XYZModemWriteBuf(cancel, sizeof(cancel));
}

void XYZModemSendNakOrC(void) {
  while (XYZModemAvailable()) { XYZModemRead(); }    // Flush input

  char out_char = 'C';
  if (XYZModem.oldChecksum) {
    out_char = XYZM_NAK;
  }
  XYZModemWrite(out_char);
}

bool XYZModemSendNak(void) {
  // When the receiver wishes to <nak>, it should call a "PURGE" subroutine, to wait
  // for the line to clear.  Recall the sender tosses any characters in its buffer
  // immediately upon completing sending a block, to ensure no glitches were mis-interpreted.
  // The most common technique is for "PURGE" to call the character receive subroutine,
  // specifying a 1-second timeout,[1] and looping back to PURGE until a timeout occurs.
  // The <nak> is then sent, ensuring the other end will see it.
  uint32_t timeout = 1 * XYZMODEM_RECV_TIMEOUT_SHORT;
  while (timeout--) {
    while (XYZModemAvailable()) { XYZModemRead(); }  // Flush input
    delay(1);
  }

  if (XYZModem.nak_count) {
    XYZModem.nak_count--;
    if (0 == XYZModem.nak_count) {
      XYZModemCancel();                              // Cancel xfer
      return false;
    }
  }

  XYZModemWrite(XYZM_NAK);

  XYZModem.mode = XYZD_SOH;
  return true;
}

bool XYZModemCheckPacket(uint8_t *buffer) {
  XYZModem.checksumBuf = 0x00;
  XYZModem.crcBuf = 0x00;

  for (uint32_t i = 0; i < XYZModem.packet_size; i++) {
    XYZModemChecksum(buffer[i + 3]);
  }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: CheckPacket %d, checksum %d,%d, crc %04X,%04X"),
//    XYZModem.oldChecksum, buffer[3 + XYZModem.packet_size], XYZModem.checksumBuf,
//    buffer[3 + XYZModem.packet_size] << 8 | buffer[3 + XYZModem.packet_size +1], XYZModem.crcBuf);

  if (XYZModem.oldChecksum) {
    return (buffer[3 + XYZModem.packet_size] == XYZModem.checksumBuf);
  } else {
    return ((buffer[3 + XYZModem.packet_size] == (XYZModem.crcBuf >> 8)) &&
            (buffer[3 + XYZModem.packet_size +1] == (XYZModem.crcBuf & 0xFF)));
  }  
}

int XYZModemReceive(uint32_t packet_no) {
  // Try to send packet, so header first. Use a buffer to reduce TCP/IP header overhead
  uint8_t xmodem_buffer[3 + XYZMODEM_STX_PACKET_SIZE + 2];
  uint32_t xmodem_buffer_ptr = 0;
  uint32_t packet_size;

  XYZModem.mode = XYZD_SOH;

  // The character-receive subroutine should be called with a parameter specifying the
  // number of seconds to wait.  The receiver should first call it with a time of 10,
  // then <nak> and try again, 10 times.
  XYZModem.receive_timeout = XYZMODEM_RECV_TIMEOUT_LONG;

  int in_char;
  bool packet_ready = false;
  do {
    in_char = XYZModemReadByte();
    if (in_char < 0) { 
      return XYZS_TIMEOUT;                           // No receive after timeout
    } 
    switch (XYZModem.mode) {
      case XYZD_SOH: {
//        AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: XYZD_SOH"));

        switch (in_char) {
          case XYZM_SOH: {                           // Start XModem 128 byte data transfer
//            AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: SOH"));

            xmodem_buffer[0] = in_char;
            // After receiving the <soh>, the receiver should call the character
            // receive subroutine with a 1-second timeout, for the remainder of the
            // message and the <cksum>.  Since they are sent as a continuous stream,
            // timing out of this implies a serious like glitch that caused, say,
            // 127 characters to be seen instead of 128.
            XYZModem.receive_timeout = XYZMODEM_RECV_TIMEOUT_SHORT;
            XYZModem.packet_size = XYZMODEM_SOH_PACKET_SIZE;
            XYZModem.mode = XYZD_BLK1;
            break;
          }
          case XYZM_STX: {                           // Start XModem 1024 byte data transfer
//            AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: STX"));

            xmodem_buffer[0] = in_char;
            // After receiving the <stx>, the receiver should call the character
            // receive subroutine with a 1-second timeout, for the remainder of the
            // message and the <cksum>.  Since they are sent as a continuous stream,
            // timing out of this implies a serious like glitch that caused, say,
            // 1023 characters to be seen instead of 1024.
            XYZModem.receive_timeout = XYZMODEM_RECV_TIMEOUT_SHORT;
            XYZModem.packet_size = XYZMODEM_STX_PACKET_SIZE;
            XYZModem.mode = XYZD_BLK1;
            break;
          }
          case XYZM_EOT: {
//            AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: EOT"));

            if (packet_no) {
              XYZModemFileWrite(nullptr);
            } else {
              // Handle YModem filename package
            }
            XYZModemWrite(XYZM_ACK);
            return XYZS_EOT;                         // Success as EOT received
            break;
          }
          case XYZM_CAN: {
//            AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: CAN"));

            XYZModem.can_count++;
            if (5 == XYZModem.can_count) {
              return XYZS_CAN;                       // Cancelled by sender
            }
            break;
          }
          default: {
//            AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Default"));

            XYZModemCancel();
            return XYZS_OTHER;                       // Unknown error
          }
        }
        break;
      }
      case XYZD_BLK1: {
//        AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: XYZD_BLK1"));
/*
        // Needed with SecureCRT unless Session Option - Telnet - Server requires bare CR is set
        if (TXMP_TELNET == XYZModem.protocol) {
          if (0x0D == in_char) {
            int in_char2 = XYZModemRead();
#ifdef XYZM_DEBUG
            Serial.printf("[%02X]",in_char2);
#endif
          }
        }
*/
        xmodem_buffer[1] = in_char;
        XYZModem.mode = XYZD_BLK2;
        break;
      }
      case XYZD_BLK2: {
//        AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: XYZD_BLK2 %02X, exor %02X"), in_char, (in_char ^ xmodem_buffer[1]));
/*
        // Needed with SecureCRT unless Session Option - Telnet - Server requires bare CR is set
        if (TXMP_TELNET == XYZModem.protocol) {
          if (0x0D == in_char) {
            int in_char2 = XYZModemRead();
#ifdef XYZM_DEBUG
            Serial.printf("[%02X]",in_char2);
#endif
          }
        }
*/
        xmodem_buffer[2] = in_char;
        if (0xFF == (in_char ^ xmodem_buffer[1])) {
          xmodem_buffer_ptr = 3;
          packet_size = 3 + XYZModem.packet_size + ((XYZModem.oldChecksum) ? 1 : 2);
          XYZModem.mode = XYZD_DATA;
        } else {
          if (!XYZModemSendNak()) {
            return XYZS_PACKET;
          }
        }
        break;
      }
      case XYZD_DATA: {                              // Read data and checksum/crc
//        AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: XYZD_DATA %d of %d"), xmodem_buffer_ptr, packet_size);

        xmodem_buffer[xmodem_buffer_ptr++] = in_char;
        if (xmodem_buffer_ptr >= packet_size) {
          XYZFile.byte_counter = packet_no * XYZModem.packet_size;
          XYZModem.mode = XYZD_SOH;
          packet_ready = true;
        } else {
        
        }
        break;
      }
    }
  } while (!packet_ready);
//  AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Packet ready"));

  if (!XYZModemCheckPacket(xmodem_buffer)) {
//    AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Checksum failure"));

    XYZFile.byte_counter -= XYZModem.packet_size;    // Restore for retry
    XYZModemSendNak();
    return XYZS_CHECKSUM;                            // Checksum failure
  }

  uint8_t packet = xmodem_buffer[1] - packet_no;

//  AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Packet %d,%d,%d"), packet, xmodem_buffer[1], packet_no);

  if (packet > 1) {
//    AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Bad packet number"));

    XYZModemCancel();
    return XYZS_PACKET;                              // Bad packet number
  }

  XYZModemWrite(XYZM_ACK);
  XYZModem.nak_count = 10;

//  if (0 == packet) {
//    return true;
//  }

  if (!XYZModemFileWrite(xmodem_buffer +3)) {
    return XYZS_FILE;                                // Unable to claim file buffer
  }

  return XYZS_OK;
}

/********************************************************************************************/

void XModemSendStart(void) {
  // *** Handle file send using XModem - sync
  // Wait for either C or NACK as a sync packet. Determines protocol details, checksum algorithm.
  XYZModem.enabled = XYZM_SEND;
  XYZModem.timeout = millis() + (XYZMODEM_SYNC_TIMEOUT * 1000);
  XYZModem.packet_no = 1;
  XYZFile.byte_counter = 0;
  XYZFile.sector_counter = -1;
  if (XYZFile.size) { 
    XYZFile.step = XYZM_SEND;
    AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Send started"));
  } else {
    XYZModemInit();
  }
}

void XModemReceiveStart(void) {
  // *** Handle receive file using XModem - sync
  XYZModem.enabled = XYZM_RECEIVE;
  XYZModem.timeout = millis() + 1000;
  XYZModem.nak_count = 30;                           // Allow 30 * 2 seconds for Xmodem Send to start
  XYZModem.can_count = 0;
  XYZModem.packet_no = 1;
  XYZModem.retry = 4;
  XYZFile.byte_counter = 0;
  XYZFile.sector_counter = 0;
  XYZFile.step = XYZM_RECEIVE;
}

bool XYZModemLoop(void) {
  switch (XYZFile.step) {
    case XYZM_IDLE: {                                // *** Send/Receive disabled
      return false;
    }
    // *** Send
    case XYZM_SEND: {                                // *** Handle file send using XModem - upload
      if (XYZModemFileAvailable()) {
        if (XYZFile.byte_counter && !(XYZFile.byte_counter % 10240)) {  // Show progress every 10KB
          AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Progress %d KB"), XYZFile.byte_counter / 1024);
        }
        if (!XYZModemSend(XYZModem.packet_no)) {
          AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Packet %d send failed"), XYZModem.packet_no);
          XYZFile.step = XYZM_ERROR;
          return true;
        }
        XYZModem.packet_no++;
      } else {
        // Once the last block is ACKed by the target, the transfer should be finalized by an
        // EOT (ASCII 0x04) packet from the source. This packet is confirmed via XModem ACK
        // from the target.
        XYZModemWrite(XYZM_EOT);                     // *** Send EOT
        XYZModem.timeout = millis() + (30 * 1000);   // Allow 30 seconds to receive EOT ACK
        XYZFile.step = XYZM_SND_ACK;
      }
      break;
    }
    case XYZM_SND_ACK: {                             // *** Wait for ACK
      // The ACK for the last XModem data packet may take much longer (1-3 seconds) than prior
      // data packets to be received.
      if (millis() > XYZModem.timeout) {
        AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: EOT ACK timeout"));
        XYZFile.step = XYZM_ERROR;
        return true;
      }
      if (XYZModemAvailable()) {
        int xmodem_ack = XYZModemWaitACK();
        if (XYZM_CAN == xmodem_ack) {
          AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Transfer invalid"));
          XYZFile.step = XYZM_ERROR;
          return true;
        }
        else if (XYZM_ACK == xmodem_ack) {
          // !!! Send an AddLog here as it previously would interfere with XModem protocol !!!
          AddLog(LOG_LEVEL_INFO, PSTR("XMD: Send %d bytes succesful"), XYZFile.size);
          XYZFile.step = XYZM_DONE;
        }
      }
      break;
    }
    // *** Receive
    case XYZM_RECEIVE: {                             // *** Handle receive file using XModem - download
      if (millis() > XYZModem.timeout) {
        XYZModem.timeout = millis() + (2 * 1000);    // Protocol 10 second receive timeout - here 2 seconds
        XYZModemSendNakOrC();
        XYZModem.nak_count--;
        if (XYZModemReadAvailable(1)) {              // Timeout after 1 second
          XYZModem.nak_count = (XYZModem.oldChecksum) ? 10 : 3;
          AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Receive started"));
          XYZFile.step = XYZM_RCV_START;
          return true;
        }
        else if (0 == XYZModem.nak_count) {          // Timeout after 30 * 2 seconds
          AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Receive timeout"));
          XYZFile.step = XYZM_ERROR;
          return true;
        }
      }
      break;
    }
    case XYZM_RCV_START: {
      if (XYZFile.byte_counter && !(XYZFile.byte_counter % 10240)) {  // Show progress every 10KB
        AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Progress %d KB"), XYZFile.byte_counter / 1024);
      }
      int result = XYZModemReceive(XYZModem.packet_no);
      if (result) {
#ifdef XYZM_DEBUG
        Serial.println();
#endif
        switch (result) {
          case XYZS_EOT: {
            XYZModemFileWriteEot(1);
            AddLog(LOG_LEVEL_INFO, PSTR("XMD: Received %d bytes succesful"), XYZFile.byte_counter);
            XYZFile.step = XYZM_DONE;
            break;
          }
          case XYZS_CHECKSUM: {
            // Checksum error or teraterm special ops on 0x0D disabled - will retry
            AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Checksum failure - retry"));
            XYZModem.teraterm = 1;                   // Fix teraterm TELNET issue
            XYZModem.retry--;
            break;
          }
          case XYZS_TIMEOUT: {
            // Receive character timeout - will retry
            AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Timeout - retry"));
            XYZModem.retry--;
            XYZFile.step = XYZM_RECEIVE;
            break;
          }
          case XYZS_OTHER: {
            // Received unknown command
            AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Unknown error"));
            XYZModem.retry = 0;
            break;
          }
          case XYZS_CAN: {
            // Receive cancelled by user
            AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Cancelled"));
            XYZModem.retry = 0;
            break;
          }
          case XYZS_PACKET: {
            // Packet number mismatch
            AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Packet number error"));
            XYZModem.retry = 0;
            break;
          }
          case XYZS_FILE: {
            // Unable to allocate memory, file write error or filesystem full
            AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: File write error"));
            XYZModem.retry = 0;
            break;
          }
          default:
            AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Receive error %d"), result);
            XYZModem.retry = 0;
        }

        if (0 == XYZModem.retry) {
          AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Packet %d receive failed"), XYZModem.packet_no);
          XYZModemFileWriteEot(0);
          XYZModemCancel();
          XYZFile.step = XYZM_ERROR;
          return true;
        }
      } else {
#ifdef XYZM_DEBUG
        Serial.println();
#endif
        XYZModem.packet_no++;
      }
      break;
    }
    // *** Finish
    case XYZM_ERROR:
      XYZFile.state = XYZM_ERROR;
      AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Failed"));
    case XYZM_DONE: {                        // *** Clean up
      XYZModemBufferFree();
      XYZModemInit();
      break;
    }
  }
  return true;
}

void XYZModemInit(void) {
  XYZModem.receive_timeout = XYZMODEM_RECV_TIMEOUT_SHORT;
  XYZModem.enabled = XYZM_IDLE;
  XYZModem.protocol = TXMP_NONE;
  XYZFile.size = 0;
  XYZFile.step = XYZM_IDLE;
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kXYZmodemCommands[] PROGMEM = "|"   // No prefix
  "TeraTerm|XSend|XReceive";

void (* const XYZmodemCommands[])(void) PROGMEM = {
  &CmndTeraTerm, &CmndXSend, &CmndXReceive };

void CmndTeraTerm(void) {
  // TeraTerm    - Show current selection
  // TeraTerm 0  - Disable adding zero after carriage return (0x0D)
  // TeraTerm 1  - Enable adding zero after carriage return (0x0D)
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    XYZModem.teraterm = XdrvMailbox.payload;
  }
  ResponseCmndStateText(XYZModem.teraterm);
}

void CmndXSend(void) {
  // XSend <filename>
  // XSend Settings     - Send active settings
  // XSend autoexec.bat - Send file autoexec.bat
  if (XdrvMailbox.data_len > 0) {
    if (!strcasecmp_P(XdrvMailbox.data, PSTR("Settings"))) {
      XYZFile.size = sizeof(TSettings);
      XYZFile.file = false;
      ResponseCmndChar("Ready to start receive Settings");
#ifdef USE_UFILESYS
    } else {
      UfsFilename(XYZFile.file_name, XdrvMailbox.data);
      if (TfsFileExists(XYZFile.file_name)) {
        XYZFile.size = TfsFileSize(XYZFile.file_name);
        if (XYZFile.size) {
          XYZFile.file = true;
          ResponseCmndChar("Ready to start receive file");
        } else {
          ResponseCmndChar("File is empty");
        }
      } else {
        ResponseCmndChar("File not found");
      }
#endif  // USE_UFILESYS
    }
  }
}

void CmndXReceive(void) {
  // XReceive[<checksum>] <filename>[,<trim1a>]  - Receive filename with optional crc (default CRC) and trim CP/M SUB from end of file (default trim)
  // XReceive Settings                           - Receive and activate settings using CRC (No need for trim1a as it's size is on 128/1024 bytes boundary)
  // XReceive autoexec.bat                       - Receive file autoexec.bat using CRC leaving CP/M SUB at end of file
  // XReceive autoexec.bat,0                     - Receive file autoexec.bat using CRC and do not trim CP/M SUB from end of file
  // XReceive autoexec.bat,1                     - Receive file autoexec.bat using CRC and trim CP/M SUB from end of file (default)
  // XReceive autoexec.bat,2                     - Receive file autoexec.bat using CRC and auto trim CP/M SUB from end of file
  // XReceive2 autoexec.bat                      - Receive file autoexec.bat using checksum (less secure)
  if (XdrvMailbox.data_len > 0) {
    XYZModem.oldChecksum = 0;                        // CRC (Start with C) - Default
    if (2 == XdrvMailbox.index) {
      XYZModem.oldChecksum = 1;                      // Checksum (Start with NAK)
    }
    XYZFile.trim1a = XYZT_TRIM;                      // Trim CP/M SUB from end of file - Default
    size_t option = strchrspn(XdrvMailbox.data, ',');
    if (option) {
      char option_crc[XdrvMailbox.data_len];
      ArgV(option_crc, 2);
      XYZFile.trim1a = atoi(option_crc) &0x03;
      XdrvMailbox.data[option] = '\0';               // Force input end of string
    }
    if (!strcasecmp_P(XdrvMailbox.data, PSTR("Settings"))) {
      XYZFile.file = false;
      XModemReceiveStart();
      ResponseCmndChar("Ready to start sending Settings");
#ifdef USE_UFILESYS
    } else {
      UfsFilename(XYZFile.file_name, XdrvMailbox.data);
      XYZFile.file = true;
      XModemReceiveStart();
      ResponseCmndChar("Ready to start sending file");
#endif  // USE_UFILESYS
    }
  }
}

/*********************************************************************************************\
 * External access
\*********************************************************************************************/

bool XYZModemStart(uint32_t protocol, uint8_t in_byte) {
  XYZModem.protocol = protocol;

//  AddLog(LOG_LEVEL_DEBUG, PSTR("XMD: Protocol %d, Received %02X(%d)"), protocol, in_byte, in_byte);

  if (XYZFile.size) {
    if ((in_byte == XYZM_NAK) ||                     // Xmodem NAK - Checksum
        (in_byte == 'C')) {                          // Xmodem NAK - CRC
      // Determine which checksum algorithm to use
      XYZModem.oldChecksum = (in_byte == XYZM_NAK);
      XModemSendStart();
      return true;
    }
  }
  return false;
}

bool XYZModemWifiClientStart(WiFiClient *client, uint8_t in_byte) {
  XYZModem.client = client;
  return XYZModemStart(TXMP_TELNET, in_byte);
}

bool XYZModemActive(uint32_t protocol) {
  return (XYZModem.enabled && (XYZModem.protocol == protocol));
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv120(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_SLEEP_LOOP:
    case FUNC_LOOP:
      if (XYZModem.enabled) {
        XYZModemLoop();
      }
      break;
    case FUNC_INIT:
      XYZModemInit();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kXYZmodemCommands, XYZmodemCommands);
      break;
    case FUNC_ACTIVE:
      result = true;
      break;
  }
  return result;
}

#endif  // USE_XYZMODEM
