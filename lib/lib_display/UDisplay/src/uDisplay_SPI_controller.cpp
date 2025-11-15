#include "uDisplay_SPI_controller.h"

// ===== GPIO Macros =====
#ifdef ESP8266
#define PIN_OUT_SET 0x60000304
#define PIN_OUT_CLEAR 0x60000308
#define GPIO_SET(A) WRITE_PERI_REG(PIN_OUT_SET, 1 << A)
#define GPIO_CLR(A) WRITE_PERI_REG(PIN_OUT_CLEAR, 1 << A)
#define GPIO_SET_SLOW(A) digitalWrite(A, HIGH)
#define GPIO_CLR_SLOW(A) digitalWrite(A, LOW)
#else // ESP32
#if CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C5 || CONFIG_IDF_TARGET_ESP32C6 || CONFIG_IDF_TARGET_ESP32P4
#define GPIO_CLR(A) GPIO.out_w1tc.val = (1 << A)
#define GPIO_SET(A) GPIO.out_w1ts.val = (1 << A)
#else // plain ESP32 or S3
#define GPIO_CLR(A) GPIO.out_w1tc = (1 << A)
#define GPIO_SET(A) GPIO.out_w1ts = (1 << A)
#endif
#define GPIO_SET_SLOW(A) digitalWrite(A, HIGH)
#define GPIO_CLR_SLOW(A) digitalWrite(A, LOW)
#endif

// ===== RA8876 Constants =====
static constexpr uint8_t RA8876_DATA_WRITE  = 0x80;
static constexpr uint8_t RA8876_DATA_READ   = 0xC0;
static constexpr uint8_t RA8876_CMD_WRITE   = 0x00;
static constexpr uint8_t RA8876_STATUS_READ = 0x40;

extern void AddLog(uint32_t loglevel, const char* formatP, ...);

SPIController::SPIController(const SPIControllerConfig& config)
    : spi_config(config)
{
    if (spi_config.dc >= 0) {
      pinMode(spi_config.dc, OUTPUT);
      digitalWrite(spi_config.dc, HIGH);
    }
    if (spi_config.cs >= 0) {
      pinMode(spi_config.cs, OUTPUT);
      digitalWrite(spi_config.cs, HIGH);
    }

#ifdef ESP8266
    if (spi_config.bus_nr <= 1) {
      SPI.begin();
      spi = &SPI;
    } else {
      pinMode(spi_config.clk, OUTPUT);
      digitalWrite(spi_config.clk, LOW);
      pinMode(spi_config.mosi, OUTPUT);
      digitalWrite(spi_config.mosi, LOW);
      if (spi_config.miso >= 0) {
        pinMode(spi_config.miso, INPUT_PULLUP);
      }
    }
#endif // ESP8266

#ifdef ESP32
    if (spi_config.bus_nr == 1) {
      spi = &SPI;
      spi->begin(spi_config.clk, spi_config.miso, spi_config.mosi, -1);
    } else if (spi_config.bus_nr == 2) {
      spi = new SPIClass(HSPI);
      spi->begin(spi_config.clk, spi_config.miso, spi_config.mosi, -1);
    } else {
      pinMode(spi_config.clk, OUTPUT);
      digitalWrite(spi_config.clk, LOW);
      pinMode(spi_config.mosi, OUTPUT);
      digitalWrite(spi_config.mosi, LOW);
      if (spi_config.miso >= 0) {
        pinMode(spi_config.miso, INPUT_PULLUP);
      }
    }
#endif // ESP32
    spi_settings = SPISettings((uint32_t)spi_config.speed*1000000, MSBFIRST, SPI_MODE3);
}

// ===== Pin Control =====

void SPIController::csLow() {
    if (spi_config.cs >= 0) GPIO_CLR_SLOW(spi_config.cs);
}

void SPIController::csHigh() {
    if (spi_config.cs >= 0) GPIO_SET_SLOW(spi_config.cs);
}

void SPIController::dcLow() {
    if (spi_config.dc >= 0) GPIO_CLR_SLOW(spi_config.dc);
}

void SPIController::dcHigh() {
    if (spi_config.dc >= 0) GPIO_SET_SLOW(spi_config.dc);
}

// ===== Transaction Control =====

void SPIController::beginTransaction() {
    if (spi_config.bus_nr <= 2) spi->beginTransaction(spi_settings);
}

