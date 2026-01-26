# Matter 1.4.1 Core Specification - AI Compact Reference

Version: 1.4.1 (March 2025)
Purpose: Token-optimized implementation reference

---

## Chapter 1: Introduction

### 1.1 Scope
Matter defines interoperable application layer for smart home devices over IPv6.

### 1.2 Key Acronyms
| Acronym | Definition |
|---------|------------|
| ACL | Access Control List |
| AEAD | Authenticated Encryption with Associated Data |
| BDX | Bulk Data Exchange |
| BTP | Bluetooth Transport Protocol |
| CASE | Certificate Authenticated Session Establishment |
| CAT | CASE Authenticated Tag |
| CD | Certification Declaration |
| DAC | Device Attestation Certificate |
| GID | Group Identifier |
| ICD | Intermittently Connected Device |
| IPK | Identity Protection Key |
| MCSP | Message Counter Synchronization Protocol |
| MRP | Message Reliability Protocol |
| NOC | Node Operational Certificate |
| NOCSR | Node Operational Certificate Signing Request |
| PAA | Product Attestation Authority |
| PAI | Product Attestation Intermediate |
| PASE | Passcode-Authenticated Session Establishment |
| PID | Product Identifier |
| SED | Sleepy End Device |
| TLV | Tag Length Value |
| VID | Vendor Identifier |
| SIT | Short Idle Time (ICD) |
| LIT | Long Idle Time (ICD) |

### 1.3 Conformance Levels
- **SHALL**: Mandatory requirement
- **SHOULD**: Strongly recommended
- **MAY**: Optional flexibility

---

## Chapter 2: Architecture

### 2.1 Layered Architecture (top to bottom)
1. **Application**: Business logic (e.g., lighting control)
2. **Data Model**: Data structures and elements
3. **Interaction Model**: Read/write/subscribe/invoke operations
4. **Action Framing**: TLV serialization
5. **Security**: Encryption + MIC
6. **Message Layer**: Headers + routing
7. **Transport**: TCP/MRP(UDP)/BTP

### 2.2 Network Topology
- Supported link layers: Ethernet, Wi-Fi, Thread
- Single network: All nodes on same IPv6 link
- Star topology: Hub network + peripheral networks via Border Routers
- Thread is primary supported LLN

### 2.3 Identifiers

#### 2.3.1 Fabric ID (64-bit)
- Scoped to Root CA public key
- Fabric ID 0 is reserved (SHALL NOT be used)
- Compressed form used for operational discovery

#### 2.3.2 Vendor ID (16-bit)
| Range | Type |
|-------|------|
| 0x0000 | Matter Standard |
| 0x0001-0xFFF0 | Manufacturer Codes |
| 0xFFF1-0xFFF4 | Test Vendors |

#### 2.3.3 Product ID (16-bit)
- Assigned by vendor, unique per VID
- 0x0000 reserved (anonymous/multi-product)

#### 2.3.4 Group ID (16-bit)
| Range | Type |
|-------|------|
| 0x0000 | Null/unspecified |
| 0x0001-0xFEFF | Application Group ID |
| 0xFF00-0xFFFF | Universal Group ID |

Universal Groups:
- 0xFFFF: All Nodes
- 0xFFFE: All non-ICD Nodes
- 0xFFFD: All Proxies

#### 2.3.5 Node ID (64-bit)
| Range | Type |
|-------|------|
| 0x0000_0000_0000_0000 | Unspecified |
| 0x0000_0000_0000_0001 - 0xFFFF_FFEF_FFFF_FFFF | Operational Node ID |
| 0xFFFF_FFFB_xxxx_xxxx | PAKE key identifiers |
| 0xFFFF_FFFD_xxxx_xxxx | CASE Authenticated Tag |
| 0xFFFF_FFFE_xxxx_xxxx | Temporary Local Node ID |
| 0xFFFF_FFFF_FFFF_xxxx | Group Node ID |

### 2.4 IPv6 Addressing

#### Multicast Address Format (RFC 3306)
```
FF35:0040:FD<FabricID>00:<GroupID>
```
- First 12 bits: 0xFF3 (RFC 3306)
- Scope: 0x5 (Site-Local)
- Network prefix: 0xFD + upper 56-bits of Fabric ID
- Group ID: lower 8-bits Fabric ID + 0x00 + 16-bit Group ID

#### Multicast Port: 5540 (IANA assigned)

### 2.5 Stack Limits

#### System Model Limits
| Limit | Value |
|-------|-------|
| Max endpoints per node | 65535 |
| Max clusters per endpoint | 65535 |
| Max attributes per cluster | 65535 |
| Max fabrics per node | 16 (min 5) |
| Max group keys per fabric | 3 |
| Max groups per fabric | 4 |
| Max ACL entries per fabric | 4 |
| Max subjects per ACL entry | 4 |
| Max targets per ACL entry | 3 |

#### Interaction Model Limits
| Limit | Value |
|-------|-------|
| Max attribute paths per request | 9 |
| Max event paths per request | 10 |
| Max data version filters | 8 |
| Max cluster paths per invoke | 1 (provisional: more) |
| Max subscriptions per fabric | 3 |

---

## Chapter 3: Cryptographic Primitives

### 3.1 Constants
```
CRYPTO_GROUP_SIZE_BYTES = 32
CRYPTO_PUBLIC_KEY_SIZE_BYTES = 65 (uncompressed)
CRYPTO_HASH_LEN_BITS = 256
CRYPTO_HASH_LEN_BYTES = 32
CRYPTO_SYMMETRIC_KEY_LENGTH_BITS = 128
CRYPTO_SYMMETRIC_KEY_LENGTH_BYTES = 16
CRYPTO_AEAD_MIC_LENGTH_BITS = 128
CRYPTO_AEAD_MIC_LENGTH_BYTES = 16
CRYPTO_AEAD_NONCE_LENGTH_BYTES = 13
CRYPTO_PBKDF_ITERATIONS_MIN = 1000
CRYPTO_PBKDF_ITERATIONS_MAX = 100000
CRYPTO_W_SIZE_BITS = 256
```

### 3.2 Primitives

| Function | Algorithm |
|----------|-----------|
| DRBG | NIST 800-90A compliant |
| Hash | SHA-256 (FIPS 180-4) |
| HMAC | HMAC-SHA256 (FIPS 198-1) |
| ECC Curve | NIST P-256 (secp256r1) |
| Signature | ECDSA (SEC 1) |
| Key Exchange | ECDH (SEC 1) |
| AEAD | AES-128-CCM (NIST 800-38C) |
| Privacy | AES-128-CTR (NIST 800-38A) |
| KDF | HKDF (RFC 5869) |
| PBKDF | PBKDF2-HMAC-SHA256 (NIST 800-132) |
| PAKE | SPAKE2+ |

### 3.3 SPAKE2+ Parameters
```
M = 02886e2f97ace46e55ba9dd7242579f2993b64e16ef3dcab95afd497333d8fa12f
N = 03d8bbd6c639c62937b04d997f38c3770719c629d7014d49a24b4f98baa1292b49
```

### 3.4 PBKDF Parameter Set (TLV)
```
Crypto_PBKDFParameterSet => STRUCTURE [tag-order] {
  iterations [1]: UNSIGNED INTEGER [32-bits],
  salt [2]: OCTET STRING [length 16..32]
}
```

### 3.5 Certificate Validation Time
- If trusted real-time available: use it
- Otherwise: use Last Known Good UTC Time
- Last Known Good UTC: updated at commissioning, may update after time sync or OTA
- Initial value: firmware compile time
- Store to persistent storage at least monthly

---

## Chapter 4: Secure Channel

### 4.1 Message Types
- **Control messages**: Internal protocols (MCSP), separate counter domain
- **Data messages**: Application data

### 4.2 Transports
- **UDP**: Individual datagrams, uses MRP for reliability
- **TCP**: Length-prefixed messages, segmentation/reassembly
- **BTP**: BLE transport, segmentation/reassembly
- **Wi-Fi Public Action Frame**: Commissioning only

### 4.3 Discovery

#### 4.3.1 Commissionable Node Discovery
- DNS-SD service type: `_matterc._udp`
- Instance name: 64-bit random hex (e.g., `DD200C20D25AE5F7`)
- New instance on boot and entering commissioning mode

**Subtypes:**
- `_L<dddd>`: Full 12-bit discriminator
- `_S<dd>`: Upper 4 bits of discriminator
- `_V<ddddd>`: Vendor ID
- `_T<ddd>`: Device type
- `_CM`: Currently in Commissioning Mode

**TXT Records:**
| Key | Description |
|-----|-------------|
| D | Full 12-bit discriminator (required) |
| VP | Vendor ID [+Product ID] |
| CM | Commissioning mode: 0=not, 1=passcode, 2=dynamic |
| DT | Primary device type |
| DN | Device name (max 32 bytes UTF-8) |
| RI | Rotating device identifier (hex) |
| PH | Pairing hint bitmap |
| PI | Pairing instructions |
| JF | Joint Fabric capabilities |

#### 4.3.2 Operational Discovery
- DNS-SD service type: `_matter._tcp`
- Instance name: `<CompressedFabricID>-<NodeID>` (16 hex chars each)
- Compressed Fabric ID = first 8 bytes of: `Crypto_Hash(MatterFabricId || RootPublicKey)`

**TXT Records:**
| Key | Description |
|-----|-------------|
| SII | Session Idle Interval (ms) |
| SAI | Session Active Interval (ms) |
| SAT | Session Active Threshold (ms) |
| T | TCP support bitmap |
| ICD | Long Idle Time ICD mode |

### 4.4 Message Frame Format

#### Message Header
| Field | Size | Description |
|-------|------|-------------|
| Message Flags | 1 byte | Version(4b), S(1b), DSIZ(2b) |
| Session ID | 2 bytes | Session identifier |
| Security Flags | 1 byte | P(1b), C(1b), MX(1b), SessionType(2b) |
| Message Counter | 4 bytes | Unique per message |
| Source Node ID | 0/8 bytes | If S flag set |
| Destination Node ID | 0/2/8 bytes | Per DSIZ field |
| Message Extensions | variable | If MX flag set |

**Message Flags:**
- Version: 0 = Matter 1.0
- S: Source Node ID present
- DSIZ: 0=none, 1=64-bit NodeID, 2=16-bit GroupID

**Security Flags:**
- P: Privacy enabled
- C: Control message
- MX: Message extensions present
- Session Type: 0=Unicast, 1=Group

#### Protocol Header
| Field | Size | Description |
|-------|------|-------------|
| Exchange Flags | 1 byte | I(1b), A(1b), R(1b), SX(1b), V(1b) |
| Protocol Opcode | 1 byte | Message type |
| Exchange ID | 2 bytes | Exchange identifier |
| Protocol Vendor ID | 0/2 bytes | If V flag set |
| Protocol ID | 2 bytes | Protocol identifier |
| Ack Message Counter | 0/4 bytes | If A flag set |
| Secured Extensions | variable | If SX flag set |
| Application Payload | variable | Message data |

