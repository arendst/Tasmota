#include "lcd_driver.h"
#include "epdiy.h"

#include "../output_common/render_method.h"
#include "esp_heap_caps.h"
#include "esp_intr_alloc.h"
#include "hal/gpio_types.h"

#ifdef RENDER_METHOD_LCD

#include <assert.h>
#include <esp_idf_version.h>
#include <esp_log.h>
#include <soc/lcd_periph.h>
#include <soc/rmt_periph.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <driver/rmt_tx.h>
#include <driver/rmt_types.h>
#include <driver/rmt_types_legacy.h>
#include <esp_private/periph_ctrl.h>
#include <hal/rmt_types.h>
#include <soc/clk_tree_defs.h>

#include <driver/gpio.h>
#include <esp_check.h>
#include <esp_err.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_rgb.h>
#include <esp_private/gdma.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <hal/dma_types.h>
#include <hal/gdma_ll.h>
#include <hal/gpio_hal.h>
#include <hal/lcd_hal.h>
#include <hal/lcd_ll.h>
#include <hal/rmt_ll.h>
#include <rom/cache.h>
#include <soc/lcd_periph.h>
#include <soc/rmt_struct.h>

#include "hal/gpio_hal.h"

gpio_hal_context_t hal = { .dev = GPIO_HAL_GET_HW(GPIO_PORT_0) };

#define TAG "epdiy"

#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 3, 2)
#define LCD_PERIPH_SIGNALS lcd_periph_signals
#else
#define LCD_PERIPH_SIGNALS lcd_periph_rgb_signals
#endif

static inline int min(int x, int y) {
    return x < y ? x : y;
}
static inline int max(int x, int y) {
    return x > y ? x : y;
}

#define S3_LCD_PIN_NUM_BK_LIGHT -1
// #define S3_LCD_PIN_NUM_MODE           4

#define LINE_BATCH 1000
#define BOUNCE_BUF_LINES 4

#define RMT_CKV_CHAN RMT_CHANNEL_1

// The extern line is declared in esp-idf/components/driver/deprecated/rmt_legacy.c. It has access
// to RMTMEM through the rmt_private.h header which we can't access outside the sdk. Declare our own
// extern here to properly use the RMTMEM smybol defined in
// components/soc/[target]/ld/[target].peripherals.ld Also typedef the new rmt_mem_t struct to the
// old rmt_block_mem_t struct. Same data fields, different names
typedef rmt_mem_t rmt_block_mem_t;
extern rmt_block_mem_t RMTMEM;

// spinlock for protecting the critical section at frame start
static portMUX_TYPE frame_start_spinlock = portMUX_INITIALIZER_UNLOCKED;

typedef struct {
    lcd_hal_context_t hal;
    intr_handle_t vsync_intr;
    intr_handle_t done_intr;

    frame_done_func_t frame_done_cb;
    line_cb_func_t line_source_cb;
    void* line_cb_payload;
    void* frame_cb_payload;

    int line_length_us;
    int line_cycles;
    int lcd_res_h;

    LcdEpdConfig_t config;

    uint8_t* bounce_buffer[2];
    // size of a single bounce buffer
    size_t bb_size;
    size_t batches;

    // Number of DMA descriptors that used to carry the frame buffer
    size_t num_dma_nodes;
    // DMA channel handle
    gdma_channel_handle_t dma_chan;
    // DMA descriptors pool
    dma_descriptor_t* dma_nodes;

    /// The number of bytes in a horizontal display register line.
    int line_bytes;

    // With 8 bit bus width, we need a dummy cycle before the actual data,
    // because the LCD peripheral behaves weirdly.
    // Also see:
    // https://blog.adafruit.com/2022/06/14/esp32uesday-hacking-the-esp32-s3-lcd-peripheral/
    int dummy_bytes;

    /// The number of lines of the display
    int display_lines;
} s3_lcd_t;

static s3_lcd_t lcd = { 0 };

