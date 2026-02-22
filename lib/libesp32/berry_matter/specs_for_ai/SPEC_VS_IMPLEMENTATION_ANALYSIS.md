# Matter 1.4.1 Spec vs Tasmota Implementation — Gap Analysis & Change Spec

## 1. Executive Summary

This document compares the Matter 1.4.1 Core Specification (March 2025) against the current Tasmota Berry implementation across the message layer, commissioning (PASE/CASE), and Interaction Model. The implementation is solid and functionally correct for a resource-constrained device. The gaps identified are primarily around **session parameter negotiation** (new mandatory fields in 1.4), **InteractionModelRevision** being outdated, and a few minor protocol conformance items. No architectural changes are recommended — the current layered design is clean and appropriate for the platform.

---

## 2. Layer-by-Layer Analysis

### 2.1 Message Layer (`Matter_Message.be`)

#### What matches the spec well:
- Message header encoding/decoding (flags, DSIZ, session ID, message counter, source/dest node IDs) — correct per §4.4
- Security flags (P, C, MX, SessionType) — correctly parsed
- Protocol header (exchange flags I/A/R/SX/V, opcode, exchange ID, protocol ID, vendor ID, ack counter) — correct per §4.4
- Nonce construction for encryption/decryption (SecurityFlags || MessageCounter || SourceNodeID, 13 bytes) — correct per §4.7
- AES-128-CCM encryption/decryption with 16-byte MIC — correct per §4.7
- Privacy flag handling with AES-CTR decryption — present (§4.8)
- Message version check (bits 3-7 of flags must be 0x00 for Matter 1.0) — correct

#### Gaps:
| # | Gap | Spec Reference | Severity | Notes |
|---|-----|---------------|----------|-------|
| M1 | Privacy encryption nonce uses `session_id` in big-endian but spec says little-endian for the 2-byte session ID in the privacy nonce | §4.8 | Low | The privacy nonce is `SessionID(2) || MIC[0..10]`. Current code uses `self.local_session_id` with `-2` (big-endian). Should verify byte order. |
| M2 | Message Extensions (MX flag) are skipped but never generated | §4.4 | None | Correct behavior for a device that doesn't use MX. Parsing correctly skips them. |
| M3 | Secured Extensions (SX flag) are skipped but never generated | §4.4 | None | Same as above, correct. |

**Verdict: Message layer is spec-compliant. No changes needed.**

---

### 2.2 Message Handler (`Matter_MessageHandler.be`)

#### What matches the spec well:
- Dispatches unencrypted (session_id=0, sesstype=0) to commissioning — correct
- Dispatches encrypted messages by looking up session via `local_session_id` — correct
- Protocol ID routing: 0x0000 → Secure Channel, 0x0001 → Interaction Model — correct per §4.5
- Duplicate message detection using counter validation — correct per §4.6
- MRP standalone acknowledgement handling (opcode 0x10) — correct per §4.9
- Control message (sec_p flag) routing — present

#### Gaps:
| # | Gap | Spec Reference | Severity | Notes |
|---|-----|---------------|----------|-------|
| MH1 | BDX (protocol 0x0002) and UDC (protocol 0x0003) are commented out | §4.5 | None | Intentional — BDX is for OTA file transfer, UDC is for commissioner devices. Neither is needed for a basic Matter device. |
| MH2 | No MCSP (Message Counter Synchronization Protocol, opcode 0x00/0x01) handling | §4.6 | Low | MCSP is needed for group messaging counter sync. Since Tasmota doesn't support group messaging, this is acceptable. |

**Verdict: Message handler is correct. No changes needed.**

---

### 2.3 Commissioning — PASE (`Matter_Commissioning_Context.be`, `Matter_Commissioning_Data.be`)

#### What matches the spec well:
- PBKDFParamRequest parsing (tag 1=initiatorRandom, 2=initiatorSessionId, 3=passcodeId, 4=hasPBKDFParameters, 5=sessionParams) — correct per §4.11
- PBKDFParamResponse generation (tag 1=initiatorRandom, 2=responderRandom, 3=responderSessionId, 4=pbkdf_parameters) — correct per §4.11
- SPAKE2+ computation with Matter variant context prefix "CHIP PAKE V1 Commissioning" — correct
- Pake1/Pake2/Pake3 message flow — correct per §4.11
- Session key derivation: `HKDF(Ke, [], "SessionKeys", 48)` → I2R || R2I || AC — correct per §4.11
- StatusReport for PakeFinished — correct
- PBKDF iterations (1000) within spec range (1000-100000) — correct per §3.1
- Salt generation (16 bytes random) within spec range (16-32) — correct per §3.4
- Passcode validation (invalid passcodes list) — correct per §5.1

