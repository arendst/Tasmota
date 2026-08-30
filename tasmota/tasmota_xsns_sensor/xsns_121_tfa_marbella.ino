/*
  xsns_121_tfa_marbella.ino - TFA Dostmann Marbella pool thermometer support for Tasmota

  SPDX-FileCopyrightText: 2026 Philipp Schroeer

  SPDX-License-Identifier: GPL-3.0-only

  The radio protocol, the checksum algorithm and the temperature formula are
  taken from the rtl_433 project:
    src/devices/tfa_marbella.c - Copyright (C) 2021 Benjamin Larsson
    src/bit_util.c             - lfsr_digest8_reflect()
  both GPL-2.0-or-later, which is compatible with the GPL-3.0 of Tasmota.
*/

#ifdef USE_SPI
#ifdef USE_TFA_MARBELLA

/*
  One CC1101, two drivers: Keeloq transmits Jarolift codes on 433 MHz, this one
  receives on 868 MHz. They would fight over the same chip, and whichever
  configured it last would win - silently, with the other simply never working.
  Fail at compile time instead.
*/
#ifdef USE_KEELOQ
#error "USE_TFA_MARBELLA and USE_KEELOQ both drive the CC1101 and cannot be combined"
#endif

/*********************************************************************************************\
 * TFA Dostmann Marbella (30.3066.01) floating pool thermometer, 868 MHz
 *
 * Receives the sensor with a CC1101 module and reports water temperature,
 * sensor id, rolling counter and battery state.
 *
 * Wiring - the shared SPI bus plus two device pins:
 *   CC1101 SCK/MISO/MOSI -> the SPI CLK, MISO and MOSI roles
 *   CC1101 CSN           -> role "CC1101 CS"
 *   CC1101 GDO0          -> role "CC1101 GDO0"
 *
 * GDO2 is not needed: receiving uses GDO0 only.
 *
 * Readings are published as they arrive, the way the other receiving
 * drivers do it, and go out with the regular TelePeriod telemetry as well.
 * SetOption147 suppresses the immediate publish.
 *
 * Commands:
 * Marbella          - Show the bound sensor id and the reception state
 * Marbella <id>     - Bind to one sensor, id as six hex digits, e.g. 683f16
 * Marbella 0        - Forget the binding and learn the next sensor seen
 * Marbella <MHz>    - Tune the receiver, e.g. 868.021, to compensate the
 *                     tolerance of the module's crystal. Stored; the decimal
 *                     point is what tells a frequency from an id.
 *
 * SetOption147 1    - Do not publish readings as they arrive; rules still see
 *                     them. Shared with the other receiving drivers.
 *
 * The binding is stored and survives a restart. TFA_MARBELLA_SERIAL fixes it
 * at compile time instead.
\*********************************************************************************************/

#define XSNS_121                     121

#include <RadioLib.h>

#ifndef TFA_MARBELLA_TIMEOUT
#define TFA_MARBELLA_TIMEOUT         900      // Seconds without a packet until the reading is dropped
#endif
#ifndef TFA_MARBELLA_SERIAL
#define TFA_MARBELLA_SERIAL          0        // 0 = learn, otherwise a fixed sensor id
#endif

/*
  Radio parameters. rtl_433 documents FSK_PULSE_PCM at 105 us per bit in the
  868 MHz band; the modulation and the band hold, the symbol rate does not.

  Measured off the air with an SDR from three recordings of this sensor, and
  cross-checked by decoding each recording at every sampling phase with the
  sensor's own checksum as the verdict: 9400 Bd, not the 9523.8 Bd that
  105 us implies, and about 34.5 kHz deviation rather than 30. All three
  recordings peaked at the same rate, and none produced a single valid frame
  at 9523.8 Bd - over the 112 bits of a transmission a rate that is 1.3 %
  off drifts more than a full bit, so the tail of every frame arrives
  shifted. That is why 9.6 kBit/s delivered nothing at all and 9.5238 only
  worked now and then.

  The frequency is the protocol's 868.0 MHz. What a particular receiver has
  to be tuned to is a separate matter - see the Marbella command below.
*/
#define TFA_MARBELLA_FREQUENCY       868.0    // MHz, the frequency the protocol uses
#define TFA_MARBELLA_FREQ_RANGE      0.2      // MHz, how far the tuning may be moved either way
#define TFA_MARBELLA_BITRATE         9.4      // kBit/s, measured - see above
#define TFA_MARBELLA_DEVIATION       34.5     // kHz, measured - see above
#define TFA_MARBELLA_RX_BANDWIDTH    135.0    // kHz
#define TFA_MARBELLA_POWER           10       // dBm, receive only but begin() wants it
#define TFA_MARBELLA_PREAMBLE        16       // bits, see the note on the quality threshold below

