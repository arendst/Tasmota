/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#if MYNEWT_VAL(BLE_MESH)

#define MESH_LOG_MODULE BLE_MESH_PROV_LOG

#if MYNEWT_VAL(BLE_MESH_PROV)

#include <errno.h>

#include "../include/mesh/mesh.h"
#include "mesh_priv.h"

#include "crypto.h"
#include "atomic.h"
#include "adv.h"
#include "net.h"
#include "access.h"
#include "foundation.h"
#include "proxy.h"
#include "prov.h"
#include "testing.h"
#include "settings.h"
#include "nodes.h"

/* 3 transmissions, 20ms interval */
#define PROV_XMIT              BT_MESH_TRANSMIT(2, 20)

#define AUTH_METHOD_NO_OOB     0x00
#define AUTH_METHOD_STATIC     0x01
#define AUTH_METHOD_OUTPUT     0x02
#define AUTH_METHOD_INPUT      0x03

#define OUTPUT_OOB_BLINK       0x00
#define OUTPUT_OOB_BEEP        0x01
#define OUTPUT_OOB_VIBRATE     0x02
#define OUTPUT_OOB_NUMBER      0x03
#define OUTPUT_OOB_STRING      0x04

#define INPUT_OOB_PUSH         0x00
#define INPUT_OOB_TWIST        0x01
#define INPUT_OOB_NUMBER       0x02
#define INPUT_OOB_STRING       0x03

#define PUB_KEY_NO_OOB         0x00
#define PUB_KEY_OOB            0x01

#define PROV_ERR_NONE          0x00
#define PROV_ERR_NVAL_PDU      0x01
#define PROV_ERR_NVAL_FMT      0x02
#define PROV_ERR_UNEXP_PDU     0x03
#define PROV_ERR_CFM_FAILED    0x04
#define PROV_ERR_RESOURCES     0x05
#define PROV_ERR_DECRYPT       0x06
#define PROV_ERR_UNEXP_ERR     0x07
#define PROV_ERR_ADDR          0x08

#define PROV_INVITE            0x00
#define PROV_CAPABILITIES      0x01
#define PROV_START             0x02
#define PROV_PUB_KEY           0x03
#define PROV_INPUT_COMPLETE    0x04
#define PROV_CONFIRM           0x05
#define PROV_RANDOM            0x06
#define PROV_DATA              0x07
#define PROV_COMPLETE          0x08
#define PROV_FAILED            0x09

#define PROV_NO_PDU            0xff

#define PROV_ALG_P256          0x00

#define GPCF(gpc)              (gpc & 0x03)
#define GPC_START(last_seg)    (((last_seg) << 2) | 0x00)
#define GPC_ACK                0x01
#define GPC_CONT(seg_id)       (((seg_id) << 2) | 0x02)
#define GPC_CTL(op)            (((op) << 2) | 0x03)

#define START_PAYLOAD_MAX      20
#define CONT_PAYLOAD_MAX       23

#define START_LAST_SEG(gpc)    (gpc >> 2)
#define CONT_SEG_INDEX(gpc)    (gpc >> 2)

#define BEARER_CTL(gpc)        (gpc >> 2)
#define LINK_OPEN              0x00
#define LINK_ACK               0x01
#define LINK_CLOSE             0x02

#define CLOSE_REASON_SUCCESS   0x00
#define CLOSE_REASON_TIMEOUT   0x01
#define CLOSE_REASON_FAILED    0x02

#define XACT_SEG_DATA(_seg) (&link.rx.buf->om_data[20 + ((_seg - 1) * 23)])
#define XACT_SEG_RECV(_seg) (link.rx.seg &= ~(1 << (_seg)))

#define XACT_NVAL              0xff

enum {
	WAIT_PUB_KEY,          /* Waiting for local PubKey to be generated */
	LINK_ACTIVE,           /* Link has been opened */
	LINK_ACK_RECVD,        /* Ack for link has been received */
	LINK_CLOSING,          /* Link is closing down */
	SEND_PUB_KEY,          /* Waiting to send PubKey */
	WAIT_NUMBER,           /* Waiting for number input from user */
	WAIT_STRING,           /* Waiting for string input from user */
	NOTIFY_INPUT_COMPLETE, /* Notify that input has been completed. */
	LINK_INVALID,          /* Error occurred during provisioning */
	PROVISIONER,           /* The link was opened as provisioner */

	NUM_FLAGS,
};

#if MYNEWT_VAL(BLE_MESH_PROVISIONER)
#define PROVISIONER_LINK 1
#else
#define PROVISIONER_LINK 0
#endif

struct provisioner_link {
	struct bt_mesh_node *node;
	u16_t addr;
	u16_t net_idx;
	u8_t  attention_duration;
};

struct prov_link {
	ATOMIC_DEFINE(flags, NUM_FLAGS);
#if (MYNEWT_VAL(BLE_MESH_PB_GATT))
	uint16_t conn_handle;    /* GATT connection */
#endif
	struct provisioner_link provisioner[PROVISIONER_LINK];

	u8_t  dhkey[32];         /* Calculated DHKey */
	u8_t  expect;            /* Next expected PDU */

	u8_t  oob_method;
	u8_t  oob_action;
	u8_t  oob_size;

	u8_t  conf[16];          /* Remote Confirmation */
	u8_t  rand[16];          /* Local Random */
	u8_t  auth[16];          /* Authentication Value */

	u8_t  conf_salt[16];     /* ConfirmationSalt */
	u8_t  conf_key[16];      /* ConfirmationKey */
	u8_t  conf_inputs[145];  /* ConfirmationInputs */
	u8_t  prov_salt[16];     /* Provisioning Salt */

#if (MYNEWT_VAL(BLE_MESH_PB_ADV))
	u32_t id;                /* Link ID */

	struct {
		u8_t  id;        /* Transaction ID */
		u8_t  prev_id;   /* Previous Transaction ID */
		u8_t  seg;       /* Bit-field of unreceived segments */
		u8_t  last_seg;  /* Last segment (to check length) */
		u8_t  fcs;       /* Expected FCS value */
		struct os_mbuf *buf;
	} rx;

	struct {
		/* Start timestamp of the transaction */
		s64_t start;

		/* Transaction id*/
		u8_t id;

		/* Pending outgoing buffer(s) */
		struct os_mbuf *buf[3];

		/* Retransmit timer */
		struct k_delayed_work retransmit;
	} tx;
#endif

	struct k_delayed_work prot_timer;
};

struct prov_rx {
	u32_t link_id;
	u8_t  xact_id;
	u8_t  gpc;
};

#define RETRANSMIT_TIMEOUT   K_MSEC(500)
#define BUF_TIMEOUT          K_MSEC(400)
#define CLOSING_TIMEOUT      K_SECONDS(3)
#define TRANSACTION_TIMEOUT  K_SECONDS(30)
#define PROTOCOL_TIMEOUT     K_SECONDS(60)

#if (MYNEWT_VAL(BLE_MESH_PB_GATT))
#define PROV_BUF_HEADROOM 5
#else
#define PROV_BUF_HEADROOM 0
static struct os_mbuf *rx_buf;
#endif

#define PROV_BUF(len) NET_BUF_SIMPLE(PROV_BUF_HEADROOM + len)

static struct prov_link link;

static const struct bt_mesh_prov *prov;

static void pub_key_ready(const u8_t *pkey);

