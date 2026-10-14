# Matter 1.6.1 Tasmota Profile and Groupcast Roadmap

Status: implementation profile and remaining-work guide
Last updated: 2026-09-29
Target: Tasmota Berry Matter implementation

## 1. Scope

The current work aligns core version metadata with Matter 1.6.1 and provides
the storage and management commands needed by Group Key Management.

Tasmota deliberately does not implement the full Matter Access Control engine.
The reason is practical: persistent ACL lists, CAT extraction, target matching,
list-write transactions, authorization checks on every Interaction Model path,
and ACL events add substantial code and state for little benefit in the common
Tasmota deployment model.

This document records that tradeoff so the simplified behavior is not mistaken
for a complete or certification-ready ACL implementation.

## 2. Fixed Access Control Policy

### 2.1 Exposed ACL

For an authenticated CASE session, the Access Control `ACL` attribute returns
one fabric-scoped entry:

```text
Privilege:   Administer
AuthMode:    CASE
Subjects:    null
Targets:     null
FabricIndex: accessing session's fabric
```

In Matter ACL semantics:

- `Subjects = null` matches every authenticated CASE subject on that fabric.
- `Targets = null` matches every endpoint, cluster, command, attribute, and
  event.
- `Administer` is the highest privilege.

The result is equivalent to trusting every commissioned node on a fabric as a
full administrator.

### 2.2 Implementation Behavior

- The wildcard entry is generated when read; it is not stored in fabric JSON.
- ACL writes return `SUCCESS` but are discarded for commissioner
  compatibility. Apple Home removes a newly commissioned fabric if its
  post-commissioning ACL write is rejected.
- `AccessControlEntriesPerFabric` reports `1`.
- `SubjectsPerAccessControlEntry` and `TargetsPerAccessControlEntry` remain
  readable because they are mandatory cluster attributes.
- No `AccessControlEntryChanged` event is emitted because the fixed entry
  cannot change.
- No CAT values are extracted from NOCs or stored in CASE sessions.
- No Node ID, CAT, target, privilege, or command-specific ACL matching occurs.
- Existing CASE session persistence and resumption remain unchanged.

The successful no-op write is an explicit interoperability compromise. It lets
Apple Home finish provisioning, but makes the controller believe restrictions
were installed when they were not. Reads continue to return the fixed wildcard
entry, and all authenticated CASE peers on the fabric remain administrators.

### 2.3 Security Tradeoff

This profile is acceptable only when every node admitted to a fabric is
trusted with full device control.

Any fabric member with valid CASE credentials can potentially:

- Read and write any exposed attribute.
- Invoke administrative or operational commands.
- Change network configuration.
- Open commissioning windows.
- Modify fabrics or operational credentials where the existing cluster
  implementation permits it.
- Configure Group Key Management data.

The policy does not grant access to devices outside the fabric. CASE
authentication still binds a session to a commissioned fabric, and the ACL
entry reports that fabric's `FabricIndex`.

### 2.4 Deliberate Matter Deviations

This profile does not implement:

- Writable per-fabric ACL lists.
- The normative minimum of four configurable ACL entries per fabric.
- Subject-specific Node ID restrictions.
- CASE Authenticated Tag matching or CAT version rules.
- Cluster, endpoint, or device-type targets.
- Group AuthMode ACL entries.
- Fabric-unfiltered ACL redaction rules.
- Access Control change events.
- ARL, AuxiliaryACL, or managed access restrictions.

Consequently, this profile must not claim full Access Control conformance or
Matter certification without a later standards review and implementation.

## 3. Current Group Key Management State

Implemented management-plane functionality:

- `GroupKeyMap`, `GroupTable`, `MaxGroupsPerFabric`, and
  `MaxGroupKeysPerFabric` attributes.
- Fabric-scoped persistence of:
  - GroupKeyMap associations.
  - Groups endpoint membership.
  - Group key sets and epoch start times.
- Mandatory commands:
  - `KeySetWrite`
  - `KeySetRead`
  - `KeySetRemove`
  - `KeySetReadAllIndices`
- Epoch key material is never returned by `KeySetRead`.
- Key length, epoch pairing, epoch ordering, policy, reserved-ID, and capacity
  validation.
- Removing a key set also removes GroupKeyMap associations that reference it.
- Atomic GroupKeyMap list writes, including ListIndex-null append and
  multi-message chunking.

## 4. Why Groups Remain Disabled

Group Key Management stores keys and associations; it does not receive or
decrypt multicast commands.

`Matter_Device.GROUP_TRANSPORT_READY` therefore remains `false` in production.
While it is false:

- `MaxGroupsPerFabric` reports zero.
- GroupKeyMap reads return an empty list.
- GroupKeyMap writes return `UNSUPPORTED_WRITE`.
- `AddGroup` fails.

This prevents a controller from provisioning a group that appears successful
but whose commands would never reach the device.

## 5. Required Group Message Data Plane

Before enabling `GROUP_TRANSPORT_READY`, implement:

1. Detection of group-secured Matter frames before unicast session lookup.
2. Fabric and key-set candidate resolution from Group Session ID.
3. Active epoch selection.
4. Group encryption-key and privacy-key derivation.
5. Privacy removal and AES-CCM authentication/decryption.
6. Per-sender replay-counter validation.
7. IPv6 multicast join/leave management.
8. Endpoint selection from persisted group membership.
9. Group-invokable command filtering.
10. Response suppression for multicast commands.
11. Message Counter Synchronization Protocol where required.

No application command may run before message authentication succeeds.

## 6. Access Policy for Future Group Transport

The fixed ACL entry uses `AuthMode = CASE`; it does not authorize
Group-authenticated messages.

Before group transport is enabled, choose and document one of these policies:

1. Add a second fixed wildcard Group ACL entry.
2. Implement a small dedicated GroupID admission policy.
3. Reintroduce a complete Matter ACL engine.

Option 1 is the smallest but gives every authenticated group broad access.
Option 2 can remain compact but is a Tasmota-specific policy. Option 3 is the
only path toward full Matter Access Control conformance.

Until that choice is implemented, group message dispatch must remain disabled.

## 7. Remaining Groupcast Work

Matter 1.6.1 Groupcast cluster `0x0065` is not implemented and must not be
advertised.

Required work includes:

- Membership and endpoint-map attributes.
- Group key association and update commands.
- Multicast policy.
- Listener and sender feature handling.
- Fabric removal cleanup.
- Derived-key cache invalidation.
- Counter persistence and replay recovery.
- Optional AuxiliaryACL behavior if the selected access policy requires it.

The Groupcast `GCAST` feature bit must remain clear until the complete receive
and dispatch path is operational.

## 8. Verification Before Enabling Groups

Minimum tests:

- Direct and wildcard GKM attribute reads.
- GroupKeyMap chunked list writes and rollback.
- Key-set write/read/remove/index commands.
- Key material redaction.
- Persistence and fabric isolation.
- Epoch rollover and mixed-width uint64 start times.
- Multicast address derivation and membership lifecycle.
- Valid and invalid group MICs.
- Duplicate and stale message counters.
- Endpoint membership filtering.
- Commands that are not group-invokable.
- No multicast response generation.
- Reboot and network-reconnect recovery.

## 9. Current Recommendation

For current Tasmota builds:

- Keep the fixed wildcard CASE/Administer ACL.
- Keep ACL writes as documented successful no-ops for commissioner
  compatibility.
- Keep CAT and fine-grained ACL code out of the firmware.
- Keep Group Key Management storage and commands.
- Keep group provisioning fail-closed.
- Do not advertise Groupcast.
- Clearly document that all commissioned fabric members are fully trusted.
