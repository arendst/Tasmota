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

 Drives a bipolar motor, controlled by Generic stepper driver circuit
 */

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"
#include "Arduino.h"
#include "Generic_Stepper.h"

#define ESP_LOGW ESP_LOGE
#define ESP_LOGI ESP_LOGE
//#define ESP_LOGD ESP_LOGE
//#define ESP_LOGV ESP_LOGE

static const char* TAG = "GenericStepperDriver";

bool PinUsed( uint32_t pin ) {
  return pin < 99;
}

const uint8_t A4988_MIS_TABLE[5] = {0b000,0b001,0b010,0b011,0b111};

Generic_Stepper::Generic_Stepper( uint32_t GSD_dir_pin
                                , uint32_t GSD_stp_pin
                                , uint32_t GSD_ena_pin
                                , uint32_t GSD_ena_inv_pin
                                , uint32_t GSD_ms1_pin
                                , uint32_t GSD_ms2_pin
                                , uint32_t GSD_ms3_pin
                                , uint32_t GSD_uart_tx_pin
                                , uint32_t GSD_uart_rx_pin
                                )
  : dir_pin( GSD_dir_pin ),
  stp_pin( GSD_stp_pin ),
  ena_pin( GSD_ena_pin ),
  ena_inv_pin( GSD_ena_inv_pin ),
  ms1_pin( GSD_ms1_pin ),
  ms2_pin( GSD_ms2_pin ),
  ms3_pin( GSD_ms3_pin ),
  uart_tx_pin( GSD_uart_tx_pin ),
  uart_rx_pin( GSD_uart_rx_pin )
{
  if( PinUsed( ena_pin ) ) {
    //AddLog_P2( LOG_LEVEL_DEBUG, "Using regular enable pin (HIGH = ON) on GPIO: %u", ena_pin );
    ESP_LOGI( TAG, "Using regular enable pin (HIGH = ON) on GPIO: %u", ena_pin );
    pinMode( ena_pin, OUTPUT );
  }
  if( PinUsed( ena_inv_pin ) ) {
    //AddLog_P2( LOG_LEVEL_DEBUG, "Using inverse enable pin (LOW = ON) on GPIO: %u", ena_inv_pin );
    ESP_LOGI( TAG, "Using inverse enable pin (LOW = ON) on GPIO: %u", ena_inv_pin );
    pinMode( ena_inv_pin, OUTPUT );
  }
  disable();

  pinMode( dir_pin, OUTPUT );
  pinMode( stp_pin, OUTPUT );

  microstep_pins_ready = false;
  if( ms1_pin < 99 && ms2_pin < 99 && ms3_pin < 99 ) {
    //AddLog_P2( LOG_LEVEL_DEBUG, "Using A4988 style microstep pins on GPIOs: %u, %u and %u", ms1_pin, ms2_pin, ms3_pin );
    ESP_LOGI( TAG, "Using A4988 style microstep pins on GPIOs: %u, %u and %u", ms1_pin, ms2_pin, ms3_pin );
    pinMode( ms1_pin, OUTPUT );
    pinMode( ms2_pin, OUTPUT );
    pinMode( ms3_pin, OUTPUT );
    microstep_pins_ready = true;
  }
  
  uart_ready = false;
  if( uart_tx_pin < 99 && uart_rx_pin < 99 ) {
    //AddLog_P2( LOG_LEVEL_DEBUG, "Using TMC2208 stype UART on GPIOs: %u (RX) and %u (TX)", uart_rx_pin, uart_tx_pin );
    ESP_LOGI( TAG, "Using TMC2208 stype UART on GPIOs: %u (RX) and %u (TX)", uart_rx_pin, uart_tx_pin );
    tserial = new TasmotaSerial( uart_rx_pin, uart_tx_pin );
    tserial->begin( 115200 );
    driver = new TMC2208Stepper( tserial, 0.11f );
    uint8_t test_result = driver->test_connection();
    ESP_LOGD( TAG, "Result of UART connection test: %u", test_result );
    if( test_result ) {
      //AddLog_P( LOG_LEVEL_ERROR, "UART connection to TMC stepper driver failed!" );
      ESP_LOGE( TAG, "UART connection to TMC stepper driver failed!" );
      uart_ready = false;
    }
    else {
      ESP_LOGI( TAG, "UART connection established." );
      driver->begin();
      uart_ready = true;
      //Set minimal current to not damage anything
      setRmsCurrent( 30 );
    }
  }

  //if( microstep_pins_ready && uart_ready ) AddLog_P( LOG_LEVEL_ERROR, "UART connection and microstep pins configured!" );
  if( microstep_pins_ready && uart_ready ) ESP_LOGW( TAG, "UART connection and microstep pins configured!" );

  setMicrosteps( microsteps );
}

