#!/usr/bin/env bash
#
# tasmota-qemu.sh
# Build Tasmota (ESP32) and run it under the Espressif QEMU fork on an amd64 host.
#
# Pipeline:
#   1. build       — compile a Tasmota ESP32 firmware with PlatformIO
#   2. make-image  — pad the .factory.bin into a valid QEMU flash image
#   3. qemu-setup  — clone + build the chosen QEMU fork (xtensa-softmmu)
#   4. run         — boot the firmware in QEMU
#   all            — do 1->4 in sequence (default)
#
# Usage:
#   ./tasmota-qemu.sh [build|make-image|qemu-setup|run|smoketest|decode|all]
#
# Networked Tasmota with a browsable web UI in one command:
#   NET_WIFI=1 ./tasmota-qemu.sh        # then open http://localhost:8080
#
# Everything is configurable via environment variables (see the block below),
# e.g.:   QEMU_VARIANT=wifi NET=eth ./tasmota-qemu.sh all
#
# Full docs, env vars, and panic-debugging notes: see README-qemu.md
#
# ---------------------------------------------------------------------------
# IMPORTANT NETWORKING CAVEAT
#   Stock Tasmota expects WiFi. The stock Espressif QEMU fork has NO WiFi, only
#   an emulated OpenCores Ethernet MAC (open_eth). So with QEMU_VARIANT=stock the
#   firmware boots fine and you get a full SERIAL console, but the web UI / MQTT
#   will NOT be reachable over the network because Tasmota never brings up an
#   interface it recognises.
#
#   For actual network access use QEMU_VARIANT=wifi (the Ebiroll/calib fork,
#   which adds an 'esp32_wifi' NIC model). Exact NIC flags drift between fork
#   revisions — if networking doesn't come up, check that fork's README and
#   adjust the -nic line in run_qemu().
# ---------------------------------------------------------------------------

set -euo pipefail

# ----------------------------- configuration -------------------------------
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# This script lives at <repo>/tools/qemu/, so the repo root is two levels up.
TASMOTA_SRC="${TASMOTA_SRC:-$(cd "$SCRIPT_DIR/../.." && pwd)}"  # Tasmota checkout to build
                                              #   (default: this repo — your local changes, no clone)
WORKDIR="${WORKDIR:-$TASMOTA_SRC/tasmota-qemu}"  # qemu builds, flash images, venv (gitignored)
TASMOTA_ENV="${TASMOTA_ENV:-tasmota32}"      # PlatformIO env (must be an ESP32 one)
MINIMAL="${MINIMAL:-0}"                       # 1 = build the stripped safeboot env (fewest
                                              #     IDF drivers -> fewest pre-app init fns;
                                              #     test if a Tasmota driver init breaks QEMU)
MINIMAL_ENV="${MINIMAL_ENV:-tasmota32-safeboot}"
[ "$MINIMAL" = "1" ] && TASMOTA_ENV="$MINIMAL_ENV"   # overrides TASMOTA_ENV; keys whole pipeline

# NET_WIFI=1 is the one-shot "networked Tasmota" recipe proven to reach the web UI:
#   wifi QEMU fork  +  unicore solo1 build (dodges the dual-core esp_timer crash)
#   +  station creds for the fork's emulated open AP -> slirp DHCP -> hostfwd works.
NET_WIFI="${NET_WIFI:-0}"
NET_WIFI_ENV="${NET_WIFI_ENV:-tasmota32solo1}"  # full-feature unicore env that does STA
WIFI_SSID="${WIFI_SSID:-Open Wifi}"             # calib fork's emulated AP (esp32_wifi_ap.c)
WIFI_PASS="${WIFI_PASS:-}"                       # that AP is open (no PSK)
if [ "$NET_WIFI" = "1" ]; then
  QEMU_VARIANT="wifi"
  [ "$TASMOTA_ENV" = "tasmota32" ] && TASMOTA_ENV="$NET_WIFI_ENV"   # only if still default
