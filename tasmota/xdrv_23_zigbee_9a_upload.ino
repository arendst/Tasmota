/*
  xdrv_23_zigbee_9a_upload.ino - zigbee: serial xmodem upload to MCU

  Copyright (C) 2021  Theo Arends and Stephan Hadinger

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

#ifdef USE_ZIGBEE

#ifdef USE_ZIGBEE_EZSP
/*********************************************************************************************\
 * MCU EFR32 firmware upload using xmodem
 *
 * Step 1 - Upload MCU firmware in ESP8266 flash free space (current size is about 200k)
 * Step 2 - Upload MCU firmware from ESP8266 flash to MCU EFR32 using XMODEM protocol
 * Step 3 - Restart
\*********************************************************************************************/

//#define ZIGBEE_BOOTLOADER_SOFTWARE_RESET_FIRST

#define XM_SOH     0x01
#define XM_EOT     0x04
#define XM_ACK     0x06
#define XM_CR      0x0d
#define XM_NAK     0x15
#define XM_CAN     0x18
#define XM_SUB     0x1a

enum ZbUploadSteps { ZBU_IDLE, ZBU_INIT,
                     ZBU_SOFTWARE_RESET, ZBU_SOFTWARE_SEND, ZBU_HARDWARE_RESET, ZBU_PROMPT,
                     ZBU_SYNC, ZBU_UPLOAD, ZBU_EOT, ZBU_COMPLETE, ZBU_DONE, ZBU_ERROR, ZBU_FINISH };

const uint8_t PIN_ZIGBEE_BOOTLOADER = 5;

struct ZBUPLOAD {
  uint32_t ota_size = 0;
  uint32_t sector_counter = 0;
  uint32_t byte_counter = 0;
  char *buffer;
  uint8_t ota_step = ZBU_IDLE;
  uint8_t bootloader = 0;
  uint8_t state = ZBU_IDLE;
} ZbUpload;

/*********************************************************************************************\
 * Flash from ESP8266 to EZSP
\*********************************************************************************************/

uint32_t ZigbeeUploadAvailable(void) {
  int available = ZbUpload.ota_size - ZbUpload.byte_counter;
  if (available < 0) { available = 0; }
  return available;
}

char ZigbeeUploadFlashRead(void) {
  if (0 == ZbUpload.byte_counter) {
    if (!(ZbUpload.buffer = (char *)malloc(SPI_FLASH_SEC_SIZE))) {
      return (-1);  // Not enough (memory) space
    }
  }

  uint32_t index = ZbUpload.byte_counter % SPI_FLASH_SEC_SIZE;
  if (0 == index) {
    ESP.flashRead(ZbUpload.sector_counter * SPI_FLASH_SEC_SIZE, (uint32_t*)ZbUpload.buffer, SPI_FLASH_SEC_SIZE);
    ZbUpload.sector_counter++;
  }

  char data = ZbUpload.buffer[index];
  ZbUpload.byte_counter++;

  if (ZbUpload.byte_counter > ZbUpload.ota_size) {
    // When the source device reaches the last XModem data block, it should be padded to 128 bytes
    // of data using SUB (ASCII 0x1A) characters.
    data = XM_SUB;
//    if (ZbUpload.buffer) { free(ZbUpload.buffer); }  // Don't in case of retries
  }
  return data;
}

/*********************************************************************************************\
 * XModem protocol
\*********************************************************************************************/

// Number of milliseconds to wait before prompt is received
const uint32_t XMODEM_FLUSH_DELAY = 1000;
// Number of seconds until giving up hope of receiving sync packets from host.
const uint8_t XMODEM_SYNC_TIMEOUT = 30;
// Number of times we try to send a packet to the host until we give up sending..
const uint8_t XMODEM_MAX_RETRY = 30;
// Packet size
const uint8_t XMODEM_PACKET_SIZE = 128;

struct XMODEM {
  uint32_t timeout = 0;
  uint32_t delay = 0;
  uint32_t flush_delay = 0xFFFFFFFF;
  uint32_t filepos = 0;
  int crcBuf = 0;
  uint8_t packetNo = 1;
  uint8_t checksumBuf = 0;
  bool oldChecksum;
} XModem;

// Send out a byte of payload data, includes checksumming
void XModemOutputByte(uint8_t out_char) {
  XModem.checksumBuf += out_char;

  XModem.crcBuf = XModem.crcBuf ^ (int) out_char << 8;
  for (uint32_t i = 0; i < 8; i++) {
    if (XModem.crcBuf & 0x8000) {
      XModem.crcBuf = XModem.crcBuf << 1 ^ 0x1021;
    } else {
      XModem.crcBuf = XModem.crcBuf << 1;
    }
  }

  ZigbeeSerial->write(out_char);
}

