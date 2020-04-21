/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __MODEL_SRV_H__
#define __MODEL_SRV_H__

#ifdef __cplusplus
extern "C" {
#endif

struct bt_mesh_gen_onoff_srv_cb {
    int (*get)(struct bt_mesh_model *model, u8_t *state);
    int (*set)(struct bt_mesh_model *model, u8_t state);
};

extern const struct bt_mesh_model_op gen_onoff_srv_op[];

#define BT_MESH_MODEL_GEN_ONOFF_SRV(srv, pub)		\
	BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_ONOFF_SRV,	\
		      gen_onoff_srv_op, pub, srv)

struct bt_mesh_gen_level_srv_cb {
    int (*get)(struct bt_mesh_model *model, s16_t *level);
    int (*set)(struct bt_mesh_model *model, s16_t level);
};

extern const struct bt_mesh_model_op gen_level_srv_op[];

#define BT_MESH_MODEL_GEN_LEVEL_SRV(srv, pub)		\
	BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_LEVEL_SRV,	\
		      gen_level_srv_op, pub, srv)

struct bt_mesh_light_lightness_srv_cb {
    int (*get)(struct bt_mesh_model *model, s16_t *level);
    int (*set)(struct bt_mesh_model *model, s16_t level);
};

extern const struct bt_mesh_model_op light_lightness_srv_op[];

#define BT_MESH_MODEL_LIGHT_LIGHTNESS_SRV(srv, pub)		\
	BT_MESH_MODEL(BT_MESH_MODEL_ID_LIGHT_LIGHTNESS_SRV,	\
		      light_lightness_srv_op, pub, srv)


void bt_mesh_set_gen_onoff_srv_cb(struct bt_mesh_gen_onoff_srv_cb *gen_onoff_cb);
void bt_mesh_set_gen_level_srv_cb(struct bt_mesh_gen_level_srv_cb *gen_level_cb);
void bt_mesh_set_light_lightness_srv_cb(struct bt_mesh_light_lightness_srv_cb *light_lightness_cb);

#ifdef __cplusplus
}
#endif

#endif /* __MODEL_SRV_H__ */