**Exchange Flags:**
- I: Initiator
- A: Acknowledgement
- R: Reliability requested
- SX: Secured extensions present
- V: Vendor ID present

#### Message Footer
| Field | Size |
|-------|------|
| MIC | 16 bytes (CRYPTO_AEAD_MIC_LENGTH_BYTES) |

### 4.5 Protocol IDs
| ID | Protocol |
|----|----------|
| 0x0000 | Secure Channel |
| 0x0001 | Interaction Model |
| 0x0002 | BDX |
| 0x0003 | User Directed Commissioning |

### 4.6 Message Counters

#### Counter Types
| Type | Scope | Rollover | Persistent |
|------|-------|----------|------------|
| Global Unencrypted | Unlimited | Allowed | Optional |
| Global Group Data | Group Key | Allowed | Mandatory |
| Global Group Control | Group Key | Allowed | Mandatory |
| Secure Session | Session Key | Expires | Optional |
| Check-In | Unlimited | Allowed | Mandatory |

#### Initialization
All counters: `Crypto_DRBG(len=28) + 1` (range 1 to 2^28)

#### Message Reception State
- max_message_counter: Highest valid counter received
- message_counter_window: Bitmap of recent counters (min 32 bits)
- Window tracks counters from (max - window_size + 1) to max

### 4.7 Message Security

#### Nonce Construction (13 bytes)
```
Nonce = SecurityFlags || MessageCounter || SourceNodeID
```
- SecurityFlags: 1 byte
- MessageCounter: 4 bytes (little-endian)
- SourceNodeID: 8 bytes (little-endian)

#### Encryption
```
AAD = MessageHeader (excluding MIC)
Ciphertext || MIC = Crypto_AEAD_GenerateEncrypt(Key, Plaintext, AAD, Nonce)
```

### 4.8 Privacy Processing

#### Privacy Key Derivation
```
PrivacyKey = Crypto_KDF(
  inputKey = EncryptionKey,
  salt = [],
  info = "PrivacyKey",
  len = 128
)
```

#### Privacy Nonce (13 bytes)
```
PrivacyNonce = SessionID || MIC[0..10]
```

#### Privacy Encryption
Encrypts: Message Counter, Source Node ID (if present)
Does NOT encrypt: Message Flags, Session ID, Security Flags



### 4.9 MRP (Message Reliability Protocol)

#### Parameters
| Parameter | Default | Description |
|-----------|---------|-------------|
| MRP_MAX_TRANSMISSIONS | 5 | Max transmission attempts |
| MRP_BACKOFF_BASE | 1.6 | Exponential backoff base |
| MRP_BACKOFF_JITTER | 0.25 | Random jitter scaler |
| MRP_BACKOFF_MARGIN | 1.1 | Margin over peer idle interval |
| MRP_BACKOFF_THRESHOLD | 1 | Retransmissions before exponential backoff |
| MRP_STANDALONE_ACK_TIMEOUT | 200ms | Time before standalone ack |

#### Retransmission Timing Algorithm
```
if (retransCount < MRP_BACKOFF_THRESHOLD):
  backoff = baseInterval
else:
  backoff = baseInterval * (MRP_BACKOFF_BASE ^ (retransCount - MRP_BACKOFF_THRESHOLD))

mrpBackoffTime = backoff * (1 + random(0, MRP_BACKOFF_JITTER))
```

#### MRP Standalone Acknowledgement
- Empty application payload
- A Flag = 1
- Protocol ID = PROTOCOL_ID_SECURE_CHANNEL
- Protocol Opcode = MRP Standalone Acknowledgement (0x10)

### 4.10 Session Parameters
| Parameter | Default | Description |
|-----------|---------|-------------|
| SESSION_IDLE_INTERVAL | 500ms | Min retry interval when idle |
| SESSION_ACTIVE_INTERVAL | 300ms | Min retry interval when active |
| SESSION_ACTIVE_THRESHOLD | 4000ms | Time to stay active after activity |

#### Session Parameter TLV
```
session-parameter-struct => STRUCTURE [tag-order] {
  SESSION_IDLE_INTERVAL [1, optional]: UNSIGNED INTEGER [32-bits],
  SESSION_ACTIVE_INTERVAL [2, optional]: UNSIGNED INTEGER [32-bits],
  SESSION_ACTIVE_THRESHOLD [3, optional]: UNSIGNED INTEGER [16-bits],
  DATA_MODEL_REVISION [4]: UNSIGNED INTEGER [16-bits],
  INTERACTION_MODEL_REVISION [5]: UNSIGNED INTEGER [16-bits],
  SPECIFICATION_VERSION [6]: UNSIGNED INTEGER [32-bits],
  MAX_PATHS_PER_INVOKE [7]: UNSIGNED INTEGER [16-bits],
  SUPPORTED_TRANSPORTS [8]: UNSIGNED INTEGER [16-bits],
  MAX_TCP_MESSAGE_SIZE [9, optional]: UNSIGNED INTEGER [32-bits]
}
```

### 4.11 PASE (Passcode-Authenticated Session Establishment)

#### Message Flow
1. **PBKDFParamRequest** (Initiator → Responder)
2. **PBKDFParamResponse** (Responder → Initiator)
3. **Pake1** (Initiator → Responder)
4. **Pake2** (Responder → Initiator)
5. **Pake3** (Initiator → Responder)
6. **PakeFinished** (Responder → Initiator via StatusReport)

#### PBKDFParamRequest TLV
```
pbkdfparamreq-struct => STRUCTURE [tag-order] {
  initiatorRandom [1]: OCTET STRING [length 32],
  initiatorSessionId [2]: UNSIGNED INTEGER [16-bits],
  passcodeId [3]: UNSIGNED INTEGER [16-bits],
  hasPBKDFParameters [4]: BOOLEAN,
  initiatorSessionParams [5, optional]: session-parameter-struct
}
```

#### PBKDFParamResponse TLV
```
pbkdfparamresp-struct => STRUCTURE [tag-order] {
  initiatorRandom [1]: OCTET STRING [length 32],
  responderRandom [2]: OCTET STRING [length 32],
  responderSessionId [3]: UNSIGNED INTEGER [16-bits],
  pbkdf_parameters [4]: Crypto_PBKDFParameterSet,
  responderSessionParams [5, optional]: session-parameter-struct
}
```

#### Pake Messages TLV
```
pake-1-struct => STRUCTURE [tag-order] {
  pA [1]: OCTET STRING [length CRYPTO_PUBLIC_KEY_SIZE_BYTES]
}

pake-2-struct => STRUCTURE [tag-order] {
  pB [1]: OCTET STRING [length CRYPTO_PUBLIC_KEY_SIZE_BYTES],
  cB [2]: OCTET STRING [length CRYPTO_HASH_LEN_BYTES]
}

pake-3-struct => STRUCTURE [tag-order] {
  cA [1]: OCTET STRING [length CRYPTO_HASH_LEN_BYTES]
}
```

#### PAKE Computation
```
// Initiator computes w0, w1
w0s || w1s = Crypto_PBKDF(passcode, salt, iterations, 2 * CRYPTO_W_SIZE_BITS)
w0 = w0s mod p
w1 = w1s mod p

// Responder stores (w0, L) where L = w1 * P

// Transcript
Context = Crypto_Hash("CHIP PAKE V1 Commissioning" || PBKDFParamRequest || PBKDFParamResponse)
TT = len(Context) || Context || 0x00..00 || 0x00..00 || len(M) || M || len(N) || N ||
     len(pA) || pA || len(pB) || pB || len(Z) || Z || len(V) || V || len(w0) || w0

// Key derivation
(cA, cB, Ke) = Crypto_P2(TT, pA, pB)
I2RKey || R2IKey || AttestationChallenge = Crypto_KDF(Ke, [], "SessionKeys", 3*128)
```

### 4.12 CASE (Certificate Authenticated Session Establishment)

#### Protocol Overview
CASE provides:
1. Mutual authentication of both peer Nodes
2. Cryptographic key generation for session security
3. Exchange of operational parameters (Session ID, MRP params)

Uses SIGMA protocol with Identity Protection Key (IPK) for identity protection.

#### Message Flow
1. **Sigma1** (Initiator → Responder)
2. **Sigma2** (Responder → Initiator)
3. **Sigma3** (Initiator → Responder)
4. **Sigma3Finished** (Responder → Initiator via StatusReport)

#### Session Resumption Flow
1. **Sigma1 with Resumption** (Initiator → Responder)
2. **Sigma2_Resume** (Responder → Initiator)
3. **SigmaFinished** (Responder → Initiator via StatusReport)

Resumption requires both peers to remember SharedSecret from previous session.

#### Sigma1 TLV
```
sigma-1-struct => STRUCTURE [tag-order] {
  initiatorRandom [1]: OCTET STRING [length 32],
  initiatorSessionId [2]: UNSIGNED INTEGER [16-bits],
  destinationId [3]: OCTET STRING [length 32],
  initiatorEphPubKey [4]: OCTET STRING [length CRYPTO_PUBLIC_KEY_SIZE_BYTES],
  initiatorSessionParams [5, optional]: session-parameter-struct,
  resumptionID [6, optional]: OCTET STRING [length 16],
  initiatorResumeMIC [7, optional]: OCTET STRING [length 16]
}
```

#### Sigma2 TLV
```
sigma-2-struct => STRUCTURE [tag-order] {
  responderRandom [1]: OCTET STRING [length 32],
  responderSessionId [2]: UNSIGNED INTEGER [16-bits],
  responderEphPubKey [3]: OCTET STRING [length CRYPTO_PUBLIC_KEY_SIZE_BYTES],
  encrypted2 [4]: OCTET STRING,
  responderSessionParams [5, optional]: session-parameter-struct
}

sigma-2-tbsdata => STRUCTURE [tag-order] {
  responderNOC [1]: OCTET STRING,
  responderICAC [2, optional]: OCTET STRING,
  responderEphPubKey [3]: ec-pub-key,
  initiatorEphPubKey [4]: ec-pub-key
}

sigma-2-tbedata => STRUCTURE [tag-order] {
  responderNOC [1]: OCTET STRING,
  responderICAC [2, optional]: OCTET STRING,
  signature [3]: ec-signature,
  resumptionID [4]: OCTET STRING [length 16]
}
```

#### Sigma3 TLV
```
sigma-3-struct => STRUCTURE [tag-order] {
  encrypted3 [1]: OCTET STRING
}

sigma-3-tbsdata => STRUCTURE [tag-order] {
  initiatorNOC [1]: OCTET STRING,
  initiatorICAC [2, optional]: OCTET STRING,
  initiatorEphPubKey [3]: ec-pub-key,
  responderEphPubKey [4]: ec-pub-key
}

sigma-3-tbedata => STRUCTURE [tag-order] {
  initiatorNOC [1]: OCTET STRING,
  initiatorICAC [2, optional]: OCTET STRING,
  signature [3]: ec-signature
}
```

