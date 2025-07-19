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

#ifndef CTASSERT
#define CTASSERT(x)		extern char  _ctassert[(x) ? 1 : -1 ]	\
				    __attribute__((__unused__))
#endif

#define MIEL_HVAC_LOGNAME "MiElHVAC"

#define D_CMND_MIEL_HVAC_SETFANSPEED "HVACSetFanSpeed"
#define D_CMND_MIEL_HVAC_SETMODE "HVACSetMode"
#define D_CMND_MIEL_HVAC_SETHAMODE "HVACSetHAMode"
#define D_CMND_MIEL_HVAC_SETTEMP "HVACSetTemp"
#define D_CMND_MIEL_HVAC_SETSWINGV "HVACSetSwingV"
#define D_CMND_MIEL_HVAC_SETSWINGH "HVACSetSwingH"
#define D_CMND_MIEL_HVAC_SETAIRDIRECTION "HVACSetAirDirection"
#define D_CMND_MIEL_HVAC_SETPROHIBIT "HVACSetProhibit"
#define D_CMND_MIEL_HVAC_REMOTETEMP "HVACRemoteTemp"
#define D_CMND_MIEL_HVAC_REMOTETEMP_AUTO_CLEAR_TIME "HVACRemoteTempClearTime"

#include <TasmotaSerial.h>

/* from hvac */
bool temp_type = false;
bool remotetemp_clear = true;
unsigned long remotetemp_auto_clear_time = 10000;
unsigned long remotetemp_last_call_time = 0;

struct miel_hvac_header
{
	uint8_t start;
#define MIEL_HVAC_H_START 0xfc
	uint8_t type;
#define MIEL_HVAC_H_TYPE_UPDATED 0x61
#define MIEL_HVAC_H_TYPE_DATA 0x62
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
#define MIEL_HVAC_SETTINGS_POWER_ON 0x01
	uint8_t mode;
#define MIEL_HVAC_SETTINGS_MODE_MASK 0x7f
#define MIEL_HVAC_SETTINGS_MODE_HEAT 0x01
#define MIEL_HVAC_SETTINGS_MODE_DRY 0x02
#define MIEL_HVAC_SETTINGS_MODE_COOL 0x03
#define MIEL_HVAC_SETTINGS_MODE_FAN 0x07
#define MIEL_HVAC_SETTINGS_MODE_AUTO 0x08
#define MIEL_HVAC_SETTINGS_MODE_HEAT_ISEE 0x09
#define MIEL_HVAC_SETTINGS_MODE_DRY_ISEE 0x0a
#define MIEL_HVAC_SETTINGS_MODE_COOL_ISEE 0x0b
	uint8_t temp;
#ifndef MIEL_HVAC_SETTINGS_TEMP_MIN
#define MIEL_HVAC_SETTINGS_TEMP_MIN 10
#endif
#ifndef MIEL_HVAC_SETTINGS_TEMP_MAX
#define MIEL_HVAC_SETTINGS_TEMP_MAX 31
#endif
	uint8_t fan;
#define MIEL_HVAC_SETTINGS_FAN_AUTO 0x00
#define MIEL_HVAC_SETTINGS_FAN_QUIET 0x01
#define MIEL_HVAC_SETTINGS_FAN_1 0x02
#define MIEL_HVAC_SETTINGS_FAN_2 0x03
#define MIEL_HVAC_SETTINGS_FAN_3 0x05
#define MIEL_HVAC_SETTINGS_FAN_4 0x06
	uint8_t vane;
#define MIEL_HVAC_SETTINGS_VANE_AUTO 0x00
#define MIEL_HVAC_SETTINGS_VANE_1 0x01
#define MIEL_HVAC_SETTINGS_VANE_2 0x02
#define MIEL_HVAC_SETTINGS_VANE_3 0x03
#define MIEL_HVAC_SETTINGS_VANE_4 0x04
#define MIEL_HVAC_SETTINGS_VANE_5 0x05
#define MIEL_HVAC_SETTINGS_VANE_SWING 0x07
	uint8_t prohibit;
#define MIEL_HVAC_SETTINGS_PROHIBIT_OFF 0x00
#define MIEL_HVAC_SETTINGS_PROHIBIT_POWER 0x01
#define MIEL_HVAC_SETTINGS_PROHIBIT_MODE 0x02
#define MIEL_HVAC_SETTINGS_PROHIBIT_MODE_POWER 0x03
#define MIEL_HVAC_SETTINGS_PROHIBIT_TEMP 0x04
#define MIEL_HVAC_SETTINGS_PROHIBIT_TEMP_POWER 0x05
#define MIEL_HVAC_SETTINGS_PROHIBIT_TEMP_MODE 0x06
#define MIEL_HVAC_SETTINGS_PROHIBIT_ALL 0x07
	uint8_t _pad2[1];
	uint8_t widevane;
#define MIEL_HVAC_SETTINGS_WIDEVANE_MASK 0x0f
#define MIEL_HVAC_SETTINGS_WIDEVANE_AUTO 0x00
#define MIEL_HVAC_SETTINGS_WIDEVANE_LL 0x01
#define MIEL_HVAC_SETTINGS_WIDEVANE_L 0x02
#define MIEL_HVAC_SETTINGS_WIDEVANE_C 0x03
#define MIEL_HVAC_SETTINGS_WIDEVANE_R 0x04
#define MIEL_HVAC_SETTINGS_WIDEVANE_RR 0x05
#define MIEL_HVAC_SETTINGS_WIDEVANE_LR 0x08
#define MIEL_HVAC_SETTINGS_WIDEVANE_SWING 0x0c
#define MIEL_HVAC_SETTINGS_WIDEVANE_ADJ 0x80
	uint8_t temp05;
	uint8_t _pad3[2];
	uint8_t airdirection;
#define MIEL_HVAC_SETTINGS_AIRDIRECTION_EVEN 0x00
#define MIEL_HVAC_SETTINGS_AIRDIRECTION_INDIRECT 0x01
#define MIEL_HVAC_SETTINGS_AIRDIRECTION_DIRECT 0x02
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
	uint8_t operationtime;
	uint8_t operationtime1;
	uint8_t operationtime2;
};

struct miel_hvac_data_timers
{
	uint8_t _pad1[2];
	uint8_t mode;
#define MIEL_HVAC_TIMER_MODE_NONE 0x00
#define MIEL_HVAC_TIMER_MODE_OFF 0x01
#define MIEL_HVAC_TIMER_MODE_ON 0x02
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
#define MIEL_HVAC_STATUS_COMPRESSOR_ON 0x01
	uint8_t operationpower;
	uint8_t operationpower1;
	uint8_t operationenergy;
	uint8_t operationenergy1;
};

