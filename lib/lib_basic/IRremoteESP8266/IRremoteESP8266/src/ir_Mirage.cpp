// Copyright 2020-2021 David Conran (crankyoldgit)
/// @file
/// @brief Support for Mirage protocol
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1289
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1573


#include "ir_Mirage.h"
#include <algorithm>
#include <cstring>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addModelToString;
using irutils::addSwingHToString;
using irutils::addSwingVToString;
using irutils::addTempToString;
using irutils::addToggleToString;
using irutils::minsToString;
using irutils::bcdToUint8;
using irutils::uint8ToBcd;
using irutils::sumNibbles;

// Constants
const uint16_t kMirageHdrMark = 8360;            ///< uSeconds
const uint16_t kMirageBitMark = 554;             ///< uSeconds
const uint16_t kMirageHdrSpace = 4248;           ///< uSeconds
const uint16_t kMirageOneSpace = 1592;           ///< uSeconds
const uint16_t kMirageZeroSpace = 545;           ///< uSeconds
const uint32_t kMirageGap = kDefaultMessageGap;  ///< uSeconds (just a guess)
const uint16_t kMirageFreq = 38000;              ///< Hz. (Just a guess)

const uint8_t kMirageAcKKG29AC1PowerOn  = 0b00;  // 0
const uint8_t kMirageAcKKG29AC1PowerOff = 0b11;  // 3


#if SEND_MIRAGE
/// Send a Mirage formatted message.
/// Status: STABLE / Reported as working.
/// @param[in] data An array of bytes containing the IR command.
/// @param[in] nbytes Nr. of bytes of data in the array. (>=kMirageStateLength)
/// @param[in] repeat Nr. of times the message is to be repeated.
void IRsend::sendMirage(const uint8_t data[], const uint16_t nbytes,
                        const uint16_t repeat) {
  sendGeneric(kMirageHdrMark, kMirageHdrSpace,
              kMirageBitMark, kMirageOneSpace,
              kMirageBitMark, kMirageZeroSpace,
              kMirageBitMark, kMirageGap,
              data, nbytes, kMirageFreq, false,  // LSB
              repeat, kDutyDefault);
}
#endif  // SEND_MIRAGE

#if DECODE_MIRAGE
/// Decode the supplied Mirage message.
/// Status: STABLE / Reported as working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeMirage(decode_results *results, uint16_t offset,
                          const uint16_t nbits, const bool strict) {
  if (strict && nbits != kMirageBits) return false;  // Compliance.

  if (!matchGeneric(results->rawbuf + offset, results->state,
                    results->rawlen - offset, nbits,
                    kMirageHdrMark, kMirageHdrSpace,
                    kMirageBitMark, kMirageOneSpace,
                    kMirageBitMark, kMirageZeroSpace,
                    kMirageBitMark, kMirageGap, true,
                    kUseDefTol, kMarkExcess, false)) return false;
  // Compliance
  if (strict && !IRMirageAc::validChecksum(results->state)) return false;

  // Success
  results->decode_type = decode_type_t::MIRAGE;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}

// Code to emulate Mirage A/C IR remote control unit.

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRMirageAc::IRMirageAc(const uint16_t pin, const bool inverted,
                               const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the state of the remote to a known good state/sequence.
void IRMirageAc::stateReset(void) {
  // The state of the IR remote in IR code form.
  static const uint8_t kReset[kMirageStateLength] = {
      0x56, 0x6C, 0x00, 0x00, 0x20, 0x1A, 0x00, 0x00,
      0x0C, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x42};
  setRaw(kReset);
  _model = mirage_ac_remote_model_t::KKG9AC1;
}

/// Set up hardware to be able to send a message.
void IRMirageAc::begin(void) { _irsend.begin(); }

#if SEND_MITSUBISHI_AC
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRMirageAc::send(const uint16_t repeat) {
  _irsend.sendMirage(getRaw(), kMirageStateLength, repeat);
  // Reset any toggles after a send.
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      setCleanToggle(false);
      setLight(false);  // For this model (only), Light is a toggle.
      break;
    default:
      break;
  }
}
#endif  // SEND_MITSUBISHI_AC

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRMirageAc::getRaw(void) {
  checksum();
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] data A valid code for this protocol.
void IRMirageAc::setRaw(const uint8_t *data) {
  std::memcpy(_.raw, data, kMirageStateLength);
  _model = getModel(true);
}

