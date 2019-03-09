// Copyright 2016 David Conran
//
// Code to emulate IR Kelvinator YALIF remote control unit, which should control
// at least the following Kelvinator A/C units:
// KSV26CRC, KSV26HRC, KSV35CRC, KSV35HRC, KSV53HRC, KSV62HRC, KSV70CRC,
// KSV70HRC, KSV80HRC.
//
// Note:
// * Unsupported:
//    - All Sleep modes.
//    - All Timer modes.
//    - "I Feel" button & mode.
//    - Energy Saving mode.
//    - Low Heat mode.
//    - Fahrenheit.

#include "ir_Kelvinator.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// KK  KK EEEEEEE LL     VV     VV IIIII NN   NN   AAA   TTTTTTT  OOOOO  RRRRRR
// KK KK  EE      LL     VV     VV  III  NNN  NN  AAAAA    TTT   OO   OO RR   RR
// KKKK   EEEEE   LL      VV   VV   III  NN N NN AA   AA   TTT   OO   OO RRRRRR
// KK KK  EE      LL       VV VV    III  NN  NNN AAAAAAA   TTT   OO   OO RR  RR
// KK  KK EEEEEEE LLLLLLL   VVV    IIIII NN   NN AA   AA   TTT    OOOO0  RR   RR

// Constants

const uint16_t kKelvinatorTick = 85;
const uint16_t kKelvinatorHdrMarkTicks = 106;
const uint16_t kKelvinatorHdrMark = kKelvinatorHdrMarkTicks * kKelvinatorTick;
const uint16_t kKelvinatorHdrSpaceTicks = 53;
const uint16_t kKelvinatorHdrSpace = kKelvinatorHdrSpaceTicks * kKelvinatorTick;
const uint16_t kKelvinatorBitMarkTicks = 8;
const uint16_t kKelvinatorBitMark = kKelvinatorBitMarkTicks * kKelvinatorTick;
const uint16_t kKelvinatorOneSpaceTicks = 18;
const uint16_t kKelvinatorOneSpace = kKelvinatorOneSpaceTicks * kKelvinatorTick;
const uint16_t kKelvinatorZeroSpaceTicks = 6;
const uint16_t kKelvinatorZeroSpace =
    kKelvinatorZeroSpaceTicks * kKelvinatorTick;
const uint16_t kKelvinatorGapSpaceTicks = 235;
const uint16_t kKelvinatorGapSpace = kKelvinatorGapSpaceTicks * kKelvinatorTick;

const uint8_t kKelvinatorCmdFooter = 2;
const uint8_t kKelvinatorCmdFooterBits = 3;

const uint8_t kKelvinatorPower = 8;
const uint8_t kKelvinatorModeMask = 0xF8;
const uint8_t kKelvinatorFanOffset = 4;
const uint8_t kKelvinatorBasicFanMask = 0xFF ^ (3U << kKelvinatorFanOffset);
const uint8_t kKelvinatorFanMask = 0xFF ^ (7U << kKelvinatorFanOffset);
const uint8_t kKelvinatorChecksumStart = 10;
const uint8_t kKelvinatorVentSwingOffset = 6;
const uint8_t kKelvinatorVentSwing = 1 << kKelvinatorVentSwingOffset;
const uint8_t kKelvinatorVentSwingV = 1;
const uint8_t kKelvinatorVentSwingH = 1 << 4;
const uint8_t kKelvinatorSleep1And3 = 1 << 7;
const uint8_t kKelvinatorQuietOffset = 7;
const uint8_t kKelvinatorQuiet = 1 << kKelvinatorQuietOffset;
const uint8_t kKelvinatorIonFilterOffset = 6;
const uint8_t kKelvinatorIonFilter = 1 << kKelvinatorIonFilterOffset;
const uint8_t kKelvinatorLightOffset = 5;
const uint8_t kKelvinatorLight = 1 << kKelvinatorLightOffset;
const uint8_t kKelvinatorXfanOffset = 7;
const uint8_t kKelvinatorXfan = 1 << kKelvinatorXfanOffset;
const uint8_t kKelvinatorTurboOffset = 4;
const uint8_t kKelvinatorTurbo = 1 << kKelvinatorTurboOffset;

