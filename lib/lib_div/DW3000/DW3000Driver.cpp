/*
 * DW3000Driver.cpp
 * Low-level HAL for the Qorvo DW3000 UWB chip
 *
 * License  : Apache 2.0 (Tasmota compatible)
 * References:
 *   - DW3000 User Manual (Qorvo)
 *   - Fhilb/DW3000_Arduino (reference for chip init sequence)
 *   - https://gist.github.com/egnor/455d510e11c22deafdec14b09da5bf54
 *
 * Note: DW3000_REG_* constants in the header use DW1000-style naming (legacy);
 * actual DW3000 register banks and offsets are defined locally here.
 */

#include "DW3000Driver.h"
#include <SPI.h>

// ─── DW3000 register banks (5-bit base address in SPI header) ────────────────
#define B_GEN   0x00   // GEN_CFG_AES_LOW  (SYS_STATUS, TX_FCTRL, TX timestamps…)
#define B_GENH  0x01   // GEN_CFG_AES_HIGH (CHAN_CTRL, EUI, PANADR…)
#define B_TUNE  0x03   // RX_TUNE / DGC LUT
#define B_SYNC  0x04   // EXT_SYNC / PGF calibration
#define B_DRX   0x06   // Digital RX (DTUNE, PAC)
#define B_RF    0x07   // RF_CONF (TX ctrl, LDO)
#define B_FS    0x09   // FS_CTRL (PLL, XTAL)
#define B_AON   0x0A   // Always-On
#define B_OTP   0x0B   // OTP interface
#define B_CIA   0x0C   // CIA — RX timestamps, signal quality
#define B_DIAG  0x0F   // DIG_DIAG / SYS_STATE
#define B_PMSC  0x11   // Power Management / clock control
#define B_RXBUF 0x12   // RX buffer 0
#define B_TXBUF 0x14   // TX buffer

// ─── Offsets in B_GEN (0x00) ──────────────────────────────────────────────────
#define O_DEV_ID    0x00
#define O_PANADR    0x0C
#define O_SYS_CFG   0x10
#define O_SYS_TIME  0x1C
#define O_TX_FCTRL  0x24
#define O_DX_TIME   0x2C
#define O_RX_FWTO   0x34
#define O_SYS_EN    0x3C
#define O_SYS_STAT  0x44   // SYS_STATUS (low 32 bits)
#define O_RX_FINFO  0x4C   // RX frame info (length…)
#define O_TX_TIME   0x74   // TX timestamp (5 bytes)

// ─── Offsets in B_GENH (0x01) ─────────────────────────────────────────────────
#define O_CHAN_CTRL  0x14

// ─── Additional offsets in B_GEN ──────────────────────────────────────────────
#define O_RX_TIME   0x64   // RX_TIME_0: RX SFD timestamp (5 bytes)

// ─── Offsets in B_CIA (0x0C) ──────────────────────────────────────────────────
#define O_CIA_DIAG  0x58   // PAC count (for RSSI)

// ─── Offsets in B_PMSC (0x11) ─────────────────────────────────────────────────
#define O_PMSC_SOFT 0x00
#define O_PMSC_CLK  0x04

// ─── SYS_STATUS bits for DW3000 (verified against Makerfabs SDK) ─────────────
#define STS_TXFRS   (1UL << 7)    // TX frame sent
#define STS_RXPHE   (1UL << 12)   // RX PHY header error
#define STS_RXDFR   (1UL << 13)   // RX data frame ready (RXFR)
#define STS_RXFCG   (1UL << 14)   // RX frame CRC good
#define STS_RXFCE   (1UL << 15)   // RX frame CRC error
#define STS_RXFTO   (1UL << 17)   // RX frame wait timeout
#define STS_RXRFTO  (1UL << 21)   // RX RF timeout
#define STS_RXPTO   (1UL << 22)   // RX preamble timeout
#define STS_SPIRDY  (1UL << 23)   // SPI ready
#define STS_RCINIT  (1UL << 24)   // RC init done

// Combined RX error mask
#define STS_RX_ERR  (STS_RXFCE | STS_RXPHE | 0x04270000UL)

