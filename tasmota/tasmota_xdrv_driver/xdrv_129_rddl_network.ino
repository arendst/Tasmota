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
#define RDDL_NETWORK_NOTARIZATION_TIMER_IN_SECONDS (3*60)


#include "planetmint.h"
#include "rddl.h"
#include "rddl_cid.h"
#include "bip39.h"
#include "bip32.h"
#include "curves.h"
#include "ed25519.h"
#include "base58.h"

#include "rddl_types.h"
#include "planetmintgo.h"
#include "planetmintgo/machine/machine.pb-c.h"
#include "cosmos/tx/v1beta1/tx.pb-c.h"
#include "planetmintgo/machine/tx.pb-c.h"
#include "planetmintgo/asset/tx.pb-c.h"
#include "google/protobuf/any.pb-c.h"


#define EXT_PUB_KEY_SIZE 112
uint32_t counted_seconds = 0;

uint8_t g_priv_key_planetmint[32+1] = {0};
uint8_t g_priv_key_liquid[32+1] = {0};
uint8_t g_pub_key_planetmint[33+1] = {0};
uint8_t g_pub_key_liquid[33+1] = {0};
char g_address[64] = {0};
char g_ext_pub_key_planetmint[EXT_PUB_KEY_SIZE+1] = {0};
char g_ext_pub_key_liquid[EXT_PUB_KEY_SIZE+1] = {0};

const char* getRDDLAddress() { return (const char*) g_address; }
const char* getExtPubKeyLiquid() { return (const char*)g_ext_pub_key_liquid; }
const char* getExtPubKeyPlanetmint() { return (const char*)g_ext_pub_key_planetmint; }
const uint8_t* getPrivKeyLiquid() { return (const uint8_t*)g_priv_key_liquid; }
const uint8_t* getPrivKeyPlanetmint() { return (const uint8_t*)g_priv_key_planetmint; }

bool g_readSeed = false;

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
  char* hexstring = (char*)malloc(length+1);
  hexstring[length] = 0;
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
  storeKeyValuePair( (const char*)"seed", (const char*) secret_seed, SEED_SIZE);
  g_readSeed = false;
}

#define PUB_KEY_SIZE 33
#define ADDRESS_HASH_SIZE 20
#define ADDRESS_TAIL 20

void getPlntmntKeys(){
  readSeed();
  HDNode node_planetmint;
  hdnode_from_seed( secret_seed, SEED_SIZE, SECP256K1_NAME, &node_planetmint);
  hdnode_private_ckd_prime(&node_planetmint, 44);
  hdnode_private_ckd_prime(&node_planetmint, 8680);
  hdnode_private_ckd_prime(&node_planetmint, 0);
  hdnode_private_ckd(&node_planetmint, 0);
  hdnode_private_ckd(&node_planetmint, 0);
  hdnode_fill_public_key(&node_planetmint);
  memcpy(g_priv_key_planetmint, node_planetmint.private_key, 32);
  memcpy(g_pub_key_planetmint, node_planetmint.public_key, PUB_KEY_SIZE);

  HDNode node_rddl;
  hdnode_from_seed( secret_seed, SEED_SIZE, SECP256K1_NAME, &node_rddl);
  hdnode_private_ckd_prime(&node_rddl, 44);
  hdnode_private_ckd_prime(&node_rddl, 1776);
  hdnode_private_ckd_prime(&node_rddl, 0);
  hdnode_private_ckd(&node_rddl, 0);
  hdnode_private_ckd(&node_rddl, 0);
  hdnode_fill_public_key(&node_rddl);
  memcpy(g_priv_key_liquid, node_rddl.private_key, 32);
  memcpy(g_pub_key_liquid, node_rddl.public_key, PUB_KEY_SIZE);

  
  uint8_t address_bytes[ADDRESS_TAIL] = {0};
  pubkey2address( g_pub_key_planetmint, PUB_KEY_SIZE, address_bytes );
  getAddressString( address_bytes, g_address);
  uint32_t fingerprint = hdnode_fingerprint(&node_planetmint);
  int ret = hdnode_serialize_public( &node_planetmint, fingerprint, PLANETMINT_PMPB, g_ext_pub_key_planetmint, EXT_PUB_KEY_SIZE);
  int ret2 = hdnode_serialize_public( &node_rddl, fingerprint, VERSION_PUBLIC, g_ext_pub_key_liquid, EXT_PUB_KEY_SIZE);
}

