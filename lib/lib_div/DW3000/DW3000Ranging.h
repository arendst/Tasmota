#pragma once
/*
 * DW3000Ranging.h
 * TWR (Two-Way Ranging) state machine for DW3000
 *
 * Implements:
 *  - SS-TWR: Single Sided (1 exchange, faster, less accurate)
 *  - DS-TWR: Double Sided (3 exchanges, recommended for accuracy)
 *
 * Non-blocking: state machine advances in small increments
 * called from the Tasmota main loop.
 */

#include "DW3000Driver.h"

// ─── UWB message types (first payload byte) ──────────────────────────────────
#define UWB_MSG_POLL      0x21  // Initiator → Responder: start ranging
#define UWB_MSG_RESPONSE  0x50  // Responder → Initiator: acknowledge
#define UWB_MSG_FINAL     0x23  // Initiator → Responder: final timestamps
#define UWB_MSG_REPORT    0x58  // Responder → Initiator: computed distance

// ─── Timeouts and delays (ms) ────────────────────────────────────────────────
// Tasmota/WiFi main loop may have gaps of 50+ ms.
#define UWB_TIMEOUT_RESPONSE_MS  200
#define UWB_TIMEOUT_REPORT_MS    300
#define UWB_RANGING_INTERVAL_MS  200
// FINAL TX delay must exceed the max Tasmota loop gap (~50 ms).
// TX is scheduled in advance and triggered by DW3000 hardware timer.
#define FINAL_TX_DELAY_MS        150UL

// ─── State machine ────────────────────────────────────────────────────────────
enum UWBRangingState : uint8_t {
  UWB_STATE_IDLE = 0,
  // Initiator (tag)
  UWB_STATE_SEND_POLL,
  UWB_STATE_WAIT_RESPONSE,
  UWB_STATE_SEND_FINAL,
  UWB_STATE_WAIT_REPORT,
  UWB_STATE_DONE,
  // Responder (anchor)
  UWB_STATE_LISTEN,
  UWB_STATE_SEND_RESPONSE,
  UWB_STATE_WAIT_FINAL,
  UWB_STATE_SEND_REPORT,
  // Error
  UWB_STATE_ERROR
};

// ─── Max anchors (multi-anchor round-robin) ───────────────────────────────────
#define UWB_MAX_ANCHORS 4

// ─── Node role ────────────────────────────────────────────────────────────────
enum UWBNodeRole : uint8_t {
  UWB_ROLE_OFF    = 0,
  UWB_ROLE_TAG    = 1,  // Initiator: computes and publishes distance
  UWB_ROLE_ANCHOR = 2   // Responder: answers poll frames
};

// ─── Ranging result ───────────────────────────────────────────────────────────
struct UWBRangingResult {
  float    distanceCm  = 0.0f;
  float    rxPowerDbm  = 0.0f;
  uint8_t  anchorId    = 0;
  bool     valid       = false;
  uint32_t timestamp   = 0;      // millis() at measurement time
  char     method[8]   = "DS-TWR";
};

// ─── DW3000Ranging main class ─────────────────────────────────────────────────
class DW3000Ranging {
public:
  DW3000Ranging(DW3000Driver& driver);

  // ── Configuration ─────────────────────────────────────────────────────────

  void setRole(UWBNodeRole role) { _role = role; }
  void setNodeAddress(uint16_t addr) { _myAddr = addr; }
  void setAnchorAddress(uint16_t addr) {
    _anchorAddrs[0] = addr; _anchorCount = 1; _currentAnchorIdx = 0;
  }
  void addAnchorAddress(uint16_t addr);
  void setRangingInterval(uint32_t ms) { _intervalMs = ms; }
  void setAntennaDelay(float delay) { _antennaDelay = delay; }
  void useDoubleSided(bool ds) { _useDS = ds; }

  UWBNodeRole     getRole()  const { return _role; }
  UWBRangingState getState() const { return _state; }

  // ── Lifecycle ─────────────────────────────────────────────────────────────

  bool begin();  // Initialize chip and start state machine for the configured role
  void loop();   // Advance state machine one step — call from FUNC_LOOP, non-blocking
  bool getResult(UWBRangingResult& result);  // Returns true if a new measurement is ready
  void reset();