// ─── Fast commands (1 byte: 0x80 | cmd<<1 | 0x01) ───────────────────────────
#define FCMD(c)     ((uint8_t)(0x81 | ((c) << 1)))
#define CMD_IDLE     0x00   // Force IDLE (cancel ongoing TX/RX) [TXRXOFF in Qorvo SDK]
#define CMD_TX       0x01   // Start immediate TX
#define CMD_RX       0x02   // Start immediate RX
#define CMD_DTX      0x03   // Deferred TX (via DX_TIME)
#define CMD_TX_W4R   0x0C   // Immediate TX then auto-RX (Wait-4-Response)
#define CMD_DTX_W4R  0x0D   // Deferred TX then auto-RX

// ─── Private internal helpers (not declared in header) ───────────────────────
static uint32_t _otpRead(DW3000Driver* d, uint8_t addr);

// ═════════════════════════════════════════════════════════════════════════════
// Constructor
// ═════════════════════════════════════════════════════════════════════════════

DW3000Driver::DW3000Driver(uint8_t csPin, uint8_t rstPin, uint8_t irqPin)
    : _csPin(csPin), _rstPin(rstPin), _irqPin(irqPin) {}

// ═════════════════════════════════════════════════════════════════════════════
// Lifecycle
// ═════════════════════════════════════════════════════════════════════════════

void DW3000Driver::begin() {
    pinMode(_csPin, OUTPUT);
    digitalWrite(_csPin, HIGH);
    pinMode(_rstPin, INPUT);    // floating = not in reset
    pinMode(_irqPin, INPUT);
    SPI.begin();
    delay(5);
}

void DW3000Driver::hardReset() {
    pinMode(_rstPin, OUTPUT);
    digitalWrite(_rstPin, LOW);
    delay(10);
    pinMode(_rstPin, INPUT);   // release — DW3000 pulls high internally
    delay(5);
}

void DW3000Driver::softReset() {
    // Clear AON config before reset
    writeReg16(B_AON, 0x0000, 0x00);
    writeReg8 (B_AON, 0x00,   0x14);
    writeReg8 (B_AON, 0x00,   0x04);
    writeReg16(B_AON, 0x0002, 0x04);
    delay(1);

    // Force clock to FAST_RC/4, reset, restore
    writeReg32(B_PMSC, 0x00000001UL, O_PMSC_CLK);
    writeReg16(B_PMSC, 0x0000, O_PMSC_SOFT);
    delay(20);
    writeReg16(B_PMSC, 0xFFFF, O_PMSC_SOFT);
    writeReg8 (B_PMSC, 0x00,   O_PMSC_CLK);
}

bool DW3000Driver::checkConnection() {
    uint32_t id = readReg32(B_GEN, O_DEV_ID);
    return (id == DW3000_DEVICE_ID || id == 0xDECA0312UL);
}

// ─── Internal IDLE wait ───────────────────────────────────────────────────────

static bool _waitIdle(DW3000Driver* d, uint32_t timeoutMs = 500) {
    uint32_t t0 = millis();
    while ((millis() - t0) < timeoutMs) {
        // PMSC in IDLE (SYS_STATE[17:16] == 0x3)
        uint32_t sysstate = d->readReg32(B_DIAG, 0x30);
        if (((sysstate >> 16) & 0x3) == 0x3) return true;
        // Or SPI_RDY + RCINIT in SYS_STATUS
        uint32_t stat = d->readReg32(B_GEN, O_SYS_STAT);
        if ((stat & (STS_SPIRDY | STS_RCINIT)) == (STS_SPIRDY | STS_RCINIT))
            return true;
    }
    return false;
}

// ─── OTP read ─────────────────────────────────────────────────────────────────

static uint32_t _otpRead(DW3000Driver* d, uint8_t addr) {
    d->writeReg8 (B_OTP, addr, 0x04);  // OTP_ADDR
    d->writeReg8 (B_OTP, 0x02, 0x08);  // OTP_CFG : bit OTP_READ
    return d->readReg32(B_OTP, 0x10);  // OTP_RDATA
}

// ═════════════════════════════════════════════════════════════════════════════
// init() — full DW3000 initialization sequence
// ═════════════════════════════════════════════════════════════════════════════

