Introduce a self-contained async HTTP/HTTPS client for ESP32 (AsyncHttpClientLight) and integrate it directly into the Berry webclient module with def selector #define USE_BERRY_WEBCLIENT_ASYNC. 
Code still include <HTTPClient.h> only for the enum/constant types (e.g., followRedirects_t, transferEncoding_t, HTTPC_TE_*)

Non-blocking GET/POST for telemetry/RPC without freezing the Berry VM.
Lower TIME_WAIT pressure and safer connection reuse.
TLS public-key pinning, optional RSA-only hardening.
Keep API/error-code compatibility while modernizing internals.
What changed

AsyncHttpClientLight

HTTP via lwIP sockets (IClientLite + non-blocking AsyncTcpAdapter) with SO_LINGER (RST-close) to reduce TIME_WAIT.
HTTPS via BearSSL WiFiClientSecure_light, with:
tls_pin_pubkey(fp1[,fp2]) (SHA-1 pubkey fingerprints, one or two pins)
tls_set_rsa_only(true) optional hardening.
Redirects (strict): 301/307 for GET/HEAD; 302/303 switch to GET; configurable limit (default 5).
Chunked & identity decoding; reuse honored unless prior body wasn’t drained (auto reconnect).
FreeRTOS worker (WDT-friendly): asyncGETStart(), asyncPOSTStart(...), asyncState(), asyncAbort().
Error codes aligned with HTTPClientLight (−1..−11).
Async in-RAM body buffer capped ~16 KB (heap safety). Use sync streaming for larger bodies.
Berry webclient (updated bindings)

New methods: async_get_start(), async_post_start(str|bytes), async_state(), async_abort().
async_state() publishes last_code (HTTP or negative error) and last_rx (received bytes), and frees hidden holds for byte payloads.
Headers added via add_header() persist per instance; they reset on begin() and after redirects (re-add if needed).
Sync API unchanged: GET/POST/PUT/PATCH/DELETE, get_string/bytes, write_file/flash.
Dependency note

<HTTPClient.h> remains included only for enum/constant types. No Arduino HTTPClient source code is copied; transport, parsing and async infra are independent.

Compatibility

#define USE_BERRY_WEBCLIENT_ASYNC selector, w/o default sync webclient is used.
Existing Berry scripts using sync APIs remain compatible.
OTA/firmware workflows (sync GET + streaming) are unaffected.

Operational notes

Call async_state() at least once after an async job finishes to publish results and clean up before reusing the same instance.
Add Content-Type once after begin() (e.g., application/json); re-add after begin() or a redirect.
Don’t call close() while an async job is running.
Typical use cases:

Secure periodic telemetry over HTTP(S) + local MQTT

Use webclient for encrypted REST telemetry to the cloud while keeping MQTT for LAN/HA use.
Pattern: create one client per endpoint, begin() once, add Content-Type: application/json, then call async_post_start(payload) from your scheduler and occasionally async_state() to publish last_code/last_rx and clean up.
Keeps the Berry VM responsive; no long blocking writes. For large uploads/downloads, fall back to sync streaming (write_file, get_bytes).
RPC long-polling (e.g., ThingsBoard) without tight polling loops

With async GET you can keep a long-poll open (e.g., ?timeout=60) instead of reconnecting every few seconds.
Pattern: async_get_start() on /rpc?timeout=60, periodically call async_state():
When async_state() returns DONE, parse the body, execute the command (optionally via async POST reply), then immediately start the next long-poll (async_get_start() again).
When it returns RUNNING, do nothing (connection is open); VM stays free for other tasks.
This reduces connection churn, lowers TIME_WAIT pressure, and prevents VM stalls seen with previous sync blocking mode.
Risk

ESP32-only paths; HTTPS relies on existing BearSSL light.
Behavior changes confined to the webclient backend; public sync APIs preserved.
Test plan

Sync: GET large binary → write_file(); verify size/hash; chunked and identity.
Async: burst async_post_start() with periodic async_state(); check last_code/last_rx, stable heap, no WDT.
Redirects: verify 301/302/303/307 and header reset behavior.
TLS: good vs bad fingerprint; RSA-only against ECDSA cert (should fail).


USE MANUAL:

Here is the complete manual for the webclient (Berry) + the underlying AsyncHttpClientLight library (ESP32).

1) Quick overview

Protocols: HTTP (raw lwIP socket) / HTTPS (BearSSL light)
Modes: sync (GET/POST/...) and async (async_get_start / async_post_start)
Buffer: in async mode, the response body is stored in internal RAM (cap 16 KB)
Keep-Alive: default ON (HTTP/1.1). begin() resets headers.

2) State machine (async)

async_state() returns:

0 = ASYNC_IDLE – nothing running (after object creation, after an error, after completion)
1 = ASYNC_RUNNING – request in progress
2 = ASYNC_DONE – finished; body is available in the internal buffer
3 = ASYNC_ERROR – error (HTTP client/transport)
4 = ASYNC_ABORTED – aborted by the user

After every async_state() call, these meta-fields are available:

last_code – HTTP status code or negative error code
last_rx – number of received bytes (async)

Important: do not call begin() while RUNNING. If you want to interrupt a running job, call async_abort(), then call async_state() at least once.

3) Error codes (selection)

Negative return values (aside from HTTP 1xx..5xx):

