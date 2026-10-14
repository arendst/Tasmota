# Baseline reference (byte-identity gate, task 1.4)

Reference numbers for the MQTT 3.1.1 selection. Re-run this gate after **every**
library modification and compare against the counts below.

Recorded at git HEAD `9abbe4ec9` ("Task 1.1 1.2"). `src/PubSubClient.{h,cpp}` at
this commit already carry the task-1.1 selector/config-macro changes (committed,
working tree clean). `tests/src/api_shape_test.cpp` (task 1.3) is present but
still untracked. No existing `*_test.cpp` has been edited.

## Commands

```
cd lib/default/TasmotaPubSub/tests
make clean && make v311
make baseline                          # ./build/pubsub_tests_v311 -ts=baseline
./build/pubsub_tests_v311 -ts=hardening
./build/pubsub_tests_v311              # full run, all suites
```

## Reference counts

| Run | Cases | Assertions | Notes |
|---|---|---|---|
| `-ts=baseline` | 104 / 104 passed, 0 failed | 2106 / 2106 passed, 0 failed | 30 skipped |
| `-ts=hardening` | 28 / 28 passed, 0 failed | 764 total, 742 passed, **22 failed** | 106 skipped |
| full v311 | 134 / 134 passed, 0 failed | 2906 total, 2884 passed, **22 failed** | 0 skipped |

## Designated expected-failure hardening cases (all 22 failing assertions)

The 22 failing assertions are confined to four findings, each present in two
files; the cases themselves still pass (marked may-fail).

| Finding | Locations | Failing assertions |
|---|---|---|
| F-03 trickle-fed packet deadline | `buffer_test.cpp:282`, `findings_test.cpp:290` | 1 + 1 = 2 |
| F-05 partial write | `findings_test.cpp:387`, `streaming_test.cpp:363` | 8 + 8 = 16 |
| F-10 SUBACK return code | `findings_test.cpp:599`, `subscribe_test.cpp:309` | 1 + 1 = 2 |
| F-11 disconnect packet | `connect_test.cpp:436`, `findings_test.cpp:620` | 1 + 1 = 2 |

## Property 27 (MQTT 3.1.1 selection is byte-identical)

Property 27 has no dedicated test. It is realized by the unmodified
Baseline_Suite passing against the v311 selection — i.e. by this gate. A drift in
any count above, other than a knowingly-added case, is a Property 27 violation.

Re-run: `make clean && make v311 && make baseline && ./build/pubsub_tests_v311 -ts=hardening`.

---

## Re-baselined counts (post task 2.0)

Task 2.0 rewrote the two frozen F-04 cases onto the refusal contract and, under
its approved scope amendment, five `baseline` cases whose payloads pushed the
Remaining Length above 16,383. That changed the assertion counts (no case count
changed). The table above is the original task-1.4 record and is kept for
history; **compare later checkpoints against the counts below.**

| Run | Cases | Assertions | Notes |
|---|---|---|---|
| `-ts=baseline` | 104 / 104 passed, 0 failed | 2144 / 2144 passed, 0 failed | 60 skipped |
| `-ts=hardening` (v311) | 28 / 28 passed, 0 failed | 778 total, 756 passed, **22 failed** | 136 skipped |

The 22 failing assertions are the same four designated open findings (F-03,
F-05, F-10, F-11) listed above; F-04 is now expected-PASS on the refusal
contract.

Verified unchanged by task 8.1 (MQTT 5.0 outbound PUBLISH framing): the counts
above were measured immediately before and immediately after that change and are
identical, which is the Property 27 byte-identity gate for it.

Verified unchanged again by task 8.2 (outbound message properties on the publish
path): `-ts=baseline` is 104 / 104 cases and 2144 / 2144 assertions with 60
skipped, and `-ts=hardening` (v311) is 28 / 28 cases and 778 assertions with the
same 22 designated failures. Task 8.2 adds no v311 case: its two new cases sit in
the `mqtt5` suite.

### v5 selection reference (not part of the byte-identity gate)

`make test` runs the v5 binary with `-tse=baseline` plus `$(V5_EXCLUDE)`. After
task 8.2 that run is 128 / 128 cases passed, 31736 assertions with the same 22
designated failures. `make mqtt5` is 71 / 71 cases, 30320 / 30320 assertions.
`make hardening-v5` is 24 / 24 cases, 601 assertions with 22 designated
failures.

The task-8.1 reference, for comparison, was 126 / 126 cases and 31687 assertions
for `make test`, and 69 / 69 cases with 30271 assertions for `make mqtt5`. Task
8.2's delta is exactly its two new writer cases (+2 cases, +49 assertions) in
both runs, so nothing else moved.

Verified unchanged again by task 9.1 (shared acknowledgement assembler and
parser): `-ts=baseline` is 104 / 104 cases and 2144 / 2144 assertions with 60
skipped, and `-ts=hardening` (v311) is 28 / 28 cases and 778 assertions with the
same 22 designated failures. Both measured after `make clean && make`. Task 9.1
adds no v311 case: `mqtt5_ack_test.cpp` is guarded by
`#if MQTT_VERSION == MQTT_VERSION_5_0` in full, and the library's new inbound
acknowledgement dispatch sits inside `if (kMqtt5)`.

In the v5 selection task 9.1 moves `make test` to 131 / 131 cases and 32121
assertions, and `make mqtt5` to 74 / 74 cases and 30705 / 30705 assertions - a
delta of exactly its three new acknowledgement cases (+3 cases, +385 assertions)
in both runs. `make hardening-v5` is unchanged at 24 / 24 cases and 601
assertions with 22 designated failures.

Verified unchanged again by tasks 9.2 / 9.3 / 9.4 (MQTT 5.0 SUBSCRIBE and UNSUBSCRIBE
emission, SUBACK/UNSUBACK per-topic reporting, DISCONNECT handling and AUTH
rejection): `-ts=baseline` is 104 / 104 cases and 2144 / 2144 assertions with 60
skipped, and `-ts=hardening` (v311) is 28 / 28 cases and 778 assertions with the
same 22 designated failures. Both measured after `make clean && make`. These tasks
add no v311 case: `mqtt5_subscribe_test.cpp` and `mqtt5_disconnect_test.cpp` are
guarded by `#if MQTT_VERSION == MQTT_VERSION_5_0` in full, the library's new
SUBACK / UNSUBACK / DISCONNECT / AUTH dispatch sits inside `if (kMqtt5)`, the
SUBSCRIBE Properties byte and the outstanding-entry bookkeeping likewise, and the
Subscription Options byte a default-constructed `MqttSubscribeOptions` produces is
byte-for-byte the trailing QoS byte the 3.1.1 build has always written - which the
five frozen baseline subscribe cases pin directly. The full v311 run is 164 / 164
cases and 3717 assertions with the same 22 failures.

