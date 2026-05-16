/*
 * DW3000Ranging.cpp
 * DS-TWR / SS-TWR state machine for DW3000
 *
 * DS-TWR (3 exchanges):
 *   Tag  → Poll     → Anchor
 *   Tag  ← Response ← Anchor
 *   Tag  → Final    → Anchor  (carries tPollTx, tRespRx, tFinalTx via deferred TX)
 *   Tag  ← Report   ← Anchor  (carries computed distance)
 *
 * All ToF computation is done on the Anchor side; Tag only receives the result.
 *
 * SS-TWR (1 exchange):
 *   Tag  → Poll     → Anchor
 *   Tag  ← Response ← Anchor  (carries tRespTx, tPollRx for computation)
 *   (computation on Tag side)
 *
 * License  : Apache 2.0 (Tasmota compatible)
 * Reference: Fhilb/DW3000_Arduino (inspiration for DS-TWR structure)
 */

#include "DW3000Ranging.h"
#include <string.h>
#include <math.h>

// ─── Internal constants ───────────────────────────────────────────────────────

#define MASK40          0xFFFFFFFFFFULL
// FINAL TX delay: FINAL_TX_DELAY_MS (defined in .h) converted to DW3000 ticks.
// DW3000 fires the TX in hardware at the exact scheduled time.
// FINAL_TX_DELAY_MS must exceed the max Tasmota loop gap (~50 ms).
// 63898 ticks/µs × 1000 to convert ms to µs.
#define FINAL_TX_DELAY  (FINAL_TX_DELAY_MS * 1000ULL * 63898ULL)
// Sequence number encoded as 1 byte in all frames
static uint8_t s_seqNum = 0;

// ─── Frame format (common header, little-endian) ──────────────────────────────
// Byte 0  : type (UWB_MSG_*)
// Byte 1  : seqNum
// Byte 2-3: dst (little-endian)
// Byte 4-5: src (little-endian)
#define FRAME_HDR_LEN   6
#define FRAME_MAX_LEN   64

// Byte offsets in Final frame (21 bytes) and SS-TWR Response (16 bytes)
#define FINAL_OFF_TPOLLTX   6    // 5 bytes
#define FINAL_OFF_TRESRX   11    // 5 bytes
#define FINAL_OFF_TFINALTX 16    // 5 bytes

#define RESP_SS_OFF_TRESPTX  6   // 5 bytes (SS-TWR: tRespTx in Report)
#define RESP_SS_OFF_TPOLLRX 11   // 5 bytes

// ─── 40-bit helpers ───────────────────────────────────────────────────────────

static inline uint64_t ts40(uint64_t t)              { return t & MASK40; }
static inline uint64_t tsdiff(uint64_t a, uint64_t b) { return ts40(a - b); }

static void put40(uint8_t* buf, uint64_t val) {
    buf[0] = (uint8_t)(val);
    buf[1] = (uint8_t)(val >> 8);
    buf[2] = (uint8_t)(val >> 16);
    buf[3] = (uint8_t)(val >> 24);
    buf[4] = (uint8_t)(val >> 32);
}

static uint64_t get40(const uint8_t* buf) {
    return (uint64_t)buf[0]
         | ((uint64_t)buf[1] << 8)
         | ((uint64_t)buf[2] << 16)
         | ((uint64_t)buf[3] << 24)
         | ((uint64_t)buf[4] << 32);
}

// ─── Constructor ─────────────────────────────────────────────────────────────

DW3000Ranging::DW3000Ranging(DW3000Driver& driver) : _drv(driver) {}

// ─── Lifecycle ────────────────────────────────────────────────────────────────

bool DW3000Ranging::begin() {
    // Driver is already initialized (called after DW3000Driver::init()).
    // Only update the network address without reinitializing the chip.
    if (!_drv.checkConnection()) return false;
    _drv.configureAddresses(0xDECA, _myAddr, _antennaDelay);

    _state        = UWB_STATE_IDLE;
    _resultReady  = false;
    _rangingCount = 0;
    _errorCount   = 0;
    _lastRangingMs = 0;

    if (_role == UWB_ROLE_ANCHOR) {
        _enterState(UWB_STATE_LISTEN);
        _drv.startReceive(0);
    }
    return true;
}

