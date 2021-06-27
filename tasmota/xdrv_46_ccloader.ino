/*
  xdrv_46_ccloader.ino - CCLoader for Tasmota

  Copyright (C) 2021  Christian Baars and Theo Arends

  based on CCLoader - Copyright (c) 2012-2014 RedBearLab

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.


  --------------------------------------------------------------------------------------------
  Version yyyymmdd  Action    Description
  --------------------------------------------------------------------------------------------

  0.9.0.0 20191124  started - further development by Christian Baars
                    forked  - CCLoader - Copyright (c) 2012-2014 RedBearLab

*/
#ifdef USE_CCLOADER

/*********************************************************************************************\
* CCLoader
*
* Usage:
*  - Configure GPIOs like:
*    - DEBUG_DATA on CC25xx device to GPIO Zigbee Rx
*    - DEBUG_CLOCK on CC25xx device to GPIO Zigbee Tx
*    - RESET_N on CC25xx device to GPIO Zigbee Rst
*    - Any GPIO as Option_A4
\*********************************************************************************************/

#define XDRV_46             46

// Start addresses on DUP (Increased buffer size improves performance)
#define CCL_ADDR_BUF0                   0x0000 // Buffer (512 bytes)
#define CCL_ADDR_DMA_DESC_0             0x0200 // DMA descriptors (8 bytes)
#define CCL_ADDR_DMA_DESC_1             (CCL_ADDR_DMA_DESC_0 + 8)

// DMA channels used on DUP
#define CCL_CH_DBG_TO_BUF0              0x01   // Channel 0
#define CCL_CH_BUF0_TO_FLASH            0x02   // Channel 1

// Debug commands
#define CCL_CMD_CHIP_ERASE              0x10
#define CCL_CMD_WR_CONFIG               0x19
#define CCL_CMD_RD_CONFIG               0x24
#define CCL_CMD_READ_STATUS             0x30
#define CCL_CMD_RESUME                  0x4C
#define CCL_CMD_DEBUG_INSTR_1B          (0x54|1)
#define CCL_CMD_DEBUG_INSTR_2B          (0x54|2)
#define CCL_CMD_DEBUG_INSTR_3B          (0x54|3)
#define CCL_CMD_BURST_WRITE             0x80
#define CCL_CMD_GET_CHIP_ID             0x68

// Debug status bitmasks
#define CCL_STATUS_CHIP_ERASE_BUSY_BM   0x80 // New debug interface
#define CCL_STATUS_PCON_IDLE_BM         0x40
#define CCL_STATUS_CPU_HALTED_BM        0x20
#define CCL_STATUS_PM_ACTIVE_BM         0x10
#define CCL_STATUS_HALT_STATUS_BM       0x08
#define CCL_STATUS_DEBUG_LOCKED_BM      0x04
#define CCL_STATUS_OSC_STABLE_BM        0x02
#define CCL_STATUS_STACK_OVERFLOW_BM    0x01

// DUP registers (XDATA space address)
#define CCL_DUP_DBGDATA                 0x6260  // Debug interface data buffer
#define CCL_DUP_FCTL                    0x6270  // Flash controller
#define CCL_DUP_FADDRL                  0x6271  // Flash controller addr
#define CCL_DUP_FADDRH                  0x6272  // Flash controller addr
#define CCL_DUP_FWDATA                  0x6273  // Clash controller data buffer
#define CCL_DUP_CLKCONSTA               0x709E  // Sys clock status
#define CCL_DUP_CLKCONCMD               0x70C6  // Sys clock configuration
#define CCL_DUP_MEMCTR                  0x70C7  // Flash bank xdata mapping
#define CCL_DUP_DMA1CFGL                0x70D2  // Low byte, DMA config ch. 1
#define CCL_DUP_DMA1CFGH                0x70D3  // Hi byte , DMA config ch. 1
#define CCL_DUP_DMA0CFGL                0x70D4  // Low byte, DMA config ch. 0
#define CCL_DUP_DMA0CFGH                0x70D5  // Low byte, DMA config ch. 0
#define CCL_DUP_DMAARM                  0x70D6  // DMA arming register

// Utility macros
//! Low nibble of 16bit variable
#define LOBYTE(w)           ((unsigned char)(w))
//! High nibble of 16bit variable
#define HIBYTE(w)           ((unsigned char)(((unsigned short)(w) >> 8) & 0xFF))