void IRAM_ATTR epd_lcd_line_source_cb(line_cb_func_t line_source, void* payload) {
    lcd.line_source_cb = line_source;
    lcd.line_cb_payload = payload;
}

void IRAM_ATTR epd_lcd_frame_done_cb(frame_done_func_t cb, void* payload) {
    lcd.frame_done_cb = cb;
    lcd.frame_cb_payload = payload;
}

static IRAM_ATTR bool fill_bounce_buffer(uint8_t* buffer) {
    bool task_awoken = false;

    for (int i = 0; i < BOUNCE_BUF_LINES; i++) {
        if (lcd.line_source_cb != NULL) {
            // this is strange, with 16 bit need a dummy cycle. But still, the first byte in the
            // FIFO is correct. So we only need a true dummy byte in the FIFO in the 8 bit
            // configuration.
            int buffer_offset = i * (lcd.line_bytes + lcd.dummy_bytes) + (lcd.dummy_bytes % 2);
            task_awoken |= lcd.line_source_cb(lcd.line_cb_payload, &buffer[buffer_offset]);
        } else {
            memset(&buffer[i * lcd.line_bytes], 0x00, lcd.line_bytes);
        }
    }
    return task_awoken;
}

static void start_ckv_cycles(int cycles) {
    rmt_ll_tx_enable_loop_count(&RMT, RMT_CKV_CHAN, true);
    rmt_ll_tx_enable_loop_autostop(&RMT, RMT_CKV_CHAN, true);
    rmt_ll_tx_set_loop_count(&RMT, RMT_CKV_CHAN, cycles);
    rmt_ll_tx_reset_pointer(&RMT, RMT_CKV_CHAN);
    rmt_ll_tx_start(&RMT, RMT_CKV_CHAN);
}

/**
 * Build the RMT signal according to the timing set in the lcd object.
 */
static void ckv_rmt_build_signal() {
    int low_time = (lcd.line_length_us * 10 - lcd.config.ckv_high_time);
    volatile rmt_item32_t* rmt_mem_ptr = &(RMTMEM.chan[RMT_CKV_CHAN].data32[0]);
    rmt_mem_ptr->duration0 = lcd.config.ckv_high_time;
    rmt_mem_ptr->level0 = 1;
    rmt_mem_ptr->duration1 = low_time;
    rmt_mem_ptr->level1 = 0;
    rmt_mem_ptr[1].val = 0;
}

/**
 * Configure the RMT peripheral for use as the CKV clock.
 */
static void init_ckv_rmt() {
    periph_module_reset(PERIPH_RMT_MODULE);
    periph_module_enable(PERIPH_RMT_MODULE);

    rmt_ll_enable_periph_clock(&RMT, true);

    // Divide 80MHz APB Clock by 8 -> .1us resolution delay
    // idf >= 5.0 calculates the clock divider differently
    rmt_ll_set_group_clock_src(&RMT, RMT_CKV_CHAN, RMT_CLK_SRC_DEFAULT, 1, 0, 0);
    rmt_ll_tx_set_channel_clock_div(&RMT, RMT_CKV_CHAN, 8);
    rmt_ll_tx_set_mem_blocks(&RMT, RMT_CKV_CHAN, 2);
    rmt_ll_enable_mem_access_nonfifo(&RMT, true);
    rmt_ll_tx_fix_idle_level(&RMT, RMT_CKV_CHAN, RMT_IDLE_LEVEL_LOW, true);
    rmt_ll_tx_enable_carrier_modulation(&RMT, RMT_CKV_CHAN, false);

    rmt_ll_tx_enable_loop(&RMT, RMT_CKV_CHAN, true);

    gpio_hal_func_sel(&hal, lcd.config.bus.ckv, PIN_FUNC_GPIO);
    gpio_set_direction(lcd.config.bus.ckv, GPIO_MODE_OUTPUT);
    esp_rom_gpio_connect_out_signal(
        lcd.config.bus.ckv, rmt_periph_signals.groups[0].channels[RMT_CKV_CHAN].tx_sig, false, 0
    );

    ckv_rmt_build_signal();
}

