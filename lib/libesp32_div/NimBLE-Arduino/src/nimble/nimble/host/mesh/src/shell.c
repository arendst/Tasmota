/** @file
 *  @brief Bluetooth Mesh shell
 *
 */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#if MYNEWT_VAL(BLE_MESH)

#if MYNEWT_VAL(BLE_MESH_SHELL)

#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "shell/shell.h"
#include "console/console.h"
#include "mesh/mesh.h"
#include "mesh/main.h"
#include "mesh/glue.h"
#include "mesh/testing.h"

/* Private includes for raw Network & Transport layer access */
#include "net.h"
#include "access.h"
#include "mesh_priv.h"
#include "lpn.h"
#include "transport.h"
#include "foundation.h"
#include "testing.h"
#include "settings.h"

#if MYNEWT_VAL(BLE_MESH_SHELL_MODELS)
#include "mesh/model_srv.h"
#include "mesh/model_cli.h"
#include "light_model.h"
#endif

/* This should be higher priority (lower value) than main task priority */
#define BLE_MESH_SHELL_TASK_PRIO 126
#define BLE_MESH_SHELL_STACK_SIZE 768

OS_TASK_STACK_DEFINE(g_blemesh_shell_stack, BLE_MESH_SHELL_STACK_SIZE);

struct os_task mesh_shell_task;
static struct os_eventq mesh_shell_queue;

#define CID_NVAL   0xffff
#define CID_VENDOR  0x05C3

/* Vendor Model data */
#define VND_MODEL_ID_1 0x1234

/* Default net, app & dev key values, unless otherwise specified */
static const u8_t default_key[16] = {
	0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
	0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
};

static struct {
	u16_t local;
	u16_t dst;
	u16_t net_idx;
	u16_t app_idx;
} net = {
	.local = BT_MESH_ADDR_UNASSIGNED,
	.dst = BT_MESH_ADDR_UNASSIGNED,
};

static struct bt_mesh_cfg_srv cfg_srv = {
	.relay = BT_MESH_RELAY_DISABLED,
	.beacon = BT_MESH_BEACON_ENABLED,
#if MYNEWT_VAL(BLE_MESH_FRIEND)
	.frnd = BT_MESH_FRIEND_DISABLED,
#else
	.frnd = BT_MESH_FRIEND_NOT_SUPPORTED,
#endif
#if MYNEWT_VAL(BLE_MESH_GATT_PROXY)
	.gatt_proxy = BT_MESH_GATT_PROXY_DISABLED,
#else
	.gatt_proxy = BT_MESH_GATT_PROXY_NOT_SUPPORTED,
#endif

	.default_ttl = 7,

	/* 3 transmissions with 20ms interval */
	.net_transmit = BT_MESH_TRANSMIT(2, 20),
	.relay_retransmit = BT_MESH_TRANSMIT(2, 20),
};

#define CUR_FAULTS_MAX 4

static u8_t cur_faults[CUR_FAULTS_MAX];
static u8_t reg_faults[CUR_FAULTS_MAX * 2];

static void get_faults(u8_t *faults, u8_t faults_size, u8_t *dst, u8_t *count)
{
	u8_t i, limit = *count;

	for (i = 0, *count = 0; i < faults_size && *count < limit; i++) {
		if (faults[i]) {
			*dst++ = faults[i];
			(*count)++;
		}
	}
}

static int fault_get_cur(struct bt_mesh_model *model, u8_t *test_id,
			 u16_t *company_id, u8_t *faults, u8_t *fault_count)
{
	printk("Sending current faults\n");

	*test_id = 0x00;
	*company_id = CID_VENDOR;

	get_faults(cur_faults, sizeof(cur_faults), faults, fault_count);

	return 0;
}

static int fault_get_reg(struct bt_mesh_model *model, u16_t cid,
			 u8_t *test_id, u8_t *faults, u8_t *fault_count)
{
	if (cid != CID_VENDOR) {
		printk("Faults requested for unknown Company ID 0x%04x\n", cid);
		return -EINVAL;
	}

	printk("Sending registered faults\n");

	*test_id = 0x00;

	get_faults(reg_faults, sizeof(reg_faults), faults, fault_count);

	return 0;
}

static int fault_clear(struct bt_mesh_model *model, uint16_t cid)
{
	if (cid != CID_VENDOR) {
		return -EINVAL;
	}

	memset(reg_faults, 0, sizeof(reg_faults));

	return 0;
}

static int fault_test(struct bt_mesh_model *model, uint8_t test_id,
		      uint16_t cid)
{
	if (cid != CID_VENDOR) {
		return -EINVAL;
	}

	if (test_id != 0x00) {
		return -EINVAL;
	}

	return 0;
}

static const struct bt_mesh_health_srv_cb health_srv_cb = {
	.fault_get_cur = fault_get_cur,
	.fault_get_reg = fault_get_reg,
	.fault_clear = fault_clear,
	.fault_test = fault_test,
};

static struct bt_mesh_health_srv health_srv = {
	.cb = &health_srv_cb,
};

static struct bt_mesh_model_pub health_pub;

static void
health_pub_init(void)
{
	health_pub.msg  = BT_MESH_HEALTH_FAULT_MSG(CUR_FAULTS_MAX);
}
#if MYNEWT_VAL(BLE_MESH_CFG_CLI)

static struct bt_mesh_cfg_cli cfg_cli = {
};

#endif /* MYNEWT_VAL(BLE_MESH_CFG_CLI) */

#if MYNEWT_VAL(BLE_MESH_HEALTH_CLI)
void show_faults(u8_t test_id, u16_t cid, u8_t *faults, size_t fault_count)
{
	size_t i;

	if (!fault_count) {
		printk("Health Test ID 0x%02x Company ID 0x%04x: no faults\n",
		       test_id, cid);
		return;
	}

	printk("Health Test ID 0x%02x Company ID 0x%04x Fault Count %zu:\n",
	       test_id, cid, fault_count);

	for (i = 0; i < fault_count; i++) {
		printk("\t0x%02x\n", faults[i]);
	}
}

static void health_current_status(struct bt_mesh_health_cli *cli, u16_t addr,
				  u8_t test_id, u16_t cid, u8_t *faults,
				  size_t fault_count)
{
	printk("Health Current Status from 0x%04x\n", addr);
	show_faults(test_id, cid, faults, fault_count);
}

static struct bt_mesh_health_cli health_cli = {
	.current_status = health_current_status,
};

#endif /* MYNEWT_VAL(BLE_MESH_HEALTH_CLI) */

#if MYNEWT_VAL(BLE_MESH_SHELL_MODELS)
static struct bt_mesh_gen_model_cli gen_onoff_cli;
static struct bt_mesh_model_pub gen_onoff_cli_pub;
static struct bt_mesh_model_pub gen_onoff_srv_pub;
static struct bt_mesh_gen_model_cli gen_level_cli;
static struct bt_mesh_model_pub gen_level_cli_pub;
static struct bt_mesh_model_pub gen_level_srv_pub;
static struct bt_mesh_model_pub light_lightness_pub;
static struct bt_mesh_gen_onoff_srv gen_onoff_srv = {
	.get = light_model_gen_onoff_get,
	.set = light_model_gen_onoff_set,
};
static struct bt_mesh_gen_level_srv gen_level_srv = {
	.get = light_model_gen_level_get,
	.set = light_model_gen_level_set,
};
static struct bt_mesh_light_lightness_srv light_lightness_srv = {
	.get = light_model_light_lightness_get,
	.set = light_model_light_lightness_set,
};

void bt_mesh_set_gen_onoff_srv_cb(int (*get)(struct bt_mesh_model *model, u8_t *state),
				  int (*set)(struct bt_mesh_model *model, u8_t state))
{
	gen_onoff_srv.get = get;
	gen_onoff_srv.set = set;
}

void bt_mesh_set_gen_level_srv_cb(int (*get)(struct bt_mesh_model *model, s16_t *level),
				  int (*set)(struct bt_mesh_model *model, s16_t level))
{
	gen_level_srv.get = get;
	gen_level_srv.set = set;
}

void bt_mesh_set_light_lightness_srv_cb(int (*get)(struct bt_mesh_model *model, s16_t *level),
					int (*set)(struct bt_mesh_model *model, s16_t level))
{
	light_lightness_srv.get = get;
	light_lightness_srv.set = set;
}
#endif

static struct bt_mesh_model root_models[] = {
	BT_MESH_MODEL_CFG_SRV(&cfg_srv),
	BT_MESH_MODEL_HEALTH_SRV(&health_srv, &health_pub),
#if MYNEWT_VAL(BLE_MESH_CFG_CLI)
	BT_MESH_MODEL_CFG_CLI(&cfg_cli),
#endif
#if MYNEWT_VAL(BLE_MESH_HEALTH_CLI)
	BT_MESH_MODEL_HEALTH_CLI(&health_cli),
#endif
#if MYNEWT_VAL(BLE_MESH_SHELL_MODELS)
	BT_MESH_MODEL_GEN_ONOFF_SRV(&gen_onoff_srv, &gen_onoff_srv_pub),
	BT_MESH_MODEL_GEN_ONOFF_CLI(&gen_onoff_cli, &gen_onoff_cli_pub),
	BT_MESH_MODEL_GEN_LEVEL_SRV(&gen_level_srv, &gen_level_srv_pub),
	BT_MESH_MODEL_GEN_LEVEL_CLI(&gen_level_cli, &gen_level_cli_pub),
	BT_MESH_MODEL_LIGHT_LIGHTNESS_SRV(&light_lightness_srv, &light_lightness_pub),
#endif
};

static struct bt_mesh_model vnd_models[] = {
	BT_MESH_MODEL_VND(CID_VENDOR, VND_MODEL_ID_1,
			  BT_MESH_MODEL_NO_OPS, NULL, NULL),
};

