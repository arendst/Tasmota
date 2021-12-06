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

#define XDRV_44			44

#define nitems(_a)		(sizeof((_a)) / sizeof((_a)[0]))

#define CTASSERT(x)		extern char  _ctassert[(x) ? 1 : -1 ]	\
				    __attribute__((__unused__))

#define MIEL_HVAC_LOGNAME	"MiElHVAC"

#define D_CMND_MIEL_HVAC_SETFANSPEED "HVACSetFanSpeed"
#define D_CMND_MIEL_HVAC_SETMODE "HVACSetMode"
#define D_CMND_MIEL_HVAC_SETHAMODE "HVACSetHAMode"
#define D_CMND_MIEL_HVAC_SETTEMP "HVACSetTemp"
#define D_CMND_MIEL_HVAC_SETSWINGV "HVACSetSwingV"
#define D_CMND_MIEL_HVAC_SETSWINGH "HVACSetSwingH"
#define D_CMND_MIEL_HVAC_REMOTETEMP "HVACRemoteTemp"

#include <TasmotaSerial.h>

/* from hvac */
bool temp_type = false; 

struct miel_hvac_header {
	uint8_t			start;
#define MIEL_HVAC_H_START		0xfc
	uint8_t			type;
#define MIEL_HVAC_H_TYPE_UPDATED	0x61
#define MIEL_HVAC_H_TYPE_DATA		0x62
#define MIEL_HVAC_H_TYPE_CONNECTED	0x7a
	uint8_t			middle1;
#define MIEL_HVAC_H_MIDDLE1		0x01
	uint8_t			middle2;
#define MIEL_HVAC_H_MIDDLE2		0x30
	uint8_t			len;
};

struct miel_hvac_data_settings {
	uint8_t			_pad1[2];
	uint8_t			power;
	uint8_t			mode;
#define MIEL_HVAC_SETTINGS_MODE_MASK	0x7f
	uint8_t			temp;
	uint8_t			fan;
	uint8_t			vane;
	uint8_t			_pad2[2];
	uint8_t			widevane;
#define MIEL_HVAC_SETTTINGS_WIDEVANE_MASK \
					0x0f
	uint8_t			temp05;
};

struct miel_hvac_data_roomtemp {
	uint8_t			_pad1[2];
	uint8_t			temp;
	uint8_t			_pad2[2];
	uint8_t			temp05;
};

struct miel_hvac_data_status {
	uint8_t			_pad1[2];
	uint8_t			compressor;
	uint8_t			operation;
};

struct miel_hvac_data {
	uint8_t			type;
#define MIEL_HVAC_DATA_T_SETTINGS	0x02
#define MIEL_HVAC_DATA_T_ROOMTEMP	0x03
#define MIEL_HVAC_DATA_T_TIMER		0x05
#define MIEL_HVAC_DATA_T_STATUS		0x06
#define MIEL_HVAC_DATA_T_STAGE		0x09

	union {
		struct miel_hvac_data_settings
					settings;
		struct miel_hvac_data_roomtemp
					roomtemp;
		struct miel_hvac_data_status
					status;

		uint8_t			bytes[15];
	}			data;
};

CTASSERT(sizeof(struct miel_hvac_data) == 16);

CTASSERT(offsetof(struct miel_hvac_data, data.settings.power) == 3);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.mode) == 4);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.temp) == 5);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.fan) == 6);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.vane) == 7);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.widevane) == 10);
CTASSERT(offsetof(struct miel_hvac_data, data.settings.temp05) == 11);

CTASSERT(offsetof(struct miel_hvac_data, data.roomtemp.temp) == 3);
CTASSERT(offsetof(struct miel_hvac_data, data.roomtemp.temp05) == 6);

/* to hvac */

#define MIEL_HVAC_H_TYPE_CONNECT	0x5a
static const uint8_t miel_hvac_msg_connect[] = { 0xca, 0x01 };

#define MIEL_HVAC_H_TYPE_REQUEST	0x42

struct miel_hvac_msg_request {
	uint8_t			type;
#define MIEL_HVAC_REQUEST_SETTINGS	0x02
#define MIEL_HVAC_REQUEST_ROOMTEMP	0x03
#define MIEL_HVAC_REQUEST_TIMERS	0x05
#define MIEL_HVAC_REQUEST_STATUS	0x06
#define MIEL_HVAC_REQUEST_STAGE		0x09
	uint8_t			zero[15];
};

#define MIEL_HVAC_H_TYPE_UPDATE		0x41