/**
 * Reset the CKV RMT configuration.
 */
static void deinit_ckv_rmt() {
    periph_module_reset(PERIPH_RMT_MODULE);
    periph_module_disable(PERIPH_RMT_MODULE);

    gpio_reset_pin(lcd.config.bus.ckv);
}

__attribute__((optimize("O3"))) IRAM_ATTR static void lcd_isr_vsync(void* args) {
    bool need_yield = false;

    uint32_t intr_status = lcd_ll_get_interrupt_status(lcd.hal.dev);
    lcd_ll_clear_interrupt_status(lcd.hal.dev, intr_status);

    if (intr_status & LCD_LL_EVENT_VSYNC_END) {
        int batches_needed = lcd.display_lines / LINE_BATCH;
        if (lcd.batches >= batches_needed) {
            lcd_ll_stop(lcd.hal.dev);
            if (lcd.frame_done_cb != NULL) {
                (*lcd.frame_done_cb)(lcd.frame_cb_payload);
            }
        } else {
            int ckv_cycles = 0;
            // last batch
            if (lcd.batches == batches_needed - 1) {
                lcd_ll_enable_auto_next_frame(lcd.hal.dev, false);
                lcd_ll_set_vertical_timing(lcd.hal.dev, 1, 0, lcd.display_lines % LINE_BATCH, 10);
                ckv_cycles = lcd.display_lines % LINE_BATCH + 10;
            } else {
                lcd_ll_set_vertical_timing(lcd.hal.dev, 1, 0, LINE_BATCH, 1);
                ckv_cycles = LINE_BATCH + 1;
            }
            // apparently, this is needed for the new timing to take effect.
            lcd_ll_start(lcd.hal.dev);

            // skip the LCD front porch line, which is not actual data
            esp_rom_delay_us(lcd.line_length_us);
            start_ckv_cycles(ckv_cycles);
        }

        lcd.batches += 1;
    }

    if (need_yield) {
        portYIELD_FROM_ISR();
    }
};

// ISR handling bounce buffer refill
static IRAM_ATTR bool lcd_rgb_panel_eof_handler(
    gdma_channel_handle_t dma_chan, gdma_event_data_t* event_data, void* user_data
) {
    dma_descriptor_t* desc = (dma_descriptor_t*)event_data->tx_eof_desc_addr;
    // Figure out which bounce buffer to write to.
    // Note: what we receive is the *last* descriptor of this bounce buffer.
    int bb = (desc == &lcd.dma_nodes[0]) ? 0 : 1;
    return fill_bounce_buffer(lcd.bounce_buffer[bb]);
}