#if SEND_KELVINATOR
// Send a Kelvinator A/C message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=kKelvinatorStateLength)
//   repeat: Nr. of times the message is to be repeated. (Default = 0).
//
// Status: STABLE / Known working.
//
void IRsend::sendKelvinator(unsigned char data[], uint16_t nbytes,
                            uint16_t repeat) {
  if (nbytes < kKelvinatorStateLength)
    return;  // Not enough bytes to send a proper message.

  for (uint16_t r = 0; r <= repeat; r++) {
    // Command Block #1 (4 bytes)
    sendGeneric(kKelvinatorHdrMark, kKelvinatorHdrSpace, kKelvinatorBitMark,
                kKelvinatorOneSpace, kKelvinatorBitMark, kKelvinatorZeroSpace,
                0, 0,  // No Footer yet.
                data, 4, 38, false, 0, 50);
    // Send Footer for the command block (3 bits (b010))
    sendGeneric(0, 0,  // No Header
                kKelvinatorBitMark, kKelvinatorOneSpace, kKelvinatorBitMark,
                kKelvinatorZeroSpace, kKelvinatorBitMark, kKelvinatorGapSpace,
                kKelvinatorCmdFooter, kKelvinatorCmdFooterBits, 38, false, 0,
                50);
    // Data Block #1 (4 bytes)
    sendGeneric(0, 0,  // No header
                kKelvinatorBitMark, kKelvinatorOneSpace, kKelvinatorBitMark,
                kKelvinatorZeroSpace, kKelvinatorBitMark,
                kKelvinatorGapSpace * 2, data + 4, 4, 38, false, 0, 50);
    // Command Block #2 (4 bytes)
    sendGeneric(kKelvinatorHdrMark, kKelvinatorHdrSpace, kKelvinatorBitMark,
                kKelvinatorOneSpace, kKelvinatorBitMark, kKelvinatorZeroSpace,
                0, 0,  // No Footer yet.
                data + 8, 4, 38, false, 0, 50);
    // Send Footer for the command block (3 bits (B010))
    sendGeneric(0, 0,  // No Header
                kKelvinatorBitMark, kKelvinatorOneSpace, kKelvinatorBitMark,
                kKelvinatorZeroSpace, kKelvinatorBitMark, kKelvinatorGapSpace,
                kKelvinatorCmdFooter, kKelvinatorCmdFooterBits, 38, false, 0,
                50);
    // Data Block #2 (4 bytes)
    sendGeneric(0, 0,  // No header
                kKelvinatorBitMark, kKelvinatorOneSpace, kKelvinatorBitMark,
                kKelvinatorZeroSpace, kKelvinatorBitMark,
                kKelvinatorGapSpace * 2, data + 12, 4, 38, false, 0, 50);
  }
}
#endif  // SEND_KELVINATOR

IRKelvinatorAC::IRKelvinatorAC(uint16_t pin) : _irsend(pin) { stateReset(); }

void IRKelvinatorAC::stateReset() {
  for (uint8_t i = 0; i < kKelvinatorStateLength; i++) remote_state[i] = 0x0;
  remote_state[3] = 0x50;
  remote_state[11] = 0x70;
}

void IRKelvinatorAC::begin() { _irsend.begin(); }

void IRKelvinatorAC::fixup() {
  // X-Fan mode is only valid in COOL or DRY modes.
  if (getMode() != kKelvinatorCool && getMode() != kKelvinatorDry)
    setXFan(false);
  checksum();  // Calculate the checksums
}

#if SEND_KELVINATOR
void IRKelvinatorAC::send() {
  fixup();  // Ensure correct settings before sending.
  _irsend.sendKelvinator(remote_state);
}
#endif  // SEND_KELVINATOR

uint8_t *IRKelvinatorAC::getRaw() {
  fixup();  // Ensure correct settings before sending.
  return remote_state;
}

void IRKelvinatorAC::setRaw(uint8_t new_code[]) {
  for (uint8_t i = 0; i < kKelvinatorStateLength; i++) {
    remote_state[i] = new_code[i];
  }
}

uint8_t IRKelvinatorAC::calcBlockChecksum(const uint8_t *block,
                                          const uint16_t length) {
  uint8_t sum = kKelvinatorChecksumStart;
  // Sum the lower half of the first 4 bytes of this block.
  for (uint8_t i = 0; i < 4 && i < length - 1; i++, block++)
    sum += (*block & 0x0FU);
  // then sum the upper half of the next 3 bytes.
  for (uint8_t i = 4; i < length - 1; i++, block++) sum += (*block >> 4);
  // Trim it down to fit into the 4 bits allowed. i.e. Mod 16.
  return sum & 0x0FU;
}