const unsigned char ccl_dma_desc_0[8] =
{
    // Debug Interface -> Buffer
    HIBYTE(CCL_DUP_DBGDATA),        // src[15:8]
    LOBYTE(CCL_DUP_DBGDATA),        // src[7:0]
    HIBYTE(CCL_ADDR_BUF0),          // dest[15:8]
    LOBYTE(CCL_ADDR_BUF0),          // dest[7:0]
    0,                              // len[12:8] - filled in later
    0,                              // len[7:0]
    31,                             // trigger: DBG_BW
    0x11                            // increment destination
};
//! DUP DMA descriptor
const unsigned char ccl_dma_desc_1[8] =
{
    // Buffer -> Flash controller
    HIBYTE(CCL_ADDR_BUF0),          // src[15:8]
    LOBYTE(CCL_ADDR_BUF0),          // src[7:0]
    HIBYTE(CCL_DUP_FWDATA),         // dest[15:8]
    LOBYTE(CCL_DUP_FWDATA),         // dest[7:0]
    0,                              // len[12:8] - filled in later
    0,                              // len[7:0]
    18,                             // trigger: FLASH
    0x42,                           // increment source
};

struct {
    struct {
        uint8_t rev;
        uint8_t ID;
    } chip;
    bool init = false;
} CCL;

const char CCLtype[] PROGMEM = "CCL";

// Debug control pins
int CCL_RESET;  // RESET_N on CC25xx device
int CCL_DD;     // DEBUG_DATA on CC25xx device
int CCL_DC;     // DEBUG_CLOCK on CC25xx device

/********************************************************************************************/
/**************************************************************************//**
* @brief    Writes a byte on the debug interface. Requires DD to be
*           output when function is called.
* @param    data    Byte to write
* @return   None.
******************************************************************************/
#pragma inline
void CCLwrite_debug_byte(unsigned char data)
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        // Set clock high and put data on DD line
        digitalWrite(CCL_DC, HIGH);
        if(data & 0x80)
        {
          digitalWrite(CCL_DD, HIGH);
        }
        else
        {
          digitalWrite(CCL_DD, LOW);
        }
        data <<= 1;
        digitalWrite(CCL_DC, LOW); // set clock low (DUP capture flank)
    }
}

/**************************************************************************//**
* @brief    Reads a byte from the debug interface. Requires DD to be
*           input when function is called.
* @return   Returns the byte read.
******************************************************************************/
#pragma inline
unsigned char CCLread_debug_byte(void)
{
    unsigned char i;
    unsigned char data = 0x00;
    for (i = 0; i < 8; i++)
    {
        digitalWrite(CCL_DC, HIGH);  // DC high
        data <<= 1;
        if(HIGH == digitalRead(CCL_DD))
        {
          data |= 0x01;
        }
        digitalWrite(CCL_DC, LOW);     // DC low
    }
    return data;
}

/**************************************************************************//**
* @brief    Function waits for DUP to indicate that it is ready. The DUP will
*           pulls DD line low when it is ready. Requires DD to be input when
*           function is called.
* @return   Returns 0 if function timed out waiting for DD line to go low
* @return   Returns 1 when DUP has indicated it is ready.
******************************************************************************/
#pragma inline
unsigned char CCLwait_dup_ready(void)
{
    // DUP pulls DD low when ready
    unsigned int count = 0;
    while ((HIGH == digitalRead(CCL_DD)) && count < 16)
    {
        // Clock out 8 bits before checking if DD is low again
        CCLread_debug_byte();
        count++;
    }
    return (count == 16) ? 0 : 1;
}

/**************************************************************************//**
* @brief    Issues a command on the debug interface. Only commands that return
*           one output byte are supported.
* @param    cmd             Command byte
* @param    cmd_bytes       Pointer to the array of data bytes following the
*                           command byte [0-3]
* @param    num_cmd_bytes   The number of data bytes (input to DUP) [0-3]
* @return   Data returned by command
******************************************************************************/
unsigned char CCLdebug_command(unsigned char cmd, unsigned char *cmd_bytes,
                            unsigned short num_cmd_bytes)
{
    unsigned short i;
    unsigned char output = 0;
    // Make sure DD is output
    pinMode(CCL_DD, OUTPUT);
    // Send command
    CCLwrite_debug_byte(cmd);
    // Send bytes
    for (i = 0; i < num_cmd_bytes; i++)
    {
        CCLwrite_debug_byte(cmd_bytes[i]);
    }
    // Set DD as input
    pinMode(CCL_DD, INPUT);
    digitalWrite(CCL_DD, HIGH);
    // Wait for data to be ready
    CCLwait_dup_ready();
    // Read returned byte
    output = CCLread_debug_byte();
    // Set DD as output
    pinMode(CCL_DD, OUTPUT);

    return output;
}

