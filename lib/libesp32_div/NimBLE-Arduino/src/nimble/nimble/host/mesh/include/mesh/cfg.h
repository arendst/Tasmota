/** @file
 *  @brief Bluetooth Mesh Runtime Configuration APIs.
 */

/*
 * Copyright (c) 2020 Nordic Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _BT_MESH_CFG_H_
#define _BT_MESH_CFG_H_

#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

/**
 * @brief Bluetooth Mesh Runtime Configuration API
 * @defgroup bt_mesh_cfg Bluetooth Mesh Runtime Configuration
 * @ingroup bt_mesh
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/** Bluetooth Mesh Feature states */
enum bt_mesh_feat_state {
	/** Feature is supported, but disabled. */
	BT_MESH_FEATURE_DISABLED,
	/** Feature is supported and enabled. */
	BT_MESH_FEATURE_ENABLED,
	/** Feature is not supported, and cannot be enabled. */
	BT_MESH_FEATURE_NOT_SUPPORTED,
};

/* Legacy feature defines */
#define BT_MESH_RELAY_DISABLED              BT_MESH_FEATURE_DISABLED
#define BT_MESH_RELAY_ENABLED               BT_MESH_FEATURE_ENABLED
#define BT_MESH_RELAY_NOT_SUPPORTED         BT_MESH_FEATURE_NOT_SUPPORTED

#define BT_MESH_BEACON_DISABLED             BT_MESH_FEATURE_DISABLED
#define BT_MESH_BEACON_ENABLED              BT_MESH_FEATURE_ENABLED

#define BT_MESH_GATT_PROXY_DISABLED         BT_MESH_FEATURE_DISABLED
#define BT_MESH_GATT_PROXY_ENABLED          BT_MESH_FEATURE_ENABLED
#define BT_MESH_GATT_PROXY_NOT_SUPPORTED    BT_MESH_FEATURE_NOT_SUPPORTED

#define BT_MESH_FRIEND_DISABLED             BT_MESH_FEATURE_DISABLED
#define BT_MESH_FRIEND_ENABLED              BT_MESH_FEATURE_ENABLED
#define BT_MESH_FRIEND_NOT_SUPPORTED        BT_MESH_FEATURE_NOT_SUPPORTED

#define BT_MESH_NODE_IDENTITY_STOPPED       BT_MESH_FEATURE_DISABLED
#define BT_MESH_NODE_IDENTITY_RUNNING       BT_MESH_FEATURE_ENABLED
#define BT_MESH_NODE_IDENTITY_NOT_SUPPORTED BT_MESH_FEATURE_NOT_SUPPORTED

/** @brief Enable or disable sending of the Secure Network Beacon.
 *
 *  @param beacon New Secure Network Beacon state.
 */
void bt_mesh_beacon_set(bool beacon);

/** @brief Get the current Secure Network Beacon state.
 *
 *  @returns Whether the Secure Network Beacon feature is enabled.
 */
bool bt_mesh_beacon_enabled(void);

/** @brief Set the default TTL value.
 *
 *  The default TTL value is used when no explicit TTL value is set. Models will
 *  use the default TTL value when @ref bt_mesh_msg_ctx::send_ttl is
 *  @ref BT_MESH_TTL_DEFAULT.
 *
 *  @param default_ttl The new default TTL value. Valid values are 0x00 and 0x02
 *                     to @ref BT_MESH_TTL_MAX.
 *
 *  @retval 0       Successfully set the default TTL value.
 *  @retval -EINVAL Invalid TTL value.
 */
int bt_mesh_default_ttl_set(uint8_t default_ttl);

/** @brief Get the current default TTL value.
 *
 *  @return The current default TTL value.
 */
uint8_t bt_mesh_default_ttl_get(void);

/** @brief Set the Network Transmit parameters.
 *
 *  The Network Transmit parameters determine the parameters local messages are
 *  transmitted with.
 *
 *  @see BT_MESH_TRANSMIT
 *
 *  @param xmit New Network Transmit parameters. Use @ref BT_MESH_TRANSMIT for
 *              encoding.
 */
