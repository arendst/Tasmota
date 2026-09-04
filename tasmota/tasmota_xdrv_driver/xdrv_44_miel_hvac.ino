/*
  xdrv_44_miel_hvac.ino - Mitsubishi Electric HVAC support for Tasmota

  Copyright (C) 2021  David Gwynne <david@gwynne.id.au>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_MIEL_HVAC
/*********************************************************************************************\
 * Mitsubishi Electric HVAC (CN105) serial interface
 *
 * Speaks the Mitsubishi "IT protocol" on the indoor unit's CN105 connector and exposes it
 * through the console (HVACSet* commands), MQTT (SENSOR / HVACSettings) and a climate
 * control panel on the web UI main page.  Protocol reference:
 * https://muart-group.github.io/developer/it-protocol/
 * Compile with USE_MIEL_HVAC; GPIOs "MiEl HVAC Rx" / "MiEl HVAC Tx".
 *
 * --- Modbus RTU slave (USE_MIEL_HVAC_MODBUS_SLAVE, ESP32) ---------------------------------
 * Optional second RS485 port that mirrors every driver state as read registers and maps
 * every driver function to write registers / coils, so the unit can be driven from a PLC
 * alongside the console / MQTT.  Off by default.
 *
 *   GPIOs      "MiEl HVAC MB Rx" / "MiEl HVAC MB Tx", and optionally "MiEl HVAC MB Tx En"
 *              (RS485 DE/RE - omit for auto-direction transceivers)
 *   Commands   HVACModbus 0|1, HVACModbusAddress 1..247, HVACModbusBaudrate 1200..115200,
 *              HVACModbusConfig 8N1|8E1|8O1|8N2|8E2|8O2   (persisted, applied live)
 *   Function   0x01/0x02 read coils / discrete inputs, 0x03/0x04 read holding / input
 *   codes      registers, 0x05/0x0F/0x06/0x10 write coils / registers; CRC-16 checked,
 *              broadcast (address 0) accepted for writes
 *   Framing    requests are framed by their expected length rather than the T3.5 gap, the
 *              RX stream resyncs byte-by-byte on a mismatch, replies wait the RTU
 *              turnaround silence and are dropped once the master has already re-polled -
 *              keeps a shared bus and rates above 9600 baud reliable
 *
 *   Input registers (FC04), 16-bit, 0x0000..0x00ff - read-only live state:
 *     0x0000..0x0006  link / capability / feature flags
 *     0x0010..0x001a  settings: power, mode, temp x10, fan, vane, widevane, prohibit,
 *                     air direction, purifier, night mode, econocool
 *     0x0020..0x002a  room / outdoor / set temperature x10, power W, energy, run time,
 *                     compressor, remote temperature, clear time
 *     0x0030..0x003a  timers and operation stage
 *     0x0040..0x0048  decoded capabilities and per-mode temperature limits
 *     0x0050..0x0053  diagnostics: requests, CRC errors, exceptions, RX overruns
 *
 *   Holding registers (FC03 / FC06 / FC10), 16-bit:
 *     0x0000..0x000e  writable control - power, mode, temp x10, fan, vane, widevane,
 *                     prohibit, air direction, purifier, night mode, econocool, HA mode,
 *                     remote temp (0x7fff clears), remote-temp clear time, raw 0x42 byte;
 *                     reads return the last written value
 *     0x000f..0x0017  read-only mirror of selected input registers, for FC03-only masters
 *
 *   Coils (FC01 / FC05 / FC0F): 0 power, 1 purifier, 2 night mode, 3 econocool,
 *                               4 clear remote-temp override (write 0)
 *   Discrete inputs (FC02): 0 connected, 1 capabilities valid, 2 compressor running,
 *                           3 i-See sensor, 4 energy metering, 5 remote temp active,
 *                           6 defrost
 *
 *   Writes reuse the miel_hvac_apply_* setters (same capability gating as the console
 *   commands) and are queued when the HVAC link is not up yet rather than rejected.
\*********************************************************************************************/

#define XDRV_44 44

#ifndef nitems
#define nitems(_a) (sizeof((_a)) / sizeof((_a)[0]))
#endif

// clang-format off
#ifndef CTASSERT
#define CTASSERT(x)		extern char  _ctassert[(x) ? 1 : -1 ]	\
				    __attribute__((__unused__))
#endif
// clang-format on

#define MIEL_HVAC_LOGNAME "MiElHVAC"

#define D_CMND_MIEL_HVAC_SETFANSPEED              "HVACSetFanSpeed"
#define D_CMND_MIEL_HVAC_SETMODE                  "HVACSetMode"
#define D_CMND_MIEL_HVAC_SETHAMODE                "HVACSetHAMode"
#define D_CMND_MIEL_HVAC_SETTEMP                  "HVACSetTemp"
#define D_CMND_MIEL_HVAC_SETSWINGV                "HVACSetSwingV"
#define D_CMND_MIEL_HVAC_SETSWINGH                "HVACSetSwingH"
#define D_CMND_MIEL_HVAC_SETAIRDIRECTION          "HVACSetAirDirection"
#define D_CMND_MIEL_HVAC_SETPROHIBIT              "HVACSetProhibit"
#define D_CMND_MIEL_HVAC_SETPURIFY                "HVACSetPurify"
#define D_CMND_MIEL_HVAC_SETNIGHTMODE             "HVACSetNightMode"
#define D_CMND_MIEL_HVAC_SETECONOCOOL             "HVACSetEconoCool"
#define D_CMND_MIEL_HVAC_REMOTETEMP               "HVACRemoteTemp"
#define D_CMND_MIEL_HVAC_REMOTETEMP_CLEAR_TIME "HVACRemoteTempClearTime"
#define D_CMND_MIEL_HVAC_SEND_COMMAND             "HVACSendCommand"
#define D_CMND_MIEL_HVAC_MODBUS                   "HVACModbus"
#define D_CMND_MIEL_HVAC_MODBUS_ADDRESS           "HVACModbusAddress"
#define D_CMND_MIEL_HVAC_MODBUS_BAUDRATE          "HVACModbusBaudrate"
#define D_CMND_MIEL_HVAC_MODBUS_CONFIG            "HVACModbusConfig"

#include <TasmotaSerial.h>


struct miel_hvac_header
{
	uint8_t start;
#define MIEL_HVAC_H_START 0xfc
	uint8_t type;
#define MIEL_HVAC_H_TYPE_UPDATED  0x61
#define MIEL_HVAC_H_TYPE_DATA     0x62
#define MIEL_HVAC_H_TYPE_CONNECTED 0x7a
	uint8_t middle1;
#define MIEL_HVAC_H_MIDDLE1 0x01
	uint8_t middle2;
#define MIEL_HVAC_H_MIDDLE2 0x30
	uint8_t len;
};

struct miel_hvac_data_settings
{
	uint8_t _pad1[2];
	uint8_t power;
#define MIEL_HVAC_SETTINGS_POWER_OFF 0x00
#define MIEL_HVAC_SETTINGS_POWER_ON  0x01
	uint8_t mode;
#define MIEL_HVAC_SETTINGS_MODE_MASK      0x7f
#define MIEL_HVAC_SETTINGS_MODE_HEAT      0x01
#define MIEL_HVAC_SETTINGS_MODE_DRY       0x02
#define MIEL_HVAC_SETTINGS_MODE_COOL      0x03
#define MIEL_HVAC_SETTINGS_MODE_FAN       0x07
#define MIEL_HVAC_SETTINGS_MODE_AUTO      0x08
#define MIEL_HVAC_SETTINGS_MODE_HEAT_ISEE 0x09
#define MIEL_HVAC_SETTINGS_MODE_DRY_ISEE  0x0a
#define MIEL_HVAC_SETTINGS_MODE_COOL_ISEE 0x0b
	uint8_t temp;
#ifndef MIEL_HVAC_SETTINGS_TEMP_MIN
#define MIEL_HVAC_SETTINGS_TEMP_MIN 10
#endif
#ifndef MIEL_HVAC_SETTINGS_TEMP_MAX
#define MIEL_HVAC_SETTINGS_TEMP_MAX 31
#endif
	uint8_t fan;
#define MIEL_HVAC_SETTINGS_FAN_AUTO  0x00
#define MIEL_HVAC_SETTINGS_FAN_QUIET 0x01
#define MIEL_HVAC_SETTINGS_FAN_1     0x02
#define MIEL_HVAC_SETTINGS_FAN_2     0x03
#define MIEL_HVAC_SETTINGS_FAN_3     0x05
#define MIEL_HVAC_SETTINGS_FAN_4     0x06
	uint8_t vane;
#define MIEL_HVAC_SETTINGS_VANE_AUTO       0x00
#define MIEL_HVAC_SETTINGS_VANE_1          0x01
#define MIEL_HVAC_SETTINGS_VANE_2          0x02
#define MIEL_HVAC_SETTINGS_VANE_3          0x03
#define MIEL_HVAC_SETTINGS_VANE_4          0x04
#define MIEL_HVAC_SETTINGS_VANE_5          0x05
#define MIEL_HVAC_SETTINGS_VANE_SWING      0x07
	uint8_t prohibit;
#define MIEL_HVAC_SETTINGS_PROHIBIT_OFF        0x00
#define MIEL_HVAC_SETTINGS_PROHIBIT_POWER      0x01
#define MIEL_HVAC_SETTINGS_PROHIBIT_MODE       0x02
#define MIEL_HVAC_SETTINGS_PROHIBIT_MODE_POWER 0x03
#define MIEL_HVAC_SETTINGS_PROHIBIT_TEMP       0x04
#define MIEL_HVAC_SETTINGS_PROHIBIT_TEMP_POWER 0x05
#define MIEL_HVAC_SETTINGS_PROHIBIT_TEMP_MODE  0x06
#define MIEL_HVAC_SETTINGS_PROHIBIT_ALL        0x07
	uint8_t _pad2[1];
	uint8_t widevane;
#define MIEL_HVAC_SETTINGS_WIDEVANE_MASK  0x0f
#define MIEL_HVAC_SETTINGS_WIDEVANE_LL    0x01
#define MIEL_HVAC_SETTINGS_WIDEVANE_L     0x02
#define MIEL_HVAC_SETTINGS_WIDEVANE_C     0x03
#define MIEL_HVAC_SETTINGS_WIDEVANE_R     0x04
#define MIEL_HVAC_SETTINGS_WIDEVANE_RR    0x05
#define MIEL_HVAC_SETTINGS_WIDEVANE_LC    0x06
#define MIEL_HVAC_SETTINGS_WIDEVANE_RC    0x07
#define MIEL_HVAC_SETTINGS_WIDEVANE_SPLIT 0x08
#define MIEL_HVAC_SETTINGS_WIDEVANE_SWING 0x0c
#define MIEL_HVAC_SETTINGS_WIDEVANE_ISEE  0x80
	uint8_t temp05;
	uint8_t _pad3[2];
	uint8_t airdirection;
/*
 * Airflow direction values (0x62 0x02 byte 14, 0x08 byte 6).
 * OFF is expressed as widevane=0x8c; no airdirection byte is used.
 */
#define MIEL_HVAC_SETTINGS_AIRDIRECTION_EVEN     0x00
#define MIEL_HVAC_SETTINGS_AIRDIRECTION_INDIRECT 0x01
#define MIEL_HVAC_SETTINGS_AIRDIRECTION_DIRECT   0x02
#define MIEL_HVAC_SETTINGS_AIRDIRECTION_OFF      0x03
};

struct miel_hvac_data_roomtemp
{
	uint8_t _pad1[2];
	uint8_t temp;
	uint8_t _pad2[1];
	uint8_t outdoortemp;
	uint8_t temp05;
	uint8_t settemp;
	uint8_t _pad3[3];
	/* 3-byte big-endian cumulative runtime, in minutes. */
	uint8_t operationtime;   /* most-significant byte */
	uint8_t operationtime1;
	uint8_t operationtime2;  /* least-significant byte */
};

struct miel_hvac_data_timers
{
	uint8_t _pad1[2];
	uint8_t mode;
#define MIEL_HVAC_TIMER_MODE_NONE 0x00
#define MIEL_HVAC_TIMER_MODE_OFF  0x01
#define MIEL_HVAC_TIMER_MODE_ON   0x02
#define MIEL_HVAC_TIMER_MODE_BOTH 0x03
	uint8_t onminutes;
	uint8_t offminutes;
	uint8_t onminutesremaining;
	uint8_t offminutesremaining;
};

struct miel_hvac_data_status
{
	uint8_t _pad1[2];
	uint8_t compressorfrequency;
	uint8_t compressor;
#define MIEL_HVAC_STATUS_COMPRESSOR_OFF 0x00
#define MIEL_HVAC_STATUS_COMPRESSOR_ON  0x01
	uint8_t operationpower;
	uint8_t operationpower1;
	uint8_t operationenergy;
	uint8_t operationenergy1;
};

struct miel_hvac_data_stage
{
	uint8_t _pad1[2];
	uint8_t operation;
#define MIEL_HVAC_STAGE_OPERATION_NORMAL   0x00
#define MIEL_HVAC_STAGE_OPERATION_SERVICE  0x01
#define MIEL_HVAC_STAGE_OPERATION_DEFROST  0x02
#define MIEL_HVAC_STAGE_OPERATION_UNKNOWN  0x03
#define MIEL_HVAC_STAGE_OPERATION_PREHEAT  0x04
#define MIEL_HVAC_STAGE_OPERATION_UNKNOWN1 0x05
#define MIEL_HVAC_STAGE_OPERATION_UNKNOWN2 0x06
#define MIEL_HVAC_STAGE_OPERATION_UNKNOWN3 0x07
#define MIEL_HVAC_STAGE_OPERATION_STANDBY  0x08
	uint8_t fan;
#define MIEL_HVAC_STAGE_FAN_OFF  0x00
#define MIEL_HVAC_STAGE_FAN_1    0x01
#define MIEL_HVAC_STAGE_FAN_2    0x02
#define MIEL_HVAC_STAGE_FAN_3    0x03
#define MIEL_HVAC_STAGE_FAN_4    0x04
#define MIEL_HVAC_STAGE_FAN_5    0x05
#define MIEL_HVAC_STAGE_FAN_QUIT 0x06
	uint8_t mode;
#define MIEL_HVAC_STAGE_MODE_DIRECT      0x00
#define MIEL_HVAC_STAGE_MODE_AUTO_FAN    0x01
#define MIEL_HVAC_STAGE_MODE_AUTO_HEAT   0x02
#define MIEL_HVAC_STAGE_MODE_AUTO_COOL   0x03
#define MIEL_HVAC_STAGE_MODE_AUTO_LEADER 0x04
};

/*
 * Response to request 0x42 (Get Options).
 * Documented at:
 * https://muart-group.github.io/developer/it-protocol/0x62-get-response/0x42-unknown-hvac-options
 *
 * The unit returns this inside a standard 0x62 data packet.
 * Byte layout (relative to the start of the data payload, i.e. after the
 * 0x62 type byte which occupies miel_hvac_data.type):
 *
 *   byte 1 (data.options.purifier)  – Air Purifier (Plasma Quad): 0=off 1=on
 *   byte 2 (data.options.nightmode) – Night Mode (dims LED):       0=off 1=on
 *   byte 3 (data.options.econocool)– EconoCool (COOL mode only):      0=off 1=on
 */
struct miel_hvac_data_options
{
	uint8_t _pad1[1];
	uint8_t purifier;
#define MIEL_HVAC_OPTIONS_PURIFIER_OFF 0x00
#define MIEL_HVAC_OPTIONS_PURIFIER_ON  0x01
	uint8_t nightmode;
#define MIEL_HVAC_OPTIONS_NIGHTMODE_OFF 0x00
#define MIEL_HVAC_OPTIONS_NIGHTMODE_ON  0x01
	uint8_t econocool;
#define MIEL_HVAC_OPTIONS_ECONOCOOL_OFF 0x00
#define MIEL_HVAC_OPTIONS_ECONOCOOL_ON  0x01
};

struct miel_hvac_data
{
	uint8_t type;
#define MIEL_HVAC_DATA_T_SETTINGS    0x02
#define MIEL_HVAC_DATA_T_ROOMTEMP    0x03
#define MIEL_HVAC_DATA_T_TIMERS      0x05
#define MIEL_HVAC_DATA_T_STATUS      0x06
#define MIEL_HVAC_DATA_T_STAGE       0x09
#define MIEL_HVAC_DATA_T_OPTIONS 0x42

	union
	{
		struct miel_hvac_data_settings     settings;
		struct miel_hvac_data_roomtemp     roomtemp;
		struct miel_hvac_data_timers       timers;
		struct miel_hvac_data_status       status;
		struct miel_hvac_data_stage        stage;
		struct miel_hvac_data_options      options;
		uint8_t bytes[15];
	} data;
};

CTASSERT(sizeof(struct miel_hvac_data) == 16);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.power)        == 3);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.mode)         == 4);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.temp)         == 5);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.fan)          == 6);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.vane)         == 7);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.prohibit)     == 8);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.widevane)     == 10);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.temp05)       == 11);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.airdirection) == 14);

CTASSERT(offsetof(struct miel_hvac_data, data.roomtemp.temp)           == 3);
CTASSERT(offsetof(struct miel_hvac_data, data.roomtemp.outdoortemp)    == 5);
CTASSERT(offsetof(struct miel_hvac_data, data.roomtemp.temp05)         == 6);
CTASSERT(offsetof(struct miel_hvac_data, data.roomtemp.settemp)        == 7);
CTASSERT(offsetof(struct miel_hvac_data, data.roomtemp.operationtime)  == 11);
CTASSERT(offsetof(struct miel_hvac_data, data.roomtemp.operationtime1) == 12);
CTASSERT(offsetof(struct miel_hvac_data, data.roomtemp.operationtime2) == 13);

CTASSERT(offsetof(struct miel_hvac_data, data.timers.mode)               == 3);
CTASSERT(offsetof(struct miel_hvac_data, data.timers.onminutes)           == 4);
CTASSERT(offsetof(struct miel_hvac_data, data.timers.offminutes)          == 5);
CTASSERT(offsetof(struct miel_hvac_data, data.timers.onminutesremaining)  == 6);
CTASSERT(offsetof(struct miel_hvac_data, data.timers.offminutesremaining) == 7);

CTASSERT(offsetof(struct miel_hvac_data, data.stage.operation) == 3);
CTASSERT(offsetof(struct miel_hvac_data, data.stage.fan)       == 4);
CTASSERT(offsetof(struct miel_hvac_data, data.stage.mode)      == 5);

CTASSERT(offsetof(struct miel_hvac_data, data.status.compressorfrequency) == 3);
CTASSERT(offsetof(struct miel_hvac_data, data.status.compressor)          == 4);
CTASSERT(offsetof(struct miel_hvac_data, data.status.operationpower)      == 5);
CTASSERT(offsetof(struct miel_hvac_data, data.status.operationpower1)     == 6);
CTASSERT(offsetof(struct miel_hvac_data, data.status.operationenergy)     == 7);
CTASSERT(offsetof(struct miel_hvac_data, data.status.operationenergy1)    == 8);

/* options: purifier=byte2, nightmode=byte3, econocool=byte4 */
CTASSERT(offsetof(struct miel_hvac_data, data.options.purifier)   == 2);
CTASSERT(offsetof(struct miel_hvac_data, data.options.nightmode)  == 3);
CTASSERT(offsetof(struct miel_hvac_data, data.options.econocool) == 4);

/* to hvac */

#define MIEL_HVAC_H_TYPE_CONNECT 0x5a
static const uint8_t miel_hvac_msg_connect[] = {0xca, 0x01};

#define MIEL_HVAC_H_TYPE_REQUEST 0x42

struct miel_hvac_msg_request
{
	uint8_t type;
#define MIEL_HVAC_REQUEST_SETTINGS    0x02
#define MIEL_HVAC_REQUEST_ROOMTEMP    0x03
#define MIEL_HVAC_REQUEST_TIMERS      0x05
#define MIEL_HVAC_REQUEST_STATUS      0x06
#define MIEL_HVAC_REQUEST_STAGE       0x09
#define MIEL_HVAC_REQUEST_OPTIONS 0x42
	uint8_t zero[15];
};

#define MIEL_HVAC_H_TYPE_UPDATE 0x41

struct miel_hvac_msg_update_settings
{
	uint8_t one;
	/*
	 * flags is big-endian on the wire. Always use htons() when setting or testing.
	 */
	uint16_t flags;
#define MIEL_HVAC_SETTINGS_F_WIDEVANE      (1 << 0)
#define MIEL_HVAC_SETTINGS_F_AIRDIRECTION  (1 << 3)
#define MIEL_HVAC_SETTINGS_F_POWER         (1 << 8)
#define MIEL_HVAC_SETTINGS_F_MODE          (1 << 9)
#define MIEL_HVAC_SETTINGS_F_TEMP          (1 << 10)
#define MIEL_HVAC_SETTINGS_F_FAN           (1 << 11)
#define MIEL_HVAC_SETTINGS_F_VANE          (1 << 12)
#define MIEL_HVAC_SETTINGS_F_PROHIBIT      (1 << 13)
	uint8_t power;
	uint8_t mode;
	uint8_t temp;
	uint8_t fan;
	uint8_t vane;
	uint8_t prohibit;
	uint8_t _pad1[4];
	uint8_t widevane;
	uint8_t temp05;
	uint8_t airdirection;
} __packed;

CTASSERT(sizeof(struct miel_hvac_msg_update_settings) == 16);
#define MIEL_HVAC_OFFS(_v) ((_v) - sizeof(struct miel_hvac_header))
CTASSERT(offsetof(struct miel_hvac_msg_update_settings, flags)        == MIEL_HVAC_OFFS(6));
CTASSERT(offsetof(struct miel_hvac_msg_update_settings, power)        == MIEL_HVAC_OFFS(8));
CTASSERT(offsetof(struct miel_hvac_msg_update_settings, mode)         == MIEL_HVAC_OFFS(9));
CTASSERT(offsetof(struct miel_hvac_msg_update_settings, temp)         == MIEL_HVAC_OFFS(10));
CTASSERT(offsetof(struct miel_hvac_msg_update_settings, fan)          == MIEL_HVAC_OFFS(11));
CTASSERT(offsetof(struct miel_hvac_msg_update_settings, vane)         == MIEL_HVAC_OFFS(12));
CTASSERT(offsetof(struct miel_hvac_msg_update_settings, prohibit)     == MIEL_HVAC_OFFS(13));
CTASSERT(offsetof(struct miel_hvac_msg_update_settings, widevane)     == MIEL_HVAC_OFFS(18));
CTASSERT(offsetof(struct miel_hvac_msg_update_settings, temp05)       == MIEL_HVAC_OFFS(19));
CTASSERT(offsetof(struct miel_hvac_msg_update_settings, airdirection) == MIEL_HVAC_OFFS(20));

struct miel_hvac_msg_update_remotetemp
{
	uint8_t seven;
	uint8_t control;
#define MIEL_HVAC_REMOTETEMP_CLR 0x00
#define MIEL_HVAC_REMOTETEMP_SET 0x01
	/* setting for older units expressed as .5C units starting at 8C */
	uint8_t temp_old;
#define MIEL_HVAC_REMOTETEMP_OLD_MIN    8
#define MIEL_HVAC_REMOTETEMP_OLD_MAX    38
#define MIEL_HVAC_REMOTETEMP_OLD_FACTOR 2
	/* setting for newer units expressed as .5C units starting at -63C */
	uint8_t temp;
#define MIEL_HVAC_REMOTETEMP_MIN    -63
#define MIEL_HVAC_REMOTETEMP_MAX    63
#define MIEL_HVAC_REMOTETEMP_OFFSET 64
#define MIEL_HVAC_REMOTETEMP_FACTOR 2
	uint8_t _pad2[12];
};

CTASSERT(sizeof(struct miel_hvac_msg_update_remotetemp) == 16);

