
/*
  xdrv_40_generic_stepper.ino - Generic Stepper Driver - support for Tasmota

  Copyright (C) 2020

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_GENERIC_STEPPER
/*********************************************************************************************\
 * Generic Stepper motor driver circuit
\*********************************************************************************************/

#define XDRV_40                    40

#include <Generic_Stepper.h>

uint32_t GSD_dir_pin = 0;
uint32_t GSD_stp_pin = 0;
uint32_t GSD_ms1_pin = 0;
uint32_t GSD_ms2_pin = 0;
uint32_t GSD_ms3_pin = 0;
uint32_t GSD_ena_pin = 0; // For drivers that enable on high
uint32_t GSD_ena_inv_pin = 0; // For drivers that enable on low
uint32_t GSD_uart_tx_pin = 0;
uint32_t GSD_uart_rx_pin = 0;

Generic_Stepper* myGSD = nullptr;

void GSDInit(void)
{
  GSD_dir_pin = Pin(GPIO_GSD_DIR);
  GSD_stp_pin = Pin(GPIO_GSD_STP);
  GSD_ena_pin = Pin(GPIO_GSD_ENA);
  GSD_ena_inv_pin = Pin(GPIO_GSD_ENA_INV);
  GSD_ms1_pin = Pin(GPIO_GSD_MS1);
  GSD_ms2_pin = Pin(GPIO_GSD_MS2);
  GSD_ms3_pin = Pin(GPIO_GSD_MS3);
  GSD_uart_tx_pin = Pin(GPIO_GSD_UART_TX);
  GSD_uart_rx_pin = Pin(GPIO_GSD_UART_RX);
  
  myGSD = new Generic_Stepper( GSD_dir_pin
                            , GSD_stp_pin
                            , GSD_ena_pin
                            , GSD_ena_inv_pin
                            , GSD_ms1_pin
                            , GSD_ms2_pin
                            , GSD_ms3_pin
                            , GSD_uart_tx_pin
                            , GSD_uart_rx_pin );
}

const char kGSDCommands[] PROGMEM = "GSD|" // prefix
  "Microsteps|StepsPerRev|RmsCurrent|Hold|Enable|Move|Rotate|Turn|Speed|RotSpeed|RPM|Acceleration|AngleAccel|RPMAccel";

void (* const GSDCommand[])(void) PROGMEM = {
  &CmndSetMicrosteps,&CmndSetFullStepsPerRev,&CmdSetRmsCurrent,&CmdSetHold,&CmdEnable,&CmndDoMove,&CmndDoRotate,&CmndDoTurn,&CmndSetSpeed,&CmdSetRotSpeed,&CmndSetRPM,&CmdSetAccel,&CmdSetAngleAccel,&CmdSetRpmAccel};

void CmndSetMicrosteps( void ) {
  if( XdrvMailbox.data_len > 0 ) {
    uint16_t newMicrosteps = strtoul( XdrvMailbox.data, nullptr, 10 );
    if( myGSD->setMicrosteps( newMicrosteps ) ) ResponseCmndDone();
    else ResponseCmndChar( D_JSON_FAILED );
  }
  else ResponseCmndChar( D_JSON_FAILED );
}

void CmndSetFullStepsPerRev( void ) {
  if( XdrvMailbox.data_len > 0 ) {
    uint16_t newFSPR = strtoul( XdrvMailbox.data, nullptr, 10 );
    myGSD->setFullStepsPerRev( newFSPR );
    ResponseCmndDone();
  }
  else ResponseCmndChar( D_JSON_FAILED );
}

void CmdSetRmsCurrent( void ) {
  if( XdrvMailbox.data_len > 0 ) {
      uint16_t newRmsCurrent = strtoul( XdrvMailbox.data, nullptr, 10 );
      myGSD->setRmsCurrent( newRmsCurrent );
      ResponseCmndDone();
  }
  else ResponseCmndChar( D_JSON_FAILED );
}