static esp_err_t init_dma_trans_link() {
    lcd.dma_nodes[0].dw0.suc_eof = 1;
    lcd.dma_nodes[0].dw0.size = lcd.bb_size;
    lcd.dma_nodes[0].dw0.length = lcd.bb_size;
    lcd.dma_nodes[0].dw0.owner = DMA_DESCRIPTOR_BUFFER_OWNER_CPU;
    lcd.dma_nodes[0].buffer = lcd.bounce_buffer[0];

    lcd.dma_nodes[1].dw0.suc_eof = 1;
    lcd.dma_nodes[1].dw0.size = lcd.bb_size;
    lcd.dma_nodes[1].dw0.length = lcd.bb_size;
    lcd.dma_nodes[1].dw0.owner = DMA_DESCRIPTOR_BUFFER_OWNER_CPU;
    lcd.dma_nodes[1].buffer = lcd.bounce_buffer[1];

    // loop end back to start
    lcd.dma_nodes[0].next = &lcd.dma_nodes[1];
    lcd.dma_nodes[1].next = &lcd.dma_nodes[0];

    // alloc DMA channel and connect to LCD peripheral
    gdma_channel_alloc_config_t dma_chan_config = {
        .direction = GDMA_CHANNEL_DIRECTION_TX,
    };
    ESP_RETURN_ON_ERROR(
        gdma_new_channel(&dma_chan_config, &lcd.dma_chan), TAG, "alloc DMA channel failed"
    );
    gdma_trigger_t trigger = GDMA_MAKE_TRIGGER(GDMA_TRIG_PERIPH_LCD, 0);
    ESP_RETURN_ON_ERROR(gdma_connect(lcd.dma_chan, trigger), TAG, "dma connect error");
    gdma_transfer_ability_t ability = {
        .psram_trans_align = 64,
        .sram_trans_align = 4,
    };
    ESP_RETURN_ON_ERROR(gdma_set_transfer_ability(lcd.dma_chan, &ability), TAG, "dma setup error");

    gdma_tx_event_callbacks_t cbs = {
        .on_trans_eof = lcd_rgb_panel_eof_handler,
    };
    ESP_RETURN_ON_ERROR(
        gdma_register_tx_event_callbacks(lcd.dma_chan, &cbs, NULL), TAG, "dma setup error"
    );

    return ESP_OK;
}

void deinit_dma_trans_link() {
    gdma_reset(lcd.dma_chan);
    gdma_disconnect(lcd.dma_chan);
    gdma_del_channel(lcd.dma_chan);
}

/**
 * Configure LCD peripheral and auxiliary GPIOs
 */
static esp_err_t init_bus_gpio() {
    const int DATA_LINES[16] = {
        lcd.config.bus.data[14], lcd.config.bus.data[15], lcd.config.bus.data[12],
        lcd.config.bus.data[13], lcd.config.bus.data[10], lcd.config.bus.data[11],
        lcd.config.bus.data[8],  lcd.config.bus.data[9],  lcd.config.bus.data[6],
        lcd.config.bus.data[7],  lcd.config.bus.data[4],  lcd.config.bus.data[5],
        lcd.config.bus.data[2],  lcd.config.bus.data[3],  lcd.config.bus.data[0],
        lcd.config.bus.data[1],
    };

    // connect peripheral signals via GPIO matrix
    for (size_t i = (16 - lcd.config.bus_width); i < 16; i++) {
        gpio_hal_func_sel(&hal, DATA_LINES[i], PIN_FUNC_GPIO);
        gpio_set_direction(DATA_LINES[i], GPIO_MODE_OUTPUT);
        esp_rom_gpio_connect_out_signal(
            DATA_LINES[i], LCD_PERIPH_SIGNALS.panels[0].data_sigs[i], false, false
        );
    }
    gpio_hal_func_sel(&hal, lcd.config.bus.leh, PIN_FUNC_GPIO);
    gpio_set_direction(lcd.config.bus.leh, GPIO_MODE_OUTPUT);
    gpio_hal_func_sel(&hal, lcd.config.bus.clock, PIN_FUNC_GPIO);
    gpio_set_direction(lcd.config.bus.clock, GPIO_MODE_OUTPUT);
    gpio_hal_func_sel(&hal, lcd.config.bus.start_pulse, PIN_FUNC_GPIO);
    gpio_set_direction(lcd.config.bus.start_pulse, GPIO_MODE_OUTPUT);

    esp_rom_gpio_connect_out_signal(
        lcd.config.bus.leh, LCD_PERIPH_SIGNALS.panels[0].hsync_sig, false, false
    );
    esp_rom_gpio_connect_out_signal(
        lcd.config.bus.clock, LCD_PERIPH_SIGNALS.panels[0].pclk_sig, false, false
    );
    esp_rom_gpio_connect_out_signal(
        lcd.config.bus.start_pulse, LCD_PERIPH_SIGNALS.panels[0].de_sig, false, false
    );

    gpio_config_t vsync_gpio_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ull << lcd.config.bus.stv,
    };
    gpio_config(&vsync_gpio_conf);
    gpio_set_level(lcd.config.bus.stv, 1);
    return ESP_OK;
}

