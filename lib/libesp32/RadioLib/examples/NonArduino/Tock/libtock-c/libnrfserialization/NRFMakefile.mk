# makefile for user application
CURRENT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += simple_ble.c
APPLICATION_SRCS += simple_adv.c
APPLICATION_SRCS += eddystone.c

APPLICATION_SRCS += app_ble_gap_sec_keys.c
APPLICATION_SRCS += app_ble_user_mem.c
APPLICATION_SRCS += app_mw_ble.c
APPLICATION_SRCS += app_mw_ble_gap.c
APPLICATION_SRCS += app_mw_ble_gattc.c
APPLICATION_SRCS += app_mw_ble_gatts.c
APPLICATION_SRCS += app_mw_ble_l2cap.c
APPLICATION_SRCS += app_mw_nrf_soc.c
APPLICATION_SRCS += ble_enable.c
APPLICATION_SRCS += ble_event.c
APPLICATION_SRCS += ble_evt_tx_complete.c
APPLICATION_SRCS += ble_db_discovery.c
APPLICATION_SRCS += ble_evt_user_mem_release.c
APPLICATION_SRCS += ble_evt_user_mem_request.c
APPLICATION_SRCS += ble_gap_address_get.c
APPLICATION_SRCS += ble_gap_address_set.c
APPLICATION_SRCS += ble_gap_adv_data_set.c
APPLICATION_SRCS += ble_gap_adv_start.c
APPLICATION_SRCS += ble_gap_adv_stop.c
APPLICATION_SRCS += ble_gap_appearance_get.c
APPLICATION_SRCS += ble_gap_appearance_set.c
APPLICATION_SRCS += ble_gap_auth_key_reply.c
APPLICATION_SRCS += ble_gap_authenticate.c
APPLICATION_SRCS += ble_gap_conn_param_update.c
APPLICATION_SRCS += ble_gap_conn_sec_get.c
APPLICATION_SRCS += ble_gap_connect.c
APPLICATION_SRCS += ble_gap_connect_cancel.c
APPLICATION_SRCS += ble_gap_device_name_get.c
APPLICATION_SRCS += ble_gap_device_name_set.c
APPLICATION_SRCS += ble_gap_disconnect.c
APPLICATION_SRCS += ble_gap_encrypt.c
APPLICATION_SRCS += ble_gap_evt_adv_report.c
APPLICATION_SRCS += ble_gap_evt_auth_key_request.c
APPLICATION_SRCS += ble_gap_evt_auth_status.c
APPLICATION_SRCS += ble_gap_evt_conn_param_update.c
APPLICATION_SRCS += ble_gap_evt_conn_param_update_request.c
APPLICATION_SRCS += ble_gap_evt_conn_sec_update.c
APPLICATION_SRCS += ble_gap_evt_connected.c
APPLICATION_SRCS += ble_gap_evt_disconnected.c
APPLICATION_SRCS += ble_gap_evt_passkey_display.c
APPLICATION_SRCS += ble_gap_evt_rssi_changed.c
APPLICATION_SRCS += ble_gap_evt_scan_req_report.c
APPLICATION_SRCS += ble_gap_evt_sec_info_request.c
APPLICATION_SRCS += ble_gap_evt_sec_params_request.c
APPLICATION_SRCS += ble_gap_evt_sec_request.c
APPLICATION_SRCS += ble_gap_evt_timeout.c
APPLICATION_SRCS += ble_gap_ppcp_get.c
APPLICATION_SRCS += ble_gap_ppcp_set.c
APPLICATION_SRCS += ble_gap_rssi_get.c
APPLICATION_SRCS += ble_gap_rssi_start.c
APPLICATION_SRCS += ble_gap_rssi_stop.c
APPLICATION_SRCS += ble_gap_scan_start.c
APPLICATION_SRCS += ble_gap_scan_stop.c
APPLICATION_SRCS += ble_gap_sec_info_reply.c
APPLICATION_SRCS += ble_gap_sec_params_reply.c
APPLICATION_SRCS += ble_gap_tx_power_set.c
APPLICATION_SRCS += ble_gattc_char_value_by_uuid_read.c
APPLICATION_SRCS += ble_gattc_char_values_read.c
APPLICATION_SRCS += ble_gattc_characteristics_discover.c
APPLICATION_SRCS += ble_gattc_descriptors_discover.c
APPLICATION_SRCS += ble_gattc_evt_char_disc_rsp.c
APPLICATION_SRCS += ble_gattc_evt_char_val_by_uuid_read_rsp.c
APPLICATION_SRCS += ble_gattc_evt_char_vals_read_rsp.c
APPLICATION_SRCS += ble_gattc_evt_desc_disc_rsp.c
APPLICATION_SRCS += ble_gattc_evt_hvx.c
APPLICATION_SRCS += ble_gattc_evt_prim_srvc_disc_rsp.c
APPLICATION_SRCS += ble_gattc_evt_read_rsp.c
APPLICATION_SRCS += ble_gattc_evt_rel_disc_rsp.c
APPLICATION_SRCS += ble_gattc_evt_timeout.c
APPLICATION_SRCS += ble_gattc_evt_write_rsp.c
APPLICATION_SRCS += ble_gattc_hv_confirm.c
APPLICATION_SRCS += ble_gattc_primary_services_discover.c
APPLICATION_SRCS += ble_gattc_read.c
APPLICATION_SRCS += ble_gattc_relationships_discover.c
APPLICATION_SRCS += ble_gattc_write.c
APPLICATION_SRCS += ble_gatts_characteristic_add.c
APPLICATION_SRCS += ble_gatts_descriptor_add.c
APPLICATION_SRCS += ble_gatts_evt_hvc.c
APPLICATION_SRCS += ble_gatts_evt_rw_authorize_request.c
APPLICATION_SRCS += ble_gatts_evt_sc_confirm.c
APPLICATION_SRCS += ble_gatts_evt_sys_attr_missing.c
APPLICATION_SRCS += ble_gatts_evt_timeout.c
APPLICATION_SRCS += ble_gatts_evt_write.c
APPLICATION_SRCS += ble_gatts_hvx.c
APPLICATION_SRCS += ble_gatts_include_add.c
APPLICATION_SRCS += ble_gatts_rw_authorize_reply.c
APPLICATION_SRCS += ble_gatts_service_add.c
APPLICATION_SRCS += ble_gatts_service_changed.c
APPLICATION_SRCS += ble_gatts_sys_attr_get.c
APPLICATION_SRCS += ble_gatts_sys_attr_set.c
APPLICATION_SRCS += ble_gatts_value_get.c
APPLICATION_SRCS += ble_gatts_value_set.c
APPLICATION_SRCS += ble_l2cap_cid_register.c
APPLICATION_SRCS += ble_l2cap_cid_unregister.c
APPLICATION_SRCS += ble_l2cap_evt_rx.c
APPLICATION_SRCS += ble_l2cap_tx.c
APPLICATION_SRCS += ble_opt_get.c
APPLICATION_SRCS += ble_opt_set.c
APPLICATION_SRCS += ble_user_mem_reply.c
APPLICATION_SRCS += ble_uuid_decode.c
APPLICATION_SRCS += ble_uuid_encode.c
APPLICATION_SRCS += ble_uuid_vs_add.c
APPLICATION_SRCS += ble_version_get.c
APPLICATION_SRCS += power_system_off.c
APPLICATION_SRCS += temp_get.c
APPLICATION_SRCS += ble_gap_evt_key_pressed.c
APPLICATION_SRCS += ble_gap_evt_lesc_dhkey_request.c
APPLICATION_SRCS += ble_gattc_evt_attr_info_disc_rsp.c

