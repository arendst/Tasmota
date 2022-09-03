/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#if MYNEWT_VAL(BLE_MESH)

#define MESH_LOG_MODULE BLE_MESH_MODEL_LOG
#if MYNEWT_VAL(BLE_MESH_CFG_CLI)

#include "../include/mesh/mesh.h"

#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "net.h"
#include "foundation.h"

#define CID_NVAL 0xffff

/* 2 byte dummy opcode for getting compile time buffer sizes. */
#define DUMMY_2_BYTE_OP	BT_MESH_MODEL_OP_2(0xff, 0xff)

struct comp_data {
	uint8_t *status;
	struct os_mbuf *comp;
};

static int32_t msg_timeout = K_SECONDS(5);

static struct bt_mesh_cfg_cli *cli;

static void comp_data_status(struct bt_mesh_model *model,
			     struct bt_mesh_msg_ctx *ctx,
			     struct os_mbuf *buf)
{
	struct comp_data *param;
	size_t to_copy;

	BT_DBG("net_idx 0x%04x app_idx 0x%04x src 0x%04x len %u: %s",
	       ctx->net_idx, ctx->app_idx, ctx->addr, buf->om_len,
	       bt_hex(buf->om_data, buf->om_len));

	if (cli->op_pending != OP_DEV_COMP_DATA_STATUS) {
		BT_WARN("Unexpected Composition Data Status");
		return;
	}

	param = cli->op_param;

	*(param->status) = net_buf_simple_pull_u8(buf);
	to_copy  = min(net_buf_simple_tailroom(param->comp), buf->om_len);
	net_buf_simple_add_mem(param->comp, buf->om_data, to_copy);

	k_sem_give(&cli->op_sync);
}

static void state_status_u8(struct bt_mesh_model *model,
			    struct bt_mesh_msg_ctx *ctx,
			    struct os_mbuf*buf,
			    uint32_t expect_status)
{
	uint8_t *status;

	BT_DBG("net_idx 0x%04x app_idx 0x%04x src 0x%04x len %u: %s",
	       ctx->net_idx, ctx->app_idx, ctx->addr, buf->om_len,
	       bt_hex(buf->om_data, buf->om_len));

	if (cli->op_pending != expect_status) {
		BT_WARN("Unexpected Status (0x%08x != 0x%08x)",
			(unsigned) cli->op_pending, (unsigned) expect_status);
		return;
	}

	status = cli->op_param;
	*status = net_buf_simple_pull_u8(buf);

	k_sem_give(&cli->op_sync);
}

static void beacon_status(struct bt_mesh_model *model,
			  struct bt_mesh_msg_ctx *ctx,
			  struct os_mbuf *buf)
{
	state_status_u8(model, ctx, buf, OP_BEACON_STATUS);
}

static void ttl_status(struct bt_mesh_model *model,
			  struct bt_mesh_msg_ctx *ctx,
			  struct os_mbuf*buf)
{
	state_status_u8(model, ctx, buf, OP_DEFAULT_TTL_STATUS);
}

static void friend_status(struct bt_mesh_model *model,
			  struct bt_mesh_msg_ctx *ctx,
			  struct os_mbuf*buf)
{
	state_status_u8(model, ctx, buf, OP_FRIEND_STATUS);
}

static void gatt_proxy_status(struct bt_mesh_model *model,
			      struct bt_mesh_msg_ctx *ctx,
			      struct os_mbuf*buf)
{
	state_status_u8(model, ctx, buf, OP_GATT_PROXY_STATUS);
}

struct relay_param {
	uint8_t *status;
	uint8_t *transmit;
};

static void relay_status(struct bt_mesh_model *model,
			 struct bt_mesh_msg_ctx *ctx,
			 struct os_mbuf*buf)
{
	struct relay_param *param;

	BT_DBG("net_idx 0x%04x app_idx 0x%04x src 0x%04x len %u: %s",
	       ctx->net_idx, ctx->app_idx, ctx->addr, buf->om_len,
	       bt_hex(buf->om_data, buf->om_len));

	if (cli->op_pending != OP_RELAY_STATUS) {
		BT_WARN("Unexpected Relay Status message");
		return;
	}

	param = cli->op_param;
	*param->status = net_buf_simple_pull_u8(buf);
	*param->transmit = net_buf_simple_pull_u8(buf);

	k_sem_give(&cli->op_sync);
}

struct net_key_param {
	uint8_t *status;
	uint16_t net_idx;
};

static void net_key_status(struct bt_mesh_model *model,
			   struct bt_mesh_msg_ctx *ctx,
			   struct os_mbuf *buf)
{
	struct net_key_param *param;
	uint16_t net_idx;
	uint8_t status;

	BT_DBG("net_idx 0x%04x app_idx 0x%04x src 0x%04x len %u: %s",
	       ctx->net_idx, ctx->app_idx, ctx->addr, buf->om_len,
	       bt_hex(buf->om_data, buf->om_len));

	if (cli->op_pending != OP_NET_KEY_STATUS) {
		BT_WARN("Unexpected Net Key Status message");
		return;
	}

	status = net_buf_simple_pull_u8(buf);
	net_idx = net_buf_simple_pull_le16(buf) & 0xfff;

	param = cli->op_param;
	if (param->net_idx != net_idx) {
		BT_WARN("Net Key Status key index does not match");
		return;
	}

	if (param->status) {
		*param->status = status;
	}

	k_sem_give(&cli->op_sync);
}

struct net_key_list_param {
	uint16_t *keys;
	size_t *key_cnt;
};

static void net_key_list(struct bt_mesh_model *model,
			 struct bt_mesh_msg_ctx *ctx,
			 struct os_mbuf *buf)
{
	struct net_key_list_param *param;
	int i;

	BT_DBG("net_idx 0x%04x app_idx 0x%04x src 0x%04x len %u: %s",
	       ctx->net_idx, ctx->app_idx, ctx->addr, buf->om_len,
	       bt_hex(buf->om_data, buf->om_len));

	if (cli->op_pending != OP_NET_KEY_LIST) {
		BT_WARN("Unexpected Net Key List message");
		return;
	}

	param = cli->op_param;

	for (i = 0; i < *param->key_cnt && buf->om_len >= 3; i += 2) {
		key_idx_unpack(buf, &param->keys[i], &param->keys[i + 1]);
	}

	if (i < *param->key_cnt && buf->om_len >= 2) {
		param->keys[i++] = net_buf_simple_pull_le16(buf) & 0xfff;
	}

	*param->key_cnt = i;

	k_sem_give(&cli->op_sync);
}

static void node_reset_status(struct bt_mesh_model *model,
		struct bt_mesh_msg_ctx *ctx, struct os_mbuf *buf)
{
	bool *param = NULL;
	BT_DBG("net_idx 0x%04x app_idx 0x%04x src 0x%04x",
		ctx->net_idx, ctx->app_idx, ctx->addr);

	if (cli->op_pending != OP_NODE_RESET_STATUS) {
		BT_WARN("Unexpected Node Reset Status message");
		return;
	}

	param = cli->op_param;

	if (param) {
		*param = true;
	}
	k_sem_give(&cli->op_sync);
}

struct app_key_param {
	uint8_t *status;
	uint16_t net_idx;
	uint16_t app_idx;
};