void bt_mesh_net_transmit_set(uint8_t xmit);

/** @brief Get the current Network Transmit parameters.
 *
 *  The @ref BT_MESH_TRANSMIT_COUNT and @ref BT_MESH_TRANSMIT_INT macros can be
 *  used to decode the Network Transmit parameters.
 *
 *  @return The current Network Transmit parameters.
 */
uint8_t bt_mesh_net_transmit_get(void);

/** @brief Configure the Relay feature.
 *
 *  Enable or disable the Relay feature, and configure the parameters to
 *  transmit relayed messages with.
 *
 *  Support for the Relay feature must be enabled through the
 *  @c CONFIG_BT_MESH_RELAY configuration option.
 *
 *  @see BT_MESH_TRANSMIT
 *
 *  @param relay New Relay feature state. Must be one of
 *               @ref BT_MESH_FEATURE_ENABLED and
 *               @ref BT_MESH_FEATURE_DISABLED.
 *  @param xmit  New Relay retransmit parameters. Use @ref BT_MESH_TRANSMIT for
 *               encoding.
 *
 *  @retval 0         Successfully changed the Relay configuration.
 *  @retval -ENOTSUP  The Relay feature is not supported.
 *  @retval -EINVAL   Invalid parameter.
 *  @retval -EALREADY Already using the given parameters.
 */
int bt_mesh_relay_set(enum bt_mesh_feat_state relay, uint8_t xmit);

/** @brief Get the current Relay feature state.
 *
 *  @returns The Relay feature state.
 */
enum bt_mesh_feat_state bt_mesh_relay_get(void);

/** @brief Get the current Relay Retransmit parameters.
 *
 *  The @ref BT_MESH_TRANSMIT_COUNT and @ref BT_MESH_TRANSMIT_INT macros can be
 *  used to decode the Relay Retransmit parameters.
 *
 *  @return The current Relay Retransmit parameters, or 0 if relay is not
 *          supported.
 */
uint8_t bt_mesh_relay_retransmit_get(void);

/** @brief Enable or disable the GATT Proxy feature.
 *
 *  Support for the GATT Proxy feature must be enabled through the
 *  @c CONFIG_BT_MESH_GATT_PROXY configuration option.
 *
 *  @note The GATT Proxy feature only controls a Proxy node's ability to relay
 *        messages to the mesh network. A node that supports GATT Proxy will
 *        still advertise Connectable Proxy beacons, even if the feature is
 *        disabled. The Proxy feature can only be fully disabled through compile
 *        time configuration.
 *
 *  @param gatt_proxy New GATT Proxy state. Must be one of
 *                    @ref BT_MESH_FEATURE_ENABLED and
 *                    @ref BT_MESH_FEATURE_DISABLED.
 *
 *  @retval 0         Successfully changed the GATT Proxy feature state.
 *  @retval -ENOTSUP  The GATT Proxy feature is not supported.
 *  @retval -EINVAL   Invalid parameter.
 *  @retval -EALREADY Already in the given state.
 */
int bt_mesh_gatt_proxy_set(enum bt_mesh_feat_state gatt_proxy);

/** @brief Get the current GATT Proxy state.
 *
 *  @returns The GATT Proxy feature state.
 */
enum bt_mesh_feat_state bt_mesh_gatt_proxy_get(void);

/** @brief Enable or disable the Friend feature.
 *
 *  Any active friendships will be terminated immediately if the Friend feature
 *  is disabled.
 *
 *  Support for the Friend feature must be enabled through the
 *  @c CONFIG_BT_MESH_FRIEND configuration option.
 *
 *  @param friendship New Friend feature state. Must be one of
 *                    @ref BT_MESH_FEATURE_ENABLED and
 *                    @ref BT_MESH_FEATURE_DISABLED.
 *
 *  @retval 0        Successfully changed the Friend feature state.
 *  @retval -ENOTSUP The Friend feature is not supported.
 *  @retval -EINVAL  Invalid parameter.
 *  @retval -EALREADY Already in the given state.
 */