struct miel_hvac_data_stage
{
	uint8_t _pad1[2];
	uint8_t operation;
#define MIEL_HVAC_STAGE_OPERATION_NORMAL 0x00
#define MIEL_HVAC_STAGE_OPERATION_UNKNOWN 0x01
#define MIEL_HVAC_STAGE_OPERATION_DEFROST 0x02
#define MIEL_HVAC_STAGE_OPERATION_UNKNOWN1 0x03
#define MIEL_HVAC_STAGE_OPERATION_PREHEAT 0x04
#define MIEL_HVAC_STAGE_OPERATION_STANDBY 0x08
	uint8_t fan;
#define MIEL_HVAC_STAGE_FAN_OFF 0x00
#define MIEL_HVAC_STAGE_FAN_1 0x01
#define MIEL_HVAC_STAGE_FAN_2 0x02
#define MIEL_HVAC_STAGE_FAN_3 0x03
#define MIEL_HVAC_STAGE_FAN_4 0x04
#define MIEL_HVAC_STAGE_FAN_5 0x05
#define MIEL_HVAC_STAGE_FAN_QUIT 0x06
	uint8_t mode;
#define MIEL_HVAC_STAGE_MODE_MANUAL 0x00
#define MIEL_HVAC_STAGE_MODE_AUTO_FAN 0x01
#define MIEL_HVAC_STAGE_MODE_AUTO_HEAT 0x02
#define MIEL_HVAC_STAGE_MODE_AUTO_COOL 0x03
#define MIEL_HVAC_STAGE_MODE_AUTO_LEADER 0x04
};

struct miel_hvac_data
{
	uint8_t type;
#define MIEL_HVAC_DATA_T_SETTINGS 0x02
#define MIEL_HVAC_DATA_T_ROOMTEMP 0x03
#define MIEL_HVAC_DATA_T_TIMERS 0x05
#define MIEL_HVAC_DATA_T_STATUS 0x06
#define MIEL_HVAC_DATA_T_STAGE 0x09

	union
	{
		struct miel_hvac_data_settings settings;
		struct miel_hvac_data_roomtemp roomtemp;
		struct miel_hvac_data_timers timers;
		struct miel_hvac_data_status status;
		struct miel_hvac_data_stage stage;
		uint8_t bytes[15];
	} data;
};

CTASSERT(sizeof(struct miel_hvac_data) == 16);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.power) == 3);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.mode) == 4);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.temp) == 5);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.fan) == 6);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.vane) == 7);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.prohibit) == 8);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.widevane) == 10);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.temp05) == 11);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.airdirection) == 14);

CTASSERT(offsetof(struct miel_hvac_data, data.roomtemp.temp) == 3);
CTASSERT(offsetof(struct miel_hvac_data, data.roomtemp.outdoortemp) == 5);
CTASSERT(offsetof(struct miel_hvac_data, data.roomtemp.temp05) == 6);
CTASSERT(offsetof(struct miel_hvac_data, data.roomtemp.settemp) == 7);
CTASSERT(offsetof(struct miel_hvac_data, data.roomtemp.operationtime) == 11);
CTASSERT(offsetof(struct miel_hvac_data, data.roomtemp.operationtime1) == 12);
CTASSERT(offsetof(struct miel_hvac_data, data.roomtemp.operationtime2) == 13);

CTASSERT(offsetof(struct miel_hvac_data, data.timers.mode) == 3);
CTASSERT(offsetof(struct miel_hvac_data, data.timers.onminutes) == 4);
CTASSERT(offsetof(struct miel_hvac_data, data.timers.offminutes) == 5);
CTASSERT(offsetof(struct miel_hvac_data, data.timers.onminutesremaining) == 6);
CTASSERT(offsetof(struct miel_hvac_data, data.timers.offminutesremaining) == 7);

CTASSERT(offsetof(struct miel_hvac_data, data.stage.operation) == 3);
CTASSERT(offsetof(struct miel_hvac_data, data.stage.fan) == 4);
CTASSERT(offsetof(struct miel_hvac_data, data.stage.mode) == 5);

CTASSERT(offsetof(struct miel_hvac_data, data.status.compressorfrequency) == 3);
CTASSERT(offsetof(struct miel_hvac_data, data.status.compressor) == 4);
CTASSERT(offsetof(struct miel_hvac_data, data.status.operationpower) == 5);
CTASSERT(offsetof(struct miel_hvac_data, data.status.operationpower1) == 6);
CTASSERT(offsetof(struct miel_hvac_data, data.status.operationenergy) == 7);
CTASSERT(offsetof(struct miel_hvac_data, data.status.operationenergy1) == 8);

/* to hvac */

#define MIEL_HVAC_H_TYPE_CONNECT 0x5a
static const uint8_t miel_hvac_msg_connect[] = {0xca, 0x01};

#define MIEL_HVAC_H_TYPE_REQUEST 0x42

struct miel_hvac_msg_request
{
	uint8_t type;
#define MIEL_HVAC_REQUEST_SETTINGS 0x02
#define MIEL_HVAC_REQUEST_ROOMTEMP 0x03
#define MIEL_HVAC_REQUEST_TIMERS 0x05
#define MIEL_HVAC_REQUEST_STATUS 0x06
#define MIEL_HVAC_REQUEST_STAGE 0x09
	uint8_t zero[15];
};

#define MIEL_HVAC_H_TYPE_UPDATE 0x41

