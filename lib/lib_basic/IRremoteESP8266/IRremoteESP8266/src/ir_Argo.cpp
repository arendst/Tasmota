// Copyright 2017 Schmolders
// Copyright 2019 crankyoldgit
// Copyright 2022 Mateusz Bronk (mbronk)
/// @file
/// @brief Argo A/C protocol.

/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1859
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1912


#include "ir_Argo.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif  // UNIT_TEST
#include "IRremoteESP8266.h"
#include "IRtext.h"
#include "IRutils.h"

// Constants
// using SPACE modulation. MARK is always const 400u
const uint16_t kArgoHdrMark = 6400;
const uint16_t kArgoHdrSpace = 3300;
const uint16_t kArgoBitMark = 400;
const uint16_t kArgoOneSpace = 2200;
const uint16_t kArgoZeroSpace = 900;
const uint32_t kArgoGap = kDefaultMessageGap;  // Made up value. Complete guess.
const uint8_t  kArgoSensorCheck = 52;  // Part of the sensor message check calc.
const uint8_t  kArgoSensorFixed = 0b011;
const uint8_t  kArgoWrem3Preamble = 0b1011;
const uint8_t  kArgoWrem3Postfix_Timer = 0b1;
const uint8_t  kArgoWrem3Postfix_ACControl = 0b110000;

using irutils::addBoolToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::addFanToString;
using irutils::addSwingVToString;
using irutils::minsToString;
using irutils::addDayToString;
using irutils::addModelToString;
using irutils::daysBitmaskToString;
using irutils::addTimerModeToString;

#if SEND_ARGO
/// Send a Argo A/C formatted message.
/// Status: [WREM-2] BETA / Probably works.
///         [WREM-3] Confirmed working w/ Argo 13 ECO (WREM-3)
/// @note The "no footer" part needs re-checking for validity but retained for
///       backwards compatibility.
///       Consider using @c sendFooter=true code for WREM-2 as well
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @param[in] sendFooter Whether to send footer and add a final gap.
///                       *REQUIRED* for WREM-3, UNKNOWN for WREM-2 (used to be
///                       disabled in previous impl., hence retained)
/// @note Consider removing this param (default to true) if WREM-2 works w/ it
void IRsend::sendArgo(const unsigned char data[], const uint16_t nbytes,
                      const uint16_t repeat, bool sendFooter /*= false*/) {
  if (nbytes < std::min({kArgo3AcControlStateLength,
                         kArgo3ConfigStateLength,
                         kArgo3iFeelReportStateLength,
                         kArgo3TimerStateLength,
                         kArgoStateLength,
                         kArgoShortStateLength})) {
    return;  // Not enough bytes to send a proper message.
  }

  const uint16_t _footermark = (sendFooter)? kArgoBitMark : 0;
  const uint32_t _gap = (sendFooter)? kArgoGap : 0;

  sendGeneric(kArgoHdrMark, kArgoHdrSpace, kArgoBitMark, kArgoOneSpace,
              kArgoBitMark, kArgoZeroSpace,
              _footermark, _gap,
              data, nbytes, kArgoFrequency, false, repeat, kDutyDefault);
}


/// Send a Argo A/C formatted message.
/// Status: Confirmed working w/ Argo 13 ECO (WREM-3)
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendArgoWREM3(const unsigned char data[], const uint16_t nbytes,
                           const uint16_t repeat) {
  sendArgo(data, nbytes, repeat, true);
}
#endif  // SEND_ARGO


/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
template <typename T>
IRArgoACBase<T>::IRArgoACBase(const uint16_t pin, const bool inverted,
                   const bool use_modulation)
      : _irsend(pin, inverted, use_modulation) { stateReset(); }


/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRArgoAC::IRArgoAC(const uint16_t pin, const bool inverted,
                   const bool use_modulation)
      : IRArgoACBase<ArgoProtocol>(pin, inverted, use_modulation) { }


/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRArgoAC_WREM3::IRArgoAC_WREM3(const uint16_t pin, const bool inverted,
                              const bool use_modulation)
    : IRArgoACBase<ArgoProtocolWREM3>(pin, inverted, use_modulation) {}

/// Set up hardware to be able to send a message.
template<typename T>
void IRArgoACBase<T>::begin(void) { _irsend.begin(); }


/// @cond
/// @brief Get byte length of raw WREM-2 message based on IR cmd type
/// @note This is a full specialization for @c ArgoProtocol type and while
///       it semantically belongs to @c IrArgoAC class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @param type The type of IR command
/// @note Not all types are supported. AC_CONTROL and TIMER are the same cmd
/// @return Byte length of state command
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
uint16_t IRArgoACBase<ArgoProtocol>::getStateLengthForIrMsgType(
  argoIrMessageType_t type) {
  switch (type) {
    case argoIrMessageType_t::AC_CONTROL:
    case argoIrMessageType_t::TIMER_COMMAND:
      return kArgoStateLength;
    case argoIrMessageType_t::IFEEL_TEMP_REPORT:
      return kArgoShortStateLength;
    case argoIrMessageType_t::CONFIG_PARAM_SET:
    default:
      return 0;  // Not supported by WREM-2
  }
}
/// @endcond

/// @brief Get byte length of raw WREM-3 message based on IR cmd type
/// @note This is a full specialization for @c ArgoProtocolWREM3 type and while
///       it semantically belongs to @c IrArgoAC_WREM3 class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @param type The type of IR command
/// @return Byte length of state command
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
uint16_t IRArgoACBase<ArgoProtocolWREM3>::getStateLengthForIrMsgType(
    argoIrMessageType_t type) {
  switch (type) {
    case argoIrMessageType_t::AC_CONTROL:
      return kArgo3AcControlStateLength;
    case argoIrMessageType_t::IFEEL_TEMP_REPORT:
      return kArgo3iFeelReportStateLength;
    case argoIrMessageType_t::TIMER_COMMAND:
      return kArgo3TimerStateLength;
    case argoIrMessageType_t::CONFIG_PARAM_SET:
      return kArgo3ConfigStateLength;
    default:
      return 0;
  }
}

/// @cond
/// @brief Get message type from raw WREM-2 data
/// 1st param ignored: WREM-2 does not carry type in payload, allegedly
/// @param length Message length: used for *heuristic* detection of message type
/// @return IR message type
/// @note This is a full specialization for @c ArgoProtocol type and while
///       it semantically belongs to @c IrArgoAC class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
argoIrMessageType_t IRArgoACBase<ArgoProtocol>::getMessageType(
    const uint8_t[], const uint16_t length) {
  if (length == kArgoShortStateLength) {
    return argoIrMessageType_t::IFEEL_TEMP_REPORT;
  }
  return argoIrMessageType_t::AC_CONTROL;
}
/// @endcond


/// @brief Get message type from raw WREM-3 data
/// @param state The raw IR data
/// @param length Length of @c state (in byte)
/// @return IR message type
/// @note This is a full specialization for @c ArgoProtocolWREM3 type and while
///       it semantically belongs to @c IrArgoAC_WREM3 class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
argoIrMessageType_t IRArgoACBase<ArgoProtocolWREM3>::getMessageType(
  const uint8_t state[], const uint16_t length) {
  if (length < 1) {
    return static_cast<argoIrMessageType_t>(-1);
  }
  return static_cast<argoIrMessageType_t>(state[0] >> 6);
}

/// @brief Get message type from raw WREM-3 data
/// @param raw Raw data
/// @return IR message type
argoIrMessageType_t IRArgoAC_WREM3::getMessageType(
  const ArgoProtocolWREM3& raw) {
  return static_cast<argoIrMessageType_t>(raw.IrCommandType);
}


/// @brief Get actual raw state byte length for the current state
/// _param 1st param ignored: WREM-2 does not caryy type in payload, allegedly
/// @param messageType Type of message the state is carrying
/// @return Actual length of state (in bytes)
/// @note This is a full specialization for @c ArgoProtocol type and while
///       it semantically belongs to @c IrArgoAC class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
uint16_t IRArgoACBase<ArgoProtocol>::getRawByteLength(const ArgoProtocol&,
    argoIrMessageType_t messageType) {
  if (messageType == argoIrMessageType_t::IFEEL_TEMP_REPORT) {
    return kArgoShortStateLength;
  }
  return kArgoStateLength;
}