int bt_mesh_friend_set(enum bt_mesh_feat_state friendship);

/** @brief Get the current Friend state.
 *
 *  @returns The Friend feature state.
 */
enum bt_mesh_feat_state bt_mesh_friend_get(void);

/**
 * @brief Bluetooth Mesh Subnet Configuration
 * @defgroup bt_mesh_cfg_subnet Bluetooth Mesh Subnet Configuration
 * @{
 */

/** @brief Add a Subnet.
 *
 *  Adds a subnet with the given network index and network key to the list of
 *  known Subnets. All messages sent on the given Subnet will be processed by
 *  this node, and the node may send and receive Network Beacons on the given
 *  Subnet.
 *
 *  @param net_idx Network index.
 *  @param key     Root network key of the Subnet. All other keys are derived
 *                 from this.
 *
 *  @retval STATUS_SUCCESS The Subnet was successfully added.
 *  @retval STATUS_INSUFF_RESOURCES No room for this Subnet.
 *  @retval STATUS_UNSPECIFIED The Subnet couldn't be created for some reason.
 */
uint8_t bt_mesh_subnet_add(uint16_t net_idx, const uint8_t key[16]);

/** @brief Update the given Subnet.
 *
 *  Starts the Key Refresh procedure for this Subnet by adding a second set of
 *  encryption keys. The Subnet will continue sending with the old key (but
 *  receiving messages using both) until the Subnet enters Key Refresh phase 2.
 *
 *  This allows a network configurator to replace old network and application
 *  keys for the entire network, effectively removing access for all nodes that
 *  aren't given the new keys.
 *
 *  @param net_idx Network index.
 *  @param key     New root network key of the Subnet.
 *
 *  @retval STATUS_SUCCESS The Subnet was updated with a second key.
 *  @retval STATUS_INVALID_NETKEY The NetIdx is unknown.
 *  @retval STATUS_IDX_ALREADY_STORED The @c key value is the same as the
 *                                    current key.
 *  @retval STATUS_CANNOT_UPDATE The Subnet cannot be updated for some reason.
 */
uint8_t bt_mesh_subnet_update(uint16_t net_idx, const uint8_t key[16]);

/** @brief Delete a Subnet.
 *
 *  Removes the Subnet with the given network index from the node. The node will
 *  stop sending Network Beacons with the given Subnet, and can no longer
 *  process messages on this Subnet.
 *
 *  All Applications bound to this Subnet are also deleted.
 *
 *  @param net_idx Network index.
 *
 *  @retval STATUS_SUCCESS The Subnet was deleted.
 *  @retval STATUS_INVALID_NETKEY The NetIdx is unknown.
 */
uint8_t bt_mesh_subnet_del(uint16_t net_idx);

/** @brief Check whether a Subnet is known.
 *
 *  @param net_idx Network index
 *
 *  @return true if a Subnet with the given index exists, false otherwise.
 */
bool bt_mesh_subnet_exists(uint16_t net_idx);

/** @brief Set the Subnet's Key Refresh phase.
 *
 *  The Key Refresh procedure is started by updating the Subnet keys through
 *  @ref bt_mesh_subnet_update. This puts the Subnet in Key Refresh Phase 1.
 *  Once all nodes have received the new Subnet key, Key Refresh Phase 2 can be
 *  activated through this function to start transmitting with the new network
 *  key. Finally, to revoke the old key, set the Key Refresh Phase to 3. This
 *  removes the old keys from the node, and returns the Subnet back to normal
 *  single-key operation with the new key set.
 *
 *  @param net_idx Network index.
 *  @param phase   Pointer to the new Key Refresh phase. Will return the actual
 *                 Key Refresh phase after updating.
 *
 *  @retval STATUS_SUCCESS The Key Refresh phase of the Subnet was successfully
 *                         changed.
 *  @retval STATUS_INVALID_NETKEY The NetIdx is unknown.
 *  @retval STATUS_CANNOT_UPDATE The given phase change is invalid.
 */