/*
 * Command 0x08 — Set Run State.
 * Used to toggle Air Purifier, Night Mode and Airflow control mode (i-See).
 * EconoCool is reported via 0x42 and set via 0x08 flag 0x10 (HVACSetEconoCool).
 * Documented at:
 * https://muart-group.github.io/developer/it-protocol/0x41-set-request/0x08-set-run-state
 *
 * Wire layout (all within a 0x41 UPDATE packet, 16-byte payload):
 *   byte  0      = 0x08  (command type)
 *   bytes 1-2    = flags (big-endian), indicate which fields to update
 *   bytes 3-5    = reserved (zeroed)
 *   byte  6      = Airflow control mode (i-See)  (update flag 0x2000)
 *                  0=even, 1=indirect, 2=direct
 *                  Requires widevane=0x80 to be set via 0x01 first.
 *                  Confirmed on MSZ-LN##VG#* units with DIRECTION key on RC.
 *   bytes 7-11   = reserved (zeroed)
 *   byte 12      = Air Purifier  (update flag 0x0004)
 *   byte 13      = Night Mode    (update flag 0x0008)
 *   byte 14      = EconoCool (update flag 0x10)
 *   byte 15      = reserved
 */
struct miel_hvac_msg_update_runstate
{
	uint8_t  eight;           /* always 0x08 */
	/*
	 * flags is big-endian on the wire. Always use htons() when setting or testing.
	 */
	uint16_t flags;
#define MIEL_HVAC_RUNSTATE_F_AIRDIRECTION (1 << 13)  /* 0x2000 */
#define MIEL_HVAC_RUNSTATE_F_PURIFIER     (1 << 2)   /* 0x0004 */
#define MIEL_HVAC_RUNSTATE_F_NIGHTMODE    (1 << 3)   /* 0x0008 */
	uint8_t  _pad1[3];        /* bytes 3-5, zeroed */
	uint8_t  airdirection;    /* byte 6 — i-See airflow control mode */
	uint8_t  _pad2[5];        /* bytes 7-11, zeroed */
	uint8_t  purifier;        /* byte 12 */
	uint8_t  nightmode;       /* byte 13 */
#define MIEL_HVAC_RUNSTATE_F_ECONOCOOL    (1 << 4)   /* 0x0010 */
	uint8_t  econocool;       /* byte 14 — EconoCool */
	uint8_t  _pad3[1];        /* byte 15 */
} __packed;

CTASSERT(sizeof(struct miel_hvac_msg_update_runstate) == 16);
CTASSERT(offsetof(struct miel_hvac_msg_update_runstate, flags)        == MIEL_HVAC_OFFS(6));
CTASSERT(offsetof(struct miel_hvac_msg_update_runstate, airdirection) == MIEL_HVAC_OFFS(11));
CTASSERT(offsetof(struct miel_hvac_msg_update_runstate, purifier)     == MIEL_HVAC_OFFS(17));
CTASSERT(offsetof(struct miel_hvac_msg_update_runstate, nightmode)    == MIEL_HVAC_OFFS(18));
CTASSERT(offsetof(struct miel_hvac_msg_update_runstate, econocool)   == MIEL_HVAC_OFFS(19));

static inline uint8_t
miel_hvac_cksum_fini(uint8_t sum)
{
	return (0xfc - sum);
}

struct miel_hvac_map
{
	uint8_t     byte;
	const char *name;
};

static const struct miel_hvac_map miel_hvac_power_map[] = {
	{MIEL_HVAC_SETTINGS_POWER_OFF, "off"},
	{MIEL_HVAC_SETTINGS_POWER_ON,  "on"},
};

static const struct miel_hvac_map miel_hvac_mode_map[] = {
	{MIEL_HVAC_SETTINGS_MODE_HEAT,      "heat"},
	{MIEL_HVAC_SETTINGS_MODE_DRY,       "dry"},
	{MIEL_HVAC_SETTINGS_MODE_COOL,      "cool"},
	{MIEL_HVAC_SETTINGS_MODE_FAN,       "fan"},
	{MIEL_HVAC_SETTINGS_MODE_FAN,       "fan_only"},  /* Home Assistant alias for "fan" */
	{MIEL_HVAC_SETTINGS_MODE_AUTO,      "auto"},
	{MIEL_HVAC_SETTINGS_MODE_HEAT_ISEE, "heat_isee"},
	{MIEL_HVAC_SETTINGS_MODE_DRY_ISEE,  "dry_isee"},
	{MIEL_HVAC_SETTINGS_MODE_COOL_ISEE, "cool_isee"},
};

static const struct miel_hvac_map miel_hvac_fan_map[] = {
	{MIEL_HVAC_SETTINGS_FAN_AUTO,  "auto"},
	{MIEL_HVAC_SETTINGS_FAN_QUIET, "quiet"},
	{MIEL_HVAC_SETTINGS_FAN_1,     "1"},
	{MIEL_HVAC_SETTINGS_FAN_2,     "2"},
	{MIEL_HVAC_SETTINGS_FAN_3,     "3"},
	{MIEL_HVAC_SETTINGS_FAN_4,     "4"},
};

static const struct miel_hvac_map miel_hvac_vane_map[] = {
	{MIEL_HVAC_SETTINGS_VANE_AUTO,       "auto"},
	{MIEL_HVAC_SETTINGS_VANE_1,          "up"},
	{MIEL_HVAC_SETTINGS_VANE_2,          "up_middle"},
	{MIEL_HVAC_SETTINGS_VANE_3,          "center"},
	{MIEL_HVAC_SETTINGS_VANE_4,          "down_middle"},
	{MIEL_HVAC_SETTINGS_VANE_5,          "down"},
	{MIEL_HVAC_SETTINGS_VANE_SWING,      "swing"},
};

static const struct miel_hvac_map miel_hvac_widevane_map[] = {
	{MIEL_HVAC_SETTINGS_WIDEVANE_LL,    "left"},
	{MIEL_HVAC_SETTINGS_WIDEVANE_L,     "left_middle"},
	{MIEL_HVAC_SETTINGS_WIDEVANE_C,     "center"},
	{MIEL_HVAC_SETTINGS_WIDEVANE_R,     "right"},
	{MIEL_HVAC_SETTINGS_WIDEVANE_RR,    "right_middle"},
	{MIEL_HVAC_SETTINGS_WIDEVANE_LC,    "left_center"},
	{MIEL_HVAC_SETTINGS_WIDEVANE_RC,    "right_center"},
	{MIEL_HVAC_SETTINGS_WIDEVANE_SPLIT, "split"},
	{MIEL_HVAC_SETTINGS_WIDEVANE_SWING, "swing"},
	{MIEL_HVAC_SETTINGS_WIDEVANE_ISEE,  "isee"},
};

static const struct miel_hvac_map miel_hvac_prohibit_map[] = {
	{MIEL_HVAC_SETTINGS_PROHIBIT_OFF,        "off"},
	{MIEL_HVAC_SETTINGS_PROHIBIT_POWER,      "power"},
	{MIEL_HVAC_SETTINGS_PROHIBIT_MODE,       "mode"},
	{MIEL_HVAC_SETTINGS_PROHIBIT_MODE_POWER, "mode_power"},
	{MIEL_HVAC_SETTINGS_PROHIBIT_TEMP,       "temp"},
	{MIEL_HVAC_SETTINGS_PROHIBIT_TEMP_POWER, "temp_power"},
	{MIEL_HVAC_SETTINGS_PROHIBIT_TEMP_MODE,  "temp_mode"},
	{MIEL_HVAC_SETTINGS_PROHIBIT_ALL,        "all"},
};

static const struct miel_hvac_map miel_hvac_airdirection_map[] = {
	{MIEL_HVAC_SETTINGS_AIRDIRECTION_INDIRECT, "indirect"},
	{MIEL_HVAC_SETTINGS_AIRDIRECTION_DIRECT,   "direct"},
	{MIEL_HVAC_SETTINGS_AIRDIRECTION_EVEN,     "even"},
	{MIEL_HVAC_SETTINGS_AIRDIRECTION_OFF,      "off"},
};

static const struct miel_hvac_map miel_hvac_purifier_map[] = {
	{MIEL_HVAC_OPTIONS_PURIFIER_OFF, "off"},
	{MIEL_HVAC_OPTIONS_PURIFIER_ON,  "on"},
};

static const struct miel_hvac_map miel_hvac_nightmode_map[] = {
	{MIEL_HVAC_OPTIONS_NIGHTMODE_OFF, "off"},
	{MIEL_HVAC_OPTIONS_NIGHTMODE_ON,  "on"},
};

static const struct miel_hvac_map miel_hvac_econocool_map[] = {
	{MIEL_HVAC_OPTIONS_ECONOCOOL_OFF, "off"},
	{MIEL_HVAC_OPTIONS_ECONOCOOL_ON,  "on"},
};

static const struct miel_hvac_map miel_hvac_compressor_map[] = {
	{MIEL_HVAC_STATUS_COMPRESSOR_OFF, "off"},
	{MIEL_HVAC_STATUS_COMPRESSOR_ON,  "on"},
};

static const struct miel_hvac_map miel_hvac_timer_mode_map[] = {
	{MIEL_HVAC_TIMER_MODE_NONE, "none"},
	{MIEL_HVAC_TIMER_MODE_OFF,  "off"},
	{MIEL_HVAC_TIMER_MODE_ON,   "on"},
	{MIEL_HVAC_TIMER_MODE_BOTH, "on_and_off"},
};

static const struct miel_hvac_map miel_hvac_stage_operation_map[] = {
	{MIEL_HVAC_STAGE_OPERATION_NORMAL,   "normal"},
	{MIEL_HVAC_STAGE_OPERATION_SERVICE,  "service"},
	{MIEL_HVAC_STAGE_OPERATION_DEFROST,  "defrost"},
	{MIEL_HVAC_STAGE_OPERATION_UNKNOWN,  "unknown"},
	{MIEL_HVAC_STAGE_OPERATION_PREHEAT,  "preheat"},
	{MIEL_HVAC_STAGE_OPERATION_UNKNOWN1, "unknown1"},
	{MIEL_HVAC_STAGE_OPERATION_UNKNOWN2, "unknown2"},
	{MIEL_HVAC_STAGE_OPERATION_UNKNOWN3, "unknown3"},
	{MIEL_HVAC_STAGE_OPERATION_STANDBY,  "standby"},
};

static const struct miel_hvac_map miel_hvac_stage_fan_map[] = {
	{MIEL_HVAC_STAGE_FAN_OFF,  "off"},
	{MIEL_HVAC_STAGE_FAN_QUIT, "quiet"},
	{MIEL_HVAC_STAGE_FAN_1,    "1"},
	{MIEL_HVAC_STAGE_FAN_2,    "2"},
	{MIEL_HVAC_STAGE_FAN_3,    "3"},
	{MIEL_HVAC_STAGE_FAN_4,    "4"},
	{MIEL_HVAC_STAGE_FAN_5,    "5"},
};

static const struct miel_hvac_map miel_hvac_stage_mode_map[] = {
	{MIEL_HVAC_STAGE_MODE_DIRECT,      "direct"},
	{MIEL_HVAC_STAGE_MODE_AUTO_FAN,    "auto_fan"},
	{MIEL_HVAC_STAGE_MODE_AUTO_HEAT,   "auto_heat"},
	{MIEL_HVAC_STAGE_MODE_AUTO_COOL,   "auto_cool"},
	{MIEL_HVAC_STAGE_MODE_AUTO_LEADER, "auto_leader"},
};

/*
 * Result of the miel_hvac_apply_* typed setters shared by the console commands
 * and the Modbus RTU slave.  Plain uint8_t is used as the return type (not this
 * enum) so the Arduino auto-generated function prototypes need not see it.
 */
enum miel_hvac_apply_result
{
	MIEL_HVAC_APPLY_OK = 0,
	MIEL_HVAC_APPLY_BAD_VALUE,   /* value outside the protocol's domain */
	MIEL_HVAC_APPLY_BAD_ADDR,    /* no such setting */
	MIEL_HVAC_APPLY_UNSUPPORTED, /* rejected by unit capabilities */
};

enum miel_hvac_parser_state
{
	MIEL_HVAC_P_START,
	MIEL_HVAC_P_TYPE,
	MIEL_HVAC_P_MIDDLE1,
	MIEL_HVAC_P_MIDDLE2,
	MIEL_HVAC_P_LEN,
	MIEL_HVAC_P_DATA,
	MIEL_HVAC_P_CKSUM,
	MIEL_HVAC_P_SKIP,
	MIEL_HVAC_P_SKIP_CKSUM,
};

#define MIEL_HVAC_DATABUFLEN 64

struct miel_hvac_parser
{
	enum miel_hvac_parser_state p_state;
	uint8_t p_tmo;
	uint8_t p_type;
	uint8_t p_sum;
	uint8_t p_len;
	uint8_t p_off;
	uint8_t p_data[MIEL_HVAC_DATABUFLEN];
};

/* 0x5B Identify Request / 0x7B Identify Response.
 * Docs: muart-group.github.io/.../0xC9-base-capabilities */
#define MIEL_HVAC_H_TYPE_IDENTIFY_REQ  0x5b
#define MIEL_HVAC_H_TYPE_IDENTIFY_RESP 0x7b

#define MIEL_HVAC_IDENTIFY_C9 0xc9   /* Base Capabilities command type */

/*
 * Parsed result of the 0x7B 0xC9 Base Capabilities response.
 *
 * Wire layout (16-byte data payload after packet header):
 *   byte  0    = 0xC9 (command type)
 *   bytes 1-6  = "arbitrary data" (partially meaningful, see notes)
 *   byte  7    = capability flags A
 *   byte  8    = capability flags B
 *   byte  9    = capability flags C
 *   bytes 10-15 = temperature range pairs (cool, heat, auto) — only present
 *                 when extended temp range is supported (flags_b & 0x04)
 *
 * Arbitrary data byte 6 bit 0x10 indicates 0x08 Set Run State support.
 */
struct miel_hvac_capabilities
{
	bool     sc_caps_valid;        /* true once 0xC9 response received */
	uint8_t  sc_caps_raw[16];      /* raw 0xC9 response data */

	/* capability flags A (byte 7) */
	bool     cap_mode_heat;             /* heat mode available (inverted: bit 0x02) */
	bool     cap_vane_v;           /* vertical vane support (bit 0x20) */
	bool     cap_vane_swing;       /* vane swing support (bit 0x40) */

	/* capability flags B (byte 8) */
	bool     cap_mode_dry;              /* dry mode available (inverted: bit 0x01) */
	bool     cap_mode_fan;         /* fan-only mode available (inverted: bit 0x02) */
	bool     cap_ext_temp;         /* extended temperature range (bit 0x04) */
	bool     cap_fan_auto;         /* auto fan speed available (inverted: bit 0x10) */
	bool     cap_installer;        /* installer function settings (bit 0x20) */

	/* capability flags C (byte 9) */
	bool     cap_outdoor_temp;     /* outdoor temperature reporting (bit 0x20) */
	bool     cap_run_state;        /* supports 0x08 Set Run State features (bit 0x10) */

	/* temperature ranges (bytes 10-15, only when cap_ext_temp) */
	bool     cap_temp_ranges;      /* temperature range bytes present */
	uint8_t  temp_cool_min;        /* byte 10 */
	uint8_t  temp_cool_max;        /* byte 11 */
	uint8_t  temp_heat_min;        /* byte 12 */
	uint8_t  temp_heat_max;        /* byte 13 */
	uint8_t  temp_auto_min;        /* byte 14 */
	uint8_t  temp_auto_max;        /* byte 15 */
};

#if defined(USE_MIEL_HVAC_MODBUS_SLAVE) && defined(ESP32)
/*
 * Modbus RTU slave state.  A second RS485 serial port that exposes every
 * driver state as read registers and every driver function as write
 * registers/coils, so the unit can be driven directly from a PLC.
 */
#define MIEL_HVAC_MB_BUFLEN           260
#define MIEL_HVAC_MB_DEFAULT_ADDRESS  1
#define MIEL_HVAC_MB_DEFAULT_BAUDRATE 9600
#define MIEL_HVAC_MB_MIN_BAUDRATE     1200
#define MIEL_HVAC_MB_MAX_BAUDRATE     115200

/* Modbus exception codes */
#define MIEL_HVAC_MB_EXC_FUNCTION     0x01
#define MIEL_HVAC_MB_EXC_ADDRESS      0x02
#define MIEL_HVAC_MB_EXC_VALUE        0x03
#define MIEL_HVAC_MB_EXC_FAILURE      0x04

struct miel_hvac_mb_softc
{
	TasmotaSerial *sc_serial;
	int8_t   sc_txen_pin;
	uint8_t  sc_address;
	uint8_t  sc_sconfig;
	uint32_t sc_baudrate;
	uint32_t sc_t35_us;      /* 3.5-char inter-frame gap, microseconds */
	uint32_t sc_last_us;     /* micros() of last received byte */
	uint16_t sc_len;
	bool     sc_no_reply;    /* suppress the response for a stale request */
	uint8_t  sc_buf[MIEL_HVAC_MB_BUFLEN];
	uint32_t sc_requests;
	uint32_t sc_crc_errors;
	uint32_t sc_exceptions;
	uint32_t sc_overruns;
};
#endif  /* USE_MIEL_HVAC_MODBUS_SLAVE && ESP32 */

struct miel_hvac_softc
{
	TasmotaSerial *sc_serial;
	struct miel_hvac_parser sc_parser;
#if defined(USE_MIEL_HVAC_MODBUS_SLAVE) && defined(ESP32)
	struct miel_hvac_mb_softc *sc_mb;
#endif

	unsigned int sc_device;
	unsigned int sc_tick;
	bool sc_settings_set;
	bool sc_connected;
	bool sc_identified;            /* true once 0x5B 0xC9 has been sent */
	bool sc_has_isee;              /* true once i-See widevane state observed */
	bool sc_has_energy;            /* true once non-zero Power or Energy seen */
	bool sc_temp_type;             /* true once extended .5°C encoding observed */
	bool sc_remotetemp_active;     /* true when remote temp override is active */
	unsigned long sc_remotetemp_auto_clear_time;
	unsigned long sc_remotetemp_last_call_time;
	int sc_remotetemp_half;        /* last remote temp in 0.5°C units */

	struct miel_hvac_data sc_settings;
	struct miel_hvac_data sc_roomtemp;
	struct miel_hvac_data sc_timers;
	struct miel_hvac_data sc_status;
	struct miel_hvac_data sc_stage;
	struct miel_hvac_data sc_options; /* 0x42 Options */

	struct miel_hvac_capabilities sc_caps; /* 0x7B 0xC9 Base Capabilities */

	struct miel_hvac_msg_update_settings   sc_settings_update;
	struct miel_hvac_msg_update_remotetemp sc_remotetemp_update;
	struct miel_hvac_msg_update_runstate   sc_runstate_update; /* 0x08 Set Run State */
};

static inline bool
miel_hvac_update_settings_pending(struct miel_hvac_softc *sc)
{
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;
	return (update->flags != htons(0));
}

static struct miel_hvac_softc *miel_hvac_sc = nullptr;
static void miel_hvac_log_bytes(struct miel_hvac_softc *, const char *, const void *, size_t);
static void miel_hvac_identify(struct miel_hvac_softc *);
static void miel_hvac_input_connected(struct miel_hvac_softc *, const void *, size_t);
static void miel_hvac_input_data(struct miel_hvac_softc *, const void *, size_t);
static void miel_hvac_input_updated(struct miel_hvac_softc *, const void *, size_t);
static void miel_hvac_input_identify(struct miel_hvac_softc *, const void *, size_t);

static enum miel_hvac_parser_state
miel_hvac_parse(struct miel_hvac_softc *sc, uint8_t byte)
{
	struct miel_hvac_parser *p = &sc->sc_parser;
	enum miel_hvac_parser_state nstate = p->p_state;

	switch (p->p_state)
	{
	case MIEL_HVAC_P_START:
		if (byte != MIEL_HVAC_H_START)
			return (MIEL_HVAC_P_START);

		/* reset state */
		p->p_sum = 0;
		p->p_tmo = 0;
		nstate = MIEL_HVAC_P_TYPE;
		break;
	case MIEL_HVAC_P_TYPE:
		p->p_type = byte;
		nstate = MIEL_HVAC_P_MIDDLE1;
		break;
	case MIEL_HVAC_P_MIDDLE1:
		if (byte != MIEL_HVAC_H_MIDDLE1)
		{
			AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME
				": parse state MIDDLE1 expected %02x got %02x, restarting"),
				MIEL_HVAC_H_MIDDLE1, byte);
			return (MIEL_HVAC_P_START);
		}
		nstate = MIEL_HVAC_P_MIDDLE2;
		break;
	case MIEL_HVAC_P_MIDDLE2:
		if (byte != MIEL_HVAC_H_MIDDLE2)
		{
			AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME
				": parse state MIDDLE2 expected %02x got %02x, restarting"),
				MIEL_HVAC_H_MIDDLE2, byte);
			return (MIEL_HVAC_P_START);
		}
		nstate = MIEL_HVAC_P_LEN;
		break;
	case MIEL_HVAC_P_LEN:
		if (byte == 0)
		{
			AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME
				": skipping 0 byte message type 0x%02x"), p->p_type);
			return (MIEL_HVAC_P_SKIP_CKSUM);
		}
		p->p_len = byte;
		p->p_off = 0;

		switch (p->p_type)
		{
		case MIEL_HVAC_H_TYPE_CONNECTED:
		case MIEL_HVAC_H_TYPE_DATA:
		case MIEL_HVAC_H_TYPE_UPDATED:
		case MIEL_HVAC_H_TYPE_IDENTIFY_RESP:
			break;
		default:
			AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME
				": skipping unknown message type 0x%02x"), p->p_type);
			return (MIEL_HVAC_P_SKIP);
		}

		if (byte > sizeof(p->p_data))
		{
			AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME
				": skipping %u data bytes of message type 0x%02x"),
				p->p_len, p->p_type);
			return (MIEL_HVAC_P_SKIP);
		}
		nstate = MIEL_HVAC_P_DATA;
		break;
	case MIEL_HVAC_P_DATA:
		p->p_data[p->p_off++] = byte;
		if (p->p_off >= p->p_len)
			nstate = MIEL_HVAC_P_CKSUM;
		break;
	case MIEL_HVAC_P_CKSUM:
		if (miel_hvac_cksum_fini(p->p_sum) != byte)
		{
			AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME
				": checksum failed, restarting"));
			return (MIEL_HVAC_P_START);
		}

		switch (p->p_type)
		{
		case MIEL_HVAC_H_TYPE_CONNECTED:
			miel_hvac_input_connected(sc, p->p_data, p->p_len);
			break;
		case MIEL_HVAC_H_TYPE_DATA:
			miel_hvac_input_data(sc, p->p_data, p->p_len);
			break;
		case MIEL_HVAC_H_TYPE_UPDATED:
			miel_hvac_input_updated(sc, p->p_data, p->p_len);
			break;
		case MIEL_HVAC_H_TYPE_IDENTIFY_RESP:
			miel_hvac_input_identify(sc, p->p_data, p->p_len);
			break;
		}

		/* this message is done, wait for another */
		return (MIEL_HVAC_P_START);
	case MIEL_HVAC_P_SKIP:
		if (++p->p_off >= p->p_len)
			return (MIEL_HVAC_P_SKIP_CKSUM);
		return (nstate);
	case MIEL_HVAC_P_SKIP_CKSUM:
		return (MIEL_HVAC_P_START);
	default:
		AddLog(LOG_LEVEL_ERROR, PSTR(MIEL_HVAC_LOGNAME
			": unknown parser state %d, resetting"), p->p_state);
		return (MIEL_HVAC_P_START);
	}
	p->p_sum += byte;

	return (nstate);
}

static uint8_t
miel_hvac_write(struct miel_hvac_softc *sc, const uint8_t *bytes, size_t len)
{
	TasmotaSerial *serial = sc->sc_serial;
	uint8_t cksum = 0;
	size_t i;

	for (i = 0; i < len; i++)
	{
		uint8_t b = bytes[i];
		serial->write(b);
		cksum += b;
	}

	return (cksum);
}

