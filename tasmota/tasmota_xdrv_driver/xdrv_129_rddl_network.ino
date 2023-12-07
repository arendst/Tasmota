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
#define RDDL_NETWORK_NOTARIZATION_TIMER_ONE_HOUR_IN_SECONDS (60*60)

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>


#include "hmac.h"
#include "sha3.h"

#include "base64_plntmnt.h"
#include "ed25519.h"

#ifdef ESP32
#ifdef USE_SDCARD
#include <SD.h>
#endif  // USE_SDCARD
#include "FFat.h"
#include "FS.h"
#endif  // ESP32

#include "rddl.h"
#include "rddl_cid.h"
#include "bip39.h"
#include "bip32.h"
#include "curves.h"
#include "ed25519.h"
#include "base58.h"
#include "math.h"
#include "secp256k1.h"

#include "rddl_types.h"
#include "planetmintgo.h"
#include "planetmintgo/machine/machine.pb-c.h"
#include "cosmos/tx/v1beta1/tx.pb-c.h"
#include "planetmintgo/machine/tx.pb-c.h"
#include "planetmintgo/asset/tx.pb-c.h"
#include "google/protobuf/any.pb-c.h"

#include "rddlSDKAPI.h"

#define EXT_PUB_KEY_SIZE 112
uint32_t counted_seconds = 0;

uint8_t g_priv_key_planetmint[32+1] = {0};
uint8_t g_priv_key_liquid[32+1] = {0};
uint8_t g_pub_key_planetmint[33+1] = {0};
uint8_t g_pub_key_liquid[33+1] = {0};
uint8_t g_machineid_public_key[33+1]={0};


char g_address[64] = {0};
char g_ext_pub_key_planetmint[EXT_PUB_KEY_SIZE+1] = {0};
char g_ext_pub_key_liquid[EXT_PUB_KEY_SIZE+1] = {0};
char g_machineid_public_key_hex[33*2+1] = {0};
  

const char* getRDDLAddress() { return (const char*) g_address; }
const char* getExtPubKeyLiquid() { return (const char*)g_ext_pub_key_liquid; }
const char* getExtPubKeyPlanetmint() { return (const char*)g_ext_pub_key_planetmint; }
const uint8_t* getPrivKeyLiquid() { return (const uint8_t*)g_priv_key_liquid; }
const uint8_t* getPrivKeyPlanetmint() { return (const uint8_t*)g_priv_key_planetmint; }
const char* getMachinePublicKey() { return (const char*) g_machineid_public_key_hex; }

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
    SettingsUpdateText( SET_DEVICENAME, "RDDL-Tasmota");
}

void RDDLNetworkSettingsSave(void) {
  // Called from FUNC_SAhttp://tasmota/VE_SETTINGS every SaveData second and at restart

}

void getNotarizationMessage(){
  MqttShowSensor(false);
}

#define PUB_KEY_SIZE 33
#define ADDRESS_HASH_SIZE 20
#define ADDRESS_TAIL 20

bool getPlntmntKeys(){
  if( readSeed() == NULL )
    return false;
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

  ecdsa_get_public_key33(&secp256k1, private_key_machine_id, g_machineid_public_key);
  toHexString( g_machineid_public_key_hex, g_machineid_public_key, 33*2);

  return true;
}

uint8_t* readSeed()
{
  if( g_readSeed )
    return secret_seed;

  if( !readfile( "seed", secret_seed, SEED_SIZE) )
    return NULL;

  g_readSeed = true;
  return secret_seed;
}




bool getAccountInfo( const char* account_address, uint64_t* account_id, uint64_t* sequence )
{
  // get account info from planetmint-go
  HTTPClientLight http;
  String uri = "/cosmos/auth/v1beta1/account_info/";

  uri = SettingsText(SET_PLANETMINT_API)  + uri;
  uri = uri + g_address;
  http.begin(uri);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.GET();
  int _account_id = 0;
  int _sequence = 0;
  //ResponseAppend_P(PSTR(",\"%s\":\"%s\"\n"), "Account parsing", );

  bool ret = get_account_info( http.getString().c_str() ,&_account_id, &_sequence );
  if( ret )
  {
    *account_id = (uint64_t) _account_id;
    *sequence = (uint64_t) _sequence;
  }
  else
    ResponseAppend_P("Account parsing issue\n");

  return ret;
}

