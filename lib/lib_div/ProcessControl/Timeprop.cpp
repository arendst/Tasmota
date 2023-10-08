/**
 * Copyright 2018 Colin Law
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * See Timeprop.h for Usage
 *
 **/


#include "Timeprop.h"

void Timeprop::initialise( int cycleTime, int deadTime, unsigned char invert, float fallbackPower, int maxUpdateInterval,
  unsigned long nowSecs) {
  m_cycleTime = cycleTime;
  m_deadTime = deadTime;
  m_invert = invert;
  m_fallbackPower = fallbackPower;
  m_maxUpdateInterval = maxUpdateInterval;

  m_dtoc = (float)deadTime/cycleTime;
  m_opState = 0;
  setPower(m_fallbackPower, nowSecs);
}

/* set current power required 0:1, given power and current time in seconds */
void Timeprop::setPower( float power, unsigned long nowSecs ) {
  if (power < 0.0) {
    power = 0.0;
  } else if (power >= 1.0) {
    power = 1.0;
  }
  m_power = power;
  m_lastPowerUpdateTime = nowSecs;
};

/* called regularly to provide new output value */
/* returns new o/p state 0, 1 */
int Timeprop::tick( unsigned long nowSecs) {
  int newState;
  float wave;
  float direction;
  float effectivePower;

  // check whether too long has elapsed since power was last updated
  if (m_maxUpdateInterval > 0  &&  nowSecs - m_lastPowerUpdateTime > m_maxUpdateInterval) {
    // yes, go to fallback power
    setPower(m_fallbackPower, nowSecs);
  }

  wave = (nowSecs % m_cycleTime)/(float)m_cycleTime;
  // determine direction of travel and convert to triangular wave
  if (wave < 0.5) {
    direction = 1;      // on the way up
    wave = wave*2;
  } else {
    direction = -1;     // on the way down
    wave = (1 - wave)*2;
  }
  // if a dead_time has been supplied for this o/p then adjust power accordingly
  if (m_deadTime > 0  && m_power > 0.0  &&  m_power < 1.0) {
      effectivePower = (1.0-2.0*m_dtoc)*m_power + m_dtoc;
  } else {
      effectivePower = m_power;
  }
  //  cope with end cases in case values outside 0..1
  if (effectivePower <= 0.0) {
      newState = 0;     // no heat
  } else if (effectivePower >= 1.0) {
      newState = 1;     // full heat
  } else {
      // only allow power to come on on the way down and off on the way up, to reduce short pulses
      if (effectivePower >= wave  &&  direction == -1) {
          newState = 1;
      } else if (effectivePower <= wave  &&  direction == 1) {
          newState = 0;
      } else {
          // otherwise leave it as it is
          newState = m_opState;
      }
  }
  m_opState = newState;
  return m_invert ? (1-m_opState) : m_opState;
}