#### Sigma2_Resume TLV
```
sigma-2-resume-struct => STRUCTURE [tag-order] {
  resumptionID [1]: OCTET STRING [length 16],
  sigma2ResumeMIC [2]: OCTET STRING [length 16],
  responderSessionID [3]: UNSIGNED INTEGER [16-bits],
  responderSessionParams [4, optional]: session-parameter-struct
}
```

#### Destination Identifier Computation
```
destinationMessage = initiatorRandom || rootPublicKey || fabricId || nodeId
destinationIdentifier = Crypto_HMAC(key=IPK, message=destinationMessage)
```
- rootPublicKey: Uncompressed EC point (65 bytes)
- fabricId: 64-bit little-endian
- nodeId: 64-bit little-endian

#### CASE Key Derivation

**Identity Protection Key (IPK)**
- Operational group key under GroupKeySetID 0 for the fabric
- For Destination Identifier: use second newest EpochStartTime key if exists

**Sigma2 Key (S2K)**
```
TranscriptHash = Crypto_Hash(message = Msg1)
S2K_Info = "Sigma2"  // {0x53, 0x69, 0x67, 0x6d, 0x61, 0x32}
S2K = Crypto_KDF(
  inputKey = SharedSecret,
  salt = IPK || ResponderRandom || ResponderEphPubKey || TranscriptHash,
  info = S2K_Info,
  len = CRYPTO_SYMMETRIC_KEY_LENGTH_BITS
)
```

**Sigma3 Key (S3K)**
```
TranscriptHash = Crypto_Hash(message = Msg1 || Msg2)
S3K_Info = "Sigma3"  // {0x53, 0x69, 0x67, 0x6d, 0x61, 0x33}
S3K = Crypto_KDF(
  inputKey = SharedSecret,
  salt = IPK || TranscriptHash,
  info = S3K_Info,
  len = CRYPTO_SYMMETRIC_KEY_LENGTH_BITS
)
```

**Sigma1 Resumption Key (S1RK)**
```
S1RK_Info = "Sigma1_Resume"  // {0x53,0x69,0x67,0x6d,0x61,0x31,0x5f,0x52,0x65,0x73,0x75,0x6d,0x65}
S1RK = Crypto_KDF(
  inputKey = SharedSecret,
  salt = Sigma1.initiatorRandom || ResumptionID,
  info = S1RK_Info,
  len = CRYPTO_SYMMETRIC_KEY_LENGTH_BITS
)
```

**Sigma2 Resumption Key (S2RK)**
```
S2RK_Info = "Sigma2_Resume"  // {0x53,0x69,0x67,0x6d,0x61,0x32,0x5f,0x52,0x65,0x73,0x75,0x6d,0x65}
S2RK = Crypto_KDF(
  inputKey = SharedSecret,
  salt = Sigma1.initiatorRandom || ResumptionID,
  info = S2RK_Info,
  len = CRYPTO_SYMMETRIC_KEY_LENGTH_BITS
)
```

**Session Encryption Keys**
```
TranscriptHash = Crypto_Hash(message = Msg1 || Msg2 || Msg3)
SEKeys_Info = "SessionKeys"  // {0x53,0x65,0x73,0x73,0x69,0x6f,0x6e,0x4b,0x65,0x79,0x73}
I2RKey || R2IKey || AttestationChallenge = Crypto_KDF(
  inputKey = SharedSecret,
  salt = IPK || TranscriptHash,
  info = SEKeys_Info,
  len = 3 * CRYPTO_SYMMETRIC_KEY_LENGTH_BITS
)
```

**Resumption Session Encryption Keys**
```
RSEKeys_Info = "SessionResumptionKeys"
I2RKey || R2IKey || AttestationChallenge = Crypto_KDF(
  inputKey = SharedSecret,
  salt = Sigma1.initiatorRandom || ResumptionID,
  info = RSEKeys_Info,
  len = 3 * CRYPTO_SYMMETRIC_KEY_LENGTH_BITS
)
```

#### CASE Nonces
```
TBEData2_Nonce = "NCASE_Sigma2N"  // {0x4e,0x43,0x41,0x53,0x45,0x5f,0x53,0x69,0x67,0x6d,0x61,0x32,0x4e}
TBEData3_Nonce = "NCASE_Sigma3N"  // {0x4e,0x43,0x41,0x53,0x45,0x5f,0x53,0x69,0x67,0x6d,0x61,0x33,0x4e}
Resume1MIC_Nonce = "NCASE_SigmaS1"  // {0x4e,0x43,0x41,0x53,0x45,0x5f,0x53,0x69,0x67,0x6d,0x61,0x53,0x31}
Resume2MIC_Nonce = "NCASE_SigmaS2"  // {0x4e,0x43,0x41,0x53,0x45,0x5f,0x53,0x69,0x67,0x6d,0x61,0x53,0x32}
```

#### CASE Validation Steps

**Sigma1 Validation (Responder)**
1. If resumptionID XOR initiatorResumeMIC present (not both): FAILURE, INVALID_PARAMETER
2. Set Peer Session Identifier to initiatorSessionId
3. If both resumption fields present: search for matching session, follow resumption path
4. Otherwise: validate destinationId against all installed NOCs

**Sigma2 Validation (Initiator)**
1. Generate SharedSecret via ECDH
2. Generate S2K, decrypt encrypted2
3. Verify NOC chain back to TrustedRCAC
4. Verify Fabric ID and Node ID match intended destination
5. Verify TBSData2Signature using responderNOC public key

**Sigma3 Validation (Responder)**
1. Generate S3K, decrypt encrypted3
2. Verify NOC chain back to TrustedRCAC
3. Verify Fabric ID matches
4. Verify TBSData3Signature using initiatorNOC public key
5. Generate session keys, initialize counters

#### Minimum CASE Sessions
- At least 3 CASE session contexts per fabric
- Device types MAY require more

### 4.13 Secure Channel Protocol Opcodes
| Opcode | Message |
|--------|---------|
| 0x00 | MsgCounterSyncReq |
| 0x01 | MsgCounterSyncRsp |
| 0x10 | MRP Standalone Acknowledgement |
| 0x20 | PBKDFParamRequest |
| 0x21 | PBKDFParamResponse |
| 0x22 | Pake1 |
| 0x23 | Pake2 |
| 0x24 | Pake3 |
| 0x30 | Sigma1 |
| 0x31 | Sigma2 |
| 0x32 | Sigma3 |
| 0x33 | Sigma2Resume |
| 0x40 | StatusReport |
| 0x50 | ICD Check-In |

### 4.14 Group Communication

#### Group Session Context
- Group Session ID: Derived from Group Key
- Encryption Key: Derived from Epoch Key
- Privacy Key: Derived from Encryption Key

#### Group Key Derivation
```
// From Epoch Key
EncryptionKey = Crypto_KDF(EpochKey, CompressedFabricId, "GroupKey v1.0", 128)
PrivacyKey = Crypto_KDF(EncryptionKey, [], "PrivacyKey", 128)

// Group Session ID (lower 16 bits)
GroupSessionId = Crypto_KDF(EpochKey, CompressedFabricId, "GroupKeyHash", 16)[0:2]
```

### 4.15 Check-In Protocol (ICD)

#### Check-In Message Content
```
check-in-message => {
  Nonce [8 bytes]: Random,
  EncryptedPayload: AES-128-CCM encrypted {
    CheckInCounter [4 bytes],
    ApplicationData [0..N bytes]
  },
  MIC [8 bytes]
}
```

#### Check-In Key Derivation
```
CheckInKey = Crypto_KDF(
  inputKey = SymmetricKey,  // From ICD Management cluster
  salt = [],
  info = "CheckInKey",
  len = 128
)
```

### 4.16 TCP Connection Management

#### Configuration Parameters
- Connection Establishment Timeout
- TCP Keep Alive: TCP_KEEP_ALIVE_TIME, TCP_KEEP_ALIVE_INTERVAL, TCP_KEEP_ALIVE_PROBES
- TCP User Timeout

#### TCP Keep Alive Timeout
```
TCP_Keep_Alive_Timeout = TCP_KEEP_ALIVE_TIME + TCP_KEEP_ALIVE_INTERVAL * TCP_KEEP_ALIVE_PROBES
```

#### Large Message Handling
- Configure Maximum Message Size for TCP
- If message header indicates size > max: close connection, send MESSAGE_TOO_LARGE StatusReport

---

## Chapter 5: Commissioning

### 5.1 Onboarding Payload

#### QR Code Format
```
QR code string = "MT:" + Base38Encode(PackedBinaryData)
```

#### Packed Binary Data Structure
| Field | Bits | Description |
|-------|------|-------------|
| Version | 3 | SHALL be 0 |
| Vendor ID | 16 | |
| Product ID | 16 | |
| Custom Flow | 2 | 0=Standard, 1=User-intent, 2=Custom |
| Discovery Capabilities | 8 | Bitmap |
| Discriminator | 12 | Full discriminator |
| Passcode | 27 | Setup code |
| Padding | 4 | Pad to byte boundary |
| TLV Data | variable | Optional |

#### Discovery Capabilities Bitmap
| Bit | Description |
|-----|-------------|
| 0 | Reserved (0) |
| 1 | BLE supported |
| 2 | On IP network |
| 3 | Wi-Fi Public Action Frame |
| 4-7 | Reserved (0) |

#### Manual Pairing Code (11 or 21 digits)
```
// Without VID/PID (11 digits)
DIGIT[1] = (VID_PID_PRESENT << 2) | (DISCRIMINATOR >> 10)
DIGIT[2..6] = ((DISCRIMINATOR & 0x300) << 6) | (PASSCODE & 0x3FFF)
DIGIT[7..10] = (PASSCODE >> 14)
DIGIT[11] = CHECK_DIGIT (Verhoeff)

// With VID/PID (21 digits)
DIGIT[11..15] = VENDOR_ID
DIGIT[16..20] = PRODUCT_ID
DIGIT[21] = CHECK_DIGIT (Verhoeff)
```

#### Passcode Rules
- 27-bit unsigned integer (00000001 to 99999998)
- Invalid passcodes: 00000000, 11111111, ..., 99999999, 12345678, 87654321
- SHALL NOT be derived from public information
- SHALL use cryptographically secure RNG

### 5.2 Commissioning Flows

#### Standard Commissioning Flow
1. Device discovery (DNS-SD/BLE)
2. PASE session establishment
3. Device attestation
4. CSR request and NOC installation
5. Network configuration (if needed)
6. CASE session establishment
7. Commissioning complete

#### Commissioning Window
- **Basic Commissioning Method (BCM)**: Uses device's built-in passcode
- **Enhanced Commissioning Method (ECM)**: Uses dynamically generated passcode/verifier