struct miel_hvac_msg_update {
	uint8_t			one;
	uint16_t		flags;
#define MIEL_HVAC_UPDATE_F_WIDEVANE	(1 << 0)
#define MIEL_HVAC_UPDATE_F_POWER	(1 << 8)
#define MIEL_HVAC_UPDATE_F_MODE		(1 << 9)
#define MIEL_HVAC_UPDATE_F_TEMP		(1 << 10)
#define MIEL_HVAC_UPDATE_F_FAN		(1 << 11)
#define MIEL_HVAC_UPDATE_F_VANE		(1 << 12)
	uint8_t			power;
#define MIEL_HVAC_UPDATE_POWER_OFF	0x00
#define MIEL_HVAC_UPDATE_POWER_ON	0x01
	uint8_t			mode;
#define MIEL_HVAC_UPDATE_MODE_HEAT	0x01
#define MIEL_HVAC_UPDATE_MODE_DRY	0x02
#define MIEL_HVAC_UPDATE_MODE_COOL	0x03
#define MIEL_HVAC_UPDATE_MODE_FAN	0x07
#define MIEL_HVAC_UPDATE_MODE_AUTO	0x08
	uint8_t			temp;
#define MIEL_HVAC_UPDATE_TEMP_MIN	16
#define MIEL_HVAC_UPDATE_TEMP_MAX	31
	uint8_t			fan;
#define MIEL_HVAC_UPDATE_FAN_AUTO	0x00
#define MIEL_HVAC_UPDATE_FAN_QUIET	0x01
#define MIEL_HVAC_UPDATE_FAN_1		0x02
#define MIEL_HVAC_UPDATE_FAN_2		0x03
#define MIEL_HVAC_UPDATE_FAN_3		0x05
#define MIEL_HVAC_UPDATE_FAN_4		0x06
	uint8_t			vane;
#define MIEL_HVAC_UPDATE_VANE_AUTO	0x00
#define MIEL_HVAC_UPDATE_VANE_1		0x01
#define MIEL_HVAC_UPDATE_VANE_2		0x02
#define MIEL_HVAC_UPDATE_VANE_3		0x03
#define MIEL_HVAC_UPDATE_VANE_4		0x04
#define MIEL_HVAC_UPDATE_VANE_5		0x05
#define MIEL_HVAC_UPDATE_VANE_SWING	0x07
	uint8_t			_pad1[5];
	uint8_t			widevane;
#define MIEL_HVAC_UPDATE_WIDEVANE_MASK	0x0f
#define MIEL_HVAC_UPDATE_WIDEVANE_LL	0x01
#define MIEL_HVAC_UPDATE_WIDEVANE_L	0x02
#define MIEL_HVAC_UPDATE_WIDEVANE_LL	0x01
#define MIEL_HVAC_UPDATE_WIDEVANE_L	0x02
#define MIEL_HVAC_UPDATE_WIDEVANE_C	0x03
#define MIEL_HVAC_UPDATE_WIDEVANE_R	0x04
#define MIEL_HVAC_UPDATE_WIDEVANE_RR	0x05
#define MIEL_HVAC_UPDATE_WIDEVANE_LR	0x08
#define MIEL_HVAC_UPDATE_WIDEVANE_SWING	0x0c
#define MIEL_HVAC_UPDATE_WIDEVANE_ADJ	0x80
	uint8_t			temp05;
	uint8_t			_pad2[1];
} __packed;

CTASSERT(sizeof(struct miel_hvac_msg_update) == 16);
#define MIEL_HVAC_OFFS(_v) ((_v) - sizeof(struct miel_hvac_header))
CTASSERT(offsetof(struct miel_hvac_msg_update, flags) == MIEL_HVAC_OFFS(6));
CTASSERT(offsetof(struct miel_hvac_msg_update, power) == MIEL_HVAC_OFFS(8));
CTASSERT(offsetof(struct miel_hvac_msg_update, mode) == MIEL_HVAC_OFFS(9));
CTASSERT(offsetof(struct miel_hvac_msg_update, temp) == MIEL_HVAC_OFFS(10));
CTASSERT(offsetof(struct miel_hvac_msg_update, fan) == MIEL_HVAC_OFFS(11));
CTASSERT(offsetof(struct miel_hvac_msg_update, vane) == MIEL_HVAC_OFFS(12));
CTASSERT(offsetof(struct miel_hvac_msg_update, widevane) == MIEL_HVAC_OFFS(18));
CTASSERT(offsetof(struct miel_hvac_msg_update, temp05) == MIEL_HVAC_OFFS(19));

static inline uint8_t
miel_hvac_deg2temp(float deg)
{
	if (!temp_type) {
		return (31 - deg);
	}
	else {
		deg = 2*deg + 128;
		return ((uint8_t) deg);
	}
}

static inline float
miel_hvac_temp2deg(uint8_t temp)
{
	if (!temp_type) {
		return (31 - temp);
	}
	else {
		temp -= 128;
		return ((float) temp/2);
	}
}

