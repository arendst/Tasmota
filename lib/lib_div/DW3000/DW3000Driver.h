#pragma once
/*
 * DW3000Driver.h
 * Low-level HAL for the Qorvo DW3000 UWB chip
 *
 * Encapsulates all SPI/register interactions,
 * independent of Tasmota logic.
 *
 * License  : Apache 2.0 (Tasmota compatible)
 * Reference: DW3000 User Manual (Qorvo)
 */

#include <SPI.h>
#include <Arduino.h>

// ─── Default pins (Makerfabs ESP32 UWB DW3000 WROOM) ─────────────────────────
// Override via Tasmota GPIO template
#ifndef UWB_PIN_CS
  #define UWB_PIN_CS   4
#endif
#ifndef UWB_PIN_RST
  #define UWB_PIN_RST  27
#endif
#ifndef UWB_PIN_IRQ
  #define UWB_PIN_IRQ  34
#endif

// ESP32 default SPI: SCK=18, MISO=19, MOSI=23

// ─── DW3000 constants ────────────────────────────────────────────────────────

// Expected device ID (register 0x00, 4 bytes)
#define DW3000_DEVICE_ID        0xDECA0302

// SPI speed: slow during init, fast for normal operation
#define DW3000_SPI_SPEED_INIT   4000000UL
#define DW3000_SPI_SPEED_FAST   16000000UL

// Register bank addresses (legacy DW1000-style naming kept for readability)
#define DW3000_REG_DEV_ID       0x00
#define DW3000_REG_EUI_64       0x01
#define DW3000_REG_PANADR       0x03
#define DW3000_REG_SYS_CFG      0x04
#define DW3000_REG_SYS_TIME     0x06
#define DW3000_REG_TX_FCTRL     0x08
#define DW3000_REG_TX_BUFFER    0x09
#define DW3000_REG_DX_TIME      0x0A
#define DW3000_REG_RX_FWTO      0x0C
#define DW3000_REG_SYS_CTRL     0x0D
#define DW3000_REG_SYS_ENABLE   0x0E
#define DW3000_REG_SYS_STATUS   0x0F
#define DW3000_REG_RX_FINFO     0x10
#define DW3000_REG_RX_BUFFER    0x11
#define DW3000_REG_RX_TIME      0x15
#define DW3000_REG_TX_TIME      0x17
#define DW3000_REG_CHAN_CTRL    0x1F
#define DW3000_REG_SFD          0x21
#define DW3000_REG_AGC_CTRL     0x23
#define DW3000_REG_EXT_SYNC     0x24
#define DW3000_REG_LDE_CTRL     0x2E
#define DW3000_REG_DIG_DIAG     0x2F
#define DW3000_REG_PMSC         0x36
#define DW3000_REG_PLL_CFG      0x08  // sub-register

// SYS_STATUS bits (DW3000 UM + Makerfabs SDK verified)
#define DW3000_SYS_STATUS_TXFRS   (1UL << 7)   // TX Frame Sent
#define DW3000_SYS_STATUS_RXDFR   (1UL << 13)  // RX Frame Ready (RXFR)
#define DW3000_SYS_STATUS_RXFCG   (1UL << 14)  // RX Frame CRC Good
#define DW3000_SYS_STATUS_RXFCE   (1UL << 15)  // RX Frame CRC Error
#define DW3000_SYS_STATUS_RXPHE   (1UL << 12)  // RX PHY Header Error
#define DW3000_SYS_STATUS_RXRFTO  (1UL << 21)  // RX RF Timeout
#define DW3000_SYS_STATUS_RXPTO   (1UL << 22)  // RX Preamble Timeout
#define DW3000_SYS_STATUS_RXSTO   (1UL << 26)  // RX SFD Timeout

// DW3000 time unit: 1 tick ≈ 15.65 ps
#define DW3000_TIME_UNIT_PS     15.65
#define SPEED_OF_LIGHT_CM_S     29979245800.0

// ─── Enumerations ─────────────────────────────────────────────────────────────

enum DW3000Channel : uint8_t {
  UWB_CHANNEL_5 = 5,   // 6.5 GHz
  UWB_CHANNEL_9 = 9    // 8.0 GHz
};

enum DW3000DataRate : uint8_t {
  UWB_RATE_850K  = 0,
  UWB_RATE_6M8   = 1   // Recommended for ranging
};

enum DW3000PRFRate : uint8_t {
  UWB_PRF_16MHz  = 1,
  UWB_PRF_64MHz  = 2   // Better accuracy
};

enum DW3000PreambleLen : uint8_t {
  UWB_PLEN_64   = 0x04,
  UWB_PLEN_128  = 0x14,
  UWB_PLEN_1024 = 0x08
};

// ─── Radio configuration struct ──────────────────────────────────────────────

struct DW3000Config {
  DW3000Channel   channel    = UWB_CHANNEL_5;
  DW3000DataRate  dataRate   = UWB_RATE_6M8;
  DW3000PRFRate   prf        = UWB_PRF_64MHz;
  DW3000PreambleLen preamble = UWB_PLEN_128;
  uint8_t         preambleCode = 9;    // Preamble code (channel-dependent)
  uint16_t        panId      = 0xDECA;
  uint16_t        shortAddr  = 0x0001;
  float           antennaDelay = 16384.0; // in DW3000 ticks (calibrate per hardware)