/// Guess the Mirage remote model from the supplied state code.
/// @param[in] state A valid state code for this protocol.
/// @return The model code.
/// @note This result isn't perfect. Both protocols can look the same but have
///       wildly different settings.
mirage_ac_remote_model_t IRMirageAc::getModel(const uint8_t *state) {
  Mirage120Protocol p;
  std::memcpy(p.raw, state, kMirageStateLength);
  // Check for KKG29AC1 specific settings.
  if (p.RecycleHeat || p.Filter || p.Sleep_Kkg29ac1 || p.CleanToggle ||
      p.IFeel || p.OffTimerEnable || p.OnTimerEnable)
    return mirage_ac_remote_model_t::KKG29AC1;
  // Check for things specific to KKG9AC1
  if ((p.Minutes || p.Seconds) ||  // Is part of the clock set?
      // Are the timer times set, but not enabled? (enable check filtered above)
      (p.OffTimerHours || p.OffTimerMins) ||
      (p.OnTimerHours || p.OnTimerMins))
    return mirage_ac_remote_model_t::KKG9AC1;
  // As the above test has a 1 in 3600+ (for 1 second an hour) chance of a false
  // negative in theory, we are going assume that anything left should be a
  // KKG29AC1 model.
  return mirage_ac_remote_model_t::KKG29AC1;  // Default.
}

/// Get the model code of the interal message state.
/// @param[in] useRaw If set, we try to get the model info from just the state.
/// @return The model code.
mirage_ac_remote_model_t IRMirageAc::getModel(const bool useRaw) const {
  return useRaw ? getModel(_.raw) : _model;
}

/// Set the model code of the interal message state.
/// @param[in] model The desired model to use for the settings.
void IRMirageAc::setModel(const mirage_ac_remote_model_t model) {
  if (model != _model) {  // Only change things if we need to.
    // Save the old settings.
    stdAc::state_t state = toCommon();
    const uint16_t ontimer = getOnTimer();
    const uint16_t offtimer = getOffTimer();
    const bool ifeel = getIFeel();
    const uint8_t sensor = getSensorTemp();
    // Change the model.
    state.model = model;
    // Restore/Convert the settings.
    fromCommon(state);
    setOnTimer(ontimer);
    setOffTimer(offtimer);
    setIFeel(ifeel);
    setSensorTemp(sensor);
  }
}

/// Calculate and set the checksum values for the internal state.
void IRMirageAc::checksum(void) { _.Sum = calculateChecksum(_.raw); }

/// Verify the checksum is valid for a given state.
/// @param[in] data The array to verify the checksum of.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRMirageAc::validChecksum(const uint8_t *data) {
  return calculateChecksum(data) == data[kMirageStateLength - 1];
}

/// Calculate the checksum for a given state.
/// @param[in] data The value to calc the checksum of.
/// @return The calculated checksum value.
uint8_t IRMirageAc::calculateChecksum(const uint8_t *data) {
  return sumNibbles(data, kMirageStateLength - 1);
}

/// Set the requested power state of the A/C to on.
void IRMirageAc::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRMirageAc::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMirageAc::setPower(bool on) {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      _.Power = on ? kMirageAcKKG29AC1PowerOn : kMirageAcKKG29AC1PowerOff;
      break;
    default:
      // In order to change the power setting, it seems must be less than
      // kMirageAcPowerOff. kMirageAcPowerOff is larger than half of the
      // possible value stored in the allocated bit space.
      // Thus if the value is larger than kMirageAcPowerOff the power is off.
      // Less than, then power is on.
      // We can't just aribitarily add or subtract the value (which analysis
      // indicates is how the power status changes. Very weird, I know!) as that
      // is not an idempotent action, we must check if the addition or
      // substraction is needed first. e.g. via getPower()
      // i.e. If we added or subtracted twice, we would cause a wrap of the
      // integer and not get the desired result.
      if (on)
        _.SwingAndPower -= getPower() ? 0 : kMirageAcPowerOff;
      else
        _.SwingAndPower += getPower() ? kMirageAcPowerOff : 0;
  }
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRMirageAc::getPower(void) const {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      return _.Power == kMirageAcKKG29AC1PowerOn;
    default:
      return _.SwingAndPower < kMirageAcPowerOff;
  }
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRMirageAc::getMode(void) const { return _.Mode; }

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRMirageAc::setMode(const uint8_t mode) {
  switch (mode) {
    case kMirageAcCool:
    case kMirageAcDry:
    case kMirageAcHeat:
    case kMirageAcFan:
    case kMirageAcRecycle:
      _.Mode = mode;
      // Reset turbo if we have to.
      setTurbo(getTurbo());
      break;
    default:  // Default to cool mode for anything else.
      setMode(kMirageAcCool);
  }
}

