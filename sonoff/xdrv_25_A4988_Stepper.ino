
/*
  xsns_22_sr04.ino - SR04 ultrasonic sensor support for Sonoff-Tasmota

  Copyright (C) 2019  Nuno Ferreira and Theo Arends

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

#ifdef USE_A4988_Stepper
#include <A4988_Stepper.h>
/*********************************************************************************************\
 * Stepper mötör on driver A4988
 *
 * - https://www.dfrobot.com/wiki/index.php/Weather-proof_Ultrasonic_Sensor_SKU_:_SEN0207
\*********************************************************************************************/

#define XDRV_98          98

enum A4988Errors { A4988_NO_ERROR, A4988_NO_JSON_COMMAND, A4988_INVALID_JSON};

short A4988_dir_pin = pin[GPIO_MAX];
short A4988_stp_pin = pin[GPIO_MAX];
short A4988_ms1_pin = pin[GPIO_MAX];
short A4988_ms2_pin = pin[GPIO_MAX];
short A4988_ms3_pin = pin[GPIO_MAX];
short A4988_ena_pin = pin[GPIO_MAX];
int   A4988_spr    = 0;
float A4988_rpm    = 0;
short A4988_mic    = 0;

A4988_Stepper* myA4988 = nullptr;

void A4988Init(void)
{
  A4988_dir_pin = pin[GPIO_A4988_DIR];
  A4988_stp_pin = pin[GPIO_A4988_STP];
  A4988_ena_pin = pin[GPIO_A4988_ENA];
  A4988_ms1_pin = pin[GPIO_A4988_MS1];
  A4988_ms2_pin = pin[GPIO_A4988_MS2];
  A4988_ms3_pin = pin[GPIO_A4988_MS3];
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
  A4988_spr     = myA4988->getSPR();
  A4988_rpm     = myA4988->getRPM();
  A4988_mis     = myA4988->getMIS();
  if ((A4988_ms1_pin < 99)&&(A4988_ms2_pin < 99)&&(A4988_ms3_pin < 99)&&(A4988_ena_pin<99)) {
    AddLog_P2(LOG_LEVEL_INFO, PSTR("STP: A4988-Driver initialized (%dSPR, %dRPM,%dMIC). Pins: Dir[%d]  Stp[%d]  Ena[%d]  MS1[%d] MS2[%d] MS3[%d]"),A4988_spr, A4988_rpm, A4988_mic, A4988_dir_pin,A4988_stp_pin,A4988_ena_pin,A4988_ms1_pin,A4988_ms2_pin,A4988_ms3_pin);
  } else {
    if ((A4988_ena_pin<99)) {
      AddLog_P2(LOG_LEVEL_INFO, PSTR("STP: A4988-Driver initialized (%dSPR, %dRPM,%dMIC). Pins: Dir[%d]  Stp[%d]  Ena[%d] MicroStepping hardwired"),A4988_spr, A4988_rpm, A4988_mic, A4988_dir_pin,A4988_stp_pin,A4988_ena_pin);
    } else {
      AddLog_P2(LOG_LEVEL_INFO, PSTR("STP: A4988-Driver initialized (%dSPR, %dRPM,%dMIC). Pins: Dir[%d]  Stp[%d] motor permanently on, MicroStepping hardwired"),A4988_spr, A4988_rpm, A4988_mic, A4988_dir_pin,A4988_stp_pin,A4988_ena_pin);
    }
  }
}

const char kA4988Commands[] PROGMEM = "MOTOR|"
  "doMove|doRotate|doTurn|setSPR|setRPM|setMIC|getSPR|getRPM|getMIC|doVader";

void (* const A4988Command[])(void) PROGMEM = { &CmndMOTOR, &CmndDoMove, &CmndDoRotate, &CmndDoTurn, &CmndSetSPR, &CmndSetRPM, &CmndSetMic , &CmndGetSPR, &CmndGetRPM, &CmndGetMIC, &CmndDoVader };


