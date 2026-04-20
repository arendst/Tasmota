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
 * Mitsubishi Electric HVAC serial interface
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
#define D_CMND_MIEL_HVAC_REMOTETEMP_AUTO_CLEAR_TIME "HVACRemoteTempClearTime"
#define D_CMND_MIEL_HVAC_SEND_COMMAND             "HVACSendCommand"

#include <TasmotaSerial.h>

/* from hvac */
bool temp_type = false;

/*
 * remotetemp_active: true when a remote temperature override is active
 * (set via HVACRemoteTemp, auto-clear timer running). false means the unit
 * uses its internal sensor.
 */
bool remotetemp_active = false;
unsigned long remotetemp_auto_clear_time = 10000;
unsigned long remotetemp_last_call_time = 0;
int remotetemp_half = 0;

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
#define MIEL_HVAC_STAGE_OPERATION_UNKNOWN  0x01
#define MIEL_HVAC_STAGE_OPERATION_DEFROST  0x02
#define MIEL_HVAC_STAGE_OPERATION_UNKNOWN1 0x03
#define MIEL_HVAC_STAGE_OPERATION_PREHEAT  0x04
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
 * Response to request 0x42 (Get HVAC Options).
 * Documented at:
 * https://muart-group.github.io/developer/it-protocol/0x62-get-response/0x42-unknown-hvac-options
 *
 * The unit returns this inside a standard 0x62 data packet.
 * Byte layout (relative to the start of the data payload, i.e. after the
 * 0x62 type byte which occupies miel_hvac_data.type):
 *
 *   byte 1 (data.hvac_options.purifier)  – Air Purifier (Plasma Quad): 0=off 1=on
 *   byte 2 (data.hvac_options.nightmode) – Night Mode (dims LED):       0=off 1=on
 *   byte 3 (data.hvac_options.econocool)– EconoCool (COOL mode only):      0=off 1=on
 */