static void app_key_status(struct bt_mesh_model *model,
			   struct bt_mesh_msg_ctx *ctx,
			   struct os_mbuf*buf)
{
	struct app_key_param *param;
	uint16_t net_idx, app_idx;
	uint8_t status;

	BT_DBG("net_idx 0x%04x app_idx 0x%04x src 0x%04x len %u: %s",
	       ctx->net_idx, ctx->app_idx, ctx->addr, buf->om_len,
	       bt_hex(buf->om_data, buf->om_len));

	if (cli->op_pending != OP_APP_KEY_STATUS) {
		BT_WARN("Unexpected App Key Status message");
		return;
	}

	status = net_buf_simple_pull_u8(buf);
	key_idx_unpack(buf, &net_idx, &app_idx);

	param = cli->op_param;
	if (param->net_idx != net_idx || param->app_idx != app_idx) {
		BT_WARN("App Key Status key indices did not match");
		return;
	}

	if (param->status) {
		*param->status = status;
	}

	k_sem_give(&cli->op_sync);
}

struct app_key_list_param {
	uint16_t net_idx;
	uint8_t *status;
	uint16_t *keys;
	size_t *key_cnt;
};

static void app_key_list(struct bt_mesh_model *model,
			 struct bt_mesh_msg_ctx *ctx,
			 struct os_mbuf *buf)
{
	struct app_key_list_param *param;
	uint16_t net_idx;
	uint8_t status;
	int i;

	BT_DBG("net_idx 0x%04x app_idx 0x%04x src 0x%04x len %u: %s",
	       ctx->net_idx, ctx->app_idx, ctx->addr, buf->om_len,
	       bt_hex(buf->om_data, buf->om_len));

	if (cli->op_pending != OP_APP_KEY_LIST) {
		BT_WARN("Unexpected App Key List message");
		return;
	}

	status = net_buf_simple_pull_u8(buf);
	net_idx = net_buf_simple_pull_le16(buf) & 0xfff;

	param = cli->op_param;
	if (param->net_idx != net_idx) {
		BT_WARN("App Key List Net Key index did not match");
		return;
	}

	for (i = 0; i < *param->key_cnt && buf->om_len >= 3; i += 2) {
		key_idx_unpack(buf, &param->keys[i], &param->keys[i + 1]);
	}

	if (i < *param->key_cnt && buf->om_len >= 2) {
		param->keys[i++] = net_buf_simple_pull_le16(buf) & 0xfff;
	}

	*param->key_cnt = i;
	if (param->status) {
		*param->status = status;
	}

	k_sem_give(&cli->op_sync);
}

struct mod_app_param {
	uint8_t *status;
	uint16_t elem_addr;
	uint16_t mod_app_idx;
	uint16_t mod_id;
	uint16_t cid;
};

static void mod_app_status(struct bt_mesh_model *model,
			   struct bt_mesh_msg_ctx *ctx,
			   struct os_mbuf*buf)
{
	uint16_t elem_addr, mod_app_idx, mod_id, cid;
	struct mod_app_param *param;
	uint8_t status;

	BT_DBG("net_idx 0x%04x app_idx 0x%04x src 0x%04x len %u: %s",
	       ctx->net_idx, ctx->app_idx, ctx->addr, buf->om_len,
	       bt_hex(buf->om_data, buf->om_len));

	if (cli->op_pending != OP_MOD_APP_STATUS) {
		BT_WARN("Unexpected Model App Status message");
		return;
	}

	status = net_buf_simple_pull_u8(buf);
	elem_addr = net_buf_simple_pull_le16(buf);
	mod_app_idx = net_buf_simple_pull_le16(buf);

	if (buf->om_len >= 4) {
		cid = net_buf_simple_pull_le16(buf);
	} else {
		cid = CID_NVAL;
	}

	mod_id = net_buf_simple_pull_le16(buf);

	param = cli->op_param;
	if (param->elem_addr != elem_addr ||
	    param->mod_app_idx != mod_app_idx || param->mod_id != mod_id ||
	    param->cid != cid) {
		BT_WARN("Model App Status parameters did not match");
		return;
	}

	if (param->status) {
		*param->status = status;
	}

	k_sem_give(&cli->op_sync);
}

struct mod_member_list_param {
	uint8_t *status;
	uint16_t elem_addr;
	uint16_t mod_id;
	uint16_t cid;
	uint16_t *members;
	size_t *member_cnt;
};

static void mod_member_list_handle(struct bt_mesh_msg_ctx *ctx,
				   struct os_mbuf *buf, bool vnd)
{
	struct mod_member_list_param *param;
	uint16_t elem_addr, mod_id, cid;
	uint8_t status;
	int i;

	status = net_buf_simple_pull_u8(buf);
	elem_addr = net_buf_simple_pull_le16(buf);
	if (vnd) {
		cid = net_buf_simple_pull_le16(buf);
	}

	mod_id = net_buf_simple_pull_le16(buf);

	param = cli->op_param;
	if (param->elem_addr != elem_addr || param->mod_id != mod_id ||
	    (vnd && param->cid != cid)) {
		BT_WARN("Model Member List parameters did not match");
		return;
	}

	if (buf->om_len % 2) {
		BT_WARN("Model Member List invalid length");
		return;
	}

	for (i = 0; i < *param->member_cnt && buf->om_len; i++) {
		param->members[i] = net_buf_simple_pull_le16(buf);
	}

	*param->member_cnt = i;
	if (param->status) {
		*param->status = status;
	}

	k_sem_give(&cli->op_sync);
}

static void mod_app_list(struct bt_mesh_model *model,
			 struct bt_mesh_msg_ctx *ctx,
			 struct os_mbuf *buf)
{
	BT_DBG("net_idx 0x%04x app_idx 0x%04x src 0x%04x len %u: %s",
	       ctx->net_idx, ctx->app_idx, ctx->addr, buf->om_len,
	       bt_hex(buf->om_data, buf->om_len));

	if (cli->op_pending != OP_SIG_MOD_APP_LIST) {
		BT_WARN("Unexpected Model App List message");
		return;
	}

	mod_member_list_handle(ctx, buf, false);
}

static void mod_app_list_vnd(struct bt_mesh_model *model,
			     struct bt_mesh_msg_ctx *ctx,
			     struct os_mbuf *buf)
{
	BT_DBG("net_idx 0x%04x app_idx 0x%04x src 0x%04x len %u: %s",
	       ctx->net_idx, ctx->app_idx, ctx->addr, buf->om_len,
	       bt_hex(buf->om_data, buf->om_len));

	if (cli->op_pending != OP_VND_MOD_APP_LIST) {
		BT_WARN("Unexpected Model App List Vendor message");
		return;
	}

	mod_member_list_handle(ctx, buf, true);
}

struct mod_pub_param {
	uint16_t                       mod_id;
	uint16_t                       cid;
	uint16_t                       elem_addr;
	uint8_t                       *status;
	struct bt_mesh_cfg_mod_pub *pub;
};

static void mod_pub_status(struct bt_mesh_model *model,
			   struct bt_mesh_msg_ctx *ctx,
			   struct os_mbuf*buf)
{
	uint16_t mod_id, cid, elem_addr;
	struct mod_pub_param *param;
	uint8_t status;

