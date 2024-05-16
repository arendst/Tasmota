/*
  xdrv_73_4_lorawan_cryptography.ino - LoRaWan cryptography support for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_SPI_LORA
#ifdef USE_LORAWAN
/*********************************************************************************************\
 * LoRaWan cryptography
\*********************************************************************************************/

//#define USE_LORAWAN_TEST

#include <utils/Cryptography.h>

uint32_t LoraWanGenerateMIC(uint8_t* msg, size_t len, uint8_t* key) {
  if((msg == NULL) || (len == 0)) {
    return(0);
  }
  RadioLibAES128Instance.init(key);
  uint8_t cmac[TAS_LORAWAN_AES128_KEY_SIZE];
  RadioLibAES128Instance.generateCMAC(msg, len, cmac);
  return(((uint32_t)cmac[0]) | ((uint32_t)cmac[1] << 8) | ((uint32_t)cmac[2] << 16) | ((uint32_t)cmac[3]) << 24);
}

/*********************************************************************************************/

// EncryptJoinAccept uses AES Decrypt to encrypt a join-accept message
// - The payload contains JoinNonce/AppNonce | NetID | DevAddr | DLSettings | RxDelay | (CFList | CFListType) | MIC
// - In LoRaWAN 1.0, the AppKey is used
// - In LoRaWAN 1.1, the NwkKey is used in reply to a JoinRequest
// - In LoRaWAN 1.1, the JSEncKey is used in reply to a RejoinRequest (type 0,1,2)
void LoraWanEncryptJoinAccept(uint8_t* key, uint8_t* payload, size_t len, uint8_t* encrypted) {
  RadioLibAES128Instance.init(key);
  RadioLibAES128Instance.decryptECB(payload, len, encrypted);
}

/*********************************************************************************************/

// deriveLegacySKey derives a session key
void _LoraWanDeriveLegacySKey(uint8_t* key, uint8_t t, uint32_t jn, uint32_t nid, uint16_t dn, uint8_t* derived) {
  uint8_t buf[TAS_LORAWAN_AES128_KEY_SIZE] = { 0 };
  buf[0] = t;
  buf[1] = jn;
  buf[2] = jn >> 8;
  buf[3] = jn >> 16;
  buf[4] = nid;
  buf[5] = nid >> 8;
  buf[6] = nid >> 16;
  buf[7] = dn;
  buf[8] = dn >> 8;

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: buf %16_H"), buf);

  RadioLibAES128Instance.init(key);
  RadioLibAES128Instance.encryptECB(buf, TAS_LORAWAN_AES128_KEY_SIZE, derived);
}

// DeriveLegacyAppSKey derives the LoRaWAN Application Session Key
// - If a LoRaWAN 1.0 device joins a LoRaWAN 1.0/1.1 network, the AppKey is used as "key"
// - If a LoRaWAN 1.1 device joins a LoRaWAN 1.0 network, the NwkKey is used as "key"
void _LoraWanDeriveLegacyAppSKey(uint8_t* key, uint32_t jn, uint32_t nid, uint16_t dn, uint8_t* AppSKey) {
  _LoraWanDeriveLegacySKey(key, 0x02, jn, nid, dn, AppSKey);

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: key %16_H, AppSKey %16_H"), key, AppSKey);
}

void LoraWanDeriveLegacyAppSKey(uint32_t node, uint8_t* AppSKey) {
  _LoraWanDeriveLegacyAppSKey(Lora->settings.end_node[node].AppKey, TAS_LORAWAN_JOINNONCE +node, TAS_LORAWAN_NETID, Lora->settings.end_node[node].DevNonce, AppSKey);
}