uint32_t MOTORCmndJson(void)
{
  // ArduinoJSON entry used to calculate jsonBuf: JSON_OBJECT_SIZE(3) + 40 = 96
  // MOTOR { "command": "doMove", "value": 200 }
  // MOTOR { "command": "doRotate", "value": 200 }
  // MOTOR { "command": "doTurn", "value": 200 }

  char dataBufUc[XdrvMailbox.data_len];
  UpperCase(dataBufUc, XdrvMailbox.data);
  RemoveSpace(dataBufUc);
  if (strlen(dataBufUc) < 8) {
    return A4988_INVALID_JSON;
  }

  StaticJsonBuffer<40> jsonBuf;
  JsonObject &root = jsonBuf.parseObject(dataBufUc);
  if (!root.success()) {
    return A4988_INVALID_JSON;
  }

  char parm_uc[10];
  const char *command    = root[PSTR(parm_uc, PSTR(D_JSON_MOTOR_COMMAND))];
  const char *commandval = root[PSTR(parm_uc, PSTR(D_JSON_MOTOR_VALUE))];

  if (!(command && commandval)) {
    return A4988_INVALID_JSON;
  }

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("MOTOR: command: '%s', value: '%s'"),command, commandval);

  switch (command)
  {     case "doMove": {
            long stepsPlease = 1;
            stepsPlease = strtoul(commandval,nullptr,10);
            myA4988->doMove(stepsPlease); break;
        }
        case "doRotate" : {
            long degrsPlease = 1;
            degrsPlease = strtoul(commandval,nullptr,10);
            AddLog_P2(LOG_LEVEL_INFO, PSTR("A4988: Rotating %d degrs"), degrsPlease);
            myA4988->doRotate(degrsPlease); break;
        }
        case "doTurn" : {
          float turnsPlease = 0;
          turnsPlease = strtod(commandval,nullptr);
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("A4988: Turning %d times"), turnsPlease);
          myA4988->doTurn(turnsPlease); break;
        }
        default:
          ResponseCmndChar(D_JSON_PROTOCOL_NOT_SUPPORTED);
  }

      return A4988_NO_ERROR;
}

void CmndMOTOR(void){
  if (XdrvMailbox.data_len) {
    if (strstr(XdrvMailbox.data, "{") == nullptr) {
      error = A4988_NO_JSON_COMMAND;
    } else {
      error = MOTORCmndJson();
    }
  }
  A4988CmndResponse(error);
}

void A4988CmndResponse(uint32_t error){

}

void CmndGetSPR(void) {
  AddLog_P2(LOG_LEVEL_INFO, PSTR("A4988: SPR = %d steps"), myA4988->getSPR());
}

void CmndGetRPM(void) {
  AddLog_P2(LOG_LEVEL_INFO, PSTR("A4988: RPM = %d rounds"), myA4988->getRPM());
}

void CmndGetMIC(void) {
  AddLog_P2(LOG_LEVEL_INFO, PSTR("A4988: MIC = %d steps"), myA4988->getMIC());
}

void CmndDoMove(void)
{
  if (XdrvMailbox.data_len > 0) {
    long stepsPlease = 1;
    stepsPlease = strtoul(XdrvMailbox.data,nullptr,10);
    AddLog_P2(LOG_LEVEL_INFO, PSTR("A4988: Moving %d steps"), stepsPlease);
    myA4988->doMove(stepsPlease);
  } else {
    AddLog_P2(LOG_LEVEL_INFO, PSTR("A4988: Moving 25 steps"));
    myA4988->doMove(25);
  }
  ResponseCmndDone();
}

void CmndDoRotate(void)
{
  if (XdrvMailbox.data_len > 0) {
    long degrsPlease = 1;
    degrsPlease = strtoul(XdrvMailbox.data,nullptr,10);
    AddLog_P2(LOG_LEVEL_INFO, PSTR("A4988: Rotating %d degrs"), degrsPlease);
    myA4988->doRotate(degrsPlease);
  } else {
    AddLog_P2(LOG_LEVEL_INFO, PSTR("A4988: Moving 45 degrs"));
    myA4988->doRotate(45);
  }
  ResponseCmndDone();
}

void CmndDoTurn(void)
{
  if (XdrvMailbox.data_len > 0) {
    float turnsPlease = 0;
    turnsPlease = strtod(XdrvMailbox.data,nullptr);
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("A4988: Turning %d times"), turnsPlease);
    myA4988->doTurn(turnsPlease);
  } else {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("A4988: Turning 0.25 times"));
    myA4988->doRotate(0.25);
  }
  ResponseCmndDone();
}

void CmndSetRPM(void)
{
  short rpmPlease = 60;
  if (XdrvMailbox.data_len > 0) {
    rpmPlease = strtoul(XdrvMailbox.data,nullptr,10);
  }
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("A4988: RPM set to %d"), rpmPlease);
  myA4988->setRPM(rpmPlease);
  ResponseCmndDone();
}

void CmndSetSPR(void)
{
  int sprPlease = 200;
  if (XdrvMailbox.data_len > 0) {
    sprPlease = strtoul(XdrvMailbox.data,nullptr,10);
  }
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("A4988: SPR set to %d"), sprPlease);
  myA4988->setSPR(sprPlease);
  ResponseCmndDone();
}