APPLICATION_SRCS += ble_gap_struct_serialization.c
APPLICATION_SRCS += ble_struct_serialization.c
APPLICATION_SRCS += ble_gatts_struct_serialization.c
APPLICATION_SRCS += ble_gattc_struct_serialization.c

APPLICATION_SRCS += ble_serialization.c
APPLICATION_SRCS += cond_field_serialization.c

APPLICATION_SRCS += ser_hal_transport.c
APPLICATION_SRCS += ser_sd_transport.c
APPLICATION_SRCS += ser_softdevice_handler.c

APPLICATION_SRCS += app_mailbox.c
APPLICATION_SRCS += ble_advdata.c
APPLICATION_SRCS += ble_conn_params.c
APPLICATION_SRCS += ble_srv_common.c
APPLICATION_SRCS += app_error.c

LIBRARY_PATHS += $(CURRENT_DIR)


DEVICE := SAM4L
SOFTDEVICE_MODEL := s130
SDK_VERSION = 11

SERIALIZATION_MODE = application

RAM_KB   ?= 32
FLASH_KB ?= 256

NRF_BASE_PATH ?= nrf5x-base

# Guess nRF51 unless otherwise set
NRF_MODEL = nrf51
NRF_IC = nrf51822

# Set default board
BOARD ?= BOARD_CUSTOM

