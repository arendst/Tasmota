// Copyright 2009 Ken Shirriff
// Copyright 2017 David Conran

#include "ir_Samsung.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

//              SSSS   AAA    MMM    SSSS  U   U  N   N   GGGG
//             S      A   A  M M M  S      U   U  NN  N  G
//              SSS   AAAAA  M M M   SSS   U   U  N N N  G  GG
//                 S  A   A  M   M      S  U   U  N  NN  G   G
//             SSSS   A   A  M   M  SSSS    UUU   N   N   GGG

// Samsung originally added from https://github.com/shirriff/Arduino-IRremote/

// Constants
// Ref:
//   http://elektrolab.wz.cz/katalog/samsung_protocol.pdf
const uint16_t kSamsungTick = 560;
const uint16_t kSamsungHdrMarkTicks = 8;
const uint16_t kSamsungHdrMark = kSamsungHdrMarkTicks * kSamsungTick;
const uint16_t kSamsungHdrSpaceTicks = 8;
const uint16_t kSamsungHdrSpace = kSamsungHdrSpaceTicks * kSamsungTick;
const uint16_t kSamsungBitMarkTicks = 1;
const uint16_t kSamsungBitMark = kSamsungBitMarkTicks * kSamsungTick;
const uint16_t kSamsungOneSpaceTicks = 3;
const uint16_t kSamsungOneSpace = kSamsungOneSpaceTicks * kSamsungTick;
const uint16_t kSamsungZeroSpaceTicks = 1;
const uint16_t kSamsungZeroSpace = kSamsungZeroSpaceTicks * kSamsungTick;
const uint16_t kSamsungRptSpaceTicks = 4;
const uint16_t kSamsungRptSpace = kSamsungRptSpaceTicks * kSamsungTick;
const uint16_t kSamsungMinMessageLengthTicks = 193;
const uint32_t kSamsungMinMessageLength =
    kSamsungMinMessageLengthTicks * kSamsungTick;
const uint16_t kSamsungMinGapTicks =
    kSamsungMinMessageLengthTicks -
    (kSamsungHdrMarkTicks + kSamsungHdrSpaceTicks +
     kSamsungBits * (kSamsungBitMarkTicks + kSamsungOneSpaceTicks) +
     kSamsungBitMarkTicks);
const uint32_t kSamsungMinGap = kSamsungMinGapTicks * kSamsungTick;

const uint16_t kSamsungAcHdrMark = 690;
const uint16_t kSamsungAcHdrSpace = 17844;
const uint8_t kSamsungAcSections = 2;
const uint16_t kSamsungAcSectionMark = 3086;
const uint16_t kSamsungAcSectionSpace = 8864;
const uint16_t kSamsungAcSectionGap = 2886;
const uint16_t kSamsungAcBitMark = 586;
const uint16_t kSamsungAcOneSpace = 1432;
const uint16_t kSamsungAcZeroSpace = 436;

#if SEND_SAMSUNG
// Send a Samsung formatted message.
// Samsung has a separate message to indicate a repeat, like NEC does.
// TODO(crankyoldgit): Confirm that is actually how Samsung sends a repeat.
//                     The refdoc doesn't indicate it is true.
//
// Args:
//   data:   The message to be sent.
//   nbits:  The bit size of the message being sent. typically kSamsungBits.
//   repeat: The number of times the message is to be repeated.
//
// Status: BETA / Should be working.
//
// Ref: http://elektrolab.wz.cz/katalog/samsung_protocol.pdf
void IRsend::sendSAMSUNG(uint64_t data, uint16_t nbits, uint16_t repeat) {
  sendGeneric(kSamsungHdrMark, kSamsungHdrSpace, kSamsungBitMark,
              kSamsungOneSpace, kSamsungBitMark, kSamsungZeroSpace,
              kSamsungBitMark, kSamsungMinGap, kSamsungMinMessageLength, data,
              nbits, 38, true, repeat, 33);
}

// Construct a raw Samsung message from the supplied customer(address) &
// command.
//
// Args:
//   customer: The customer code. (aka. Address)
//   command:  The command code.
// Returns:
//   A raw 32-bit Samsung message suitable for sendSAMSUNG().
//
// Status: BETA / Should be working.
uint32_t IRsend::encodeSAMSUNG(uint8_t customer, uint8_t command) {
  customer = reverseBits(customer, sizeof(customer) * 8);
  command = reverseBits(command, sizeof(command) * 8);
  return ((command ^ 0xFF) | (command << 8) | (customer << 16) |
          (customer << 24));
}
#endif