static void
miel_hvac_send(struct miel_hvac_softc *sc, uint8_t type, const void *data, size_t len)
{
	TasmotaSerial *serial = sc->sc_serial;
	struct miel_hvac_header h = {
		MIEL_HVAC_H_START,
		type,
		MIEL_HVAC_H_MIDDLE1,
		MIEL_HVAC_H_MIDDLE2,
		(uint8_t)len,
	};
	uint8_t cksum = 0;

	cksum += miel_hvac_write(sc, (const uint8_t *)&h, sizeof(h));
	cksum += miel_hvac_write(sc, (const uint8_t *)data, len);

	char hex_h[(sizeof(h) + 1) * 2];
	char hex_d[(MIEL_HVAC_DATABUFLEN + 1) * 2];
	AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME ": sending %s %s %02x"),
		ToHex_P((uint8_t *)&h, sizeof(h), hex_h, sizeof(hex_h)),
		ToHex_P((uint8_t *)data, len, hex_d, sizeof(hex_d)),
		miel_hvac_cksum_fini(cksum));

	serial->write(miel_hvac_cksum_fini(cksum));
	serial->flush();
}

#define miel_hvac_send_connect(_sc)                   \
	miel_hvac_send((_sc), MIEL_HVAC_H_TYPE_CONNECT,   \
		miel_hvac_msg_connect, sizeof(miel_hvac_msg_connect))

static const struct miel_hvac_map *
miel_hvac_map_byname(const char *name, const struct miel_hvac_map *m, size_t n)
{
	const struct miel_hvac_map *e;
	size_t i;

	for (i = 0; i < n; i++)
	{
		e = &m[i];
		if (strcasecmp(e->name, name) == 0)
			return (e);
	}

	return (NULL);
}

static const char *
miel_hvac_map_byval(uint8_t byte, const struct miel_hvac_map *m, size_t n)
{
	const struct miel_hvac_map *e;
	size_t i;

	for (i = 0; i < n; i++)
	{
		e = &m[i];
		if (byte == e->byte)
			return (e->name);
	}

	return (NULL);
}

static void
miel_hvac_request(struct miel_hvac_softc *sc, uint8_t type)
{
	struct miel_hvac_msg_request request = {type};
	miel_hvac_send(sc, MIEL_HVAC_H_TYPE_REQUEST, &request, sizeof(request));
}

/*
 * Some command types (e.g. 0x42 Options) do not use the standard
 * 16-byte body — the protocol documentation states the body is ignored
 * and some units only respond when len=1 (command byte only).
 */
static void
miel_hvac_request_short(struct miel_hvac_softc *sc, uint8_t type)
{
	miel_hvac_send(sc, MIEL_HVAC_H_TYPE_REQUEST, &type, sizeof(type));
}

static void
miel_hvac_init_update_settings(struct miel_hvac_msg_update_settings *update)
{
	memset(update, 0, sizeof(*update));
	update->one = 1;
}

static inline void
miel_hvac_send_update_settings(struct miel_hvac_softc *sc,
	const struct miel_hvac_msg_update_settings *update)
{
	miel_hvac_send(sc, MIEL_HVAC_H_TYPE_UPDATE, update, sizeof(*update));
}

static inline void
miel_hvac_send_update_remotetemp(struct miel_hvac_softc *sc,
	const struct miel_hvac_msg_update_remotetemp *update)
{
	miel_hvac_send(sc, MIEL_HVAC_H_TYPE_UPDATE, update, sizeof(*update));
}

static inline void
miel_hvac_send_update_runstate(struct miel_hvac_softc *sc,
	const struct miel_hvac_msg_update_runstate *update)
{
	miel_hvac_send(sc, MIEL_HVAC_H_TYPE_UPDATE, update, sizeof(*update));
}

static inline bool
miel_hvac_update_runstate_pending(struct miel_hvac_softc *sc)
{
	return (sc->sc_runstate_update.flags != 0);
}

/* cmnd helpers */

static void
miel_hvac_respond_unsupported(void)
{
	ResponseCmndChar_P(PSTR("Unsupported"));
}

static void
miel_hvac_respond_not_supported(void)
{
	ResponseCmndChar_P(PSTR("NotSupported"));
}

static void
miel_hvac_respond_control_not_supported(void)
{
	ResponseCmndChar_P(PSTR("ControlNotSupported"));
}

/*
 * Returns the number of fan speed steps supported by the unit as decoded
 * from the three fan bits in the 0xC9 Base Capabilities response.
 * Returns 0 if capabilities have not been received yet.
 */
static uint8_t
miel_hvac_get_fan_count(const struct miel_hvac_softc *sc)
{
	static const uint8_t fan_speed_lut[8] = {
		3,  /* 0b000 = 0 → 3 speeds */
		1,  /* 0b001 = 1 → 1 speed  */
		2,  /* 0b010 = 2 → 2 speeds */
		0,  /* 0b011 = 3 → (unused) */
		4,  /* 0b100 = 4 → 4 speeds */
		0,  /* 0b101 = 5 → (unused) */
		5,  /* 0b110 = 6 → 5 speeds */
		0,  /* 0b111 = 7 → (unused) */
	};
	const struct miel_hvac_capabilities *caps = &sc->sc_caps;
	uint8_t raw;

	if (!caps->sc_caps_valid)
		return (0);

	raw = ((caps->sc_caps_raw[7] & 0x10) >> 2)
	    | ((caps->sc_caps_raw[8] & 0x08) >> 2)
	    | ((caps->sc_caps_raw[9] & 0x02) >> 1);

	return (fan_speed_lut[raw]);
}

static inline uint8_t
miel_hvac_deg2temp(bool temp_type, float deg)
{
	if (!temp_type)
		return (31 - deg);
	else
	{
		deg = 2 * deg + 128;
		return ((uint8_t)deg);
	}
}

static inline float
miel_hvac_temp2deg(bool temp_type, uint8_t temp)
{
	if (!temp_type)
		return (31 - temp);
	else
	{
		temp -= 128;
		return ((float)temp / 2);
	}
}

static inline float
miel_hvac_roomtemp2deg(bool temp_type, uint8_t roomtemp)
{
	if (!temp_type)
		return ((unsigned int)roomtemp + 10);
	else
	{
		roomtemp -= 128;
		return ((float)roomtemp / 2);
	}
}

static inline float
miel_hvac_outdoortemp2deg(uint8_t outdoortemp)
{
	outdoortemp -= 128;
	return ((float)outdoortemp / 2);
}

static inline uint8_t
miel_hvac_remotetemp2old(int degc_half)
{
	int min_half = MIEL_HVAC_REMOTETEMP_OLD_MIN * 2;
	int max_half = MIEL_HVAC_REMOTETEMP_OLD_MAX * 2;

	if (degc_half < min_half)
		degc_half = min_half;
	else if (degc_half > max_half)
		degc_half = max_half;

	return (degc_half - min_half);
}

static inline uint8_t
miel_hvac_remotetemp2new(int degc_half)
{
	int min_half = MIEL_HVAC_REMOTETEMP_MIN * 2;
	int max_half = MIEL_HVAC_REMOTETEMP_MAX * 2;

	if (degc_half < min_half)
		degc_half = min_half;
	else if (degc_half > max_half)
		degc_half = max_half;

	return (degc_half + MIEL_HVAC_REMOTETEMP_OFFSET * 2);
}

/*
 * Sends a CLR frame to the unit and resets the remote temperature override.
 * Triggered after remotetemp_auto_clear_time ms of inactivity.
 */
static void
miel_hvac_remotetemp_auto_clear(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update_remotetemp *update = &sc->sc_remotetemp_update;

	sc->sc_remotetemp_half = 0;
	sc->sc_remotetemp_active = false;

	memset(update, 0, sizeof(*update));
	update->seven   = 0x7;
	update->control = MIEL_HVAC_REMOTETEMP_CLR;
	update->temp_old = miel_hvac_remotetemp2old(0);
	update->temp     = miel_hvac_remotetemp2new(0);
}

/* cmnd */

static bool
miel_hvac_cmnd_setpower(struct miel_hvac_softc *sc)
{
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;
	uint16_t source = XdrvMailbox.payload;

	if (source == SRC_SWITCH)
		return (false);

	update->flags |= htons(MIEL_HVAC_SETTINGS_F_POWER);
	update->power = (XdrvMailbox.index & (1 << sc->sc_device))
		? MIEL_HVAC_SETTINGS_POWER_ON
		: MIEL_HVAC_SETTINGS_POWER_OFF;

	return (true);
}

static void
miel_hvac_cmnd_setfanspeed(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;
	const struct miel_hvac_map *e;

	if (XdrvMailbox.data_len == 0)
		return;

	e = miel_hvac_map_byname(XdrvMailbox.data,
		miel_hvac_fan_map, nitems(miel_hvac_fan_map));
	if (e == NULL)
	{
		miel_hvac_respond_unsupported();
		return;
	}

	if (sc->sc_caps.sc_caps_valid)
	{
		/* AUTO requires cap_fan_auto; QUIET needs 5 speeds; FAN_4 needs 4 speeds. */
		uint8_t fan_count = miel_hvac_get_fan_count(sc);
		if (e->byte == MIEL_HVAC_SETTINGS_FAN_AUTO && !sc->sc_caps.cap_fan_auto)
		{
			miel_hvac_respond_not_supported();
			return;
		}
		if (fan_count > 0)
		{
			if (e->byte == MIEL_HVAC_SETTINGS_FAN_QUIET && fan_count < 5)
			{
				miel_hvac_respond_not_supported();
				return;
			}
			if (e->byte == MIEL_HVAC_SETTINGS_FAN_4 && fan_count < 4)
			{
				miel_hvac_respond_not_supported();
				return;
			}
		}
	}

	update->flags |= htons(MIEL_HVAC_SETTINGS_F_FAN);
	update->fan = e->byte;

	ResponseCmndChar_P(e->name);
}

/* Translate a miel_hvac_apply_* result into the console cmnd response. */
static void
miel_hvac_cmnd_apply_response(uint8_t result, const char *name)
{
	switch (result)
	{
	case MIEL_HVAC_APPLY_OK:
		ResponseCmndChar_P(name);
		break;
	case MIEL_HVAC_APPLY_UNSUPPORTED:
		miel_hvac_respond_not_supported();
		break;
	default:
		miel_hvac_respond_unsupported();
		break;
	}
}

static void
miel_hvac_cmnd_setmode(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	const struct miel_hvac_map *e;

	if (XdrvMailbox.data_len == 0)
		return;

	e = miel_hvac_map_byname(XdrvMailbox.data,
		miel_hvac_mode_map, nitems(miel_hvac_mode_map));
	if (e == NULL)
	{
		miel_hvac_respond_unsupported();
		return;
	}

	miel_hvac_cmnd_apply_response(miel_hvac_apply_mode(sc, e->byte), e->name);
}

static void
miel_hvac_cmnd_sethamode(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	const struct miel_hvac_map *e;

	if (XdrvMailbox.data_len == 0)
		return;

	if (strcasecmp(XdrvMailbox.data, "off") == 0)
	{
		miel_hvac_apply_hamode(sc, 0);
		ResponseCmndChar_P(PSTR("off"));
		return;
	}

	/*
	 * Set both power and mode atomically so we do not turn the unit on
	 * if the mode argument turns out to be invalid.
	 */
	e = miel_hvac_map_byname(XdrvMailbox.data,
		miel_hvac_mode_map, nitems(miel_hvac_mode_map));
	if (e == NULL)
	{
		miel_hvac_respond_unsupported();
		return;
	}

	miel_hvac_cmnd_apply_response(miel_hvac_apply_hamode(sc, e->byte), e->name);
}

static void
miel_hvac_cmnd_settemp(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;
	float degc;

	if (XdrvMailbox.data_len == 0)
		return;

	degc = strtof(XdrvMailbox.data, nullptr);
	if (degc < MIEL_HVAC_SETTINGS_TEMP_MIN || degc > MIEL_HVAC_SETTINGS_TEMP_MAX)
	{
		miel_hvac_respond_unsupported();
		return;
	}

	/* Narrow the range using unit-reported capabilities when available. */
	if (sc->sc_caps.sc_caps_valid && sc->sc_caps.cap_temp_ranges)
	{
		const struct miel_hvac_capabilities *caps = &sc->sc_caps;
		const struct miel_hvac_data_settings *set =
			&sc->sc_settings.data.settings;
		uint8_t mode = set->mode & MIEL_HVAC_SETTINGS_MODE_MASK;
		float cap_min, cap_max;

		if (mode == MIEL_HVAC_SETTINGS_MODE_HEAT)
		{
			cap_min = (caps->temp_heat_min - 128) / 2.0f;
			cap_max = (caps->temp_heat_max - 128) / 2.0f;
		}
		else if (mode == MIEL_HVAC_SETTINGS_MODE_AUTO)
		{
			cap_min = (caps->temp_auto_min - 128) / 2.0f;
			cap_max = (caps->temp_auto_max - 128) / 2.0f;
		}
		else
		{
			cap_min = (caps->temp_cool_min - 128) / 2.0f;
			cap_max = (caps->temp_cool_max - 128) / 2.0f;
		}

		if (degc < cap_min || degc > cap_max)
		{
			miel_hvac_respond_not_supported();
			return;
		}
	}
	update->flags |= htons(MIEL_HVAC_SETTINGS_F_TEMP);
	if (!sc->sc_temp_type)
	{
		update->temp   = miel_hvac_deg2temp(sc->sc_temp_type, degc);
		update->temp05 = 0;
	}
	else
	{
		update->temp   = 0;
		update->temp05 = miel_hvac_deg2temp(sc->sc_temp_type, degc);
	}

	ResponseCmndNumber(degc);
}

static void
miel_hvac_cmnd_setvane(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;
	const struct miel_hvac_map *e;

	if (XdrvMailbox.data_len == 0)
		return;

	e = miel_hvac_map_byname(XdrvMailbox.data,
		miel_hvac_vane_map, nitems(miel_hvac_vane_map));
	if (e == NULL)
	{
		miel_hvac_respond_unsupported();
		return;
	}
	update->flags |= htons(MIEL_HVAC_SETTINGS_F_VANE);
	update->vane = e->byte;

	ResponseCmndChar_P(e->name);
}

static void
miel_hvac_cmnd_setprohibit(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;
	const struct miel_hvac_map *e;

	if (XdrvMailbox.data_len == 0)
		return;

	e = miel_hvac_map_byname(XdrvMailbox.data,
		miel_hvac_prohibit_map, nitems(miel_hvac_prohibit_map));
	if (e == NULL)
	{
		miel_hvac_respond_unsupported();
		return;
	}
	update->flags |= htons(MIEL_HVAC_SETTINGS_F_PROHIBIT);
	update->prohibit = e->byte;

	ResponseCmndChar_P(e->name);
}

static void
miel_hvac_cmnd_setwidevane(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;
	const struct miel_hvac_map *e;

	if (XdrvMailbox.data_len == 0)
		return;

	e = miel_hvac_map_byname(XdrvMailbox.data,
		miel_hvac_widevane_map, nitems(miel_hvac_widevane_map));
	if (e == NULL)
	{
		miel_hvac_respond_unsupported();
		return;
	}

	update->flags |= htons(MIEL_HVAC_SETTINGS_F_WIDEVANE);
	update->widevane = e->byte;

	ResponseCmndChar_P(e->name);
}

static void
miel_hvac_cmnd_setairdirection(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;
	const struct miel_hvac_map *e;

	if (XdrvMailbox.data_len == 0)
		return;

	e = miel_hvac_map_byname(XdrvMailbox.data,
		miel_hvac_airdirection_map, nitems(miel_hvac_airdirection_map));
	if (e == NULL)
	{
		miel_hvac_respond_unsupported();
		return;
	}

	/* AirDirection control requires a vertical vane and an observed i-See
	 * sensor. It works independently of cap_run_state. */
	if (sc->sc_caps.sc_caps_valid
	    && (!sc->sc_caps.cap_vane_v || !sc->sc_has_isee))
	{
		miel_hvac_respond_not_supported();
		return;
	}

	/*
	 * Airflow control (i-See direction) is set via 0x41 0x08 byte 6, flag 0x2000.
	 * Values: 0=EVEN, 1=INDIRECT, 2=DIRECT. Requires widevane=0x80 set via 0x01.
	 * OFF has no 0x08 equivalent and is expressed as widevane=0x8c via 0x01.
	 */
	switch (e->byte)
	{
	case MIEL_HVAC_SETTINGS_AIRDIRECTION_INDIRECT:
	case MIEL_HVAC_SETTINGS_AIRDIRECTION_DIRECT:
	case MIEL_HVAC_SETTINGS_AIRDIRECTION_EVEN:
	{
		struct miel_hvac_msg_update_runstate *rs = &sc->sc_runstate_update;

		/* Enable i-See airflow control first: widevane=0x80 via 0x01.
		 * The dispatcher sends the settings (0x01) packet before the
		 * runstate (0x41) packet, so this reaches the unit ahead of the
		 * airdirection value below. */
		update->flags     |= htons(MIEL_HVAC_SETTINGS_F_WIDEVANE);
		update->widevane   = MIEL_HVAC_SETTINGS_WIDEVANE_ISEE;

		rs->eight          = 0x08;
		rs->flags         |= htons(MIEL_HVAC_RUNSTATE_F_AIRDIRECTION);
		rs->airdirection   = e->byte;
		break;
	}
	case MIEL_HVAC_SETTINGS_AIRDIRECTION_OFF:
		update->flags   |= htons(MIEL_HVAC_SETTINGS_F_WIDEVANE);
		update->widevane = 0x8c;
		break;
	}

	ResponseCmndChar_P(e->name);
}

static void
miel_hvac_cmnd_setpurify(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update_runstate *update = &sc->sc_runstate_update;
	const struct miel_hvac_map *e;

	if (XdrvMailbox.data_len == 0)
		return;

	e = miel_hvac_map_byname(XdrvMailbox.data,
		miel_hvac_purifier_map, nitems(miel_hvac_purifier_map));
	if (e == NULL)
	{
		miel_hvac_respond_unsupported();
		return;
	}


	if (sc->sc_caps.sc_caps_valid && !sc->sc_caps.cap_run_state)
	{
		miel_hvac_respond_not_supported();
		return;
	}

	update->eight     = 0x08;
	update->flags    |= htons(MIEL_HVAC_RUNSTATE_F_PURIFIER);
	update->purifier  = e->byte;

	ResponseCmndChar_P(e->name);
}

static void
miel_hvac_cmnd_setnightmode(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update_runstate *update = &sc->sc_runstate_update;
	const struct miel_hvac_map *e;

	if (XdrvMailbox.data_len == 0)
		return;

	e = miel_hvac_map_byname(XdrvMailbox.data,
		miel_hvac_nightmode_map, nitems(miel_hvac_nightmode_map));
	if (e == NULL)
	{
		miel_hvac_respond_unsupported();
		return;
	}


	if (sc->sc_caps.sc_caps_valid && !sc->sc_caps.cap_run_state)
	{
		miel_hvac_respond_not_supported();
		return;
	}

	update->eight      = 0x08;
	update->flags     |= htons(MIEL_HVAC_RUNSTATE_F_NIGHTMODE);
	update->nightmode  = e->byte;

	ResponseCmndChar_P(e->name);
}

static void
miel_hvac_cmnd_seteconocool(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update_runstate *update = &sc->sc_runstate_update;
	const struct miel_hvac_map *e;

	if (XdrvMailbox.data_len == 0)
		return;

	e = miel_hvac_map_byname(XdrvMailbox.data,
		miel_hvac_econocool_map, nitems(miel_hvac_econocool_map));
	if (e == NULL)
	{
		miel_hvac_respond_unsupported();
		return;
	}


	if (sc->sc_caps.sc_caps_valid && !sc->sc_caps.cap_run_state)
	{
		miel_hvac_respond_not_supported();
		return;
	}

	update->eight       = 0x08;
	update->flags      |= htons(MIEL_HVAC_RUNSTATE_F_ECONOCOOL);
	update->econocool  = e->byte;

	ResponseCmndChar_P(e->name);
}

static void
miel_hvac_cmnd_remotetemp_auto_clear_time(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;

	if (XdrvMailbox.data_len == 0)
		return;

	unsigned long clear_time = strtoul(XdrvMailbox.data, nullptr, 10);
	if (clear_time < 1000 || clear_time > 600000)
	{
		miel_hvac_respond_unsupported();
		return;
	}
	sc->sc_remotetemp_auto_clear_time = clear_time;

	ResponseCmndNumber(sc->sc_remotetemp_auto_clear_time);
}

static void
miel_hvac_cmnd_remotetemp(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update_remotetemp *update = &sc->sc_remotetemp_update;
	uint8_t control;
	int temp_half = 0;

	if (XdrvMailbox.data_len == 0)
		return;

	if (strcasecmp(XdrvMailbox.data, "clear") == 0)
	{
		control                  = MIEL_HVAC_REMOTETEMP_CLR;
		sc->sc_remotetemp_half   = 0;
		sc->sc_remotetemp_active = false;

		ResponseCmndChar_P("clear");
	}
	else
	{
		char *end;
		double input = strtod(XdrvMailbox.data, &end);

		if (*end != '\0')
		{
			ResponseCmndChar_P("invalid");
			return;
		}

		temp_half = (int)(input * 2.0 + 0.5);

		int min_half = MIEL_HVAC_REMOTETEMP_MIN * 2;
		int max_half = MIEL_HVAC_REMOTETEMP_MAX * 2;

		if (temp_half < min_half)
			temp_half = min_half;
		else if (temp_half > max_half)
			temp_half = max_half;

		sc->sc_remotetemp_half          = temp_half;
		control                         = MIEL_HVAC_REMOTETEMP_SET;
		sc->sc_remotetemp_active        = true;
		sc->sc_remotetemp_last_call_time = millis();

		ResponseCmndFloat(sc->sc_remotetemp_half / 2.0, 1);
	}

	memset(update, 0, sizeof(*update));
	update->seven    = 0x7;
	update->control  = control;
	update->temp_old = miel_hvac_remotetemp2old(sc->sc_remotetemp_half);
	update->temp     = miel_hvac_remotetemp2new(sc->sc_remotetemp_half);
}

static void
miel_hvac_cmnd_send_command(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;
	uint8_t val;

	if (XdrvMailbox.data_len == 0)
		return;

	char *endptr;
	long parsed = strtol(XdrvMailbox.data, &endptr, 0);

	if (endptr == XdrvMailbox.data || parsed < 0 || parsed > 255)
	{
		miel_hvac_respond_unsupported();
		return;
	}

	val = (uint8_t)parsed;

	update->flags |= htons(MIEL_HVAC_SETTINGS_F_WIDEVANE);
	update->widevane = val;

	ResponseCmndChar_P(XdrvMailbox.data);
}

#ifdef MIEL_HVAC_DEBUG
static void
miel_hvac_cmnd_request(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	uint8_t type = MIEL_HVAC_REQUEST_ROOMTEMP;

	if (XdrvMailbox.data_len > 0)
		type = strtoul(XdrvMailbox.data, nullptr, 0);

	miel_hvac_request(sc, type);

	ResponseCmndDone();
}
#endif

/* serial data handlers */

static void
miel_hvac_identify(struct miel_hvac_softc *sc)
{
	uint8_t cmd = MIEL_HVAC_IDENTIFY_C9;
	miel_hvac_send(sc, MIEL_HVAC_H_TYPE_IDENTIFY_REQ, &cmd, sizeof(cmd));
}

/*
 * Parse 0x7B 0xC9 Base Capabilities response and populate sc_caps.
 * Wire layout (16-byte data payload):
 *   [0]    = 0xC9 (command type)
 *   [1-6]  = "arbitrary data"
 *   [7]    = flags_a (vane types, heat, fan bits)
 *   [8]    = flags_b (dry, fan mode, ext temp, installer)
 *   [9]    = flags_c (outside temp, run state features)
 *   [10-15]= temperature ranges (only when flags_b & 0x04)
 */