	BT_DBG("net_idx 0x%04x app_idx 0x%04x src 0x%04x len %u: %s",
	       ctx->net_idx, ctx->app_idx, ctx->addr, buf->om_len,
	       bt_hex(buf->om_data, buf->om_len));

	if (cli->op_pending != OP_MOD_PUB_STATUS) {
		BT_WARN("Unexpected Model Pub Status message");
		return;
	}

	param = cli->op_param;
	if (param->cid != CID_NVAL) {
		if (buf->om_len < 14) {
			BT_WARN("Unexpected Mod Pub Status with SIG Model");
			return;
		}

		cid = sys_get_le16(&buf->om_data[10]);
		mod_id = sys_get_le16(&buf->om_data[12]);
	} else {
		if (buf->om_len > 12) {
			BT_WARN("Unexpected Mod Pub Status with Vendor Model");
			return;
		}

		cid = CID_NVAL;
		mod_id = sys_get_le16(&buf->om_data[10]);
	}

	if (mod_id != param->mod_id || cid != param->cid) {
		BT_WARN("Mod Pub Model ID or Company ID mismatch");
		return;
	}

	status = net_buf_simple_pull_u8(buf);

	elem_addr = net_buf_simple_pull_le16(buf);
	if (elem_addr != param->elem_addr) {
		BT_WARN("Model Pub Status for unexpected element (0x%04x)",
			elem_addr);
		return;
	}

	if (param->status) {
		*param->status = status;
	}

	if (param->pub) {
		param->pub->addr = net_buf_simple_pull_le16(buf);
		param->pub->app_idx = net_buf_simple_pull_le16(buf);
		param->pub->cred_flag = (param->pub->app_idx & BIT(12));
		param->pub->app_idx &= BIT_MASK(12);
		param->pub->ttl = net_buf_simple_pull_u8(buf);
		param->pub->period = net_buf_simple_pull_u8(buf);
		param->pub->transmit = net_buf_simple_pull_u8(buf);
	}

	k_sem_give(&cli->op_sync);
}

struct mod_sub_param {
	uint8_t *status;
	uint16_t elem_addr;
	uint16_t *sub_addr;
	uint16_t *expect_sub;
	uint16_t mod_id;
	uint16_t cid;
};

static void mod_sub_status(struct bt_mesh_model *model,
			   struct bt_mesh_msg_ctx *ctx,
			   struct os_mbuf*buf)
{
	uint16_t elem_addr, sub_addr, mod_id, cid;
	struct mod_sub_param *param;
	uint8_t status;

	BT_DBG("net_idx 0x%04x app_idx 0x%04x src 0x%04x len %u: %s",
	       ctx->net_idx, ctx->app_idx, ctx->addr, buf->om_len,
	       bt_hex(buf->om_data, buf->om_len));

	if (cli->op_pending != OP_MOD_SUB_STATUS) {
		BT_WARN("Unexpected Model Subscription Status message");
		return;
	}

	status = net_buf_simple_pull_u8(buf);
	elem_addr = net_buf_simple_pull_le16(buf);
	sub_addr = net_buf_simple_pull_le16(buf);

	if (buf->om_len >= 4) {
		cid = net_buf_simple_pull_le16(buf);
	} else {
		cid = CID_NVAL;
	}

	mod_id = net_buf_simple_pull_le16(buf);

	param = cli->op_param;
	if (param->elem_addr != elem_addr || param->mod_id != mod_id ||
	    (param->expect_sub && *param->expect_sub != sub_addr) ||
	    param->cid != cid) {
		BT_WARN("Model Subscription Status parameters did not match");
		return;
	}

	if (param->sub_addr) {
		*param->sub_addr = sub_addr;
	}

	if (param->status) {
		*param->status = status;
	}

	k_sem_give(&cli->op_sync);
}

static void mod_sub_list(struct bt_mesh_model *model,
			 struct bt_mesh_msg_ctx *ctx,
			 struct os_mbuf *buf)
{
	BT_DBG("net_idx 0x%04x app_idx 0x%04x src 0x%04x len %u: %s",
	       ctx->net_idx, ctx->app_idx, ctx->addr, buf->om_len,
	       bt_hex(buf->om_data, buf->om_len));

	if (cli->op_pending != OP_MOD_SUB_LIST) {
		BT_WARN("Unexpected Model Subscription List message");
		return;
	}

	mod_member_list_handle(ctx, buf, false);
}

static void mod_sub_list_vnd(struct bt_mesh_model *model,
			     struct bt_mesh_msg_ctx *ctx,
			     struct os_mbuf *buf)
{
	BT_DBG("net_idx 0x%04x app_idx 0x%04x src 0x%04x len %u: %s",
	       ctx->net_idx, ctx->app_idx, ctx->addr, buf->om_len,
	       bt_hex(buf->om_data, buf->om_len));

	if (cli->op_pending != OP_MOD_SUB_LIST_VND) {
		BT_WARN("Unexpected Model Subscription List Vendor message");
		return;
	}

	mod_member_list_handle(ctx, buf, true);
}

struct hb_sub_param {
	uint8_t *status;
	struct bt_mesh_cfg_hb_sub *sub;
};

static void hb_sub_status(struct bt_mesh_model *model,
			  struct bt_mesh_msg_ctx *ctx,
			  struct os_mbuf*buf)
{
	struct hb_sub_param *param;

	BT_DBG("net_idx 0x%04x app_idx 0x%04x src 0x%04x len %u: %s",
	       ctx->net_idx, ctx->app_idx, ctx->addr, buf->om_len,
	       bt_hex(buf->om_data, buf->om_len));

	if (cli->op_pending != OP_HEARTBEAT_SUB_STATUS) {
		BT_WARN("Unexpected Heartbeat Subscription Status message");
		return;
	}

	param = cli->op_param;

	*param->status = net_buf_simple_pull_u8(buf);

	param->sub->src = net_buf_simple_pull_le16(buf);
	param->sub->dst = net_buf_simple_pull_le16(buf);
	param->sub->period = net_buf_simple_pull_u8(buf);
	param->sub->count = net_buf_simple_pull_u8(buf);
	param->sub->min = net_buf_simple_pull_u8(buf);
	param->sub->max = net_buf_simple_pull_u8(buf);

	k_sem_give(&cli->op_sync);
}

struct hb_pub_param {
	uint8_t *status;
	struct bt_mesh_cfg_hb_pub *pub;
};

static void hb_pub_status(struct bt_mesh_model *model,
			  struct bt_mesh_msg_ctx *ctx,
			  struct os_mbuf *buf)
{
	struct hb_pub_param *param;

	BT_DBG("net_idx 0x%04x app_idx 0x%04x src 0x%04x len %u: %s",
	       ctx->net_idx, ctx->app_idx, ctx->addr, buf->om_len,
	       bt_hex(buf->om_data, buf->om_len));

	if (cli->op_pending != OP_HEARTBEAT_PUB_STATUS) {
		BT_WARN("Unexpected Heartbeat Publication Status message");
		return;
	}

	param = cli->op_param;

	*param->status = net_buf_simple_pull_u8(buf);