static struct bt_mesh_elem elements[] = {
	BT_MESH_ELEM(0, root_models, vnd_models),
};

static const struct bt_mesh_comp comp = {
	.cid = CID_VENDOR,
	.elem = elements,
	.elem_count = ARRAY_SIZE(elements),
};

static u8_t hex2val(char c)
{
	if (c >= '0' && c <= '9') {
		return c - '0';
	} else if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	} else if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	} else {
		return 0;
	}
}

static size_t hex2bin(const char *hex, u8_t *bin, size_t bin_len)
{
	size_t len = 0;

	while (*hex && len < bin_len) {
		bin[len] = hex2val(*hex++) << 4;

		if (!*hex) {
			len++;
			break;
		}

		bin[len++] |= hex2val(*hex++);
	}

	return len;
}

static void prov_complete(u16_t net_idx, u16_t addr)
{
	printk("Local node provisioned, net_idx 0x%04x address 0x%04x\n",
	       net_idx, addr);
	net.local = addr;
	net.net_idx = net_idx,
	net.dst = addr;
}

static void prov_node_added(u16_t net_idx, u16_t addr, u8_t num_elem)
{
	printk("Node provisioned, net_idx 0x%04x address "
	       "0x%04x elements %d", net_idx, addr, num_elem);

	net.net_idx = net_idx,
	net.dst = addr;
}

static void prov_input_complete(void)
{
	printk("Input complete");
}

static void prov_reset(void)
{
	printk("The local node has been reset and needs reprovisioning\n");
}

static int output_number(bt_mesh_output_action_t action, uint32_t number)
{
	printk("OOB Number: %lu\n", number);
	return 0;
}

static int output_string(const char *str)
{
	printk("OOB String: %s\n", str);
	return 0;
}

static bt_mesh_input_action_t input_act;
static u8_t input_size;

static int cmd_input_num(int argc, char *argv[])
{
	int err;

	if (argc < 2) {
		return -EINVAL;
	}

	if (input_act != BT_MESH_ENTER_NUMBER) {
		printk("A number hasn't been requested!\n");
		return 0;
	}

	if (strlen(argv[1]) < input_size) {
		printk("Too short input (%u digits required)\n",
		       input_size);
		return 0;
	}

	err = bt_mesh_input_number(strtoul(argv[1], NULL, 10));
	if (err) {
		printk("Numeric input failed (err %d)\n", err);
		return 0;
	}

	input_act = BT_MESH_NO_INPUT;
	return 0;
}

struct shell_cmd_help cmd_input_num_help = {
	NULL, "<number>", NULL
};

static int cmd_input_str(int argc, char *argv[])
{
	int err;

	if (argc < 2) {
		return -EINVAL;
	}

	if (input_act != BT_MESH_ENTER_STRING) {
		printk("A string hasn't been requested!\n");
		return 0;
	}

	if (strlen(argv[1]) < input_size) {
		printk("Too short input (%u characters required)\n",
		       input_size);
		return 0;
	}

	err = bt_mesh_input_string(argv[1]);
	if (err) {
		printk("String input failed (err %d)\n", err);
		return 0;
	}

	input_act = BT_MESH_NO_INPUT;
	return 0;
}

struct shell_cmd_help cmd_input_str_help = {
	NULL, "<string>", NULL
};

static int input(bt_mesh_input_action_t act, u8_t size)
{
	switch (act) {
	case BT_MESH_ENTER_NUMBER:
		printk("Enter a number (max %u digits) with: input-num <num>\n",
		       size);
		break;
	case BT_MESH_ENTER_STRING:
		printk("Enter a string (max %u chars) with: input-str <str>\n",
		       size);
		break;
	default:
		printk("Unknown input action %u (size %u) requested!\n",
		       act, size);
		return -EINVAL;
	}

	input_act = act;
	input_size = size;
	return 0;
}

static const char *bearer2str(bt_mesh_prov_bearer_t bearer)
{
	switch (bearer) {
	case BT_MESH_PROV_ADV:
		return "PB-ADV";
	case BT_MESH_PROV_GATT:
		return "PB-GATT";
	default:
		return "unknown";
	}
}

static void link_open(bt_mesh_prov_bearer_t bearer)
{
	printk("Provisioning link opened on %s\n", bearer2str(bearer));
}

static void link_close(bt_mesh_prov_bearer_t bearer)
{
	printk("Provisioning link closed on %s\n", bearer2str(bearer));
}

static u8_t dev_uuid[16] = MYNEWT_VAL(BLE_MESH_DEV_UUID);

static u8_t static_val[16];

static struct bt_mesh_prov prov = {
	.uuid = dev_uuid,
	.link_open = link_open,
	.link_close = link_close,
	.complete = prov_complete,
	.node_added = prov_node_added,
	.reset = prov_reset,
	.static_val = NULL,
	.static_val_len = 0,
	.output_size = MYNEWT_VAL(BLE_MESH_OOB_OUTPUT_SIZE),
	.output_actions = MYNEWT_VAL(BLE_MESH_OOB_OUTPUT_ACTIONS),
	.output_number = output_number,
	.output_string = output_string,
	.input_size = MYNEWT_VAL(BLE_MESH_OOB_INPUT_SIZE),
	.input_actions = MYNEWT_VAL(BLE_MESH_OOB_INPUT_ACTIONS),
	.input = input,
	.input_complete = prov_input_complete,
};

static int cmd_static_oob(int argc, char *argv[])
{
	if (argc < 2) {
		prov.static_val = NULL;
		prov.static_val_len = 0;
	} else {
		prov.static_val_len = hex2bin(argv[1], static_val, 16);
		if (prov.static_val_len) {
			prov.static_val = static_val;
		} else {
			prov.static_val = NULL;
		}
	}

	if (prov.static_val) {
		printk("Static OOB value set (length %u)\n",
		       prov.static_val_len);
	} else {
		printk("Static OOB value cleared\n");
	}

	return 0;
}

struct shell_cmd_help cmd_static_oob_help = {
	NULL, "[val: 1-16 hex values]", NULL
};

static int cmd_uuid(int argc, char *argv[])
{
	u8_t uuid[16];
	size_t len;

	if (argc < 2) {
		return -EINVAL;
	}

	len = hex2bin(argv[1], uuid, sizeof(uuid));
	if (len < 1) {
		return -EINVAL;
	}

	memcpy(dev_uuid, uuid, len);
	memset(dev_uuid + len, 0, sizeof(dev_uuid) - len);

	printk("Device UUID set\n");

	return 0;
}

struct shell_cmd_help cmd_uuid_help = {
	NULL, "<UUID: 1-16 hex values>", NULL
};

static int cmd_reset(int argc, char *argv[])
{
	bt_mesh_reset();
	printk("Local node reset complete\n");
	return 0;
}

static u8_t str2u8(const char *str)
{
	if (isdigit(str[0])) {
		return strtoul(str, NULL, 0);
	}

	return (!strcmp(str, "on") || !strcmp(str, "enable"));
}

static bool str2bool(const char *str)
{
	return str2u8(str);
}

#if MYNEWT_VAL(BLE_MESH_LOW_POWER)
static int cmd_lpn(int argc, char *argv[])
{
	static bool enabled;
	int err;

	if (argc < 2) {
		printk("%s\n", enabled ? "enabled" : "disabled");
		return 0;
	}

	if (str2bool(argv[1])) {
		if (enabled) {
			printk("LPN already enabled\n");
			return 0;
		}

		err = bt_mesh_lpn_set(true);
		if (err) {
			printk("Enabling LPN failed (err %d)\n", err);
		} else {
			enabled = true;
		}
	} else {
		if (!enabled) {
			printk("LPN already disabled\n");
			return 0;
		}

		err = bt_mesh_lpn_set(false);
		if (err) {
			printk("Enabling LPN failed (err %d)\n", err);
		} else {
			enabled = false;
		}
	}

	return 0;
}

static int cmd_poll(int argc, char *argv[])
{
	int err;

	err = bt_mesh_lpn_poll();
	if (err) {
		printk("Friend Poll failed (err %d)\n", err);
	}

	return 0;
}

static void lpn_cb(u16_t friend_addr, bool established)
{
	if (established) {
		printk("Friendship (as LPN) established to Friend 0x%04x\n",
		       friend_addr);
	} else {
		printk("Friendship (as LPN) lost with Friend 0x%04x\n",
		       friend_addr);
	}
}

struct shell_cmd_help cmd_lpn_help = {
	NULL, "<value: off, on>", NULL
};

#endif /* MESH_LOW_POWER */

static int check_pub_addr_unassigned(void)
{
#ifdef ARCH_sim
	return 0;
#else
	uint8_t zero_addr[BLE_DEV_ADDR_LEN] = { 0 };

	return memcmp(MYNEWT_VAL(BLE_PUBLIC_DEV_ADDR),
		      zero_addr, BLE_DEV_ADDR_LEN) == 0;
#endif
}

int cmd_mesh_init(int argc, char *argv[])
{
	int err;
	ble_addr_t addr;

	if (check_pub_addr_unassigned()) {
		/* Use NRPA */
		err = ble_hs_id_gen_rnd(1, &addr);
		assert(err == 0);
		err = ble_hs_id_set_rnd(addr.val);
		assert(err == 0);

		err = bt_mesh_init(addr.type, &prov, &comp);
	}
	else {
		err = bt_mesh_init(0, &prov, &comp);
	}

	if (err) {
		printk("Mesh initialization failed (err %d)\n", err);
	}

	printk("Mesh initialized\n");

	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
	}

	if (bt_mesh_is_provisioned()) {
		printk("Mesh network restored from flash\n");
	} else {
		printk("Use \"pb-adv on\" or \"pb-gatt on\" to enable"
		       " advertising\n");
	}