struct miel_hvac_msg_update_settings
{
	uint8_t one;
	uint16_t flags;
#define MIEL_HVAC_SETTINGS_F_WIDEVANE (1 << 0)
#define MIEL_HVAC_SETTINGS_F_POWER (1 << 8)
#define MIEL_HVAC_SETTINGS_F_MODE (1 << 9)
#define MIEL_HVAC_SETTINGS_F_TEMP (1 << 10)
#define MIEL_HVAC_SETTINGS_F_FAN (1 << 11)
#define MIEL_HVAC_SETTINGS_F_VANE (1 << 12)
#define MIEL_HVAC_SETTINGS_F_PROHIBIT (1 << 13)
#define MIEL_HVAC_SETTINGS_F_AIRDIRECTION (1 << 14)
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
CTASSERT(offsetof(struct miel_hvac_msg_update_settings, flags) == MIEL_HVAC_OFFS(6));
CTASSERT(offsetof(struct miel_hvac_msg_update_settings, power) == MIEL_HVAC_OFFS(8));
CTASSERT(offsetof(struct miel_hvac_msg_update_settings, mode) == MIEL_HVAC_OFFS(9));
CTASSERT(offsetof(struct miel_hvac_msg_update_settings, temp) == MIEL_HVAC_OFFS(10));
CTASSERT(offsetof(struct miel_hvac_msg_update_settings, fan) == MIEL_HVAC_OFFS(11));
CTASSERT(offsetof(struct miel_hvac_msg_update_settings, vane) == MIEL_HVAC_OFFS(12));
CTASSERT(offsetof(struct miel_hvac_msg_update_settings, prohibit) == MIEL_HVAC_OFFS(13));
CTASSERT(offsetof(struct miel_hvac_msg_update_settings, widevane) == MIEL_HVAC_OFFS(18));
CTASSERT(offsetof(struct miel_hvac_msg_update_settings, temp05) == MIEL_HVAC_OFFS(19));
CTASSERT(offsetof(struct miel_hvac_msg_update_settings, airdirection) == MIEL_HVAC_OFFS(20));

static inline uint8_t
miel_hvac_deg2temp(float deg)
{
	if (!temp_type)
	{
		return (31 - deg);
	}
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
	{
		return (31 - temp);
	}
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
	{
		return ((unsigned int)roomtemp + 10);
	}
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

struct miel_hvac_msg_update_remotetemp
{
	uint8_t seven;
	uint8_t control;
#define MIEL_HVAC_REMOTETEMP_CLR 0x00
#define MIEL_HVAC_REMOTETEMP_SET 0x01
	/* setting for older units expressed as .5C units starting at 8C */
	uint8_t temp_old;
#define MIEL_HVAC_REMOTETEMP_OLD_MIN 8
#define MIEL_HVAC_REMOTETEMP_OLD_MAX 38
#define MIEL_HVAC_REMOTETEMP_OLD_FACTOR 2
	/* setting for newer units expressed as .5C units starting at -63C */
	uint8_t temp;
#define MIEL_HVAC_REMOTETEMP_MIN -63
#define MIEL_HVAC_REMOTETEMP_MAX 63
#define MIEL_HVAC_REMOTETEMP_OFFSET 64
#define MIEL_HVAC_REMOTETEMP_FACTOR 2
	uint8_t _pad2[12];
};

CTASSERT(sizeof(struct miel_hvac_msg_update_remotetemp) == 16);

static inline uint8_t
miel_hvac_cksum_fini(uint8_t sum)
{
	return (0xfc - sum);
}

struct miel_hvac_map
{
	uint8_t byte;
	const char *name;
};

static const struct miel_hvac_map miel_hvac_power_map[] = {
	{MIEL_HVAC_SETTINGS_POWER_OFF, "off"},
	{MIEL_HVAC_SETTINGS_POWER_ON, "on"},
};

static const struct miel_hvac_map miel_hvac_mode_map[] = {
	{MIEL_HVAC_SETTINGS_MODE_HEAT, "heat"},
	{MIEL_HVAC_SETTINGS_MODE_DRY, "dry"},
	{MIEL_HVAC_SETTINGS_MODE_COOL, "cool"},
	{MIEL_HVAC_SETTINGS_MODE_FAN, "fan"},
	{MIEL_HVAC_SETTINGS_MODE_AUTO, "auto"},
	{MIEL_HVAC_SETTINGS_MODE_HEAT_ISEE, "heat_isee"},
	{MIEL_HVAC_SETTINGS_MODE_DRY_ISEE, "dry_isee"},
	{MIEL_HVAC_SETTINGS_MODE_COOL_ISEE, "cool_isee"},
};

static const struct miel_hvac_map miel_hvac_fan_map[] = {
	{MIEL_HVAC_SETTINGS_FAN_AUTO, "auto"},
	{MIEL_HVAC_SETTINGS_FAN_QUIET, "quiet"},
	{MIEL_HVAC_SETTINGS_FAN_1, "1"},
	{MIEL_HVAC_SETTINGS_FAN_2, "2"},
	{MIEL_HVAC_SETTINGS_FAN_3, "3"},
	{MIEL_HVAC_SETTINGS_FAN_4, "4"},
};

static const struct miel_hvac_map miel_hvac_vane_map[] = {
	{MIEL_HVAC_SETTINGS_VANE_AUTO, "auto"},
	{MIEL_HVAC_SETTINGS_VANE_1, "up"},
	{MIEL_HVAC_SETTINGS_VANE_2, "up_middle"},
	{MIEL_HVAC_SETTINGS_VANE_3, "center"},
	{MIEL_HVAC_SETTINGS_VANE_4, "down_middle"},
	{MIEL_HVAC_SETTINGS_VANE_5, "down"},
	{MIEL_HVAC_SETTINGS_VANE_SWING, "swing"},
};

static const struct miel_hvac_map miel_hvac_widevane_map[] = {
	{MIEL_HVAC_SETTINGS_WIDEVANE_AUTO, "auto"},
	{MIEL_HVAC_SETTINGS_WIDEVANE_LL, "left"},
	{MIEL_HVAC_SETTINGS_WIDEVANE_L, "left_middle"},
	{MIEL_HVAC_SETTINGS_WIDEVANE_C, "center"},
	{MIEL_HVAC_SETTINGS_WIDEVANE_R, "right"},
	{MIEL_HVAC_SETTINGS_WIDEVANE_RR, "right_middle"},
	{MIEL_HVAC_SETTINGS_WIDEVANE_LR, "split"},
	{MIEL_HVAC_SETTINGS_WIDEVANE_SWING, "swing"},
};

static const struct miel_hvac_map miel_hvac_prohibit_map[] = {
	{MIEL_HVAC_SETTINGS_PROHIBIT_OFF, "off"},
	{MIEL_HVAC_SETTINGS_PROHIBIT_POWER, "power"},
	{MIEL_HVAC_SETTINGS_PROHIBIT_MODE, "mode"},
	{MIEL_HVAC_SETTINGS_PROHIBIT_MODE_POWER, "mode_power"},
	{MIEL_HVAC_SETTINGS_PROHIBIT_TEMP, "temp"},
	{MIEL_HVAC_SETTINGS_PROHIBIT_TEMP_POWER, "temp_power"},
	{MIEL_HVAC_SETTINGS_PROHIBIT_TEMP_MODE, "temp_mode"},
	{MIEL_HVAC_SETTINGS_PROHIBIT_ALL, "all"},
};

static const struct miel_hvac_map miel_hvac_airdirection_map[] = {
	{MIEL_HVAC_SETTINGS_AIRDIRECTION_EVEN, "even"},
	{MIEL_HVAC_SETTINGS_AIRDIRECTION_INDIRECT, "indirect"},
	{MIEL_HVAC_SETTINGS_AIRDIRECTION_DIRECT, "direct"},
};

static const struct miel_hvac_map miel_hvac_compressor_map[] = {
	{MIEL_HVAC_STATUS_COMPRESSOR_OFF, "off"},
	{MIEL_HVAC_STATUS_COMPRESSOR_ON, "on"},
};

static const struct miel_hvac_map miel_hvac_timer_mode_map[] = {
	{MIEL_HVAC_TIMER_MODE_NONE, "none"},
	{MIEL_HVAC_TIMER_MODE_OFF, "off"},
	{MIEL_HVAC_TIMER_MODE_ON, "on"},
	{MIEL_HVAC_TIMER_MODE_BOTH, "on_and_off"},
};

static const struct miel_hvac_map miel_hvac_stage_operation_map[] = {
	{MIEL_HVAC_STAGE_OPERATION_NORMAL, "normal"},
	{MIEL_HVAC_STAGE_OPERATION_UNKNOWN, "unknown"},
	{MIEL_HVAC_STAGE_OPERATION_DEFROST, "defrost"},
	{MIEL_HVAC_STAGE_OPERATION_STANDBY, "standby"},
	{MIEL_HVAC_STAGE_OPERATION_PREHEAT, "preheat"},
};

static const struct miel_hvac_map miel_hvac_stage_fan_map[] = {
	{MIEL_HVAC_STAGE_FAN_OFF, "off"},
	{MIEL_HVAC_STAGE_FAN_QUIT, "quiet"},
	{MIEL_HVAC_STAGE_FAN_1, "1"},
	{MIEL_HVAC_STAGE_FAN_2, "2"},
	{MIEL_HVAC_STAGE_FAN_3, "3"},
	{MIEL_HVAC_STAGE_FAN_4, "4"},
	{MIEL_HVAC_STAGE_FAN_5, "5"},
};

static const struct miel_hvac_map miel_hvac_stage_mode_map[] = {
	{MIEL_HVAC_STAGE_MODE_MANUAL, "manual"},
	{MIEL_HVAC_STAGE_MODE_AUTO_FAN, "auto_fan"},
	{MIEL_HVAC_STAGE_MODE_AUTO_HEAT, "auto_heat"},
	{MIEL_HVAC_STAGE_MODE_AUTO_COOL, "auto_cool"},
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

struct miel_hvac_softc
{
	TasmotaSerial *sc_serial;
	struct miel_hvac_parser sc_parser;

	unsigned int sc_device;
	unsigned int sc_tick;
	bool sc_settings_set;
	bool sc_connected;

	struct miel_hvac_data sc_settings;
	struct miel_hvac_data sc_roomtemp;
	struct miel_hvac_data sc_timers;
	struct miel_hvac_data sc_status;
	struct miel_hvac_data sc_stage;

	struct miel_hvac_msg_update_settings sc_settings_update;
	struct miel_hvac_msg_update_remotetemp sc_remotetemp_update;
};

static inline bool
miel_hvac_update_settings_pending(struct miel_hvac_softc *sc)
{
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;
	return (update->flags != htons(0));
}

static struct miel_hvac_softc *miel_hvac_sc = nullptr;
static void miel_hvac_input_connected(struct miel_hvac_softc *, const void *, size_t);
static void miel_hvac_input_data(struct miel_hvac_softc *, const void *, size_t);
static void miel_hvac_input_updated(struct miel_hvac_softc *, const void *, size_t);

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
			AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME ": parse state MIDDLE1 expected %02x got %02x"
														   ", restarting"),
				   MIEL_HVAC_H_MIDDLE1, byte);
			return (MIEL_HVAC_P_START);
		}
		nstate = MIEL_HVAC_P_MIDDLE2;
		break;
	case MIEL_HVAC_P_MIDDLE2:
		if (byte != MIEL_HVAC_H_MIDDLE2)
		{
			AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME ": parse state MIDDLE2 expected %02x got %02x"
														   ", restarting"),
				   MIEL_HVAC_H_MIDDLE2, byte);
			return (MIEL_HVAC_P_START);
		}
		nstate = MIEL_HVAC_P_LEN;
		break;
	case MIEL_HVAC_P_LEN:
		if (byte == 0)
		{
			AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME ": skipping 0 byte message type 0x%02x"), p->p_type);
			return (MIEL_HVAC_P_SKIP_CKSUM);
		}
		p->p_len = byte;
		p->p_off = 0;

		switch (p->p_type)
		{
		case MIEL_HVAC_H_TYPE_CONNECTED:
		case MIEL_HVAC_H_TYPE_DATA:
		case MIEL_HVAC_H_TYPE_UPDATED:
			break;
		default:
			AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME ": skipping unknown message type 0x%02x"), p->p_type);
			return (MIEL_HVAC_P_SKIP);
		}

		if (byte > sizeof(p->p_data))
		{
			AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME ": skipping %u data bytes of message type 0x%02x"), p->p_len, p->p_type);
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
			AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME ": checksum failed, restarting"));
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
		}

		/* this message is done, wait for another */
		return (MIEL_HVAC_P_START);
	case MIEL_HVAC_P_SKIP:
		if (++p->p_off >= p->p_len)
			return (MIEL_HVAC_P_SKIP_CKSUM);
		return (nstate);
	case MIEL_HVAC_P_SKIP_CKSUM:
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
	AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME ": sending %s %s %02x"), ToHex_P((uint8_t *)&h, sizeof(h), hex_h, sizeof(hex_h)), ToHex_P((uint8_t *)data, len, hex_d, sizeof(hex_d)), miel_hvac_cksum_fini(cksum));

	serial->write(miel_hvac_cksum_fini(cksum));
	serial->flush();
}