static inline float
miel_hvac_roomtemp2deg(uint8_t roomtemp)
{
	if (!temp_type) {
		return ((unsigned int)roomtemp + 10);
	}
	else {
		roomtemp -= 128;
		return ((float) roomtemp/2);
	}
}

struct miel_hvac_msg_remotetemp {
	uint8_t			seven;
	uint8_t			control;
#define MIEL_HVAC_REMOTETEMP_CLR	0x00
#define MIEL_HVAC_REMOTETEMP_SET	0x01
	/* setting for older units expressed as .5C units starting at 8C */
	uint8_t			temp_old;
#define MIEL_HVAC_REMOTETEMP_OLD_MIN	8
#define MIEL_HVAC_REMOTETEMP_OLD_MAX	38
#define MIEL_HVAC_REMOTETEMP_OLD_FACTOR	2
	/* setting for newer units expressed as .5C units starting at -63C */
	uint8_t			temp;
#define MIEL_HVAC_REMOTETEMP_MIN	-63
#define MIEL_HVAC_REMOTETEMP_MAX	 63
#define MIEL_HVAC_REMOTETEMP_OFFSET	 64
#define MIEL_HVAC_REMOTETEMP_FACTOR	 2
	uint8_t			_pad2[12];
};

CTASSERT(sizeof(struct miel_hvac_msg_remotetemp) == 16);

static inline uint8_t
miel_hvac_cksum_fini(uint8_t sum)
{
	return (0xfc - sum);
}

struct miel_hvac_map {
	uint8_t			 byte;
	const char		*name;
};

static const struct miel_hvac_map miel_hvac_mode_map[] = {
	{ MIEL_HVAC_UPDATE_MODE_HEAT,		"heat"	},
	{ MIEL_HVAC_UPDATE_MODE_DRY,		"dry"	},
	{ MIEL_HVAC_UPDATE_MODE_COOL,		"cool"	},
	{ MIEL_HVAC_UPDATE_MODE_FAN,		"fan_only" },
	{ MIEL_HVAC_UPDATE_MODE_AUTO,		"auto"	},
};

static const struct miel_hvac_map miel_hvac_fan_map[] = {
	{ MIEL_HVAC_UPDATE_FAN_AUTO,		"auto"	},
	{ MIEL_HVAC_UPDATE_FAN_QUIET,		"quiet"	},
	{ MIEL_HVAC_UPDATE_FAN_1,		"1"	},
	{ MIEL_HVAC_UPDATE_FAN_2,		"2"	},
	{ MIEL_HVAC_UPDATE_FAN_3,		"3"	},
	{ MIEL_HVAC_UPDATE_FAN_4,		"4"	},
};

static const struct miel_hvac_map miel_hvac_vane_map[] = {
	{ MIEL_HVAC_UPDATE_VANE_AUTO,		"auto"	},
	{ MIEL_HVAC_UPDATE_VANE_1,		"1"	},
	{ MIEL_HVAC_UPDATE_VANE_2,		"2"	},
	{ MIEL_HVAC_UPDATE_VANE_3,		"3"	},
	{ MIEL_HVAC_UPDATE_VANE_4,		"4"	},
	{ MIEL_HVAC_UPDATE_VANE_5,		"5"	},
	{ MIEL_HVAC_UPDATE_VANE_SWING,		"swing"	},
};

static const struct miel_hvac_map miel_hvac_widevane_map[] = {
	{ MIEL_HVAC_UPDATE_WIDEVANE_LL,		"LL"	},
	{ MIEL_HVAC_UPDATE_WIDEVANE_L,		"L"	},
	{ MIEL_HVAC_UPDATE_WIDEVANE_C,		"C"	},
	{ MIEL_HVAC_UPDATE_WIDEVANE_R,		"R"	},
	{ MIEL_HVAC_UPDATE_WIDEVANE_RR,		"RR"	},
	{ MIEL_HVAC_UPDATE_WIDEVANE_LR,		"split"	},
	{ MIEL_HVAC_UPDATE_WIDEVANE_SWING,	"swing"	},
};

enum miel_hvac_parser_state {
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

#define MIEL_HVAC_DATABUFLEN	64

struct miel_hvac_parser {
	enum miel_hvac_parser_state
				 p_state;
	uint8_t			 p_type;
	uint8_t			 p_sum;
	uint8_t			 p_len;
	uint8_t			 p_off;
	uint8_t			 p_data[MIEL_HVAC_DATABUFLEN];
};

struct miel_hvac_softc {
	TasmotaSerial		*sc_serial;
	struct miel_hvac_parser	 sc_parser;

