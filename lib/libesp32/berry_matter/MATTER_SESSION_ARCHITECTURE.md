# Matter Session Establishment Architecture

## Overview

This document describes the session establishment architecture in Tasmota's Matter implementation. The implementation supports two types of session establishment:

1. **PASE (Passcode-Authenticated Session Establishment)** - Used during initial commissioning with a passcode
2. **CASE (Certificate-Authenticated Session Establishment)** - Used for subsequent connections with certificates

## Class Hierarchy for Session Management

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                           Matter_Device                                      │
│  (Main device controller - Matter_zz_Device.be)                             │
│  - Orchestrates all Matter operations                                        │
│  - Manages plugins, UDP server, message handler                             │
└─────────────────────────────────────────────────────────────────────────────┘
         │
         │ owns
         ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                        Matter_Session_Store                                  │
│  (Session persistence - Matter_Session_Store.be)                            │
│  - Manages all sessions and fabrics                                          │
│  - Persists to /_matter_fabrics.json                                        │
│  - Creates/removes sessions and fabrics                                      │
└─────────────────────────────────────────────────────────────────────────────┘
         │
         │ contains
         ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                          Matter_Fabric                                       │
│  (Fabric management - Matter_Fabric.be)                                     │
│  - Represents a commissioned fabric (controller ecosystem)                   │
│  - Stores NOC, ICAC, root CA, IPK                                           │
│  - Contains list of CASE sessions                                            │
└─────────────────────────────────────────────────────────────────────────────┘
         │
         │ contains
         ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                          Matter_Session                                      │
│  (Session state - Matter_Session.be)                                        │
│  - Represents a single secure session                                        │
│  - Stores encryption keys (i2r, r2i)                                        │
│  - Manages message counters                                                  │
│  - Tracks session mode (PASE/CASE)                                          │
└─────────────────────────────────────────────────────────────────────────────┘
```

## Message Flow Architecture

```
┌──────────────┐     UDP      ┌──────────────────┐
│   Matter     │◄────────────►│  Matter_UDP      │
│  Controller  │              │  Server          │
└──────────────┘              └────────┬─────────┘
                                       │
                                       │ raw bytes
                                       ▼
                              ┌──────────────────┐
                              │ Matter_Message   │
                              │ Handler          │
                              │ (dispatch)       │
                              └────────┬─────────┘
                                       │
                    ┌──────────────────┼──────────────────┐
                    │                  │                  │
                    ▼                  ▼                  ▼
           ┌───────────────┐  ┌───────────────┐  ┌───────────────┐
           │ Commissioning │  │ Interaction   │  │ Control       │
           │ Context       │  │ Model (IM)    │  │ Message       │
           │ (PASE/CASE)   │  │               │  │               │
           └───────────────┘  └───────────────┘  └───────────────┘
```

## Key Classes and Their Responsibilities

### 1. Matter_Device (Matter_zz_Device.be)
The main orchestrator class that:
- Initializes all Matter subsystems
- Manages the plugin system for endpoints
- Handles UDP message routing
- Coordinates commissioning and session management

```berry
class Matter_Device
  var sessions                # Matter_Session_Store
  var message_handler         # Matter_MessageHandler
  var commissioning           # Matter_Commissioning
  var udp_server             # Matter_UDPServer
  var plugins                # List of endpoint plugins
```

### 2. Matter_Session_Store (Matter_Session_Store.be)
Manages all sessions and fabrics:
- Persists fabrics to `/_matter_fabrics.json`
- Creates and removes sessions
- Finds sessions by local_session_id or source_node_id
- Manages fabric lifecycle

```berry
class Matter_Session_Store
  var sessions               # matter.Expirable_list of sessions
  var fabrics                # matter.Expirable_list of fabrics
  
  def create_session(local_session_id, initiator_session_id)
  def get_session_by_local_session_id(id)
  def find_session_by_resumption_id(resumption_id)
  def add_fabric(fabric)
  def remove_fabric(fabric)
  def save_fabrics()
  def load_fabrics()
