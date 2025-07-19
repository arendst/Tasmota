 /*
  stm32flash - Open Source ST STM32 flash program for Arduino
  Copyright 2010 Geoffrey McRae <geoff@spacevs.com>
  Copyright 2012-2014 Tormod Volden <debian.tormod@gmail.com>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <stdint.h>
#include <Arduino.h>

#include "stm32flash.h"
#include "stm32_flash_debug.h"

#include "dev_table.h"

#define STM32_ACK	0x79
#define STM32_NACK	0x1F
#define STM32_BUSY	0x76

#define STM32_CMD_INIT	0x7F
#define STM32_CMD_GET	0x00	/* get the version and command supported */
#define STM32_CMD_GVR	0x01	/* get version and read protection status */
#define STM32_CMD_GID	0x02	/* get ID */
#define STM32_CMD_RM	0x11	/* read memory */
#define STM32_CMD_GO	0x21	/* go */
#define STM32_CMD_WM	0x31	/* write memory */
#define STM32_CMD_WM_NS	0x32	/* no-stretch write memory */
#define STM32_CMD_ER	0x43	/* erase */
#define STM32_CMD_EE	0x44	/* extended erase */
#define STM32_CMD_EE_NS	0x45	/* extended erase no-stretch */
#define STM32_CMD_WP	0x63	/* write protect */
#define STM32_CMD_WP_NS	0x64	/* write protect no-stretch */
#define STM32_CMD_UW	0x73	/* write unprotect */
#define STM32_CMD_UW_NS	0x74	/* write unprotect no-stretch */
#define STM32_CMD_RP	0x82	/* readout protect */
#define STM32_CMD_RP_NS	0x83	/* readout protect no-stretch */
#define STM32_CMD_UR	0x92	/* readout unprotect */
#define STM32_CMD_UR_NS	0x93	/* readout unprotect no-stretch */
#define STM32_CMD_CRC	0xA1	/* compute CRC */
#define STM32_CMD_ERR	0xFF	/* not a valid command */

#define STM32_RESYNC_TIMEOUT	35 * 1000	/* milliseconds */
#define STM32_MASSERASE_TIMEOUT	35 * 1000	/* milliseconds */
#define STM32_PAGEERASE_TIMEOUT	5	* 1000 /* milliseconds */
#define STM32_BLKWRITE_TIMEOUT	1	* 1000 /* milliseconds */
#define STM32_WUNPROT_TIMEOUT	1	* 1000 /* milliseconds */
#define STM32_WPROT_TIMEOUT	1	* 1000 /* milliseconds */
#define STM32_RPROT_TIMEOUT	1	* 1000 /* milliseconds */

#define STM32_CMD_GET_LENGTH	17	/* bytes in the reply */

struct stm32_cmd {
	uint8_t get;
	uint8_t gvr;
	uint8_t gid;
	uint8_t rm;
	uint8_t go;
	uint8_t wm;
	uint8_t er; /* this may be extended erase */
	uint8_t wp;
	uint8_t uw;
	uint8_t rp;
	uint8_t ur;
	uint8_t	crc;
};

/* Reset code for ARMv7-M (Cortex-M3) and ARMv6-M (Cortex-M0)
 * see ARMv7-M or ARMv6-M Architecture Reference Manual (table B3-8)
 * or "The definitive guide to the ARM Cortex-M3", section 14.4.
 */
static const uint8_t stm_reset_code[] = {
	0x01, 0x49,		// ldr     r1, [pc, #4] ; (<AIRCR_OFFSET>)
	0x02, 0x4A,		// ldr     r2, [pc, #8] ; (<AIRCR_RESET_VALUE>)
	0x0A, 0x60,		// str     r2, [r1, #0]
	0xfe, 0xe7,		// endless: b endless
	0x0c, 0xed, 0x00, 0xe0,	// .word 0xe000ed0c <AIRCR_OFFSET> = NVIC AIRCR register address
	0x04, 0x00, 0xfa, 0x05	// .word 0x05fa0004 <AIRCR_RESET_VALUE> = VECTKEY | SYSRESETREQ
};

static const uint32_t stm_reset_code_length = sizeof(stm_reset_code);

/* RM0360, Empty check
 * On STM32F070x6 and STM32F030xC devices only, internal empty check flag is
 * implemented to allow easy programming of the virgin devices by the boot loader. This flag is
 * used when BOOT0 pin is defining Main Flash memory as the target boot space. When the
 * flag is set, the device is considered as empty and System memory (boot loader) is selected
 * instead of the Main Flash as a boot space to allow user to program the Flash memory.
 * This flag is updated only during Option bytes loading: it is set when the content of the
 * address 0x08000 0000 is read as 0xFFFF FFFF, otherwise it is cleared. It means a power
 * on or setting of OBL_LAUNCH bit in FLASH_CR register is needed to clear this flag after
 * programming of a virgin device to execute user code after System reset.
 */
static const uint8_t stm_obl_launch_code[] = {
	0x01, 0x49,		// ldr     r1, [pc, #4] ; (<FLASH_CR>)
	0x02, 0x4A,		// ldr     r2, [pc, #8] ; (<OBL_LAUNCH>)
	0x0A, 0x60,		// str     r2, [r1, #0]
	0xfe, 0xe7,		// endless: b endless
	0x10, 0x20, 0x02, 0x40, // address: FLASH_CR = 40022010
	0x00, 0x20, 0x00, 0x00  // value: OBL_LAUNCH = 00002000
};

