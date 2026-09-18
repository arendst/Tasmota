# UdpListener code review

Review of `src/UdpListener.h` and its two consumers, `tasmota_support/support_udp.ino`
(SSDP/UPnP emulation) and `tasmota_xdrv_driver/xdrv_04_light_artnet.ino` (Art-Net DMX).

## Verdict

The core idea is sound and well suited to its purpose: copy out of the `pbuf` inside the
lwIP callback, free it immediately, and cap memory with a fixed ring. That correctly solves
the original problem, which is SSDP multicast storms exhausting the heap.

The implementation had a data race in the ring bookkeeping and two unchecked-allocation
crash paths. Everything else was minor. All items below were fixed, see *Resolution* on
each one.

The header is ESP8266-only (`#ifdef ESP8266`, and the library is listed in `lib_ignore`
for ESP32 in `platformio_tasmota32.ini`).

## Correctness issues

### 1. Non-atomic ring bookkeeping between lwIP context and `loop()` (highest severity)

`_recv()` is invoked from the lwIP/SDK context, which can preempt the Arduino `loop()` on
ESP8266. That is the whole reason the `pbuf` is freed inline. So `_recv()` (producer) and
`next()` (consumer) genuinely run concurrently, but they shared two mutable fields with no
synchronization and no `volatile`:

```cpp
_udp_packets--;                                  // consumer, step 1
_udp_index = (_udp_index + 1) % _packet_number;  // consumer, step 2
```

If `_recv()` fires between those two statements it computes

```cpp
uint8_t next_slot = (_udp_index + _udp_packets) % _packet_number;
```

with a decremented count and a stale index, landing on the slot holding the *last pending*
packet and overwriting it. `_udp_packets++` then restores the count, so the consumer later
reads that slot as valid. The result is a silently corrupted packet rather than a dropped
one.

Reordering the two statements does not fix it, it only moves the window: the producer would
then skip a slot and the consumer would later read a stale one.

Separately, nothing was `volatile`, so in `while (UdpCtx.next())` the compiler was free to
keep `_udp_packets` in a register across iterations. `next()` is an implicitly-inline
in-class definition and `_recv()` is only reachable through a function-pointer cast, so
there was no visible write for the optimizer to respect. It happened to work, it was not
guaranteed to.

**Resolution.** Replaced the count plus index pair with a lock-free single-producer /
single-consumer ring. `_head` is written only by `_recv()`, `_tail` only by the consumer,
so no read-modify-write is shared between the two sides. Cross-context accesses use the
GNU-compatible `__atomic_*` built-ins on GCC and Clang: release stores publish completed
packet writes and slot reads, while acquire loads make those operations visible to the
other context. Other C++11 compilers fall back to `std::atomic<uint32_t>` with equivalent
memory ordering. This supplies the ordering that `volatile` alone cannot guarantee.

The counters run in `[0 .. 2 * _packet_number)` rather than `[0 .. _packet_number)`, which
is the classic "one extra bit" trick to tell full from empty without sacrificing a slot.
Sacrificing a slot would have cost 140 bytes of RAM on the Art-Net path, which allocates 30
slots.

### 2. `udp_new()` return value never checked

```cpp
_pcb = udp_new();                            // may return nullptr under memory pressure
...
udp_recv(_pcb, &_s_recv, (void *) this);     // null deref
```

`listen()` guarded `_buffers` but not `_pcb`, and `~UdpListener()` called `udp_remove(_pcb)`
unconditionally, which asserts or faults on `nullptr`. This mattered for Art-Net, where the
object is heap-allocated at runtime, exactly when allocation is most likely to fail.

**Resolution.** Added `valid()`, which reports whether both the pcb and the ring were
allocated. `listen()` refuses to run when invalid, the destructor guards `_pcb`, and
`ArtNetStart()` now checks `valid()` after `new` so it reports "cannot allocate memory"
instead of crashing. The ring allocation also uses `new (std::nothrow)` rather than relying
on the platform's default `new` returning `nullptr`.

### 3. `int16_t srcport`

UDP ports are unsigned. Ephemeral source ports above 32767 are common, so this stored
negative values. It survived only because `support_udp.ino` assigns straight into a
`uint16_t` and the bits round-trip. Any comparison, log format, or arithmetic on it would
have been wrong.