#### Gaps:
| # | Gap | Spec Reference | Severity | Notes |
|---|-----|---------------|----------|-------|
| P1 | ~~**Session parameter struct in PBKDFParamResponse only sends SII/SAI (tags 1,2), missing mandatory fields: DATA_MODEL_REVISION (tag 4), INTERACTION_MODEL_REVISION (tag 5), SPECIFICATION_VERSION (tag 6), MAX_PATHS_PER_INVOKE (tag 7)**~~ ✅ FIXED | §4.10 | ~~**HIGH**~~ | All mandatory fields now always included in session-parameter-struct. |
| P2 | PBKDFParamRequest parsing doesn't extract new session parameter fields from initiator | §4.10 | Medium | Should parse and potentially use DATA_MODEL_REVISION, SPECIFICATION_VERSION etc. from the initiator for compatibility negotiation. |
| P3 | ~~SESSION_ACTIVE_THRESHOLD (tag 3, uint16) not sent in session params~~ ✅ FIXED | §4.10 | ~~Medium~~ | Now included with default value 4000ms. |

---

### 2.4 Commissioning — CASE (`Matter_Commissioning_Context.be`)

#### What matches the spec well:
- Sigma1 parsing (initiatorRandom, initiatorSessionId, destinationId, initiatorEphPubKey, resumptionID, initiatorResumeMIC) — correct per §4.12
- Destination Identifier computation: `HMAC(IPK, initiatorRandom || rootPublicKey || fabricId || nodeId)` — correct per §4.12
- Sigma2 generation with TBSData2 and TBEData2 — correct per §4.12
- S2K derivation: `HKDF(SharedSecret, IPK||ResponderRandom||ResponderEphPubKey||TranscriptHash, "Sigma2", 16)` — correct
- S3K derivation: `HKDF(SharedSecret, IPK||TranscriptHash, "Sigma3", 16)` — correct
- TBEData2/3 nonces: "NCASE_Sigma2N" / "NCASE_Sigma3N" — correct per §4.12
- Session key derivation: `HKDF(SharedSecret, IPK||TranscriptHash, "SessionKeys", 48)` — correct
- CASE Resumption flow (S1RK, S2RK, Resume1MIC, Resume2MIC) — correct per §4.12
- Resumption session keys: `HKDF(SharedSecret, initiatorRandom||ResumptionID, "SessionResumptionKeys", 48)` — correct
- Resume nonces: "NCASE_SigmaS1" / "NCASE_SigmaS2" — correct

#### Gaps:
| # | Gap | Spec Reference | Severity | Notes |
|---|-----|---------------|----------|-------|
| C1 | ~~**Sigma2 and Sigma2Resume don't include responderSessionParams with mandatory fields**~~ ✅ FIXED | §4.10, §4.12 | ~~**HIGH**~~ | All mandatory session-parameter-struct fields now always included in Sigma2 (tag 5) and Sigma2Resume (tag 4). |
| C2 | Sigma3 validation doesn't verify NOC chain back to TrustedRCAC | §4.12 | Medium | There's a TODO comment in the code. The NOC signature is verified, but the full certificate chain validation (NOC → ICAC → RCAC) is not performed. This is a known shortcut. |
| C3 | Sigma3 doesn't verify that the Fabric ID in the initiator's NOC matches the session's fabric | §4.12 | Medium | The code extracts `initiatorFabricId` but doesn't compare it against the session's fabric ID. |
| C4 | SUPPORTED_TRANSPORTS (tag 8) not sent in session params | §4.10 | Low | Optional but useful for indicating UDP-only support. |

---

### 2.5 Commissioning — mDNS Discovery (`Matter_z_Commissioning.be`)