/// @brief Get actual raw state byte length for the current state
/// @param raw The raw state
/// _param 2nd param ignored (1st byte of @c raw is sufficient to get len)
/// @return Actual length of state (in bytes)
/// @note This is a full specialization for @c ArgoProtocolWREM3 type and while
///       it semantically belongs to @c IrArgoAC_WREM3 class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
uint16_t IRArgoACBase<ArgoProtocolWREM3>::getRawByteLength(
    const ArgoProtocolWREM3& raw,  argoIrMessageType_t) {
  return IRArgoAC_WREM3::getStateLengthForIrMsgType(
    IRArgoAC_WREM3::getMessageType(raw));
}


/// @brief Get actual raw state byte length for the current state
/// @return Actual length of state (in bytes)
template<typename T>
uint16_t IRArgoACBase<T>::getRawByteLength() const {
  return getRawByteLength(_, _messageType);
}

/// @cond
/// Calculate the checksum for a given state (WREM-2).
/// @note This is a full specialization for @c ArgoProtocol type and while
///       it semantically belongs to @c IrArgoAC class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @warning This does NOT calculate 'short' (iFeel) message checksums
/// @param[in] state The array to calculate the checksum for.
/// @param[in] length The size of the state.
/// @return The 8-bit calculated result.
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
uint8_t IRArgoACBase<ArgoProtocol>::calcChecksum(const uint8_t state[],
                                                 const uint16_t length) {
  // Corresponds to byte 11 being constant 0b01
  // Only add up bytes to 9. byte 10 is 0b01 constant anyway.
  // Assume that argo array is MSB first (left)
  return sumBytes(state, length - 2, 2);
}
/// @endcond


/// Calculate the checksum for a given state (WREM-3).
/// @note This is a full specialization for @c ArgoProtocolWREM3 type and while
///       it semantically belongs to @c IrArgoAC_WREM3 class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @param[in] state The array to calculate the checksum for.
/// @param[in] length The size of the state.
/// @return The 8-bit calculated result.
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
uint8_t IRArgoACBase<ArgoProtocolWREM3>::calcChecksum(const uint8_t state[],
                                                      const uint16_t length) {
  if (length < 1) {
    return -1;  // Nothing to compute on
  }

  uint16_t payloadSizeBits = (length - 1) * 8;  // Last byte carries checksum

  argoIrMessageType_t msgType = getMessageType(state, length);
  if (msgType == argoIrMessageType_t::IFEEL_TEMP_REPORT) {
    payloadSizeBits += 5;  // For WREM3::iFeel the checksum is 3-bit
  } else if (msgType == argoIrMessageType_t::TIMER_COMMAND) {
    payloadSizeBits += 3;  // For WREM3::Timer the checksum is 5-bit
  }  // Otherwise: full 8-bit checksum

  uint8_t checksum = sumBytes(state, payloadSizeBits / 8, 0);

  // Add stray bits from last byte to the checksum (if any)
  const uint8_t maskPayload = 0xFF >> (8 - (payloadSizeBits % 8));
  checksum += (state[length-1] & maskPayload);

  const uint8_t maskChecksum = 0xFF >> (payloadSizeBits % 8);
  return checksum & maskChecksum;
}


/// Update the checksum for a given state (WREM2).
/// @note This is a full specialization for @c ArgoProtocol type and while
///       it semantically belongs to @c IrArgoAC class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @warning This impl does not support short message format (iFeel)
/// @param[in,out] state Pointer to a binary representation of the A/C state.
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
void IRArgoACBase<ArgoProtocol>::_checksum(ArgoProtocol *state) {
  uint8_t sum = calcChecksum(state->raw, kArgoStateLength);
  // Append sum to end of array
  // Set const part of checksum bit 10
  state->Post = kArgoPost;
  state->Sum = sum;
}


/// @brief Update the checksum for a given state (WREM3).
/// @note This is a full specialization for @c ArgoProtocolWREM3 type and while
///       it semantically belongs to @c IrArgoAC_WREM3 class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @param[in,out] state Pointer to a binary representation of the A/C state.
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
void IRArgoACBase<ArgoProtocolWREM3>::_checksum(ArgoProtocolWREM3 *state) {
  argoIrMessageType_t msgType = IRArgoAC_WREM3::getMessageType(*state);

  uint8_t sum = calcChecksum(state->raw, getRawByteLength(*state));
  switch (msgType) {
    case argoIrMessageType_t::IFEEL_TEMP_REPORT:
      state->CheckHi = sum;
      break;
    case argoIrMessageType_t::TIMER_COMMAND:
      state->timer.Checksum = sum;
      break;
    case argoIrMessageType_t::CONFIG_PARAM_SET:
      state->config.Checksum = sum;
      break;
    case argoIrMessageType_t::AC_CONTROL:
    default:
      state->Sum = sum;
      break;
  }
}


/// Update the checksum for the internal state.
template<typename T>
void IRArgoACBase<T>::checksum(void) { _checksum(&_); }


/// Reset the given state to a known good state.
/// @note This is a full specialization for @c ArgoProtocol type and while
///       it semantically belongs to @c IrArgoAC class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @param[in,out] state Pointer to a binary representation of the A/C state.
/// _param 2nd param unused (always resets to AC_CONTROL state)
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
void IRArgoACBase<ArgoProtocol>::_stateReset(ArgoProtocol *state,
    argoIrMessageType_t) {
  for (uint8_t i = 2; i < kArgoStateLength; i++) state->raw[i] = 0x0;
  state->Pre1 = kArgoPreamble1;  // LSB first (as sent) 0b00110101;
  state->Pre2 = kArgoPreamble2;  // LSB first: 0b10101111;
  state->Post = kArgoPost;
}


/// Reset the given state to a known good state
/// @note This is a full specialization for @c ArgoProtocolWREM3 type and while
///       it semantically belongs to @c IrArgoAC_WREM3 class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @param[in,out] state Pointer to a binary representation of the A/C state.
/// @param messageType Type of message to reset the state for
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
void IRArgoACBase<ArgoProtocolWREM3>::_stateReset(ArgoProtocolWREM3 *state,
    argoIrMessageType_t messageType) {
  for (uint8_t i = 1; i < sizeof(state->raw) / sizeof(state->raw[0]); i++) {
    state->raw[i] = 0x0;
  }
  state->Pre1 = kArgoWrem3Preamble;  // LSB first (as sent) 0b00110101;
  state->IrChannel = 0;
  state->IrCommandType = static_cast<uint8_t>(messageType);

  if (messageType == argoIrMessageType_t::TIMER_COMMAND) {
    state->timer.Post1 = kArgoWrem3Postfix_Timer;  // 0b1
  } else if (messageType == argoIrMessageType_t::AC_CONTROL) {
    state->Post1 = kArgoWrem3Postfix_ACControl;  // 0b110000
  }
}


/// @brief Reset the internals of the object to a known good state.
/// @param messageType Type of message to reset the state for
template<typename T>
void IRArgoACBase<T>::stateReset(argoIrMessageType_t messageType) {
  _stateReset(&_, messageType);
  if (messageType == argoIrMessageType_t::AC_CONTROL) {
    off();
    setTemp(20);
    setSensorTemp(25);
    setMode(argoMode_t::AUTO);
    setFan(argoFan_t::FAN_AUTO);
  }
  _messageType = messageType;
  _length = getStateLengthForIrMsgType(_messageType);
}


/// @brief Retrieve the checksum value from transmitted state
/// @note This is a full specialization for @c ArgoProtocol type and while
///       it semantically belongs to @c IrArgoAC class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @param[in] state Raw state
/// @param length Length of @c state in bytes
/// @return Checksum value (8-bit)
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
uint8_t IRArgoACBase<ArgoProtocol>::getChecksum(const uint8_t state[],
                                                const uint16_t length) {
  if (length < 1) {
    return -1;
  }
  return (state[length - 2] >> 2) + (state[length - 1] << 6);
}

/// @cond
/// @brief Retrieve the checksum value from transmitted state
/// @note This is a full specialization for @c ArgoProtocolWREM3 type and while
///       it semantically belongs to @c IrArgoAC_WREM3 class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @param[in] state Raw state
/// @param length Length of @c state in bytes
/// @return Checksum value (up to 8-bit)
template<>
uint8_t IRArgoACBase<ArgoProtocolWREM3>::getChecksum(const uint8_t state[],
                                                     const uint16_t length) {
  if (length < 1) {
    return -1;
  }
  argoIrMessageType_t msgType = getMessageType(state, length);
  if (msgType == argoIrMessageType_t::IFEEL_TEMP_REPORT) {
    return (state[length - 1] & 0b11100000) >> 5;
  }
  if (msgType == argoIrMessageType_t::TIMER_COMMAND) {
    return state[length - 1] >> 3;
  }
  return (state[length - 1]);
}
/// @endcond