bool DW3000Driver::init(const DW3000Config& cfg) {
    if (!cfg.isValid()) return false;
    _config = cfg;

    hardReset();
    begin();

    if (!checkConnection()) return false;

    // Force INIT (SYS_CFG bit 4)
    uint32_t syscfg = readReg32(B_GEN, O_SYS_CFG);
    writeReg32(B_GEN, syscfg | (1UL << 4), O_SYS_CFG);

    if (!_waitIdle(this)) return false;

    softReset();
    delay(200);

    if (!_waitIdle(this)) return false;

    // ── OTP load ─────────────────────────────────────────────────────────────
    uint32_t ldo_low  = _otpRead(this, 0x04);
    uint32_t ldo_high = _otpRead(this, 0x05);
    uint32_t bias_raw = _otpRead(this, 0x0A);
    uint32_t bias_tune = (bias_raw >> 16) & 0x1F;

    if (ldo_low != 0 && ldo_high != 0 && bias_tune != 0) {
        writeReg8(B_PMSC, (uint8_t)bias_tune, 0x1F);  // BIAS_CTRL
        writeReg16(B_OTP, 0x0100, 0x08);               // LDO_KICK
    }

    uint32_t xtrim = _otpRead(this, 0x1E);
    if (xtrim == 0) xtrim = 0x2E;
    writeReg8(B_FS, (uint8_t)xtrim, 0x14);  // XTAL trim

    // ── System config ─────────────────────────────────────────────────────────
    // SYS_CFG: bit3=DIS_DRXB, bit7=CIA_IPATOV (required for RX_TIME_0 / IP_TOA),
    //          bit10=RXAUTR (auto re-arm RX after reception)
    // CIA_STS (bit8) must be 0 in non-STS mode — it was the original bug in 0x188.
    uint32_t usr_cfg = 0x00000488UL;
    writeReg32(B_GEN, usr_cfg, O_SYS_CFG);

    // Enable all interrupts
    writeReg32(B_GEN, 0xFFFFFFFFUL, O_SYS_EN);
    writeReg16(B_GEN, 0xFFFF, 0x40);

    // AON digital config: auto RX recalibration + GO2IDLE on wake
    writeReg32(B_AON, 0x000900UL, 0x00);

    // ── DGC (Dynamic Gain Control) — hardcoded values (OTP sometimes absent) ──
    writeReg32(B_TUNE, 0x10000240UL, 0x1C);  // DGC_CFG0
    writeReg32(B_TUNE, 0x1B6DA489UL, 0x20);  // DGC_CFG1
    if (cfg.channel == UWB_CHANNEL_9) {
        writeReg32(B_TUNE, 0x0002A8FEUL, 0x38);  // DGC_LUT_0 ch9
        writeReg32(B_TUNE, 0x0002AC36UL, 0x3C);  // DGC_LUT_1 ch9
        writeReg32(B_TUNE, 0x0002A5FEUL, 0x40);  // DGC_LUT_2 ch9
        writeReg32(B_TUNE, 0x0002AF3EUL, 0x44);  // DGC_LUT_3 ch9
        writeReg32(B_TUNE, 0x0002AF7DUL, 0x48);  // DGC_LUT_4 ch9
        writeReg32(B_TUNE, 0x0002AFB5UL, 0x4C);  // DGC_LUT_5 ch9
        writeReg32(B_TUNE, 0x0002AFB5UL, 0x50);  // DGC_LUT_6 ch9
    } else {
        writeReg32(B_TUNE, 0x0001C0FDUL, 0x38);  // DGC_LUT_0 ch5
        writeReg32(B_TUNE, 0x0001C43EUL, 0x3C);  // DGC_LUT_1 ch5
        writeReg32(B_TUNE, 0x0001C6BEUL, 0x40);  // DGC_LUT_2 ch5
        writeReg32(B_TUNE, 0x0001C77EUL, 0x44);  // DGC_LUT_3 ch5
        writeReg32(B_TUNE, 0x0001CF36UL, 0x48);  // DGC_LUT_4 ch5
        writeReg32(B_TUNE, 0x0001CFB5UL, 0x4C);  // DGC_LUT_5 ch5
        writeReg32(B_TUNE, 0x0001CFF5UL, 0x50);  // DGC_LUT_6 ch5
    }

    // THR_64 = 0x32 (egnor: "always change")
    writeReg16(B_TUNE, 0xE5E5, 0x18);

    // PAC size = 8 (recommended) + DTUNE0 clear DT0B4
    writeReg32(B_DRX, 0x00811018UL, 0x00);

    // OTP: STS_CFG (STS length = 64)
    writeReg16(B_OTP, 0x1400, 0x08);
    writeReg8 (B_GEN, 0x00, 0x29);    // clear STS_MODE

    // DTUNE3: recommended default value
    writeReg32(B_DRX, 0xAF5F584CUL, 0x0C);

    // ── Channel, preamble code, data rate ────────────────────────────────────
    uint32_t chan_ctrl = readReg32(B_GENH, O_CHAN_CTRL);
    chan_ctrl &= ~0x1FFFUL;
    chan_ctrl |= (cfg.channel == UWB_CHANNEL_9) ? 0x01UL : 0x00UL;
    chan_ctrl |= (uint32_t)(cfg.preambleCode & 0x1F) << 8;   // TX pcode
    chan_ctrl |= (uint32_t)(cfg.preambleCode & 0x1F) << 3;   // RX pcode
    // SFD type bits[2:1]: 0b01 = DW proprietary (non-STS); type 2 (4z) requires STS
    chan_ctrl |= (0x01UL << 1);
    writeReg32(B_GENH, chan_ctrl, O_CHAN_CTRL);

    // TX_FCTRL: preamble length (TXPSR) + data rate
    // TXPSR codes: 0x1=64, 0x2=128, 0x3=256, 0x4=512, 0x5=1024 symbols
    // UWB_PLEN_128=0x14 is a DRX register value, not a TXPSR code
    static const uint8_t txpsr_lut[] = {
        /* UWB_PLEN_64=0x04 → */ 1,
        /* UWB_PLEN_128=0x14 → */ 2,
        /* UWB_PLEN_1024=0x08 → */ 5
    };
    uint8_t txpsr = (cfg.preamble == UWB_PLEN_64)   ? 1 :
                    (cfg.preamble == UWB_PLEN_1024)  ? 5 : 2;
    uint32_t fctrl = readReg32(B_GEN, O_TX_FCTRL);
    fctrl &= ~0x000FC00UL;  // clear TXBR[11:10] and TXPSR[15:12]
    fctrl |= ((uint32_t)txpsr        << 12);  // TXPSR
    fctrl |= ((uint32_t)cfg.dataRate << 10);  // TXBR
    writeReg32(B_GEN, fctrl, O_TX_FCTRL);

    // SFD timeout = 0x8000 (large value, reduces RXSTO frequency with polling approach)
    writeReg16(B_DRX, 0x8000, 0x02);

    // ── RF config values documented by egnor ────────────────────────────────
    writeReg8 (B_RF, 0x14,       0x48);  // LDO_RLOAD
    writeReg8 (B_RF, 0x0E,       0x1A);  // RF_TX_CTRL_1
    if (cfg.channel == UWB_CHANNEL_9) {
        writeReg32(B_RF, 0x1C010034UL, 0x1C);  // RF_TX_CTRL_2 ch9
        writeReg32(B_RF, 0x08B5A833UL, 0x10);  // RF_RX_CTRL_HI ch9
        writeReg16(B_FS, 0x0F3C,       0x00);  // PLL_CFG ch9
    } else {
        writeReg32(B_RF, 0x1C071134UL, 0x1C);  // RF_TX_CTRL_2 ch5
        writeReg16(B_FS, 0x1F3C,       0x00);  // PLL_CFG ch5
    }
    writeReg8(B_FS, 0x81, 0x08);     // PLL_CAL config (PLL_CFG_LD = 0x8)

    // System clock in auto mode
    writeReg32(B_PMSC, 0x00B40200UL, 0x04);
    writeReg32(B_PMSC, 0x80030738UL, 0x08);

    // ── PLL lock ──────────────────────────────────────────────────────────────
    _initPLL();

    // ── DGC KICK via OTP ─────────────────────────────────────────────────────
    uint32_t otp_val = readReg32(B_OTP, 0x08);
    otp_val |= 0x40UL;
    if (cfg.channel == UWB_CHANNEL_9) otp_val |= 0x2000UL;
    writeReg32(B_OTP, otp_val, 0x08);

    // ── PGF calibration (receiver) ────────────────────────────────────────────
    writeReg8(B_TUNE, 0xF0, 0x19);  // RX_CTRL_LO reset

    uint32_t ldo_ctrl_save = readReg32(B_RF, 0x48);
    writeReg32(B_RF, 0x00000105UL, 0x48);  // Enable required LDOs

    writeReg32(B_SYNC, 0x00020000UL, 0x0C);  // RX_CAL: start calibration
    delay(5);
    writeReg32(B_SYNC, 0x00000011UL, 0x0C);  // Validate calibration

    // Wait for calibration to complete (max 1 s)
    uint32_t t0 = millis();
    while ((millis() - t0) < 1000) {
        if (readReg32(B_SYNC, 0x20) != 0) break;
        delay(5);
    }

    writeReg32(B_SYNC, 0x00000000UL, 0x0C);
    writeReg8 (B_SYNC, 0x01, 0x20);  // Clear calibration flag

    writeReg32(B_RF, ldo_ctrl_save, 0x48);  // Restore LDO_CTRL

    // Enable full CIA diagnostics (for signal strength) — CIA_CONF bit0
    writeReg8(B_CIA, 0x01, 0x02);

    // ── Antenna delay ─────────────────────────────────────────────────────────
    // Stored in EUI / TX antenna delay register (0x01:0x04)
    writeReg16(B_GENH, (uint16_t)cfg.antennaDelay, 0x04);

    // ── Network address ───────────────────────────────────────────────────────
    _configurePAN(cfg.panId, cfg.shortAddr);

    // Explicit IDLE + clear all residual status flags
    {
        uint8_t idle = FCMD(CMD_IDLE);
        _spiBeginTransaction(DW3000_SPI_SPEED_INIT);
        digitalWrite(_csPin, LOW);
        SPI.transfer(idle);
        digitalWrite(_csPin, HIGH);
        _spiEndTransaction();
    }
    delay(5);
    writeReg32(B_GEN, 0xFFFFFFFFUL, O_SYS_STAT);

    _initialized = true;
    return true;
}