	unsigned int		 sc_device;
	unsigned int		 sc_tick;
	bool			 sc_settings_set;
	bool			 sc_connected;

	struct miel_hvac_data	 sc_settings;
	struct miel_hvac_data	 sc_temp;
	struct miel_hvac_data	 sc_status;
	struct miel_hvac_data	 sc_stage;

	struct miel_hvac_msg_update
				 sc_update;
	struct miel_hvac_msg_remotetemp
				 sc_remotetemp;
};

static inline bool
miel_hvac_update_pending(struct miel_hvac_softc *sc)
{
	struct miel_hvac_msg_update *update = &sc->sc_update;

	return (update->flags != htons(0));
}

static struct miel_hvac_softc	*miel_hvac_sc = nullptr;

static void	miel_hvac_input_connected(struct miel_hvac_softc *,
		    const void *, size_t);
static void	miel_hvac_input_data(struct miel_hvac_softc *,
		    const void *, size_t);
static void	miel_hvac_input_updated(struct miel_hvac_softc *,
		    const void *, size_t);

static enum miel_hvac_parser_state
miel_hvac_parse(struct miel_hvac_softc *sc, uint8_t byte)
{
	struct miel_hvac_parser *p = &sc->sc_parser;
	enum miel_hvac_parser_state nstate = p->p_state;

	switch (p->p_state) {
	case MIEL_HVAC_P_START:
		if (byte != MIEL_HVAC_H_START)
			return (MIEL_HVAC_P_START);

		/* reset state */
		p->p_sum = 0;

		nstate = MIEL_HVAC_P_TYPE;
		break;

	case MIEL_HVAC_P_TYPE:
		p->p_type = byte;
		nstate = MIEL_HVAC_P_MIDDLE1;
		break;

	case MIEL_HVAC_P_MIDDLE1:
		if (byte != MIEL_HVAC_H_MIDDLE1) {
			AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME
			   ": parse state MIDDLE1 expected %02x got %02x"
			   ", restarting"), MIEL_HVAC_H_MIDDLE1, byte);
			return (MIEL_HVAC_P_START);
		}

		nstate = MIEL_HVAC_P_MIDDLE2;
		break;

	case MIEL_HVAC_P_MIDDLE2:
		if (byte != MIEL_HVAC_H_MIDDLE2) {
			AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME
			   ": parse state MIDDLE2 expected %02x got %02x"
			   ", restarting"), MIEL_HVAC_H_MIDDLE2, byte);
			return (MIEL_HVAC_P_START);
		}

		nstate = MIEL_HVAC_P_LEN;
		break;

	case MIEL_HVAC_P_LEN:
		if (byte == 0) {
			AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME
			    ": skipping 0 byte message type 0x%02x"),
			    p->p_type);
			return (MIEL_HVAC_P_SKIP_CKSUM);
		}

		p->p_len = byte;
		p->p_off = 0;

		switch (p->p_type) {
		case MIEL_HVAC_H_TYPE_CONNECTED:
		case MIEL_HVAC_H_TYPE_DATA:
		case MIEL_HVAC_H_TYPE_UPDATED:
			break;
		default:
			AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME
			    ": skipping unknown message type 0x%02x"),
			    p->p_type);
			return (MIEL_HVAC_P_SKIP);
		}

		if (byte > sizeof(p->p_data)) {
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
		if (miel_hvac_cksum_fini(p->p_sum) != byte) {
			AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME
			    ": checksum failed, restarting"));
			return (MIEL_HVAC_P_START);
		}

		switch (p->p_type) {
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

	for (i = 0; i < len; i++) {
		uint8_t b = bytes[i];
		serial->write(b);
		cksum += b;
	}

	return (cksum);
}

static void
miel_hvac_send(struct miel_hvac_softc *sc, uint8_t type,
    const void *data, size_t len)
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
	AddLog(LOG_LEVEL_DEBUG,
	    PSTR(MIEL_HVAC_LOGNAME ": sending %s %s %02x"),
	    ToHex_P((uint8_t *)&h, sizeof(h), hex_h, sizeof(hex_h)),
	    ToHex_P((uint8_t *)data, len, hex_d, sizeof(hex_d)),
	    miel_hvac_cksum_fini(cksum));

	serial->write(miel_hvac_cksum_fini(cksum));
	serial->flush();
}

#define miel_hvac_send_connect(_sc) \
    miel_hvac_send((_sc), MIEL_HVAC_H_TYPE_CONNECT, \
    miel_hvac_msg_connect, sizeof(miel_hvac_msg_connect))

static const struct miel_hvac_map *
miel_hvac_map_byname(const char *name,
    const struct miel_hvac_map *m, size_t n)
{
	const struct miel_hvac_map *e;
	size_t i;

	for (i = 0; i < n; i++) {
		e = &m[i];
		if (strcasecmp(e->name, name) == 0)
			return (e);
	}

	return (NULL);
}