bool hasKey(const char * key){
  char result [10]= {0};
  BrREPLRun((char*)"import persist");
  String cmd = String("persist.has(\"") + String(key) + String("\")");
  BrREPLRunRDDL((char*)cmd.c_str(), result );
  if( strcmp( result,"true") == 0)
    return true;
  else 
    return false;
}



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
  char pubkey_out[66+1] = {0};
  char sig_out[128+1] = {0};
  char hash_out[64+1] = {0};
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
    //ResponseAppend_P(PSTR("Payload received %s\n"), challenge.c_str());

  }
  else {
    Serial.println("Error on HTTP request\n");
    //ResponseAppend_P(PSTR("Error on HTTPS request\n"));
  }

  http.end();
  return challenge;
}

String getJWTTokenCall( const char* public_key, const unsigned char* signature){
  String jwt_token;
  HTTPClientLight http;
  //char hexed_signature[200]={0};

  //toHexString( hexed_signature, (unsigned char*)signature, 68 );
  //ResponseAppend_P(PSTR("b58 signature: %s\n"), signature);
  String uri = "https://cid-resolver.rddl.io/auth/?public_key=";
  uri = uri + public_key + "&signature=" + (char *)signature;
  //ResponseAppend_P(PSTR("JWT URI: %s\n"), uri.c_str());
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
    //ResponseAppend_P(PSTR("Payload received JWT: %s\n"), jwt_token.c_str());

  }
  else {
    Serial.println("Error on HTTP request");
    //ResponseAppend_P(PSTR("Error on HTTPS request\n"));
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
    //ResponseAppend_P(PSTR("Payload received CID registration: %s\n"), result.c_str());

  }
  else {
    Serial.println("Error on HTTP request\n");
    //ResponseAppend_P(PSTR("Error on HTTPS request\n"));
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
  // if( ret == true )
  //   ResponseAppend_P(PSTR("B58 encoding: %u - %s\n"), b58_size, b58_pub_key);
  // else
  //   ResponseAppend_P(PSTR("B58 encoding: failed\n"));

  String challenge = getEdDSAChallengeCall((const char *)b58_pub_key);

  SHA256_CTX ctx;
  uint8_t hash[SHA256_DIGEST_LENGTH+1] = {0};
  sha256_Init(&ctx);
  sha256_Update(&ctx, (const uint8_t*)challenge.c_str(), challenge.length());
  sha256_Final(&ctx, hash);

  char hexbuf[67]={0};
  toHexString( hexbuf,(uint8_t*)hash, 66 );
  //ResponseAppend_P(PSTR("HASH: %s\n"), hexbuf);

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

int broadcast_TX( const char* tx_bytes ){
  char planetmintapi[100] = {0};
  getValueForKey( "planetmintapi", planetmintapi);
  HTTPClientLight http;
  String uri = "/cosmos/tx/v1beta1/txs";
  uri = planetmintapi + uri;
  http.begin(uri);
  http.addHeader("accept", "application/json");
  http.addHeader("Content-Type", "application/json");
  String payload = "{ \"tx_bytes\": \"";
  payload = payload + tx_bytes + "\", \"mode\": \"BROADCAST_MODE_SYNC\" }";

  int ret = http.POST( payload );
  ResponseAppend_P(PSTR(",\"%s\":\"%u\"\n"), "respose code", ret);
  //ResponseAppend_P(PSTR(",\"%s\":\"%u\"\n"), "respose code", ret);
  return ret;
}



bool getAccountInfo( const char* account_address, uint64_t* account_id, uint64_t* sequence )
{
  // get account info from planetmint-go
  HTTPClientLight http;
  String uri = "/cosmos/auth/v1beta1/account_info/";
  char planetmintapi[100] = {0};
  getValueForKey( "planetmintapi", planetmintapi);
  uri = planetmintapi + uri;
  uri = uri + g_address;
  http.begin(uri);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.GET();
  int _account_id = 0;
  int _sequence = 0;
  bool ret = get_account_info( http.getString().c_str() ,&_account_id, &_sequence );
  if( !ret )
    return false;
  // {

  //   uri = "/cosmos/auth/v1beta1/accounts";
  //   uri = PLANETMINT_API_URI + uri;
  //   http.begin(uri);
  //   http.addHeader("Content-Type", "application/json");

  //   int httpResponseCode = http.GET();
  //   ret = get_address_info_from_accounts( http.getString().c_str(), account_address, &_account_id, &_sequence );
  //   if( !ret )
  //     return false;
  // }
  *account_id = (uint64_t) _account_id;
  *sequence = (uint64_t) _sequence;
  return true;
}


int create_broadcast_tx( void* anyMsg, char* tokenAmount, bool first_tx )
{

 Google__Protobuf__Any* local_msg = (Google__Protobuf__Any*) anyMsg;
  
  // get address 
  getPlntmntKeys();

  uint64_t account_id = 0;
  uint64_t sequence = 0;
  bool ret = getAccountInfo( g_address, &account_id, &sequence );
  if( !ret )
  {
    if( !first_tx )
      return -2;

    char buffer[10] = {0};
    char* paccountid = getValueForKey("accountid", buffer);
    account_id = (uint64_t) atoi( (const char*) buffer);
  }

  Cosmos__Base__V1beta1__Coin coin = COSMOS__BASE__V1BETA1__COIN__INIT;
  coin.denom = "token";
  coin.amount = tokenAmount;
  
  uint8_t* txbytes = NULL;
  size_t tx_size = 0;
  char* chain_id = "planetmintgo";
  prepareTx( local_msg, &coin, g_priv_key_planetmint, g_pub_key_planetmint, sequence, chain_id, account_id, &txbytes, &tx_size);
  //free(anyMsg->value.data);

  char* tx_bytes_b64 = (char*) malloc( 1000 );
  char * p = bintob64( tx_bytes_b64, txbytes, tx_size);

  int broadcast_return = broadcast_TX( tx_bytes_b64 );
  free(tx_bytes_b64);

  return broadcast_return;
}

int registerMachine(){
    char hexpubkey[66+1] = {0};
    toHexString( hexpubkey, g_pub_key_planetmint, 66);

    Planetmintgo__Machine__Metadata metadata = PLANETMINTGO__MACHINE__METADATA__INIT;
    metadata.additionaldatacid = "";
    metadata.gps = "{\"Latitude\":\"-48.876667\",\"Longitude\":\"-123.393333\"}";
    metadata.assetdefinition = "{\"Version\": \"0.1\"}";
    metadata.device = "{\"Manufacturer\": \"RDDL\",\"Serial\":\"AdnT2uyt\"}";

    Planetmintgo__Machine__Machine machine = PLANETMINTGO__MACHINE__MACHINE__INIT;
    machine.name = (char*)g_address;
    machine.ticker = NULL;
    machine.domain = "lab.r3c.network";
    machine.reissue = false;
    machine.amount = 1;
    machine.precision = 8;
    machine.issuerplanetmint = g_ext_pub_key_planetmint;
    machine.issuerliquid = g_ext_pub_key_liquid;
    machine.machineid = hexpubkey;
    machine.metadata = &metadata;
    machine.type = RDDL_MACHINE_POWER_SWITCH;

    Planetmintgo__Machine__MsgAttestMachine machineMsg = PLANETMINTGO__MACHINE__MSG_ATTEST_MACHINE__INIT;
    machineMsg.creator = (char*)g_address;
    machineMsg.machine = &machine;
    Google__Protobuf__Any msg = GOOGLE__PROTOBUF__ANY__INIT;
    generateAnyAttestMachineMsg( &msg, &machineMsg);
    int ret = create_broadcast_tx( &msg, "2", true);
    free( msg.value.data );
    return ret;
}

void runRDDLNotarizationWorkflow(const char* data_str, size_t data_length){

  String signature = signRDDLNetworkMessageContent(data_str, data_length);

  //compute CID
  const char* cid_str = (const char*) create_cid_v1_from_string( data_str );

  // store cid
  storeKeyValuePair( cid_str, data_str,0 );

  // register CID
  registerCID( cid_str );
 
  // notarize message vi planetmint
  getPlntmntKeys();

  Google__Protobuf__Any anyMsg = GOOGLE__PROTOBUF__ANY__INIT;
  gnerateAnyCIDAttestMsgGeneric(&anyMsg, cid_str, g_priv_key_planetmint, g_pub_key_planetmint, g_address );
  int ret = create_broadcast_tx(&anyMsg, "2", false);
  free( anyMsg.value.data);

  ResponseJsonEnd();
  free( (char*)cid_str );

  if( ret == -2 )
  {
    Serial.println("Register: Machine\n");
    registerMachine();
  }
  else
  {
    Serial.println("Notarize: CID Asset\n");
  }
  
}

void RDDLNotarize(){
  // create notarization message
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
    RDDLNotarize();
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