fi
FLASH_SIZE="${FLASH_SIZE:-4MB}"              # QEMU esp32 accepts ONLY 2MB|4MB|8MB|16MB
QEMU_VARIANT="${QEMU_VARIANT:-stock}"        # stock | wifi
NET="${NET:-eth}"                            # none | eth  (eth = add an emulated NIC)
WEB_PORT_HOST="${WEB_PORT_HOST:-8080}"       # host port forwarded to guest :80 (web UI)
DISABLE_WDT="${DISABLE_WDT:-1}"              # 1 = disable TG watchdogs (avoids boot reset loops)
JOBS="${JOBS:-$(nproc)}"
INSTALL_DEPS="${INSTALL_DEPS:-0}"            # 1 = apt-get the QEMU build deps (needs sudo)
USE_SYSTEM_QEMU="${USE_SYSTEM_QEMU:-0}"      # 1 = use qemu-system-xtensa already on PATH

# QEMU source repos for the two paths
QEMU_STOCK_REPO="https://github.com/espressif/qemu.git"
QEMU_STOCK_BRANCH="esp-develop"
QEMU_WIFI_REPO="https://github.com/Ebiroll/calib.git"
QEMU_WIFI_BRANCH="master"
ESP32_ROM_URL="https://raw.githubusercontent.com/espressif/qemu/esp-develop/pc-bios/esp32-r0-rom.bin"

QEMU_BIN=""   # resolved by setup_qemu()

# ------------------------------- helpers -----------------------------------
log() { printf '\033[1;32m[+]\033[0m %s\n' "$*"; }
err() { printf '\033[1;31m[!]\033[0m %s\n' "$*" >&2; }

flash_bytes() {
  case "$1" in
    2MB)  echo $(( 2 * 1024 * 1024 ));;
    4MB)  echo $(( 4 * 1024 * 1024 ));;
    8MB)  echo $(( 8 * 1024 * 1024 ));;
    16MB) echo $(( 16 * 1024 * 1024 ));;
    *) err "Invalid FLASH_SIZE '$1' (use 2MB|4MB|8MB|16MB)"; exit 1;;
  esac
}

download() {  # download <url> <dest>
  if command -v curl >/dev/null; then curl -fsSL "$1" -o "$2"
  elif command -v wget >/dev/null; then wget -qO "$2" "$1"
  else err "need curl or wget to fetch $1"; exit 1; fi
}

# Write/refresh the WiFi station creds into a managed block in
# user_config_override.h so the build joins the emulated AP (NET_WIFI=1).
ensure_wifi_creds() {
  local ovr="$TASMOTA_SRC/tasmota/user_config_override.h"
  local b="// >>> run-it.sh NET_WIFI creds (managed) >>>"
  local e="// <<< run-it.sh NET_WIFI creds (managed) <<<"
  local block
  block="$(cat <<EOF
$b
#undef  CFG_HOLDER
#define CFG_HOLDER        4618               // bump forces SECTION1 (Wi-Fi) to defaults
#undef  STA_SSID1
#define STA_SSID1         "$WIFI_SSID"
#undef  STA_PASS1
#define STA_PASS1         "$WIFI_PASS"
#undef  WIFI_CONFIG_TOOL
#define WIFI_CONFIG_TOOL  WIFI_RETRY         // keep retrying STA, no captive-AP fallback
$e
EOF
)"
  if [ ! -f "$ovr" ]; then
    printf '#ifndef _USER_CONFIG_OVERRIDE_H_\n#define _USER_CONFIG_OVERRIDE_H_\n\n%s\n\n#endif  // _USER_CONFIG_OVERRIDE_H_\n' "$block" > "$ovr"
    log "Wrote WiFi creds override (SSID '$WIFI_SSID'): $ovr"
    return
  fi
  # strip any previous managed block, then insert fresh before the guard #endif
  awk -v b="$b" -v e="$e" '
    $0==b {skip=1} skip && $0==e {skip=0; next} !skip' "$ovr" > "$ovr.tmp"
  awk -v block="$block" '
    /#endif[[:space:]]*\/\/[[:space:]]*_USER_CONFIG_OVERRIDE_H_/ && !done {print block; done=1}
    {print}' "$ovr.tmp" > "$ovr"
  rm -f "$ovr.tmp"
  log "Refreshed WiFi creds override (SSID '$WIFI_SSID'): $ovr"
}