#if MYNEWT_VAL(BLE_MESH_LOW_POWER)
	bt_mesh_lpn_set_cb(lpn_cb);
#endif

	return 0;
}

#if MYNEWT_VAL(BLE_MESH_GATT_PROXY)
static int cmd_ident(int argc, char *argv[])
{
	int err;

	err = bt_mesh_proxy_identity_enable();
	if (err) {
		printk("Failed advertise using Node Identity (err %d)\n", err);
	}

	return 0;
}
#endif /* MESH_GATT_PROXY */

static int cmd_dst(int argc, char *argv[])
{
	if (argc < 2) {
		printk("Destination address: 0x%04x%s\n", net.dst,
		       net.dst == net.local ? " (local)" : "");
		return 0;
	}

	if (!strcmp(argv[1], "local")) {
		net.dst = net.local;
	} else {
		net.dst = strtoul(argv[1], NULL, 0);
	}

	printk("Destination address set to 0x%04x%s\n", net.dst,
	       net.dst == net.local ? " (local)" : "");
	return 0;
}

struct shell_cmd_help cmd_dst_help = {
	NULL, "[destination address]", NULL
};

static int cmd_netidx(int argc, char *argv[])
{
	if (argc < 2) {
		printk("NetIdx: 0x%04x\n", net.net_idx);
		return 0;
	}

	net.net_idx = strtoul(argv[1], NULL, 0);
	printk("NetIdx set to 0x%04x\n", net.net_idx);
	return 0;
}

struct shell_cmd_help cmd_netidx_help = {
	NULL, "[NetIdx]", NULL
};

static int cmd_appidx(int argc, char *argv[])
{
	if (argc < 2) {
		printk("AppIdx: 0x%04x\n", net.app_idx);
		return 0;
	}

	net.app_idx = strtoul(argv[1], NULL, 0);
	printk("AppIdx set to 0x%04x\n", net.app_idx);
	return 0;
}

struct shell_cmd_help cmd_appidx_help = {
	NULL, "[AppIdx]", NULL
};

static int cmd_net_send(int argc, char *argv[])
{
	struct os_mbuf *msg = NET_BUF_SIMPLE(32);
	struct bt_mesh_msg_ctx ctx = {
		.send_ttl = BT_MESH_TTL_DEFAULT,
		.net_idx = net.net_idx,
		.addr = net.dst,
		.app_idx = net.app_idx,

	};
	struct bt_mesh_net_tx tx = {
		.ctx = &ctx,
		.src = net.local,
		.xmit = bt_mesh_net_transmit_get(),
		.sub = bt_mesh_subnet_get(net.net_idx),
	};
	size_t len;
	int err = 0;

	if (argc < 2) {
		err = -EINVAL;
		goto done;
	}

	if (!tx.sub) {
		printk("No matching subnet for NetKey Index 0x%04x\n",
		       net.net_idx);
		goto done;
	}

	net_buf_simple_init(msg, 0);
	len = hex2bin(argv[1], msg->om_data, net_buf_simple_tailroom(msg) - 4);
	net_buf_simple_add(msg, len);

	err = bt_mesh_trans_send(&tx, msg, NULL, NULL);
	if (err) {
		printk("Failed to send (err %d)\n", err);
	}

done:
	os_mbuf_free_chain(msg);
	return err;
}

struct shell_cmd_help cmd_net_send_help = {
	NULL, "<hex string>", NULL
};

static int cmd_rpl_clear(int argc, char *argv[])
{
	bt_mesh_rpl_clear();
	return 0;
}

#if MYNEWT_VAL(BLE_MESH_LOW_POWER)
static int cmd_lpn_subscribe(int argc, char *argv[])
{
	u16_t address;

	if (argc < 2) {
		return -EINVAL;
	}

	address = strtoul(argv[1], NULL, 0);

	printk("address 0x%04x", address);

	bt_mesh_lpn_group_add(address);

	return 0;
}

struct shell_cmd_help cmd_lpn_subscribe_help = {
	NULL, "<addr>", NULL
};

static int cmd_lpn_unsubscribe(int argc, char *argv[])
{
	u16_t address;

	if (argc < 2) {
		return -EINVAL;
	}

	address = strtoul(argv[1], NULL, 0);

	printk("address 0x%04x", address);

	bt_mesh_lpn_group_del(&address, 1);

	return 0;
}

struct shell_cmd_help cmd_lpn_unsubscribe_help = {
	NULL, "<addr>", NULL
};
#endif

#if MYNEWT_VAL(BLE_MESH_IV_UPDATE_TEST)
static int cmd_iv_update(int argc, char *argv[])
{
	if (bt_mesh_iv_update()) {
		printk("Transitioned to IV Update In Progress state\n");
	} else {
		printk("Transitioned to IV Update Normal state\n");
	}

	printk("IV Index is 0x%08lx\n", bt_mesh.iv_index);

	return 0;
}

static int cmd_iv_update_test(int argc, char *argv[])
{
	bool enable;

	if (argc < 2) {
		return -EINVAL;
	}

	enable = str2bool(argv[1]);
	if (enable) {
		printk("Enabling IV Update test mode\n");
	} else {
		printk("Disabling IV Update test mode\n");
	}

	bt_mesh_iv_update_test(enable);

	return 0;
}

struct shell_cmd_help cmd_iv_update_test_help = {
	NULL, "<value: off, on>", NULL
};
#endif

#if MYNEWT_VAL(BLE_MESH_CFG_CLI)

int cmd_timeout(int argc, char *argv[])
{
	s32_t timeout;

	if (argc < 2) {
		timeout = bt_mesh_cfg_cli_timeout_get();
		if (timeout == K_FOREVER) {
			printk("Message timeout: forever\n");
		} else {
			printk("Message timeout: %lu seconds\n",
			       timeout / 1000);
		}

		return 0;
	}

	timeout = strtol(argv[1], NULL, 0);
	if (timeout < 0 || timeout > (INT32_MAX / 1000)) {
		timeout = K_FOREVER;
	} else {
		timeout = timeout * 1000;
	}

	bt_mesh_cfg_cli_timeout_set(timeout);
	if (timeout == K_FOREVER) {
		printk("Message timeout: forever\n");
	} else {
		printk("Message timeout: %lu seconds\n",
		       timeout / 1000);
	}

	return 0;
}

struct shell_cmd_help cmd_timeout_help = {
	NULL, "[timeout in seconds]", NULL
};


static int cmd_get_comp(int argc, char *argv[])
{
	struct os_mbuf *comp = NET_BUF_SIMPLE(32);
	u8_t status, page = 0x00;
	int err = 0;

	if (argc > 1) {
		page = strtol(argv[1], NULL, 0);
	}

	net_buf_simple_init(comp, 0);
	err = bt_mesh_cfg_comp_data_get(net.net_idx, net.dst, page,
					&status, comp);
	if (err) {
		printk("Getting composition failed (err %d)\n", err);
		goto done;
	}

	if (status != 0x00) {
		printk("Got non-success status 0x%02x\n", status);
		goto done;
	}

	printk("Got Composition Data for 0x%04x:\n", net.dst);
	printk("\tCID      0x%04x\n", net_buf_simple_pull_le16(comp));
	printk("\tPID      0x%04x\n", net_buf_simple_pull_le16(comp));
	printk("\tVID      0x%04x\n", net_buf_simple_pull_le16(comp));
	printk("\tCRPL     0x%04x\n", net_buf_simple_pull_le16(comp));
	printk("\tFeatures 0x%04x\n", net_buf_simple_pull_le16(comp));

	while (comp->om_len > 4) {
		u8_t sig, vnd;
		u16_t loc;
		int i;

		loc = net_buf_simple_pull_le16(comp);
		sig = net_buf_simple_pull_u8(comp);
		vnd = net_buf_simple_pull_u8(comp);

		printk("\n\tElement @ 0x%04x:\n", loc);

		if (comp->om_len < ((sig * 2) + (vnd * 4))) {
			printk("\t\t...truncated data!\n");
			break;
		}

		if (sig) {
			printk("\t\tSIG Models:\n");
		} else {
			printk("\t\tNo SIG Models\n");
		}

		for (i = 0; i < sig; i++) {
			u16_t mod_id = net_buf_simple_pull_le16(comp);

			printk("\t\t\t0x%04x\n", mod_id);
		}

		if (vnd) {
			printk("\t\tVendor Models:\n");
		} else {
			printk("\t\tNo Vendor Models\n");
		}

		for (i = 0; i < vnd; i++) {
			u16_t cid = net_buf_simple_pull_le16(comp);
			u16_t mod_id = net_buf_simple_pull_le16(comp);

			printk("\t\t\tCompany 0x%04x: 0x%04x\n", cid, mod_id);
		}
	}

done:
	os_mbuf_free_chain(comp);
	return err;
}

struct shell_cmd_help cmd_get_comp_help = {
	NULL, "[page]", NULL
};

static int cmd_beacon(int argc, char *argv[])
{
	u8_t status;
	int err;

	if (argc < 2) {
		err = bt_mesh_cfg_beacon_get(net.net_idx, net.dst, &status);
	} else {
		u8_t val = str2u8(argv[1]);

		err = bt_mesh_cfg_beacon_set(net.net_idx, net.dst, val,
					     &status);
	}

	if (err) {
		printk("Unable to send Beacon Get/Set message (err %d)\n", err);
		return 0;
	}

	printk("Beacon state is 0x%02x\n", status);

	return 0;
}

struct shell_cmd_help cmd_beacon_help = {
	NULL, "[val: off, on]", NULL
};