static void
miel_hvac_input_identify(struct miel_hvac_softc *sc,
	const void *buf, size_t len)
{
	const uint8_t *d = (const uint8_t *)buf;
	struct miel_hvac_capabilities *caps = &sc->sc_caps;
	char hex[(16 + 1) * 2];

	miel_hvac_log_bytes(sc, "identify", buf, len);

	if (len < 10)
	{
		AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME
			": short identify response (%zu), ignoring"), len);
		return;
	}

	if (d[0] != MIEL_HVAC_IDENTIFY_C9)
	{
		AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME
			": unknown identify command 0x%02x, ignoring"), d[0]);
		return;
	}

	/* store raw bytes */
	memcpy(caps->sc_caps_raw, d, len < 16 ? len : 16);

	/* flags_a = byte 7 */
	uint8_t fa = d[7];
	caps->cap_mode_heat      = !(fa & 0x02);  /* inverted */
	caps->cap_vane_v    = (fa & 0x20) != 0;
	caps->cap_vane_swing= (fa & 0x40) != 0;

	/* flags_b = byte 8 */
	uint8_t fb = d[8];
	caps->cap_mode_dry       = !(fb & 0x01);  /* inverted */
	caps->cap_mode_fan  = !(fb & 0x02);  /* inverted */
	caps->cap_ext_temp  = (fb & 0x04) != 0;
	caps->cap_fan_auto  = !(fb & 0x10);  /* inverted */
	caps->cap_installer = (fb & 0x20) != 0;

	/* flags_c = byte 9 */
	uint8_t fc = d[9];
	caps->cap_outdoor_temp = (fc & 0x20) != 0;
	/* bit 0x10 of byte 9 indicates 0x08 Set Run State support. */
	caps->cap_run_state = (fc & 0x10) != 0;

	/* temperature ranges — only present when cap_ext_temp */
	caps->cap_temp_ranges = (len >= 16 && caps->cap_ext_temp);
	if (caps->cap_temp_ranges)
	{
		caps->temp_cool_min = d[10];
		caps->temp_cool_max = d[11];
		caps->temp_heat_min = d[12];
		caps->temp_heat_max = d[13];
		caps->temp_auto_min = d[14];
		caps->temp_auto_max = d[15];
	}

	caps->sc_caps_valid = true;

	AddLog(LOG_LEVEL_INFO, PSTR(MIEL_HVAC_LOGNAME
		": capabilities: mode_heat=%d mode_dry=%d mode_fan=%d vane_v=%d swing=%d "
		"ext_temp=%d fan_auto=%d outdoor_temp=%d run_state=%d"),
		caps->cap_mode_heat, caps->cap_mode_dry, caps->cap_mode_fan,
		caps->cap_vane_v, caps->cap_vane_swing, caps->cap_ext_temp,
		caps->cap_fan_auto, caps->cap_outdoor_temp, caps->cap_run_state);

	/* raw packet bytes */
	AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME ": capabilities hex %s"),
		ToHex_P(caps->sc_caps_raw, 16, hex, sizeof(hex)));
}

static void
miel_hvac_log_bytes(struct miel_hvac_softc *sc, const char *name,
	const void *buf, size_t len)
{
	char hex[(MIEL_HVAC_DATABUFLEN + 1) * 2];
	const unsigned char *b = (const unsigned char *)buf;
	AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME ": response %s %s"),
		name, ToHex_P(b, len, hex, sizeof(hex)));
}

static void
miel_hvac_input_connected(struct miel_hvac_softc *sc,
	const void *buf, size_t len)
{
	AddLog(LOG_LEVEL_INFO, PSTR(MIEL_HVAC_LOGNAME
		": connected to Mitsubishi Electric HVAC"));
	sc->sc_connected = true;
}

/*
 * Appends all current settings fields to the active response buffer.
 * Used by both miel_hvac_publish_settings() (HVACSettings topic) and
 * miel_hvac_sensor() (SENSOR topic).
 */
static void
miel_hvac_append_settings_json(struct miel_hvac_softc *sc)
{
	const struct miel_hvac_data_settings *set = &sc->sc_settings.data.settings;
	char hex[(sizeof(sc->sc_settings) + 1) * 2];
	char temp[33];
	const char *name;
	const char *ha_name = "off";
	/* i-See direction control is active for widevane 0x80, 0x28, 0xaa.
	 * Other values with the 0x80 bit encode the last vane position; direction is OFF. */
	bool widevane_isee = (set->widevane == 0x80
	                   || set->widevane == 0x28
	                   || set->widevane == 0xaa);

	/* Power */
	name = miel_hvac_map_byval(set->power,
		miel_hvac_power_map, nitems(miel_hvac_power_map));
	if (name != NULL)
		ResponseAppend_P(PSTR("\"PowerState\":\"%s\""), name);

	/* Mode */
	name = miel_hvac_map_byval(set->mode & MIEL_HVAC_SETTINGS_MODE_MASK,
		miel_hvac_mode_map, nitems(miel_hvac_mode_map));
	if (name != NULL)
	{
		ResponseAppend_P(PSTR(",\"" D_JSON_IRHVAC_MODE "\":\"%s\""), name);
		if (set->power)
		{
			switch (set->mode & MIEL_HVAC_SETTINGS_MODE_MASK)
			{
			case MIEL_HVAC_SETTINGS_MODE_HEAT:
			case MIEL_HVAC_SETTINGS_MODE_HEAT_ISEE:
				ha_name = "heat"; break;
			case MIEL_HVAC_SETTINGS_MODE_COOL:
			case MIEL_HVAC_SETTINGS_MODE_COOL_ISEE:
				ha_name = "cool"; break;
			case MIEL_HVAC_SETTINGS_MODE_DRY:
			case MIEL_HVAC_SETTINGS_MODE_DRY_ISEE:
				ha_name = "dry"; break;
			case MIEL_HVAC_SETTINGS_MODE_FAN:
				ha_name = "fan_only"; break;
			case MIEL_HVAC_SETTINGS_MODE_AUTO:
				ha_name = "auto"; break;
			}
		}
		ResponseAppend_P(PSTR(",\"HA" D_JSON_IRHVAC_MODE "\":\"%s\""), ha_name);
	}

	/* Temperature */
	if (set->temp05 == 0)
		dtostrfd(ConvertTemp(miel_hvac_temp2deg(sc->sc_temp_type, set->temp)),
			Settings->flag2.temperature_resolution, temp);
	else
		dtostrfd(ConvertTemp(miel_hvac_temp2deg(sc->sc_temp_type, set->temp05)),
			Settings->flag2.temperature_resolution, temp);
	ResponseAppend_P(PSTR(",\"SetTemperature\":%s"), temp);

	/* Fan */
	name = miel_hvac_map_byval(set->fan,
		miel_hvac_fan_map, nitems(miel_hvac_fan_map));
	if (name != NULL)
		ResponseAppend_P(PSTR(",\"" D_JSON_IRHVAC_FANSPEED "\":\"%s\""), name);

	/* Swing vertical */
	name = miel_hvac_map_byval(set->vane,
		miel_hvac_vane_map, nitems(miel_hvac_vane_map));
	if (name != NULL)
		ResponseAppend_P(PSTR(",\"" D_JSON_IRHVAC_SWINGV "\":\"%s\""), name);

	/* Swing horizontal / widevane */
	name = widevane_isee
		? "isee"
		: miel_hvac_map_byval(set->widevane & MIEL_HVAC_SETTINGS_WIDEVANE_MASK,
			miel_hvac_widevane_map, nitems(miel_hvac_widevane_map));
	if (name != NULL)
		ResponseAppend_P(PSTR(",\"" D_JSON_IRHVAC_SWINGH "\":\"%s\""), name);

	/* Air direction — only reported when the unit has both a vertical vane
	 * and an observed i-See sensor. Without i-See the direction value is
	 * meaningless regardless of whether vaneV is present. */
	if (!sc->sc_caps.sc_caps_valid
	    || (sc->sc_caps.cap_vane_v && sc->sc_has_isee))
	{
		name = widevane_isee
			? miel_hvac_map_byval(set->airdirection,
				miel_hvac_airdirection_map, nitems(miel_hvac_airdirection_map))
			: "off";
		if (name != NULL)
			ResponseAppend_P(PSTR(",\"AirDirection\":\"%s\""), name);
	}

	/* Prohibit */
	name = miel_hvac_map_byval(set->prohibit,
		miel_hvac_prohibit_map, nitems(miel_hvac_prohibit_map));
	if (name != NULL)
		ResponseAppend_P(PSTR(",\"Prohibit\":\"%s\""), name);

	/* Purifier, NightMode, EconoCool — state from 0x62 0x42 Options. */
	if ((!sc->sc_caps.sc_caps_valid || sc->sc_caps.cap_run_state)
		&& sc->sc_options.type != 0)
	{
		const struct miel_hvac_data_options *opt =
			&sc->sc_options.data.options;

		name = miel_hvac_map_byval(opt->purifier,
			miel_hvac_purifier_map, nitems(miel_hvac_purifier_map));
		if (name != NULL)
			ResponseAppend_P(PSTR(",\"Purifier\":\"%s\""), name);

		name = miel_hvac_map_byval(opt->nightmode,
			miel_hvac_nightmode_map, nitems(miel_hvac_nightmode_map));
		if (name != NULL)
			ResponseAppend_P(PSTR(",\"NightMode\":\"%s\""), name);

		name = miel_hvac_map_byval(opt->econocool,
			miel_hvac_econocool_map, nitems(miel_hvac_econocool_map));
		if (name != NULL)
			ResponseAppend_P(PSTR(",\"EconoCool\":\"%s\""), name);
	}

	/* raw packet bytes */
	ResponseAppend_P(PSTR(",\"SettingsHex\":\"%s\""),
		ToHex_P((uint8_t *)&sc->sc_settings,
			sizeof(sc->sc_settings), hex, sizeof(hex)));
}

static void
miel_hvac_publish_settings(struct miel_hvac_softc *sc)
{
	Response_P(PSTR("{"));
	miel_hvac_append_settings_json(sc);
	ResponseAppend_P(PSTR("}"));
	MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR("HVACSettings"));
}

static void
miel_hvac_input_settings(struct miel_hvac_softc *sc,
	const struct miel_hvac_data *d)
{
	const struct miel_hvac_data_settings *set = &d->data.settings;
	uint32_t state = set->power ? 1 : 0;
	bool publish;

	if (miel_hvac_update_settings_pending(sc))
	{
		/*
		 * Do not publish settings that might be mid-change.
		 * Force re-publication on the next settled read.
		 */
		sc->sc_settings_set = false;
		return;
	}

	if (bitRead(TasmotaGlobal.power, sc->sc_device) != !!state)
		ExecuteCommandPower(sc->sc_device, state, SRC_SWITCH);

	/* Detect presence of i-See sensor from widevane bit 0x80 or the
	 * two known i-See-active non-0x80 values. Once set, stays set. */
	if ((set->widevane & 0x80) || set->widevane == 0x28 || set->widevane == 0xaa)
		sc->sc_has_isee = true;

	publish = (sc->sc_settings_set == 0)
	       || (memcmp(d, &sc->sc_settings, sizeof(sc->sc_settings)) != 0);
	sc->sc_settings_set = true;
	sc->sc_settings = *d;

	if (publish)
	{
		miel_hvac_publish_settings(sc);
		MqttPublishSensor();
		TasRediscover();
	}
}

static void
miel_hvac_data_response(struct miel_hvac_softc *sc,
	const struct miel_hvac_data *d)
{
	char hex[(sizeof(*d) + 1) * 2];
	Response_P(PSTR("{\"Bytes\":\"%s\"}"),
		ToHex_P((uint8_t *)d, sizeof(*d), hex, sizeof(hex)));
	MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR("HVACData"));
}

static void
miel_hvac_input_sensor(struct miel_hvac_softc *sc,
	struct miel_hvac_data *dst, const struct miel_hvac_data *src)
{
	bool publish = (memcmp(dst, src, sizeof(*dst)) != 0);
	*dst = *src;

	if (publish)
		MqttPublishSensor();
}

static void
miel_hvac_input_data(struct miel_hvac_softc *sc,
	const void *buf, size_t len)
{
	const struct miel_hvac_data *d;

	miel_hvac_log_bytes(sc, "data", buf, len);
	if (len < sizeof(*d))
	{
		AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME
			": short data response (%zu < %zu)"), len, sizeof(*d));
		return;
	}
	d = (const struct miel_hvac_data *)buf;

	switch (d->type)
	{
	case MIEL_HVAC_DATA_T_SETTINGS:
		if (d->data.settings.temp05 != 0)
			sc->sc_temp_type = true;
		miel_hvac_input_settings(sc, d);
		break;
	case MIEL_HVAC_DATA_T_ROOMTEMP:
		if (d->data.roomtemp.temp05 != 0)
			sc->sc_temp_type = true;
		miel_hvac_input_sensor(sc, &sc->sc_roomtemp, d);
		break;
	case MIEL_HVAC_DATA_T_TIMERS:
		miel_hvac_input_sensor(sc, &sc->sc_timers, d);
		break;
	case MIEL_HVAC_DATA_T_STATUS:
		miel_hvac_input_sensor(sc, &sc->sc_status, d);
		/* Mark energy metering as supported once we see any non-zero
		 * power or energy value. Units that never report these will
		 * keep the flag false and the ENERGY sub-object stays hidden. */
		{
			const struct miel_hvac_data_status *st = &d->data.status;
			uint16_t p = ((uint16_t)st->operationpower << 8)
			           | (uint16_t)st->operationpower1;
			uint16_t e = ((uint16_t)st->operationenergy << 8)
			           | (uint16_t)st->operationenergy1;
			if (p != 0 || e != 0)
				sc->sc_has_energy = true;
		}
		break;
	case MIEL_HVAC_DATA_T_STAGE:
		miel_hvac_input_sensor(sc, &sc->sc_stage, d);
		break;
	case MIEL_HVAC_DATA_T_OPTIONS:
	{
		bool changed = (memcmp(&sc->sc_options, d, sizeof(sc->sc_options)) != 0);
		sc->sc_options = *d;
		if (changed)
		{
			MqttPublishSensor();
			miel_hvac_publish_settings(sc);
			TasRediscover();
		}
		break;
	}
	default:
		miel_hvac_data_response(sc, d);
		break;
	}
}

static void
miel_hvac_input_updated(struct miel_hvac_softc *sc,
	const void *buf, size_t len)
{
	miel_hvac_log_bytes(sc, "updated", buf, len);
}

/*
 * Typed setters shared by the console commands and (when enabled) the Modbus
 * RTU slave.  Each queues the matching wire update in the same
 * sc_settings_update / sc_runstate_update / sc_remotetemp_update structures the
 * console handlers use and returns the outcome so each front end can render its
 * own response.  The capability gating mirrors the checks in the miel_hvac_cmnd_*
 * handlers below; keep the two in sync.
 */

static uint8_t
miel_hvac_apply_power(struct miel_hvac_softc *sc, bool on)
{
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;

	update->flags |= htons(MIEL_HVAC_SETTINGS_F_POWER);
	update->power = on ? MIEL_HVAC_SETTINGS_POWER_ON : MIEL_HVAC_SETTINGS_POWER_OFF;

	return (MIEL_HVAC_APPLY_OK);
}

static uint8_t
miel_hvac_apply_mode(struct miel_hvac_softc *sc, uint8_t mode_raw)
{
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;
	uint8_t mode = mode_raw & MIEL_HVAC_SETTINGS_MODE_MASK;

	if (miel_hvac_map_byval(mode_raw,
	    miel_hvac_mode_map, nitems(miel_hvac_mode_map)) == NULL)
		return (MIEL_HVAC_APPLY_BAD_VALUE);

	/* heat_isee/dry_isee/cool_isee are reported by the unit only (i-See
	 * sensor active) and cannot be commanded on any interface. */
	if (mode_raw >= MIEL_HVAC_SETTINGS_MODE_HEAT_ISEE)
		return (MIEL_HVAC_APPLY_BAD_VALUE);

	if (sc->sc_caps.sc_caps_valid)
	{
		if (mode == MIEL_HVAC_SETTINGS_MODE_HEAT && !sc->sc_caps.cap_mode_heat)
			return (MIEL_HVAC_APPLY_UNSUPPORTED);
		if (mode == MIEL_HVAC_SETTINGS_MODE_DRY && !sc->sc_caps.cap_mode_dry)
			return (MIEL_HVAC_APPLY_UNSUPPORTED);
		if (mode == MIEL_HVAC_SETTINGS_MODE_FAN && !sc->sc_caps.cap_mode_fan)
			return (MIEL_HVAC_APPLY_UNSUPPORTED);
	}

	update->flags |= htons(MIEL_HVAC_SETTINGS_F_MODE);
	update->mode = mode_raw;

	return (MIEL_HVAC_APPLY_OK);
}

static uint8_t
miel_hvac_apply_hamode(struct miel_hvac_softc *sc, uint8_t mode_raw)
{
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;

	if (mode_raw == 0)
	{
		update->flags |= htons(MIEL_HVAC_SETTINGS_F_POWER);
		update->power = MIEL_HVAC_SETTINGS_POWER_OFF;
		return (MIEL_HVAC_APPLY_OK);
	}

	if (miel_hvac_map_byval(mode_raw,
	    miel_hvac_mode_map, nitems(miel_hvac_mode_map)) == NULL)
		return (MIEL_HVAC_APPLY_BAD_VALUE);

	/* *_isee mode variants are report-only, see miel_hvac_apply_mode(). */
	if (mode_raw >= MIEL_HVAC_SETTINGS_MODE_HEAT_ISEE)
		return (MIEL_HVAC_APPLY_BAD_VALUE);

	if (sc->sc_caps.sc_caps_valid)
	{
		uint8_t mode = mode_raw & MIEL_HVAC_SETTINGS_MODE_MASK;
		if (mode == MIEL_HVAC_SETTINGS_MODE_HEAT && !sc->sc_caps.cap_mode_heat)
			return (MIEL_HVAC_APPLY_UNSUPPORTED);
		if (mode == MIEL_HVAC_SETTINGS_MODE_DRY && !sc->sc_caps.cap_mode_dry)
			return (MIEL_HVAC_APPLY_UNSUPPORTED);
		if (mode == MIEL_HVAC_SETTINGS_MODE_FAN && !sc->sc_caps.cap_mode_fan)
			return (MIEL_HVAC_APPLY_UNSUPPORTED);
	}

	update->flags |= htons(MIEL_HVAC_SETTINGS_F_POWER)
	              |  htons(MIEL_HVAC_SETTINGS_F_MODE);
	update->power = MIEL_HVAC_SETTINGS_POWER_ON;
	update->mode = mode_raw;

	return (MIEL_HVAC_APPLY_OK);
}

static uint8_t
miel_hvac_apply_temp(struct miel_hvac_softc *sc, float degc)
{
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;

	if (degc < MIEL_HVAC_SETTINGS_TEMP_MIN || degc > MIEL_HVAC_SETTINGS_TEMP_MAX)
		return (MIEL_HVAC_APPLY_BAD_VALUE);

	if (sc->sc_caps.sc_caps_valid && sc->sc_caps.cap_temp_ranges)
	{
		const struct miel_hvac_capabilities *caps = &sc->sc_caps;
		const struct miel_hvac_data_settings *set =
			&sc->sc_settings.data.settings;
		uint8_t mode = set->mode & MIEL_HVAC_SETTINGS_MODE_MASK;
		float cap_min, cap_max;

		if (mode == MIEL_HVAC_SETTINGS_MODE_HEAT)
		{
			cap_min = (caps->temp_heat_min - 128) / 2.0f;
			cap_max = (caps->temp_heat_max - 128) / 2.0f;
		}
		else if (mode == MIEL_HVAC_SETTINGS_MODE_AUTO)
		{
			cap_min = (caps->temp_auto_min - 128) / 2.0f;
			cap_max = (caps->temp_auto_max - 128) / 2.0f;
		}
		else
		{
			cap_min = (caps->temp_cool_min - 128) / 2.0f;
			cap_max = (caps->temp_cool_max - 128) / 2.0f;
		}

		if (degc < cap_min || degc > cap_max)
			return (MIEL_HVAC_APPLY_UNSUPPORTED);
	}

	update->flags |= htons(MIEL_HVAC_SETTINGS_F_TEMP);
	if (!sc->sc_temp_type)
	{
		update->temp   = miel_hvac_deg2temp(sc->sc_temp_type, degc);
		update->temp05 = 0;
	}
	else
	{
		update->temp   = 0;
		update->temp05 = miel_hvac_deg2temp(sc->sc_temp_type, degc);
	}

	return (MIEL_HVAC_APPLY_OK);
}

static uint8_t
miel_hvac_apply_fan(struct miel_hvac_softc *sc, uint8_t fan_raw)
{
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;

	if (miel_hvac_map_byval(fan_raw,
	    miel_hvac_fan_map, nitems(miel_hvac_fan_map)) == NULL)
		return (MIEL_HVAC_APPLY_BAD_VALUE);

	if (sc->sc_caps.sc_caps_valid)
	{
		uint8_t fan_count = miel_hvac_get_fan_count(sc);
		if (fan_raw == MIEL_HVAC_SETTINGS_FAN_AUTO && !sc->sc_caps.cap_fan_auto)
			return (MIEL_HVAC_APPLY_UNSUPPORTED);
		if (fan_count > 0)
		{
			if (fan_raw == MIEL_HVAC_SETTINGS_FAN_QUIET && fan_count < 5)
				return (MIEL_HVAC_APPLY_UNSUPPORTED);
			if (fan_raw == MIEL_HVAC_SETTINGS_FAN_4 && fan_count < 4)
				return (MIEL_HVAC_APPLY_UNSUPPORTED);
		}
	}

	update->flags |= htons(MIEL_HVAC_SETTINGS_F_FAN);
	update->fan = fan_raw;

	return (MIEL_HVAC_APPLY_OK);
}

static uint8_t
miel_hvac_apply_vane(struct miel_hvac_softc *sc, uint8_t vane_raw)
{
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;

	if (miel_hvac_map_byval(vane_raw,
	    miel_hvac_vane_map, nitems(miel_hvac_vane_map)) == NULL)
		return (MIEL_HVAC_APPLY_BAD_VALUE);

	update->flags |= htons(MIEL_HVAC_SETTINGS_F_VANE);
	update->vane = vane_raw;

	return (MIEL_HVAC_APPLY_OK);
}

static uint8_t
miel_hvac_apply_prohibit(struct miel_hvac_softc *sc, uint8_t proh_raw)
{
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;

	if (miel_hvac_map_byval(proh_raw,
	    miel_hvac_prohibit_map, nitems(miel_hvac_prohibit_map)) == NULL)
		return (MIEL_HVAC_APPLY_BAD_VALUE);

	update->flags |= htons(MIEL_HVAC_SETTINGS_F_PROHIBIT);
	update->prohibit = proh_raw;

	return (MIEL_HVAC_APPLY_OK);
}

static uint8_t
miel_hvac_apply_widevane(struct miel_hvac_softc *sc, uint8_t wv_raw)
{
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;

	if (miel_hvac_map_byval(wv_raw,
	    miel_hvac_widevane_map, nitems(miel_hvac_widevane_map)) == NULL)
		return (MIEL_HVAC_APPLY_BAD_VALUE);

	update->flags |= htons(MIEL_HVAC_SETTINGS_F_WIDEVANE);
	update->widevane = wv_raw;

	return (MIEL_HVAC_APPLY_OK);
}

/* HVACSendCommand: raw widevane byte pass-through, no validation. */
static uint8_t
miel_hvac_apply_command_raw(struct miel_hvac_softc *sc, uint8_t byte)
{
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;

	update->flags |= htons(MIEL_HVAC_SETTINGS_F_WIDEVANE);
	update->widevane = byte;

	return (MIEL_HVAC_APPLY_OK);
}

static uint8_t
miel_hvac_apply_airdirection(struct miel_hvac_softc *sc, uint8_t dir)
{
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;

	if (miel_hvac_map_byval(dir,
	    miel_hvac_airdirection_map, nitems(miel_hvac_airdirection_map)) == NULL)
		return (MIEL_HVAC_APPLY_BAD_VALUE);

	if (sc->sc_caps.sc_caps_valid
	    && (!sc->sc_caps.cap_vane_v || !sc->sc_has_isee))
		return (MIEL_HVAC_APPLY_UNSUPPORTED);

	switch (dir)
	{
	case MIEL_HVAC_SETTINGS_AIRDIRECTION_INDIRECT:
	case MIEL_HVAC_SETTINGS_AIRDIRECTION_DIRECT:
	case MIEL_HVAC_SETTINGS_AIRDIRECTION_EVEN:
	{
		struct miel_hvac_msg_update_runstate *rs = &sc->sc_runstate_update;

		update->flags   |= htons(MIEL_HVAC_SETTINGS_F_WIDEVANE);
		update->widevane = MIEL_HVAC_SETTINGS_WIDEVANE_ISEE;

		rs->eight        = 0x08;
		rs->flags       |= htons(MIEL_HVAC_RUNSTATE_F_AIRDIRECTION);
		rs->airdirection = dir;
		break;
	}
	case MIEL_HVAC_SETTINGS_AIRDIRECTION_OFF:
		update->flags   |= htons(MIEL_HVAC_SETTINGS_F_WIDEVANE);
		update->widevane = 0x8c;
		break;
	}

	return (MIEL_HVAC_APPLY_OK);
}

