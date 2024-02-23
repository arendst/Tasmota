// Implementation of an RF233 radio stack for the Tock operating
// system. Adapted from the Contiki RF233 stack provided as part
// of Atmel Studio. The licenses for the original code are at the
// bottom of the file to reduce clutter.
//
// Author: Philip Levis <pal@cs.stanford.edu>
// Date: April 18 2016


#include <gpio.h>
#include <spi.h>
#include <stdint.h>
#include <timer.h>

#include "rf233-arch.h"
#include "rf233-config.h"
#include "rf233-const.h"
#include "rf233.h"
#include "trx_access.h"

#define RADIO_SLP 8
#define RADIO_RST 9
#define RADIO_IRQ 10

/*---------------------------------------------------------------------------*/
static int on(void);
static int off(void);
static void rf_generate_random_seed(void);
static void flush_buffer(void);
static uint8_t flag_transmit      = 0;
static uint8_t ack_status         = 0;
static volatile int radio_is_on   = 0;
static volatile int pending_frame = 0;
static volatile int sleep_on      = 0;
static int rf233_prepare_without_header(const uint8_t *data, unsigned short data_len);
static int rf233_setup(void);
static int rf233_prepare(const void *payload, unsigned short payload_len);
static int rf233_transmit(void);
static int (*rx_callback)(void*, int, uint16_t, uint16_t, uint16_t);
static int set_callback = 0;

#define IEEE802154_CONF_PANID 0x1111

enum {
  RADIO_TX_OK = 0,
  RADIO_TX_ERR = 1,
  RADIO_TX_NOACK = 2,
  RADIO_TX_COLLISION = 3
};

typedef struct {
  uint16_t fcf;
  uint8_t seq;
  uint16_t pan;
  uint16_t dest;
  uint16_t src;
  char payload[];
} __attribute__((packed)) header_t;

static header_t radio_header;

/*---------------------------------------------------------------------------*/
int rf233_read(void *buf, unsigned short bufsize);
int rf233_receiving_packet(void);
int rf233_pending_packet(void);

/*---------------------------------------------------------------------------*/
/* convenience macros */
// #define RF233_STATUS()                    rf233_arch_status()
#define RF233_COMMAND(c)                  trx_reg_write(RF233_REG_TRX_STATE, c)

/* each frame has a footer consisting of LQI, ED, RX_STATUS added by the radio */
// #define FOOTER_LEN                        3   /* bytes */
#define MAX_PACKET_LEN                    127 /* bytes, excluding the length (first) byte */
#define PACKETBUF_SIZE                    128 /* bytes, for even int writes */
#define HEADER_SIZE                       9 /* bytes */

/*---------------------------------------------------------------------------*/
#define _DEBUG_               0
#define DEBUG_PRINTDATA       0    /* print frames to/from the radio; requires DEBUG == 1 */
#if _DEBUG_
#define PRINTF(...)       printf(__VA_ARGS__)

// Used for debugging output
static const char* state_str(uint8_t state) {
  switch (state) {
    case STATE_P_ON:
      return "STATE_POWER_ON";
    case STATE_BUSY_RX:
      return "STATE_BUSY_RX";
    case STATE_BUSY_TX:
      return "STATE_BUSY_TX";
    case STATE_RX_ON:
      return "STATE_RX_ON";
    case STATE_TRX_OFF:
      return "STATE_TRX_OFF";
    case STATE_PLL_ON:
      return "STATE_PLL_ON";
    case STATE_SLEEP:
      return "STATE_SLEEP";
    case STATE_PREP_DEEP_SLEEP:
      return "STATE_PREP_DEEP_SLEEP";
    case STATE_BUSY_RX_AACK:
      return "STATE_BUSY_RX_AACK";
    case STATE_BUSY_TX_ARET:
      return "STATE_BUSY_TX_ARET";
    case STATE_RX_AACK_ON:
      return "STATE_RX_AACK_ON";
    case STATE_TX_ARET_ON:
      return "STATE_TX_ARET_ON";
    case STATE_TRANSITION:
      return "STATE_TRANSITION";
    default:
      return "STATE UNKNOWN!!!";
  }
}