# --------------------------- step 1: build ---------------------------------
build_tasmota() {
  [ -f "$TASMOTA_SRC/platformio.ini" ] \
    || { err "TASMOTA_SRC ($TASMOTA_SRC) is not a Tasmota checkout (no platformio.ini)"; exit 1; }
  log "Building env '$TASMOTA_ENV' from local source $TASMOTA_SRC (this pulls toolchains on first run) ..."
  mkdir -p "$WORKDIR"

  [ "$NET_WIFI" = "1" ] && ensure_wifi_creds

  # Keep the venv out of the source tree so the working copy stays clean.
  local venv="$WORKDIR/.venv"
  [ -d "$venv" ] || python3 -m venv "$venv"
  # shellcheck disable=SC1091
  source "$venv/bin/activate"
  pip install -q --upgrade pip
  pip install -q platformio
  ( cd "$TASMOTA_SRC" && pio run -e "$TASMOTA_ENV" )
  deactivate

  local factory="$TASMOTA_SRC/build_output/firmware/${TASMOTA_ENV}.factory.bin"
  [ -f "$factory" ] || { err "expected merged image not found: $factory"; exit 1; }
  log "Firmware built: $factory"
}

# ------------------------ step 2: make flash image -------------------------
make_image() {
  local size; size="$(flash_bytes "$FLASH_SIZE")"
  local img="$WORKDIR/flash_image.bin"
  local factory="$TASMOTA_SRC/build_output/firmware/${TASMOTA_ENV}.factory.bin"

  if [ -f "$factory" ]; then
    # .factory.bin already starts at offset 0x0 (bootloader + partitions + app);
    # just pad up to a flash size QEMU accepts. truncate zero-fills.
    cp -f "$factory" "$img"
    truncate -s "$size" "$img"
    log "Flash image ready: $img ($FLASH_SIZE, from factory.bin)"
    return
  fi

  # No factory image (e.g. the safeboot env ships app-only). Merge the raw parts
  # at the standard ESP32 offsets ourselves, same as smoketest.
  local bd="$TASMOTA_SRC/.pio/build/${TASMOTA_ENV}"
  local boot="$bd/bootloader.bin" part="$bd/partitions.bin" app="$bd/firmware.bin"
  local f
  for f in "$boot" "$part" "$app"; do
    [ -f "$f" ] || { err "no factory.bin and missing part: $f — run '$0 build' first"; exit 1; }
  done
  local bootapp0
  bootapp0="$(ls "$HOME"/.platformio/packages/framework-arduinoespressif32*/tools/partitions/boot_app0.bin 2>/dev/null | head -n1)"

  log "No factory.bin for '$TASMOTA_ENV' — merging raw parts (bootloader/partitions/app)."
  truncate -s "$size" "$img"   # zero-filled blank flash
  dd if="$boot" of="$img" bs=1024 seek=4  conv=notrunc status=none
  dd if="$part" of="$img" bs=1024 seek=32 conv=notrunc status=none
  [ -n "$bootapp0" ] && dd if="$bootapp0" of="$img" bs=1024 seek=56 conv=notrunc status=none
  dd if="$app"  of="$img" bs=1024 seek=64 conv=notrunc status=none
  log "Flash image ready: $img ($FLASH_SIZE, merged from parts)"
}