/*
  Sync word 0xAAD2, not the 0xAA 0x2D 0xD4 the frame itself starts with -
  worth a word, because it looks like a mistake.

  The CC1101 synchronises in hardware and strips everything up to and
  including the word it matched, and the buffer then begins with AA 2D D4. So
  the sync word has to sit in front of that. Demodulating recordings bit by
  bit shows what the sensor actually sends:

    ... 1010 1010 1010 | 1101 0010 | 1010 1010 0010 1101 1101 0100 ...
        <- preamble ->    <- D2 ->   <-        AA 2D D4        ->

  The last preamble byte is 0xAA, so the 16 bits before the frame read AA D2 -
  which lands the radio precisely on the frame start. 0x2DD4 delivers nothing.
*/
#define TFA_MARBELLA_SYNC1           0xAA
#define TFA_MARBELLA_SYNC0           0xD2

/*
  A packet as rtl_433 counts it, and as it arrives in the buffer:

    AA 2D D4 68 3F 16 0A 31 9A AA AE
    PP SS SS RR RR RR FF TT TA AA LL

    P  preamble
    S  sync word 0x2DD4
    R  sensor serial number, changes only after a sensor reset
    F  flags: bit 7 low battery, bits 3-1 rolling counter
    T  12 bit temperature
    A  constant 0xA
    L  checksum over the 7 bytes starting at the serial number

  Captured from the sensor, three consecutive frames:

    AA 2D D4 68 B9 4A 4A 2C BA AA B8   31.5 C, counter 5
    AA 2D D4 68 B9 4A 4C 2C CA AA 8A   31.6 C, counter 6
    AA 2D D4 68 B9 4A 4E 2C CA AA 8D   31.6 C, counter 7
*/
// The first three bytes of the frame itself - not to be confused with the sync
// word above, which is what the radio matches on, one bit earlier.
#define TFA_MARBELLA_HEADER0         0xAA
#define TFA_MARBELLA_HEADER1         0x2D
#define TFA_MARBELLA_HEADER2         0xD4

#define TFA_MARBELLA_PACKET_LENGTH   11
#define TFA_MARBELLA_CHECKED_BYTES   7        // Bytes covered by the checksum
#define TFA_MARBELLA_PAYLOAD_LENGTH  TFA_MARBELLA_PACKET_LENGTH
#define TFA_MARBELLA_SERIAL_MASK     0x00FFFFFF

// Re-arm the receiver if no valid frame arrived for this long - a lost GDO0
// edge would otherwise leave the radio waiting on a full buffer forever.
// Two missed transmissions, the sensor sends once a minute.
#define TFA_MARBELLA_REARM           120      // Seconds

CC1101 TfaMarbellaRadio = nullptr;

struct TfaMarbellaData_s {
  float    temperature;
  uint32_t serial;                            // Serial number of the last valid packet
  uint32_t last_packet;                       // Uptime in seconds of the last valid packet
  uint32_t last_rearm;                        // Uptime in seconds of the last receiver restart
  uint8_t  lqi;                               // Link quality of the last valid packet, 0..127, lower is better
  uint8_t  counter;
  bool     low_battery;
  bool     valid;
} *TfaMarbellaData = nullptr;

/*
  Set from the interrupt, evaluated in the main loop - the same split the LoRa
  drivers use (xdrv_73_3_lora_sx127x_v2.ino). The handler does nothing but set
  a flag, so it stays short enough to live in IRAM on ESP8266.
*/
volatile bool TfaMarbellaPacketFlag = false;

