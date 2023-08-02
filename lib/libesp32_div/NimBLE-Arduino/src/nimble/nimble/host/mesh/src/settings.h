/*
 * Copyright (c) 2018 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

void bt_mesh_store_net(void);
void bt_mesh_store_iv(bool only_duration);
void bt_mesh_store_seq(void);
void bt_mesh_store_rpl(struct bt_mesh_rpl *rpl);
void bt_mesh_store_subnet(uint16_t net_idx);
void bt_mesh_store_app_key(uint16_t app_idx);
void bt_mesh_store_hb_pub(void);
void bt_mesh_store_cfg(void);
void bt_mesh_store_mod_bind(struct bt_mesh_model *mod);
void bt_mesh_store_mod_sub(struct bt_mesh_model *mod);
void bt_mesh_store_mod_pub(struct bt_mesh_model *mod);
void bt_mesh_store_label(void);
void bt_mesh_store_cdb(void);
void bt_mesh_store_cdb_node(const struct bt_mesh_cdb_node *node);
void bt_mesh_store_cdb_subnet(const struct bt_mesh_cdb_subnet *sub);
void bt_mesh_store_cdb_app_key(const struct bt_mesh_cdb_app_key *app);

void bt_mesh_clear_net(void);
void bt_mesh_clear_subnet(uint16_t net_idx);
void bt_mesh_clear_app_key(uint16_t app_idx);
void bt_mesh_clear_rpl(void);
void bt_mesh_clear_cdb_node(struct bt_mesh_cdb_node *node);
void bt_mesh_clear_cdb_subnet(struct bt_mesh_cdb_subnet *sub);
void bt_mesh_clear_cdb_app_key(struct bt_mesh_cdb_app_key *app);

void bt_mesh_settings_init(void);
