/*
 * xdrv_65_tuyamcubr.ino - TuyaMCU Bridge support for Tasmota
 */

/*
 * Copyright (C) 2023 David Gwynne <david@gwynne.id.au>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifdef USE_TUYAMCUBR

/*
 * Tuya MCU Bridge
 */

/*
 * TODO:
 *
 * - handling wifi reset requests from the MCU
 * - low power stuff?
 * - support for (re)sending status updates and device info queries
 * - supporting the raw and string Dp types
 * - restarting the tuya mcu state machine?
 * - restarting the rx state machine when no bytes are rxed for a while
 * - gmtime sync
 */

#define XDRV_65			65

#ifndef nitems
#define nitems(_a)		(sizeof((_a)) / sizeof((_a)[0]))
#endif

#ifndef CTASSERT
#define CTASSERT(x)		extern char  _ctassert[(x) ? 1 : -1 ]	\
				    __attribute__((__unused__))
#endif

#define TUYAMCUBR_LOGNAME		"TYB"
#define TUYAMCUBR_FMT(_fmt)		PSTR(TUYAMCUBR_LOGNAME ": " _fmt)

#define D_CMND_TUYAMCUBR_PREFIX		"TuyaMCU"

#define D_CMND_TUYAMCUBR_DATA_RAW	"Raw"
#define D_CMND_TUYAMCUBR_DATA_BOOL	"Bool"
#define D_CMND_TUYAMCUBR_DATA_VALUE	"Value"
#define D_CMND_TUYAMCUBR_DATA_STRING	"String"
#define D_CMND_TUYAMCUBR_DATA_ENUM	"Enum"

#include <TasmotaSerial.h>

struct tuyamcubr_header {
	uint8_t			header[2];
#define TUYAMCUBR_H_ONE			0x55
#define TUYAMCUBR_H_TWO			0xaa
	uint8_t			version;
	uint8_t			command;
	uint16_t		datalen;
};

CTASSERT(sizeof(struct tuyamcubr_header) == 6);

#define TUYAMCUBR_CMD_HEARTBEAT		0x00
#define TUYAMCUBR_CMD_PRODUCT		0x01
#define TUYAMCUBR_CMD_MODE		0x02
#define TUYAMCUBR_CMD_WIFI_STATE	0x03
#define TUYAMCUBR_CMD_WIFI_RESET	0x04
#define TUYAMCUBR_CMD_WIFI_SELECT	0x05
#define TUYAMCUBR_CMD_SET_DP		0x06
#define TUYAMCUBR_CMD_STATE		0x07
#define TUYAMCUBR_CMD_QUERY_STATE	0x08
#define TUYAMCUBR_CMD_INIT_UPGRADE	0x0a
#define TUYAMCUBR_CMD_UPGRADE_PKG	0x0b
#define TUYAMCUBR_CMD_GMTIME		0x0c
#define TUYAMCUBR_CMD_TIME		0x1c

/* wifi state */

#define TUYAMCUBR_NETWORK_STATUS_1	0x00 /* pairing in EZ mode */
#define TUYAMCUBR_NETWORK_STATUS_2	0x01 /* pairing in AP mode */
#define TUYAMCUBR_NETWORK_STATUS_3	0x02 /* WiFi */
#define TUYAMCUBR_NETWORK_STATUS_4	0x03 /* WiFi + router */
#define TUYAMCUBR_NETWORK_STATUS_5	0x04 /* WiFi + router + cloud*/
#define TUYAMCUBR_NETWORK_STATUS_6	0x05 /* low power mode */
#define TUYAMCUBR_NETWORK_STATUS_7	0x06 /* pairing in EZ+AP mode */

/* gmtime */

struct tuyamcubr_gmtime {
	uint8_t			valid;
	uint8_t			year;		/* + 2000 */
	uint8_t			month;		/* 1 to 12 */
	uint8_t			day;		/* 1 to 31 */
	uint8_t			hour;		/* 0 to 23 */
	uint8_t			minute;		/* 0 to 59 */
	uint8_t			second;		/* 0 to 59 */
};

CTASSERT(sizeof(struct tuyamcubr_gmtime) == 7);

/* time */

struct tuyamcubr_time {
	uint8_t			valid;
	uint8_t			year;		/* 2000 + */
	uint8_t			month;		/* 1 to 12 */
	uint8_t			day;		/* 1 to 31 */
	uint8_t			hour;		/* 0 to 23 */
	uint8_t			minute;		/* 0 to 59 */
	uint8_t			second;		/* 0 to 59 */
	uint8_t			weekday;	/* 1 (monday) to 7 */
};

CTASSERT(sizeof(struct tuyamcubr_time) == 8);

/* set dp */

struct tuyamcubr_data_header {
	uint8_t			dpid;
	uint8_t			type;
	uint16_t		len;

