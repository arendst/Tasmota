/** @file
 *  @brief Bluetooth Mesh Access Layer APIs.
 */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __BT_MESH_ACCESS_H
#define __BT_MESH_ACCESS_H

/**
 * @brief Bluetooth Mesh Access Layer
 * @defgroup bt_mesh_access Bluetooth Mesh Access Layer
 * @ingroup bt_mesh
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#define BT_MESH_ADDR_UNASSIGNED   0x0000
#define BT_MESH_ADDR_ALL_NODES    0xffff
#define BT_MESH_ADDR_PROXIES      0xfffc
#define BT_MESH_ADDR_FRIENDS      0xfffd
#define BT_MESH_ADDR_RELAYS       0xfffe

#define BT_MESH_KEY_UNUSED        0xffff
#define BT_MESH_KEY_DEV           0xfffe

/** Helper to define a mesh element within an array.
 *
 *  In case the element has no SIG or Vendor models the helper
 *  macro BT_MESH_MODEL_NONE can be given instead.
 *
 *  @param _loc       Location Descriptor.
 *  @param _mods      Array of models.
 *  @param _vnd_mods  Array of vendor models.
 */
#define BT_MESH_ELEM(_loc, _mods, _vnd_mods)        \
{                                                   \
	.loc              = (_loc),                 \
	.model_count      = ARRAY_SIZE(_mods),      \
	.models           = (_mods),                \
	.vnd_model_count  = ARRAY_SIZE(_vnd_mods),  \
	.vnd_models       = (_vnd_mods),            \
}

/** Abstraction that describes a Mesh Element */
struct bt_mesh_elem {
	/* Unicast Address. Set at runtime during provisioning. */
	u16_t addr;

	/* Location Descriptor (GATT Bluetooth Namespace Descriptors) */
	const u16_t loc;

	const u8_t model_count;
	const u8_t vnd_model_count;

	struct bt_mesh_model * const models;
	struct bt_mesh_model * const vnd_models;
};

/* Foundation Models */
#define BT_MESH_MODEL_ID_CFG_SRV                   0x0000
#define BT_MESH_MODEL_ID_CFG_CLI                   0x0001
#define BT_MESH_MODEL_ID_HEALTH_SRV                0x0002
#define BT_MESH_MODEL_ID_HEALTH_CLI                0x0003