static const uint32_t stm_obl_launch_code_length = sizeof(stm_obl_launch_code);

static struct varlen_cmd i2c_cmd_get_reply[] = {
	{0x10, 11},
	{0x11, 17},
	{0x12, 18},
	{ /* sentinel */ }
};

extern const stm32_dev_t devices[];

int flash_addr_to_page_ceil(const stm32_t *stm, uint32_t addr)
{
	int page;
	uint32_t *psize;

	if (!(addr >= stm->dev->fl_start && addr <= stm->dev->fl_end))
		return 0;

	page = 0;
	addr -= stm->dev->fl_start;
	psize = stm->dev->fl_ps;

	while (addr >= psize[0]) {
		addr -= psize[0];
		page++;
		if (psize[1])
			psize++;
	}

	return addr ? page + 1 : page;
}

static void stm32_warn_stretching(const char *f)
{
	DEBUG_MSG("Attention !!!");
	DEBUG_MSG("\tThis %s error could be caused by your I2C", f);
	DEBUG_MSG("\tcontroller not accepting \"clock stretching\"");
	DEBUG_MSG("\tas required by bootloader.");
	DEBUG_MSG("\tCheck \"I2C.txt\" in stm32flash source code.");
}

static stm32_err_t stm32_get_ack_timeout(const stm32_t *stm, uint32_t timeout)
{
	Stream *stream = stm->stream;
	uint8_t byte;
	size_t ret;
	uint32_t t0 = 0, t1;

	if (!(stm->flags & STREAM_OPT_RETRY))
		timeout = 0;

	if (timeout) {
	  stream->setTimeout(timeout);
		t0 = millis();
	}

	do {
		ret = stream->readBytes(&byte, 1);
		if (ret == 0 && timeout) {
			t1 = millis();
			if (t1 < t0 + timeout)
				continue;
		}

		if (ret != 1) {
			DEBUG_MSG("Failed to read ACK byte");
			return STM32_ERR_UNKNOWN;
		}

		if (byte == STM32_ACK)
			return STM32_ERR_OK;
		if (byte == STM32_NACK)
			return STM32_ERR_NACK;
		if (byte != STM32_BUSY) {
			DEBUG_MSG("Got byte 0x%02x instead of ACK",
				byte);
			return STM32_ERR_UNKNOWN;
		}
	} while (1);
}

static stm32_err_t stm32_get_ack(const stm32_t *stm)
{
	return stm32_get_ack_timeout(stm, 0);
}

static stm32_err_t stm32_send_command_timeout(const stm32_t *stm,
					      const uint8_t cmd,
					      uint32_t timeout)
{
	Stream *stream = stm->stream;
	stm32_err_t s_err;
	size_t ret;
	uint8_t buf[2];

	buf[0] = cmd;
	buf[1] = cmd ^ 0xFF;
	ret = stream->write(buf, 2);
	if (ret != 2) {
		DEBUG_MSG("Failed to send command");
		return STM32_ERR_UNKNOWN;
	}
	s_err = stm32_get_ack_timeout(stm, timeout);
	if (s_err == STM32_ERR_OK)
		return STM32_ERR_OK;
	if (s_err == STM32_ERR_NACK)
		DEBUG_MSG("Got NACK from device on command 0x%02x", cmd);
	else
		DEBUG_MSG("Unexpected reply from device on command 0x%02x", cmd);
	return STM32_ERR_UNKNOWN;
}

static stm32_err_t stm32_send_command(const stm32_t *stm, const uint8_t cmd)
{
	return stm32_send_command_timeout(stm, cmd, 0);
}

/* if we have lost sync, send a wrong command and expect a NACK */
static stm32_err_t stm32_resync(const stm32_t *stm)
{
	Stream *stream = stm->stream;
	size_t ret;
	uint8_t buf[2], ack;
	uint32_t t0, t1;

	t0 = millis();
	t1 = t0;

	buf[0] = STM32_CMD_ERR;
	buf[1] = STM32_CMD_ERR ^ 0xFF;
	while (t1 < t0 + STM32_RESYNC_TIMEOUT) {
		ret = stream->write(buf, 2);
		if (ret == 0) {
			delay(500000);
			t1 = millis();
			continue;
		}
		ret = stream->readBytes(&ack, 1);
		if (ret == 0) {
			t1 = millis();
			continue;
		}
		if (ack == STM32_NACK)
			return STM32_ERR_OK;
		t1 = millis();
	}
	return STM32_ERR_UNKNOWN;
}

/*
 * some command receive reply frame with variable length, and length is
 * embedded in reply frame itself.
 * We can guess the length, but if we guess wrong the protocol gets out
 * of sync.
 * Use resync for frame oriented interfaces (e.g. I2C) and byte-by-byte
 * read for byte oriented interfaces (e.g. UART).
 *
 * to run safely, data buffer should be allocated for 256+1 bytes
 *
 * len is value of the first byte in the frame.
 */