	if (param->pub) {
		param->pub->dst = net_buf_simple_pull_le16(buf);
		param->pub->count = net_buf_simple_pull_u8(buf);
		param->pub->period = net_buf_simple_pull_u8(buf);
		param->pub->ttl = net_buf_simple_pull_u8(buf);
		param->pub->feat = net_buf_simple_pull_u8(buf);
		param->pub->net_idx = net_buf_simple_pull_u8(buf);
	}

	k_sem_give(&cli->op_sync);
}

const struct bt_mesh_model_op bt_mesh_cfg_cli_op[] = {
	{ OP_DEV_COMP_DATA_STATUS,   15,  comp_data_status },
	{ OP_BEACON_STATUS,          1,   beacon_status },
	{ OP_DEFAULT_TTL_STATUS,     1,   ttl_status },
	{ OP_FRIEND_STATUS,          1,   friend_status },
	{ OP_GATT_PROXY_STATUS,      1,   gatt_proxy_status },
	{ OP_RELAY_STATUS,           2,   relay_status },
	{ OP_NET_KEY_STATUS,         3,   net_key_status },
	{ OP_NET_KEY_LIST,           0,   net_key_list },
	{ OP_APP_KEY_STATUS,         4,   app_key_status },
	{ OP_APP_KEY_LIST,           3,   app_key_list },
	{ OP_MOD_APP_STATUS,         7,   mod_app_status },
	{ OP_SIG_MOD_APP_LIST,       5,   mod_app_list},
	{ OP_VND_MOD_APP_LIST,       7,   mod_app_list_vnd},
	{ OP_MOD_PUB_STATUS,         12,  mod_pub_status },
	{ OP_MOD_SUB_STATUS,         7,   mod_sub_status },
	{ OP_MOD_SUB_LIST,           5,   mod_sub_list},
	{ OP_MOD_SUB_LIST_VND,       7,   mod_sub_list_vnd},
	{ OP_HEARTBEAT_SUB_STATUS,   9,   hb_sub_status },
	{ OP_HEARTBEAT_PUB_STATUS,   10,  hb_pub_status },
	{ OP_NODE_RESET_STATUS,      0,   node_reset_status},
	BT_MESH_MODEL_OP_END,
};

static int cfg_cli_init(struct bt_mesh_model *model)
{
	BT_DBG("");

	if (!bt_mesh_model_in_primary(model)) {
		BT_ERR("Configuration Client only allowed in primary element");
		return -EINVAL;
	}

	if (!model->user_data) {
		BT_ERR("No Configuration Client context provided");
		return -EINVAL;
	}

	cli = model->user_data;
	cli->model = model;

	/*
	 * Configuration Model security is device-key based and both the local
	 * and remote keys are allowed to access this model.
	 */
	model->keys[0] = BT_MESH_KEY_DEV_ANY;

	k_sem_init(&cli->op_sync, 0, 1);

	return 0;
}

const struct bt_mesh_model_cb bt_mesh_cfg_cli_cb = {
	.init = cfg_cli_init,
};

static int cli_prepare(void *param, uint32_t op)
{
	if (!cli) {
		BT_ERR("No available Configuration Client context!");
		return -EINVAL;
	}

	if (cli->op_pending) {
		BT_WARN("Another synchronous operation pending");
		return -EBUSY;
	}

	cli->op_param = param;
	cli->op_pending = op;

	return 0;
}

static void cli_reset(void)
{
	cli->op_pending = 0;
	cli->op_param = NULL;
}

static int cli_wait(void)
{
	int err;

	err = k_sem_take(&cli->op_sync, msg_timeout);

	cli_reset();

	return err;
}

int bt_mesh_cfg_comp_data_get(uint16_t net_idx, uint16_t addr, uint8_t page,
			      uint8_t *status, struct os_mbuf *comp)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(OP_DEV_COMP_DATA_GET, 1);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct comp_data param = {
		.status = status,
		.comp = comp,
	};
	int err;

	err = cli_prepare(&param, OP_DEV_COMP_DATA_STATUS);
	if (err) {
		goto done;
	}

	bt_mesh_model_msg_init(msg, OP_DEV_COMP_DATA_GET);
	net_buf_simple_add_u8(msg, page);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		goto done;
	}

	err = cli_wait();
done:
	os_mbuf_free_chain(msg);
	return err;
}

static int get_state_u8(uint16_t net_idx, uint16_t addr, uint32_t op, uint32_t rsp,
			uint8_t *val)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(DUMMY_2_BYTE_OP, 0);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	int err;

	err = cli_prepare(val, rsp);
	if (err) {
		goto done;
	}

	bt_mesh_model_msg_init(msg, op);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		goto done;
	}

	err = cli_wait();
done:
	os_mbuf_free_chain(msg);
	return err;
}

static int set_state_u8(uint16_t net_idx, uint16_t addr, uint32_t op, uint32_t rsp,
			uint8_t new_val, uint8_t *val)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(DUMMY_2_BYTE_OP, 1);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	int err;

	err = cli_prepare(val, rsp);
	if (err) {
		goto done;
	}

	bt_mesh_model_msg_init(msg, op);
	net_buf_simple_add_u8(msg, new_val);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		goto done;
	}

	err = cli_wait();
done:
	os_mbuf_free_chain(msg);
	return err;
}

int bt_mesh_cfg_beacon_get(uint16_t net_idx, uint16_t addr, uint8_t *status)
{
	return get_state_u8(net_idx, addr, OP_BEACON_GET, OP_BEACON_STATUS,
			    status);
}

int bt_mesh_cfg_beacon_set(uint16_t net_idx, uint16_t addr, uint8_t val, uint8_t *status)
{
	return set_state_u8(net_idx, addr, OP_BEACON_SET, OP_BEACON_STATUS,
			    val, status);
}

int bt_mesh_cfg_ttl_get(uint16_t net_idx, uint16_t addr, uint8_t *ttl)
{
	return get_state_u8(net_idx, addr, OP_DEFAULT_TTL_GET,
			    OP_DEFAULT_TTL_STATUS, ttl);
}

int bt_mesh_cfg_ttl_set(uint16_t net_idx, uint16_t addr, uint8_t val, uint8_t *ttl)
{
	return set_state_u8(net_idx, addr, OP_DEFAULT_TTL_SET,
			    OP_DEFAULT_TTL_STATUS, val, ttl);
}

int bt_mesh_cfg_friend_get(uint16_t net_idx, uint16_t addr, uint8_t *status)
{
	return get_state_u8(net_idx, addr, OP_FRIEND_GET,
			    OP_FRIEND_STATUS, status);
}

int bt_mesh_cfg_friend_set(uint16_t net_idx, uint16_t addr, uint8_t val, uint8_t *status)
{
	return set_state_u8(net_idx, addr, OP_FRIEND_SET, OP_FRIEND_STATUS,
			    val, status);
}

int bt_mesh_cfg_gatt_proxy_get(uint16_t net_idx, uint16_t addr, uint8_t *status)
{
	return get_state_u8(net_idx, addr, OP_GATT_PROXY_GET,
			    OP_GATT_PROXY_STATUS, status);
}