USE_BLE = 1
SOFTDEVICE_VERSION = 2.0.0

# Location for BLE Address if stored in Flash
BLEADDR_FLASH_LOCATION ?= 0x0007FFF8
CFLAGS += -DBLEADDR_FLASH_LOCATION=$(BLEADDR_FLASH_LOCATION)

# Add tock specific useful paths
#LIBRARY_PATHS += $(TOCK_APPS_DIR)/libs

# Add useful paths from nRF5x-base
LIBRARY_PATHS += $(NRF_BASE_PATH)/advertisement/
LIBRARY_PATHS += $(NRF_BASE_PATH)/devices/
LIBRARY_PATHS += $(NRF_BASE_PATH)/lib/
LIBRARY_PATHS += $(NRF_BASE_PATH)/peripherals/
LIBRARY_PATHS += $(NRF_BASE_PATH)/services/

SOURCE_PATHS += $(CURRENT_DIR)
SOURCE_PATHS += $(NRF_BASE_PATH)/advertisement/
SOURCE_PATHS += $(NRF_BASE_PATH)/devices/
SOURCE_PATHS += $(NRF_BASE_PATH)/lib/
SOURCE_PATHS += $(NRF_BASE_PATH)/peripherals/
SOURCE_PATHS += $(NRF_BASE_PATH)/services/
SOURCE_PATHS += $(NRF_BASE_PATH)/startup


# Add paths for each SDK version
# Set the path
SDK_PATH ?= $(NRF_BASE_PATH)/sdk/nrf51_sdk_11.0.0/

# Other knowns about the SDK paths
SDK_INCLUDE_PATH   = $(SDK_PATH)components/
SDK_SOURCE_PATH    = $(SDK_PATH)components/
CMSIS_INCLUDE_PATH = $(SDK_PATH)components/toolchain/gcc/

# Need to add the paths for all the directories in the SDK.
# Note that we do not use * because some folders have conflicting files.
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)libraries/*/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/adc/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/ble_flash/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/clock/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/common/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/delay/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/gpiote/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/hal/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/lpcomp/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/ppi/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/pstorage/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/pstorage/config/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/radio_config/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/rng/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/rtc/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/sdio/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/spi_master/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/spi_slave/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/swi/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/timer/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/twi_master/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/uart/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/wdt/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_nrf/validation/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)drivers_ext/*/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)device/)
LIBRARY_PATHS += $(SDK_INCLUDE_PATH)toolchain/gcc/
LIBRARY_PATHS += $(SDK_INCLUDE_PATH)toolchain/
LIBRARY_PATHS += $(SDK_INCLUDE_PATH)toolchain/CMSIS/Include/

SOURCE_PATHS += $(SDK_SOURCE_PATH)
SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)*/)
SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)libraries/*/)
SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)drivers_nrf/*/)
SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)drivers_ext/*/)

LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/*/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/common/transport/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/$(SERIALIZATION_MODE)/codecs/common/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/$(SERIALIZATION_MODE)/hal/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/$(SERIALIZATION_MODE)/transport/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/common/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/common/transport/ser_phy/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)serialization/common/transport/ser_phy/config/)

SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/*/)
SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/common/transport/)
SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/$(SERIALIZATION_MODE)/codecs/common/)
SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/$(SERIALIZATION_MODE)/hal/)
SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/$(SERIALIZATION_MODE)/transport/)
SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/common/)
SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/common/transport/ser_phy/)
SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)serialization/common/transport/ser_phy/config/)


CFLAGS += -DSVCALL_AS_NORMAL_FUNCTION -DBLE_STACK_SUPPORT_REQD

# How many central/peripherals are defined changes how much memory the
# softdevice requires. Change the amount of memory allotted in a custom ld
# file if your configuration is different than default.

ifeq ($(RAM_KB), 16)
  # limit 16 kB RAM nRFs to only act as peripherals. Doing otherwise
  # requires careful balancing of memory requirements and should be done
  # manually, not automatically
  CENTRAL_LINK_COUNT ?= 0
  PERIPHERAL_LINK_COUNT ?= 1
else
  CENTRAL_LINK_COUNT ?= 1
  PERIPHERAL_LINK_COUNT ?= 1
endif

LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)ble/*/)
LIBRARY_PATHS += $(wildcard $(SDK_INCLUDE_PATH)ble/ble_services/*/)
SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)ble/*/)
SOURCE_PATHS += $(wildcard $(SDK_SOURCE_PATH)ble/ble_services/*/)
CFLAGS += -DBLE_STACK_SUPPORT_REQD -DSOFTDEVICE_PRESENT -DCENTRAL_LINK_COUNT=$(CENTRAL_LINK_COUNT) -DPERIPHERAL_LINK_COUNT=$(PERIPHERAL_LINK_COUNT)

ifdef ENABLE_WIRELESS_DFU
    CFLAGS += -DENABLE_DFU
    APPLICATION_SRCS += bootloader_util.c
endif

LIBRARY_PATHS += $(SDK_INCLUDE_PATH)softdevice/common/softdevice_handler/
LIBRARY_PATHS += $(SDK_INCLUDE_PATH)softdevice/$(SOFTDEVICE_MODEL)/headers/
LIBRARY_PATHS += $(SDK_INCLUDE_PATH)softdevice/$(SOFTDEVICE_MODEL)/headers/nrf51

SOURCE_PATHS += $(SDK_SOURCE_PATH)softdevice/common/softdevice_handler/
SOURCE_PATHS += $(SDK_SOURCE_PATH)softdevice/$(SOFTDEVICE_MODEL)/headers/
SOURCE_PATHS += $(SDK_SOURCE_PATH)softdevice/$(SOFTDEVICE_MODEL)/headers/nrf51

# Load the sources from the serialization library
LIBRARY_PATHS += $(SDK_INCLUDE_PATH)serialization/$(SERIALIZATION_MODE)/codecs/$(SOFTDEVICE_MODEL)/middleware/
LIBRARY_PATHS += $(SDK_INCLUDE_PATH)serialization/$(SERIALIZATION_MODE)/codecs/$(SOFTDEVICE_MODEL)/serializers/
LIBRARY_PATHS += $(SDK_INCLUDE_PATH)serialization/common/struct_ser/$(SOFTDEVICE_MODEL)/

SOURCE_PATHS += $(SDK_SOURCE_PATH)serialization/$(SERIALIZATION_MODE)/codecs/$(SOFTDEVICE_MODEL)/middleware/
SOURCE_PATHS += $(SDK_SOURCE_PATH)serialization/$(SERIALIZATION_MODE)/codecs/$(SOFTDEVICE_MODEL)/serializers/
SOURCE_PATHS += $(SDK_SOURCE_PATH)serialization/common/struct_ser/$(SOFTDEVICE_MODEL)/



print-% : ; @echo $* = $($*)

LIBRARY_INCLUDES = $(addprefix -I,$(LIBRARY_PATHS))
CMSIS_INCLUDE = $(addprefix -I,$(CMSIS_INCLUDE_PATH))

VPATH = $(SOURCE_PATHS)

CFLAGS += -g -D$(DEVICE) -D$(BOARD) $(LIBRARY_INCLUDES) -std=c11 -Os -DSDK_VERSION_$(SDK_VERSION) -DSOFTDEVICE_$(SOFTDEVICE_MODEL)
CFLAGS += -D$(shell echo $(SOFTDEVICE_MODEL) | tr a-z A-Z)
CFLAGS += -D__TOCK__
COMPILE_ONLY += -c

# These are the stock NRF rules. Needs to be done per-arch by our Makefile so we replace them in our Makefile.
#SRCS = $(SYSTEM_FILE) $(notdir $(APPLICATION_SRCS))
#OBJS = $(addprefix $(OUTPUT_PATH), $(SRCS:.c=.o)) $(addprefix $(OUTPUT_PATH),$(APPLICATION_LIBS))
#HDRS = $(addprefix $(OUTPUT_PATH), $(SRCS:.c=.headers)) $(addprefix $(OUTPUT_PATH),$(APPLICATION_LIBS))