#define miel_hvac_send_connect(_sc)                 \
	miel_hvac_send((_sc), MIEL_HVAC_H_TYPE_CONNECT, \
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

static void
miel_hvac_init_update_settings(struct miel_hvac_msg_update_settings *update)
{
	memset(update, 0, sizeof(*update));
	update->one = 1;
}

static inline void
miel_hvac_send_update_settings(struct miel_hvac_softc *sc, const struct miel_hvac_msg_update_settings *update)
{
	miel_hvac_send(sc, MIEL_HVAC_H_TYPE_UPDATE, update, sizeof(*update));
}

static inline void
miel_hvac_send_update_remotetemp(struct miel_hvac_softc *sc, const struct miel_hvac_msg_update_remotetemp *update)
{
	miel_hvac_send(sc, MIEL_HVAC_H_TYPE_UPDATE, update, sizeof(*update));
}

static bool
miel_hvac_set_power(struct miel_hvac_softc *sc)
{
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;
	uint16_t source = XdrvMailbox.payload;

	if (source == SRC_SWITCH)
		return (false);

	update->flags |= htons(MIEL_HVAC_SETTINGS_F_POWER);
	update->power = (XdrvMailbox.index & (1 << sc->sc_device)) ? MIEL_HVAC_SETTINGS_POWER_ON : MIEL_HVAC_SETTINGS_POWER_OFF;

	return (true);
}

static void
miel_hvac_respond_unsupported(void)
{
	ResponseCmndChar_P(PSTR("Unsupported"));
}

static void
miel_hvac_cmnd_setfanspeed(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;
	const struct miel_hvac_map *e;

	if (XdrvMailbox.data_len == 0)
		return;

	e = miel_hvac_map_byname(XdrvMailbox.data, miel_hvac_fan_map, nitems(miel_hvac_fan_map));
	if (e == NULL)
	{
		miel_hvac_respond_unsupported();
		return;
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

	e = miel_hvac_map_byname(XdrvMailbox.data, miel_hvac_mode_map, nitems(miel_hvac_mode_map));
	if (e == NULL)
	{
		miel_hvac_respond_unsupported();
		return;
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
	 * I could set power and then call miel_hvac_cmnd_setmode,
	 * but that would mean power gets turned on even if there's
	 * an invalid argument.
	 */
	e = miel_hvac_map_byname(XdrvMailbox.data, miel_hvac_mode_map, nitems(miel_hvac_mode_map));
	if (e == NULL)
	{
		miel_hvac_respond_unsupported();
		return;
	}
	update->flags |= htons(MIEL_HVAC_SETTINGS_F_POWER) | htons(MIEL_HVAC_SETTINGS_F_MODE);
	update->power = MIEL_HVAC_SETTINGS_POWER_ON;
	update->mode = e->byte;

	ResponseCmndChar_P(e->name);
}

static void
miel_hvac_cmnd_settemp(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;

	if (XdrvMailbox.data_len == 0)
		return;

	float degc = CharToFloat(XdrvMailbox.data);
	if (degc < MIEL_HVAC_SETTINGS_TEMP_MIN || degc > MIEL_HVAC_SETTINGS_TEMP_MAX)
	{
		miel_hvac_respond_unsupported();
		return;
	}
	update->flags |= htons(MIEL_HVAC_SETTINGS_F_TEMP);
	if (!temp_type)
	{
		update->temp = miel_hvac_deg2temp(degc);
		update->temp05 = 0;
	}

	else
	{
		update->temp = 0;
		update->temp05 = miel_hvac_deg2temp(degc);
	}

	ResponseCmndFloat(degc, Settings->flag2.temperature_resolution);
}

static void
miel_hvac_cmnd_setvane(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update_settings *update = &sc->sc_settings_update;
	const struct miel_hvac_map *e;

	if (XdrvMailbox.data_len == 0)
		return;

	e = miel_hvac_map_byname(XdrvMailbox.data, miel_hvac_vane_map, nitems(miel_hvac_vane_map));
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

	e = miel_hvac_map_byname(XdrvMailbox.data, miel_hvac_prohibit_map, nitems(miel_hvac_prohibit_map));
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

	e = miel_hvac_map_byname(XdrvMailbox.data, miel_hvac_widevane_map, nitems(miel_hvac_widevane_map));
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

	e = miel_hvac_map_byname(XdrvMailbox.data, miel_hvac_airdirection_map, nitems(miel_hvac_airdirection_map));
	if (e == NULL)
	{
		miel_hvac_respond_unsupported();
		return;
	}
	update->flags |= htons(MIEL_HVAC_SETTINGS_F_AIRDIRECTION);
	update->airdirection = e->byte;

	ResponseCmndChar_P(e->name);
}

static inline uint8_t
miel_hvac_remotetemp_degc2old(long degc)
{
	/*
	 * If a remote temperature reading is provided that cannot be
	 * represented by the temp_old field, implicitly clamp it to the
	 * supported min or max. The hardware does this anyway if you
	 * provide a high value, but without this the min value will
	 * underflow and turn a high value that the hardware thinks is 38.
	 */

	if (degc < MIEL_HVAC_REMOTETEMP_OLD_MIN)
		degc = MIEL_HVAC_REMOTETEMP_OLD_MIN;
	else if (degc > MIEL_HVAC_REMOTETEMP_OLD_MAX)
		degc = MIEL_HVAC_REMOTETEMP_OLD_MIN;

	return ((degc - MIEL_HVAC_REMOTETEMP_OLD_MIN) * MIEL_HVAC_REMOTETEMP_OLD_FACTOR);
}

static void
miel_hvac_remotetemp_auto_clear(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update_remotetemp *update = &sc->sc_remotetemp_update;
	uint8_t control = MIEL_HVAC_REMOTETEMP_CLR;
	long degc = 0;

	memset(update, 0, sizeof(*update));
	update->seven = 0x7;
	update->control = control;
	update->temp_old = miel_hvac_remotetemp_degc2old(degc);
	update->temp = (degc + MIEL_HVAC_REMOTETEMP_OFFSET) * MIEL_HVAC_REMOTETEMP_OLD_FACTOR;
	remotetemp_clear = false;
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
	uint8_t control = MIEL_HVAC_REMOTETEMP_SET;
	long degc;

	if (XdrvMailbox.data_len == 0)
		return;

	if (strcasecmp(XdrvMailbox.data, "clear") == 0)
	{
		control = MIEL_HVAC_REMOTETEMP_CLR;
		degc = 0;

		ResponseCmndChar_P("clear");
	}
	else
	{
		degc = strtol(XdrvMailbox.data, nullptr, 0);

		/* clamp the argument to supported values */
		if (degc < MIEL_HVAC_REMOTETEMP_MIN)
			degc = MIEL_HVAC_REMOTETEMP_MIN;
		else if (degc > MIEL_HVAC_REMOTETEMP_MAX)
			degc = MIEL_HVAC_REMOTETEMP_MAX;

		ResponseCmndNumber(degc);
	}
	memset(update, 0, sizeof(*update));
	update->seven = 0x7;
	update->control = control;

	/*
	 * Different HVACs (or more likely different generations
	 * of these HVACs) have different ways to encode the remote
	 * temperature value. This provides both of them to hopefully
	 * support all known types of HVACs.
	 */

	update->temp_old = miel_hvac_remotetemp_degc2old(degc);
	update->temp = (degc + MIEL_HVAC_REMOTETEMP_OFFSET) * MIEL_HVAC_REMOTETEMP_OLD_FACTOR;

	remotetemp_last_call_time = control == MIEL_HVAC_REMOTETEMP_SET ? millis() : 0;
	remotetemp_clear = control == MIEL_HVAC_REMOTETEMP_SET ? true : false;
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

/*
 * serial data handlers
 */

static void
miel_hvac_log_bytes(struct miel_hvac_softc *sc, const char *name, const void *buf, size_t len)
{
	char hex[(MIEL_HVAC_DATABUFLEN + 1) * 2];
	const unsigned char *b = (const unsigned char *)buf;
	AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME ": response %s %s"), name, ToHex_P(b, len, hex, sizeof(hex)));
}

static void
miel_hvac_input_connected(struct miel_hvac_softc *sc, const void *buf, size_t len)
{
	AddLog(LOG_LEVEL_INFO, PSTR(MIEL_HVAC_LOGNAME ": connected to Mitsubishi Electric HVAC"));
	sc->sc_connected = 1;
}

static void
miel_hvac_publish_settings(struct miel_hvac_softc *sc)
{
	const struct miel_hvac_data_settings *set = &sc->sc_settings.data.settings;
	char hex[(sizeof(sc->sc_settings) + 1) * 2];
	char temp[33];
	const char *name;
	const char *name_swing_h;

	name = miel_hvac_map_byval(set->power, miel_hvac_power_map, nitems(miel_hvac_power_map));
	if (name != NULL)
	{
		Response_P(PSTR("{\"" D_JSON_IRHVAC_POWER "\":\"%s\""), name);
	}

	name = miel_hvac_map_byval(set->mode & MIEL_HVAC_SETTINGS_MODE_MASK, miel_hvac_mode_map, nitems(miel_hvac_mode_map));
	if (name != NULL)
	{
		ResponseAppend_P(PSTR(",\"" D_JSON_IRHVAC_MODE "\":\"%s\""), name);
		ResponseAppend_P(PSTR(",\"HA" D_JSON_IRHVAC_MODE "\":\"%s\""), set->power ? name : "off");
	}
	if (set->temp05 == 0)
	{
		dtostrfd(ConvertTemp(miel_hvac_temp2deg(set->temp)), Settings->flag2.temperature_resolution, temp);
	}
	else
	{
		temp_type = true;
		dtostrfd(ConvertTemp(miel_hvac_temp2deg(set->temp05)), Settings->flag2.temperature_resolution, temp);
	}
	ResponseAppend_P(PSTR(",\"" D_JSON_IRHVAC_TEMP "\":%s"), temp);

	name = miel_hvac_map_byval(set->fan, miel_hvac_fan_map, nitems(miel_hvac_fan_map));
	if (name != NULL)
	{
		ResponseAppend_P(PSTR(",\"" D_JSON_IRHVAC_FANSPEED "\":\"%s\""), name);
	}

	name = miel_hvac_map_byval(set->vane, miel_hvac_vane_map, nitems(miel_hvac_vane_map));
	if (name != NULL)
	{
		ResponseAppend_P(PSTR(",\"" D_JSON_IRHVAC_SWINGV "\":\"%s\""), name);
	}

	name_swing_h = miel_hvac_map_byval(set->widevane & MIEL_HVAC_SETTINGS_WIDEVANE_MASK, miel_hvac_widevane_map, nitems(miel_hvac_widevane_map));
	if (name_swing_h != NULL)
	{
		ResponseAppend_P(PSTR(",\"" D_JSON_IRHVAC_SWINGH "\":\"%s\""), name_swing_h);
	}

	name = miel_hvac_map_byval(set->airdirection, miel_hvac_airdirection_map, nitems(miel_hvac_airdirection_map));
	if (name != NULL)
	{
		ResponseAppend_P(PSTR(",\"AirDirection\":\"%s\""), name_swing_h == "auto" ? name : "OFF");
	}

	name = miel_hvac_map_byval(set->prohibit, miel_hvac_prohibit_map, nitems(miel_hvac_prohibit_map));
	if (name != NULL)
	{
		ResponseAppend_P(PSTR(",\"Prohibit\":\"%s\""), name);
	}

	ResponseAppend_P(PSTR(",\"Settings\":\"%s\""), ToHex_P((uint8_t *)&sc->sc_settings, sizeof(sc->sc_settings), hex, sizeof(hex)));
	ResponseAppend_P(PSTR("}"));

	MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR("HVACSettings"));
}

static void
miel_hvac_input_settings(struct miel_hvac_softc *sc, const struct miel_hvac_data *d)
{
	const struct miel_hvac_data_settings *set = &d->data.settings;
	uint32_t state = set->power ? 1 : 0; /* see ExecuteCommandPower */
	bool publish;

	if (miel_hvac_update_settings_pending(sc))
	{
		/*
		 * Don't flop around printing settings that we might be
		 * changing, but also force them to be published again.
		 */
		sc->sc_settings_set = 0;
		return;
	}

	if (bitRead(TasmotaGlobal.power, sc->sc_device) != !!state)
		ExecuteCommandPower(sc->sc_device, state, SRC_SWITCH);

	publish = (sc->sc_settings_set == 0) || (memcmp(d, &sc->sc_settings, sizeof(sc->sc_settings)) != 0);
	sc->sc_settings_set = 1;
	sc->sc_settings = *d;

	if (publish)
		miel_hvac_publish_settings(sc);
}

static void
miel_hvac_data_response(struct miel_hvac_softc *sc, const struct miel_hvac_data *d)
{
	char hex[(sizeof(*d) + 1) * 2];
	Response_P(PSTR("{\"Bytes\":\"%s\"}"), ToHex_P((uint8_t *)d, sizeof(*d), hex, sizeof(hex)));
	MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR("HVACData"));
}