/**************************************************************************//**
* @brief    Resets the DUP into debug mode. Function assumes that
*           the programmer I/O has already been configured using e.g.
*           ProgrammerInit().
* @return   None.
******************************************************************************/
void CCLdebug_init(void)
{
    volatile unsigned char i;

    // Send two flanks on DC while keeping RESET_N low
    // All low (incl. RESET_N)
    digitalWrite(CCL_DD, LOW);
    digitalWrite(CCL_DC, LOW);
    digitalWrite(CCL_RESET, LOW);
    delay(10);   // Wait
    digitalWrite(CCL_DC, HIGH);                    // DC high
    delay(10);   // Wait
    digitalWrite(CCL_DC, LOW);                     // DC low
    delay(10);   // Wait
    digitalWrite(CCL_DC, HIGH);                    // DC high
    delay(10);   // Wait
    digitalWrite(CCL_DC, LOW);                     // DC low
    delay(10);   // Wait
    digitalWrite(CCL_RESET, HIGH);              // Release RESET_N
    delay(10);   // Wait
}

/**************************************************************************//**
* @brief    Reads the chip ID over the debug interface using the
*           GET_CHIP_ID command.
* @return   Returns the chip id returned by the DUP
******************************************************************************/
void CCLread_chip_id(void)
{
    // Make sure DD is output
    pinMode(CCL_DD, OUTPUT);
    delay(1);
    // Send command
    CCLwrite_debug_byte(CCL_CMD_GET_CHIP_ID);
    // Set DD as input
    pinMode(CCL_DD, INPUT);
    digitalWrite(CCL_DD, HIGH);
    delay(1);
    // Wait for data to be ready
    if(CCLwait_dup_ready() == 1)
    {
      // Read ID and revision
      CCL.chip.ID = CCLread_debug_byte();
      CCL.chip.rev = CCLread_debug_byte();
    }
    // Set DD as output
    pinMode(CCL_DD, OUTPUT);

    return;
}


/**************************************************************************//**
* @brief    Sends a block of data over the debug interface using the
*           BURST_WRITE command.
* @param    src         Pointer to the array of input bytes
* @param    num_bytes   The number of input bytes
* @return   None.
******************************************************************************/
void CCLburst_write_block(unsigned char *src, unsigned short num_bytes)
{
    unsigned short i;

    // Make sure DD is output
    pinMode(CCL_DD, OUTPUT);

    CCLwrite_debug_byte(CCL_CMD_BURST_WRITE | HIBYTE(num_bytes));
    CCLwrite_debug_byte(LOBYTE(num_bytes));
    for (i = 0; i < num_bytes; i++)
    {
        CCLwrite_debug_byte(src[i]);
    }

    // Set DD as input
    pinMode(CCL_DD, INPUT);
    digitalWrite(CCL_DD, HIGH);
    // Wait for DUP to be ready
    CCLwait_dup_ready();
    CCLread_debug_byte(); // ignore output
    // Set DD as output
    pinMode(CCL_DD, OUTPUT);
}

/**************************************************************************//**
* @brief    Issues a CHIP_ERASE command on the debug interface and waits for it
*           to complete.
* @return   None.
******************************************************************************/
void CCLchip_erase(void)
{
    volatile unsigned char status;
    // Send command
    CCLdebug_command(CCL_CMD_CHIP_ERASE, 0, 0);

    // Wait for status bit 7 to go low
    do {
        status = CCLdebug_command(CCL_CMD_READ_STATUS, 0, 0);
    } while((status & CCL_STATUS_CHIP_ERASE_BUSY_BM));
}