In the v5 selection these tasks move `make test` to 148 / 148 cases and 33143
assertions, and `make mqtt5` to 94 / 94 cases and 32164 / 32164 assertions - a
delta of exactly the 20 new cases (+1459 assertions in `mqtt5`) over the task-9.1
reference of 131 / 74 cases.

`make hardening-v5` DROPS from 24 / 24 cases and 601 assertions to **22 / 22 cases
and 384 assertions**, still with the 22 designated failures. That drop is the two
frozen F-08 exact-buffer SUBSCRIBE cases being excluded from the v5 run only -
`subscribe_test.cpp` "F-08 exact-buffer SUBSCRIBE does not write the QoS byte out
of bounds" and `findings_test.cpp` "F-08 exact-buffer SUBSCRIBE stays in-bounds and
null topics are rejected". Both shrink the working buffer to the literal 3.1.1
bound `10 + topicLength` and then `REQUIRE(psc.subscribe(...))`, which a 5.0
SUBSCRIBE (`11 + topicLength`, one byte more for the Properties field) cannot
satisfy, and both then decode the result with the 3.1.1 `decodeSubscribe`. This is
the same case-level exclusion already applied to the F-01 inbound and F-07 outbound
cases, for the same reason and by the same mechanism (`V5_EXCLUDE` in the
Makefile); **no frozen test source is edited.** Both still run in the v311
selection, where the reference is unchanged at 28 / 28 cases and 778 assertions.
Their 5.0 equivalents are in `mqtt5_subscribe_test.cpp`: "exact-buffer SUBSCRIBE at
the MQTT 5.0 bound stays in bounds" (the boundary from both sides under ASan) and
"a null topic filter is rejected before any length computation".

`make selector-check` still rejects both invalid `MQTT_VERSION` values.

### Post-task-9.4 reference summary (compare later checkpoints against these)

| Run | Cases | Assertions |
|---|---|---|
| `make baseline` (v311) | 104 / 104 | 2144 / 2144, 60 skipped |
| `make hardening` (v311) | 28 / 28 | 778 total, 22 designated failures |
| full v311 (`make test`, first binary) | 164 / 164 | 3717 total, 22 designated failures |
| `make mqtt5` | 94 / 94 | 32164 / 32164 |
| `make hardening-v5` | 22 / 22 | 384 total, 22 designated failures |
| full v5 (`make test`, second binary) | 148 / 148 | 33143 total, 22 designated failures |

---

## Post task 9.8 (Property 24: server-initiated DISCONNECT is absorbed silently)

Task 9.8 adds test code only — one `TEST_CASE` at the bottom of
`mqtt5_disconnect_test.cpp`, inside `TEST_SUITE("mqtt5")` and guarded by
`#if MQTT_VERSION == MQTT_VERSION_5_0` like the rest of that file. No library
source, no frozen baseline or hardening source, and no Makefile line was
touched, so the v311 selection cannot move — and it does not.

**Byte-identity gate unchanged.** Measured after `make clean && make v311`:

| Run | Cases | Assertions |
|---|---|---|
| `make baseline` (v311) | 104 / 104 | 2144 / 2144, 60 skipped |
| `make hardening` (v311) | 28 / 28 | 778 total, 22 designated failures |
| full v311 (`make test`, first binary) | 164 / 164 | 3717 total, 22 designated failures |

Identical to the post-task-9.4 reference summary above, so Property 27 holds for
this task by construction and by measurement.

**v5 selection** (not part of the byte-identity gate):

| Run | Cases | Assertions |
|---|---|---|
| `make mqtt5` | 97 / 97 | 54675 / 54675 |
| `make hardening-v5` | 22 / 22 | 384 total, 22 designated failures |
| full v5 (`make test`, second binary) | 151 / 151 | 55654 total, 22 designated failures |

`make mqtt5` was measured immediately before this task at 96 / 96 cases and
46213 assertions, so the delta is exactly the one new case (+1 case, +8462
assertions) — the 768-point sweep of 256 Reason Codes × the three inbound
DISCONNECT forms plus the two outbound example subcases. `make hardening-v5` is
unchanged, and `make selector-check` still rejects both invalid `MQTT_VERSION`
values.

Note that the pre-9.8 `make mqtt5` figure of 96 / 46213 is already above the
post-9.4 record of 94 / 32164 in the table further up: tasks 9.6 and 9.7 added
their two `mqtt5_subscribe_test.cpp` property cases without re-recording here.
Their v311 counts were unchanged too, which is why the gate above still matches
the post-9.4 row exactly.

## Task 10 checkpoint (framing complete)

Re-ran the byte-identity gate after `make clean && make v311 && make v5`. No
library source or frozen test source was touched since task 9.8, so this is a
confirmation run only.

| Run | Cases | Assertions |
|---|---|---|
| `make baseline` (v311) | 104 / 104 | 2144 / 2144 passed, 0 failed, 60 skipped |
| `make hardening` (v311) | 28 / 28 | 778 total, 756 passed, 22 designated failures, 136 skipped |
| full v311 (`make test`, first binary) | 164 / 164 | 3717 total, 3695 passed, 22 designated failures, 0 skipped |
| full v5 (`make test`, second binary) | 151 / 151 | 55654 total, 55632 passed, 22 designated failures, 111 skipped |
| `make selector-check` | PASS | both MQTT_VERSION=3 and MQTT_VERSION=7 rejected |

Identical to the post-task-9.8 reference above. Property 27 holds: the 3.1.1
selection's baseline and hardening results are unchanged against the count
re-baselined after task 2.0.

---

## Post task 11.1 (shared packet identifier allocator, in-flight count, ack deadline)