static void
miel_hvac_input_sensor(struct miel_hvac_softc *sc, struct miel_hvac_data *dst, const struct miel_hvac_data *src)
{
	bool publish;
	publish = (memcmp(dst, src, sizeof(*dst)) != 0);
	*dst = *src;

	if (publish)
		MqttPublishSensor();
}

static void
miel_hvac_input_data(struct miel_hvac_softc *sc, const void *buf, size_t len)
{
	const struct miel_hvac_data *d;

	miel_hvac_log_bytes(sc, "data", buf, len);
	if (len < sizeof(*d))
	{
		AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME ": short data response (%zu < %zu)"), len, sizeof(*d));
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
		break;
	case MIEL_HVAC_DATA_T_STAGE:
		miel_hvac_input_sensor(sc, &sc->sc_stage, d);
		break;
	default:
		miel_hvac_data_response(sc, d);
		break;
	}
}

static void
miel_hvac_input_updated(struct miel_hvac_softc *sc, const void *buf, size_t len)
{
	miel_hvac_log_bytes(sc, "updated", buf, len);
}

/*
 * FUNC handlers
 */

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
		AddLog(LOG_LEVEL_ERROR, PSTR(MIEL_HVAC_LOGNAME ": unable to allocate state"));
		return;
	}

	memset(sc, 0, sizeof(*sc));
	miel_hvac_init_update_settings(&sc->sc_settings_update);

	sc->sc_serial = new TasmotaSerial(Pin(GPIO_MIEL_HVAC_RX), Pin(GPIO_MIEL_HVAC_TX), 2);
	if (!sc->sc_serial->begin(baudrate, SERIAL_8E1))
	{
		AddLog(LOG_LEVEL_ERROR,
			   PSTR(MIEL_HVAC_LOGNAME ": unable to begin serial "
									  "(baudrate %d)"),
			   baudrate);
		goto del;
	}

	if (sc->sc_serial->hardwareSerial())
	{
		ClaimSerial();
		SetSerial(baudrate, TS_SERIAL_8E1);
	}