static uint8_t
miel_hvac_apply_runstate(struct miel_hvac_softc *sc, uint16_t flag,
	uint8_t *field, bool on)
{
	struct miel_hvac_msg_update_runstate *update = &sc->sc_runstate_update;

	if (sc->sc_caps.sc_caps_valid && !sc->sc_caps.cap_run_state)
		return (MIEL_HVAC_APPLY_UNSUPPORTED);

	update->eight = 0x08;
	update->flags |= htons(flag);
	*field = on ? 0x01 : 0x00;

	return (MIEL_HVAC_APPLY_OK);
}

static inline uint8_t
miel_hvac_apply_purifier(struct miel_hvac_softc *sc, bool on)
{
	return (miel_hvac_apply_runstate(sc, MIEL_HVAC_RUNSTATE_F_PURIFIER,
	    &sc->sc_runstate_update.purifier, on));
}

static inline uint8_t
miel_hvac_apply_nightmode(struct miel_hvac_softc *sc, bool on)
{
	return (miel_hvac_apply_runstate(sc, MIEL_HVAC_RUNSTATE_F_NIGHTMODE,
	    &sc->sc_runstate_update.nightmode, on));
}

static inline uint8_t
miel_hvac_apply_econocool(struct miel_hvac_softc *sc, bool on)
{
	return (miel_hvac_apply_runstate(sc, MIEL_HVAC_RUNSTATE_F_ECONOCOOL,
	    &sc->sc_runstate_update.econocool, on));
}

/* temp_half is the remote temperature in 0.5 degC units. */
static uint8_t
miel_hvac_apply_remotetemp(struct miel_hvac_softc *sc, int temp_half, bool clear)
{
	struct miel_hvac_msg_update_remotetemp *update = &sc->sc_remotetemp_update;
	uint8_t control;

	if (clear)
	{
		control = MIEL_HVAC_REMOTETEMP_CLR;
		sc->sc_remotetemp_half   = 0;
		sc->sc_remotetemp_active = false;
	}
	else
	{
		int min_half = MIEL_HVAC_REMOTETEMP_MIN * 2;
		int max_half = MIEL_HVAC_REMOTETEMP_MAX * 2;

		if (temp_half < min_half)
			temp_half = min_half;
		else if (temp_half > max_half)
			temp_half = max_half;

		sc->sc_remotetemp_half           = temp_half;
		control                          = MIEL_HVAC_REMOTETEMP_SET;
		sc->sc_remotetemp_active         = true;
		sc->sc_remotetemp_last_call_time = millis();
	}

	memset(update, 0, sizeof(*update));
	update->seven    = 0x7;
	update->control  = control;
	update->temp_old = miel_hvac_remotetemp2old(sc->sc_remotetemp_half);
	update->temp     = miel_hvac_remotetemp2new(sc->sc_remotetemp_half);

	return (MIEL_HVAC_APPLY_OK);
}

static uint8_t
miel_hvac_apply_remotetemp_clear_time(struct miel_hvac_softc *sc,
	unsigned long clear_time)
{
	if (clear_time < 1000 || clear_time > 600000)
		return (MIEL_HVAC_APPLY_BAD_VALUE);

	sc->sc_remotetemp_auto_clear_time = clear_time;

	return (MIEL_HVAC_APPLY_OK);
}

#if defined(USE_MIEL_HVAC_MODBUS_SLAVE) && defined(ESP32)
/*********************************************************************************************\
 * Modbus RTU slave
 *
 * A second RS485 serial port that lets a PLC read every driver state and drive every
 * driver function.  Function codes 0x01-0x06, 0x0F and 0x10 are supported.
 *
 * Input registers (FC04) and holding-register reads (FC03) expose live state; holding
 * register / coil writes (FC06/0x10/0x05/0x0F) are translated into the miel_hvac_apply_*
 * setters above.  See the register map in the documentation.
\*********************************************************************************************/

static uint16_t
miel_hvac_mb_crc(const uint8_t *buf, size_t len)
{
	uint16_t crc = 0xffff;
	size_t i;
	uint8_t j;

	for (i = 0; i < len; i++)
	{
		crc ^= buf[i];
		for (j = 0; j < 8; j++)
			crc = (crc & 1) ? ((crc >> 1) ^ 0xa001) : (crc >> 1);
	}

	return (crc);
}

static uint32_t
miel_hvac_mb_t35_us(uint32_t baud)
{
	if (baud == 0)
		return (2000);
	if (baud > 19200)
		return (1750);
	/* 3.5 chars * 11 bits/char * 1e6 us / baud */
	return (38500000UL / baud);
}

static int16_t
miel_hvac_mb_x10(float v)
{
	v *= 10.0f;
	return ((int16_t)(v + (v >= 0 ? 0.5f : -0.5f)));
}

static void
miel_hvac_mb_reply(struct miel_hvac_mb_softc *mb, uint8_t *buf, uint16_t len)
{
	uint16_t crc;
	uint32_t since, quiet;

	if (mb->sc_no_reply)
		return;   /* master already re-polled - a late answer would derail it */

	crc = miel_hvac_mb_crc(buf, len);
	buf[len++] = (uint8_t)crc;
	buf[len++] = (uint8_t)(crc >> 8);

	/*
	 * Modbus RTU turnaround: keep quiet for ~3.5 char times after the last
	 * received byte so the master (and an auto-direction transceiver) has
	 * switched from transmit to receive before the response starts.
	 */
	since = micros() - mb->sc_last_us;
	quiet = mb->sc_t35_us > 3500 ? 3500 : mb->sc_t35_us;
	if (since < quiet)
		delayMicroseconds(quiet - since);

	if (mb->sc_txen_pin >= 0)
	{
		digitalWrite(mb->sc_txen_pin, HIGH);
		mb->sc_serial->write(buf, len);
		mb->sc_serial->flush();   /* hold DE until the frame has left the UART */
		digitalWrite(mb->sc_txen_pin, LOW);
	}
	else
	{
		/*
		 * Auto-direction transceiver: just queue the frame.  flush() is
		 * avoided here because on ESP32 it also discards the RX buffer,
		 * which would drop the next request on a busy bus.
		 */
		mb->sc_serial->write(buf, len);
	}
}

static void
miel_hvac_mb_exception(struct miel_hvac_mb_softc *mb, uint8_t fc, uint8_t code)
{
	uint8_t r[5];   /* 3-byte pdu + 2-byte crc appended by miel_hvac_mb_reply */

	r[0] = mb->sc_address;
	r[1] = fc | 0x80;
	r[2] = code;

	mb->sc_exceptions++;
	miel_hvac_mb_reply(mb, r, 3);
}

static uint8_t
miel_hvac_mb_exc_for(uint8_t r)
{
	switch (r)
	{
	case MIEL_HVAC_APPLY_OK:       return (0);
	case MIEL_HVAC_APPLY_BAD_ADDR: return (MIEL_HVAC_MB_EXC_ADDRESS);
	default:                       return (MIEL_HVAC_MB_EXC_VALUE);
	}
}

/* Read-only state map (FC04, and FC03 fallback for unmapped control regs). */
static uint16_t
miel_hvac_mb_reg_input(struct miel_hvac_softc *sc, uint16_t addr, bool *ok)
{
	const struct miel_hvac_data_settings *set = &sc->sc_settings.data.settings;
	const struct miel_hvac_data_roomtemp *rt = &sc->sc_roomtemp.data.roomtemp;
	const struct miel_hvac_data_timers *tm = &sc->sc_timers.data.timers;
	const struct miel_hvac_data_status *st = &sc->sc_status.data.status;
	const struct miel_hvac_data_stage *sg = &sc->sc_stage.data.stage;
	const struct miel_hvac_data_options *op = &sc->sc_options.data.options;
	const struct miel_hvac_capabilities *caps = &sc->sc_caps;
	bool has_set = (sc->sc_settings.type != 0);
	bool has_rt  = (sc->sc_roomtemp.type != 0);
	bool has_tm  = (sc->sc_timers.type != 0);
	bool has_st  = (sc->sc_status.type != 0);
	bool has_sg  = (sc->sc_stage.type != 0);
	bool has_op  = (sc->sc_options.type != 0);

	*ok = true;

	switch (addr)
	{
	case 0x0000: return (1);
	case 0x0001: return (sc->sc_connected ? 1 : 0);
	case 0x0002: return (caps->sc_caps_valid ? 1 : 0);
	case 0x0003: return (sc->sc_has_isee ? 1 : 0);
	case 0x0004: return (sc->sc_has_energy ? 1 : 0);
	case 0x0005: return (sc->sc_temp_type ? 1 : 0);
	case 0x0006: return (sc->sc_remotetemp_active ? 1 : 0);

	case 0x0010: return (has_set ? (set->power ? 1 : 0) : 0);
	case 0x0011: return (has_set ? (set->mode & MIEL_HVAC_SETTINGS_MODE_MASK) : 0);
	case 0x0012:
		if (!has_set) return (0);
		return ((uint16_t)miel_hvac_mb_x10(miel_hvac_temp2deg(sc->sc_temp_type,
		    set->temp05 != 0 ? set->temp05 : set->temp)));
	case 0x0013: return (has_set ? set->fan : 0);
	case 0x0014: return (has_set ? set->vane : 0);
	case 0x0015:
		/* report the wide-vane position only; the unit mixes the i-See
		 * sensor bit (0x80) into this byte. 0x80 = i-See direction mode. */
		if (!has_set)
			return (0);
		if (set->widevane == 0x80 || set->widevane == 0x28 || set->widevane == 0xaa)
			return (0x80);
		return (set->widevane & MIEL_HVAC_SETTINGS_WIDEVANE_MASK);
	case 0x0016: return (has_set ? set->prohibit : 0);
	case 0x0017: return (has_set ? set->airdirection : 0);
	case 0x0018: return (has_op ? (op->purifier ? 1 : 0) : 0);
	case 0x0019: return (has_op ? (op->nightmode ? 1 : 0) : 0);
	case 0x001a: return (has_op ? (op->econocool ? 1 : 0) : 0);

	case 0x0020:
		if (!has_rt) return (0);
		return ((uint16_t)miel_hvac_mb_x10(miel_hvac_roomtemp2deg(sc->sc_temp_type,
		    rt->temp05 != 0 ? rt->temp05 : rt->temp)));
	case 0x0021:
		if (!has_rt || rt->outdoortemp <= 1) return (0x8000);
		return ((uint16_t)miel_hvac_mb_x10(miel_hvac_outdoortemp2deg(rt->outdoortemp)));
	case 0x0022:
		if (!has_rt) return (0);
		return ((uint16_t)miel_hvac_mb_x10(miel_hvac_temp2deg(sc->sc_temp_type,
		    rt->settemp)));
	case 0x0023: return (has_st ? (st->compressor ? 1 : 0) : 0);
	case 0x0024: return (has_st ? st->compressorfrequency : 0);
	case 0x0025:
		if (!has_st) return (0);
		return (((uint16_t)st->operationpower << 8) | st->operationpower1);
	case 0x0026:
		if (!has_st) return (0);
		return (((uint16_t)st->operationenergy << 8) | st->operationenergy1);
	case 0x0027:
		if (!has_rt) return (0);
		return ((uint16_t)((((uint32_t)rt->operationtime << 16)
		    | ((uint32_t)rt->operationtime1 << 8)
		    | rt->operationtime2) >> 16));
	case 0x0028:
		if (!has_rt) return (0);
		return ((uint16_t)((((uint32_t)rt->operationtime << 16)
		    | ((uint32_t)rt->operationtime1 << 8)
		    | rt->operationtime2) & 0xffff));
	case 0x0029: return ((uint16_t)(int16_t)(sc->sc_remotetemp_half * 5));
	case 0x002a: return ((uint16_t)(sc->sc_remotetemp_auto_clear_time / 1000));

	case 0x0030: return (has_tm ? tm->mode : 0);
	case 0x0031: return (has_tm ? (uint16_t)tm->onminutes * 10 : 0);
	case 0x0032: return (has_tm ? (uint16_t)tm->offminutes * 10 : 0);
	case 0x0033: return (has_tm ? (uint16_t)tm->onminutesremaining * 10 : 0);
	case 0x0034: return (has_tm ? (uint16_t)tm->offminutesremaining * 10 : 0);

	case 0x0038: return (has_sg ? sg->operation : 0);
	case 0x0039: return (has_sg ? sg->fan : 0);
	case 0x003a: return (has_sg ? sg->mode : 0);

	case 0x0040: return (caps->sc_caps_valid ? 1 : 0);
	case 0x0041:
		if (!caps->sc_caps_valid) return (0);
		return ((caps->cap_mode_heat    ? (1 << 0) : 0)
		     |  (caps->cap_mode_dry     ? (1 << 1) : 0)
		     |  (caps->cap_mode_fan     ? (1 << 2) : 0)
		     |  (caps->cap_vane_v       ? (1 << 3) : 0)
		     |  (caps->cap_vane_swing   ? (1 << 4) : 0)
		     |  (caps->cap_fan_auto     ? (1 << 5) : 0)
		     |  (caps->cap_outdoor_temp ? (1 << 6) : 0)
		     |  (caps->cap_run_state    ? (1 << 7) : 0)
		     |  (caps->cap_ext_temp     ? (1 << 8) : 0)
		     |  (caps->cap_installer    ? (1 << 9) : 0));
	case 0x0042: return (miel_hvac_get_fan_count(sc));
	case 0x0043: return (caps->cap_temp_ranges ? (uint16_t)miel_hvac_mb_x10((caps->temp_cool_min - 128) / 2.0f) : 0);
	case 0x0044: return (caps->cap_temp_ranges ? (uint16_t)miel_hvac_mb_x10((caps->temp_cool_max - 128) / 2.0f) : 0);
	case 0x0045: return (caps->cap_temp_ranges ? (uint16_t)miel_hvac_mb_x10((caps->temp_heat_min - 128) / 2.0f) : 0);
	case 0x0046: return (caps->cap_temp_ranges ? (uint16_t)miel_hvac_mb_x10((caps->temp_heat_max - 128) / 2.0f) : 0);
	case 0x0047: return (caps->cap_temp_ranges ? (uint16_t)miel_hvac_mb_x10((caps->temp_auto_min - 128) / 2.0f) : 0);
	case 0x0048: return (caps->cap_temp_ranges ? (uint16_t)miel_hvac_mb_x10((caps->temp_auto_max - 128) / 2.0f) : 0);

	case 0x0050: return ((uint16_t)sc->sc_mb->sc_requests);
	case 0x0051: return ((uint16_t)sc->sc_mb->sc_crc_errors);
	case 0x0052: return ((uint16_t)sc->sc_mb->sc_exceptions);
	case 0x0053: return ((uint16_t)sc->sc_mb->sc_overruns);
	}

	if (addr <= 0x00ff)
		return (0);

	*ok = false;
	return (0);
}

/*
 * FC03 holding-register reads.
 *   0x0000..0x000e  read-back of the writable control registers
 *   0x000f..0x0017  mirror of selected read-only sensor values, so a master
 *                   that only speaks FC03 can still reach them
 */
#define MIEL_HVAC_MB_HOLD_MIRROR_BASE 0x000f
static uint16_t
miel_hvac_mb_reg_holding(struct miel_hvac_softc *sc, uint16_t addr, bool *ok)
{
	static const uint16_t control_of[] = {
		0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
		0x0018, 0x0019, 0x001a, 0x0011, 0x0029, 0x002a,
	};
	static const uint16_t mirror_of[] = {
		0x0020,   /* 0x000f room temperature C x10 */
		0x0023,   /* 0x0010 compressor 0/1 */
		0x0025,   /* 0x0011 instantaneous power W */
		0x0038,   /* 0x0012 stage operation */
		0x0039,   /* 0x0013 stage fan */
		0x003a,   /* 0x0014 stage mode */
		0x0050,   /* 0x0015 diagnostics: requests received */
		0x0051,   /* 0x0016 diagnostics: CRC errors */
		0x0001,   /* 0x0017 connected to unit 0/1 */
	};

	*ok = true;

	if (addr < nitems(control_of))
		return (miel_hvac_mb_reg_input(sc, control_of[addr], ok));
	if (addr == 0x000e)
		return (0);
	if (addr >= MIEL_HVAC_MB_HOLD_MIRROR_BASE
	    && addr < MIEL_HVAC_MB_HOLD_MIRROR_BASE + nitems(mirror_of))
		return (miel_hvac_mb_reg_input(sc,
		    mirror_of[addr - MIEL_HVAC_MB_HOLD_MIRROR_BASE], ok));

	*ok = false;
	return (0);
}

static uint8_t
miel_hvac_mb_write_reg(struct miel_hvac_softc *sc, uint16_t addr, uint16_t val)
{
	switch (addr)
	{
	case 0x0000: return (miel_hvac_apply_power(sc, val != 0));
	case 0x0001: return (miel_hvac_apply_mode(sc, val & 0xff));
	case 0x0002: return (miel_hvac_apply_temp(sc, (int16_t)val / 10.0f));
	case 0x0003: return (miel_hvac_apply_fan(sc, val & 0xff));
	case 0x0004: return (miel_hvac_apply_vane(sc, val & 0xff));
	case 0x0005: return (miel_hvac_apply_widevane(sc, val & 0xff));
	case 0x0006: return (miel_hvac_apply_prohibit(sc, val & 0xff));
	case 0x0007: return (miel_hvac_apply_airdirection(sc, val & 0xff));
	case 0x0008: return (miel_hvac_apply_purifier(sc, val != 0));
	case 0x0009: return (miel_hvac_apply_nightmode(sc, val != 0));
	case 0x000a: return (miel_hvac_apply_econocool(sc, val != 0));
	case 0x000b: return (miel_hvac_apply_hamode(sc, val & 0xff));
	case 0x000c:
		if (val == 0x7fff)
			return (miel_hvac_apply_remotetemp(sc, 0, true));
		/* val is degC x10; convert to 0.5 degC units (x10 -> x2 = /5) */
		return (miel_hvac_apply_remotetemp(sc,
		    ((int16_t)val * 2 + ((int16_t)val >= 0 ? 5 : -5)) / 10, false));
	case 0x000d:
		return (miel_hvac_apply_remotetemp_clear_time(sc,
		    (unsigned long)val * 1000UL));
	case 0x000e: return (miel_hvac_apply_command_raw(sc, val & 0xff));
	}

	return (MIEL_HVAC_APPLY_BAD_ADDR);
}

static bool
miel_hvac_mb_read_bit(struct miel_hvac_softc *sc, uint8_t fc, uint16_t addr, bool *ok)
{
	const struct miel_hvac_data_settings *set = &sc->sc_settings.data.settings;
	const struct miel_hvac_data_status *st = &sc->sc_status.data.status;
	const struct miel_hvac_data_stage *sg = &sc->sc_stage.data.stage;
	const struct miel_hvac_data_options *op = &sc->sc_options.data.options;

	*ok = true;

	if (fc == 0x02)   /* discrete inputs */
	{
		switch (addr)
		{
		case 0: return (sc->sc_connected);
		case 1: return (sc->sc_caps.sc_caps_valid);
		case 2: return (sc->sc_status.type != 0 && st->compressor);
		case 3: return (sc->sc_has_isee);
		case 4: return (sc->sc_has_energy);
		case 5: return (sc->sc_remotetemp_active);
		case 6: return (sc->sc_stage.type != 0
		    && sg->operation == MIEL_HVAC_STAGE_OPERATION_DEFROST);
		}
	}
	else              /* coils */
	{
		switch (addr)
		{
		case 0: return (sc->sc_settings.type != 0 && set->power);
		case 1: return (sc->sc_options.type != 0 && op->purifier);
		case 2: return (sc->sc_options.type != 0 && op->nightmode);
		case 3: return (sc->sc_options.type != 0 && op->econocool);
		case 4: return (sc->sc_remotetemp_active);
		}
	}

	*ok = false;
	return (false);
}

static uint8_t
miel_hvac_mb_write_coil(struct miel_hvac_softc *sc, uint16_t addr, bool on)
{
	switch (addr)
	{
	case 0: return (miel_hvac_apply_power(sc, on));
	case 1: return (miel_hvac_apply_purifier(sc, on));
	case 2: return (miel_hvac_apply_nightmode(sc, on));
	case 3: return (miel_hvac_apply_econocool(sc, on));
	case 4:
		/* coil 4: write 0 clears the remote-temp override, 1 is a no-op ack */
		return (on ? MIEL_HVAC_APPLY_OK
		           : miel_hvac_apply_remotetemp(sc, 0, true));
	}

	return (MIEL_HVAC_APPLY_BAD_ADDR);
}

static void
miel_hvac_mb_do_read_regs(struct miel_hvac_softc *sc, uint8_t fc)
{
	struct miel_hvac_mb_softc *mb = sc->sc_mb;
	uint16_t start = ((uint16_t)mb->sc_buf[2] << 8) | mb->sc_buf[3];
	uint16_t count = ((uint16_t)mb->sc_buf[4] << 8) | mb->sc_buf[5];
	uint8_t r[MIEL_HVAC_MB_BUFLEN];
	uint16_t i;

	if (mb->sc_len < 6)
		return;
	if (count < 1 || count > 125)
	{
		miel_hvac_mb_exception(mb, fc, MIEL_HVAC_MB_EXC_VALUE);
		return;
	}

	r[0] = mb->sc_address;
	r[1] = fc;
	r[2] = count * 2;
	for (i = 0; i < count; i++)
	{
		bool ok = true;
		uint16_t v = (fc == 0x03)
			? miel_hvac_mb_reg_holding(sc, start + i, &ok)
			: miel_hvac_mb_reg_input(sc, start + i, &ok);
		if (!ok)
		{
			miel_hvac_mb_exception(mb, fc, MIEL_HVAC_MB_EXC_ADDRESS);
			return;
		}
		r[3 + i * 2]     = (uint8_t)(v >> 8);
		r[3 + i * 2 + 1] = (uint8_t)v;
	}

	miel_hvac_mb_reply(mb, r, 3 + count * 2);
}

static void
miel_hvac_mb_do_read_bits(struct miel_hvac_softc *sc, uint8_t fc)
{
	struct miel_hvac_mb_softc *mb = sc->sc_mb;
	uint16_t start = ((uint16_t)mb->sc_buf[2] << 8) | mb->sc_buf[3];
	uint16_t count = ((uint16_t)mb->sc_buf[4] << 8) | mb->sc_buf[5];
	uint8_t r[MIEL_HVAC_MB_BUFLEN];
	uint16_t nbytes, i;

	if (mb->sc_len < 6)
		return;
	if (count < 1 || count > 2000)
	{
		miel_hvac_mb_exception(mb, fc, MIEL_HVAC_MB_EXC_VALUE);
		return;
	}

	nbytes = (count + 7) / 8;
	r[0] = mb->sc_address;
	r[1] = fc;
	r[2] = nbytes;
	memset(&r[3], 0, nbytes);
	for (i = 0; i < count; i++)
	{
		bool ok = true;
		if (miel_hvac_mb_read_bit(sc, fc, start + i, &ok))
			r[3 + (i / 8)] |= (1 << (i % 8));
		if (!ok)
		{
			miel_hvac_mb_exception(mb, fc, MIEL_HVAC_MB_EXC_ADDRESS);
			return;
		}
	}

	miel_hvac_mb_reply(mb, r, 3 + nbytes);
}

static void
miel_hvac_mb_do_write_single_reg(struct miel_hvac_softc *sc, bool broadcast)
{
	struct miel_hvac_mb_softc *mb = sc->sc_mb;
	uint16_t addr = ((uint16_t)mb->sc_buf[2] << 8) | mb->sc_buf[3];
	uint16_t val  = ((uint16_t)mb->sc_buf[4] << 8) | mb->sc_buf[5];
	uint8_t res;

	if (mb->sc_len < 6)
		return;

	res = miel_hvac_mb_write_reg(sc, addr, val);
	if (broadcast)
		return;
	if (res != MIEL_HVAC_APPLY_OK)
	{
		miel_hvac_mb_exception(mb, 0x06, miel_hvac_mb_exc_for(res));
		return;
	}
	miel_hvac_mb_reply(mb, mb->sc_buf, 6);   /* echo request */
}