static stm32_err_t stm32_guess_len_cmd(const stm32_t *stm, uint8_t cmd,
				       uint8_t *data, unsigned int len)
{
	Stream *stream = stm->stream;
	size_t ret;

	if (stm32_send_command(stm, cmd) != STM32_ERR_OK)
		return STM32_ERR_UNKNOWN;
	if (stm->flags & STREAM_OPT_BYTE) {
		/* interface is UART-like */
		ret = stream->readBytes(data, 1);
		if (ret != 1)
			return STM32_ERR_UNKNOWN;
		len = data[0];
		ret = stream->readBytes(data + 1, len + 1);
		if (ret != len + 1)
			return STM32_ERR_UNKNOWN;
		return STM32_ERR_OK;
	}

	ret = stream->readBytes(data, len + 2);
	if (ret == len + 2 && len == data[0])
		return STM32_ERR_OK;
	if (ret == 0 || ret != len + 2) {
		/* restart with only one byte */
		if (stm32_resync(stm) != STM32_ERR_OK)
			return STM32_ERR_UNKNOWN;
		if (stm32_send_command(stm, cmd) != STM32_ERR_OK)
			return STM32_ERR_UNKNOWN;
		ret = stream->readBytes(data, 1);
		if (ret != 1)
			return STM32_ERR_UNKNOWN;
	}

	DEBUG_MSG("Re sync (len = %d)", data[0]);
	if (stm32_resync(stm) != STM32_ERR_OK)
		return STM32_ERR_UNKNOWN;

	len = data[0];
	if (stm32_send_command(stm, cmd) != STM32_ERR_OK)
		return STM32_ERR_UNKNOWN;
	ret = stream->readBytes(data, len + 2);
	if (ret != len + 2)
		return STM32_ERR_UNKNOWN;
	return STM32_ERR_OK;
}

/*
 * Some interface, e.g. UART, requires a specific init sequence to let STM32
 * autodetect the interface speed.
 * The sequence is only required one time after reset.
 * This function sends the init sequence and, in case of timeout, recovers
 * the interface.
 */
static stm32_err_t stm32_send_init_seq(const stm32_t *stm)
{
	Stream *stream = stm->stream;
	size_t ret;
	uint8_t byte, cmd = STM32_CMD_INIT;

	ret = stream->write(&cmd, 1);
	if (ret != 1) {
		DEBUG_MSG("Failed to send init to device");
		return STM32_ERR_UNKNOWN;
	}
	ret = stream->readBytes(&byte, 1);
	if (ret == 1 && byte == STM32_ACK)
		return STM32_ERR_OK;
	if (ret == 1 && byte == STM32_NACK) {
		/* We could get error later, but let's continue, for now. */
		DEBUG_MSG("Warning: the interface was not closed properly.");
		return STM32_ERR_OK;
	}
	if (ret != 1) {
		DEBUG_MSG("Failed to init device.");
		return STM32_ERR_UNKNOWN;
	}

	/*
	 * Check if previous STM32_CMD_INIT was taken as first byte
	 * of a command. Send a new byte, we should get back a NACK.
	 */
	ret = stream->write(&cmd, 1);
	if (ret != 1) {
		DEBUG_MSG("Failed to send init to device");
		return STM32_ERR_UNKNOWN;
	}
	ret = stream->readBytes(&byte, 1);
	if (ret == 1 && byte == STM32_NACK)
		return STM32_ERR_OK;
	DEBUG_MSG("Failed to init device.");
	return STM32_ERR_UNKNOWN;
}

/* find newer command by higher code */
#define newer(prev, a) (((prev) == STM32_CMD_ERR) \
			? (a) \
			: (((prev) > (a)) ? (prev) : (a)))