#else
#define PRINTF(...)
#endif


// Section 9.8 of the RF233 manual suggests recalibrating filters at
// least every 5 minutes of operation. Transitioning out of sleep
// resets the filters automatically.
#pragma GCC diagnostic ignored "-Wunused-function"
static void calibrate_filters(void) {
  PRINTF("RF233: Calibrating filters.\n");
  trx_reg_write(RF233_REG_FTN_CTRL, 0x80);
  while (trx_reg_read(RF233_REG_FTN_CTRL) & 0x80);
}

uint8_t recv_data[PACKETBUF_SIZE];
uint8_t packetbuf[PACKETBUF_SIZE];

static void* packetbuf_dataptr(void) {
  return (void*)packetbuf;
}

static void packetbuf_clear(void) {
  int* ptr = (int*)packetbuf;
  for (int i = 0; i < PACKETBUF_SIZE / 4; i++) {
    *ptr++ = 0x00000000;
  }
}

uint8_t trx_reg_read(uint8_t addr) {
  uint8_t command = addr | READ_ACCESS_COMMAND;
  char buf[2];
  buf[0] = command;
  buf[1] = 0;
  spi_read_write_sync(buf, buf, 2);
  return buf[1];
}

uint8_t trx_bit_read(uint8_t addr, uint8_t mask, uint8_t pos) {
  uint8_t ret;
  ret   = trx_reg_read(addr);
  ret  &= mask;
  ret >>= pos;
  return ret;
}

void trx_reg_write(uint8_t addr, uint8_t data) {
  uint8_t command = addr | WRITE_ACCESS_COMMAND;
  char buf[2];
  buf[0] = command;
  buf[1] = data;
  spi_write_sync(buf, 2);
  return;
}

void trx_bit_write(uint8_t reg_addr,
                   uint8_t mask,
                   uint8_t pos,
                   uint8_t new_value) {
  uint8_t current_reg_value;
  current_reg_value  = trx_reg_read(reg_addr);
  current_reg_value &= ~mask;
  new_value        <<= pos;
  new_value         &= mask;
  new_value         |= current_reg_value;
  trx_reg_write(reg_addr, new_value);
}

void trx_sram_read(uint8_t addr, uint8_t *data, uint8_t length)  {
  spi_hold_low();
  /* Send the command byte */
  uint8_t __attribute__ ((unused)) tmp1 = spi_write_byte(TRX_CMD_SR);
  /* Send the command byte */
  uint8_t __attribute__ ((unused)) tmp2 = spi_write_byte(addr);
  PRINTF("RF233: SRAM read");
  PRINTF("0x%x 0x%x, ", tmp1, tmp2);
  /* Send the address from which the read operation should start */
  /* Upload the received byte in the user provided location */
  for (uint8_t i = 0; i < length; i++) {
    PRINTF("%02x ", data[i]);
    data[i] = spi_write_byte(0);
  }
  spi_release_low();
  PRINTF("\n");

}

void trx_frame_read(uint8_t *data, uint8_t length)  {
  spi_hold_low();
  spi_write_byte(TRX_CMD_FR);
  for (uint8_t i = 0; i < length; i++) {
    data[i] = spi_write_byte(0);
  }
  spi_release_low();
}

void trx_frame_write(uint8_t *data, uint8_t length) {
  spi_hold_low();
  spi_write_byte(TRX_CMD_FW);
  for (uint8_t i = 0; i < length; i++) {
    spi_write_byte(data[i]);
  }
  spi_release_low();
}


/*---------------------------------------------------------------------------*/
/**
 * \brief      Get radio channel
 * \return     The radio channel
 */
int rf_get_channel(void) {
  uint8_t channel;
  channel = trx_reg_read(RF233_REG_PHY_CC_CCA) & PHY_CC_CCA_CHANNEL;
  // printf("rf233 channel%d\n",channel);
  return (int)channel;
}
/*---------------------------------------------------------------------------*/
/**
 * \brief      Set radio channel
 * \param ch   The radio channel
 * \retval -1  Fail: channel number out of bounds
 * \retval 0   Success
 */