static void
miel_hvac_mb_do_write_multi_reg(struct miel_hvac_softc *sc, bool broadcast)
{
	struct miel_hvac_mb_softc *mb = sc->sc_mb;
	uint16_t start = ((uint16_t)mb->sc_buf[2] << 8) | mb->sc_buf[3];
	uint16_t count = ((uint16_t)mb->sc_buf[4] << 8) | mb->sc_buf[5];
	uint8_t bc = mb->sc_buf[6];
	uint8_t r[8];   /* 6-byte pdu + 2-byte crc */
	uint16_t i;

	if (mb->sc_len < 7 || mb->sc_len < 7 + bc)
		return;
	if (count < 1 || count > 123 || bc != count * 2)
	{
		if (!broadcast)
			miel_hvac_mb_exception(mb, 0x10, MIEL_HVAC_MB_EXC_VALUE);
		return;
	}

	for (i = 0; i < count; i++)
	{
		uint16_t val = ((uint16_t)mb->sc_buf[7 + i * 2] << 8)
		             | mb->sc_buf[7 + i * 2 + 1];
		uint8_t res =
			miel_hvac_mb_write_reg(sc, start + i, val);
		if (res != MIEL_HVAC_APPLY_OK)
		{
			if (!broadcast)
				miel_hvac_mb_exception(mb, 0x10, miel_hvac_mb_exc_for(res));
			return;
		}
	}

	if (broadcast)
		return;
	memcpy(r, mb->sc_buf, 6);
	miel_hvac_mb_reply(mb, r, 6);
}

static void
miel_hvac_mb_do_write_single_coil(struct miel_hvac_softc *sc, bool broadcast)
{
	struct miel_hvac_mb_softc *mb = sc->sc_mb;
	uint16_t addr = ((uint16_t)mb->sc_buf[2] << 8) | mb->sc_buf[3];
	uint16_t val  = ((uint16_t)mb->sc_buf[4] << 8) | mb->sc_buf[5];
	uint8_t res;

	if (mb->sc_len < 6)
		return;
	if (val != 0xff00 && val != 0x0000)
	{
		if (!broadcast)
			miel_hvac_mb_exception(mb, 0x05, MIEL_HVAC_MB_EXC_VALUE);
		return;
	}

	res = miel_hvac_mb_write_coil(sc, addr, val == 0xff00);
	if (broadcast)
		return;
	if (res != MIEL_HVAC_APPLY_OK)
	{
		miel_hvac_mb_exception(mb, 0x05, miel_hvac_mb_exc_for(res));
		return;
	}
	miel_hvac_mb_reply(mb, mb->sc_buf, 6);   /* echo request */
}

static void
miel_hvac_mb_do_write_multi_coil(struct miel_hvac_softc *sc, bool broadcast)
{
	struct miel_hvac_mb_softc *mb = sc->sc_mb;
	uint16_t start = ((uint16_t)mb->sc_buf[2] << 8) | mb->sc_buf[3];
	uint16_t count = ((uint16_t)mb->sc_buf[4] << 8) | mb->sc_buf[5];
	uint8_t bc = mb->sc_buf[6];
	uint8_t r[8];   /* 6-byte pdu + 2-byte crc */
	uint16_t i;

	if (mb->sc_len < 7 || mb->sc_len < 7 + bc)
		return;
	if (count < 1 || count > 0x07b0 || bc != (count + 7) / 8)
	{
		if (!broadcast)
			miel_hvac_mb_exception(mb, 0x0f, MIEL_HVAC_MB_EXC_VALUE);
		return;
	}

	for (i = 0; i < count; i++)
	{
		bool on = (mb->sc_buf[7 + (i / 8)] >> (i % 8)) & 1;
		uint8_t res =
			miel_hvac_mb_write_coil(sc, start + i, on);
		if (res != MIEL_HVAC_APPLY_OK)
		{
			if (!broadcast)
				miel_hvac_mb_exception(mb, 0x0f, miel_hvac_mb_exc_for(res));
			return;
		}
	}

	if (broadcast)
		return;
	memcpy(r, mb->sc_buf, 6);
	miel_hvac_mb_reply(mb, r, 6);
}

/*
 * Total length (including the 2-byte CRC) of the RTU request whose leading
 * bytes are in buf, or 0 if it cannot be determined yet / the function code is
 * unknown.  Used to frame incoming requests by length rather than by relying on
 * the T3.5 inter-frame gap, which a cooperatively-scheduled FUNC_LOOP poll is
 * too coarse to measure reliably above 9600 baud.
 */
static uint16_t
miel_hvac_mb_framelen(const uint8_t *buf, uint16_t len)
{
	if (len < 2)
		return (0);

	switch (buf[1])
	{
	case 0x01: case 0x02: case 0x03: case 0x04:
	case 0x05: case 0x06:
		return (8);
	case 0x0f: case 0x10:
		if (len < 7)
			return (0);
		return (7 + buf[6] + 2);
	default:
		return (0);
	}
}

/* Dispatch one CRC-validated request; sc_buf holds the PDU without the CRC. */
static void
miel_hvac_mb_handle(struct miel_hvac_softc *sc)
{
	struct miel_hvac_mb_softc *mb = sc->sc_mb;
	uint8_t fc = mb->sc_buf[1];
	bool broadcast = (mb->sc_buf[0] == 0);

	mb->sc_requests++;

	switch (fc)
	{
	case 0x01:
	case 0x02:
		if (!broadcast)
			miel_hvac_mb_do_read_bits(sc, fc);
		break;
	case 0x03:
	case 0x04:
		if (!broadcast)
			miel_hvac_mb_do_read_regs(sc, fc);
		break;
	/*
	 * Writes are accepted whether or not the unit is connected yet - the
	 * update is queued and sent once the link is up, matching the HVACSet*
	 * console commands.
	 */
	case 0x05:
		miel_hvac_mb_do_write_single_coil(sc, broadcast);
		break;
	case 0x06:
		miel_hvac_mb_do_write_single_reg(sc, broadcast);
		break;
	case 0x0f:
		miel_hvac_mb_do_write_multi_coil(sc, broadcast);
		break;
	case 0x10:
		miel_hvac_mb_do_write_multi_reg(sc, broadcast);
		break;
	default:
		if (!broadcast)
			miel_hvac_mb_exception(mb, fc, MIEL_HVAC_MB_EXC_FUNCTION);
		break;
	}
}

/*
 * Try to consume one request from the front of sc_buf.  Returns the number of
 * leading bytes dealt with (0 = wait for more data).  On a framing/CRC mismatch
 * a single byte is dropped so a genuine frame starting later in the buffer can
 * still be recovered without waiting for the bus to fall idle - important on a
 * shared bus where our slave also sees every other device's traffic.
 */
static uint16_t
miel_hvac_mb_consume(struct miel_hvac_softc *sc)
{
	struct miel_hvac_mb_softc *mb = sc->sc_mb;
	uint16_t need, crc;

	if (mb->sc_len < 4)
		return (0);

	need = miel_hvac_mb_framelen(mb->sc_buf, mb->sc_len);
	if (need == 0)
	{
		/* FC 0x0f / 0x10 byte-count field not received yet -> wait */
		if ((mb->sc_buf[1] == 0x0f || mb->sc_buf[1] == 0x10) && mb->sc_len < 7)
			return (0);
		return (1);   /* unknown function code -> slide to resync */
	}
	if (mb->sc_len < need)
		return (0);   /* rest of the frame is still on the wire */
	if (need > MIEL_HVAC_MB_BUFLEN)
		return (1);   /* absurd length -> resync */

	crc = ((uint16_t)mb->sc_buf[need - 1] << 8) | mb->sc_buf[need - 2];
	if (miel_hvac_mb_crc(mb->sc_buf, need - 2) != crc)
	{
		if (mb->sc_buf[0] == mb->sc_address)
			mb->sc_crc_errors++;   /* count only frames that claim to be ours */
		return (1);
	}

	if (mb->sc_buf[0] == mb->sc_address || mb->sc_buf[0] == 0)
	{
		uint16_t total = mb->sc_len;

		/*
		 * If another request is already buffered behind this one, the master
		 * has timed out and re-polled: apply this (stale) request but do not
		 * put a late response on the bus - it would land on the master's next
		 * transaction and make that one fail too.
		 */
		mb->sc_no_reply = (total > need
		    && (mb->sc_buf[need] == mb->sc_address || mb->sc_buf[need] == 0));

		mb->sc_len = need - 2;       /* handlers see the PDU without the CRC */
		miel_hvac_mb_handle(sc);
		mb->sc_len = total;
		mb->sc_no_reply = false;
	}
	/* else: a valid frame for another slave - consumed silently */

	return (need);
}

static void
miel_hvac_mb_loop(struct miel_hvac_softc *sc)
{
	struct miel_hvac_mb_softc *mb = sc->sc_mb;
	TasmotaSerial *serial = mb->sc_serial;
	int avail;

	while ((avail = serial->available()) > 0)
	{
		uint16_t room = MIEL_HVAC_MB_BUFLEN - mb->sc_len;
		size_t want, n;

		if (room == 0)
		{
			mb->sc_overruns++;
			mb->sc_len = 0;   /* buffer full of unparseable data -> resync */
			room = MIEL_HVAC_MB_BUFLEN;
		}
		want = ((size_t)avail < room) ? (size_t)avail : room;
		n = serial->read(&mb->sc_buf[mb->sc_len], want);
		if (n == 0)
			break;
		mb->sc_len += n;
		mb->sc_last_us = micros();   /* needed by the turnaround wait in reply */

		for (;;)
		{
			uint16_t used = miel_hvac_mb_consume(sc);
			uint16_t rem;

			if (used == 0)
				break;
			rem = mb->sc_len > used ? mb->sc_len - used : 0;
			if (rem)
				memmove(mb->sc_buf, &mb->sc_buf[used], rem);
			mb->sc_len = rem;
		}
	}

	/* drop a stale sub-minimal fragment once the bus has gone idle */
	if (mb->sc_len > 0 && mb->sc_len < 4)
	{
		uint32_t gap = mb->sc_t35_us > 4000 ? mb->sc_t35_us : 4000;
		if ((micros() - mb->sc_last_us) > gap)
			mb->sc_len = 0;
	}
}

static void
miel_hvac_mb_apply_config(struct miel_hvac_softc *sc)
{
	struct miel_hvac_mb_softc *mb = sc->sc_mb;

	if (mb == NULL)
		return;

	mb->sc_address  = Settings->miel_hvac_mb_address;
	mb->sc_sconfig  = Settings->miel_hvac_mb_sconfig;
	mb->sc_baudrate = (uint32_t)Settings->miel_hvac_mb_baudrate * 300;
	mb->sc_t35_us   = miel_hvac_mb_t35_us(mb->sc_baudrate);
	mb->sc_len      = 0;
	mb->sc_serial->begin(mb->sc_baudrate, ConvertSerialConfig(mb->sc_sconfig));
	mb->sc_last_us  = micros();
}

static void
miel_hvac_mb_settings_clamp(void)
{
	uint32_t baud;

	if (Settings->miel_hvac_mb_address < 1 || Settings->miel_hvac_mb_address > 247)
		Settings->miel_hvac_mb_address = MIEL_HVAC_MB_DEFAULT_ADDRESS;

	baud = (uint32_t)Settings->miel_hvac_mb_baudrate * 300;
	if (baud < MIEL_HVAC_MB_MIN_BAUDRATE || baud > MIEL_HVAC_MB_MAX_BAUDRATE)
		Settings->miel_hvac_mb_baudrate = MIEL_HVAC_MB_DEFAULT_BAUDRATE / 300;

	/* Modbus RTU is always 8 data bits; anything else (incl. a zeroed field) -> 8N1 */
	if ((Settings->miel_hvac_mb_sconfig & 0x03) != 0x03
	    || Settings->miel_hvac_mb_sconfig > TS_SERIAL_8O2)
		Settings->miel_hvac_mb_sconfig = TS_SERIAL_8N1;
}

static void
miel_hvac_mb_init(struct miel_hvac_softc *sc)
{
	struct miel_hvac_mb_softc *mb;

	if (sc->sc_mb != NULL)
		return;
	if (!Settings->sbflag1.miel_hvac_mb_enable)
		return;
	if (!PinUsed(GPIO_MIEL_HVAC_MB_RX) || !PinUsed(GPIO_MIEL_HVAC_MB_TX))
		return;

	mb = (struct miel_hvac_mb_softc *)calloc(1, sizeof(*mb));
	if (mb == NULL)
	{
		AddLog(LOG_LEVEL_ERROR, PSTR(MIEL_HVAC_LOGNAME
			": modbus: unable to allocate state"));
		return;
	}

	miel_hvac_mb_settings_clamp();

	mb->sc_address  = Settings->miel_hvac_mb_address;
	mb->sc_sconfig  = Settings->miel_hvac_mb_sconfig;
	mb->sc_baudrate = (uint32_t)Settings->miel_hvac_mb_baudrate * 300;
	mb->sc_t35_us   = miel_hvac_mb_t35_us(mb->sc_baudrate);
	mb->sc_txen_pin = PinUsed(GPIO_MIEL_HVAC_MB_TXEN) ? Pin(GPIO_MIEL_HVAC_MB_TXEN) : -1;

	/* Large RX buffer: on a shared bus we also see every other slave's
	 * traffic and FUNC_LOOP can be milliseconds late. */
	mb->sc_serial = new TasmotaSerial(Pin(GPIO_MIEL_HVAC_MB_RX),
	    Pin(GPIO_MIEL_HVAC_MB_TX), 2, 0, 1024);
	if (!mb->sc_serial->begin(mb->sc_baudrate,
	    ConvertSerialConfig(mb->sc_sconfig)))
	{
		AddLog(LOG_LEVEL_ERROR, PSTR(MIEL_HVAC_LOGNAME
			": modbus: unable to begin serial (baudrate %u)"), mb->sc_baudrate);
		delete mb->sc_serial;
		free(mb);
		return;
	}

	if (mb->sc_txen_pin >= 0)
	{
		pinMode(mb->sc_txen_pin, OUTPUT);
		digitalWrite(mb->sc_txen_pin, LOW);
	}
	mb->sc_last_us = micros();
	sc->sc_mb = mb;

	AddLog(LOG_LEVEL_INFO, PSTR(MIEL_HVAC_LOGNAME
		": modbus RTU slave: address %u, %u baud, UART%d"),
		mb->sc_address, mb->sc_baudrate, mb->sc_serial->getUart());
}

static void
miel_hvac_mb_deinit(struct miel_hvac_softc *sc)
{
	if (sc->sc_mb == NULL)
		return;

	delete sc->sc_mb->sc_serial;
	free(sc->sc_mb);
	sc->sc_mb = nullptr;
}

/* console commands */

static void
miel_hvac_cmnd_modbus(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;

	if (XdrvMailbox.data_len > 0)
	{
		bool on = (strtol(XdrvMailbox.data, nullptr, 10) != 0);
		Settings->sbflag1.miel_hvac_mb_enable = on;
		if (on)
			miel_hvac_mb_init(sc);
		else
			miel_hvac_mb_deinit(sc);
	}

	ResponseCmndStateText(Settings->sbflag1.miel_hvac_mb_enable);
}

static void
miel_hvac_cmnd_modbus_address(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;

	if (XdrvMailbox.data_len > 0)
	{
		long v = strtol(XdrvMailbox.data, nullptr, 10);
		if (v < 1 || v > 247)
		{
			miel_hvac_respond_unsupported();
			return;
		}
		Settings->miel_hvac_mb_address = (uint8_t)v;
		if (sc->sc_mb != NULL)
			sc->sc_mb->sc_address = (uint8_t)v;
	}

	ResponseCmndNumber(Settings->miel_hvac_mb_address);
}

static void
miel_hvac_cmnd_modbus_baudrate(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;

	if (XdrvMailbox.data_len > 0)
	{
		uint32_t baud = strtoul(XdrvMailbox.data, nullptr, 10);
		if (baud < MIEL_HVAC_MB_MIN_BAUDRATE || baud > MIEL_HVAC_MB_MAX_BAUDRATE)
		{
			miel_hvac_respond_unsupported();
			return;
		}
		Settings->miel_hvac_mb_baudrate = baud / 300;
		miel_hvac_mb_apply_config(sc);
	}

	ResponseCmndNumber((uint32_t)Settings->miel_hvac_mb_baudrate * 300);
}

static void
miel_hvac_cmnd_modbus_config(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;

	if (XdrvMailbox.data_len > 0)
	{
		int8_t cfg = ParseSerialConfig(XdrvMailbox.data);
		if (cfg < 0 || (cfg & 0x03) != 0x03 || cfg > TS_SERIAL_8O2)
		{
			miel_hvac_respond_unsupported();
			return;
		}
		Settings->miel_hvac_mb_sconfig = (uint8_t)cfg;
		miel_hvac_mb_apply_config(sc);
	}

	ResponseCmndChar(GetSerialConfig(Settings->miel_hvac_mb_sconfig).c_str());
}

static void
miel_hvac_mb_sensor(struct miel_hvac_softc *sc)
{
	struct miel_hvac_mb_softc *mb = sc->sc_mb;

	ResponseAppend_P(PSTR(",\"Modbus\":{\"Enable\":\"%s\",\"Address\":%u,"
		"\"Baudrate\":%u,\"Config\":\"%s\""),
		Settings->sbflag1.miel_hvac_mb_enable ? "on" : "off",
		Settings->miel_hvac_mb_address,
		(uint32_t)Settings->miel_hvac_mb_baudrate * 300,
		GetSerialConfig(Settings->miel_hvac_mb_sconfig).c_str());
	if (mb != NULL)
	{
		ResponseAppend_P(PSTR(",\"Requests\":%u,\"CrcErrors\":%u,"
			"\"Exceptions\":%u,\"Overruns\":%u"),
			mb->sc_requests, mb->sc_crc_errors,
			mb->sc_exceptions, mb->sc_overruns);
	}
	ResponseAppend_P(PSTR("}"));
}
#endif  /* USE_MIEL_HVAC_MODBUS_SLAVE && ESP32 */

/* FUNC handlers */

static void
miel_hvac_pre_init(void)
{
	struct miel_hvac_softc *sc;
	int baudrate = 2400;

	if (!PinUsed(GPIO_MIEL_HVAC_TX) || !PinUsed(GPIO_MIEL_HVAC_RX))
		return;

	sc = (struct miel_hvac_softc *)malloc(sizeof(*sc));
	if (sc == NULL)
	{
		AddLog(LOG_LEVEL_ERROR, PSTR(MIEL_HVAC_LOGNAME
			": unable to allocate state"));
		return;
	}

	memset(sc, 0, sizeof(*sc));
	sc->sc_remotetemp_auto_clear_time = 10000;
	miel_hvac_init_update_settings(&sc->sc_settings_update);

	sc->sc_serial = new TasmotaSerial(
		Pin(GPIO_MIEL_HVAC_RX), Pin(GPIO_MIEL_HVAC_TX), 2);

	if (!sc->sc_serial->begin(baudrate, SERIAL_8E1))
	{
		AddLog(LOG_LEVEL_ERROR, PSTR(MIEL_HVAC_LOGNAME
			": unable to begin serial (baudrate %d)"), baudrate);
		delete sc->sc_serial;
		free(sc);
		return;
	}

	if (sc->sc_serial->hardwareSerial())
	{
		ClaimSerial();
		SetSerial(baudrate, TS_SERIAL_8E1);
	}
#ifdef ESP32
	AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME ": Serial UART%d"),
		sc->sc_serial->getUart());
#endif

	sc->sc_device = TasmotaGlobal.devices_present;
	UpdateDevicesPresent(1);

	miel_hvac_sc = sc;

#if defined(USE_MIEL_HVAC_MODBUS_SLAVE) && defined(ESP32)
	/* Normalise the persisted config even when the slave is disabled, so the
	 * SENSOR block and the HVACModbus* queries never report the zeroed field. */
	miel_hvac_mb_settings_clamp();
	miel_hvac_mb_init(sc);
#endif
}

static void
miel_hvac_loop(struct miel_hvac_softc *sc)
{
	TasmotaSerial *serial = sc->sc_serial;

	while (serial->available())
	{
		yield();
		sc->sc_parser.p_state = miel_hvac_parse(sc, serial->read());
	}
}