# ----------------------- step 3: build / find QEMU -------------------------
qemu_build_deps() {
  local pkgs=(git build-essential ninja-build pkg-config python3-venv \
              libglib2.0-dev libpixman-1-dev libgcrypt20-dev libslirp-dev \
              flex bison)
  if [ "$INSTALL_DEPS" = "1" ]; then
    log "Installing QEMU build dependencies (sudo) ..."
    sudo apt-get update
    sudo apt-get install -y "${pkgs[@]}"
  else
    log "QEMU build needs (Debian/Ubuntu): ${pkgs[*]}"
    log "    re-run with INSTALL_DEPS=1 to apt-get them automatically."
  fi
}

setup_qemu() {
  if [ "$USE_SYSTEM_QEMU" = "1" ]; then
    command -v qemu-system-xtensa >/dev/null \
      || { err "USE_SYSTEM_QEMU=1 but qemu-system-xtensa is not on PATH"; exit 1; }
    QEMU_BIN="$(command -v qemu-system-xtensa)"
    log "Using system QEMU: $QEMU_BIN"
    return
  fi

  local repo branch src
  case "$QEMU_VARIANT" in
    stock) repo="$QEMU_STOCK_REPO"; branch="$QEMU_STOCK_BRANCH"; src="$WORKDIR/qemu-stock";;
    wifi)  repo="$QEMU_WIFI_REPO";  branch="$QEMU_WIFI_BRANCH";  src="$WORKDIR/qemu-wifi";;
    *) err "Unknown QEMU_VARIANT '$QEMU_VARIANT' (use stock|wifi)"; exit 1;;
  esac

  QEMU_BIN="$src/build/qemu-system-xtensa"
  if [ -x "$QEMU_BIN" ]; then
    log "QEMU already built: $QEMU_BIN"
    return
  fi

  qemu_build_deps
  mkdir -p "$WORKDIR"
  if [ ! -d "$src/.git" ]; then
    log "Cloning $repo ($branch) — large, can take a while ..."
    git clone --depth 1 -b "$branch" "$repo" "$src"
  fi
  cd "$src"
  git submodule update --init --depth 1 || true

  # The 'wifi' fork needs an ESP32 ROM in pc-bios. Recent forks already ship one
  # (esp32-v3-rom.bin etc.), so only fetch if none is present. The old r0-rom URL
  # 404s now, so fall back to copying from the stock clone before downloading.
  if [ "$QEMU_VARIANT" = "wifi" ]; then
    mkdir -p "$src/pc-bios"
    if ls "$src"/pc-bios/esp32*rom*.bin >/dev/null 2>&1; then
      log "ESP32 ROM already in fork pc-bios ($(ls "$src"/pc-bios/esp32*rom*.bin | xargs -n1 basename | tr '\n' ' ')) — skipping fetch."
    elif ls "$WORKDIR"/qemu-stock/pc-bios/esp32*rom*.bin >/dev/null 2>&1; then
      log "Copying ESP32 ROM(s) from the stock clone."
      cp -f "$WORKDIR"/qemu-stock/pc-bios/esp32*rom*.bin "$src/pc-bios/"
    else
      download "$ESP32_ROM_URL" "$src/pc-bios/esp32-r0-rom.bin" \
        || { err "no ESP32 ROM in fork and download 404'd — fetch one into $src/pc-bios manually"; exit 1; }
    fi
  fi

  log "Configuring QEMU (headless, xtensa only) ..."
  ./configure --target-list=xtensa-softmmu \
      --enable-gcrypt --enable-slirp \
      --disable-strip --disable-user --disable-capstone --disable-vnc

  log "Compiling QEMU with $JOBS jobs ..."
  ninja -C build -j "$JOBS" qemu-system-xtensa
  [ -x "$QEMU_BIN" ] || { err "QEMU build failed: $QEMU_BIN not found"; exit 1; }
  log "Built QEMU: $QEMU_BIN"
}