void SPIController::endTransaction() {
    if (spi_config.bus_nr <= 2) spi->endTransaction();
}

// ===== Low-Level Write Functions =====
void SPIController::writeCommand(uint8_t cmd) {
    if (spi_config.dc < 0) {
        // 9-bit mode
        if (spi_config.bus_nr > 2) {
            if (spi_config.bus_nr == 3) write9(cmd, 0);
            else write9_slow(cmd, 0);
        } else {
            hw_write9(cmd, 0);
        }
    } else {
        // 8-bit mode  
        dcLow();
        writeData8(cmd);
        dcHigh();
    }
}

void SPIController::writeData8(uint8_t data) {
    if (spi_config.dc < 0) {
        // 9-bit mode
        if (spi_config.bus_nr > 2) {
            if (spi_config.bus_nr == 3) write9(data, 1);
            else write9_slow(data, 1);
        } else {
            hw_write9(data, 1);
        }
    } else {
        // 8-bit mode
        if (spi_config.bus_nr > 2) {
            if (spi_config.bus_nr == 3) write8(data);
            else write8_slow(data);
        } else {
            spi->write(data);
        }
    }
}

void SPIController::writeData16(uint16_t data) {
    if (spi_config.dc < 0) {
        // 9-bit: break into bytes
        writeData8(data >> 8);
        writeData8(data);
    } else {
        // 8-bit mode
        if (spi_config.bus_nr > 2) {
            if (spi_config.bus_nr == 3) write16(data);
            else {
                // Slow mode: break into bytes
                writeData8(data >> 8);
                writeData8(data);
            }
        } else {
            spi->write16(data); // Assume SPI has write16
        }
    }
}

void SPIController::writeData32(uint32_t data) {
    if (spi_config.dc < 0) {
        // 9-bit mode: break into bytes
        writeData8(data >> 24);
        writeData8(data >> 16);
        writeData8(data >> 8);
        writeData8(data);
    } else {
        // 8-bit mode
        if (spi_config.bus_nr > 2) {
            if (spi_config.bus_nr == 3) {
                write32(data);  // Fast bit-banging
            } else {
                // Slow mode: break into bytes
                writeData8(data >> 24);
                writeData8(data >> 16);
                writeData8(data >> 8);
                writeData8(data);
            }
        } else {
            // Hardware SPI
            spi->write32(data);  // Assume SPI has write32 on ESP32
        }
    }
}

// ===== Low-Level Write Functions =====

void SPIController::write8(uint8_t val) {
    for (uint8_t bit = 0x80; bit; bit >>= 1) {
        GPIO_CLR(spi_config.clk);
        if (val & bit) GPIO_SET(spi_config.mosi);
        else GPIO_CLR(spi_config.mosi);
        GPIO_SET(spi_config.clk);
    }
}

void SPIController::write8_slow(uint8_t val) {
    for (uint8_t bit = 0x80; bit; bit >>= 1) {
        GPIO_CLR_SLOW(spi_config.clk);
        if (val & bit) GPIO_SET_SLOW(spi_config.mosi);
        else GPIO_CLR_SLOW(spi_config.mosi);
        GPIO_SET_SLOW(spi_config.clk);
    }
}

void SPIController::write9(uint8_t val, uint8_t dc) {
    GPIO_CLR(spi_config.clk);
    if (dc) GPIO_SET(spi_config.mosi);
    else GPIO_CLR(spi_config.mosi);
    GPIO_SET(spi_config.clk);

    for (uint8_t bit = 0x80; bit; bit >>= 1) {
        GPIO_CLR(spi_config.clk);
        if (val & bit) GPIO_SET(spi_config.mosi);
        else GPIO_CLR(spi_config.mosi);
        GPIO_SET(spi_config.clk);
    }
}

void SPIController::write9_slow(uint8_t val, uint8_t dc) {
    GPIO_CLR_SLOW(spi_config.clk);
    if (dc) GPIO_SET_SLOW(spi_config.mosi);
    else GPIO_CLR_SLOW(spi_config.mosi);
    GPIO_SET_SLOW(spi_config.clk);

    for (uint8_t bit = 0x80; bit; bit >>= 1) {
        GPIO_CLR_SLOW(spi_config.clk);
        if (val & bit) GPIO_SET_SLOW(spi_config.mosi);
        else GPIO_CLR_SLOW(spi_config.mosi);
        GPIO_SET_SLOW(spi_config.clk);
    }
}