/* Models from the Mesh Model Specification */
#define BT_MESH_MODEL_ID_GEN_ONOFF_SRV             0x1000
#define BT_MESH_MODEL_ID_GEN_ONOFF_CLI             0x1001
#define BT_MESH_MODEL_ID_GEN_LEVEL_SRV             0x1002
#define BT_MESH_MODEL_ID_GEN_LEVEL_CLI             0x1003
#define BT_MESH_MODEL_ID_GEN_DEF_TRANS_TIME_SRV    0x1004
#define BT_MESH_MODEL_ID_GEN_DEF_TRANS_TIME_CLI    0x1005
#define BT_MESH_MODEL_ID_GEN_POWER_ONOFF_SRV       0x1006
#define BT_MESH_MODEL_ID_GEN_POWER_ONOFF_SETUP_SRV 0x1007
#define BT_MESH_MODEL_ID_GEN_POWER_ONOFF_CLI       0x1008
#define BT_MESH_MODEL_ID_GEN_POWER_LEVEL_SRV       0x1009
#define BT_MESH_MODEL_ID_GEN_POWER_LEVEL_SETUP_SRV 0x100a
#define BT_MESH_MODEL_ID_GEN_POWER_LEVEL_CLI       0x100b
#define BT_MESH_MODEL_ID_GEN_BATTERY_SRV           0x100c
#define BT_MESH_MODEL_ID_GEN_BATTERY_CLI           0x100d
#define BT_MESH_MODEL_ID_GEN_LOCATION_SRV          0x100e
#define BT_MESH_MODEL_ID_GEN_LOCATION_SETUPSRV     0x100f
#define BT_MESH_MODEL_ID_GEN_LOCATION_CLI          0x1010
#define BT_MESH_MODEL_ID_GEN_ADMIN_PROP_SRV        0x1011
#define BT_MESH_MODEL_ID_GEN_MANUFACTURER_PROP_SRV 0x1012
#define BT_MESH_MODEL_ID_GEN_USER_PROP_SRV         0x1013
#define BT_MESH_MODEL_ID_GEN_CLIENT_PROP_SRV       0x1014
#define BT_MESH_MODEL_ID_GEN_PROP_CLI              0x1015
#define BT_MESH_MODEL_ID_SENSOR_SRV                0x1100
#define BT_MESH_MODEL_ID_SENSOR_SETUP_SRV          0x1101
#define BT_MESH_MODEL_ID_SENSOR_CLI                0x1102
#define BT_MESH_MODEL_ID_TIME_SRV                  0x1200
#define BT_MESH_MODEL_ID_TIME_SETUP_SRV            0x1201
#define BT_MESH_MODEL_ID_TIME_CLI                  0x1202
#define BT_MESH_MODEL_ID_SCENE_SRV                 0x1203
#define BT_MESH_MODEL_ID_SCENE_SETUP_SRV           0x1204
#define BT_MESH_MODEL_ID_SCENE_CLI                 0x1205
#define BT_MESH_MODEL_ID_SCHEDULER_SRV             0x1206
#define BT_MESH_MODEL_ID_SCHEDULER_SETUP_SRV       0x1207
#define BT_MESH_MODEL_ID_SCHEDULER_CLI             0x1208
#define BT_MESH_MODEL_ID_LIGHT_LIGHTNESS_SRV       0x1300
#define BT_MESH_MODEL_ID_LIGHT_LIGHTNESS_SETUP_SRV 0x1301
#define BT_MESH_MODEL_ID_LIGHT_LIGHTNESS_CLI       0x1302
#define BT_MESH_MODEL_ID_LIGHT_CTL_SRV             0x1303
#define BT_MESH_MODEL_ID_LIGHT_CTL_SETUP_SRV       0x1304
#define BT_MESH_MODEL_ID_LIGHT_CTL_CLI             0x1305
#define BT_MESH_MODEL_ID_LIGHT_CTL_TEMP_SRV        0x1306
#define BT_MESH_MODEL_ID_LIGHT_HSL_SRV             0x1307
#define BT_MESH_MODEL_ID_LIGHT_HSL_SETUP_SRV       0x1308
#define BT_MESH_MODEL_ID_LIGHT_HSL_CLI             0x1309
#define BT_MESH_MODEL_ID_LIGHT_HSL_HUE_SRV         0x130a
#define BT_MESH_MODEL_ID_LIGHT_HSL_SAT_SRV         0x130b
#define BT_MESH_MODEL_ID_LIGHT_XYL_SRV             0x130c
#define BT_MESH_MODEL_ID_LIGHT_XYL_SETUP_SRV       0x130d
#define BT_MESH_MODEL_ID_LIGHT_XYL_CLI             0x130e
#define BT_MESH_MODEL_ID_LIGHT_LC_SRV              0x130f
#define BT_MESH_MODEL_ID_LIGHT_LC_SETUPSRV         0x1310
#define BT_MESH_MODEL_ID_LIGHT_LC_CLI              0x1311

/** Message sending context. */
struct bt_mesh_msg_ctx {
	/** NetKey Index of the subnet to send the message on. */
	u16_t net_idx;

	/** AppKey Index to encrypt the message with. */
	u16_t app_idx;

	/** Remote address. */
	u16_t addr;

	/** Destination address of a received message. Not used for sending. */
	u16_t recv_dst;

	/** Received TTL value. Not used for sending. */
	u8_t  recv_ttl:7;

	/** Force sending reliably by using segment acknowledgement */
	u8_t  send_rel:1;

	/** TTL, or BT_MESH_TTL_DEFAULT for default TTL. */
	u8_t  send_ttl;
};

struct bt_mesh_model_op {
	/* OpCode encoded using the BT_MESH_MODEL_OP_* macros */
	const u32_t  opcode;

	/* Minimum required message length */
	const size_t min_len;

	/* Message handler for the opcode */
	void (*const func)(struct bt_mesh_model *model,
			   struct bt_mesh_msg_ctx *ctx,
			   struct os_mbuf *buf);
};

#define BT_MESH_MODEL_OP_1(b0) (b0)
#define BT_MESH_MODEL_OP_2(b0, b1) (((b0) << 8) | (b1))
#define BT_MESH_MODEL_OP_3(b0, cid) ((((b0) << 16) | 0xc00000) | (cid))

#define BT_MESH_MODEL_OP_END { 0, 0, NULL }
#define BT_MESH_MODEL_NO_OPS ((struct bt_mesh_model_op []) \
			      { BT_MESH_MODEL_OP_END })

/** Helper to define an empty model array */
#define BT_MESH_MODEL_NONE ((struct bt_mesh_model []){})