// ═════════════════════════════════════════════════════════════════════════════
// Register access — public
// ═════════════════════════════════════════════════════════════════════════════

uint32_t DW3000Driver::readReg32(uint8_t reg, uint16_t offset) {
    uint8_t buf[4];
    readBytes(reg, offset, buf, 4);
    return (uint32_t)buf[0]
         | ((uint32_t)buf[1] << 8)
         | ((uint32_t)buf[2] << 16)
         | ((uint32_t)buf[3] << 24);
}

uint16_t DW3000Driver::readReg16(uint8_t reg, uint16_t offset) {
    uint8_t buf[2];
    readBytes(reg, offset, buf, 2);
    return (uint16_t)buf[0] | ((uint16_t)buf[1] << 8);
}

uint8_t DW3000Driver::readReg8(uint8_t reg, uint16_t offset) {
    uint8_t val;
    readBytes(reg, offset, &val, 1);
    return val;
}

void DW3000Driver::writeReg32(uint8_t reg, uint32_t val, uint16_t offset) {
    uint8_t buf[4] = {
        (uint8_t) val,
        (uint8_t)(val >> 8),
        (uint8_t)(val >> 16),
        (uint8_t)(val >> 24)
    };
    writeBytes(reg, offset, buf, 4);
}

void DW3000Driver::writeReg16(uint8_t reg, uint16_t val, uint16_t offset) {
    uint8_t buf[2] = { (uint8_t)val, (uint8_t)(val >> 8) };
    writeBytes(reg, offset, buf, 2);
}