bool Generic_Stepper::setMicrosteps( uint16_t OneToSixteen ) {
  ESP_LOGI( TAG, "Setting microsteps to: %u", OneToSixteen );
  if( !microstep_pins_ready && !uart_ready ) {
    //AddLog_P2( LOG_LEVEL_INFO, "Neither UART nor microstep pins configured: Using microsteps only for calculation. Setting microsteps to %u", OneToSixteen );
    ESP_LOGW( TAG, "Neither UART nor microstep pins configured: Using microsteps only for calculation." );
    microsteps = OneToSixteen;
    return true;
  }
  else {
    uint8_t mask = 0;
    switch( OneToSixteen ) {
      case 1:
        mask = A4988_MIS_TABLE[0];
        break;
      case 2:
        mask = A4988_MIS_TABLE[1];
        break;
      case 4:
        mask = A4988_MIS_TABLE[2];
        break;
      case 8:
        mask = A4988_MIS_TABLE[3];
        break;
      case 16:
        mask = A4988_MIS_TABLE[4];
        break;
      case 32:
      case 64:
      case 128:
      case 256:
        if( microstep_pins_ready ) {
          //AddLog_P2( LOG_LEVEL_ERROR, "Cannot set microsteps %u using pins. Use UART instead!", OneToSixteen );
          ESP_LOGE( TAG, "Cannot set microsteps %u using pins. Use (only) UART!", OneToSixteen );
          return false;
        }
        break;
      default:
        //AddLog_P2( LOG_LEVEL_ERROR, "Invalid microstep number: %u", OneToSixteen );
        ESP_LOGE( TAG, "Invalid microstep number: %u", OneToSixteen );
        return false;
    }
    bool success = false;
    if( uart_ready ) {
      // set steps
      // microsteps 1 is special
      if( OneToSixteen == 1 ) {
        ESP_LOGD( TAG, "Microsteps 1 requested, setting to 0 for TMC driver." );
        driver->microsteps( 0 );
      }
      else driver->microsteps( OneToSixteen );
      // check what was set
      microsteps = driver->microsteps();
      // correct TMC driver treatment of 1 microstep
      if( microsteps == 0 ) {
        ESP_LOGD( TAG, "TMC driver returned 0 microsteps, correcting to 1." );
        microsteps = 1;
      }
      ESP_LOGI( TAG, "Microsteps set via UART to: %u", microsteps );
      if( microsteps != OneToSixteen ) {
        //AddLog_P2( LOG_LEVEL_ERROR, "Failed to set microsteps to %u. Using old microsteps: %u", OneToSixteen, microsteps );
        ESP_LOGE( TAG, "Failed to set microsteps using UART to %u. Using old microsteps: %u", OneToSixteen, microsteps );
        if( microstep_pins_ready ) ESP_LOGW( TAG, "Using old microsteps for setting microstep pins." );
        success = false;
      }
      else success = true;
    }
    if( microstep_pins_ready ) {
      ESP_LOGD( TAG, "Using microstep pin mask: %u%u%u", 0b1 & mask, 0b01 & mask, 0b001 & mask );
      digitalWrite( ms1_pin,   0b1 & mask ? HIGH : LOW );
      digitalWrite( ms2_pin,  0b01 & mask ? HIGH : LOW );
      digitalWrite( ms3_pin, 0b001 & mask ? HIGH : LOW );
      success = true;
      ESP_LOGI( TAG, "Microsteps set via pins to: %u", microsteps );
    }
    if( success ) microsteps = OneToSixteen;
    return success;
  }
}

void Generic_Stepper::setFullStepsPerRev( uint16_t newFullStepsPerRev ) {
  ESP_LOGI( TAG, "Setting full steps per revolution to: %u", newFullStepsPerRev );
  full_steps_per_rev = newFullStepsPerRev;
}

