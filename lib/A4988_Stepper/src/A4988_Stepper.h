/* This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Drives a bipolar motor, controlled by A4988 stepper driver circuit
 */

#ifndef A4988_Stepper_h
#define A4988_Stepper_h

class A4988_Stepper {
  public:
    // constructor:
    A4988_Stepper( int motor_spr
              ,   int motor_rpm
              , short motor_mic
              , short motor_dir_pin
              , short motor_stp_pin
              , short motor_ena_pin
              , short motor_ms1_pin
              , short motor_ms2_pin
              , short motor_ms3_pin
              );

    void  setRPM  (int   whatRPM     );
    int   getRPM  (void              );

    void  setMIS  (short OneToSixteen);
    short getMIS  (void              );

    void  setSPR  (int   howMany     );
    int   getSPR  (void              );

    void  doMove  (long steps_to_move);
    void  doRotate(long degrs_to_turn);
    void  doTurn  (float howManyTimes);

    void  enable  (void              );
    void  disable (void              );

    int   version (void              );

  private:
    void adjustDelay(void);
    void adjustPins(void);
    void adjustMicrosteps(void);
    unsigned long motor_delay; // delay between steps, in ms
    int           motor_SPR;   // Steps Per Revolution
    int           motor_RPM;   // Rounds Per Minute
    short         motor_MIC;   // Micro Steps

    // motor pins:
    short motor_dir_pin;
    short motor_stp_pin;
    short motor_ena_pin;
    short motor_ms1_pin;
    short motor_ms2_pin;
    short motor_ms3_pin;

    unsigned long last_time;  // timestamp of last pincycle of last step
};

#endif