void CmdSetHold( void ) {
  if( XdrvMailbox.data_len > 0 ) {
      uint32_t flagHold = strtoul( XdrvMailbox.data, nullptr, 10 );
      myGSD->setHold( flagHold );
      ResponseCmndDone();
  }
  else ResponseCmndChar( D_JSON_FAILED );
}

void CmdEnable( void ) {
  if( XdrvMailbox.data_len > 0 ) {
      uint32_t flagEnable = strtoul( XdrvMailbox.data, nullptr, 10 );
      if( flagEnable > 0 ) myGSD->enable();
      else myGSD->disable();
      ResponseCmndDone();
  }
  else ResponseCmndChar( D_JSON_FAILED );
}

void CmndDoMove( void ) {
  if( XdrvMailbox.data_len > 0 ) {
    int32_t microStepsPlease = strtol( XdrvMailbox.data, nullptr, 10 );
    myGSD->doMove( microStepsPlease );
    ResponseCmndDone();
  }
  else ResponseCmndChar( D_JSON_FAILED );
}

void CmndDoRotate( void ) {
  if( XdrvMailbox.data_len > 0 ) {
    float degrsPlease = strtof( XdrvMailbox.data, nullptr );
    myGSD->doRotate( degrsPlease );
    ResponseCmndDone();
  }
  else ResponseCmndChar( D_JSON_FAILED );
}

void CmndDoTurn( void ) {
  if( XdrvMailbox.data_len > 0 ) {
    float turnsPlease = strtof( XdrvMailbox.data, nullptr );
    myGSD->doTurn( turnsPlease );
    ResponseCmndDone();
  }
  else ResponseCmndChar( D_JSON_FAILED );
}

void CmndSetSpeed( void ) {
  if( XdrvMailbox.data_len > 0 ) {
    float newFullStepsPerSec = strtof( XdrvMailbox.data, nullptr );
    myGSD->setSpeed( newFullStepsPerSec );
    ResponseCmndDone();
  }
  else ResponseCmndChar( D_JSON_FAILED );
}

void CmdSetRotSpeed( void ) {
  if( XdrvMailbox.data_len > 0 ) {
    float newRotSpeed = strtof( XdrvMailbox.data, nullptr );
    myGSD->setRotSpeed( newRotSpeed );
    ResponseCmndDone();
  }
  else ResponseCmndChar( D_JSON_FAILED );
}

void CmndSetRPM( void ) {
  if( XdrvMailbox.data_len > 0 ) {
    float newRPM = strtof( XdrvMailbox.data, nullptr );
    myGSD->setRPM( newRPM );
    ResponseCmndDone();
  }
  else ResponseCmndChar( D_JSON_FAILED );
}

void CmdSetAccel( void ) {
  if( XdrvMailbox.data_len > 0 ) {
    float newFullStepAccel = strtof( XdrvMailbox.data, nullptr );
    myGSD->setAccel( newFullStepAccel );
    ResponseCmndDone();
  }
  else ResponseCmndChar( D_JSON_FAILED );
}

void CmdSetAngleAccel( void ) {
  if( XdrvMailbox.data_len > 0 ) {
    float newRotAccel = strtof( XdrvMailbox.data, nullptr );
    myGSD->setAngleAccel( newRotAccel );
    ResponseCmndDone();
  }
  else ResponseCmndChar( D_JSON_FAILED );
}

void CmdSetRpmAccel( void ) {
  if( XdrvMailbox.data_len > 0 ) {
    float newRPMAccel = strtof( XdrvMailbox.data, nullptr );
    myGSD->setRpmAccel( newRPMAccel );
    ResponseCmndDone();
  }
  else ResponseCmndChar( D_JSON_FAILED );
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdrv40( uint8_t function ) {
  bool result = false;
  if( PinUsed(GPIO_GSD_DIR) && PinUsed(GPIO_GSD_STP) ) {
    switch (function) {
      case FUNC_INIT:
        GSDInit();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kGSDCommands, GSDCommand);
        break;
    }
  }
  return result;
}

#endif  // USE_GENERIC_STEPPER