static int reset_state(void)
{
	static struct bt_pub_key_cb pub_key_cb = {
		.func = pub_key_ready,
	};
	int err;

	k_delayed_work_cancel(&link.prot_timer);

	/* Disable Attention Timer if it was set */
	if (link.conf_inputs[0]) {
		bt_mesh_attention(NULL, 0);
	}

	if (IS_ENABLED(CONFIG_BT_MESH_PROVISIONER) &&
	    link.provisioner->node != NULL) {
		bt_mesh_node_del(link.provisioner->node, false);
	}

#if (MYNEWT_VAL(BLE_MESH_PB_ADV))
	/* Clear everything except the retransmit and protocol timer
	 * delayed work objects.
	 */
	(void)memset(&link, 0, offsetof(struct prov_link, tx.retransmit));
	link.rx.prev_id = XACT_NVAL;

#if (MYNEWT_VAL(BLE_MESH_PB_GATT))
	link.rx.buf = bt_mesh_proxy_get_buf();
#else
	if (!rx_buf) {
	    rx_buf = NET_BUF_SIMPLE(65);
	}
	net_buf_simple_init(rx_buf, 0);
	link.rx.buf = rx_buf;
#endif /* PB_GATT */

#else /* !PB_ADV */
	/* Clear everything except the protocol timer (k_delayed_work) */
	(void)memset(&link, 0, offsetof(struct prov_link, prot_timer));
#endif /* PB_ADV */

#if (MYNEWT_VAL(BLE_MESH_PB_GATT))
	link.conn_handle = BLE_HS_CONN_HANDLE_NONE;
#endif

	err = bt_pub_key_gen(&pub_key_cb);
	if (err) {
		BT_ERR("Failed to generate public key (%d)", err);
		return err;
	}

	return 0;
}

#if (MYNEWT_VAL(BLE_MESH_PB_ADV))
static void buf_sent(int err, void *user_data)
{
	BT_DBG("buf_sent");

	if (!link.tx.buf[0]) {
		return;
	}

	k_delayed_work_submit(&link.tx.retransmit, RETRANSMIT_TIMEOUT);
}

static struct bt_mesh_send_cb buf_sent_cb = {
	.end = buf_sent,
};

static void free_segments(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(link.tx.buf); i++) {
		struct os_mbuf *buf = link.tx.buf[i];

		if (!buf) {
			break;
		}

		link.tx.buf[i] = NULL;
		/* Mark as canceled */
		BT_MESH_ADV(buf)->busy = 0;
		net_buf_unref(buf);
	}
}

static void prov_clear_tx(void)
{
	BT_DBG("");

	k_delayed_work_cancel(&link.tx.retransmit);

	free_segments();
}

static void reset_adv_link(void)
{
	prov_clear_tx();

	if (prov->link_close) {
		prov->link_close(BT_MESH_PROV_ADV);
	}

	reset_state();
}

static struct os_mbuf *adv_buf_create(void)
{
	struct os_mbuf *buf;

	buf = bt_mesh_adv_create(BT_MESH_ADV_PROV, PROV_XMIT, BUF_TIMEOUT);
	if (!buf) {
		BT_ERR("Out of provisioning buffers");
		assert(0);
		return NULL;
	}

	return buf;
}

static u8_t pending_ack = XACT_NVAL;

static void ack_complete(u16_t duration, int err, void *user_data)
{
	BT_DBG("xact %u complete", (u8_t)pending_ack);
	pending_ack = XACT_NVAL;
}

static void gen_prov_ack_send(u8_t xact_id)
{
	static const struct bt_mesh_send_cb cb = {
		.start = ack_complete,
	};
	const struct bt_mesh_send_cb *complete;
	struct os_mbuf *buf;

	BT_DBG("xact_id %u", xact_id);

	if (pending_ack == xact_id) {
		BT_DBG("Not sending duplicate ack");
		return;
	}

	buf = adv_buf_create();
	if (!buf) {
		return;
	}

	if (pending_ack == XACT_NVAL) {
		pending_ack = xact_id;
		complete = &cb;
	} else {
		complete = NULL;
	}

	net_buf_add_be32(buf, link.id);
	net_buf_add_u8(buf, xact_id);
	net_buf_add_u8(buf, GPC_ACK);

	bt_mesh_adv_send(buf, complete, NULL);
	net_buf_unref(buf);
}

static void send_reliable(void)
{
	int i;

	link.tx.start = k_uptime_get();

	for (i = 0; i < ARRAY_SIZE(link.tx.buf); i++) {
		struct os_mbuf *buf = link.tx.buf[i];

		if (!buf) {
			break;
		}

		if (i + 1 < ARRAY_SIZE(link.tx.buf) && link.tx.buf[i + 1]) {
			bt_mesh_adv_send(buf, NULL, NULL);
		} else {
			bt_mesh_adv_send(buf, &buf_sent_cb, NULL);
		}
	}
}

static int bearer_ctl_send(u8_t op, const void *data, u8_t data_len)
{
	struct os_mbuf *buf;

	BT_DBG("op 0x%02x data_len %u", op, data_len);

	prov_clear_tx();

	buf = adv_buf_create();
	if (!buf) {
		return -ENOBUFS;
	}

	net_buf_add_be32(buf, link.id);
	/* Transaction ID, always 0 for Bearer messages */
	net_buf_add_u8(buf, 0x00);
	net_buf_add_u8(buf, GPC_CTL(op));
	net_buf_add_mem(buf, data, data_len);

	link.tx.buf[0] = buf;
	send_reliable();

	return 0;
}

static u8_t last_seg(u8_t len)
{
	if (len <= START_PAYLOAD_MAX) {
		return 0;
	}

	len -= START_PAYLOAD_MAX;

	return 1 + (len / CONT_PAYLOAD_MAX);
}

static inline u8_t next_transaction_id(void)
{
	if (atomic_test_bit(link.flags, PROVISIONER)) {
		if (link.tx.id != 0x7F) {
			link.tx.id++;
		} else {
			link.tx.id = 0;
		}
	} else {
		if (link.tx.id != 0U && link.tx.id != 0xFF) {
			link.tx.id++;
		} else {
			link.tx.id = 0x80;
		}
	}

	return link.tx.id;
}

static int prov_send_adv(struct os_mbuf *msg)
{
	struct os_mbuf *start, *buf;
	u8_t seg_len, seg_id;
	u8_t xact_id;

	BT_DBG("len %u: %s", msg->om_len, bt_hex(msg->om_data, msg->om_len));

	prov_clear_tx();

	start = adv_buf_create();
	if (!start) {
		return -ENOBUFS;
	}

	xact_id = next_transaction_id();
	net_buf_add_be32(start, link.id);
	net_buf_add_u8(start, xact_id);

	net_buf_add_u8(start, GPC_START(last_seg(msg->om_len)));
	net_buf_add_be16(start, msg->om_len);
	net_buf_add_u8(start, bt_mesh_fcs_calc(msg->om_data, msg->om_len));

	link.tx.buf[0] = start;

	seg_len = min(msg->om_len, START_PAYLOAD_MAX);
	BT_DBG("seg 0 len %u: %s", seg_len, bt_hex(msg->om_data, seg_len));
	net_buf_add_mem(start, msg->om_data, seg_len);
	net_buf_simple_pull(msg, seg_len);

	buf = start;
	for (seg_id = 1; msg->om_len > 0; seg_id++) {
		if (seg_id >= ARRAY_SIZE(link.tx.buf)) {
			BT_ERR("Too big message");
			free_segments();
			return -E2BIG;
		}

		buf = adv_buf_create();
		if (!buf) {
			free_segments();
			return -ENOBUFS;
		}

		link.tx.buf[seg_id] = buf;

		seg_len = min(msg->om_len, CONT_PAYLOAD_MAX);

		BT_DBG("seg_id %u len %u: %s", seg_id, seg_len,
		       bt_hex(msg->om_data, seg_len));

		net_buf_add_be32(buf, link.id);
		net_buf_add_u8(buf, xact_id);
		net_buf_add_u8(buf, GPC_CONT(seg_id));
		net_buf_add_mem(buf, msg->om_data, seg_len);
		net_buf_simple_pull(msg, seg_len);
	}

	send_reliable();

	return 0;
}