/**************************************************************************//**
* @brief    Writes a block of data to the DUP's XDATA space.
* @param    address     XDATA start address
* @param    values      Pointer to the array of bytes to write
* @param    num_bytes   Number of bytes to write
* @return   None.
******************************************************************************/
void CCLwrite_xdata_memory_block(unsigned short address,
                              const unsigned char *values,
                              unsigned short num_bytes)
{
    unsigned char instr[3];
    unsigned short i;

    // MOV DPTR, address
    instr[0] = 0x90;
    instr[1] = HIBYTE(address);
    instr[2] = LOBYTE(address);
    CCLdebug_command(CCL_CMD_DEBUG_INSTR_3B, instr, 3);

    for (i = 0; i < num_bytes; i++)
    {
        // MOV A, values[i]
        instr[0] = 0x74;
        instr[1] = values[i];
        CCLdebug_command(CCL_CMD_DEBUG_INSTR_2B, instr, 2);

        // MOV @DPTR, A
        instr[0] = 0xF0;
        CCLdebug_command(CCL_CMD_DEBUG_INSTR_1B, instr, 1);

        // INC DPTR
        instr[0] = 0xA3;
        CCLdebug_command(CCL_CMD_DEBUG_INSTR_1B, instr, 1);
    }
}

/**************************************************************************//**
* @brief    Writes a byte to a specific address in the DUP's XDATA space.
* @param    address     XDATA address
* @param    value       Value to write
* @return   None.
******************************************************************************/
void CCLwrite_xdata_memory(unsigned short address, unsigned char value)
{
    unsigned char instr[3];

    // MOV DPTR, address
    instr[0] = 0x90;
    instr[1] = HIBYTE(address);
    instr[2] = LOBYTE(address);
    CCLdebug_command(CCL_CMD_DEBUG_INSTR_3B, instr, 3);

    // MOV A, values[i]
    instr[0] = 0x74;
    instr[1] = value;
    CCLdebug_command(CCL_CMD_DEBUG_INSTR_2B, instr, 2);

    // MOV @DPTR, A
    instr[0] = 0xF0;
    CCLdebug_command(CCL_CMD_DEBUG_INSTR_1B, instr, 1);
}

/**************************************************************************//**
* @brief    Read a byte from a specific address in the DUP's XDATA space.
* @param    address     XDATA address
* @return   Value read from XDATA
******************************************************************************/
unsigned char CCLread_xdata_memory(unsigned short address)
{
    unsigned char instr[3];

    // MOV DPTR, address
    instr[0] = 0x90;
    instr[1] = HIBYTE(address);
    instr[2] = LOBYTE(address);
    CCLdebug_command(CCL_CMD_DEBUG_INSTR_3B, instr, 3);

    // MOVX A, @DPTR
    instr[0] = 0xE0;
    return CCLdebug_command(CCL_CMD_DEBUG_INSTR_1B, instr, 1);
}

/**************************************************************************//**
* @brief    Reads 1-32767 bytes from DUP's flash to a given buffer on the
*           programmer.
* @param    bank        Flash bank to read from [0-7]
* @param    address     Flash memory start address [0x0000 - 0x7FFF]
* @param    values      Pointer to destination buffer.
* @return   None.
******************************************************************************/
void CCLread_flash_memory_block(unsigned char bank,unsigned short flash_addr,
                             unsigned short num_bytes, unsigned char *values)
{
    unsigned char instr[3];
    unsigned short i;
    unsigned short xdata_addr = (0x8000 + flash_addr);

    // 1. Map flash memory bank to XDATA address 0x8000-0xFFFF
    CCLwrite_xdata_memory(CCL_DUP_MEMCTR, bank);

    // 2. Move data pointer to XDATA address (MOV DPTR, xdata_addr)
    instr[0] = 0x90;
    instr[1] = HIBYTE(xdata_addr);
    instr[2] = LOBYTE(xdata_addr);
    CCLdebug_command(CCL_CMD_DEBUG_INSTR_3B, instr, 3);

    for (i = 0; i < num_bytes; i++)
    {
        // 3. Move value pointed to by DPTR to accumulator (MOVX A, @DPTR)
        instr[0] = 0xE0;
        values[i] = CCLdebug_command(CCL_CMD_DEBUG_INSTR_1B, instr, 1);

        // 4. Increment data pointer (INC DPTR)
        instr[0] = 0xA3;
        CCLdebug_command(CCL_CMD_DEBUG_INSTR_1B, instr, 1);
    }
}