/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The size of the state.
/// @return A boolean indicating if it's checksum is valid.
template<typename T>
bool IRArgoACBase<T>::validChecksum(const uint8_t state[],
                                    const uint16_t length) {
  return (getChecksum(state, length) == calcChecksum(state, length));
}


#if SEND_ARGO
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
template<typename T>
void IRArgoACBase<T>::send(const uint16_t repeat) {
  _irsend.sendArgo(getRaw(), getRawByteLength(), repeat);
}

/// @cond
/// Send the current internal state as an IR message.
/// @note This is a full specialization for @c ArgoProtocolWREM3 type and while
///       it semantically belongs to @c IrArgoAC_WREM3 class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @param[in] repeat Nr. of times the message will be repeated.
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
void IRArgoACBase<ArgoProtocolWREM3>::send(const uint16_t repeat) {
  _irsend.sendArgoWREM3(getRaw(), getRawByteLength(), repeat);
}
/// @endcond


/// Send current room temperature for the iFeel feature as a silent IR
/// message (no acknowledgement from the device) (WREM2)
/// @param[in] degrees The temperature in degrees celsius.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRArgoAC::sendSensorTemp(const uint8_t degrees, const uint16_t repeat) {
  const uint8_t temp = std::max(std::min(degrees, kArgoMaxRoomTemp),
                                kArgoTempDelta) - kArgoTempDelta;
  const uint8_t check = kArgoSensorCheck + temp;

  ArgoProtocol data;
  _stateReset(&data, argoIrMessageType_t::IFEEL_TEMP_REPORT);
  data.SensorT = temp;
  data.CheckHi = check >> 5;
  data.CheckLo = check;
  data.Fixed = kArgoSensorFixed;
  _checksum(&data);
  uint16_t msgLen = getRawByteLength(data,
                                     argoIrMessageType_t::IFEEL_TEMP_REPORT);

  _irsend.sendArgo(data.raw, msgLen, repeat);
}

/// Send current room temperature for the iFeel feature as a silent IR
/// message (no acknowledgement from the device) (WREM3)
/// @param[in] degrees The temperature in degrees celsius.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRArgoAC_WREM3::sendSensorTemp(const uint8_t degrees,
    const uint16_t repeat) {
  const uint8_t temp = std::max(std::min(degrees, kArgoMaxRoomTemp),
                                kArgoTempDelta) - kArgoTempDelta;
  ArgoProtocolWREM3 data = {};
  _stateReset(&data, argoIrMessageType_t::IFEEL_TEMP_REPORT);
  data.SensorT = temp;
  _checksum(&data);
  uint16_t msgLen = getRawByteLength(data,
                                     argoIrMessageType_t::IFEEL_TEMP_REPORT);
  _irsend.sendArgoWREM3(data.raw, msgLen, repeat);
}
#endif


/// Get the raw state of the object, suitable to be sent with the appropriate
/// IRsend object method.
/// @return A PTR to the internal state.
template<typename T>
uint8_t* IRArgoACBase<T>::getRaw(void) {
  checksum();  // Ensure correct bit array before returning
  return _.raw;
}


/// Set the raw state of the object.
/// @param[in] state The raw state from the native IR message.
/// @param[in] length The length of raw state in bytes.
template<typename T>
void IRArgoACBase<T>::setRaw(const uint8_t state[], const uint16_t length) {
  std::memcpy(_.raw, state, length);
  _messageType = getMessageType(state, length);
  _length = length;
}

/// Set the internal state to have the power on.
template<typename T>
void IRArgoACBase<T>::on(void) { setPower(true); }

/// Set the internal state to have the power off.
template<typename T>
void IRArgoACBase<T>::off(void) { setPower(false); }

/// @cond
/// Set the internal state to have the desired power.
/// @param[in] on The desired power state.
/// @note This is a full specialization for @c ArgoProtocol type and while
///       it semantically belongs to @c IrArgoAC class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
void IRArgoACBase<ArgoProtocol>::setPower(const bool on) {
  _.Power = on;
}
/// @endcond

/// @brief Set the internal state to have the desired power.
/// @note This is a full specialization for @c ArgoProtocolWREM3 type and while
///       it semantically belongs to @c IrArgoAC_WREM3 class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @param[in] on The desired power state.
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
void IRArgoACBase<ArgoProtocolWREM3>::setPower(const bool on) {
  if (_messageType == argoIrMessageType_t::TIMER_COMMAND) {
    _.timer.IsOn = on;
  } else {
    _.Power = on;
  }
}

/// Get the power setting from the internal state.
/// @note This is a full specialization for @c ArgoProtocol type and while
///       it semantically belongs to @c IrArgoAC class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @return A boolean indicating the power setting.
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
bool IRArgoACBase<ArgoProtocol>::getPower(void) const { return _.Power; }

/// Get the power setting from the internal state.
/// @note This is a full specialization for @c ArgoProtocolWREM3 type and while
///       it semantically belongs to @c IrArgoAC_WREM3 class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @return A boolean indicating the power setting.
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
bool IRArgoACBase<ArgoProtocolWREM3>::getPower(void) const {
  if (_messageType == argoIrMessageType_t::TIMER_COMMAND) {
    return _.timer.IsOn;
  }
  return _.Power;
}

/// Control the current Max setting. (i.e. Turbo)
/// @param[in] on The desired setting.
template<typename T>
void IRArgoACBase<T>::setMax(const bool on) {
  _.Max = on;
}

/// Is the Max (i.e. Turbo) setting on?
/// @return The current value.
template<typename T>
bool IRArgoACBase<T>::getMax(void) const { return _.Max; }

/// Set the temperature.
/// @param[in] degrees The temperature in degrees celsius.
/// @note Sending 0 equals +4
template<typename T>
void IRArgoACBase<T>::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max(kArgoMinTemp, degrees);
  // delta 4 degrees. "If I want 12 degrees, I need to send 8"
  temp = std::min(kArgoMaxTemp, temp) - kArgoTempDelta;
  // mask out bits
  _.Temp = temp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
template<typename T>
uint8_t IRArgoACBase<T>::getTemp(void) const {
  return _.Temp + kArgoTempDelta;
}


/// @brief Get the current fan mode setting as a strongly typed value (WREM2).
/// @note This is a full specialization for @c ArgoProtocol type and while
///       it semantically belongs to @c IrArgoAC class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @return The current fan mode.
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
argoFan_t IRArgoACBase<ArgoProtocol>::getFanEx(void) const {
  switch (_.Fan) {
    case kArgoFan3:
      return argoFan_t::FAN_HIGHEST;
    case kArgoFan2:
      return argoFan_t::FAN_MEDIUM;
    case kArgoFan1:
      return argoFan_t::FAN_LOWEST;
    case kArgoFanAuto:
      return argoFan_t::FAN_AUTO;
    default:
      return static_cast<argoFan_t>(_.Fan);
  }
}

/// @brief Get the current fan mode setting as a strongly typed value (WREM3).
/// @note This is a full specialization for @c ArgoProtocolWREM3 type and while
///       it semantically belongs to @c IrArgoAC_WREM3 class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @return The current fan mode.
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
argoFan_t IRArgoACBase<ArgoProtocolWREM3>::getFanEx(void) const {
  return static_cast<argoFan_t>(_.Fan);
}