int bt_mesh_cfg_gatt_proxy_set(uint16_t net_idx, uint16_t addr, uint8_t val,
			       uint8_t *status)
{
	return set_state_u8(net_idx, addr, OP_GATT_PROXY_SET,
			    OP_GATT_PROXY_STATUS, val, status);
}

int bt_mesh_cfg_net_transmit_set(uint16_t net_idx, uint16_t addr,
		uint8_t val, uint8_t *transmit)
{
	return set_state_u8(net_idx, addr, OP_NET_TRANSMIT_SET,
				OP_NET_TRANSMIT_STATUS, val, transmit);
}

int bt_mesh_cfg_net_transmit_get(uint16_t net_idx, uint16_t addr,
		uint8_t *transmit)
{
	return get_state_u8(net_idx, addr, OP_NET_TRANSMIT_GET,
			OP_NET_TRANSMIT_STATUS, transmit);
}

int bt_mesh_cfg_relay_get(uint16_t net_idx, uint16_t addr, uint8_t *status,
			  uint8_t *transmit)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(OP_RELAY_GET, 0);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct relay_param param = {
		.status = status,
		.transmit = transmit,
	};
	int err;

	err = cli_prepare(&param, OP_RELAY_STATUS);
	if (err) {
		goto done;
	}

	bt_mesh_model_msg_init(msg, OP_RELAY_GET);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		goto done;
	}

	err = cli_wait();
done:
	os_mbuf_free_chain(msg);
	return err;
}

int bt_mesh_cfg_relay_set(uint16_t net_idx, uint16_t addr, uint8_t new_relay,
			  uint8_t new_transmit, uint8_t *status, uint8_t *transmit)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(OP_RELAY_SET, 2);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct relay_param param = {
		.status = status,
		.transmit = transmit,
	};
	int err;

	err = cli_prepare(&param, OP_RELAY_STATUS);
	if (err) {
		goto done;
	}

	bt_mesh_model_msg_init(msg, OP_RELAY_SET);
	net_buf_simple_add_u8(msg, new_relay);
	net_buf_simple_add_u8(msg, new_transmit);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		goto done;
	}

	err = cli_wait();
done:
	os_mbuf_free_chain(msg);
	return err;
}

int bt_mesh_cfg_net_key_add(uint16_t net_idx, uint16_t addr, uint16_t key_net_idx,
			    const uint8_t net_key[16], uint8_t *status)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(OP_NET_KEY_ADD, 18);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct net_key_param param = {
		.status = status,
		.net_idx = key_net_idx,
	};
	int err;

	err = cli_prepare(&param, OP_NET_KEY_STATUS);
	if (err) {
		goto done;
	}

	bt_mesh_model_msg_init(msg, OP_NET_KEY_ADD);
	net_buf_simple_add_le16(msg, key_net_idx);
	net_buf_simple_add_mem(msg, net_key, 16);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		goto done;
	}

	if (!status) {
		cli_reset();
		goto done;
	}

	err = cli_wait();
done:
	os_mbuf_free_chain(msg);
	return err;
}

int bt_mesh_cfg_net_key_get(uint16_t net_idx, uint16_t addr, uint16_t *keys,
			     size_t *key_cnt)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(OP_NET_KEY_GET, 0);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct net_key_list_param param = {
		.keys = keys,
		.key_cnt = key_cnt,
	};
	int err;

	err = cli_prepare(&param, OP_NET_KEY_LIST);
	if (err) {
		return err;
	}

	bt_mesh_model_msg_init(msg, OP_NET_KEY_GET);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		return err;
	}

	os_mbuf_free_chain(msg);
	return cli_wait();
}

int bt_mesh_cfg_net_key_del(uint16_t net_idx, uint16_t addr,
			    uint16_t key_net_idx, uint8_t *status)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(OP_NET_KEY_DEL, 2);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct net_key_param param = {
		.status = status,
		.net_idx = key_net_idx,
	};
	int err;

	err = cli_prepare(&param, OP_NET_KEY_STATUS);
	if (err) {
		goto done;
	}

	bt_mesh_model_msg_init(msg, OP_NET_KEY_DEL);
	net_buf_simple_add_le16(msg, key_net_idx);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		goto done;
	}

	if (!status) {
		cli_reset();
		goto done;
	}

	err = cli_wait();
done:
	os_mbuf_free_chain(msg);
	return err;
}

int bt_mesh_cfg_app_key_add(uint16_t net_idx, uint16_t addr, uint16_t key_net_idx,
			    uint16_t key_app_idx, const uint8_t app_key[16],
			    uint8_t *status)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(OP_APP_KEY_ADD, 19);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct app_key_param param = {
		.status = status,
		.net_idx = key_net_idx,
		.app_idx = key_app_idx,
	};
	int err;

	err = cli_prepare(&param, OP_APP_KEY_STATUS);
	if (err) {
		goto done;
	}

	bt_mesh_model_msg_init(msg, OP_APP_KEY_ADD);
	key_idx_pack(msg, key_net_idx, key_app_idx);
	net_buf_simple_add_mem(msg, app_key, 16);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		goto done;
	}

	if (!status) {
		cli_reset();
		goto done;
	}

	err = cli_wait();
done:
	os_mbuf_free_chain(msg);
	return err;
}

int bt_mesh_cfg_node_reset(uint16_t net_idx, uint16_t addr, bool *status)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(OP_NODE_RESET, 0);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};

	int err;

	if (status) {
		*status = false;
	}

	err = cli_prepare(status, OP_NODE_RESET_STATUS);
	if (err) {
		goto done;
	}

	bt_mesh_model_msg_init(msg, OP_NODE_RESET);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		goto done;
	}

	if (!status) {
		cli_reset();
		goto done;
	}

	err = cli_wait();
done:
	os_mbuf_free_chain(msg);
	if (err) {
		return err;
	} else {
		return 0;
	}
}

int bt_mesh_cfg_app_key_get(uint16_t net_idx, uint16_t addr, uint16_t key_net_idx,
			     uint8_t *status, uint16_t *keys, size_t *key_cnt)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(OP_APP_KEY_GET, 2);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct app_key_list_param param = {
		.net_idx = key_net_idx,
		.status = status,
		.keys = keys,
		.key_cnt = key_cnt,
	};
	int err;

	err = cli_prepare(&param, OP_APP_KEY_LIST);
	if (err) {
		os_mbuf_free_chain(msg);
		return err;
	}

	bt_mesh_model_msg_init(msg, OP_APP_KEY_GET);
	net_buf_simple_add_le16(msg, key_net_idx);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		os_mbuf_free_chain(msg);
		cli_reset();
		return err;
	}

	os_mbuf_free_chain(msg);
	return cli_wait();
}

int bt_mesh_cfg_app_key_del(uint16_t net_idx, uint16_t addr,
	uint16_t key_net_idx, uint16_t key_app_idx, uint8_t *status)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(OP_APP_KEY_DEL, 3);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct app_key_param param = {
		.status = status,
		.net_idx = key_net_idx,
		.app_idx = key_app_idx,
	};
	int err;

	err = cli_prepare(&param, OP_APP_KEY_STATUS);
	if (err) {
		os_mbuf_free_chain(msg);
		return err;
	}

	bt_mesh_model_msg_init(msg, OP_APP_KEY_DEL);
	key_idx_pack(msg, key_net_idx, key_app_idx);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		os_mbuf_free_chain(msg);
		cli_reset();
		return err;
	}

	if (!status) {
		cli_reset();
		os_mbuf_free_chain(msg);
		return 0;
	}

	os_mbuf_free_chain(msg);
	return cli_wait();
}

