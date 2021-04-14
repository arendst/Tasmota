/** @file
 *  @brief Bluetooth Mesh Health Client Model APIs.
 */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __BT_MESH_HEALTH_CLI_H
#define __BT_MESH_HEALTH_CLI_H

/**
 * @brief Bluetooth Mesh
 * @defgroup bt_mesh_health_cli Bluetooth Mesh Health Client Model
 * @ingroup bt_mesh
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/** Mesh Health Client Model Context */
struct bt_mesh_health_cli {
	struct bt_mesh_model *model;

	void (*current_status)(struct bt_mesh_health_cli *cli, u16_t addr,
			       u8_t test_id, u16_t cid, u8_t *faults,
			       size_t fault_count);

	struct k_sem          op_sync;
	u32_t                 op_pending;
	void                 *op_param;
};

extern const struct bt_mesh_model_op bt_mesh_health_cli_op[];

#define BT_MESH_MODEL_HEALTH_CLI(cli_data)                                   \
		BT_MESH_MODEL(BT_MESH_MODEL_ID_HEALTH_CLI,                   \
			      bt_mesh_health_cli_op, NULL, cli_data)

int bt_mesh_health_cli_set(struct bt_mesh_model *model);

int bt_mesh_health_fault_get(u16_t net_idx, u16_t addr, u16_t app_idx,
			     u16_t cid, u8_t *test_id, u8_t *faults,
			     size_t *fault_count);

int bt_mesh_health_fault_clear(u16_t net_idx, u16_t addr, u16_t app_idx,
			       u16_t cid, u8_t *test_id, u8_t *faults,
			       size_t *fault_count);

int bt_mesh_health_fault_test(u16_t net_idx, u16_t addr, u16_t app_idx,
			      u16_t cid, u8_t test_id, u8_t *faults,
			      size_t *fault_count);

int bt_mesh_health_period_get(u16_t net_idx, u16_t addr, u16_t app_idx,
			      u8_t *divisor);

int bt_mesh_health_period_set(u16_t net_idx, u16_t addr, u16_t app_idx,
			      u8_t divisor, u8_t *updated_divisor);

int bt_mesh_health_attention_get(u16_t net_idx, u16_t addr, u16_t app_idx,
				 u8_t *attention);

int bt_mesh_health_attention_set(u16_t net_idx, u16_t addr, u16_t app_idx,
				 u8_t attention, u8_t *updated_attention);

s32_t bt_mesh_health_cli_timeout_get(void);
void bt_mesh_health_cli_timeout_set(s32_t timeout);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __BT_MESH_HEALTH_CLI_H */