#if DECODE_SAMSUNG
// Decode the supplied Samsung message.
// Samsung messages whilst 32 bits in size, only contain 16 bits of distinct
// data. e.g. In transmition order:
//   customer_byte + customer_byte(same) + address_byte + invert(address_byte)
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of bits to expect in the data portion. Typically kSamsungBits.
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE
//
// Note:
//   LG 32bit protocol appears near identical to the Samsung protocol.
//   They differ on their compliance criteria and how they repeat.
// Ref:
//  http://elektrolab.wz.cz/katalog/samsung_protocol.pdf
bool IRrecv::decodeSAMSUNG(decode_results *results, uint16_t nbits,
                           bool strict) {
  if (results->rawlen < 2 * nbits + kHeader + kFooter - 1)
    return false;  // Can't possibly be a valid Samsung message.
  if (strict && nbits != kSamsungBits)
    return false;  // We expect Samsung to be 32 bits of message.

  uint64_t data = 0;
  uint16_t offset = kStartOffset;

  // Header
  if (!matchMark(results->rawbuf[offset], kSamsungHdrMark)) return false;
  // Calculate how long the common tick time is based on the header mark.
  uint32_t m_tick = results->rawbuf[offset++] * kRawTick / kSamsungHdrMarkTicks;
  if (!matchSpace(results->rawbuf[offset], kSamsungHdrSpace)) return false;
  // Calculate how long the common tick time is based on the header space.
  uint32_t s_tick =
      results->rawbuf[offset++] * kRawTick / kSamsungHdrSpaceTicks;
  // Data
  match_result_t data_result =
      matchData(&(results->rawbuf[offset]), nbits,
                kSamsungBitMarkTicks * m_tick, kSamsungOneSpaceTicks * s_tick,
                kSamsungBitMarkTicks * m_tick, kSamsungZeroSpaceTicks * s_tick);
  if (data_result.success == false) return false;
  data = data_result.data;
  offset += data_result.used;
  // Footer
  if (!matchMark(results->rawbuf[offset++], kSamsungBitMarkTicks * m_tick))
    return false;
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], kSamsungMinGapTicks * s_tick))
    return false;

  // Compliance

  // According to the spec, the customer (address) code is the first 8
  // transmitted bits. It's then repeated. Check for that.
  uint8_t address = data >> 24;
  if (strict && address != ((data >> 16) & 0xFF)) return false;
  // Spec says the command code is the 3rd block of transmitted 8-bits,
  // followed by the inverted command code.
  uint8_t command = (data & 0xFF00) >> 8;
  if (strict && command != ((data & 0xFF) ^ 0xFF)) return false;

  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = SAMSUNG;
  // command & address need to be reversed as they are transmitted LSB first,
  results->command = reverseBits(command, sizeof(command) * 8);
  results->address = reverseBits(address, sizeof(address) * 8);
  return true;
}
#endif

#if SEND_SAMSUNG_AC
// Send a Samsung A/C message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=kSamsungAcStateLength)
//   repeat: Nr. of times the message is to be repeated. (Default = 0).
//
// Status: ALPHA / Untested.
//
// Ref:
//   https://github.com/markszabo/IRremoteESP8266/issues/505
void IRsend::sendSamsungAC(uint8_t data[], uint16_t nbytes, uint16_t repeat) {
  if (nbytes < kSamsungAcStateLength && nbytes % kSamsungACSectionLength)
    return;  // Not an appropriate number of bytes to send a proper message.

  enableIROut(38);
  for (uint16_t r = 0; r <= repeat; r++) {
    // Header
    mark(kSamsungAcHdrMark);
    space(kSamsungAcHdrSpace);
    // Send in 7 byte sections.
    for (uint16_t offset = 0; offset < nbytes;
         offset += kSamsungACSectionLength) {
      sendGeneric(kSamsungAcSectionMark, kSamsungAcSectionSpace,
                  kSamsungAcBitMark, kSamsungAcOneSpace, kSamsungAcBitMark,
                  kSamsungAcZeroSpace, kSamsungAcBitMark, kSamsungAcSectionGap,
                  data + offset, kSamsungACSectionLength,  // 7 bytes == 56 bits
                  38000, false, 0, 50);                    // Send in LSBF order
    }
    // Complete made up guess at inter-message gap.
    space(100000 - kSamsungAcSectionGap);
  }
}
#endif  // SEND_SAMSUNG_AC

IRSamsungAc::IRSamsungAc(uint16_t pin) : _irsend(pin) { stateReset(); }