void SPIController::write16(uint16_t val) {
    for (uint16_t bit = 0x8000; bit; bit >>= 1) {
        GPIO_CLR(spi_config.clk);
        if (val & bit) GPIO_SET(spi_config.mosi);
        else GPIO_CLR(spi_config.mosi);
        GPIO_SET(spi_config.clk);
    }
}

void SPIController::write32(uint32_t val) {
    for (uint32_t bit = 0x80000000; bit; bit >>= 1) {
        GPIO_CLR(spi_config.clk);
        if (val & bit) GPIO_SET(spi_config.mosi);
        else GPIO_CLR(spi_config.mosi);
        GPIO_SET(spi_config.clk);
    }
}

// ===== Hardware 9-bit Mode =====

#ifdef ESP32
void SPIController::hw_write9(uint8_t val, uint8_t dc) {
    if (spi_config.dc < -1) {
        // RA8876 mode
        if (!dc) {
            spi->write(RA8876_CMD_WRITE);
            spi->write(val);
        } else {
            spi->write(RA8876_DATA_WRITE);
            spi->write(val);
        }
    } else {
        uint32_t regvalue = val >> 1;
        if (dc) regvalue |= 0x80;
        else regvalue &= 0x7f;
        if (val & 1) regvalue |= 0x8000;

        REG_SET_BIT(SPI_USER_REG(3), SPI_USR_MOSI);
        REG_WRITE(SPI_MOSI_DLEN_REG(3), 9 - 1);
        uint32_t *dp = (uint32_t*)SPI_W0_REG(3);
        *dp = regvalue;
        REG_SET_BIT(SPI_CMD_REG(3), SPI_USR);
        while (REG_GET_FIELD(SPI_CMD_REG(3), SPI_USR));
    }
}
#else
void SPIController::hw_write9(uint8_t val, uint8_t dc) {
    if (spi_config.dc < -1) {
        // RA8876 mode
        if (!dc) {
            spi->write(RA8876_CMD_WRITE);
            spi->write(val);
        } else {
            spi->write(RA8876_DATA_WRITE);
            spi->write(val);
        }
    } else {
        uint32_t regvalue;
        uint8_t bytetemp;
        if (!dc) {
            bytetemp = (val >> 1) & 0x7f;
        } else {
            bytetemp = (val >> 1) | 0x80;
        }
        regvalue = ((8 & SPI_USR_COMMAND_BITLEN) << SPI_USR_COMMAND_BITLEN_S) | ((uint32)bytetemp);
        if (val & 0x01) regvalue |= BIT15;
        while (READ_PERI_REG(SPI_CMD(1)) & SPI_USR);
        WRITE_PERI_REG(SPI_USER2(1), regvalue);
        SET_PERI_REG_MASK(SPI_CMD(1), SPI_USR);
    }
}
#endif

// DMA
#ifdef ESP32
bool SPIController::initDMA(uint16_t width, uint16_t flushlines, uint8_t data) {
    AddLog(3,"init dma %u %u %d",flushlines,data, spi_config.cs);
    if (!spi && spi_config.cs == -1) return false;
    if((data&1) == 0){
        AddLog(3,"no dma selected");
        return false;
    }
    if (spi_config.bus_nr == 1){
        AddLog(3,"dma spi 1");
    } else if (spi_config.bus_nr == 2){
        AddLog(3,"dma spi 2");
        spi_host = HSPI_HOST;
    } else {
        return false;
    }
    
    esp_err_t ret;
    spi_bus_config_t buscfg = {
        .mosi_io_num = spi_config.mosi,
        .miso_io_num = spi_config.miso,
        .sclk_io_num = spi_config.clk,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = width * flushlines * 2 + 8,
        .flags = 0,
        .intr_flags = 0
    };

    spi_device_interface_config_t devcfg = {
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
        .mode = SPI_MODE3,
        .duty_cycle_pos = 0,
        .cs_ena_pretrans = 0,
        .cs_ena_posttrans = 0,
        .clock_speed_hz = (int)spi_config.speed,
        .input_delay_ns = 0,
        .spics_io_num = spi_config.cs,
        .flags = SPI_DEVICE_NO_DUMMY,
        .queue_size = 1,
        .pre_cb = 0,
        .post_cb = 0
    };
    
    // spi_host_device_t spi_host = (spi_config.bus_nr == 1) ? VSPI_HOST : HSPI_HOST;
    
    // Try to initialize the bus, but if it's already initialized (by Arduino SPI), that's OK

    ret = spi_bus_initialize(spi_host, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE) {
        AddLog(3,"init dma bus init failed: %d", ret);
        return false;
    }
    if (ret == ESP_ERR_INVALID_STATE) {
        AddLog(3,"init dma bus already initialized (OK)");
    }
    
    ret = spi_bus_add_device(spi_host, &devcfg, &dmaHAL);
    if (ret == ESP_OK) {
        DMA_Enabled = true;
        async_dma_enabled = ((data&4) != 0);
        dma_enabled = true;
        spiBusyCheck = 0;
        AddLog(3,"init dma succes");
        return true;
    }
    return false;
}