void IRAM_ATTR TfaMarbellaOnReceive(void);
void TfaMarbellaOnReceive(void) {
  TfaMarbellaPacketFlag = true;
}

/********************************************************************************************/

/*
  Reverse Galois LFSR digest, reflected, generator and key both 0x31.

  Port of lfsr_digest8_reflect() from rtl_433 (src/bit_util.c), reduced to the
  one parameter set this sensor uses. Operates on the seven payload bytes and
  yields the value the sensor transmits as its last byte.
*/
uint8_t TfaMarbellaChecksum(const uint8_t* payload) {
  const uint8_t generator = 0x31;
  uint8_t key = 0x31;
  uint8_t sum = 0;

  // Process the message from the last byte to the first, and each byte from
  // its least significant bit upwards - that is what "reflected" means here.
  for (int8_t i = TFA_MARBELLA_CHECKED_BYTES -1; i >= 0; i--) {
    const uint8_t data = payload[i];
    for (uint8_t bit = 0; bit < 8; bit++) {
      if ((data >> bit) & 1) {
        sum ^= key;
      }
      if (key & 0x80) {
        key = (uint8_t)((key << 1) ^ generator);
      } else {
        key = (uint8_t)(key << 1);
      }
    }
  }
  return sum;
}

/*
  Decode one frame into TfaMarbellaData, return false for anything that is not
  a valid packet of this sensor family.

  Note what is not checked: a plausibility range for the temperature. A range
  of 0..50 degrees, as sometimes used in place of a checksum, would wrongly
  discard a real reading of 51 degrees. The checksum is the stronger test.
*/
bool TfaMarbellaDecode(const uint8_t* packet) {
  // Preamble and sync word
  if ((TFA_MARBELLA_HEADER0 != packet[0]) || (TFA_MARBELLA_HEADER1 != packet[1]) ||
      (TFA_MARBELLA_HEADER2 != packet[2])) {
    return false;
  }
  // Sanity: byte 9 is constant in this protocol
  if (0xAA != packet[9]) { return false; }
  // Checksum over serial number, flags and temperature
  if (TfaMarbellaChecksum(&packet[3]) != packet[10]) { return false; }

  const uint16_t raw = (uint16_t)((packet[7] << 4) | (packet[8] >> 4));

  TfaMarbellaData->serial = ((uint32_t)packet[3] << 16) | ((uint32_t)packet[4] << 8) | packet[5];
  TfaMarbellaData->temperature = ((int16_t)raw - 400) * 0.1f;
  TfaMarbellaData->counter = (packet[6] >> 1) & 0x07;
  TfaMarbellaData->low_battery = (packet[6] >> 7) & 0x01;
  return true;
}

bool TfaMarbellaReadingValid(void) {
  if (!TfaMarbellaData || !TfaMarbellaData->valid) { return false; }
  return (TasmotaGlobal.uptime - TfaMarbellaData->last_packet) < TFA_MARBELLA_TIMEOUT;
}

/*
  Apply one radio setting and report the first failure. Without this the driver
  would announce a running receiver while the chip is misconfigured and never
  delivers a packet.
*/
bool TfaMarbellaApply(int16_t state, const char* what) {
  if (RADIOLIB_ERR_NONE == state) { return true; }
  AddLog(LOG_LEVEL_DEBUG, PSTR("MRB: %s failed (%d)"), what, state);
  return false;
}

/*
  The frequency the radio is actually tuned to: the protocol frequency plus
  whatever this module's crystal needs. Stored in 100 Hz steps, which is
  finer than the CC1101 can tune anyway (its step is 26 MHz / 2^16, so about
  397 Hz) and leaves the setting a comfortable range in an int16.
*/
float TfaMarbellaTuning(void) {
  return TFA_MARBELLA_FREQUENCY + (float)Settings->marbella_frequency / 10000.0f;
}