static const char *
miel_hvac_map_byval(uint8_t byte,
    const struct miel_hvac_map *m, size_t n)
{
	const struct miel_hvac_map *e;
	size_t i;

	for (i = 0; i < n; i++) {
		e = &m[i];
		if (byte == e->byte)
			return (e->name);
	}

	return (NULL);
}

static void
miel_hvac_request(struct miel_hvac_softc *sc, uint8_t type)
{
	struct miel_hvac_msg_request request = { type };

	miel_hvac_send(sc, MIEL_HVAC_H_TYPE_REQUEST,
	    &request, sizeof(request));
}

static void
miel_hvac_init_update(struct miel_hvac_msg_update *update)
{
	memset(update, 0, sizeof(*update));
	update->one = 1;
}

static inline void
miel_hvac_send_update(struct miel_hvac_softc *sc,
    const struct miel_hvac_msg_update *update)
{
	miel_hvac_send(sc, MIEL_HVAC_H_TYPE_UPDATE, update, sizeof(*update));
}

static inline void
miel_hvac_send_remotetemp(struct miel_hvac_softc *sc,
    const struct miel_hvac_msg_remotetemp *remotetemp)
{
	miel_hvac_send(sc, MIEL_HVAC_H_TYPE_UPDATE,
	    remotetemp, sizeof(*remotetemp));
}

static bool
miel_hvac_set_power(struct miel_hvac_softc *sc)
{
	struct miel_hvac_msg_update *update = &sc->sc_update;
	uint16_t source = XdrvMailbox.payload;

	if (source == SRC_SWITCH)
		return (false);

	update->flags |= htons(MIEL_HVAC_UPDATE_F_POWER);
	update->power = (XdrvMailbox.index & (1 << sc->sc_device)) ?
	    MIEL_HVAC_UPDATE_POWER_ON : MIEL_HVAC_UPDATE_POWER_OFF;

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
	struct miel_hvac_msg_update *update = &sc->sc_update;
	const struct miel_hvac_map *e;

	if (XdrvMailbox.data_len == 0)
		return;

	e = miel_hvac_map_byname(XdrvMailbox.data,
	    miel_hvac_fan_map, nitems(miel_hvac_fan_map));
	if (e == NULL) {
		miel_hvac_respond_unsupported();
		return;
	}

	update->flags |= htons(MIEL_HVAC_UPDATE_F_FAN);
	update->fan = e->byte;

	ResponseCmndChar_P(e->name);
}

static void
miel_hvac_cmnd_setmode(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update *update = &sc->sc_update;
	const struct miel_hvac_map *e;

	if (XdrvMailbox.data_len == 0)
		return;

	e = miel_hvac_map_byname(XdrvMailbox.data,
	    miel_hvac_mode_map, nitems(miel_hvac_mode_map));
	if (e == NULL) {
		miel_hvac_respond_unsupported();
		return;
	}

	update->flags |= htons(MIEL_HVAC_UPDATE_F_MODE);
	update->mode = e->byte;

	ResponseCmndChar_P(e->name);
}

static void
miel_hvac_cmnd_sethamode(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update *update = &sc->sc_update;
	const struct miel_hvac_map *e;

	if (XdrvMailbox.data_len == 0)
		return;

	if (strcasecmp(XdrvMailbox.data, "off") == 0) {
		update->flags |= htons(MIEL_HVAC_UPDATE_F_POWER);
		update->power = MIEL_HVAC_UPDATE_POWER_OFF;
		ResponseCmndChar_P(PSTR("off"));
		return;
	}

	/*
	 * I could set power and then call miel_hvac_cmnd_setmode,
	 * but that would mean power gets turned on even if there's
	 * an invalid argument.
	 */
	e = miel_hvac_map_byname(XdrvMailbox.data,
	    miel_hvac_mode_map, nitems(miel_hvac_mode_map));
	if (e == NULL) {
		miel_hvac_respond_unsupported();
		return;
	}

	update->flags |= htons(MIEL_HVAC_UPDATE_F_POWER) |
	    htons(MIEL_HVAC_UPDATE_F_MODE);
	update->power = MIEL_HVAC_UPDATE_POWER_ON;
	update->mode = e->byte;

	ResponseCmndChar_P(e->name);
}