void DW3000Ranging::loop() {
    switch (_state) {
    case UWB_STATE_IDLE:
        if (_role == UWB_ROLE_TAG) {
            if (millis() - _lastRangingMs >= _intervalMs)
                _enterState(UWB_STATE_SEND_POLL);
        }
        break;
    case UWB_STATE_SEND_POLL:      _stepSendPoll();      break;
    case UWB_STATE_WAIT_RESPONSE:  _stepWaitResponse();  break;
    case UWB_STATE_SEND_FINAL:     _stepSendFinal();     break;
    case UWB_STATE_WAIT_REPORT:    _stepWaitReport();    break;
    case UWB_STATE_DONE:
        _lastRangingMs = millis();
        if (_anchorCount > 1)
            _currentAnchorIdx = (_currentAnchorIdx + 1) % _anchorCount;
        _enterState(UWB_STATE_IDLE);
        break;
    case UWB_STATE_LISTEN:         _stepListen();        break;
    case UWB_STATE_SEND_RESPONSE:  _stepSendResponse();  break;
    case UWB_STATE_WAIT_FINAL:     _stepWaitFinal();     break;
    case UWB_STATE_SEND_REPORT:    _stepSendReport();    break;
    case UWB_STATE_ERROR:
        _errorCount++;
        if (_role == UWB_ROLE_TAG) {
            _lastRangingMs = millis();
            if (_anchorCount > 1)
                _currentAnchorIdx = (_currentAnchorIdx + 1) % _anchorCount;
            _enterState(UWB_STATE_IDLE);
        } else {
            _enterState(UWB_STATE_LISTEN);
            _drv.startReceive(0);
        }
        break;
    default:
        break;
    }
}

bool DW3000Ranging::getResult(UWBRangingResult& result) {
    if (!_resultReady) return false;
    result = _lastResult;
    _resultReady = false;
    return true;
}

void DW3000Ranging::reset() {
    _state              = UWB_STATE_IDLE;
    _resultReady        = false;
    _finalTxScheduled   = false;
    _currentAnchorIdx   = 0;
    _drv.clearSysStatus();
    if (_role == UWB_ROLE_ANCHOR) {
        _enterState(UWB_STATE_LISTEN);
        _drv.startReceive(0);
    }
}

void DW3000Ranging::addAnchorAddress(uint16_t addr) {
    if (_anchorCount < UWB_MAX_ANCHORS)
        _anchorAddrs[_anchorCount++] = addr;
}

// ─── TAG state machine ───────────────────────────────────────────────────────

void DW3000Ranging::_stepSendPoll() {
    uint8_t buf[FRAME_MAX_LEN];
    size_t  len;
    _buildPollFrame(buf, len);
    // CMD_TX_W4R: atomic TX then auto-start RX
    _drv.transmitW4R(buf, len);

    uint32_t t0 = millis();
    while (!_drv.isTxDone()) {
        if (millis() - t0 > 10) {
            _enterState(UWB_STATE_ERROR); return;
        }
    }
    _tPollTx = _drv.getTxTimestamp();
    // Only clear TX bits — chip is already in RX via CMD_TX_W4R
    _drv.clearSysStatus(DW3000_SYS_STATUS_TXFRS);

    _enterState(UWB_STATE_WAIT_RESPONSE);
}

void DW3000Ranging::_stepWaitResponse() {
    uint32_t stat = _drv.getSysStatus();
    bool rxerr = stat & (DW3000_SYS_STATUS_RXFCE | DW3000_SYS_STATUS_RXPHE);
    bool rxto  = stat & (DW3000_SYS_STATUS_RXRFTO | DW3000_SYS_STATUS_RXPTO | DW3000_SYS_STATUS_RXSTO);
    bool rxdone = stat & DW3000_SYS_STATUS_RXDFR;
    bool fwto   = stat & (1UL << 17);  // RXFTO (frame wait timeout)

    if (rxerr || rxto) {
        _enterState(UWB_STATE_ERROR); return;
    }
    if (!rxdone) {
        if (fwto) {
            _drv.clearSysStatus();
            _enterState(UWB_STATE_ERROR); return;
        }
        if (_isTimeout(UWB_TIMEOUT_RESPONSE_MS + 5))
            _enterState(UWB_STATE_ERROR);
        return;
    }

    uint8_t buf[FRAME_MAX_LEN];
    size_t  len = _drv.readRxFrame(buf, sizeof(buf));
    _tRespRx = _drv.getRxTimestamp();
    _drv.clearSysStatus();

    if (!_useDS) {
        // SS-TWR: Response contains tRespTx and tPollRx
        uint64_t tRespTx_anc, tPollRx_anc;
        if (!_parseResponseFrame(buf, len)) { _enterState(UWB_STATE_ERROR); return; }
        tRespTx_anc = get40(buf + RESP_SS_OFF_TRESPTX);
        tPollRx_anc = get40(buf + RESP_SS_OFF_TPOLLRX);
        float dist = _computeDistanceSS(_tPollTx, _tRespRx, tRespTx_anc, tPollRx_anc);
        _lastResult.distanceCm = dist;
        _lastResult.valid      = (dist >= 0.0f && dist < 10000.0f);
        _lastResult.anchorId   = _currentAnchorIdx;
        _lastResult.timestamp  = millis();
        _lastResult.rxPowerDbm = _drv.getRxPower();
        strncpy(_lastResult.method, "SS-TWR", sizeof(_lastResult.method));
        _resultReady = true;
        _rangingCount++;
        _enterState(UWB_STATE_DONE);
        return;
    }

    if (!_parseResponseFrame(buf, len)) { _enterState(UWB_STATE_ERROR); return; }
    _enterState(UWB_STATE_SEND_FINAL);
}

