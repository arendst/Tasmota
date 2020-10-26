/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __BT_MESH_LIGHT_MODEL_H
#define __BT_MESH_LIGHT_MODEL_H

#include "syscfg/syscfg.h"
#include "mesh/mesh.h"

int light_model_gen_onoff_get(struct bt_mesh_model *model, u8_t *state);
int light_model_gen_onoff_set(struct bt_mesh_model *model, u8_t state);
int light_model_gen_level_get(struct bt_mesh_model *model, s16_t *level);
int light_model_gen_level_set(struct bt_mesh_model *model, s16_t level);
int light_model_light_lightness_get(struct bt_mesh_model *model, s16_t *lightness);
int light_model_light_lightness_set(struct bt_mesh_model *model, s16_t lightness);

#endif