/**
 * Reset bus GPIO pin functions.
 */
static void deinit_bus_gpio() {
    for (size_t i = (16 - lcd.config.bus_width); i < 16; i++) {
        gpio_reset_pin(lcd.config.bus.data[i]);
    }

    gpio_reset_pin(lcd.config.bus.leh);
    gpio_reset_pin(lcd.config.bus.clock);
    gpio_reset_pin(lcd.config.bus.start_pulse);
    gpio_reset_pin(lcd.config.bus.stv);
}

/**
 * Check if the PSRAM cache is properly configured.
 */
static void check_cache_configuration() {
    if (CONFIG_ESP32S3_DATA_CACHE_LINE_SIZE < 64) {
        ESP_LOGE(
            "epdiy",
            "cache line size is set to %d (< 64B)! This will degrade performance, please update "
            "this option in menuconfig.",
            CONFIG_ESP32S3_DATA_CACHE_LINE_SIZE
        );
        ESP_LOGE(
            "epdiy",
            "If you are on arduino, you can't set this option yourself, you'll need to use a lower "
            "speed."
        );
        ESP_LOGE(
            "epdiy",
            "Reducing the pixel clock from %d MHz to %d MHz for now!",
            lcd.config.pixel_clock / 1000 / 1000,
            lcd.config.pixel_clock / 1000 / 1000 / 2
        );
        lcd.config.pixel_clock = lcd.config.pixel_clock / 2;

        // fixme: this would be nice, but doesn't work :(
        // uint32_t d_autoload = Cache_Suspend_DCache();
        /// Cache_Set_DCache_Mode(CACHE_SIZE_FULL, CACHE_4WAYS_ASSOC, CACHE_LINE_SIZE_32B);
        // Cache_Invalidate_DCache_All();
        // Cache_Resume_DCache(d_autoload);
    }
}

/**
 * Assign LCD configuration parameters from a given configuration, without allocating memory or
 * touching the LCD peripheral config.
 */
static void assign_lcd_parameters_from_config(
    const LcdEpdConfig_t* config, int display_width, int display_height
) {
    // copy over the configuraiton object
    memcpy(&lcd.config, config, sizeof(LcdEpdConfig_t));

    // Make sure the bounce buffers divide the display height evenly.
    lcd.display_lines = (((display_height + 7) / 8) * 8);

    lcd.line_bytes = display_width / 4;
    lcd.lcd_res_h = lcd.line_bytes / (lcd.config.bus_width / 8);

    // With 8 bit bus width, we need a dummy cycle before the actual data,
    // because the LCD peripheral behaves weirdly.
    // Also see:
    // https://blog.adafruit.com/2022/06/14/esp32uesday-hacking-the-esp32-s3-lcd-peripheral/
    lcd.dummy_bytes = lcd.config.bus_width / 8;

    // each bounce buffer holds a number of lines with data + dummy bytes each
    lcd.bb_size = BOUNCE_BUF_LINES * (lcd.line_bytes + lcd.dummy_bytes);

    check_cache_configuration();

    ESP_LOGI(TAG, "using resolution %dx%d", lcd.lcd_res_h, lcd.display_lines);
}

/**
 * Allocate buffers for LCD driver operation.
 */