// Wait for the remote to acknowledge or cancel.
// Returns the received char if no timeout occured or a CAN was received. In this cases, it returns -1.
char XModemWaitACK(void)
{
  char in_char;
  do {
    uint8_t i = 0;
    while (!ZigbeeSerial->available()) {
      delayMicroseconds(100);
      i++;
      if (i > 200) { return -1; }
    }
    in_char = ZigbeeSerial->read();
    if (XM_CAN == in_char) { return XM_CAN; }
  } while ((in_char != XM_NAK) && (in_char != XM_ACK) && (in_char != 'C'));
  return in_char;
}

bool XModemSendPacket(uint32_t packet_no) {
  XModem.filepos = ZbUpload.byte_counter;

  // Sending a packet will be retried
  uint32_t retries = 0;
  char in_char;
  do {
    // Seek to start of current data block,
    // will advance through the file as block will be acked..
    ZbUpload.byte_counter = XModem.filepos;

    // Reset checksum stuff
    XModem.checksumBuf = 0x00;
    XModem.crcBuf = 0x00;

    // Try to send packet, so header first
    ZigbeeSerial->write(XM_SOH);
    ZigbeeSerial->write(packet_no);
    ZigbeeSerial->write(~packet_no);
    for (uint32_t i = 0; i < XMODEM_PACKET_SIZE; i++) {
      in_char = ZigbeeUploadFlashRead();
      XModemOutputByte(in_char);
    }
    // Send out checksum, either CRC-16 CCITT or classical inverse of sum of bytes.
    // Depending on how the received introduced himself
    if (XModem.oldChecksum) {
      ZigbeeSerial->write((char)XModem.checksumBuf);
    } else {
      ZigbeeSerial->write((char)(XModem.crcBuf >> 8));
      ZigbeeSerial->write((char)(XModem.crcBuf & 0xFF));
    }
    in_char = XModemWaitACK();
    if (XM_CAN == in_char) { return false; }
    retries++;
    if (retries > XMODEM_MAX_RETRY) { return false; }
  } while (in_char != XM_ACK);
  return true;
}

/*********************************************************************************************\
 * Step 2 - Upload MCU firmware from ESP8266 flash to MCU EFR32 using XMODEM protocol
 *
 * https://www.silabs.com/documents/public/application-notes/an760-using-legacy-standalone-bootloader.pdf
\*********************************************************************************************/

void ZigbeeUploadSetSoftwareBootloader() {
  // https://github.com/arendst/Tasmota/issues/8583#issuecomment-663967883
  SBuffer buf(4);
  buf.add16(EZSP_launchStandaloneBootloader);
  buf.add8(0x01);
  ZigbeeEZSPSendCmd(buf.getBuffer(), buf.len());  // Send software bootloader init
}

void ZigbeeUploadSetBootloader(uint8_t state) {
  pinMode(PIN_ZIGBEE_BOOTLOADER, OUTPUT);
  digitalWrite(PIN_ZIGBEE_BOOTLOADER, state);  // Toggle Gecko bootloader
  digitalWrite(Pin(GPIO_ZIGBEE_RST), 0);
  delay(100);                                  // Need to experiment to find a value as low as possible
  digitalWrite(Pin(GPIO_ZIGBEE_RST), 1);       // Reboot MCU EFR32
}

bool ZigbeeUploadBootloaderPrompt(void) {
  // Scripts that interact with the bootloader should use only the “BL >” prompt to determine
  // when the bootloader is ready for input. While current menu options should remain functionally
  // unchanged, the menu title and options text is liable to change, and new options might be added.

  uint8_t serial_buffer[255];
  uint32_t buf_len = 0;

  while (ZigbeeSerial->available()) {
    yield();
    char bootloader_byte = ZigbeeSerial->read();

    if (((uint8_t)bootloader_byte >=0) && (buf_len < sizeof(serial_buffer) -2)) {
      serial_buffer[buf_len++] = bootloader_byte;
    }

    if (ZbUpload.byte_counter != 4) {
      switch (ZbUpload.byte_counter) {
        case 0:
          if ('B' == bootloader_byte) { ZbUpload.byte_counter++; } break;
        case 1:
          if ('L' == bootloader_byte) { ZbUpload.byte_counter++; } break;
        case 2:
          if (' ' == bootloader_byte) { ZbUpload.byte_counter++; } break;
        case 3:
          if ('>' == bootloader_byte) {
            ZbUpload.byte_counter++;
            XModem.flush_delay = millis() + XMODEM_FLUSH_DELAY;
            XModem.delay = XModem.flush_delay + XMODEM_FLUSH_DELAY;
          }
      }
    }
  }

  if (buf_len) {
    char hex_char[256];
    ToHex_P(serial_buffer, buf_len, hex_char, 256);
    AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("XMD: Rcvd %s"), hex_char);
  }

  return ((4 == ZbUpload.byte_counter) && (millis() > XModem.flush_delay));
}

