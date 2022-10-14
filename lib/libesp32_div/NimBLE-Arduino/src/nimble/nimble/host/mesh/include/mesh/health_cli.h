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

	void (*current_status)(struct bt_mesh_health_cli *cli, uint16_t addr,
			       uint8_t test_id, uint16_t cid, uint8_t *faults,
			       size_t fault_count);

	struct k_sem          op_sync;
	uint32_t                 op_pending;
	void                 *op_param;
};

extern const struct bt_mesh_model_op bt_mesh_health_cli_op[];
extern const struct bt_mesh_model_cb bt_mesh_health_cli_cb;

#define BT_MESH_MODEL_HEALTH_CLI(cli_data)                                     \
	BT_MESH_MODEL_CB(BT_MESH_MODEL_ID_HEALTH_CLI, bt_mesh_health_cli_op,   \
			 NULL, cli_data, &bt_mesh_health_cli_cb)

int bt_mesh_health_cli_set(struct bt_mesh_model *model);

int bt_mesh_health_fault_get(uint16_t addr, uint16_t app_idx, uint16_t cid,
							 uint8_t *test_id, uint8_t *faults,
							 size_t *fault_count);

int bt_mesh_health_fault_clear(uint16_t addr, uint16_t app_idx, uint16_t cid,
							   uint8_t *test_id, uint8_t *faults,
							   size_t *fault_count);

int bt_mesh_health_fault_test(uint16_t addr, uint16_t app_idx, uint16_t cid,
							  uint8_t test_id, uint8_t *faults,
							  size_t *fault_count);

int bt_mesh_health_period_get(uint16_t addr, uint16_t app_idx, uint8_t *divisor);

int bt_mesh_health_period_set(uint16_t addr, uint16_t app_idx, uint8_t divisor,
							  uint8_t *updated_divisor);

int bt_mesh_health_attention_get(uint16_t addr, uint16_t app_idx, uint8_t *attention);

int bt_mesh_health_attention_set(uint16_t addr, uint16_t app_idx, uint8_t attention,
								 uint8_t *updated_attention);

int32_t bt_mesh_health_cli_timeout_get(void);
void bt_mesh_health_cli_timeout_set(int32_t timeout);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __BT_MESH_HEALTH_CLI_H */
