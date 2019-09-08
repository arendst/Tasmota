
/*
  xdrv_25_A4988_Stepper.ino - A4988-StepMotorDriverCircuit- support for Sonoff-Tasmota

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
#define XDRV_25                    25

enum A4988Errors { A4988_NO_ERROR, A4988_NO_JSON_COMMAND, A4988_INVALID_JSON, A4988_MOVE, A4988_ROTATE, A4988_TURN};

short A4988_dir_pin = pin[GPIO_MAX];
short A4988_stp_pin = pin[GPIO_MAX];
short A4988_ms1_pin = pin[GPIO_MAX];
short A4988_ms2_pin = pin[GPIO_MAX];
short A4988_ms3_pin = pin[GPIO_MAX];
short A4988_ena_pin = pin[GPIO_MAX];
int   A4988_spr    = 0;
float A4988_rpm    = 0;
short A4988_mis    = 0;

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
}

const char kA4988Commands[] PROGMEM = "|"
  "MOTOR";

void (* const A4988Command[])(void) PROGMEM = { &CmndMOTOR};

uint32_t MOTORCmndJson(void)
{
  // MOTOR {"doMove":200}
  // MOTOR {"doRotate":360}
  // MOTOR {"doTurn":1.0}
  uint32_t returnValue =A4988_NO_JSON_COMMAND;

  char parm_uc[12];
  char dataBufUc[XdrvMailbox.data_len];
  UpperCase(dataBufUc, XdrvMailbox.data);
  RemoveSpace(dataBufUc);
  if (strlen(dataBufUc) < 8) { returnValue =A4988_INVALID_JSON; }

  DynamicJsonBuffer jsonBuf;
  JsonObject &json = jsonBuf.parseObject(dataBufUc);
  if (json.success()) {
    while (json.count()>0) {
    UpperCase_P(parm_uc, PSTR(D_JSON_MOTOR_SPR));
    if (json.containsKey(parm_uc)){
      int howManySteps =strtoul(json[parm_uc],nullptr,10);
      myA4988->setSPR(howManySteps);
      returnValue = A4988_NO_ERROR;
      json.remove(parm_uc);
    }
    UpperCase_P(parm_uc, PSTR(D_JSON_MOTOR_RPM));
    if (json.containsKey(parm_uc)){
      int howManyRounds =strtoul(json[parm_uc],nullptr,10);
      myA4988->setRPM(howManyRounds);
      returnValue = A4988_NO_ERROR;
      json.remove(parm_uc);
    }
    UpperCase_P(parm_uc, PSTR(D_JSON_MOTOR_MIS));
    if (json.containsKey(parm_uc)){
      short oneToSixteen =strtoul(json[parm_uc],nullptr,10);
      myA4988->setMIS(oneToSixteen);
      returnValue = A4988_NO_ERROR;
      json.remove(parm_uc);
    }
    UpperCase_P(parm_uc, PSTR(D_JSON_MOTOR_MOVE));
    if (json.containsKey(parm_uc)){
      long stepsPlease = strtoul(json[parm_uc],nullptr,10);
      myA4988->doMove(stepsPlease);
      returnValue = A4988_MOVE;
      json.remove(parm_uc);
    }
    UpperCase_P(parm_uc, PSTR(D_JSON_MOTOR_ROTATE));
    if (json.containsKey(parm_uc)){
      long degrsPlease = strtoul(json[parm_uc],nullptr,10);
      myA4988->doRotate(degrsPlease);
      returnValue = A4988_ROTATE;
      json.remove(parm_uc);
    }
    UpperCase_P(parm_uc, PSTR(D_JSON_MOTOR_TURN));
    if (json.containsKey(parm_uc)){
      float turnsPlease = strtod(json[parm_uc],nullptr);
      myA4988->doTurn(turnsPlease);
      returnValue = A4988_TURN;
      json.remove(parm_uc);
    }
  }
  } else returnValue =A4988_INVALID_JSON;
  return returnValue;
}

void CmndMOTOR(void){
  uint32_t error;
  if (XdrvMailbox.data_len) {
    if (strstr(XdrvMailbox.data, "}") == nullptr) {
      error = A4988_NO_JSON_COMMAND;
    } else {
      error = MOTORCmndJson();
    }
  }
  A4988CmndResponse(error);
}

void A4988CmndResponse(uint32_t error){
  switch (error) {
    case A4988_NO_JSON_COMMAND:
      ResponseCmndChar(PSTR("Kein Commando!"));
      break;
    case A4988_MOVE:
      ResponseCmndChar(PSTR("Stepping!"));
      break;
    case A4988_ROTATE:
      ResponseCmndChar(PSTR("Rotating!"));
      break;
    case A4988_TURN:
      ResponseCmndChar(PSTR("Turning!"));
      break;
    default:  // A4988_NO_ERROR
      ResponseCmndDone();
  }

}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdrv25(uint8_t function)
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