bool ZigbeeUploadXmodem(void) {
  switch (ZbUpload.ota_step) {
    case ZBU_IDLE: {                     // *** Upload disabled
      return false;
    }
#ifdef ZIGBEE_BOOTLOADER_SOFTWARE_RESET_FIRST
    case ZBU_INIT: {                     // *** Init ESF32 bootloader
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("XMD: Init bootloader"));
      ZbUpload.ota_step = ZBU_SOFTWARE_RESET;
      return false;  // Keep Zigbee serial active
    }
    case ZBU_SOFTWARE_RESET: {
      SBuffer buf(4);
      buf.add16(EZSP_launchStandaloneBootloader);
      buf.add8(0x01);
      ZigbeeEZSPSendCmd(buf.getBuffer(), buf.len());  // Send software bootloader init
      XModem.timeout = millis() + (10 * 1000);  // Allow 10 seconds to send Zigbee command
      ZbUpload.ota_step = ZBU_SOFTWARE_SEND;
      return false;  // Keep Zigbee serial active
    }
    case ZBU_SOFTWARE_SEND: {
      if (millis() > XModem.timeout) {
        AddLog_P(LOG_LEVEL_DEBUG, PSTR("XMD: Bootloader software reset send timeout"));
        ZbUpload.ota_step = ZBU_HARDWARE_RESET;
        return true;
      }
      if (EZSP_Serial.to_send == EZSP_Serial.to_end) {
        ZbUpload.bootloader = ZBU_SOFTWARE_RESET;
        XModem.timeout = millis() + (10 * 1000);  // Allow 10 seconds to receive EBL prompt
        XModem.delay = millis() + (2 * XMODEM_FLUSH_DELAY);
        ZbUpload.byte_counter = 0;
        ZbUpload.ota_step = ZBU_PROMPT;
      }
      break;
    }
    case ZBU_HARDWARE_RESET: {
      ZbUpload.bootloader = ZBU_HARDWARE_RESET;
      ZigbeeUploadSetBootloader(0);      // Reboot MCU EFR32 which returns below text
      XModem.timeout = millis() + (30 * 1000);  // Allow 30 seconds to receive EBL prompt
      XModem.delay = millis() + (2 * XMODEM_FLUSH_DELAY);
      ZbUpload.byte_counter = 0;
      ZbUpload.ota_step = ZBU_PROMPT;
      break;
    }
    case ZBU_PROMPT: {                   // *** Wait for prompt and select option upload ebl
      if (millis() > XModem.timeout) {
        if (ZBU_SOFTWARE_RESET == ZbUpload.bootloader) {
          AddLog_P(LOG_LEVEL_DEBUG, PSTR("XMD: Bootloader software reset timeout"));
          ZbUpload.ota_step = ZBU_HARDWARE_RESET;
        } else {
          AddLog_P(LOG_LEVEL_DEBUG, PSTR("XMD: Bootloader hardware reset timeout"));
          ZbUpload.ota_step = ZBU_ERROR;
        }
        return true;
      }
#else  // No ZIGBEE_BOOTLOADER_SOFTWARE_RESET_FIRST
    case ZBU_INIT: {                     // *** Init ESF32 bootloader
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("XMD: Init bootloader"));
      ZigbeeUploadSetBootloader(0);      // Reboot MCU EFR32 which returns below text
      XModem.timeout = millis() + (30 * 1000);  // Allow 30 seconds to receive EBL prompt
      XModem.delay = millis() + (2 * XMODEM_FLUSH_DELAY);
      ZbUpload.byte_counter = 0;
      ZbUpload.ota_step = ZBU_PROMPT;
      break;
    }
    case ZBU_PROMPT: {                   // *** Wait for prompt and select option upload ebl
      if (millis() > XModem.timeout) {
        AddLog_P(LOG_LEVEL_DEBUG, PSTR("XMD: Bootloader timeout"));
        ZbUpload.ota_step = ZBU_ERROR;
        return true;
      }
#endif  // ZIGBEE_BOOTLOADER_SOFTWARE_RESET_FIRST
      else if (!ZigbeeSerial->available() && (millis() < XModem.flush_delay)) {
        // The target device’s bootloader sends output over its serial port after it receives a
        // carriage return from the source device
        if (millis() > XModem.delay) {
          ZigbeeSerial->write(XM_CR);
          XModem.delay = millis() + (2 * XMODEM_FLUSH_DELAY);
        }
      } else {
        // After the bootloader receives a carriage return from the target device, it displays a menu
        // Gecko Bootloader v1.A.3
        // 1. upload gbl
        // 2. run
        // 3. ebl info
        // BL >
        if (ZigbeeUploadBootloaderPrompt()) {
          AddLog_P(LOG_LEVEL_DEBUG, PSTR("XMD: Init sync"));
          ZigbeeSerial->flush();
          ZigbeeSerial->write('1');      // upload ebl
          if (TasmotaGlobal.sleep > 0) {
            TasmotaGlobal.sleep = 1;                  // Speed up loop used for xmodem upload
          }
          XModem.timeout = millis() + (XMODEM_SYNC_TIMEOUT * 1000);
          ZbUpload.ota_step = ZBU_SYNC;
        }
      }
      break;
    }
    case ZBU_SYNC: {                     // *** Handle file upload using XModem - sync
      if (millis() > XModem.timeout) {
        AddLog_P(LOG_LEVEL_DEBUG, PSTR("XMD: SYNC timeout"));
        ZbUpload.ota_step = ZBU_ERROR;
        return true;
      }
      // Wait for either C or NACK as a sync packet. Determines protocol details, checksum algorithm.
      if (ZigbeeSerial->available()) {
        char xmodem_sync = ZigbeeSerial->read();
        if (('C' == xmodem_sync) || (XM_NAK == xmodem_sync)) {
          // Determine which checksum algorithm to use
          XModem.oldChecksum = (xmodem_sync == XM_NAK);
          XModem.packetNo = 1;
          ZbUpload.byte_counter = 0;
          ZbUpload.ota_step = ZBU_UPLOAD;
          AddLog_P(LOG_LEVEL_DEBUG, PSTR("XMD: Init packet send"));
        }
      }
      break;
    }
    case ZBU_UPLOAD: {                   // *** Handle file upload using XModem - upload
      if (ZigbeeUploadAvailable()) {
        if (!XModemSendPacket(XModem.packetNo)) {
          AddLog_P(LOG_LEVEL_DEBUG, PSTR("XMD: Packet send failed"));
          ZbUpload.ota_step = ZBU_ERROR;
          return true;
        }
        XModem.packetNo++;
      } else {
        // Once the last block is ACKed by the target, the transfer should be finalized by an
        // EOT (ASCII 0x04) packet from the source. Once this packet is confirmed via XModem ACK
        // from the target, the device will reboot, causing the new firmware to be launched.
        ZigbeeSerial->write(XM_EOT);
        XModem.timeout = millis() + (30 * 1000);  // Allow 30 seconds to receive EOT ACK
        ZbUpload.ota_step = ZBU_EOT;
      }
      break;
    }
    case ZBU_EOT: {                      // *** Send EOT and wait for ACK
      // The ACK for the last XModem data packet may take much longer (1-3 seconds) than prior
      // data packets to be received. This is due to the CRC32 checksum being performed across
      // the received EBL file data prior to sending the ACK. The source device must ensure that
      // its XModem state machine waits a sufficient amount of time to allow this checksum process
      // to occur without timing out on the response just before the EOT is sent.
      if (millis() > XModem.timeout) {
        AddLog_P(LOG_LEVEL_DEBUG, PSTR("XMD: EOT ACK timeout"));
        ZbUpload.ota_step = ZBU_ERROR;
        return true;
      }
      if (ZigbeeSerial->available()) {
        char xmodem_ack = XModemWaitACK();
        if (XM_ACK == xmodem_ack) {
          AddLog_P(LOG_LEVEL_DEBUG, PSTR("XMD: " D_SUCCESSFUL));
          XModem.timeout = millis() + (30 * 1000);  // Allow 30 seconds to receive EBL prompt
          ZbUpload.byte_counter = 0;
          ZbUpload.ota_step = ZBU_COMPLETE;
        }
      }
      break;
    }
    case ZBU_COMPLETE: {                 // *** Wait for Serial upload complete EBL prompt
      if (millis() > XModem.timeout) {
        AddLog_P(LOG_LEVEL_DEBUG, PSTR("XMD: Bootloader timeout"));
        ZbUpload.ota_step = ZBU_ERROR;
        return true;
      } else {
        // After an image successfully uploads, the XModem transaction completes and the bootloader displays
        // ‘Serial upload complete’ before redisplaying the menu
        // Serial upload complete
        // Gecko Bootloader v1.A.3
        // 1. upload gbl
        // 2. run
        // 3. ebl info
        // BL >
        if (ZigbeeUploadBootloaderPrompt()) {
          ZbUpload.state = ZBU_COMPLETE;
          ZbUpload.ota_step = ZBU_DONE;
        }
      }
      break;
    }
    case ZBU_ERROR:
      ZbUpload.state = ZBU_ERROR;
    case ZBU_DONE: {                     // *** Clean up and restart to disable bootloader and use new firmware
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("XMD: " D_RESTARTING));
      ZigbeeUploadSetBootloader(1);      // Disable bootloader and reset MCU - should happen at restart
      if (1 == TasmotaGlobal.sleep) {
        TasmotaGlobal.sleep = Settings.sleep;         // Restore loop sleep
      }
//      TasmotaGlobal.restart_flag = 2;    // Restart to disable bootloader and use new firmware
      ZbUpload.ota_step = ZBU_FINISH;    // Never return to zero without a restart to get a sane Zigbee environment
      break;
    }
    case ZBU_FINISH: {                   // *** Wait for restart making sure not to start Zigbee serial again
      // Wait for restart
      break;
    }
  }
  return true;
}

