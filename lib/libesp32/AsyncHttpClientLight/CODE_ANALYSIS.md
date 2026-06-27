# Code Review: `USE_BERRY_WEBCLIENT_ASYNC` async web client

**Status:** Not mergeable as-is. Device-crashing/hanging defects present.
**Scope reviewed:**
- `lib/libesp32/AsyncHttpClientLight/src/AsyncHttpClientLight.{h,cpp}` (~1600 lines)
- `tasmota/tasmota_xdrv_driver/xdrv_52_3_berry_webclient.ino` (Berry bindings)
- `lib/libesp32/berry_tasmota/src/be_webclient_lib.c` (class definition)
- Cross-checked against `lib/lib_ssl/tls_mini/src/WiFiClientSecureLightBearSSL.{h,cpp}`

---

## What it is

A new, self-contained async HTTP/HTTPS client (`AsyncHttpClientLight`) by an external
contributor (HexaMaster), wired into the existing Berry `webclient` class as a
compile-time alternative to `HTTPClientLight`. It adds:

- `async_get_start()`, `async_post_start()`, `async_state()`, `async_abort()`
- TLS public-key pinning: `tls_pin_pubkey()`, `tls_clear_pins()`, `tls_set_rsa_only()`

It is **off by default** (commented out in `my_user_config.h`), which is the right call
given the state below. Existing sync Berry scripts are meant to remain compatible.

---

## Architecture concern: a FreeRTOS task per request

Every operation — including **synchronous** `GET/POST/getString/writeToStream` — runs by
spawning a new FreeRTOS task (`WorkerTask`, 12 KB stack via `ASYNCHTTP_WKR_STACK`) and then
the caller busy-waits on a semaphore (`runJob`).

- For **sync** calls this is pure overhead: the main loop is blocked the whole time anyway
  (it spins on `xSemaphoreTake` + `delay(0)`), so you pay 12 KB stack + task create/teardown
  per request and gain nothing.
- For **async** it legitimately offloads blocking I/O to the worker.

The cost model (a fresh 12 KB-stack task for each job, low priority `tskIDLE_PRIORITY+1`) is
heavy for an ESP32 and atypical for Tasmota, which normally does cooperative networking on
the main loop / `fast_loop`. **This central design decision deserves a discussion before the
feature goes further.**

---

## Critical bugs (blocking)

### C1. Use-after-free on destruction while a job is in flight
`~AsyncHttpClientLight()` calls `end()` (which does `_transport->stop()` + `_transport.reset()`)
and deletes `_busy`/`_asyncDone`, but it **never aborts or joins a running worker task**.

If the Berry `webclient` object is GC'd or `deinit`'d while `async_state()` is still
`RUNNING` (script drops its reference, or calls `close()`/reuses incorrectly), the worker
keeps dereferencing `self->_transport`, `self->_asyncBody`, and the freed semaphores.
Guaranteed crash.

**Fix:** destructor (and `end()`/`deinit`) must set `_asyncAbortReq`, wait for the worker to
exit, then free.

### C2. Deadlock if response accessors are called while `RUNNING`
`getString()` / `writeToStream()` only take the fast path when `_asyncState == ASYNC_DONE`.
If called while still `RUNNING`, they fall through to `runJob()` →
`xSemaphoreTake(_busy, portMAX_DELAY)`. But `_busy` is only released inside `asyncState()` on
the *same* (now-blocked) main thread. The main loop hangs forever → WDT reset.

The docs say "don't do this," but a library should not deadlock the device on misuse.

### C3. No timeout/abort in the sync body reader → infinite loop
`writeToStreamDataBlock()` (used by sync `getString`, `writeToStream`, `get_bytes`,
`write_file`, `write_flash`) loops:

```cpp
while (_transport && _transport->connected() && (len > 0 || len == -1)) {
  int avail = _transport->available();
  if (avail <= 0) { delay(1); continue; }   // no timeout, no abort check
  ...
}
```

With an identity response, keep-alive, and no `Content-Length` (`_size == -1`), a server that
holds the socket open with no further data spins forever. The readme acknowledges this
"identity with no data" hazard, but only the **async** path (`readBodyToStreamString`) guards
it (abort flag + 16 KB cap). The sync path has neither a timeout nor an abort check.

### C4. Stale `Content-Length` on a 302/303 redirect-to-GET
In `Job_SendBuf`/`Job_SendStr`, each redirect iteration does **not** reset `_headers`;
`Content-Length` is only (re)added when a payload is present. On a 302/303 the code sets
`payload=nullptr`/`size=0` and switches to GET, so the previously injected
`Content-Length: N` stays in `_headers` and is sent on the bodyless GET. The server then
waits for a body that never arrives → hang/timeout.

