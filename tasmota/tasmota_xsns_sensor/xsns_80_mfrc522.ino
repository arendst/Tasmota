/*
  xsns_80_mfrc522.ino - Support for MFRC522 (SPI) NFC Tag Reader on Tasmota

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
*/

#ifdef USE_SPI
#ifdef USE_RC522
/*********************************************************************************************\
 * MFRC522 - 13.56 MHz RFID reader
 *
 * Connections:
 * MFRC522  ESP8266         Tasmota
 * -------  --------------  ----------
 *  SDA     GPIO0..5,15,16  RC522 CS
 *  SCK     GPIO14          SPI CLK
 *  MOSI    GPIO13          SPI MOSI
 *  MISO    GPIO12          SPI MISO
 *  IRQ     not used
 *  Gnd     Gnd
 *  Rst     GPIO0..5,15,16  RC522 Rst
 *  3V3     3V3
\*********************************************************************************************/

#define XSNS_80        80

//#define USE_RC522_DATA_FUNCTION              // Add support for reading data block content (+0k4 code)
//#define USE_RC522_TYPE_INFORMATION           // Add support for showing card type (+0k4 code)

#include <MFRC522.h>
MFRC522 *Mfrc522;

// Watchdog: check two registers every second that reveal a broken-but-alive
// reader even though a plain VersionReg read (what RC522Status checks) keeps
// succeeding:
//  - Status2Reg bit3 (MFCrypto1On) - set while the crypto1 cipher is engaged
//    after a MIFARE authentication. If it's ever still set outside of an
//    active auth+read, every subsequent exchange with a card gets encrypted/
//    garbled by the reader itself, which silently breaks detection. Normally
//    PCD_StopCrypto1() clears it right after each read; we also no longer
//    authenticate at all (USE_RC522_DATA_FUNCTION is off), so this should
//    never legitimately be set - if it is, the reader is in a bad state.
//  - TxControlReg bits0-1 (Tx1RFEn|Tx2RFEn) - the antenna RF field. If this
//    ever reads back off, no card can physically be detected until it's
//    turned back on (see PCD_AntennaOn()).
// Toggle with RC522Watchdog 0/1 (on by default).

struct RC522 {
  char uids[21];           // Number of bytes in the UID. 4, 7 or 10
  bool present = false;
  bool watchdog = true;
  uint8_t scantimer = 16;
  uint8_t backoff = 0;     // extra 250ms ticks to skip after a slow/failing scan (see RC522ScanForTag)
} Rc522;

bool RC522IsConfigured(void) {
  return PinUsed(GPIO_RC522_CS) && (SPI_MOSI_MISO == TasmotaGlobal.spi_enabled);
}

bool RC522VersionAlive(uint8_t version) {
  return (0x00 != version) && (0xFF != version);
}

// Shortens the reader's own "wait for a PICC response" timer, which
// PCD_Init() sets to 25ms by default (TReloadRegH/L = 0x03E8 at a 25us tick,
// see PCD_Init() in the MFRC522 library). A real tag answers a REQA within a
// few hundred microseconds (ISO14443A FDT), so this comfortably preserves
// detection reliability. Measured on real hardware: this 25ms wait is the
// ACTUAL cost of every "no card" poll (PCD_CommunicateWithPICC blocks
// synchronously with no yield() until this on-chip timer fires) - cutting it
// to ~2.5ms cuts that recurring cost ~10x, which is what was driving
// Tasmota's LoadAvg toward its ceiling even with polling already throttled
// to roughly once a second (see RC522ScanForTag/RC522_IDLE_SCAN_TICKS).
// Must be called after every Mfrc522->PCD_Init(), which resets these
// registers back to the library's 25ms default.
void RC522ShortenPollTimeout(void) {
  Mfrc522->PCD_WriteRegister(MFRC522::TReloadRegH, 0x00);
  Mfrc522->PCD_WriteRegister(MFRC522::TReloadRegL, 0x64);  // 100 * 25us = 2.5ms
}

// Returns a reason string (for logging) if a known-bad state is found, else nullptr.
const char *RC522UnhealthyReason(void) {
  uint8_t status2 = Mfrc522->PCD_ReadRegister(MFRC522::Status2Reg);
  if (status2 & 0x08) {  // MFCrypto1On
    return PSTR("MFCrypto1On stuck set");
  }
  uint8_t txcontrol = Mfrc522->PCD_ReadRegister(MFRC522::TxControlReg);
  if ((txcontrol & 0x03) != 0x03) {  // Tx1RFEn|Tx2RFEn
    return PSTR("antenna RF field off");
  }
  return nullptr;
}