#define BT_MESH_MODEL(_id, _op, _pub, _user_data)                            \
{                                                                            \
	.id = (_id),                                                         \
	.op = _op,                                                           \
	.keys = { [0 ... (CONFIG_BT_MESH_MODEL_KEY_COUNT - 1)] =             \
			BT_MESH_KEY_UNUSED },                                \
	.pub = _pub,                                                         \
	.groups = { [0 ... (CONFIG_BT_MESH_MODEL_GROUP_COUNT - 1)] =         \
			BT_MESH_ADDR_UNASSIGNED },                           \
	.user_data = _user_data,                                             \
}

#define BT_MESH_MODEL_VND(_company, _id, _op, _pub, _user_data)              \
{                                                                            \
	.vnd.company = (_company),                                           \
	.vnd.id = (_id),                                                     \
	.op = _op,                                                           \
	.pub = _pub,                                                         \
	.keys = { [0 ... (CONFIG_BT_MESH_MODEL_KEY_COUNT - 1)] =             \
			BT_MESH_KEY_UNUSED },                                \
	.groups = { [0 ... (CONFIG_BT_MESH_MODEL_GROUP_COUNT - 1)] =         \
			BT_MESH_ADDR_UNASSIGNED },                           \
	.user_data = _user_data,                                             \
}

/** @def BT_MESH_TRANSMIT
 *
 *  @brief Encode transmission count & interval steps.
 *
 *  @param count   Number of retransmissions (first transmission is excluded).
 *  @param int_ms  Interval steps in milliseconds. Must be greater than 0,
 *                 less than or equal to 320, and a multiple of 10.
 *
 *  @return Mesh transmit value that can be used e.g. for the default
 *          values of the configuration model data.
 */
#define BT_MESH_TRANSMIT(count, int_ms) ((count) | (((int_ms / 10) - 1) << 3))

/** @def BT_MESH_TRANSMIT_COUNT
 *
 *  @brief Decode transmit count from a transmit value.
 *
 *  @param transmit Encoded transmit count & interval value.
 *
 *  @return Transmission count (actual transmissions is N + 1).
 */
#define BT_MESH_TRANSMIT_COUNT(transmit) (((transmit) & (u8_t)BIT_MASK(3)))

/** @def BT_MESH_TRANSMIT_INT
 *
 *  @brief Decode transmit interval from a transmit value.
 *
 *  @param transmit Encoded transmit count & interval value.
 *
 *  @return Transmission interval in milliseconds.
 */
#define BT_MESH_TRANSMIT_INT(transmit) ((((transmit) >> 3) + 1) * 10)

/** @def BT_MESH_PUB_TRANSMIT
 *
 *  @brief Encode Publish Retransmit count & interval steps.
 *
 *  @param count   Number of retransmissions (first transmission is excluded).
 *  @param int_ms  Interval steps in milliseconds. Must be greater than 0
 *                 and a multiple of 50.
 *
 *  @return Mesh transmit value that can be used e.g. for the default
 *          values of the configuration model data.
 */
#define BT_MESH_PUB_TRANSMIT(count, int_ms) BT_MESH_TRANSMIT(count,           \
							     (int_ms) / 5)

/** @def BT_MESH_PUB_TRANSMIT_COUNT
 *
 *  @brief Decode Pubhlish Retransmit count from a given value.
 *
 *  @param transmit Encoded Publish Retransmit count & interval value.
 *
 *  @return Retransmission count (actual transmissions is N + 1).
 */
#define BT_MESH_PUB_TRANSMIT_COUNT(transmit) BT_MESH_TRANSMIT_COUNT(transmit)

/** @def BT_MESH_PUB_TRANSMIT_INT
 *
 *  @brief Decode Publish Retransmit interval from a given value.
 *
 *  @param transmit Encoded Publish Retransmit count & interval value.
 *
 *  @return Transmission interval in milliseconds.
 */
#define BT_MESH_PUB_TRANSMIT_INT(transmit) ((((transmit) >> 3) + 1) * 50)

/** Model publication context. */
struct bt_mesh_model_pub {
	/** The model the context belongs to. Initialized by the stack. */
	struct bt_mesh_model *mod;

	u16_t addr;         /**< Publish Address. */
	u16_t key;          /**< Publish AppKey Index. */

	u8_t  ttl;          /**< Publish Time to Live. */
	u8_t  retransmit;   /**< Retransmit Count & Interval Steps. */
	u8_t  period;       /**< Publish Period. */
	u8_t  period_div:4, /**< Divisor for the Period. */
	      cred:1,       /**< Friendship Credentials Flag. */
	      count:3;      /**< Retransmissions left. */

