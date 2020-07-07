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


#ifndef Generic_Stepper_h
#define Generic_Stepper_h

#include "TasmotaSerial.h"
#include "TMCStepper.h"

class Generic_Stepper {
  public:
    // constructor:
    Generic_Stepper(  uint32_t GSD_dir_pin
                    , uint32_t GSD_stp_pin
                    , uint32_t GSD_ena_pin
                    , uint32_t GSD_ena_inv_pin
                    , uint32_t GSD_ms1_pin
                    , uint32_t GSD_ms2_pin
                    , uint32_t GSD_ms3_pin
                    , uint32_t GSD_uart_tx_pin
                    , uint32_t GSD_uart_rx_pin
                    );

    bool setMicrosteps( uint16_t OneToSixteen );
    void setFullStepsPerRev( uint16_t newFullStepsPerRev );
    bool setRmsCurrent( uint16_t newRmsCurrent );
    void setHold( uint32_t flagHold );

    void doMove( int32_t microsteps_to_move );
    void doRotate( float degrs_to_turn );
    void doTurn( float turns_to_make );

    void setSpeed( float newMicrostepsPerSec );
    void setRotSpeed( float newRotSpeed );
    void setRPM( float newRPM );

    void setAccel( float newMicrostepAccel );
    void setAngleAccel( float newAngleAccel );
    void setRpmAccel( float newRpmAccel );

    void enable( void );
    void disable( void );

  private:
    uint16_t microsteps = 1; // Number of microsteps per full step
    uint32_t full_steps_per_rev = 200; // Full steps per revolution
    float microstep_speed = 100; // Full steps per second
    float microstep_accel = 0; // Full steps per second^2
    bool holdMode = false;

    // motor pins:
    uint32_t dir_pin;
    uint32_t stp_pin;
    uint32_t ena_pin;
    uint32_t ena_inv_pin;
    uint32_t ms1_pin;
    uint32_t ms2_pin;
    uint32_t ms3_pin;
    uint32_t uart_tx_pin;
    uint32_t uart_rx_pin;

    bool uart_ready;
    bool microstep_pins_ready;

    TasmotaSerial* tserial;
    TMC2208Stepper* driver;
};

#endif