  // ── Statistics ────────────────────────────────────────────────────────────
  uint32_t getRangingCount()   const { return _rangingCount; }
  uint32_t getErrorCount()     const { return _errorCount; }
  float    getLastDistanceCm() const { return _lastResult.distanceCm; }

  // ── Raw DW3000 timestamps (debug / calibration) ───────────────────────────
  // TAG side  : tPollTx, tRespRx, tFinalTx
  // ANCHOR side: tPollRx, tRespTx, tFinalRx + TAG's 3 received in FINAL frame
  uint64_t getTPollTx()  const { return _tPollTx; }
  uint64_t getTPollRx()  const { return _tPollRx; }
  uint64_t getTRespTx()  const { return _tRespTx; }
  uint64_t getTRespRx()  const { return _tRespRx; }
  uint64_t getTFinalTx() const { return _tFinalTx; }
  uint64_t getTFinalRx() const { return _tFinalRx; }

private:
  DW3000Driver& _drv;

  UWBNodeRole     _role        = UWB_ROLE_OFF;
  UWBRangingState _state       = UWB_STATE_IDLE;

  uint16_t  _myAddr            = 0x0001;
  uint16_t  _anchorAddrs[UWB_MAX_ANCHORS] = {0x0002};
  uint8_t   _anchorCount       = 1;
  uint8_t   _currentAnchorIdx  = 0;
  uint16_t  _tagAddr           = 0;
  uint32_t  _intervalMs        = UWB_RANGING_INTERVAL_MS;
  float     _antennaDelay      = 16384.0f;
  bool      _useDS             = true;   // DS-TWR by default
  bool      _finalTxScheduled  = false;  // true between transmitAt() and TX done

  uint32_t  _lastRangingMs     = 0;
  uint32_t  _stateEnteredMs    = 0;

  uint32_t  _rangingCount      = 0;
  uint32_t  _errorCount        = 0;

  UWBRangingResult _lastResult;
  bool             _resultReady = false;

  // DS-TWR timestamps (DW3000 ticks, 40-bit)
  uint64_t _tPollTx  = 0;
  uint64_t _tPollRx  = 0;
  uint64_t _tRespTx  = 0;
  uint64_t _tRespRx  = 0;
  uint64_t _tFinalTx = 0;
  uint64_t _tFinalRx = 0;

  // ── State machine steps (initiator) ──────────────────────────────────────
  void _stepSendPoll();
  void _stepWaitResponse();
  void _stepSendFinal();
  void _stepWaitReport();

  // ── State machine steps (responder) ──────────────────────────────────────
  void _stepListen();
  void _stepSendResponse();
  void _stepWaitFinal();
  void _stepSendReport();

  // ── Distance computation ──────────────────────────────────────────────────

  // DS-TWR formula APS013: tof = (tRound1*tRound2 - tReply1*tReply2) /
  //                               (tRound1+tRound2+tReply1+tReply2)
  float _computeDistanceDS(uint64_t tPollTx, uint64_t tPollRx,
                            uint64_t tRespTx, uint64_t tRespRx,
                            uint64_t tFinalTx, uint64_t tFinalRx);

  float _computeDistanceSS(uint64_t tPollTx, uint64_t tRespRx,
                            uint64_t tRespTx, uint64_t tPollRx);

  float _ticksToCm(double ticks);

  // ── Frame helpers ─────────────────────────────────────────────────────────
  void _buildPollFrame    (uint8_t* buf, size_t& len);
  void _buildResponseFrame(uint8_t* buf, size_t& len);
  void _buildFinalFrame   (uint8_t* buf, size_t& len,
                           uint64_t tPollTx, uint64_t tRespRx, uint64_t tFinalTx);
  void _buildReportFrame  (uint8_t* buf, size_t& len, float distanceCm);

  bool _parsePollFrame    (const uint8_t* buf, size_t len);
  bool _parseResponseFrame(const uint8_t* buf, size_t len);
  bool _parseFinalFrame   (const uint8_t* buf, size_t len,
                           uint64_t& tPollTx, uint64_t& tRespRx, uint64_t& tFinalTx);
  bool _parseReportFrame  (const uint8_t* buf, size_t len, float& distanceCm);

  void _enterState(UWBRangingState newState);
  bool _isTimeout(uint32_t timeoutMs);
};