#endif /* MYNEWT_VAL(BLE_MESH_PB_ADV) */

#if (MYNEWT_VAL(BLE_MESH_PB_GATT))
static int prov_send_gatt(struct os_mbuf *msg)
{
	if (link.conn_handle == BLE_HS_CONN_HANDLE_NONE) {
		BT_ERR("No connection handle!?");
		return -ENOTCONN;
	}

	return bt_mesh_proxy_send(link.conn_handle, BT_MESH_PROXY_PROV, msg);
}
#endif /* MYNEWT_VAL(BLE_MESH_PB_GATT) */

static inline int prov_send(struct os_mbuf *buf)
{
	k_delayed_work_submit(&link.prot_timer, PROTOCOL_TIMEOUT);

#if (MYNEWT_VAL(BLE_MESH_PB_GATT))
	if (link.conn_handle != BLE_HS_CONN_HANDLE_NONE) {
		return prov_send_gatt(buf);
	}
#endif
#if (MYNEWT_VAL(BLE_MESH_PB_ADV))
	return prov_send_adv(buf);
#else
	return 0;
#endif
}

static void prov_buf_init(struct os_mbuf *buf, u8_t type)
{
	net_buf_simple_init(buf, PROV_BUF_HEADROOM);
	net_buf_simple_add_u8(buf, type);
}

static void prov_send_fail_msg(u8_t err)
{
	struct os_mbuf *buf = PROV_BUF(2);

	prov_buf_init(buf, PROV_FAILED);
	net_buf_simple_add_u8(buf, err);

	if (prov_send(buf)) {
		BT_ERR("Failed to send Provisioning Failed message");
	}

	atomic_set_bit(link.flags, LINK_INVALID);

	os_mbuf_free_chain(buf);
}

static void prov_invite(const u8_t *data)
{
	struct os_mbuf *buf = PROV_BUF(12);

	BT_DBG("Attention Duration: %u seconds", data[0]);

	if (data[0]) {
		bt_mesh_attention(NULL, data[0]);
	}

	link.conf_inputs[0] = data[0];

	prov_buf_init(buf, PROV_CAPABILITIES);

	/* Number of Elements supported */
	net_buf_simple_add_u8(buf, bt_mesh_elem_count());

	/* Supported algorithms - FIPS P-256 Eliptic Curve */
	net_buf_simple_add_be16(buf, BIT(PROV_ALG_P256));

	/* Public Key Type, Only "No OOB" Public Key is supported*/
	net_buf_simple_add_u8(buf, PUB_KEY_NO_OOB);

	/* Static OOB Type */
	net_buf_simple_add_u8(buf, prov->static_val ? BIT(0) : 0x00);

	/* Output OOB Size */
	net_buf_simple_add_u8(buf, prov->output_size);

	/* Output OOB Action */
	net_buf_simple_add_be16(buf, prov->output_actions);

	/* Input OOB Size */
	net_buf_simple_add_u8(buf, prov->input_size);

	/* Input OOB Action */
	net_buf_simple_add_be16(buf, prov->input_actions);

	memcpy(&link.conf_inputs[1], &buf->om_data[1], 11);

	if (prov_send(buf)) {
		BT_ERR("Failed to send capabilities");
		goto done;
	}

	link.expect = PROV_START;

done:
	os_mbuf_free_chain(buf);
}

#if MYNEWT_VAL(BLE_MESH_PB_ADV)
static void send_invite(void)
{
	struct os_mbuf *inv = PROV_BUF(2);

	BT_DBG("");

	prov_buf_init(inv, PROV_INVITE);
	net_buf_simple_add_u8(inv, link.provisioner->attention_duration);

	link.conf_inputs[0] = link.provisioner->attention_duration;

	if (prov_send(inv)) {
		BT_ERR("Failed to send invite");
		goto done;
	}

	link.expect = PROV_CAPABILITIES;

done:
	os_mbuf_free_chain(inv);
}
#endif

static void send_start(void)
{
	struct os_mbuf *start = PROV_BUF(6);

	BT_DBG("");

	prov_buf_init(start, PROV_START);

	net_buf_simple_add_u8(start, PROV_ALG_P256);
	net_buf_simple_add_u8(start, PUB_KEY_NO_OOB);
	net_buf_simple_add_u8(start, AUTH_METHOD_NO_OOB);
	memset(link.auth, 0, sizeof(link.auth));

	net_buf_simple_add_u8(start, 0); /* Auth Action */
	net_buf_simple_add_u8(start, 0); /* Auth Size */

	memcpy(&link.conf_inputs[12], &start->om_data[1], 5);

	if (prov_send(start)) {
		BT_ERR("Failed to send start");
	}

	os_mbuf_free_chain(start);
}

static void prov_capabilities(const u8_t *data)
{
	u16_t algorithms, output_action, input_action;

	if (!IS_ENABLED(CONFIG_BT_MESH_PROVISIONER)) {
		return;
	}

	BT_DBG("Elements: %u", data[0]);

	algorithms = sys_get_be16(&data[1]);
	BT_DBG("Algorithms:        %u", algorithms);

	BT_DBG("Public Key Type:   0x%02x", data[3]);
	BT_DBG("Static OOB Type:   0x%02x", data[4]);
	BT_DBG("Output OOB Size:   %u", data[5]);

	output_action = sys_get_be16(&data[6]);
	BT_DBG("Output OOB Action: 0x%04x", output_action);

	BT_DBG("Input OOB Size:    %u", data[8]);

	input_action = sys_get_be16(&data[9]);
	BT_DBG("Input OOB Action:  0x%04x", input_action);

	if (data[0] == 0) {
		BT_ERR("Invalid number of elements");
		prov_send_fail_msg(PROV_ERR_NVAL_FMT);
		return;
	}

	link.provisioner->node = bt_mesh_node_alloc(link.provisioner->addr,
						    data[0],
						    link.provisioner->net_idx);
	if (link.provisioner->node == NULL) {
		prov_send_fail_msg(PROV_ERR_RESOURCES);
		return;
	}

	memcpy(&link.conf_inputs[1], data, 11);

	atomic_set_bit(link.flags, SEND_PUB_KEY);

	send_start();
}

static bt_mesh_output_action_t output_action(u8_t action)
{
	switch (action) {
	case OUTPUT_OOB_BLINK:
		return BT_MESH_BLINK;
	case OUTPUT_OOB_BEEP:
		return BT_MESH_BEEP;
	case OUTPUT_OOB_VIBRATE:
		return BT_MESH_VIBRATE;
	case OUTPUT_OOB_NUMBER:
		return BT_MESH_DISPLAY_NUMBER;
	case OUTPUT_OOB_STRING:
		return BT_MESH_DISPLAY_STRING;
	default:
		return BT_MESH_NO_OUTPUT;
	}
}

static bt_mesh_input_action_t input_action(u8_t action)
{
	switch (action) {
	case INPUT_OOB_PUSH:
		return BT_MESH_PUSH;
	case INPUT_OOB_TWIST:
		return BT_MESH_TWIST;
	case INPUT_OOB_NUMBER:
		return BT_MESH_ENTER_NUMBER;
	case INPUT_OOB_STRING:
		return BT_MESH_ENTER_STRING;
	default:
		return BT_MESH_NO_INPUT;
	}
}

