# JBD/Xiaoxiang BMS over BLE

Custom driver for this fork that reads Jiabaida (JBD/Xiaoxiang) battery BMS units over BLE, using the generic ESP32 BLE driver (`xdrv_79_esp32_ble.ino`) as its transport. Tested on a WT32-ETH01 board against multiple real JBD-based LiFePO4 packs.

- Driver: `tasmota/tasmota_xsns_sensor/xsns_119_jbd_bms_ble.ino`
- BLE transport (shared with other BLE drivers): `tasmota/tasmota_xdrv_driver/xdrv_79_esp32_ble.ino`
- Enabled via `tasmota/user_config_override.h`:
  ```cpp
  #define USE_BLE_ESP32
  #define USE_JBD_BMS_BLE
  ```

## What it does

- Polls up to **4 BMS units at once** (`JBD_BMS_MAX_UNITS`), one BLE connection at a time, each unit's own poll cadence naturally staggering them.
- Each poll reads three JBD registers per unit: basic info (voltage/current/capacity/cycles/protection/MOS status/temps), per-cell voltages, and the device/version string.
- Publishes everything as JSON (`JBDBMS1`, `JBDBMS2`, ...) via the normal Tasmota `Status 10` / `tele/.../SENSOR` mechanism.
- Renders a live table per unit on the Tasmota main web page, plus a collapsible picker listing nearby BLE devices with one-click Add/Remove.
- Can optionally push every field to an external HTTP endpoint (`WebSend`-style) the moment fresh data arrives, per unit.

## Requirements / gotchas specific to this BMS

