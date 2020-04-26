
/*
  xdrv_25_a4988_stepper.ino - A4988 StepMotorDriverCircuit- support for Tasmota

  Copyright (C) 2020  Tim Leuscher and Theo Arends

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
  A4988_ms2_pin = Pin(GPIO_A4988_MS2);
  A4988_ms3_pin = Pin(GPIO_A4988_MS3);
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
  if (XdrvMailbox.data_len > 0) {
    long stepsPlease = strtoul(XdrvMailbox.data,nullptr,10);
    myA4988->doMove(stepsPlease);
    ResponseCmndDone();
  }
}

void CmndDoRotate(void) {
  if (XdrvMailbox.data_len > 0) {
    long degrsPlease = strtoul(XdrvMailbox.data,nullptr,10);
    myA4988->doRotate(degrsPlease);
    ResponseCmndDone();
  }
}

void CmndDoTurn(void) {
  if (XdrvMailbox.data_len > 0) {
    float turnsPlease = strtod(XdrvMailbox.data,nullptr);
    myA4988->doTurn(turnsPlease);
    ResponseCmndDone();
  }
}

void CmndSetMIS(void) {
  if ((Pin(GPIO_A4988_MS1) < 99) && (Pin(GPIO_A4988_MS2) < 99) && (Pin(GPIO_A4988_MS3) < 99) && (XdrvMailbox.data_len > 0)) {
    short newMIS = strtoul(XdrvMailbox.data,nullptr,10);
    myA4988->setMIS(newMIS);
    ResponseCmndDone();
  }
}

void CmndSetSPR(void) {
  if (XdrvMailbox.data_len > 0) {
    int newSPR = strtoul(XdrvMailbox.data,nullptr,10);
    myA4988->setSPR(newSPR);
    ResponseCmndDone();
  }
}

void CmndSetRPM(void) {
  if (XdrvMailbox.data_len > 0) {
    short newRPM = strtoul(XdrvMailbox.data,nullptr,10);
    myA4988->setRPM(newRPM);
    ResponseCmndDone();
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdrv25(uint8_t function)
{
  bool result = false;
  if ((Pin(GPIO_A4988_DIR) < 99) && (Pin(GPIO_A4988_STP) < 99)) {
    switch (function) {
      case FUNC_INIT:
        A4988Init();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kA4988Commands, A4988Command);
        break;
    }
  }
  return result;
}

#endif  // USE_A4988_STEPPER
