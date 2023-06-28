/*
  xdrv_129_rddl_network.ino - RDDL Network protocol integration

  Copyright (C) 2023  Jürgen Eckel

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
#define USE_DRV_RDDL_NETWORK
#ifdef USE_DRV_RDDL_NETWORK
/*********************************************************************************************\
 * Settings to participate in the RDDL Network
 *
 * To test this file:
 * - ****************
 * - *********** in  platform_override.ini
\*********************************************************************************************/

#include "rddl.h"

#ifdef USE_WEBCLIENT_HTTPS
#warning **** USE_WEBCLIENT_HTTPS is enabled ****
#endif

#define XDRV_129               129

#define DRV_DEMO_MAX_DRV_TEXT  16
//#define RDDL_NETWORK_15_MINUTES_IN_MS  (15*60*1000)
#define RDDL_NETWORK_NOTARIZATION_TIMER_IN_SECONDS (30)

//#include <HT#TPClientLight.h>


uint32_t counted_seconds = 0;

// Demo command line commands
const char kDrvDemoCommands[] PROGMEM = "Drv|"  // Prefix
  "Text";

void (* const DrvDemoCommand[])(void) PROGMEM = {
  &CmndDrvText };

// Global structure containing driver saved variables
struct {
  uint32_t  crc32;    // To detect file changes
  uint32_t  version;  // To detect driver function changes
  char      drv_text[DRV_DEMO_MAX_DRV_TEXT -1][10];
} DrvDemoSettings;

// Global structure containing driver non-saved variables
struct {
  uint32_t any_value;
} DrvDemoGlobal;

void CmndDrvText(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= DRV_DEMO_MAX_DRV_TEXT)) {
    if (!XdrvMailbox.usridx) {
      // Command DrvText
      for (uint32_t i = 0; i < DRV_DEMO_MAX_DRV_TEXT; i++) {
        AddLog(LOG_LEVEL_DEBUG, PSTR("DRV: DrvText%02d %s"), i, DrvDemoSettings.drv_text[i]);
      }
      ResponseCmndDone();
    } else {
      // Command DrvText<index> <text>
      uint32_t index = XdrvMailbox.index -1;
      if (XdrvMailbox.data_len > 0) {
        snprintf_P(DrvDemoSettings.drv_text[index], sizeof(DrvDemoSettings.drv_text[index]), XdrvMailbox.data);
      }
      ResponseCmndIdxChar(DrvDemoSettings.drv_text[index]);
    }
  }
}

/*********************************************************************************************\
 * Driver Settings load and save
\*********************************************************************************************/

void RDDLNetworkSettingsLoad(bool erase) {
  // Called from FUNC_PRE_INIT (erase = 0) once at restart
  // Called from FUNC_RESET_SETTINGS (erase = 1) after command reset 4, 5, or 6

}

void RDDLNetworkSettingsSave(void) {
  // Called from FUNC_SAhttp://tasmota/VE_SETTINGS every SaveData second and at restart

}

void getNotarizationMessage(){
  MqttShowSensor(false);
}

void signRDDLNetworkMessage(int start_position){
    char pubkey_out[68] = {0};
    char sig_out[130] = {0};
    char hash_out[66] = {0};
    int current_position  = ResponseLength();
    const char* data_str = TasmotaGlobal.mqtt_data.c_str();
    SignDataHash(start_position, current_position, data_str, pubkey_out, sig_out, hash_out);
    ResponseAppend_P(PSTR(",\"%s\":\"%s\""), "EnergyHash", hash_out);
    ResponseAppend_P(PSTR(",\"%s\":\"%s\""), "EnergySig", sig_out);
    ResponseAppend_P(PSTR(",\"%s\":\"%s\""), "PublicKey", pubkey_out);
}

void getCIDString( char* cidbuffer, size_t buffersize, const char* message ){

}

String getEdDSAChallenge( const char* public_key ){
  String challenge;
  HTTPClientLight http;

  http.begin("https://cid-resolver.rddl.io/auth/?public_key=asdkfj%C3%B6alskdjf%C3%B6aldskjf");
  //http.begin("http://node1-rddl-testnet.twilightparadox.com:9984/");
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String payload = http.getString();
    Serial.println(httpResponseCode);
    Serial.println(payload);
    
    int start = payload.indexOf("{");
    int end = payload.indexOf("}");
    //start +14 to skip  {"challenge":"
    //end -2 to skip "}
    challenge = payload.substring(start+14, end-2);
    ResponseAppend_P(PSTR("Payload received %s}"), challenge.c_str());

  }
  else {
    Serial.println("Error on HTTP request");
    ResponseAppend_P(PSTR("Error on HTTPS request"));
  }

  http.end();
  return challenge;
}
void getAuthToken(){
  char* public_key = NULL;
  // get public key
  String challenge = getEdDSAChallenge(public_key);
  //sign EdDSA challenge
  //send signed challenge to  server, get JWT

}

void sendNotarizationMessage(){

}

#define CID_BUFFER_SIZE 256

void RDDLNotarize(){
  //char cidbuffer[CID_BUFFER_SIZE] = {0};

  // create notarization message
  Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS8_POWER "\":"));
  int start_position = ResponseLength();
  getNotarizationMessage();
  signRDDLNetworkMessage(start_position);
  ResponseJsonEnd();
  //getAuthToken();
  // compute CID of the message
  //getCIDString( cidbuffer, CID_BUFFER_SIZE, TasmotaGlobal.mqtt_data.c_str() );
  
  // upload message to cid-resolver
  // auth to cid-resolver
  // send notarize message to cid-resolver

  // notarize message vi planetmint
    
  sendNotarizationMessage();
}

void RDDLNetworkNotarizationScheduler(){
  ++counted_seconds;
  if( counted_seconds >= RDDL_NETWORK_NOTARIZATION_TIMER_IN_SECONDS)
  {
    counted_seconds=0;
    //RDDLNotarize();
  }
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv129(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_RESET_SETTINGS:
      RDDLNetworkSettingsLoad(1);
      break;
    case FUNC_SAVE_SETTINGS:
      RDDLNetworkSettingsSave();
      break;
    case FUNC_COMMAND:
      RDDLNotarize();
      result = true;
      break;
    case FUNC_PRE_INIT:
      RDDLNetworkSettingsLoad(0);
      break;
    case FUNC_SAVE_BEFORE_RESTART:
      // !!! DO NOT USE AS IT'S FUNCTION IS BETTER HANDLED BY FUNC_SAVE_SETTINGS !!!
      break;
  }
  return result;
}

#endif  // USE_DRV_FILE_DEMO