### 5.3 User Directed Commissioning (UDC)

#### Protocol Messages
| Opcode | Message |
|--------|---------|
| 0x00 | IdentificationDeclaration |
| 0x01 | CommissionerDeclaration |

#### IdentificationDeclaration TLV
```
identification-declaration => STRUCTURE [tag-order] {
  vendorId [1]: UNSIGNED INTEGER [16-bits],
  productId [2]: UNSIGNED INTEGER [16-bits],
  deviceName [3, optional]: STRING [max 32],
  deviceType [4, optional]: UNSIGNED INTEGER [16-bits],
  pairingInstruction [5, optional]: STRING [max 128],
  pairingHint [6, optional]: UNSIGNED INTEGER [16-bits],
  rotatingDeviceId [7, optional]: OCTET STRING [max 50],
  targetAppList [8, optional]: ARRAY OF target-app-struct,
  noPasscode [9, optional]: BOOLEAN,
  cdPort [10, optional]: UNSIGNED INTEGER [16-bits],
  commissionerPasscode [11, optional]: BOOLEAN
}
```

---

## Chapter 6: Device Attestation

### 6.1 Certificate Hierarchy
```
PAA (Product Attestation Authority)
  └── PAI (Product Attestation Intermediate)
        └── DAC (Device Attestation Certificate)
```

### 6.2 Device Attestation Procedure

#### Attestation Request/Response
1. Commissioner sends AttestationRequest with AttestationNonce
2. Device returns AttestationResponse with:
   - AttestationElements (TLV)
   - AttestationSignature

#### Attestation Elements TLV
```
attestation-elements => STRUCTURE [tag-order] {
  certification_declaration [1]: OCTET STRING,
  attestation_nonce [2]: OCTET STRING [length 32],
  timestamp [3]: UNSIGNED INTEGER [32-bits],
  firmware_information [4, optional]: OCTET STRING
}
```

#### Attestation Signature
```
AttestationSignature = Crypto_Sign(
  DAC_PrivateKey,
  AttestationElements || AttestationChallenge
)
```

### 6.3 Certification Declaration (CD)

#### CD TLV Structure
```
certification-elements => STRUCTURE [tag-order] {
  format_version [0]: UNSIGNED INTEGER [16-bits],  // SHALL be 1
  vendor_id [1]: UNSIGNED INTEGER [16-bits],
  product_id_array [2]: ARRAY [1..100] OF UNSIGNED INTEGER [16-bits],
  device_type_id [3]: UNSIGNED INTEGER [32-bits],
  certificate_id [4]: STRING [length 19],
  security_level [5]: UNSIGNED INTEGER [8-bits],  // Reserved, set to 0
  security_information [6]: UNSIGNED INTEGER [16-bits],  // Reserved, set to 0
  version_number [7]: UNSIGNED INTEGER [16-bits],
  certification_type [8]: UNSIGNED INTEGER [8-bits],
  dac_origin_vendor_id [9, optional]: UNSIGNED INTEGER [16-bits],
  dac_origin_product_id [10, optional]: UNSIGNED INTEGER [16-bits],
  authorized_paa_list [11, optional]: ARRAY [1..10] OF OCTET STRING [length 20]
}
```

#### Certification Types
| Value | Meaning |
|-------|---------|
| 0 | Development/test |
| 1 | Provisional |
| 2 | Official (certified) |

### 6.4 Node Operational Credentials

#### NOC Hierarchy
```
RCAC (Root CA Certificate)
  └── ICAC (Intermediate CA Certificate, optional)
        └── NOC (Node Operational Certificate)
```

#### NOCSR Procedure
1. Commissioner sends CSRRequest with CSRNonce
2. Node generates candidate key pair
3. Node creates PKCS#10 CSR
4. Node returns CSRResponse with NOCSRElements

#### NOCSRElements TLV
```
nocsr-elements => STRUCTURE [tag-order] {
  csr [1]: OCTET STRING,  // PKCS#10 CSR
  CSRNonce [2]: OCTET STRING [length 32],
  vendor_reserved1 [3, optional]: OCTET STRING,
  vendor_reserved2 [4, optional]: OCTET STRING,
  vendor_reserved3 [5, optional]: OCTET STRING
}
```



### 6.5 Access Control

#### Privilege Levels
| Level | Value | Description |
|-------|-------|-------------|
| View | 1 | Read attributes, receive events |
| ProxyView | 2 | View through proxy |
| Operate | 3 | View + invoke commands |
| Manage | 4 | Operate + write attributes |
| Administer | 5 | Full access including ACL |

#### Privilege Hierarchy
- Administer includes: Manage, Operate, ProxyView, View
- Manage includes: Operate, View
- Operate includes: View
- ProxyView includes: View

#### ACL Entry Structure
```
AccessControlEntryStruct => STRUCTURE {
  privilege [1]: AccessControlEntryPrivilegeEnum,
  authMode [2]: AccessControlEntryAuthModeEnum,
  subjects [3, nullable]: list[subject-id],
  targets [4, nullable]: list[AccessControlTargetStruct],
  fabricIndex [254]: fabric-idx
}
```

#### AccessControlTargetStruct
```
AccessControlTargetStruct => STRUCTURE {
  Cluster [0, nullable]: cluster-id,
  Endpoint [1, nullable]: endpoint-no,
  DeviceType [2, nullable]: devtype-id
}
```
- Cannot specify both Endpoint and DeviceType
- At least one of Cluster, Endpoint, or DeviceType must be non-null

#### Auth Modes
| Value | Mode |
|-------|------|
| 1 | PASE |
| 2 | CASE |
| 3 | Group |

#### Incoming Subject Descriptor (ISD)
```
struct SubjectDescriptor {
  bool IsCommissioning;
  AuthModeEnum AuthMode;
  list<SubjectID> Subjects;  // max 3 items (NodeID + up to 3 CATs)
  FabricIndex FabricIndex;
}
```

#### Access Control Algorithm (Conceptual)
```python
def subject_matches(acl_subject, isd_subject):
    # Exact match or CAT match with version check
    return (acl_subject == isd_subject) or
           (is_cat(acl_subject) and is_cat(isd_subject) and
            get_cat_id(acl_subject) == get_cat_id(isd_subject) and
            get_cat_version(isd_subject) >= get_cat_version(acl_subject))

def get_granted_privileges(acl, subject_desc, endpoint_id, cluster_id):
    granted_privileges = set()
    
    # PASE commissioning implicitly grants Administer
    if subject_desc.AuthMode == PASE and subject_desc.IsCommissioning:
        granted_privileges.add(Administer)
    
    for acl_entry in acl:
        if Administer in granted_privileges: break
        if acl_entry.FabricIndex != subject_desc.FabricIndex: continue
        if acl_entry.AuthMode != subject_desc.AuthMode: continue
        
        # Subject matching (empty = wildcard)
        if not is_empty(acl_entry.Subjects):
            if not any(subject_matches(acl_s, isd_s) 
                      for acl_s in acl_entry.Subjects 
                      for isd_s in subject_desc.Subjects):
                continue
        
        # Target matching (empty = wildcard)
        if not is_empty(acl_entry.Targets):
            matched = False
            for target in acl_entry.Targets:
                if target.Cluster and target.Cluster != cluster_id: continue
                if target.Endpoint and target.Endpoint != endpoint_id: continue
                if target.DeviceType and not endpoint_has_device_type(endpoint_id, target.DeviceType): continue
                matched = True
                break
            if not matched: continue
        
        add_granted_privilege(granted_privileges, acl_entry.privilege)
    
    return granted_privileges

def get_access_status(acl, arl, request_privilege, subject_desc, endpoint_id, cluster_id):
    granted = get_granted_privileges(acl, subject_desc, endpoint_id, cluster_id)
    if request_privilege in granted:
        if is_request_restricted(arl, subject_desc, endpoint_id, cluster_id):
            return AccessRestricted
        return AccessGranted
    return AccessDenied
```

#### ISD Derivation from Message
```python
def get_isd_from_message(message):
    isd = SubjectDescriptor()
    session_id = message.get_session_id()
    
    if session_auth_mode(session_id) == PASE:
        isd.AuthMode = PASE
        isd.IsCommissioning = True
        isd.Subjects = [DEFAULT_COMMISSIONING_PASSCODE]
        isd.FabricIndex = session_fabric_index(session_id)  # may be 0
    
    elif session_auth_mode(session_id) == CASE:
        isd.AuthMode = CASE
        isd.IsCommissioning = (session_fabric_index == pending_fabric_index)
        isd.Subjects = [session_src_node_id(session_id)]
        isd.Subjects.extend(session_case_authenticated_tags(session_id))  # up to 3 CATs
        isd.FabricIndex = session_fabric_index(session_id)
    
    elif session_auth_mode(session_id) == Group:
        isd.AuthMode = Group
        isd.Subjects = [message.source_node_id]
        isd.FabricIndex = group_fabric_index(session_id)
    
    return isd
```

---

## Chapter 7: Data Model

### 7.1 Hierarchy
```
Node
  └── Endpoint (0-65534)
        └── Cluster (Server/Client)
              ├── Attributes
              ├── Commands
              └── Events
```

### 7.2 Base Data Types
| Type | Short | ID | Size |
|------|-------|-----|------|
| Boolean | bool | 0x10 | 1 byte |
| 8-bit bitmap | map8 | 0x18 | 1 byte |
| 16-bit bitmap | map16 | 0x19 | 2 bytes |
| 32-bit bitmap | map32 | 0x1B | 4 bytes |
| 64-bit bitmap | map64 | 0x1F | 8 bytes |
| uint8 | uint8 | 0x20 | 1 byte |
| uint16 | uint16 | 0x21 | 2 bytes |
| uint32 | uint32 | 0x23 | 4 bytes |
| uint64 | uint64 | 0x27 | 8 bytes |
| int8 | int8 | 0x28 | 1 byte |
| int16 | int16 | 0x29 | 2 bytes |
| int32 | int32 | 0x2B | 4 bytes |
| int64 | int64 | 0x2F | 8 bytes |
| single | single | 0x39 | 4 bytes |
| double | double | 0x3A | 8 bytes |
| octet string | octstr | 0x41 | variable |
| string | string | 0x42 | variable |
| list | list | 0x48 | variable |
| struct | struct | 0x4C | variable |