stm32_t *stm32_init(Stream *stream, const uint8_t flags, const char init)
{
	uint8_t len, val, buf[257];
	stm32_t *stm;
	int i, new_cmds;

	stm      = (stm32_t *) calloc(sizeof(stm32_t), 1);
	stm->cmd = (stm32_cmd_t *) malloc(sizeof(stm32_cmd_t));
	memset(stm->cmd, STM32_CMD_ERR, sizeof(stm32_cmd_t));
	stm->stream = stream;
	stm->flags = flags;
	if (stm->flags & STREAM_OPT_I2C)
    stm->cmd_get_reply = i2c_cmd_get_reply;

	if ((stm->flags & STREAM_OPT_CMD_INIT) && init)
		if (stm32_send_init_seq(stm) != STM32_ERR_OK)
			return NULL;

	/* get the version and read protection status  */
	if (stm32_send_command(stm, STM32_CMD_GVR) != STM32_ERR_OK) {
		stm32_close(stm);
		return NULL;
	}

	/* From AN, only UART bootloader returns 3 bytes */
	len = (stm->flags & STREAM_OPT_GVR_ETX) ? 3 : 1;
	if (stream->readBytes(buf, len) != len)
		return NULL;
	stm->version = buf[0];
	stm->option1 = (stm->flags & STREAM_OPT_GVR_ETX) ? buf[1] : 0;
	stm->option2 = (stm->flags & STREAM_OPT_GVR_ETX) ? buf[2] : 0;
	if (stm32_get_ack(stm) != STM32_ERR_OK) {
		stm32_close(stm);
		return NULL;
	}

	/* get the bootloader information */
	len = STM32_CMD_GET_LENGTH;
	if (stm->cmd_get_reply)
		for (i = 0; stm->cmd_get_reply[i].length; i++)
			if (stm->version == stm->cmd_get_reply[i].version) {
				len = stm->cmd_get_reply[i].length;
				break;
			}
	if (stm32_guess_len_cmd(stm, STM32_CMD_GET, buf, len) != STM32_ERR_OK)
		return NULL;
	len = buf[0] + 1;
	stm->bl_version = buf[1];
	new_cmds = 0;
	for (i = 1; i < len; i++) {
		val = buf[i + 1];
		switch (val) {
		case STM32_CMD_GET:
			stm->cmd->get = val; break;
		case STM32_CMD_GVR:
			stm->cmd->gvr = val; break;
		case STM32_CMD_GID:
			stm->cmd->gid = val; break;
		case STM32_CMD_RM:
			stm->cmd->rm = val; break;
		case STM32_CMD_GO:
			stm->cmd->go = val; break;
		case STM32_CMD_WM:
		case STM32_CMD_WM_NS:
			stm->cmd->wm = newer(stm->cmd->wm, val);
			break;
		case STM32_CMD_ER:
		case STM32_CMD_EE:
		case STM32_CMD_EE_NS:
			stm->cmd->er = newer(stm->cmd->er, val);
			break;
		case STM32_CMD_WP:
		case STM32_CMD_WP_NS:
			stm->cmd->wp = newer(stm->cmd->wp, val);
			break;
		case STM32_CMD_UW:
		case STM32_CMD_UW_NS:
			stm->cmd->uw = newer(stm->cmd->uw, val);
			break;
		case STM32_CMD_RP:
		case STM32_CMD_RP_NS:
			stm->cmd->rp = newer(stm->cmd->rp, val);
			break;
		case STM32_CMD_UR:
		case STM32_CMD_UR_NS:
			stm->cmd->ur = newer(stm->cmd->ur, val);
			break;
		case STM32_CMD_CRC:
			stm->cmd->crc = newer(stm->cmd->crc, val);
			break;
		default:
			if (new_cmds++ == 0)
				DEBUG_MSG("GET returns unknown commands (0x%2x", val);
			else
				DEBUG_MSG(", 0x%2x", val);
		}
	}
	if (new_cmds)
		DEBUG_MSG(")");
	if (stm32_get_ack(stm) != STM32_ERR_OK) {
		stm32_close(stm);
		return NULL;
	}

	if (stm->cmd->get == STM32_CMD_ERR
	    || stm->cmd->gvr == STM32_CMD_ERR
	    || stm->cmd->gid == STM32_CMD_ERR) {
		DEBUG_MSG("Error: bootloader did not returned correct information from GET command");
		return NULL;
	}

	/* get the device ID */
	if (stm32_guess_len_cmd(stm, stm->cmd->gid, buf, 1) != STM32_ERR_OK) {
		stm32_close(stm);
		return NULL;
	}
	len = buf[0] + 1;
	if (len < 2) {
		stm32_close(stm);
		DEBUG_MSG("Only %d bytes sent in the PID, unknown/unsupported device", len);
		return NULL;
	}
	stm->pid = (buf[1] << 8) | buf[2];
	if (len > 2) {
		DEBUG_MSG("This bootloader returns %d extra bytes in PID:", len);
		for (i = 2; i <= len ; i++)
			DEBUG_MSG(" %02x", buf[i]);
	}
	if (stm32_get_ack(stm) != STM32_ERR_OK) {
		stm32_close(stm);
		return NULL;
	}

	stm->dev = devices;
	while (stm->dev->id != 0x00 && stm->dev->id != stm->pid)
		++stm->dev;

	if (!stm->dev->id) {
		DEBUG_MSG("Unknown/unsupported device (Device ID: 0x%03x)", stm->pid);
		stm32_close(stm);
		return NULL;
	}

	return stm;
}

void stm32_close(stm32_t *stm)
{
	if (stm)
		free(stm->cmd);
	free(stm);
}

stm32_err_t stm32_read_memory(const stm32_t *stm, uint32_t address,
			      uint8_t data[], unsigned int len)
{
	Stream *stream = stm->stream;
	uint8_t buf[5];

	if (!len)
		return STM32_ERR_OK;

	if (len > 256) {
		DEBUG_MSG("Error: READ length limit at 256 bytes");
		return STM32_ERR_UNKNOWN;
	}

	if (stm->cmd->rm == STM32_CMD_ERR) {
		DEBUG_MSG("Error: READ command not implemented in bootloader.");
		return STM32_ERR_NO_CMD;
	}

	if (stm32_send_command(stm, stm->cmd->rm) != STM32_ERR_OK)
		return STM32_ERR_UNKNOWN;

	buf[0] = address >> 24;
	buf[1] = (address >> 16) & 0xFF;
	buf[2] = (address >> 8) & 0xFF;
	buf[3] = address & 0xFF;
	buf[4] = buf[0] ^ buf[1] ^ buf[2] ^ buf[3];
	if (stream->write(buf, 5) != 5)
		return STM32_ERR_UNKNOWN;
	if (stm32_get_ack(stm) != STM32_ERR_OK)
		return STM32_ERR_UNKNOWN;

	if (stm32_send_command(stm, len - 1) != STM32_ERR_OK)
		return STM32_ERR_UNKNOWN;

	if (stream->readBytes(data, len) != len)
		return STM32_ERR_UNKNOWN;

	return STM32_ERR_OK;
}

