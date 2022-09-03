/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __FUNDATION_H__
#define __FUNDATION_H__

#define OP_APP_KEY_ADD                     BT_MESH_MODEL_OP_1(0x00)
#define OP_APP_KEY_UPDATE                  BT_MESH_MODEL_OP_1(0x01)
#define OP_DEV_COMP_DATA_STATUS            BT_MESH_MODEL_OP_1(0x02)
#define OP_MOD_PUB_SET                     BT_MESH_MODEL_OP_1(0x03)
#define OP_HEALTH_CURRENT_STATUS           BT_MESH_MODEL_OP_1(0x04)
#define OP_HEALTH_FAULT_STATUS             BT_MESH_MODEL_OP_1(0x05)
#define OP_HEARTBEAT_PUB_STATUS            BT_MESH_MODEL_OP_1(0x06)
#define OP_APP_KEY_DEL                     BT_MESH_MODEL_OP_2(0x80, 0x00)
#define OP_APP_KEY_GET                     BT_MESH_MODEL_OP_2(0x80, 0x01)
#define OP_APP_KEY_LIST                    BT_MESH_MODEL_OP_2(0x80, 0x02)
#define OP_APP_KEY_STATUS                  BT_MESH_MODEL_OP_2(0x80, 0x03)
#define OP_ATTENTION_GET                   BT_MESH_MODEL_OP_2(0x80, 0x04)
#define OP_ATTENTION_SET                   BT_MESH_MODEL_OP_2(0x80, 0x05)
#define OP_ATTENTION_SET_UNREL             BT_MESH_MODEL_OP_2(0x80, 0x06)
#define OP_ATTENTION_STATUS                BT_MESH_MODEL_OP_2(0x80, 0x07)
#define OP_DEV_COMP_DATA_GET               BT_MESH_MODEL_OP_2(0x80, 0x08)
#define OP_BEACON_GET                      BT_MESH_MODEL_OP_2(0x80, 0x09)
#define OP_BEACON_SET                      BT_MESH_MODEL_OP_2(0x80, 0x0a)
#define OP_BEACON_STATUS                   BT_MESH_MODEL_OP_2(0x80, 0x0b)
#define OP_DEFAULT_TTL_GET                 BT_MESH_MODEL_OP_2(0x80, 0x0c)
#define OP_DEFAULT_TTL_SET                 BT_MESH_MODEL_OP_2(0x80, 0x0d)
#define OP_DEFAULT_TTL_STATUS              BT_MESH_MODEL_OP_2(0x80, 0x0e)
#define OP_FRIEND_GET                      BT_MESH_MODEL_OP_2(0x80, 0x0f)
#define OP_FRIEND_SET                      BT_MESH_MODEL_OP_2(0x80, 0x10)
#define OP_FRIEND_STATUS                   BT_MESH_MODEL_OP_2(0x80, 0x11)
#define OP_GATT_PROXY_GET                  BT_MESH_MODEL_OP_2(0x80, 0x12)
#define OP_GATT_PROXY_SET                  BT_MESH_MODEL_OP_2(0x80, 0x13)
#define OP_GATT_PROXY_STATUS               BT_MESH_MODEL_OP_2(0x80, 0x14)
#define OP_KRP_GET                         BT_MESH_MODEL_OP_2(0x80, 0x15)
#define OP_KRP_SET                         BT_MESH_MODEL_OP_2(0x80, 0x16)
#define OP_KRP_STATUS                      BT_MESH_MODEL_OP_2(0x80, 0x17)
#define OP_MOD_PUB_GET                     BT_MESH_MODEL_OP_2(0x80, 0x18)
#define OP_MOD_PUB_STATUS                  BT_MESH_MODEL_OP_2(0x80, 0x19)
#define OP_MOD_PUB_VA_SET                  BT_MESH_MODEL_OP_2(0x80, 0x1a)
#define OP_MOD_SUB_ADD                     BT_MESH_MODEL_OP_2(0x80, 0x1b)
#define OP_MOD_SUB_DEL                     BT_MESH_MODEL_OP_2(0x80, 0x1c)
#define OP_MOD_SUB_DEL_ALL                 BT_MESH_MODEL_OP_2(0x80, 0x1d)
#define OP_MOD_SUB_OVERWRITE               BT_MESH_MODEL_OP_2(0x80, 0x1e)
#define OP_MOD_SUB_STATUS                  BT_MESH_MODEL_OP_2(0x80, 0x1f)
#define OP_MOD_SUB_VA_ADD                  BT_MESH_MODEL_OP_2(0x80, 0x20)
#define OP_MOD_SUB_VA_DEL                  BT_MESH_MODEL_OP_2(0x80, 0x21)
#define OP_MOD_SUB_VA_OVERWRITE            BT_MESH_MODEL_OP_2(0x80, 0x22)
#define OP_NET_TRANSMIT_GET                BT_MESH_MODEL_OP_2(0x80, 0x23)
#define OP_NET_TRANSMIT_SET                BT_MESH_MODEL_OP_2(0x80, 0x24)
#define OP_NET_TRANSMIT_STATUS             BT_MESH_MODEL_OP_2(0x80, 0x25)
#define OP_RELAY_GET                       BT_MESH_MODEL_OP_2(0x80, 0x26)
#define OP_RELAY_SET                       BT_MESH_MODEL_OP_2(0x80, 0x27)
#define OP_RELAY_STATUS                    BT_MESH_MODEL_OP_2(0x80, 0x28)
#define OP_MOD_SUB_GET                     BT_MESH_MODEL_OP_2(0x80, 0x29)
#define OP_MOD_SUB_LIST                    BT_MESH_MODEL_OP_2(0x80, 0x2a)
#define OP_MOD_SUB_GET_VND                 BT_MESH_MODEL_OP_2(0x80, 0x2b)
#define OP_MOD_SUB_LIST_VND                BT_MESH_MODEL_OP_2(0x80, 0x2c)
#define OP_LPN_TIMEOUT_GET                 BT_MESH_MODEL_OP_2(0x80, 0x2d)
#define OP_LPN_TIMEOUT_STATUS              BT_MESH_MODEL_OP_2(0x80, 0x2e)
#define OP_HEALTH_FAULT_CLEAR              BT_MESH_MODEL_OP_2(0x80, 0x2f)
#define OP_HEALTH_FAULT_CLEAR_UNREL        BT_MESH_MODEL_OP_2(0x80, 0x30)
#define OP_HEALTH_FAULT_GET                BT_MESH_MODEL_OP_2(0x80, 0x31)
#define OP_HEALTH_FAULT_TEST               BT_MESH_MODEL_OP_2(0x80, 0x32)
#define OP_HEALTH_FAULT_TEST_UNREL         BT_MESH_MODEL_OP_2(0x80, 0x33)
#define OP_HEALTH_PERIOD_GET               BT_MESH_MODEL_OP_2(0x80, 0x34)
#define OP_HEALTH_PERIOD_SET               BT_MESH_MODEL_OP_2(0x80, 0x35)
#define OP_HEALTH_PERIOD_SET_UNREL         BT_MESH_MODEL_OP_2(0x80, 0x36)
#define OP_HEALTH_PERIOD_STATUS            BT_MESH_MODEL_OP_2(0x80, 0x37)
#define OP_HEARTBEAT_PUB_GET               BT_MESH_MODEL_OP_2(0x80, 0x38)
#define OP_HEARTBEAT_PUB_SET               BT_MESH_MODEL_OP_2(0x80, 0x39)
#define OP_HEARTBEAT_SUB_GET               BT_MESH_MODEL_OP_2(0x80, 0x3a)
#define OP_HEARTBEAT_SUB_SET               BT_MESH_MODEL_OP_2(0x80, 0x3b)
#define OP_HEARTBEAT_SUB_STATUS            BT_MESH_MODEL_OP_2(0x80, 0x3c)
#define OP_MOD_APP_BIND                    BT_MESH_MODEL_OP_2(0x80, 0x3d)
#define OP_MOD_APP_STATUS                  BT_MESH_MODEL_OP_2(0x80, 0x3e)
#define OP_MOD_APP_UNBIND                  BT_MESH_MODEL_OP_2(0x80, 0x3f)
#define OP_NET_KEY_ADD                     BT_MESH_MODEL_OP_2(0x80, 0x40)
#define OP_NET_KEY_DEL                     BT_MESH_MODEL_OP_2(0x80, 0x41)
#define OP_NET_KEY_GET                     BT_MESH_MODEL_OP_2(0x80, 0x42)
#define OP_NET_KEY_LIST                    BT_MESH_MODEL_OP_2(0x80, 0x43)
#define OP_NET_KEY_STATUS                  BT_MESH_MODEL_OP_2(0x80, 0x44)
#define OP_NET_KEY_UPDATE                  BT_MESH_MODEL_OP_2(0x80, 0x45)
#define OP_NODE_IDENTITY_GET               BT_MESH_MODEL_OP_2(0x80, 0x46)
#define OP_NODE_IDENTITY_SET               BT_MESH_MODEL_OP_2(0x80, 0x47)
#define OP_NODE_IDENTITY_STATUS            BT_MESH_MODEL_OP_2(0x80, 0x48)
#define OP_NODE_RESET                      BT_MESH_MODEL_OP_2(0x80, 0x49)
#define OP_NODE_RESET_STATUS               BT_MESH_MODEL_OP_2(0x80, 0x4a)
#define OP_SIG_MOD_APP_GET                 BT_MESH_MODEL_OP_2(0x80, 0x4b)
#define OP_SIG_MOD_APP_LIST                BT_MESH_MODEL_OP_2(0x80, 0x4c)
#define OP_VND_MOD_APP_GET                 BT_MESH_MODEL_OP_2(0x80, 0x4d)
#define OP_VND_MOD_APP_LIST                BT_MESH_MODEL_OP_2(0x80, 0x4e)