void DW3000Driver::writeReg8(uint8_t reg, uint8_t val, uint16_t offset) {
    writeBytes(reg, offset, &val, 1);
}

void DW3000Driver::readBytes(uint8_t reg, uint16_t offset, uint8_t* buf, size_t len) {
    _spiWrite(reg, offset, false, nullptr, buf, len);
}

void DW3000Driver::writeBytes(uint8_t reg, uint16_t offset, const uint8_t* buf, size_t len) {
    _spiWrite(reg, offset, true, buf, nullptr, len);
}

// ═════════════════════════════════════════════════════════════════════════════
// TX / RX
// ═════════════════════════════════════════════════════════════════════════════

void DW3000Driver::transmit(const uint8_t* data, size_t len) {
    writeBytes(B_TXBUF, 0, data, len);

    // Update frame length in TX_FCTRL (+2 for FCS)
    uint32_t fctrl = readReg32(B_GEN, O_TX_FCTRL);
    fctrl = (fctrl & 0xFFFFFC00UL) | ((len + 2) & 0x3FFUL);
    writeReg32(B_GEN, fctrl, O_TX_FCTRL);

    clearSysStatus(STS_TXFRS);

    // Force IDLE — chip may be in RX (RXAUTR re-arms automatically after frame reception)
    {
        uint8_t idle = FCMD(CMD_IDLE);
        _spiBeginTransaction(DW3000_SPI_SPEED_FAST);
        digitalWrite(_csPin, LOW);
        SPI.transfer(idle);
        digitalWrite(_csPin, HIGH);
        _spiEndTransaction();
    }

    // Fast command TX
    uint8_t cmd = FCMD(CMD_TX);
    _spiBeginTransaction(DW3000_SPI_SPEED_FAST);
    digitalWrite(_csPin, LOW);
    SPI.transfer(cmd);
    digitalWrite(_csPin, HIGH);
    _spiEndTransaction();
}