void CmndSetMIS(void)
{
  if ((pin[GPIO_A4988_MS1] < 99) && (pin[GPIO_A4988_MS2] < 99) && (pin[GPIO_A4988_MS3] < 99)) {
    short micPlease = 1;
    if (XdrvMailbox.data_len > 0) {
      micPlease = strtoul(XdrvMailbox.data,nullptr,10);
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("A4988: Microsteps set to %d"), micPlease);
      myA4988->setMIC(micPlease);
    }
  } else {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("A4988: Microsteps constant = 1. You'll have to define GPIO's for MS1-MS3 and connect them to A4988 or hardwire A4988 itself"));
  }
  ResponseCmndDone();
}

void CmndDoVader(void){
  myA4988->enable();
  myA4988->setRPM(25); myA4988->doMove( 30); delay(5);
  myA4988->setRPM(25); myA4988->doMove(-30); delay(5);
  myA4988->setRPM(25); myA4988->doMove( 30); delay(5);
  myA4988->setRPM(20); myA4988->doMove(-25); delay(5);
  myA4988->setRPM(30); myA4988->doMove( 20); delay(5);
  myA4988->setRPM(25); myA4988->doMove( 50); delay(5);
  myA4988->setRPM(20); myA4988->doMove(-25); delay(5);
  myA4988->setRPM(30); myA4988->doMove( 20); delay(5);
  myA4988->setRPM(25); myA4988->doMove(-50); delay(5);//
  myA4988->setRPM(38); myA4988->doMove( 31); delay(5);
  myA4988->setRPM(38); myA4988->doMove(-31); delay(5);
  myA4988->setRPM(38); myA4988->doMove( 31); delay(5);
  myA4988->setRPM(38); myA4988->doMove(-31); delay(5);
  myA4988->setRPM(41); myA4988->doMove( 21); delay(5);
  myA4988->setRPM(30); myA4988->doMove(-21); delay(5);
  myA4988->setRPM(25); myA4988->doMove( 30); delay(5);
  myA4988->setRPM(20); myA4988->doMove(-25); delay(5);
  myA4988->setRPM(30); myA4988->doMove( 20); delay(5);
  myA4988->setRPM(25); myA4988->doMove(-50); delay(5);//
  myA4988->setRPM(50); myA4988->doMove( 40); delay(5);
  myA4988->setRPM(25); myA4988->doMove(-20); delay(5);
  myA4988->setRPM(25); myA4988->doMove( 15); delay(5);
  myA4988->setRPM(50); myA4988->doMove(-40); delay(5);
  myA4988->setRPM(48); myA4988->doMove( 35); delay(5);
  myA4988->setRPM(45); myA4988->doMove(-25); delay(5);
  myA4988->setRPM(41); myA4988->doMove( 25); delay(5);
  myA4988->setRPM(38); myA4988->doMove(-26); delay(5);
  myA4988->setRPM(41); myA4988->doMove( 25); delay(5);
  myA4988->setRPM(25); myA4988->doMove( 25); delay(5);
  myA4988->setRPM(41); myA4988->doMove(-35); delay(5);
  myA4988->setRPM(38); myA4988->doMove( 25); delay(5);
  myA4988->setRPM(36); myA4988->doMove(-25); delay(5);
  myA4988->setRPM(33); myA4988->doMove( 25); delay(5);
  myA4988->setRPM(30); myA4988->doMove(-27); delay(5);
  myA4988->setRPM(33); myA4988->doMove( 25); delay(5);
  myA4988->setRPM(33); myA4988->doMove( 25); delay(5);
  myA4988->setRPM(38); myA4988->doMove(-30); delay(5);
  myA4988->setRPM(18); myA4988->doMove( 10); delay(5);
  myA4988->setRPM(18); myA4988->doMove(-10); delay(5);
  myA4988->setRPM(25); myA4988->doMove( 20); delay(5);
  myA4988->setRPM(20); myA4988->doMove(-25); delay(5);
  myA4988->setRPM(30); myA4988->doMove( 20); delay(5);
  myA4988->setRPM(25); myA4988->doMove(-50); delay(5);
  myA4988->setRPM(20); myA4988->doMove( 25); delay(5);
  myA4988->setRPM(30); myA4988->doMove(-20); delay(5);
  myA4988->setRPM(25); myA4988->doMove( 50); delay(5);
  myA4988->disable();
  AddLog_P2(LOG_LEVEL_INFO, PSTR("Stepper: may the force be with you!"));
  ResponseCmndDone();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdrv98(uint8_t function)
{
  bool result = false;

  if ((pin[GPIO_A4988_DIR] < 99) && (pin[GPIO_A4988_STP] < 99)) {
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

#endif