/// Set the temperature.
/// @param[in] degrees The temperature in degrees celsius.
void IRMirageAc::setTemp(const uint8_t degrees) {
  // Make sure we have desired temp in the correct range.
  uint8_t celsius = std::max(degrees, kMirageAcMinTemp);
  _.Temp = std::min(celsius, kMirageAcMaxTemp) + kMirageAcTempOffset;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRMirageAc::getTemp(void) const { return _.Temp - kMirageAcTempOffset; }

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRMirageAc::setFan(const uint8_t speed) {
  _.Fan = (speed <= kMirageAcFanLow) ? speed : kMirageAcFanAuto;
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRMirageAc::getFan(void) const { return _.Fan; }

/// Change the Turbo setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMirageAc::setTurbo(bool on) {
  const bool value = (on && (getMode() == kMirageAcCool));
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      _.Turbo_Kkg29ac1 = value;
      break;
    default:
      _.Turbo_Kkg9ac1 = value;
  }
}

/// Get the value of the current Turbo setting.
/// @return true, the setting is on. false, the setting is off.
bool IRMirageAc::getTurbo(void) const {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1: return _.Turbo_Kkg29ac1;
    default:                                 return _.Turbo_Kkg9ac1;
  }
}

/// Change the Sleep setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMirageAc::setSleep(bool on) {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      _.Sleep_Kkg29ac1 = on;
      break;
    default:
      _.Sleep_Kkg9ac1 = on;
  }
}

/// Get the value of the current Sleep setting.
/// @return true, the setting is on. false, the setting is off.
bool IRMirageAc::getSleep(void) const {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1: return _.Sleep_Kkg29ac1;
    default:                                 return _.Sleep_Kkg9ac1;
  }
}

/// Change the Light/Display setting.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note Light is a toggle on the KKG29AC1 model.
void IRMirageAc::setLight(bool on) {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      _.LightToggle_Kkg29ac1 = on;
      break;
    default:
      _.Light_Kkg9ac1 = on;
  }
}

/// Get the value of the current Light/Display setting.
/// @return true, the setting is on. false, the setting is off.
/// @note Light is a toggle on the KKG29AC1 model.
bool IRMirageAc::getLight(void) const {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1: return _.LightToggle_Kkg29ac1;
    default:                                 return _.Light_Kkg9ac1;
  }
}

/// Get the clock time of the A/C unit.
/// @return Nr. of seconds past midnight.
uint32_t IRMirageAc::getClock(void) const {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      return 0;
    default:
      return ((bcdToUint8(_.Hours) * 60) + bcdToUint8(_.Minutes)) * 60 +
          bcdToUint8(_.Seconds);
  }
}

/// Set the clock time on the A/C unit.
/// @param[in] nr_of_seconds Nr. of seconds past midnight.
void IRMirageAc::setClock(const uint32_t nr_of_seconds) {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      _.Minutes = _.Seconds = 0;  // No clock setting. Clear it just in case.
      break;
    default:
      uint32_t remaining = std::min(
          nr_of_seconds, (uint32_t)(24 * 60 * 60 - 1));  // Limit to 23:59:59.
      _.Seconds = uint8ToBcd(remaining % 60);
      remaining /= 60;
      _.Minutes = uint8ToBcd(remaining % 60);
      remaining /= 60;
      _.Hours = uint8ToBcd(remaining);
  }
}

/// Set the Vertical Swing setting/position of the A/C.
/// @param[in] position The desired swing setting.
void IRMirageAc::setSwingV(const uint8_t position) {
  switch (position) {
    case kMirageAcSwingVOff:
    case kMirageAcSwingVLowest:
    case kMirageAcSwingVLow:
    case kMirageAcSwingVMiddle:
    case kMirageAcSwingVHigh:
    case kMirageAcSwingVHighest:
    case kMirageAcSwingVAuto:
      switch (_model) {
        case mirage_ac_remote_model_t::KKG29AC1:
          _.SwingV = (position != kMirageAcSwingVOff);
          break;
        default:
          const bool power = getPower();
          _.SwingAndPower = position;
          // Power needs to be reapplied after overwriting SwingAndPower
          setPower(power);
      }
      break;
    default:  // Default to Auto for anything else.
      setSwingV(kMirageAcSwingVAuto);
  }
}