static int cmd_ttl(int argc, char *argv[])
{
	u8_t ttl;
	int err;

	if (argc < 2) {
		err = bt_mesh_cfg_ttl_get(net.net_idx, net.dst, &ttl);
	} else {
		u8_t val = strtoul(argv[1], NULL, 0);

		err = bt_mesh_cfg_ttl_set(net.net_idx, net.dst, val, &ttl);
	}

	if (err) {
		printk("Unable to send Default TTL Get/Set (err %d)\n", err);
		return 0;
	}

	printk("Default TTL is 0x%02x\n", ttl);

	return 0;
}

struct shell_cmd_help cmd_ttl_help = {
	NULL, "[ttl: 0x00, 0x02-0x7f]", NULL
};

static int cmd_friend(int argc, char *argv[])
{
	u8_t frnd;
	int err;

	if (argc < 2) {
		err = bt_mesh_cfg_friend_get(net.net_idx, net.dst, &frnd);
	} else {
		u8_t val = str2u8(argv[1]);

		err = bt_mesh_cfg_friend_set(net.net_idx, net.dst, val, &frnd);
	}

	if (err) {
		printk("Unable to send Friend Get/Set (err %d)\n", err);
		return 0;
	}

	printk("Friend is set to 0x%02x\n", frnd);

	return 0;
}

struct shell_cmd_help cmd_friend_help = {
	NULL, "[val: off, on]", NULL
};

static int cmd_gatt_proxy(int argc, char *argv[])
{
	u8_t proxy;
	int err;

	if (argc < 2) {
		err = bt_mesh_cfg_gatt_proxy_get(net.net_idx, net.dst, &proxy);
	} else {
		u8_t val = str2u8(argv[1]);

		err = bt_mesh_cfg_gatt_proxy_set(net.net_idx, net.dst, val,
						 &proxy);
	}

	if (err) {
		printk("Unable to send GATT Proxy Get/Set (err %d)\n", err);
		return 0;
	}

	printk("GATT Proxy is set to 0x%02x\n", proxy);

	return 0;
}

struct shell_cmd_help cmd_gatt_proxy_help = {
	NULL, "[val: off, on]", NULL
};

static int cmd_relay(int argc, char *argv[])
{
	u8_t relay, transmit;
	int err;

	if (argc < 2) {
		err = bt_mesh_cfg_relay_get(net.net_idx, net.dst, &relay,
					    &transmit);
	} else {
		u8_t val = str2u8(argv[1]);
		u8_t count, interval, new_transmit;

		if (val) {
			if (argc > 2) {
				count = strtoul(argv[2], NULL, 0);
			} else {
				count = 2;
			}

			if (argc > 3) {
				interval = strtoul(argv[3], NULL, 0);
			} else {
				interval = 20;
			}

			new_transmit = BT_MESH_TRANSMIT(count, interval);
		} else {
			new_transmit = 0;
		}

		err = bt_mesh_cfg_relay_set(net.net_idx, net.dst, val,
					    new_transmit, &relay, &transmit);
	}

	if (err) {
		printk("Unable to send Relay Get/Set (err %d)\n", err);
		return 0;
	}

	printk("Relay is 0x%02x, Transmit 0x%02x (count %u interval %ums)\n",
	       relay, transmit, BT_MESH_TRANSMIT_COUNT(transmit),
	       BT_MESH_TRANSMIT_INT(transmit));

	return 0;
}

struct shell_cmd_help cmd_relay_help = {
	NULL, "[val: off, on] [count: 0-7] [interval: 0-32]", NULL
};

static int cmd_net_key_add(int argc, char *argv[])
{
	u8_t key_val[16];
	u16_t key_net_idx;
	u8_t status;
	int err;

	if (argc < 2) {
		return -EINVAL;
	}

	key_net_idx = strtoul(argv[1], NULL, 0);

	if (argc > 2) {
		size_t len;

		len = hex2bin(argv[3], key_val, sizeof(key_val));
		memset(key_val, 0, sizeof(key_val) - len);
	} else {
		memcpy(key_val, default_key, sizeof(key_val));
	}

	err = bt_mesh_cfg_net_key_add(net.net_idx, net.dst, key_net_idx,
				      key_val, &status);
	if (err) {
		printk("Unable to send NetKey Add (err %d)\n", err);
		return 0;
	}

	if (status) {
		printk("NetKeyAdd failed with status 0x%02x\n", status);
	} else {
		printk("NetKey added with NetKey Index 0x%03x\n", key_net_idx);
	}

	return 0;
}

struct shell_cmd_help cmd_net_key_add_help = {
	NULL, "<NetKeyIndex> [val]", NULL
};

static int cmd_app_key_add(int argc, char *argv[])
{
	u8_t key_val[16];
	u16_t key_net_idx, key_app_idx;
	u8_t status;
	int err;

	if (argc < 3) {
		return -EINVAL;
	}

	key_net_idx = strtoul(argv[1], NULL, 0);
	key_app_idx = strtoul(argv[2], NULL, 0);

	if (argc > 3) {
		size_t len;

		len = hex2bin(argv[3], key_val, sizeof(key_val));
		memset(key_val, 0, sizeof(key_val) - len);
	} else {
		memcpy(key_val, default_key, sizeof(key_val));
	}

	err = bt_mesh_cfg_app_key_add(net.net_idx, net.dst, key_net_idx,
				      key_app_idx, key_val, &status);
	if (err) {
		printk("Unable to send App Key Add (err %d)\n", err);
		return 0;
	}

	if (status) {
		printk("AppKeyAdd failed with status 0x%02x\n", status);
	} else {
		printk("AppKey added, NetKeyIndex 0x%04x AppKeyIndex 0x%04x\n",
		       key_net_idx, key_app_idx);
	}

	return 0;
}

struct shell_cmd_help cmd_app_key_add_help = {
	NULL, "<NetKeyIndex> <AppKeyIndex> [val]", NULL
};

static int cmd_mod_app_bind(int argc, char *argv[])
{
	u16_t elem_addr, mod_app_idx, mod_id, cid;
	u8_t status;
	int err;

	if (argc < 4) {
		return -EINVAL;
	}

	elem_addr = strtoul(argv[1], NULL, 0);
	mod_app_idx = strtoul(argv[2], NULL, 0);
	mod_id = strtoul(argv[3], NULL, 0);

	if (argc > 4) {
		cid = strtoul(argv[4], NULL, 0);
		err = bt_mesh_cfg_mod_app_bind_vnd(net.net_idx, net.dst,
						   elem_addr, mod_app_idx,
						   mod_id, cid, &status);
	} else {
		err = bt_mesh_cfg_mod_app_bind(net.net_idx, net.dst, elem_addr,
					       mod_app_idx, mod_id, &status);
	}

	if (err) {
		printk("Unable to send Model App Bind (err %d)\n", err);
		return 0;
	}

	if (status) {
		printk("Model App Bind failed with status 0x%02x\n", status);
	} else {
		printk("AppKey successfully bound\n");
	}

	return 0;
}

struct shell_cmd_help cmd_mod_app_bind_help = {
	NULL, "<addr> <AppIndex> <Model ID> [Company ID]", NULL
};

static int cmd_mod_sub_add(int argc, char *argv[])
{
	u16_t elem_addr, sub_addr, mod_id, cid;
	u8_t status;
	int err;

	if (argc < 4) {
		return -EINVAL;
	}

	elem_addr = strtoul(argv[1], NULL, 0);
	sub_addr = strtoul(argv[2], NULL, 0);
	mod_id = strtoul(argv[3], NULL, 0);

	if (argc > 4) {
		cid = strtoul(argv[4], NULL, 0);
		err = bt_mesh_cfg_mod_sub_add_vnd(net.net_idx, net.dst,
						  elem_addr, sub_addr, mod_id,
						  cid, &status);
	} else {
		err = bt_mesh_cfg_mod_sub_add(net.net_idx, net.dst, elem_addr,
					      sub_addr, mod_id, &status);
	}

	if (err) {
		printk("Unable to send Model Subscription Add (err %d)\n", err);
		return 0;
	}

	if (status) {
		printk("Model Subscription Add failed with status 0x%02x\n",
		       status);
	} else {
		printk("Model subscription was successful\n");
	}

	return 0;
}

struct shell_cmd_help cmd_mod_sub_add_help = {
	NULL, "<elem addr> <sub addr> <Model ID> [Company ID]", NULL
};

static int cmd_mod_sub_del(int argc, char *argv[])
{
	u16_t elem_addr, sub_addr, mod_id, cid;
	u8_t status;
	int err;

	if (argc < 4) {
		return -EINVAL;
	}

	elem_addr = strtoul(argv[1], NULL, 0);
	sub_addr = strtoul(argv[2], NULL, 0);
	mod_id = strtoul(argv[3], NULL, 0);

	if (argc > 4) {
		cid = strtoul(argv[4], NULL, 0);
		err = bt_mesh_cfg_mod_sub_del_vnd(net.net_idx, net.dst,
						  elem_addr, sub_addr, mod_id,
						  cid, &status);
	} else {
		err = bt_mesh_cfg_mod_sub_del(net.net_idx, net.dst, elem_addr,
					      sub_addr, mod_id, &status);
	}

	if (err) {
		printk("Unable to send Model Subscription Delete (err %d)\n",
		       err);
		return 0;
	}

	if (status) {
		printk("Model Subscription Delete failed with status 0x%02x\n",
		       status);
	} else {
		printk("Model subscription deltion was successful\n");
	}

	return 0;
}

struct shell_cmd_help cmd_mod_sub_del_help = {
	NULL, "<elem addr> <sub addr> <Model ID> [Company ID]", NULL
};