void IRSamsungAc::stateReset() {
  for (uint8_t i = 0; i < kSamsungAcExtendedStateLength; i++)
    remote_state[i] = 0x0;
  remote_state[0] = 0x02;
  remote_state[1] = 0x92;
  remote_state[2] = 0x0F;
  remote_state[6] = 0xF0;
  remote_state[7] = 0x01;
  remote_state[8] = 0x02;
  remote_state[9] = 0xAE;
  remote_state[10] = 0x71;
  remote_state[12] = 0x15;
  remote_state[13] = 0xF0;
}

void IRSamsungAc::begin() { _irsend.begin(); }

uint8_t IRSamsungAc::calcChecksum(const uint8_t state[],
                                  const uint16_t length) {
  uint8_t sum = 0;
  uint8_t currentbyte;
  // Safety check so we don't go outside the array.
  if (length <= 5) return 255;
  // Shamelessly inspired by:
  //   https://github.com/adafruit/Raw-IR-decoder-for-Arduino/pull/3/files
  // Count most of the '1' bits after the checksum location.
  for (uint8_t i = length - 5; i < length - 1; i++) {
    currentbyte = state[i];
    if (i == length - 5) currentbyte = state[length - 5] & 0b11111110;
    for (; currentbyte; currentbyte >>= 1)
      if (currentbyte & 1) sum++;
  }
  return (28 - sum) & 0xF;
}

bool IRSamsungAc::validChecksum(const uint8_t state[], const uint16_t length) {
  if (length <= 5) return true;  // No checksum to compare with. Assume okay.
  return (state[length - 6] >> 4) == calcChecksum(state, length);
}

// Update the checksum for the internal state.
void IRSamsungAc::checksum(uint16_t length) {
  if (length < 9) return;
  remote_state[length - 6] &= 0x0F;
  remote_state[length - 6] |= (calcChecksum(remote_state, length) << 4);
}

#if SEND_SAMSUNG_AC
void IRSamsungAc::send(const bool calcchecksum) {
  if (calcchecksum) checksum();
  _irsend.sendSamsungAC(remote_state);
}
#endif  // SEND_SAMSUNG_AC

#if SEND_SAMSUNG_AC
void IRSamsungAc::sendExtended(const bool calcchecksum) {
  if (calcchecksum) checksum();
  uint8_t extended_state[kSamsungAcExtendedStateLength] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xD2, 0x0F, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  // Copy/convert the internal state to an extended state.
  for (uint16_t i = 0; i < kSamsungACSectionLength; i++)
    extended_state[i] = remote_state[i];
  for (uint16_t i = kSamsungACSectionLength; i < kSamsungAcStateLength; i++)
    extended_state[i + kSamsungACSectionLength] = remote_state[i];
  // Send it.
  _irsend.sendSamsungAC(extended_state, kSamsungAcExtendedStateLength);
}
#endif  // SEND_SAMSUNG_AC

uint8_t *IRSamsungAc::getRaw() {
  checksum();
  return remote_state;
}

void IRSamsungAc::setRaw(const uint8_t new_code[], const uint16_t length) {
  for (uint8_t i = 0; i < length && i < kSamsungAcExtendedStateLength; i++) {
    remote_state[i] = new_code[i];
  }
  // Shrink the extended state into a normal state.
  if (length > kSamsungAcStateLength) {
    for (uint8_t i = kSamsungAcStateLength; i < length; i++)
      remote_state[i - kSamsungACSectionLength] = remote_state[i];
  }
}

void IRSamsungAc::on() {
  remote_state[1] &= ~kSamsungAcPowerMask1;
  remote_state[6] |= kSamsungAcPowerMask2;
}

void IRSamsungAc::off() {
  remote_state[1] |= kSamsungAcPowerMask1;
  remote_state[6] &= ~kSamsungAcPowerMask2;
}

void IRSamsungAc::setPower(const bool state) {
  if (state)
    on();
  else
    off();
}

bool IRSamsungAc::getPower() {
  return ((remote_state[6] & kSamsungAcPowerMask2) != 0) &&
         ((remote_state[1] & kSamsungAcPowerMask1) == 0);
}

// Set the temp. in deg C
void IRSamsungAc::setTemp(const uint8_t temp) {
  uint8_t newtemp = std::max(kSamsungAcMinTemp, temp);
  newtemp = std::min(kSamsungAcMaxTemp, newtemp);
  remote_state[11] = (remote_state[11] & ~kSamsungAcTempMask) |
                     ((newtemp - kSamsungAcMinTemp) << 4);
}

