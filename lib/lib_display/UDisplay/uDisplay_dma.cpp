#include "uDisplay.h"
#include "uDisplay_config.h"

#ifdef ESP32

// ===== DMA Initialization and Control =====
// ESP 32 DMA section , derived from TFT_eSPI

bool uDisplay::initDMA(int32_t ctrl_cs) {
  if (DMA_Enabled) return false;

  esp_err_t ret;
  spi_bus_config_t buscfg = {
    .mosi_io_num = spi_mosi,
    .miso_io_num = -1,
    .sclk_io_num = spi_clk,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = width() * height() * 2 + 8, // TFT screen size
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
    .clock_speed_hz = spi_speed*1000000,
    .input_delay_ns = 0,
    .spics_io_num = ctrl_cs,
    .flags = SPI_DEVICE_NO_DUMMY, //0,
    .queue_size = 1,
    .pre_cb = 0, //dc_callback, //Callback to handle D/C line
    .post_cb = 0
  };
  
  ret = spi_bus_initialize(spi_host, &buscfg, 1);
  ESP_ERROR_CHECK(ret);
  ret = spi_bus_add_device(spi_host, &devcfg, &dmaHAL);
  ESP_ERROR_CHECK(ret);

  DMA_Enabled = true;
  spiBusyCheck = 0;
  return true;
}

void uDisplay::deInitDMA(void) {
  if (!DMA_Enabled) return;
  spi_bus_remove_device(dmaHAL);
  spi_bus_free(spi_host);
  DMA_Enabled = false;
}

bool uDisplay::dmaBusy(void) {
  if (!DMA_Enabled || !spiBusyCheck) return false;

  spi_transaction_t *rtrans;
  esp_err_t ret;
  uint8_t checks = spiBusyCheck;
  for (int i = 0; i < checks; ++i) {
    ret = spi_device_get_trans_result(dmaHAL, &rtrans, 0);
    if (ret == ESP_OK) spiBusyCheck--;
  }

  //Serial.print("spiBusyCheck=");Serial.println(spiBusyCheck);
  if (spiBusyCheck == 0) return false;
  return true;
}

void uDisplay::dmaWait(void) {
  if (!DMA_Enabled || !spiBusyCheck) return;
  spi_transaction_t *rtrans;
  esp_err_t ret;
  for (int i = 0; i < spiBusyCheck; ++i) {
    ret = spi_device_get_trans_result(dmaHAL, &rtrans, portMAX_DELAY);
    assert(ret == ESP_OK);
  }
  spiBusyCheck = 0;
}

// ===== DMA Data Transfer Functions =====

void uDisplay::pushPixelsDMA(uint16_t* image, uint32_t len) {
  if ((len == 0) || (!DMA_Enabled)) return;

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
  if (!lvgl_param.async_dma) {
    dmaWait();
  }
}

void uDisplay::pushPixels3DMA(uint8_t* image, uint32_t len) {
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
  if (!lvgl_param.async_dma) {
    dmaWait();
  }
}

#endif // ESP32