### 7.3 Derived Data Types
| Type | Short | Base | ID |
|------|-------|------|-----|
| percent | percent | uint8 | 0xE6 |
| percent100ths | percent100ths | uint16 | 0xE7 |
| epoch-us | epoch-us | uint64 | 0xE3 |
| epoch-s | epoch-s | uint32 | 0xE4 |
| systime-us | systime-us | uint64 | 0xD0 |
| systime-ms | systime-ms | uint64 | 0xD1 |
| elapsed-s | elapsed-s | uint32 | 0xD2 |
| temperature | temperature | int16 | 0xD8 |
| enum8 | enum8 | uint8 | 0x30 |
| enum16 | enum16 | uint16 | 0x31 |
| group-id | group-id | uint16 | 0xC0 |
| endpoint-no | endpoint-no | uint16 | 0xC1 |
| vendor-id | vendor-id | uint16 | 0xC2 |
| devtype-id | devtype-id | uint32 | 0xC3 |
| fabric-id | fabric-id | uint64 | 0xC4 |
| fabric-idx | fabric-idx | uint8 | 0xC5 |
| cluster-id | cluster-id | uint32 | 0xE8 |
| attrib-id | attrib-id | uint32 | 0xE9 |
| event-id | event-id | uint32 | 0xEC |
| command-id | command-id | uint32 | 0xED |
| node-id | node-id | uint64 | 0xF0 |
| data-ver | data-ver | uint32 | 0xC7 |
| event-no | event-no | uint64 | 0xC8 |

### 7.4 Global Attributes
| ID | Name | Type |
|----|------|------|
| 0xFFFD | ClusterRevision | uint16 |
| 0xFFFC | FeatureMap | map32 |
| 0xFFFB | AttributeList | list[attrib-id] |
| 0xFFF9 | AcceptedCommandList | list[command-id] |
| 0xFFF8 | GeneratedCommandList | list[command-id] |

### 7.5 Conformance Notation
| Symbol | Meaning |
|--------|---------|
| M | Mandatory |
| O | Optional |
| P | Provisional |
| D | Deprecated |
| X | Disallowed |
| [XX] | Conditional on feature XX |
| [!XX] | Conditional on NOT feature XX |
| a \| b | Choice: a OR b |
| a, b | Both a AND b |

### 7.6 Qualities
| Quality | Symbol | Description |
|---------|--------|-------------|
| Nullable | X | Can be null |
| Non-Volatile | N | Persisted across reboot |
| Fixed | F | Cannot change after commissioning |
| Scene | S | Part of scene data |
| Reportable | P | Can be reported in subscriptions |
| Changes Omitted | C | Changes may not be reported |
| Singleton | I | Single instance across endpoints |
| Diagnostics | K | Diagnostic data |
| Large Message | L | May exceed MTU |
| Quieter Reporting | Q | Reduced reporting frequency |

### 7.7 Manufacturer Extensible Identifier (MEI)
```
MEI = (VendorID << 16) | LocalID

Standard (VID=0x0000): 0x0000_0000 to 0x0000_FFFE
Manufacturer: 0x0001_0000 to 0xFFF0_FFFF
Test: 0xFFF1_0000 to 0xFFF4_FFFF
```

---

## Chapter 8: Interaction Model

### 8.1 Interactions
| Interaction | Description |
|-------------|-------------|
| Read | Read attributes/events |
| Write | Write attributes |
| Subscribe | Subscribe to attribute/event changes |
| Invoke | Execute commands |

### 8.2 Paths

#### Attribute Path
```
AttributePathIB => STRUCTURE {
  EnableTagCompression [0, optional]: BOOLEAN,
  Node [1, optional]: node-id,
  Endpoint [2, optional]: endpoint-no,
  Cluster [3, optional]: cluster-id,
  Attribute [4, optional]: attrib-id,
  ListIndex [5, optional]: uint16 | null
}
```

#### Event Path
```
EventPathIB => STRUCTURE {
  Node [0, optional]: node-id,
  Endpoint [1, optional]: endpoint-no,
  Cluster [2, optional]: cluster-id,
  Event [3, optional]: event-id,
  IsUrgent [4, optional]: BOOLEAN
}
```

#### Command Path
```
CommandPathIB => STRUCTURE {
  Endpoint [0]: endpoint-no,
  Cluster [1]: cluster-id,
  Command [2]: command-id
}
```

### 8.3 Status Codes
| Code | Name | Value |
|------|------|-------|
| SUCCESS | Success | 0x00 |
| FAILURE | Generic failure | 0x01 |
| INVALID_SUBSCRIPTION | Invalid subscription | 0x7D |
| UNSUPPORTED_ACCESS | Access denied | 0x7E |
| UNSUPPORTED_ENDPOINT | Endpoint not found | 0x7F |
| INVALID_ACTION | Invalid action | 0x80 |
| UNSUPPORTED_COMMAND | Command not supported | 0x81 |
| INVALID_COMMAND | Invalid command | 0x85 |
| UNSUPPORTED_ATTRIBUTE | Attribute not supported | 0x86 |
| CONSTRAINT_ERROR | Value out of range | 0x87 |
| UNSUPPORTED_WRITE | Write not supported | 0x88 |
| RESOURCE_EXHAUSTED | Resources exhausted | 0x89 |
| NOT_FOUND | Not found | 0x8B |
| UNREPORTABLE_ATTRIBUTE | Cannot report | 0x8C |
| INVALID_DATA_TYPE | Wrong data type | 0x8D |
| UNSUPPORTED_READ | Read not supported | 0x8F |
| DATA_VERSION_MISMATCH | Version mismatch | 0x92 |
| TIMEOUT | Timeout | 0x94 |
| BUSY | Busy | 0x9C |
| UNSUPPORTED_CLUSTER | Cluster not supported | 0xC3 |
| NO_UPSTREAM_SUBSCRIPTION | No upstream sub | 0xC5 |
| NEEDS_TIMED_INTERACTION | Timed required | 0xC6 |
| UNSUPPORTED_EVENT | Event not supported | 0xC7 |
| PATHS_EXHAUSTED | Too many paths | 0xC8 |
| TIMED_REQUEST_MISMATCH | Timed mismatch | 0xC9 |
| FAILSAFE_REQUIRED | Failsafe required | 0xCA |
| INVALID_IN_STATE | Invalid in current state | 0xCB |
| NO_COMMAND_RESPONSE | No response | 0xCC |
| WRITE_IGNORED | Write ignored | 0xF0 |

### 8.4 IM Protocol Messages
| Opcode | Message |
|--------|---------|
| 0x01 | StatusResponse |
| 0x02 | ReadRequest |
| 0x05 | ReportData |
| 0x03 | SubscribeRequest |
| 0x04 | SubscribeResponse |
| 0x06 | WriteRequest |
| 0x07 | WriteResponse |
| 0x08 | InvokeRequest |
| 0x09 | InvokeResponse |
| 0x0A | TimedRequest |

### 8.5 Message Structures

#### ReadRequestMessage
```
ReadRequestMessage => STRUCTURE {
  AttributeRequests [0, optional]: list[AttributePathIB],
  EventRequests [1, optional]: list[EventPathIB],
  EventFilters [2, optional]: list[EventFilterIB],
  FabricFiltered [3]: BOOLEAN,
  DataVersionFilters [4, optional]: list[DataVersionFilterIB]
}
```

#### ReportDataMessage
```
ReportDataMessage => STRUCTURE {
  SubscriptionId [0, optional]: uint32,
  AttributeReports [1, optional]: list[AttributeReportIB],
  EventReports [2, optional]: list[EventReportIB],
  MoreChunkedMessages [3, optional]: BOOLEAN,
  SuppressResponse [4, optional]: BOOLEAN
}
```

#### SubscribeRequestMessage
```
SubscribeRequestMessage => STRUCTURE {
  KeepSubscriptions [0]: BOOLEAN,
  MinIntervalFloor [1]: uint16,
  MaxIntervalCeiling [2]: uint16,
  AttributeRequests [3, optional]: list[AttributePathIB],
  EventRequests [4, optional]: list[EventPathIB],
  EventFilters [5, optional]: list[EventFilterIB],
  FabricFiltered [7]: BOOLEAN,
  DataVersionFilters [8, optional]: list[DataVersionFilterIB]
}
```

#### SubscribeResponseMessage
```
SubscribeResponseMessage => STRUCTURE {
  SubscriptionId [0]: uint32,
  MaxInterval [2]: uint16
}
```

#### WriteRequestMessage
```
WriteRequestMessage => STRUCTURE {
  SuppressResponse [0, optional]: BOOLEAN,
  TimedRequest [1]: BOOLEAN,
  WriteRequests [2]: list[AttributeDataIB],
  MoreChunkedMessages [3, optional]: BOOLEAN
}
```

#### WriteResponseMessage
```
WriteResponseMessage => STRUCTURE {
  WriteResponses [0]: list[AttributeStatusIB]
}
```

#### InvokeRequestMessage
```
InvokeRequestMessage => STRUCTURE {
  SuppressResponse [0]: BOOLEAN,
  TimedRequest [1]: BOOLEAN,
  InvokeRequests [2]: list[CommandDataIB]
}
```

#### InvokeResponseMessage
```
InvokeResponseMessage => STRUCTURE {
  SuppressResponse [0]: BOOLEAN,
  InvokeResponses [1]: list[InvokeResponseIB],
  MoreChunkedMessages [2, optional]: BOOLEAN
}
```

#### TimedRequestMessage
```
TimedRequestMessage => STRUCTURE {
  Timeout [0]: uint16  // milliseconds
}
```

### 8.6 Information Blocks

#### AttributeDataIB
```
AttributeDataIB => STRUCTURE {
  DataVersion [0, optional]: data-ver,
  Path [1]: AttributePathIB,
  Data [2]: any
}
```

#### AttributeReportIB
```
AttributeReportIB => STRUCTURE {
  AttributeStatus [0, optional]: AttributeStatusIB,
  AttributeData [1, optional]: AttributeDataIB
}
```

#### EventDataIB
```
EventDataIB => STRUCTURE {
  Path [0]: EventPathIB,
  EventNumber [1]: event-no,
  Priority [2]: priority,
  EpochTimestamp [3, optional]: epoch-us,
  SystemTimestamp [4, optional]: systime-us,
  DeltaEpochTimestamp [5, optional]: epoch-us,
  DeltaSystemTimestamp [6, optional]: systime-us,
  Data [7, optional]: any
}
```

#### CommandDataIB
```
CommandDataIB => STRUCTURE {
  CommandPath [0]: CommandPathIB,
  CommandFields [1, optional]: any
}
```

#### StatusIB
```
StatusIB => STRUCTURE {
  Status [0]: status,
  ClusterStatus [1, optional]: status
}
```

---

## Chapter 9: System Model

### 9.1 Endpoint Composition

#### Root Node (Endpoint 0)
Required clusters:
- Basic Information (0x0028)
- Access Control (0x001F)
- General Commissioning (0x0030)
- Network Commissioning (0x0031)
- General Diagnostics (0x0033)
- Administrator Commissioning (0x003C)
- Node Operational Credentials (0x003E)
- Group Key Management (0x003F)

Optional clusters:
- Localization Configuration (0x002B)
- Time Format Localization (0x002C)
- Unit Localization (0x002D)
- Power Source Configuration (0x002E)
- Time Synchronization (0x0038)
- Diagnostic Logs (0x0032)
- Software Diagnostics (0x0034)
- Thread Network Diagnostics (0x0035)
- Wi-Fi Network Diagnostics (0x0036)
- Ethernet Network Diagnostics (0x0037)
- ICD Management (0x0046)