void DW3000Driver::transmitW4R(const uint8_t* data, size_t len) {
    writeBytes(B_TXBUF, 0, data, len);

    uint32_t fctrl = readReg32(B_GEN, O_TX_FCTRL);
    fctrl = (fctrl & 0xFFFFFC00UL) | ((len + 2) & 0x3FFUL);
    writeReg32(B_GEN, fctrl, O_TX_FCTRL);

    clearSysStatus(STS_TXFRS);

    // Force IDLE first (cancel any auto-re-armed RX from RXAUTR)
    {
        uint8_t idle = FCMD(CMD_IDLE);
        _spiBeginTransaction(DW3000_SPI_SPEED_FAST);
        digitalWrite(_csPin, LOW);
        SPI.transfer(idle);
        digitalWrite(_csPin, HIGH);
        _spiEndTransaction();
    }

    // CMD_TX_W4R: TX then automatically enable RX (atomic, no software CMD_RX needed)
    uint8_t cmd = FCMD(CMD_TX_W4R);
    _spiBeginTransaction(DW3000_SPI_SPEED_FAST);
    digitalWrite(_csPin, LOW);
    SPI.transfer(cmd);
    digitalWrite(_csPin, HIGH);
    _spiEndTransaction();
}

bool DW3000Driver::transmitAt(const uint8_t* data, size_t len, uint64_t txTime) {
    writeBytes(B_TXBUF, 0, data, len);

    // Preserve TXPSR and TXBR bits; clear TXFLEN [9:0] AND TXB_OFFSET [25:16]
    uint32_t fctrl = readReg32(B_GEN, O_TX_FCTRL);
    fctrl &= 0xFC00FC00UL;  // clear TXFLEN[9:0] and TXB_OFFSET[25:16]
    fctrl |= ((len + 2) & 0x3FFUL);
    writeReg32(B_GEN, fctrl, O_TX_FCTRL);

    // DX_TIME[31:0] = bits[39:8] of the 40-bit target TX time
    uint32_t dx = (uint32_t)(txTime >> 8);
    writeReg32(B_GEN, dx, O_DX_TIME);

    clearSysStatus(STS_TXFRS);

    // Force IDLE before deferred TX
    {
        uint8_t idle = FCMD(CMD_IDLE);
        _spiBeginTransaction(DW3000_SPI_SPEED_FAST);
        digitalWrite(_csPin, LOW);
        SPI.transfer(idle);
        digitalWrite(_csPin, HIGH);
        _spiEndTransaction();
    }

    // CMD_DTX_W4R: deferred TX at DX_TIME, then auto-enable RX (for REPORT reception)
    uint8_t cmd = FCMD(CMD_DTX_W4R);
    _spiBeginTransaction(DW3000_SPI_SPEED_FAST);
    digitalWrite(_csPin, LOW);
    SPI.transfer(cmd);
    digitalWrite(_csPin, HIGH);
    _spiEndTransaction();

    // HPDWARN (bit 27) set if the planned TX time has already passed
    uint32_t status = readReg32(B_GEN, O_SYS_STAT);
    if (status & (1UL << 27)) {
        return false;  // TX time already passed — increase FINAL_TX_DELAY
    }
    return true;
}

void DW3000Driver::startReceive(uint16_t timeoutMs) {
    // Force IDLE before enabling RX (command ignored if chip is not in IDLE)
    {
        uint8_t idle = FCMD(CMD_IDLE);
        _spiBeginTransaction(DW3000_SPI_SPEED_FAST);
        digitalWrite(_csPin, LOW);
        SPI.transfer(idle);
        digitalWrite(_csPin, HIGH);
        _spiEndTransaction();
    }

    clearSysStatus(0xFFFFFFFFUL);

    if (timeoutMs > 0) {
        // DW3000: RX_FWTO units ~1.026 µs (512 × 2 ns)
        uint32_t ticks = (uint32_t)timeoutMs * 974UL;  // ≈ 1000 / 1.026
        writeReg32(B_GEN, ticks, O_RX_FWTO);
        // RXWTOE = SYS_CFG bit 9 (mask 0x200)
        uint32_t sc = readReg32(B_GEN, O_SYS_CFG);
        writeReg32(B_GEN, sc | (1UL << 9), O_SYS_CFG);
    } else {
        // Disable timeout
        uint32_t sc = readReg32(B_GEN, O_SYS_CFG);
        writeReg32(B_GEN, sc & ~(1UL << 9), O_SYS_CFG);
    }

    uint8_t cmd = FCMD(CMD_RX);
    _spiBeginTransaction(DW3000_SPI_SPEED_FAST);
    digitalWrite(_csPin, LOW);
    SPI.transfer(cmd);
    digitalWrite(_csPin, HIGH);
    _spiEndTransaction();
}