void DW3000Ranging::_stepSendFinal() {
    // Non-blocking: first call schedules the delayed TX, subsequent calls
    // poll TX done. Avoids blocking the Tasmota loop for FINAL_TX_DELAY_MS.
    if (!_finalTxScheduled) {
        uint64_t tFinalTx_sched = ts40(_tRespRx + FINAL_TX_DELAY);
        // Predict the TX_TIME register value: DW3000 reports (DX_TIME<<8) + antenna_delay.
        // The lower 8 bits of tFinalTx_sched are lost when written to DX_TIME (32-bit register).
        uint64_t tFinalTx_frame = (tFinalTx_sched & ~0xFFULL) + (uint64_t)_antennaDelay;
        uint8_t buf[FRAME_MAX_LEN];
        size_t  len;
        _buildFinalFrame(buf, len, _tPollTx, _tRespRx, tFinalTx_frame);
        if (!_drv.transmitAt(buf, len, tFinalTx_sched)) {
            _enterState(UWB_STATE_ERROR); return;
        }
        _finalTxScheduled = true;
        return;  // return on next FUNC_LOOP call
    }

    // TX scheduled — wait for TX done (non-blocking)
    if (_drv.isTxDone()) {
        _finalTxScheduled = false;
        _drv.clearSysStatus(DW3000_SYS_STATUS_TXFRS);
        _enterState(UWB_STATE_WAIT_REPORT);
        return;
    }
    // Timeout: FINAL_TX_DELAY_MS + 200 ms margin
    if (_isTimeout(FINAL_TX_DELAY_MS + 200)) {
        _finalTxScheduled = false;
        _enterState(UWB_STATE_ERROR);
    }
}

void DW3000Ranging::_stepWaitReport() {
    if (_drv.isRxError() || _drv.isRxTimeout()) {
        _enterState(UWB_STATE_ERROR); return;
    }
    if (!_drv.isRxDone()) {
        if (_isTimeout(UWB_TIMEOUT_REPORT_MS + 5))
            _enterState(UWB_STATE_ERROR);
        return;
    }

    uint8_t buf[FRAME_MAX_LEN];
    size_t  len = _drv.readRxFrame(buf, sizeof(buf));
    _drv.clearSysStatus();

    float dist;
    if (!_parseReportFrame(buf, len, dist)) { _enterState(UWB_STATE_ERROR); return; }

    _lastResult.distanceCm = dist;
    _lastResult.valid      = (dist >= 0.0f && dist < 10000.0f);
    _lastResult.anchorId   = _currentAnchorIdx;
    _lastResult.timestamp  = millis();
    _lastResult.rxPowerDbm = _drv.getRxPower();
    strncpy(_lastResult.method, "DS-TWR", sizeof(_lastResult.method));
    _resultReady = true;
    _rangingCount++;
    _enterState(UWB_STATE_DONE);
}

// ─── ANCHOR state machine ────────────────────────────────────────────────────