bool RC522EnsureReader(void) {
  if (!RC522IsConfigured()) { return false; }
  if (Mfrc522) { return true; }

  Mfrc522 = new MFRC522(Pin(GPIO_RC522_CS), Pin(GPIO_RC522_RST));
  if (!Mfrc522) {
    AddLog(LOG_LEVEL_INFO, PSTR("MFR: RC522 Rfid Reader init failed, no memory"));
    return false;
  }
  return true;
}

void RC522VersionText(uint8_t version, char *text, size_t text_size) {
  switch (version) {
    case 0x92: strcpy_P(text, PSTR("v2.0")); break;
    case 0x91: strcpy_P(text, PSTR("v1.0")); break;
    case 0x88: strcpy_P(text, PSTR("clone")); break;
    case 0x00:
    case 0xFF:
      strcpy_P(text, PSTR("fail"));
      break;
    default:
      snprintf_P(text, text_size, PSTR("unknown"));
      break;
  }
}

uint8_t RC522ReadRawVersionReg(void) {
  if (!RC522IsConfigured()) { return 0x00; }

  pinMode(Pin(GPIO_RC522_CS), OUTPUT);
  digitalWrite(Pin(GPIO_RC522_CS), HIGH);

  digitalWrite(Pin(GPIO_RC522_CS), LOW);
  SPI.beginTransaction(SPISettings(MFRC522_SPICLOCK, MSBFIRST, SPI_MODE0));
  SPI.transfer(MFRC522::VersionReg | 0x80);
  uint8_t version = SPI.transfer(0x00);
  SPI.endTransaction();
  digitalWrite(Pin(GPIO_RC522_CS), HIGH);

  return version;
}

// Measured on real hardware: a "no card" poll is NOT a hang - it's the
// MFRC522 library's normal, designed TimerIRq-bound timeout path
// (PCD_CommunicateWithPICC in MFRC522.cpp), reliably ~25-26ms, blocking
// synchronously with no yield(). The stock code called this every single
// 250ms tick unconditionally, i.e. ~10% permanent duty cycle - enough on its
// own to drive Tasmota's LoadAvg toward its ceiling within well under a
// minute. Polling less often while idle fixes this directly.
#define RC522_IDLE_SCAN_TICKS 2  // ~500ms between idle polls - combined with RC522ShortenPollTimeout()
                                  // (each poll now ~2.5ms instead of 25ms), duty cycle drops to ~0.5%

void RC522ScanForTag(void) {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle. And if present, select one.
  uint32_t scan_start = millis();
  bool card_read = Mfrc522->PICC_IsNewCardPresent() && Mfrc522->PICC_ReadCardSerial();
  int32_t elapsed = TimePassedSince(scan_start);
  if (!card_read) {
    // Exponential backoff as a safety net for the genuinely-abnormal case (a
    // truly slow/hanging poll, e.g. from RF noise triggering the heavier
    // anticollision path) - separate from the baseline idle interval below,
    // which handles the normal case.
    if (elapsed > 50) {
      AddLog(LOG_LEVEL_INFO, PSTR("MFR: slow scan attempt (%dms), backing off"), elapsed);
      Rc522.backoff = Rc522.backoff ? ((Rc522.backoff < 32) ? Rc522.backoff * 2 : 32) : 4;
    } else if (Rc522.backoff) {
      Rc522.backoff--;
    }
    Rc522.scantimer = Rc522.backoff ? Rc522.backoff : RC522_IDLE_SCAN_TICKS;
    return;
  }
  Rc522.backoff = 0;

  ToHex_P((unsigned char*)Mfrc522->uid.uidByte, Mfrc522->uid.size, Rc522.uids, sizeof(Rc522.uids));
  ResponseTime_P(PSTR(",\"RC522\":{\"UID\":\"%s\""), Rc522.uids);

  MFRC522::PICC_Type picc_type = Mfrc522->PICC_GetType(Mfrc522->uid.sak);
#ifdef USE_RC522_DATA_FUNCTION
  if (   picc_type == MFRC522::PICC_TYPE_MIFARE_MINI
      || picc_type == MFRC522::PICC_TYPE_MIFARE_1K
      || picc_type == MFRC522::PICC_TYPE_MIFARE_4K) {

    MFRC522::MIFARE_Key key;
    for (uint32_t i = 0; i < 6; i++) {
      key.keyByte[i] = 0xFF;
    }
    MFRC522::StatusCode status = Mfrc522->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(Mfrc522->uid));
    if (status == MFRC522::STATUS_OK) {
      uint8_t buffer[18];  // The buffer must be at least 18 bytes because a CRC_A is also returned
      uint8_t size = sizeof(buffer);
      status = (MFRC522::StatusCode) Mfrc522->MIFARE_Read(1, buffer, &size);
      if (status == MFRC522::STATUS_OK) {
        char card_datas[34];
        for (uint32_t i = 0; i < size -2; i++) {
          if ((isalpha(buffer[i])) || ((isdigit(buffer[i])))) {
            card_datas[i] = char(buffer[i]);
          } else {
            card_datas[i] = '\0';
          }
        }
        ResponseAppend_P(PSTR(",\"" D_JSON_DATA "\":\"%s\""), card_datas);
      }
    }
  }