// Many Bothans died to bring us this information.
void IRKelvinatorAC::checksum(const uint16_t length) {
  // For each command + options block.
  for (uint16_t offset = 0; offset + 7 < length; offset += 8) {
    uint8_t sum = calcBlockChecksum(remote_state + offset);
    remote_state[7 + offset] = (sum << 4) | (remote_state[7 + offset] & 0xFU);
  }
}

// Verify the checksum is valid for a given state.
// Args:
//   state:  The array to verify the checksum of.
//   length: The size of the state.
// Returns:
//   A boolean.
bool IRKelvinatorAC::validChecksum(const uint8_t state[],
                                   const uint16_t length) {
  for (uint16_t offset = 0; offset + 7 < length; offset += 8) {
    // Top 4 bits of the last byte in the block is the block's checksum.
    if (state[offset + 7] >> 4 != calcBlockChecksum(state + offset))
      return false;
  }
  return true;
}

void IRKelvinatorAC::on() {
  remote_state[0] |= kKelvinatorPower;
  remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
}

void IRKelvinatorAC::off() {
  remote_state[0] &= ~kKelvinatorPower;
  remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
}

void IRKelvinatorAC::setPower(bool state) {
  if (state)
    on();
  else
    off();
}

bool IRKelvinatorAC::getPower() {
  return ((remote_state[0] & kKelvinatorPower) != 0);
}

// Set the temp. in deg C
void IRKelvinatorAC::setTemp(uint8_t temp) {
  temp = std::max(kKelvinatorMinTemp, temp);
  temp = std::min(kKelvinatorMaxTemp, temp);
  remote_state[1] = (remote_state[1] & 0xF0U) | (temp - kKelvinatorMinTemp);
  remote_state[9] = remote_state[1];  // Duplicate to the 2nd command chunk.
}

// Return the set temp. in deg C
uint8_t IRKelvinatorAC::getTemp() {
  return ((remote_state[1] & 0xFU) + kKelvinatorMinTemp);
}

// Set the speed of the fan, 0-5, 0 is auto, 1-5 is the speed
void IRKelvinatorAC::setFan(uint8_t fan) {
  fan = std::min(kKelvinatorFanMax, fan);  // Bounds check

  // Only change things if we need to.
  if (fan != getFan()) {
    // Set the basic fan values.
    uint8_t fan_basic = std::min(kKelvinatorBasicFanMax, fan);
    remote_state[0] = (remote_state[0] & kKelvinatorBasicFanMask) |
                      (fan_basic << kKelvinatorFanOffset);
    remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
    // Set the advanced(?) fan value.
    remote_state[14] =
        (remote_state[14] & kKelvinatorFanMask) | (fan << kKelvinatorFanOffset);
    setTurbo(false);  // Turbo mode is turned off if we change the fan settings.
  }
}

uint8_t IRKelvinatorAC::getFan() {
  return ((remote_state[14] & ~kKelvinatorFanMask) >> kKelvinatorFanOffset);
}

uint8_t IRKelvinatorAC::getMode() {
  return (remote_state[0] & ~kKelvinatorModeMask);
}

void IRKelvinatorAC::setMode(uint8_t mode) {
  // If we get an unexpected mode, default to AUTO.
  if (mode > kKelvinatorHeat) mode = kKelvinatorAuto;
  remote_state[0] = (remote_state[0] & kKelvinatorModeMask) | mode;
  remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
  if (mode == kKelvinatorAuto || kKelvinatorDry)
    // When the remote is set to Auto or Dry, it defaults to 25C and doesn't
    // show it.
    setTemp(kKelvinatorAutoTemp);
}