	/* followed by len bytes */
};

CTASSERT(sizeof(struct tuyamcubr_data_header) == 4);

#define TUYAMCUBR_DATA_TYPE_RAW		0x00
#define TUYAMCUBR_DATA_TYPE_BOOL	0x01
#define TUYAMCUBR_DATA_TYPE_VALUE	0x02
#define TUYAMCUBR_DATA_TYPE_STRING	0x03
#define TUYAMCUBR_DATA_TYPE_ENUM	0x04

struct tuyamcubr_data_type {
	const char			*t_name;
	int				 t_len;
	uint32_t			 t_max;
	uint32_t (*t_rd)(const uint8_t *);
	void	 (*t_wr)(uint8_t *, uint32_t);
};

static uint32_t
tuyamcubr_rd_u8(const uint8_t *b)
{
	return (*b);
}

static void
tuyamcubr_wr_u8(uint8_t *b, uint32_t v)
{
	*b = v;
}

static uint32_t
tuyamcubr_rd_u32(const uint8_t *b)
{
	uint32_t be32;
	memcpy(&be32, b, sizeof(be32));
	return (ntohl(be32));
}

static void
tuyamcubr_wr_u32(uint8_t *b, uint32_t v)
{
	uint32_t be32 = htonl(v);
	memcpy(b, &be32, sizeof(be32));
}

static const struct tuyamcubr_data_type tuyamcubr_data_types[] = {
	[TUYAMCUBR_DATA_TYPE_RAW] = {
		.t_name = D_CMND_TUYAMCUBR_DATA_RAW,
		.t_len = -1,
	},
	[TUYAMCUBR_DATA_TYPE_BOOL] = {
		.t_name = D_CMND_TUYAMCUBR_DATA_BOOL,
		.t_len = 1,
		.t_max = 1,
		.t_rd = tuyamcubr_rd_u8,
		.t_wr = tuyamcubr_wr_u8,
	},
	[TUYAMCUBR_DATA_TYPE_VALUE] = {
		.t_name = D_CMND_TUYAMCUBR_DATA_VALUE,
		.t_len = sizeof(uint32_t),
		.t_max = 0xffffffff,
		.t_rd = tuyamcubr_rd_u32,
		.t_wr = tuyamcubr_wr_u32,
	},
	[TUYAMCUBR_DATA_TYPE_STRING] = {
		.t_name = D_CMND_TUYAMCUBR_DATA_STRING,
		.t_len = -1,
	},
	[TUYAMCUBR_DATA_TYPE_ENUM] = {
		.t_name = D_CMND_TUYAMCUBR_DATA_ENUM,
		.t_len = 1,
		.t_max = 0xff,
		.t_rd = tuyamcubr_rd_u8,
		.t_wr = tuyamcubr_wr_u8,
	},
};

static inline const struct tuyamcubr_data_type *
tuyamcubr_find_data_type(uint8_t type)
{
	const struct tuyamcubr_data_type *dt;

	if (type > nitems(tuyamcubr_data_types))
		return (NULL);

	dt = &tuyamcubr_data_types[type];
	if (dt->t_name == NULL)
		return (NULL);

	return (dt);
}

static inline uint8_t
tuyamcubr_cksum_fini(uint8_t sum)
{
	/*
	 * "Start from the header, add up all the bytes, and then divide
	 * the sum by 256 to get the remainder."
	 *
	 * If we accumulate bytes in a uint8_t, we get this for free.
	 */

	return (sum);
}

enum tuyamcubr_parser_state {
	TUYAMCUBR_P_START,
	TUYAMCUBR_P_HEADER,
	TUYAMCUBR_P_VERSION,
	TUYAMCUBR_P_COMMAND,
	TUYAMCUBR_P_LEN1,
	TUYAMCUBR_P_LEN2,
	TUYAMCUBR_P_DATA,
	TUYAMCUBR_P_CKSUM,

	TUYAMCUBR_P_SKIP,
	TUYAMCUBR_P_SKIP_CKSUM,
};

//#ifdef ESP8266
//#define TUYAMCUBR_BUFLEN	256
//#else
#define TUYAMCUBR_BUFLEN	1024
//#endif

struct tuyamcubr_parser {
	enum tuyamcubr_parser_state	 p_state;
	unsigned int			 p_deadline;

	uint8_t				 p_version;
	uint8_t				 p_command;
	uint8_t				 p_sum;

	uint16_t			 p_len;
	uint8_t				 p_off;
	uint8_t				 p_data[TUYAMCUBR_BUFLEN];
};

struct tuyamcubr_dp {
	STAILQ_ENTRY(tuyamcubr_dp)	 dp_entry;
	uint8_t				 dp_id;
	uint8_t				 dp_type;

	uint32_t			 dp_value;
};
STAILQ_HEAD(tuyamcubr_dps, tuyamcubr_dp);

