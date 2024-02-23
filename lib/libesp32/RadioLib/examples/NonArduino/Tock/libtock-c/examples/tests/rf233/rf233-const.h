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

#ifndef _RF233_CONST_H_
#define _RF233_CONST_H_

/* AT86RF233 register addresses and some bit fields */
#define RF233_REG_TRX_STATUS      (0x01)
  #define TRX_CCA_DONE            (1 << 7)
  #define TRX_CCA_STATUS          (1 << 6)
  #define TRX_STATUS              (0x1F)  /* radio status bit field */
    #define STATE_P_ON              (0x00)
    #define STATE_BUSY_RX           (0x01)
    #define STATE_BUSY_TX           (0x02)    /* 7.2 - 13.8 mA */
    #define STATE_RX_ON             (0x06)    /* 11.8 mA */
    #define STATE_TRX_OFF           (0x08)    /* 300 uA */
    #define STATE_PLL_ON            (0x09)    /* 5.2 mA */
    #define STATE_SLEEP             (0x0F)    /* 0.2 uA */
    #define STATE_PREP_DEEP_SLEEP   (0x10)
    #define STATE_BUSY_RX_AACK      (0x11)
    #define STATE_BUSY_TX_ARET      (0x12)
    #define STATE_RX_AACK_ON        (0x16)
    #define STATE_TX_ARET_ON        (0x19)
    #define STATE_TRANSITION        (0x1F)
	#define RF233_REG_TRX_STATE       (0x02)
	/** Access parameters for sub-register RX_PDT_DIS in register @ref RG_RX_SYN */
	#define SR_RX_PDT_DIS   0x15, 0x80, 7
	/** Constant RX_ENABLE for sub-register @ref SR_RX_PDT_DIS in register RX_SYN */
	#define RX_ENABLE   (0)
	/** Access parameters for sub-register RND_VALUE in register @ref RG_PHY_RSSI */
	#define SR_RND_VALUE   0x06, 0x60, 5	
	/** Access parameters for sub-register MAX_FRAME_RETRIES in register @ref RG_XAH_CTRL_0 */
    #define SR_MAX_FRAME_RETRIES   0x2C, 0xF0, 4
	/** Access parameters for sub-register MAX_CSMA_RETRIES in register @ref RG_XAH_CTRL_0 */
    #define  SR_MAX_CSMA_RETRIES	   0x2C, 0X0E, 1
		
    #define TRX_STATE_TRAC_STATUS     (0xE0)  /* result of transaction in extended mode */
    #define TRAC_SUCCESS              (0 << 5)
    #define TRAC_SUCCESS_DATA_PENDING (1 << 5)
    #define TRAC_SUCCESS_WAIT_FOR_ACK (2 << 5)
    #define TRAC_CHANNEL_ACCESS_FAIL  (3 << 5)
    #define TRAC_NO_ACK               (5 << 5)
    #define TRAC_INVALID              (7 << 5)
  #define TRX_STATE_TRX_COMMAND     (0x1F)
    #define TRXCMD_NOP                (0x00)
    #define TRXCMD_TX_START           (0x02)
    #define TRXCMD_FORCE_TRX_OFF      (0x03)
    #define TRXCMD_FORCE_PLL_ON       (0x04)
    #define TRXCMD_RX_ON              (0x06)
    #define TRXCMD_TRX_OFF            (0x08)
    #define TRXCMD_PLL_ON             (0x09)
    #define TRXCMD_PREP_DEEP_SLEEP    (0x10)
    #define TRXCMD_RX_AACK_ON         (0x16)
    #define TRXCMD_TX_ARET_ON         (0x19)
#define RF233_REG_TRX_CTRL_0      (0x03)
#define RF233_REG_TRX_CTRL_1      (0x04)
  #define TRX_CTRL_1_PA_EXT_EN            (1 << 7)  /* pin DIG3/DIG4 indicates tx/rx mode */
  #define TRX_CTRL_1_DIG34_RXTX_INDICATOR TRX_CTRL_1_PA_EXT_EN    /* synonym */
  #define TRX_CTRL_1_DIG2_TIMESTAMPING    (1 << 6)  /* pin DIG2 goes high ca 200us after SFD finished */
  #define TRX_CTRL_1_AUTO_CRC             (1 << 5)
  #define TRX_CTRL_1_FRAME_BUF_EMPTY_EN   (1 << 4)  /* IRQ pin turns into indicator during frame buffer reads */
  #define TRX_CTRL_1_SPI_CMD_TRX_STATUS   (1 << 2)
  #define TRX_CTRL_1_SPI_CMD_RSSI         (2 << 2)
  #define TRX_CTRL_1_SPI_CMD_IRQ_STATUS   (3 << 2)
  #define TRX_CTRL_1_IRQ_MASK_MODE        (1 << 1)  /* irqs shown in IRQ_STATUS even if not in IRQ_MASK */
  #define TRX_CTRL_1_IRQ_POL_ACTIVE_LOW   (1 << 0)