### 9.2 Descriptor Cluster (0x001D)

#### Attributes
| ID | Name | Type |
|----|------|------|
| 0x0000 | DeviceTypeList | list[DeviceTypeStruct] |
| 0x0001 | ServerList | list[cluster-id] |
| 0x0002 | ClientList | list[cluster-id] |
| 0x0003 | PartsList | list[endpoint-no] |
| 0x0004 | TagList | list[SemanticTagStruct] |

#### DeviceTypeStruct
```
DeviceTypeStruct => STRUCTURE {
  DeviceType [0]: devtype-id,
  Revision [1]: uint16
}
```

### 9.3 Binding Cluster (0x001E)

#### Attributes
| ID | Name | Type |
|----|------|------|
| 0x0000 | Binding | list[TargetStruct] |

#### TargetStruct
```
TargetStruct => STRUCTURE {
  Node [1, optional]: node-id,
  Group [2, optional]: group-id,
  Endpoint [3, optional]: endpoint-no,
  Cluster [4, optional]: cluster-id,
  FabricIndex [254]: fabric-idx
}
```

### 9.4 Access Control Cluster (0x001F)

#### Attributes
| ID | Name | Type |
|----|------|------|
| 0x0000 | ACL | list[AccessControlEntryStruct] |
| 0x0001 | Extension | list[AccessControlExtensionStruct] |
| 0x0002 | SubjectsPerAccessControlEntry | uint16 |
| 0x0003 | TargetsPerAccessControlEntry | uint16 |
| 0x0004 | AccessControlEntriesPerFabric | uint16 |



### 9.5 ICD Management Cluster (0x0046)

#### Classification
- Hierarchy: Base
- Role: Utility
- Scope: Node
- PICS Code: ICDM

#### Features
| Bit | Code | Description | Conformance |
|-----|------|-------------|-------------|
| 0 | CIP | CheckInProtocolSupport | LITS, O |
| 1 | UAT | UserActiveModeTrigger | LITS, O |
| 2 | LITS | LongIdleTimeSupport | O |
| 3 | DSLS | DynamicSitLitSupport | [LITS] |

#### Attributes
| ID | Name | Type | Constraint | Quality | Default | Conformance |
|----|------|------|------------|---------|---------|-------------|
| 0x0000 | IdleModeDuration | uint32 | 1-64800 | F | 1 | M |
| 0x0001 | ActiveModeDuration | uint32 | all | F | 300 | M |
| 0x0002 | ActiveModeThreshold | uint16 | all | F | 300 | M |
| 0x0003 | RegisteredClients | list[MonitoringRegistrationStruct] | desc | N | [] | CIP |
| 0x0004 | ICDCounter | uint32 | all | NC | 0 | CIP |
| 0x0005 | ClientsSupportedPerFabric | uint16 | min 1 | F | 1 | CIP |
| 0x0006 | UserActiveModeTriggerHint | UserActiveModeTriggerBitmap | desc | F | 0 | UAT |
| 0x0007 | UserActiveModeTriggerInstruction | string | max 128 | F | "" | desc |
| 0x0008 | OperatingMode | OperatingModeEnum | all | | | LITS |
| 0x0009 | MaximumCheckInBackOff | uint32 | IdleModeDuration-64800 | F | 1 | CIP |

#### MonitoringRegistrationStruct
```
MonitoringRegistrationStruct => STRUCTURE {
  CheckInNodeID [1]: node-id,
  MonitoredSubject [2]: subject-id,
  ClientType [4]: ClientTypeEnum,
  FabricIndex [254]: fabric-idx
}
```

#### ClientTypeEnum
| Value | Name | Description |
|-------|------|-------------|
| 0 | Permanent | Resident, always-on infrastructure |
| 1 | Ephemeral | Mobile or non-resident client |

#### OperatingModeEnum
| Value | Name | Description |
|-------|------|-------------|
| 0 | SIT | Short Idle Time ICD |
| 1 | LIT | Long Idle Time ICD |

#### UserActiveModeTriggerBitmap
| Bit | Name | Description |
|-----|------|-------------|
| 0 | PowerCycle | Power cycle to enter Active Mode |
| 1 | SettingsMenu | Settings menu provides instructions |
| 2 | CustomInstruction | Custom instruction in UserActiveModeTriggerInstruction |
| 3 | DeviceManual | Device manual provides instructions |
| 4 | ActuateSensor | Actuate sensor |
| 5 | ActuateSensorSeconds | Actuate sensor for N seconds |
| 6 | ActuateSensorTimes | Actuate sensor N times |
| 7 | ActuateSensorLightsBlink | Actuate sensor until light blinks |
| 8 | ResetButton | Press reset button |
| 9 | ResetButtonLightsBlink | Press reset button until light blinks |
| 10 | ResetButtonSeconds | Press reset button for N seconds |
| 11 | ResetButtonTimes | Press reset button N times |
| 12 | SetupButton | Press setup button |
| 13 | SetupButtonSeconds | Press setup button for N seconds |
| 14 | SetupButtonLightsBlink | Press setup button until light blinks |
| 15 | SetupButtonTimes | Press setup button N times |
| 16 | AppDefinedButton | Press the N button |

#### Commands
| ID | Name | Direction | Conformance |
|----|------|-----------|-------------|
| 0x00 | RegisterClient | Client→Server | CIP |
| 0x01 | RegisterClientResponse | Server→Client | CIP |
| 0x02 | UnregisterClient | Client→Server | CIP |
| 0x03 | StayActiveRequest | Client→Server | CIP |
| 0x04 | StayActiveResponse | Server→Client | CIP |

#### ICD Operating Modes

**Idle Mode**: Device unreachable, max duration = IdleModeDuration
**Active Mode**: Device reachable, min duration = ActiveModeDuration + ActiveModeThreshold

**SIT ICD Requirements**:
- IdleModeDuration ≤ 15 seconds
- Clients MAY ignore Check-In Protocol
- Clients MAY ignore ActiveModeThreshold

**LIT ICD Requirements**:
- IdleModeDuration > 15 seconds
- Requires Check-In Protocol support
- Requires client registration
- Operates as SIT if no registered clients

---

## Chapter 10: Interaction Encoding (TLV)

### 10.1 TLV Element Structure
```
Element = ControlByte [Tag] [Length] [Value]
```

### 10.2 Control Byte
```
Bits 7-5: Tag Control
Bits 4-0: Element Type
```

#### Element Types
| Value | Type |
|-------|------|
| 0x00 | Signed Int (1 byte) |
| 0x01 | Signed Int (2 bytes) |
| 0x02 | Signed Int (4 bytes) |
| 0x03 | Signed Int (8 bytes) |
| 0x04 | Unsigned Int (1 byte) |
| 0x05 | Unsigned Int (2 bytes) |
| 0x06 | Unsigned Int (4 bytes) |
| 0x07 | Unsigned Int (8 bytes) |
| 0x08 | Boolean False |
| 0x09 | Boolean True |
| 0x0A | Float (4 bytes) |
| 0x0B | Double (8 bytes) |
| 0x0C | UTF-8 String (1-byte length) |
| 0x0D | UTF-8 String (2-byte length) |
| 0x0E | UTF-8 String (4-byte length) |
| 0x0F | UTF-8 String (8-byte length) |
| 0x10 | Octet String (1-byte length) |
| 0x11 | Octet String (2-byte length) |
| 0x12 | Octet String (4-byte length) |
| 0x13 | Octet String (8-byte length) |
| 0x14 | Null |
| 0x15 | Structure |
| 0x16 | Array |
| 0x17 | List |
| 0x18 | End of Container |

#### Tag Control
| Value | Form |
|-------|------|
| 0x00 | Anonymous |
| 0x20 | Context-specific (1-byte tag) |
| 0x40 | Common Profile (2-byte tag) |
| 0x60 | Common Profile (4-byte tag) |
| 0x80 | Implicit Profile (2-byte tag) |
| 0xA0 | Implicit Profile (4-byte tag) |
| 0xC0 | Fully-qualified (6-byte tag) |
| 0xE0 | Fully-qualified (8-byte tag) |

### 10.3 Tag Encoding

#### Context-Specific Tag (1 byte)
```
Tag = TagNumber (0-255)
```

#### Fully-Qualified Tag (6 or 8 bytes)
```
Tag = VendorID (2 bytes) || ProfileNumber (2 bytes) || TagNumber (2 or 4 bytes)
```

---

## Chapter 11: Device Management Clusters

### 11.1 Basic Information Cluster (0x0028)

#### Attributes
| ID | Name | Type | Constraint |
|----|------|------|------------|
| 0x0000 | DataModelRevision | uint16 | |
| 0x0001 | VendorName | string | max 32 |
| 0x0002 | VendorID | vendor-id | |
| 0x0003 | ProductName | string | max 32 |
| 0x0004 | ProductID | uint16 | |
| 0x0005 | NodeLabel | string | max 32 |
| 0x0006 | Location | string | max 2 |
| 0x0007 | HardwareVersion | uint16 | |
| 0x0008 | HardwareVersionString | string | 1-64 |
| 0x0009 | SoftwareVersion | uint32 | |
| 0x000A | SoftwareVersionString | string | 1-64 |
| 0x000B | ManufacturingDate | string | 8-16 |
| 0x000C | PartNumber | string | max 32 |
| 0x000D | ProductURL | string | max 256 |
| 0x000E | ProductLabel | string | max 64 |
| 0x000F | SerialNumber | string | max 32 |
| 0x0010 | LocalConfigDisabled | bool | |
| 0x0011 | Reachable | bool | |
| 0x0012 | UniqueID | string | max 32 |
| 0x0013 | CapabilityMinima | CapabilityMinimaStruct | |
| 0x0014 | ProductAppearance | ProductAppearanceStruct | |
| 0x0015 | SpecificationVersion | uint32 | |
| 0x0016 | MaxPathsPerInvoke | uint16 | |

#### CapabilityMinimaStruct
```
CapabilityMinimaStruct => STRUCTURE {
  CaseSessionsPerFabric [0]: uint16,
  SubscriptionsPerFabric [1]: uint16
}
```

### 11.2 General Commissioning Cluster (0x0030)

#### Attributes
| ID | Name | Type |
|----|------|------|
| 0x0000 | Breadcrumb | uint64 |
| 0x0001 | BasicCommissioningInfo | BasicCommissioningInfo |
| 0x0002 | RegulatoryConfig | RegulatoryLocationTypeEnum |
| 0x0003 | LocationCapability | RegulatoryLocationTypeEnum |
| 0x0004 | SupportsConcurrentConnection | bool |

#### BasicCommissioningInfo
```
BasicCommissioningInfo => STRUCTURE {
  FailSafeExpiryLengthSeconds [0]: uint16,
  MaxCumulativeFailsafeSeconds [1]: uint16
}
```