stm32_err_t stm32_write_memory(const stm32_t *stm, uint32_t address,
			       const uint8_t data[], unsigned int len)
{
	Stream *stream = stm->stream;
	uint8_t cs, buf[256 + 2];
	unsigned int i, aligned_len;
	stm32_err_t s_err;

	if (!len)
		return STM32_ERR_OK;

	if (len > 256) {
		DEBUG_MSG("Error: READ length limit at 256 bytes");
		return STM32_ERR_UNKNOWN;
	}

	/* must be 32bit aligned */
	if (address & 0x3) {
		DEBUG_MSG("Error: WRITE address must be 4 byte aligned");
		return STM32_ERR_UNKNOWN;
	}

	if (stm->cmd->wm == STM32_CMD_ERR) {
		DEBUG_MSG("Error: WRITE command not implemented in bootloader.");
		return STM32_ERR_NO_CMD;
	}

	/* send the address and checksum */
	if (stm32_send_command(stm, stm->cmd->wm) != STM32_ERR_OK)
		return STM32_ERR_UNKNOWN;

	buf[0] = address >> 24;
	buf[1] = (address >> 16) & 0xFF;
	buf[2] = (address >> 8) & 0xFF;
	buf[3] = address & 0xFF;
	buf[4] = buf[0] ^ buf[1] ^ buf[2] ^ buf[3];
	if (stream->write(buf, 5) != 5)
		return STM32_ERR_UNKNOWN;
	if (stm32_get_ack(stm) != STM32_ERR_OK)
		return STM32_ERR_UNKNOWN;

	aligned_len = (len + 3) & ~3;
	cs = aligned_len - 1;
	buf[0] = aligned_len - 1;
	for (i = 0; i < len; i++) {
		cs ^= data[i];
		buf[i + 1] = data[i];
	}
	/* padding data */
	for (i = len; i < aligned_len; i++) {
		cs ^= 0xFF;
		buf[i + 1] = 0xFF;
	}
	buf[aligned_len + 1] = cs;
	if (stream->write(buf, aligned_len + 2) != aligned_len + 2)
		return STM32_ERR_UNKNOWN;

	s_err = stm32_get_ack_timeout(stm, STM32_BLKWRITE_TIMEOUT);
	if (s_err != STM32_ERR_OK) {
		if (stm->flags & STREAM_OPT_STRETCH_W
		    && stm->cmd->wm != STM32_CMD_WM_NS)
			stm32_warn_stretching("write");
		return STM32_ERR_UNKNOWN;
	}
	return STM32_ERR_OK;
}

stm32_err_t stm32_wunprot_memory(const stm32_t *stm)
{
	Stream *stream = stm->stream;
	stm32_err_t s_err;

	if (stm->cmd->uw == STM32_CMD_ERR) {
		DEBUG_MSG("Error: WRITE UNPROTECT command not implemented in bootloader.");
		return STM32_ERR_NO_CMD;
	}

	if (stm32_send_command(stm, stm->cmd->uw) != STM32_ERR_OK)
		return STM32_ERR_UNKNOWN;

	s_err = stm32_get_ack_timeout(stm, STM32_WUNPROT_TIMEOUT);
	if (s_err == STM32_ERR_NACK) {
		DEBUG_MSG("Error: Failed to WRITE UNPROTECT");
		return STM32_ERR_UNKNOWN;
	}
	if (s_err != STM32_ERR_OK) {
		if (stm->flags & STREAM_OPT_STRETCH_W
		    && stm->cmd->uw != STM32_CMD_UW_NS)
			stm32_warn_stretching("WRITE UNPROTECT");
		return STM32_ERR_UNKNOWN;
	}
	return STM32_ERR_OK;
}

stm32_err_t stm32_wprot_memory(const stm32_t *stm)
{
	Stream *stream = stm->stream;
	stm32_err_t s_err;

	if (stm->cmd->wp == STM32_CMD_ERR) {
		DEBUG_MSG("Error: WRITE PROTECT command not implemented in bootloader.");
		return STM32_ERR_NO_CMD;
	}

	if (stm32_send_command(stm, stm->cmd->wp) != STM32_ERR_OK)
		return STM32_ERR_UNKNOWN;

	s_err = stm32_get_ack_timeout(stm, STM32_WPROT_TIMEOUT);
	if (s_err == STM32_ERR_NACK) {
		DEBUG_MSG("Error: Failed to WRITE PROTECT");
		return STM32_ERR_UNKNOWN;
	}
	if (s_err != STM32_ERR_OK) {
		if (stm->flags & STREAM_OPT_STRETCH_W
		    && stm->cmd->wp != STM32_CMD_WP_NS)
			stm32_warn_stretching("WRITE PROTECT");
		return STM32_ERR_UNKNOWN;
	}
	return STM32_ERR_OK;
}

stm32_err_t stm32_runprot_memory(const stm32_t *stm)
{
	Stream *stream = stm->stream;
	stm32_err_t s_err;

	if (stm->cmd->ur == STM32_CMD_ERR) {
		DEBUG_MSG("Error: READOUT UNPROTECT command not implemented in bootloader.");
		return STM32_ERR_NO_CMD;
	}

	if (stm32_send_command(stm, stm->cmd->ur) != STM32_ERR_OK)
		return STM32_ERR_UNKNOWN;

	s_err = stm32_get_ack_timeout(stm, STM32_MASSERASE_TIMEOUT);
	if (s_err == STM32_ERR_NACK) {
		DEBUG_MSG("Error: Failed to READOUT UNPROTECT");
		return STM32_ERR_UNKNOWN;
	}
	if (s_err != STM32_ERR_OK) {
		if (stm->flags & STREAM_OPT_STRETCH_W
		    && stm->cmd->ur != STM32_CMD_UR_NS)
			stm32_warn_stretching("READOUT UNPROTECT");
		return STM32_ERR_UNKNOWN;
	}
	return STM32_ERR_OK;
}

