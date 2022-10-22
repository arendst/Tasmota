/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#if MYNEWT_VAL(BLE_MESH)

#define MESH_LOG_MODULE BLE_MESH_MODEL_LOG

#include "../include/mesh/mesh.h"
#include "../include/mesh/model_cli.h"
#include "mesh_priv.h"

static int32_t msg_timeout = K_SECONDS(5);

static struct bt_mesh_gen_model_cli *gen_onoff_cli;
static struct bt_mesh_gen_model_cli *gen_level_cli;

static uint8_t transaction_id = 0;

struct gen_onoff_param {
    uint8_t *state;
};

struct gen_level_param {
    int16_t *level;
};

static void gen_onoff_status(struct bt_mesh_model *model,
			     struct bt_mesh_msg_ctx *ctx,
			     struct os_mbuf *buf)
{
	struct bt_mesh_gen_model_cli *cli = model->user_data;
	struct gen_onoff_param *param;
	uint8_t state;


	BT_DBG("net_idx 0x%04x app_idx 0x%04x src 0x%04x len %u: %s",
	       ctx->net_idx, ctx->app_idx, ctx->addr, buf->om_len,
	       bt_hex(buf->om_data, buf->om_len));

	if (cli->op_pending != OP_GEN_ONOFF_STATUS) {
		BT_WARN("Unexpected Generic OnOff Status message");
		return;
	}

	param = cli->op_param;

	state = net_buf_simple_pull_u8(buf);
	if (param->state) {
		*param->state = state;
	}

	BT_DBG("state: %d", state);

	k_sem_give(&cli->op_sync);
}

static void gen_level_status(struct bt_mesh_model *model,
			     struct bt_mesh_msg_ctx *ctx,
			     struct os_mbuf *buf)
{
	struct bt_mesh_gen_model_cli *cli = model->user_data;
	struct gen_level_param *param;
	int16_t level;


	BT_DBG("net_idx 0x%04x app_idx 0x%04x src 0x%04x len %u: %s",
	       ctx->net_idx, ctx->app_idx, ctx->addr, buf->om_len,
	       bt_hex(buf->om_data, buf->om_len));

	if (cli->op_pending != OP_GEN_LEVEL_STATUS) {
		BT_WARN("Unexpected Generic LEVEL Status message");
		return;
	}

	param = cli->op_param;

	level = net_buf_simple_pull_le16(buf);
	if (param->level) {
		*param->level = level;
	}

	BT_DBG("level: %d", level);

	k_sem_give(&cli->op_sync);
}

const struct bt_mesh_model_op gen_onoff_cli_op[] = {
	{ OP_GEN_ONOFF_STATUS, 1, gen_onoff_status },
	BT_MESH_MODEL_OP_END,
};

static int onoff_cli_init(struct bt_mesh_model *model)
{
	BT_DBG("");

	if (!model->user_data) {
		BT_ERR("No Generic OnOff Client context provided");
		return -EINVAL;
	}

	gen_onoff_cli = model->user_data;
	gen_onoff_cli->model = model;

	k_sem_init(&gen_onoff_cli->op_sync, 0, 1);

	return 0;
}

const struct bt_mesh_model_cb bt_mesh_gen_onoff_cli_cb = {
	.init = onoff_cli_init,
};

const struct bt_mesh_model_op gen_level_cli_op[] = {
	{ OP_GEN_LEVEL_STATUS, 2, gen_level_status },
	BT_MESH_MODEL_OP_END,
};

static int level_cli_init(struct bt_mesh_model *model)
{
	BT_DBG("");

	if (!model->user_data) {
		BT_ERR("No Generic Level Client context provided");
		return -EINVAL;
	}

	gen_level_cli = model->user_data;
	gen_level_cli->model = model;

	k_sem_init(&gen_level_cli->op_sync, 0, 1);

	return 0;
}

const struct bt_mesh_model_cb bt_mesh_gen_level_cli_cb = {
	.init = level_cli_init,
};