static int mod_app_bind(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
			uint16_t mod_app_idx, uint16_t mod_id, uint16_t cid,
			uint8_t *status)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(OP_MOD_APP_BIND, 8);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct mod_app_param param = {
		.status = status,
		.elem_addr = elem_addr,
		.mod_app_idx = mod_app_idx,
		.mod_id = mod_id,
		.cid = cid,
	};
	int err;

	err = cli_prepare(&param, OP_MOD_APP_STATUS);
	if (err) {
		goto done;
	}

	bt_mesh_model_msg_init(msg, OP_MOD_APP_BIND);
	net_buf_simple_add_le16(msg, elem_addr);
	net_buf_simple_add_le16(msg, mod_app_idx);

	if (cid != CID_NVAL) {
		net_buf_simple_add_le16(msg, cid);
	}

	net_buf_simple_add_le16(msg, mod_id);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		goto done;
	}

	if (!status) {
		cli_reset();
		goto done;
	}

	err = cli_wait();
done:
	os_mbuf_free_chain(msg);
	return err;
}

int bt_mesh_cfg_mod_app_bind(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
			     uint16_t mod_app_idx, uint16_t mod_id, uint8_t *status)
{
	return mod_app_bind(net_idx, addr, elem_addr, mod_app_idx, mod_id,
			    CID_NVAL, status);
}

int bt_mesh_cfg_mod_app_bind_vnd(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
				 uint16_t mod_app_idx, uint16_t mod_id, uint16_t cid,
				 uint8_t *status)
{
	if (cid == CID_NVAL) {
		return -EINVAL;
	}

	return mod_app_bind(net_idx, addr, elem_addr, mod_app_idx, mod_id, cid,
			    status);
}

static int mod_app_unbind(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
			uint16_t mod_app_idx, uint16_t mod_id, uint16_t cid,
			uint8_t *status)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(OP_MOD_APP_UNBIND, 8);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct mod_app_param param = {
		.status = status,
		.elem_addr = elem_addr,
		.mod_app_idx = mod_app_idx,
		.mod_id = mod_id,
		.cid = cid,
	};
	int err;

	err = cli_prepare(&param, OP_MOD_APP_STATUS);
	if (err) {
		goto done;
	}

	bt_mesh_model_msg_init(msg, OP_MOD_APP_UNBIND);
	net_buf_simple_add_le16(msg, elem_addr);
	net_buf_simple_add_le16(msg, mod_app_idx);

	if (cid != CID_NVAL) {
		net_buf_simple_add_le16(msg, cid);
	}

	net_buf_simple_add_le16(msg, mod_id);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		goto done;
	}

	if (!status) {
		cli_reset();
		err = 0;
		goto done;
	}

done:
	os_mbuf_free_chain(msg);
	if (err) {
		return err;
	} else {
		return cli_wait();
	}
}

int bt_mesh_cfg_mod_app_unbind(uint16_t net_idx, uint16_t addr,
	uint16_t elem_addr, uint16_t mod_app_idx,
	uint16_t mod_id, uint8_t *status)
{
	return mod_app_unbind(net_idx, addr, elem_addr, mod_app_idx, mod_id,
			CID_NVAL, status);
}

int bt_mesh_cfg_mod_app_unbind_vnd(uint16_t net_idx, uint16_t addr,
		uint16_t elem_addr, uint16_t mod_app_idx,
		uint16_t mod_id, uint16_t cid, uint8_t *status)
{
	if (cid == CID_NVAL) {
		return -EINVAL;
	}

	return mod_app_unbind(net_idx, addr, elem_addr, mod_app_idx,
			mod_id, cid, status);
}

static int mod_member_list_get(uint32_t op, uint32_t expect_op, uint16_t net_idx,
			       uint16_t addr, uint16_t elem_addr, uint16_t mod_id,
			       uint16_t cid, uint8_t *status, uint16_t *apps,
			       size_t *app_cnt)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(DUMMY_2_BYTE_OP, 6);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct mod_member_list_param param = {
		.status = status,
		.elem_addr = elem_addr,
		.mod_id = mod_id,
		.cid = cid,
		.members = apps,
		.member_cnt = app_cnt,
	};
	int err;

	err = cli_prepare(&param, expect_op);
	if (err) {
		goto done;
	}

	BT_DBG("net_idx 0x%04x addr 0x%04x elem_addr 0x%04x",
	       net_idx, addr, elem_addr);
	BT_DBG("mod_id 0x%04x cid 0x%04x op: %x", mod_id, cid, op);

	bt_mesh_model_msg_init(msg, op);
	net_buf_simple_add_le16(msg, elem_addr);

	if (cid != CID_NVAL) {
		net_buf_simple_add_le16(msg, cid);
	}

	net_buf_simple_add_le16(msg, mod_id);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		goto done;
	}

	err = cli_wait();
done:
	os_mbuf_free_chain(msg);
	return err;
}

int bt_mesh_cfg_mod_app_get(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
			    uint16_t mod_id, uint8_t *status, uint16_t *apps,
			    size_t *app_cnt)
{
	return mod_member_list_get(OP_SIG_MOD_APP_GET, OP_SIG_MOD_APP_LIST,
				   net_idx, addr, elem_addr, mod_id, CID_NVAL,
				   status, apps, app_cnt);
}

int bt_mesh_cfg_mod_app_get_vnd(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
				uint16_t mod_id, uint16_t cid, uint8_t *status,
				uint16_t *apps, size_t *app_cnt)
{
	if (cid == CID_NVAL) {
		return -EINVAL;
	}

	return mod_member_list_get(OP_VND_MOD_APP_GET, OP_VND_MOD_APP_LIST,
				   net_idx, addr, elem_addr, mod_id, CID_NVAL,
				   status, apps, app_cnt);
}

static int mod_sub(uint32_t op, uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
		   uint16_t sub_addr, uint16_t mod_id, uint16_t cid, uint8_t *status)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(DUMMY_2_BYTE_OP, 8);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct mod_sub_param param = {
		.status = status,
		.elem_addr = elem_addr,
		.expect_sub = &sub_addr,
		.mod_id = mod_id,
		.cid = cid,
	};
	int err;

	err = cli_prepare(&param, OP_MOD_SUB_STATUS);
	if (err) {
		goto done;
	}

	bt_mesh_model_msg_init(msg, op);
	net_buf_simple_add_le16(msg, elem_addr);
	net_buf_simple_add_le16(msg, sub_addr);

	if (cid != CID_NVAL) {
		net_buf_simple_add_le16(msg, cid);
	}

	net_buf_simple_add_le16(msg, mod_id);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		goto done;
	}

	if (!status) {
		cli_reset();
		err = 0;
		goto done;
	}

	err = cli_wait();
done:
	os_mbuf_free_chain(msg);
	return err;
}