void TfaMarbellaInit(void) {
  if ((SPI_MOSI_MISO != TasmotaGlobal.spi_enabled) ||
      !PinUsed(GPIO_CC1101_CS) || !PinUsed(GPIO_CC1101_GDO0)) { return; }

  // RadioLib calls SPI.begin() without arguments, which picks the default bus
  // pins - on ESP32 that ignores the GPIO template entirely. Set the bus up
  // here, the way the LoRa drivers do (xdrv_73_9_lora.ino).
#ifdef ESP8266
  SPI.begin();
#endif  // ESP8266
#ifdef ESP32
  SPI.begin(Pin(GPIO_SPI_CLK), Pin(GPIO_SPI_MISO), Pin(GPIO_SPI_MOSI), -1);
#endif  // ESP32

  TfaMarbellaRadio = new Module(Pin(GPIO_CC1101_CS), Pin(GPIO_CC1101_GDO0), RADIOLIB_NC, RADIOLIB_NC, SPI);

  int16_t state = TfaMarbellaRadio.begin(TfaMarbellaTuning(), TFA_MARBELLA_BITRATE,
                                         TFA_MARBELLA_DEVIATION, TFA_MARBELLA_RX_BANDWIDTH,
                                         TFA_MARBELLA_POWER, TFA_MARBELLA_PREAMBLE);
  if (!TfaMarbellaApply(state, PSTR("CC1101 not found"))) { return; }

  /*
    2-FSK without encoding, a fixed length frame, and the radio's own CRC
    switched off: this sensor carries a checksum of its own, and the decoder
    verifies that one. A CRC filter here would drop every packet.
  */
  /*
    Order matters and follows a configuration proven against this sensor:
    shaping, encoding, sync word, CRC filtering, packet length, address
    filtering. setCrcFiltering() and fixedPacketLengthMode() both write
    PKTCTRL0, so swapping them is not a cosmetic change.
  */
  /*
    Switch the preamble quality threshold off. begin() derives it from the
    preamble length - setPreambleLength(len, len - 4) - and the chip then only
    accepts a sync word when it has counted enough clean preamble transitions
    right before it. Anything from 32 bits upwards clamps the threshold to its
    hardest setting, and the sensor's preamble is not good enough for that: the
    sync word is dropped in silence, so no interrupt arrives at all and the
    driver sees nothing while the sensor transmits happily. Measured: not a
    single edge in 90 seconds, where 0 makes frames arrive on every
    transmission.
  */
  if (!TfaMarbellaApply(TfaMarbellaRadio.setPreambleLength(TFA_MARBELLA_PREAMBLE, 0), PSTR("Preamble")) ||
      !TfaMarbellaApply(TfaMarbellaRadio.setDataShaping(RADIOLIB_SHAPING_NONE), PSTR("Data shaping")) ||
      !TfaMarbellaApply(TfaMarbellaRadio.setEncoding(RADIOLIB_ENCODING_NRZ), PSTR("Encoding")) ||
      !TfaMarbellaApply(TfaMarbellaRadio.setSyncWord(TFA_MARBELLA_SYNC1, TFA_MARBELLA_SYNC0), PSTR("Sync word")) ||
      !TfaMarbellaApply(TfaMarbellaRadio.setCrcFiltering(false), PSTR("CRC filter")) ||
      !TfaMarbellaApply(TfaMarbellaRadio.fixedPacketLengthMode(TFA_MARBELLA_PAYLOAD_LENGTH), PSTR("Packet length")) ||
      !TfaMarbellaApply(TfaMarbellaRadio.disableAddressFiltering(), PSTR("Address filter"))) {
    return;
  }

  /*
    disableAddressFiltering() is not optional: RadioLib leaves address checking
    on, and the chip then discards every frame whose first byte does not match
    the node address. This sensor starts its frames with 0xAA, so without this
    call reception runs but nothing ever arrives.
  */

  TfaMarbellaData = (TfaMarbellaData_s *)calloc(1, sizeof(struct TfaMarbellaData_s));
  if (!TfaMarbellaData) { return; }
  /*
    The binding lives in the settings and therefore survives a restart. That
    matters: the sensor sends about once a minute, so a driver that forgets
    its sensor on every reboot shows nothing for minutes afterwards - and
    worse, would happily bind to whatever it hears first, which need not be
    the right one.
  */
  if (TFA_MARBELLA_SERIAL) {
    Settings->marbella_serial = TFA_MARBELLA_SERIAL & TFA_MARBELLA_SERIAL_MASK;
  }

  /*
    The packet-received edge, not both edges. With GDO0 mapped inverted the
    rising edge marks the end of a frame, which is exactly when the FIFO holds
    it complete - reading on any other edge races the chip filling it, and the
    byte last read comes out twice with everything after it shifted along
    (CC1101 errata SWRZ020). setPacketReceivedAction() is RadioLib's own name
    for this and picks the right edge for us.
  */
  TfaMarbellaRadio.setPacketReceivedAction(TfaMarbellaOnReceive);
  TfaMarbellaRadio.startReceive();
  TfaMarbellaData->last_rearm = TasmotaGlobal.uptime;
  AddLog(LOG_LEVEL_INFO, PSTR("MRB: TFA Marbella receiver started"));
}

