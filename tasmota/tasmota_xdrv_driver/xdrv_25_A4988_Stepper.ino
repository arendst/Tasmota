
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

#ifdef USE_A4988_STEPPER
/*********************************************************************************************\
 * A4988 Stepper motor driver circuit
\*********************************************************************************************/

#define XDRV_25                    25

#include <A4988_Stepper.h>

short A4988_dir_pin = 0;
short A4988_stp_pin = 0;
short A4988_ms1_pin = 0;
short A4988_ms2_pin = 0;
short A4988_ms3_pin = 0;
short A4988_ena_pin = 0;
int   A4988_spr    = 0;
float A4988_rpm    = 0;
short A4988_mis    = 0;

A4988_Stepper* myA4988 = nullptr;

void A4988Init(void)
{
  A4988_dir_pin = Pin(GPIO_A4988_DIR);
  A4988_stp_pin = Pin(GPIO_A4988_STP);
  A4988_ena_pin = Pin(GPIO_A4988_ENA);
  A4988_ms1_pin = Pin(GPIO_A4988_MS1);
  A4988_ms2_pin = Pin(GPIO_A4988_MS1, 1);
  A4988_ms3_pin = Pin(GPIO_A4988_MS1, 2);
  A4988_spr     = 200;
  A4988_rpm     = 30;
  A4988_mis     = 1;

  myA4988 = new A4988_Stepper( A4988_spr
                            , A4988_rpm
                            , A4988_mis
                            , A4988_dir_pin
                            , A4988_stp_pin
                            , A4988_ena_pin
                            , A4988_ms1_pin
                            , A4988_ms2_pin
                            , A4988_ms3_pin );
}

const char kA4988Commands[] PROGMEM = "Motor|" // prefix
  "Move|Rotate|Turn|MIS|SPR|RPM";

void (* const A4988Command[])(void) PROGMEM = {
  &CmndDoMove,&CmndDoRotate,&CmndDoTurn,&CmndSetMIS,&CmndSetSPR,&CmndSetRPM};

void CmndDoMove(void) {
  // Move the motor the given number of steps (positive values: clockwise, negative values: counterclockwise)
  if (XdrvMailbox.data_len > 0) {
    long stepsPlease = strtol(XdrvMailbox.data, nullptr, 10);
    myA4988->doMove(stepsPlease);
    ResponseCmndDone();
  }
}

void CmndDoRotate(void) {
  // Rotate the motor the given number of degrees (positive values: clockwise, negative values: counterclockwise)
  if (XdrvMailbox.data_len > 0) {
    long degrsPlease = strtol(XdrvMailbox.data, nullptr, 10);
    myA4988->doRotate(degrsPlease);
    ResponseCmndDone();
  }
}

void CmndDoTurn(void) {
  //  Spin the motor the given number of turns (positive values: clockwise, negative values: counterclockwise)
  if (XdrvMailbox.data_len > 0) {
//    float turnsPlease = strtod(XdrvMailbox.data,nullptr);
    float turnsPlease = CharToFloat(XdrvMailbox.data);  // Save 8k code size over strtod()
    myA4988->doTurn(turnsPlease);
    ResponseCmndDone();
  }
}

void CmndSetMIS(void) {
  // 1,2,4,8,16 Set micro stepping increment - 1/1 (full steps) to 1/16 (default = 1)
  if (PinUsed(GPIO_A4988_MS1) && PinUsed(GPIO_A4988_MS1, 1) && PinUsed(GPIO_A4988_MS1, 2) && (XdrvMailbox.payload > 0)) {
//    short newMIS = strtoul(XdrvMailbox.data,nullptr,10);
    myA4988->setMIS(XdrvMailbox.payload);
    ResponseCmndDone();
  }
}

void CmndSetSPR(void) {
  // Set the number of steps the given motor needs for one revolution (default = 200)
  if (XdrvMailbox.payload > 0) {
//    int newSPR = strtoul(XdrvMailbox.data,nullptr,10);
    myA4988->setSPR(XdrvMailbox.payload);
    ResponseCmndDone();
  }
}

void CmndSetRPM(void) {
  // Set revolutions per minute (default = 30)
  if (XdrvMailbox.payload > 0) {
//    short newRPM = strtoul(XdrvMailbox.data,nullptr,10);
    myA4988->setRPM(XdrvMailbox.payload);
    ResponseCmndDone();
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdrv25(uint32_t function)
{
  bool result = false;
  if (PinUsed(GPIO_A4988_DIR) && PinUsed(GPIO_A4988_STP)) {
    switch (function) {
      case FUNC_INIT:
        A4988Init();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kA4988Commands, A4988Command);
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_A4988_STEPPER