// DeriveLegacyNwkSKey derives the LoRaWAN 1.0 Network Session Key. AppNonce is entered as JoinNonce.
// - If a LoRaWAN 1.0 device joins a LoRaWAN 1.0/1.1 network, the AppKey is used as "key"
// - If a LoRaWAN 1.1 device joins a LoRaWAN 1.0 network, the NwkKey is used as "key"
void _LoraWanDeriveLegacyNwkSKey(uint8_t* appKey, uint32_t jn, uint32_t nid, uint16_t dn, uint8_t* NwkSKey) {
  _LoraWanDeriveLegacySKey(appKey, 0x01, jn, nid, dn, NwkSKey);

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: appKey %16_H, NwkSKey %16_H"), appKey, NwkSKey);
}

void LoraWanDeriveLegacyNwkSKey(uint32_t node, uint8_t* NwkSKey) {
  _LoraWanDeriveLegacyNwkSKey(Lora->settings.end_node[node].AppKey, TAS_LORAWAN_JOINNONCE +node, TAS_LORAWAN_NETID, Lora->settings.end_node[node].DevNonce, NwkSKey);
}

#ifdef USE_LORAWAN_TEST
/*-------------------------------------------------------------------------------------------*/
void LoraWanTestKeyDerivation(void) {
  uint8_t key[] =  {0xBE, 0xC4, 0x99, 0xC6, 0x9E, 0x9C, 0x93, 0x9E, 0x41, 0x3B, 0x66, 0x39, 0x61, 0x63, 0x6C, 0x61};
  uint16_t dn = 0x7369;       //	dn := types.DevNonce{0x73, 0x69}
  uint32_t nid = 0x00020101;  //	nid := types.NetID{0x02, 0x01, 0x01}
  uint32_t jn = 0x00AE3B1C;   //	jn := types.JoinNonce{0xAE, 0x3B, 0x1C}

  uint8_t appSKey[TAS_LORAWAN_AES128_KEY_SIZE];
  _LoraWanDeriveLegacyAppSKey(key, jn, nid, dn, appSKey);
  AddLog(LOG_LEVEL_DEBUG, PSTR("TST: LoraWanDeriveLegacyAppSKey %16_H"), appSKey);
//  a.So(appSKey, should.Equal, types.AES128Key{0x8C, 0x1E, 0x05, 0x43, 0xA2, 0x29, 0x08, 0x8D, 0xE6, 0xF8, 0x4E, 0x74, 0xBB, 0x46, 0xBD, 0x62})

  uint8_t nwkSKey[TAS_LORAWAN_AES128_KEY_SIZE];
  _LoraWanDeriveLegacyNwkSKey(key, jn, nid, dn, nwkSKey);
  AddLog(LOG_LEVEL_DEBUG, PSTR("TST: LoraWanDeriveLegacyNwkSKey %16_H"), nwkSKey);
//  a.So(nwkSKey, should.Equal, types.AES128Key{0x0D, 0xB9, 0x24, 0xEE, 0x6A, 0xF9, 0x06, 0x98, 0xE0, 0x5F, 0xC7, 0xCE, 0x48, 0x30, 0x3C, 0x01})
}
#endif  // USE_LORAWAN_TEST

/*********************************************************************************************/