bool Generic_Stepper::setRmsCurrent( uint16_t newRmsCurrent ) {
  ESP_LOGI( TAG, "Setting RMS current to: %u", newRmsCurrent );
  if( uart_ready ) {
    driver->rms_current( newRmsCurrent );
    //AddLog_P2( LOG_LEVEL_INFO, "RMS current set to: %u", driver->rms_current() );
    ESP_LOGI( TAG, "RMS current set to: %u", driver->rms_current() );
    return true;
  }
  else {
    ESP_LOGE( TAG, "Unable to set RMS current: UART not configured." );
    return false;
  }
}

void Generic_Stepper::setHold( uint32_t flagHold ) {
  holdMode = flagHold > 0;
  if( holdMode ) ESP_LOGI( TAG, "Hold mode enabled." );
  else ESP_LOGI( TAG, "Hold mode disabled." );
}

void Generic_Stepper::doMove( int32_t microsteps_to_move ) {
  ESP_LOGI( TAG, "Moving %d at max speed %f and acceleration %f", microsteps_to_move, microstep_speed, microstep_accel );
  digitalWrite( dir_pin, microsteps_to_move > 0 ? HIGH : LOW );
  // always enable, just to be sure
  enable();
  uint32_t steps_to_go = abs( microsteps_to_move );  // how many steps to take
  uint32_t minStepDelay = 1.0e6f / ( microstep_speed ); // min delay at max speed
  ESP_LOGD( TAG, "Shortest delay: %u", minStepDelay );
  uint32_t maxStepDelay = 1.0e6f * sqrtf( 2.0f / microstep_accel ); // max delay at start of acceleration
  ESP_LOGD( TAG, "Longest delay: %u", maxStepDelay );
  float minSpeed = 1.0e6f / maxStepDelay;
  ESP_LOGD( TAG, "Minimum speed: %f", minSpeed );
  int32_t accel_state = microstep_accel > 0 ? 1 : 0;
  ESP_LOGD( TAG, "Acceleration state: %d", accel_state );
  float current_speed = 0;
  bool stepPinHigh = false;
  uint32_t stepDelay = minStepDelay / 2; // divide by two, because we need to switch HIGH and LOW for one step
  ESP_LOGD( TAG, "Default half step delay: %u", stepDelay );
  uint32_t timeLastStep = 0;
  while( steps_to_go > 0 ) {
    //delay(0); // don't get watchdoged in loop
    uint32_t now = micros();
    // move if delay has passed:
    ESP_LOGV( TAG, "Checking time" );
    if( now - timeLastStep >= stepDelay ) {
      // switch step pin
      digitalWrite( stp_pin, stepPinHigh ? LOW : HIGH );
      stepPinHigh = !stepPinHigh;
      ESP_LOGV( TAG, "New step pin state: %u", stepPinHigh );
      // update delay only when we switched the step pin to high
      if( stepPinHigh ) {
        // if we are accelerating or cruising and acceleration is not zero, we have to check if we have to start slowing down
        if( accel_state >= 0 && microstep_accel > 0 ) {
          ESP_LOGV( TAG, "Checking break point" );
          if( steps_to_go <= current_speed * current_speed / microstep_accel ) {
            accel_state = -1;
            ESP_LOGI( TAG, "Break point reached, new acceleration state: %d", accel_state );
          }
        }
        // nothing to do if we are cruising (accel_state == 0)
        // otherwise we have to update speed and delay
        if( accel_state != 0 ) {
          stepDelay = 1.0e6f * ( sqrtf( current_speed * current_speed + 2 * accel_state * microstep_accel ) - current_speed ) / ( accel_state * microstep_accel );
          ESP_LOGD( TAG, "New step delay: %u", stepDelay );
          if( stepDelay < minStepDelay ) {
            // if delay is shorter than minimum delay we've reached maximum speed
            stepDelay = minStepDelay;
            current_speed = microstep_speed;
            // stop accelerating
            accel_state = 0;
            ESP_LOGI( TAG, "Acceleration stopped at max speed %f and min delay %u", current_speed, stepDelay );
          }
          else if( stepDelay > maxStepDelay ) {
            stepDelay = maxStepDelay;
            current_speed = minSpeed;
            accel_state = 0;
            ESP_LOGI( TAG, "Decceleration stopped at min speed %f and max delay %u", current_speed, stepDelay );
          }
          else {
            current_speed = 1.0e6f / stepDelay;
            ESP_LOGD( TAG, "New speed: %f", current_speed );
          }
          stepDelay /= 2; // divide by two, because we need to switch HIGH and LOW for one step
          ESP_LOGD( TAG, "New half step delay: %u", stepDelay );
        }
      }
      else {
        steps_to_go--; // step is complete on step pin LOW
        ESP_LOGD( TAG, "Remaining microsteps: %u", steps_to_go );
      }
      // remember step-time
      timeLastStep = now;
    }
  }
  // only automatically disable if not in hold mode
  if( !holdMode ) disable();
  ESP_LOGI( TAG, "Move finished" );
}

