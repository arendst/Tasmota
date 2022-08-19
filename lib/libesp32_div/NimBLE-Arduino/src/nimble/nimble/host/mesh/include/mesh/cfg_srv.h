/** @file
 *  @brief Bluetooth Mesh Configuration Server Model APIs.
 */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __BT_MESH_CFG_SRV_H
#define __BT_MESH_CFG_SRV_H

/**
 * @brief Bluetooth Mesh
 * @defgroup bt_mesh_cfg_srv Bluetooth Mesh Configuration Server Model
 * @ingroup bt_mesh
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif


extern const struct bt_mesh_model_op bt_mesh_cfg_srv_op[];
extern const struct bt_mesh_model_cb bt_mesh_cfg_srv_cb;

#define BT_MESH_MODEL_CFG_SRV                                  \
	BT_MESH_MODEL_CB(BT_MESH_MODEL_ID_CFG_SRV, bt_mesh_cfg_srv_op, NULL,   \
			 		 NULL, &bt_mesh_cfg_srv_cb)

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __BT_MESH_CFG_SRV_H */