void IRKelvinatorAC::setSwingVertical(bool state) {
  if (state) {
    remote_state[0] |= kKelvinatorVentSwing;
    remote_state[4] |= kKelvinatorVentSwingV;
  } else {
    remote_state[4] &= ~kKelvinatorVentSwingV;
    if (!getSwingHorizontal()) remote_state[0] &= ~kKelvinatorVentSwing;
  }
  remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getSwingVertical() {
  return ((remote_state[4] & kKelvinatorVentSwingV) != 0);
}

void IRKelvinatorAC::setSwingHorizontal(bool state) {
  if (state) {
    remote_state[0] |= kKelvinatorVentSwing;
    remote_state[4] |= kKelvinatorVentSwingH;
  } else {
    remote_state[4] &= ~kKelvinatorVentSwingH;
    if (!getSwingVertical()) remote_state[0] &= ~kKelvinatorVentSwing;
  }
  remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getSwingHorizontal() {
  return ((remote_state[4] & kKelvinatorVentSwingH) != 0);
}

void IRKelvinatorAC::setQuiet(bool state) {
  remote_state[12] &= ~kKelvinatorQuiet;
  remote_state[12] |= (state << kKelvinatorQuietOffset);
}

bool IRKelvinatorAC::getQuiet() {
  return ((remote_state[12] & kKelvinatorQuiet) != 0);
}

void IRKelvinatorAC::setIonFilter(bool state) {
  remote_state[2] &= ~kKelvinatorIonFilter;
  remote_state[2] |= (state << kKelvinatorIonFilterOffset);
  remote_state[10] = remote_state[2];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getIonFilter() {
  return ((remote_state[2] & kKelvinatorIonFilter) != 0);
}

void IRKelvinatorAC::setLight(bool state) {
  remote_state[2] &= ~kKelvinatorLight;
  remote_state[2] |= (state << kKelvinatorLightOffset);
  remote_state[10] = remote_state[2];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getLight() {
  return ((remote_state[2] & kKelvinatorLight) != 0);
}

// Note: XFan mode is only valid in Cool or Dry mode.
void IRKelvinatorAC::setXFan(bool state) {
  remote_state[2] &= ~kKelvinatorXfan;
  remote_state[2] |= (state << kKelvinatorXfanOffset);
  remote_state[10] = remote_state[2];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getXFan() {
  return ((remote_state[2] & kKelvinatorXfan) != 0);
}

// Note: Turbo mode is turned off if the fan speed is changed.
void IRKelvinatorAC::setTurbo(bool state) {
  remote_state[2] &= ~kKelvinatorTurbo;
  remote_state[2] |= (state << kKelvinatorTurboOffset);
  remote_state[10] = remote_state[2];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getTurbo() {
  return ((remote_state[2] & kKelvinatorTurbo) != 0);
}

// Convert the internal state into a human readable string.
#ifdef ARDUINO
String IRKelvinatorAC::toString() {
  String result = "";
#else
std::string IRKelvinatorAC::toString() {
  std::string result = "";
#endif  // ARDUINO
  result += "Power: ";
  if (getPower())
    result += "On";
  else
    result += "Off";
  result += ", Mode: " + uint64ToString(getMode());
  switch (getMode()) {
    case kKelvinatorAuto:
      result += " (AUTO)";
      break;
    case kKelvinatorCool:
      result += " (COOL)";
      break;
    case kKelvinatorHeat:
      result += " (HEAT)";
      break;
    case kKelvinatorDry:
      result += " (DRY)";
      break;
    case kKelvinatorFan:
      result += " (FAN)";
      break;
    default:
      result += " (UNKNOWN)";
  }
  result += ", Temp: " + uint64ToString(getTemp()) + "C";
  result += ", Fan: " + uint64ToString(getFan());
  switch (getFan()) {
    case kKelvinatorFanAuto:
      result += " (AUTO)";
      break;
    case kKelvinatorFanMax:
      result += " (MAX)";
      break;
  }
  result += ", Turbo: ";
  if (getTurbo())
    result += "On";
  else
    result += "Off";
  result += ", Quiet: ";
  if (getQuiet())
    result += "On";
  else
    result += "Off";
  result += ", XFan: ";
  if (getXFan())
    result += "On";
  else
    result += "Off";
  result += ", IonFilter: ";
  if (getIonFilter())
    result += "On";
  else
    result += "Off";
  result += ", Light: ";
  if (getLight())
    result += "On";
  else
    result += "Off";
  result += ", Swing (Horizontal): ";
  if (getSwingHorizontal())
    result += "On";
  else
    result += "Off";
  result += ", Swing (Vertical): ";
  if (getSwingVertical())
    result += "On";
  else
    result += "Off";
  return result;
}

#if DECODE_KELVINATOR
// Decode the supplied Kelvinator message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kKelvinatorBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: ALPHA / Untested.
bool IRrecv::decodeKelvinator(decode_results *results, uint16_t nbits,
                              bool strict) {
  if (results->rawlen <
      2 * (nbits + kKelvinatorCmdFooterBits) + (kHeader + kFooter + 1) * 2 - 1)
    return false;  // Can't possibly be a valid Kelvinator message.
  if (strict && nbits != kKelvinatorBits)
    return false;  // Not strictly a Kelvinator message.

  uint32_t data;
  uint16_t offset = kStartOffset;

  // There are two messages back-to-back in a full Kelvinator IR message
  // sequence.
  int8_t state_pos = 0;
  for (uint8_t s = 0; s < 2; s++) {
    match_result_t data_result;

    // Header
    if (!matchMark(results->rawbuf[offset], kKelvinatorHdrMark)) return false;
    // Calculate how long the lowest tick time is based on the header mark.
    uint32_t mark_tick =
        results->rawbuf[offset++] * kRawTick / kKelvinatorHdrMarkTicks;
    if (!matchSpace(results->rawbuf[offset], kKelvinatorHdrSpace)) return false;
    // Calculate how long the common tick time is based on the header space.
    uint32_t space_tick =
        results->rawbuf[offset++] * kRawTick / kKelvinatorHdrSpaceTicks;

    // Data (Command) (32 bits)
    data_result = matchData(
        &(results->rawbuf[offset]), 32, kKelvinatorBitMarkTicks * mark_tick,
        kKelvinatorOneSpaceTicks * space_tick,
        kKelvinatorBitMarkTicks * mark_tick,
        kKelvinatorZeroSpaceTicks * space_tick, kTolerance, kMarkExcess, false);
    if (data_result.success == false) return false;
    data = data_result.data;
    offset += data_result.used;

    // Record command data in the state.
    for (uint16_t i = 0; i < 4; i++, data >>= 8)
      results->state[state_pos + i] = data & 0xFF;
    state_pos += 4;

    // Command data footer (3 bits, B010)
    data_result = matchData(
        &(results->rawbuf[offset]), kKelvinatorCmdFooterBits,
        kKelvinatorBitMarkTicks * mark_tick,
        kKelvinatorOneSpaceTicks * space_tick,
        kKelvinatorBitMarkTicks * mark_tick,
        kKelvinatorZeroSpaceTicks * space_tick, kTolerance, kMarkExcess, false);
    if (data_result.success == false) return false;
    if (data_result.data != kKelvinatorCmdFooter) return false;
    offset += data_result.used;

    // Interdata gap.
    if (!matchMark(results->rawbuf[offset++],
                   kKelvinatorBitMarkTicks * mark_tick))
      return false;
    if (!matchSpace(results->rawbuf[offset++],
                    kKelvinatorGapSpaceTicks * space_tick))
      return false;

    // Data (Options) (32 bits)
    data_result = matchData(
        &(results->rawbuf[offset]), 32, kKelvinatorBitMarkTicks * mark_tick,
        kKelvinatorOneSpaceTicks * space_tick,
        kKelvinatorBitMarkTicks * mark_tick,
        kKelvinatorZeroSpaceTicks * space_tick, kTolerance, kMarkExcess, false);
    if (data_result.success == false) return false;
    data = data_result.data;
    offset += data_result.used;

    // Record option data in the state.
    for (uint16_t i = 0; i < 4; i++, data >>= 8)
      results->state[state_pos + i] = data & 0xFF;
    state_pos += 4;

    // Inter-sequence gap. (Double length gap)
    if (!matchMark(results->rawbuf[offset++],
                   kKelvinatorBitMarkTicks * mark_tick))
      return false;
    if (s == 0) {
      if (!matchSpace(results->rawbuf[offset++],
                      kKelvinatorGapSpaceTicks * space_tick * 2))
        return false;
    } else {
      if (offset <= results->rawlen &&
          !matchAtLeast(results->rawbuf[offset],
                        kKelvinatorGapSpaceTicks * 2 * space_tick))
        return false;
    }
  }

  // Compliance
  if (strict) {
    // Correct size/length)
    if (state_pos != kKelvinatorStateLength) return false;
    // Verify the message's checksum is correct.
    if (!IRKelvinatorAC::validChecksum(results->state)) return false;
  }

  // Success
  results->decode_type = KELVINATOR;
  results->bits = state_pos * 8;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_KELVINATOR