#endif  // USE_RC522_DATA_FUNCTION
#ifdef USE_RC522_TYPE_INFORMATION
  ResponseAppend_P(PSTR(",\"" D_JSON_TYPE "\":\"%s\""), String(Mfrc522->PICC_GetTypeName(picc_type)).c_str());
#endif  // USE_RC522_TYPE_INFORMATION
  ResponseJsonEndEnd();
  MqttPublishTeleSensor();

  Mfrc522->PICC_HaltA();       // Halt PICC
  Mfrc522->PCD_StopCrypto1();  // Stop encryption on PCD

  Rc522.scantimer = 7;         // Ignore tags found for two seconds
}

void RC522Init(void) {
  if (RC522IsConfigured()) {

    pinMode(Pin(GPIO_RC522_CS), OUTPUT);
    digitalWrite(Pin(GPIO_RC522_CS), HIGH);

#ifdef ESP8266
    SPI.begin();
#endif

#ifdef ESP32
    SPI.begin(Pin(GPIO_SPI_CLK), Pin(GPIO_SPI_MISO), Pin(GPIO_SPI_MOSI), -1);
#endif

    delay(50);

    if (!RC522EnsureReader()) { return; }
    Mfrc522->PCD_Init();
    RC522ShortenPollTimeout();

    uint8_t v = Mfrc522->PCD_ReadRegister(MFRC522::VersionReg);

    char ver[16] = { 0 };
    RC522VersionText(v, ver, sizeof(ver));

    if (!RC522VersionAlive(v)) {
      AddLog(LOG_LEVEL_INFO, PSTR("MFR: RC522 Rfid Reader detected %s, VersionReg 0x%02X"), ver, v);
      Rc522.present = false;
      return;
    }

    uint8_t empty_uid[4] = { 0 };
    ToHex_P((unsigned char*)empty_uid, sizeof(empty_uid), Rc522.uids, sizeof(Rc522.uids));

    AddLog(LOG_LEVEL_INFO, PSTR("MFR: RC522 Rfid Reader detected %s, VersionReg 0x%02X"), ver, v);

    // NOTE: deliberately NOT calling Mfrc522->PCD_PerformSelfTest() here anymore.
    // It does PCD_Reset() + writes test patterns into several registers and
    // requires a full PCD_Init() afterward to undo (the library's own docs call
    // this out - "self-test blows init"). On marginal/noisy real-world modules
    // this was observed to leave the reader in a state where every subsequent
    // 250ms scan attempt hit the slow/timeout path in the library's busy-wait
    // loop (PCD_CommunicateWithPICC, up to ~36ms of blocking SPI polling per
    // call with no yield()), which is expensive enough when it happens on
    // every tick to peg Tasmota's LoadAvg near 999 within ~60-90s of boot and
    // starve the rest of the system. VersionReg alone is a reliable presence
    // check without this risk; see RC522ScanForTag()'s backoff for a second,
    // independent guard against the same failure mode regardless of cause.
    Rc522.present = true;
  }
}