static int cmd_mod_sub_add_va(int argc, char *argv[])
{
	u16_t elem_addr, sub_addr, mod_id, cid;
	u8_t label[16];
	u8_t status;
	size_t len;
	int err;

	if (argc < 4) {
		return -EINVAL;
	}

	elem_addr = strtoul(argv[1], NULL, 0);

	len = hex2bin(argv[2], label, sizeof(label));
	memset(label + len, 0, sizeof(label) - len);

	mod_id = strtoul(argv[3], NULL, 0);

	if (argc > 4) {
		cid = strtoul(argv[4], NULL, 0);
		err = bt_mesh_cfg_mod_sub_va_add_vnd(net.net_idx, net.dst,
						     elem_addr, label, mod_id,
						     cid, &sub_addr, &status);
	} else {
		err = bt_mesh_cfg_mod_sub_va_add(net.net_idx, net.dst,
						 elem_addr, label, mod_id,
						 &sub_addr, &status);
	}

	if (err) {
		printk("Unable to send Mod Sub VA Add (err %d)\n", err);
		return 0;
	}

	if (status) {
		printk("Mod Sub VA Add failed with status 0x%02x\n",
		       status);
	} else {
		printk("0x%04x subscribed to Label UUID %s (va 0x%04x)\n",
		       elem_addr, argv[2], sub_addr);
	}

	return 0;
}

struct shell_cmd_help cmd_mod_sub_add_va_help = {
	NULL, "<elem addr> <Label UUID> <Model ID> [Company ID]", NULL
};

static int cmd_mod_sub_del_va(int argc, char *argv[])
{
	u16_t elem_addr, sub_addr, mod_id, cid;
	u8_t label[16];
	u8_t status;
	size_t len;
	int err;

	if (argc < 4) {
		return -EINVAL;
	}

	elem_addr = strtoul(argv[1], NULL, 0);

	len = hex2bin(argv[2], label, sizeof(label));
	memset(label + len, 0, sizeof(label) - len);

	mod_id = strtoul(argv[3], NULL, 0);

	if (argc > 4) {
		cid = strtoul(argv[4], NULL, 0);
		err = bt_mesh_cfg_mod_sub_va_del_vnd(net.net_idx, net.dst,
						     elem_addr, label, mod_id,
						     cid, &sub_addr, &status);
	} else {
		err = bt_mesh_cfg_mod_sub_va_del(net.net_idx, net.dst,
						 elem_addr, label, mod_id,
						 &sub_addr, &status);
	}

	if (err) {
		printk("Unable to send Model Subscription Delete (err %d)\n",
		       err);
		return 0;
	}

	if (status) {
		printk("Model Subscription Delete failed with status 0x%02x\n",
		       status);
	} else {
		printk("0x%04x unsubscribed from Label UUID %s (va 0x%04x)\n",
		       elem_addr, argv[2], sub_addr);
	}

	return 0;
}

struct shell_cmd_help cmd_mod_sub_del_va_help = {
	NULL, "<elem addr> <Label UUID> <Model ID> [Company ID]", NULL
};

static int mod_pub_get(u16_t addr, u16_t mod_id, u16_t cid)
{
	struct bt_mesh_cfg_mod_pub pub;
	u8_t status;
	int err;

	if (cid == CID_NVAL) {
		err = bt_mesh_cfg_mod_pub_get(net.net_idx, net.dst, addr,
					      mod_id, &pub, &status);
	} else {
		err = bt_mesh_cfg_mod_pub_get_vnd(net.net_idx, net.dst, addr,
						  mod_id, cid, &pub, &status);
	}

	if (err) {
		printk("Model Publication Get failed (err %d)\n", err);
		return 0;
	}

	if (status) {
		printk("Model Publication Get failed (status 0x%02x)\n",
		       status);
		return 0;
	}

	printk("Model Publication for Element 0x%04x, Model 0x%04x:\n"
	       "\tPublish Address:                0x%04x\n"
	       "\tAppKeyIndex:                    0x%04x\n"
	       "\tCredential Flag:                %u\n"
	       "\tPublishTTL:                     %u\n"
	       "\tPublishPeriod:                  0x%02x\n"
	       "\tPublishRetransmitCount:         %u\n"
	       "\tPublishRetransmitInterval:      %ums\n",
	       addr, mod_id, pub.addr, pub.app_idx, pub.cred_flag, pub.ttl,
	       pub.period, BT_MESH_PUB_TRANSMIT_COUNT(pub.transmit),
	       BT_MESH_PUB_TRANSMIT_INT(pub.transmit));

	return 0;
}

static int mod_pub_set(u16_t addr, u16_t mod_id, u16_t cid, char *argv[])
{
	struct bt_mesh_cfg_mod_pub pub;
	u8_t status, count;
	u16_t interval;
	int err;

	pub.addr = strtoul(argv[0], NULL, 0);
	pub.app_idx = strtoul(argv[1], NULL, 0);
	pub.cred_flag = str2bool(argv[2]);
	pub.ttl = strtoul(argv[3], NULL, 0);
	pub.period = strtoul(argv[4], NULL, 0);

	count = strtoul(argv[5], NULL, 0);
	if (count > 7) {
		printk("Invalid retransmit count\n");
		return -EINVAL;
	}

	interval = strtoul(argv[6], NULL, 0);
	if (interval > (31 * 50) || (interval % 50)) {
		printk("Invalid retransmit interval %u\n", interval);
		return -EINVAL;
	}

	pub.transmit = BT_MESH_PUB_TRANSMIT(count, interval);

	if (cid == CID_NVAL) {
		err = bt_mesh_cfg_mod_pub_set(net.net_idx, net.dst, addr,
					      mod_id, &pub, &status);
	} else {
		err = bt_mesh_cfg_mod_pub_set_vnd(net.net_idx, net.dst, addr,
						  mod_id, cid, &pub, &status);
	}

	if (err) {
		printk("Model Publication Set failed (err %d)\n", err);
		return 0;
	}

	if (status) {
		printk("Model Publication Set failed (status 0x%02x)\n",
		       status);
	} else {
		printk("Model Publication successfully set\n");
	}

	return 0;
}

static int cmd_mod_pub(int argc, char *argv[])
{
	u16_t addr, mod_id, cid;

	if (argc < 3) {
		return -EINVAL;
	}

	addr = strtoul(argv[1], NULL, 0);
	mod_id = strtoul(argv[2], NULL, 0);

	argc -= 3;
	argv += 3;

	if (argc == 1 || argc == 8) {
		cid = strtoul(argv[0], NULL, 0);
		argc--;
		argv++;
	} else {
		cid = CID_NVAL;
	}

	if (argc > 0) {
		if (argc < 7) {
			return -EINVAL;
		}

		return mod_pub_set(addr, mod_id, cid, argv);
	} else {
		return mod_pub_get(addr, mod_id, cid);
	}
}

struct shell_cmd_help cmd_mod_pub_help = {
	NULL, "<addr> <mod id> [cid] [<PubAddr> "
	"<AppKeyIndex> <cred> <ttl> <period> <count> <interval>]" , NULL
};

static void hb_sub_print(struct bt_mesh_cfg_hb_sub *sub)
{
	printk("Heartbeat Subscription:\n"
	       "\tSource:      0x%04x\n"
	       "\tDestination: 0x%04x\n"
	       "\tPeriodLog:   0x%02x\n"
	       "\tCountLog:    0x%02x\n"
	       "\tMinHops:     %u\n"
	       "\tMaxHops:     %u\n",
	       sub->src, sub->dst, sub->period, sub->count,
	       sub->min, sub->max);
}

static int hb_sub_get(int argc, char *argv[])
{
	struct bt_mesh_cfg_hb_sub sub;
	u8_t status;
	int err;

	err = bt_mesh_cfg_hb_sub_get(net.net_idx, net.dst, &sub, &status);
	if (err) {
		printk("Heartbeat Subscription Get failed (err %d)\n", err);
		return 0;
	}

	if (status) {
		printk("Heartbeat Subscription Get failed (status 0x%02x)\n",
		       status);
	} else {
		hb_sub_print(&sub);
	}

	return 0;
}

static int hb_sub_set(int argc, char *argv[])
{
	struct bt_mesh_cfg_hb_sub sub;
	u8_t status;
	int err;

	sub.src = strtoul(argv[1], NULL, 0);
	sub.dst = strtoul(argv[2], NULL, 0);
	sub.period = strtoul(argv[3], NULL, 0);

	err = bt_mesh_cfg_hb_sub_set(net.net_idx, net.dst, &sub, &status);
	if (err) {
		printk("Heartbeat Subscription Set failed (err %d)\n", err);
		return 0;
	}

	if (status) {
		printk("Heartbeat Subscription Set failed (status 0x%02x)\n",
		       status);
	} else {
		hb_sub_print(&sub);
	}

	return 0;
}

static int cmd_hb_sub(int argc, char *argv[])
{
	if (argc > 1) {
		if (argc < 4) {
			return -EINVAL;
		}

		return hb_sub_set(argc, argv);
	} else {
		return hb_sub_get(argc, argv);
	}
}

struct shell_cmd_help cmd_hb_sub_help = {
	NULL, "<src> <dst> <period>", NULL
};

static int hb_pub_get(int argc, char *argv[])
{
	struct bt_mesh_cfg_hb_pub pub;
	u8_t status;
	int err;

	err = bt_mesh_cfg_hb_pub_get(net.net_idx, net.dst, &pub, &status);
	if (err) {
		printk("Heartbeat Publication Get failed (err %d)\n", err);
		return 0;
	}

	if (status) {
		printk("Heartbeat Publication Get failed (status 0x%02x)\n",
		       status);
		return 0;
	}

	printk("Heartbeat publication:\n");
	printk("\tdst 0x%04x count 0x%02x period 0x%02x\n",
	       pub.dst, pub.count, pub.period);
	printk("\tttl 0x%02x feat 0x%04x net_idx 0x%04x\n",
	       pub.ttl, pub.feat, pub.net_idx);

	return 0;
}

