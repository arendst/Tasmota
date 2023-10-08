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
  * A PID control class
  *
  * Github repository https://github.com/colinl/process-control.git
  *
  * Given ...
  *
  * Usage:
  * First call initialise(), see below for parameters then
  * ...
  * The functions require a parameter nowSecs which is a representation of the
  * current time in seconds. The absolute value of this is immaterial, it is
  * used for relative timing only.
  *
 **/


#ifndef PID_h
#define PID_h

class PID {
public:

  PID();

  /*
    Initialiser given

      current time in seconds
  */
  void initialise( double setpoint, double prop_band, double t_integral, double t_derivative,
    double integral_default, int max_interval, double smooth_factor, unsigned char mode_auto, double manual_op );


  /* called regularly to calculate and return new power value */
  double tick(unsigned long nowSecs);

  // call to pass in new process value
  void setPv( double pv, unsigned long nowSecs );

  // methods to modify configuration data
  void setSp( double setpoint );
  void setPb( double prop_band );
  void setTi( double t_integral );
  void setTd( double t_derivative );
  void setInitialInt( double integral_default );
  void setDSmooth( double smooth_factor );
  void setAuto( unsigned char mode_auto );
  void setManualPower( double manual_op );
  void setMaxInterval( int max_interval );

  double getPv();
  double getSp();
  double getPb();
  double getTi();
  double getTd();
  double getInitialInt();
  double getDSmooth();
  unsigned char getAuto();
  double getManualPower();
  int getMaxInterval();

private:
  double m_pv;
  double m_setpoint;
  double m_prop_band;
  double m_t_integral;
  double m_t_derivative;
  double m_integral_default;
  double m_smooth_factor;
  unsigned char m_mode_auto;
  double m_manual_op;
  int m_max_interval;
  double m_last_power;


  unsigned char m_initialised;
  unsigned long m_last_pv_update_time;   // the time of last pv update secs
  unsigned long m_last_sample_time;   // the time of the last tick() run
  double m_smoothed_value;
  double m_integral;
  double m_derivative ;
};

#endif // Timeprop_h