# Kill any QEMU we previously launched against this WORKDIR, so the new run can
# grab the hostfwd port (avoids 'Could not set up host forwarding ... address
# already in use'). Matches only our own QEMU (cmdline references $WORKDIR); the
# script's own process has no 'qemu-system-xtensa' in its cmdline, so no self-kill.
kill_stale_qemu() {
  local pids
  pids="$(pgrep -f "qemu-system-xtensa.*$WORKDIR" 2>/dev/null || true)"
  [ -n "$pids" ] || return 0
  log "Stopping previous QEMU (pid: $(echo $pids | tr '\n' ' ')) to free port ${WEB_PORT_HOST} ..."
  # shellcheck disable=SC2086
  kill -9 $pids 2>/dev/null || true
  sleep 1
}

# ----------------------------- step 4: run ---------------------------------
run_qemu() {
  [ -f "$WORKDIR/flash_image.bin" ] || { err "flash_image.bin missing — run '$0 make-image' first"; exit 1; }
  if [ -z "$QEMU_BIN" ] || [ ! -x "$QEMU_BIN" ]; then setup_qemu; fi
  kill_stale_qemu

  # When running from a source build, point QEMU at its bios/rom directory.
  local src_dir pcbios=""
  src_dir="$(dirname "$(dirname "$QEMU_BIN")")"
  [ -d "$src_dir/pc-bios" ] && pcbios="-L $src_dir/pc-bios"

  local args=(-nographic -machine esp32
              -no-reboot
              -drive "file=$WORKDIR/flash_image.bin,if=mtd,format=raw")

  if [ "$DISABLE_WDT" = "1" ]; then
    args+=(-global driver=timer.esp32.timg,property=wdt_disable,value=true)
  fi

  if [ "$NET" = "eth" ]; then
    # stock fork -> OpenCores Ethernet MAC; wifi fork -> emulated WiFi NIC
    local nicmodel="open_eth"
    [ "$QEMU_VARIANT" = "wifi" ] && nicmodel="esp32_wifi"
    args+=(-nic "user,model=${nicmodel},hostfwd=tcp::${WEB_PORT_HOST}-:80")
    log "Network: ${nicmodel}, host :${WEB_PORT_HOST} -> guest :80"
    if [ "$QEMU_VARIANT" = "stock" ]; then
      err "NOTE: stock Tasmota uses WiFi; over open_eth the web UI likely won't"
      err "      respond. Use QEMU_VARIANT=wifi for real network access."
    fi
  elif [ "$NET" != "none" ]; then
    err "Unknown NET '$NET' (use none|eth)"; exit 1
  fi

  log "Web UI (if the guest link comes up): http://localhost:${WEB_PORT_HOST}"
  log "Quit QEMU with:  Ctrl-A  then  X"
  log "Serial log: $WORKDIR/qemu.log   (decode panic with: $0 decode)"
  echo
  # NET_WIFI: auto-run fixui once the web server is up, so the forwarded UI is
  # clickable with no manual step. A background watcher polls qemu.log while QEMU
  # stays in the foreground (keeps the interactive serial console / Ctrl-A X).
  if [ "$NET_WIFI" = "1" ]; then
    ( i=0
      while [ "$i" -lt 90 ]; do
        i=$((i+1)); sleep 1
        [ -f "$WORKDIR/qemu.log" ] || continue
        grep -qa 'Guru Meditation' "$WORKDIR/qemu.log" && break
        if grep -qa 'Web server active' "$WORKDIR/qemu.log"; then
          sleep 1; fix_webui || true; break
        fi
      done
    ) &
  fi
  # tee serial to qemu.log so a panic backtrace survives the scroll.
  # -no-reboot makes QEMU exit on the first guest reset, so the loop stops.
  # shellcheck disable=SC2086
  "$QEMU_BIN" $pcbios "${args[@]}" 2>&1 | tee "$WORKDIR/qemu.log"
}