static int hb_pub_set(int argc, char *argv[])
{
	struct bt_mesh_cfg_hb_pub pub;
	u8_t status;
	int err;

	pub.dst = strtoul(argv[1], NULL, 0);
	pub.count = strtoul(argv[2], NULL, 0);
	pub.period = strtoul(argv[3], NULL, 0);
	pub.ttl = strtoul(argv[4], NULL, 0);
	pub.feat = strtoul(argv[5], NULL, 0);
	pub.net_idx = strtoul(argv[5], NULL, 0);

	err = bt_mesh_cfg_hb_pub_set(net.net_idx, net.dst, &pub, &status);
	if (err) {
		printk("Heartbeat Publication Set failed (err %d)\n", err);
		return 0;
	}

	if (status) {
		printk("Heartbeat Publication Set failed (status 0x%02x)\n",
		       status);
	} else {
		printk("Heartbeat publication successfully set\n");
	}

	return 0;
}

static int cmd_hb_pub(int argc, char *argv[])
{
	if (argc > 1) {
		if (argc < 7) {
			return -EINVAL;
		}

		return hb_pub_set(argc, argv);
	} else {
		return hb_pub_get(argc, argv);
	}
}

struct shell_cmd_help cmd_hb_pub_help = {
	NULL, "<dst> <count> <period> <ttl> <features> <NetKeyIndex>" , NULL
};

#endif /* MYNEWT_VAL(BLE_MESH_CFG_CLI) */

#if MYNEWT_VAL(BLE_MESH_PROV)
static int cmd_pb(bt_mesh_prov_bearer_t bearer, int argc, char *argv[])
{
	int err;

	if (argc < 2) {
		return -EINVAL;
	}

	if (str2bool(argv[1])) {
		err = bt_mesh_prov_enable(bearer);
		if (err) {
			printk("Failed to enable %s (err %d)\n",
			       bearer2str(bearer), err);
		} else {
			printk("%s enabled\n", bearer2str(bearer));
		}
	} else {
		err = bt_mesh_prov_disable(bearer);
		if (err) {
			printk("Failed to disable %s (err %d)\n",
			       bearer2str(bearer), err);
		} else {
			printk("%s disabled\n", bearer2str(bearer));
		}
	}

	return 0;

}

struct shell_cmd_help cmd_pb_help = {
	NULL, "<val: off, on>", NULL
};

#endif

#if MYNEWT_VAL(BLE_MESH_PB_ADV)
static int cmd_pb_adv(int argc, char *argv[])
{
	return cmd_pb(BT_MESH_PROV_ADV, argc, argv);
}

#if MYNEWT_VAL(BLE_MESH_PROVISIONER)
static int cmd_provision_adv(int argc, char *argv[])
{
	u8_t uuid[16];
	u8_t attention_duration;
	u16_t net_idx;
	u16_t addr;
	size_t len;
	int err;

	len = hex2bin(argv[1], uuid, sizeof(uuid));
	(void)memset(uuid + len, 0, sizeof(uuid) - len);

	net_idx = strtoul(argv[2], NULL, 0);
	addr = strtoul(argv[3], NULL, 0);
	attention_duration = strtoul(argv[4], NULL, 0);

	err = bt_mesh_provision_adv(uuid, net_idx, addr, attention_duration);
	if (err) {
		printk("Provisioning failed (err %d)", err);
	}

	return 0;
}

struct shell_cmd_help cmd_provision_adv_help = {
	NULL, "<UUID> <NetKeyIndex> <addr> <AttentionDuration>" , NULL
};
#endif /* CONFIG_BT_MESH_PROVISIONER */

#endif /* CONFIG_BT_MESH_PB_ADV */

#if MYNEWT_VAL(BLE_MESH_PB_GATT)
static int cmd_pb_gatt(int argc, char *argv[])
{
	return cmd_pb(BT_MESH_PROV_GATT, argc, argv);
}
#endif /* CONFIG_BT_MESH_PB_GATT */

static int cmd_provision(int argc, char *argv[])
{
	u16_t net_idx, addr;
	u32_t iv_index;
	int err;

	if (argc < 3) {
		return -EINVAL;
	}

	net_idx = strtoul(argv[1], NULL, 0);
	addr = strtoul(argv[2], NULL, 0);

	if (argc > 3) {
		iv_index = strtoul(argv[3], NULL, 0);
	} else {
		iv_index = 0;
	}

	err = bt_mesh_provision(default_key, net_idx, 0, iv_index, addr,
				default_key);
	if (err) {
		printk("Provisioning failed (err %d)\n", err);
	}

	return 0;
}

struct shell_cmd_help cmd_provision_help = {
	NULL, "<NetKeyIndex> <addr> [IVIndex]" , NULL
};

#if MYNEWT_VAL(BLE_MESH_HEALTH_CLI)

static int cmd_fault_get(int argc, char *argv[])
{
	u8_t faults[32];
	size_t fault_count;
	u8_t test_id;
	u16_t cid;
	int err;

	if (argc < 2) {
		return -EINVAL;
	}

	cid = strtoul(argv[1], NULL, 0);
	fault_count = sizeof(faults);

	err = bt_mesh_health_fault_get(net.net_idx, net.dst, net.app_idx, cid,
				       &test_id, faults, &fault_count);
	if (err) {
		printk("Failed to send Health Fault Get (err %d)\n", err);
	} else {
		show_faults(test_id, cid, faults, fault_count);
	}

	return 0;
}

struct shell_cmd_help cmd_fault_get_help = {
	NULL, "<Company ID>", NULL
};

static int cmd_fault_clear(int argc, char *argv[])
{
	u8_t faults[32];
	size_t fault_count;
	u8_t test_id;
	u16_t cid;
	int err;

	if (argc < 2) {
		return -EINVAL;
	}

	cid = strtoul(argv[1], NULL, 0);
	fault_count = sizeof(faults);

	err = bt_mesh_health_fault_clear(net.net_idx, net.dst, net.app_idx,
					 cid, &test_id, faults, &fault_count);
	if (err) {
		printk("Failed to send Health Fault Clear (err %d)\n", err);
	} else {
		show_faults(test_id, cid, faults, fault_count);
	}

	return 0;
}

struct shell_cmd_help cmd_fault_clear_help = {
	NULL, "<Company ID>", NULL
};

static int cmd_fault_clear_unack(int argc, char *argv[])
{
	u16_t cid;
	int err;

	if (argc < 2) {
		return -EINVAL;
	}

	cid = strtoul(argv[1], NULL, 0);

	err = bt_mesh_health_fault_clear(net.net_idx, net.dst, net.app_idx,
					 cid, NULL, NULL, NULL);
	if (err) {
		printk("Health Fault Clear Unacknowledged failed (err %d)\n",
		       err);
	}

	return 0;
}

struct shell_cmd_help cmd_fault_clear_unack_help = {
	NULL, "<Company ID>", NULL
};

static int cmd_fault_test(int argc, char *argv[])
{
	u8_t faults[32];
	size_t fault_count;
	u8_t test_id;
	u16_t cid;
	int err;

	if (argc < 3) {
		return -EINVAL;
	}

	cid = strtoul(argv[1], NULL, 0);
	test_id = strtoul(argv[2], NULL, 0);
	fault_count = sizeof(faults);

	err = bt_mesh_health_fault_test(net.net_idx, net.dst, net.app_idx,
					cid, test_id, faults, &fault_count);
	if (err) {
		printk("Failed to send Health Fault Test (err %d)\n", err);
	} else {
		show_faults(test_id, cid, faults, fault_count);
	}

	return 0;
}

struct shell_cmd_help cmd_fault_test_help = {
	NULL, "<Company ID> <Test ID>", NULL
};

static int cmd_fault_test_unack(int argc, char *argv[])
{
	u16_t cid;
	u8_t test_id;
	int err;

	if (argc < 3) {
		return -EINVAL;
	}

	cid = strtoul(argv[1], NULL, 0);
	test_id = strtoul(argv[2], NULL, 0);

	err = bt_mesh_health_fault_test(net.net_idx, net.dst, net.app_idx,
					cid, test_id, NULL, NULL);
	if (err) {
		printk("Health Fault Test Unacknowledged failed (err %d)\n",
		       err);
	}

	return 0;
}

struct shell_cmd_help cmd_fault_test_unack_help = {
	NULL, "<Company ID> <Test ID>", NULL
};

static int cmd_period_get(int argc, char *argv[])
{
	u8_t divisor;
	int err;

	err = bt_mesh_health_period_get(net.net_idx, net.dst, net.app_idx,
					&divisor);
	if (err) {
		printk("Failed to send Health Period Get (err %d)\n", err);
	} else {
		printk("Health FastPeriodDivisor: %u\n", divisor);
	}

	return 0;
}

static int cmd_period_set(int argc, char *argv[])
{
	u8_t divisor, updated_divisor;
	int err;

	if (argc < 2) {
		return -EINVAL;
	}

	divisor = strtoul(argv[1], NULL, 0);

	err = bt_mesh_health_period_set(net.net_idx, net.dst, net.app_idx,
					divisor, &updated_divisor);
	if (err) {
		printk("Failed to send Health Period Set (err %d)\n", err);
	} else {
		printk("Health FastPeriodDivisor: %u\n", updated_divisor);
	}

	return 0;
}

struct shell_cmd_help cmd_period_set_help = {
	NULL, "<divisor>", NULL
};

static int cmd_period_set_unack(int argc, char *argv[])
{
	u8_t divisor;
	int err;

	if (argc < 2) {
		return -EINVAL;
	}

	divisor = strtoul(argv[1], NULL, 0);

	err = bt_mesh_health_period_set(net.net_idx, net.dst, net.app_idx,
					divisor, NULL);
	if (err) {
		printk("Failed to send Health Period Set (err %d)\n", err);
	}

	return 0;
}

struct shell_cmd_help cmd_period_set_unack_help = {
	NULL, "<divisor>", NULL
};