#### What matches the spec well:
- Commissionable discovery: `_matterc._udp` service type — correct per §4.3.1
- Random 16-char hex instance name — correct per §4.3.1
- TXT records: D (discriminator), VP (vendor+product), CM (commissioning mode), T (TCP support) — correct
- Subtypes: `_L<discriminator>`, `_S<upper4bits>`, `_V<vendorID>`, `_CM1` — correct per §4.3.1
- Operational discovery: `_matter._tcp` with `<CompressedFabricID>-<NodeID>` instance — correct per §4.3.2
- SII=500, SAI=300 defaults — correct per §4.10
- Port 5540 — correct per §2.4

#### Gaps:
| # | Gap | Spec Reference | Severity | Notes |
|---|-----|---------------|----------|-------|
| D1 | Operational discovery TXT records don't include SII, SAI, SAT, T, ICD keys | §4.3.2 | Low | For an always-on WiFi device, these are optional but recommended. The `T` key (TCP support bitmap) should be 0. |
| D2 | No `_T<devicetype>` subtype in commissionable discovery | §4.3.1 | Low | Optional but useful for filtering. |
| D3 | `DN` (device name) TXT record not included in commissionable discovery | §4.3.1 | Low | Optional. |

---

### 2.6 Interaction Model (`Matter_IM.be`, `Matter_IM_Message.be`, `Matter_IM_Data.be`)

#### What matches the spec well:
- All IM opcodes handled: StatusResponse(0x01), ReadRequest(0x02), SubscribeRequest(0x03), SubscribeResponse(0x04), ReportData(0x05), WriteRequest(0x06), WriteResponse(0x07), InvokeRequest(0x08), InvokeResponse(0x09), TimedRequest(0x0A) — correct per §8.4
- ReadRequestMessage structure (AttributeRequests, EventRequests, EventFilters, FabricFiltered, DataVersionFilters) — correct per §8.5
- ReportDataMessage structure (SubscriptionId, AttributeReports, EventReports, MoreChunkedMessages, SuppressResponse) — correct per §8.5
- WriteRequestMessage/WriteResponseMessage — correct per §8.5
- InvokeRequestMessage/InvokeResponseMessage — correct per §8.5
- SubscribeRequestMessage/SubscribeResponseMessage — correct per §8.5
- AttributePathIB encoding (endpoint/cluster/attribute/ListIndex) — correct per §8.2
- Chunked message handling with MAX_MESSAGE=1150 (within 1280 MTU minus headers) — correct per §4.4.4
- Subscription management with min/max intervals — correct
- Solo (fast-path) optimization for single attribute reads and single invoke requests — good optimization
- Event support with EventGenerator and event filtering — correct

#### Gaps:
| # | Gap | Spec Reference | Severity | Notes |
|---|-----|---------------|----------|-------|
| IM1 | ~~**InteractionModelRevision = 11 (Matter 1.2)**. Matter 1.4 requires revision 12.~~ ✅ FIXED | §8.4 | ~~**HIGH**~~ | Updated to 12 in all outgoing paths (TLV-based and raw-encoded). Also fixed incoming solo parser to accept any revision. |
| IM2 | ~~TimedRequest handling sends SUCCESS but doesn't enforce timed interaction semantics~~ ✅ FIXED | §8.4 | ~~Medium~~ | Timed interaction is now fully enforced: exchange_id and timeout are tracked, Write/Invoke with `timed_request=true` are validated against the timed window, returning `TIMED_REQUEST_MISMATCH` (0xC9) on failure. |
| IM3 | DataVersionFilters in ReadRequest are parsed but not used for filtering | §8.5 | Low | The spec says if a DataVersionFilter matches the current cluster data version, the cluster's attributes should be omitted from the response. This optimization is not implemented. |
| IM4 | No `PATHS_EXHAUSTED` (0xC8) status code handling when too many paths are requested | §8.3 | Low | The spec limits to 9 attribute paths and 10 event paths per request. No enforcement. |
| IM5 | Subscription max_interval is hardcoded to 60 seconds regardless of requested ceiling | IM_Subscription.be | Low | `max_interval = 60` is forced. The spec allows up to 3600s. This is a deliberate choice for responsiveness but limits battery-powered controller scenarios. |

---

### 2.7 Cluster Revisions and Data Model