// just a placeholder
// void SPIController::deInitDMA(void) {
//   if (!DMA_Enabled) return;
//   spi_bus_remove_device(dmaHAL);
//   spi_bus_free(spi_host);
//   DMA_Enabled = false;
// }

bool SPIController::dmaBusy(void) {
  if (!DMA_Enabled || !spiBusyCheck) return false;

  spi_transaction_t *rtrans;
  esp_err_t ret;
  uint8_t checks = spiBusyCheck;
  for (int i = 0; i < checks; ++i) {
    ret = spi_device_get_trans_result(dmaHAL, &rtrans, 0);
    if (ret == ESP_OK) spiBusyCheck--;
  }
  if (spiBusyCheck == 0) return false;
  return true;
}

void SPIController::dmaWait(void) {
  if (!DMA_Enabled || !spiBusyCheck) return;
  spi_transaction_t *rtrans;
  esp_err_t ret;
  for (int i = 0; i < spiBusyCheck; ++i) {
    ret = spi_device_get_trans_result(dmaHAL, &rtrans, portMAX_DELAY);
    assert(ret == ESP_OK);
  }
  spiBusyCheck = 0;
}

void SPIController::pushPixelsDMA(uint16_t* image, uint32_t len) {
  if(!DMA_Enabled){
    getSPI()->writePixels(image, len * 2);
    return;
  }
  if (len == 0) return;

  dmaWait();

  esp_err_t ret;

  memset(&trans, 0, sizeof(spi_transaction_t));

  trans.user = (void *)1;
  trans.tx_buffer = image;  //finally send the line data
  trans.length = len * 16;        //Data length, in bits
  trans.flags = 0;                //SPI_TRANS_USE_TXDATA flag

  ret = spi_device_queue_trans(dmaHAL, &trans, portMAX_DELAY);
  assert(ret == ESP_OK);

  spiBusyCheck++;
  if (!async_dma_enabled) {
    dmaWait();
  }
}

void SPIController::pushPixels3DMA(uint8_t* image, uint32_t len) {
  if ((len == 0) || (!DMA_Enabled)) return;

  dmaWait();

  esp_err_t ret;

  memset(&trans, 0, sizeof(spi_transaction_t));

  trans.user = (void *)1;
  trans.tx_buffer = image;  //finally send the line data
  trans.length = len * 24;        //Data length, in bits
  trans.flags = 0;                //SPI_TRANS_USE_TXDATA flag

  ret = spi_device_queue_trans(dmaHAL, &trans, portMAX_DELAY);
  assert(ret == ESP_OK);

  spiBusyCheck++;
  if (!async_dma_enabled) {
    dmaWait();
  }
}
#endif // ESP32
// ===== RA8876 Specific =====

uint8_t SPIController::writeReg16(uint8_t reg, uint16_t wval) {
    hw_write9(reg, 0);
    hw_write9(wval, 1);
    hw_write9(reg + 1, 0);
    hw_write9(wval >> 8, 1);
    return 0;
}

uint8_t SPIController::readData(void) {
    if (!spi) return 0;
    spi->write(RA8876_DATA_READ);
    return spi->transfer(0);
}

uint8_t SPIController::readStatus(void) {
    if (!spi) return 0;
    spi->write(RA8876_STATUS_READ);
    return spi->transfer(0);
}