void DW3000Ranging::_stepListen() {
    uint32_t stat = _drv.getSysStatus();
    bool rxdone = stat & DW3000_SYS_STATUS_RXDFR;
    bool rxerr  = stat & (DW3000_SYS_STATUS_RXFCE | DW3000_SYS_STATUS_RXPHE);
    bool rxsto  = stat & DW3000_SYS_STATUS_RXSTO;
    bool rxrfto = stat & (DW3000_SYS_STATUS_RXRFTO | DW3000_SYS_STATUS_RXPTO);

    // RXSTO: DW3000 RXAUTR does NOT re-arm on RXSTO — must restart manually
    if (rxsto && !rxdone && !rxerr) {
        _drv.clearSysStatus(DW3000_SYS_STATUS_RXSTO);
        _drv.startReceive(0);
        return;
    }
    if (rxerr || rxrfto) {
        _drv.clearSysStatus();
        _drv.startReceive(0);
        return;
    }
    if (!rxdone) {
        // Re-arm RX periodically: RXSTO goes to IDLE, RXAUTR does NOT fire on RXSTO.
        // If RXSTO was cleared before we saw it, chip is silently IDLE.
        static uint32_t s_rearmMs = 0;
        uint32_t now = millis();
        if (now - s_rearmMs > 80) {
            s_rearmMs = now;
            _drv.startReceive(0);
        }
        return;
    }

    uint8_t buf[FRAME_MAX_LEN];
    size_t  len = _drv.readRxFrame(buf, sizeof(buf));
    _tPollRx = _drv.getRxTimestamp();
    _drv.clearSysStatus();

    if (len < FRAME_HDR_LEN || !_parsePollFrame(buf, len)) {
        _drv.startReceive(0);
        return;
    }
    _enterState(UWB_STATE_SEND_RESPONSE);
}

void DW3000Ranging::_stepSendResponse() {
    uint8_t buf[FRAME_MAX_LEN];
    size_t  len;
    _buildResponseFrame(buf, len);

    // CMD_TX_W4R: TX then chip auto-enables RX — no separate startReceive() needed
    _drv.transmitW4R(buf, len);

    uint32_t t0 = millis();
    while (!_drv.isTxDone()) {
        if (millis() - t0 > 10) {
            _enterState(UWB_STATE_ERROR); return;
        }
    }
    _tRespTx = _drv.getTxTimestamp();
    // Clear only TX bits — chip is already in RX, do NOT call startReceive()
    _drv.clearSysStatus(DW3000_SYS_STATUS_TXFRS);

    _enterState(UWB_STATE_WAIT_FINAL);
}

void DW3000Ranging::_stepWaitFinal() {
    // Non-blocking: status checked on each FUNC_LOOP call.
    // FINAL_TX_DELAY = 300 ms → FINAL arrives ~300 ms after the response.
    // Global timeout: 600 ms.
    uint32_t stat = _drv.getSysStatus();
    bool rxdone = stat & DW3000_SYS_STATUS_RXDFR;
    bool rxerr  = stat & (DW3000_SYS_STATUS_RXFCE | DW3000_SYS_STATUS_RXPHE);
    bool rxsto  = stat & DW3000_SYS_STATUS_RXSTO;
    // RXSTO excluded from rxto — sticky bit, handled separately
    bool rxto   = stat & (DW3000_SYS_STATUS_RXRFTO | DW3000_SYS_STATUS_RXPTO | (1UL << 17));

    // RXSTO: sticky bit — always clear and re-arm (RXAUTR does not)
    if (rxsto) {
        _drv.clearSysStatus(DW3000_SYS_STATUS_RXSTO);
        _drv.startReceive(0);
    }

    // rxdone takes priority: process FINAL even if RXSTO was also set
    if (rxdone && !rxerr) {
        uint8_t buf[FRAME_MAX_LEN];
        size_t  len = _drv.readRxFrame(buf, sizeof(buf));
        _tFinalRx = _drv.getRxTimestamp();
        _drv.clearSysStatus();

        uint64_t tPollTx_tag, tRespRx_tag, tFinalTx_tag;
        if (!_parseFinalFrame(buf, len, tPollTx_tag, tRespRx_tag, tFinalTx_tag)) {
            _enterState(UWB_STATE_LISTEN);
            _drv.startReceive(0);
            return;
        }
        _tPollTx  = tPollTx_tag;
        _tRespRx  = tRespRx_tag;
        _tFinalTx = tFinalTx_tag;
        _enterState(UWB_STATE_SEND_REPORT);
        return;
    }
    if (rxerr) {
        // CRC error — RXAUTR re-arms automatically, but clear anyway
        _drv.clearSysStatus();
        _drv.startReceive(0);
        return;
    }
    if (rxto) {
        // Preamble/frame timeout: RXAUTR does not re-arm on RXPTO/RXRFTO.
        // If global timeout has not expired, re-arm RX and wait for FINAL.
        _drv.clearSysStatus();
        if (!_isTimeout(600)) {
            _drv.startReceive(0);  // re-arm, stay in WAIT_FINAL
            return;
        }
        _enterState(UWB_STATE_LISTEN);
        _drv.startReceive(0);
        return;
    }
    if (_isTimeout(600)) {
        _drv.clearSysStatus();
        _enterState(UWB_STATE_LISTEN);
        _drv.startReceive(0);
    }
}