enum tuyamcubr_state {
	TUYAMCUBR_S_START,
	TUYAMCUBR_S_PROD_INFO,
	TUYAMCUBR_S_MODE,
	TUYAMCUBR_S_NET_STATUS,
	TUYAMCUBR_S_RUNNING,
};

struct tuyamcubr_softc {
	TasmotaSerial		*sc_serial;
	struct tuyamcubr_parser	 sc_parser;

	enum tuyamcubr_state	 sc_state;
	unsigned int		 sc_deadline;
	unsigned int		 sc_waiting;
	uint8_t			 sc_network_status;

	unsigned int		 sc_clock;
	struct tuyamcubr_dps	 sc_dps;
};

static struct tuyamcubr_softc	*tuyamcubr_sc = nullptr;

struct tuyamcubr_recv_command {
	uint8_t			 r_command;
	void (*r_func)(struct tuyamcubr_softc *, uint8_t,
	    const uint8_t *, size_t);
};

static void	tuyamcubr_recv_heartbeat(struct tuyamcubr_softc *, uint8_t,
		    const uint8_t *, size_t);
static void	tuyamcubr_recv_product_info(struct tuyamcubr_softc *, uint8_t,
		    const uint8_t *, size_t);
static void	tuyamcubr_recv_mode(struct tuyamcubr_softc *, uint8_t,
		    const uint8_t *, size_t);
static void	tuyamcubr_recv_net_status(struct tuyamcubr_softc *, uint8_t,
		    const uint8_t *, size_t);
static void	tuyamcubr_recv_status(struct tuyamcubr_softc *, uint8_t,
		    const uint8_t *, size_t);
static void	tuyamcubr_recv_time(struct tuyamcubr_softc *, uint8_t,
		    const uint8_t *, size_t);

static const struct tuyamcubr_recv_command tuyamcubr_recv_commands[] = {
	{ TUYAMCUBR_CMD_HEARTBEAT,	tuyamcubr_recv_heartbeat },
	{ TUYAMCUBR_CMD_PRODUCT,	tuyamcubr_recv_product_info },
	{ TUYAMCUBR_CMD_MODE,		tuyamcubr_recv_mode },
	{ TUYAMCUBR_CMD_WIFI_STATE,	tuyamcubr_recv_net_status },
	{ TUYAMCUBR_CMD_STATE,		tuyamcubr_recv_status },
	{ TUYAMCUBR_CMD_TIME,		tuyamcubr_recv_time },
};

static void
tuyamcubr_recv(struct tuyamcubr_softc *sc, const struct tuyamcubr_parser *p)
{
	const struct tuyamcubr_recv_command *r;
	const uint8_t *data = p->p_data;
	size_t len = p->p_len;
	size_t i;

	if (len > 0) {
		AddLog(LOG_LEVEL_DEBUG,
		    TUYAMCUBR_FMT("recv version 0x%02x command 0x%02x: %*_H"),
		    p->p_version, p->p_command, len, data);
	} else {
		AddLog(LOG_LEVEL_DEBUG,
		    TUYAMCUBR_FMT("recv version 0x%02x command 0x%02x"),
		    p->p_version, p->p_command);
	}

	for (i = 0; i < nitems(tuyamcubr_recv_commands); i++) {
		r = &tuyamcubr_recv_commands[i];

		if (r->r_command == p->p_command) {
			r->r_func(sc, p->p_version, data, len);
			return;
		}
	}

	/* unhandled command? */
}