static int prov_auth(u8_t method, u8_t action, u8_t size)
{
	bt_mesh_output_action_t output;
	bt_mesh_input_action_t input;

	switch (method) {
	case AUTH_METHOD_NO_OOB:
		if (action || size) {
			return -EINVAL;
		}

		memset(link.auth, 0, sizeof(link.auth));
		return 0;
	case AUTH_METHOD_STATIC:
		if (action || size) {
			return -EINVAL;
		}

		memcpy(link.auth + 16 - prov->static_val_len,
		       prov->static_val, prov->static_val_len);
		memset(link.auth, 0, sizeof(link.auth) - prov->static_val_len);
		return 0;

	case AUTH_METHOD_OUTPUT:
		output = output_action(action);
		if (!output) {
			return -EINVAL;
		}

		if (!(prov->output_actions & output)) {
			return -EINVAL;
		}

		if (size > prov->output_size) {
			return -EINVAL;
		}

		atomic_set_bit(link.flags, NOTIFY_INPUT_COMPLETE);

		if (output == BT_MESH_DISPLAY_STRING) {
			unsigned char str[9];
			u8_t i;

			bt_rand(str, size);

			/* Normalize to '0' .. '9' & 'A' .. 'Z' */
			for (i = 0; i < size; i++) {
				str[i] %= 36;
				if (str[i] < 10) {
					str[i] += '0';
				} else {
					str[i] += 'A' - 10;
				}
			}
			str[size] = '\0';

			memcpy(link.auth, str, size);
			memset(link.auth + size, 0, sizeof(link.auth) - size);

			return prov->output_string((char *)str);
		} else {
			u32_t div[8] = { 10, 100, 1000, 10000, 100000,
					    1000000, 10000000, 100000000 };
			u32_t num;

			bt_rand(&num, sizeof(num));
			num %= div[size - 1];

			sys_put_be32(num, &link.auth[12]);
			memset(link.auth, 0, 12);

			return prov->output_number(output, num);
		}

	case AUTH_METHOD_INPUT:
		input = input_action(action);
		if (!input) {
			return -EINVAL;
		}

		if (!(prov->input_actions & input)) {
			return -EINVAL;
		}

		if (size > prov->input_size) {
			return -EINVAL;
		}

		if (input == BT_MESH_ENTER_STRING) {
			atomic_set_bit(link.flags, WAIT_STRING);
		} else {
			atomic_set_bit(link.flags, WAIT_NUMBER);
		}

		return prov->input(input, size);

	default:
		return -EINVAL;
	}
}

static void prov_start(const u8_t *data)
{
	BT_DBG("Algorithm:   0x%02x", data[0]);
	BT_DBG("Public Key:  0x%02x", data[1]);
	BT_DBG("Auth Method: 0x%02x", data[2]);
	BT_DBG("Auth Action: 0x%02x", data[3]);
	BT_DBG("Auth Size:   0x%02x", data[4]);

	if (data[0] != PROV_ALG_P256) {
		BT_ERR("Unknown algorithm 0x%02x", data[0]);
		prov_send_fail_msg(PROV_ERR_NVAL_FMT);
		return;
	}

	if (data[1] != PUB_KEY_NO_OOB) {
		BT_ERR("Invalid public key type: 0x%02x", data[1]);
		prov_send_fail_msg(PROV_ERR_NVAL_FMT);
		return;
	}

	memcpy(&link.conf_inputs[12], data, 5);

	/* TODO: reset link when auth fails? */
	link.expect = PROV_PUB_KEY;

	if (prov_auth(data[2], data[3], data[4]) < 0) {
		BT_ERR("Invalid authentication method: 0x%02x; "
			"action: 0x%02x; size: 0x%02x", data[2], data[3],
			data[4]);
		prov_send_fail_msg(PROV_ERR_NVAL_FMT);
	}
}

static void send_confirm(void)
{
	struct os_mbuf *cfm = PROV_BUF(17);

	BT_DBG("ConfInputs[0]   %s", bt_hex(link.conf_inputs, 64));
	BT_DBG("ConfInputs[64]  %s", bt_hex(&link.conf_inputs[64], 64));
	BT_DBG("ConfInputs[128] %s", bt_hex(&link.conf_inputs[128], 17));

	if (bt_mesh_prov_conf_salt(link.conf_inputs, link.conf_salt)) {
		BT_ERR("Unable to generate confirmation salt");
		prov_send_fail_msg(PROV_ERR_UNEXP_ERR);
		goto done;
	}

	BT_DBG("ConfirmationSalt: %s", bt_hex(link.conf_salt, 16));

	if (bt_mesh_prov_conf_key(link.dhkey, link.conf_salt, link.conf_key)) {
		BT_ERR("Unable to generate confirmation key");
		prov_send_fail_msg(PROV_ERR_UNEXP_ERR);
		goto done;
	}

	BT_DBG("ConfirmationKey: %s", bt_hex(link.conf_key, 16));

	if (bt_rand(link.rand, 16)) {
		BT_ERR("Unable to generate random number");
		prov_send_fail_msg(PROV_ERR_UNEXP_ERR);
		goto done;
	}

	BT_DBG("LocalRandom: %s", bt_hex(link.rand, 16));

	prov_buf_init(cfm, PROV_CONFIRM);

	if (bt_mesh_prov_conf(link.conf_key, link.rand, link.auth,
			      net_buf_simple_add(cfm, 16))) {
		BT_ERR("Unable to generate confirmation value");
		prov_send_fail_msg(PROV_ERR_UNEXP_ERR);
		goto done;
	}

	if (prov_send(cfm)) {
		BT_ERR("Failed to send Provisioning Confirm");
		goto done;
	}

	if (atomic_test_bit(link.flags, PROVISIONER)) {
		link.expect = PROV_CONFIRM;
	} else {
		link.expect = PROV_RANDOM;
	}

done:
	os_mbuf_free_chain(cfm);
}

static void send_input_complete(void)
{
	struct os_mbuf *buf = PROV_BUF(1);

	prov_buf_init(buf, PROV_INPUT_COMPLETE);
	if (prov_send(buf)) {
		BT_ERR("Failed to send Provisioning Input Complete");
	}
	link.expect = PROV_CONFIRM;

	os_mbuf_free_chain(buf);
}

int bt_mesh_input_number(u32_t num)
{
	BT_DBG("%u", (unsigned) num);

	if (!atomic_test_and_clear_bit(link.flags, WAIT_NUMBER)) {
		return -EINVAL;
	}

	sys_put_be32(num, &link.auth[12]);

	send_input_complete();

	return 0;
}

int bt_mesh_input_string(const char *str)
{
	BT_DBG("%s", str);

	if (!atomic_test_and_clear_bit(link.flags, WAIT_STRING)) {
		return -EINVAL;
	}

	strncpy((char *)link.auth, str, prov->input_size);

	send_input_complete();

	return 0;
}

static void send_pub_key(void)
{
	struct os_mbuf *buf = PROV_BUF(65);
	const u8_t *key;

	key = bt_pub_key_get();
	if (!key) {
		BT_ERR("No public key available");
		prov_send_fail_msg(PROV_ERR_UNEXP_ERR);
		goto done;
	}

	BT_DBG("Local Public Key: %s", bt_hex(key, 64));

	prov_buf_init(buf, PROV_PUB_KEY);

	/* Swap X and Y halves independently to big-endian */
	sys_memcpy_swap(net_buf_simple_add(buf, 32), key, 32);
	sys_memcpy_swap(net_buf_simple_add(buf, 32), &key[32], 32);

	if (atomic_test_bit(link.flags, PROVISIONER)) {
		/* PublicKeyProvisioner */
		memcpy(&link.conf_inputs[17], &buf->om_data[1], 64);
	} else {
		/* PublicKeyRemote */
		memcpy(&link.conf_inputs[81], &buf->om_data[1], 64);
	}

	if (prov_send(buf)) {
		BT_ERR("Failed to send Public Key");
		goto done;
	}

	if (atomic_test_bit(link.flags, PROVISIONER)) {
		link.expect = PROV_PUB_KEY;
	} else {
		if (atomic_test_bit(link.flags, WAIT_NUMBER) ||
		    atomic_test_bit(link.flags, WAIT_STRING)) {
			link.expect = PROV_NO_PDU; /* Wait for input */
		} else {
			link.expect = PROV_CONFIRM;
		}
	}

done:
	os_mbuf_free_chain(buf);
}

