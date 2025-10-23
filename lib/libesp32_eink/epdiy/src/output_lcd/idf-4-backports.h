/**
 * Backported functions to make the LCD-based driver compile with IDF < 5.0
 */

#define RMT_BASECLK_DEFAULT RMT_BASECLK_APB
typedef int rmt_clock_source_t;

static inline void rmt_ll_enable_periph_clock(rmt_dev_t* dev, bool enable) {
    dev->sys_conf.clk_en = enable;            // register clock gating
    dev->sys_conf.mem_clk_force_on = enable;  // memory clock gating
}

static inline void rmt_ll_enable_mem_access_nonfifo(rmt_dev_t* dev, bool enable) {
    dev->sys_conf.apb_fifo_mask = enable;
}

__attribute__((always_inline)) static inline void rmt_ll_tx_fix_idle_level(
    rmt_dev_t* dev, uint32_t channel, uint8_t level, bool enable
) {
    dev->chnconf0[channel].idle_out_en_n = enable;
    dev->chnconf0[channel].idle_out_lv_n = level;
}