size_t DW3000Driver::readRxFrame(uint8_t* buf, size_t maxLen) {
    uint32_t finfo = readReg32(B_GEN, O_RX_FINFO);
    uint16_t rxLen = finfo & 0x3FF;
    if (rxLen < 2) return 0;
    rxLen -= 2;  // strip 2-byte FCS
    if (rxLen > maxLen) rxLen = maxLen;
    readBytes(B_RXBUF, 0, buf, rxLen);
    return rxLen;
}

// ═════════════════════════════════════════════════════════════════════════════
// Status / Timestamps
// ═════════════════════════════════════════════════════════════════════════════

uint32_t DW3000Driver::getSysStatus() {
    return readReg32(B_GEN, O_SYS_STAT);
}

void DW3000Driver::clearSysStatus(uint32_t mask) {
    writeReg32(B_GEN, mask, O_SYS_STAT);
}

uint64_t DW3000Driver::getTxTimestamp() {
    uint8_t buf[5];
    readBytes(B_GEN, O_TX_TIME, buf, 5);
    return (uint64_t)buf[0]
         | ((uint64_t)buf[1] << 8)
         | ((uint64_t)buf[2] << 16)
         | ((uint64_t)buf[3] << 24)
         | ((uint64_t)buf[4] << 32);
}

uint64_t DW3000Driver::getRxTimestamp() {
    uint8_t buf[5] = {0};
    readBytes(B_GEN, O_RX_TIME, buf, 5);
    return (uint64_t)buf[0]
         | ((uint64_t)buf[1] << 8)
         | ((uint64_t)buf[2] << 16)
         | ((uint64_t)buf[3] << 24)
         | ((uint64_t)buf[4] << 32);
}

uint64_t DW3000Driver::getSysTime() {
    uint8_t buf[4];
    readBytes(B_GEN, O_SYS_TIME, buf, 4);
    return (uint64_t)buf[0]
         | ((uint64_t)buf[1] << 8)
         | ((uint64_t)buf[2] << 16)
         | ((uint64_t)buf[3] << 24);
}

// ═════════════════════════════════════════════════════════════════════════════
// Diagnostic
// ═════════════════════════════════════════════════════════════════════════════

float DW3000Driver::getRxPower() {
    // Formula per DW3000 User Manual §4.7.2 (PRF 64 MHz)
    uint32_t cir  = readReg32(B_CIA, 0x2C) & 0x1FF;
    uint32_t pac  = readReg32(B_CIA, O_CIA_DIAG) & 0xFFF;
    uint32_t dgc  = (readReg32(B_TUNE, 0x60) >> 28) & 0x7;
    if (pac == 0) return -100.0f;
    double rssi = 10.0 * log10(((double)cir * (1UL << 21)) / ((double)pac * (double)pac))
                + 6.0 * dgc - 121.7;
    return (float)rssi;
}

void DW3000Driver::dumpStatus() {
    uint32_t stat = getSysStatus();
    uint32_t id   = getDeviceId();
    Serial.print("[DW3000] DevID=0x"); Serial.print(id, HEX);
    Serial.print(" SYS_STATUS=0x"); Serial.println(stat, HEX);
    Serial.print("  TX_DONE=");    Serial.println(isTxDone());
    Serial.print("  RX_DONE=");    Serial.println(isRxDone());
    Serial.print("  RX_ERR=");     Serial.println(isRxError());
    Serial.print("  RX_TIMEOUT="); Serial.println(isRxTimeout());
}

// ═════════════════════════════════════════════════════════════════════════════
// Low-level SPI — private
// ═════════════════════════════════════════════════════════════════════════════