static void prov_dh_key_cb(const u8_t dhkey[32])
{
	BT_DBG("%p", dhkey);

	if (!dhkey) {
		BT_ERR("DHKey generation failed");
		prov_send_fail_msg(PROV_ERR_UNEXP_ERR);
		return;
	}

	sys_memcpy_swap(link.dhkey, dhkey, 32);

	BT_DBG("DHkey: %s", bt_hex(link.dhkey, 32));

	if (atomic_test_bit(link.flags, PROVISIONER)) {
		send_confirm();
	} else {
		send_pub_key();
	}
}

static void prov_dh_key_gen(void)
{
	u8_t remote_pk_le[64], *remote_pk;

	if (atomic_test_bit(link.flags, PROVISIONER)) {
		remote_pk = &link.conf_inputs[81];
	} else {
		remote_pk = &link.conf_inputs[17];
	}

	/* Copy remote key in little-endian for bt_dh_key_gen().
	 * X and Y halves are swapped independently. The bt_dh_key_gen()
	 * will also take care of validating the remote public key.
	 */
	sys_memcpy_swap(remote_pk_le, remote_pk, 32);
	sys_memcpy_swap(&remote_pk_le[32], &remote_pk[32], 32);

	if (bt_dh_key_gen(remote_pk_le, prov_dh_key_cb)) {
		BT_ERR("Failed to generate DHKey");
		prov_send_fail_msg(PROV_ERR_UNEXP_ERR);
	}
}

static void prov_pub_key(const u8_t *data)
{
	BT_DBG("Remote Public Key: %s", bt_hex(data, 64));

	if (atomic_test_bit(link.flags, PROVISIONER)) {
		/* PublicKeyDevice */
		memcpy(&link.conf_inputs[81], data, 64);

#if (MYNEWT_VAL(BLE_MESH_PB_ADV))
		prov_clear_tx();
#endif
	} else {
		/* PublicKeyProvisioner */
		memcpy(&link.conf_inputs[17], data, 64);

		if (!bt_pub_key_get()) {
			/* Clear retransmit timer */
#if (MYNEWT_VAL(BLE_MESH_PB_ADV))
			prov_clear_tx();
#endif

			atomic_set_bit(link.flags, WAIT_PUB_KEY);
			BT_WARN("Waiting for local public key");
			return;
		}
	}

	prov_dh_key_gen();
}

static void pub_key_ready(const u8_t *pkey)
{
	if (!pkey) {
		BT_WARN("Public key not available");
		return;
	}

	BT_DBG("Local public key ready");

	if (atomic_test_and_clear_bit(link.flags, WAIT_PUB_KEY)) {
		if (atomic_test_bit(link.flags, PROVISIONER)) {
			send_pub_key();
		} else {
			prov_dh_key_gen();
		}
	}
}

static void notify_input_complete(void)
{
	if (atomic_test_and_clear_bit(link.flags, NOTIFY_INPUT_COMPLETE) &&
	    prov->input_complete) {
		prov->input_complete();
	}
}

static void prov_input_complete(const u8_t *data)
{
	BT_DBG("");
	notify_input_complete();
}

static void send_prov_data(void)
{
	struct os_mbuf *pdu = PROV_BUF(34);
	struct bt_mesh_subnet *sub;
	u8_t session_key[16];
	u8_t nonce[13];
	int err;

	err = bt_mesh_session_key(link.dhkey, link.prov_salt, session_key);
	if (err) {
		BT_ERR("Unable to generate session key");
		prov_send_fail_msg(PROV_ERR_UNEXP_ERR);
		goto done;
	}

	BT_DBG("SessionKey: %s", bt_hex(session_key, 16));

	err = bt_mesh_prov_nonce(link.dhkey, link.prov_salt, nonce);
	if (err) {
		BT_ERR("Unable to generate session nonce");
		prov_send_fail_msg(PROV_ERR_UNEXP_ERR);
		goto done;
	}

	BT_DBG("Nonce: %s", bt_hex(nonce, 13));

	err = bt_mesh_dev_key(link.dhkey, link.prov_salt,
			      link.provisioner->node->dev_key);
	if (err) {
		BT_ERR("Unable to generate device key");
		prov_send_fail_msg(PROV_ERR_UNEXP_ERR);
		goto done;
	}

	BT_DBG("DevKey: %s", bt_hex(link.provisioner->node->dev_key, 16));

	sub = bt_mesh_subnet_get(link.provisioner->node->net_idx);
	if (sub == NULL) {
		BT_ERR("No subnet with net_idx %u",
		       link.provisioner->node->net_idx);
		prov_send_fail_msg(PROV_ERR_UNEXP_ERR);
		goto done;
	}

	prov_buf_init(pdu, PROV_DATA);
	net_buf_simple_add_mem(pdu, sub->keys[sub->kr_flag].net, 16);
	net_buf_simple_add_be16(pdu, link.provisioner->node->net_idx);
	net_buf_simple_add_u8(pdu, bt_mesh_net_flags(sub));
	net_buf_simple_add_be32(pdu, bt_mesh.iv_index);
	net_buf_simple_add_be16(pdu, link.provisioner->node->addr);
	net_buf_simple_add(pdu, 8); /* For MIC */

	BT_DBG("net_idx %u, iv_index 0x%08x, addr 0x%04x",
	       link.provisioner->node->net_idx, bt_mesh.iv_index,
	       link.provisioner->node->addr);

	err = bt_mesh_prov_encrypt(session_key, nonce, &pdu->om_data[1],
				   &pdu->om_data[1]);
	if (err) {
		BT_ERR("Unable to encrypt provisioning data");
		prov_send_fail_msg(PROV_ERR_DECRYPT);
		goto done;
	}

	if (prov_send(pdu)) {
		BT_ERR("Failed to send Provisioning Data");
		goto done;
	}

	link.expect = PROV_COMPLETE;

done:
	os_mbuf_free_chain(pdu);
}

static void prov_complete(const u8_t *data)
{
	if (!IS_ENABLED(CONFIG_BT_MESH_PROVISIONER)) {
		return;
	}

	struct bt_mesh_node *node = link.provisioner->node;
#if MYNEWT_VAL(BLE_MESH_PB_ADV)
	u8_t reason = CLOSE_REASON_SUCCESS;
#endif

	BT_DBG("key %s, net_idx %u, num_elem %u, addr 0x%04x",
	       bt_hex(node->dev_key, 16), node->net_idx, node->num_elem,
	       node->addr);

	if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
		bt_mesh_store_node(node);
	}

	link.provisioner->node = NULL;
	link.expect = PROV_NO_PDU;
	atomic_set_bit(link.flags, LINK_CLOSING);

#if MYNEWT_VAL(BLE_MESH_PB_ADV)
	bearer_ctl_send(LINK_CLOSE, &reason, sizeof(reason));
#endif

	bt_mesh_prov_node_added(node->net_idx, node->addr, node->num_elem);

	/*
	 * According to mesh profile spec (5.3.1.4.3), the close message should
	 * be restransmitted at least three times. Retransmit the LINK_CLOSE
	 * message until CLOSING_TIMEOUT has elapsed instead of resetting the
	 * link here.
	 */
}

static void send_random(void)
{
	struct os_mbuf *rnd = PROV_BUF(17);

	prov_buf_init(rnd, PROV_RANDOM);
	net_buf_simple_add_mem(rnd, link.rand, 16);

	if (prov_send(rnd)) {
		BT_ERR("Failed to send Provisioning Random");
		goto done;
	}

	if (atomic_test_bit(link.flags, PROVISIONER)) {
		link.expect = PROV_RANDOM;
	} else {
		link.expect = PROV_DATA;
	}

done:
	os_mbuf_free_chain(rnd);
}