```

### 3. Matter_Fabric (Matter_Fabric.be)
Represents a commissioned fabric:
- Stores cryptographic material (NOC, ICAC, root CA, IPK)
- Contains device identity within the fabric
- Manages group message counters
- Holds list of active CASE sessions

```berry
class Matter_Fabric : Matter_Expirable
  var fabric_id              # bytes(8) - fabric identifier
  var device_id              # bytes(8) - our device ID in this fabric
  var fabric_compressed      # compressed fabric ID
  var root_ca_certificate    # Root CA certificate
  var noc                    # Node Operational Certificate
  var icac                   # Intermediate CA Certificate
  var ipk_epoch_key          # IPK for group key derivation
  var no_private_key         # Device's private key
  var _sessions              # List of CASE sessions
```

### 4. Matter_Session (Matter_Session.be)
Represents a single secure session:
- Stores session encryption keys
- Manages message counters (send/receive)
- Tracks session mode (PASE=1, CASE=2)
- Handles session lifecycle

```berry
class Matter_Session : Matter_Expirable
  static var _PASE = 1       # PASE mode
  static var _CASE = 2       # CASE mode
  
  var mode                   # Current session mode
  var local_session_id       # Our session ID
  var initiator_session_id   # Controller's session ID
  var i2rkey                 # Initiator to Responder key
  var r2ikey                 # Responder to Initiator key
  var attestation_challenge  # For attestation
  var counter_rcv            # Receive counter
  var counter_snd            # Send counter
  var resumption_id          # For CASE resumption
  var shared_secret          # ECDH shared secret
  var _fabric                # Reference to fabric
```

### 5. Matter_MessageHandler (Matter_MessageHandler.be)
Central message dispatcher:
- Receives raw UDP packets
- Decodes message headers
- Routes to appropriate handler (commissioning, IM, control)
- Manages encryption/decryption

```berry
class Matter_MessageHandler
  var commissioning          # Matter_Commisioning_Context
  var im                     # Matter_IM (Interaction Model)
  var control_message        # Matter_Control_Message
  
  def msg_received(raw, addr, port)
  def send_response_frame(msg)
```

### 6. Matter_Commisioning_Context (Matter_Commissioning_Context.be)
Handles PASE and CASE protocol exchanges:
- Processes PBKDF parameter exchange
- Implements SPAKE2+ for PASE
- Implements Sigma protocol for CASE
- Derives session keys

```berry
class Matter_Commisioning_Context
  def process_incoming(msg)
  def parse_PBKDFParamRequest(msg)
  def parse_Pake1(msg)
  def parse_Pake3(msg)
  def parse_Sigma1(msg)
  def parse_Sigma3(msg)
```

### 7. Matter_Frame (Matter_Message.be)
Message encoding/decoding:
- Parses Matter message headers
- Handles payload encryption/decryption
- Builds response frames

```berry
class Matter_Frame
  var session                # Associated session
  var raw                    # Raw message bytes
  var local_session_id       # Session ID from header
  var message_counter        # Message counter
  var opcode                 # Protocol opcode
  var exchange_id            # Exchange identifier
  
  def decode_header()
  def decode_payload()
  def encode_frame(payload)
  def encrypt()
  def decrypt()
```

## Session Establishment Workflows

### PASE (Passcode-Authenticated Session Establishment)

PASE is used during initial device commissioning when the controller knows the device's passcode.

```
Controller                                    Device
    │                                            │
    │  ──── PBKDFParamRequest (0x20) ────────►  │
    │       - initiatorRandom                    │
    │       - initiator_session_id               │
    │       - passcodeId                         │
    │                                            │
    │  ◄──── PBKDFParamResponse (0x21) ───────  │
    │       - initiatorRandom (echo)             │
    │       - responderRandom                    │
    │       - responderSessionId                 │
    │       - pbkdf_parameters (salt, iterations)│
    │                                            │
    │  ──── Pake1 (0x22) ─────────────────────► │
    │       - pA (SPAKE2+ public value)          │
    │                                            │
    │  ◄──── Pake2 (0x23) ────────────────────  │
    │       - pB (SPAKE2+ public value)          │
    │       - cB (SPAKE2+ confirmation)          │
    │                                            │
    │  ──── Pake3 (0x24) ─────────────────────► │
    │       - cA (SPAKE2+ confirmation)          │
    │                                            │
    │  ◄──── StatusReport (0x40) ─────────────  │
    │       - SUCCESS                            │
    │                                            │
    │  ═══════ Session Established ═══════════  │
