/*
 This library is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef A4988_Stepper_h
#define A4988_Stepper_h

class A4988_Stepper {
  public:
    // constructor:
    A4988_Stepper( int motor_spr
              ,   int motor_rpm
              , short motor_mis
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
    const unsigned short MIS_TABLE[5] = {0b000,0b001,0b010,0b011,0b111};

  private:
    void adjustDelay(void);
    void adjustPins(void);
    void adjustMicrosteps(void);
    unsigned long motor_delay; // delay between steps, in ms
    int           motor_SPR;   // Steps Per Revolution
    int           motor_RPM;   // Rounds Per Minute
    short         motor_MIS;   // Micro Steps

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