// Return the set temp. in deg C
uint8_t IRSamsungAc::getTemp() {
  return ((remote_state[11] & kSamsungAcTempMask) >> 4) + kSamsungAcMinTemp;
}

void IRSamsungAc::setMode(const uint8_t mode) {
  // If we get an unexpected mode, default to AUTO.
  uint8_t newmode = mode;
  if (newmode > kSamsungAcHeat) newmode = kSamsungAcAuto;
  remote_state[12] = (remote_state[12] & ~kSamsungAcModeMask) | (newmode << 4);

  // Auto mode has a special fan setting valid only in auto mode.
  if (newmode == kSamsungAcAuto) {
    setFan(kSamsungAcFanAuto2);
  } else {
    if (getFan() == kSamsungAcFanAuto2)  // Non-Auto can't have this fan setting
      setFan(kSamsungAcFanAuto);         // Default to something safe.
  }
}

uint8_t IRSamsungAc::getMode() {
  return (remote_state[12] & kSamsungAcModeMask) >> 4;
}

void IRSamsungAc::setFan(const uint8_t speed) {
  switch (speed) {
    case kSamsungAcFanAuto:
    case kSamsungAcFanLow:
    case kSamsungAcFanMed:
    case kSamsungAcFanHigh:
    case kSamsungAcFanTurbo:
      if (getMode() == kSamsungAcAuto) return;  // Not valid in Auto mode.
      break;
    case kSamsungAcFanAuto2:  // Special fan setting for when in Auto mode.
      if (getMode() != kSamsungAcAuto) return;
      break;
    default:
      return;
  }
  remote_state[12] = (remote_state[12] & ~kSamsungAcFanMask) | (speed << 1);
}

uint8_t IRSamsungAc::getFan() {
  return ((remote_state[12] & kSamsungAcFanMask) >> 1);
}

bool IRSamsungAc::getSwing() {
  // TODO(Hollako): Explain why sometimes the LSB of remote_state[9] is a 1.
  // e.g. 0xAE or 0XAF for swing move.
  return ((remote_state[9] & kSamsungAcSwingMask) >> 4) == kSamsungAcSwingMove;
}

void IRSamsungAc::setSwing(const bool state) {
  // TODO(Hollako): Explain why sometimes the LSB of remote_state[9] is a 1.
  // e.g. 0xAE or 0XAF for swing move.
  remote_state[9] &= ~kSamsungAcSwingMask;  // Clear the previous swing state.
  if (state)
    remote_state[9] |= (kSamsungAcSwingMove << 4);
  else
    remote_state[9] |= (kSamsungAcSwingStop << 4);
}

bool IRSamsungAc::getBeep() { return remote_state[13] & kSamsungAcBeepMask; }

void IRSamsungAc::setBeep(const bool state) {
  if (state)
    remote_state[13] |= kSamsungAcBeepMask;
  else
    remote_state[13] &= ~kSamsungAcBeepMask;
}

bool IRSamsungAc::getClean() {
  return (remote_state[10] & kSamsungAcCleanMask10) &&
         (remote_state[11] & kSamsungAcCleanMask11);
}

void IRSamsungAc::setClean(const bool state) {
  if (state) {
    remote_state[10] |= kSamsungAcCleanMask10;
    remote_state[11] |= kSamsungAcCleanMask11;
  } else {
    remote_state[10] &= ~kSamsungAcCleanMask10;
    remote_state[11] &= ~kSamsungAcCleanMask11;
  }
}

// Very unsure this is correct.
bool IRSamsungAc::getQuiet() {
  return remote_state[11] & kSamsungAcQuietMask11;
}

// Very unsure this is correct.
void IRSamsungAc::setQuiet(const bool state) {
  if (state) {
    remote_state[11] |= kSamsungAcQuietMask11;
    setFan(kSamsungAcFanAuto);  // Quiet mode seems to set fan speed to auto.
  } else {
    remote_state[11] &= ~kSamsungAcQuietMask11;
  }
}