int bt_mesh_cfg_mod_sub_add(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
			    uint16_t sub_addr, uint16_t mod_id, uint8_t *status)
{
	return mod_sub(OP_MOD_SUB_ADD, net_idx, addr, elem_addr, sub_addr,
		       mod_id, CID_NVAL, status);
}

int bt_mesh_cfg_mod_sub_add_vnd(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
				 uint16_t sub_addr, uint16_t mod_id, uint16_t cid,
				 uint8_t *status)
{
	if (cid == CID_NVAL) {
		return -EINVAL;
	}

	return mod_sub(OP_MOD_SUB_ADD, net_idx, addr, elem_addr, sub_addr,
		       mod_id, cid, status);
}

int bt_mesh_cfg_mod_sub_del(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
			    uint16_t sub_addr, uint16_t mod_id, uint8_t *status)
{
	return mod_sub(OP_MOD_SUB_DEL, net_idx, addr, elem_addr, sub_addr,
		       mod_id, CID_NVAL, status);
}

int bt_mesh_cfg_mod_sub_del_vnd(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
				 uint16_t sub_addr, uint16_t mod_id, uint16_t cid,
				 uint8_t *status)
{
	if (cid == CID_NVAL) {
		return -EINVAL;
	}

	return mod_sub(OP_MOD_SUB_DEL, net_idx, addr, elem_addr, sub_addr,
		       mod_id, cid, status);
}

int bt_mesh_cfg_mod_sub_overwrite(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
				  uint16_t sub_addr, uint16_t mod_id, uint8_t *status)
{
	return mod_sub(OP_MOD_SUB_OVERWRITE, net_idx, addr, elem_addr,
		       sub_addr, mod_id, CID_NVAL, status);
}

int bt_mesh_cfg_mod_sub_overwrite_vnd(uint16_t net_idx, uint16_t addr,
				      uint16_t elem_addr, uint16_t sub_addr,
				      uint16_t mod_id, uint16_t cid, uint8_t *status)
{
	if (cid == CID_NVAL) {
		return -EINVAL;
	}

	return mod_sub(OP_MOD_SUB_OVERWRITE, net_idx, addr, elem_addr,
		       sub_addr, mod_id, cid, status);
}

static int mod_sub_va(uint32_t op, uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
		      const uint8_t label[16], uint16_t mod_id, uint16_t cid,
		      uint16_t *virt_addr, uint8_t *status)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(DUMMY_2_BYTE_OP, 22);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct mod_sub_param param = {
		.status = status,
		.elem_addr = elem_addr,
		.sub_addr = virt_addr,
		.mod_id = mod_id,
		.cid = cid,
	};
	int err;

	err = cli_prepare(&param, OP_MOD_SUB_STATUS);
	if (err) {
		goto done;
	}

	BT_DBG("net_idx 0x%04x addr 0x%04x elem_addr 0x%04x label %s",
	       net_idx, addr, elem_addr, label);
	BT_DBG("mod_id 0x%04x cid 0x%04x", mod_id, cid);

	bt_mesh_model_msg_init(msg, op);
	net_buf_simple_add_le16(msg, elem_addr);
	net_buf_simple_add_mem(msg, label, 16);

	if (cid != CID_NVAL) {
		net_buf_simple_add_le16(msg, cid);
	}

	net_buf_simple_add_le16(msg, mod_id);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		goto done;
	}

	if (!status) {
		cli_reset();
		goto done;
	}

	err = cli_wait();
done:
	os_mbuf_free_chain(msg);
	return err;
}

int bt_mesh_cfg_mod_sub_va_add(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
			       const uint8_t label[16], uint16_t mod_id,
			       uint16_t *virt_addr, uint8_t *status)
{
	return mod_sub_va(OP_MOD_SUB_VA_ADD, net_idx, addr, elem_addr, label,
			  mod_id, CID_NVAL, virt_addr, status);
}

int bt_mesh_cfg_mod_sub_va_add_vnd(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
				   const uint8_t label[16], uint16_t mod_id,
				   uint16_t cid, uint16_t *virt_addr, uint8_t *status)
{
	if (cid == CID_NVAL) {
		return -EINVAL;
	}

	return mod_sub_va(OP_MOD_SUB_VA_ADD, net_idx, addr, elem_addr, label,
			  mod_id, cid, virt_addr, status);
}

int bt_mesh_cfg_mod_sub_va_del(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
			       const uint8_t label[16], uint16_t mod_id,
			       uint16_t *virt_addr, uint8_t *status)
{
	return mod_sub_va(OP_MOD_SUB_VA_DEL, net_idx, addr, elem_addr, label,
			  mod_id, CID_NVAL, virt_addr, status);
}

int bt_mesh_cfg_mod_sub_va_del_vnd(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
				   const uint8_t label[16], uint16_t mod_id,
				   uint16_t cid, uint16_t *virt_addr, uint8_t *status)
{
	if (cid == CID_NVAL) {
		return -EINVAL;
	}

	return mod_sub_va(OP_MOD_SUB_VA_DEL, net_idx, addr, elem_addr, label,
			  mod_id, cid, virt_addr, status);
}

int bt_mesh_cfg_mod_sub_va_overwrite(uint16_t net_idx, uint16_t addr,
				     uint16_t elem_addr, const uint8_t label[16],
				     uint16_t mod_id, uint16_t *virt_addr,
				     uint8_t *status)
{
	return mod_sub_va(OP_MOD_SUB_VA_OVERWRITE, net_idx, addr, elem_addr,
			  label, mod_id, CID_NVAL, virt_addr, status);
}

int bt_mesh_cfg_mod_sub_va_overwrite_vnd(uint16_t net_idx, uint16_t addr,
					 uint16_t elem_addr, const uint8_t label[16],
					 uint16_t mod_id, uint16_t cid,
					 uint16_t *virt_addr, uint8_t *status)
{
	if (cid == CID_NVAL) {
		return -EINVAL;
	}

	return mod_sub_va(OP_MOD_SUB_VA_OVERWRITE, net_idx, addr, elem_addr,
			  label, mod_id, cid, virt_addr, status);
}

int bt_mesh_cfg_mod_sub_get(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
			    uint16_t mod_id, uint8_t *status, uint16_t *subs,
			    size_t *sub_cnt)
{
	return mod_member_list_get(OP_MOD_SUB_GET, OP_MOD_SUB_LIST, net_idx,
				   addr, elem_addr, mod_id, CID_NVAL, status,
				   subs, sub_cnt);
}

int bt_mesh_cfg_mod_sub_get_vnd(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
				uint16_t mod_id, uint16_t cid, uint8_t *status,
				uint16_t *subs, size_t *sub_cnt)
{
	if (cid == CID_NVAL) {
		return -EINVAL;
	}

	return mod_member_list_get(OP_MOD_SUB_GET_VND, OP_MOD_SUB_LIST_VND,
				   net_idx, addr, elem_addr, mod_id, CID_NVAL,
				   status, subs, sub_cnt);
}