static esp_err_t allocate_lcd_buffers() {
    uint32_t dma_flags = MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA;

    // allocate bounce buffers
    for (int i = 0; i < 2; i++) {
        lcd.bounce_buffer[i] = heap_caps_aligned_calloc(4, 1, lcd.bb_size, dma_flags);
        ESP_RETURN_ON_FALSE(lcd.bounce_buffer[i], ESP_ERR_NO_MEM, TAG, "install interrupt failed");
    }

    // So far, I haven't seen any displays with > 4096 pixels per line,
    // so we only need one DMA node for now.
    assert(lcd.bb_size < DMA_DESCRIPTOR_BUFFER_MAX_SIZE);
    lcd.dma_nodes = heap_caps_calloc(1, sizeof(dma_descriptor_t) * 2, dma_flags);
    ESP_RETURN_ON_FALSE(lcd.dma_nodes, ESP_ERR_NO_MEM, TAG, "no mem for dma nodes");
    return ESP_OK;
}

static void free_lcd_buffers() {
    for (int i = 0; i < 2; i++) {
        uint8_t* buf = lcd.bounce_buffer[i];
        if (buf != NULL) {
            heap_caps_free(buf);
            lcd.bounce_buffer[i] = NULL;
        }
    }

    if (lcd.dma_nodes != NULL) {
        heap_caps_free(lcd.dma_nodes);
        lcd.dma_nodes = NULL;
    }
}

/**
 * Initialize the LCD peripheral itself and install interrupts.
 */
static esp_err_t init_lcd_peripheral() {
    esp_err_t ret = ESP_OK;

    // enable APB to access LCD registers
    periph_module_enable(PERIPH_LCD_CAM_MODULE);
    periph_module_reset(PERIPH_LCD_CAM_MODULE);

    lcd_hal_init(&lcd.hal, 0);
    lcd_ll_enable_clock(lcd.hal.dev, true);
    lcd_ll_select_clk_src(lcd.hal.dev, LCD_CLK_SRC_PLL240M);
    ESP_RETURN_ON_ERROR(ret, TAG, "set source clock failed");

    // install interrupt service, (LCD peripheral shares the interrupt source with Camera by
    // different mask)
    int flags = ESP_INTR_FLAG_IRAM | ESP_INTR_FLAG_INTRDISABLED | ESP_INTR_FLAG_SHARED
                | ESP_INTR_FLAG_LOWMED;

    int source = LCD_PERIPH_SIGNALS.panels[0].irq_id;
    uint32_t status = (uint32_t)lcd_ll_get_interrupt_status_reg(lcd.hal.dev);
    ret = esp_intr_alloc_intrstatus(
        source, flags, status, LCD_LL_EVENT_VSYNC_END, lcd_isr_vsync, NULL, &lcd.vsync_intr
    );
    ESP_RETURN_ON_ERROR(ret, TAG, "install interrupt failed");

    status = (uint32_t)lcd_ll_get_interrupt_status_reg(lcd.hal.dev);
    ret = esp_intr_alloc_intrstatus(
        source, flags, status, LCD_LL_EVENT_TRANS_DONE, lcd_isr_vsync, NULL, &lcd.done_intr
    );
    ESP_RETURN_ON_ERROR(ret, TAG, "install interrupt failed");

    lcd_ll_fifo_reset(lcd.hal.dev);
    lcd_ll_reset(lcd.hal.dev);

    // pixel clock phase and polarity
    lcd_ll_set_clock_idle_level(lcd.hal.dev, false);
    lcd_ll_set_pixel_clock_edge(lcd.hal.dev, false);

    // enable RGB mode and set data width
    lcd_ll_enable_rgb_mode(lcd.hal.dev, true);
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 3, 0)
    lcd_ll_set_dma_read_stride(lcd.hal.dev, lcd.config.bus_width);
    lcd_ll_set_data_wire_width(lcd.hal.dev, lcd.config.bus_width);
#else
    lcd_ll_set_data_width(lcd.hal.dev, lcd.config.bus_width);