stm32_err_t stm32_readprot_memory(const stm32_t *stm)
{
	Stream *stream = stm->stream;
	stm32_err_t s_err;

	if (stm->cmd->rp == STM32_CMD_ERR) {
		DEBUG_MSG("Error: READOUT PROTECT command not implemented in bootloader.");
		return STM32_ERR_NO_CMD;
	}

	if (stm32_send_command(stm, stm->cmd->rp) != STM32_ERR_OK)
		return STM32_ERR_UNKNOWN;

	s_err = stm32_get_ack_timeout(stm, STM32_RPROT_TIMEOUT);
	if (s_err == STM32_ERR_NACK) {
		DEBUG_MSG("Error: Failed to READOUT PROTECT");
		return STM32_ERR_UNKNOWN;
	}
	if (s_err != STM32_ERR_OK) {
		if (stm->flags & STREAM_OPT_STRETCH_W
		    && stm->cmd->rp != STM32_CMD_RP_NS)
			stm32_warn_stretching("READOUT PROTECT");
		return STM32_ERR_UNKNOWN;
	}
	return STM32_ERR_OK;
}

static stm32_err_t stm32_mass_erase(const stm32_t *stm)
{
	Stream *stream = stm->stream;
	stm32_err_t s_err;
	uint8_t buf[3];

	if (stm32_send_command(stm, stm->cmd->er) != STM32_ERR_OK) {
		DEBUG_MSG("Can't initiate chip mass erase!");
		return STM32_ERR_UNKNOWN;
	}

	/* regular erase (0x43) */
	if (stm->cmd->er == STM32_CMD_ER) {
		s_err = stm32_send_command_timeout(stm, 0xFF, STM32_MASSERASE_TIMEOUT);
		if (s_err != STM32_ERR_OK) {
			if (stm->flags & STREAM_OPT_STRETCH_W)
				stm32_warn_stretching("mass erase");
			return STM32_ERR_UNKNOWN;
		}
		return STM32_ERR_OK;
	}

	/* extended erase */
	buf[0] = 0xFF;	/* 0xFFFF the magic number for mass erase */
	buf[1] = 0xFF;
	buf[2] = 0x00;  /* checksum */
	if (stream->write(buf, 3) != 3) {
		DEBUG_MSG("Mass erase error.");
		return STM32_ERR_UNKNOWN;
	}
	s_err = stm32_get_ack_timeout(stm, STM32_MASSERASE_TIMEOUT);
	if (s_err != STM32_ERR_OK) {
		DEBUG_MSG("Mass erase failed. Try specifying the number of pages to be erased.");
	if (stm->flags & STREAM_OPT_STRETCH_W
	    && stm->cmd->er != STM32_CMD_EE_NS)
		stm32_warn_stretching("mass erase");
		return STM32_ERR_UNKNOWN;
	}
	return STM32_ERR_OK;
}

static stm32_err_t stm32_pages_erase(const stm32_t *stm, uint32_t spage, uint32_t pages)
{
	Stream *stream = stm->stream;
	stm32_err_t s_err;
	size_t ret;
	uint32_t pg_num;
	uint8_t pg_byte;
	uint8_t cs = 0;
	uint8_t *buf;
	int i = 0;

	/* The erase command reported by the bootloader is either 0x43, 0x44 or 0x45 */
	/* 0x44 is Extended Erase, a 2 byte based protocol and needs to be handled differently. */
	/* 0x45 is clock no-stretching version of Extended Erase for I2C port. */
	if (stm32_send_command(stm, stm->cmd->er) != STM32_ERR_OK) {
		DEBUG_MSG("Can't initiate chip mass erase!");
		return STM32_ERR_UNKNOWN;
	}

	/* regular erase (0x43) */
	if (stm->cmd->er == STM32_CMD_ER) {
		buf = (uint8_t *) malloc(1 + pages + 1);
		if (!buf)
			return STM32_ERR_UNKNOWN;

		buf[i++] = pages - 1;
		cs ^= (pages-1);
		for (pg_num = spage; pg_num < (pages + spage); pg_num++) {
			buf[i++] = pg_num;
			cs ^= pg_num;
		}
		buf[i++] = cs;
		ret = stream->write(buf, i);
		free(buf);
		if (ret != i) {
			DEBUG_MSG("Erase failed.");
			return STM32_ERR_UNKNOWN;
		}
		s_err = stm32_get_ack_timeout(stm, pages * STM32_PAGEERASE_TIMEOUT);
		if (s_err != STM32_ERR_OK) {
			if (stm->flags & STREAM_OPT_STRETCH_W)
				stm32_warn_stretching("erase");
			return STM32_ERR_UNKNOWN;
		}
		return STM32_ERR_OK;
	}

	/* extended erase */
	buf = (uint8_t *) malloc(2 + 2 * pages + 1);
	if (!buf)
		return STM32_ERR_UNKNOWN;

	/* Number of pages to be erased - 1, two bytes, MSB first */
	pg_byte = (pages - 1) >> 8;
	buf[i++] = pg_byte;
	cs ^= pg_byte;
	pg_byte = (pages - 1) & 0xFF;
	buf[i++] = pg_byte;
	cs ^= pg_byte;

	for (pg_num = spage; pg_num < spage + pages; pg_num++) {
		pg_byte = pg_num >> 8;
		cs ^= pg_byte;
		buf[i++] = pg_byte;
		pg_byte = pg_num & 0xFF;
		cs ^= pg_byte;
		buf[i++] = pg_byte;
	}
	buf[i++] = cs;
	ret = stream->write(buf, i);
	free(buf);
	if (ret != i) {
		DEBUG_MSG("Page-by-page erase error.");
		return STM32_ERR_UNKNOWN;
	}

	s_err = stm32_get_ack_timeout(stm, pages * STM32_PAGEERASE_TIMEOUT);
	if (s_err != STM32_ERR_OK) {
		DEBUG_MSG("Page-by-page erase failed. Check the maximum pages your device supports.");
		if (stm->flags & STREAM_OPT_STRETCH_W
		    && stm->cmd->er != STM32_CMD_EE_NS)
			stm32_warn_stretching("erase");
		return STM32_ERR_UNKNOWN;
	}

	return STM32_ERR_OK;
}

