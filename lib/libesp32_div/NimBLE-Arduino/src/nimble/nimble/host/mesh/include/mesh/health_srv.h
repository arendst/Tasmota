/** @file
 *  @brief Bluetooth Mesh Health Server Model APIs.
 */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __BT_MESH_HEALTH_SRV_H
#define __BT_MESH_HEALTH_SRV_H

/**
 * @brief Mesh Bluetooth Mesh Health Server Model
 * @defgroup bt_mesh_health_srv
 * @ingroup bt_mesh
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

struct bt_mesh_health_srv_cb {
	/* Fetch current faults */
	int (*fault_get_cur)(struct bt_mesh_model *model, uint8_t *test_id,
			     uint16_t *company_id, uint8_t *faults,
			     uint8_t *fault_count);

	/* Fetch registered faults */
	int (*fault_get_reg)(struct bt_mesh_model *model, uint16_t company_id,
			     uint8_t *test_id, uint8_t *faults,
			     uint8_t *fault_count);

	/* Clear registered faults */
	int (*fault_clear)(struct bt_mesh_model *model, uint16_t company_id);

	/* Run a specific test */
	int (*fault_test)(struct bt_mesh_model *model, uint8_t test_id,
			  uint16_t company_id);

	/* Attention on */
	void (*attn_on)(struct bt_mesh_model *model);

	/* Attention off */
	void (*attn_off)(struct bt_mesh_model *model);
};

/** @def BT_MESH_HEALTH_FAULT_MSG
 *
 *  A helper to define a health fault message.
 *
 *  @param max_faults Maximum number of faults the element can have.
 *
 *  @return a New os_mbuf of the needed size.
 */
#define BT_MESH_HEALTH_FAULT_MSG(max_faults) \
	NET_BUF_SIMPLE(1 + 3 + (max_faults))

/** Mesh Health Server Model Context */
struct bt_mesh_health_srv {
	struct bt_mesh_model *model;

	/* Optional callback struct */
	const struct bt_mesh_health_srv_cb *cb;

	/* Attention Timer state */
	struct k_delayed_work attn_timer;
};

int bt_mesh_fault_update(struct bt_mesh_elem *elem);

extern const struct bt_mesh_model_op bt_mesh_health_srv_op[];
extern const struct bt_mesh_model_cb bt_mesh_health_srv_cb;

/** @def BT_MESH_MODEL_HEALTH_SRV
 *
 *  Define a new health server model. Note that this API needs to be
 *  repeated for each element that the application wants to have a
 *  health server model on. Each instance also needs a unique
 *  bt_mesh_health_srv and bt_mesh_model_pub context.
 *
 *  @param srv Pointer to a unique struct bt_mesh_health_srv.
 *  @param pub Pointer to a unique struct bt_mesh_model_pub.
 *
 *  @return New mesh model instance.
 */
#define BT_MESH_MODEL_HEALTH_SRV(srv, pub)                                     \
	BT_MESH_MODEL_CB(BT_MESH_MODEL_ID_HEALTH_SRV, bt_mesh_health_srv_op,   \
			 pub, srv, &bt_mesh_health_srv_cb)

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __BT_MESH_HEALTH_SRV_H */