static void
miel_hvac_sensor(struct miel_hvac_softc *sc)
{
	const char *name;

	ResponseAppend_P(PSTR(",\"MiElHVAC\":{"));

	/* Settings (power, mode, temp, fan, vane, widevane, prohibit, purifier, nightmode) */
	if (sc->sc_settings.type != 0)
		miel_hvac_append_settings_json(sc);

	/* Room temperature */
	if (sc->sc_roomtemp.type != 0)
	{
		const struct miel_hvac_data_roomtemp *rt =
			&sc->sc_roomtemp.data.roomtemp;
		char hex[(sizeof(sc->sc_roomtemp) + 1) * 2];
		char room_temp[33];
		char remote_temp[33];

		if (rt->temp05 == 0)
		{
			unsigned int t = miel_hvac_roomtemp2deg(sc->sc_temp_type, rt->temp);
			dtostrfd(ConvertTemp(t),
				Settings->flag2.temperature_resolution, room_temp);
		}
		else
		{
			float t = miel_hvac_roomtemp2deg(sc->sc_temp_type, rt->temp05);
			dtostrfd(ConvertTemp(t),
				Settings->flag2.temperature_resolution, room_temp);
		}
		ResponseAppend_P(PSTR(",\"RoomTemperature\":%s"), room_temp);

		dtostrfd(sc->sc_remotetemp_half / 2.0, 1, remote_temp);
		ResponseAppend_P(PSTR(",\"RemoteTemperature\":%s"), remote_temp);

		/* "on" = remote temperature override is active */
		ResponseAppend_P(PSTR(",\"RemoteTemperatureSensorState\":\"%s\""),
			sc->sc_remotetemp_active ? "on" : "off");

		ResponseAppend_P(PSTR(",\"RemoteTemperatureSensorAutoClearTime\":%lu"),
			sc->sc_remotetemp_auto_clear_time);

		if (rt->outdoortemp > 1)
		{
			char outdoor_temp[33];
			float t = miel_hvac_outdoortemp2deg(rt->outdoortemp);
			dtostrfd(ConvertTemp(t), 1, outdoor_temp);
			ResponseAppend_P(PSTR(",\"OutdoorTemperature\":%s"), outdoor_temp);
		}

		/* operationtime: 3-byte big-endian, minutes */
		uint32_t combined_time =
			((uint32_t)rt->operationtime  << 16) |
			((uint32_t)rt->operationtime1 << 8)  |
			 (uint32_t)rt->operationtime2;
		char operationtime[33];
		dtostrf((float)combined_time, 1, 0, operationtime);
		ResponseAppend_P(PSTR(",\"OperationTime\":%s"), operationtime);

		ResponseAppend_P(PSTR(",\"RoomTempHex\":\"%s\""),
			ToHex_P((uint8_t *)&sc->sc_roomtemp,
				sizeof(sc->sc_roomtemp), hex, sizeof(hex)));
	}

	/* Timers */
	if (sc->sc_timers.type != 0)
	{
		const struct miel_hvac_data_timers *timer =
			&sc->sc_timers.data.timers;
		char hex[(sizeof(sc->sc_timers) + 1) * 2];

		name = miel_hvac_map_byval(timer->mode,
			miel_hvac_timer_mode_map, nitems(miel_hvac_timer_mode_map));
		if (name != NULL)
		{
			ResponseAppend_P(PSTR(",\"TimerMode\":\"%s\""), name);

			char buf[33];

			utoa(timer->onminutes * 10, buf, 10);
			ResponseAppend_P(PSTR(",\"TimerOn\":%s"), buf);

			utoa(timer->onminutesremaining * 10, buf, 10);
			ResponseAppend_P(PSTR(",\"TimerOnRemaining\":%s"), buf);

			utoa(timer->offminutes * 10, buf, 10);
			ResponseAppend_P(PSTR(",\"TimerOff\":%s"), buf);

			utoa(timer->offminutesremaining * 10, buf, 10);
			ResponseAppend_P(PSTR(",\"TimerOffRemaining\":%s"), buf);
		}

		ResponseAppend_P(PSTR(",\"TimersHex\":\"%s\""),
			ToHex_P((uint8_t *)&sc->sc_timers,
				sizeof(sc->sc_timers), hex, sizeof(hex)));
	}

	/* Status */
	if (sc->sc_status.type != 0)
	{
		const struct miel_hvac_data_status *status =
			&sc->sc_status.data.status;
		char hex[(sizeof(sc->sc_status) + 1) * 2];
		char buf[33];

		name = miel_hvac_map_byval(status->compressor,
			miel_hvac_compressor_map, nitems(miel_hvac_compressor_map));
		ResponseAppend_P(PSTR(",\"CompressorState\":\"%s\""),
			name != NULL ? name : "N/A");

		utoa(status->compressorfrequency, buf, 10);
		ResponseAppend_P(PSTR(",\"CompressorFrequency\":%s"), buf);

		/* Power / Energy — only when the unit reports energy metering. */
		if (sc->sc_has_energy)
		{
			uint16_t combined_power =
				((uint16_t)status->operationpower << 8) |
				 (uint16_t)status->operationpower1;
			dtostrfd((float)combined_power, 0, buf);
			ResponseAppend_P(PSTR(",\"Power\":%s"), buf);

			uint16_t combined_energy =
				((uint16_t)status->operationenergy << 8) |
				 (uint16_t)status->operationenergy1;
			dtostrfd((float)combined_energy / 10.0f, 1, buf);
			ResponseAppend_P(PSTR(",\"Energy\":%s"), buf);
		}

		ResponseAppend_P(PSTR(",\"StatusHex\":\"%s\""),
			ToHex_P((uint8_t *)&sc->sc_status,
				sizeof(sc->sc_status), hex, sizeof(hex)));
	}

	/* Stage */
	if (sc->sc_stage.type != 0)
	{
		const struct miel_hvac_data_stage *stage =
			&sc->sc_stage.data.stage;
		char hex[(sizeof(sc->sc_stage) + 1) * 2];

		name = miel_hvac_map_byval(stage->operation,
			miel_hvac_stage_operation_map,
			nitems(miel_hvac_stage_operation_map));
		if (name != NULL)
			ResponseAppend_P(PSTR(",\"OperationStage\":\"%s\""), name);

		name = miel_hvac_map_byval(stage->fan,
			miel_hvac_stage_fan_map, nitems(miel_hvac_stage_fan_map));
		if (name != NULL)
			ResponseAppend_P(PSTR(",\"FanStage\":\"%s\""), name);

		name = miel_hvac_map_byval(stage->mode,
			miel_hvac_stage_mode_map, nitems(miel_hvac_stage_mode_map));
		if (name != NULL)
			ResponseAppend_P(PSTR(",\"ModeStage\":\"%s\""), name);

		ResponseAppend_P(PSTR(",\"StageHex\":\"%s\""),
			ToHex_P((uint8_t *)&sc->sc_stage,
				sizeof(sc->sc_stage), hex, sizeof(hex)));
	}

	/* Options raw hex — Purifier/NightMode/EconoCool already in settings block above. */
	if ((!sc->sc_caps.sc_caps_valid || sc->sc_caps.cap_run_state)
		&& sc->sc_options.type != 0)
	{
		char hex[(sizeof(sc->sc_options) + 1) * 2];
		ResponseAppend_P(PSTR(",\"OptionsHex\":\"%s\""),
			ToHex_P((uint8_t *)&sc->sc_options,
				sizeof(sc->sc_options), hex, sizeof(hex)));
	}

	/* Capabilities from 0x7B 0xC9 Base Capabilities response */
	if (sc->sc_caps.sc_caps_valid)
	{
		const struct miel_hvac_capabilities *caps = &sc->sc_caps;
		char hex_caps[(16 + 1) * 2];

		ResponseAppend_P(PSTR(","
			"\"ModeHeatSupported\":\"%s\","
			"\"ModeDrySupported\":\"%s\","
			"\"ModeFanSupported\":\"%s\","
			"\"VaneVSupported\":\"%s\","
			"\"SwingSupported\":\"%s\","
			"\"FanAutoSupported\":\"%s\","
			"\"OutdoorTemperatureSupported\":\"%s\","
			"\"AirDirectionSupported\":\"%s\","
			"\"PurifierSupported\":\"%s\","
			"\"NightModeSupported\":\"%s\","
			"\"EconoCoolSupported\":\"%s\""),
			caps->cap_mode_heat    ? "on" : "off",
			caps->cap_mode_dry     ? "on" : "off",
			caps->cap_mode_fan     ? "on" : "off",
			caps->cap_vane_v       ? "on" : "off",
			caps->cap_vane_swing   ? "on" : "off",
			caps->cap_fan_auto     ? "on" : "off",
			caps->cap_outdoor_temp ? "on" : "off",
			/* AirDirection requires cap_vane_v and an observed i-See sensor.
			 * It works independently of cap_run_state. */
			(!caps->cap_vane_v || !sc->sc_has_isee) ? "not_supported" : "on",
			caps->cap_run_state    ? "on" : "not_supported",
			caps->cap_run_state    ? "on" : "not_supported",
			caps->cap_run_state    ? "on" : "not_supported");

		if (caps->cap_temp_ranges)
		{
			char tmin[12], tmax[12];
			dtostrfd((caps->temp_cool_min - 128) / 2.0f, 1, tmin);
			dtostrfd((caps->temp_cool_max - 128) / 2.0f, 1, tmax);
			ResponseAppend_P(PSTR(",\"SetTemperatureCoolMinMax\":[%s,%s]"), tmin, tmax);
			dtostrfd((caps->temp_heat_min - 128) / 2.0f, 1, tmin);
			dtostrfd((caps->temp_heat_max - 128) / 2.0f, 1, tmax);
			ResponseAppend_P(PSTR(",\"SetTemperatureHeatMinMax\":[%s,%s]"), tmin, tmax);
			dtostrfd((caps->temp_auto_min - 128) / 2.0f, 1, tmin);
			dtostrfd((caps->temp_auto_max - 128) / 2.0f, 1, tmax);
			ResponseAppend_P(PSTR(",\"SetTemperatureAutoMinMax\":[%s,%s]"), tmin, tmax);
		}

		ResponseAppend_P(PSTR(",\"CapabilitiesHex\":\"%s\""),
			ToHex_P(caps->sc_caps_raw, 16, hex_caps, sizeof(hex_caps)));
	}

#if defined(USE_MIEL_HVAC_MODBUS_SLAVE) && defined(ESP32)
	miel_hvac_mb_sensor(sc);
#endif

	ResponseAppend_P(PSTR("}"));

	/* ENERGY sub-object: Power (W) and Total (kWh).
	 * Only published when non-zero values have been observed at least once. */
	if (sc->sc_status.type != 0 && sc->sc_has_energy)
	{
		const struct miel_hvac_data_status *status =
			&sc->sc_status.data.status;
		char buf[33];

		uint16_t combined_power =
			((uint16_t)status->operationpower << 8) |
			 (uint16_t)status->operationpower1;
		dtostrfd((float)combined_power, 0, buf);
		ResponseAppend_P(PSTR(",\"ENERGY\":{\"" D_JSON_POWERUSAGE "\":%s"), buf);

		uint16_t combined_energy =
			((uint16_t)status->operationenergy << 8) |
			 (uint16_t)status->operationenergy1;
		dtostrfd((float)combined_energy / 10.0f, 1, buf);
		ResponseAppend_P(PSTR(",\"" D_JSON_TOTAL "\":%s}"), buf);
	}
}

#ifdef USE_WEBSERVER
/*
 * Current mode as a control-panel keyword ("off" when powered down; i-See
 * mode variants folded onto their base mode).
 */
static const char *
miel_hvac_web_curmode(const struct miel_hvac_data_settings *set)
{
	if (!set->power)
		return ("off");

	switch (set->mode & MIEL_HVAC_SETTINGS_MODE_MASK)
	{
	case MIEL_HVAC_SETTINGS_MODE_HEAT:
	case MIEL_HVAC_SETTINGS_MODE_HEAT_ISEE:
		return ("heat");
	case MIEL_HVAC_SETTINGS_MODE_COOL:
	case MIEL_HVAC_SETTINGS_MODE_COOL_ISEE:
		return ("cool");
	case MIEL_HVAC_SETTINGS_MODE_DRY:
	case MIEL_HVAC_SETTINGS_MODE_DRY_ISEE:
		return ("dry");
	case MIEL_HVAC_SETTINGS_MODE_FAN:
		return ("fan");
	case MIEL_HVAC_SETTINGS_MODE_AUTO:
		return ("auto");
	}
	return ("");
}

/* Apply the locale decimal separator to a plain number string in place. */
static void
miel_hvac_dsep(char *s)
{
	if (D_DECIMAL_SEPARATOR[0] == '.')
		return;
	for (; *s != '\0'; s++)
	{
		if (*s == '.')
			*s = D_DECIMAL_SEPARATOR[0];
	}
}

/*
 * Emit one state row.  js=false renders the initial <tr> into the panel
 * card (miel_hvac_web_panel); js=true emits a JS statement that refreshes
 * that same row's <td> in place, driven from FUNC_WEB_SENSOR so the values
 * update without a page reload.
 */
static void
miel_hvac_web_ro(bool js, const char *id, const char *label, const char *value)
{
	char v[48];
	size_t i;

	/* protocol keywords use '_' between words; show them with spaces */
	for (i = 0; i + 1 < sizeof(v) && value[i] != '\0'; i++)
		v[i] = (value[i] == '_') ? ' ' : value[i];
	v[i] = '\0';

	if (js)
		WSContentSend_P(PSTR("(e=eb('%s'))&&(e.innerHTML='%s');"), id, v);
	else
		WSContentSend_P(PSTR("<tr><th>%s</th><td id='%s'>%s</td></tr>"),
			label, id, v);
}

/*
 * Read-only state table shown inside the control-panel card, below the
 * controls (matching the design proposal).  Called once with js=false to
 * lay it out, then every Settings->web_refresh ms with js=true to refresh
 * the values live.
 */
static void
miel_hvac_web_readout(struct miel_hvac_softc *sc, bool js)
{
	const struct miel_hvac_data_settings *set = &sc->sc_settings.data.settings;
	uint8_t traw = (set->temp05 != 0) ? set->temp05 : set->temp;
	const char *name;
	const char *vh;
	char vhbuf[8];
	char val[48];
	char buf[33];

	if (!js)
		WSContentSend_P(PSTR("<div class='hp-ro-w'><table class='hp-ro'>"));

	if (sc->sc_roomtemp.type != 0)
	{
		const struct miel_hvac_data_roomtemp *rt =
			&sc->sc_roomtemp.data.roomtemp;
		float t = (rt->temp05 != 0)
			? miel_hvac_roomtemp2deg(sc->sc_temp_type, rt->temp05)
			: miel_hvac_roomtemp2deg(sc->sc_temp_type, rt->temp);

		dtostrfd(ConvertTemp(t),
			Settings->flag2.temperature_resolution, buf);
		miel_hvac_dsep(buf);
		snprintf_P(val, sizeof(val), PSTR("%s " D_UNIT_DEGREE "%c"), buf, TempUnit());
		miel_hvac_web_ro(js, "hvro_room", "Room Temp", val);
	}

	dtostrfd(ConvertTemp(miel_hvac_temp2deg(sc->sc_temp_type, traw)),
		Settings->flag2.temperature_resolution, buf);
	miel_hvac_dsep(buf);
	snprintf_P(val, sizeof(val), PSTR("%s " D_UNIT_DEGREE "%c"), buf, TempUnit());
	miel_hvac_web_ro(js, "hvro_set", "Set Temp", val);

	name = set->power
		? miel_hvac_map_byval(set->mode & MIEL_HVAC_SETTINGS_MODE_MASK,
			miel_hvac_mode_map, nitems(miel_hvac_mode_map))
		: "off";
	miel_hvac_web_ro(js, "hvro_mode", "Mode", name != NULL ? name : "-");

	name = miel_hvac_map_byval(set->fan,
		miel_hvac_fan_map, nitems(miel_hvac_fan_map));
	miel_hvac_web_ro(js, "hvro_fan", "Fan", name != NULL ? name : "-");

	name = miel_hvac_map_byval(set->vane,
		miel_hvac_vane_map, nitems(miel_hvac_vane_map));
	if (name == NULL)
		name = "auto";

	bool wv_isee = (set->widevane == 0x80 || set->widevane == 0x28
	             || set->widevane == 0xaa);

	if (wv_isee)
		vh = "isee";
	else
	{
		vh = miel_hvac_map_byval(set->widevane & MIEL_HVAC_SETTINGS_WIDEVANE_MASK,
			miel_hvac_widevane_map, nitems(miel_hvac_widevane_map));
		if (vh == NULL)
		{
			snprintf_P(vhbuf, sizeof(vhbuf), PSTR("0x%02x"), set->widevane);
			vh = vhbuf;
		}
	}
	snprintf_P(val, sizeof(val), PSTR("%s / %s"), name, vh);
	miel_hvac_web_ro(js, "hvro_vane", "Vane V / H", val);

	/* Air Direction — separate i-See function; "off" unless wide vane is
	 * in i-See mode.  Shown only when the unit supports it. */
	if (!sc->sc_caps.sc_caps_valid
	    || (sc->sc_caps.cap_vane_v && sc->sc_has_isee))
	{
		const char *ad = wv_isee
			? miel_hvac_map_byval(set->airdirection,
				miel_hvac_airdirection_map, nitems(miel_hvac_airdirection_map))
			: "off";
		miel_hvac_web_ro(js, "hvro_dir", "Air Direction",
			ad != NULL ? ad : "off");
	}

	if (sc->sc_stage.type != 0)
	{
		name = miel_hvac_map_byval(sc->sc_stage.data.stage.operation,
			miel_hvac_stage_operation_map,
			nitems(miel_hvac_stage_operation_map));
		miel_hvac_web_ro(js, "hvro_oper", "Operation",
			name != NULL ? name : "-");
	}

	if (sc->sc_status.type != 0)
	{
		const struct miel_hvac_data_status *st = &sc->sc_status.data.status;

		snprintf_P(val, sizeof(val), PSTR("%u Hz"), st->compressorfrequency);
		miel_hvac_web_ro(js, "hvro_comp", "Compressor", val);

		if (sc->sc_has_energy)
		{
			uint16_t p = ((uint16_t)st->operationpower << 8) |
				(uint16_t)st->operationpower1;
			dtostrfd((float)p, 0, buf);
			miel_hvac_dsep(buf);
			snprintf_P(val, sizeof(val), PSTR("%s " D_UNIT_WATT), buf);
			miel_hvac_web_ro(js, "hvro_pow", "Power Usage", val);

			uint16_t e = ((uint16_t)st->operationenergy << 8) |
				(uint16_t)st->operationenergy1;
			dtostrfd((float)e / 10.0f, 1, buf);
			miel_hvac_dsep(buf);
			snprintf_P(val, sizeof(val), PSTR("%s " D_UNIT_KILOWATTHOUR), buf);
			miel_hvac_web_ro(js, "hvro_egy", "Energy Total", val);
		}
	}

	if (js)
	{
		/*
		 * Keep the interactive controls in sync with the unit — but not
		 * while a change is still on its way to the unit (the settings we
		 * would sync from are stale then), and never yank a control the
		 * user is currently interacting with.
		 */
		if (!miel_hvac_update_settings_pending(sc)
		    && !miel_hvac_update_runstate_pending(sc))
		{
			const char *fn = miel_hvac_map_byval(set->fan,
				miel_hvac_fan_map, nitems(miel_hvac_fan_map));
			const char *vn = miel_hvac_map_byval(set->vane,
				miel_hvac_vane_map, nitems(miel_hvac_vane_map));
			const char *wn = wv_isee ? NULL
				: miel_hvac_map_byval(set->widevane & MIEL_HVAC_SETTINGS_WIDEVANE_MASK,
					miel_hvac_widevane_map, nitems(miel_hvac_widevane_map));
			const char *ad = wv_isee
				? miel_hvac_map_byval(set->airdirection,
					miel_hvac_airdirection_map, nitems(miel_hvac_airdirection_map))
				: "off";

			WSContentSend_P(PSTR(
				"function S(i,v){var s=eb(i);"
				"if(s&&v&&s!==document.activeElement)s.value=v;}"
				"var b=document.querySelectorAll('#hvacp .hp-seg .hp-b'),i;"
				"for(i=0;i<b.length;i++){"
				"b[i].className=(b[i].dataset.m=='%s')?'hp-b':'hp-b off';}"),
				miel_hvac_web_curmode(set));
			if (fn != NULL)
				WSContentSend_P(PSTR("S('hvf','%s');"), fn);
			if (vn != NULL)
				WSContentSend_P(PSTR("S('hvv','%s');"), vn);
			if (wn != NULL)
				WSContentSend_P(PSTR("S('hvh','%s');"), wn);
			if (ad != NULL)
				WSContentSend_P(PSTR("S('hvd','%s');"), ad);

			/* target temperature: reuse the panel's own hvts() setter */
			char tstr[12];
			dtostrfd(miel_hvac_temp2deg(sc->sc_temp_type, traw), 1, tstr);
			WSContentSend_P(PSTR("if(eb('hvtr')&&eb('hvtr')!==document.activeElement)"
				"hvts(%s);"), tstr);
		}
	}
	else
		WSContentSend_P(PSTR("</table></div>"));
}

/*
 * FUNC_WEB_SENSOR — refresh the panel's read-only rows in place.  Uses the
 * same <img onerror> exec trick as other Tasmota drivers so the values
 * update on every root-page poll without redrawing the interactive panel.
 */
static void
miel_hvac_web_sensor(struct miel_hvac_softc *sc)
{
	if (sc->sc_settings.type == 0)
		return;

	WSContentSend_P(PSTR("</table>"));
	WSContentSend_P(HTTP_MSG_EXEC_JAVASCRIPT);
	WSContentSend_P(PSTR("var e;"));
	miel_hvac_web_readout(sc, true);
	WSContentSend_P(PSTR("\">{t}"));
	WSContentSeparator(3);
}

/*
 * Interactive HVAC control panel on the Tasmota main page.
 *
 * Rendered once per full page load from FUNC_WEB_ADD_MAIN_BUTTON (it is
 * not part of the la() refresh region, so it keeps focus/selection while
 * the sensor rows above update).  Each control optimistically updates its
 * own appearance and calls la('&<key>=<value>'); the browser issues
 * GET /?m=1&<key>=<value> and miel_hvac_web_getarg() (FUNC_WEB_GET_ARG)
 * turns that into the matching HVACSet* console command.
 *
 * Which controls/options render is gated by the 0x7B 0xC9 Base
 * Capabilities response (sc_caps); when capabilities are not yet known
 * everything is shown and the unit rejects anything it cannot do.
 */
#define MIEL_HVAC_WEBARG_MODE   "hvm"
#define MIEL_HVAC_WEBARG_TEMP   "hvt"
#define MIEL_HVAC_WEBARG_FAN    "hvf"
#define MIEL_HVAC_WEBARG_VANEV  "hvv"
#define MIEL_HVAC_WEBARG_VANEH  "hvh"
#define MIEL_HVAC_WEBARG_AIRDIR "hvd"
#define MIEL_HVAC_WEBARG_PURIFY "hvp"
#define MIEL_HVAC_WEBARG_NIGHT  "hvn"
#define MIEL_HVAC_WEBARG_ECONO  "hve"

/*
 * Scoped stylesheet reproducing the design proposal exactly (dark palette,
 * type scale, spacing).  Everything is namespaced under #hvacp so it never
 * touches the rest of the Tasmota page.  The webfonts load when the device
 * has internet; the fallback stack keeps the same metrics otherwise.
 */
static const char miel_hvac_web_style[] PROGMEM =
	"<link rel='stylesheet' href='https://fonts.googleapis.com/css2?"
	"family=Barlow:wght@400;500;600&amp;"
	"family=Barlow+Semi+Condensed:wght@600;700&amp;display=swap'>"
	"<style>"
	"#hvacp{margin-top:10px;text-align:left;color:#e7eaec;font-size:14px;line-height:1.4;"
		"font-family:'Barlow','Segoe UI',system-ui,-apple-system,sans-serif}"
	"#hvacp *{box-sizing:border-box}"
	"#hvacp,#hvacp div,#hvacp input,#hvacp table,#hvacp td,#hvacp th{padding:0;margin:0}"
	"#hvacp .hp-panel{background:#262a2d;border:1px solid #4a5054;border-radius:8px;padding:12px;"
		"box-shadow:0 1px 2px rgba(0,0,0,.4),0 10px 34px rgba(0,0,0,.45)}"
	"#hvacp .hp-field{margin-bottom:12px}"
	"#hvacp .hp-field:last-child{margin-bottom:0}"
	"#hvacp .hp-label{font-family:'Barlow Semi Condensed','Barlow',system-ui,sans-serif;"
		"text-transform:uppercase;letter-spacing:.06em;font-size:11px;font-weight:600;"
		"color:#9aa1a7;margin-bottom:5px}"
	"#hvacp .hp-seg{display:grid;grid-template-columns:repeat(3,1fr);gap:6px;margin-bottom:12px}"
	"#hvacp .hp-b{font-family:'Barlow Semi Condensed','Barlow',system-ui,sans-serif;font-weight:600;"
		"font-size:13px;line-height:1.3;color:#08151d;background:#1fa3ec;border:0;border-radius:6px;"
		"padding:8px 4px;width:100%%;cursor:pointer;transition:filter .12s ease}"
	"#hvacp .hp-b:hover{filter:brightness(1.06)}"
	"#hvacp .hp-b.off{background:#565c61;color:#e7eaec}"
	"#hvacp .hp-b:focus-visible,#hvacp .hp-sel:focus-visible{outline:2px solid #1fa3ec;outline-offset:1px}"
	"#hvacp .hp-seg .hp-b{display:flex;flex-direction:column;align-items:center;gap:2px;line-height:1.15}"
	"#hvacp .hp-ico{font-size:15px;line-height:1}"
	"#hvacp .hp-temp{display:flex;align-items:center;gap:10px}"
	"#hvacp .hp-temp .hp-b{width:40px;height:40px;font-size:20px;flex:none;padding:0}"
	"#hvacp .hp-val{font-family:'Barlow Semi Condensed','Barlow',system-ui,sans-serif;font-weight:700;"
		"font-size:26px;flex:1;text-align:center;font-variant-numeric:tabular-nums}"
	"#hvacp .hp-val small{font-size:14px;font-weight:600;color:#9aa1a7}"
	"#hvacp .hp-sel{width:100%%;font-family:'Barlow','Segoe UI',system-ui,sans-serif;font-size:14px;"
		"color:#e7eaec;background:#1f2325;border:1px solid #4a5054;border-radius:6px;padding:8px 9px;"
		"cursor:pointer}"
	"#hvacp input[type=range]{width:100%%;accent-color:#1fa3ec;margin-top:8px;display:block}"
	"#hvacp .hp-toggles{display:grid;grid-template-columns:repeat(3,1fr);gap:6px}"
	"#hvacp .hp-toggles .hp-b{font-size:12px}"
	"#hvacp .hp-ro-w{padding-top:10px;border-top:1px solid #4a5054}"
	"#hvacp .hp-ro{width:100%%;border-collapse:collapse;font-size:13px}"
	"#hvacp .hp-ro th{text-align:left;font-weight:500;color:#9aa1a7;padding:3px 0}"
	"#hvacp .hp-ro td{text-align:right;font-variant-numeric:tabular-nums;padding:3px 0;white-space:nowrap}"
	"</style>";