#### Commands
| ID | Name | Direction |
|----|------|-----------|
| 0x00 | ArmFailSafe | Client→Server |
| 0x01 | ArmFailSafeResponse | Server→Client |
| 0x02 | SetRegulatoryConfig | Client→Server |
| 0x03 | SetRegulatoryConfigResponse | Server→Client |
| 0x04 | CommissioningComplete | Client→Server |
| 0x05 | CommissioningCompleteResponse | Server→Client |

### 11.3 Network Commissioning Cluster (0x0031)

#### Features
| Bit | Code | Description |
|-----|------|-------------|
| 0 | WI | Wi-Fi |
| 1 | TH | Thread |
| 2 | ET | Ethernet |
| 3 | PC | Per-device credentials |

#### Attributes
| ID | Name | Type |
|----|------|------|
| 0x0000 | MaxNetworks | uint8 |
| 0x0001 | Networks | list[NetworkInfoStruct] |
| 0x0002 | ScanMaxTimeSeconds | uint8 |
| 0x0003 | ConnectMaxTimeSeconds | uint8 |
| 0x0004 | InterfaceEnabled | bool |
| 0x0005 | LastNetworkingStatus | NetworkCommissioningStatusEnum |
| 0x0006 | LastNetworkID | octstr |
| 0x0007 | LastConnectErrorValue | int32 |
| 0x0008 | SupportedWiFiBands | list[WiFiBandEnum] |
| 0x0009 | SupportedThreadFeatures | ThreadCapabilitiesBitmap |
| 0x000A | ThreadVersion | uint16 |

#### NetworkInfoStruct
```
NetworkInfoStruct => STRUCTURE {
  NetworkID [0]: octstr,
  Connected [1]: bool
}
```

#### Commands
| ID | Name | Direction |
|----|------|-----------|
| 0x00 | ScanNetworks | Client→Server |
| 0x01 | ScanNetworksResponse | Server→Client |
| 0x02 | AddOrUpdateWiFiNetwork | Client→Server |
| 0x03 | AddOrUpdateThreadNetwork | Client→Server |
| 0x04 | RemoveNetwork | Client→Server |
| 0x05 | NetworkConfigResponse | Server→Client |
| 0x06 | ConnectNetwork | Client→Server |
| 0x07 | ConnectNetworkResponse | Server→Client |
| 0x08 | ReorderNetwork | Client→Server |
| 0x09 | QueryIdentity | Client→Server |
| 0x0A | QueryIdentityResponse | Server→Client |

### 11.4 Node Operational Credentials Cluster (0x003E)

#### Attributes
| ID | Name | Type |
|----|------|------|
| 0x0000 | NOCs | list[NOCStruct] |
| 0x0001 | Fabrics | list[FabricDescriptorStruct] |
| 0x0002 | SupportedFabrics | uint8 |
| 0x0003 | CommissionedFabrics | uint8 |
| 0x0004 | TrustedRootCertificates | list[octstr] |
| 0x0005 | CurrentFabricIndex | fabric-idx |

#### NOCStruct
```
NOCStruct => STRUCTURE {
  NOC [1]: octstr,
  ICAC [2, nullable]: octstr,
  FabricIndex [254]: fabric-idx
}
```

#### FabricDescriptorStruct
```
FabricDescriptorStruct => STRUCTURE {
  RootPublicKey [1]: octstr,
  VendorID [2]: vendor-id,
  FabricID [3]: fabric-id,
  NodeID [4]: node-id,
  Label [5]: string,
  FabricIndex [254]: fabric-idx
}
```

#### Commands
| ID | Name | Direction |
|----|------|-----------|
| 0x00 | AttestationRequest | Client→Server |
| 0x01 | AttestationResponse | Server→Client |
| 0x02 | CertificateChainRequest | Client→Server |
| 0x03 | CertificateChainResponse | Server→Client |
| 0x04 | CSRRequest | Client→Server |
| 0x05 | CSRResponse | Server→Client |
| 0x06 | AddNOC | Client→Server |
| 0x07 | UpdateNOC | Client→Server |
| 0x08 | NOCResponse | Server→Client |
| 0x09 | UpdateFabricLabel | Client→Server |
| 0x0A | RemoveFabric | Client→Server |
| 0x0B | AddTrustedRootCertificate | Client→Server |

### 11.5 Administrator Commissioning Cluster (0x003C)

#### Attributes
| ID | Name | Type |
|----|------|------|
| 0x0000 | WindowStatus | CommissioningWindowStatusEnum |
| 0x0001 | AdminFabricIndex | fabric-idx |
| 0x0002 | AdminVendorId | vendor-id |

#### CommissioningWindowStatusEnum
| Value | Name |
|-------|------|
| 0 | WindowNotOpen |
| 1 | EnhancedWindowOpen |
| 2 | BasicWindowOpen |

#### Commands
| ID | Name | Direction |
|----|------|-----------|
| 0x00 | OpenCommissioningWindow | Client→Server |
| 0x01 | OpenBasicCommissioningWindow | Client→Server |
| 0x02 | RevokeCommissioning | Client→Server |

### 11.6 Group Key Management Cluster (0x003F)

#### Attributes
| ID | Name | Type |
|----|------|------|
| 0x0000 | GroupKeyMap | list[GroupKeyMapStruct] |
| 0x0001 | GroupTable | list[GroupInfoMapStruct] |
| 0x0002 | MaxGroupsPerFabric | uint16 |
| 0x0003 | MaxGroupKeysPerFabric | uint16 |

#### GroupKeyMapStruct
```
GroupKeyMapStruct => STRUCTURE {
  GroupId [1]: group-id,
  GroupKeySetID [2]: uint16,
  FabricIndex [254]: fabric-idx
}
```

#### GroupKeySetStruct
```
GroupKeySetStruct => STRUCTURE {
  GroupKeySetID [0]: uint16,
  GroupKeySecurityPolicy [1]: GroupKeySecurityPolicyEnum,
  EpochKey0 [2, nullable]: octstr,
  EpochStartTime0 [3, nullable]: epoch-us,
  EpochKey1 [4, nullable]: octstr,
  EpochStartTime1 [5, nullable]: epoch-us,
  EpochKey2 [6, nullable]: octstr,
  EpochStartTime2 [7, nullable]: epoch-us
}
```

#### Commands
| ID | Name | Direction |
|----|------|-----------|
| 0x00 | KeySetWrite | Client→Server |
| 0x01 | KeySetRead | Client→Server |
| 0x02 | KeySetReadResponse | Server→Client |
| 0x03 | KeySetRemove | Client→Server |
| 0x04 | KeySetReadAllIndices | Client→Server |
| 0x05 | KeySetReadAllIndicesResponse | Server→Client |



### 11.7 General Diagnostics Cluster (0x0033)

#### Attributes
| ID | Name | Type |
|----|------|------|
| 0x0000 | NetworkInterfaces | list[NetworkInterface] |
| 0x0001 | RebootCount | uint16 |
| 0x0002 | UpTime | uint64 |
| 0x0003 | TotalOperationalHours | uint32 |
| 0x0004 | BootReason | BootReasonEnum |
| 0x0005 | ActiveHardwareFaults | list[HardwareFaultEnum] |
| 0x0006 | ActiveRadioFaults | list[RadioFaultEnum] |
| 0x0007 | ActiveNetworkFaults | list[NetworkFaultEnum] |
| 0x0008 | TestEventTriggersEnabled | bool |

#### BootReasonEnum
| Value | Name |
|-------|------|
| 0 | Unspecified |
| 1 | PowerOnReboot |
| 2 | BrownOutReset |
| 3 | SoftwareWatchdogReset |
| 4 | HardwareWatchdogReset |
| 5 | SoftwareUpdateCompleted |
| 6 | SoftwareReset |

#### Commands
| ID | Name | Direction |
|----|------|-----------|
| 0x00 | TestEventTrigger | Client→Server |
| 0x01 | TimeSnapshot | Client→Server |
| 0x02 | TimeSnapshotResponse | Server→Client |

### 11.8 Wi-Fi Network Diagnostics Cluster (0x0036)

#### Attributes
| ID | Name | Type |
|----|------|------|
| 0x0000 | BSSID | octstr |
| 0x0001 | SecurityType | SecurityTypeEnum |
| 0x0002 | WiFiVersion | WiFiVersionEnum |
| 0x0003 | ChannelNumber | uint16 |
| 0x0004 | RSSI | int8 |
| 0x0005 | BeaconLostCount | uint32 |
| 0x0006 | BeaconRxCount | uint32 |
| 0x0007 | PacketMulticastRxCount | uint32 |
| 0x0008 | PacketMulticastTxCount | uint32 |
| 0x0009 | PacketUnicastRxCount | uint32 |
| 0x000A | PacketUnicastTxCount | uint32 |
| 0x000B | CurrentMaxRate | uint64 |
| 0x000C | OverrunCount | uint64 |

#### SecurityTypeEnum
| Value | Name |
|-------|------|
| 0 | Unspecified |
| 1 | None |
| 2 | WEP |
| 3 | WPA |
| 4 | WPA2 |
| 5 | WPA3 |

### 11.9 OTA Software Update

#### OTA Provider Cluster (0x0029)

##### Commands
| ID | Name | Direction |
|----|------|-----------|
| 0x00 | QueryImage | Client→Server |
| 0x01 | QueryImageResponse | Server→Client |
| 0x02 | ApplyUpdateRequest | Client→Server |
| 0x03 | ApplyUpdateResponse | Server→Client |
| 0x04 | NotifyUpdateApplied | Client→Server |

#### OTA Requestor Cluster (0x002A)

##### Attributes
| ID | Name | Type |
|----|------|------|
| 0x0000 | DefaultOTAProviders | list[ProviderLocation] |
| 0x0001 | UpdatePossible | bool |
| 0x0002 | UpdateState | UpdateStateEnum |
| 0x0003 | UpdateStateProgress | uint8 |

##### UpdateStateEnum
| Value | Name |
|-------|------|
| 0 | Unknown |
| 1 | Idle |
| 2 | Querying |
| 3 | DelayedOnQuery |
| 4 | Downloading |
| 5 | Applying |
| 6 | DelayedOnApply |
| 7 | RollingBack |
| 8 | DelayedOnUserConsent |

##### Commands
| ID | Name | Direction |
|----|------|-----------|
| 0x00 | AnnounceOTAProvider | Client→Server |

#### OTA File Format
```
OTA File = Header + Payload

Header:
  - FileIdentifier (4 bytes): 0x1BEEF11E
  - TotalSize (8 bytes)
  - HeaderSize (4 bytes)
  - HeaderVersion (4 bytes)
  - VendorID (2 bytes)
  - ProductID (2 bytes)
  - SoftwareVersion (4 bytes)
  - SoftwareVersionString (64 bytes max)
  - PayloadSize (8 bytes)
  - MinApplicableSoftwareVersion (4 bytes, optional)
  - MaxApplicableSoftwareVersion (4 bytes, optional)
  - ReleaseNotesURL (256 bytes max, optional)
  - ImageDigestType (1 byte)
  - ImageDigest (variable)
```