#ifdef USE_WEBSERVER
void RC522Show(void) {
  WSContentSend_PD(PSTR("{s}RC522 UID{m}%s{e}"), Rc522.uids);
}
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Supported commands for Sensor80:
 *
 * Sensor80 1        - Show antenna gain
 * Sensor80 1 <gain> - Set antenna gain 0..7 (default 4)
 * RC522Status       - Show MFRC522 SPI/VersionReg status
 * RC522Reset        - Soft reset and re-init MFRC522
\*********************************************************************************************/

bool RC522Command(void) {
  bool serviced = true;
  char argument[XdrvMailbox.data_len];

  for (uint32_t ca = 0; ca < XdrvMailbox.data_len; ca++) {
    if ((' ' == XdrvMailbox.data[ca]) || ('=' == XdrvMailbox.data[ca])) { XdrvMailbox.data[ca] = ','; }
  }

  switch (XdrvMailbox.payload) {
    case 1:  // Antenna gain
      uint8_t gain;
      if (strchr(XdrvMailbox.data, ',') != nullptr) {
        gain = strtol(ArgV(argument, 2), nullptr, 10) & 0x7;
        Mfrc522->PCD_SetAntennaGain(gain << 4);
      }
      gain = Mfrc522->PCD_GetAntennaGain() >> 4;  // 0..7
      Response_P(PSTR("{\"Sensor80\":{\"Gain\":%d}}"), gain);
      break;
  }

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

void RC522StatusResponse(void) {
  uint8_t raw = RC522ReadRawVersionReg();
  uint8_t version = Mfrc522 ? Mfrc522->PCD_ReadRegister(MFRC522::VersionReg) : raw;
  bool alive = RC522IsConfigured() && RC522VersionAlive(version);

  char ver[16] = { 0 };
  RC522VersionText(version, ver, sizeof(ver));
  Rc522.present = alive;

  const char *unhealthy = (Mfrc522 && alive) ? RC522UnhealthyReason() : nullptr;

  Response_P(PSTR("{\"%s\":{\"Configured\":%d,\"Present\":%d,\"Alive\":%d,\"Version\":\"%s\",\"VersionReg\":\"0x%02X\",\"RawVersionReg\":\"0x%02X\",\"UID\":\"%s\",\"Healthy\":%d}}"),
    XdrvMailbox.command,
    RC522IsConfigured(),
    Rc522.present,
    alive,
    ver,
    version,
    raw,
    Rc522.uids,
    !unhealthy
  );
}

void CmndRC522Status(void) {
  RC522StatusResponse();
}

void RC522FullReset(void) {
  Mfrc522->PICC_HaltA();
  Mfrc522->PCD_StopCrypto1();
  Mfrc522->PCD_Reset();
  delay(4);
  Mfrc522->PCD_Init();
  RC522ShortenPollTimeout();
  delay(4);
  Rc522.scantimer = 0;
}

void CmndRC522Reset(void) {
  if (RC522EnsureReader()) {
    RC522FullReset();
  } else {
    Rc522.present = false;
  }
  RC522StatusResponse();
}

void CmndRC522Watchdog(void) {
  if (XdrvMailbox.data_len > 0) {
    Rc522.watchdog = (XdrvMailbox.payload != 0);
  }
  Response_P(PSTR("{\"%s\":%d}"), XdrvMailbox.command, Rc522.watchdog);
}

const char kRC522Commands[] PROGMEM = "RC522|Status|Reset|Watchdog";
void (* const RC522Commands[])(void) PROGMEM = { &CmndRC522Status, &CmndRC522Reset, &CmndRC522Watchdog };

bool Xsns80(uint32_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    RC522Init();
  }
  else if (FUNC_COMMAND == function) {
    result = DecodeCommand(kRC522Commands, RC522Commands);
  }
  else if (Rc522.present) {
    switch (function) {
      case FUNC_EVERY_250_MSECOND:
        if (Rc522.scantimer) {
          Rc522.scantimer--;
        } else {
          RC522ScanForTag();
        }
        break;
      case FUNC_EVERY_SECOND: {
        if (Rc522.watchdog) {
          const char *reason = RC522UnhealthyReason();
          if (reason) {
            AddLog(LOG_LEVEL_INFO, PSTR("MFR: watchdog reset, reason: %s"), reason);
            RC522FullReset();
          }
        }
        break;
      }
      case FUNC_COMMAND_SENSOR:
        if (XSNS_80 == XdrvMailbox.index) {
          result = RC522Command();
        }
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        RC522Show();
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_RC522
#endif  // USE_SPI