```

#### PASE Implementation Details

1. **PBKDFParamRequest Processing** (`parse_PBKDFParamRequest`):
   - Validates message format
   - Records initiator_session_id
   - Generates local session ID
   - Returns PBKDF parameters (salt, iterations)

2. **Pake1 Processing** (`parse_Pake1`):
   - Receives initiator's SPAKE2+ public value (pA)
   - Computes responder's public value (pB) using SPAKE2+
   - Computes shared secret (Z, V)
   - Derives confirmation values (cA, cB)
   - Stores cA for verification in Pake3

3. **Pake3 Processing** (`parse_Pake3`):
   - Verifies initiator's confirmation (cA)
   - Derives session keys using HKDF:
     - I2RKey (Initiator to Responder)
     - R2IKey (Responder to Initiator)
     - AttestationChallenge
   - Creates new session with derived keys

### CASE (Certificate-Authenticated Session Establishment)

CASE is used for subsequent connections after commissioning, using certificates.

```
Controller                                    Device
    │                                            │
    │  ──── Sigma1 (0x30) ────────────────────► │
    │       - initiatorRandom                    │
    │       - initiator_session_id               │
    │       - destinationId                      │
    │       - initiatorEphPubKey                 │
    │       - [resumptionID] (optional)          │
    │       - [initiatorResumeMIC] (optional)    │
    │                                            │
    │  ◄──── Sigma2 (0x31) ───────────────────  │
    │       - responderRandom                    │
    │       - responderSessionId                 │
    │       - responderEphPubKey                 │
    │       - encrypted2 (TBEData2)              │
    │                                            │
    │  ──── Sigma3 (0x32) ────────────────────► │
    │       - TBEData3Encrypted                  │
    │                                            │
    │  ◄──── StatusReport (0x40) ─────────────  │
    │       - SUCCESS                            │
    │                                            │
    │  ═══════ Session Established ═══════════  │
```

#### CASE Resumption Flow

If the controller has a valid resumption_id from a previous session:

```
Controller                                    Device
    │                                            │
    │  ──── Sigma1 (0x30) ────────────────────► │
    │       - initiatorRandom                    │
    │       - resumptionID                       │
    │       - initiatorResumeMIC                 │
    │                                            │
    │  ◄──── Sigma2Resume (0x33) ─────────────  │
    │       - resumptionID (new)                 │
    │       - sigma2ResumeMIC                    │
    │       - responderSessionID                 │
    │                                            │
    │  ═══════ Session Resumed ═══════════════  │
```

#### CASE Implementation Details

1. **Sigma1 Processing** (`parse_Sigma1`):
   - Checks for resumption attempt first
   - If resumption: validates resumption MIC, derives new keys
   - If new session: finds fabric by destinationId
   - Generates ephemeral key pair
   - Computes ECDH shared secret

2. **Sigma2 Generation**:
   - Creates TBSData2 (To Be Signed Data):
     - NOC, ICAC, responder public key, initiator public key
   - Signs with device's private key
   - Creates TBEData2 (To Be Encrypted Data):
     - NOC, ICAC, signature, resumption_id
   - Encrypts with S2K (derived from shared secret)

3. **Sigma3 Processing** (`parse_Sigma3`):
   - Decrypts TBEData3 with S3K
   - Verifies initiator's NOC signature
   - Derives final session keys using HKDF:
     - I2RKey, R2IKey, AttestationChallenge
   - Persists session to fabric

## Key Derivation

### PASE Key Derivation

```
passcode (4 bytes)
    │
    ▼
┌─────────────────────────────────────────┐
│  PBKDF2-HMAC-SHA256                     │
│  - salt: commissioning_salt (16 bytes)  │
│  - iterations: 1000 (default)           │
│  - output: 80 bytes                     │
└─────────────────────────────────────────┘
    │
    ├── w0s (40 bytes) ──► w0 = mod(w0s, P256_order)
    │
    └── w1s (40 bytes) ──► w1 = mod(w1s, P256_order)
                              │
                              ▼
                          L = w1 * G (public key)
```

### CASE Key Derivation

```
ECDH(responder_priv, initiator_pub) = shared_secret
    │
    ▼