Task 11.1 changed the library: `subscribe` / `unsubscribe` now take their Packet
Identifier from the shared rotoring `allocatePacketId()` instead of `nextMsgId++`,
`releasePacketId()` / `inFlightCount()` / `reportAbandoned()` /
`abandonExpiredOutstanding()` were added, `claimOutstanding()` now stamps `sentAt`
itself, and `resetConnectionState()` reports still-occupied entries as abandoned
before clearing the table. It also adds one new test file,
`tests/src/mqtt5_qos_test.cpp`, whose whole body is guarded by
`#if MQTT_VERSION == MQTT_VERSION_5_0`.

**Byte-identity gate unchanged.** The allocator advances the same rotor by one per
call and, with an empty table, returns exactly what `nextMsgId++` returned — and the
table is never populated in the 3.1.1 selection, where every claim site sits inside
`if (kMqtt5)`. The abandonment sweep and the teardown report are likewise inside
`if (kMqtt5)`. Measured after `make clean && make`:

| Run | Cases | Assertions |
|---|---|---|
| `make baseline` (v311) | 104 / 104 | 2144 / 2144 passed, 0 failed, 60 skipped |
| `make hardening` (v311) | 28 / 28 | 778 total, 756 passed, 22 designated failures, 136 skipped |
| full v311 (`make test`, first binary) | 164 / 164 | 3717 total, 3695 passed, 22 designated failures, 0 skipped |

Identical to the post-task-9.4 / post-9.8 / task-10 reference, so Property 27 holds.

**v5 selection** (not part of the byte-identity gate):

| Run | Cases | Assertions |
|---|---|---|
| `make mqtt5` | 103 / 103 | 54748 / 54748 |
| `make hardening-v5` | 22 / 22 | 384 total, 22 designated failures |
| full v5 (`make test`, second binary) | 157 / 157 | 55727 total, 22 designated failures |
| `make selector-check` | PASS | both `MQTT_VERSION=3` and `MQTT_VERSION=7` rejected |

The delta against the task-10 checkpoint (97 / 54675 for `make mqtt5`, 151 / 55654
for full v5) is exactly the six new `mqtt5_qos_test.cpp` cases and their 73
assertions. `make hardening-v5` is unchanged.

---

## Post task 11.2 (QoS-capable public publish entry points)

Task 11.2 adds three new public members — `publish(topic, payload, plength, retained,
qos)`, `publish(topic, payload, retained, qos)` and `publish(const
MqttPublishRequest&)` — plus the `MqttPublishRequest` struct, and moves the
`MqttMessageProperties` definition out of the `#if MQTT_VERSION ==
MQTT_VERSION_5_0` block so both selections can hold one by value inside
`MqttPublishRequest`. No preserved overload was touched and no test source was
added or edited.

**Byte-identity gate unchanged.** The four preserved `publish` overloads,
`publish_P` and `beginPublish` still pass qos 0 / packetId 0 / null properties
through the same assembler, statement for statement. The new entry points are
separate functions that nothing existing calls, and in the MQTT 3.1.1 selection
they refuse `qos > 0` before any byte is assembled (the inbound acknowledgement
dispatch and the abandonment sweep both sit inside `if (kMqtt5)`, so a QoS 1/2
publish there would hold a slot nothing could free). The relocated
`MqttMessageProperties` definition declares storage and no code. Measured after
`make clean && make`:

| Run | Cases | Assertions |
|---|---|---|
| `make baseline` (v311) | 104 / 104 | 2144 / 2144 passed, 0 failed, 60 skipped |
| `make hardening` (v311) | 28 / 28 | 778 total, 756 passed, 22 designated failures, 136 skipped |
| full v311 (`make test`, first binary) | 164 / 164 | 3717 total, 3695 passed, 22 designated failures, 0 skipped |

Identical to the post-task-11.1 reference, so Property 27 holds.

**v5 selection** (not part of the byte-identity gate):

| Run | Cases | Assertions |
|---|---|---|
| `make mqtt5` | 103 / 103 | 54748 / 54748 |
| `make hardening-v5` | 22 / 22 | 384 total, 22 designated failures |
| full v5 (`make test`, second binary) | 157 / 157 | 55727 total, 22 designated failures |
| `make selector-check` | PASS | both `MQTT_VERSION=3` and `MQTT_VERSION=7` rejected |

Every count is identical to the post-task-11.1 reference in both selections: this
task adds library capability and no test case. Its property coverage is tasks
11.6–11.11.

The new entry points were exercised once out of tree (a throwaway `main()` linked
against the built objects, since `tests/src/*.cpp` is wildcarded into the suite)
to confirm the emitted framing before handing back. In the v5 selection a QoS 1
retained publish of `hi` to `a/b` emits `33 0A 00 03 61 2F 62 00 02 00 68 69` —
QoS bits 01, retain set, Packet Identifier 0x0002, then the empty Properties field
*after* the identifier (Requirement 6.1) — a QoS 2 publish emits `34 …`, `qos 3`
is refused with zero bytes out, and a fifth concurrent QoS 1 publish at
`MQTT_MAX_INFLIGHT = 4` is refused with `lastError() == PacketIdUnavailable`,
zero bytes out and `inFlightCount()` unchanged. In the v311 selection the same
QoS 0 publish emits `30 07 00 03 61 2F 62 68 69`, with no Properties field.

---

## Post task 11.3 (PUBACK correlation for outbound QoS 1)

Task 11.3 adds `reportPublishAck()` to the library and one call to it from the
inbound-acknowledgement branch of `loop()`: a PUBACK whose Packet Identifier is
held by an entry in state `AwaitingPuback` latches the Reason Code into
`lastReasonCode()` and the identifier into `lastPacketId()`, then releases the
entry and its identifier (Requirements 7.3, 7.4, 8.16). A Reason Code of 0x80 or
above releases exactly like a success code, which is what makes it terminal.
PUBREC / PUBREL / PUBCOMP still fall through to Requirement 7.8's discard (tasks
11.4 and 12.1). Four example cases were appended to
`tests/src/mqtt5_qos_test.cpp`, whose whole body is guarded by
`#if MQTT_VERSION == MQTT_VERSION_5_0`.

**Byte-identity gate unchanged.** The dispatch this hangs off already sits inside
`if (kMqtt5 && ...)`, a compile-time false in the 3.1.1 selection, and nothing
outside it was touched. Measured after `make clean && make`:

| Run | Cases | Assertions |
|---|---|---|
| `make baseline` (v311) | 104 / 104 | 2144 / 2144 passed, 0 failed, 60 skipped |
| `make hardening` (v311) | 28 / 28 | 778 total, 756 passed, 22 designated failures, 136 skipped |
| full v311 (`make test`, first binary) | 164 / 164 | 3717 total, 3695 passed, 22 designated failures, 0 skipped |

Identical to the post-task-11.1 / post-task-11.2 reference, so Property 27 holds.

**v5 selection** (not part of the byte-identity gate):

| Run | Cases | Assertions |
|---|---|---|
| `make mqtt5` | 107 / 107 | 54920 / 54920 |
| `make hardening-v5` | 22 / 22 | 384 total, 22 designated failures |
| full v5 (`make test`, second binary) | 161 / 161 | 55899 total, 22 designated failures |
| `make selector-check` | PASS | both `MQTT_VERSION=3` and `MQTT_VERSION=7` rejected |

The delta against the post-task-11.2 reference (103 / 54748 for `make mqtt5`,
157 / 55727 for full v5) is exactly the four new `mqtt5_qos_test.cpp` cases and
their 172 assertions. `make hardening-v5` is unchanged. The exhaustive
256-reason-code × three-form sweep of the QoS 1 lifecycle is Property 13 (task
11.7) and is not part of this count.

---

## Post task 11.4 (outbound QoS 2 handshake)

Task 11.4 adds `reportPublishRec()` and `reportPublishComp()` to the library and
two calls to them from the inbound-acknowledgement branch of `loop()`:

- A PUBREC whose Packet Identifier is held by an entry in state `AwaitingPubrec`
  with a Reason Code **below 0x80** emits exactly one PUBREL through the shared
  acknowledgement assembler — `62 02 <id hi> <id lo>`, the mandatory fixed-header
  flags 0x02 (Requirement 7.7) and the short form for the success code — moves the
  entry to `AwaitingPubcomp` (the design's `PubrelSent`) with `sentAt` restamped so
  the `MQTT_ACK_TIMEOUT` window measures the wait for the PUBCOMP, and latches
  nothing into `lastReasonCode()` because the exchange is not over. The matching
  PUBCOMP then completes it, releases the identifier and reports the code through
  `lastReasonCode()` with the identifier through `lastPacketId()`, exactly as a
  PUBACK does for QoS 1 (Requirement 8.12).
- A PUBREC with a Reason Code of **0x80 or above** writes no PUBREL at all,
  releases the entry and its identifier, and exposes the received code together
  with that released identifier through the same two accessors (Requirement 8.14).
  `errorState` is untouched in both cases: one refused message is a normal broker
  answer, not a transport failure.
- "Exactly one PUBREL" is bought by the state guard: only an entry in
  `AwaitingPubrec` is advanced, so a duplicate PUBREC arriving after the PUBREL
  went out is discarded like any other uncorrelated acknowledgement (Requirement
  7.8) rather than emitting a second one. A PUBCOMP is likewise only accepted for
  an entry in `AwaitingPubcomp`. If the transport refuses the PUBREL write the
  entry deliberately stays in `AwaitingPubrec`, so the deadline sweep or the
  teardown abandons it instead of recording a completion that never happened.
- Inbound PUBREL still falls through to Requirement 7.8's discard: it belongs to
  the inbound QoS 2 handshake (task 12.1).

Four example cases were appended to `tests/src/mqtt5_qos_test.cpp`, whose whole
body is guarded by `#if MQTT_VERSION == MQTT_VERSION_5_0`.

**Byte-identity gate unchanged.** The dispatch this hangs off already sits inside
`if (kMqtt5 && ...)`, a compile-time false in the 3.1.1 selection, and nothing
outside it was touched. Measured after `make clean && make`:

| Run | Cases | Assertions |
|---|---|---|
| `make baseline` (v311) | 104 / 104 | 2144 / 2144 passed, 0 failed, 60 skipped |
| `make hardening` (v311) | 28 / 28 | 778 total, 756 passed, 22 designated failures, 136 skipped |
| full v311 (`make test`, first binary) | 164 / 164 | 3717 total, 3695 passed, 22 designated failures, 0 skipped |

Identical to the post-task-11.1 / 11.2 / 11.3 reference, so Property 27 holds.

**v5 selection** (not part of the byte-identity gate):

| Run | Cases | Assertions |
|---|---|---|
| `make mqtt5` | 111 / 111 | 55049 / 55049 |
| `make hardening-v5` | 22 / 22 | 384 total, 22 designated failures |
| full v5 (`make test`, second binary) | 165 / 165 | 56028 total, 22 designated failures |
| `make selector-check` | PASS | both `MQTT_VERSION=3` and `MQTT_VERSION=7` rejected |

The delta against the post-task-11.3 reference (107 / 54920 for `make mqtt5`,
161 / 55899 for full v5) is exactly the four new `mqtt5_qos_test.cpp` cases and
their 129 assertions. `make hardening-v5` is unchanged. The exhaustive
256-reason-code sweep of the QoS 2 handshake is Property 14 (task 11.8) and is
not part of this count.

---

## Post task 11.5 (DUP retransmission encoding and the opt-in in-flight payload copy)

> **Superseded by task 11.5a — kept for history.** The opt-in shape recorded below is
> withdrawn. `MQTT_INFLIGHT_PAYLOAD_COPY` no longer exists, retention is unconditional in
> the MQTT 5.0 selection, and **there is no `v5copy` binary**, so the `v5copy` tables in
> this section have no standing as a reference: they measure a build that cannot be
> produced. The `make baseline` / `make hardening` figures in this section are unchanged and
> still current. The live reference is the *Post task 11.5a* section at the end of this
> file.

Task 11.5 implements Requirement 8.7. Two things landed, and they are gated very
differently.

**Always compiled.** Nothing. The DUP encoding itself already existed —
`buildPublishHeader()` has taken a `dup` argument and set fixed-header bit 3 since task
8.1 — so this task adds the *caller* that asks for it, and that caller is entirely
opt-in.

**Behind `MQTT_INFLIGHT_PAYLOAD_COPY` (default 0) and `MQTT_INFLIGHT_COPY_BUDGET`
(default 0).** The retention, the survival of an outstanding entry across a connection
drop, and the retransmission itself:

- `MqttOutstandingMessage` gains `packetBody` / `packetBodyLen` / `headerByte` — declared
  inside `#if MQTT_INFLIGHT_PAYLOAD_COPY`, so the default entry is byte-for-byte the one
  it has always been.
- The buffered QoS-capable publish path retains the **assembled packet body** plus the
  fixed-header byte, not the payload alone. Requirement 8.7 asks for a retransmission that
  differs from the original in the DUP flag and nothing else, and `MqttMessageProperties`
  is borrowed only for the duration of assembly, so re-framing from the caller's inputs is
  not possible later. Retaining the body makes the resend byte-identical by construction
  and reuses the original Packet Identifier for free, since the identifier is inside those
  bytes. `MQTT_INFLIGHT_COPY_BUDGET` bounds the total across every entry holding a copy.
- `publish_P()` never retains (it streams from flash and has no QoS-capable entry point,
  so it never holds an outstanding entry) and `beginPublish()` never retains **by
  construction**: the declared length is a promise, the bytes arrive incrementally and are
  gone once written, and the length may be many times the working buffer.
- A message whose body does not fit the remaining budget — or whose allocation fails — is
  transmitted **uncopied** and left non-retransmittable. It is not rejected.
- `clearOutstanding()` and the abandonment report in `resetConnectionState()` skip an entry
  holding a copy, so it survives the teardown; `~PubSubClient()` is the backstop.
- Retransmission fires from **exactly one place**: immediately after a CONNACK reporting
  Session Present 1 has put the client into `MQTT_CONNECTED`. Session Present 0 abandons
  and reports every retained entry instead. That is [MQTT-4.4.0-1] — a client "MUST NOT
  resend messages at any other time" — and it means the acknowledgement deadline abandons
  rather than resends, and that Tasmota's stock driver (Clean Start 1 unless
  `flag5.mqtt_persistent`) never takes the retransmission path at all.