static void
miel_hvac_cmnd_settemp(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update *update = &sc->sc_update;
	float degc;

	if (XdrvMailbox.data_len == 0)
		return;
	
	degc = strtof(XdrvMailbox.data, nullptr);
	if (degc < MIEL_HVAC_UPDATE_TEMP_MIN ||
	    degc > MIEL_HVAC_UPDATE_TEMP_MAX) {
		miel_hvac_respond_unsupported();
		return;
	}
	update->flags |= htons(MIEL_HVAC_UPDATE_F_TEMP);
	if (!temp_type) {
		update->temp = miel_hvac_deg2temp(degc);
		update->temp05 = 0;
	}

	else {
		update->temp = 0;
		update->temp05 = miel_hvac_deg2temp(degc);
	}

	ResponseCmndNumber(degc);
}

static void
miel_hvac_cmnd_setvane(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update *update = &sc->sc_update;
	const struct miel_hvac_map *e;

	if (XdrvMailbox.data_len == 0)
		return;

	e = miel_hvac_map_byname(XdrvMailbox.data,
	    miel_hvac_vane_map, nitems(miel_hvac_vane_map));
	if (e == NULL) {
		miel_hvac_respond_unsupported();
		return;
	}

	update->flags |= htons(MIEL_HVAC_UPDATE_F_VANE);
	update->vane = e->byte;

	ResponseCmndChar_P(e->name);
}

static void
miel_hvac_cmnd_setwidevane(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_update *update = &sc->sc_update;
	const struct miel_hvac_map *e;

	if (XdrvMailbox.data_len == 0)
		return;

	e = miel_hvac_map_byname(XdrvMailbox.data,
	    miel_hvac_widevane_map, nitems(miel_hvac_widevane_map));
	if (e == NULL) {
		miel_hvac_respond_unsupported();
		return;
	}

	update->flags |= htons(MIEL_HVAC_UPDATE_F_WIDEVANE);
	update->widevane = e->byte;

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

	return ((degc - MIEL_HVAC_REMOTETEMP_OLD_MIN) *
	    MIEL_HVAC_REMOTETEMP_OLD_FACTOR);
}

static void
miel_hvac_cmnd_remotetemp(void)
{
	struct miel_hvac_softc *sc = miel_hvac_sc;
	struct miel_hvac_msg_remotetemp *rt = &sc->sc_remotetemp;
	uint8_t control = MIEL_HVAC_REMOTETEMP_SET;
	long degc;

	if (XdrvMailbox.data_len == 0)
		return;

	if (strcasecmp(XdrvMailbox.data, "clear") == 0) {
		control = MIEL_HVAC_REMOTETEMP_CLR;
		degc = 0;

		ResponseCmndChar_P("clear");
	} else {
		degc = strtol(XdrvMailbox.data, nullptr, 0);

		/* clamp the argument to supported values */
		if (degc < MIEL_HVAC_REMOTETEMP_MIN)
			degc = MIEL_HVAC_REMOTETEMP_MIN;
		else if (degc > MIEL_HVAC_REMOTETEMP_MAX)
			degc = MIEL_HVAC_REMOTETEMP_MAX;

		ResponseCmndNumber(degc);
	}

	memset(rt, 0, sizeof(*rt));
	rt->seven = 0x7;
	rt->control = control;

	/*
	 * Different HVACs (or more likely different generations
	 * of these HVACs) have different ways to encode the remote
	 * temperature value. This provides both of them to hopefully
	 * support all known types of HVACs.
	 */

	rt->temp_old = miel_hvac_remotetemp_degc2old(degc);
	rt->temp = (degc + MIEL_HVAC_REMOTETEMP_OFFSET) *
	    MIEL_HVAC_REMOTETEMP_OLD_FACTOR;
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
miel_hvac_log_bytes(struct miel_hvac_softc *sc, const char *name,
    const void *buf, size_t len)
{
	char hex[(MIEL_HVAC_DATABUFLEN + 1) * 2];
	const unsigned char *b = (const unsigned char *)buf;

	AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME
	    ": response %s %s"), name, ToHex_P(b, len, hex, sizeof(hex)));
}

static void
miel_hvac_input_connected(struct miel_hvac_softc *sc,
    const void *buf, size_t len)
{
	AddLog(LOG_LEVEL_INFO,
	    PSTR(MIEL_HVAC_LOGNAME ": connected to Mitsubishi Electric HVAC"));
	sc->sc_connected = 1;
}

