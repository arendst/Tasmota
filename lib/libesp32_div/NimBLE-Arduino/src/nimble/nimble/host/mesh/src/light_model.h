/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __BT_MESH_LIGHT_MODEL_H
#define __BT_MESH_LIGHT_MODEL_H

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "../include/mesh/mesh.h"

int light_model_gen_onoff_get(struct bt_mesh_model *model, uint8_t *state);
int light_model_gen_onoff_set(struct bt_mesh_model *model, uint8_t state);
int light_model_gen_level_get(struct bt_mesh_model *model, int16_t *level);
int light_model_gen_level_set(struct bt_mesh_model *model, int16_t level);
int light_model_light_lightness_get(struct bt_mesh_model *model, int16_t *lightness);
int light_model_light_lightness_set(struct bt_mesh_model *model, int16_t lightness);

#endif