static void prov_random(const u8_t *data)
{
	u8_t conf_verify[16];
	const u8_t *prov_rand, *dev_rand;

	BT_DBG("Remote Random: %s", bt_hex(data, 16));

	if (bt_mesh_prov_conf(link.conf_key, data, link.auth, conf_verify)) {
		BT_ERR("Unable to calculate confirmation verification");
		prov_send_fail_msg(PROV_ERR_UNEXP_ERR);
		return;
	}

	if (memcmp(conf_verify, link.conf, 16)) {
		BT_ERR("Invalid confirmation value");
		BT_DBG("Received:   %s", bt_hex(link.conf, 16));
		BT_DBG("Calculated: %s",  bt_hex(conf_verify, 16));
		prov_send_fail_msg(PROV_ERR_CFM_FAILED);
		return;
	}

	if (atomic_test_bit(link.flags, PROVISIONER)) {
		prov_rand = link.rand;
		dev_rand = data;
	} else {
		prov_rand = data;
		dev_rand = link.rand;
	}

	if (bt_mesh_prov_salt(link.conf_salt, prov_rand, dev_rand,
			      link.prov_salt)) {
		BT_ERR("Failed to generate provisioning salt");
		prov_send_fail_msg(PROV_ERR_UNEXP_ERR);
		return;
	}

	BT_DBG("ProvisioningSalt: %s", bt_hex(link.prov_salt, 16));

	if (IS_ENABLED(CONFIG_BT_MESH_PROVISIONER) &&
	    atomic_test_bit(link.flags, PROVISIONER)) {
		send_prov_data();
	} else {
		send_random();
	}
}

static void prov_confirm(const u8_t *data)
{
	BT_DBG("Remote Confirm: %s", bt_hex(data, 16));

	memcpy(link.conf, data, 16);

	notify_input_complete();

	if (atomic_test_bit(link.flags, PROVISIONER)) {
		send_random();
	} else {
		send_confirm();
	}
}

static inline bool is_pb_gatt(void)
{
#if MYNEWT_VAL(BLE_MESH_PB_GATT)
	return (link.conn_handle != BLE_HS_CONN_HANDLE_NONE);
#else
	return false;
#endif
}

static void prov_data(const u8_t *data)
{
	struct os_mbuf *msg = PROV_BUF(1);
	u8_t session_key[16];
	u8_t nonce[13];
	u8_t dev_key[16];
	u8_t pdu[25];
	u8_t flags;
	u32_t iv_index;
	u16_t addr;
	u16_t net_idx;
	int err;
	bool identity_enable;

	BT_DBG("");

	err = bt_mesh_session_key(link.dhkey, link.prov_salt, session_key);
	if (err) {
		BT_ERR("Unable to generate session key");
		prov_send_fail_msg(PROV_ERR_UNEXP_ERR);
		goto done;
	}

	BT_DBG("SessionKey: %s", bt_hex(session_key, 16));

	err = bt_mesh_prov_nonce(link.dhkey, link.prov_salt, nonce);
	if (err) {
		BT_ERR("Unable to generate session nonce");
		prov_send_fail_msg(PROV_ERR_UNEXP_ERR);
		goto done;
	}

	BT_DBG("Nonce: %s", bt_hex(nonce, 13));

	err = bt_mesh_prov_decrypt(session_key, nonce, data, pdu);
	if (err) {
		BT_ERR("Unable to decrypt provisioning data");
		prov_send_fail_msg(PROV_ERR_DECRYPT);
		goto done;
	}

	err = bt_mesh_dev_key(link.dhkey, link.prov_salt, dev_key);
	if (err) {
		BT_ERR("Unable to generate device key");
		prov_send_fail_msg(PROV_ERR_UNEXP_ERR);
		goto done;
	}

	BT_DBG("DevKey: %s", bt_hex(dev_key, 16));

	net_idx = sys_get_be16(&pdu[16]);
	flags = pdu[18];
	iv_index = sys_get_be32(&pdu[19]);
	addr = sys_get_be16(&pdu[23]);

	BT_DBG("net_idx %u iv_index 0x%08x, addr 0x%04x",
	       net_idx, (unsigned) iv_index, addr);

	prov_buf_init(msg, PROV_COMPLETE);
	if (prov_send(msg)) {
		BT_ERR("Failed to send Provisioning Complete");
		goto done;
	}

	/* Ignore any further PDUs on this link */
	link.expect = PROV_NO_PDU;

	/* Store info, since bt_mesh_provision() will end up clearing it */
	if (IS_ENABLED(CONFIG_BT_MESH_GATT_PROXY)) {
		identity_enable = is_pb_gatt();
	} else {
		identity_enable = false;
	}

	err = bt_mesh_provision(pdu, net_idx, flags, iv_index, addr, dev_key);
	if (err) {
		BT_ERR("Failed to provision (err %d)", err);
		goto done;
	}

	/* After PB-GATT provisioning we should start advertising
	 * using Node Identity.
	 */
	if (IS_ENABLED(CONFIG_BT_MESH_GATT_PROXY) && identity_enable) {
		bt_mesh_proxy_identity_enable();
	}

done:
	os_mbuf_free_chain(msg);
}

static void prov_failed(const u8_t *data)
{
	BT_WARN("Error: 0x%02x", data[0]);
}

static const struct {
	void (*func)(const u8_t *data);
	u16_t len;
} prov_handlers[] = {
	{ prov_invite, 1 },
	{ prov_capabilities, 11 },
	{ prov_start, 5, },
	{ prov_pub_key, 64 },
	{ prov_input_complete, 0 },
	{ prov_confirm, 16 },
	{ prov_random, 16 },
	{ prov_data, 33 },
	{ prov_complete, 0 },
	{ prov_failed, 1 },
};

#if (MYNEWT_VAL(BLE_MESH_PB_ADV))
static void prov_retransmit(struct ble_npl_event *work)
{
	int i, timeout;

	BT_DBG("");

	if (!atomic_test_bit(link.flags, LINK_ACTIVE)) {
		BT_WARN("Link not active");
		return;
	}

	if (atomic_test_bit(link.flags, LINK_CLOSING)) {
		timeout = CLOSING_TIMEOUT;
	} else {
		timeout = TRANSACTION_TIMEOUT;
	}

	if (k_uptime_get() - link.tx.start > timeout) {
		BT_WARN("Giving up transaction");
		reset_adv_link();
		return;
	}

	for (i = 0; i < ARRAY_SIZE(link.tx.buf); i++) {
		struct os_mbuf *buf = link.tx.buf[i];

		if (!buf) {
			break;
		}

		if (BT_MESH_ADV(buf)->busy) {
			continue;
		}

		BT_DBG("%u bytes: %s", buf->om_len, bt_hex(buf->om_data, buf->om_len));

		if (i + 1 < ARRAY_SIZE(link.tx.buf) && link.tx.buf[i + 1]) {
			bt_mesh_adv_send(buf, NULL, NULL);
		} else {
			bt_mesh_adv_send(buf, &buf_sent_cb, NULL);
		}

	}
}

static void link_open(struct prov_rx *rx, struct os_mbuf *buf)
{
	BT_DBG("link open: len %u", buf->om_len);

	if (buf->om_len < 16) {
		BT_ERR("Too short bearer open message (len %u)", buf->om_len);
		return;
	}

	if (atomic_test_bit(link.flags, LINK_ACTIVE)) {
		/* Send another link ack if the provisioner missed the last */
		if (link.id == rx->link_id && link.expect == PROV_INVITE) {
			BT_DBG("Resending link ack");
			bearer_ctl_send(LINK_ACK, NULL, 0);
		} else {
			BT_WARN("Ignoring bearer open: link already active");
		}

		return;
	}

	if (memcmp(buf->om_data, prov->uuid, 16)) {
		BT_DBG("Bearer open message not for us");
		return;
	}

	if (prov->link_open) {
		prov->link_open(BT_MESH_PROV_ADV);
	}

	link.id = rx->link_id;
	atomic_set_bit(link.flags, LINK_ACTIVE);
	net_buf_simple_init(link.rx.buf, 0);

	bearer_ctl_send(LINK_ACK, NULL, 0);

	link.expect = PROV_INVITE;
}