#ifdef ESP32
	AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME ": Serial UART%d"), sc->sc_serial->getUart());
#endif

	sc->sc_device = TasmotaGlobal.devices_present;
	UpdateDevicesPresent(1); /* claim a POWER device slot */

	miel_hvac_sc = sc;
	return;
del:
	delete sc->sc_serial;
free:
	free(sc);
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
	const char *mode;
	const char *name_swing_h;

	ResponseAppend_P(PSTR(",\"MiElHVAC\":{"));

	if (sc->sc_settings.type != 0)
	{
		const struct miel_hvac_data_settings *set = &sc->sc_settings.data.settings;
		char hex[(sizeof(sc->sc_settings) + 1) * 2];
		char temp[33];

		name = miel_hvac_map_byval(set->power, miel_hvac_power_map, nitems(miel_hvac_power_map));
		if (name != NULL)
		{
			ResponseAppend_P(PSTR("\"Power\":\"%s\""), name);
		}

		mode = miel_hvac_map_byval(set->mode & MIEL_HVAC_SETTINGS_MODE_MASK, miel_hvac_mode_map, nitems(miel_hvac_mode_map));
		if (mode != NULL)
		{
			ResponseAppend_P(PSTR(",\"Mode\":\"%s\""), mode);
		}

		if (set->temp05 == 0)
		{
			dtostrfd(ConvertTemp(miel_hvac_temp2deg(set->temp)), Settings->flag2.temperature_resolution, temp);
		}
		else
		{
			temp_type = true;
			dtostrfd(ConvertTemp(miel_hvac_temp2deg(set->temp05)), Settings->flag2.temperature_resolution, temp);
		}
		ResponseAppend_P(PSTR(",\"SetTemperature\":\"%s\""), temp);

		name = miel_hvac_map_byval(set->fan, miel_hvac_fan_map, nitems(miel_hvac_fan_map));
		if (name != NULL)
		{
			ResponseAppend_P(PSTR(",\"FanSpeed\":\"%s\""), name);
		}

		name = miel_hvac_map_byval(set->vane, miel_hvac_vane_map, nitems(miel_hvac_vane_map));
		if (name != NULL)
		{
			ResponseAppend_P(PSTR(",\"SwingV\":\"%s\""), name);
		}

		name_swing_h = miel_hvac_map_byval(set->widevane & MIEL_HVAC_SETTINGS_WIDEVANE_MASK, miel_hvac_widevane_map, nitems(miel_hvac_widevane_map));
		if (name_swing_h != NULL)
		{
			ResponseAppend_P(PSTR(",\"SwingH\":\"%s\""), name_swing_h);
		}

		name = miel_hvac_map_byval(set->airdirection, miel_hvac_airdirection_map, nitems(miel_hvac_airdirection_map));
		if (name != NULL)
		{
			ResponseAppend_P(PSTR(",\"AirDirection\":\"%s\""), name_swing_h == "auto" ? name : "off");
		}

		name = miel_hvac_map_byval(set->prohibit, miel_hvac_prohibit_map, nitems(miel_hvac_prohibit_map));
		if (name != NULL)
		{
			ResponseAppend_P(PSTR(",\"Prohibit\":\"%s\""), name);
		}

		ResponseAppend_P(PSTR(",\"Settings\":\"%s\""), ToHex_P((uint8_t *)&sc->sc_settings, sizeof(sc->sc_settings), hex, sizeof(hex)));
	}

	if (sc->sc_roomtemp.type != 0)
	{
		const struct miel_hvac_data_roomtemp *rt = &sc->sc_roomtemp.data.roomtemp;
		char hex[(sizeof(sc->sc_roomtemp) + 1) * 2];
		char room_temp[33];
		if (rt->temp05 == 0)
		{
			unsigned int temp = miel_hvac_roomtemp2deg(rt->temp);
			dtostrfd(ConvertTemp(temp), Settings->flag2.temperature_resolution, room_temp);
		}
		else
		{
			temp_type = true;
			float temp = miel_hvac_roomtemp2deg(rt->temp05);
			dtostrfd(ConvertTemp(temp), Settings->flag2.temperature_resolution, room_temp);
		}
		ResponseAppend_P(PSTR(",\"Temperature\":\"%s\""), room_temp);
		ResponseAppend_P(PSTR(",\"RemoteTemperatureSensorState\":\"%s\""), remotetemp_clear ? "on" : "off");

		char remotetempautocleartime[33];
		ultoa(remotetemp_auto_clear_time, remotetempautocleartime, 10);
		ResponseAppend_P(PSTR(",\"RemoteTemperatureSensorAutoClearTime\":\"%s\""), remotetempautocleartime);

		if (rt->outdoortemp > 1)
		{
			char outdoor_temp[33];
			float temp = miel_hvac_outdoortemp2deg(rt->outdoortemp);
			dtostrfd(ConvertTemp(temp), 1, outdoor_temp);
			ResponseAppend_P(PSTR(",\"OutdoorTemperature\":\"%s\""), outdoor_temp);
		}

		uint32_t combined_time = ((uint32_t)rt->operationtime << 16) | ((uint32_t)rt->operationtime1 << 8) | (uint32_t)rt->operationtime2;
		float operationtime_in_min = (float)combined_time;
		char operationtime[33];
		dtostrf(operationtime_in_min, 1, 0, operationtime);
		ResponseAppend_P(PSTR(",\"OperationTime\":\"%s\""), operationtime);

		ResponseAppend_P(PSTR(",\"RoomTemp\":\"%s\""), ToHex_P((uint8_t *)&sc->sc_roomtemp, sizeof(sc->sc_roomtemp), hex, sizeof(hex)));
	}

	if (sc->sc_timers.type != 0)
	{
		const struct miel_hvac_data_timers *timer = &sc->sc_timers.data.timers;
		char hex[(sizeof(sc->sc_timers) + 1) * 2];

		name = miel_hvac_map_byval(timer->mode, miel_hvac_timer_mode_map, nitems(miel_hvac_timer_mode_map));
		if (name != NULL)
		{
			ResponseAppend_P(PSTR(",\"TimerMode\":\"%s\""), name);

			unsigned int on_timer = timer->onminutes * 10;
			char timeron[33];
			utoa(on_timer, timeron, 10);
			ResponseAppend_P(PSTR(",\"TimerOn\":\"%s\""), timeron);

			unsigned int timer_on_remaining = timer->onminutesremaining * 10;
			char timeronremaining[33];
			utoa(timer_on_remaining, timeronremaining, 10);
			ResponseAppend_P(PSTR(",\"TimerOnRemaining\":\"%s\""), timeronremaining);

			unsigned int off_timer = timer->offminutes * 10;
			char timeroff[33];
			utoa(off_timer, timeroff, 10);
			ResponseAppend_P(PSTR(",\"TimerOff\":\"%s\""), timeroff);

			unsigned int timer_off_remaining = timer->offminutesremaining * 10;
			char timeroffremaining[33];
			utoa(timer_off_remaining, timeroffremaining, 10);
			ResponseAppend_P(PSTR(",\"TimerOffRemaining\":\"%s\""), timeroffremaining);
		}

		ResponseAppend_P(PSTR(",\"Timers\":\"%s\""), ToHex_P((uint8_t *)&sc->sc_timers, sizeof(sc->sc_timers), hex, sizeof(hex)));
	}

	if (sc->sc_status.type != 0)
	{
		const struct miel_hvac_data_status *status = &sc->sc_status.data.status;
		char hex[(sizeof(sc->sc_status) + 1) * 2];

		name = miel_hvac_map_byval(status->compressor, miel_hvac_compressor_map, nitems(miel_hvac_compressor_map));
		ResponseAppend_P(PSTR(",\"Compressor\":\"%s\""), name != NULL ? name : "N/A");

		unsigned int compressor_frequency = status->compressorfrequency;
		char compressorfrequency[33];
		utoa(compressor_frequency, compressorfrequency, 10);
		ResponseAppend_P(PSTR(",\"CompressorFrequency\":\"%s\""), compressorfrequency);

		uint16_t combined_power = ((uint16_t)status->operationpower << 8) | (uint16_t)status->operationpower1;
		char operationpower[33];
		dtostrfd((float)combined_power, 0, operationpower);
		ResponseAppend_P(PSTR(",\"OperationPower\":\"%s\""), operationpower);

		uint16_t combined_energy = ((uint16_t)status->operationenergy << 8) | (uint16_t)status->operationenergy1;
		float operationenergy_in_kWh = (float)combined_energy / 10.0;
		char operationenergy[33];
		dtostrfd((float)operationenergy_in_kWh, 1, operationenergy);
		ResponseAppend_P(PSTR(",\"OperationEnergy\":\"%s\""), operationenergy);

		ResponseAppend_P(PSTR(",\"Status\":\"%s\""), ToHex_P((uint8_t *)&sc->sc_status, sizeof(sc->sc_status), hex, sizeof(hex)));
	}

	if (sc->sc_stage.type != 0)
	{
		const struct miel_hvac_data_stage *stage = &sc->sc_stage.data.stage;
		char hex[(sizeof(sc->sc_stage) + 1) * 2];

		name = miel_hvac_map_byval(stage->operation, miel_hvac_stage_operation_map, nitems(miel_hvac_stage_operation_map));
		if (name != NULL)
		{
			ResponseAppend_P(PSTR(",\"OperationStage\":\"%s\""), name);
		}

		name = miel_hvac_map_byval(stage->fan, miel_hvac_stage_fan_map, nitems(miel_hvac_stage_fan_map));
		if (name != NULL)
		{
			ResponseAppend_P(PSTR(",\"FanStage\":\"%s\""), name);
		}

		name = miel_hvac_map_byval(stage->mode, miel_hvac_stage_mode_map, nitems(miel_hvac_stage_mode_map));
		if (name != NULL)
		{
			ResponseAppend_P(PSTR(",\"ModeStage\":\"%s\""), name == "manual" ? mode : name);
		}

		ResponseAppend_P(PSTR(",\"Stage\":\"%s\""), ToHex_P((uint8_t *)&sc->sc_stage, sizeof(sc->sc_stage), hex, sizeof(hex)));
	}

	ResponseAppend_P(PSTR("}"));
}