void TfaMarbellaEvery50ms(void) {
  if (!TfaMarbellaPacketFlag) { return; }

  uint8_t packet[TFA_MARBELLA_PACKET_LENGTH];
  int16_t state = TfaMarbellaRadio.readData(packet, sizeof(packet));
  TfaMarbellaRadio.startReceive();                   // Back to listening either way
  /*
    Clear the flag AFTER restarting reception, not before: startReceive() puts
    GDO0 back to its idle level, which is an edge in itself and sets the flag
    again. Clearing first means the driver's own restart re-arms it
    immediately, and the loop then reads the same stale buffer thousands of
    times per second - measured, not feared.
  */
  TfaMarbellaPacketFlag = false;
  if (RADIOLIB_ERR_NONE != state) { return; }

  const uint32_t bound = Settings->marbella_serial;
  if (!TfaMarbellaDecode(packet)) { return; }

  // Bind to the first sensor heard, then ignore every other one. Without this
  // a neighbour's identical sensor would feed the same reading.
  if (!bound) {
    Settings->marbella_serial = TfaMarbellaData->serial;
    AddLog(LOG_LEVEL_INFO, PSTR("MRB: Bound to sensor %06X"), TfaMarbellaData->serial);
  } else if (bound != TfaMarbellaData->serial) {
    return;
  }

  TfaMarbellaData->lqi = TfaMarbellaRadio.getLQI();   // From the packet's status bytes
  TfaMarbellaData->last_packet = TasmotaGlobal.uptime;
  TfaMarbellaData->valid = true;

  /*
    Publish as the reading arrives, and let SetOption147 turn that off - the
    same handle the other receiving drivers use (serial bridge, IR, LoRaWAN,
    WizMote). Sharing it costs no new SetOption number, and a user who has
    already silenced one receiver on a busy broker means the same thing here.

    Publishing on arrival is the right default for this sensor: it transmits
    about once a minute, so with the default TelePeriod of 300 seconds four
    out of five readings would never leave the device, and the one that does
    could be almost five minutes old.
  */
  if (!Settings->flag6.mqtt_disable_publish) {  // SetOption147 - If it is activated, Tasmota will not publish MQTT messages, but it will proccess event trigger rules
    MqttPublishSensor();
  }
}

/*
  Restart reception if the radio has been quiet for a while. A missed GDO0 edge
  leaves the CC1101 sitting on a full buffer, and nothing would ever wake it
  again - the reading would simply stop, which is the hardest failure to spot.
*/
void TfaMarbellaEverySecond(void) {
  /*
    Measured against the last valid frame, not against the last interrupt.
    Interrupts keep arriving while the radio is stuck - startReceive() puts
    GDO0 back to its idle level, which is an edge in itself - so a timer fed
    by interrupts is restarted forever and never fires. Observed on hardware:
    the receiver went quiet for seven minutes while the re-arm timer, reset by
    every one of those edges, never ran out. A restart brought the reading
    back at once.
  */
  if ((TasmotaGlobal.uptime - TfaMarbellaData->last_packet) < TFA_MARBELLA_REARM) { return; }
  if ((TasmotaGlobal.uptime - TfaMarbellaData->last_rearm) < TFA_MARBELLA_REARM) { return; }
  TfaMarbellaData->last_rearm = TasmotaGlobal.uptime;
  TfaMarbellaRadio.startReceive();
  AddLog(LOG_LEVEL_DEBUG, PSTR("MRB: Receiver re-armed after %d s without a frame"),
         TFA_MARBELLA_REARM);
}