uint8_t bt_mesh_subnet_kr_phase_set(uint16_t net_idx, uint8_t *phase);

/** @brief Get the Subnet's Key Refresh phase.
 *
 *  @param net_idx Network index.
 *  @param phase   Pointer to the Key Refresh variable to fill.
 *
 *  @retval STATUS_SUCCESS Successfully populated the @c phase variable.
 *  @retval STATUS_INVALID_NETKEY The NetIdx is unknown.
 */
uint8_t bt_mesh_subnet_kr_phase_get(uint16_t net_idx, uint8_t *phase);

/** @brief Set the Node Identity state of the Subnet.
 *
 *  The Node Identity state of a Subnet determines whether the Subnet advertises
 *  connectable Node Identity beacons for Proxy Clients to connect to.
 *  Once started, the Node Identity beacon runs for 60 seconds, or until it is
 *  stopped.
 *
 *  This function serves the same purpose as @ref bt_mesh_proxy_identity_enable,
 *  but only acts on a single Subnet.
 *
 *  GATT Proxy support must be enabled through
 *  @option{CONFIG_BT_MESH_GATT_PROXY}.
 *
 *  @param net_idx Network index.
 *  @param node_id New Node Identity state, must be either @ref
 *                 BT_MESH_FEATURE_ENABLED or @ref BT_MESH_FEATURE_DISABLED.
 *
 *  @retval STATUS_SUCCESS Successfully set the Node Identity state of the
 *                         Subnet.
 *  @retval STATUS_INVALID_NETKEY The NetIdx is unknown.
 *  @retval STATUS_FEAT_NOT_SUPP The Node Identity feature is not supported.
 *  @retval STATUS_CANNOT_SET Couldn't set the Node Identity state.
 */
uint8_t bt_mesh_subnet_node_id_set(uint16_t net_idx,
				   enum bt_mesh_feat_state node_id);

/** @brief Get the Node Identity state of the Subnet.
 *
 *  @param net_idx Network index.
 *  @param node_id Node Identity variable to fill.
 *
 *  @retval STATUS_SUCCESS Successfully populated the @c node_id variable.
 *  @retval STATUS_INVALID_NETKEY The NetIdx is unknown.
 */
uint8_t bt_mesh_subnet_node_id_get(uint16_t net_idx,
				   enum bt_mesh_feat_state *node_id);

/** @brief Get a list of all known Subnet indexes.
 *
 *  Builds a list of all known Subnet indexes in the @c net_idxs array.
 *  If the @c net_idxs array is smaller than the list of known Subnets, this
 *  function fills all available entries and returns @c -ENOMEM. In this
 *  case, the next @c max entries of the list can be read out by calling
 *  @code
 *  bt_mesh_subnets_get(list, max, max);
 *  @endcode
 *
 *  Note that any changes to the Subnet list between calls to this function
 *  could change the order and number of entries in the list.
 *
 *  @param net_idxs Array to fill.
 *  @param max      Max number of indexes to return.
 *  @param skip     Number of indexes to skip. Enables batched processing of the
 *                  list.
 *
 *  @return The number of indexes added to the @c net_idxs array, or @c -ENOMEM
 *          if the number of known Subnets exceeds the @c max parameter.
 */
ssize_t bt_mesh_subnets_get(uint16_t net_idxs[], size_t max, off_t skip);

/**
 * @}
 */

/**
 * @brief Bluetooth Mesh Application Configuration
 * @defgroup bt_mesh_cfg_app Bluetooth Mesh Application Configuration
 * @{
 */