#define STATUS_SUCCESS                     0x00
#define STATUS_INVALID_ADDRESS             0x01
#define STATUS_INVALID_MODEL               0x02
#define STATUS_INVALID_APPKEY              0x03
#define STATUS_INVALID_NETKEY              0x04
#define STATUS_INSUFF_RESOURCES            0x05
#define STATUS_IDX_ALREADY_STORED          0x06
#define STATUS_NVAL_PUB_PARAM              0x07
#define STATUS_NOT_SUB_MOD                 0x08
#define STATUS_STORAGE_FAIL                0x09
#define STATUS_FEAT_NOT_SUPP               0x0a
#define STATUS_CANNOT_UPDATE               0x0b
#define STATUS_CANNOT_REMOVE               0x0c
#define STATUS_CANNOT_BIND                 0x0d
#define STATUS_TEMP_STATE_CHG_FAIL         0x0e
#define STATUS_CANNOT_SET                  0x0f
#define STATUS_UNSPECIFIED                 0x10
#define STATUS_INVALID_BINDING             0x11

void bt_mesh_cfg_reset(void);

void bt_mesh_attention(struct bt_mesh_model *model, uint8_t time);

static inline void key_idx_pack(struct os_mbuf *buf,
				uint16_t idx1, uint16_t idx2)
{
	net_buf_simple_add_le16(buf, idx1 | ((idx2 & 0x00f) << 12));
	net_buf_simple_add_u8(buf, idx2 >> 4);
}

static inline void key_idx_unpack(struct os_mbuf *buf,
				  uint16_t *idx1, uint16_t *idx2)
{
	*idx1 = sys_get_le16(&buf->om_data[0]) & 0xfff;
	*idx2 = sys_get_le16(&buf->om_data[1]) >> 4;
	net_buf_simple_pull_mem(buf, 3);
}

#endif