int rf_set_channel(uint8_t ch) {
  uint8_t temp;
  PRINTF("RF233: setting channel %u\n", ch);
  if (ch > 26 || ch < 11) {
    return -1;
  }

  /* read-modify-write to conserve other settings */
  temp  = trx_reg_read(RF233_REG_PHY_CC_CCA);
  temp &= ~PHY_CC_CCA_CHANNEL;
  temp |= ch;
  trx_reg_write(RF233_REG_PHY_CC_CCA, temp);
  return 0;
}
/*---------------------------------------------------------------------------*/
/**
 * \brief      Get transmission power
 * \return     The transmission power
 */
int rf233_get_txp(void) {
  PRINTF("RF233: get txp\n");
  return trx_reg_read(RF233_REG_PHY_TX_PWR_CONF) & PHY_TX_PWR_TXP;
}
/*---------------------------------------------------------------------------*/
/**
 * \brief      Set transmission power
 * \param txp  The transmission power
 * \retval -1  Fail: transmission power out of bounds
 * \retval 0   Success
 */
int rf233_set_txp(uint8_t txp) {
  PRINTF("RF233: setting txp %u\n", txp);
  if (txp > TXP_M17) {
    /* undefined */
    return -1;
  }

  trx_reg_write(RF233_REG_PHY_TX_PWR_CONF, txp);
  return 0;
}

static bool radio_pll;
static bool radio_tx;
static bool radio_rx;

static void interrupt_callback(int   _a __attribute__((unused)),
                               int   _b __attribute__((unused)),
                               int   _c __attribute__((unused)),
                               void* _ud __attribute__((unused))) {
  volatile uint8_t irq_source;
  PRINTF("RF233: interrupt handler.\n");
  /* handle IRQ source (for what IRQs are enabled, see rf233-config.h) */
  irq_source = trx_reg_read(RF233_REG_IRQ_STATUS);
  PRINTF("  interrupt sources: 0x%x\n", (int)irq_source);
  if (irq_source & IRQ_PLL_LOCK) {
    PRINTF("RF233: PLL locked.\n");
    radio_pll = true;
  }

  if (irq_source & IRQ_RX_START) {
    PRINTF("RF233: Interrupt receive start.\n");
  } else if (irq_source & IRQ_TRX_DONE) {
    PRINTF("RF233: TRX_DONE handler.\n");
    // Completed a transmission
    if (flag_transmit != 0) {
      PRINTF("RF233: Interrupt transmit.\n");
      flag_transmit = 0;
      if (!(trx_reg_read(RF233_REG_TRX_STATE) & TRX_STATE_TRAC_STATUS)) {
        flag_transmit = ack_status = 1;
      }
      RF233_COMMAND(TRXCMD_RX_ON);
      PRINTF("RF233: TX complete, go back to RX with acks on.\n");
      radio_tx = true;
      return;
    } else {
      // PRINTF("RF233: Interrupt receive.\n");
      packetbuf_clear();
      pending_frame = 1;
      int len = rf233_read(packetbuf_dataptr(), MAX_PACKET_LEN);
      if (len > 0) {
        PRINTF("RF233: Received packet and read from device.\n\n");
      } else {
        PRINTF("RF233: Read failed.\n\n");
      }
      radio_rx = true;
      return;
    }
  }
}

/*---------------------------------------------------------------------------*/
/**
 * \brief      Initialize the radio library
 * \return     Returns success/fail
 * \retval 0   Success
 */
int rf233_init(uint16_t channel, uint16_t from_addr, uint16_t pan_id) {
  PRINTF("RF233: init.\n");
  // 0x61 0xAA
  radio_header.dest = channel; // TODO ??
  radio_header.fcf  = 0xAA61; // TODO verify
  radio_header.src  = from_addr;
  radio_header.pan  = pan_id;
  spi_set_chip_select(1);
  spi_set_rate(1000000);
  spi_set_phase(0);
  spi_set_polarity(0);
  return rf233_setup();
}