Related: `beginInternal()` (reached via `setURL()` for absolute redirect targets) never clears
`_headers`, contradicting the readme's "headers reset after redirect."

### C5. Stale async body returned after a later sync call
`_asyncState` is never reset to `IDLE` once `DONE`, and sync `GET()` doesn't touch it. After
one async job, a subsequent **sync** `GET()` on the same instance populates `_size`/transport,
but `getString()` still sees `_asyncState == ASYNC_DONE` and returns the **old** `_asyncBody`.

**Fix:** any new request (sync or async) must invalidate the DONE fast-path.

---

## High / medium issues

### H1. Async POST from `bytes` does not copy the payload
`asyncPOSTStart(uint8_t*, size)` stores the raw pointer; the worker reads it later. The
binding guards GC by stashing the bytes object in `.__async_hold`, but nothing prevents the
script from **mutating/resizing** that `bytes` while the job runs, which can realloc the
buffer → worker reads freed memory. The `String` overload copies (safe); the buffer overload
should too, or the contract must be enforced.

### H2. Unbounded RAM growth for chunked sync responses
`wc_getstring`/`wc_getbytes` guard with `if (sz >= 32767)` using `getSize()`, but a chunked
response reports `_size == -1`, so the guard is bypassed and `Job_GetString` accumulates the
entire body into a `StreamString` with no cap → OOM on a large chunked download. (The async
path is capped at 16 KB; the sync path is not.)

### H3. IPv4-only HTTP transport
`AsyncTcpAdapter` uses `sockaddr_in` / `AF_INET` and `(in_addr_t)ip`. Tasmota supports IPv6;
the previous `WiFiClient`-based path did too. Plain-HTTP async over IPv6 will not work.

### M1. TLS transmit buffer effectively 85 bytes
`new BearSSL::WiFiClientSecure_light(16384, 0)` → `setBufferSizes(recv, xmit)` yields
`_iobuf_out_size = 0 + MAX_OUT_OVERHEAD(85)`. So a 16 KB receive buffer (heavy heap, allocated
up front) is paired with a near-zero send buffer (every TLS record carries almost no app data
— poor POST throughput). Tasmota normally uses balanced sizes (e.g. 1024/1024). Needs tuning.

### M2. `SO_LINGER` RST-close is unconditional and aggressive
`AsyncTcpAdapter::stop()` always sets `l_onoff=1, l_linger=0`, forcing a TCP RST on every close
to cut TIME_WAIT. That can discard unread data and surface as connection-reset on the peer.
Fine as an *option*, questionable as an unconditional default.

### M3. Default HTTPS is unauthenticated
With no pins, `BearSslAdapter` calls `setPubKeyFingerprint(any, any, allow_all=true)` — accepts
any server key (MITM-open). This matches existing Tasmota `_light` behavior, so it is not a
regression, but combined with SHA-1 pubkey pinning it should be documented clearly that
security requires explicit pinning.

---

## Code quality / process

- **License mismatch:** source headers say `GPL-3.0-or-later`; `library.json` says
  `"license": "MIT"`. Must be reconciled before merge.
- **`library.json`** description ("Async extension for HttpClientLight by Stephan Hadinger")
  misattributes authorship, and the `authors` field is a single object, not the array
  PlatformIO expects.
- **Non-English comments and typos** throughout (Slovak/Czech: "telo", "drž payload, aby ho GC
  nevyhodil kým beží job", "za chunkom"; typos: "wheen", "writin", "succesful",
  "contetn-length", "metho"). Tasmota upstream is English-only.
- **Massive `#ifdef USE_BERRY_WEBCLIENT_ASYNC` forking** in `xdrv_52_3` (~20 duplicated
  `wc_getclient` blocks). Selecting the client type with a single typedef/helper would remove
  almost all of it and shrink the diff dramatically.
- **`asyncState()` is declared `const` but `const_cast`s `this`** to mutate state and free
  resources — misleading signature.
- Worker task names are fixed strings, so multiple concurrent clients collide (harmless, but
  indicative).

---

## What's actually fine

- lwIP non-blocking connect with `select()` timeout.
- Error-code table mirroring `HTTPClientLight`.
- Chunked / identity decoding.
- Abort-aware async body reader with a hard 16 KB cap.
- Holding the Berry payload via `.__async_hold` to prevent GC.
- Cross-thread result handoff is correctly ordered: the worker writes results **before**
  `xSemaphoreGive`, and the main thread reads them **after** `xSemaphoreTake`, so the published
  `_asyncHttpCode` / `_asyncRxBytes` / `_asyncBody` are not racy *as long as the documented call
  sequence is followed*.
- Feature is default-off, which is appropriate.