static enum tuyamcubr_parser_state
tuyamcubr_parse(struct tuyamcubr_softc *sc, uint8_t byte)
{
	struct tuyamcubr_parser *p = &sc->sc_parser;
	enum tuyamcubr_parser_state nstate = p->p_state;

	switch (p->p_state) {
	case TUYAMCUBR_P_START:
		if (byte != TUYAMCUBR_H_ONE)
			return (TUYAMCUBR_P_START);

		/* reset state */
		p->p_sum = 0;

		nstate = TUYAMCUBR_P_HEADER;
		break;
	case TUYAMCUBR_P_HEADER:
		if (byte != TUYAMCUBR_H_TWO)
			return (TUYAMCUBR_P_START);

		p->p_deadline = sc->sc_clock + (10 * 1000);
		nstate = TUYAMCUBR_P_VERSION;
		break;
	case TUYAMCUBR_P_VERSION:
		p->p_version = byte;
		nstate = TUYAMCUBR_P_COMMAND;
		break;
	case TUYAMCUBR_P_COMMAND:
		p->p_command = byte;
		nstate = TUYAMCUBR_P_LEN1;
		break;

	case TUYAMCUBR_P_LEN1:
		p->p_len = (uint16_t)byte << 8;
		nstate = TUYAMCUBR_P_LEN2;
		break;
	case TUYAMCUBR_P_LEN2:
		p->p_len |= (uint16_t)byte;
		p->p_off = 0;

		if (p->p_len > sizeof(p->p_data)) {
			AddLog(LOG_LEVEL_DEBUG,
			    TUYAMCUBR_FMT("skipping command %02x"
			    ", too much data %zu/%zu"), p->p_command,
			    p->p_len, sizeof(p->p_data));
			return (TUYAMCUBR_P_SKIP);
		}

		nstate = (p->p_len > 0) ? TUYAMCUBR_P_DATA : TUYAMCUBR_P_CKSUM;
		break;

	case TUYAMCUBR_P_DATA:
		p->p_data[p->p_off++] = byte;
		if (p->p_off >= p->p_len)
			nstate = TUYAMCUBR_P_CKSUM;
		break;

	case TUYAMCUBR_P_CKSUM:
		if (tuyamcubr_cksum_fini(p->p_sum) != byte) {
			AddLog(LOG_LEVEL_DEBUG,
			    TUYAMCUBR_FMT("checksum failed, skipping"));
			return (TUYAMCUBR_P_START);
		}

		tuyamcubr_recv(sc, p);

		/* this message is done, wait for another */
		return (TUYAMCUBR_P_START);

	case TUYAMCUBR_P_SKIP:
		if (++p->p_off >= p->p_len)
			return (TUYAMCUBR_P_SKIP_CKSUM);
		return (nstate);
	case TUYAMCUBR_P_SKIP_CKSUM:
		return (TUYAMCUBR_P_START);
	}

	p->p_sum += byte;

	return (nstate);
}