/// Get the Vertical Swing setting/position of the A/C.
/// @return The desired Vertical Swing setting/position.
uint8_t IRMirageAc::getSwingV(void) const {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      return _.SwingV ? kMirageAcSwingVAuto : kMirageAcSwingVOff;
    default:
      return _.SwingAndPower - (getPower() ? 0 : kMirageAcPowerOff);
  }
}

/// Set the Horizontal Swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMirageAc::setSwingH(const bool on) {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      _.SwingH = on;
      break;
    default:
      break;
  }
}

/// Get the Horizontal Swing setting of the A/C.
/// @return on true, the setting is on. false, the setting is off.
bool IRMirageAc::getSwingH(void) const {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1: return _.SwingH;
    default:                                 return false;
  }
}

/// Set the Quiet setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMirageAc::setQuiet(const bool on) {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      _.Quiet = on;
      break;
    default:
      break;
  }
}

/// Get the Quiet setting of the A/C.
/// @return on true, the setting is on. false, the setting is off.
bool IRMirageAc::getQuiet(void) const {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1: return _.Quiet;
    default:                                 return false;
  }
}

/// Set the CleanToggle setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMirageAc::setCleanToggle(const bool on) {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      _.CleanToggle = on;
      break;
    default:
      break;
  }
}

/// Get the Clean Toggle setting of the A/C.
/// @return on true, the setting is on. false, the setting is off.
bool IRMirageAc::getCleanToggle(void) const {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1: return _.CleanToggle;
    default:                                 return false;
  }
}

/// Set the Filter setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMirageAc::setFilter(const bool on) {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      _.Filter = on;
      break;
    default:
      break;
  }
}

/// Get the Filter setting of the A/C.
/// @return on true, the setting is on. false, the setting is off.
bool IRMirageAc::getFilter(void) const {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1: return _.Filter;
    default:                                 return false;
  }
}

/// Set the IFeel setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMirageAc::setIFeel(const bool on) {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      _.IFeel = on;
      if (on) {
        // If no previous sensor temp, default to currently desired temp.
        if (!_.SensorTemp) _.SensorTemp = getTemp();
      } else {
        _.SensorTemp = 0;  // When turning it off, clear the Sensor Temp.
      }
      break;
    default:
      break;
  }
}

/// Get the IFeel setting of the A/C.
/// @return on true, the setting is on. false, the setting is off.
bool IRMirageAc::getIFeel(void) const {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1: return _.IFeel;
    default:                                 return false;
  }
}

/// Set the Sensor Temp setting of the A/C's remote.
/// @param[in] degrees The desired sensor temp. in degrees celsius.
void IRMirageAc::setSensorTemp(const uint8_t degrees) {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      _.SensorTemp = std::min(kMirageAcSensorTempMax, degrees) +
          kMirageAcSensorTempOffset;
      break;
    default:
      break;
  }
}

/// Get the Sensor Temp setting of the A/C's remote.
/// @return The current setting for the sensor temp. in degrees celsius.
uint16_t IRMirageAc::getSensorTemp(void) const {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      return _.SensorTemp - kMirageAcSensorTempOffset;
    default:
      return false;
  }
}

/// Get the number of minutes the On Timer is currently set for.
/// @return Nr. of Minutes the timer is set for. 0, is the timer is not in use.
uint16_t IRMirageAc::getOnTimer(void) const {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      return _.OnTimerEnable ? _.OnTimerHours * 60 + _.OnTimerMins : 0;
    default:
      return 0;
  }
}

/// Set the number of minutes for the On Timer.
/// @param[in] nr_of_mins How long to set the timer for. 0 disables the timer.
void IRMirageAc::setOnTimer(const uint16_t nr_of_mins) {
  uint16_t mins = std::min(nr_of_mins, (uint16_t)(24 * 60));
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      _.OnTimerEnable = (mins > 0);
      _.OnTimerHours = mins / 60;
      _.OnTimerMins = mins % 60;
      break;
    default:
      break;
  }
}

/// Get the number of minutes the Off Timer is currently set for.
/// @return Nr. of Minutes the timer is set for. 0, is the timer is not in use.
uint16_t IRMirageAc::getOffTimer(void) const {
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      return _.OffTimerEnable ? _.OffTimerHours * 60 + _.OffTimerMins : 0;
    default:
      return 0;
  }
}