/**************************************************************************//**
* @brief    Writes 4-2048 bytes to DUP's flash memory. Parameter \c num_bytes
*           must be a multiple of 4.
* @param    src         Pointer to programmer's source buffer (in XDATA space)
* @param    start_addr  FLASH memory start address [0x0000 - 0x7FFF]
* @param    num_bytes   Number of bytes to transfer [4-1024]
* @return   None.
******************************************************************************/
void CCLwrite_flash_memory_block(unsigned char *src, unsigned long start_addr,
                              unsigned short num_bytes)
{
    // 1. Write the 2 DMA descriptors to RAM
    CCLwrite_xdata_memory_block(CCL_ADDR_DMA_DESC_0, ccl_dma_desc_0, 8);
    CCLwrite_xdata_memory_block(CCL_ADDR_DMA_DESC_1, ccl_dma_desc_1, 8);

    // 2. Update LEN value in DUP's DMA descriptors
    unsigned char len[2] = {HIBYTE(num_bytes), LOBYTE(num_bytes)};
    CCLwrite_xdata_memory_block((CCL_ADDR_DMA_DESC_0+4), len, 2);  // LEN, DBG => ram
    CCLwrite_xdata_memory_block((CCL_ADDR_DMA_DESC_1+4), len, 2);  // LEN, ram => flash

    // 3. Set DMA controller pointer to the DMA descriptors
    CCLwrite_xdata_memory(CCL_DUP_DMA0CFGH, HIBYTE(CCL_ADDR_DMA_DESC_0));
    CCLwrite_xdata_memory(CCL_DUP_DMA0CFGL, LOBYTE(CCL_ADDR_DMA_DESC_0));
    CCLwrite_xdata_memory(CCL_DUP_DMA1CFGH, HIBYTE(CCL_ADDR_DMA_DESC_1));
    CCLwrite_xdata_memory(CCL_DUP_DMA1CFGL, LOBYTE(CCL_ADDR_DMA_DESC_1));

    // 4. Set Flash controller start address (wants 16MSb of 18 bit address)
    CCLwrite_xdata_memory(CCL_DUP_FADDRH, HIBYTE( (start_addr)));//>>2) ));
    CCLwrite_xdata_memory(CCL_DUP_FADDRL, LOBYTE( (start_addr)));//>>2) ));

    // 5. Arm DBG=>buffer DMA channel and start burst write
    CCLwrite_xdata_memory(CCL_DUP_DMAARM, CCL_CH_DBG_TO_BUF0);
    CCLburst_write_block(src, num_bytes);

    // 6. Start programming: buffer to flash
    CCLwrite_xdata_memory(CCL_DUP_DMAARM, CCL_CH_BUF0_TO_FLASH);
    CCLwrite_xdata_memory(CCL_DUP_FCTL, 0x0A);//0x06

    // 7. Wait until flash controller is done
    while (CCLread_xdata_memory(CCL_DUP_FCTL) & 0x80);
}

void CCLRunDUP(void)
{
  volatile unsigned char i;

  // Send two flanks on DC while keeping RESET_N low
  // All low (incl. RESET_N)
  digitalWrite(CCL_DD, LOW);
  digitalWrite(CCL_DC, LOW);
  digitalWrite(CCL_RESET, LOW);
  delay(10);   // Wait

  digitalWrite(CCL_RESET, HIGH);
  delay(10);   // Wait
}

void CCLoaderinit(void) {
  if (PinUsed(GPIO_ZIGBEE_RX) && PinUsed(GPIO_ZIGBEE_TX) && PinUsed(GPIO_ZIGBEE_RST)) {
    CCL_RESET = Pin(GPIO_ZIGBEE_RST);
    CCL_DD = Pin(GPIO_ZIGBEE_RX);
    CCL_DC = Pin(GPIO_ZIGBEE_TX);

    pinMode(CCL_DD, OUTPUT);
    pinMode(CCL_DC, OUTPUT);
    pinMode(CCL_RESET, OUTPUT);
    digitalWrite(CCL_DD, LOW);
    digitalWrite(CCL_DC, LOW);
    digitalWrite(CCL_RESET, HIGH);

    AddLog(LOG_LEVEL_INFO, PSTR("CCL: programmer init"));
    CCL.init = true;
  }
}

String CCLChipName(uint8_t chipID) {
    switch(chipID){
        case 0xa5:
        return F("CC2530");
        break;
        case 0xb5:
        return F("CC2531");
        break;
        case 0x95:
        return F("CC2533");
        break;
        case 0x8d:
        return F("CC2540");
        break;
        case 0x41:
        return F("CC2541");
        break;
    }
    return F("CCL: unknown");
}