/*
  Renders the age of the last valid packet: "12 sec ago", "4 min ago",
  "3 hours ago". A bare number of seconds stops being readable after a few
  minutes, and this line is read precisely when something has been quiet for
  a long time.
*/
void TfaMarbellaAge(char* dest, size_t size, uint32_t seconds) {
  if (seconds < 120) {
    snprintf_P(dest, size, PSTR("%u " D_UNIT_SECOND " ago"), seconds);
  } else if (seconds < 7200) {
    snprintf_P(dest, size, PSTR("%u " D_UNIT_MINUTE " ago"), seconds / 60);
  } else {
    snprintf_P(dest, size, PSTR("%u " D_UNIT_HOUR " ago"), seconds / 3600);
  }
}

/*
  The reading is dropped from JSON once TFA_MARBELLA_TIMEOUT has passed without
  a packet: a stale value looks exactly like a measurement that simply does not
  change - the one failure this driver must not produce.

  The web interface, however, always shows a line. Showing nothing at all is
  indistinguishable from a driver that is not running, from a radio that never
  starts and from a sensor whose battery died - and those need different
  answers. So the age of the last packet is stated, and if there never was one,
  that is said too.
*/
void TfaMarbellaShow(bool json) {
  bool valid = TfaMarbellaReadingValid();
  bool ever = (TfaMarbellaData && TfaMarbellaData->valid);

  if (json && !valid) { return; }

#ifdef USE_WEBSERVER
  if (!json && !valid) {
    if (ever) {
      char age[24];
      TfaMarbellaAge(age, sizeof(age), TasmotaGlobal.uptime - TfaMarbellaData->last_packet);
      WSContentSend_PD(PSTR("{s}TFA Marbella{m}no signal, last %s{e}"), age);
    } else {
      WSContentSend_PD(PSTR("{s}TFA Marbella{m}no signal yet{e}"));
    }
    return;
  }
#else
  if (!valid) { return; }
#endif  // USE_WEBSERVER

  float temperature = ConvertTemp(TfaMarbellaData->temperature);
  char id[7];
  snprintf_P(id, sizeof(id), PSTR("%06X"), Settings->marbella_serial);

  if (json) {
    ResponseAppend_P(PSTR(",\"TFAMarbella\":{\"" D_JSON_ID "\":\"%s\",\"" D_JSON_TEMPERATURE "\":%*_f"
                          ",\"Counter\":%d,\"BatteryLow\":%d,\"LQI\":%d}"),
                     id, Settings->flag2.temperature_resolution, &temperature,
                     TfaMarbellaData->counter, TfaMarbellaData->low_battery,
                     TfaMarbellaData->lqi);
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_Temp("TFA Marbella", temperature);
    /*
      Always show the battery, not only when it is low: a line that appears
      only in the bad case cannot be told apart from a driver that does not
      report it at all. The sensor sends a single bit, so this is "ok" or
      "low" - not a percentage.
    */
    WSContentSend_PD(PSTR("{s}TFA Marbella " D_BATTERY "{m}%s{e}"),
                     TfaMarbellaData->low_battery ? "low - replace" : "ok");
    /*
      The age of the reading, not a timestamp: the sensor sends once a minute,
      so what matters is whether the last packet is seconds or hours old. A
      clock time would also be wrong whenever the device has no NTP yet.
    */
    char age[24];
    TfaMarbellaAge(age, sizeof(age), TasmotaGlobal.uptime - TfaMarbellaData->last_packet);
    WSContentSend_PD(PSTR("{s}TFA Marbella Last reading{m}%s{e}"), age);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#define D_CMND_MARBELLA "Marbella"

const char kTfaMarbellaCommands[] PROGMEM = "|" D_CMND_MARBELLA;

void (* const TfaMarbellaCommand[])(void) PROGMEM = { &CmndTfaMarbella };

void CmndTfaMarbella(void) {
  if (XdrvMailbox.data_len) {
    /*
      A frequency carries a decimal point and an id does not - that is what
      tells the two apart, and it means neither needs a keyword.

      Why this is a setting and not a constant: the CC1101 is tuned by its own
      26 MHz crystal, and its tolerance lands the receiver somewhere around
      the frequency it was told. One module measured here sat 27 kHz low.
      That still fits inside the 135 kHz receive filter, but it spends the
      margin the signal needs, and it belongs to the board, not the driver.
    */
    if (strchr(XdrvMailbox.data, '.')) {
      char* end = nullptr;
      float freq = strtof(XdrvMailbox.data, &end);
      if ((end == XdrvMailbox.data) || (*end != '\0') ||
          (freq < TFA_MARBELLA_FREQUENCY - TFA_MARBELLA_FREQ_RANGE) ||
          (freq > TFA_MARBELLA_FREQUENCY + TFA_MARBELLA_FREQ_RANGE)) {
        return;                                      // Leaves the command unhandled -> "Invalid"
      }
      Settings->marbella_frequency = (int16_t)roundf((freq - TFA_MARBELLA_FREQUENCY) * 10000.0f);
      /*
        startReceive() is not optional: setFrequency() leaves the chip in
        standby and does not bring it back, so without it the receiver goes
        deaf the moment it is retuned - and stays deaf, because the re-arm
        timer did not recover it either when measured on hardware. A retune
        that kills reception looks exactly like a retune to a bad frequency.
      */
      if (!TfaMarbellaApply(TfaMarbellaRadio.setFrequency(TfaMarbellaTuning()), PSTR("Frequency")) ||
          !TfaMarbellaApply(TfaMarbellaRadio.startReceive(), PSTR("Receive"))) {
        return;                                      // Leaves the command unhandled -> "Invalid"
      }
    } else {
      /*
        The id is printed as hex everywhere, so it is entered as hex too, and
        as the full six digits - "0" to clear the binding is the one exception.

        Six digits exactly, because a shorter number is more likely a mistyped
        frequency than an id: "Marbella 868" without the decimal point would
        otherwise bind to sensor 000868 and report success, and reception
        would stop for a reason nothing points at.
      */
      char* end = nullptr;
      uint32_t id = strtoul(XdrvMailbox.data, &end, 16);
      const uint32_t len = strlen(XdrvMailbox.data);
      if ((end == XdrvMailbox.data) || (*end != '\0') || (id > TFA_MARBELLA_SERIAL_MASK) ||
          ((6 != len) && !((1 == len) && (0 == id)))) {
        return;                                      // Leaves the command unhandled -> "Invalid"
      }
      Settings->marbella_serial = id;
      TfaMarbellaData->valid = false;
    }
  }
  float tuning = TfaMarbellaTuning();
  Response_P(PSTR("{\"%s\":{\"" D_JSON_ID "\":\"%06X\",\"Bound\":\"%s\",\"Reading\":\"%s\""
                  ",\"" D_JSON_FREQUENCY "\":%*_f}}"),
             XdrvMailbox.command, Settings->marbella_serial,
             Settings->marbella_serial ? "YES" : "LEARNING",
             TfaMarbellaReadingValid() ? "VALID" : "NONE",
             3, &tuning);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns121(uint32_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    TfaMarbellaInit();
  }
  else if (TfaMarbellaData) {
    switch (function) {
      case FUNC_EVERY_50_MSECOND:
        TfaMarbellaEvery50ms();
        break;
      case FUNC_EVERY_SECOND:
        TfaMarbellaEverySecond();
        break;
      case FUNC_JSON_APPEND:
        TfaMarbellaShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        TfaMarbellaShow(0);
        break;
#endif  // USE_WEBSERVER
      case FUNC_COMMAND:
        result = DecodeCommand(kTfaMarbellaCommands, TfaMarbellaCommand);
        break;
    }
  }
  return result;
}

#endif  // USE_TFA_MARBELLA
#endif  // USE_SPI