static int cli_wait(struct bt_mesh_gen_model_cli *cli, void *param, uint32_t op)
{
	int err;

	BT_DBG("");

	cli->op_param = param;
	cli->op_pending = op;

	err = k_sem_take(&cli->op_sync, msg_timeout);

	cli->op_pending = 0;
	cli->op_param = NULL;

	return err;
}

int bt_mesh_gen_onoff_get(uint16_t net_idx, uint16_t addr, uint16_t app_idx,
			  uint8_t *state)
{
	struct os_mbuf *msg = NET_BUF_SIMPLE(2 + 0 + 4);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = app_idx,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct gen_onoff_param param = {
		.state = state,
	};
	int err;

	bt_mesh_model_msg_init(msg, OP_GEN_ONOFF_GET);

	err = bt_mesh_model_send(gen_onoff_cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		goto done;
	}

	err = cli_wait(gen_onoff_cli, &param, OP_GEN_ONOFF_STATUS);
done:
	os_mbuf_free_chain(msg);
	return err;
}

int bt_mesh_gen_onoff_set(uint16_t net_idx, uint16_t addr, uint16_t app_idx,
			  uint8_t val, uint8_t *state)
{
	struct os_mbuf *msg = NET_BUF_SIMPLE(2 + 2 + 4);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = app_idx,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct gen_onoff_param param = {
		.state = state,
	};
	int err;

	if (state) {
		bt_mesh_model_msg_init(msg, OP_GEN_ONOFF_SET);
	} else {
		bt_mesh_model_msg_init(msg, OP_GEN_ONOFF_SET_UNACK);
	}

	net_buf_simple_add_u8(msg, val);
	net_buf_simple_add_u8(msg, transaction_id);

	err = bt_mesh_model_send(gen_onoff_cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		goto done;
	}

	if (!state) {
		goto done;
	}

	err = cli_wait(gen_onoff_cli, &param, OP_GEN_ONOFF_STATUS);
done:
	if (err == 0) {
		transaction_id++;
	}
	os_mbuf_free_chain(msg);
	return err;
}

int bt_mesh_gen_level_get(uint16_t net_idx, uint16_t addr, uint16_t app_idx,
			  int16_t *level)
{
	struct os_mbuf *msg = NET_BUF_SIMPLE(2 + 0 + 4);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = app_idx,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct gen_level_param param = {
		.level = level,
	};
	int err;

	bt_mesh_model_msg_init(msg, OP_GEN_LEVEL_GET);

	err = bt_mesh_model_send(gen_level_cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		goto done;
	}

	err = cli_wait(gen_level_cli, &param, OP_GEN_LEVEL_STATUS);
done:
	os_mbuf_free_chain(msg);
	return err;
}

int bt_mesh_gen_level_set(uint16_t net_idx, uint16_t addr, uint16_t app_idx,
			  int16_t val, int16_t *state)
{
	struct os_mbuf *msg = NET_BUF_SIMPLE(2 + 3 + 4);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = app_idx,
		.addr = addr,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	struct gen_level_param param = {
		.level = state,
	};
	int err;

	if (state) {
		bt_mesh_model_msg_init(msg, OP_GEN_LEVEL_SET);
	} else {
		bt_mesh_model_msg_init(msg, OP_GEN_LEVEL_SET_UNACK);
	}

	net_buf_simple_add_le16(msg, val);
	net_buf_simple_add_u8(msg, transaction_id);

	err = bt_mesh_model_send(gen_level_cli->model, &ctx, msg, NULL, NULL);
	if (err) {
		BT_ERR("model_send() failed (err %d)", err);
		goto done;
	}

	if (!state) {
		goto done;
	}

	err = cli_wait(gen_level_cli, &param, OP_GEN_LEVEL_STATUS);
done:
	if (err == 0) {
		transaction_id++;
	}
	os_mbuf_free_chain(msg);
	return err;
}
#endif