#endif
    lcd_ll_set_phase_cycles(lcd.hal.dev, 0, (lcd.dummy_bytes > 0), 1);  // enable data phase only
    lcd_ll_enable_output_hsync_in_porch_region(lcd.hal.dev, false);     // enable data phase only

    // number of data cycles is controlled by DMA buffer size
    lcd_ll_enable_output_always_on(lcd.hal.dev, false);
    lcd_ll_set_idle_level(lcd.hal.dev, false, true, true);

    // configure blank region timing
    // RGB panel always has a front and back blank (porch region)
    lcd_ll_set_blank_cycles(lcd.hal.dev, 1, 1);

    // output hsync even in porch region?
    lcd_ll_enable_output_hsync_in_porch_region(lcd.hal.dev, false);
    // send next frame automatically in stream mode
    lcd_ll_enable_auto_next_frame(lcd.hal.dev, false);

    lcd_ll_enable_interrupt(lcd.hal.dev, LCD_LL_EVENT_VSYNC_END, true);
    lcd_ll_enable_interrupt(lcd.hal.dev, LCD_LL_EVENT_TRANS_DONE, true);

    // enable intr
    esp_intr_enable(lcd.vsync_intr);
    esp_intr_enable(lcd.done_intr);
    return ret;
}

static void deinit_lcd_peripheral() {
    // disable and free interrupts
    esp_intr_disable(lcd.vsync_intr);
    esp_intr_disable(lcd.done_intr);
    esp_intr_free(lcd.vsync_intr);
    esp_intr_free(lcd.done_intr);

    lcd_ll_fifo_reset(lcd.hal.dev);
    lcd_ll_reset(lcd.hal.dev);

    periph_module_reset(PERIPH_LCD_CAM_MODULE);
    periph_module_disable(PERIPH_LCD_CAM_MODULE);
}

/**
 * Configure the LCD driver for epdiy.
 */
void epd_lcd_init(const LcdEpdConfig_t* config, int display_width, int display_height) {
    esp_err_t ret = ESP_OK;
    assign_lcd_parameters_from_config(config, display_width, display_height);

    check_cache_configuration();

    ret = allocate_lcd_buffers();
    ESP_GOTO_ON_ERROR(ret, err, TAG, "lcd buffer allocation failed");

    ret = init_lcd_peripheral();
    ESP_GOTO_ON_ERROR(ret, err, TAG, "lcd peripheral init failed");

    ret = init_dma_trans_link();
    ESP_GOTO_ON_ERROR(ret, err, TAG, "install DMA failed");

    ret = init_bus_gpio();
    ESP_GOTO_ON_ERROR(ret, err, TAG, "configure GPIO failed");

    init_ckv_rmt();

    // setup driver state
    epd_lcd_set_pixel_clock_MHz(lcd.config.pixel_clock / 1000 / 1000);
    epd_lcd_line_source_cb(NULL, NULL);

    ESP_LOGI(TAG, "LCD init done.");
    return;
err:
    ESP_LOGE(TAG, "LCD initialization failed!");
    abort();
}

/**
 * Deinitializue the LCD driver, i.e., free resources and peripherals.
 */
void epd_lcd_deinit() {
    epd_lcd_line_source_cb(NULL, NULL);

    deinit_bus_gpio();
    deinit_lcd_peripheral();
    deinit_dma_trans_link();
    free_lcd_buffers();
    deinit_ckv_rmt();

    ESP_LOGI(TAG, "LCD deinitialized.");
}

void epd_lcd_set_pixel_clock_MHz(int frequency) {
    lcd.config.pixel_clock = frequency * 1000 * 1000;

    // set pclk
    int flags = 0;

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 3, 0)
    hal_utils_clk_div_t clk_div = {};
/**
 * There was a change in the parameters of this function in this commit:
 * https://github.com/espressif/esp-idf/commit/d39388fe4f4c5bfb0b52df9177307b1688f41016#diff-2df607d77e3f6e350bab8eb31cfd914500ae42744564e1640cec47006cc17a9c
 * There are different builds with the same IDF minor version, some with, some without the commit.
 * So we try to select the correct one by checking if the flag value is defined.
 */