static void link_ack(struct prov_rx *rx, struct os_mbuf *buf)
{
	BT_DBG("Link ack: len %u", buf->om_len);

	if (IS_ENABLED(CONFIG_BT_MESH_PROVISIONER) &&
	    atomic_test_bit(link.flags, PROVISIONER)) {
		if (atomic_test_and_set_bit(link.flags, LINK_ACK_RECVD)) {
			return;
		}

		prov_clear_tx();

		if (prov->link_open) {
			prov->link_open(BT_MESH_PROV_ADV);
		}

		send_invite();
	}
}

static void link_close(struct prov_rx *rx, struct os_mbuf *buf)
{
	BT_DBG("Link close: len %u", buf->om_len);

	reset_adv_link();
}

static void gen_prov_ctl(struct prov_rx *rx, struct os_mbuf *buf)
{
	BT_DBG("op 0x%02x len %u", BEARER_CTL(rx->gpc), buf->om_len);

	switch (BEARER_CTL(rx->gpc)) {
	case LINK_OPEN:
		link_open(rx, buf);
		break;
	case LINK_ACK:
		if (!atomic_test_bit(link.flags, LINK_ACTIVE)) {
			return;
		}

		link_ack(rx, buf);
		break;
	case LINK_CLOSE:
		if (!atomic_test_bit(link.flags, LINK_ACTIVE)) {
			return;
		}

		link_close(rx, buf);
		break;
	default:
		BT_ERR("Unknown bearer opcode: 0x%02x", BEARER_CTL(rx->gpc));

		if (IS_ENABLED(CONFIG_BT_TESTING)) {
			bt_test_mesh_prov_invalid_bearer(BEARER_CTL(rx->gpc));
		}

		return;
	}
}

static void prov_msg_recv(void)
{
	u8_t type = link.rx.buf->om_data[0];

	BT_DBG("type 0x%02x len %u", type, link.rx.buf->om_len);

	k_delayed_work_submit(&link.prot_timer, PROTOCOL_TIMEOUT);

	if (!bt_mesh_fcs_check(link.rx.buf, link.rx.fcs)) {
		BT_ERR("Incorrect FCS");
		return;
	}

	gen_prov_ack_send(link.rx.id);
	link.rx.prev_id = link.rx.id;
	link.rx.id = 0;

	if (atomic_test_bit(link.flags, LINK_INVALID)) {
		BT_WARN("Unexpected msg 0x%02x on invalidated link", type);
		prov_send_fail_msg(PROV_ERR_UNEXP_PDU);
		return;
	}

	if (type != PROV_FAILED && type != link.expect) {
		BT_WARN("Unexpected msg 0x%02x != 0x%02x", type, link.expect);
		prov_send_fail_msg(PROV_ERR_UNEXP_PDU);
		return;
	}

	if (type >= ARRAY_SIZE(prov_handlers)) {
		BT_ERR("Unknown provisioning PDU type 0x%02x", type);
		prov_send_fail_msg(PROV_ERR_NVAL_PDU);
		return;
	}

	if (1 + prov_handlers[type].len != link.rx.buf->om_len) {
		BT_ERR("Invalid length %u for type 0x%02x",
		       link.rx.buf->om_len, type);
		prov_send_fail_msg(PROV_ERR_NVAL_FMT);
		return;
	}

	prov_handlers[type].func(&link.rx.buf->om_data[1]);
}

static void gen_prov_cont(struct prov_rx *rx, struct os_mbuf *buf)
{
	u8_t seg = CONT_SEG_INDEX(rx->gpc);

	BT_DBG("len %u, seg_index %u", buf->om_len, seg);

	if (!link.rx.seg && link.rx.prev_id == rx->xact_id) {
		BT_WARN("Resending ack");
		gen_prov_ack_send(rx->xact_id);
		return;
	}

	if (rx->xact_id != link.rx.id) {
		BT_WARN("Data for unknown transaction (%u != %u)",
			rx->xact_id, link.rx.id);
		return;
	}

	if (seg > link.rx.last_seg) {
		BT_ERR("Invalid segment index %u", seg);
		prov_send_fail_msg(PROV_ERR_NVAL_FMT);
		return;
	} else if (seg == link.rx.last_seg) {
		u8_t expect_len;

		expect_len = (link.rx.buf->om_len - 20 -
			      ((link.rx.last_seg - 1) * 23));
		if (expect_len != buf->om_len) {
			BT_ERR("Incorrect last seg len: %u != %u",
			       expect_len, buf->om_len);
			prov_send_fail_msg(PROV_ERR_NVAL_FMT);
			return;
		}
	}

	if (!(link.rx.seg & BIT(seg))) {
		BT_WARN("Ignoring already received segment");
		return;
	}

	memcpy(XACT_SEG_DATA(seg), buf->om_data, buf->om_len);
	XACT_SEG_RECV(seg);

	if (!link.rx.seg) {
		prov_msg_recv();
	}
}

static void gen_prov_ack(struct prov_rx *rx, struct os_mbuf *buf)
{
	BT_DBG("len %u", buf->om_len);

	if (!link.tx.buf[0]) {
		return;
	}

	if (rx->xact_id == link.tx.id) {
		/* Don't clear resending of LINK_CLOSE messages */
		if (!atomic_test_bit(link.flags, LINK_CLOSING)) {
			prov_clear_tx();
		}

		/* Send the PubKey when the the Start message is ACK'ed */
		if (IS_ENABLED(CONFIG_BT_MESH_PROVISIONER) &&
		    atomic_test_and_clear_bit(link.flags, SEND_PUB_KEY)) {
			if (!bt_pub_key_get()) {
				atomic_set_bit(link.flags, WAIT_PUB_KEY);
				BT_WARN("Waiting for local public key");
			} else {
				send_pub_key();
			}
		}
	}
}

static void gen_prov_start(struct prov_rx *rx, struct os_mbuf *buf)
{
	u16_t trailing_space = 0;

	if (link.rx.seg) {
		BT_WARN("Got Start while there are unreceived segments");
		return;
	}

	if (link.rx.prev_id == rx->xact_id) {
		BT_WARN("Resending ack");
		gen_prov_ack_send(rx->xact_id);
		return;
	}

	trailing_space = OS_MBUF_TRAILINGSPACE(link.rx.buf);

	link.rx.buf->om_len = net_buf_simple_pull_be16(buf);
	link.rx.id  = rx->xact_id;
	link.rx.fcs = net_buf_simple_pull_u8(buf);

	BT_DBG("len %u last_seg %u total_len %u fcs 0x%02x", buf->om_len,
	       START_LAST_SEG(rx->gpc), link.rx.buf->om_len, link.rx.fcs);

	if (link.rx.buf->om_len < 1) {
		BT_ERR("Ignoring zero-length provisioning PDU");
		prov_send_fail_msg(PROV_ERR_NVAL_FMT);
		return;
	}

	if (link.rx.buf->om_len > trailing_space) {
		BT_ERR("Too large provisioning PDU (%u bytes)",
		       link.rx.buf->om_len);
		prov_send_fail_msg(PROV_ERR_NVAL_FMT);
		return;
	}

	if (START_LAST_SEG(rx->gpc) > 0 && link.rx.buf->om_len <= 20) {
		BT_ERR("Too small total length for multi-segment PDU");
		prov_send_fail_msg(PROV_ERR_NVAL_FMT);
		return;
	}

	link.rx.seg = (1 << (START_LAST_SEG(rx->gpc) + 1)) - 1;
	link.rx.last_seg = START_LAST_SEG(rx->gpc);
	memcpy(link.rx.buf->om_data, buf->om_data, buf->om_len);
	XACT_SEG_RECV(0);

	if (!link.rx.seg) {
		prov_msg_recv();
	}
}