/*---------------------------------------------------------------------------*/
/**
 * \brief      Sets callback function for radio receive
 * \return     Returns success/fail
 * \retval 0   Success
 */
int rf233_rx_data(int (*callback)(void*, int, uint16_t, uint16_t, uint16_t)) {
  rx_callback  = callback;
  set_callback = 1;
  return 0;
}

/*---------------------------------------------------------------------------*/
/**
 * \brief      Send data over radio
 * \return     Returns success/fail
 * \retval 0   Success
 */
int rf233_tx_data(uint16_t to_addr, void* payload, int payload_len) {
  PRINTF("RF233: send %u\n", payload_len);
  radio_header.dest = to_addr;
  if (rf233_prepare_without_header(payload, payload_len) != RADIO_TX_OK) {
    return RADIO_TX_ERR;
  }
  return rf233_transmit();
}

/*---------------------------------------------------------------------------*/
/**
 * \brief      Setup and turn on the radio
 * \return     Returns success/fail
 * \retval 0   Success
 */
int rf233_setup(void) {
  volatile uint8_t regtemp;
  volatile uint8_t radio_state;  /* don't optimize this away, it's important */

  /* init SPI and GPIOs, wake up from sleep/power up. */
  spi_init();
  // RF233 expects line low for CS, this is default SAM4L behavior
  // POL = 0 means idle is low
  spi_set_chip_select(3);
  spi_set_rate(400000);
  spi_set_polarity(0);
  // PHASE = 0 means sample leading edge
  spi_set_phase(0);

  /* reset will put us into TRX_OFF state */
  /* reset the radio core */
  gpio_enable_output(RADIO_RST);
  gpio_enable_output(RADIO_SLP);
  gpio_clear(RADIO_RST);
  delay_ms(1);
  gpio_set(RADIO_RST);
  gpio_clear(RADIO_SLP); /* be awake from sleep*/

  /* Read the PART_NUM register to verify that the radio is
   * working/responding. Could check in software, I just look at
   * the bus. If this is working, the first write should be 0x9C x00
   * and the return bytes should be 0x00 0x0B. - pal*/
  PRINTF("Reading part num...\n");
  regtemp = trx_reg_read(RF233_REG_PART_NUM);
  PRINTF("RegTemp Is: %u\n", regtemp); // on the wire thing right, but in
                                       // something is off by one.

  /* before enabling interrupts, make sure we have cleared IRQ status */
  regtemp = trx_reg_read(RF233_REG_IRQ_STATUS);
  PRINTF("RF233: After wake from sleep\n");
  radio_state = rf233_status();
  PRINTF("RF233: Radio state 0x%04x\n", radio_state);
  // calibrate_filters();
  if (radio_state == STATE_P_ON) {
    trx_reg_write(RF233_REG_TRX_STATE, TRXCMD_TRX_OFF);
  }
  /* Assign regtemp to regtemp to avoid compiler warnings */
  regtemp = regtemp;
  // Set up interrupts
  gpio_interrupt_callback(interrupt_callback, NULL);
  gpio_enable_input(RADIO_IRQ, PullNone);
  gpio_clear(RADIO_IRQ);
  gpio_enable_input(RADIO_IRQ, PullNone);
  gpio_enable_interrupt(RADIO_IRQ, RisingEdge);

  /* Configure the radio using the default values except these. */
  trx_reg_write(RF233_REG_TRX_CTRL_1,      RF233_REG_TRX_CTRL_1_CONF);
  trx_reg_write(RF233_REG_PHY_CC_CCA,      RF233_REG_PHY_CC_CCA_CONF);
  trx_reg_write(RF233_REG_PHY_TX_PWR, RF233_REG_PHY_TX_PWR_CONF);
  trx_reg_write(RF233_REG_TRX_CTRL_2,      RF233_REG_TRX_CTRL_2_CONF);
  trx_reg_write(RF233_REG_IRQ_MASK,        RF233_REG_IRQ_MASK_CONF);
  trx_reg_write(RF233_REG_XAH_CTRL_1,      0x02);
  trx_bit_write(SR_MAX_FRAME_RETRIES, 0);
  trx_bit_write(SR_MAX_CSMA_RETRIES, 0);
  PRINTF("RF233: Configured transciever.\n");
  {
    PRINTF("Configuring Addresses...\n");
    uint8_t addr[8];
    addr[0] = 0x22;
    addr[1] = 0x22;
    addr[2] = 0x22;
    addr[3] = 0x22;
    addr[4] = 0x22;
    addr[5] = 0x22;
    addr[6] = 0x22;
    addr[7] = 0x22;
    SetPanId(IEEE802154_CONF_PANID);

    SetIEEEAddr(addr); // I think it breaks here
    SetShortAddr(0x2222);

  }
  rf_generate_random_seed();

  for (uint8_t i = 0; i < 8; i++) {
    regtemp = trx_reg_read(0x24 + i);
  }

  /* 11_09_rel */
  trx_reg_write(RF233_REG_TRX_RPC, 0xFF); /* Enable RPC feature by default */
  PRINTF("RF233: Installed addresses. Turning on radio.\n");
  rf233_on();
  return 0;
}

