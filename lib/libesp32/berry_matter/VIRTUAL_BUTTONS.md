# Virtual Matter buttons

A virtual Generic Switch endpoint accepts completed button gestures through
`MtrUpdate` and reports them as Matter Switch events. A remote Tasmota device can
send these commands directly to an ESP32 Matter bridge using `WebSend`; this path
does not require an MQTT broker or another server.

## Configure and trigger a button

In the bridge's Matter configuration, add a virtual **Generic Switch/Button**
(type `v_gensw`) and give it a unique name, for example `Wall button 1`. Use the
assigned endpoint number or that name in commands sent to the bridge:

```text
MtrUpdate {"Name":"Wall button 1","Presses":1}
MtrUpdate {"Ep":33,"Presses":2}
```

`Presses` must be an integer from 1 to 5. Each command represents one complete
gesture, so sending `Presses:1` twice produces two independent single presses.
A count of 2 represents a double press, rather than two separate single presses.
The endpoint returns to its released position (`Switch:0`) after each gesture.
Omitting `Presses` queries state without emitting an event. Invalid values return
an error and do not emit events. Long presses are not supported.

For example, a rule on another Tasmota device can send a single press directly:

```text
ON Button1#State=10 DO WebSend [192.0.2.1] MtrUpdate {"Name":"Wall button 1","Presses":1} ENDON
```

Replace the example address with the bridge's address and use the normal
`WebSend` authentication syntax if its web interface has a password. Add the
rule to an available rule set without overwriting existing rules. On senders
configured with `ButtonTopic 0` (local rule routing), `SetOption73 1` (buttons
detached from relays), and `SetOption13 1` (immediate single presses),
`Button1#State=10` is the immediate press event. Clearing `ButtonTopic` keeps
routing independent of any MQTT connection. These settings apply to all buttons
on the sender: review its other buttons and existing rules before changing them. Other sender configurations can map their
own local events to the same `MtrUpdate` command.

The receiving Matter controller must support Generic Switch devices. Configure
its button action to control the desired Matter accessories; existing actions
that target accessories supplied by another server still depend on that server.
Verify the controller's action, not just the HTTP response. These commands are
not idempotent, and automatically retrying a request can repeat an action.

## Matter behavior

The endpoint advertises Generic Switch device type `0x000F`, Switch cluster
`0x003B`, and feature map `0x16` (momentary switch, release, and multi-press).
It starts released with a non-null `CurrentPosition` of 0. `EventList` declares
`InitialPress`, `ShortRelease`, `MultiPressOngoing`, and `MultiPressComplete`.
A single press emits InitialPress, ShortRelease, and MultiPressComplete with a
count of 1. Multi-press gestures also emit MultiPressOngoing from the second tap.
Virtual buttons do not receive the bridge's physical GPIO button callbacks.

## Host regression tests

From the repository root:

```sh
make -C lib/libesp32/berry
cd lib/libesp32/berry_matter/tests
../../berry/berry -g run_tests.be
```

These tests cover state, event metadata and sequences, button isolation, input
validation, repeated gestures, and `MtrUpdate` dispatch. They do not replace a
firmware build or an end-to-end test with a physical sender and Matter controller.

For a repeated-gesture memory check, run from the same tests directory:

```sh
../../berry/berry -g button_stability_tests.be
```

This also runs the regression suite, then exercises four virtual endpoints with
13,000 gestures and 117,000 events. It checks bounded event queues, released
button state, 64-bit event-number rollover, serialization, and retained memory
after garbage collection. The native `int64` result allocator must release its
payloads for this check to pass.

The soak uses the actual button plugins, event queues, and TLV encoding/decoding,
with simulated network transport and flash persistence. It does not validate
Wi-Fi, controller acknowledgements or actions, power-loss recovery, or hardware
uptime. Run those checks separately on the device.