static uint8_t
tuyamcubr_write(struct tuyamcubr_softc *sc, const void *data, size_t len)
{
	TasmotaSerial *serial = sc->sc_serial;
	const uint8_t *bytes = (const uint8_t *)data;
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
tuyamcubr_send(struct tuyamcubr_softc *sc, uint8_t command,
    const void *data, size_t len)
{
	TasmotaSerial *serial = sc->sc_serial;
	struct tuyamcubr_header h = {
		.header = { TUYAMCUBR_H_ONE, TUYAMCUBR_H_TWO },
		.version = 0x00,
		.command = command,
		.datalen = htons(len),
	};
	uint8_t cksum = 0;

	if (len) {
		AddLog(LOG_LEVEL_DEBUG,
		    TUYAMCUBR_FMT("send version 0x%02x command 0x%02x: %*_H"),
		    h.version, h.command, len, data);
	} else {
		AddLog(LOG_LEVEL_DEBUG,
		    TUYAMCUBR_FMT("send version 0x%02x command 0x%02x"),
		    h.version, h.command);
	}

	cksum += tuyamcubr_write(sc, &h, sizeof(h));
	if (len > 0)
		cksum += tuyamcubr_write(sc, data, len);
	cksum = tuyamcubr_cksum_fini(cksum);
	serial->write(cksum);
	serial->flush();
}

/* if we have polymorphic funcions then we may as well (ab)use them */
static void
tuyamcubr_send(struct tuyamcubr_softc *sc, uint8_t command)
{
	tuyamcubr_send(sc, command, NULL, 0);
}

static void
tuyamcubr_heartbeat(struct tuyamcubr_softc *sc, unsigned int deadline)
{
	sc->sc_deadline += deadline;
	tuyamcubr_send(sc, TUYAMCUBR_CMD_HEARTBEAT);
}

static struct tuyamcubr_dp *
tuyamcubr_find_dp(struct tuyamcubr_softc *sc, uint32_t index, uint8_t type)
{
	struct tuyamcubr_dp *dp;

	if (index > 0xff)
		return (NULL);

	STAILQ_FOREACH(dp, &sc->sc_dps, dp_entry) {
		if (dp->dp_id == index &&
		    dp->dp_type == type)
			return (dp);
	}

	return (NULL);
}

static void
tuyamcubr_cmnd_data(struct tuyamcubr_softc *sc, uint8_t type)
{
	const struct tuyamcubr_data_type *dt = &tuyamcubr_data_types[type];
	struct {
		struct tuyamcubr_data_header h;
		uint8_t value[4]; /* only up to 4 bytes */
	} data;
	size_t len = sizeof(data.h) + dt->t_len;
	struct tuyamcubr_dp *dp;

	dp = tuyamcubr_find_dp(sc, XdrvMailbox.index, type);
	if (dp == NULL) {
		ResponseCmndChar_P(PSTR("Unknown DpId"));
		return;
	}

	if (XdrvMailbox.data_len == 0) {
		ResponseCmndNumber(dp->dp_value);
		return;
	}

	if (XdrvMailbox.payload < 0x00 || XdrvMailbox.payload > dt->t_max) {
		ResponseCmndChar_P(PSTR("Invalid"));
		return;
	}

	dp->dp_value = XdrvMailbox.payload;

	data.h.dpid = dp->dp_id;
	data.h.type = dp->dp_type;
	data.h.len = htons(dt->t_len);
	dt->t_wr(data.value, dp->dp_value);

	tuyamcubr_send(sc, TUYAMCUBR_CMD_SET_DP, &data, len);
	tuyamcubr_rule_dp(sc, dp);

	ResponseCmndNumber(dp->dp_value);

	/* SetOption59 */
	if (Settings->flag3.hass_tele_on_power)
		tuyamcubr_publish_dp(sc, dp);
}

static void
tuyamcubr_cmnd_data_bool(void)
{
	struct tuyamcubr_softc *sc = tuyamcubr_sc;
	struct {
		struct tuyamcubr_data_header h;
		uint8_t value[1];
	} data;
	struct tuyamcubr_dp *dp;
	uint32_t value;

	dp = tuyamcubr_find_dp(sc, XdrvMailbox.index, TUYAMCUBR_DATA_TYPE_BOOL);
	if (dp == NULL) {
		ResponseCmndChar_P(PSTR("Unknown DpId"));
		return;
	}

	if (XdrvMailbox.data_len == 0) {
		ResponseCmndNumber(dp->dp_value);
		return;
	}

	switch (XdrvMailbox.payload) {
	case 0:
	case 1:
		value = XdrvMailbox.payload;
		break;
	case 2:
		value = !dp->dp_value;
		break;
	default:
		ResponseCmndChar_P(PSTR("Invalid"));
		return;
	}

	dp->dp_value = value;

	data.h.dpid = dp->dp_id;
	data.h.type = dp->dp_type;
	data.h.len = htons(sizeof(data.value));
	data.value[0] = value;

	tuyamcubr_send(sc, TUYAMCUBR_CMD_SET_DP, &data, sizeof(data));
	tuyamcubr_rule_dp(sc, dp);

	ResponseCmndNumber(dp->dp_value);

	/* SetOption59 */
	if (Settings->flag3.hass_tele_on_power)
		tuyamcubr_publish_dp(sc, dp);
}

static void
tuyamcubr_cmnd_data_value(void)
{
	tuyamcubr_cmnd_data(tuyamcubr_sc, TUYAMCUBR_DATA_TYPE_VALUE);
}

static void
tuyamcubr_cmnd_data_enum(void)
{
	tuyamcubr_cmnd_data(tuyamcubr_sc, TUYAMCUBR_DATA_TYPE_ENUM);
}

static void
tuyamcubr_rule_dp(struct tuyamcubr_softc *sc, const struct tuyamcubr_dp *dp)
{
	const struct tuyamcubr_data_type *dt =
	    &tuyamcubr_data_types[dp->dp_type];

	/* XXX this only handles numeric types */

	Response_P(PSTR("{\"%s\":{\"%s%u\":%u}}"),
	    D_CMND_TUYAMCUBR_PREFIX,
	    dt->t_name, dp->dp_id,
	    dp->dp_value);
	XdrvRulesProcess(0);
}

static void
tuyamcubr_publish_dp(struct tuyamcubr_softc *sc, const struct tuyamcubr_dp *dp)
{
	const struct tuyamcubr_data_type *dt =
	    &tuyamcubr_data_types[dp->dp_type];
	char topic[64]; /* how long is a (bit of) string? */

	/* XXX this only handles numeric types */

	snprintf(topic, sizeof(topic), PSTR("%s%s%u"),
	    D_CMND_TUYAMCUBR_PREFIX, dt->t_name, dp->dp_id);
	Response_P(PSTR("%u"), dp->dp_value);
	MqttPublishPrefixTopic_P(TELE, topic);
}

static void
tuyamcubr_publish(struct tuyamcubr_softc *sc)
{
	struct tuyamcubr_dp *dp;

	STAILQ_FOREACH(dp, &sc->sc_dps, dp_entry)
		tuyamcubr_publish_dp(sc, dp);
}

static void
tuyamcubr_send_heartbeat(struct tuyamcubr_softc *sc, unsigned int deadline)
{
	sc->sc_deadline += deadline;
	tuyamcubr_send(sc, TUYAMCUBR_CMD_HEARTBEAT);
}

static void
tuyamcubr_recv_heartbeat(struct tuyamcubr_softc *sc, uint8_t v,
    const uint8_t *data, size_t datalen)
{
	/* check the data? */

	switch (sc->sc_state) {
	case TUYAMCUBR_S_START:
		sc->sc_state = TUYAMCUBR_S_PROD_INFO;
		tuyamcubr_send(sc, TUYAMCUBR_CMD_PRODUCT);
		break;
	case TUYAMCUBR_S_RUNNING:
		sc->sc_waiting = 0;
		break;
	default:
		AddLog(LOG_LEVEL_ERROR,
		    TUYAMCUBR_FMT("unexpected heartbeat in state %u"),
		    sc->sc_state);
		break;
	}
}

static void
tuyamcubr_recv_product_info(struct tuyamcubr_softc *sc, uint8_t v,
    const uint8_t *data, size_t datalen)
{
	AddLog(LOG_LEVEL_INFO, TUYAMCUBR_FMT("MCU Product ID: %.*s"),
	    datalen, data);

	switch (sc->sc_state) {
	case TUYAMCUBR_S_PROD_INFO:
		sc->sc_state = TUYAMCUBR_S_MODE;
		tuyamcubr_send(sc, TUYAMCUBR_CMD_MODE);
		break;
	default:
		AddLog(LOG_LEVEL_ERROR,
		    TUYAMCUBR_FMT("unexpected product info in state %u"),
		    sc->sc_state);
		break;
	}
}

static void
tuyamcubr_recv_mode(struct tuyamcubr_softc *sc, uint8_t v,
    const uint8_t *data, size_t datalen)
{
	switch (sc->sc_state) {
	case TUYAMCUBR_S_MODE:
		switch (datalen) {
		case 0:
			AddLog(LOG_LEVEL_INFO,
			    TUYAMCUBR_FMT("MCU Mode: Coordinated"));
			break;
		case 2:
			AddLog(LOG_LEVEL_INFO, TUYAMCUBR_FMT("MCU Mode"
			    ": Status GPIO%u, Reset GPIO%u"),
			    data[0], data[1]);

			sc->sc_state = TUYAMCUBR_S_RUNNING;
			tuyamcubr_send(sc, TUYAMCUBR_CMD_QUERY_STATE);
			return;
		default:
			AddLog(LOG_LEVEL_ERROR, TUYAMCUBR_FMT("MCU Mode"
			    ": unexpected data length %zu"), datalen);
			break;
		}

		sc->sc_state = TUYAMCUBR_S_NET_STATUS;
		tuyamcubr_send(sc, TUYAMCUBR_CMD_WIFI_STATE,
		    &sc->sc_network_status, sizeof(sc->sc_network_status));
		break;
	default:
		AddLog(LOG_LEVEL_ERROR,
		    TUYAMCUBR_FMT("unexpected product info in state %u"),
		    sc->sc_state);
		break;
	}
}

static void
tuyamcubr_recv_net_status(struct tuyamcubr_softc *sc, uint8_t v,
    const uint8_t *data, size_t datalen)
{
	switch (sc->sc_state) {
	case TUYAMCUBR_S_NET_STATUS:
		sc->sc_state = TUYAMCUBR_S_RUNNING;
		tuyamcubr_send(sc, TUYAMCUBR_CMD_QUERY_STATE);
		break;
	default:
		AddLog(LOG_LEVEL_ERROR,
		    TUYAMCUBR_FMT("unexpected product info in state %u"),
		    sc->sc_state);
		break;
	}
}

static void
tuyamcubr_recv_status(struct tuyamcubr_softc *sc, uint8_t v,
    const uint8_t *data, size_t datalen)
{
	const struct tuyamcubr_data_type *dt;
	struct tuyamcubr_dp *dp;
	struct tuyamcubr_data_header h;
	size_t len;
	const uint8_t *b;
	uint32_t value;

	/* take dp status updates at any time */

	do {
		if (datalen < sizeof(h)) {
			AddLog(LOG_LEVEL_ERROR,
			    TUYAMCUBR_FMT("status header short %zu<%zu"),
			    datalen, sizeof(h));
			return;
		}

		memcpy(&h, data, sizeof(h));
		data += sizeof(h);
		datalen -= sizeof(h);

		len = ntohs(h.len);
		if (datalen < len) {
			AddLog(LOG_LEVEL_ERROR,
			    TUYAMCUBR_FMT("status data short %zu<%zu"),
			    datalen, len);
			return;
		}

		b = data;
		data += len;
		datalen -= len;

		dt = tuyamcubr_find_data_type(h.type);
		if (dt == NULL ||
		    dt->t_len == -1) { /* XXX revisit this */
			AddLog(LOG_LEVEL_INFO,
			    TUYAMCUBR_FMT("DpId %u unsupported type 0x%02x"),
			    h.dpid, h.type);
			continue;
		}

		if (len != dt->t_len) {
			AddLog(LOG_LEVEL_ERROR,
			    TUYAMCUBR_FMT("%s%s%u: unexpected len %zu"),
			    D_CMND_TUYAMCUBR_PREFIX, dt->t_name, len);
			continue;
		}

		value = dt->t_rd(b);
		if (value > dt->t_max) {
			AddLog(LOG_LEVEL_ERROR,
			    TUYAMCUBR_FMT("%s%s%u: unexpected value %u>%u"),
			    D_CMND_TUYAMCUBR_PREFIX, dt->t_name, value,
			    dt->t_max);
			continue;
		}

		dp = tuyamcubr_find_dp(sc, h.dpid, h.type);
		if (dp == NULL) {
			dp = (struct tuyamcubr_dp *)malloc(sizeof(*dp));
			if (dp == NULL) {
				AddLog(LOG_LEVEL_ERROR,
				    TUYAMCUBR_FMT("%s%s%u no memory"),
				    D_CMND_TUYAMCUBR_PREFIX,
				    tuyamcubr_data_types[h.type], h.dpid);
				continue;
			}

			dp->dp_id = h.dpid;
			dp->dp_type = h.type;
			STAILQ_INSERT_TAIL(&sc->sc_dps, dp, dp_entry);
		} else if (dp->dp_value == value) {
			/* nop */
			continue;
		}

		dp->dp_value = value;
		tuyamcubr_rule_dp(sc, dp);
		tuyamcubr_publish_dp(sc, dp);
	} while (datalen > 0);
}

static void
tuyamcubr_recv_time(struct tuyamcubr_softc *sc, uint8_t v,
    const uint8_t *data, size_t datalen)
{
	struct tuyamcubr_time tm;
	uint8_t weekday;

	weekday = RtcTime.day_of_week - 1;
	if (weekday == 0)
		weekday = 7;

	/* check datalen? should be 0 */

	tm.valid = 1; /* XXX check whether time is valid */
	tm.year = RtcTime.year % 100;
	tm.month = RtcTime.month;
	tm.day = RtcTime.day_of_month;
	tm.hour = RtcTime.hour;
	tm.minute = RtcTime.minute;
	tm.second = RtcTime.second;
	tm.weekday = weekday;

	tuyamcubr_send(sc, TUYAMCUBR_CMD_TIME, &tm, sizeof(tm));
}

static void
tuyamcubr_tick(struct tuyamcubr_softc *sc, unsigned int ms)
{
	int diff;

	sc->sc_clock += ms;

	if (sc->sc_parser.p_state >= TUYAMCUBR_P_VERSION) {
		/* parser timeout only starts after the header */
		diff = sc->sc_clock - sc->sc_parser.p_deadline;
		if (diff > 0) {
			AddLog(LOG_LEVEL_ERROR,
			    TUYAMCUBR_FMT("recv timeout"));
			sc->sc_parser.p_state = TUYAMCUBR_P_START;
		}
	}

	diff = sc->sc_clock - sc->sc_deadline;
	if (diff < 0) {
		/* deadline hasn't been reached, nothing to do */
		return;
	}

	switch (sc->sc_state) {
	case TUYAMCUBR_S_START:
		tuyamcubr_send_heartbeat(sc, 3000);
		break;
	case TUYAMCUBR_S_RUNNING:
		tuyamcubr_send_heartbeat(sc, 15000);
		if (sc->sc_waiting) {
			AddLog(LOG_LEVEL_ERROR,
			    TUYAMCUBR_FMT("no heartbeat response"));
			/* XXX restart? */
		}
		sc->sc_waiting = 1;
		break;
	}
}

static void
tuyamcubr_every_1sec(struct tuyamcubr_softc *sc)
{
	/* start with the assumption that wifi is configured */
	uint8_t network_status = TUYAMCUBR_NETWORK_STATUS_3;

	if (MqttIsConnected()) {
		/* the device is connected to the "cloud" */
		network_status = TUYAMCUBR_NETWORK_STATUS_5;
	} else {
		switch (WifiState()) {
		case WIFI_MANAGER:
			/* Pairing in AP mode */
			network_status = TUYAMCUBR_NETWORK_STATUS_2;
			break;
		case WIFI_RESTART:
			/* WiFi + router */
			network_status = TUYAMCUBR_NETWORK_STATUS_4;
			break;
		}
	}

	if (sc->sc_network_status != network_status) {
		sc->sc_network_status = network_status;

		if (sc->sc_state == TUYAMCUBR_S_RUNNING) {
			tuyamcubr_send(sc, TUYAMCUBR_CMD_WIFI_STATE,
			    &network_status, sizeof(network_status));
		}
	}
}

static void
tuyamcubr_pre_init(void)
{
	struct tuyamcubr_softc *sc;
	int baudrate;

	/*
	 * SetOption97 - Set Baud rate for TuyaMCU serial communication
	 * (0 = 9600 or 1 = 115200)
	 */
	baudrate = (Settings->flag4.tuyamcu_baudrate) ? 115200 : 9600;

	if (!PinUsed(GPIO_TUYAMCUBR_TX) || !PinUsed(GPIO_TUYAMCUBR_RX))
		return;

	sc = (struct tuyamcubr_softc *)calloc(1, sizeof(*sc));
	if (sc == NULL) {
		AddLog(LOG_LEVEL_ERROR,
		    TUYAMCUBR_FMT("unable to allocate state"));
		return;
	}

	sc->sc_parser.p_state = TUYAMCUBR_P_START;

	sc->sc_state = TUYAMCUBR_S_START;
	sc->sc_clock = 0;
	sc->sc_network_status = (WifiState() == WIFI_MANAGER) ?
	    TUYAMCUBR_NETWORK_STATUS_2 : TUYAMCUBR_NETWORK_STATUS_3;
	STAILQ_INIT(&sc->sc_dps);

	sc->sc_serial = new TasmotaSerial(Pin(GPIO_TUYAMCUBR_RX),
	    Pin(GPIO_TUYAMCUBR_TX), 2);
	if (!sc->sc_serial->begin(baudrate)) {
		AddLog(LOG_LEVEL_ERROR,
		    TUYAMCUBR_FMT("unable to begin serial (baudrate %d)"),
		    baudrate);
		goto del;
	}

	if (sc->sc_serial->hardwareSerial())
		ClaimSerial();

#ifdef ESP32
    AddLog(LOG_LEVEL_DEBUG, PSTR(TUYAMCUBR_LOGNAME ": Serial UART%d"), sc->sc_serial->getUart());
#endif

	/* commit */
	tuyamcubr_sc = sc;

	/* kick the state machine off */
	tuyamcubr_tick(sc, 0);
	return;
del:
	delete sc->sc_serial;
free:
	free(sc);
}

static void
tuyamcubr_loop(struct tuyamcubr_softc *sc)
{
	TasmotaSerial *serial = sc->sc_serial;

	while (serial->available()) {
		yield();
		sc->sc_parser.p_state = tuyamcubr_parse(sc, serial->read());
	}
}

/*
 * Interface
 */

#ifdef USE_WEBSERVER
static void
tuyamcubr_web_sensor(struct tuyamcubr_softc *sc)
{
	struct tuyamcubr_dp *dp;
	const struct tuyamcubr_data_type *dt;

	STAILQ_FOREACH(dp, &sc->sc_dps, dp_entry) {
		dt = &tuyamcubr_data_types[dp->dp_type];

		WSContentSend_PD(PSTR("{s}%s%u{m}%u{e}"),
		    dt->t_name, dp->dp_id, dp->dp_value);
	}
}
#endif // USE_WEBSERVER

static const char tuyamcubr_cmnd_names[] PROGMEM =
	D_CMND_TUYAMCUBR_PREFIX
	"|" D_CMND_TUYAMCUBR_DATA_BOOL
	"|" D_CMND_TUYAMCUBR_DATA_VALUE
	"|" D_CMND_TUYAMCUBR_DATA_ENUM
	;

static void (*const tuyamcubr_cmnds[])(void) PROGMEM = {
	&tuyamcubr_cmnd_data_bool,
	&tuyamcubr_cmnd_data_value,
	&tuyamcubr_cmnd_data_enum,
};

bool
Xdrv65(uint32_t function)
{
	bool result = false;
	struct tuyamcubr_softc *sc;

	switch (function) {
	case FUNC_PRE_INIT:
		tuyamcubr_pre_init();
		return (false);
	}

	sc = tuyamcubr_sc;
	if (sc == NULL)
		return (false);

	switch (function) {
	case FUNC_LOOP:
		tuyamcubr_loop(sc);
		break;

#if 0
	case FUNC_SET_DEVICE_POWER:
		result = tuyamcubr_set_power(sc);
		break;
#endif

	case FUNC_EVERY_100_MSECOND:
		tuyamcubr_tick(sc, 100);
		break;

	case FUNC_EVERY_50_MSECOND:
	case FUNC_EVERY_200_MSECOND:
	case FUNC_EVERY_250_MSECOND:
		break;
	case FUNC_EVERY_SECOND:
		tuyamcubr_every_1sec(sc);
		break;

#if 0
	case FUNC_JSON_APPEND:
		tuyamcubr_sensor(sc);
		break;
#endif
	case FUNC_AFTER_TELEPERIOD:
		tuyamcubr_publish(sc);
		break;
#ifdef USE_WEBSERVER
	case FUNC_WEB_ADD_MAIN_BUTTON:
		break;
	case FUNC_WEB_SENSOR:
		tuyamcubr_web_sensor(sc);
		break;
#endif // USE_WEBSERVER

	case FUNC_COMMAND:
		result = DecodeCommand(tuyamcubr_cmnd_names, tuyamcubr_cmnds);
		break;

    case FUNC_ACTIVE:
        result = true;
        break;
	}

	return (result);
}

#endif	// USE_TUYAMCUBR