void DW3000Ranging::_stepSendReport() {
    float dist = _computeDistanceDS(_tPollTx, _tPollRx,
                                    _tRespTx, _tRespRx,
                                    _tFinalTx, _tFinalRx);

    _lastResult.distanceCm = dist;
    _lastResult.valid      = (dist >= 0.0f && dist < 10000.0f);
    _lastResult.anchorId   = 0;
    _lastResult.timestamp  = millis();
    _lastResult.rxPowerDbm = _drv.getRxPower();
    strncpy(_lastResult.method, "DS-TWR", sizeof(_lastResult.method));
    _resultReady = true;
    _rangingCount++;

    uint8_t buf[FRAME_MAX_LEN];
    size_t  len;
    _buildReportFrame(buf, len, dist);
    _drv.transmit(buf, len);

    uint32_t t0 = millis();
    while (!_drv.isTxDone()) {
        if (millis() - t0 > 10) break;
    }
    _drv.clearSysStatus();

    _enterState(UWB_STATE_LISTEN);
    _drv.startReceive(0);
}

// ─── Distance computation ────────────────────────────────────────────────────

float DW3000Ranging::_computeDistanceDS(uint64_t tPollTx, uint64_t tPollRx,
                                         uint64_t tRespTx, uint64_t tRespRx,
                                         uint64_t tFinalTx, uint64_t tFinalRx) {
    // All time intervals are positive modulo 2^40
    double tRound1 = (double)tsdiff(tRespRx,  tPollTx);
    double tReply1 = (double)tsdiff(tRespTx,  tPollRx);
    double tRound2 = (double)tsdiff(tFinalRx, tRespTx);
    double tReply2 = (double)tsdiff(tFinalTx, tRespRx);

    double denom = tRound1 + tRound2 + tReply1 + tReply2;
    if (denom < 1.0) return -1.0f;

    double tof = (tRound1 * tRound2 - tReply1 * tReply2) / denom;
    return _ticksToCm(tof);
}

float DW3000Ranging::_computeDistanceSS(uint64_t tPollTx, uint64_t tRespRx,
                                         uint64_t tRespTx, uint64_t tPollRx) {
    double tRound = (double)tsdiff(tRespRx, tPollTx);
    double tReply = (double)tsdiff(tRespTx, tPollRx);
    double tof    = (tRound - tReply) / 2.0;
    return _ticksToCm(tof);
}

float DW3000Ranging::_ticksToCm(double ticks) {
    if (!(ticks > 0.0)) return -1.0f;  // catches NaN, inf, negative, zero
    float result = (float)(ticks * DW3000_TIME_UNIT_PS * 1e-12 * SPEED_OF_LIGHT_CM_S);
    if (!(result > 0.0f)) return -1.0f;  // catches NaN/inf in result
    return result;
}

// ─── Frame builders ──────────────────────────────────────────────────────────

void DW3000Ranging::_buildPollFrame(uint8_t* buf, size_t& len) {
    uint16_t ancAddr = _anchorAddrs[_currentAnchorIdx];
    buf[0] = UWB_MSG_POLL;
    buf[1] = ++s_seqNum;
    buf[2] = (uint8_t)(ancAddr);
    buf[3] = (uint8_t)(ancAddr >> 8);
    buf[4] = (uint8_t)(_myAddr);
    buf[5] = (uint8_t)(_myAddr >> 8);
    len = FRAME_HDR_LEN;
}