### 11.10 BDX (Bulk Data Exchange) Protocol

#### Protocol ID
- Vendor ID: 0x0000 (Matter Common)
- Protocol ID: 0x0002 (PROTOCOL_ID_BDX)

#### BDX Protocol Opcodes
| Opcode | Message |
|--------|---------|
| 0x01 | SendInit |
| 0x02 | SendAccept |
| 0x04 | ReceiveInit |
| 0x05 | ReceiveAccept |
| 0x10 | BlockQuery |
| 0x11 | Block |
| 0x12 | BlockEOF |
| 0x13 | BlockAck |
| 0x14 | BlockAckEOF |
| 0x15 | BlockQueryWithSkip |

#### BDX Status Codes
| Code | Name | Description |
|------|------|-------------|
| 0x0012 | LENGTH_TOO_LARGE | Definite length too large |
| 0x0013 | LENGTH_TOO_SHORT | Definite length too short |
| 0x0014 | LENGTH_MISMATCH | Pre-negotiated size not fulfilled |
| 0x0015 | LENGTH_REQUIRED | Definite length required |
| 0x0016 | BAD_MESSAGE_CONTENTS | Malformed message |
| 0x0017 | BAD_BLOCK_COUNTER | Block counter out of order |
| 0x0018 | UNEXPECTED_MESSAGE | Contextually inappropriate message |
| 0x0019 | RESPONDER_BUSY | Responder too busy |
| 0x001F | TRANSFER_FAILED_UNKNOWN_ERROR | Other error |
| 0x0050 | TRANSFER_METHOD_NOT_SUPPORTED | Transfer mode mismatch |
| 0x0051 | FILE_DESIGNATOR_UNKNOWN | Unknown file designator |
| 0x0052 | START_OFFSET_NOT_SUPPORTED | Start offset not supported |
| 0x0053 | VERSION_NOT_SUPPORTED | No common version |
| 0x005F | UNKNOWN | Other unexpected error |

#### BDX Terminology
- **Sender**: Node sending bulk data
- **Receiver**: Node receiving bulk data
- **Initiator**: Node starting transfer (SendInit or ReceiveInit)
- **Responder**: Node accepting/rejecting transfer
- **Driver**: Controls transfer rate in synchronous mode
- **Follower**: Responds to driver in synchronous mode

#### Transfer Modes
- **Synchronous (Driven)**: One party controls rate, each message acknowledged
- **Asynchronous**: No driver, messages sent freely, flow control by transport

#### SendInit/ReceiveInit Message Fields
| Field | Size | Description |
|-------|------|-------------|
| PTC | 1 byte | Proposed Transfer Control |
| RC | 1 byte | Range Control |
| PMBS | 2 bytes | Proposed Max Block Size |
| STARTOFS | 4/8 bytes | Start Offset (optional) |
| LEN | 4/8 bytes | Definite Length (optional) |
| FDL | 2 bytes | File Designator Length |
| FD | variable | File Designator |
| MDATA | variable | Metadata TLV (optional) |

#### Proposed Transfer Control (PTC) Bits
| Bit | Name | Description |
|-----|------|-------------|
| 0-3 | VERSION | Protocol version (0 for Matter 1.0) |
| 4 | SENDER_DRIVE | Sender drive supported |
| 5 | RECEIVER_DRIVE | Receiver drive supported |
| 6 | ASYNC | Asynchronous mode supported |
| 7 | RFU | Reserved |

#### Range Control (RC) Bits
| Bit | Name | Description |
|-----|------|-------------|
| 0 | DEFLEN | Definite length present |
| 1 | STARTOFS | Start offset present |
| 4 | WIDERANGE | 64-bit (1) or 32-bit (0) offset/length |

---

## Chapter 12: Multiple Fabrics

### 12.1 Multi-Admin Support
- Node can be member of multiple fabrics (min 5, max 16)
- Each fabric has independent:
  - NOC and RCAC
  - ACL entries
  - Bindings
  - Group memberships
  - Subscriptions

### 12.2 Joint Fabric
- Single fabric with single root of trust
- Administered by multiple ecosystems
- Uses Anchor Administrator for RCAC

---

## Chapter 13: Security Requirements

### 13.1 Commissioning Security
- PASE provides mutual authentication
- Device attestation verifies authenticity
- NOC binds node to fabric

### 13.2 Factory Reset
- SHALL clear all fabric data
- SHALL clear all operational credentials
- SHALL clear all ACL entries
- SHALL clear all bindings
- MAY retain device attestation credentials

### 13.3 Firmware Security
- Secure boot recommended
- Signed firmware updates
- Rollback protection recommended

---

## Appendix A: TLV Encoding Reference

### A.1 Common Patterns

#### Empty Structure
```
0x15 0x18
```

#### Boolean True
```
0x09
```

#### Boolean False
```
0x08
```

#### Unsigned Integer (context tag 1, value 42)
```
0x24 0x01 0x2A
```

#### String (context tag 2, "hello")
```
0x2C 0x02 0x05 0x68 0x65 0x6C 0x6C 0x6F
```

#### Octet String (context tag 3, 3 bytes)
```
0x30 0x03 0x03 0xAA 0xBB 0xCC
```

### A.2 Nested Structure Example
```
{
  field1 (1) = 100,
  field2 (2) = "test",
  nested (3) = {
    inner (1) = true
  }
}

Encoding:
0x15                    // Structure start
  0x24 0x01 0x64        // uint8, tag 1, value 100
  0x2C 0x02 0x04 0x74 0x65 0x73 0x74  // string, tag 2, "test"
  0x35 0x03             // Structure, tag 3
    0x29 0x01           // bool true, tag 1
  0x18                  // End nested structure
0x18                    // End outer structure
```

---

## Appendix B: Certificate Encoding

### B.1 Matter Certificate TLV Structure
```
matter-certificate => STRUCTURE {
  serial-num [1]: OCTET STRING,
  sig-algo [2]: UNSIGNED INTEGER,
  issuer [3]: dn-list,
  not-before [4]: UNSIGNED INTEGER,
  not-after [5]: UNSIGNED INTEGER,
  subject [6]: dn-list,
  pub-key-algo [7]: UNSIGNED INTEGER,
  ec-curve-id [8]: UNSIGNED INTEGER,
  ec-pub-key [9]: OCTET STRING,
  extensions [10]: cert-extensions,
  signature [11]: OCTET STRING
}
```

### B.2 DN Attribute Types
| OID | Tag | Name |
|-----|-----|------|
| 2.5.4.3 | 1 | CommonName |
| 2.5.4.6 | 2 | CountryName |
| 2.5.4.10 | 3 | OrganizationName |
| 1.3.6.1.4.1.37244.1.1 | 17 | matter-rcac-id |
| 1.3.6.1.4.1.37244.1.2 | 18 | matter-fabric-id |
| 1.3.6.1.4.1.37244.1.3 | 19 | matter-noc-cat |
| 1.3.6.1.4.1.37244.1.4 | 20 | matter-icac-id |
| 1.3.6.1.4.1.37244.1.5 | 21 | matter-node-id |
| 1.3.6.1.4.1.37244.2.1 | 22 | matter-vid |
| 1.3.6.1.4.1.37244.2.2 | 23 | matter-pid |

### B.3 Signature Algorithm Values
| Value | Algorithm |
|-------|-----------|
| 1 | ECDSA with SHA-256 |

### B.4 Public Key Algorithm Values
| Value | Algorithm |
|-------|-----------|
| 1 | EC Public Key |

### B.5 EC Curve ID Values
| Value | Curve |
|-------|-------|
| 1 | prime256v1 (P-256) |

---

## Appendix C: Status Report Format

### C.1 Structure
```
StatusReport => STRUCTURE {
  GeneralCode [0]: uint16,
  ProtocolId [1]: uint32,
  ProtocolCode [2]: uint16,
  ProtocolData [3, optional]: OCTET STRING
}
```

### C.2 General Codes
| Code | Name |
|------|------|
| 0 | SUCCESS |
| 1 | FAILURE |
| 2 | BAD_PRECONDITION |
| 3 | OUT_OF_RANGE |
| 4 | BAD_REQUEST |
| 5 | UNSUPPORTED |
| 6 | UNEXPECTED |
| 7 | RESOURCE_EXHAUSTED |
| 8 | BUSY |
| 9 | TIMEOUT |
| 10 | CONTINUE |
| 11 | ABORTED |
| 12 | INVALID_ARGUMENT |
| 13 | NOT_FOUND |
| 14 | ALREADY_EXISTS |
| 15 | PERMISSION_DENIED |
| 16 | DATA_LOSS |

### C.3 Secure Channel Protocol Codes
| Code | Name |
|------|------|
| 0 | SESSION_ESTABLISHMENT_SUCCESS |
| 1 | NO_SHARED_TRUST_ROOTS |
| 2 | INVALID_PARAMETER |
| 3 | CLOSE_SESSION |
| 4 | BUSY |

---

## Appendix D: Key Constants Summary

### D.1 Cryptographic Constants
```
CRYPTO_GROUP_SIZE_BYTES = 32
CRYPTO_PUBLIC_KEY_SIZE_BYTES = 65
CRYPTO_HASH_LEN_BYTES = 32
CRYPTO_SYMMETRIC_KEY_LENGTH_BYTES = 16
CRYPTO_AEAD_MIC_LENGTH_BYTES = 16
CRYPTO_AEAD_NONCE_LENGTH_BYTES = 13
```

### D.2 Protocol Constants
```
MATTER_PORT = 5540
PROTOCOL_ID_SECURE_CHANNEL = 0x0000
PROTOCOL_ID_INTERACTION_MODEL = 0x0001
PROTOCOL_ID_BDX = 0x0002
PROTOCOL_ID_UDC = 0x0003
```

### D.3 Session Defaults
```
SESSION_IDLE_INTERVAL = 500ms
SESSION_ACTIVE_INTERVAL = 300ms
SESSION_ACTIVE_THRESHOLD = 4000ms
```

### D.4 MRP Defaults
```
MRP_MAX_TRANSMISSIONS = 5
MRP_BACKOFF_BASE = 1.6
MRP_BACKOFF_JITTER = 0.25
MRP_BACKOFF_THRESHOLD = 1
MRP_STANDALONE_ACK_TIMEOUT = 200ms
```

### D.5 ICD Defaults
```
IdleModeDuration = 1 second
ActiveModeDuration = 300ms
ActiveModeThreshold = 300ms
SIT_MAX_IDLE_DURATION = 15 seconds
```

---

*End of Matter 1.4.1 Core Specification Compact Reference*