/** @brief Add an Application key.
 *
 *  Adds the Application with the given index to the list of known applications.
 *  Allows the node to send and receive model messages encrypted with this
 *  Application key.
 *
 *  Every Application is bound to a specific Subnet. The node must know the
 *  Subnet the Application is bound to before it can add the Application.
 *
 *  @param app_idx Application index.
 *  @param net_idx Network index the Application is bound to.
 *  @param key     Application key value.
 *
 *  @retval STATUS_SUCCESS The Application was successfully added.
 *  @retval STATUS_INVALID_NETKEY The NetIdx is unknown.
 *  @retval STATUS_INSUFF_RESOURCES There's no room for storing this
 *                                  Application.
 *  @retval STATUS_INVALID_BINDING This AppIdx is already bound to another
 *                                 Subnet.
 *  @retval STATUS_IDX_ALREADY_STORED This AppIdx is already stored with a
 *                                    different key value.
 *  @retval STATUS_CANNOT_SET Cannot set the Application key for some reason.
 */
uint8_t bt_mesh_app_key_add(uint16_t app_idx, uint16_t net_idx,
			    const uint8_t key[16]);

/** @brief Update an Application key.
 *
 *  Update an Application with a second Application key, as part of the
 *  Key Refresh procedure of the bound Subnet. The node will continue
 *  transmitting with the old application key (but receiving on both) until the
 *  Subnet enters Key Refresh phase 2. Once the Subnet enters Key Refresh phase
 *  3, the old application key will be deleted.
 *
 *  @note The Application key can only be updated if the bound Subnet is in Key
 *        Refresh phase 1.
 *
 *  @param app_idx Application index.
 *  @param net_idx Network index the Application is bound to, or
 *                 @ref BT_MESH_KEY_ANY to skip the binding check.
 *  @param key     New key value.
 *
 *  @retval STATUS_SUCCESS The Application key was successfully updated.
 *  @retval STATUS_INVALID_NETKEY The NetIdx is unknown.
 *  @retval STATUS_INVALID_BINDING This AppIdx is not bound to the given NetIdx.
 *  @retval STATUS_CANNOT_UPDATE The Application key cannot be updated for some
 *                               reason.
 *  @retval STATUS_IDX_ALREADY_STORED This AppIdx is already updated with a
 *                                    different key value.
 */
uint8_t bt_mesh_app_key_update(uint16_t app_idx, uint16_t net_idx,
			       const uint8_t key[16]);

/** @brief Delete an Application key.
 *
 *  All models bound to this application will remove this binding.
 *  All models publishing with this application will stop publishing.
 *
 *  @param app_idx Application index.
 *  @param net_idx Network index.
 *
 *  @retval STATUS_SUCCESS The Application key was successfully deleted.
 *  @retval STATUS_INVALID_NETKEY The NetIdx is unknown.
 *  @retval STATUS_INVALID_BINDING This AppIdx is not bound to the given NetIdx.
 */
uint8_t bt_mesh_app_key_del(uint16_t app_idx, uint16_t net_idx);

/** @brief Check if an Application key is known.
 *
 *  @param app_idx Application index.
 *
 *  @return true if the Application is known, false otherwise.
 */
bool bt_mesh_app_key_exists(uint16_t app_idx);

/** @brief Get a list of all known Application key indexes.
 *
 *  Builds a list of all Application indexes for the given network index in the
 *  @c app_idxs array. If the @c app_idxs array cannot fit all bound
 *  Applications, this function fills all available entries and returns @c
 *  -ENOMEM. In this case, the next @c max entries of the list can be read out
 *  by calling
 *  @code
 *  bt_mesh_app_keys_get(net_idx, list, max, max);
 *  @endcode
 *
 *  Note that any changes to the Application key list between calls to this
 *  function could change the order and number of entries in the list.
 *
 *  @param net_idx  Network Index to get the Applications of, or @ref
 *                  BT_MESH_KEY_ANY to get all Applications.
 *  @param app_idxs Array to fill.
 *  @param max      Max number of indexes to return.
 *  @param skip     Number of indexes to skip. Enables batched processing of the
 *                  list.
 *
 *  @return The number of indexes added to the @c app_idxs array, or @c -ENOMEM
 *          if the number of known Applications exceeds the @c max parameter.
 */
ssize_t bt_mesh_app_keys_get(uint16_t net_idx, uint16_t app_idxs[], size_t max,
			     off_t skip);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* _BT_MESH_CFG_H_ */