//func EncryptMessage(key types.AES128Key, dir uint8, addr types.DevAddr, fCnt uint32, payload []byte, opts ...EncryptionOption) ([]byte, error) {
void LoraWanProcessAES(uint8_t* in, size_t len, uint8_t* key, uint8_t* out, uint32_t addr, uint32_t fcnt, uint8_t dir, uint8_t ctrId, bool counter) {
  // figure out how many encryption blocks are there
  size_t numBlocks = len / TAS_LORAWAN_AES128_KEY_SIZE;
  if (len % TAS_LORAWAN_AES128_KEY_SIZE) {
    numBlocks++;
  }

  // generate the encryption blocks
  uint8_t encBuffer[TAS_LORAWAN_AES128_KEY_SIZE] = { 0 };
  uint8_t encBlock[TAS_LORAWAN_AES128_KEY_SIZE] = { 0 };
  encBlock[0] = 0x01;
  encBlock[4] = ctrId;
  encBlock[5] = dir;
  encBlock[6] = addr;
  encBlock[7] = addr >> 8;
  encBlock[8] = addr >> 16;
  encBlock[9] = addr >> 24;
  encBlock[10] = fcnt;
  encBlock[11] = fcnt >> 8;
  encBlock[12] = fcnt >> 16;
  encBlock[13] = fcnt >> 24;

  // now encrypt the input
  // on downlink frames, this has a decryption effect because server actually "decrypts" the plaintext
  size_t remLen = len;
  for (size_t i = 0; i < numBlocks; i++) {

    if (counter) {
      encBlock[15] = i + 1;
    }

    // encrypt the buffer
    RadioLibAES128Instance.init(key);
    RadioLibAES128Instance.encryptECB(encBlock, TAS_LORAWAN_AES128_KEY_SIZE, encBuffer);

    // now xor the buffer with the input
    size_t xorLen = remLen;
    if (xorLen > TAS_LORAWAN_AES128_KEY_SIZE) {
      xorLen = TAS_LORAWAN_AES128_KEY_SIZE;
    }
    for (uint8_t j = 0; j < xorLen; j++) {
      out[i * TAS_LORAWAN_AES128_KEY_SIZE + j] = in[i * TAS_LORAWAN_AES128_KEY_SIZE + j] ^ encBuffer[j];
    }
    remLen -= xorLen;
  }
}

// DecryptUplink decrypts an uplink payload
// - The payload contains the FRMPayload bytes
// - For FPort>0, the AppSKey is used
// - For FPort=0, the NwkSEncKey/NwkSKey is used
void LoraWanDecryptUplink(uint8_t* key, uint32_t addr, uint32_t fCnt, uint8_t* payload, size_t payload_len, uint32_t opts, uint8_t* res) {
  LoraWanProcessAES(payload, payload_len, key, res, addr, fCnt, 0, opts, true);
}

// EncryptDownlink encrypts a downlink payload
// - The payload contains the FRMPayload bytes
// - For FPort>0, the AppSKey is used
// - For FPort=0, the NwkSEncKey/NwkSKey is used
//func EncryptDownlink(key types.AES128Key, addr types.DevAddr, fCnt uint32, payload []byte, opts ...EncryptionOption) ([]byte, error) {
//	return encryptMessage(key, 1, addr, fCnt, payload, opts...)
void LoraWanEncryptDownlink(uint8_t* key, uint32_t addr, uint32_t fCnt, uint8_t* payload, size_t payload_len, uint32_t opts, uint8_t* res) {
  LoraWanProcessAES(payload, payload_len, key, res, addr, fCnt, 1, opts, true);
}

