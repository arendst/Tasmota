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

#ifndef Tsl2561Util_H
#define Tsl2561Util_H

#include <Tsl2561.h>

namespace Tsl2561Util {

  // delay until next sample is available
  void waitNext( Tsl2561::exposure_t exposure );

  // get saturation limit for given exposure time
  uint16_t getLimit( Tsl2561::exposure_t exposure );

  // in a loop wait for a sample, get it, check if within thresholds,
  //   until luminosity is either valid or at upper or lower saturation limit
  bool autoGain( Tsl2561 &tsl, bool &gain, Tsl2561::exposure_t &exposure, uint16_t &full, uint16_t &ir );

  // adjust luminosity as if measured with maximum exposure time and maximum gain
  bool normalizedLuminosity( bool gain, Tsl2561::exposure_t exposure, uint32_t &full, uint32_t &ir );

  // adjust luminosity according to sensor temperature (max +/-20% from 25°C)
  bool compensateTemperature( int16_t centiCelsius, uint32_t &full, uint32_t &ir );

  // calculate lux from normalized (and optionally temperature adjusted) luminosity
  bool milliLux( uint32_t full, uint32_t ir, uint32_t &milliLux, bool csType = false );

};

#endif
