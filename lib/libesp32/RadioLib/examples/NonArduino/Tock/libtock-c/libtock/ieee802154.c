#include <string.h>

#include "ieee802154.h"
#include "timer.h"

const int RADIO_DRIVER = 0x30001;

const int ALLOW_RX  = 0;
const int ALLOW_CFG = 1;

const int ALLOW_RO_TX = 0;

const int SUBSCRIBE_RX = 0;
const int SUBSCRIBE_TX = 1;

const int COMMAND_STATUS        = 1;
const int COMMAND_SET_ADDR      = 2;
const int COMMAND_SET_ADDR_LONG = 3;
const int COMMAND_SET_PAN       = 4;
const int COMMAND_SET_CHANNEL   = 5;
const int COMMAND_SET_POWER     = 6;
const int COMMAND_CONFIG_COMMIT = 7;

const int COMMAND_GET_ADDR      = 8;
const int COMMAND_GET_ADDR_LONG = 9;
const int COMMAND_GET_PAN       = 10;
const int COMMAND_GET_CHANNEL   = 11;
const int COMMAND_GET_POWER     = 12;

const int COMMAND_MAX_NEIGHBORS          = 13;
const int COMMAND_NUM_NEIGHBORS          = 14;
const int COMMAND_GET_NEIGHBOR_ADDR      = 15;
const int COMMAND_GET_NEIGHBOR_ADDR_LONG = 16;
const int COMMAND_ADD_NEIGHBOR    = 17;
const int COMMAND_REMOVE_NEIGHBOR = 18;

const int COMMAND_MAX_KEYS      = 19;
const int COMMAND_NUM_KEYS      = 20;
const int COMMAND_GET_KEY_LEVEL = 21;
const int COMMAND_GET_KEY_ID    = 22;
const int COMMAND_GET_KEY       = 23;
const int COMMAND_ADD_KEY       = 24;
const int COMMAND_REMOVE_KEY    = 25;

const int COMMAND_SEND = 26;

// Temporary buffer used for some commands where the system call interface
// parameters / return codes are not enough te contain the required data.
unsigned char BUF_CFG[27];

bool ieee802154_driver_is_present(void) {
  return driver_exists(RADIO_DRIVER);
}

int ieee802154_up(void) {
  // Spin until radio is on. Maybe this can be done with a callback?
  while (!ieee802154_is_up()) {
    delay_ms(10);
  }
  delay_ms(10); // without this delay, immediate calls to send can still fail.
  return RETURNCODE_SUCCESS;
}

int ieee802154_down(void) {
  // Currently unsupported: there is no way to implement this with the existing
  // radio interface.
  return RETURNCODE_ENOSUPPORT;
}

bool ieee802154_is_up(void) {
  return command(RADIO_DRIVER, COMMAND_STATUS, 0, 0).type == TOCK_SYSCALL_SUCCESS;
}

