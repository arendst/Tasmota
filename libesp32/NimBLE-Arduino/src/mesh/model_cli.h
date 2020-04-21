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

extern struct bt_mesh_gen_model_cli gen_onoff_cli;
extern const struct bt_mesh_model_op gen_onoff_cli_op[];

#define BT_MESH_MODEL_GEN_ONOFF_CLI()			\
	BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_ONOFF_CLI,	\
		      gen_onoff_cli_op, NULL, &gen_onoff_cli)

extern struct bt_mesh_gen_model_cli gen_level_cli;
extern const struct bt_mesh_model_op gen_level_cli_op[];

#define BT_MESH_MODEL_GEN_LEVEL_CLI(pub) 		\
	BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_LEVEL_CLI, 	\
		      gen_level_cli_op, NULL, &gen_level_cli)


int bt_mesh_gen_onoff_get(u16_t net_idx, u16_t addr, u16_t app_idx,
			  u8_t *state);
int bt_mesh_gen_onoff_set(u16_t net_idx, u16_t addr, u16_t app_idx,
			  u8_t val, u8_t *state);
int bt_mesh_gen_level_get(u16_t net_idx, u16_t addr, u16_t app_idx,
			  s16_t *level);
int bt_mesh_gen_level_set(u16_t net_idx, u16_t addr, u16_t app_idx,
			  s16_t val, s16_t *state);
int bt_mesh_gen_model_cli_init(struct bt_mesh_model *model, bool primary);

#ifdef __cplusplus
}
#endif

#endif /* __MODEL_CLI_H__ */