static int cmd_attention_get(int argc, char *argv[])
{
	u8_t attention;
	int err;

	err = bt_mesh_health_attention_get(net.net_idx, net.dst, net.app_idx,
					   &attention);
	if (err) {
		printk("Failed to send Health Attention Get (err %d)\n", err);
	} else {
		printk("Health Attention Timer: %u\n", attention);
	}

	return 0;
}

static int cmd_attention_set(int argc, char *argv[])
{
	u8_t attention, updated_attention;
	int err;

	if (argc < 2) {
		return -EINVAL;
	}

	attention = strtoul(argv[1], NULL, 0);

	err = bt_mesh_health_attention_set(net.net_idx, net.dst, net.app_idx,
					   attention, &updated_attention);
	if (err) {
		printk("Failed to send Health Attention Set (err %d)\n", err);
	} else {
		printk("Health Attention Timer: %u\n", updated_attention);
	}

	return 0;
}

struct shell_cmd_help cmd_attention_set_help = {
	NULL, "<timer>", NULL
};

static int cmd_attention_set_unack(int argc, char *argv[])
{
	u8_t attention;
	int err;

	if (argc < 2) {
		return -EINVAL;
	}

	attention = strtoul(argv[1], NULL, 0);

	err = bt_mesh_health_attention_set(net.net_idx, net.dst, net.app_idx,
					   attention, NULL);
	if (err) {
		printk("Failed to send Health Attention Set (err %d)\n", err);
	}

	return 0;
}

struct shell_cmd_help cmd_attention_set_unack_help = {
	NULL, "<timer>", NULL
};

#endif /* MYNEWT_VAL(BLE_MESH_HEALTH_CLI) */

static int cmd_add_fault(int argc, char *argv[])
{
	u8_t fault_id;
	u8_t i;

	if (argc < 2) {
		return -EINVAL;
	}

	fault_id = strtoul(argv[1], NULL, 0);
	if (!fault_id) {
		printk("The Fault ID must be non-zero!\n");
		return -EINVAL;
	}

	for (i = 0; i < sizeof(cur_faults); i++) {
		if (!cur_faults[i]) {
			cur_faults[i] = fault_id;
			break;
		}
	}

	if (i == sizeof(cur_faults)) {
		printk("Fault array is full. Use \"del-fault\" to clear it\n");
		return 0;
	}

	for (i = 0; i < sizeof(reg_faults); i++) {
		if (!reg_faults[i]) {
			reg_faults[i] = fault_id;
			break;
		}
	}

	if (i == sizeof(reg_faults)) {
		printk("No space to store more registered faults\n");
	}

	bt_mesh_fault_update(&elements[0]);

	return 0;
}

struct shell_cmd_help cmd_add_fault_help = {
	NULL, "<Fault ID>", NULL
};

static int cmd_del_fault(int argc, char *argv[])
{
	u8_t fault_id;
	u8_t i;

	if (argc < 2) {
		memset(cur_faults, 0, sizeof(cur_faults));
		printk("All current faults cleared\n");
		bt_mesh_fault_update(&elements[0]);
		return 0;
	}

	fault_id = strtoul(argv[1], NULL, 0);
	if (!fault_id) {
		printk("The Fault ID must be non-zero!\n");
		return -EINVAL;
	}

	for (i = 0; i < sizeof(cur_faults); i++) {
		if (cur_faults[i] == fault_id) {
			cur_faults[i] = 0;
			printk("Fault cleared\n");
		}
	}

	bt_mesh_fault_update(&elements[0]);

	return 0;
}

struct shell_cmd_help cmd_del_fault_help = {
	NULL, "[Fault ID]", NULL
};

#if MYNEWT_VAL(BLE_MESH_SHELL_MODELS)
static int cmd_gen_onoff_get(int argc, char *argv[])
{
	u8_t state;
	int err;

	err = bt_mesh_gen_onoff_get(net.net_idx, net.dst, net.app_idx,
				    &state);
	if (err) {
		printk("Failed to send Generic OnOff Get (err %d)\n", err);
	} else {
		printk("Gen OnOff State %d\n", state);
	}

	return 0;
}

static int cmd_gen_onoff_set(int argc, char *argv[])
{
	u8_t state;
	u8_t val;
	int err;

	if (argc < 2) {
		return -EINVAL;
	}

	val = strtoul(argv[1], NULL, 0);

	err = bt_mesh_gen_onoff_set(net.net_idx, net.dst, net.app_idx,
				    val, &state);
	if (err) {
		printk("Failed to send Generic OnOff Get (err %d)\n", err);
	} else {
		printk("Gen OnOff State %d\n", state);
	}

	return 0;
}

struct shell_cmd_help cmd_gen_onoff_set_help = {
	NULL, "<0|1>", NULL
};

static int cmd_gen_onoff_set_unack(int argc, char *argv[])
{
	u8_t val;
	int err;

	if (argc < 2) {
		return -EINVAL;
	}

	val = strtoul(argv[1], NULL, 0);

	err = bt_mesh_gen_onoff_set(net.net_idx, net.dst, net.app_idx,
				    val, NULL);
	if (err) {
		printk("Failed to send Generic OnOff Get (err %d)\n", err);
	}

	return 0;
}

struct shell_cmd_help cmd_gen_onoff_set_unack_help = {
	NULL, "<0|1>", NULL
};

static int cmd_gen_level_get(int argc, char *argv[])
{
	s16_t state;
	int err;

	err = bt_mesh_gen_level_get(net.net_idx, net.dst, net.app_idx,
				    &state);
	if (err) {
		printk("Failed to send Generic Level Get (err %d)\n", err);
	} else {
		printk("Gen Level State %d\n", state);
	}

	return 0;
}

static int cmd_gen_level_set(int argc, char *argv[])
{
	s16_t state;
	s16_t val;
	int err;

	if (argc < 2) {
		return -EINVAL;
	}

	val = (s16_t)strtoul(argv[1], NULL, 0);

	err = bt_mesh_gen_level_set(net.net_idx, net.dst, net.app_idx,
				    val, &state);
	if (err) {
		printk("Failed to send Generic Level Get (err %d)\n", err);
	} else {
		printk("Gen Level State %d\n", state);
	}

	return 0;
}

struct shell_cmd_help cmd_gen_level_set_help = {
	NULL, "<level>", NULL
};

static int cmd_gen_level_set_unack(int argc, char *argv[])
{
	s16_t val;
	int err;

	if (argc < 2) {
		return -EINVAL;
	}

	val = (s16_t)strtoul(argv[1], NULL, 0);

	err = bt_mesh_gen_level_set(net.net_idx, net.dst, net.app_idx,
				    val, NULL);
	if (err) {
		printk("Failed to send Generic Level Get (err %d)\n", err);
	}

	return 0;
}

struct shell_cmd_help cmd_gen_level_set_unack_help = {
	NULL, "<level>", NULL
};

#endif /* MYNEWT_VAL(BLE_MESH_SHELL_MODELS) */

static int cmd_print_credentials(int argc, char *argv[])
{
	bt_test_print_credentials();
	return 0;
}

static void print_comp_elem(struct bt_mesh_elem *elem,
			    bool primary)
{
	struct bt_mesh_model *mod;
	int i;

	printk("Loc: %u\n", elem->loc);
	printk("Model count: %u\n", elem->model_count);
	printk("Vnd model count: %u\n", elem->vnd_model_count);

	for (i = 0; i < elem->model_count; i++) {
		mod = &elem->models[i];
		printk("  Model: %u\n", i);
		printk("    ID: 0x%04x\n", mod->id);
		printk("    Opcode: 0x%08lx\n", mod->op->opcode);
	}

	for (i = 0; i < elem->vnd_model_count; i++) {
		mod = &elem->vnd_models[i];
		printk("  Vendor model: %u\n", i);
		printk("    Company: 0x%04x\n", mod->vnd.company);
		printk("    ID: 0x%04x\n", mod->vnd.id);
		printk("    Opcode: 0x%08lx\n", mod->op->opcode);
	}
}

static int cmd_print_composition_data(int argc, char *argv[])
{
	const struct bt_mesh_comp *comp;
	int i;

	comp = bt_mesh_comp_get();

	printk("CID: %u\n", comp->cid);
	printk("PID: %u\n", comp->pid);
	printk("VID: %u\n", comp->vid);

	for (i = 0; i < comp->elem_count; i++) {
		print_comp_elem(&comp->elem[i], i == 0);
	}

	return 0;
}