/// Set the number of minutes for the Off Timer.
/// @param[in] nr_of_mins How long to set the timer for. 0 disables the timer.
void IRMirageAc::setOffTimer(const uint16_t nr_of_mins) {
  uint16_t mins = std::min(nr_of_mins, (uint16_t)(24 * 60));
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      _.OffTimerEnable = (mins > 0);
      _.OffTimerHours = mins / 60;
      _.OffTimerMins = mins % 60;
      break;
    default:
      break;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRMirageAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kMirageAcHeat: return stdAc::opmode_t::kHeat;
    case kMirageAcDry:  return stdAc::opmode_t::kDry;
    case kMirageAcFan:  return stdAc::opmode_t::kFan;
    default:            return stdAc::opmode_t::kCool;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @param[in] model The model type to use to influence the conversion.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRMirageAc::toCommonFanSpeed(const uint8_t speed,
    const mirage_ac_remote_model_t model) {
  switch (model) {
      case mirage_ac_remote_model_t::KKG29AC1:
      switch (speed) {
        case kMirageAcKKG29AC1FanHigh:  return stdAc::fanspeed_t::kHigh;
        case kMirageAcKKG29AC1FanMed:   return stdAc::fanspeed_t::kMedium;
        case kMirageAcKKG29AC1FanLow:   return stdAc::fanspeed_t::kLow;
        default:                        return stdAc::fanspeed_t::kAuto;
      }
      break;
    default:
      switch (speed) {
        case kMirageAcFanHigh:  return stdAc::fanspeed_t::kHigh;
        case kMirageAcFanMed:   return stdAc::fanspeed_t::kMedium;
        case kMirageAcFanLow:   return stdAc::fanspeed_t::kLow;
        default:                return stdAc::fanspeed_t::kAuto;
      }
  }
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMirageAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kHeat: return kMirageAcHeat;
    case stdAc::opmode_t::kDry:  return kMirageAcDry;
    case stdAc::opmode_t::kFan:  return kMirageAcFan;
    default:                     return kMirageAcCool;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @param[in] model The model type to use to influence the conversion.
/// @return The native equivalent of the enum.
uint8_t IRMirageAc::convertFan(const stdAc::fanspeed_t speed,
                               const mirage_ac_remote_model_t model) {
  uint8_t low;
  uint8_t med;
  switch (model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      low = kMirageAcKKG29AC1FanLow;
      med = kMirageAcKKG29AC1FanMed;
      break;
    default:
      low = kMirageAcFanLow;
      med = kMirageAcFanMed;
  }
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return low;
    case stdAc::fanspeed_t::kMedium: return med;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kMirageAcFanHigh;
    default:                         return kMirageAcFanAuto;
  }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMirageAc::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest:     return kMirageAcSwingVHighest;
    case stdAc::swingv_t::kHigh:        return kMirageAcSwingVHigh;
    case stdAc::swingv_t::kMiddle:      return kMirageAcSwingVMiddle;
    case stdAc::swingv_t::kLow:         return kMirageAcSwingVLow;
    case stdAc::swingv_t::kLowest:      return kMirageAcSwingVLowest;
    case stdAc::swingv_t::kOff:         return kMirageAcSwingVOff;
    default:                            return kMirageAcSwingVAuto;
  }
}