/// @cond
/// Set the desired fan mode (WREM2).
/// @note This is a full specialization for @c ArgoProtocol type and while
///       it semantically belongs to @c IrArgoAC class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @param[in] fan The desired fan speed.
/// @note Only a subset of fan speeds are supported (1|2|3|Auto)
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
void IRArgoACBase<ArgoProtocol>::setFan(argoFan_t fan) {
  switch (fan) {
    case argoFan_t::FAN_AUTO:
      _.Fan = kArgoFanAuto;
      break;
    case argoFan_t::FAN_HIGHEST:
    case argoFan_t::FAN_HIGH:
      _.Fan = kArgoFan3;
      break;
    case argoFan_t::FAN_MEDIUM:
    case argoFan_t::FAN_LOW:
      _.Fan = kArgoFan2;
      break;
    case argoFan_t::FAN_LOWER:
    case argoFan_t::FAN_LOWEST:
      _.Fan = kArgoFan1;
      break;
    default:
      uint8_t raw_value = static_cast<uint8_t>(fan);  // 2-bit value, per def.
      if ((raw_value & 0b11) == raw_value) {
        // Outside of known value range, but matches field length
        // Let's assume the caller knows what they're doing and pass it through
        _.Fan = raw_value;
      } else {
        _.Fan = kArgoFanAuto;
      }
      break;
  }
}
/// @endcond

/// Set the desired fan mode (WREM3).
/// @note This is a full specialization for @c ArgoProtocolWREM3 type and while
///       it semantically belongs to @c IrArgoAC_WREM3 class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @param[in] fan The desired fan speed.
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
void IRArgoACBase<ArgoProtocolWREM3>::setFan(argoFan_t fan) {
  switch (fan) {
    case argoFan_t::FAN_AUTO:
    case argoFan_t::FAN_HIGHEST:
    case argoFan_t::FAN_HIGH:
    case argoFan_t::FAN_MEDIUM:
    case argoFan_t::FAN_LOW:
    case argoFan_t::FAN_LOWER:
    case argoFan_t::FAN_LOWEST:
      _.Fan = static_cast<uint8_t>(fan);
      break;
    default:
      _.Fan = static_cast<uint8_t>(argoFan_t::FAN_AUTO);
      break;
  }
}

/// Set the speed of the fan.
/// @deprecated
/// @param[in] fan The desired setting.
void IRArgoAC::setFan(const uint8_t fan) {
  _.Fan = std::min(fan, kArgoFan3);
}

/// Get the current fan speed setting.
/// @deprecated
/// @return The current fan speed.
uint8_t IRArgoAC::getFan(void) const {
  return _.Fan;
}

/// @brief Get Flap (VSwing) value as a strongly-typed value
/// @note This @c getFlapEx() method has been introduced to be able to retain
///       old implementation of @c getFlap() for @c IRArgoAc which used uint8_t
/// @return Flap setting
template<typename T>
argoFlap_t IRArgoACBase<T>::getFlapEx(void) const {
  return static_cast<argoFlap_t>(_.Flap);
}

/// Set the desired flap mode
/// @param[in] flap The desired flap mode.
template<typename T>
void IRArgoACBase<T>::setFlap(argoFlap_t flap) {
  uint8_t raw_value = static_cast<uint8_t>(flap);
  if ((raw_value & 0b111) == raw_value) {
    // Outside of known value range, but matches field length
    // Let's assume the caller knows what they're doing and pass it through
    _.Flap = raw_value;
  } else {
    _.Flap = static_cast<uint8_t>(argoFlap_t::FLAP_AUTO);
  }
}

/// Set the flap position. i.e. Swing. (WREM2)
/// @warning Not yet working!
/// @deprecated
/// @param[in] flap The desired setting.
void IRArgoAC::setFlap(const uint8_t flap) {
  setFlap(static_cast<argoFlap_t>(flap));
  // TODO(kaschmo): set correct bits for flap mode
}

/// Get the flap position. i.e. Swing. (WREM2)
/// @warning Not yet working!
/// @deprecated
/// @return The current flap setting.
uint8_t IRArgoAC::getFlap(void) const {
  return _.Flap;
}

/// Get the current operation mode setting.
/// @note This is a full specialization for @c ArgoProtocol type and while
///       it semantically belongs to @c IrArgoAC class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @return The current operation mode.
/// @note This @c getModeEx() method has been introduced to be able to retain
///       old implementation of @c getMode() for @c IRArgoAc which used uint8_t
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
argoMode_t IRArgoACBase<ArgoProtocol>::getModeEx(void) const {
  switch (_.Mode) {
    case kArgoCool:
      return argoMode_t::COOL;
    case kArgoDry:
      return argoMode_t::DRY;
    case kArgoAuto:
      return argoMode_t::AUTO;
    case kArgoHeat:
      return argoMode_t::HEAT;
    case kArgoOff:  // Modelling "FAN" as "OFF", for the lack of better constant
      return argoMode_t::FAN;
    case kArgoHeatAuto:
    default:
      return static_cast<argoMode_t>(_.Mode);
  }
}

/// Get the current operation mode setting.
/// @note This is a full specialization for @c ArgoProtocolWREM3 type and while
///       it semantically belongs to @c IrArgoAC_WREM3 class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function.
/// @return The current operation mode.
/// @note This @c getModeEx() method has been introduced to be able to retain
///       old implementation of @c getMode() for @c IRArgoAc which used uint8_t
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
argoMode_t IRArgoACBase<ArgoProtocolWREM3>::getModeEx(void) const {
  return static_cast<argoMode_t>(_.Mode);
}

/// @cond
/// Set the desired operation mode.
/// @param[in] mode The desired operation mode.
/// @note This is a full specialization for @c ArgoProtocol type and while
///       it semantically belongs to @c IrArgoAC class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
void IRArgoACBase<ArgoProtocol>::setMode(argoMode_t mode) {
  switch (mode) {
    case argoMode_t::COOL:
      _.Mode = static_cast<uint8_t>(kArgoCool);
      break;
    case argoMode_t::DRY:
      _.Mode = static_cast<uint8_t>(kArgoDry);
      break;
    case argoMode_t::HEAT:
      _.Mode = static_cast<uint8_t>(kArgoHeat);
      break;
    case argoMode_t::FAN:
      _.Mode = static_cast<uint8_t>(kArgoOff);
      break;
    case argoMode_t::AUTO:
      _.Mode = static_cast<uint8_t>(kArgoAuto);
      break;
    default:
      uint8_t raw_value = static_cast<uint8_t>(mode);
      if ((raw_value & 0b111) == raw_value) {
        // Outside of known value range, but matches field length
        // Let's assume the caller knows what they're doing and pass it through
        _.Mode = raw_value;
      } else {
        _.Mode = static_cast<uint8_t>(kArgoAuto);
      }
      break;
  }
}
/// @endcond

/// @brief Set the desired operation mode.
/// @note This is a full specialization for @c ArgoProtocolWREM3 type and while
///       it semantically belongs to @c IrArgoAC_WREM3 class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @param[in] mode The desired operation mode.
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
void IRArgoACBase<ArgoProtocolWREM3>::setMode(argoMode_t mode) {
  switch (mode) {
    case argoMode_t::COOL:
    case argoMode_t::DRY:
    case argoMode_t::HEAT:
    case argoMode_t::FAN:
    case argoMode_t::AUTO:
      _.Mode = static_cast<uint8_t>(mode);
      break;
    default:
      _.Mode = static_cast<uint8_t>(argoMode_t::AUTO);
      break;
  }
}

/// @brief Set the desired operation mode.
/// @deprecated
/// @param mode The desired operation mode.
void IRArgoAC::setMode(uint8_t mode) {
  switch (mode) {
    case kArgoCool:
    case kArgoDry:
    case kArgoAuto:
    case kArgoOff:
    case kArgoHeat:
    case kArgoHeatAuto:
      _.Mode = mode;
      break;
    default:
      _.Mode = kArgoAuto;
      break;
  }
}

/// @brief Get the current operation mode
/// @deprecated
/// @return The current operation mode
uint8_t IRArgoAC::getMode() const { return _.Mode;}

argoFan_t IRArgoAC_WREM3::getFan(void) const { return getFanEx(); }
argoFlap_t IRArgoAC_WREM3::getFlap(void) const { return getFlapEx(); }
argoMode_t IRArgoAC_WREM3::getMode(void) const { return getModeEx(); }

/// Turn on/off the Night mode. i.e. Sleep.
/// @param[in] on The desired setting.
template<typename T>
void IRArgoACBase<T>::setNight(const bool on) { _.Night = on; }

/// Get the status of Night mode. i.e. Sleep.
/// @return true if on, false if off.
template<typename T>
bool IRArgoACBase<T>::getNight(void) const { return _.Night; }

/// @brief Turn on/off the Economy mode (lowered power mode)
/// @param[in] on The desired setting.
void IRArgoAC_WREM3::setEco(const bool on) { _.Eco = on; }