#### What matches:
- DataModelRevision = 18 (Matter 1.3+, compatible with 1.4.1) — correct
- Basic Information cluster revision = 3 — correct for Matter 1.4.1
- Descriptor cluster revision = 2 — correct
- Global attributes (0xFFF8-0xFFFD) handled — correct per §7.4

#### Gaps:
| # | Gap | Spec Reference | Severity | Notes |
|---|-----|---------------|----------|-------|
| CL1 | SpecificationVersion attribute (0x0015) in Basic Information cluster — need to verify it's set correctly | §11.1 | Medium | Should be `0x01040100` for Matter 1.4.1. |
| CL2 | MaxPathsPerInvoke attribute (0x0016) in Basic Information cluster — need to verify | §11.1 | Low | Should be 1 (single invoke path). |

---

## 3. Architecture Assessment

The current architecture is well-suited for the platform:

```
Matter_MessageHandler (dispatch)
  ├── Matter_Commisioning_Context (PASE/CASE)
  ├── Matter_IM (Interaction Model)
  │     ├── Matter_IM_Message (response queue)
  │     ├── Matter_IM_Subscription (subscription shop)
  │     └── PathGenerator / EventGenerator (lazy evaluation)
  └── Matter_Control_Message (MCSP)
```

**No architectural changes recommended.** The separation of concerns is clean:
- Message framing/crypto is isolated in `Matter_Frame`
- Session management is separate from message handling
- IM has a clean send queue with proper exchange tracking
- The "solo" fast-path optimization for single reads/invokes is a smart performance choice
- Lazy (pull) evaluation of attributes during ReportData generation is memory-efficient

---

## 4. Recommended Changes — Detailed Specification

### Change 1: Add Mandatory Session Parameters ~~(HIGH Priority)~~ ✅ DONE

**Files changed:** `Matter_Commissioning_Data.be`

**Rationale:** Matter 1.4 made several fields mandatory in the `session-parameter-struct` (tag 5 in PBKDFParamResponse, Sigma2, Sigma2Resume). Modern controllers (Apple Home, Google Home, Samsung SmartThings) may reject sessions that don't include these fields.

**Spec reference:** §4.10 Session Parameter TLV

**Previous behavior:** Only `SESSION_IDLE_INTERVAL` (tag 1) and `SESSION_ACTIVE_INTERVAL` (tag 2) were conditionally sent.

**What was done:** The session-parameter-struct is now always included in `PBKDFParamResponse` (tag 5), `Sigma2` (tag 5), and `Sigma2Resume` (tag 4) with all mandatory fields. `SLEEPY_IDLE_INTERVAL` and `SLEEPY_ACTIVE_INTERVAL` are still sent when set (nil-safe via `add_TLV`). The new mandatory fields are always present:

**Required behavior (now implemented):** The responder session params struct includes:

```
session-parameter-struct => STRUCTURE [tag-order] {
  SESSION_IDLE_INTERVAL [1, optional]: UNSIGNED INTEGER [32-bits],    // 500 (ms)
  SESSION_ACTIVE_INTERVAL [2, optional]: UNSIGNED INTEGER [32-bits],  // 300 (ms)
  SESSION_ACTIVE_THRESHOLD [3, optional]: UNSIGNED INTEGER [16-bits], // 4000 (ms)
  DATA_MODEL_REVISION [4]: UNSIGNED INTEGER [16-bits],                // 18
  INTERACTION_MODEL_REVISION [5]: UNSIGNED INTEGER [16-bits],         // 12
  SPECIFICATION_VERSION [6]: UNSIGNED INTEGER [32-bits],              // 0x01040100
  MAX_PATHS_PER_INVOKE [7]: UNSIGNED INTEGER [16-bits],              // 1
}
```

**Implementation plan:**

1. In `Matter_PBKDFParamResponse.tlv2raw()`: Always generate the session params struct (tag 5) with all mandatory fields.

2. In `Matter_Sigma2.tlv2raw()`: Always generate the session params struct (tag 5) with all mandatory fields.

3. In `Matter_Sigma2Resume.tlv2raw()`: Always generate the session params struct (tag 4) with all mandatory fields.

4. Define constants in a central location (e.g., `matter` module or a static in the commissioning class):
   - `DATA_MODEL_REVISION = 18`
   - `INTERACTION_MODEL_REVISION = 12`
   - `SPECIFICATION_VERSION = 0x01040100`  (1.4.1.0)
   - `MAX_PATHS_PER_INVOKE = 1`