**A new build variant, `v5copy`.** The option is compile-time on the *library* object, so
it cannot be reached from a test translation unit: exercising it at all requires the
library rebuilt with it on. The Makefile therefore adds a third binary,
`build/pubsub_tests_v5copy`, which is the MQTT 5.0 selection plus
`-DMQTT_INFLIGHT_PAYLOAD_COPY=1 -DMQTT_INFLIGHT_COPY_BUDGET=4096`. It is **not** a third
protocol selection and is **not** part of the byte-identity gate. `make all` and `make
test` build and run it alongside the other two; `make copy` runs it alone. The new test
file `tests/src/mqtt5_retransmit_test.cpp` is guarded by
`#if (MQTT_VERSION == MQTT_VERSION_5_0) && MQTT_INFLIGHT_PAYLOAD_COPY`, so it contributes
cases to that binary only.

**Byte-identity gate unchanged.** Measured after `make clean && make`:

| Run | Cases | Assertions |
|---|---|---|
| `make baseline` (v311) | 104 / 104 | 2144 / 2144 passed, 0 failed, 60 skipped |
| `make hardening` (v311) | 28 / 28 | 778 total, 756 passed, 22 designated failures, 136 skipped |
| full v311 (`make test`, first binary) | 164 / 164 | 3717 total, 3695 passed, 22 designated failures, 0 skipped |

Identical to the post-task-11.1 / 11.2 / 11.3 / 11.4 reference, so Property 27 holds. It
holds by construction as well: with both macros at their default 0 every line this task
added is preprocessed away, which is directly observable in the built objects —
`nm build/obj/<sel>/lib/PubSubClient.o | c++filt` finds **0** of
`retainPacketCopy` / `releasePacketCopy` / `holdsPacketCopy` / `retransmitOutstanding` /
`discardRetransmittable` in the `v311` and `v5` objects and all **5** in the `v5copy`
object.

**v5 selection** (not part of the byte-identity gate):

| Run | Cases | Assertions |
|---|---|---|
| `make mqtt5` | 111 / 111 | 55049 / 55049 |
| `make hardening-v5` | 22 / 22 | 384 total, 22 designated failures |
| full v5 (`make test`, second binary) | 165 / 165 | 56028 total, 22 designated failures |
| `make selector-check` | PASS | both `MQTT_VERSION=3` and `MQTT_VERSION=7` rejected |

Every count is identical to the post-task-11.4 reference: the default v5 binary is
unaffected because the feature is compiled out of it too.

