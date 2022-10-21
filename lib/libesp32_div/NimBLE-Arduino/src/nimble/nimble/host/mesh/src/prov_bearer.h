/*  Bluetooth Mesh */

/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define PROTOCOL_TIMEOUT     K_SECONDS(60)

/** @def PROV_BEARER_BUF_HEADROOM
 *
 *  @brief Required headroom for the bearer packet buffers.
 */
#if MYNEWT_VAL(BLE_MESH_PB_GATT)
#define PROV_BEARER_BUF_HEADROOM 5
#else
#define PROV_BEARER_BUF_HEADROOM 0
#endif

enum prov_bearer_link_status {
	PROV_BEARER_LINK_STATUS_SUCCESS,
	PROV_BEARER_LINK_STATUS_TIMEOUT,
	PROV_BEARER_LINK_STATUS_FAIL,
};

struct prov_bearer;

/** Callbacks from bearer to host */
struct prov_bearer_cb {

	void (*link_opened)(const struct prov_bearer *bearer, void *cb_data);

	void (*link_closed)(const struct prov_bearer *bearer, void *cb_data,
			    enum prov_bearer_link_status reason);

	void (*error)(const struct prov_bearer *bearer, void *cb_data,
		      uint8_t err);

	void (*recv)(const struct prov_bearer *bearer, void *cb_data,
		     struct os_mbuf *buf);
};

typedef void (*prov_bearer_send_complete_t)(int err, void *cb_data);

/** Provisioning bearer API */
struct prov_bearer {
	/** Provisioning bearer type. */
	bt_mesh_prov_bearer_t type;

	/** @brief Enable link establishment as a provisionee.
	 *
	 *  Prompts the bearer to make itself visible to provisioners, and
	 *  start accepting link open messages.
	 *
	 *  @param cb Bearer event callbacks used for the duration of the link.
	 *  @param cb_data Context parameter to pass to the bearer callbacks.
	 *
	 *  @return Zero on success, or (negative) error code otherwise.
	 */
	int (*link_accept)(const struct prov_bearer_cb *cb, void *cb_data);

	/** @brief Send a packet on an established link.
	 *
	 *  @param buf     Payload buffer. Requires @ref
	 *                 PROV_BEARER_BUF_HEADROOM bytes of headroom.
	 *  @param cb      Callback to call when sending is complete.
	 *  @param cb_data Callback data.
	 *
	 *  @return Zero on success, or (negative) error code otherwise.
	 */
	int (*send)(struct os_mbuf *buf, prov_bearer_send_complete_t cb,
		    void *cb_data);

	/** @brief Clear any ongoing transmissions, if possible.
	 *
	 *  Bearers that don't support tx clearing must implement this callback
	 *  and leave it empty.
	 */
	void (*clear_tx)(void);

	/* Only available in provisioners: */

	/** @brief Open a new link as a provisioner.
	 *
	 *  Only available in provisioners. Bearers that don't support the
	 *  provisioner role should leave this as NULL.
	 *
	 *  @param uuid UUID of the node to establish a link to.
	 *  @param timeout Protocol timeout.
	 *  @param cb Bearer event callbacks used for the duration of the link.
	 *  @param cb_data Context parameter to pass to the bearer callbacks.
	 *
	 *  @return Zero on success, or (negative) error code otherwise.
	 */
	int (*link_open)(const uint8_t uuid[16], int32_t timeout,
			 const struct prov_bearer_cb *cb, void *cb_data);

	/** @brief Close the current link.
	 *
	 *  Only available in provisioners. Bearers that don't support the
	 *  provisioner role should leave this as NULL.
	 *
	 *  @param status Link status for the link close message.
	 */
	void (*link_close)(enum prov_bearer_link_status status);
};

extern const struct prov_bearer pb_adv;
extern const struct prov_bearer pb_gatt;

void pb_adv_init(void);
void pb_gatt_init(void);

void pb_adv_reset(void);
void pb_gatt_reset(void);