static const char miel_hvac_web_script[] PROGMEM =
	"<script>"
	"function hvm(b,v){"
		"var q=b.parentNode.getElementsByTagName('button');"
		"for(var i=0;i<q.length;i++){q[i].className='hp-b off';}"
		"b.className='hp-b';"
		"la('&" MIEL_HVAC_WEBARG_MODE "='+v);"
	"}"
	"function hvo(b,k){"
		"var o=(b.dataset.on=='1')?0:1;"
		"b.dataset.on=o;"
		"b.className=o?'hp-b':'hp-b off';"
		"la('&'+k+'='+(o?'on':'off'));"
	"}"
	"function hvts(t){"
		"t=Math.round(t/hvcs)*hvcs;"
		"if(t<hvcl){t=hvcl;}if(t>hvch){t=hvch;}"
		"hvct=t;"
		"eb('hvtv').innerHTML=t.toFixed(1)+'<small>&deg;C</small>';"
		"eb('hvtr').value=t;"
	"}"
	"function hvtb(d){hvts(hvct+d);la('&" MIEL_HVAC_WEBARG_TEMP "='+hvct.toFixed(1));}"
	"function hvsl(v){hvts(v*1);la('&" MIEL_HVAC_WEBARG_TEMP "='+hvct.toFixed(1));}"
	"</script>";

static void
miel_hvac_web_label(const char *label)
{
	WSContentSend_P(PSTR("<div class='hp-label'>%s</div>"), label);
}

static void
miel_hvac_web_modebtn(const char *v, const char *icon, const char *label,
    const char *curm)
{
	WSContentSend_P(PSTR("<button class='hp-b%s' data-m='%s' onclick=\"hvm(this,'%s')\">"
		"<span class='hp-ico'>%s</span>%s</button>"),
		(strcmp(v, curm) == 0) ? "" : " off", v, v, icon, label);
}

static void
miel_hvac_web_optbtn(const char *key, const char *label, bool on)
{
	WSContentSend_P(PSTR("<button class='hp-b%s' data-on='%d' "
		"onclick=\"hvo(this,'%s')\">%s</button>"),
		on ? "" : " off", on ? 1 : 0, key, label);
}

/*
 * Friendly display text for a protocol keyword.  The <option> value stays
 * the machine name so the emitted HVACSet* command is unchanged.
 */
static const char *
miel_hvac_web_optlabel(const char *name)
{
	static const struct {
		const char *k;
		const char *v;
	} lbl[] = {
		{ "auto",         "Auto"          },
		{ "quiet",        "Quiet"         },
		{ "up",           "Up"            },
		{ "up_middle",    "Up-Middle"     },
		{ "center",       "Center"        },
		{ "down_middle",  "Down-Middle"   },
		{ "down",         "Down"          },
		{ "swing",        "Swing"         },
		{ "left",         "Left"          },
		{ "left_middle",  "Left-Middle"   },
		{ "right",        "Right"         },
		{ "right_middle", "Right-Middle"  },
		{ "left_center",  "Left-Center"   },
		{ "right_center", "Right-Center"  },
		{ "split",        "Split"         },
		{ "even",         "Even"          },
		{ "indirect",     "Indirect"      },
		{ "direct",       "Direct"        },
		{ "off",          "Off"           },
	};
	size_t i;

	for (i = 0; i < nitems(lbl); i++)
	{
		if (strcmp(name, lbl[i].k) == 0)
			return (lbl[i].v);
	}

	return (name);
}

static void
miel_hvac_web_select(const char *label, const char *id, const char *key,
    const struct miel_hvac_map *m, size_t n, uint8_t cur,
    const uint8_t *skip, size_t nskip)
{
	size_t i, j;

	WSContentSend_P(PSTR("<div class='hp-field'>"));
	miel_hvac_web_label(label);
	WSContentSend_P(PSTR("<select class='hp-sel' id='%s' onchange=\"la('&%s='+this.value)\">"),
		id, key);

	for (i = 0; i < n; i++)
	{
		bool skipit = false;
		for (j = 0; j < nskip; j++)
		{
			if (skip[j] == m[i].byte)
			{
				skipit = true;
				break;
			}
		}
		if (skipit)
			continue;

		WSContentSend_P(PSTR("<option value='%s'%s>%s</option>"),
			m[i].name, (m[i].byte == cur) ? " selected" : "",
			miel_hvac_web_optlabel(m[i].name));
	}

	WSContentSend_P(PSTR("</select></div>"));
}

static void
miel_hvac_web_panel(struct miel_hvac_softc *sc)
{
	if (sc->sc_settings.type == 0)
		return;

	const struct miel_hvac_data_settings *set = &sc->sc_settings.data.settings;
	const struct miel_hvac_capabilities *caps = &sc->sc_caps;
	bool cv = caps->sc_caps_valid;
	bool m_heat = !cv || caps->cap_mode_heat;
	bool m_dry  = !cv || caps->cap_mode_dry;
	bool m_fan  = !cv || caps->cap_mode_fan;
	const char *curm = miel_hvac_web_curmode(set);
	uint8_t traw = (set->temp05 != 0) ? set->temp05 : set->temp;
	float curtemp = miel_hvac_temp2deg(sc->sc_temp_type, traw);
	int tlo = 16, thi = 31;
	char tbuf[16];

	if (cv && caps->cap_temp_ranges)
	{
		uint8_t mm = set->mode & MIEL_HVAC_SETTINGS_MODE_MASK;
		if (mm == MIEL_HVAC_SETTINGS_MODE_HEAT)
		{
			tlo = (caps->temp_heat_min - 128) / 2;
			thi = (caps->temp_heat_max - 128) / 2;
		}
		else if (mm == MIEL_HVAC_SETTINGS_MODE_AUTO)
		{
			tlo = (caps->temp_auto_min - 128) / 2;
			thi = (caps->temp_auto_max - 128) / 2;
		}
		else
		{
			tlo = (caps->temp_cool_min - 128) / 2;
			thi = (caps->temp_cool_max - 128) / 2;
		}
	}
	dtostrfd(curtemp, 1, tbuf);

	WSContentSend_P(miel_hvac_web_style);
	WSContentSend_P(PSTR("<div id='hvacp'><div class='hp-panel'>"));

	/* per-page JS state, then the shared helpers */
	WSContentSend_P(PSTR("<script>var hvct=%s,hvcs=%s,hvcl=%d,hvch=%d;</script>"),
		tbuf, sc->sc_temp_type ? "0.5" : "1", tlo, thi);
	WSContentSend_P(miel_hvac_web_script);

	/* Mode */
	miel_hvac_web_label("Mode");
	WSContentSend_P(PSTR("<div class='hp-seg'>"));
	miel_hvac_web_modebtn("auto", "A", "Auto", curm);
	if (m_heat)
		miel_hvac_web_modebtn("heat", "\xe2\x98\x80", "Heat", curm);
	miel_hvac_web_modebtn("cool", "\xe2\x9d\x84", "Cool", curm);
	if (m_dry)
		miel_hvac_web_modebtn("dry", "\xf0\x9f\x92\xa7", "Dry", curm);
	if (m_fan)
		miel_hvac_web_modebtn("fan", "\xe2\x9c\xb1", "Fan", curm);
	miel_hvac_web_modebtn("off", "\xe2\x8f\xbb", "Off", curm);
	WSContentSend_P(PSTR("</div>"));

	/* Target temperature */
	WSContentSend_P(PSTR("<div class='hp-field'>"));
	miel_hvac_web_label("Target temperature");
	WSContentSend_P(PSTR("<div class='hp-temp'>"
		"<button class='hp-b' onclick='hvtb(-hvcs)'>&minus;</button>"
		"<div class='hp-val' id='hvtv'>%s<small>&deg;C</small></div>"
		"<button class='hp-b' onclick='hvtb(hvcs)'>+</button></div>"
		"<input type='range' id='hvtr' min='%d' max='%d' step='%s' value='%s' "
		"onchange='hvsl(this.value)'></div>"),
		tbuf, tlo, thi, sc->sc_temp_type ? "0.5" : "1", tbuf);

	/* Fan speed */
	{
		uint8_t fskip[3];
		size_t nf = 0;
		uint8_t fc = miel_hvac_get_fan_count(sc);

		if (cv && !caps->cap_fan_auto)
			fskip[nf++] = MIEL_HVAC_SETTINGS_FAN_AUTO;
		if (fc != 0 && fc < 5)
			fskip[nf++] = MIEL_HVAC_SETTINGS_FAN_QUIET;
		if (fc != 0 && fc < 4)
			fskip[nf++] = MIEL_HVAC_SETTINGS_FAN_4;

		miel_hvac_web_select("Fan speed", "hvf", MIEL_HVAC_WEBARG_FAN,
			miel_hvac_fan_map, nitems(miel_hvac_fan_map),
			set->fan, fskip, nf);
	}

	/* Vane vertical — only when the unit has a controllable vertical vane */
	if (!cv || caps->cap_vane_v)
	{
		uint8_t vskip[1];
		size_t nv = 0;

		if (cv && !caps->cap_vane_swing)
			vskip[nv++] = MIEL_HVAC_SETTINGS_VANE_SWING;

		miel_hvac_web_select("Vane vertical", "hvv", MIEL_HVAC_WEBARG_VANEV,
			miel_hvac_vane_map, nitems(miel_hvac_vane_map),
			set->vane, vskip, nv);
	}

	/* Vane horizontal / wide vane */
	{
		static const uint8_t hskip[] = { MIEL_HVAC_SETTINGS_WIDEVANE_ISEE };

		miel_hvac_web_select("Vane horizontal", "hvh", MIEL_HVAC_WEBARG_VANEH,
			miel_hvac_widevane_map, nitems(miel_hvac_widevane_map),
			set->widevane & MIEL_HVAC_SETTINGS_WIDEVANE_MASK,
			hskip, nitems(hskip));
	}

	/* Air direction (i-See) — separate function; needs a vertical vane and
	 * an observed i-See sensor.  Direction is only meaningful while the wide
	 * vane is in i-See mode; otherwise the control reads "off". */
	if (!cv || (caps->cap_vane_v && sc->sc_has_isee))
	{
		bool wv_isee = (set->widevane == 0x80 || set->widevane == 0x28
		             || set->widevane == 0xaa);
		uint8_t adcur = wv_isee
			? set->airdirection
			: MIEL_HVAC_SETTINGS_AIRDIRECTION_OFF;

		miel_hvac_web_select("Air direction", "hvd", MIEL_HVAC_WEBARG_AIRDIR,
			miel_hvac_airdirection_map, nitems(miel_hvac_airdirection_map),
			adcur, NULL, 0);
	}

	/* Purifier / Night mode / EconoCool (0x08 Set Run State) */
	if (!cv || caps->cap_run_state)
	{
		const struct miel_hvac_data_options *opt = &sc->sc_options.data.options;
		bool have = (sc->sc_options.type != 0);

		WSContentSend_P(PSTR("<div class='hp-field'>"));
		miel_hvac_web_label("Options");
		WSContentSend_P(PSTR("<div class='hp-toggles'>"));
		miel_hvac_web_optbtn(MIEL_HVAC_WEBARG_PURIFY, "Purifier",
			have && opt->purifier == MIEL_HVAC_OPTIONS_PURIFIER_ON);
		miel_hvac_web_optbtn(MIEL_HVAC_WEBARG_NIGHT, "Night",
			have && opt->nightmode == MIEL_HVAC_OPTIONS_NIGHTMODE_ON);
		miel_hvac_web_optbtn(MIEL_HVAC_WEBARG_ECONO, "EconoCool",
			have && opt->econocool == MIEL_HVAC_OPTIONS_ECONOCOOL_ON);
		WSContentSend_P(PSTR("</div></div>"));
	}

	miel_hvac_web_readout(sc, false);
	WSContentSend_P(PSTR("</div></div>"));	/* close .hp-panel, #hvacp */
}

/*
 * FUNC_WEB_GET_ARG — turn a control-panel GET argument into the matching
 * HVACSet* console command.  Mode uses HVACSetHAMode so a mode button also
 * powers the unit on and "Off" powers it down.
 */
static void
miel_hvac_web_getarg(void)
{
	char tmp[24];
	char cmnd[48];

#define MIEL_HVAC_WEB_GETARG(_k, _c) do {                              \
		WebGetArg(PSTR(_k), tmp, sizeof(tmp));                         \
		if (strlen(tmp))                                               \
		{                                                             \
			snprintf_P(cmnd, sizeof(cmnd), PSTR(_c " %s"), tmp);       \
			ExecuteWebCommand(cmnd);                                   \
		}                                                             \
	} while (0)

	MIEL_HVAC_WEB_GETARG(MIEL_HVAC_WEBARG_MODE,   D_CMND_MIEL_HVAC_SETHAMODE);
	MIEL_HVAC_WEB_GETARG(MIEL_HVAC_WEBARG_TEMP,   D_CMND_MIEL_HVAC_SETTEMP);
	MIEL_HVAC_WEB_GETARG(MIEL_HVAC_WEBARG_FAN,    D_CMND_MIEL_HVAC_SETFANSPEED);
	MIEL_HVAC_WEB_GETARG(MIEL_HVAC_WEBARG_VANEV,  D_CMND_MIEL_HVAC_SETSWINGV);
	MIEL_HVAC_WEB_GETARG(MIEL_HVAC_WEBARG_VANEH,  D_CMND_MIEL_HVAC_SETSWINGH);
	MIEL_HVAC_WEB_GETARG(MIEL_HVAC_WEBARG_AIRDIR, D_CMND_MIEL_HVAC_SETAIRDIRECTION);
	MIEL_HVAC_WEB_GETARG(MIEL_HVAC_WEBARG_PURIFY, D_CMND_MIEL_HVAC_SETPURIFY);
	MIEL_HVAC_WEB_GETARG(MIEL_HVAC_WEBARG_NIGHT,  D_CMND_MIEL_HVAC_SETNIGHTMODE);
	MIEL_HVAC_WEB_GETARG(MIEL_HVAC_WEBARG_ECONO,  D_CMND_MIEL_HVAC_SETECONOCOOL);

#undef MIEL_HVAC_WEB_GETARG
}
#endif  /* USE_WEBSERVER */

/*
 * Connection negotiation: try 2400 baud then 9600 baud, sending a connect
 * packet on each non-baud tick.  Once sc_connected is set we move to the
 * normal request cycle in miel_hvac_tick().
 */

enum miel_hvac_connect_states
{
	MIEL_HVAC_CONNECT_S_2400,
	MIEL_HVAC_CONNECT_S_2400_MSG,
	MIEL_HVAC_CONNECT_S_9600,
	MIEL_HVAC_CONNECT_S_9600_MSG,
	MIEL_HVAC_CONNECT_S_COUNT,
};

static void
miel_hvac_connect(struct miel_hvac_softc *sc)
{
	TasmotaSerial *serial = sc->sc_serial;
	uint32_t baudrate;
	unsigned int state;

	state = (sc->sc_tick++ % MIEL_HVAC_CONNECT_S_COUNT);
	switch (state)
	{
	case MIEL_HVAC_CONNECT_S_2400:
		baudrate = 2400;
		break;
	case MIEL_HVAC_CONNECT_S_9600:
		baudrate = 9600;
		break;
	default:
		miel_hvac_send_connect(sc);
		return;
	}

	serial->begin(baudrate, SERIAL_8E1);
	if (serial->hardwareSerial())
		SetSerial(baudrate, TS_SERIAL_8E1);
}

static void
miel_hvac_tick(struct miel_hvac_softc *sc)
{
	static const uint8_t updates[] = {
		MIEL_HVAC_REQUEST_SETTINGS,
		MIEL_HVAC_REQUEST_STATUS,
		MIEL_HVAC_REQUEST_SETTINGS,
		MIEL_HVAC_REQUEST_ROOMTEMP,
		MIEL_HVAC_REQUEST_SETTINGS,
		MIEL_HVAC_REQUEST_TIMERS,
		MIEL_HVAC_REQUEST_SETTINGS,
		/* MUZ-GA80VA does not respond to STAGE */
		MIEL_HVAC_REQUEST_STAGE,
		/* 0x42: Purifier, NightMode, EconoCool state. Sent with len=1
		 * (short request form). Non-supporting units timeout via p_tmo. */
		MIEL_HVAC_REQUEST_OPTIONS,
	};

	struct miel_hvac_parser *p = &sc->sc_parser;
	unsigned int i;

	if (p->p_state != MIEL_HVAC_P_START)
	{
		if (p->p_tmo)
		{
			AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME ": read timeout"));
			sc->sc_parser.p_state = MIEL_HVAC_P_START;
		}
		else
		{
			p->p_tmo = 1;
			return;
		}
	}

	/* Send 0x5B 0xC9 Base Capabilities request once after connecting.
	 * sc_identified is cleared on each new connection so reconnect re-queries. */
	if (!sc->sc_identified)
	{
		miel_hvac_identify(sc);
		sc->sc_identified = true;
		return;
	}

	if (miel_hvac_update_settings_pending(sc))
	{
		struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;

		miel_hvac_send_update_settings(sc, update);
		miel_hvac_init_update_settings(update);

		/* refresh settings on next tick */
		sc->sc_tick = 0;
		return;
	}

	if (sc->sc_remotetemp_update.seven)
	{
		struct miel_hvac_msg_update_remotetemp *remotetemp =
			&sc->sc_remotetemp_update;

		miel_hvac_send_update_remotetemp(sc, remotetemp);
		memset(remotetemp, 0, sizeof(*remotetemp));
		return;
	}

	if (miel_hvac_update_runstate_pending(sc))
	{
		struct miel_hvac_msg_update_runstate *runstate =
			&sc->sc_runstate_update;
		uint16_t sent_flags = runstate->flags;

		/* Optimistic update: apply values to sc_options before sending
		 * so SENSOR reflects intended state immediately. Confirmed by next 0x42 read. */
		if (sent_flags & htons(MIEL_HVAC_RUNSTATE_F_PURIFIER))
		{
			sc->sc_options.type = MIEL_HVAC_DATA_T_OPTIONS;
			sc->sc_options.data.options.purifier = runstate->purifier;
		}
		if (sent_flags & htons(MIEL_HVAC_RUNSTATE_F_NIGHTMODE))
		{
			sc->sc_options.type = MIEL_HVAC_DATA_T_OPTIONS;
			sc->sc_options.data.options.nightmode = runstate->nightmode;
		}
		if (sent_flags & htons(MIEL_HVAC_RUNSTATE_F_ECONOCOOL))
		{
			sc->sc_options.type = MIEL_HVAC_DATA_T_OPTIONS;
			sc->sc_options.data.options.econocool = runstate->econocool;
		}

		miel_hvac_send_update_runstate(sc, runstate);
		memset(runstate, 0, sizeof(*runstate));

		/* 0x61 set-response has no value echo — immediately re-read the state
		 * packet. airdirection: 0x62 0x02; purifier/nightmode: 0x62 0x42. */
		if (sent_flags & htons(MIEL_HVAC_RUNSTATE_F_AIRDIRECTION))
			miel_hvac_request(sc, MIEL_HVAC_REQUEST_SETTINGS);
		else
			miel_hvac_request_short(sc, MIEL_HVAC_REQUEST_OPTIONS);
		return;
	}

	i = (sc->sc_tick++ % nitems(updates));

	/* 0x42 uses short request form (len=1). Units without cap_run_state
	 * never respond to 0x42, so skip polling to avoid timeouts. */
	if (updates[i] == MIEL_HVAC_REQUEST_OPTIONS)
	{
		if (sc->sc_caps.sc_caps_valid && !sc->sc_caps.cap_run_state)
		{
			/* skip this slot silently — advance tick counter only */
		}
		else
		{
			miel_hvac_request_short(sc, updates[i]);
		}
	}
	else
		miel_hvac_request(sc, updates[i]);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
static const char miel_hvac_cmnd_names[] PROGMEM =
	"|" D_CMND_MIEL_HVAC_SETFANSPEED
	"|" D_CMND_MIEL_HVAC_SETMODE
	"|" D_CMND_MIEL_HVAC_SETHAMODE
	"|" D_CMND_MIEL_HVAC_SETTEMP
	"|" D_CMND_MIEL_HVAC_SETSWINGV
	"|" D_CMND_MIEL_HVAC_SETSWINGH
	"|" D_CMND_MIEL_HVAC_SETAIRDIRECTION
	"|" D_CMND_MIEL_HVAC_SETPROHIBIT
	"|" D_CMND_MIEL_HVAC_SETPURIFY
	"|" D_CMND_MIEL_HVAC_SETNIGHTMODE
	"|" D_CMND_MIEL_HVAC_SETECONOCOOL
	"|" D_CMND_MIEL_HVAC_REMOTETEMP
	"|" D_CMND_MIEL_HVAC_REMOTETEMP_CLEAR_TIME
	"|" D_CMND_MIEL_HVAC_SEND_COMMAND
#if defined(USE_MIEL_HVAC_MODBUS_SLAVE) && defined(ESP32)
	"|" D_CMND_MIEL_HVAC_MODBUS
	"|" D_CMND_MIEL_HVAC_MODBUS_ADDRESS
	"|" D_CMND_MIEL_HVAC_MODBUS_BAUDRATE
	"|" D_CMND_MIEL_HVAC_MODBUS_CONFIG
#endif
#ifdef MIEL_HVAC_DEBUG
	"|HVACRequest"
#endif
	;

static void (*const miel_hvac_cmnds[])(void) PROGMEM = {
	&miel_hvac_cmnd_setfanspeed,
	&miel_hvac_cmnd_setmode,
	&miel_hvac_cmnd_sethamode,
	&miel_hvac_cmnd_settemp,
	&miel_hvac_cmnd_setvane,
	&miel_hvac_cmnd_setwidevane,
	&miel_hvac_cmnd_setairdirection,
	&miel_hvac_cmnd_setprohibit,
	&miel_hvac_cmnd_setpurify,
	&miel_hvac_cmnd_setnightmode,
	&miel_hvac_cmnd_seteconocool,
	&miel_hvac_cmnd_remotetemp,
	&miel_hvac_cmnd_remotetemp_auto_clear_time,
	&miel_hvac_cmnd_send_command,
#if defined(USE_MIEL_HVAC_MODBUS_SLAVE) && defined(ESP32)
	&miel_hvac_cmnd_modbus,
	&miel_hvac_cmnd_modbus_address,
	&miel_hvac_cmnd_modbus_baudrate,
	&miel_hvac_cmnd_modbus_config,
#endif
#ifdef MIEL_HVAC_DEBUG
	&miel_hvac_cmnd_request,
#endif
};

bool Xdrv44(uint32_t function)
{
	if (function == FUNC_PRE_INIT)
	{
		miel_hvac_pre_init();
		return false;
	}
	struct miel_hvac_softc *sc = miel_hvac_sc;
	if (sc == NULL)
		return false;

	switch (function)
	{
	case FUNC_LOOP:
		miel_hvac_loop(sc);
#if defined(USE_MIEL_HVAC_MODBUS_SLAVE) && defined(ESP32)
		if (sc->sc_mb != nullptr)
			miel_hvac_mb_loop(sc);
#endif
		break;
	case FUNC_SET_DEVICE_POWER:
		return miel_hvac_cmnd_setpower(sc);
	case FUNC_EVERY_250_MSECOND:
		if (sc->sc_connected)
			miel_hvac_tick(sc);
		else
			miel_hvac_connect(sc);
		break;
	/* Auto-clear the remote temperature override after timeout. */
	case FUNC_EVERY_50_MSECOND:
		 break;
	case FUNC_EVERY_100_MSECOND:
		 break;
	case FUNC_EVERY_200_MSECOND:
	     break;
	case FUNC_EVERY_SECOND:
		if (sc->sc_remotetemp_active &&
			millis() - sc->sc_remotetemp_last_call_time > sc->sc_remotetemp_auto_clear_time)
		{
			miel_hvac_remotetemp_auto_clear();
		}
		break;
	case FUNC_JSON_APPEND:
		miel_hvac_sensor(sc);
		break;
#ifdef USE_WEBSERVER
	case FUNC_WEB_SENSOR:
		miel_hvac_web_sensor(sc);
		break;
	case FUNC_WEB_ADD_MAIN_BUTTON:
		miel_hvac_web_panel(sc);
		break;
	case FUNC_WEB_GET_ARG:
		miel_hvac_web_getarg();
		break;
#endif
	case FUNC_AFTER_TELEPERIOD:
		if (sc->sc_settings_set)
			miel_hvac_publish_settings(sc);
		break;
	case FUNC_COMMAND:
		return DecodeCommand(miel_hvac_cmnd_names, miel_hvac_cmnds);
	case FUNC_ACTIVE:
		return true;
	}

	return false;
}

#endif // USE_MIEL_HVAC
