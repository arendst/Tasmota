/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "syscfg/syscfg.h"
#define MESH_LOG_MODULE BLE_MESH_MODEL_LOG

#include "mesh/mesh.h"
#include "mesh/model_srv.h"
#include "mesh_priv.h"

static struct bt_mesh_gen_onoff_srv *gen_onoff_srv;
static struct bt_mesh_gen_level_srv *gen_level_srv;
static struct bt_mesh_light_lightness_srv *light_lightness_srv;

static void gen_onoff_status(struct bt_mesh_model *model,
			     struct bt_mesh_msg_ctx *ctx)
{
	struct bt_mesh_gen_onoff_srv *cb = model->user_data;
	struct os_mbuf *msg = NET_BUF_SIMPLE(3);
	u8_t *state;

	bt_mesh_model_msg_init(msg, OP_GEN_ONOFF_STATUS);
	state = net_buf_simple_add(msg, 1);
	if (cb && cb->get) {
		cb->get(model, state);
	}

	BT_DBG("state: %d", *state);

	if (bt_mesh_model_send(model, ctx, msg, NULL, NULL)) {
		BT_ERR("Send status failed");
	}

	os_mbuf_free_chain(msg);
}

static void gen_onoff_get(struct bt_mesh_model *model,
			  struct bt_mesh_msg_ctx *ctx,
			  struct os_mbuf *buf)
{
	BT_DBG("");

	gen_onoff_status(model, ctx);
}

static void gen_onoff_set_unack(struct bt_mesh_model *model,
				struct bt_mesh_msg_ctx *ctx,
				struct os_mbuf *buf)
{
	struct bt_mesh_gen_onoff_srv *cb = model->user_data;
	u8_t state;

	state = buf->om_data[0];

	BT_DBG("state: %d", state);

	if (cb && cb->set) {
		cb->set(model, state);
	}
}

static void gen_onoff_set(struct bt_mesh_model *model,
			  struct bt_mesh_msg_ctx *ctx,
			  struct os_mbuf *buf)
{
	BT_DBG("");

	gen_onoff_set_unack(model, ctx, buf);
	gen_onoff_status(model, ctx);
}

static void gen_level_status(struct bt_mesh_model *model,
			     struct bt_mesh_msg_ctx *ctx)
{
	struct bt_mesh_gen_level_srv *cb = model->user_data;
	struct os_mbuf *msg = NET_BUF_SIMPLE(4);
	s16_t *level;

	bt_mesh_model_msg_init(msg, OP_GEN_LEVEL_STATUS);
	level = net_buf_simple_add(msg, 2);
	if (cb && cb->get) {
		cb->get(model, level);
	}

	BT_DBG("level: %d", *level);

	if (bt_mesh_model_send(model, ctx, msg, NULL, NULL)) {
		BT_ERR("Send status failed");
	}

	os_mbuf_free_chain(msg);
}

static void gen_level_get(struct bt_mesh_model *model,
			  struct bt_mesh_msg_ctx *ctx,
			  struct os_mbuf *buf)
{
	BT_DBG("");

	gen_level_status(model, ctx);
}

static void gen_level_set_unack(struct bt_mesh_model *model,
				struct bt_mesh_msg_ctx *ctx,
				struct os_mbuf *buf) {
	struct bt_mesh_gen_level_srv *cb = model->user_data;
	s16_t level;

	level = (s16_t) net_buf_simple_pull_le16(buf);
	BT_DBG("level: %d", level);

	if (cb && cb->set) {
		cb->set(model, level);
	}
}

static void gen_level_set(struct bt_mesh_model *model,
			  struct bt_mesh_msg_ctx *ctx,
			  struct os_mbuf *buf)
{
	gen_level_set_unack(model, ctx, buf);
	gen_level_status(model, ctx);
}

static void light_lightness_status(struct bt_mesh_model *model,
			     struct bt_mesh_msg_ctx *ctx)
{
	struct bt_mesh_light_lightness_srv *cb = model->user_data;
	struct os_mbuf *msg = NET_BUF_SIMPLE(4);
	s16_t *lightness;