#define RF233_REG_PHY_TX_PWR      (0x05)
  #define PHY_TX_PWR_TXP            (0x0F)
    #define TXP_4         (0x00)
    #define TXP_3P7       (0x01)
    #define TXP_3P4       (0x02)
    #define TXP_3         (0x03)
    #define TXP_2P5       (0x04)
    #define TXP_2         (0x05)
    #define TXP_1         (0x06)
    #define TXP_0         (0x07)
    #define TXP_M1        (0x08)
    #define TXP_M2        (0x09)
    #define TXP_M3        (0x0A)
    #define TXP_M4        (0x0B)
    #define TXP_M6        (0x0C)
    #define TXP_M8        (0x0D)
    #define TXP_M12       (0x0E)
    #define TXP_M17       (0x0F)
#define RF233_REG_PHY_RSSI        (0x06)
  #define PHY_RSSI_CRC_VALID        (1 << 7)  /* FCS/CRC valid if set */
  #define PHY_RSSI_RNG              (0x60)    /* two-bit RNG */
  #define PHY_RSSI_RSSI             (0x1F)    /* RSSI measurement */
#define RF233_REG_PHY_ED_LEVEL    (0x07)
#define RF233_REG_PHY_CC_CCA      (0x08)
  #define PHY_CC_CCA_CHANNEL            (0x1F)
  #define PHY_CC_CCA_DO_CCA             (1 << 7)  /* starts manual CCA */
  #define PHY_CC_CCA_MODE_CS_OR_ED      (0 << 5)
  #define PHY_CC_CCA_MODE_ED            (1 << 5)
  #define PHY_CC_CCA_MODE_CS            (2 << 5)
  #define PHY_CC_CCA_MODE_CS_AND_ED     (3 << 5)
#define RF233_REG_CCA_THRES       (0x09)
#define RF233_REG_RX_CTRL         (0x0A)
#define RF233_REG_SFD_VALUE       (0x0B)
#define RF233_REG_TRX_CTRL_2      (0x0C)
#define FTN_START                 (0x80)
  #define TRX_CTRL_2_RX_SAFE_MODE   (1 << 7)  /* disallow rx buffer overwrites */
  #define DATA_RATE_250             (0)
  #define DATA_RATE_500             (1)
  #define DATA_RATE_1000            (2)
  #define DATA_RATE_2000            (3)
  #define DATA_RATE_2000_SCRAM_EN   (1 << 5) /* enable additional scrambling, mandatory for 2k data rate */
#define RF233_REG_ANT_DIV         (0x0D)
#define RF233_REG_IRQ_MASK        (0x0E)
  #define IRQ_BAT_LOW                     (1 << 7)
  #define IRQ_TRX_UR                      (1 << 6)      /* frame buffer violation */
  #define IRQ_TRXBUF_ACCESS_VIOLATION     IRQ_TRX_UR    /* synonym */
  #define IRQ_AMI                         (1 << 5)      /* address matching on rx */
  #define IRQ_RX_ADDRESS_MATCH            IRQ_AMI       /* synonym */
  #define IRQ_CCA_ED_DONE                 (1 << 4)      /* CCA or ED is done */
  #define IRQ_TRX_END                     (1 << 3)      /* frame tx or rx complete */
  #define IRQ_TRX_DONE                    IRQ_TRX_END   /* synonym */
  #define IRQ_RX_START                    (1 << 2)      /* frame rx started, frame len can be read from fifo[0] */
  #define IRQ_FRAME_RX_START              IRQ_RX_START  /* synonym */
  #define IRQ_PLL_UNLOCK                  (1 << 1)
  #define IRQ_PLL_LOCK                    (1 << 0)
