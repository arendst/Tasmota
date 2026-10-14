# Tasmota Fixed ACL Profile

Decision date: 2026-09-29

## Policy

Tasmota exposes one Access Control entry for the accessing fabric:

```text
Privilege   = Administer
AuthMode    = CASE
Subjects    = null
Targets     = null
FabricIndex = accessing fabric
```

This means every authenticated CASE peer on a commissioned fabric is treated
as a full administrator for every endpoint and cluster.

## Implementation

- The entry is generated for reads and is not persisted.
- ACL writes are acknowledged with `SUCCESS` but discarded. This is required
  for Apple Home commissioning, which removes the newly commissioned fabric
  when its post-commissioning ACL write is rejected.
- Reading ACL always returns the fixed wildcard entry, regardless of the last
  value written by a commissioner.
- `AccessControlEntriesPerFabric` reports one.
- No CATs are extracted from NOCs or persisted in sessions.
- No Node ID, CAT, target, or privilege matching is performed.
- No Access Control change event is emitted because the entry is immutable.

## Rationale

Full Matter ACL support requires persistent per-fabric lists, chunked list
writes, subject and target validation, CAT processing, authorization checks on
all Interaction Model paths, migration handling, and ACL events. Tasmota
deliberately omits that complexity for code-size and maintenance reasons.

## Security Consequence

Every commissioned fabric member must be trusted with complete control of the
device. Compromise of any fabric member can expose all operations available to
that fabric.

The policy does not admit nodes outside the fabric: CASE authentication and
fabric binding are still required.

## Conformance Consequence

This is an intentional Matter deviation:

- ACL writes report success but do not change the effective or readable ACL.
- The normative configurable-entry minimum is not provided.
- Fine-grained subjects and targets are unavailable.
- CASE Authenticated Tags are unsupported.
- Group AuthMode ACL entries are unsupported.
- Access Control change events are unavailable.

Tasmota must not claim full Access Control conformance with this profile.

The successful no-op write is particularly important to document: a controller
can believe that its requested Node ID, CAT, target, or privilege restrictions
were installed, while Tasmota continues granting Administer access to every
authenticated CASE peer on that fabric.

## Group Transport

The fixed entry authorizes CASE only. It does not authorize Group-authenticated
messages. Group transport must remain disabled until a separate group access
policy is implemented or full ACL support is restored.