  bool isValid() const {
    if (channel != UWB_CHANNEL_5 && channel != UWB_CHANNEL_9) return false;
    if (channel == UWB_CHANNEL_5 && preambleCode > 15) return false;
    // Channel 9 valid preamble codes: 9-12 (BPRF) and 17-24 (HPRF) per Qorvo SDK
    if (channel == UWB_CHANNEL_9 && !((preambleCode >= 9 && preambleCode <= 12) || (preambleCode >= 17 && preambleCode <= 24))) return false;
    return true;
  }
};

// ─── DW3000Driver main class ─────────────────────────────────────────────────

class DW3000Driver {
public:
  DW3000Driver(uint8_t csPin  = UWB_PIN_CS,
               uint8_t rstPin = UWB_PIN_RST,
               uint8_t irqPin = UWB_PIN_IRQ);

  // ── Lifecycle ────────────────────────────────────────────────────────────

  void begin();       // Initialize SPI bus and GPIO
  void hardReset();   // Hardware reset via RST pulse
  void softReset();   // Software reset via PMSC register

  // Configure chip from DW3000Config; returns true on success
  bool init(const DW3000Config& config = DW3000Config{});

  // Verify connection by reading Device ID
  bool checkConnection();

  // Update network address and antenna delay without reinitializing chip
  void configureAddresses(uint16_t panId, uint16_t shortAddr, float antennaDelay = 16384.0f);

  // ── Register access ───────────────────────────────────────────────────────

  uint32_t readReg32(uint8_t reg, uint16_t offset = 0);
  uint16_t readReg16(uint8_t reg, uint16_t offset = 0);
  uint8_t  readReg8 (uint8_t reg, uint16_t offset = 0);

  void     writeReg32(uint8_t reg, uint32_t val, uint16_t offset = 0);
  void     writeReg16(uint8_t reg, uint16_t val, uint16_t offset = 0);
  void     writeReg8 (uint8_t reg, uint8_t  val, uint16_t offset = 0);

  void     readBytes (uint8_t reg, uint16_t offset, uint8_t* buf, size_t len);
  void     writeBytes(uint8_t reg, uint16_t offset, const uint8_t* buf, size_t len);

  // ── TX / RX ───────────────────────────────────────────────────────────────

  void transmit(const uint8_t* data, size_t len);

  // Immediate TX then auto-RX (CMD_TX_W4R) — chip enters RX automatically after TX.
  // Do NOT call startReceive() afterwards.
  void transmitW4R(const uint8_t* data, size_t len);

  // Delayed TX: schedule transmission at txTime (40-bit DW3000 ticks).
  // txTime must be at least ~1 ms in the future.
  // Returns false if chip reports a timing error (HPDWARN).
  bool transmitAt(const uint8_t* data, size_t len, uint64_t txTime);

  void   startReceive(uint16_t timeoutMs = 0);  // Start RX, 0 = no timeout
  size_t readRxFrame(uint8_t* buf, size_t maxLen);

  // ── Status / IRQ ─────────────────────────────────────────────────────────

  uint32_t getSysStatus();
  bool isTxDone()    { return getSysStatus() & DW3000_SYS_STATUS_TXFRS; }
  bool isRxDone()    { return getSysStatus() & DW3000_SYS_STATUS_RXDFR; }
  bool isRxError()   { return getSysStatus() & (DW3000_SYS_STATUS_RXFCE | DW3000_SYS_STATUS_RXPHE); }
  bool isRxTimeout() { return getSysStatus() & (DW3000_SYS_STATUS_RXRFTO | DW3000_SYS_STATUS_RXPTO | DW3000_SYS_STATUS_RXSTO); }
  void clearSysStatus(uint32_t mask = 0xFFFFFFFF);

  // ── Timestamps ────────────────────────────────────────────────────────────

  uint64_t getTxTimestamp();   // TX timestamp (40-bit DW3000 ticks)
  uint64_t getRxTimestamp();   // RX timestamp (40-bit DW3000 ticks)
  uint64_t getSysTime();       // Current system time (40-bit)

  // ── Diagnostics ──────────────────────────────────────────────────────────

  float    getRxPower();       // Approximate RSSI of last received frame (dBm)
  uint32_t getDeviceId() { return readReg32(DW3000_REG_DEV_ID); }
  void     dumpStatus();       // Print status summary to Serial (debug)

private:
  uint8_t _csPin;
  uint8_t _rstPin;
  uint8_t _irqPin;
  DW3000Config _config;
  bool _initialized = false;

  // ── Low-level SPI ────────────────────────────────────────────────────────
  void _spiBeginTransaction(uint32_t speed = DW3000_SPI_SPEED_FAST);
  void _spiEndTransaction();
  void _spiWrite(uint8_t reg, uint16_t offset, bool write,
                 const uint8_t* wbuf, uint8_t* rbuf, size_t len);

  // Build DW3000 SPI header (reg + offset + r/w, per DW3000 UM §3.2)
  void _buildHeader(uint8_t reg, uint16_t offset, bool write,
                    uint8_t* header, uint8_t& headerLen);

  // ── Internal configuration ────────────────────────────────────────────────
  void _configureAGC();
  void _configureLDE();
  void _configureChannel(DW3000Channel ch, uint8_t preambleCode);
  void _configurePRF(DW3000PRFRate prf);
  void _configureDataRate(DW3000DataRate rate);
  void _configurePAN(uint16_t panId, uint16_t shortAddr);
  void _loadLDECode();
  void _initPLL();
};