/*
 * \brief Generates a 16-bit random number used as initial seed for srand()
 *
 */
static void rf_generate_random_seed(void) {
  srand(55);
}

/*---------------------------------------------------------------------------*/
// Append header with FCF, sequence number,
static int rf233_prepare_without_header(const uint8_t *data, unsigned short data_len) {
  if ((data_len + HEADER_SIZE) > MAX_PACKET_LEN) {
    PRINTF("RF233: error, data too large (%u) (prepare_without_header)\n", data_len);
    return RADIO_TX_ERR;
  }

  // append mac header with length 9
  uint8_t data_with_header[MAX_PACKET_LEN];
  data_with_header[0] = 0x61;
  data_with_header[1] = 0xAA;
  data_with_header[2] = radio_header.seq & 0xFF;
  // note: sequence number is incremented in rf233_prepare
  *((uint16_t *)(data_with_header + 3)) = radio_header.pan;
  *((uint16_t *)(data_with_header + 5)) = radio_header.dest;
  *((uint16_t *)(data_with_header + 7)) = radio_header.src;

  int i;
  // Copy over data into new buffer with header
  for (i = 0; i < data_len; i++) {
    data_with_header[i + HEADER_SIZE] = ((uint8_t*)data)[i];
  }

  for (i = 0; i < data_len + HEADER_SIZE; i++) {
    PRINTF("   data[%i] = %x\n", i, (uint8_t) data_with_header[i]);
  }
  // first 9 bytes are now MAC header
  return rf233_prepare(data_with_header, data_len + HEADER_SIZE);
}

/*---------------------------------------------------------------------------*/
/**
 * \brief      prepare a frame and the radio for immediate transmission
 * \param payload         Pointer to data to copy/send
 * \param payload_len     length of data to copy
 * \return     Returns success/fail, refer to radio.h for explanation
 */
int rf233_prepare(const void *payload, unsigned short payload_len) {
  int i;
  // Frame length is number of bytes in MPDU
  uint8_t templen;
  uint8_t radio_status;
  uint8_t data[1 + MAX_PACKET_LEN + 2]; // Length + Payload + FCS

  PRINTF("RF233: prepare %u\n", payload_len);
  if (payload_len > MAX_PACKET_LEN) {
    PRINTF("RF233: error, frame too large to tx\n");
    return RADIO_TX_ERR;
  }

  /* Add length of the FCS (2 bytes) */
  templen = payload_len + 2;
  data[0] = templen;
  for (i = 0; i < templen; i++) {
    data[i + 1] = ((uint8_t*)payload)[i];
  }
  // TODO verify this is unnecessary bc of append_header?
  data[3] = (uint8_t)(radio_header.seq & 0xff);
  radio_header.seq++;

#if DEBUG_PRINTDATA
  PRINTF("RF233 prepare (%u/%u): 0x", payload_len, templen);
  for (i = 0; i < templen; i++) {
    PRINTF("%02x", *(uint8_t *)(payload + i));
  }
  PRINTF("\n");
#endif  /* DEBUG_PRINTDATA */

  /* check that the FIFO is clear to access */
  radio_status = rf233_status();
  if (radio_status == STATE_BUSY_RX_AACK ||
      radio_status == STATE_BUSY_RX ||
      radio_status == STATE_BUSY_TX_ARET) {
    PRINTF("RF233: TRX buffer unavailable: prep when state %s\n", state_str(radio_status));
    return RADIO_TX_ERR;
  }

  /* Write packet to TX FIFO. */
  PRINTF("RF233: sqno: %02x len = %u\n", radio_header.seq, payload_len);
  trx_frame_write((uint8_t *)data, templen + 1);
  return RADIO_TX_OK;
}
/*---------------------------------------------------------------------------*/
/**
 * \brief      Transmit a frame already put in the radio with 'prepare'
 * \param payload_len    Length of the frame to send
 * \return     Returns success/fail, refer to radio.h for explanation
 */
