/*
Copyright: Joachim Banzhaf, 2018

This file is part of the Joba_Tsl2561 Library.

    Joba_Tsl2561 is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Joba_Tsl2561 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Joba_Tsl2561.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Tsl2561Util.h"

namespace Tsl2561Util {

// Tsl2561Util::normalizedLuminosity returncode false can mean:
// - saturation: full and/or ir have value ~0 (aka -1)
// - manual exposure time: full and ir are corrected only for gain
// If true, full and ir have values as if exposure was 402 and gain 16.
bool normalizedLuminosity( bool gain, Tsl2561::exposure_t exposure, uint32_t &full, uint32_t &ir ) {

  uint16_t scaledFull = (uint16_t)full;
  uint16_t scaledIr = (uint16_t)ir;

  if( scaledFull ) {

    if( ! gain ) {
      full <<= 4;
      ir   <<= 4;
    }

    switch( exposure ) {
      case Tsl2561::EXP_14:
        full = (scaledFull >=  5047/4*3) ? ~0 : ((full + 5) * 322) / 11;
        ir   = (scaledIr   >=  5047/4*3) ? ~0 : ((ir   + 5) * 322) / 11;
        break;
      case Tsl2561::EXP_101:
        full = (scaledFull >= 37177/4*3) ? ~0 : ((full + 40) * 322) / 81;
        ir   = (scaledIr   >= 37177/4*3) ? ~0 : ((ir   + 40) * 322) / 81;
        break;
      case Tsl2561::EXP_402:
        if( scaledFull >= 65535/4*3 ) full = ~0;
        if( scaledIr   >= 65535/4*3 ) ir   = ~0;
        break;
      default:
        return false;
    }

    return full != ~0U && ir != ~0U;
  }

  return false;
}

// Return upper saturation limit upto which chip returns accurate data
uint16_t getLimit( Tsl2561::exposure_t exposure ) {
  switch( exposure ) {
    case Tsl2561::EXP_14:  return 5047/4*3;
    case Tsl2561::EXP_101: return 37177/4*3;
    default: return 65535/4*3;
  }
}

// Wait for one measurement interval plus some empirically tested extra millis
void waitNext( Tsl2561::exposure_t exposure ) {
  switch( exposure ) {
    case Tsl2561::EXP_14:  delay(16);  break;
    case Tsl2561::EXP_101: delay(103); break;
    default:               delay(408); break;
  }
}

// Wait for next sample, read luminosity and adjust sensitivity, if needed and possible
bool autoGain( Tsl2561 &tsl, bool &gain, Tsl2561::exposure_t &exposure, uint16_t &full, uint16_t &ir ) {

  static const struct {
    bool                gain;
    Tsl2561::exposure_t exposure;
  } sensitivity[] = {
    { false, Tsl2561::EXP_14, }, // min
    { false, Tsl2561::EXP_101 },
    { true,  Tsl2561::EXP_14  },
    { false, Tsl2561::EXP_402 },
    { true,  Tsl2561::EXP_101 },
    { true,  Tsl2561::EXP_402 }  // max
  };

  // get current sensitivity
  if( !tsl.getSensitivity(gain, exposure) ) {
    return false;
  }

  // find index of current sensitivity
  uint8_t curr = 0;
  while( curr < sizeof(sensitivity)/sizeof(sensitivity[0]) ) {
    if( sensitivity[curr].gain == gain && sensitivity[curr].exposure == exposure ) {
      break;
    }
    curr++;
  }
  if( curr == sizeof(sensitivity)/sizeof(sensitivity[0]) ) {
    return false; // should not happen...
  }

  // in a loop wait for next sample, get values and adjust sensitivity if needed
  while( true ) {
    waitNext(exposure);

    if( !tsl.fullLuminosity(full) || !tsl.irLuminosity(ir) ) {
      return false;
    }

    uint16_t limit = getLimit(exposure);
    if( full >= 1000 && full <= limit ) {
      return true; // new value within limits
    }

    if( (full < 1000 && ++curr < sizeof(sensitivity)/sizeof(sensitivity[0]))
     || (full > limit && curr-- > 0) ) {
      if( !tsl.setSensitivity(sensitivity[curr].gain, sensitivity[curr].exposure) ) {
        return false;
      }
      gain = sensitivity[curr].gain;
      exposure = sensitivity[curr].exposure;
    }
    else {
      return true; // saturated, but best we can do
    }
  }
}

// Measurement is up to 20% too high for temperatures above 25°C. Compensate for that.
bool compensateTemperature( int16_t centiCelsius, uint32_t &full, uint32_t &ir ) {
  // assume linear gradient 0% at 25°C to +20% at 70°C
  if( centiCelsius >= -3000 && centiCelsius <= 7000 ) {
    full -= (full * (centiCelsius - 2500) * 20) / (100 * (7000 - 2500));
    ir   -= (ir   * (centiCelsius - 2500) * 20) / (100 * (7000 - 2500));
    return true;
  }

  return false;
}

// Calculate lux from raw luminosity values
bool milliLux( uint32_t full, uint32_t ir, uint32_t &mLux, bool csType ) {
  if( !full ) {
    mLux = 0;
    return true;
  }

  uint32_t milliRatio = ir * 1000 / full;

  if( csType ) {
    if(       milliRatio <  130 ) { mLux = 1000 * full -  840 * ir; }
    else if ( milliRatio <  260 ) { mLux = 1080 * full - 1380 * ir; }
    else if ( milliRatio <  390 ) { mLux = 1160 * full - 1690 * ir; }
    else if ( milliRatio <  520 ) { mLux = 1260 * full - 1940 * ir; }
    else if ( milliRatio <  650 ) { mLux =  730 * full -  930 * ir; }
    else if ( milliRatio <  800 ) { mLux =  500 * full -  575 * ir; }
    else if ( milliRatio < 1300 ) { mLux =  110 * full -   85 * ir; }
    else {
      mLux = 0;
      return false;
    }
    mLux /= 400 * 16 / 200; // 32 = counts/lux (cpl)
  }
  else {
    if(       milliRatio <  125 ) { mLux = 1000 * full -  895 * ir; }
    else if ( milliRatio <  250 ) { mLux = 1070 * full - 1045 * ir; }
    else if ( milliRatio <  375 ) { mLux = 1150 * full - 1790 * ir; }
    else if ( milliRatio <  500 ) { mLux = 1260 * full - 2050 * ir; }
    else if ( milliRatio <  610 ) { mLux =  740 * full - 1020 * ir; }
    else if ( milliRatio <  800 ) { mLux =  420 * full -  500 * ir; }
    else if ( milliRatio < 1300 ) { mLux =   48 * full -   37 * ir; }
    else {
      mLux = 0;
      return false;
    }
    mLux /= 400 * 16 / 193; // 33 = counts/lux (cpl)
  }

  return true;
}

} // namespace Tsl2561Util