/*********************************************************************************************\
 * Step 1 - Upload MCU firmware in ESP8266 flash free space (current size is about 200k)
\*********************************************************************************************/

#ifdef USE_WEBSERVER

uint8_t ZigbeeUploadStep1Init(void) {
  if (!PinUsed(GPIO_ZIGBEE_RST) && (ZigbeeSerial == nullptr)) { return 1; }  // Wrong pin configuration - No file selected

  ZbUpload.ota_step = ZBU_IDLE;
  ZbUpload.state = ZBU_IDLE;
  return 0;
}

void ZigbeeUploadStep1Done(uint32_t data, size_t size) {
  ZbUpload.sector_counter = data;
  ZbUpload.ota_size = size;
  ZbUpload.ota_step = ZBU_INIT;
  ZbUpload.state = ZBU_UPLOAD;      // Signal upload done and ready for delayed upload to MCU EFR32
}

bool ZigbeeUploadFinish(void) {
  return (ZBU_FINISH == ZbUpload.ota_step);
}

#define WEB_HANDLE_ZIGBEE_XFER "zx"

const char HTTP_SCRIPT_XFER_STATE[] PROGMEM =
  "function z9(){"
    "if(x!=null){x.abort();}"       // Abort if no response within 2 seconds (happens on restart 1)
    "x=new XMLHttpRequest();"
    "x.onreadystatechange=function(){"
      "if(x.readyState==4&&x.status==200){"
        "var s=x.responseText;"
        "if(s!=7){"                 // ZBU_UPLOAD
          "location.href='/u3';"    // Load page HandleUploadDone()
        "}"
      "}"
    "};"
    "x.open('GET','" WEB_HANDLE_ZIGBEE_XFER "?z=1',true);"  // ?z related to Webserver->hasArg("z")
    "x.send();"
    "lt=setTimeout(z9,950);"        // Poll every 0.95 second
  "}"
  "wl(z9);";                        // Execute z9() on page load

void HandleZigbeeXfer(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  if (Webserver->hasArg("z")) {     // Status refresh requested
    WSContentBegin(200, CT_PLAIN);
    WSContentSend_P(PSTR("%d"), ZbUpload.state);
    WSContentEnd();
    if (ZBU_ERROR == ZbUpload.state) {
      Web.upload_error = 7;         // Upload aborted (xmodem transfer failed)
    }
    return;
  }

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_UPLOAD_TRANSFER));

  WSContentStart_P(PSTR(D_INFORMATION));
  WSContentSend_P(HTTP_SCRIPT_XFER_STATE);
  WSContentSendStyle();
  WSContentSend_P(PSTR("<div style='text-align:center;'><b>" D_UPLOAD_TRANSFER " ...</b></div>"));
  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();
}

#endif  // USE_WEBSERVER

#endif  // USE_ZIGBEE_EZSP

#endif  // USE_ZIGBEE