#define RF233_REG_IRQ_STATUS      (0x0F)    /* reading this also clears it */
#define RF233_REG_VREG_CTRL       (0x10)
#define RF233_REG_BATMON          (0x11)
#define RF233_REG_XOSC_CTRL       (0x12)
#define RF233_REG_CC_CTRL_0       (0x13)
#define RF233_REG_CC_CTRL_1       (0x14)
#define RF233_REG_RX_SYN          (0x15)
#define RF233_REG_TRX_RPC         (0x16)
#define RF233_REG_XAH_CTRL_1      (0x17)
  #define AACK_PROM_MODE            (1 << 1)
#define RF233_REG_FTN_CTRL        (0x18)
#define RF233_REG_XAH_CTRL_2      (0x19)
#define RF233_REG_PLL_CF          (0x1A)
  #define PLL_CF_START_CAL          (1 << 7)  /* start manual calibration; reads one until done */
  #define PLL_CF_DO_MANUAL_CAL      (0xD7) /* to do manual calibration, write this value to the reg (includes defaults) */
#define RF233_REG_PLL_DCU         (0x1B)
#define RF233_REG_PART_NUM        (0x1C)
#define RF233_REG_VERSION_NUM     (0x1D)
#define RF233_REG_MAN_ID_0        (0x1E)
#define RF233_REG_MAN_ID_1        (0x1F)
#define RF233_REG_SHORT_ADDR_0    (0x20)
#define RF233_REG_SHORT_ADDR_1    (0x21)
#define RF233_REG_PAN_ID_0        (0x22)
#define RF233_REG_PAN_ID_1        (0x23)
#define RF233_REG_IEEE_ADDR_0     (0x24)
#define RF233_REG_IEEE_ADDR_1     (0x25)
#define RF233_REG_IEEE_ADDR_2     (0x26)
#define RF233_REG_IEEE_ADDR_3     (0x27)
#define RF233_REG_IEEE_ADDR_4     (0x28)
#define RF233_REG_IEEE_ADDR_5     (0x29)
#define RF233_REG_IEEE_ADDR_6     (0x2A)
#define RF233_REG_IEEE_ADDR_7     (0x2B)
#define RF233_REG_XAH_CTRL_0      (0x2C)
#define RF233_REG_CSMA_SEED_0     (0x2D)
#define RF233_REG_CSMA_SEED_1     (0x2E)
#define RF233_REG_CSMA_BE         (0x2F)
#define RF233_REG_TST_CTRL_DIGI   (0x36)
#define RF233_REG_TST_AGC         (0x3C)
#define RF233_REG_TST_SDM         (0x3D)
#define RF233_REG_PHY_TX_TIME     (0x3B)

#define RF233_REG_ADDR_MIN        (0x00)  /* lowest register address */
#define RF233_REG_ADDR_MAX        (0x3D)  /* highest register address */

/* datasheet 6.3, SPI access and first SPI byte */
/* 
 * Following Atmel nomenclature,
 * Register access is single-register-read access
 * Frame Buffer access is access to the shared rx/tx data buffer, always from 0
 * SRAM access is access to the same buffer but from a given offset, and to access the
 *    AES buffer.
 * 
 * This means, registers can only be read/written in single r/w mode.
 * SRAM is the same as Frame buffer access, but can also read AES, and from an offset
 * Frame Buffer reads the trx-fifo, and first byte is the Length byte, then up
 *    to 128 bytes data, followed by 3 bytes metadata (LQI, ED, RX_STATUS).
 * 
 */
#define READ_ACCESS           (0 << 6)
#define WRITE_ACCESS          (1 << 6)

#define REGISTER_ACCESS       (1 << 7)
#define FRAME_BUFFER_ACCESS   (1 << 5)
#define SRAM_ACCESS           (0 << 5)

#define FRAMEBUF_ADDR_LO        (0x00)
#define FRAMEBUF_ADDR_HI        (0x7F)    /* ie the trx-fifo is 128B long */
#define AES_ADDR_LO             (0x82)
#define AES_ADDR_HI             (0x94)

/* bit fields of RX_STATUS (last byte when reading packet buffer) */
#define RX_STATUS_CRC_OK        (1 << 7) /* CRC/FCS */
#define RX_STATUS_TRAC_STATUS   (0x70)

/* 
 * buffer is shared between Rx and Tx; rxdata rx is overwritten by new rx-data
 * or any write access.
 */
#define TRX_BUFFER_LENGTH 128   /* bytes */

#endif  /* _RF233_CONST_H_ */