stm32_err_t stm32_erase_memory(const stm32_t *stm, uint32_t spage, uint32_t pages)
{
	uint32_t n;
	stm32_err_t s_err;

	if (!pages || spage > STM32_MAX_PAGES ||
	    ((pages != STM32_MASS_ERASE) && ((spage + pages) > STM32_MAX_PAGES)))
		return STM32_ERR_OK;

	if (stm->cmd->er == STM32_CMD_ERR) {
		DEBUG_MSG("Error: ERASE command not implemented in bootloader.");
		return STM32_ERR_NO_CMD;
	}

	if (pages == STM32_MASS_ERASE) {
		/*
		 * Not all chips support mass erase.
		 * Mass erase can be obtained executing a "readout protect"
		 * followed by "readout un-protect". This method is not
		 * suggested because can hang the target if a debug SWD/JTAG
		 * is connected. When the target enters in "readout
		 * protection" mode it will consider the debug connection as
		 * a tentative of intrusion and will hang.
		 * Erasing the flash page-by-page is the safer way to go.
		 */
		if (!(stm->dev->flags & F_NO_ME))
			return stm32_mass_erase(stm);

		pages = flash_addr_to_page_ceil(stm, stm->dev->fl_end);
	}

	/*
	 * Some device, like STM32L152, cannot erase more than 512 pages in
	 * one command. Split the call.
	 */
	while (pages) {
		n = (pages <= 512) ? pages : 512;
		s_err = stm32_pages_erase(stm, spage, n);
		if (s_err != STM32_ERR_OK)
			return s_err;
		spage += n;
		pages -= n;
	}
	return STM32_ERR_OK;
}

/* detect CPU endian */
char cpu_le() {
	const uint32_t cpu_le_test = 0x12345678;
	return ((const unsigned char*)&cpu_le_test)[0] == 0x78;
}

uint32_t le_u32(const uint32_t v) {
        if (!cpu_le())
                return  ((v & 0xFF000000) >> 24) |
                        ((v & 0x00FF0000) >>  8) |
                        ((v & 0x0000FF00) <<  8) |
                        ((v & 0x000000FF) << 24);
        return v;
}

static stm32_err_t stm32_run_raw_code(const stm32_t *stm,
				      uint32_t target_address,
				      const uint8_t *code, uint32_t code_size)
{
	uint32_t stack_le = le_u32(0x20002000);
	uint32_t code_address_le = le_u32(target_address + 8 + 1); // thumb mode address (!)
	uint32_t length = code_size + 8;
	uint8_t *mem, *pos;
	uint32_t address, w;

	/* Must be 32-bit aligned */
	if (target_address & 0x3) {
		DEBUG_MSG("Error: code address must be 4 byte aligned");
		return STM32_ERR_UNKNOWN;
	}

	mem = (uint8_t *) malloc(length);
	if (!mem)
		return STM32_ERR_UNKNOWN;

	memcpy(mem, &stack_le, sizeof(uint32_t));
	memcpy(mem + 4, &code_address_le, sizeof(uint32_t));
	memcpy(mem + 8, code, code_size);

	pos = mem;
	address = target_address;
	while (length > 0) {
		w = length > 256 ? 256 : length;
		if (stm32_write_memory(stm, address, pos, w) != STM32_ERR_OK) {
			free(mem);
			return STM32_ERR_UNKNOWN;
		}

		address += w;
		pos += w;
		length -= w;
	}

	free(mem);
	return stm32_go(stm, target_address);
}

stm32_err_t stm32_go(const stm32_t *stm, uint32_t address)
{
	Stream *stream = stm->stream;
	uint8_t buf[5];

	if (stm->cmd->go == STM32_CMD_ERR) {
		DEBUG_MSG("Error: GO command not implemented in bootloader.");
		return STM32_ERR_NO_CMD;
	}

	if (stm32_send_command(stm, stm->cmd->go) != STM32_ERR_OK)
		return STM32_ERR_UNKNOWN;

	buf[0] = address >> 24;
	buf[1] = (address >> 16) & 0xFF;
	buf[2] = (address >> 8) & 0xFF;
	buf[3] = address & 0xFF;
	buf[4] = buf[0] ^ buf[1] ^ buf[2] ^ buf[3];
	if (stream->write(buf, 5) != 5)
		return STM32_ERR_UNKNOWN;

	if (stm32_get_ack(stm) != STM32_ERR_OK)
		return STM32_ERR_UNKNOWN;
	return STM32_ERR_OK;
}