/*
 * This is set up to pace interactions with the aircon so we should
 * only have a single outstanding request at a time, and to avoid trying
 * to change settings while in the middle of reading them. SETTINGS is
 * requested frequently so changes from the IR remote are noticed quickly
 * and published. Posting new settings preempts queries for information.
 */

enum miel_hvac_connect_states
{
	MIEL_HVAC_CONNECT_S_2400_MSG,
	MIEL_HVAC_CONNECT_S_9600,
	MIEL_HVAC_CONNECT_S_9600_MSG,
	MIEL_HVAC_CONNECT_S_2400,
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
		/* MUZ-GA80VA doesnt respond :( */
		MIEL_HVAC_REQUEST_STAGE,
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
		struct miel_hvac_msg_update_remotetemp *remotetemp = &sc->sc_remotetemp_update;

		miel_hvac_send_update_remotetemp(sc, remotetemp);
		memset(remotetemp, 0, sizeof(*remotetemp));
		return;
	}

	i = (sc->sc_tick++ % nitems(updates));
	miel_hvac_request(sc, updates[i]);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

static const char miel_hvac_cmnd_names[] PROGMEM =
	// No prefix
	"|" D_CMND_MIEL_HVAC_SETFANSPEED
	"|" D_CMND_MIEL_HVAC_SETMODE
	"|" D_CMND_MIEL_HVAC_SETHAMODE
	"|" D_CMND_MIEL_HVAC_SETTEMP
	"|" D_CMND_MIEL_HVAC_SETSWINGV
	"|" D_CMND_MIEL_HVAC_SETSWINGH
	"|" D_CMND_MIEL_HVAC_SETAIRDIRECTION
	"|" D_CMND_MIEL_HVAC_SETPROHIBIT
	"|" D_CMND_MIEL_HVAC_REMOTETEMP
	"|" D_CMND_MIEL_HVAC_REMOTETEMP_AUTO_CLEAR_TIME