static void
miel_hvac_publish_settings(struct miel_hvac_softc *sc)
{
	const struct miel_hvac_data_settings *set =
	    &sc->sc_settings.data.settings;
	char hex[(sizeof(sc->sc_settings) + 1) * 2];
	char temp[33];
	const char *name;

	Response_P(PSTR("{\"" D_JSON_IRHVAC_POWER "\":\"%s\""),
	    set->power ? "ON" : "OFF");

	name = miel_hvac_map_byval( set->mode &
	    MIEL_HVAC_SETTINGS_MODE_MASK,
	    miel_hvac_mode_map, nitems(miel_hvac_mode_map));
	if (name != NULL) {
		ResponseAppend_P(PSTR(",\"" D_JSON_IRHVAC_MODE "\":\"%s\""),
		    name);
		ResponseAppend_P(PSTR(",\"HA" D_JSON_IRHVAC_MODE "\":\"%s\""),
		    set->power ? name : "off");
	}
	if (set->temp05 == 0) {
		dtostrfd(ConvertTemp(miel_hvac_temp2deg(set->temp)),
	    		Settings->flag2.temperature_resolution, temp);
	}
	else {
		temp_type = true;
		dtostrfd(ConvertTemp(miel_hvac_temp2deg(set->temp05)),
	    		Settings->flag2.temperature_resolution, temp);
	}
	ResponseAppend_P(PSTR(",\"" D_JSON_IRHVAC_TEMP "\":%s"), temp);

	name = miel_hvac_map_byval(set->fan,
	    miel_hvac_fan_map, nitems(miel_hvac_fan_map));
	if (name != NULL) {
		ResponseAppend_P(PSTR(",\"" D_JSON_IRHVAC_FANSPEED "\":\"%s\""),
		     name);
	}

	name = miel_hvac_map_byval(set->vane,
	    miel_hvac_vane_map, nitems(miel_hvac_vane_map));
	if (name != NULL) {
		ResponseAppend_P(PSTR(",\"" D_JSON_IRHVAC_SWINGV "\":\"%s\""),
		    name);
	}

	name = miel_hvac_map_byval(set->widevane &
	    MIEL_HVAC_SETTTINGS_WIDEVANE_MASK,
	    miel_hvac_widevane_map, nitems(miel_hvac_widevane_map));
	if (name != NULL) {
		ResponseAppend_P(PSTR(",\"" D_JSON_IRHVAC_SWINGH "\":\"%s\""),
		    name);
	}

	ResponseAppend_P(PSTR(",\"Bytes\":\"%s\""),
	    ToHex_P((uint8_t *)&sc->sc_settings, sizeof(sc->sc_settings),
	    hex, sizeof(hex)));

	ResponseAppend_P(PSTR("}"));

	MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR("HVACSettings"));
}