int readfile( const char* filename, uint8_t* content, size_t length ){
  char* filename_local = (char*) getStack( strlen( filename ) +2 );
  filename_local[0]= '/';
  int limit = 35;
  int offset = 0;
  if( strlen(filename) > limit)
    offset = strlen(filename) -limit;
  strcpy( &filename_local[1], filename+ offset);

  return TfsLoadFile(filename_local, (uint8_t*)content, length);
}

bool rddl_writefile( const char* filename, uint8_t* content, size_t length) {
  char* filename_local = (char*) getStack( strlen( filename ) +3 );
  memset( filename_local, 0, strlen( filename ) +3 );
  filename_local[0]= '/';
  int limit = 35;
  int offset = 0;
  if( strlen(filename) > limit)
    offset = strlen(filename) -limit;
  strcpy( &filename_local[1], filename+ offset);

#ifdef USE_UFILESYS
  if (TfsSaveFile(filename_local, (const uint8_t*)content, length)) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("Failed to write file content"));
    return false;
  } 
  return true;
#else
  return false;
#endif  // USE_UFILESYS
}

bool g_mutex_running_notarization = false;

bool claimNotarizationMutex()
{
  if ( !g_mutex_running_notarization )
  {
    g_mutex_running_notarization = true;
    return true;
  }
  else
    return false;
}

void releaseNotarizationMutex()
{
  g_mutex_running_notarization = false;
}

void RDDLNotarize(){
  if( claimNotarizationMutex() )
  {
    // create notarization message
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS8_POWER "\":"));
    int start_position = ResponseLength();
    getNotarizationMessage();
    int current_position  = ResponseLength();
    size_t data_length = (size_t)(current_position - start_position);
    const char* data_str = TasmotaGlobal.mqtt_data.c_str() + start_position;

    runRDDLSDKNotarizationWorkflow(data_str, data_length);
    releaseNotarizationMutex();
  }
}

void RemoveFiles(){
  deleteOldestFiles(500);
}

extern fs::FS* TfsFileSysHandle();
void deleteOldestFiles( int count ) {
    FS* filesystem = TfsFileSysHandle();
    if( !filesystem )
      Serial.println("Failed to mount file system");
    
    File dir = filesystem->open("/");
    std::vector<String> files;
    String nextFile = dir.getNextFileName();
    while (nextFile.length() > 0) {
        //unsigned long timeStamp = extractTimestamp(fileName);
        //files.push_back(std::make_pair(fileName, timeStamp));
        if( nextFile.length() > 20 ){
          files.push_back( nextFile );
        }
        nextFile = dir.getNextFileName();
    }

    // // Sort files based on timestamp
    // std::sort(files.begin(), files.end(), [](const auto &a, const auto &b) {
    //     return a.second < b.second;
    // });

    // Delete the oldest 'count' files
    for (int i = 0; i < count && i < files.size(); ++i) {
        //AddLog(LOG_LEVEL_INFO, PSTR("Removing %s") ,files[i]);
#ifdef USE_UFILESYS
         if( TfsDeleteFile( files[i].c_str()) ) {
          //if (LittleFS.remove(files[i])) {
          Serial.println("Deleted file: " + files[i]);
         } else {

          Serial.println("Failed to delete file: " + files[i]);
         }
#else
        Serial.println("Failed to delete file: " + files[i]);
#endif 

    }
}

void RDDLNetworkNotarizationScheduler(){
  ++counted_seconds;
  if( counted_seconds >= (uint32_t)atoi(SettingsText( SET_NOTARIZTATION_PERIODICITY)))
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
      break;
    case FUNC_PRE_INIT:
      RDDLNetworkSettingsLoad(0);
      break;
    case FUNC_SAVE_BEFORE_RESTART:
      // !!! DO NOT USE AS IT'S FUNCTION IS BETTER HANDLED BY FUNC_SAVE_SETTINGS !!!
      break;
  }
  result = true;
  return result;
}

#endif  // USE_DRV_FILE_DEMO