#ifdef USE_LORAWAN_TEST
/*-------------------------------------------------------------------------------------------*/
void LorWanTestUplinkDownlinkEncryption(void) {
	uint8_t key[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	uint32_t addr = 0x01020304;            // addr := types.DevAddr{1, 2, 3, 4}
	uint32_t frameIdentifier = 0x0000001;  // frameIdentifier := [4]byte{0x00, 0x00, 0x00, 0x01}

	uint8_t res[TAS_LORAWAN_AES128_KEY_SIZE] = { 0 };

	// FRM Payload
  uint8_t payloadd[] = {0xCF, 0xF3, 0x0B, 0x4E};
	LoraWanDecryptUplink(key, addr, 1, payloadd, 4, 0, res);
  AddLog(LOG_LEVEL_DEBUG, PSTR("TST: LoraWanDecryptUplink %4_H"), res);
//	a.So(res, should.Resemble, []byte{1, 2, 3, 4})

  uint8_t payloade[] = {1, 2, 3, 4};
  LoraWanEncryptDownlink(key, addr, 1, payloade, 4, 0, res);
  AddLog(LOG_LEVEL_DEBUG, PSTR("TST: LoraWanEncryptDownlink %4_H"), res);
//  a.So(res, should.Resemble, []byte{0x4E, 0x75, 0xF4, 0x40})
}
#endif  // USE_LORAWAN_TEST

/*********************************************************************************************/

uint32_t LoraWanComputeMIC(uint8_t* key, uint8_t dir, uint16_t confFCnt, uint32_t addr, uint32_t fCnt, uint8_t* payload, size_t payload_len) {
	uint8_t b0[TAS_LORAWAN_AES128_KEY_SIZE + payload_len];  // error: variable-sized object 'b0' may not be initialized
  memset(b0, 0, sizeof(b0));
	b0[0] = 0x49;
  b0[1] = confFCnt;
  b0[2] = confFCnt >> 8;
	b0[5] = dir;
  b0[6] = addr;
  b0[7] = addr >> 8;
  b0[8] = addr >> 16;
  b0[9] = addr >> 24;
  b0[10] = fCnt;
  b0[11] = fCnt >> 8;
	b0[15] = payload_len;
  memcpy(&b0[16], payload, payload_len);
  return LoraWanGenerateMIC(b0, sizeof(b0), key);
}

// ComputeLegacyUplinkMIC computes the Uplink Message Integrity Code.
// - The payload contains MHDR | FHDR | FPort | FRMPayload
// - The NwkSKey is used
uint32_t LoraWanComputeLegacyUplinkMIC(uint8_t* key, uint32_t addr, uint32_t fCnt, uint8_t* payload, uint8_t payload_len) {
	return LoraWanComputeMIC(key, 0, 0, addr, fCnt, payload, payload_len);
}

// ComputeLegacyDownlinkMIC computes the Downlink Message Integrity Code.
// - The payload contains MHDR | FHDR | FPort | FRMPayload
// - The NwkSKey is used
uint32_t LoraWanComputeLegacyDownlinkMIC(uint8_t* key, uint32_t addr, uint32_t fCnt, uint8_t* payload, uint8_t payload_len) {
	return LoraWanComputeMIC(key, 1, 0, addr, fCnt, payload, payload_len);
}

#ifdef USE_LORAWAN_TEST
/*-------------------------------------------------------------------------------------------*/
void LorWanTestUplinkDownlinkMIC(void) {
  uint8_t key[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  uint32_t addr = 0x01020304;  // addr := types.DevAddr{1, 2, 3, 4}
  uint8_t payloadWithoutMIC[] ={
    0x40,                      // Unconfirmed Uplink
    0x04, 0x03, 0x02, 0x01,    // DevAddr 01020304
    0x00,                      // Empty FCtrl
    0x01, 0x00,                // FCnt 1
    0x01,                      // FPort 1
    0x01, 0x02, 0x03, 0x04     // Data
  };

  uint32_t upMIC = LoraWanComputeLegacyUplinkMIC(key, addr, 1, payloadWithoutMIC, sizeof(payloadWithoutMIC));
  AddLog(LOG_LEVEL_DEBUG, PSTR("TST: LoraWanComputeLegacyUplinkMIC %08X"), upMIC);
//  a.So(mic, should.Equal, [4]byte{0x3B, 0x07, 0x31, 0x82}) - as uint32_t 0x8231073B

  uint32_t dnMIC = LoraWanComputeLegacyDownlinkMIC(key, addr, 1, payloadWithoutMIC, sizeof(payloadWithoutMIC));
  AddLog(LOG_LEVEL_DEBUG, PSTR("TST: LoraWanComputeLegacyDownlinkMIC %08X"), dnMIC);
//  a.So(mic, should.Equal, [4]byte{0xA5, 0x60, 0x9F, 0xA9}) - as uint32_t 0xA99F60A5
}
#endif  // USE_LORAWAN_TEST

#endif  // USE_LORAWAN
#endif  // USE_SPI_LORA
