
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
#define XDRV_25                    25

enum A4988Errors { A4988_NO_ERROR, A4988_NO_JSON_COMMAND, A4988_INVALID_JSON};

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

const char kA4988Commands[] PROGMEM = "MOTOR|"
  "doMove|doRotate|doTurn|setSPR|setRPM|setMIS";

void (* const A4988Command[])(void) PROGMEM = { &CmndMOTOR};

uint32_t MOTORCmndJson(void)
{
  // MOTOR {"Command":"doMove","Value":200}
  // MOTOR {"Command":"doRotate","Value":360}
  // MOTOR {"Command":"doTurn","Value":1.0}
  char dataBufUc[XdrvMailbox.data_len];
  UpperCase(dataBufUc, XdrvMailbox.data);
  RemoveSpace(dataBufUc);
  if (strlen(dataBufUc) < 8) { return A4988_INVALID_JSON; }

  DynamicJsonBuffer jsonBuf;
  JsonObject &json = jsonBuf.parseObject(dataBufUc);
  if (!json.success()) { return A4988_INVALID_JSON; }
  if (json.containsKey(D_JSON_MOTOR_MOVE )){
    long stepsPlease = 50;
    stepsPlease = strtoul(json[D_JSON_MOTOR_MOVE],nullptr,10);
    myA4988->doMove(stepsPlease);
  } else if (json.containsKey(D_JSON_MOTOR_ROTATE )){
    long degrsPlease = 45;
    degrsPlease = strtoul(json[D_JSON_MOTOR_ROTATE],nullptr,10);
    myA4988->doRotate(degrsPlease);
  } else if (json.containsKey(D_JSON_MOTOR_TURN )){
    float turnsPlease = 0.25;
    turnsPlease = strtod(json[D_JSON_MOTOR_TURN],nullptr);
    myA4988->doTurn(turnsPlease);
  } else if (json.containsKey(D_JSON_MOTOR_SPR )){
    int howManySteps =strtoul(json[D_JSON_MOTOR_SPR],nullptr,10);
    myA4988->setSPR(howManySteps);
  } else if (json.containsKey(D_JSON_MOTOR_RPM )){
    int howManyRounds =strtoul(json[D_JSON_MOTOR_RPM],nullptr,10);
    myA4988->setRPM(howManyRounds);
  } else if (json.containsKey(D_JSON_MOTOR_MIS )){
    short oneToSixteen =strtoul(json[D_JSON_MOTOR_MIS],nullptr,10);
    myA4988->setMIS(oneToSixteen);
  } else return A4988_NO_JSON_COMMAND;
  return A4988_NO_ERROR;
}

void CmndMOTOR(void){
  uint32_t error;
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
  switch (error) {
    case A4988_NO_JSON_COMMAND:
      ResponseCmndChar(D_JSON_INVALID_JSON);
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