static const struct shell_cmd mesh_commands[] = {
    {
        .sc_cmd = "init",
        .sc_cmd_func = cmd_mesh_init,
        .help = NULL,
    },
#if MYNEWT_VAL(BLE_MESH_PB_ADV)
    {
        .sc_cmd = "pb-adv",
        .sc_cmd_func = cmd_pb_adv,
        .help = &cmd_pb_help,
    },
#if MYNEWT_VAL(BLE_MESH_PROVISIONER)
    {
        .sc_cmd = "provision-adv",
        .sc_cmd_func = cmd_provision_adv,
        .help = &cmd_provision_adv_help,
    },
#endif
#endif
#if MYNEWT_VAL(BLE_MESH_PB_GATT)
    {
        .sc_cmd = "pb-gatt",
        .sc_cmd_func = cmd_pb_gatt,
        .help = &cmd_pb_help,
    },
#endif
    {
        .sc_cmd = "reset",
        .sc_cmd_func = cmd_reset,
        .help = NULL,
    },
    {
        .sc_cmd = "uuid",
        .sc_cmd_func = cmd_uuid,
        .help = &cmd_uuid_help,
    },
    {
        .sc_cmd = "input-num",
        .sc_cmd_func = cmd_input_num,
        .help = &cmd_input_num_help,
    },
    {
        .sc_cmd = "input-str",
        .sc_cmd_func = cmd_input_str,
        .help = &cmd_input_str_help,
    },
    {
        .sc_cmd = "static-oob",
        .sc_cmd_func = cmd_static_oob,
        .help = &cmd_static_oob_help,
    },
    {
        .sc_cmd = "provision",
        .sc_cmd_func = cmd_provision,
        .help = &cmd_provision_help,
    },
#if MYNEWT_VAL(BLE_MESH_LOW_POWER)
    {
        .sc_cmd = "lpn",
        .sc_cmd_func = cmd_lpn,
        .help = &cmd_lpn_help,
    },
    {
        .sc_cmd = "poll",
        .sc_cmd_func = cmd_poll,
        .help = NULL,
    },
#endif
#if MYNEWT_VAL(BLE_MESH_GATT_PROXY)
    {
        .sc_cmd = "ident",
        .sc_cmd_func = cmd_ident,
        .help = NULL,
    },
#endif
    {
        .sc_cmd = "dst",
        .sc_cmd_func = cmd_dst,
        .help = &cmd_dst_help,
    },
    {
        .sc_cmd = "netidx",
        .sc_cmd_func = cmd_netidx,
        .help = &cmd_netidx_help,
    },
    {
        .sc_cmd = "appidx",
        .sc_cmd_func = cmd_appidx,
        .help = &cmd_appidx_help,
    },

    /* Commands which access internal APIs, for testing only */
    {
        .sc_cmd = "net-send",
        .sc_cmd_func = cmd_net_send,
        .help = &cmd_net_send_help,
    },
#if MYNEWT_VAL(BLE_MESH_IV_UPDATE_TEST)
    {
        .sc_cmd = "iv-update",
        .sc_cmd_func = cmd_iv_update,
        .help = NULL,
    },
    {
        .sc_cmd = "iv-update-test",
        .sc_cmd_func = cmd_iv_update_test,
        .help = &cmd_iv_update_test_help,
    },
#endif
    {
        .sc_cmd = "rpl-clear",
        .sc_cmd_func = cmd_rpl_clear,
        .help = NULL,
    },
#if MYNEWT_VAL(BLE_MESH_LOW_POWER)
    {
        .sc_cmd = "lpn-subscribe",
        .sc_cmd_func = cmd_lpn_subscribe,
        .help = &cmd_lpn_subscribe_help,
    },
    {
        .sc_cmd = "lpn-unsubscribe",
        .sc_cmd_func = cmd_lpn_unsubscribe,
        .help = &cmd_lpn_unsubscribe_help,
    },
#endif
    {
        .sc_cmd = "print-credentials",
        .sc_cmd_func = cmd_print_credentials,
        .help = NULL,
    },
    {
        .sc_cmd = "print-composition-data",
        .sc_cmd_func = cmd_print_composition_data,
        .help = NULL,
    },


#if MYNEWT_VAL(BLE_MESH_CFG_CLI)
    /* Configuration Client Model operations */
    {
        .sc_cmd = "timeout",
        .sc_cmd_func = cmd_timeout,
        .help = &cmd_timeout_help,
    },
    {
        .sc_cmd = "get-comp",
        .sc_cmd_func = cmd_get_comp,
        .help = &cmd_get_comp_help,
    },
    {
        .sc_cmd = "beacon",
        .sc_cmd_func = cmd_beacon,
        .help = &cmd_beacon_help,
    },
    {
        .sc_cmd = "ttl",
        .sc_cmd_func = cmd_ttl,
        .help = &cmd_ttl_help,
    },
    {
        .sc_cmd = "friend",
        .sc_cmd_func = cmd_friend,
        .help = &cmd_friend_help,
    },
    {
        .sc_cmd = "gatt-proxy",
        .sc_cmd_func = cmd_gatt_proxy,
        .help = &cmd_gatt_proxy_help,
    },
    {
        .sc_cmd = "relay",
        .sc_cmd_func = cmd_relay,
        .help = &cmd_relay_help,
    },
    {
        .sc_cmd = "net-key-add",
        .sc_cmd_func = cmd_net_key_add,
        .help = &cmd_net_key_add_help,
    },
    {
        .sc_cmd = "app-key-add",
        .sc_cmd_func = cmd_app_key_add,
        .help = &cmd_app_key_add_help,
    },
    {
        .sc_cmd = "mod-app-bind",
        .sc_cmd_func = cmd_mod_app_bind,
        .help = &cmd_mod_app_bind_help,
    },
    {
        .sc_cmd = "mod-pub",
        .sc_cmd_func = cmd_mod_pub,
        .help = &cmd_mod_pub_help,
    },
    {
        .sc_cmd = "mod-sub-add",
        .sc_cmd_func = cmd_mod_sub_add,
        .help = &cmd_mod_sub_add_help,
    },
    {
        .sc_cmd = "mod-sub-del",
        .sc_cmd_func = cmd_mod_sub_del,
        .help = &cmd_mod_sub_del_help,
    },
    {
        .sc_cmd = "mod-sub-add-va",
        .sc_cmd_func = cmd_mod_sub_add_va,
        .help = &cmd_mod_sub_add_va_help,
    },
    {
        .sc_cmd = "mod-sub-del-va",
        .sc_cmd_func = cmd_mod_sub_del_va,
        .help = &cmd_mod_sub_del_va_help,
    },
    {
        .sc_cmd = "hb-sub",
        .sc_cmd_func = cmd_hb_sub,
        .help = &cmd_hb_sub_help,
    },
    {
        .sc_cmd = "hb-pub",
        .sc_cmd_func = cmd_hb_pub,
        .help = &cmd_hb_pub_help,
    },
#endif

#if MYNEWT_VAL(BLE_MESH_HEALTH_CLI)
    /* Health Client Model Operations */
    {
        .sc_cmd = "fault-get",
        .sc_cmd_func = cmd_fault_get,
        .help = &cmd_fault_get_help,
    },
    {
        .sc_cmd = "fault-clear",
        .sc_cmd_func = cmd_fault_clear,
        .help = &cmd_fault_clear_help,
    },
    {
        .sc_cmd = "fault-clear-unack",
        .sc_cmd_func = cmd_fault_clear_unack,
        .help = &cmd_fault_clear_unack_help,
    },
    {
        .sc_cmd = "fault-test",
        .sc_cmd_func = cmd_fault_test,
        .help = &cmd_fault_test_help,
    },
    {
        .sc_cmd = "fault-test-unack",
        .sc_cmd_func = cmd_fault_test_unack,
        .help = &cmd_fault_test_unack_help,
    },
    {
        .sc_cmd = "period-get",
        .sc_cmd_func = cmd_period_get,
        .help = NULL,
    },
    {
        .sc_cmd = "period-set",
        .sc_cmd_func = cmd_period_set,
        .help = &cmd_period_set_help,
    },
    {
        .sc_cmd = "period-set-unack",
        .sc_cmd_func = cmd_period_set_unack,
        .help = &cmd_period_set_unack_help,
    },
    {
        .sc_cmd = "attention-get",
        .sc_cmd_func = cmd_attention_get,
        .help = NULL,
    },
    {
        .sc_cmd = "attention-set",
        .sc_cmd_func = cmd_attention_set,
        .help = &cmd_attention_set_help,
    },
    {
        .sc_cmd = "attention-set-unack",
        .sc_cmd_func = cmd_attention_set_unack,
        .help = &cmd_attention_set_unack_help,
    },
#endif

    /* Health Server Model Operations */
    {
        .sc_cmd = "add-fault",
        .sc_cmd_func = cmd_add_fault,
        .help = &cmd_add_fault_help,
    },
    {
        .sc_cmd = "del-fault",
        .sc_cmd_func = cmd_del_fault,
        .help = &cmd_del_fault_help,
    },

#if MYNEWT_VAL(BLE_MESH_SHELL_MODELS)
    /* Generic Client Model Operations */
    {
        .sc_cmd = "gen-onoff-get",
        .sc_cmd_func = cmd_gen_onoff_get,
        .help = NULL,
    },
    {
        .sc_cmd = "gen-onoff-set",
        .sc_cmd_func = cmd_gen_onoff_set,
        .help = &cmd_gen_onoff_set_help,
    },
    {
        .sc_cmd = "gen-onoff-set-unack",
        .sc_cmd_func = cmd_gen_onoff_set_unack,
        .help = &cmd_gen_onoff_set_unack_help,
    },
    {
        .sc_cmd = "gen-level-get",
        .sc_cmd_func = cmd_gen_level_get,
        .help = NULL,
    },
    {
        .sc_cmd = "gen-level-set",
        .sc_cmd_func = cmd_gen_level_set,
        .help = &cmd_gen_level_set_help,
    },
    {
        .sc_cmd = "gen-level-set-unack",
        .sc_cmd_func = cmd_gen_level_set_unack,
        .help = &cmd_gen_level_set_unack_help,
    },
#endif

    { 0 },
};

static void mesh_shell_thread(void *args)
{
	while (1) {
		os_eventq_run(&mesh_shell_queue);
	}
}

static void bt_mesh_shell_task_init(void)
{
	os_eventq_init(&mesh_shell_queue);

	os_task_init(&mesh_shell_task, "mesh_sh", mesh_shell_thread, NULL,
		     BLE_MESH_SHELL_TASK_PRIO, OS_WAIT_FOREVER, g_blemesh_shell_stack,
		     BLE_MESH_SHELL_STACK_SIZE);
}
#endif

void ble_mesh_shell_init(void)
{
#if (MYNEWT_VAL(BLE_MESH_SHELL))

	/* Initialize health pub message */
	health_pub_init();

	/* Shell and other mesh clients should use separate task to
	   avoid deadlocks with mesh message processing queue */
	bt_mesh_shell_task_init();
	shell_evq_set(&mesh_shell_queue);
	shell_register("mesh", mesh_commands);

#endif
}
#endif