void CCLoaderLoop() {
    static uint32_t step = 0;
    switch(step) {
        case 0:
            CCLdebug_init();
            AddLog(LOG_LEVEL_INFO,PSTR("CCL: debug init"));
            step++;
            break;
        case 1:
            CCLread_chip_id();
            if((CCL.chip.ID!=0)) {
                AddLog(LOG_LEVEL_INFO,PSTR("CCL: found chip with ID: %x, Rev: %x -> %s"), CCL.chip.ID, CCL.chip.rev, CCLChipName(CCL.chip.ID).c_str());
                step++;
            }
            else {
                AddLog(LOG_LEVEL_INFO,PSTR("CCL: no chip found"));
                return;
            }
            break;

    }
}

bool CLLFlashFirmware(uint8_t* data, uint32_t size)
{
    bool ret = true;
    unsigned char debug_config = 0;
    unsigned char Verify = 0;
    AddLog(LOG_LEVEL_INFO,PSTR("CCL: .bin file downloaded with size: %u blocks"), size/512);
    if (CCL.chip.ID!=0)
    {
        CCLRunDUP();
        CCLdebug_init();
        CCLchip_erase();
        CCLRunDUP();
        CCLdebug_init();
        // Switch DUP to external crystal osc. (XOSC) and wait for it to be stable.
        // This is recommended if XOSC is available during programming. If
        // XOSC is not available, comment out these two lines.
        CCLwrite_xdata_memory(CCL_DUP_CLKCONCMD, 0x80);
        while (CCLread_xdata_memory(CCL_DUP_CLKCONSTA) != 0x80) {};//0x80)

        // Enable DMA (Disable DMA_PAUSE bit in debug configuration)
        debug_config = 0x22;
        CCLdebug_command(CCL_CMD_WR_CONFIG, &debug_config, 1);

        unsigned char  rxBuf[512];
        uint32_t block = 0;
        unsigned int addr = 0x0000;
        AddLog(LOG_LEVEL_INFO,PSTR("CCL: will flash ...."));
        AddLogBuffer(LOG_LEVEL_DEBUG,data,16); // quick check to compare with a hex editor

        while((block*512)<size)
        {
            memcpy_P(rxBuf,data+(block*512),512);
            CCLwrite_flash_memory_block(rxBuf, addr, 512); // src, address, count

            unsigned char bank = addr / (512 * 16);
            unsigned int  offset = (addr % (512 * 16)) * 4;
            unsigned char read_data[512];
            CCLread_flash_memory_block(bank, offset, 512, read_data); // Bank, address, count, dest.
            for(unsigned int i = 0; i < 512; i++)
                {
                    if(read_data[i] !=rxBuf[i])
                    {
                    AddLog(LOG_LEVEL_INFO,PSTR("CCL: flashing error, erasing flash!!"));
                    CCLchip_erase();
                    return true;
                    }
                }

            addr += (unsigned int)128;
            block++;
            delay(10); // feed the dog
            AddLog(LOG_LEVEL_INFO,PSTR("CCL: written block %u of %u"), block, size/512);
        }
        CCLRunDUP();
    }
    return false;
}

bool CCLChipFound() {
    return CCL.chip.ID!=0;
}
/*********************************************************************************************\
 * oresentation
\*********************************************************************************************/

void CCLoadershow(bool json) {
    if (json) {
        // unused
    } else {
        WSContentSend_PD(PSTR("<h3>CCLoader</h3>"));
        if (CCL.chip.ID!=0){
            WSContentSend_PD(PSTR("Chip ID: %x<br>"),CCL.chip.ID);
            WSContentSend_PD(PSTR("Chip Revision: %x<br>"),CCL.chip.rev);
            WSContentSend_PD(PSTR("Chip Name: %s<br>"),CCLChipName(CCL.chip.ID).c_str());
        }
    }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv46(uint8_t function) {
  if (!TasmotaGlobal.gpio_optiona.enable_ccloader) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    CCLoaderinit();
  }
  if (CCL.init) {
    switch(function){
      case FUNC_EVERY_100_MSECOND:
        CCLoaderLoop();
        break;
      case FUNC_WEB_SENSOR:
        CCLoadershow(0);
        break;
    }
  }
  return result;
}

#endif  // USE_CCLOADER

