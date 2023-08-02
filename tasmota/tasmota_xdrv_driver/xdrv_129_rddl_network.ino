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

#include "esp_random.h"

#define XDRV_129               129

#define DRV_DEMO_MAX_DRV_TEXT  16
//#define RDDL_NETWORK_15_MINUTES_IN_MS  (15*60*1000)
#define RDDL_NETWORK_NOTARIZATION_TIMER_IN_SECONDS (30)


#include "planetmint.h"
#include "rddl.h"
#include "rddl_cid.h"
#include "bip39.h"
#include "bip32.h"
#include "curves.h"
#include "ed25519.h"
#include "base58.h"


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


void storeKeyValuePair( const char* key, const char* value, size_t length)
{
  
  if( 0 == length )
    length = strlen(value);
  length = length * 2;
  char* hexstring = (char*)malloc(length);
  toHexString( (char*)hexstring, (uint8_t*)value, length);
  BrREPLRun((char*)"import persist");
  String key_string = key;
  String value_string = hexstring;
  String cmd = String("persist.") + key + String("=\"") + value_string + String("\"");
  BrREPLRun((char*)cmd.c_str());
  BrREPLRun((char*)"persist.save()");
  free(hexstring);
}

void storeSeed()
{
  //uint8_t seed_message[SEED_SIZE*2] = {0};
  //toHexString( (char*)seed_message, secret_seed, SEED_SIZE*2);
  storeKeyValuePair( (const char*)"seed", (const char*) secret_seed, SEED_SIZE);
}

bool hasKey(const char * key){
  char result [300]= {0};
  BrREPLRun((char*)"import persist");
  String cmd = String("persist.has(\"") + String(key) + String("\")");
  BrREPLRunRDDL((char*)cmd.c_str(), result );
  if( strcmp( result,"true") == 0)
    return true;
  else 
    return false;
}
bool g_readSeed = false;

char* getValueForKey( const char* key, char* buffer )
{
  if( ! hasKey(key) )
    return NULL;
  String key_string = key;
  String cmd = String("persist.find(\"") + key_string + String("\")");
  BrREPLRun((char*)"import persist");
  BrREPLRunRDDL((char*)cmd.c_str(), buffer );
  const uint8_t * storageString = fromHexString(buffer);
  strcpy(buffer, (const char*) storageString );
  ResponseAppend_P(PSTR("HAS Key: %s\n"), buffer);
  return buffer;
}

uint8_t* readSeed()
{
  if( g_readSeed )
    return secret_seed;

  char buffer[300] = {0};
  char* ret = getValueForKey( "seed", buffer);
  if( ret == NULL )
    return NULL;

  //const uint8_t * seed = fromHexString(buffer);
  //memset( secret_seed, 0, SEED_SIZE );
  memcpy( secret_seed, (const void *)buffer, SEED_SIZE);
  g_readSeed = true;

  return secret_seed;
}

String signRDDLNetworkMessageContent( const char* data_str, size_t data_length){
  char pubkey_out[68] = {0};
  char sig_out[130] = {0};
  char hash_out[66] = {0};
  if( readSeed() != NULL )
  {
    SignDataHash( data_str, data_length,  pubkey_out, sig_out, hash_out);
  }
  ResponseAppend_P(PSTR(",\"%s\":\"%s\"\n"), "EnergyHash", hash_out);
  ResponseAppend_P(PSTR(",\"%s\":\"%s\"\n"), "EnergySig", sig_out);
  ResponseAppend_P(PSTR(",\"%s\":\"%s\"\n"), "PublicKey", pubkey_out);
  return String(sig_out);
}

void getCIDString( char* cidbuffer, size_t buffersize, const char* message ){

}

String getEdDSAChallengeCall( const char* public_key ){
  String challenge;
  HTTPClientLight http;

  String uri = "https://cid-resolver.rddl.io/auth/?public_key=";
  uri = uri + public_key;
  http.begin(uri);
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
    ResponseAppend_P(PSTR("Payload received %s\n"), challenge.c_str());

  }
  else {
    Serial.println("Error on HTTP request\n");
    ResponseAppend_P(PSTR("Error on HTTPS request\n"));
  }

  http.end();
  return challenge;
}

String getJWTTokenCall( const char* public_key, const unsigned char* signature){
  String jwt_token;
  HTTPClientLight http;
  //char hexed_signature[200]={0};

  //toHexString( hexed_signature, (unsigned char*)signature, 68 );
  ResponseAppend_P(PSTR("b58 signature: %s\n"), signature);
  String uri = "https://cid-resolver.rddl.io/auth/?public_key=";
  uri = uri + public_key + "&signature=" + (char *)signature;
  ResponseAppend_P(PSTR("JWT URI: %s\n"), uri.c_str());
  http.begin(uri);
  http.addHeader("accept", "application/json");

  int httpResponseCode = http.POST((uint8_t*)"",0);

  if (httpResponseCode > 0) {
    String payload = http.getString();
    Serial.println(httpResponseCode);
    Serial.println(payload);
    
    int start = payload.indexOf("{");
    int end = payload.indexOf("}");
    //start +14 to skip  {"challenge":"
    //start +17 to skip  {"access_token":"
    //end -2 to skip "}
    jwt_token = payload.substring(start+17, end-1);
    ResponseAppend_P(PSTR("Payload received JWT: %s\n"), jwt_token.c_str());

  }
  else {
    Serial.println("Error on HTTP request");
    ResponseAppend_P(PSTR("Error on HTTPS request\n"));
  }

  http.end();
  return jwt_token;
}