static void
miel_hvac_input_settings(struct miel_hvac_softc *sc,
    const struct miel_hvac_data *d)
{
	const struct miel_hvac_data_settings *set = &d->data.settings;
	uint32_t state = set->power ? 1 : 0; /* see ExecuteCommandPower */
	bool publish;

	if (miel_hvac_update_pending(sc)) {
		/*
		 * Don't flop around printing settings that we might be
		 * changing, but also force them to be published again.
		 */
		sc->sc_settings_set = 0;
		return;
	}

	if (bitRead(TasmotaGlobal.power, sc->sc_device) != !!state)
		ExecuteCommandPower(sc->sc_device, state, SRC_SWITCH);

	publish = (sc->sc_settings_set == 0) ||
	    (memcmp(d, &sc->sc_settings, sizeof(sc->sc_settings)) != 0);
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
miel_hvac_input_sensor(struct miel_hvac_softc *sc, struct miel_hvac_data *dst,
    const struct miel_hvac_data *src)
{
	bool publish;

	publish = (memcmp(dst, src, sizeof(*dst)) != 0);
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
	if (len < sizeof(*d)) {
		AddLog(LOG_LEVEL_DEBUG, PSTR(MIEL_HVAC_LOGNAME
		    ": short data response (%zu < %zu)"), len, sizeof(*d));
		return;
	}

	d = (const struct miel_hvac_data *)buf;

	switch (d->type) {
	case MIEL_HVAC_DATA_T_SETTINGS:
		miel_hvac_input_settings(sc, d);
		break;
	case MIEL_HVAC_DATA_T_ROOMTEMP:
		miel_hvac_input_sensor(sc, &sc->sc_temp, d);
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
miel_hvac_input_updated(struct miel_hvac_softc *sc,
    const void *buf, size_t len)
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
	if (sc == NULL) {
		AddLog(LOG_LEVEL_ERROR,
		    PSTR(MIEL_HVAC_LOGNAME ": unable to allocate state"));
		return;
	}

	memset(sc, 0, sizeof(*sc));
	miel_hvac_init_update(&sc->sc_update);

	sc->sc_serial = new TasmotaSerial(Pin(GPIO_MIEL_HVAC_RX),
	    Pin(GPIO_MIEL_HVAC_TX), 2);

	if (!sc->sc_serial->begin(baudrate, 2)) {
		AddLog(LOG_LEVEL_ERROR,
		    PSTR(MIEL_HVAC_LOGNAME ": unable to begin serial "
		    "(baudrate %d)"), baudrate);
		goto del;
	}

	if (sc->sc_serial->hardwareSerial()) {
		ClaimSerial();
		SetSerial(baudrate, TS_SERIAL_8E1);
	}

	sc->sc_device = TasmotaGlobal.devices_present++; /* claim a POWER device slot */

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

	while (serial->available()) {
		yield();

		sc->sc_parser.p_state = miel_hvac_parse(sc, serial->read());
	}
}

static void
miel_hvac_sensor(struct miel_hvac_softc *sc)
{
	char hex[(sizeof(sc->sc_status) + 1) * 2];
	const char *sep = "";

	ResponseAppend_P(PSTR("," "\"MiElHVAC\":{"));

	if (sc->sc_temp.type != 0) {
		const struct miel_hvac_data_roomtemp *rt =
		    &sc->sc_temp.data.roomtemp;
		char room_temp[33];
		if(rt->temp05 == 0) {
			unsigned int temp = miel_hvac_roomtemp2deg(rt->temp);
			dtostrfd(ConvertTemp(temp),
		    	    Settings->flag2.temperature_resolution, room_temp);
			}
		else {
			temp_type = true;
			float temp = miel_hvac_roomtemp2deg(rt->temp05);
			dtostrfd(ConvertTemp(temp),
		    	    Settings->flag2.temperature_resolution, room_temp);
		}
		ResponseAppend_P(PSTR("\"" D_JSON_TEMPERATURE "\":%s"),
		    room_temp);

		sep = ",";
	}

	if (sc->sc_status.type != 0) {
		const struct miel_hvac_data_status *s =
		    &sc->sc_status.data.status;

		ResponseAppend_P(PSTR("%s" "\"Operation\":\"%s\"" ","
		    "\"Compressor\":\"%s\""), sep,
		    s->operation ? "ON" : "OFF",
		    s->compressor ? "ON" : "OFF");

		sep = ",";
	}

	if (sc->sc_temp.type != 0) {
		ResponseAppend_P(PSTR("%s" "\"roomtemp\":\"%s\""), sep,
		    ToHex_P((uint8_t *)&sc->sc_temp, sizeof(sc->sc_temp),
		    hex, sizeof(hex)));

		sep = ",";
	}

	if (sc->sc_status.type != 0) {
		ResponseAppend_P(PSTR("%s" "\"status\":\"%s\""), sep,
		    ToHex_P((uint8_t *)&sc->sc_status, sizeof(sc->sc_status),
		    hex, sizeof(hex)));

		sep = ",";
	}

	if (sc->sc_stage.type != 0) {
		ResponseAppend_P(PSTR("%s" "\"stage\":\"%s\""), sep,
		    ToHex_P((uint8_t *)&sc->sc_stage, sizeof(sc->sc_stage),
		    hex, sizeof(hex)));
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

enum miel_hvac_connect_states {
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

	switch (state) {
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

	serial->begin(baudrate, 2);
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
		/* MUZ-GA80VA doesnt respond :( */
		MIEL_HVAC_REQUEST_STAGE,
	};

	unsigned int i;

	if (miel_hvac_update_pending(sc)) {
		struct miel_hvac_msg_update *update = &sc->sc_update;

		miel_hvac_send_update(sc, update);

		miel_hvac_init_update(update);

		/* refresh settings on next tick */
		sc->sc_tick = 0;
		return;
	}

	if (sc->sc_remotetemp.seven) {
		struct miel_hvac_msg_remotetemp *remotetemp =
		    &sc->sc_remotetemp;

		miel_hvac_send_remotetemp(sc, remotetemp);
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
	"|" D_CMND_MIEL_HVAC_REMOTETEMP
#ifdef MIEL_HVAC_DEBUG
	"|" "HVACRequest"
#endif
	;

static void (*const miel_hvac_cmnds[])(void) PROGMEM = {
	&miel_hvac_cmnd_setfanspeed,
	&miel_hvac_cmnd_setmode,
	&miel_hvac_cmnd_sethamode, /* rain of hate */
	&miel_hvac_cmnd_settemp,
	&miel_hvac_cmnd_setvane,
	&miel_hvac_cmnd_setwidevane,
	&miel_hvac_cmnd_remotetemp,
#ifdef MIEL_HVAC_DEBUG
	&miel_hvac_cmnd_request,
#endif
};

bool Xdrv44(uint8_t function) {
	bool result = false;
	struct miel_hvac_softc *sc = miel_hvac_sc;

	switch (function) {
	case FUNC_PRE_INIT:
		miel_hvac_pre_init();
		return (false);
	}

	if (sc == NULL)
		return (false);

	switch (function) {
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
	}

	return (result);
}

#endif  // USE_MIEL_HVAC