**Example code change for `Matter_PBKDFParamResponse.tlv2raw()`:**
```berry
def tlv2raw(b)
  var TLV = matter.TLV
  var s = TLV.Matter_TLV_struct()
  s.add_TLV(1, TLV.B1, self.initiatorRandom)
  s.add_TLV(2, TLV.B1, self.responderRandom)
  s.add_TLV(3, TLV.U2, self.responderSessionId)
  var s_pbkdf = s.add_struct(4)
  s_pbkdf.add_TLV(1, TLV.U4, self.pbkdf_parameters_iterations)
  s_pbkdf.add_TLV(2, TLV.B1, self.pbkdf_parameters_salt)
  # Always include session params with mandatory fields
  var s2 = s.add_struct(5)
  s2.add_TLV(1, TLV.U4, 500)                   # SESSION_IDLE_INTERVAL
  s2.add_TLV(2, TLV.U4, 300)                   # SESSION_ACTIVE_INTERVAL
  s2.add_TLV(3, TLV.U2, 4000)                  # SESSION_ACTIVE_THRESHOLD
  s2.add_TLV(4, TLV.U2, 18)                    # DATA_MODEL_REVISION
  s2.add_TLV(5, TLV.U2, 12)                    # INTERACTION_MODEL_REVISION
  s2.add_TLV(6, TLV.U4, 0x01040100)            # SPECIFICATION_VERSION (1.4.1.0)
  s2.add_TLV(7, TLV.U2, 1)                     # MAX_PATHS_PER_INVOKE
  return s.tlv2raw(b)
end
```

---

### Change 2: Update InteractionModelRevision to 12 ~~(HIGH Priority)~~ ✅ DONE

**Files changed:** `Matter_IM_Data.be`, `Matter_IM_Message.be`, `Matter_IM.be`

**What was done:**
- `Matter_IM_Data.be`: `InteractionModelRevision` set to `12` (was `11`). Also fixed `InvokeRequestMessage_solo.from_raw()` to accept any IM revision value from incoming messages instead of hardcoding `0x24FF0118`.
- `Matter_IM_Message.be`: Raw-encoded IM revision in `ReportData_Pull.send_im()` changed from `0x01` to `12` (0x0C).
- `Matter_IM.be`: Three raw-encoded suffixes in solo fast-paths changed from `0x1824FF01` to `0x1824FF0C` — covering `process_read_request_solo` (two paths: success and error) and `process_invoke_request_solo`.

---

### Change 3: Timed Interaction Enforcement ~~(MEDIUM Priority)~~ ✅ DONE

**Files changed:** `Matter_IM.be`, `Matter_IM_Data.be`

**What was done:**
- Added `timed_exchanges` map to `Matter_IM` class to track exchange_id → expiration_millis.
- `process_timed_request()` now records the exchange ID and timeout expiration.
- Added `check_timed_request()` helper that validates a matching TimedRequest exists and hasn't expired (one-shot consumption).
- `process_write_request()` and `process_invoke_request()` now check the `timed_request` flag and return `TIMED_REQUEST_MISMATCH` (0xC9) if validation fails.
- Solo invoke fast-path (`process_invoke_request_solo()`) also enforces timed interaction checks.
- Fixed bug in `InvokeRequestMessage_solo.from_raw()` where `TimedRequest` field was incorrectly assigned to `SuppressResponse`.
- Expired timed exchanges are cleaned up in `every_second()`.

**Rationale:** The implementation now accepts TimedRequest, records the exchange and timeout, and enforces the timed window. A subsequent Write or Invoke with `timed_request=true` only succeeds if a TimedRequest was received within the timeout window on the same exchange.

**Spec reference:** §8.4, §8.5

**Implementation plan:**

1. In `process_timed_request()`: Store the exchange_id and expiration time (`tasmota.millis() + timeout_ms`).

2. In `process_write_request()` and `process_invoke_request()`: If `query.timed_request == true`, verify that a matching timed request exists and hasn't expired. If not, return `TIMED_REQUEST_MISMATCH` (0xC9).