**Resolution.** Now `uint16_t`.

### 4. `_packet_number` was `uint8_t` while the constructor took `size_t`

`packet_number == 256` truncated to 0, `new UdpPacket[0]` returns a non-null pointer, and
the first `% _packet_number` divided by zero. Current callers pass 3 and 30, so this was
latent.

**Resolution.** The field is `size_t`, matching the parameter, and 0 is clamped to 1.

### 5. `disconnect()` was asymmetric with connect

It called `udp_disconnect()` but left the recv callback registered, so a disconnected
listener kept filling its ring. `udp_disconnect()` only clears the remote address, the pcb
stays bound. It also never left the IGMP group: callers did `igmp_joingroup()` themselves
and nobody ever called `igmp_leavegroup()`. And `reset()` did not clear `_udp_ready`, so a
`read()` before the next `next()` could hand back a stale slot, masked only by callers
happening to call `reset()` right before `listen()`.

**Resolution.** Group membership moved into the class as `joinMulticast()` /
`leaveMulticast()`, tracked so that `disconnect()` and the destructor release it.
`disconnect()` now also unregisters the recv callback, after which lwIP frees incoming
packets itself, and drops any pending packets. `reset()` clears `_udp_ready`. Both callers
were updated to use `joinMulticast()` instead of calling `igmp_joingroup()` directly.

### 6. Copy and move not deleted

The class owns a raw `udp_pcb*` and a `new[]` buffer, and registers `this` as the lwIP
callback argument. A copy would double-free and leave the callback pointing at the wrong
object.

**Resolution.** Copy constructor and copy assignment are `= delete`, which suppresses the
implicit move operations too.

## Design and hygiene

- **Silent truncation.** `packet_len` was clamped to `PACKET_SIZE` with no flag, so a
  consumer could not tell a 120-byte packet from one truncated at 120. `support_udp.ino`
  compensated by stealing the last byte for a NUL terminator, which worked but was fragile
  coupling. *Resolution:* the buffer is now `PACKET_SIZE + 1`, `_recv()` always
  NUL-terminates at `buf[len]`, and a `truncated` flag is exposed. The
  `if (packet->len >= UDP_BUFFER_SIZE) { packet->len--; }` dance in `support_udp.ino` is
  gone.
- **Dead weight per slot.** `dstaddr` and `input_netif` were populated on every packet and
  read by neither consumer, which is odd for a class whose reason for existing is RAM.
  *Resolution:* removed. On the Art-Net path that is 8 to 24 bytes times 30 slots,
  depending on whether the build has IPv6 enabled.
- **Pointless `IPAddress` round trip.** `listen()` took `const IPAddress&` but both callers
  build an `ip_addr_t` and pass `&addr`, relying on an implicit `IPAddress` temporary that
  gets converted straight back to `const ip_addr_t*` for `udp_bind()`. *Resolution:* the
  primary overload takes `const ip_addr_t*`, with an `IPAddress` overload kept for
  convenience.
- **Failed bind left a live callback.** `udp_recv()` was registered before `udp_bind()` and
  not undone on failure. *Resolution:* unregistered on the failure path.
- **Unused declarations.** `typedef std::function<void(void)> rxhandler_t;` was never used,
  and dropping it also drops the `<functional>` dependency. `_recv()`'s `upcb` parameter was
  unused. *Resolution:* both removed.
- **Redundant initialization.** `_packet_number` was set in the init list and assigned again
  in the constructor body. *Resolution:* init list only.
- **Cosmetics.** The header comment said "webserver for Tasmota", the include guard was
  `UDPMULTICASTLISTENER_H` and did not match the filename, there was a stray `@` in the
  license text, and `library.properties` claimed `architectures=esp8266,esp32` for
  ESP8266-only code. *Resolution:* all corrected.

## Notes for future work

- There is no drop counter. When the ring is full, packets vanish with no diagnostic. A
  `dropped()` accessor would be cheap and useful when debugging Art-Net frame loss.
- `read()` returns a pointer into the ring that is only valid until the next `next()` call.
  That is documented in the header now, but it is still an easy contract to get wrong.
- The class only receives. Sending still goes through `WiFiUDP`, which is why
  `support_udp.ino` keeps a `PortUdp` instance around on ESP8266.