/*
 * DW3000 SPI header (1 or 2 bytes), per DW3000 User Manual §3.2:
 *
 * Byte 0: [W/R | EA | A4 | A3 | A2 | A1 | A0 | 0]
 *   W/R = bit7: 1=write, 0=read
 *   EA  = bit6: 1=sub-address follows, 0=zero offset
 *   A[4:0] = base address (5 bits) in bits[5:1]
 *
 * Byte 1 (if EA=1): [(S6 | S5 | S4 | S3 | S2 | S1 | S0) << 2 | mode]
 *   S[6:0] = offset (7 bits) in bits[8:2] of the combined word
 *   mode[1:0] = 00 (no further extension)
 */
void DW3000Driver::_buildHeader(uint8_t reg, uint16_t offset, bool write,
                                 uint8_t* header, uint8_t& headerLen) {
    uint32_t hdr = 0;
    if (write) hdr |= 0x80;
    hdr |= ((uint32_t)(reg & 0x1F) << 1);

    if (offset == 0) {
        header[0] = (uint8_t)hdr;
        headerLen = 1;
    } else {
        hdr |= 0x40;   // EA: sub-address follows
        hdr <<= 8;
        hdr |= (uint32_t)((offset & 0x7F) << 2);  // S[6:0] dans bits[8:2]
        header[0] = (uint8_t)(hdr >> 8);
        header[1] = (uint8_t)(hdr & 0xFF);
        headerLen = 2;
    }
}

void DW3000Driver::_spiBeginTransaction(uint32_t speed) {
    SPI.beginTransaction(SPISettings(speed, MSBFIRST, SPI_MODE0));
}

void DW3000Driver::_spiEndTransaction() {
    SPI.endTransaction();
}

void DW3000Driver::_spiWrite(uint8_t reg, uint16_t offset, bool write,
                              const uint8_t* wbuf, uint8_t* rbuf, size_t len) {
    uint8_t header[2];
    uint8_t hlen;
    _buildHeader(reg, offset, write, header, hlen);

    uint32_t speed = _initialized ? DW3000_SPI_SPEED_FAST : DW3000_SPI_SPEED_INIT;
    _spiBeginTransaction(speed);
    digitalWrite(_csPin, LOW);
    for (uint8_t i = 0; i < hlen; i++) SPI.transfer(header[i]);
    for (size_t i = 0; i < len; i++) {
        uint8_t out = wbuf ? wbuf[i] : 0x00;
        uint8_t in  = SPI.transfer(out);
        if (rbuf) rbuf[i] = in;
    }
    digitalWrite(_csPin, HIGH);
    _spiEndTransaction();
}

// ═════════════════════════════════════════════════════════════════════════════
// Internal configuration helpers
// ═════════════════════════════════════════════════════════════════════════════

void DW3000Driver::_initPLL() {
    // Clear CP_LOCK flag before polling (write 1 to clear bit 1 of SYS_STATUS)
    writeReg32(B_GEN, 0x02UL, O_SYS_STAT);

    // Poll CP_LOCK (bit 1 of SYS_STATUS) until PLL locks, max 2 s
    // AINIT2IDLE was already set in SEQ_CTRL by the preceding writeReg32(B_PMSC, 0x80030738, 0x08)
    uint32_t t0 = millis();
    while ((millis() - t0) < 2000) {
        if (readReg32(B_GEN, O_SYS_STAT) & 0x02UL) return;
        delay(1);
    }
    // Timeout: PLL did not lock — TX/RX will not work
}

void DW3000Driver::_loadLDECode() {
    // DW3000: LDE loaded automatically via OTP at startup
    // (no manual loading required, unlike DW1000)
}

void DW3000Driver::_configureAGC() {
    // Handled in init() via hardcoded DGC values
}

void DW3000Driver::_configureLDE() {
    // Handled in init() via OTP kick
}

void DW3000Driver::_configureChannel(DW3000Channel ch, uint8_t preambleCode) {
    // Handled in init()
}

void DW3000Driver::_configurePRF(DW3000PRFRate prf) {
    // Handled in init() via TX_FCTRL
}

void DW3000Driver::_configureDataRate(DW3000DataRate rate) {
    // Handled in init() via TX_FCTRL
}

void DW3000Driver::_configurePAN(uint16_t panId, uint16_t shortAddr) {
    // PANADR: short address [31:16], PAN ID [15:0]
    uint32_t panadr = ((uint32_t)shortAddr << 16) | panId;
    writeReg32(B_GEN, panadr, O_PANADR);
}

void DW3000Driver::configureAddresses(uint16_t panId, uint16_t shortAddr, float antennaDelay) {
    _configurePAN(panId, shortAddr);
    writeReg16(B_GENH, (uint16_t)antennaDelay, 0x04);
}