┌─────────────────────────────────────────┐
│  HKDF-SHA256                            │
│  - ikm: shared_secret                   │
│  - salt: IPK_group_key + TranscriptHash │
│  - info: "SessionKeys"                  │
│  - output: 48 bytes                     │
└─────────────────────────────────────────┘
    │
    ├── I2RKey (16 bytes) - Initiator to Responder
    ├── R2IKey (16 bytes) - Responder to Initiator
    └── AttestationChallenge (16 bytes)
```

## Message Encryption

All encrypted messages use AES-CCM with:
- Key: I2RKey (incoming) or R2IKey (outgoing)
- Nonce: sec_flags (1) + message_counter (4) + source_node_id (8) = 13 bytes
- AAD: Message header (up to payload_idx)
- Tag length: 16 bytes

```berry
# Decryption (Matter_Frame.decrypt)
def decrypt()
  var n = bytes(13)
  n.add(self.sec_flags, 1)
  n.add(self.message_counter, 4)
  n .. self.source_node_id  # or peer_node_id
  
  crypto.AES_CCM.decrypt1(i2r, n, header, payload, tag)
end
```

## Session Lifecycle

```
                    ┌─────────────────┐
                    │   No Session    │
                    └────────┬────────┘
                             │
              ┌──────────────┴──────────────┐
              │                             │
              ▼                             ▼
    ┌─────────────────┐           ┌─────────────────┐
    │  PASE Session   │           │  CASE Session   │
    │  (mode = 1)     │           │  (mode = 2)     │
    │  - Temporary    │           │  - Persistent   │
    │  - No fabric    │           │  - Has fabric   │
    └────────┬────────┘           └────────┬────────┘
             │                             │
             │ Commissioning               │ Session
             │ Complete                    │ Established
             │                             │
             ▼                             ▼
    ┌─────────────────┐           ┌─────────────────┐
    │  Fabric Created │           │  Session Active │
    │  - NOC assigned │           │  - Encrypted    │
    │  - IPK set      │           │  - Resumable    │
    └────────┬────────┘           └─────────────────┘
             │
             │ CASE
             │ Established
             ▼
    ┌─────────────────┐
    │  CASE Session   │
    │  - Persistent   │
    │  - Resumable    │
    └─────────────────┘
```

## Persistence

### Fabric Storage (`/_matter_fabrics.json`)

```json
[
  {
    "fabric_index": 1,
    "fabric_id": "$base64_encoded",
    "device_id": "$base64_encoded",
    "fabric_compressed": "$base64_encoded",
    "root_ca_certificate": "$base64_encoded",
    "noc": "$base64_encoded",
    "icac": "$base64_encoded",
    "ipk_epoch_key": "$base64_encoded",
    "no_private_key": "$base64_encoded",
    "fabric_label": "Home",
    "admin_subject": 12345,
    "admin_vendor": 4996,
    "_sessions": [
      {
        "local_session_id": 1234,
        "initiator_session_id": 5678,
        "counter_rcv": 100,
        "counter_snd": 200,
        "i2rkey": "$base64_encoded",
        "r2ikey": "$base64_encoded",
        "resumption_id": "$base64_encoded",
        "shared_secret": "$base64_encoded"
      }
    ]
  }
]
```

## Security Considerations

1. **Counter Management**: Message counters prevent replay attacks
   - Counters are persisted with a safety margin (`_COUNTER_SND_INCR = 1024`)
   - Receive counters validate incoming messages

2. **Session Expiration**: Sessions can expire
   - PASE sessions expire after commissioning window closes
   - CASE sessions persist but can be removed

3. **Key Isolation**: Each session has unique keys
   - I2RKey and R2IKey are derived per-session
   - Privacy key derived from I2RKey for header encryption

4. **Fabric Isolation**: Each fabric has separate cryptographic material
   - Different NOC, IPK per fabric
   - Sessions are bound to specific fabrics

## Known Discrepancies from Matter 1.0 Specification

This section documents known deviations from the Matter 1.0 specification found in the Tasmota implementation.

### 1. SPAKE2+ Context Prefix String (CRITICAL)

**Location**: `Matter_Commissioning_Context.be`, line 29

**Spec says**: `"Matter PAKE V1 Commissioning"`

**Implementation uses**: `"CHIP PAKE V1 Commissioning"`

```berry
# static Matter_Context_Prefix = "Matter PAKE V1 Commissioning" # spec is wrong
  static var Matter_Context_Prefix = "CHIP PAKE V1 Commissioning" # from CHIP code