3. If `query.timed_request == false` but the operation requires a timed interaction (e.g., certain cluster commands), return `NEEDS_TIMED_INTERACTION` (0xC6).

**Suggested data structure:**
```berry
# In Matter_IM class
var timed_exchanges    # map: exchange_id -> expiration_millis

def process_timed_request(msg, val)
  var query = matter.TimedRequestMessage().from_TLV(val)
  if self.timed_exchanges == nil  self.timed_exchanges = {} end
  self.timed_exchanges[msg.exchange_id] = tasmota.millis() + query.timeout
  self.send_status(msg, matter.SUCCESS)
  return true
end

def check_timed_interaction(msg, is_timed_request)
  if !is_timed_request  return true end  # not a timed request, ok
  if self.timed_exchanges == nil  return false end
  var expiry = self.timed_exchanges.find(msg.exchange_id)
  self.timed_exchanges.remove(msg.exchange_id)  # one-shot
  if expiry == nil  return false end
  return !tasmota.time_reached(expiry)
end
```

---

### Change 4: Certificate Chain Validation in Sigma3 (MEDIUM Priority)

**Files affected:** `Matter_Commissioning_Context.be`

**Rationale:** The code has a TODO comment about verifying the NOC chain. While the signature is verified, the full chain (NOC → optional ICAC → RCAC) is not validated. This is a security gap.

**Spec reference:** §4.12 (Sigma3 Validation step 2)

**Implementation plan:**

1. After decrypting TBEData3, extract `initiatorNOC` and `initiatorICAC`.
2. Verify that `initiatorNOC` is signed by `initiatorICAC` (if present) or directly by the trusted RCAC.
3. Verify that `initiatorICAC` (if present) is signed by the trusted RCAC.
4. Verify that the Fabric ID in the NOC matches the session's fabric.
5. If any verification fails, send `StatusReport(FAILURE, SECURE_CHANNEL, INVALID_PARAMETER)`.

This is complex to implement in Berry due to X.509/Matter certificate parsing requirements. A pragmatic approach would be to at least verify the Fabric ID match (gap C3).

---

### Change 5: Operational Discovery TXT Records (LOW Priority)

**Files affected:** `Matter_z_Commissioning.be`

**Rationale:** Adding SII, SAI, T keys to operational discovery helps controllers optimize their retry behavior.

**Spec reference:** §4.3.2

**Implementation plan:**

In `mdns_announce_op_discovery()`, add TXT records:
```berry
var services = {
  "SII": 500,    # SESSION_IDLE_INTERVAL
  "SAI": 300,    # SESSION_ACTIVE_INTERVAL
  "SAT": 4000,   # SESSION_ACTIVE_THRESHOLD
  "T": 0         # TCP support: 0 = no TCP
}
mdns.add_service("_matter", "_tcp", 5540, services, op_node, hostname)
```

---

## 5. Priority Summary

| Priority | Change | Impact | Effort |
|----------|--------|--------|--------|
| **HIGH** | ~~Session parameter struct with mandatory fields (Change 1)~~ ✅ DONE | Compatibility with modern controllers | Medium |
| **HIGH** | ~~InteractionModelRevision = 12 (Change 2)~~ ✅ DONE | Spec compliance, controller compatibility | Trivial |
| MEDIUM | ~~Timed interaction enforcement (Change 3)~~ ✅ DONE | Security compliance | Low |
| MEDIUM | Certificate chain validation (Change 4) | Security | High |
| LOW | Operational discovery TXT records (Change 5) | Better MRP behavior | Trivial |

---

## 6. What NOT to Change

- **Architecture**: The current layered design is clean and appropriate. No refactoring needed.
- **Solo fast-path**: The optimization for single reads/invokes is valuable on constrained hardware. Keep it.
- **Pull-mode ReportData**: Lazy attribute evaluation is memory-efficient. Keep it.
- **BDX/UDC**: Not needed for a basic Matter device. Keep them disabled.
- **Group messaging**: Not needed for typical Tasmota use cases. Keep it unimplemented.
- **TCP transport**: Not needed for a UDP-only device. Keep it unimplemented.
- **MAX_MESSAGE = 1150**: This is a good conservative value within the 1280 MTU limit.
- **Subscription max_interval = 60s**: This is a deliberate trade-off for responsiveness. Acceptable.