# ------------------ make the forwarded web UI clickable --------------------
# Tasmota's referer check (xdrv_01_9_webserver.ino) only accepts a Referer whose
# host is its own hostname or local IP. Through the host port-forward the browser's
# Referer host is 'localhost', which matches neither, so every sub-page past '/'
# returns an empty response. SetOption128 1 disables that check; it persists to
# flash. We send it once using a Referer that DOES match the guest IP so the
# command itself is accepted. Run this after the web server is up (QEMU running).
GUEST_IP="${GUEST_IP:-10.0.2.15}"   # slirp hands the guest this by default
fix_webui() {
  local url="http://localhost:${WEB_PORT_HOST}"
  command -v curl >/dev/null || { err "curl needed for fixui"; exit 1; }
  log "Disabling referer check (SetOption128 1) via accepted referer http://$GUEST_IP/ ..."
  if curl -fsS -m 8 -H "Referer: http://$GUEST_IP/" "$url/cm?cmnd=SetOption128%201" ; then
    echo
    log "Done (persisted to flash). Reload $url — all sub-pages now work."
  else
    err "Request failed — is QEMU running with the web server up? ($url)"
    exit 1
  fi
}

# --------------------------- decode a panic --------------------------------
find_addr2line() {
  if command -v xtensa-esp32-elf-addr2line >/dev/null; then
    command -v xtensa-esp32-elf-addr2line; return
  fi
  local c
  # PlatformIO ships the binary as xtensa-esp32-elf-addr2line inside the
  # toolchain-xtensa-esp-elf (newer) or toolchain-xtensa-esp32 (older) package.
  for c in "$HOME"/.platformio/packages/toolchain-xtensa-esp*/bin/xtensa-esp32-elf-addr2line; do
    [ -x "$c" ] && { echo "$c"; return; }
  done
  return 1
}

decode_panic() {
  local logfile="${1:-$WORKDIR/qemu.log}"
  [ -f "$logfile" ] || { err "log not found: $logfile (run '$0 run' first)"; exit 1; }

  # Pick the matching ELF: smoke.log -> the smoketest build, else Tasmota.
  # The .elf stays in PlatformIO's build dir; build_output/ only gets the .bins.
  local elf
  case "$logfile" in
    *smoke*) elf="$WORKDIR/smoke/.pio/build/smoke/firmware.elf";;
    *)       elf="$TASMOTA_SRC/.pio/build/${TASMOTA_ENV}/firmware.elf";;
  esac
  [ -f "$elf" ] || { err "ELF not found: $elf (run the matching build first)"; exit 1; }

  local a2l; a2l="$(find_addr2line)" \
    || { err "xtensa-esp32-elf-addr2line not found (build once so PlatformIO installs the toolchain)"; exit 1; }

  # Pull the LAST 'Backtrace:' line, keep the PC half (before ':') of each pc:sp pair.
  local bt
  bt="$(grep -a 'Backtrace:' "$logfile" | tail -n1)" \
    || { err "no 'Backtrace:' line in $logfile"; exit 1; }

  local addrs
  addrs="$(echo "$bt" \
    | grep -oE '0x[0-9a-fA-F]{8}:0x[0-9a-fA-F]{8}' \
    | cut -d: -f1 \
    | grep -viE '^0xfffffff' )"   # drop the sentinel 0xffffffff/0xfffffffd frame

  [ -n "$addrs" ] || { err "no decodable addresses in backtrace: $bt"; exit 1; }

  log "ELF:  $elf"
  log "Tool: $a2l"
  log "Decoding last panic backtrace:"
  echo
  # shellcheck disable=SC2086
  "$a2l" -pfiaC -e "$elf" $addrs
}

