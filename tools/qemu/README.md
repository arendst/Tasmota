# Running Tasmota (ESP32) under QEMU

`tasmota-qemu.sh` (in `tools/qemu/`) builds a Tasmota ESP32 firmware with PlatformIO and
boots it under an Espressif QEMU fork on an amd64 host — serial console always, and a
**browsable web UI** when the WiFi-capable fork is used.

It builds the **current checkout** (your local changes), not a fresh clone. Build
artifacts and the QEMU forks live under `tasmota-qemu/` at the repo root (gitignored).
Examples below assume you run it from `tools/qemu/`; from the repo root use
`tools/qemu/tasmota-qemu.sh …` — either works, paths resolve the same.

## Quick start

Networked Tasmota with a reachable web UI, in one command:

```bash
NET_WIFI=1 ./tasmota-qemu.sh        # build -> image -> wifi QEMU -> boot, joins emulated AP
# then browse:
open http://localhost:8080
```

First run is slow: it clones Tasmota, pulls the toolchain, compiles the firmware, and
clones + builds the QEMU fork. Subsequent runs are cached.

Serial-only (no network, stock QEMU, fastest path):

```bash
./tasmota-qemu.sh                   # stock fork, serial console only
```

## Commands

```
./tasmota-qemu.sh [command]
  build        Compile Tasmota firmware (PlatformIO env $TASMOTA_ENV)
  make-image   Build the QEMU flash image (factory.bin, or merge raw parts)
  qemu-setup   Clone + build the chosen QEMU fork
  run          Boot the firmware in QEMU (serial tee'd to qemu.log)
  smoketest    Boot a minimal Arduino app (same IDF) to sanity-check QEMU itself
  decode [log] Symbolize the last panic backtrace from a log (default: qemu.log)
  all          build -> make-image -> qemu-setup -> run   (default)
```

## Key environment variables

| Variable          | Default              | Meaning |
|-------------------|----------------------|---------|
| `NET_WIFI`        | `0`                  | `1` = one-shot networked recipe (see below) |
| `WIFI_SSID`       | `Open Wifi`          | AP the firmware joins when `NET_WIFI=1` |
| `WIFI_PASS`       | *(empty)*            | password for that AP (the emulated one is open) |
| `NET_WIFI_ENV`    | `tasmota32solo1`     | env used by `NET_WIFI=1` |
| `TASMOTA_ENV`     | `tasmota32`          | PlatformIO env (must be an ESP32 one) |
| `MINIMAL`         | `0`                  | `1` = build the stripped `tasmota32-safeboot` env |
| `QEMU_VARIANT`    | `stock`              | `stock` (no WiFi) or `wifi` (Ebiroll/calib fork) |
| `FLASH_SIZE`      | `4MB`                | `2MB`/`4MB`/`8MB`/`16MB` only |
| `WEB_PORT_HOST`   | `8080`               | host port forwarded to guest `:80` |
| `USE_SYSTEM_QEMU` | `0`                  | `1` = use `qemu-system-xtensa` from `PATH` |
| `INSTALL_DEPS`    | `0`                  | `1` = apt-get the QEMU build deps (needs sudo) |

## How `NET_WIFI=1` works

Getting a networked Tasmota into QEMU needs three things to line up; `NET_WIFI=1`
sets all of them:

1. **Survive early IDF init** — the full dual-core `tasmota32` build panics in
   `esp_timer_impl_init` under QEMU (null interrupt vector). The **unicore
   `tasmota32solo1`** build does not, so `NET_WIFI=1` selects it.
2. **Survive WiFi bring-up** — Tasmota's WiFi blobs poke RF registers (`0x6003xxxx`)
   that the **stock** QEMU does not emulate (`LoadStorePIFAddrError`). The
   **`wifi` fork** (Ebiroll/calib) emulates them, so `NET_WIFI=1` forces
   `QEMU_VARIANT=wifi`.
3. **Get an IP the host can reach** — that fork advertises one open AP named
   **`Open Wifi`**. `NET_WIFI=1` writes station credentials into a managed block in
   `tasmota/user_config_override.h`, so the firmware joins it in station mode, gets a
   slirp DHCP lease (`10.0.2.15`), and the `:8080 -> :80` port-forward becomes routable.

The credential block is delimited and idempotent — re-running rewrites only that block
and leaves the rest of `user_config_override.h` intact.

## Debugging a panic

The `run` step boots with `-no-reboot` (QEMU exits on the first guest reset instead of
looping) and tees the serial console to `tasmota-qemu/qemu.log`. To symbolize the last
panic:

```bash
./tasmota-qemu.sh decode                 # decodes tasmota-qemu/qemu.log
./tasmota-qemu.sh decode some-other.log  # decodes a specific log
```

`decode` strips the `0xffffffff` sentinel frame, takes the PC of each `pc:sp` pair, and
runs `addr2line` against the matching `firmware.elf`. Frames in app flash
(`0x4018xxxx`+) are the useful ones. Addresses that resolve to `??` are inside the
closed-source WiFi/BT blobs (no symbols) — usually a sign the firmware touched radio
hardware the emulator doesn't model.

Is it QEMU or the firmware? Run `./tasmota-qemu.sh smoketest`: it builds a minimal Arduino
app with the *same* IDF/toolchain and boots it. If the minimal app runs, QEMU is fine
and the problem is in the Tasmota build.

## Web UI: only `/` loads, sub-pages give `ERR_EMPTY_RESPONSE`

This is expected on first use and is **not** a crash. Tasmota's referer check only
accepts a `Referer` whose host is its own hostname or local IP (`10.0.2.15`). Through
the host port-forward your browser sends `Referer: http://localhost:8080/`, whose host
is `localhost` — matching neither — so every page past `/` is dropped with an empty
response.

With `NET_WIFI=1` this is handled automatically: a background watcher waits for the web
server to come up and issues `SetOption128 1` for you, so the UI is clickable out of the
box. If you ever need to do it by hand (e.g. you ran a plain `run` on a fresh image):

```bash
./tasmota-qemu.sh fixui          # sends SetOption128 1 (disable referer check)
```

The setting persists to flash. A fresh `make-image` wipes flash, but the next
`NET_WIFI=1` run re-applies it automatically.

## Caveats

- The `wifi` fork is older than the firmware's IDF; its RF-register model is good enough
  to associate but is not a full WiFi stack.
- `stock` + a real Tasmota build will boot to a serial console but the web UI will not
  respond (no WiFi emulation). Use `NET_WIFI=1` for network access.
- Stop a running QEMU with `Ctrl-A` then `X`, or:
  `for p in $(pgrep -f qemu-system-xtensa); do kill -9 $p; done`