static int rf233_transmit(void) {
  static uint8_t status_now;

  status_now = rf233_status();
  PRINTF("RF233: attempting transmit, in state %s\n", state_str(status_now));

  if (status_now == STATE_BUSY_RX_AACK ||
      status_now == STATE_BUSY_TX_ARET) {
    PRINTF("RF233: collision, was in state %s\n", state_str(status_now));
    /* NOTE: to avoid loops */
    return RADIO_TX_ERR;
    ;
  }

  if (status_now != STATE_PLL_ON) {
    trx_reg_write(RF233_REG_TRX_STATE, STATE_PLL_ON);
    do { // I think this code is broken, does nothing -pal
      status_now = trx_bit_read(RF233_REG_TRX_STATUS, 0x1F, 0);
    } while (status_now == 0x1f);
  }

  if (rf233_status() != STATE_PLL_ON) {
    /* failed moving into PLL_ON state, gracefully try to recover */
    PRINTF("RF233: failed going to STATE_PLL_ON\n");
    RF233_COMMAND(TRXCMD_PLL_ON);   /* try again */
    static uint8_t state;
    state = rf233_status();
    if (state != STATE_PLL_ON) {
      PRINTF("RF233: graceful recovery (in tx) failed, giving up. State: 0x%02X\n", rf233_status());
      return RADIO_TX_ERR;
    }
  }

  /* perform transmission */
  flag_transmit = 1;
  radio_tx      = false;
  RF233_COMMAND(TRXCMD_TX_ARET_ON);
  RF233_COMMAND(TRXCMD_TX_START);

  PRINTF("RF233:: Issued TX_START, wait for completion interrupt.\n");
  yield_for(&radio_tx);
  PRINTF("RF233: tx ok\n\n");

  return RADIO_TX_OK;
}

/*---------------------------------------------------------------------------*/
/**
 * \brief      read a received frame out of the radio buffer
 * \param buf         pointer to where to copy received data
 * \param bufsize     Maximum size we can copy into bufsize
 * \return     Returns length of data read (> 0) if successful
 * \retval -1  Failed, was transmitting so FIFO is invalid
 * \retval -2  Failed, rx timed out (stuck in rx?)
 * \retval -3  Failed, too large frame for buffer
 * \retval -4  Failed, CRC/FCS failed (if USE_HW_FCS_CHECK is true)
 */