# --------------------- smoketest: known-good baseline ----------------------
# Build a MINIMAL Arduino app with the SAME platform/IDF/toolchain Tasmota uses
# (no PSRAM, stock partitions, default sdkconfig) and boot it in the built QEMU.
# Bisects "QEMU/IDF baseline broken" vs "Tasmota config breaks it":
#   boots OK  -> QEMU fine, Tasmota's own config/PSRAM is the culprit
#   same panic-> QEMU/IDF baseline can't boot this IDF; fix QEMU, not Tasmota
smoketest() {
  local smoke="$WORKDIR/smoke" venv="$WORKDIR/.venv"
  [ -d "$venv" ] || { err "need the build venv+platform — run '$0 build' first"; exit 1; }

  # Reuse the exact espressif32 platform Tasmota pins, so IDF matches.
  local platform
  platform="$(grep -hoE 'https://github.com/tasmota/platform-espressif32/[^ ]+\.zip' \
                "$TASMOTA_SRC/platformio_tasmota32.ini" | head -n1)"
  [ -n "$platform" ] || { err "espressif32 platform URL not found in platformio_tasmota32.ini"; exit 1; }
  log "Smoketest platform (matches Tasmota): $platform"

  mkdir -p "$smoke/src"
  cat > "$smoke/platformio.ini" <<EOF
[env:smoke]
platform = $platform
framework = arduino
board = esp32dev
board_build.flash_size = ${FLASH_SIZE}
board_build.partitions = default.csv
monitor_speed = 115200
EOF

  cat > "$smoke/src/main.cpp" <<'EOF'
#include <Arduino.h>
void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println();
  Serial.println("=== QEMU SMOKETEST: minimal Arduino app booted OK ===");
}
void loop() {
  Serial.println("SMOKE alive");
  delay(1000);
}
EOF

  # shellcheck disable=SC1091
  source "$venv/bin/activate"
  ( cd "$smoke" && pio run -e smoke )

  local bd="$smoke/.pio/build/smoke"
  local boot="$bd/bootloader.bin" part="$bd/partitions.bin" app="$bd/firmware.bin"
  local f
  for f in "$boot" "$part" "$app"; do
    [ -f "$f" ] || { err "smoketest artifact missing: $f"; deactivate; exit 1; }
  done

  # otadata image (optional — bootloader falls back to the factory app without it)
  local bootapp0
  bootapp0="$(ls "$HOME"/.platformio/packages/framework-arduinoespressif32*/tools/partitions/boot_app0.bin 2>/dev/null | head -n1)"

  deactivate

  # Merge by hand at the standard ESP32 flash offsets (no esptool dependency).
  # All offsets are 4 KiB-aligned, so dd with bs=1024 + seek-in-KiB is exact.
  # bootloader 0x1000, partitions 0x8000, otadata 0xe000, factory app 0x10000.
  local img="$WORKDIR/smoke_flash.bin"
  truncate -s "$(flash_bytes "$FLASH_SIZE")" "$img"   # zero-filled blank flash
  dd if="$boot" of="$img" bs=1024 seek=4  conv=notrunc status=none
  dd if="$part" of="$img" bs=1024 seek=32 conv=notrunc status=none
  [ -n "$bootapp0" ] && dd if="$bootapp0" of="$img" bs=1024 seek=56 conv=notrunc status=none
  dd if="$app"  of="$img" bs=1024 seek=64 conv=notrunc status=none
  log "Smoketest image: $img ($FLASH_SIZE)"

  if [ -z "$QEMU_BIN" ] || [ ! -x "$QEMU_BIN" ]; then setup_qemu; fi
  kill_stale_qemu
  local src_dir pcbios=""
  src_dir="$(dirname "$(dirname "$QEMU_BIN")")"
  [ -d "$src_dir/pc-bios" ] && pcbios="-L $src_dir/pc-bios"

  local args=(-nographic -machine esp32 -no-reboot
              -drive "file=$img,if=mtd,format=raw")
  [ "$DISABLE_WDT" = "1" ] && args+=(-global driver=timer.esp32.timg,property=wdt_disable,value=true)

  log "Booting minimal app (Ctrl-A X to quit). Serial -> $WORKDIR/smoke.log"
  log "  'SMOKETEST ... booted OK' -> QEMU fine, Tasmota config is the culprit."
  log "  same InstrFetchProhibited panic -> QEMU/IDF baseline broken, fix QEMU."
  log "Decode a smoke panic with:  $0 decode $WORKDIR/smoke.log"
  echo
  # shellcheck disable=SC2086
  "$QEMU_BIN" $pcbios "${args[@]}" 2>&1 | tee "$WORKDIR/smoke.log"
}

