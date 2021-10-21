/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __MODEL_CLI_H__
#define __MODEL_CLI_H__

#ifdef __cplusplus
extern "C" {
#endif

struct bt_mesh_gen_model_cli {
    struct bt_mesh_model *model;

    struct k_sem          op_sync;
    u32_t                 op_pending;
    void                 *op_param;
};

extern const struct bt_mesh_model_op gen_onoff_cli_op[];
extern const struct bt_mesh_model_cb bt_mesh_gen_onoff_cli_cb;

#define BT_MESH_MODEL_GEN_ONOFF_CLI(cli_data, pub)                             \
	BT_MESH_MODEL_CB(BT_MESH_MODEL_ID_GEN_ONOFF_CLI, gen_onoff_cli_op, pub,\
			 cli_data, &bt_mesh_gen_onoff_cli_cb)

extern const struct bt_mesh_model_op gen_level_cli_op[];
extern const struct bt_mesh_model_cb bt_mesh_gen_level_cli_cb;

#define BT_MESH_MODEL_GEN_LEVEL_CLI(cli_data, pub)                             \
	BT_MESH_MODEL_CB(BT_MESH_MODEL_ID_GEN_LEVEL_CLI, gen_level_cli_op, pub,\
			 cli_data, &bt_mesh_gen_level_cli_cb)

int bt_mesh_gen_onoff_get(u16_t net_idx, u16_t addr, u16_t app_idx,
			  u8_t *state);
int bt_mesh_gen_onoff_set(u16_t net_idx, u16_t addr, u16_t app_idx,
			  u8_t val, u8_t *state);
int bt_mesh_gen_level_get(u16_t net_idx, u16_t addr, u16_t app_idx,
			  s16_t *level);
int bt_mesh_gen_level_set(u16_t net_idx, u16_t addr, u16_t app_idx,
			  s16_t val, s16_t *state);

#ifdef __cplusplus
}
#endif

#endif /* __MODEL_CLI_H__ */