// Convert the internal state into a human readable string.
#ifdef ARDUINO
String IRSamsungAc::toString() {
  String result = "";
#else
std::string IRSamsungAc::toString() {
  std::string result = "";
#endif  // ARDUINO
  result += "Power: ";
  if (getPower())
    result += "On";
  else
    result += "Off";
  result += ", Mode: " + uint64ToString(getMode());
  switch (getMode()) {
    case kSamsungAcAuto:
      result += " (AUTO)";
      break;
    case kSamsungAcCool:
      result += " (COOL)";
      break;
    case kSamsungAcHeat:
      result += " (HEAT)";
      break;
    case kSamsungAcDry:
      result += " (DRY)";
      break;
    case kSamsungAcFan:
      result += " (FAN)";
      break;
    default:
      result += " (UNKNOWN)";
  }
  result += ", Temp: " + uint64ToString(getTemp()) + "C";
  result += ", Fan: " + uint64ToString(getFan());
  switch (getFan()) {
    case kSamsungAcFanAuto:
    case kSamsungAcFanAuto2:
      result += " (AUTO)";
      break;
    case kSamsungAcFanLow:
      result += " (LOW)";
      break;
    case kSamsungAcFanMed:
      result += " (MED)";
      break;
    case kSamsungAcFanHigh:
      result += " (HIGH)";
      break;
    case kSamsungAcFanTurbo:
      result += " (TURBO)";
      break;
    default:
      result += " (UNKNOWN)";
      break;
  }
  result += ", Swing: ";
  if (getSwing())
    result += "On";
  else
    result += "Off";
  result += ", Beep: ";
  if (getBeep())
    result += "On";
  else
    result += "Off";
  result += ", Clean: ";
  if (getBeep())
    result += "On";
  else
    result += "Off";
  result += ", Quiet: ";
  if (getQuiet())
    result += "On";
  else
    result += "Off";
  return result;
}

#if DECODE_SAMSUNG_AC
// Decode the supplied Samsung A/C message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kSamsungAcBits
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / Appears to mostly work.
//
// Ref:
//   https://github.com/markszabo/IRremoteESP8266/issues/505
bool IRrecv::decodeSamsungAC(decode_results *results, uint16_t nbits,
                             bool strict) {
  if (results->rawlen < 2 * nbits + kHeader * 3 + kFooter * 2 - 1)
    return false;  // Can't possibly be a valid Samsung A/C message.
  if (nbits != kSamsungAcBits && nbits != kSamsungAcExtendedBits) return false;

  uint16_t offset = kStartOffset;
  uint16_t dataBitsSoFar = 0;
  match_result_t data_result;

  // Message Header
  if (!matchMark(results->rawbuf[offset++], kSamsungAcBitMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kSamsungAcHdrSpace)) return false;
  // Section(s)
  for (uint16_t pos = kSamsungACSectionLength, i = 0; pos <= nbits / 8;
       pos += kSamsungACSectionLength) {
    uint64_t sectiondata = 0;
    // Section Header
    if (!matchMark(results->rawbuf[offset++], kSamsungAcSectionMark))
      return false;
    if (!matchSpace(results->rawbuf[offset++], kSamsungAcSectionSpace))
      return false;
    // Section Data
    // Keep reading bytes until we either run out of section or state to fill.
    for (; offset <= results->rawlen - 16 && i < pos;
         i++, dataBitsSoFar += 8, offset += data_result.used) {
      data_result = matchData(&(results->rawbuf[offset]), 8, kSamsungAcBitMark,
                              kSamsungAcOneSpace, kSamsungAcBitMark,
                              kSamsungAcZeroSpace, kTolerance, 0, false);
      if (data_result.success == false) {
        DPRINT("DEBUG: offset = ");
        DPRINTLN(offset + data_result.used);
        return false;  // Fail
      }
      results->state[i] = data_result.data;
      sectiondata = (sectiondata << 8) + data_result.data;
    }
    DPRINTLN("DEBUG: sectiondata = 0x" + uint64ToString(sectiondata, 16));
    // Section Footer
    if (!matchMark(results->rawbuf[offset++], kSamsungAcBitMark)) return false;
    if (pos < nbits / 8) {  // Inter-section gap.
      if (!matchSpace(results->rawbuf[offset++], kSamsungAcSectionGap))
        return false;
    } else {  // Last section / End of message gap.
      if (offset <= results->rawlen &&
          !matchAtLeast(results->rawbuf[offset++], kSamsungAcSectionGap))
        return false;
    }
  }
  // Compliance
  // Re-check we got the correct size/length due to the way we read the data.
  if (dataBitsSoFar != nbits) return false;
  // Is the signature correct?
  DPRINTLN("DEBUG: Checking signature.");
  if (results->state[0] != 0x02 || results->state[2] != 0x0F) return false;
  if (results->state[1] != 0x92 && results->state[1] != 0xB2) return false;
  if (strict) {
    // Is the checksum valid?
    if (!IRSamsungAc::validChecksum(results->state, nbits / 8)) {
      DPRINTLN("DEBUG: Checksum failed!");
      return false;
    }
  }
  // Success
  results->decode_type = SAMSUNG_AC;
  results->bits = dataBitsSoFar;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_SAMSUNG_AC