/// @brief Get the status of Economy function
/// @return true if on, false if off.
bool IRArgoAC_WREM3::getEco(void) const { return _.Eco; }

/// @brief Turn on/off the Filter mode (not supported by Argo Ulisse)
/// @param[in] on The desired setting.
void IRArgoAC_WREM3::setFilter(const bool on) { _.Filter = on; }

/// @brief Get status of the filter function
/// @return true if on, false if off.
bool IRArgoAC_WREM3::getFilter(void) const { return _.Filter; }

/// @brief Turn on/off the device Lights (LED)
/// @param[in] on The desired setting.
void IRArgoAC_WREM3::setLight(const bool on) { _.Light = on; }

/// @brief Get status of device lights
/// @return true if on, false if off.
bool IRArgoAC_WREM3::getLight(void) const { return _.Light; }

/// @brief Set the IR channel on which to communicate
/// @param[in] channel The desired IR channel.
void IRArgoAC_WREM3::setChannel(const uint8_t channel) {
  _.IrChannel = std::min(channel, kArgoMaxChannel);
}

/// @brief Get the currently set transmission channel
/// @return Channel number
uint8_t IRArgoAC_WREM3::getChannel(void) const { return _.IrChannel;}

/// @brief Set the config data to send
///        Valid only for @c argoIrMessageType_t::CONFIG_PARAM_SET message
/// @param paramId The param ID
/// @param value The value of the parameter
void IRArgoAC_WREM3::setConfigEntry(const uint8_t paramId,
                                    const uint8_t value) {
  _.config.Key = paramId;
  _.config.Value = value;
}

/// @brief Get the config entry previously set
/// @return Key->value pair (paramID: value)
std::pair<uint8_t, uint8_t> IRArgoAC_WREM3::getConfigEntry(void) const {
  return std::make_pair(_.config.Key, _.config.Value);
}

/// Turn on/off the iFeel mode.
/// @param[in] on The desired setting.
template<typename T>
void IRArgoACBase<T>::setiFeel(const bool on) { _.iFeel = on; }

/// Get the status of iFeel mode.
/// @return true if on, false if off.
template<typename T>
bool IRArgoACBase<T>::getiFeel(void) const { return _.iFeel; }

/// @brief Set the message type of the next command (setting this resets state)
/// @param msgType The message type to set
template<typename T>
void IRArgoACBase<T>::setMessageType(const argoIrMessageType_t msgType) {
  stateReset(msgType);
}

/// @brief Get the message type
/// @return Message type currently set
template<typename T>
argoIrMessageType_t IRArgoACBase<T>::getMessageType(void) const {
  return _messageType;
}

/// Set the value for the current room temperature.
/// @note Depending on message type - this will set `sensor` or `roomTemp` value
/// @param[in] degrees The temperature in degrees celsius.
template<typename T>
void IRArgoACBase<T>::setSensorTemp(const uint8_t degrees) {
  uint8_t temp = std::min(degrees, kArgoMaxRoomTemp);
  temp = std::max(temp, kArgoTempDelta) - kArgoTempDelta;
  if (getMessageType() == argoIrMessageType_t::IFEEL_TEMP_REPORT) {
    _.SensorT = temp;
  } else {
    _.RoomTemp = temp;
  }
}

/// Get the currently stored value for the room temperature setting.
/// @note Depending on message type - this will get `sensor` or `roomTemp` value
/// @return The current setting for the room temp. in degrees celsius.
template<typename T>
uint8_t IRArgoACBase<T>::getSensorTemp(void) const {
  if (getMessageType() == argoIrMessageType_t::IFEEL_TEMP_REPORT) {
    return _.SensorT + kArgoTempDelta;
  }
  return _.RoomTemp + kArgoTempDelta;
}

/// @brief Convert a stdAc::ac_command_t enum into its native message type.
/// @param command The enum to be converted.
/// @return The native equivalent of the enum.
template<typename T>
argoIrMessageType_t IRArgoACBase<T>::convertCommand(
      const stdAc::ac_command_t command) {
  switch (command) {
    case stdAc::ac_command_t::kSensorTempReport:
      return argoIrMessageType_t::IFEEL_TEMP_REPORT;
    case stdAc::ac_command_t::kTimerCommand:
      return argoIrMessageType_t::TIMER_COMMAND;
    case stdAc::ac_command_t::kConfigCommand:
      return argoIrMessageType_t::CONFIG_PARAM_SET;
    case stdAc::ac_command_t::kControlCommand:
    default:
      return argoIrMessageType_t::AC_CONTROL;
  }
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
template<typename T>
argoMode_t IRArgoACBase<T>::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool:
      return argoMode_t::COOL;
    case stdAc::opmode_t::kHeat:
      return argoMode_t::HEAT;
    case stdAc::opmode_t::kDry:
      return argoMode_t::DRY;
    case stdAc::opmode_t::kFan:
      return argoMode_t::FAN;
    case stdAc::opmode_t::kAuto:
    default:  // No off mode.
      return argoMode_t::AUTO;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
template<typename T>
argoFan_t IRArgoACBase<T>::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
      return argoFan_t::FAN_LOWEST;
    case stdAc::fanspeed_t::kLow:
      return argoFan_t::FAN_LOWER;
    case stdAc::fanspeed_t::kMedium:
      return argoFan_t::FAN_LOW;
    case stdAc::fanspeed_t::kMediumHigh:
      return argoFan_t::FAN_MEDIUM;
    case stdAc::fanspeed_t::kHigh:
      return argoFan_t::FAN_HIGH;
    case stdAc::fanspeed_t::kMax:
      return argoFan_t::FAN_HIGHEST;
    default:
      return argoFan_t::FAN_AUTO;
  }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
template<typename T>
argoFlap_t IRArgoACBase<T>::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest:
      return argoFlap_t::FLAP_1;
    case stdAc::swingv_t::kHigh:
      return argoFlap_t::FLAP_2;
    case stdAc::swingv_t::kUpperMiddle:
      return argoFlap_t::FLAP_3;
    case stdAc::swingv_t::kMiddle:
      return argoFlap_t::FLAP_4;
    case stdAc::swingv_t::kLow:
      return argoFlap_t::FLAP_5;
    case stdAc::swingv_t::kLowest:
      return argoFlap_t::FLAP_6;
    case stdAc::swingv_t::kOff:  // This is abusing the semantics quite a bit
      return argoFlap_t::FLAP_FULL;
    case stdAc::swingv_t::kAuto:
    default:
      return argoFlap_t::FLAP_AUTO;
  }
}

/// @cond
/// Convert a native flap mode into its stdAc equivalent (WREM2).
/// @note This is a full specialization for @c ArgoProtocol type and while
///       it semantically belongs to @c IrArgoAC class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @param[in] position The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
stdAc::swingv_t IRArgoACBase<ArgoProtocol>::toCommonSwingV(
    const uint8_t position) {
  switch (position) {
    case kArgoFlapFull:
      return stdAc::swingv_t::kHighest;
    case kArgoFlap5:
      return stdAc::swingv_t::kHigh;
    case kArgoFlap4:
      return stdAc::swingv_t::kMiddle;
    case kArgoFlap3:
      return stdAc::swingv_t::kLow;
    case kArgoFlap1:
      return stdAc::swingv_t::kLowest;
    default:
      return stdAc::swingv_t::kAuto;
  }
}
/// @endcond

/// Convert a native flap mode into its stdAc equivalent (WREM3).
/// @note This is a full specialization for @c ArgoProtocolWREM3 type and while
///       it semantically belongs to @c IrArgoAC_WREM3 class impl., it has *not*
///       been pushed there, to avoid having to use a virtual function
/// @param[in] position The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
/// @relates IRArgoACBase\<ARGO_PROTOCOL_T\>
template<>
stdAc::swingv_t IRArgoACBase<ArgoProtocolWREM3>::toCommonSwingV(
    const uint8_t position) {
  switch (static_cast<argoFlap_t>(position)) {
    case argoFlap_t::FLAP_FULL:
      return stdAc::swingv_t::kOff;
    case argoFlap_t::FLAP_6:
      return stdAc::swingv_t::kHighest;
    case argoFlap_t::FLAP_5:
      return stdAc::swingv_t::kHigh;
    case argoFlap_t::FLAP_4:
      return stdAc::swingv_t::kUpperMiddle;
    case argoFlap_t::FLAP_3:
      return stdAc::swingv_t::kMiddle;
    case argoFlap_t::FLAP_2:
      return stdAc::swingv_t::kLow;
    case argoFlap_t::FLAP_1:
      return stdAc::swingv_t::kLowest;
    case argoFlap_t::FLAP_AUTO:
    default:
      return stdAc::swingv_t::kAuto;
  }
}