```

**Reason**: The Matter specification contains an error. The actual CHIP/Matter SDK implementation uses `"CHIP PAKE V1 Commissioning"` as the context prefix for SPAKE2+ transcript hashing. Using the spec-documented string would cause interoperability failures with all Matter controllers.

**Impact**: None - this is the correct behavior for interoperability.

---

### 2. Sigma3 Signature Verification (FIXED)

**Location**: `Matter_Commissioning_Context.be`, lines 640-645

**Spec says**: Sigma3 TBSData signature MUST be verified, and session establishment MUST fail if invalid.

**Implementation**: Now correctly rejects invalid signatures:

```berry
if !sigma3_tbs_valid
  log("MTR: sigma3_tbs does not have a valid signature", 2)
  log("MTR: StatusReport(General Code: FAILURE, ProtocolId: SECURE_CHANNEL, ProtocolCode: INVALID_PARAMETER)", 2)
  self.send_status_report(msg, 0x01, 0x0000, 0x0002, false)
  return false
end
log("MTR: Sigma3 verified, computing new keys", 3)
```

**Status**: COMPLIANT - The implementation now properly validates the Sigma3 TBSData signature and rejects sessions with invalid signatures by sending a StatusReport with INVALID_PARAMETER error code.

**Impact**: None - fully compliant with Matter 1.0 specification.

---

### 3. PBKDF2 Iterations (COMPLIANT but minimal)

**Location**: `Matter_z_Commissioning.be`, line 27

**Spec says**: Minimum 1000 iterations, recommended higher for security.

**Implementation uses**: Exactly 1000 iterations (the minimum)

```berry
static var PBKDF_ITERATIONS = 1000  # I don't see any reason to choose a different number
```

**Impact**: Compliant but uses minimum security. Higher iteration counts would provide better protection against brute-force attacks on the passcode.

---

### 4. Message Counter Window Size (COMPLIANT)

**Location**: `be_matter_counter.cpp`, line 34

**Spec says**: Window size of 32 for message counter validation (Section 4.5.4)

**Implementation uses**: 32

```cpp
#define MESSAGE_COUNTER_WINDOW_SIZE   32
```

**Impact**: None - fully compliant.

---

### 5. Counter Rollover Handling (SIMPLIFIED)

**Location**: `be_matter_counter.cpp`, line 133

**Spec says**: Complex rollover handling with specific rules for counter wrap-around.

**Implementation does**: Simplified handling that ignores rollover:

```cpp
// in the past and out of the window (we ignore roll-over to simplify)
if (strict_mode) {
  return bfalse;
} else {
  c->counter = new_val;
  c->window.reset();
  return btrue;
}
```

**Impact**: Minor - could theoretically cause issues after 2^32 messages, but practically unlikely in embedded device scenarios.

---

### 6. TLV Encoding Type Selection

**Location**: Various files using `TLV.B1` vs `TLV.B2`

**Observation**: The implementation uses both `B1` (1-byte length) and `B2` (2-byte length) for octet strings. The choice appears to be based on expected data size rather than strict spec requirements.

**Impact**: None - TLV encoding is flexible and both are valid.

---

### 7. Session Resumption ID Generation

**Location**: `Matter_Commissioning_Context.be`

**Spec says**: Resumption ID should be cryptographically random 16 bytes.

**Implementation**: Uses `crypto.random(16)` which is compliant.

**Impact**: None - compliant.

---

## Recommendations

1. **MEDIUM PRIORITY**: Consider increasing PBKDF2 iterations above the minimum 1000 for better security, especially for devices that may be exposed to network attacks.

2. **LOW PRIORITY**: The CHIP context prefix discrepancy is actually correct behavior - the spec documentation is wrong. Consider adding a comment explaining this more clearly.

---
*Documentation generated: January 2026*
*Based on Tasmota Matter Berry implementation analysis*