-1 HTTPC_ERROR_CONNECTION_REFUSED
-2 HTTPC_ERROR_SEND_HEADER_FAILED
-3 HTTPC_ERROR_SEND_PAYLOAD_FAILED
-4 HTTPC_ERROR_NOT_CONNECTED
-5 HTTPC_ERROR_CONNECTION_LOST
-6 HTTPC_ERROR_NO_STREAM
-7 HTTPC_ERROR_NO_HTTP_SERVER
-8 HTTPC_ERROR_TOO_LESS_RAM
-9 HTTPC_ERROR_ENCODING (e.g. unsupported TE)
-10 HTTPC_ERROR_STREAM_WRITE
-11 HTTPC_ERROR_READ_TIMEOUT

4) API (Berry – webclient)
Initialization and security

d = webclient()

d.tls_pin_pubkey(fp1 [, fp2]) -> self (SHA-1 fingerprint “AA:BB:…:FF” or 40 hex)
d.tls_clear_pins() -> self
d.tls_set_rsa_only(bool) -> self (allow RSA certs only)

Parameters and connection

d.set_useragent(ua_str) -> self
d.set_timeouts(http_timeout_ms, tcp_connect_timeout_ms) -> self
d.set_follow_redirects(bool) -> self (if true = “strict”)
d.begin(url) -> self (resets headers, sets URL, optionally (re)creates transport)
d.add_header(name, value [, first=false [, replace=true]]) -> nil
Alias: d.addheader(...)
d.close() -> nil
d.connected() -> bool

Sync HTTP (blocking)

d.GET() -> int http_code
d.POST(str|bytes) -> int
d.PUT(str|bytes) -> int
d.PATCH(str|bytes) -> int
d.DELETE(str|bytes) -> int

Async HTTP (non-blocking core)

d.async_get_start() -> int (1=started, 0=busy, <0=error)
d.async_post_start(str|bytes) -> int (1=started, 0=busy, <0=error)
d.async_state() -> int (returns state 0..4 and fills last_code, last_rx)
d.async_abort() -> nil (signal to the worker; completion is observed via async_state())

Accessing the response

d.get_string() -> string
d.get_bytes() -> bytes
d.get_size() -> int
d.write_file(path) -> int
d.write_flash(addr) -> int (ESP-IDF<5/Tasmota only)

Headers (re-annotation)

begin() deletes all custom headers.
If you call begin() (init, URL change, after an error), re-add e.g. Content-Type.

Between successful requests you don’t need begin() or to repeat headers.

5) Best practice (telemetry / RPC)

Timeouts: reasonable default for telemetry: set_timeouts(4000, 3000). Worse links: 6000/4000.

Retry/abort: if you see RUNNING (1) and don’t want to wait, use async_abort() → async_state() → try a new async_post_start(...).

Re-begin: only after an error (last_code < 0), after changing URL/UA/pins, or when you want to reset headers.

Server response: should return Content-Length or 204 (CL=0). Avoid “identity with no data” (peer silently keeps the socket open).

Long-poll: use a longer HTTP timeout for RPC GET (e.g. 15000); connect can stay 3000–4000.

6) Examples (Berry, no comments)
Async POST (one-shot)

var d = webclient()
d.tls_set_rsa_only(true)
d.tls_pin_pubkey(syscfg.fingerprint1, syscfg.fingerprint2)
d.set_timeouts(4000, 3000)
d.set_useragent("MSBNode/1.0")
d.begin("https://example.com/api/v1/telemetry")
d.add_header("Content-Type","application/json")
var rc = d.async_post_start(json.dump({"k":"v"}))

Status check (e.g. in your tick):

var st = d.async_state()
if st != 1
var code = d.last_code
if code == 200 || code == 204
var body = d.get_string()
end
end

Async GET (long-poll start)

var d = webclient()
d.tls_set_rsa_only(true)
d.tls_pin_pubkey(syscfg.fingerprint1, syscfg.fingerprint2)
d.set_timeouts(15000, 3000)
d.set_useragent("MSBNode/1.0")
d.begin("https://example.com/api/v1/rpc?timeout=10")
var s = d.async_get_start()

In tick (clean handling):

var st = d.async_state()
if st == 2
var code = d.last_code
var body = d.get_string()
end

Abort and immediate restart (no begin())

var st = d.async_state()
if st == 1
d.async_abort()
d.async_state()
var r = d.async_post_start(json.dump({"k":"v"}))
end

Re-begin after error

var st = d.async_state()
if d.last_code < 0 && st != 1
d.close()
d.begin("https://example.com/api/v1/telemetry")
d.add_header("Content-Type","application/json")
end

7) What is NOT limited to 16 KB

The 16 KB cap applies only to the response in async mode (internal buffer).

POST payload can be larger (sent in chunks); watch RAM and throughput.

8) FAQ (brief)

Do I have to call begin() before every POST/GET? No. Only on init/change/after error.
When do headers disappear? Always after begin(). Then add them again.
Can RUNNING (1) hang forever? In practice no, because you have timeouts; the edge case “identity with no data” is broken by async_abort().
Does close()+begin() during RUNNING help? No. Resources are freed only via async_state() after completion/abort.
Is it enough to set tls_pin_pubkey once? Yes, per instance; it persists until clear_pins() or the object is destroyed.