# ------------------------------- usage -------------------------------------
usage() {
  cat <<EOF
tasmota-qemu.sh — build & run Tasmota (ESP32) under QEMU on amd64

Commands:
  build        Compile Tasmota firmware from \$TASMOTA_SRC (PlatformIO env: \$TASMOTA_ENV)
  make-image   Pad the .factory.bin into a QEMU flash image
  qemu-setup   Clone + build the chosen QEMU fork
  run          Boot the firmware in QEMU (serial tee'd to \$WORKDIR/qemu.log)
  smoketest    Boot a minimal Arduino app (same IDF, no PSRAM) to bisect QEMU vs Tasmota
  fixui        Disable Tasmota referer check (SetOption128 1) so forwarded sub-pages work
  decode [log] Symbolize the last panic backtrace from a log (default: qemu.log)
  all          build -> make-image -> qemu-setup -> run   (default)

Key environment variables (current defaults):
  WORKDIR=$WORKDIR
  TASMOTA_SRC=$TASMOTA_SRC
                            Tasmota checkout to build (default: this repo — your changes)
  TASMOTA_ENV=$TASMOTA_ENV          ESP32 PlatformIO env
  MINIMAL=$MINIMAL                  1 = build $MINIMAL_ENV (stripped, fewest drivers)
  NET_WIFI=$NET_WIFI                 1 = one-shot networked recipe (wifi fork + $NET_WIFI_ENV
                            + emulated-AP creds); browse http://localhost:$WEB_PORT_HOST
  WIFI_SSID=$WIFI_SSID         emulated AP SSID joined when NET_WIFI=1
  FLASH_SIZE=$FLASH_SIZE            2MB|4MB|8MB|16MB only
  QEMU_VARIANT=$QEMU_VARIANT        stock | wifi
  NET=$NET                  none | eth
  WEB_PORT_HOST=$WEB_PORT_HOST       host port -> guest :80
  DISABLE_WDT=$DISABLE_WDT             1 = disable timer-group watchdogs
  INSTALL_DEPS=$INSTALL_DEPS            1 = apt-get QEMU build deps (sudo)
  USE_SYSTEM_QEMU=$USE_SYSTEM_QEMU         1 = use qemu-system-xtensa from PATH
  JOBS=$JOBS

Examples:
  ./tasmota-qemu.sh                       # full pipeline, stock fork, serial console
  QEMU_VARIANT=wifi ./tasmota-qemu.sh     # build the WiFi-capable fork and run networked
  NET=none ./tasmota-qemu.sh run          # just boot, serial only (re-run without rebuilding)
  INSTALL_DEPS=1 QEMU_VARIANT=wifi ./tasmota-qemu.sh
  NET_WIFI=1 ./tasmota-qemu.sh            # networked Tasmota, web UI at localhost:$WEB_PORT_HOST
EOF
}

# ------------------------------- dispatch ----------------------------------
mkdir -p "$WORKDIR"
case "${1:-all}" in
  build)              build_tasmota;;
  make-image|image)   make_image;;
  qemu-setup|qemu)    setup_qemu;;
  run)                run_qemu;;
  smoketest|smoke)    smoketest;;
  fixui|fix-webui)    fix_webui;;
  decode)             shift; decode_panic "${1:-}";;
  all)                build_tasmota; make_image; setup_qemu; run_qemu;;
  -h|--help|help)     usage;;
  *) err "Unknown command '${1}'"; usage; exit 1;;
esac