static int mod_pub_get(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
		       uint16_t mod_id, uint16_t cid,
		       struct bt_mesh_cfg_mod_pub *pub, uint8_t *status)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(OP_MOD_PUB_GET, 6);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct mod_pub_param param = {
		.mod_id = mod_id,
		.cid = cid,
		.elem_addr = elem_addr,
		.status = status,
		.pub = pub,
	};
	int err;

	err = cli_prepare(&param, OP_MOD_PUB_STATUS);
	if (err) {
		goto done;
	}

	bt_mesh_model_msg_init(msg, OP_MOD_PUB_GET);

	net_buf_simple_add_le16(msg, elem_addr);

	if (cid != CID_NVAL) {
		net_buf_simple_add_le16(msg, cid);
	}

	net_buf_simple_add_le16(msg, mod_id);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		goto done;
	}

	if (!status) {
		cli_reset();
		goto done;
	}

	err = cli_wait();
done:
	os_mbuf_free_chain(msg);
	return err;
}

int bt_mesh_cfg_mod_pub_get(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
			    uint16_t mod_id, struct bt_mesh_cfg_mod_pub *pub,
			    uint8_t *status)
{
	return mod_pub_get(net_idx, addr, elem_addr, mod_id, CID_NVAL,
			   pub, status);
}

int bt_mesh_cfg_mod_pub_get_vnd(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
				uint16_t mod_id, uint16_t cid,
				struct bt_mesh_cfg_mod_pub *pub, uint8_t *status)
{
	if (cid == CID_NVAL) {
		return -EINVAL;
	}

	return mod_pub_get(net_idx, addr, elem_addr, mod_id, cid, pub, status);
}

static int mod_pub_set(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
		       uint16_t mod_id, uint16_t cid,
		       struct bt_mesh_cfg_mod_pub *pub, uint8_t *status)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(OP_MOD_PUB_SET, 13);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct mod_pub_param param = {
		.mod_id = mod_id,
		.cid = cid,
		.elem_addr = elem_addr,
		.status = status,
		.pub = pub,
	};
	int err;

	err = cli_prepare(&param, OP_MOD_PUB_STATUS);
	if (err) {
		goto done;
	}

	bt_mesh_model_msg_init(msg, OP_MOD_PUB_SET);

	net_buf_simple_add_le16(msg, elem_addr);
	net_buf_simple_add_le16(msg, pub->addr);
	net_buf_simple_add_le16(msg, (pub->app_idx & (pub->cred_flag << 12)));
	net_buf_simple_add_u8(msg, pub->ttl);
	net_buf_simple_add_u8(msg, pub->period);
	net_buf_simple_add_u8(msg, pub->transmit);

	if (cid != CID_NVAL) {
		net_buf_simple_add_le16(msg, cid);
	}

	net_buf_simple_add_le16(msg, mod_id);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		goto done;
	}

	if (!status) {
		cli_reset();
		goto done;
	}

	err = cli_wait();
done:
	os_mbuf_free_chain(msg);
	return err;
}

int bt_mesh_cfg_mod_pub_set(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
			    uint16_t mod_id, struct bt_mesh_cfg_mod_pub *pub,
			    uint8_t *status)
{
	return mod_pub_set(net_idx, addr, elem_addr, mod_id, CID_NVAL,
			   pub, status);
}

int bt_mesh_cfg_mod_pub_set_vnd(uint16_t net_idx, uint16_t addr, uint16_t elem_addr,
				uint16_t mod_id, uint16_t cid,
				struct bt_mesh_cfg_mod_pub *pub, uint8_t *status)
{
	if (cid == CID_NVAL) {
		return -EINVAL;
	}

	return mod_pub_set(net_idx, addr, elem_addr, mod_id, cid, pub, status);
}

int bt_mesh_cfg_hb_sub_set(uint16_t net_idx, uint16_t addr,
			   struct bt_mesh_cfg_hb_sub *sub, uint8_t *status)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(OP_HEARTBEAT_SUB_SET, 5);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct hb_sub_param param = {
		.status = status,
		.sub = sub,
	};
	int err;

	err = cli_prepare(&param, OP_HEARTBEAT_SUB_STATUS);
	if (err) {
		goto done;
	}

	bt_mesh_model_msg_init(msg, OP_HEARTBEAT_SUB_SET);
	net_buf_simple_add_le16(msg, sub->src);
	net_buf_simple_add_le16(msg, sub->dst);
	net_buf_simple_add_u8(msg, sub->period);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		goto done;
	}

	if (!status) {
		cli_reset();
		goto done;
	}

	err = cli_wait();
done:
	os_mbuf_free_chain(msg);
	return err;
}

int bt_mesh_cfg_hb_sub_get(uint16_t net_idx, uint16_t addr,
			   struct bt_mesh_cfg_hb_sub *sub, uint8_t *status)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(OP_HEARTBEAT_SUB_GET, 0);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct hb_sub_param param = {
		.status = status,
		.sub = sub,
	};
	int err;

	err = cli_prepare(&param, OP_HEARTBEAT_SUB_STATUS);
	if (err) {
		goto done;
	}

	bt_mesh_model_msg_init(msg, OP_HEARTBEAT_SUB_GET);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		goto done;
	}

	if (!status) {
		cli_reset();
		goto done;
	}

	err = cli_wait();
done:
	os_mbuf_free_chain(msg);
	return err;
}

int bt_mesh_cfg_hb_pub_set(uint16_t net_idx, uint16_t addr,
			   const struct bt_mesh_cfg_hb_pub *pub, uint8_t *status)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(OP_HEARTBEAT_PUB_SET, 9);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV_REMOTE,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct hb_pub_param param = {
		.status = status,
	};
	int err;

	err = cli_prepare(&param, OP_HEARTBEAT_PUB_STATUS);
	if (err) {
		goto done;
	}

	bt_mesh_model_msg_init(msg, OP_HEARTBEAT_PUB_SET);
	net_buf_simple_add_le16(msg, pub->dst);
	net_buf_simple_add_u8(msg, pub->count);
	net_buf_simple_add_u8(msg, pub->period);
	net_buf_simple_add_u8(msg, pub->ttl);
	net_buf_simple_add_le16(msg, pub->feat);
	net_buf_simple_add_le16(msg, pub->net_idx);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		goto done;
	}

	if (!status) {
		cli_reset();
		goto done;
	}

	err = cli_wait();
done:
	os_mbuf_free_chain(msg);
	return err;
}

int bt_mesh_cfg_hb_pub_get(uint16_t net_idx, uint16_t addr,
			   struct bt_mesh_cfg_hb_pub *pub, uint8_t *status)
{
	struct os_mbuf *msg = BT_MESH_MODEL_BUF(OP_HEARTBEAT_PUB_GET, 0);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = BT_MESH_KEY_DEV,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct hb_pub_param param = {
		.status = status,
		.pub = pub,
	};
	int err;

	err = cli_prepare(&param, OP_HEARTBEAT_PUB_STATUS);
	if (err) {
		goto done;
	}

	bt_mesh_model_msg_init(msg, OP_HEARTBEAT_PUB_GET);

	err = bt_mesh_model_send(cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		cli_reset();
		goto done;
	}

	if (!status) {
		cli_reset();
		goto done;
	}

	err = cli_wait();
done:
	os_mbuf_free_chain(msg);
	return err;
}

int32_t bt_mesh_cfg_cli_timeout_get(void)
{
	return msg_timeout;
}

void bt_mesh_cfg_cli_timeout_set(int32_t timeout)
{
	msg_timeout = timeout;
}

#endif
#endif