- The JBD BLE module **ignores a single write** — the protocol needs the same write command resent 1-3 times within one connection (~2s apart) before it actually replies with a notify. This is implemented generically in the BLE driver via `writerepeat`/`writerepeatinterval` (see `BLEOp`'s `q:`/`i:` params), and the JBD driver sets `writerepeat=3`, `writerepeatinterval=2000` by default.
- Only one Bluetooth client can hold a connection to the BMS at a time — if the JBD phone app is connected, the ESP32 will fail to connect (`FAILCONNECT`). Close the app before testing/polling.

## Console commands

| Command | Effect |
|---|---|
| `JBDBMSAdd <mac>[/type]` | Track a BMS in the next free slot (up to 4). Re-running on an already-tracked MAC just re-syncs it. |
| `JBDBMSDel <mac>[/type]` | Stop tracking that MAC, freeing its slot. |
| `JBDBMSAddr [<mac>[/type]]` | Legacy single-device command: with an argument, **clears every tracked slot** and tracks only this one in slot 1. With no argument, shows slot 1's address. Prefer `Add`/`Del` for more than one unit. |
| `JBDBMSPoll` | Poll all tracked, idle units now (each still serialized one at a time). |
| `JBDBMSPeriod [<seconds>]` | Shared auto-poll interval for all tracked units (default 60, max 3600). |
| `JBDBMSStatus` | Full JSON for every tracked unit, same shape as what lands in `Status 10`. |
| `JBDBMSWebhook<N> [<target>\|0]` | Per-unit webhook target, e.g. `JBDBMSWebhook1 [192.168.1.50:7580]/batteryInfo`. `N` matches the `JBDBMSn` JSON key (1-4); no `N` = unit 1. Pass literal `0` to disable (an empty argument just queries the current value). |

MAC address format: `AABBCCDDEEFF` or `AA:BB:CC:DD:EE:FF`, optional `/<type>` suffix (0-3, defaults to 0 = public).

### Persistence

Tracked units, `JBDBMSPeriod`, and every unit's webhook target are saved to `/jbdbms.cfg` on the filesystem partition (`USE_UFILESYS`, not the shared Tasmota Settings struct) and restored automatically at boot. The file is written on every `JBDBMSAdd`/`JBDBMSDel`/`JBDBMSAddr`/`JBDBMSPeriod`/`JBDBMSWebhook<N>` call, so no extra step is needed after setting things up once. Per-poll runtime state (voltage, cell data, poll/error counters, etc.) is never persisted - that's always re-read from the BMS after boot.

### BLE-level debug (shared with all BLE drivers)

Verbose BLE internals (subscribe/write details, per-fragment notify logs) are silent by default. Turn them on temporarily with:
```
BLEDebug2 3
```
(`BLEDebug<class> <level>`, class 2 = info-level BLE logs, level 3 = show at DEBUG). Set back to `BLEDebug2 0` when done.

## JSON fields (`JBDBMS1`, `JBDBMS2`, ...)

```json
{"JBDBMS1":{
  "Addr":"A5C239328860/0","Configured":1,"Active":0,"Polls":12,"Errors":0,"LastState":7,"Age":8,
  "Voltage":13.37,"Current":0,"Power":0,
  "SOC":99,"Capacity":99.26,"FullCapacity":100,"Cycles":1,
  "Protection":"0x0000","SWVersion":42,
  "ChargeMOS":1,"DischargeMOS":1,"Balancing":0,"BalanceBits":"0x00000000",
  "ProductionDate":"2026-03-12",
  "Temp1":25.7,
  "Cells":4,"CellMin":3336,"CellMax":3349,"CellDelta":13,
  "Cell1":3338,"Cell2":3336,"Cell3":3349,"Cell4":3347,
  "Version":"DP04S007L4S100A"
}}
```

- `Age` (seconds since last successful poll) and `Errors` are worth watching for a stale/failing unit.
- Floats are real JSON numbers, not strings — safe to use directly in Rules comparisons.

## Web UI

The main Tasmota page (`http://<device-ip>/`) shows, per tracked unit, a compact two-metric-per-row table (voltage/current, power/charge, capacity/cycles, MOS status/balancing, cell voltages, temperature, last-update age), with no console needed.

Below that, a collapsible **"N tracked, M seen - tap to manage"** section lists BLE devices the scanner has recently picked up (MAC + advertised name + RSSI where known), each with an **Add** or **Remove** link — clicking either updates the tracked list and refreshes in place (no page navigation). The open/closed state of that panel survives the page's periodic data refresh.

To hide all of the above from the main page entirely (data/JSON/webhook keep working regardless), go to **Configuration → Configure BLE** (`http://<device-ip>/ble`) and uncheck **"Show JBD in main web"**, then Save. This is a third checkbox on the same "Bluetooth Settings" panel as "Enable Bluetooth"/"Enable Active Scan", persisted alongside the rest of the JBD config (see Persistence above). If `/ble` 404s right after a reboot, visit **Configuration** once first — Tasmota only registers that page's handler once the Configuration page itself has been rendered at least once (or once BLE finishes initializing, whichever comes first).

## Rules

Tasmota Rules can trigger on any field via the usual `ON JBDBMS1#Field<op>value DO ... ENDON` syntax, and `%value%` resolves to whatever field matched the trigger:

```
Rule1 ON JBDBMS1#Voltage<13.0 DO Publish stat/%topic%/ALERT LowVoltage:%value% ENDON
Rule1 ON JBDBMS1#Power>50 DO Publish stat/%topic%/ALERT HighPower:%value% ENDON
Rule1 1
Rule1 5
```
(`RuleN 5` = "Once" mode — fires once per new matching value instead of every poll cycle the condition stays true; drop it if you want a repeated alert instead.)

**Important limitation:** `%value%` only ever resolves to the *single field that matched the trigger*. There is **no** `%OtherKey#Field%` cross-reference into sibling fields of the same JSON object in stock Tasmota Rules — `%JBDBMS1#Current%` inside a rule triggered by `Voltage` will NOT substitute; it's passed through as literal text. For anything that needs *multiple* fields together (a combined alert message, a full webhook payload), either:
- use `JBDBMSWebhook<N>` (below) — it builds the full payload in C++ with real values, or
- capture each field you need into its own `VarN`/`MemN` via separate small rules, then combine `%VarN%` references in a final rule.

## Webhook (send everything, per unit)

`JBDBMSWebhook<N> <target>` fires automatically right after unit N finishes a fresh poll (all three sub-reads succeeded), building and executing:
```
WebSend <target>?addr=...&voltageFull=...&current=...&power=...&soc=...&capacity=...&fullCapacity=...
        &cycles=...&chargeMos=...&dischargeMos=...&balancing=...
        &cell1=...&cell2=...&...&cellMin=...&cellMax=...&cellDelta=...
        &temp1=...&protection=0x....&version=...
```
`<target>` is whatever you'd normally pass to Tasmota's own `WebSend` command, e.g. `[192.168.1.50:7580]/batteryInfo` (bracket syntax for a raw host:port not otherwise known to Tasmota). Every value is a real number/string from the driver's own struct fields — no Rules templating involved, so this is the reliable way to get a complete snapshot out over HTTP. Verified end-to-end against a real receiving server logging correct, distinct data for two simultaneously-tracked units.

Example, two units to two different servers:
```
JBDBMSWebhook1 [192.168.1.50:7580]/batteryInfo
JBDBMSWebhook2 [192.168.1.60:7580]/batteryInfo
```
Or both units to the same server (differentiate by the `addr=` query param):
```
JBDBMSWebhook1 [192.168.1.50:7580]/batteryInfo
JBDBMSWebhook2 [192.168.1.50:7580]/batteryInfo
```

## Adding more fields / a new BMS command

The three JBD sub-commands (basic info `0x03`, cell voltages `0x04`, hardware/version `0x05`) and their parsing live in `JbdBmsParseBasic`/`JbdBmsParseCells`/`JbdBmsParseVersion`. To decode more of the basic-info frame (it's a standard JBD protocol response — see `JbdBmsParseBasic` for the current offset map), add a field to `JbdBmsBleData`, parse it there, and expose it in both `JbdBmsAppendJsonOne` (JSON) and `JbdBmsShowOne` (web UI) and, if wanted, `JbdBmsFireWebhook` (webhook query string).

**A build gotcha worth knowing**: functions that are visible sketch-wide (i.e. not `static`, not local) must not take a pointer to a struct/class type defined in the same `.ino` file as a parameter — Tasmota's build auto-generates function prototypes near the top of the merged translation unit, *before* that struct is visible there, and the build fails with `'StructName' was not declared in this scope`. Pass an `int`/index instead and look the struct up inside the function body (see `JbdBmsParseBasic(int slot, ...)` for the pattern).