#ifdef MIEL_HVAC_DEBUG
	"|"
	"HVACRequest"
#endif
	;

static void (*const miel_hvac_cmnds[])(void) PROGMEM = {
	&miel_hvac_cmnd_setfanspeed,
	&miel_hvac_cmnd_setmode,
	&miel_hvac_cmnd_sethamode, /* rain of hate */
	&miel_hvac_cmnd_settemp,
	&miel_hvac_cmnd_setvane,
	&miel_hvac_cmnd_setwidevane,
	&miel_hvac_cmnd_setairdirection,
	&miel_hvac_cmnd_setprohibit,
	&miel_hvac_cmnd_remotetemp,
	&miel_hvac_cmnd_remotetemp_auto_clear_time,
#ifdef MIEL_HVAC_DEBUG
	&miel_hvac_cmnd_request,
#endif
};

bool Xdrv44(uint32_t function)
{
	bool result = false;
	struct miel_hvac_softc *sc = miel_hvac_sc;

	switch (function)
	{
	case FUNC_PRE_INIT:
		miel_hvac_pre_init();
		return (false);
	}

	if (sc == NULL)
		return (false);

	switch (function)
	{
	case FUNC_LOOP:
		miel_hvac_loop(sc);
		break;
	case FUNC_SET_DEVICE_POWER:
		result = miel_hvac_set_power(sc);
		break;
	case FUNC_EVERY_250_MSECOND:
		if (sc->sc_connected)
			miel_hvac_tick(sc);
		else
			miel_hvac_connect(sc);
		break;
	case FUNC_EVERY_50_MSECOND:
	case FUNC_EVERY_100_MSECOND:
	case FUNC_EVERY_200_MSECOND:
	case FUNC_EVERY_SECOND:
		if (remotetemp_clear && ((millis() - remotetemp_last_call_time) > remotetemp_auto_clear_time || remotetemp_last_call_time == 0))
		{
			miel_hvac_remotetemp_auto_clear();
		}
		break;
	case FUNC_JSON_APPEND:
		miel_hvac_sensor(sc);
		break;
	case FUNC_AFTER_TELEPERIOD:
		if (sc->sc_settings_set)
			miel_hvac_publish_settings(sc);
		break;
	case FUNC_COMMAND:
		result = DecodeCommand(miel_hvac_cmnd_names, miel_hvac_cmnds);
		break;
	case FUNC_ACTIVE:
		result = true;
		break;
	}
	return (result);
}

#endif // USE_MIEL_HVAC