int rf233_read(void *buf, unsigned short bufsize) {
  // uint8_t radio_state;
  // uint8_t ed;       /* frame metadata */
  uint8_t frame_len = 0;
  uint8_t len       = 0;

  PRINTF("RF233: Receiving.\n");

  if (pending_frame == 0) {
    PRINTF("RF233: No frame pending, abort.\n");
    return 0;
  }
  pending_frame = 0;

  /* get length of data in FIFO */
  spi_hold_low();
  spi_write_byte(TRX_CMD_FR);
  frame_len = spi_write_byte(0);

  if (frame_len <= 2 ||
      (frame_len - 2) > bufsize) {
    spi_release_low();
    flush_buffer();
    PRINTF("Frame (is not long enough or too large for buffer, abort.\n");
    return 0;
  }

  len = frame_len - 2;

  if (len > bufsize) {
    spi_release_low();
    /* too large frame for the buffer, drop */
    PRINTF("RF233: too large frame for buffer, dropping (%u > %u).\n", frame_len, bufsize);

    return 0;
  }

  /* read out the data into the buffer, disregarding the length and metadata bytes */
  // spi_read_write_sync(wbuf, (char*)buf, len - 1);
  // TODO len or len - 1
  for (uint8_t i = 0; i < len; i++) {
    uint8_t val = spi_write_byte(0);
    ((uint8_t*)buf)[i] = val;
    PRINTF("%02x ", ((uint8_t*)buf)[i]);
    if (i % 10 == 9) {
      PRINTF("\n");
    }
  }

  PRINTF("\n");
  spi_release_low();

  // trx_sram_read(1, (uint8_t *)buf, len);
  if (len >= 10) {
    header_t* header = (header_t*)buf;
    PRINTF("  FCF: %x\n", header->fcf);
    PRINTF("  SEQ: %x\n", header->seq);
    PRINTF("  PAN: %x\n", header->pan);
    PRINTF("  DST: %x\n", header->dest);
    PRINTF("  SRC: %x\n", header->src);

    // skip first 9 bytes of header
    for (int i = 0; i < PACKETBUF_SIZE; i++) {
      recv_data[i] = 0;
    }
    for (int i = 0; i < len; i++) {
      recv_data[i] = header->payload[i];
    }
    // Call user callback function
    if (set_callback) {
      rx_callback(recv_data, len, header->src, header->dest, header->pan);
    }
  }
  // PRINTF("RF233: Final state = %s = %i\n", state_str(rf233_status()), rf233_status());

  flush_buffer();

  rf233_off();
  delay_ms(100);
  rf233_sleep();
  delay_ms(100);
  rf233_on();
  delay_ms(100);

  return len;
}

/*---------------------------------------------------------------------------*/
/**
 * \brief      check whether we are currently receiving a frame
 * \retval >0  we are currently receiving a frame
 * \retval 0   we are not currently receiving a frame
 */
int rf233_receiving_packet(void) {
  uint8_t trx_state;
  trx_state = rf233_status();
  if (trx_state == STATE_BUSY_RX_AACK) {
    PRINTF("RF233: Receiving frame\n");
    return 1;
  }
  PRINTF("RF233: not Receiving frame\n");
  return 0;
}
/*---------------------------------------------------------------------------*/
/**
 * \brief      check whether we have a frame awaiting processing
 * \retval >0  we have a frame awaiting processing
 * \retval 0   we have not a frame awaiting processing
 */
int rf233_pending_packet(void) {
  PRINTF("RF233: Frame %spending\n", pending_frame ? "" : "not ");
  return pending_frame;
}
/*---------------------------------------------------------------------------*/
/**
 * \brief      switch the radio on to listen (rx) mode
 * \retval 0   Success
 */
int rf233_on(void) {
  PRINTF(
    "RF233: turning on from state %s\n  - sleeping radio will be POWER_ON since it doesn't respond to SPI and 0x0 is POWER_ON",
    state_str(rf233_status()));
  on();
  return 0;
}
/*---------------------------------------------------------------------------*/
/**
 * \brief      switch the radio off
 * \retval 0   Success
 */
int rf233_off(void) {
  PRINTF("RF233: turning off from state %s\n", state_str(rf233_status()));
  off();
  return 0;
}

void SetIEEEAddr(uint8_t *ieee_addr) {
  uint8_t *ptr_to_reg = ieee_addr;
  // for (uint8_t i = 0; i < 8; i++) {
  trx_reg_write((0x2b), *ptr_to_reg);
  ptr_to_reg++;
  trx_reg_write((0x2a), *ptr_to_reg);
  ptr_to_reg++;
  trx_reg_write((0x29), *ptr_to_reg);
  ptr_to_reg++;
  trx_reg_write((0x28), *ptr_to_reg);
  ptr_to_reg++;
  trx_reg_write((0x27), *ptr_to_reg);
  ptr_to_reg++;
  trx_reg_write((0x26), *ptr_to_reg);
  ptr_to_reg++;
  trx_reg_write((0x25), *ptr_to_reg);
  ptr_to_reg++;
  trx_reg_write((0x24), *ptr_to_reg);
  ptr_to_reg++;
  // }
}