	bt_mesh_model_msg_init(msg, OP_LIGHT_LIGHTNESS_STATUS);
	lightness = net_buf_simple_add(msg, 2);
	if (cb && cb->get) {
		cb->get(model, lightness);
	}

	BT_DBG("lightness: %d", *lightness);

	if (bt_mesh_model_send(model, ctx, msg, NULL, NULL)) {
		BT_ERR("Send status failed");
	}

	os_mbuf_free_chain(msg);
}

static void light_lightness_get(struct bt_mesh_model *model,
			  struct bt_mesh_msg_ctx *ctx,
			  struct os_mbuf *buf)
{
	BT_DBG("");

	light_lightness_status(model, ctx);
}

static void light_lightness_set_unack(struct bt_mesh_model *model,
				struct bt_mesh_msg_ctx *ctx,
				struct os_mbuf *buf) {
	struct bt_mesh_light_lightness_srv *cb = model->user_data;
	s16_t lightness;

	lightness = (s16_t) net_buf_simple_pull_le16(buf);
	BT_DBG("lightness: %d", lightness);

	if (cb && cb->set) {
		cb->set(model, lightness);
	}
}

static void light_lightness_set(struct bt_mesh_model *model,
			  struct bt_mesh_msg_ctx *ctx,
			  struct os_mbuf *buf)
{
	light_lightness_set_unack(model, ctx, buf);
	light_lightness_status(model, ctx);
}

const struct bt_mesh_model_op gen_onoff_srv_op[] = {
	{ OP_GEN_ONOFF_GET, 		0, gen_onoff_get },
	{ OP_GEN_ONOFF_SET, 		2, gen_onoff_set },
	{ OP_GEN_ONOFF_SET_UNACK, 	2, gen_onoff_set_unack },
	BT_MESH_MODEL_OP_END,
};

const struct bt_mesh_model_op gen_level_srv_op[] = {
	{ OP_GEN_LEVEL_GET, 		0, gen_level_get },
	{ OP_GEN_LEVEL_SET, 		3, gen_level_set },
	{ OP_GEN_LEVEL_SET_UNACK, 	3, gen_level_set_unack },
	BT_MESH_MODEL_OP_END,
};

const struct bt_mesh_model_op light_lightness_srv_op[] = {
	{ OP_LIGHT_LIGHTNESS_GET, 		0, light_lightness_get },
	{ OP_LIGHT_LIGHTNESS_SET, 		3, light_lightness_set },
	{ OP_LIGHT_LIGHTNESS_SET_UNACK, 	3, light_lightness_set_unack },
	BT_MESH_MODEL_OP_END,
};

static int onoff_srv_init(struct bt_mesh_model *model)
{
	struct bt_mesh_gen_onoff_srv *cfg = model->user_data;

	BT_DBG("");

	if (!cfg) {
		BT_ERR("No Generic OnOff Server context provided");
		return -EINVAL;
	}

	cfg->model = model;

	gen_onoff_srv = cfg;

	return 0;
}

const struct bt_mesh_model_cb gen_onoff_srv_cb = {
	.init = onoff_srv_init,
};

static int level_srv_init(struct bt_mesh_model *model)
{
	struct bt_mesh_gen_level_srv *cfg = model->user_data;

	BT_DBG("");

	if (!cfg) {
		BT_ERR("No Generic Level Server context provided");
		return -EINVAL;
	}

	cfg->model = model;

	gen_level_srv = cfg;

	return 0;
}

const struct bt_mesh_model_cb gen_level_srv_cb = {
	.init = level_srv_init,
};

static int lightness_srv_init(struct bt_mesh_model *model)
{
	struct bt_mesh_light_lightness_srv *cfg = model->user_data;

	BT_DBG("");

	if (!cfg) {
		BT_ERR("No Light Lightness Server context provided");
		return -EINVAL;
	}

	cfg->model = model;

	light_lightness_srv = cfg;

	return 0;
}

const struct bt_mesh_model_cb light_lightness_srv_cb = {
	.init = lightness_srv_init,
};