#ifdef LCD_HAL_PCLK_FLAG_ALLOW_EQUAL_SYSCLK
    uint32_t freq
        = lcd_hal_cal_pclk_freq(&lcd.hal, 240000000, lcd.config.pixel_clock, flags, &clk_div);
#else
    uint32_t freq = lcd_hal_cal_pclk_freq(&lcd.hal, 240000000, lcd.config.pixel_clock, &clk_div);
#endif
    lcd_ll_set_group_clock_coeff(
        &LCD_CAM, (int)clk_div.integer, (int)clk_div.denominator, (int)clk_div.numerator
    );
#else
    uint32_t freq = lcd_hal_cal_pclk_freq(&lcd.hal, 240000000, lcd.config.pixel_clock, flags);
#endif

    ESP_LOGI(TAG, "pclk freq: %d Hz", freq);
    lcd.line_length_us = (lcd.lcd_res_h + lcd.config.le_high_time + lcd.config.line_front_porch - 1)
                             * 1000000 / lcd.config.pixel_clock
                         + 1;
    lcd.line_cycles = lcd.line_length_us * lcd.config.pixel_clock / 1000000;
    ESP_LOGI(TAG, "line width: %dus, %d cylces", lcd.line_length_us, lcd.line_cycles);

    ckv_rmt_build_signal();
}

void IRAM_ATTR epd_lcd_start_frame() {
    int initial_lines = min(LINE_BATCH, lcd.display_lines);

    // hsync: pulse with, back porch, active width, front porch
    int end_line
        = lcd.line_cycles - lcd.lcd_res_h - lcd.config.le_high_time - lcd.config.line_front_porch;
    lcd_ll_set_horizontal_timing(
        lcd.hal.dev,
        lcd.config.le_high_time - (lcd.dummy_bytes > 0),
        lcd.config.line_front_porch,
        // a dummy byte is neeed in 8 bit mode to work around LCD peculiarities
        lcd.lcd_res_h + (lcd.dummy_bytes > 0),
        end_line
    );
    lcd_ll_set_vertical_timing(lcd.hal.dev, 1, 0, initial_lines, 1);

    // generate the hsync at the very beginning of line
    lcd_ll_set_hsync_position(lcd.hal.dev, 1);

    // reset FIFO of DMA and LCD, incase there remains old frame data
    gdma_reset(lcd.dma_chan);
    lcd_ll_stop(lcd.hal.dev);
    lcd_ll_fifo_reset(lcd.hal.dev);
    lcd_ll_enable_auto_next_frame(lcd.hal.dev, true);

    lcd.batches = 0;
    fill_bounce_buffer(lcd.bounce_buffer[0]);
    fill_bounce_buffer(lcd.bounce_buffer[1]);

    // the start of DMA should be prior to the start of LCD engine
    gdma_start(lcd.dma_chan, (intptr_t)&lcd.dma_nodes[0]);

    // enter a critical section to ensure the frame start timing is correct
    taskENTER_CRITICAL(&frame_start_spinlock);

    // delay 1us is sufficient for DMA to pass data to LCD FIFO
    // in fact, this is only needed when LCD pixel clock is set too high
    gpio_set_level(lcd.config.bus.stv, 0);
    // esp_rom_delay_us(1);
    //  for picture clarity, it seems to be important to start CKV at a "good"
    //  time, seemingly start or towards end of line.
    start_ckv_cycles(initial_lines + 5);
    esp_rom_delay_us(lcd.line_length_us);
    gpio_set_level(lcd.config.bus.stv, 1);
    esp_rom_delay_us(lcd.line_length_us);
    esp_rom_delay_us(lcd.config.ckv_high_time / 10);

    // start LCD engine
    lcd_ll_start(lcd.hal.dev);

    taskEXIT_CRITICAL(&frame_start_spinlock);
}

#else

/// Dummy implementation to link on the old ESP32
void epd_lcd_init(const LcdEpdConfig_t* config, int display_width, int display_height) {
    assert(false);
}

#endif  // S3 Target