/// Convert a native vertical swing postion to it's common equivalent.
/// @param[in] pos A native position to convert.
/// @return The common vertical swing position.
stdAc::swingv_t IRMirageAc::toCommonSwingV(const uint8_t pos) {
  switch (pos) {
    case kMirageAcSwingVHighest: return stdAc::swingv_t::kHighest;
    case kMirageAcSwingVHigh:    return stdAc::swingv_t::kHigh;
    case kMirageAcSwingVMiddle:  return stdAc::swingv_t::kMiddle;
    case kMirageAcSwingVLow:     return stdAc::swingv_t::kLow;
    case kMirageAcSwingVLowest:  return stdAc::swingv_t::kLowest;
    case kMirageAcSwingVAuto:    return stdAc::swingv_t::kAuto;
    default:                     return stdAc::swingv_t::kOff;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRMirageAc::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::MIRAGE;
  result.model = _model;
  result.power = getPower();
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.sensorTemperature = getSensorTemp();
  result.fanspeed = toCommonFanSpeed(getFan(), _model);
  result.swingv = toCommonSwingV(getSwingV());
  result.swingh = getSwingH() ? stdAc::swingh_t::kAuto : stdAc::swingh_t::kOff;
  result.turbo = getTurbo();
  result.light = getLight();
  result.clean = getCleanToggle();
  result.filter = getFilter();
  result.sleep = getSleep() ? 0 : -1;
  result.quiet = getQuiet();
  result.clock = getClock() / 60;
  result.iFeel = getIFeel();
  // Not supported.
  result.econo = false;
  result.beep = false;
  return result;
}

/// Convert & set a stdAc::state_t to its equivalent internal settings.
/// @param[in] state The desired state in stdAc::state_t form.
void IRMirageAc::fromCommon(const stdAc::state_t state) {
  stateReset();
  _model = (mirage_ac_remote_model_t)state.model;  // Set directly to avoid loop
  setPower(state.power);
  setTemp(state.celsius ? state.degrees : fahrenheitToCelsius(state.degrees));
  setMode(convertMode(state.mode));
  setFan(convertFan(state.fanspeed, _model));
  setTurbo(state.turbo);
  setSleep(state.sleep >= 0);
  setLight(state.light);
  setSwingV(convertSwingV(state.swingv));
  setSwingH(state.swingh != stdAc::swingh_t::kOff);
  setQuiet(state.quiet);
  setCleanToggle(state.clean);
  setFilter(state.filter);
  // setClock() expects seconds, not minutes.
  setClock((state.clock > 0) ? state.clock * 60 : 0);
  setIFeel(state.iFeel);
  if (state.sensorTemperature != kNoTempValue) {
    setSensorTemp(state.celsius ? state.sensorTemperature
                                : fahrenheitToCelsius(state.sensorTemperature));
  }
  // Non-common settings.
  setOnTimer(0);
  setOffTimer(0);
}

/// Convert the internal state into a human readable string.
/// @return A string containing the settings in human-readable form.
String IRMirageAc::toString(void) const {
  String result = "";
  result.reserve(240);  // Reserve some heap for the string to reduce fragging.
  result += addModelToString(decode_type_t::MIRAGE, _model, false);
  result += addBoolToString(getPower(), kPowerStr);
  result += addModeToString(_.Mode, 0xFF, kMirageAcCool,
                            kMirageAcHeat, kMirageAcDry,
                            kMirageAcFan);
  result += addTempToString(getTemp());
  uint8_t fanlow;
  uint8_t fanmed;
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      fanlow = kMirageAcKKG29AC1FanLow;
      fanmed = kMirageAcKKG29AC1FanMed;
      break;
    default:  // e.g. Model KKG9AC1
      fanlow = kMirageAcFanLow;
      fanmed = kMirageAcFanMed;
  }
  result += addFanToString(_.Fan, kMirageAcFanHigh, fanlow, kMirageAcFanAuto,
                           kMirageAcFanAuto, fanmed);
  result += addBoolToString(getTurbo(), kTurboStr);
  result += addBoolToString(getSleep(), kSleepStr);
  switch (_model) {
    case mirage_ac_remote_model_t::KKG29AC1:
      result += addBoolToString(_.Quiet, kQuietStr);
      result += addToggleToString(getLight(), kLightStr);
      result += addBoolToString(_.SwingV, kSwingVStr);
      result += addBoolToString(_.SwingH, kSwingHStr);
      result += addBoolToString(_.Filter, kFilterStr);
      result += addToggleToString(_.CleanToggle, kCleanStr);
      result += addLabeledString(getOnTimer() ? minsToString(getOnTimer())
                                              : kOffStr,
                                 kOnTimerStr);
      result += addLabeledString(getOffTimer() ? minsToString(getOffTimer())
                                               : kOffStr,
                                 kOffTimerStr);
      result += addBoolToString(_.IFeel, kIFeelStr);
      if (_.IFeel) {
        result += addIntToString(getSensorTemp(), kSensorTempStr);
        result += 'C';
      }
      break;
    default:  // e.g. Model KKG9AC1
      result += addBoolToString(getLight(), kLightStr);
      result += addSwingVToString(getSwingV(),
                                  kMirageAcSwingVAuto,
                                  kMirageAcSwingVHighest,
                                  kMirageAcSwingVHigh,
                                  0xFF,  // Unused.
                                  kMirageAcSwingVMiddle,
                                  0xFF,  // Unused.
                                  kMirageAcSwingVLow,
                                  kMirageAcSwingVLowest,
                                  kMirageAcSwingVOff,
                                  0xFF, 0xFF, 0xFF);  // Unused.
      result += addLabeledString(minsToString(getClock() / 60), kClockStr);
  }
  return result;
}
#endif  // DECODE_MIRAGE
