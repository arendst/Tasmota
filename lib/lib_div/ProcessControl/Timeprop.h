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
 **/

 /**
  * A class to generate a time proportioned digital output from a linear input
  *
  * Github repository https://github.com/colinl/process-control.git
  *
  * Given a required power value in the range 0.0 to 1.0 this class generates
  * a time proportioned 0/1 output (representing OFF/ON) which averages to the
  * required power value. The cycle time is configurable.  If, for example, this
  * is set to 10 minutes and the power input is 0.2 then the output will be on
  * for two minutes in every ten minutes.
  *
  * A value for actuator dead time may be provided. If you have a device that
  * takes a significant time to open/close then set this to the average of the
  * open and close times.  The algorithim will then adjust the output timing
  * accordingly to ensure that the output is not switched more rapidly than
  * the actuator can cope with.
  *
  * A facility to invert the output is provided which can be useful when used in
  * refrigeration processes and similar.
  *
  * Usage:
  * First call initialise(), see below for parameters then call setPower() to
  * specify the current power required.
  * Then regularly call tick() to determine the output state required.
  * setPower may be called as often as required to change the power required.
  * The functions require a parameter nowSecs which is a representation of the
  * current time in seconds. The absolute value of this is immaterial, it is
  * used for relative timing only.
  *
 **/


#ifndef Timeprop_h
#define Timeprop_h

class Timeprop {
public:
  /*
    Initialiser given
      cycleTime seconds
      actuator deadTime seconds
      whether to invert the output
      fallback power value if updates are not received within time below
      max number of seconds to allow between power updates before falling back to default power (0 to disable)
      current time in seconds
  */
  void initialise( int cycleTime, int deadTime, unsigned char invert, float fallbackPower, int maxUpdateInterval,
    unsigned long nowSecs);

  /* set current power required 0:1, given power and current time in seconds */
  void setPower( float power, unsigned long nowSecs );

  /* called regularly to provide new output value */
  /* returns new o/p state 0, 1 */
  int tick(unsigned long nowSecs);

private:
  int m_cycleTime;        // cycle time seconds, float to force float calcs
  int m_deadTime;         // actuator action time seconds
  unsigned char m_invert; // whether to invert the output
  float m_dtoc;           // deadTime/m_cycleTime
  int m_opState;          // current output state (before invert)
  float m_power;          // required power 0:1
  float m_fallbackPower;  // falls back to this if updates not received with max allowed timezone
  int m_maxUpdateInterval;  // max time between updates
  unsigned long m_lastPowerUpdateTime;   // the time of last power update secs
};

#endif // Timeprop_h