static const struct {
	void (*func)(struct prov_rx *rx, struct os_mbuf *buf);
	bool require_link;
	u8_t min_len;
} gen_prov[] = {
	{ gen_prov_start, true, 3 },
	{ gen_prov_ack, true, 0 },
	{ gen_prov_cont, true, 0 },
	{ gen_prov_ctl, false, 0 },
};

static void gen_prov_recv(struct prov_rx *rx, struct os_mbuf *buf)
{
	if (buf->om_len < gen_prov[GPCF(rx->gpc)].min_len) {
		BT_ERR("Too short GPC message type %u", GPCF(rx->gpc));
		return;
	}

	if (!atomic_test_bit(link.flags, LINK_ACTIVE) &&
	    gen_prov[GPCF(rx->gpc)].require_link) {
		BT_DBG("Ignoring message that requires active link");
		return;
	}

	BT_DBG("prov_action: %d", GPCF(rx->gpc));
	gen_prov[GPCF(rx->gpc)].func(rx, buf);
}

void bt_mesh_pb_adv_recv(struct os_mbuf *buf)
{
	struct prov_rx rx;

	if (!bt_prov_active() && bt_mesh_is_provisioned()) {
		BT_DBG("Ignoring provisioning PDU - already provisioned");
		return;
	}

	if (buf->om_len < 6) {
		BT_WARN("Too short provisioning packet (len %u)", buf->om_len);
		return;
	}

	rx.link_id = net_buf_simple_pull_be32(buf);
	rx.xact_id = net_buf_simple_pull_u8(buf);
	rx.gpc = net_buf_simple_pull_u8(buf);

	BT_DBG("link_id 0x%08x xact_id %u", (unsigned) rx.link_id, rx.xact_id);

	if (atomic_test_bit(link.flags, LINK_ACTIVE) && link.id != rx.link_id) {
		BT_DBG("Ignoring mesh beacon for unknown link");
		return;
	}

	gen_prov_recv(&rx, buf);
}

int bt_mesh_pb_adv_open(const u8_t uuid[16], u16_t net_idx, u16_t addr,
			u8_t attention_duration)
{
	BT_DBG("uuid %s", bt_hex(uuid, 16));

	if (atomic_test_and_set_bit(link.flags, LINK_ACTIVE)) {
		return -EBUSY;
	}

	atomic_set_bit(link.flags, PROVISIONER);

	bt_rand(&link.id, sizeof(link.id));
	link.tx.id = 0x7F;
	link.provisioner->addr = addr;
	link.provisioner->net_idx = net_idx;
	link.provisioner->attention_duration = attention_duration;

	net_buf_simple_init(link.rx.buf, 0);

	bearer_ctl_send(LINK_OPEN, uuid, 16);

	return 0;
}

#endif /* MYNEWT_VAL(BLE_MESH_PB_ADV) */

#if (MYNEWT_VAL(BLE_MESH_PB_GATT))
int bt_mesh_pb_gatt_recv(uint16_t conn_handle, struct os_mbuf *buf)
{
	u8_t type;

	BT_DBG("%u bytes: %s", buf->om_len, bt_hex(buf->om_data, buf->om_len));

	if (link.conn_handle != conn_handle) {
		BT_WARN("Data for unexpected connection");
		return -ENOTCONN;
	}

	if (buf->om_len < 1) {
		BT_WARN("Too short provisioning packet (len %u)", buf->om_len);
		return -EINVAL;
	}

	type = net_buf_simple_pull_u8(buf);
	if (type != PROV_FAILED && type != link.expect) {
		BT_WARN("Unexpected msg 0x%02x != 0x%02x", type, link.expect);
		prov_send_fail_msg(PROV_ERR_UNEXP_PDU);
		return -EINVAL;
	}

	if (type >= ARRAY_SIZE(prov_handlers)) {
		BT_ERR("Unknown provisioning PDU type 0x%02x", type);
		return -EINVAL;
	}

	if (prov_handlers[type].len != buf->om_len) {
		BT_ERR("Invalid length %u for type 0x%02x", buf->om_len, type);
		return -EINVAL;
	}

	prov_handlers[type].func(buf->om_data);

	return 0;
}

int bt_mesh_pb_gatt_open(uint16_t conn_handle)
{
	BT_DBG("conn_handle %d", conn_handle);

	if (atomic_test_and_set_bit(link.flags, LINK_ACTIVE)) {
		BT_ERR("Link already opened?");
		return -EBUSY;
	}

	link.conn_handle = conn_handle;
	link.expect = PROV_INVITE;

	if (prov->link_open) {
		prov->link_open(BT_MESH_PROV_GATT);
	}

	return 0;
}

int bt_mesh_pb_gatt_close(uint16_t conn_handle)
{
	BT_DBG("conn_handle %d", conn_handle);

	if (link.conn_handle != conn_handle) {
		BT_ERR("Not connected");
		return -ENOTCONN;
	}

	if (prov->link_close) {
		prov->link_close(BT_MESH_PROV_GATT);
	}

	return reset_state();
}
#endif /* MYNEWT_VAL(BLE_MESH_PB_GATT) */

const struct bt_mesh_prov *bt_mesh_prov_get(void)
{
	return prov;
}

bool bt_prov_active(void)
{
	return atomic_test_bit(link.flags, LINK_ACTIVE);
}

static void protocol_timeout(struct ble_npl_event *work)
{
	BT_DBG("Protocol timeout");

#if MYNEWT_VAL(BLE_MESH_PB_GATT)
	if (link.conn_handle != BLE_HS_CONN_HANDLE_NONE) {
		bt_mesh_pb_gatt_close(link.conn_handle);
		return;
	}
#endif

#if MYNEWT_VAL(BLE_MESH_PB_ADV)
	u8_t reason = CLOSE_REASON_TIMEOUT;

	link.rx.seg = 0U;
	bearer_ctl_send(LINK_CLOSE, &reason, sizeof(reason));

	reset_state();
#endif
}

int bt_mesh_prov_init(const struct bt_mesh_prov *prov_info)
{
	if (!prov_info) {
		BT_ERR("No provisioning context provided");
		return -EINVAL;
	}

	k_delayed_work_init(&link.prot_timer, protocol_timeout);

	prov = prov_info;

#if MYNEWT_VAL(BLE_MESH_PB_ADV)
	k_delayed_work_init(&link.tx.retransmit, prov_retransmit);
#endif

	return reset_state();
}

void bt_mesh_prov_reset_link(void)
{
#if (MYNEWT_VAL(BLE_MESH_PB_ADV))
#if (MYNEWT_VAL(BLE_MESH_PB_GATT))
	link.rx.buf = bt_mesh_proxy_get_buf();
#else
	net_buf_simple_init(rx_buf, 0);
	link.rx.buf = rx_buf;
#endif
#endif
}

void bt_mesh_prov_complete(u16_t net_idx, u16_t addr)
{
	if (prov->complete) {
		prov->complete(net_idx, addr);
	}
}

void bt_mesh_prov_node_added(u16_t net_idx, u16_t addr, u8_t num_elem)
{
	if (prov->node_added) {
		prov->node_added(net_idx, addr, num_elem);
	}
}

void bt_mesh_prov_reset(void)
{
	if (prov->reset) {
		prov->reset();
	}
}

#endif /* MYNEWT_VAL(BLE_MESH_PROV) */
#endif