/// Convert a native message type into its stdAc equivalent.
/// @param[in] command The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
template<typename T>
stdAc::ac_command_t IRArgoACBase<T>::toCommonCommand(
      const argoIrMessageType_t command) {
  switch (command) {
    case argoIrMessageType_t::AC_CONTROL:
        return stdAc::ac_command_t::kControlCommand;
    case argoIrMessageType_t::IFEEL_TEMP_REPORT:
        return stdAc::ac_command_t::kSensorTempReport;
    case argoIrMessageType_t::TIMER_COMMAND:
        return stdAc::ac_command_t::kTimerCommand;
    case argoIrMessageType_t::CONFIG_PARAM_SET:
        return stdAc::ac_command_t::kConfigCommand;
    default:
        return stdAc::ac_command_t::kControlCommand;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
template<typename T>
stdAc::opmode_t IRArgoACBase<T>::toCommonMode(const argoMode_t mode) {
  switch (mode) {
    case argoMode_t::COOL: return stdAc::opmode_t::kCool;
    case argoMode_t::DRY : return stdAc::opmode_t::kDry;
    case argoMode_t::FAN : return stdAc::opmode_t::kFan;
    case argoMode_t::HEAT : return stdAc::opmode_t::kHeat;
    case argoMode_t::AUTO : return stdAc::opmode_t::kAuto;
    default: return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
template<typename T>
stdAc::fanspeed_t IRArgoACBase<T>::toCommonFanSpeed(const argoFan_t speed) {
  switch (speed) {
    case argoFan_t::FAN_AUTO: return stdAc::fanspeed_t::kAuto;
    case argoFan_t::FAN_HIGHEST: return stdAc::fanspeed_t::kMax;
    case argoFan_t::FAN_HIGH: return stdAc::fanspeed_t::kHigh;
    case argoFan_t::FAN_MEDIUM: return stdAc::fanspeed_t::kMediumHigh;
    case argoFan_t::FAN_LOW: return stdAc::fanspeed_t::kMedium;
    case argoFan_t::FAN_LOWER: return stdAc::fanspeed_t::kLow;
    case argoFan_t::FAN_LOWEST: return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRArgoAC::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::ARGO;
  result.model = argo_ac_remote_model_t::SAC_WREM2;
  result.command = toCommonCommand(_messageType);
  result.power = _.Power;
  result.mode = toCommonMode(getModeEx());
  result.celsius = true;
  result.degrees = getTemp();
  result.sensorTemperature = getSensorTemp();
  result.iFeel = getiFeel();
  result.fanspeed = toCommonFanSpeed(getFanEx());
  result.turbo = _.Max;
  result.sleep = _.Night ? 0 : -1;
  // Not supported.
  result.swingv = stdAc::swingv_t::kOff;
  result.swingh = stdAc::swingh_t::kOff;
  result.light = false;
  result.filter = false;
  result.econo = false;
  result.quiet = false;
  result.clean = false;
  result.beep = false;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRArgoAC_WREM3::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::ARGO;
  result.model = argo_ac_remote_model_t::SAC_WREM3;
  result.command = toCommonCommand(_messageType);
  result.power = getPower();
  result.mode = toCommonMode(getModeEx());
  result.celsius = true;
  result.degrees = getTemp();
  result.sensorTemperature = getSensorTemp();
  result.iFeel = getiFeel();
  result.fanspeed = toCommonFanSpeed(getFanEx());
  result.turbo = _.Max;
  result.swingv = toCommonSwingV(_.Flap);
  result.light = getLight();
  result.filter = getFilter();
  result.econo = getEco();
  result.quiet = getNight();
  result.beep = (_messageType != argoIrMessageType_t::IFEEL_TEMP_REPORT);

  result.clock = -1;
  result.sleep = _.Night ? 0 : -1;
  if (_messageType == argoIrMessageType_t::TIMER_COMMAND) {
    result.clock = getCurrentTimeMinutes();
    result.sleep = getDelayTimerMinutes();
  }

  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.clean = false;


  return result;
}


namespace {
  /// @brief Short-hand for casting enum to its underlying storage type
  /// @tparam E The type of enum
  /// @param e Enum value
  /// @return Type of underlying value
  template <typename E>
  constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
      return static_cast<typename std::underlying_type<E>::type>(e);
  }
}

/// Convert the current internal state into a human readable string (WREM2).
/// @return A human readable string.
String IRArgoAC::toString(void) const {
  String result = "";
  result.reserve(118);  // Reserve some heap for the string to reduce fragging.
    // E.g.:  Model: 1 (WREM2), Power: On, Mode: 0 (Cool), Fan: 0 (Auto),
    //        Temp: 20C, Room Temp: 21C, Max: On, IFeel: On, Night: On
  result += addModelToString(decode_type_t::ARGO,
                             argo_ac_remote_model_t::SAC_WREM2, false);
  if (_messageType == argoIrMessageType_t::IFEEL_TEMP_REPORT) {
    result += addIntToString(getSensorTemp(), kSensorTempStr);
    result += 'C';
  } else {
    result += addBoolToString(_.Power, kPowerStr);
    result += addIntToString(_.Mode, kModeStr);
    result += kSpaceLBraceStr;
    switch (_.Mode) {
      case kArgoAuto:
        result += kAutoStr;
        break;
      case kArgoCool:
        result += kCoolStr;
        break;
      case kArgoHeat:
        result += kHeatStr;
        break;
      case kArgoDry:
        result += kDryStr;
        break;
      case kArgoHeatAuto:
        result += kHeatStr;
        result += ' ';
        result += kAutoStr;
        break;
      case kArgoOff:
        result += kOffStr;
        break;
      default:
        result += kUnknownStr;
    }
    result += ')';
    result += addIntToString(_.Fan, kFanStr);
    result += kSpaceLBraceStr;
    switch (_.Fan) {
      case kArgoFanAuto:
        result += kAutoStr;
        break;
      case kArgoFan3:
        result += kMaxStr;
        break;
      case kArgoFan1:
        result += kMinStr;
        break;
      case kArgoFan2:
        result += kMedStr;
        break;
      default:
        result += kUnknownStr;
    }
    result += ')';
    result += addTempToString(getTemp());
    result += addTempToString(getSensorTemp(), true, true, true);
    result += addBoolToString(_.Max, kMaxStr);
    result += addBoolToString(_.iFeel, kIFeelStr);
    result += addBoolToString(_.Night, kNightStr);
  }
  return result;
}

/// @brief Set current clock (as minutes, counted from 0:00)
///        E.g. 13:38 becomes 818 (13*60+38)
/// @param currentTimeMinutes Current time (in minutes)
void IRArgoAC_WREM3::setCurrentTimeMinutes(uint16_t currentTimeMinutes) {
  uint16_t time = std::min(currentTimeMinutes, static_cast<uint16_t>(23*60+59));
  _.timer.CurrentTimeHi = (time >> 4);
  _.timer.CurrentTimeLo = (time & 0b1111);
}

/// @brief Retrieve current time
/// @return Current time as minutes from 0:00
uint16_t IRArgoAC_WREM3::getCurrentTimeMinutes(void) const {
  return (_.timer.CurrentTimeHi << 4)  + _.timer.CurrentTimeLo;
}

/// @brief Set current day of week
/// @param dayOfWeek Current day of week
void IRArgoAC_WREM3::setCurrentDayOfWeek(argoWeekday dayOfWeek) {
  uint8_t day = std::min(to_underlying(dayOfWeek),
                         to_underlying(argoWeekday::SATURDAY));
  _.timer.CurrentWeekdayHi = (day >> 1);
  _.timer.CurrentWeekdayLo = (day & 0b1);
}

/// @brief Get current day of week
/// @return Current day of week
argoWeekday IRArgoAC_WREM3::getCurrentDayOfWeek(void) const {
  return static_cast<argoWeekday>((_.timer.CurrentWeekdayHi << 1) +
                                  _.timer.CurrentWeekdayLo);
}

/// @brief Set timer type
/// @param timerType Timer type to use OFF | DELAY | SCHEDULE<1|2|3>
/// @note 2 timer types supported: delay | schedule timer
///       - @c DELAY_TIMER requires setting @c setDelayTimerMinutes
///         and @c setCurrentTimeMinutes and (optionally) @c setCurrentDayOfWeek
///       - @c SCHEDULE_TIMER<x> requires setting:
///            @c setScheduleTimerStartMinutes
///            @c setScheduleTimerStopMinutes
///            @c setScheduleTimerActiveDays
///          as well as current time *and* day
///          @c setCurrentTimeMinutes and @c setCurrentDayOfWeek
void IRArgoAC_WREM3::setTimerType(const argoTimerType_t timerType) {
  if (timerType > argoTimerType_t::SCHEDULE_TIMER_3) {
    _.timer.TimerType = to_underlying(argoTimerType_t::NO_TIMER);
  } else {
    _.timer.TimerType = to_underlying(timerType);
  }
}

/// @brief Get currently set timer type
/// @return Timer type
argoTimerType_t IRArgoAC_WREM3::getTimerType(void) const {
  return static_cast<argoTimerType_t>(_.timer.TimerType);
}

/// @brief Set delay timer delay in minutes (10-minute increments only)
///        Max is 1190 (19h50m)
/// @note The delay timer also accepts current device state: set by @c setPower
/// @param delayMinutes Delay minutes
void IRArgoAC_WREM3::setDelayTimerMinutes(const uint16_t delayMinutes) {
  const uint16_t DELAY_TIMER_MAX = 19*60+50;
  uint16_t time = std::min(delayMinutes, DELAY_TIMER_MAX);

  // only full 10 minute increments are allowed
  time = static_cast<uint16_t>((time / 10.0) + 0.5) * 10;

  _.timer.DelayTimeHi = (time >> 6);
  _.timer.DelayTimeLo = (time & 0b111111);
}

/// @brief Get current delay timer value
/// @return Delay timer value (in minutes)
uint16_t IRArgoAC_WREM3::getDelayTimerMinutes(void) const {
  return (_.timer.DelayTimeHi << 6) + _.timer.DelayTimeLo;
}

/// @brief Set schedule timer on time (time when the device should turn on)
///        (10-minute increments only)
/// @param startTimeMinutes Time when the device should turn itself on
///                         expressed as # of minutes counted from 0:00
///                         The value is in 10-minute increments (rounded)
///                         E.g. 13:38 becomes 820 (13:40 in minutes)
void IRArgoAC_WREM3::setScheduleTimerStartMinutes(
    const uint16_t startTimeMinutes) {
  const uint16_t SCHEDULE_TIMER_MAX = 23*60+50;
  uint16_t time = std::min(startTimeMinutes, SCHEDULE_TIMER_MAX);

  // only full 10 minute increments are allowed
  time = static_cast<uint16_t>((time / 10.0) + 0.5) * 10;

  _.timer.TimerStartHi = (time >> 3);
  _.timer.TimerStartLo = (time & 0b111);
}

/// @brief Get schedule timer ON time
/// @return Schedule on time (as # of minutes from 0:00)
uint16_t IRArgoAC_WREM3::getScheduleTimerStartMinutes(void) const {
  return (_.timer.TimerStartHi << 3) + _.timer.TimerStartLo;
}

/// @brief Set schedule timer off time (time when the device should turn off)
///        (10-minute increments only)
/// @param stopTimeMinutes Time when the device should turn itself off
///                        expressed as # of minutes counted from 0:00
///                        The value is in 10-minute increments (rounded)
///                        E.g. 13:38 becomes 820 (13:40 in minutes)
void IRArgoAC_WREM3::setScheduleTimerStopMinutes(
    const uint16_t stopTimeMinutes) {
  const uint16_t SCHEDULE_TIMER_MAX = 23*60+50;
  uint16_t time = std::min(stopTimeMinutes, SCHEDULE_TIMER_MAX);

  // only full 10 minute increments are allowed
  time = static_cast<uint16_t>((time / 10.0) + 0.5) * 10;

  _.timer.TimerEndHi = (time >> 8);
  _.timer.TimerEndLo = (time & 0b11111111);
}

/// @brief Get schedule timer OFF time
/// @return Schedule off time (as # of minutes from 0:00)
uint16_t IRArgoAC_WREM3::getScheduleTimerStopMinutes(void) const {
  return (_.timer.TimerEndHi << 8) + _.timer.TimerEndLo;
}

/// @brief Get the days when shedule timer shall be active (as bitmap)
/// @return Days when schedule timer is active, as raw bitmap type
///         where bit[0] is Sunday, bit[1] -> Monday, ...
uint8_t IRArgoAC_WREM3::getTimerActiveDaysBitmap(void) const {
  return (_.timer.TimerActiveDaysHi << 5) + _.timer.TimerActiveDaysLo;
}

/// @brief Set the days when the schedule timer shall be active
/// @param days A set of days when the timer shall run
void IRArgoAC_WREM3::setScheduleTimerActiveDays(
    const std::set<argoWeekday>& days) {
  uint8_t daysBitmap = 0;
  for (const argoWeekday& day : days) {
    daysBitmap |= (0b1 << to_underlying(day));
  }
  _.timer.TimerActiveDaysHi = (daysBitmap >> 5);
  _.timer.TimerActiveDaysLo = (daysBitmap & 0b11111);
}

/// @brief Get the days when shedule timer shall be active (as set)
/// @return Days when the schedule timer runs
std::set<argoWeekday> IRArgoAC_WREM3::getScheduleTimerActiveDays(void) const {
  std::set<argoWeekday> result = {};
  uint8_t daysBitmap = getTimerActiveDaysBitmap();
  for (uint8_t i = to_underlying(argoWeekday::SUNDAY);
       i <= to_underlying(argoWeekday::SATURDAY);
       ++i) {
    if (((daysBitmap >> i) & 0b1) == 0b1) {
      result.insert(static_cast<argoWeekday>(i));
    }
  }
  return result;
}

/// @brief Get device model
/// @return Device model
argo_ac_remote_model_t IRArgoAC_WREM3::getModel() const {
  return argo_ac_remote_model_t::SAC_WREM3;
}

namespace {
  String commandTypeToString(argoIrMessageType_t type, uint8_t channel) {
    String result = irutils::irCommandTypeToString(to_underlying(type),
        to_underlying(argoIrMessageType_t::AC_CONTROL),
        to_underlying(argoIrMessageType_t::IFEEL_TEMP_REPORT),
        to_underlying(argoIrMessageType_t::TIMER_COMMAND),
        to_underlying(argoIrMessageType_t::CONFIG_PARAM_SET));
    result += irutils::channelToString(channel);
    result += kColonSpaceStr;
    return result;
  }
}  // namespace

/// Convert the current internal state into a human readable string (WREM3).
/// @return A human readable string.
String IRArgoAC_WREM3::toString(void) const {
  String result = "";
  result.reserve(190);  // Reserve some heap for the string to reduce fragging.
  // E.g.:  Command[CH#0]: Model: 2 (WREM3), Power: On, Mode: 1 (Cool),
  //        Temp: 22C, Room: 26C, Fan: 0 (Auto), Swing(V): 7 (Breeze),
  //        IFeel: Off, Night: Off, Econo: Off, Max: Off, Filter: Off, Light: On
  //        Temp: 20C, Room Temp: 21C, Max: On, IFeel: On, Night: On

  argoIrMessageType_t commandType = this->getMessageType();
  argo_ac_remote_model_t model = getModel();

  result += commandTypeToString(commandType, getChannel());
  result += addModelToString(decode_type_t::ARGO, model, false);

  switch (commandType) {
    case argoIrMessageType_t::IFEEL_TEMP_REPORT:
      result += addTempToString(getSensorTemp(), true, true, true);
      break;

    case argoIrMessageType_t::AC_CONTROL:
      result += addBoolToString(getPower(), kPowerStr);
      result += addModeToString(to_underlying(getModeEx()),
                                to_underlying(argoMode_t::AUTO),
                                to_underlying(argoMode_t::COOL),
                                to_underlying(argoMode_t::HEAT),
                                to_underlying(argoMode_t::DRY),
                                to_underlying(argoMode_t::FAN));
      result += addTempToString(getTemp());
      result += addTempToString(getSensorTemp(), true, true, true);
      result += addFanToString(to_underlying(getFanEx()),
                              to_underlying(argoFan_t::FAN_HIGH),
                              to_underlying(argoFan_t::FAN_LOWER),
                              to_underlying(argoFan_t::FAN_AUTO),
                              to_underlying(argoFan_t::FAN_LOWEST),
                              to_underlying(argoFan_t::FAN_LOW),
                              to_underlying(argoFan_t::FAN_HIGHEST),
                              to_underlying(argoFan_t::FAN_MEDIUM));
      result += addSwingVToString(to_underlying(getFlapEx()),
                                  to_underlying(argoFlap_t::FLAP_AUTO),
                                  to_underlying(argoFlap_t::FLAP_1),
                                  to_underlying(argoFlap_t::FLAP_2),
                                  to_underlying(argoFlap_t::FLAP_3),
                                  to_underlying(argoFlap_t::FLAP_4), -1,
                                  to_underlying(argoFlap_t::FLAP_5),
                                  to_underlying(argoFlap_t::FLAP_6), -1, -1,
                                  to_underlying(argoFlap_t::FLAP_FULL), -1);
      result += addBoolToString(getiFeel(), kIFeelStr);
      result += addBoolToString(getNight(), kNightStr);
      result += addBoolToString(getEco(), kEconoStr);
      result += addBoolToString(getMax(), kMaxStr);  // Turbo
      result += addBoolToString(getFilter(), kFilterStr);
      result += addBoolToString(getLight(), kLightStr);
      break;

  case argoIrMessageType_t::TIMER_COMMAND:
    result += addBoolToString(_.timer.IsOn, kPowerStr);
    result += addTimerModeToString(to_underlying(getTimerType()),
        to_underlying(argoTimerType_t::NO_TIMER),
        to_underlying(argoTimerType_t::DELAY_TIMER),
        to_underlying(argoTimerType_t::SCHEDULE_TIMER_1),
        to_underlying(argoTimerType_t::SCHEDULE_TIMER_2),
        to_underlying(argoTimerType_t::SCHEDULE_TIMER_3));
    result += addLabeledString(minsToString(getCurrentTimeMinutes()),
                               kClockStr);
    result += addDayToString(to_underlying(getCurrentDayOfWeek()));
    switch (getTimerType()) {
      case argoTimerType_t::NO_TIMER:
        result += addLabeledString(kOffStr, kTimerStr);
        break;
      case argoTimerType_t::DELAY_TIMER:
        result += addLabeledString(minsToString(getDelayTimerMinutes()),
                                   kTimerStr);
        break;
      default:
        result += addLabeledString(minsToString(getScheduleTimerStartMinutes()),
                                   kOnTimerStr);
        result += addLabeledString(minsToString(getScheduleTimerStopMinutes()),
                                   kOffTimerStr);

        result += addLabeledString(daysBitmaskToString(
          getTimerActiveDaysBitmap()), kTimerActiveDaysStr);
        break;
    }
    break;

  case argoIrMessageType_t::CONFIG_PARAM_SET:
    result += addIntToString(_.config.Key, kKeyStr);
    result += addIntToString(_.config.Value, kValueStr);
    break;
  }

  return result;
}

/// @brief Check if raw ARGO state starts with valid WREM3 preamble
/// @param state The state bytes
/// @param length Length of state in bytes
/// @return True if state starts wiht valid WREM3 preamble, False otherwise
bool IRArgoAC_WREM3::hasValidPreamble(const uint8_t state[],
                                      const uint16_t length) {
  if (length < 1) {
    return false;
  }
  uint8_t preamble = state[0] & 0x0F;
  return preamble == kArgoWrem3Preamble;
}

#if DECODE_ARGO
/// Decode the supplied Argo message (WREM2).
/// Status: BETA / Probably works.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @note This decoder is based soley off sendArgo(). We have no actual captures
///  to test this against. If you have one of these units, please let us know.
bool IRrecv::decodeArgo(decode_results *results, uint16_t offset,
                        const uint16_t nbits,
                        const bool strict) {
  if (strict && nbits != kArgoBits) return false;

  // Match Header + Data
  if (!matchGeneric(results->rawbuf + offset, results->state,
                    results->rawlen - offset, nbits,
                    kArgoHdrMark, kArgoHdrSpace,
                    kArgoBitMark, kArgoOneSpace,
                    kArgoBitMark, kArgoZeroSpace,
                    0, 0,  // Footer (None, allegedly. This seems very wrong.)
                    true, _tolerance, 0, false)) return false;

  // Compliance
  // Verify we got a valid checksum.
  if (strict && !IRArgoAC::validChecksum(results->state, kArgoStateLength)) {
    return false;
  }
  // Success
  results->decode_type = decode_type_t::ARGO;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}

/// Decode the supplied Argo message (WREM3).
/// Status: Confirmed working w/ Argo 13 ECO (WREM-3)
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @note This decoder is separate from @c decodeArgo to maintain backwards
///       compatibility. Contrary to WREM2, this expects a footer and gap!
bool IRrecv::decodeArgoWREM3(decode_results *results, uint16_t offset,
                             const uint16_t nbits,
                             const bool strict) {
  if (strict
      && nbits != kArgo3AcControlStateLength * 8
      && nbits != kArgo3ConfigStateLength * 8
      && nbits != kArgo3iFeelReportStateLength * 8
      && nbits != kArgo3TimerStateLength * 8) {
    return false;
  }

  uint16_t bytesRead = matchGeneric(results->rawbuf + offset, results->state,
                  results->rawlen - offset, nbits,
                  kArgoHdrMark, kArgoHdrSpace,
                  kArgoBitMark, kArgoOneSpace,
                  kArgoBitMark, kArgoZeroSpace,
                  kArgoBitMark, kArgoGap,  // difference vs decodeArgo
                  true, _tolerance, 0,
                  false);
  if (!bytesRead) {
    return false;
  }

  // If 'strict', assert it is a valid WREM-3 'model' protocolar message
  // vs. just 'any ARGO'
  if (strict &&
      !IRArgoAC_WREM3::isValidWrem3Message(results->state, nbits, true)) {
    return false;
  }

  // Success: Matched ARGO protocol and WREM3-model
  // Note that unfortunately decode_type does not allow to persist model...
  // so we will be re-detecting it later :)
  results->decode_type = decode_type_t::ARGO;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}

/// @brief Detects if an ARGO protocol message is a WREM-3 sub-type (model)
/// @param state The raw IR decore state
/// @param nbits The length of @c state **IN BITS**
/// @param verifyChecksum Whether to perform checksum verification
/// @return True if the message is a WREM-3 one
bool IRArgoAC_WREM3::isValidWrem3Message(const uint8_t state[],
                                         const uint16_t nbits,
                                         bool verifyChecksum) {
  if ((nbits % 8) != 0) {
    return false;  // WREM-3 protocol always has a full byte length commands
  }
  uint16_t stateLengthBytes = std::min(static_cast<uint16_t>(nbits / 8),
                                       kStateSizeMax);
  if (!IRArgoAC_WREM3::hasValidPreamble(state, stateLengthBytes)) {
    return false;
  }

  argoIrMessageType_t messageType = IRArgoACBase<ArgoProtocolWREM3>
      ::getMessageType(state, stateLengthBytes);

  switch (messageType) {
    case argoIrMessageType_t::AC_CONTROL :
      if (stateLengthBytes != kArgo3AcControlStateLength) { return false; }
      break;
  case argoIrMessageType_t::CONFIG_PARAM_SET:
      if (stateLengthBytes != kArgo3ConfigStateLength) { return false; }
      break;
  case argoIrMessageType_t::TIMER_COMMAND:
      if (stateLengthBytes != kArgo3TimerStateLength) { return false; }
      break;
    case argoIrMessageType_t::IFEEL_TEMP_REPORT:
      if (stateLengthBytes != kArgo3iFeelReportStateLength) { return false; }
      break;
    default:
      return false;
  }

  // Compliance: Verify we got a valid checksum.
  if (verifyChecksum &&
      !IRArgoAC_WREM3::validChecksum(state, stateLengthBytes)) {
    return false;
  }
  return true;
}

#endif  // DECODE_ARGO


// force template instantiation
template class IRArgoACBase<ArgoProtocol>;
template class IRArgoACBase<ArgoProtocolWREM3>;