void Generic_Stepper::doRotate( float degrs_to_turn ) {
  ESP_LOGI( TAG, "Moving %f deg", degrs_to_turn );
  float turns_to_make = degrs_to_turn / 360.0f;
  doTurn( turns_to_make );
}

void Generic_Stepper::doTurn( float turns_to_make ) {
  ESP_LOGI( TAG, "Moving %f turns", turns_to_make );
  int32_t microsteps_to_move = turns_to_make * full_steps_per_rev * microsteps;
  doMove( microsteps_to_move );
}

void Generic_Stepper::setSpeed( float newMicrostepsPerSec ) {
  ESP_LOGI( TAG, "Speed set to %f microsteps/sec", newMicrostepsPerSec );
  microstep_speed = newMicrostepsPerSec;
}

void Generic_Stepper::setRotSpeed( float newRotSpeed ) {
  ESP_LOGI( TAG, "Rotation speed set to %f deg/sec", newRotSpeed );
  float newRPM = newRotSpeed / 360.0f * 60.0f;
  setRPM( newRPM );
}

void Generic_Stepper::setRPM( float newRPM ) {
  ESP_LOGI( TAG, "Rotation speed set to %f RPM", newRPM );
  float newMicrostepsPerSec = newRPM / 60.0f * full_steps_per_rev * microsteps;
  setSpeed( newMicrostepsPerSec );
}

void Generic_Stepper::setAccel( float newMicrostepAccel ) {
  ESP_LOGI( TAG, "Acceleration set to %f microsteps/sec^2", newMicrostepAccel );
  microstep_accel = newMicrostepAccel;
}

void Generic_Stepper::setAngleAccel( float newAngleAccel ) {
  ESP_LOGI( TAG, "Acceleration set to %f deg/sec^2", newAngleAccel );
  float newRpmAccel = newAngleAccel / 360.0f * 60.0f;
  setRpmAccel( newRpmAccel );
}

void Generic_Stepper::setRpmAccel( float newRpmAccel ) {
  ESP_LOGI( TAG, "Acceleration set to %f RPM/sec", newRpmAccel );
  float newMicrostepAccel = newRpmAccel / 60.0f * full_steps_per_rev * microsteps;
  setAccel( newMicrostepAccel );
}

void Generic_Stepper::enable() {
  if( !PinUsed( ena_pin ) && !PinUsed( ena_inv_pin ) ) {
    ESP_LOGW( TAG, "Cannot enable driver: No enable pin configured." );
  }
  if( PinUsed( ena_pin ) ) {
    ESP_LOGI( TAG, "Enabling driver by setting regular enable pin to HIGH." );
    digitalWrite( ena_pin, HIGH );
  }
  if( PinUsed( ena_inv_pin ) ) {
    ESP_LOGI( TAG, "Enabling driver by setting inverted enable pin to LOW." );
    digitalWrite( ena_inv_pin, LOW );
  }
}

void Generic_Stepper::disable(){
  if( !PinUsed( ena_pin ) && !PinUsed( ena_inv_pin ) ) {
    ESP_LOGW( TAG, "Cannot disable driver: No enable pin configured." );
  }
  if( PinUsed( ena_pin ) ) {
    ESP_LOGI( TAG, "Disabling driver by setting regular enable pin to LOW." );
    digitalWrite( ena_pin, LOW );
  }
  if( PinUsed( ena_inv_pin ) ) {
    ESP_LOGI( TAG, "Disabling driver by setting inverted enable pin to HIGH." );
    digitalWrite( ena_inv_pin, HIGH );
  }
}