**v5copy variant** (new; the only run in which this task's behavior exists):

| Run | Cases | Assertions |
|---|---|---|
| `./build/pubsub_tests_v5copy -ts=mqtt5` | 116 / 116 | 55267 / 55267 |
| `make copy` (full run, same exclusions as v5) | 170 / 170 | 56246 total, 22 designated failures |

The delta against the v5 binary is exactly the five new `mqtt5_retransmit_test.cpp` cases
(+5 cases, +218 assertions in the `mqtt5` suite): the DUP / identifier / byte-equality
comparison at QoS 1 and QoS 2, Session Present 0 abandoning instead of resending, the
acknowledgement deadline abandoning rather than resending on a live connection, the
budget-overflow message being sent uncopied and then not resent while its three copied
peers are, and the streaming path retaining nothing. `make copy` reports 170 rather than
171 because `$(V5_EXCLUDE)`'s `*exact-buffer SUBSCRIBE*` pattern also matches the MQTT 5.0
case of that name in `mqtt5_subscribe_test.cpp` — pre-existing behavior of that variable,
identical in the v5 run, and not introduced here.

The two macros were also compiled in their remaining combinations to confirm they are
independent and safe: `MQTT_INFLIGHT_PAYLOAD_COPY=1` with the budget left at 0 (in which
case every message exceeds the remaining budget and nothing is ever retained) and the
MQTT 3.1.1 selection with both set (where the code compiles but is unreachable, since that
selection refuses `qos > 0` and the CONNACK hooks sit inside `if (kMqtt5)`). Both are clean
apart from the one pre-existing `-Wsign-compare` warning in `readByte()`.

**Not verified:** the Tasmota firmware build. The PlatformIO toolchain is unavailable in
this environment, so no `pio run` was attempted. The driver-facing surface is unchanged by
this task — no public member was added or altered — so `xdrv_02_9_mqtt.ino` compiles
against the same API; the command that would confirm it is
`pio run -e tasmota32` from the repository root.
---

## Post task 11.5a (in-flight retention made unconditional, PUBREL resend, two binaries again)

Task 11.5a carries the in-flight retention amendment: Requirements 8.19–8.24, 13.11 and
15.8. It **re-baselines the v5 counts and retires the `v5copy` variant** — the tables in the
post-11.5 section above that name that binary no longer describe anything buildable.

**What moved from "opt-in" to "always compiled".** Retention of the Retained_Packet_Body,
the survival of an outstanding entry across a connection drop, and the retransmission itself
are now **compiled into the `v5` binary** rather than into a build variant:

- `MQTT_INFLIGHT_PAYLOAD_COPY` is **removed entirely** — the macro, and every `#if` guard on
  it, in both `PubSubClient.h` and `PubSubClient.cpp`. There is no on/off selector left and
  no build in which the feature is compiled out (Requirement 8.19).
- `MQTT_INFLIGHT_COPY_BUDGET` **stays**, defaulting to **2048** instead of 0, and means one
  thing: the byte ceiling on the sum of every Retained_Packet_Body held at one time
  (Requirement 8.20). Overriding it to 0 *starves* the budget — every publish transmitted
  uncopied and non-retransmittable — rather than compiling anything out.
- Retention sits **inside the existing `if (kMqtt5)` structure** (`kMqtt5 &&` at the
  retention site, `if (kMqtt5)` in the destructor, `releaseOutstanding()` and
  `clearOutstanding()`), so the MQTT 3.1.1 statement sequence is untouched and Property 27
  holds. Note the difference from task 11.5's mechanism: the retention members and helpers
  are now *compiled* in both selections and eliminated at their **call sites** instead of by
  the preprocessor, which is the same treatment every other MQTT 5.0-only member already
  gets (`claimOutstanding`, `abandonExpiredOutstanding`, …). `nm build/obj/v311/lib/
  PubSubClient.o` therefore finds them present and unreferenced; the gate is the emitted
  wire bytes, and `make baseline` / `make hardening` confirm them below.
- **New behavior: the PUBREL resend (Requirement 8.23).** An entry awaiting PUBCOMP — PUBREC
  received, PUBREL sent, no PUBCOMP — now resends exactly one PUBREL carrying its Packet
  Identifier on a Session Present 1 reconnect, at no budget cost and whether or not the
  entry is marked non-retransmittable. Task 11.5's `holdsPacketCopy` deliberately excluded
  that state, so no PUBREL was ever resent. The predicate was split and renamed to match
  what each half answers: `holdsRetainedBody()` (a PUBLISH replay is due) and
  `resendsOnSessionResume()` (a PUBLISH replay **or** a PUBREL resend is due — which is
  exactly the set of entries that survives a teardown).
- A retained body is released when the entry advances to `AwaitingPubcomp`, since the
  PUBLISH is done as far as the broker is concerned. That returns its bytes to the budget at
  the earliest honest moment instead of pinning them for the whole PUBCOMP wait.

**Two binaries, and no more (Requirement 15.8).** `tests/Makefile` drops `v5copy` from
`SELECTIONS`, `all` and `test`, drops the `copy` target and `SELDEF_v5copy` / `BIN_v5copy`,
and its header comment block records the two-binary rule instead of the third variant. The
protocol selection is the only build dimension again. `mqtt5_retransmit_test.cpp` is guarded
by `#if MQTT_VERSION == MQTT_VERSION_5_0` alone, so its cases run in the ordinary `v5`
binary; its budget-exhaustion case derives its body size **from** the compiled-in 2048-byte
budget (three bodies fit, the fourth does not) rather than assuming a 4096-byte one, and the
one case needing a single body larger than the whole budget calls `setBufferSize()`.

**Byte-identity gate — unchanged.** Measured after `make clean && make test`:

| Run | Cases | Assertions |
|---|---|---|
| `make baseline` (v311) | 104 / 104 | 2144 / 2144 passed, 0 failed, 60 skipped |
| `make hardening` (v311) | 28 / 28 | 778 total, 756 passed, 22 designated failures, 136 skipped |
| full v311 (`make test`, first binary) | 164 / 164 | 3717 total, 3695 passed, 22 designated failures, 0 skipped |

Identical to every reference since task 11.1, so Property 27 holds.

**v5 selection — re-baselined** (not part of the byte-identity gate):

| Run | Cases | Assertions |
|---|---|---|
| `make mqtt5` | 117 / 117 | 55365 / 55365 |
| `make hardening-v5` | 22 / 22 | 384 total, 22 designated failures |
| full v5 (`make test`, second binary) | 171 / 171 | 56344 total, 22 designated failures |
| `make selector-check` | PASS | both `MQTT_VERSION=3` and `MQTT_VERSION=7` rejected |

The delta against the post-task-11.5 v5 reference (111 / 55049 for `make mqtt5`, 165 / 56028
for full v5) is the six `mqtt5_retransmit_test.cpp` cases arriving in this binary for the
first time: +6 cases and +316 assertions in the `mqtt5` suite. `make hardening-v5` is
unchanged. Those six are the DUP / identifier / byte-equality comparison at QoS 1 and QoS 2,
the **new** PUBREL resend from `AwaitingPubcomp` (both for a retained entry and for one made
non-retransmittable by a body larger than the whole budget), Session Present 0 abandoning
both surviving kinds instead of resending, the acknowledgement deadline still abandoning
both kinds on a live connection, the refitted budget-exhaustion case, and the streaming path
retaining nothing.

**Expectations re-derived, not assumed.** An entry awaiting PUBCOMP now survives a teardown,
so three existing `mqtt5_retransmit_test.cpp` cases were re-derived and two of them gained a
QoS 2 / `AwaitingPubcomp` arm alongside their QoS 1 arm; `"Session Present 0 abandons the
retained message…"` was renamed to `"…abandons the carried-over entry…"` to name the wider
set. `mqtt5_qos_test.cpp` needed no change: its abandonment cases (`"disconnect abandons
every outstanding operation"`, the deadline case) drive **subscriptions**, which hold no
retained body and are not in `resendsOnSessionResume()`, and its QoS 2 handshake cases never
cross a teardown. The split the amendment introduces is between the CONNACK and everything
else — Session Present 1 resends, while Session Present 0, the `MQTT_ACK_TIMEOUT` deadline on
a live connection and `disconnect()` all still abandon.

**Not verified:** the Tasmota firmware build. PlatformIO is unavailable in this environment,
so no `pio run` was attempted. No public member was added or altered by this task — the two
renamed helpers are private and the removed macro was never referenced by the driver — so
`xdrv_02_9_mqtt.ino` compiles against the same API. The commands that would confirm it are
`pio run -e tasmota32` and `pio run -e tasmota` from the repository root, for both the
MQTT 3.1.1 and the MQTT 5.0 selection.

---

## Post task 11.6 (Property 12: Packet Identifier uniqueness and release)

Test-only task. One case, `"Property 12: Packet Identifier uniqueness and release"`, was
appended to `tests/src/mqtt5_qos_test.cpp` inside the existing
`#if MQTT_VERSION == MQTT_VERSION_5_0` guard. **No library source and no frozen test source
was touched.**

The case carries a small model of what the library currently holds — one entry per started
operation, with the identifier read back off the emitted bytes — and re-asserts the
invariant after every step: `inFlightCount()` equals the model's size, every identifier in
it is nonzero, and no two are equal. So uniqueness and range are checked at every instant of
every script rather than at its end. Four subcases:

- **Curated interleavings.** All four packet types that allocate an identifier (QoS 1
  publish, QoS 2 publish, subscribe, unsubscribe) in flight at once and acknowledged out of
  order; strictly alternating start/acknowledge twice round the kinds; nothing answered, so
  the `MQTT_ACK_TIMEOUT` deadline abandons the whole table and a fresh full set then
  completes normally; a PUBREC of 0x87 releasing only its own identifier; and
  acknowledgements naming identifiers nothing holds (and identifiers held by the wrong kind
  of operation) changing nothing.
- **Identifier vectors 1, 2, 0x00FF, 0x0100, 0xFFFE, 0xFFFF**, each exercised by a different
  packet type. The rotor is walked to the value before each target with UNSUBSCRIBEs, the
  table emptied on both sides of the walk, and the operation under test then asserted to
  carry exactly that identifier and to release it on its acknowledgement. The vectors are
  ascending so ONE sweep of the identifier space serves all six: reaching 0xFFFF leaves the
  rotor one step from the wrap, which is how identifier 1 is reached without a second sweep.
- **A full rotor wrap with one identifier held and one released.** A QoS 1 publish is
  completed by its PUBACK and a QoS 2 publish is left awaiting its PUBREC; the space is then
  walked once. The held identifier is never reissued, the released one is — which is the
  "exactly when" half of Requirement 8.6 / 8.16 — no identifier is 0, and the walk reaches
  65535. The held entry then still completes its handshake with that same identifier on the
  PUBREL, so it kept it across the whole wrap.
- **Table saturation.** Four publish entries (never evicted, unlike a subscription) fill the
  table; a further QoS 1 and QoS 2 publish are each refused with
  `lastError() == PacketIdUnavailable` and an empty outbound record — Requirement 8.5's "no
  byte of that packet is transmitted" — with every in-flight entry undisturbed and the
  connection still up. One PUBACK then makes room again.

**Byte-identity gate unchanged.** Measured after `make clean && make`:

| Run | Cases | Assertions |
|---|---|---|
| `make baseline` (v311) | 104 / 104 | 2144 / 2144 passed, 0 failed, 60 skipped |
| `make hardening` (v311) | 28 / 28 | 778 total, 756 passed, 22 designated failures, 136 skipped |
| full v311 (`make test`, first binary) | 164 / 164 | 3717 total, 3695 passed, 22 designated failures, 0 skipped |

Identical to every reference since task 11.1, so Property 27 holds. The new case is guarded
into the 5.0 selection only, so the v311 binary is untouched by it.

**v5 selection** (not part of the byte-identity gate):

| Run | Cases | Assertions |
|---|---|---|
| `make mqtt5` | 118 / 118 | 56067 / 56067 |
| `make hardening-v5` | 22 / 22 | 384 total, 22 designated failures |
| full v5 (`make test`, second binary) | 172 / 172 | 57046 total, 22 designated failures |
| `make selector-check` | PASS | both `MQTT_VERSION=3` and `MQTT_VERSION=7` rejected |

The delta against the post-task-11.5a v5 reference (117 / 55365 for `make mqtt5`, 171 /
56344 for full v5) is exactly the one new case and its 702 assertions. `make hardening-v5`
is unchanged. The two rotor sweeps are aggregated into single flags rather than asserted per
step, which is why ~131,000 allocations cost 702 assertions and about a second of runtime.

**Not verified:** the Tasmota firmware build. This task adds no library change at all, so
`xdrv_02_9_mqtt.ino` compiles against the same API; the commands that would confirm it are
`pio run -e tasmota32` and `pio run -e tasmota` from the repository root.

---

## Post task 11.8 (Property 14: Outbound QoS 2 handshake)

Test-only task. One case, `"Property 14: Outbound QoS 2 handshake"`, was appended to
`tests/src/mqtt5_qos_test.cpp` inside the existing `#if MQTT_VERSION == MQTT_VERSION_5_0`
guard, directly after `"Property 13: Outbound QoS 1 lifecycle"` (task 11.7, itself
undocumented in this file until now). **No library source and no frozen test source was
touched.**

Two subcases:

- **Every PUBREC Reason Code, 0x00 through 0xFF.** For each code a fresh connection
  publishes at QoS 2 and receives a PUBREC carrying it. Below 0x80: exactly one PUBREL is
  in the outbound record (size 4, fixed-header byte `MQTTPUBREL | 0x02`), decoded with
  `MqttParser::decodeAck5(..., MQTTPUBREL)` and checked to carry the same identifier; the
  entry still holds it until the matching PUBCOMP completes it, releases it, and reports it
  through `lastPacketId()`/`lastReasonCode()` (Requirement 8.12). At 0x80 or above: no
  PUBREL — the outbound record is empty — the identifier is released immediately, and both
  the received Reason Code and the released identifier are exposed as the failure result,
  with the connection staying up and a later stray PUBCOMP discarded (Requirement 8.14).
- **The identifier boundary vectors 1, 2, 0x00FF, 0x0100, 0xFFFE, 0xFFFF.** Reuses
  `mqtt5_p12::Session` (task 11.6) to prime the shared rotor to each target via
  `primeTo()`, then drives one QoS 2 publish carrying that exact identifier through both
  sides of the handshake: a success run (PUBREC 0x00 → one correctly framed PUBREL → PUBCOMP
  completes it) and a failure run at the same identifier (PUBREC 0x87 → no PUBREL, released,
  Reason Code exposed).

**Byte-identity gate unchanged.** Measured after `make clean && make`:

| Run | Cases | Assertions |
|---|---|---|
| `make baseline` (v311) | 104 / 104 | 2144 / 2144 passed, 0 failed, 60 skipped |
| `make hardening` (v311) | 28 / 28 | 778 total, 756 passed, 22 designated failures, 136 skipped |
| full v311 (`make test`, first binary) | 164 / 164 | 3717 total, 3695 passed, 22 designated failures, 0 skipped |

Identical to every reference since task 11.1, so Property 27 holds. The new case is guarded
into the 5.0 selection only, so the v311 binary is untouched by it.

**v5 selection** (not part of the byte-identity gate):

| Run | Cases | Assertions |
|---|---|---|
| `make mqtt5` | 120 / 120 | 75237 / 75237 |
| `make hardening-v5` | 22 / 22 | 384 total, 362 passed, 22 designated failures, 263 skipped |
| full v5 (`make test`, second binary) | 174 / 174 | 76216 total, 76194 passed, 22 designated failures, 111 skipped |
| `make selector-check` | PASS | both `MQTT_VERSION=3` and `MQTT_VERSION=7` rejected |

The delta against the pre-task-11.8 v5 reference (119 / 69405 for `make mqtt5`, measured
directly before this task's case was added) is exactly the one new case and its 5832
assertions. `make hardening-v5` is unchanged. The 256-reason-code sweep runs a full
connect/publish/PUBREC[/PUBCOMP] cycle per code, which is why it costs roughly the same
assertion count per iteration as Property 13's PUBACK sweep.

**Not verified:** the Tasmota firmware build. This task adds no library change at all, so
`xdrv_02_9_mqtt.ino` compiles against the same API; the commands that would confirm it are
`pio run -e tasmota32` and `pio run -e tasmota` from the repository root.

---

## Post task 11.9 (Property 16: Retransmission differs only by the DUP flag, and happens only on a Session Present 1 reconnect)

Test-only task. One case, `"Property 16: Retransmission differs only by the DUP flag, and
happens only on a Session Present 1 reconnect"`, was appended to
`tests/src/mqtt5_qos_test.cpp` inside the existing `#if MQTT_VERSION == MQTT_VERSION_5_0`
guard, directly after `"Property 14: Outbound QoS 2 handshake"`. **No library source and no
frozen test source was touched.** Retention is unconditional in the MQTT 5.0 selection since
task 11.5a, so there is no macro to enable and nothing to vary but body sizes and timelines.

Four subcases:

- **The PUBLISH-replay half, over a curated sweep** of topic names, payloads (empty, short,
  long), retain flags, QoS 1/2 and message property sets (none; payload format + content
  type; response topic paired with correlation data; five user properties with a duplicate
  key; every kind combined). For each vector: publish, drop the connection with the entry
  unacknowledged, reconnect with Session Present 1, and compare the two recorded byte
  sequences for equality everywhere except bit 3 of the fixed header (Requirement 8.7) —
  including the reused Packet Identifier and the property set, which never differs because
  the retained bytes are the assembled body, not a re-framing from the caller's inputs.
- **The negative half.** Two publishes (QoS 1 and QoS 2) sit unacknowledged through an
  unrelated subscription's full acknowledgement cycle and two keepalive cycles on a live
  connection — every byte written during either cycle is decoded and asserted to be a
  PINGREQ, never a PUBLISH — and then through the `MQTT_ACK_TIMEOUT` deadline, which
  abandons both rather than resending them, again asserting nothing but at most a PINGREQ
  reaches the wire.
- **Session Present 0.** Two retained entries (QoS 1 and QoS 2) survive a connection drop and
  are then abandoned and freed instead of resent, with the abandonment reported through
  `lastReasonCode() == MQTT_RC_UNSPECIFIED_ERROR` and the reconnect's post-CONNACK bytes
  empty.
- **The PUBREL-resend half**, for an entry awaiting PUBCOMP (PUBLISH QoS 2 → PUBREC → PUBREL
  sent, no PUBCOMP), in both arms Requirement 8.23 names: the entry retaining its PUBLISH
  body, and the entry made non-retransmittable by a body larger than the whole
  `MQTT_INFLIGHT_COPY_BUDGET` (via `setBufferSize()`). Both arms survive the drop and both
  resend exactly one PUBREL — never a PUBLISH replay — carrying the original identifier with
  fixed-header flags 0x02 on a Session Present 1 reconnect, and the PUBCOMP still completes
  it on the new connection.

**Byte-identity gate unchanged.** Measured after `make clean && make`:

| Run | Cases | Assertions |
|---|---|---|
| `make baseline` (v311) | 104 / 104 | 2144 / 2144 passed, 0 failed, 60 skipped |
| `make hardening` (v311) | 28 / 28 | 778 total, 756 passed, 22 designated failures, 136 skipped |
| full v311 (`make test`, first binary) | 164 / 164 | 3717 total, 3695 passed, 22 designated failures, 0 skipped |

Identical to every reference since task 11.1, so Property 27 holds. The new case is guarded
into the 5.0 selection only, so the v311 binary is untouched by it.

**v5 selection** (not part of the byte-identity gate):

| Run | Cases | Assertions |
|---|---|---|
| `make mqtt5` | 121 / 121 | 75933 / 75933 |
| `make hardening-v5` | 22 / 22 | 384 total, 362 passed, 22 designated failures, 264 skipped |
| full v5 (`make test`, second binary) | 175 / 175 | 76912 total, 76890 passed, 22 designated failures, 111 skipped |
| `make selector-check` | PASS | both `MQTT_VERSION=3` and `MQTT_VERSION=7` rejected |

The delta against the post-task-11.8 v5 reference (120 / 75237 for `make mqtt5`, 174 / 76216
for full v5) is exactly the one new case and its 696 assertions. `make hardening-v5` is
unchanged.

**Not verified:** the Tasmota firmware build. This task adds no library change at all, so
`xdrv_02_9_mqtt.ino` compiles against the same API; the commands that would confirm it are
`pio run -e tasmota32` and `pio run -e tasmota` from the repository root.