struct miel_hvac_data_hvac_options
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
#define MIEL_HVAC_DATA_T_HVAC_OPTIONS 0x42

	union
	{
		struct miel_hvac_data_settings     settings;
		struct miel_hvac_data_roomtemp     roomtemp;
		struct miel_hvac_data_timers       timers;
		struct miel_hvac_data_status       status;
		struct miel_hvac_data_stage        stage;
		struct miel_hvac_data_hvac_options hvac_options;
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

/* hvac_options: purifier=byte2, nightmode=byte3, econocool=byte4 */
CTASSERT(offsetof(struct miel_hvac_data, data.hvac_options.purifier)   == 2);
CTASSERT(offsetof(struct miel_hvac_data, data.hvac_options.nightmode)  == 3);
CTASSERT(offsetof(struct miel_hvac_data, data.hvac_options.econocool) == 4);

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
#define MIEL_HVAC_REQUEST_HVAC_OPTIONS 0x42
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
	/* flags is little-endian on the wire; use values directly (no htons()). */
	uint16_t flags;
#define MIEL_HVAC_RUNSTATE_F_AIRDIRECTION 0x20
#define MIEL_HVAC_RUNSTATE_F_PURIFIER     0x04
#define MIEL_HVAC_RUNSTATE_F_NIGHTMODE    0x08
	uint8_t  _pad1[3];        /* bytes 3-5, zeroed */
	uint8_t  airdirection;    /* byte 6 — i-See airflow control mode */
	uint8_t  _pad2[5];        /* bytes 7-11, zeroed */
	uint8_t  purifier;        /* byte 12 */
	uint8_t  nightmode;       /* byte 13 */
#define MIEL_HVAC_RUNSTATE_F_ECONOCOOL    0x10
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
	{MIEL_HVAC_STAGE_OPERATION_NORMAL,  "normal"},
	{MIEL_HVAC_STAGE_OPERATION_UNKNOWN, "unknown"},
	{MIEL_HVAC_STAGE_OPERATION_DEFROST, "defrost"},
	{MIEL_HVAC_STAGE_OPERATION_STANDBY, "standby"},
	{MIEL_HVAC_STAGE_OPERATION_PREHEAT, "preheat"},
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

struct miel_hvac_softc
{
	TasmotaSerial *sc_serial;
	struct miel_hvac_parser sc_parser;

	unsigned int sc_device;
	unsigned int sc_tick;
	bool sc_settings_set;
	bool sc_connected;
	bool sc_identified;            /* true once 0x5B 0xC9 has been sent */
	bool sc_has_isee;              /* true once i-See widevane state observed */
	bool sc_has_energy;            /* true once non-zero Power or Energy seen */

	struct miel_hvac_data sc_settings;
	struct miel_hvac_data sc_roomtemp;
	struct miel_hvac_data sc_timers;
	struct miel_hvac_data sc_status;
	struct miel_hvac_data sc_stage;
	struct miel_hvac_data sc_hvac_options; /* 0x42 HVAC Options */

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
	char hex_d[(len + 1) * 2];
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
 * Some command types (e.g. 0x42 HVAC Options) do not use the standard
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
miel_hvac_deg2temp(float deg)
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
miel_hvac_temp2deg(uint8_t temp)
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
miel_hvac_roomtemp2deg(uint8_t roomtemp)
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

	remotetemp_half = 0;
	remotetemp_active = false;

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

static void
miel_hvac_cmnd_setmode(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;
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

	/* heat_isee/dry_isee/cool_isee share the same capability bit as
	 * heat/dry/cool — mask out the ISEE bit before checking. */
	if (sc->sc_caps.sc_caps_valid)
	{
		uint8_t mode = e->byte & MIEL_HVAC_SETTINGS_MODE_MASK;
		if (mode == MIEL_HVAC_SETTINGS_MODE_HEAT && !sc->sc_caps.cap_mode_heat)
		{
			miel_hvac_respond_not_supported();
			return;
		}
		if (mode == MIEL_HVAC_SETTINGS_MODE_DRY && !sc->sc_caps.cap_mode_dry)
		{
			miel_hvac_respond_not_supported();
			return;
		}
		if (mode == MIEL_HVAC_SETTINGS_MODE_FAN && !sc->sc_caps.cap_mode_fan)
		{
			miel_hvac_respond_not_supported();
			return;
		}
	}

	update->flags |= htons(MIEL_HVAC_SETTINGS_F_MODE);
	update->mode = e->byte;

	ResponseCmndChar_P(e->name);
}

static void
miel_hvac_cmnd_sethamode(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;
	const struct miel_hvac_map *e;

	if (XdrvMailbox.data_len == 0)
		return;

	if (strcasecmp(XdrvMailbox.data, "off") == 0)
	{
		update->flags |= htons(MIEL_HVAC_SETTINGS_F_POWER);
		update->power = MIEL_HVAC_SETTINGS_POWER_OFF;
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

	if (sc->sc_caps.sc_caps_valid)
	{
		uint8_t mode = e->byte & MIEL_HVAC_SETTINGS_MODE_MASK;
		if (mode == MIEL_HVAC_SETTINGS_MODE_HEAT && !sc->sc_caps.cap_mode_heat)
		{
			miel_hvac_respond_not_supported();
			return;
		}
		if (mode == MIEL_HVAC_SETTINGS_MODE_DRY && !sc->sc_caps.cap_mode_dry)
		{
			miel_hvac_respond_not_supported();
			return;
		}
		if (mode == MIEL_HVAC_SETTINGS_MODE_FAN && !sc->sc_caps.cap_mode_fan)
		{
			miel_hvac_respond_not_supported();
			return;
		}
	}

	update->flags |= htons(MIEL_HVAC_SETTINGS_F_POWER)
	              |  htons(MIEL_HVAC_SETTINGS_F_MODE);
	update->power = MIEL_HVAC_SETTINGS_POWER_ON;
	update->mode  = e->byte;

	ResponseCmndChar_P(e->name);
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
	if (!temp_type)
	{
		update->temp   = miel_hvac_deg2temp(degc);
		update->temp05 = 0;
	}
	else
	{
		update->temp   = 0;
		update->temp05 = miel_hvac_deg2temp(degc);
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

	/* AirDirection control requires 0x08 Set Run State;
	 * state is still readable from 0x62 when control is not supported. */
	if (sc->sc_caps.sc_caps_valid && !sc->sc_caps.cap_run_state)
	{
		miel_hvac_respond_control_not_supported();
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
		rs->eight          = 0x08;
		rs->flags         |= MIEL_HVAC_RUNSTATE_F_AIRDIRECTION;
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
	update->flags    |= MIEL_HVAC_RUNSTATE_F_PURIFIER;
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
	update->flags     |= MIEL_HVAC_RUNSTATE_F_NIGHTMODE;
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
	update->flags      |= MIEL_HVAC_RUNSTATE_F_ECONOCOOL;
	update->econocool  = e->byte;

	ResponseCmndChar_P(e->name);
}

static void
miel_hvac_cmnd_remotetemp_auto_clear_time(void)
{
	if (XdrvMailbox.data_len == 0)
		return;

	unsigned long clear_time = strtoul(XdrvMailbox.data, nullptr, 10);
	if (clear_time < 1000 || clear_time > 600000)
	{
		miel_hvac_respond_unsupported();
		return;
	}
	remotetemp_auto_clear_time = clear_time;

	ResponseCmndNumber(remotetemp_auto_clear_time);
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
		control           = MIEL_HVAC_REMOTETEMP_CLR;
		remotetemp_half   = 0;
		remotetemp_active = false;

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

		remotetemp_half          = temp_half;
		control                  = MIEL_HVAC_REMOTETEMP_SET;
		remotetemp_active        = true;
		remotetemp_last_call_time = millis();

		ResponseCmndFloat(remotetemp_half / 2.0, 1);
	}

	memset(update, 0, sizeof(*update));
	update->seven    = 0x7;
	update->control  = control;
	update->temp_old = miel_hvac_remotetemp2old(remotetemp_half);
	update->temp     = miel_hvac_remotetemp2new(remotetemp_half);
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
	sc->sc_connected = 1;
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
		dtostrfd(ConvertTemp(miel_hvac_temp2deg(set->temp)),
			Settings->flag2.temperature_resolution, temp);
	else
	{
		temp_type = true;
		dtostrfd(ConvertTemp(miel_hvac_temp2deg(set->temp05)),
			Settings->flag2.temperature_resolution, temp);
	}
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

	/* Purifier, NightMode, EconoCool — state from 0x62 0x42 HVAC Options. */
	if ((!sc->sc_caps.sc_caps_valid || sc->sc_caps.cap_run_state)
		&& sc->sc_hvac_options.type != 0)
	{
		const struct miel_hvac_data_hvac_options *opt =
			&sc->sc_hvac_options.data.hvac_options;

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
		sc->sc_settings_set = 0;
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
	sc->sc_settings_set = 1;
	sc->sc_settings = *d;

	if (publish)
		miel_hvac_publish_settings(sc);
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
		miel_hvac_input_settings(sc, d);
		break;
	case MIEL_HVAC_DATA_T_ROOMTEMP:
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
	case MIEL_HVAC_DATA_T_HVAC_OPTIONS:
		miel_hvac_input_sensor(sc, &sc->sc_hvac_options, d);
		break;
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
			unsigned int t = miel_hvac_roomtemp2deg(rt->temp);
			dtostrfd(ConvertTemp(t),
				Settings->flag2.temperature_resolution, room_temp);
		}
		else
		{
			temp_type = true;
			float t = miel_hvac_roomtemp2deg(rt->temp05);
			dtostrfd(ConvertTemp(t),
				Settings->flag2.temperature_resolution, room_temp);
		}
		ResponseAppend_P(PSTR(",\"RoomTemperature\":%s"), room_temp);

		dtostrfd(remotetemp_half / 2.0, 1, remote_temp);
		ResponseAppend_P(PSTR(",\"RemoteTemperature\":%s"), remote_temp);

		/* "on" = remote temperature override is active */
		ResponseAppend_P(PSTR(",\"RemoteTemperatureSensorState\":\"%s\""),
			remotetemp_active ? "on" : "off");

		char remotetempautocleartime[33];
		ultoa(remotetemp_auto_clear_time, remotetempautocleartime, 10);
		ResponseAppend_P(PSTR(",\"RemoteTemperatureSensorAutoClearTime\":\"%s\""),
			remotetempautocleartime);

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

	/* HVAC Options — Purifier, NightMode, EconoCool. */
	if ((!sc->sc_caps.sc_caps_valid || sc->sc_caps.cap_run_state)
		&& sc->sc_hvac_options.type != 0)
	{
		const struct miel_hvac_data_hvac_options *opt =
			&sc->sc_hvac_options.data.hvac_options;
		char hex[(sizeof(sc->sc_hvac_options) + 1) * 2];

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

		ResponseAppend_P(PSTR(",\"OptionsHex\":\"%s\""),
			ToHex_P((uint8_t *)&sc->sc_hvac_options,
				sizeof(sc->sc_hvac_options), hex, sizeof(hex)));
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
			/* AirDirection requires a vertical vane, an observed i-See
			 * sensor (widevane 0x80/0x28/0xaa seen at least once), and
			 * 0x08 Set Run State for control. */
			(!caps->cap_vane_v || !sc->sc_has_isee) ? "not_supported"
			  : (caps->cap_run_state ? "on" : "control_not_supported"),
			caps->cap_run_state    ? "on" : "not_supported",
			caps->cap_run_state    ? "on" : "not_supported",
			caps->cap_run_state    ? "on" : "not_supported");

		if (caps->cap_temp_ranges)
		{
			ResponseAppend_P(PSTR(",\"SetTemperatureCoolMinMax\":[%.1f,%.1f]"
				",\"SetTemperatureHeatMinMax\":[%.1f,%.1f]"
				",\"SetTemperatureAutoMinMax\":[%.1f,%.1f]"),
				(caps->temp_cool_min - 128) / 2.0f,
				(caps->temp_cool_max - 128) / 2.0f,
				(caps->temp_heat_min - 128) / 2.0f,
				(caps->temp_heat_max - 128) / 2.0f,
				(caps->temp_auto_min - 128) / 2.0f,
				(caps->temp_auto_max - 128) / 2.0f);
		}

		ResponseAppend_P(PSTR(",\"CapabilitiesHex\":\"%s\""),
			ToHex_P(caps->sc_caps_raw, 16, hex_caps, sizeof(hex_caps)));
	}

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
 * Web UI sensor display — shows instantaneous Power (W) and cumulative
 * Total energy (kWh) rows on the Tasmota main page, matching how other
 * energy drivers render their values.
 */
static void
miel_hvac_web_sensor(struct miel_hvac_softc *sc)
{
	if (sc->sc_status.type == 0 || !sc->sc_has_energy)
		return;

	const struct miel_hvac_data_status *status =
		&sc->sc_status.data.status;
	char buf[33];

	uint16_t combined_power =
		((uint16_t)status->operationpower << 8) |
		 (uint16_t)status->operationpower1;
	dtostrfd((float)combined_power, 0, buf);
	WSContentSend_PD(PSTR("{s}" D_POWERUSAGE "{m}%s " D_UNIT_WATT "{e}"), buf);

	uint16_t combined_energy =
		((uint16_t)status->operationenergy << 8) |
		 (uint16_t)status->operationenergy1;
	dtostrfd((float)combined_energy / 10.0f, 1, buf);
	WSContentSend_PD(PSTR("{s}" D_ENERGY_TOTAL "{m}%s " D_UNIT_KILOWATTHOUR "{e}"), buf);
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
		MIEL_HVAC_REQUEST_HVAC_OPTIONS,
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

		/* Optimistic update: apply values to sc_hvac_options before sending
		 * so SENSOR reflects intended state immediately. Confirmed by next 0x42 read. */
		if (sent_flags & MIEL_HVAC_RUNSTATE_F_PURIFIER)
		{
			sc->sc_hvac_options.type = MIEL_HVAC_DATA_T_HVAC_OPTIONS;
			sc->sc_hvac_options.data.hvac_options.purifier = runstate->purifier;
		}
		if (sent_flags & MIEL_HVAC_RUNSTATE_F_NIGHTMODE)
		{
			sc->sc_hvac_options.type = MIEL_HVAC_DATA_T_HVAC_OPTIONS;
			sc->sc_hvac_options.data.hvac_options.nightmode = runstate->nightmode;
		}
		if (sent_flags & MIEL_HVAC_RUNSTATE_F_ECONOCOOL)
		{
			sc->sc_hvac_options.type = MIEL_HVAC_DATA_T_HVAC_OPTIONS;
			sc->sc_hvac_options.data.hvac_options.econocool = runstate->econocool;
		}

		miel_hvac_send_update_runstate(sc, runstate);
		memset(runstate, 0, sizeof(*runstate));

		/* 0x61 set-response has no value echo — immediately re-read the state
		 * packet. airdirection: 0x62 0x02; purifier/nightmode: 0x62 0x42. */
		if (sent_flags & MIEL_HVAC_RUNSTATE_F_AIRDIRECTION)
			miel_hvac_request(sc, MIEL_HVAC_REQUEST_SETTINGS);
		else
			miel_hvac_request_short(sc, MIEL_HVAC_REQUEST_HVAC_OPTIONS);
		return;
	}

	i = (sc->sc_tick++ % nitems(updates));

	/* 0x42 uses short request form (len=1). Units without cap_run_state
	 * never respond to 0x42, so skip polling to avoid timeouts. */
	if (updates[i] == MIEL_HVAC_REQUEST_HVAC_OPTIONS)
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
	"|" D_CMND_MIEL_HVAC_REMOTETEMP_AUTO_CLEAR_TIME
	"|" D_CMND_MIEL_HVAC_SEND_COMMAND
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
	case FUNC_EVERY_100_MSECOND:
	case FUNC_EVERY_200_MSECOND:
	case FUNC_EVERY_SECOND:
		if (remotetemp_active &&
			millis() - remotetemp_last_call_time > remotetemp_auto_clear_time)
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