stm32_err_t stm32_reset_device(const stm32_t *stm)
{
	uint32_t target_address = stm->dev->ram_start;

	if (stm->dev->flags & F_OBLL) {
		/* set the OBL_LAUNCH bit to reset device (see RM0360, 2.5) */
		return stm32_run_raw_code(stm, target_address, stm_obl_launch_code, stm_obl_launch_code_length);
	} else {
		return stm32_run_raw_code(stm, target_address, stm_reset_code, stm_reset_code_length);
	}
}

stm32_err_t stm32_crc_memory(const stm32_t *stm, uint32_t address,
			     uint32_t length, uint32_t *crc)
{
	Stream *stream = stm->stream;
	uint8_t buf[5];

	if (address & 0x3 || length & 0x3) {
		DEBUG_MSG("Start and end addresses must be 4 byte aligned");
		return STM32_ERR_UNKNOWN;
	}

	if (stm->cmd->crc == STM32_CMD_ERR) {
		DEBUG_MSG("Error: CRC command not implemented in bootloader.");
		return STM32_ERR_NO_CMD;
	}

	if (stm32_send_command(stm, stm->cmd->crc) != STM32_ERR_OK)
		return STM32_ERR_UNKNOWN;

	buf[0] = address >> 24;
	buf[1] = (address >> 16) & 0xFF;
	buf[2] = (address >> 8) & 0xFF;
	buf[3] = address & 0xFF;
	buf[4] = buf[0] ^ buf[1] ^ buf[2] ^ buf[3];
	if (stream->write(buf, 5) != 5)
		return STM32_ERR_UNKNOWN;

	if (stm32_get_ack(stm) != STM32_ERR_OK)
		return STM32_ERR_UNKNOWN;

	buf[0] = length >> 24;
	buf[1] = (length >> 16) & 0xFF;
	buf[2] = (length >> 8) & 0xFF;
	buf[3] = length & 0xFF;
	buf[4] = buf[0] ^ buf[1] ^ buf[2] ^ buf[3];
	if (stream->write(buf, 5) != 5)
		return STM32_ERR_UNKNOWN;

	if (stm32_get_ack(stm) != STM32_ERR_OK)
		return STM32_ERR_UNKNOWN;

	if (stm32_get_ack(stm) != STM32_ERR_OK)
		return STM32_ERR_UNKNOWN;

	if (stream->readBytes(buf, 5) != 5)
		return STM32_ERR_UNKNOWN;

	if (buf[4] != (buf[0] ^ buf[1] ^ buf[2] ^ buf[3]))
		return STM32_ERR_UNKNOWN;

	*crc = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
	return STM32_ERR_OK;
}

/*
 * CRC computed by STM32 is similar to the standard crc32_be()
 * implemented, for example, in Linux kernel in ./lib/crc32.c
 * But STM32 computes it on units of 32 bits word and swaps the
 * bytes of the word before the computation.
 * Due to byte swap, I cannot use any CRC available in existing
 * libraries, so here is a simple not optimized implementation.
 */
#define CRCPOLY_BE	0x04c11db7
#define CRC_MSBMASK	0x80000000
#define CRC_INIT_VALUE	0xFFFFFFFF
uint32_t stm32_sw_crc(uint32_t crc, uint8_t *buf, unsigned int len)
{
	int i;
	uint32_t data;

	if (len & 0x3) {
		DEBUG_MSG("Buffer length must be multiple of 4 bytes");
		return 0;
	}

	while (len) {
		data = *buf++;
		data |= *buf++ << 8;
		data |= *buf++ << 16;
		data |= *buf++ << 24;
		len -= 4;

		crc ^= data;

		for (i = 0; i < 32; i++)
			if (crc & CRC_MSBMASK)
				crc = (crc << 1) ^ CRCPOLY_BE;
			else
				crc = (crc << 1);
	}
	return crc;
}

stm32_err_t stm32_crc_wrapper(const stm32_t *stm, uint32_t address,
			      uint32_t length, uint32_t *crc)
{
	uint8_t buf[256];
	uint32_t start, total_len, len, current_crc;

	if (address & 0x3 || length & 0x3) {
		DEBUG_MSG("Start and end addresses must be 4 byte aligned");
		return STM32_ERR_UNKNOWN;
	}

	if (stm->cmd->crc != STM32_CMD_ERR)
		return stm32_crc_memory(stm, address, length, crc);

	start = address;
	total_len = length;
	current_crc = CRC_INIT_VALUE;
	while (length) {
		len = length > 256 ? 256 : length;
		if (stm32_read_memory(stm, address, buf, len) != STM32_ERR_OK) {
			DEBUG_MSG("Failed to read memory at address 0x%08x, target write-protected?", address);
			return STM32_ERR_UNKNOWN;
		}
		current_crc = stm32_sw_crc(current_crc, buf, len);
		length -= len;
		address += len;

		DEBUG_MSG("\rCRC address 0x%08x (%.2f%%) ",
			address,
			(100.0f / (float)total_len) * (float)(address - start)
		);
	}
	DEBUG_MSG("Done.");
	*crc = current_crc;
	return STM32_ERR_OK;
}