String registerCID_call( const char* jwt_token, const char* url, const char* cid){
  String result;
  HTTPClientLight http;

  String token = "Bearer ";
  token = token + jwt_token;


  String uri = "https://cid-resolver.rddl.io/entry/?cid=";
  uri = uri + cid + "&url=" + url;
  http.begin(uri);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", token);

  int httpResponseCode = http.POST((uint8_t*)"",0);

  if (httpResponseCode > 0) {
    String payload = http.getString();
    Serial.println(httpResponseCode);
    Serial.println(payload);
    
    int start = payload.indexOf("{");
    int end = payload.indexOf("}");
    //start +14 to skip  {"challenge":"
    //end -2 to skip "}
    result = payload.substring(start, end);
    ResponseAppend_P(PSTR("Payload received CID registration: %s\n"), result.c_str());

  }
  else {
    Serial.println("Error on HTTP request\n");
    ResponseAppend_P(PSTR("Error on HTTPS request\n"));
  }

  http.end();
  return result;
}

String registerCID(const char* cid){
  char* public_key = NULL;
  size_t b58_size = 80;
  uint8_t priv_key[33] = {0};
  uint8_t pub_key[34] = {0};
  uint8_t b58_pub_key[b58_size] = {0};
  unsigned char signature[65] = {0};
  

  readSeed();
  getKeyFromSeed((const uint8_t*)secret_seed, priv_key, pub_key, ED25519_NAME);
  char* planetmint_key = (char*) pub_key +1;
  
  bool ret = b58enc((char*)b58_pub_key, &b58_size, planetmint_key, 32);
  if( ret == true )
    ResponseAppend_P(PSTR("B58 encoding: %u - %s\n"), b58_size, b58_pub_key);
  else
    ResponseAppend_P(PSTR("B58 encoding: failed\n"));

  String challenge = getEdDSAChallengeCall((const char *)b58_pub_key);

  SHA256_CTX ctx;
  uint8_t hash[SHA256_DIGEST_LENGTH+1] = {0};
  sha256_Init(&ctx);
  sha256_Update(&ctx, (const uint8_t*)challenge.c_str(), challenge.length());
  sha256_Final(&ctx, hash);

  char hexbuf[68]={0};
  toHexString( hexbuf,(uint8_t*)hash, 66 );
  ResponseAppend_P(PSTR("HASH: %s\n"), hexbuf);

  //sign EdDSA challenge
  ed25519_sign( (const unsigned char*)hash, SHA256_DIGEST_LENGTH, (const unsigned char*)priv_key,(const unsigned char*)planetmint_key, signature);
  size_t sig_size = 200;
  char b58sig[sig_size] = {0};
  b58enc( b58sig, &sig_size, signature, 64);

  //send signed challenge to  server, get JWT
  String jwt_token = getJWTTokenCall( (const char *)b58_pub_key, (const unsigned char*)b58sig);
  
  //register CID
  char uri[300]={0};
  snprintf( uri, 300, "xmpp:%s.%s@m2m.rddl.io", cid, planetmint_key);
  registerCID_call( jwt_token.c_str(), (const char*) uri, cid);

  return jwt_token;
}

void sendNotarizationMessage(){

}

void runRDDLNotarizationWorkflow(const char* data_str, size_t data_length){
  String signature = signRDDLNetworkMessageContent(data_str, data_length);

  //compute CID
  const char* cid_str = (const char*) create_cid_v1_from_string( data_str );
  ResponseAppend_P(PSTR(",\"%s\":\"%s\"\n"), "Data ", data_str);
  ResponseAppend_P(PSTR(",\"%s\":\"%s\"\n"), "CID ", cid_str);
  
  // store cid
  storeKeyValuePair( cid_str, data_str,0 );

  // register CID
  registerCID( cid_str );
 
  // notarize message vi planetmint
  sendNotarizationMessage();

  ResponseJsonEnd();
  free( (char*)cid_str );
}

void RDDLNotarize(){
  // create notarization message
  ResponseAppend_P(PSTR(",\"%s\":\"%s\"\n"), "RDDLNotarize CALL ", "insane");
  return;
  Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS8_POWER "\":"));
  int start_position = ResponseLength();
  getNotarizationMessage();
  int current_position  = ResponseLength();
  size_t data_length = (size_t)(current_position - start_position);
  const char* data_str = TasmotaGlobal.mqtt_data.c_str() + start_position;

  runRDDLNotarizationWorkflow(data_str, data_length);
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