void DW3000Ranging::_buildResponseFrame(uint8_t* buf, size_t& len) {
    buf[0] = UWB_MSG_RESPONSE;
    buf[1] = s_seqNum;
    buf[2] = (uint8_t)(_tagAddr);  // dst = tag
    buf[3] = (uint8_t)(_tagAddr >> 8);
    buf[4] = (uint8_t)(_myAddr);   // src = anchor
    buf[5] = (uint8_t)(_myAddr >> 8);

    if (!_useDS) {
        // SS-TWR: include tRespTx (placeholder 0 — read after TX) and tPollRx.
        // The true tRespTx is not known before transmission.
        // SS-TWR approach: anchor sends tPollRx; tag computes using tRespRx.
        // tRespTx is not needed with the asymmetric formula.
        // Send 0 as placeholder for parser consistency.
        put40(buf + RESP_SS_OFF_TRESPTX, 0);    // placeholder tRespTx
        put40(buf + RESP_SS_OFF_TPOLLRX, _tPollRx);
        len = RESP_SS_OFF_TPOLLRX + 5;
    } else {
        len = FRAME_HDR_LEN;
    }
}

void DW3000Ranging::_buildFinalFrame(uint8_t* buf, size_t& len,
                                      uint64_t tPollTx, uint64_t tRespRx, uint64_t tFinalTx) {
    uint16_t ancAddr = _anchorAddrs[_currentAnchorIdx];
    buf[0] = UWB_MSG_FINAL;
    buf[1] = s_seqNum;
    buf[2] = (uint8_t)(ancAddr);
    buf[3] = (uint8_t)(ancAddr >> 8);
    buf[4] = (uint8_t)(_myAddr);
    buf[5] = (uint8_t)(_myAddr >> 8);
    put40(buf + FINAL_OFF_TPOLLTX,  tPollTx);
    put40(buf + FINAL_OFF_TRESRX,   tRespRx);
    put40(buf + FINAL_OFF_TFINALTX, tFinalTx);
    len = FINAL_OFF_TFINALTX + 5;   // 21 bytes
}

void DW3000Ranging::_buildReportFrame(uint8_t* buf, size_t& len, float distanceCm) {
    buf[0] = UWB_MSG_REPORT;
    buf[1] = s_seqNum;
    buf[2] = (uint8_t)(_tagAddr);  // dst = tag
    buf[3] = (uint8_t)(_tagAddr >> 8);
    buf[4] = (uint8_t)(_myAddr);   // src = anchor
    buf[5] = (uint8_t)(_myAddr >> 8);
    memcpy(buf + FRAME_HDR_LEN, &distanceCm, 4);
    len = FRAME_HDR_LEN + 4;
}

// ─── Frame parsers ────────────────────────────────────────────────────────────

bool DW3000Ranging::_parsePollFrame(const uint8_t* buf, size_t len) {
    if (len < FRAME_HDR_LEN || buf[0] != UWB_MSG_POLL) return false;
    uint16_t dst = (uint16_t)buf[2] | ((uint16_t)buf[3] << 8);
    if (dst != _myAddr) return false;
    _tagAddr = (uint16_t)buf[4] | ((uint16_t)buf[5] << 8);
    return true;
}

bool DW3000Ranging::_parseResponseFrame(const uint8_t* buf, size_t len) {
    return (len >= FRAME_HDR_LEN && buf[0] == UWB_MSG_RESPONSE);
}

bool DW3000Ranging::_parseFinalFrame(const uint8_t* buf, size_t len,
                                      uint64_t& tPollTx, uint64_t& tRespRx, uint64_t& tFinalTx) {
    if (len < FINAL_OFF_TFINALTX + 5 || buf[0] != UWB_MSG_FINAL) return false;
    tPollTx  = get40(buf + FINAL_OFF_TPOLLTX);
    tRespRx  = get40(buf + FINAL_OFF_TRESRX);
    tFinalTx = get40(buf + FINAL_OFF_TFINALTX);
    return true;
}

bool DW3000Ranging::_parseReportFrame(const uint8_t* buf, size_t len, float& distanceCm) {
    if (len < FRAME_HDR_LEN + 4 || buf[0] != UWB_MSG_REPORT) return false;
    memcpy(&distanceCm, buf + FRAME_HDR_LEN, 4);
    return true;
}

// ─── State helpers ────────────────────────────────────────────────────────────

void DW3000Ranging::_enterState(UWBRangingState newState) {
    _state          = newState;
    _stateEnteredMs = millis();
}

bool DW3000Ranging::_isTimeout(uint32_t timeoutMs) {
    return (millis() - _stateEnteredMs) >= timeoutMs;
}