int ieee802154_set_address(unsigned short addr) {
  syscall_return_t com = command(RADIO_DRIVER, COMMAND_SET_ADDR, (unsigned int) addr, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ieee802154_set_address_long(unsigned char *addr_long) {
  if (!addr_long) return RETURNCODE_EINVAL;

  allow_rw_return_t rw = allow_readwrite(RADIO_DRIVER, ALLOW_CFG, (void *) addr_long, 8);
  if (!rw.success) return tock_status_to_returncode(rw.status);

  syscall_return_t com = command(RADIO_DRIVER, COMMAND_SET_ADDR_LONG, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ieee802154_set_pan(unsigned short pan) {
  syscall_return_t com = command(RADIO_DRIVER, COMMAND_SET_PAN, (unsigned int) pan, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ieee802154_set_channel(unsigned char channel) {
  syscall_return_t com = command(RADIO_DRIVER, COMMAND_SET_CHANNEL, (unsigned int) channel, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ieee802154_set_power(char power) {
  // Cast the signed char to an unsigned char before zero-padding it.
  syscall_return_t com = command(RADIO_DRIVER, COMMAND_SET_POWER, (unsigned int) (unsigned char) power, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ieee802154_config_commit(void) {
  syscall_return_t com = command(RADIO_DRIVER, COMMAND_CONFIG_COMMIT, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ieee802154_get_address(unsigned short *addr) {
  if (!addr) return RETURNCODE_EINVAL;

  syscall_return_t com = command(RADIO_DRIVER, COMMAND_GET_ADDR, 0, 0);
  int ret = tock_command_return_u32_to_returncode(com, (uint32_t*) addr);

  // Driver adds 1 to make the value positive.
  *addr -= 1;

  return ret;
}

int ieee802154_get_address_long(unsigned char *addr_long) {
  if (!addr_long) return RETURNCODE_EINVAL;

  allow_rw_return_t rw = allow_readwrite(RADIO_DRIVER, ALLOW_CFG, (void *) addr_long, 8);
  if (!rw.success) return tock_status_to_returncode(rw.status);

  syscall_return_t com = command(RADIO_DRIVER, COMMAND_GET_ADDR_LONG, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ieee802154_get_pan(unsigned short *pan) {
  if (!pan) return RETURNCODE_EINVAL;

  syscall_return_t com = command(RADIO_DRIVER, COMMAND_GET_PAN, 0, 0);
  int ret = tock_command_return_u32_to_returncode(com, (uint32_t*) pan);

  // Driver adds 1 to make the value positive.
  *pan -= 1;

  return ret;
}

int ieee802154_get_channel(unsigned char *channel) {
  if (!channel) return RETURNCODE_EINVAL;

  syscall_return_t com = command(RADIO_DRIVER, COMMAND_GET_PAN, 0, 0);
  int ret = tock_command_return_u32_to_returncode(com, (uint32_t*) channel);

  // Driver adds 1 to make the value positive.
  *channel -= 1;

  return ret;
}

int ieee802154_get_power(char *power) {
  if (!power) return RETURNCODE_EINVAL;

  syscall_return_t com = command(RADIO_DRIVER, COMMAND_GET_POWER, 0, 0);
  int ret = tock_command_return_u32_to_returncode(com, (uint32_t*) power);

  // Driver adds 1 to the power after casting it to unsigned, so this works
  *power -= 1;

  return ret;
}

int ieee802154_max_neighbors(int* neighbors) {
  syscall_return_t com = command(RADIO_DRIVER, COMMAND_MAX_NEIGHBORS, 0, 0);
  int ret = tock_command_return_u32_to_returncode(com, (uint32_t*) neighbors);

  // Driver adds 1 to the power after casting it to unsigned, so this works
  *neighbors -= 1;

  return ret;
}

int ieee802154_num_neighbors(int* neighbors) {
  syscall_return_t com = command(RADIO_DRIVER, COMMAND_NUM_NEIGHBORS, 0, 0);
  int ret = tock_command_return_u32_to_returncode(com, (uint32_t*) neighbors);

  // Driver adds 1 to the power after casting it to unsigned, so this works
  *neighbors -= 1;

  return ret;
}

int ieee802154_get_neighbor_address(unsigned index, unsigned short *addr) {
  if (!addr) return RETURNCODE_EINVAL;

  syscall_return_t com = command(RADIO_DRIVER, COMMAND_GET_NEIGHBOR_ADDR, (unsigned int) index, 0);
  int ret = tock_command_return_u32_to_returncode(com, (uint32_t*) addr);

  // Driver adds 1 to ensure it is positive.
  *addr -= 1;

  return ret;
}

int ieee802154_get_neighbor_address_long(unsigned index, unsigned char *addr_long) {
  if (!addr_long) return RETURNCODE_EINVAL;

  allow_rw_return_t rw = allow_readwrite(RADIO_DRIVER, ALLOW_CFG, (void *) addr_long, 8);
  if (!rw.success) return tock_status_to_returncode(rw.status);

  syscall_return_t com = command(RADIO_DRIVER, COMMAND_GET_NEIGHBOR_ADDR_LONG, (unsigned int) index, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ieee802154_get_neighbor(unsigned        index,
                            unsigned short *addr,
                            unsigned char * addr_long) {
  int err = ieee802154_get_neighbor_address(index, addr);
  if (err < 0) return err;
  return ieee802154_get_neighbor_address_long(index, addr_long);
}

int ieee802154_add_neighbor(unsigned short addr, unsigned char *addr_long, unsigned *index) {
  if (!addr_long) return RETURNCODE_EINVAL;

  allow_rw_return_t rw = allow_readwrite(RADIO_DRIVER, ALLOW_CFG, (void *) addr_long, 8);
  if (!rw.success) return tock_status_to_returncode(rw.status);

  syscall_return_t com = command(RADIO_DRIVER, COMMAND_ADD_NEIGHBOR, (unsigned int) addr, 0);
  int ret = tock_command_return_u32_to_returncode(com, (uint32_t*) index);

  // Driver adds 1 to ensure it is positive.
  *index -= 1;

  return ret;
}

int ieee802154_remove_neighbor(unsigned index) {
  syscall_return_t com = command(RADIO_DRIVER, COMMAND_REMOVE_NEIGHBOR, (unsigned int) index, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ieee802154_max_keys(int* keys) {
  syscall_return_t com = command(RADIO_DRIVER, COMMAND_MAX_KEYS, 0, 0);
  int ret = tock_command_return_u32_to_returncode(com, (uint32_t*) keys);

  // Driver adds 1 to ensure it is positive.
  *keys -= 1;

  return ret;
}

int ieee802154_num_keys(int* keys) {
  syscall_return_t com = command(RADIO_DRIVER, COMMAND_NUM_KEYS, 0, 0);
  int ret = tock_command_return_u32_to_returncode(com, (uint32_t*) keys);

  // Driver adds 1 to ensure it is positive.
  *keys -= 1;

  return ret;
}

int ieee802154_get_key_security_level(unsigned index, security_level_t *level) {
  if (!level) return RETURNCODE_EINVAL;

  syscall_return_t com = command(RADIO_DRIVER, COMMAND_GET_KEY_LEVEL, (unsigned int) index, 0);
  int store_u32        = 0;
  int ret = tock_command_return_u32_to_returncode(com, (uint32_t*) store_u32);

  if (ret == RETURNCODE_SUCCESS) {
    // Driver adds 1 to ensure it is positive.
    store_u32 -= 1;
    *level     = store_u32;
  }

  return ret;
}

int ieee802154_key_id_bytes(key_id_mode_t key_id_mode) {
  switch (key_id_mode) {
    default:
    case KEY_ID_IMPLICIT:
      return 0;
    case KEY_ID_INDEX:
      return 1;
    case KEY_ID_SRC_4_INDEX:
      return 5;
    case KEY_ID_SRC_8_INDEX:
      return 9;
  }
}

int ieee802154_get_key_id(unsigned       index,
                          key_id_mode_t *key_id_mode,
                          unsigned char *key_id) {
  if (!key_id_mode || !key_id) return RETURNCODE_EINVAL;

  allow_rw_return_t rw = allow_readwrite(RADIO_DRIVER, ALLOW_CFG, (void *) BUF_CFG, 10);
  if (!rw.success) return tock_status_to_returncode(rw.status);

  syscall_return_t com = command(RADIO_DRIVER, COMMAND_GET_KEY_ID, (unsigned int) index, 0);
  int ret = tock_command_return_novalue_to_returncode(com);
  if (ret == RETURNCODE_SUCCESS) {
    *key_id_mode = (key_id_mode_t) (BUF_CFG[0]);
    memcpy(key_id, BUF_CFG + 1, ieee802154_key_id_bytes(*key_id_mode));
  }

  return ret;
}

int ieee802154_get_key(unsigned index, unsigned char *key) {
  if (!key) return RETURNCODE_EINVAL;

  allow_rw_return_t rw = allow_readwrite(RADIO_DRIVER, ALLOW_CFG, (void *) key, 16);
  if (!rw.success) return tock_status_to_returncode(rw.status);

  syscall_return_t com = command(RADIO_DRIVER, COMMAND_GET_KEY, (unsigned int) index, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int ieee802154_get_key_desc(unsigned          index,
                            security_level_t *level,
                            key_id_mode_t *   key_id_mode,
                            unsigned char *   key_id,
                            unsigned char *   key) {
  int err = ieee802154_get_key_security_level(index, level);
  if (err < 0) return err;
  err = ieee802154_get_key_id(index, key_id_mode, key_id);
  if (err < 0) return err;
  return ieee802154_get_key(index, key);
}

int ieee802154_add_key(security_level_t level,
                       key_id_mode_t    key_id_mode,
                       unsigned char *  key_id,
                       unsigned char *  key,
                       unsigned *       index) {
  if (!key) return RETURNCODE_EINVAL;

  allow_rw_return_t rw = allow_readwrite(RADIO_DRIVER, ALLOW_CFG, (void *) BUF_CFG, 27);
  if (!rw.success) return tock_status_to_returncode(rw.status);

  BUF_CFG[0] = level;
  BUF_CFG[1] = key_id_mode;
  int bytes = ieee802154_key_id_bytes(key_id_mode);
  if (bytes > 0) {
    memcpy(BUF_CFG + 2, key_id, bytes);
  }
  memcpy(BUF_CFG + 2 + 9, key, 16);

  syscall_return_t com = command(RADIO_DRIVER, COMMAND_ADD_KEY, 0, 0);
  int ret = tock_command_return_u32_to_returncode(com, (uint32_t*) index);

  // Driver adds 1 to ensure it is positive.
  *index -= 1;

  return ret;
}

int ieee802154_remove_key(unsigned index) {
  syscall_return_t com = command(RADIO_DRIVER, COMMAND_REMOVE_KEY, (unsigned int) index, 0);
  return tock_command_return_novalue_to_returncode(com);
}

// Internal callback for transmission
static int tx_result;
static int tx_acked;
static void tx_done_callback(int                          status,
                             int                          acked,
                             __attribute__ ((unused)) int arg3,
                             void*                        ud) {
  tx_result     = tock_status_to_returncode(status);
  tx_acked      = acked;
  *((bool*) ud) = true;
}

int ieee802154_send(unsigned short   addr,
                    security_level_t level,
                    key_id_mode_t    key_id_mode,
                    unsigned char *  key_id,
                    const char *     payload,
                    unsigned char    len) {
  // Setup parameters in ALLOW_CFG and ALLOW_RO_TX
  allow_rw_return_t rw = allow_readwrite(RADIO_DRIVER, ALLOW_CFG, (void *) BUF_CFG, 11);
  if (!rw.success) return tock_status_to_returncode(rw.status);

  BUF_CFG[0] = level;
  BUF_CFG[1] = key_id_mode;
  int bytes = ieee802154_key_id_bytes(key_id_mode);
  if (bytes > 0) {
    memcpy(BUF_CFG + 2, key_id, bytes);
  }
  allow_ro_return_t ro = allow_readonly(RADIO_DRIVER, ALLOW_RO_TX, (void *) payload, len);
  if (!ro.success) return tock_status_to_returncode(ro.status);

  // Subscribe to the transmit callback
  bool tx_done = false;
  subscribe_return_t sub = subscribe(RADIO_DRIVER, SUBSCRIBE_TX,
                                     tx_done_callback, (void *) &tx_done);
  if (!sub.success) return tock_status_to_returncode(sub.status);

  // Issue the send command and wait for the transmission to be done.
  syscall_return_t com = command(RADIO_DRIVER, COMMAND_SEND, (unsigned int) addr, 0);
  int ret = tock_command_return_novalue_to_returncode(com);
  if (ret < 0) return ret;

  yield_for(&tx_done);
  if (tx_result != RETURNCODE_SUCCESS) {
    return tx_result;
  } else if (tx_acked == 0) {
    return RETURNCODE_ENOACK;
  }

  return RETURNCODE_SUCCESS;
}

// Internal callback for receive
static void rx_done_callback(__attribute__ ((unused)) int pans,
                             __attribute__ ((unused)) int dst_addr,
                             __attribute__ ((unused)) int src_addr,
                             void*                        ud) {
  ieee802154_unallow_rx_buf();
  *((bool*) ud) = true;
}

int ieee802154_receive_sync(const char *frame, unsigned char len) {
  // Provide the buffer to the kernel
  allow_rw_return_t rw = allow_readwrite(RADIO_DRIVER, ALLOW_RX, (void *) frame, len);
  if (!rw.success) return tock_status_to_returncode(rw.status);

  // Subscribe to the received callback
  bool rx_done = false;
  subscribe_return_t sub = subscribe(RADIO_DRIVER, SUBSCRIBE_RX, rx_done_callback, (void *) &rx_done);
  if (!sub.success) return tock_status_to_returncode(sub.status);

  // Wait for a frame
  yield_for(&rx_done);
  return RETURNCODE_SUCCESS;
}

// must be called before accessing the contents of the "allowed" receive buffer
bool ieee802154_unallow_rx_buf(void) {
  allow_rw_return_t rw = allow_readwrite(RADIO_DRIVER, ALLOW_RX, NULL, 0);
  return rw.success;
}

int ieee802154_receive(subscribe_upcall callback,
                       const char *     frame,
                       unsigned char    len) {
  // Provide the buffer to the kernel
  allow_rw_return_t rw = allow_readwrite(RADIO_DRIVER, ALLOW_RX, (void *) frame, len);
  if (!rw.success) return tock_status_to_returncode(rw.status);

  subscribe_return_t sub = subscribe(RADIO_DRIVER, SUBSCRIBE_RX, callback, NULL);
  return tock_subscribe_return_to_returncode(sub);
}

int ieee802154_frame_get_length(const char *frame) {
  if (!frame) return 0;
  // data_offset + data_len - 2 header bytes
  return frame[0] + frame[1] - 2;
}

int ieee802154_frame_get_payload_offset(const char *frame) {
  if (!frame) return 0;
  return frame[0];
}

int ieee802154_frame_get_payload_length(const char *frame) {
  if (!frame) return 0;
  return frame[1];
}

// Utility function to determine if the source and destination PAN and
// addresses are present depending on the frame control field. This is used
// only internally as a header parsing subroutine. Supports only 2003, 2006 or
// 2015 frame versions. Returns false if the addressing mode combination is
// invalid or the frame version is not supported. All out-parameters must be
// provided.
//
// If the source pan is dropped, that means that it is the same as the
// destination pan, which must be present.
static bool ieee802154_get_addressing(uint16_t     frame_control,
                                      bool *       dst_pan_present,
                                      addr_mode_t *dst_mode,
                                      bool *       src_pan_present,
                                      bool *       src_pan_dropped,
                                      addr_mode_t *src_mode) {
  if (!dst_pan_present || !dst_mode || !src_pan_present || !src_pan_dropped ||
      !src_mode) {
    return false;
  }

  typedef enum {
    VERSION_2003 = 0x0,
    VERSION_2006 = 0x1,
    VERSION_2015 = 0x2,
  } version_t;

  // Fields that determine if the PANs are present
  version_t version = (version_t) ((frame_control >> 12) & 0x3);
  *dst_mode = (addr_mode_t) ((frame_control >> 10) & 0x3);
  *src_mode = (addr_mode_t) ((frame_control >> 14) & 0x3);
  bool pan_id_compression = (frame_control >> 6) & 0x1;
  bool dst_present        = dst_mode != ADDR_NONE;
  bool src_present        = src_mode != ADDR_NONE;

  // The flags that we are trying to determine
  *src_pan_dropped = false;

  // IEEE 802.15.4: Section 7.2.1.5 determines whether the PANs are present
  // depending on the pan ID compression field and the addressing modes.
  if (version == VERSION_2015) {
    if (dst_present) {
      if (src_present) {
        *src_pan_dropped = pan_id_compression;
        *dst_pan_present = true;
        *src_pan_present = !pan_id_compression;
      } else {
        *dst_pan_present = !pan_id_compression;
        *src_pan_present = false;
      }
    } else {
      if (src_present) {
        *dst_pan_present = false;
        *src_pan_present = !pan_id_compression;
      } else {
        *dst_pan_present = pan_id_compression;
        *src_pan_present = false;
      }
    }
  } else if (version == VERSION_2003 || version == VERSION_2006) {
    *src_pan_dropped = pan_id_compression;
    *dst_pan_present = dst_present;
    *src_pan_present = src_present && !src_pan_dropped;
  } else {
    return false;
  }

  // Check validity of addressing modes
  if (*src_pan_dropped && !*dst_pan_present) {
    return 0xff;
  }

  return true;
}

addr_mode_t ieee802154_frame_get_dst_addr(__attribute__ ((unused)) const char *    frame,
                                          __attribute__ ((unused)) unsigned short *short_addr,
                                          __attribute__ ((unused)) unsigned char * long_addr) {
  if (!frame) return ADDR_NONE;
  uint16_t frame_control = ((uint16_t) frame[2]) | (((uint16_t) frame[3]) << 8);
  bool dst_pan_present, src_pan_present, src_pan_dropped;
  addr_mode_t dst_mode, src_mode;
  if (!ieee802154_get_addressing(frame_control, &dst_pan_present, &dst_mode,
                                 &src_pan_present, &src_pan_dropped, &src_mode)) {
    return ADDR_NONE;
  }

  // The addressing fields are after the sequence number, which can be ommitted
  const uint16_t SEQ_SUPPRESSED = 0x0100;
  int addr_offset = (frame_control & SEQ_SUPPRESSED) ? 4 : 5;
  if (dst_pan_present) addr_offset += 2;

  if (dst_mode == ADDR_SHORT && short_addr) {
    *short_addr = ((unsigned short) frame[addr_offset]) |
                  (((unsigned short) frame[addr_offset + 1]) << 8);
  }
  if (dst_mode == ADDR_LONG && long_addr) {
    int i;
    for (i = 0; i < 8; i++) {
      long_addr[i] = frame[addr_offset + 7 - i];
    }
  }

  return dst_mode;
}

addr_mode_t ieee802154_frame_get_src_addr(__attribute__ ((unused)) const char *    frame,
                                          __attribute__ ((unused)) unsigned short *short_addr,
                                          __attribute__ ((unused)) unsigned char * long_addr) {
  if (!frame) return ADDR_NONE;
  uint16_t frame_control = ((uint16_t) frame[2]) | (((uint16_t) frame[3]) << 8);
  bool dst_pan_present, src_pan_present, src_pan_dropped;
  addr_mode_t dst_mode, src_mode;
  if (!ieee802154_get_addressing(frame_control, &dst_pan_present, &dst_mode,
                                 &src_pan_present, &src_pan_dropped, &src_mode)) {
    return ADDR_NONE;
  }

  // The addressing fields are after the sequence number, which can be ommitted
  const uint16_t SEQ_SUPPRESSED = 0x0100;
  int addr_offset = (frame_control & SEQ_SUPPRESSED) ? 4 : 5;
  if (dst_pan_present) addr_offset += 2;
  if (dst_mode == ADDR_SHORT) {
    addr_offset += 2;
  } else if (dst_mode == ADDR_LONG) {
    addr_offset += 8;
  }
  if (src_pan_present) addr_offset += 2;

  if (src_mode == ADDR_SHORT && short_addr) {
    *short_addr = ((unsigned short) frame[addr_offset]) |
                  (((unsigned short) frame[addr_offset + 1]) << 8);
  }
  if (src_mode == ADDR_LONG && long_addr) {
    int i;
    for (i = 0; i < 8; i++) {
      long_addr[i] = frame[addr_offset + 7 - i];
    }
  }

  return src_mode;
}

bool ieee802154_frame_get_dst_pan(__attribute__ ((unused)) const char *    frame,
                                  __attribute__ ((unused)) unsigned short *pan) {
  if (!frame) return false;
  uint16_t frame_control = ((uint16_t) frame[2]) | (((uint16_t) frame[3]) << 8);
  bool dst_pan_present, src_pan_present, src_pan_dropped;
  addr_mode_t dst_mode, src_mode;
  if (!ieee802154_get_addressing(frame_control, &dst_pan_present, &dst_mode,
                                 &src_pan_present, &src_pan_dropped, &src_mode)) {
    return false;
  }

  // The addressing fields are after the sequence number, which can be ommitted
  const uint16_t SEQ_SUPPRESSED = 0x0100;
  int addr_offset = (frame_control & SEQ_SUPPRESSED) ? 4 : 5;

  if (dst_pan_present && pan) {
    *pan = ((unsigned short) frame[addr_offset]) |
           (((unsigned short) frame[addr_offset + 1]) << 8);
  }

  return dst_pan_present;
}

bool ieee802154_frame_get_src_pan(__attribute__ ((unused)) const char *    frame,
                                  __attribute__ ((unused)) unsigned short *pan) {
  if (!frame) return false;
  uint16_t frame_control = ((uint16_t) frame[2]) | (((uint16_t) frame[3]) << 8);
  bool dst_pan_present, src_pan_present, src_pan_dropped;
  addr_mode_t dst_mode, src_mode;
  if (!ieee802154_get_addressing(frame_control, &dst_pan_present, &dst_mode,
                                 &src_pan_present, &src_pan_dropped, &src_mode)) {
    return false;
  }

  // The addressing fields are after the sequence number, which can be ommitted
  const uint16_t SEQ_SUPPRESSED = 0x0100;
  int addr_offset = (frame_control & SEQ_SUPPRESSED) ? 4 : 5;

  if (src_pan_dropped) {
    // We can assume that the destination pan is present.
    if (pan) {
      *pan = ((unsigned short) frame[addr_offset]) |
             (((unsigned short) frame[addr_offset + 1]) << 8);
    }
  } else {
    if (dst_pan_present) addr_offset += 2;
    if (dst_mode == ADDR_SHORT) {
      addr_offset += 2;
    } else if (dst_mode == ADDR_LONG) {
      addr_offset += 8;
    }

    if (src_pan_present && pan) {
      *pan = ((unsigned short) frame[addr_offset]) |
             (((unsigned short) frame[addr_offset + 1]) << 8);
    }
  }

  return src_pan_present || src_pan_dropped;
}