void SetPanId(uint16_t panId) {
  uint8_t *d = (uint8_t *)&panId;

  trx_reg_write(0x22, d[0]);
  trx_reg_write(0x23, d[1]);
}

void SetShortAddr(uint16_t addr) {
  uint8_t *d = (uint8_t *)&addr;

  trx_reg_write(0x20, d[0]);
  trx_reg_write(0x21, d[1]);
  trx_reg_write(0x2d, d[0] + d[1]);
}

/*---------------------------------------------------------------------------*/
/* switch the radio on */
int on(void) {
  /* Check whether radio is in sleep */
  if (sleep_on) {
    /* Wake the radio. It'll move to TRX_OFF state */
    gpio_clear(RADIO_SLP);
    delay_ms(1);
    PRINTF("RF233: Wake from sleep\n");
    sleep_on = 0;
  }
  uint8_t state_now = rf233_status();
  if (state_now != STATE_PLL_ON &&
      state_now != STATE_TRX_OFF &&
      state_now != STATE_TX_ARET_ON) {
    PRINTF("RF233: Failed to turn radio on, state is %s.\n", state_str(state_now));
    return -1;
  }

  PRINTF("RF233: State is %s, transitioning to PLL_ON.\n", state_str(rf233_status()));
  radio_pll = false;
  RF233_COMMAND(TRXCMD_PLL_ON);
  yield_for(&radio_pll);
  delay_ms(1);
  PRINTF("RF233: State is %s, transitioning to RX_ON.\n", state_str(rf233_status()));
  /* go to RX_ON state */
  RF233_COMMAND(TRXCMD_RX_ON);
  radio_is_on = 1;
  PRINTF("RF233: Radio is on, state is %s.\n", state_str(rf233_status()));
  return 0;
}
/*---------------------------------------------------------------------------*/
/* switch the radio off */
int off(void) {
  if (rf233_status() != STATE_RX_ON ) {
    /* fail, we need the radio transceiver to be in this state */
    return -1;
  }

  /* turn off the radio transceiver */
  RF233_COMMAND(TRXCMD_TRX_OFF);
  radio_is_on = 0;
  return 0;
}
/*---------------------------------------------------------------------------*/
/* Put the Radio in sleep mode */

int rf233_sleep(void) {
  /* Check whether we're already sleeping */
  if (!sleep_on) {
    PRINTF("RF233: Putting to sleep.\n");
    // delay_ms(1);
    sleep_on = 1;
    /* Turn off the Radio */
    rf233_off();
    /* Set the SLP_PIN to high */
    gpio_set(RADIO_SLP);
  }

  return 0;
}

/*---------------------------------------------------------------------------*/
/*
 * Crude way of flushing the Tx/Rx FIFO: write the first byte as 0, indicating
 * a zero-length frame in the buffer. This is interpreted by the driver as an
 * empty buffer.
 */
static void flush_buffer(void) {
  /* NB: tentative untested implementation */
  uint8_t temp = 0;
  trx_frame_write(&temp, 1);
}

void goto_sleep(void) {}

void wake_from_sleep(void) {
  /*
   * Triggers a radio state transition - assumes that the radio already is in
   * state SLEEP or DEEP_SLEEP and SLP pin is low. Refer to datasheet 6.6.
   *
   * Note: this is the only thing that can get the radio from state SLEEP or
   * state DEEP_SLEEP!
   */
}

uint8_t rf233_status(void) {
  return trx_reg_read(RF233_REG_TRX_STATUS) & TRX_STATUS;
}
/*---------------------------------------------------------------------------*/

/*
 * Copyright (c) 2013, Thingsquare, http://www.thingsquare.com/.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * Copyright (c) 2015 Atmel Corporation and
 * 2012 - 2013, Thingsquare, http://www.thingsquare.com/. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Atmel nor the name of Thingsquare nor the names of its
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 * Atmel microcontroller or Atmel wireless product.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