	u32_t period_start; /**< Start of the current period. */

	/** @brief Publication buffer, containing the publication message.
	 *
	 *  The application is expected to initialize this with
	 *  a valid net_buf_simple pointer, with the help of e.g.
	 *  the NET_BUF_SIMPLE() macro. The publication buffer must
	 *  contain a valid publication message before calling the
	 *  bt_mesh_model_publish() API or after the publication's
	 *  @ref bt_mesh_model_pub.update callback has been called
	 *  and returned success. The buffer must be created outside
	 *  of function context, i.e. it must not be on the stack.
	 *  This is most conveniently acheived by creating it inline
	 *  when declaring the publication context:
	 *
	 *      static struct bt_mesh_model_pub my_pub = {
	 *              .msg = NET_BUF_SIMPLE(size),
	 *      };
	 */
	struct os_mbuf *msg;

	/** @brief Callback for updating the publication buffer.
	 *
	 *  When set to NULL, the model is assumed not to support
	 *  periodic publishing. When set to non-NULL the callback
	 *  will be called periodically and is expected to update
	 *  @ref bt_mesh_model_pub.msg with a valid publication
	 *  message.
	 *
	 *  @param mod The Model the Publication Context belogs to.
	 *
	 *  @return Zero on success or (negative) error code otherwise.
	 */
	int (*update)(struct bt_mesh_model *mod);

	/** Publish Period Timer. Only for stack-internal use. */
	struct k_delayed_work timer;
};

/** Abstraction that describes a Mesh Model instance */
struct bt_mesh_model {
	union {
		const u16_t id;
		struct {
			u16_t company;
			u16_t id;
		} vnd;
	};

	/* Internal information, mainly for persistent storage */
	u8_t  elem_idx;   /* Belongs to Nth element */
	u8_t  mod_idx;    /* Is the Nth model in the element */
	u16_t flags;      /* Information about what has changed */

	/* Model Publication */
	struct bt_mesh_model_pub * const pub;

	/* AppKey List */
	u16_t keys[CONFIG_BT_MESH_MODEL_KEY_COUNT];

	/* Subscription List (group or virtual addresses) */
	u16_t groups[CONFIG_BT_MESH_MODEL_GROUP_COUNT];

	const struct bt_mesh_model_op * const op;

	/* Model-specific user data */
	void *user_data;
};

struct bt_mesh_send_cb {
	void (*start)(u16_t duration, int err, void *cb_data);
	void (*end)(int err, void *cb_data);
};

void bt_mesh_model_msg_init(struct os_mbuf *msg, u32_t opcode);

/** Special TTL value to request using configured default TTL */
#define BT_MESH_TTL_DEFAULT 0xff

/** Maximum allowed TTL value */
#define BT_MESH_TTL_MAX     0x7f

/**
 * @brief Send an Access Layer message.
 *
 * @param model     Mesh (client) Model that the message belongs to.
 * @param ctx       Message context, includes keys, TTL, etc.
 * @param msg       Access Layer payload (the actual message to be sent).
 * @param cb        Optional "message sent" callback.
 * @param cb_data   User data to be passed to the callback.
 *
 * @return 0 on success, or (negative) error code on failure.
 */
int bt_mesh_model_send(struct bt_mesh_model *model,
		       struct bt_mesh_msg_ctx *ctx,
		       struct os_mbuf *msg,
		       const struct bt_mesh_send_cb *cb,
		       void *cb_data);

/**
 * @brief Send a model publication message.
 *
 * Before calling this function, the user needs to ensure that the model
 * publication message (@ref bt_mesh_model_pub.msg) contains a valid
 * message to be sent. Note that this API is only to be used for
 * non-period publishing. For periodic publishing the app only needs
 * to make sure that @ref bt_mesh_model_pub.msg contains a valid message
 * whenever the @ref bt_mesh_model_pub.update callback is called.
 *
 * @param model  Mesh (client) Model that's publishing the message.
 *
 * @return 0 on success, or (negative) error code on failure.
 */
int bt_mesh_model_publish(struct bt_mesh_model *model);

/**
 * @brief Get the element that a model belongs to.
 *
 * @param mod  Mesh model.
 *
 * @return Pointer to the element that the given model belongs to.
 */
struct bt_mesh_elem *bt_mesh_model_elem(struct bt_mesh_model *mod);

/** Node Composition */
struct bt_mesh_comp {
	u16_t cid;
	u16_t pid;
	u16_t vid;

	size_t elem_count;
	struct bt_mesh_elem *elem;
};

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __BT_MESH_ACCESS_H */
