/*
  xdrv_25_a4988_stepper.ino - A4988 StepMotorDriverCircuit- support for Tasmota

  Copyright (C) 2021  Tim Leuscher and Theo Arends

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
#ifdef USE_ULN2003_STEPPER
/*********************************************************************************************\
 * A4988 Stepper motor driver circuit
\*********************************************************************************************/

#define XDRV_25                     25

#include <Stepper.h>

short ULN2003_in1_pin = 0;
short ULN2003_in2_pin = 0;
short ULN2003_in3_pin = 0;
short ULN2003_in4_pin = 0;
int   ULN2003_spr     = 0;
float ULN2003_rpm     = 0;
bool forward = true;

Stepper* myStepper = nullptr;

void ULN2003Init(void)
{
  ULN2003_in1_pin = Pin(GPIO_ULN2003_IN1);
  ULN2003_in2_pin = Pin(GPIO_ULN2003_IN2);
  ULN2003_in3_pin = Pin(GPIO_ULN2003_IN3);
  ULN2003_in4_pin = Pin(GPIO_ULN2003_IN4);
  ULN2003_spr     = 2048;
  ULN2003_rpm     = 5;

  myStepper = new Stepper( ULN2003_spr
                        , ULN2003_in1_pin
                        , ULN2003_in3_pin
                        , ULN2003_in2_pin
                        , ULN2003_in4_pin);

  myStepper->setSpeed(ULN2003_rpm);
}

const char kULN2003Commands[] PROGMEM = "Motor|" // prefix
  "Move|Turn|SPR|RPM|DIR";

void (* const ULN2003Command[])(void) PROGMEM = {
  &CmndDoMove,&CmndDoTurn,&CmndSetSPR,&CmndSetRPM,&CmndSetDir};

void CmndDoMove(void) {
  if (XdrvMailbox.data_len > 0) {
    int stepsPlease = (int)strtoul(XdrvMailbox.data,nullptr,10);
    myStepper->step(forward ? stepsPlease : -stepsPlease);
    ResponseCmndDone();
  }
}

void CmndDoTurn(void) {
  if (XdrvMailbox.data_len > 0) {
    int  turnsPlease = (int)strtod(XdrvMailbox.data,nullptr);
    myStepper->step(forward ? (turnsPlease * ULN2003_spr) : -(turnsPlease * ULN2003_spr));
    ResponseCmndDone();
  }
}

void CmndSetSPR(void) {
  if (XdrvMailbox.data_len > 0) {
    int newSPR = strtoul(XdrvMailbox.data,nullptr,10);
    myStepper = new Stepper( newSPR
                        , ULN2003_in1_pin
                        , ULN2003_in2_pin
                        , ULN2003_in3_pin
                        , ULN2003_in4_pin);
    ResponseCmndDone();
  }
}

void CmndSetDir(void) {
  if (XdrvMailbox.data_len > 0) {
    forward =  strtoul(XdrvMailbox.data,nullptr,10);
    ResponseCmndDone();
  }
}

void CmndSetRPM(void) {
  if (XdrvMailbox.data_len > 0) {
    short newRPM = strtoul(XdrvMailbox.data,nullptr,10);
    myStepper->setSpeed(newRPM);
    ResponseCmndDone();
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdrv25(uint8_t function)
{
  bool result = false;
  if (PinUsed(GPIO_ULN2003_IN1) && PinUsed(GPIO_ULN2003_IN2)) {
    switch (function) {
      case FUNC_INIT:
        ULN2003Init();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kULN2003Commands, ULN2003Command);
        break;
    }
  }
  return result;
}

#endif  // USE_ULN2003_STEPPER