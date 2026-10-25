// ======================================================
// panel/uDisplay_rgb_panel.cpp - RGB Panel Implementation
// ======================================================
#include "uDisplay_RGB_panel.h"

#if SOC_LCD_RGB_SUPPORTED

#include <cstdint>
#include <algorithm>
#include <rom/cache.h>
#include "esp_cpu.h"
#include "esp_attr.h"
#include <string.h>
extern void AddLog(uint32_t loglevel, const char* formatP, ...);

// ======================================================
// Sync guard - recover permanent shifts of the image
//
// The frame buffer is in PSRAM, which shares the SPI bus with the flash. A flash write
// (settings, file system), or heavy PSRAM traffic, can starve the LCD DMA longer than the
// 16-pixel LCD FIFO: the LCD keeps clocking, the DMA falls behind by N pixels and the
// image stays shifted right/down until the DMA is restarted. In plain PSRAM mode (no
// bounce buffer) the IDF driver never restarts by itself, unless
// CONFIG_LCD_RGB_RESTART_IN_VSYNC is set, which restarts every frame and causes visible
// jitter when the VSYNC interrupt is serviced after the back porch.
//
// Instead, the guard detects the shift and restarts only when needed:
// - LCD pixel clock and CPU clock derive from the same PLL: the frame period is
//   calibrated once in CPU cycles, then the latency-free time of each VSYNC is estimated
//   (interrupt latency only delays, so the earliest observations are the true ones).
// - The GDMA EOF is paced by the LCD consuming pixels: its phase relative to VSYNC moves
//   by exactly the DMA lag, and the synced phase is a hardware constant (identical after
//   every restart). A reference phase is taken once (earliest phase over RGBG_REF_FRAMES
//   frames, interrupt latency only delays). The image is shifted when the phase stays
//   beyond a threshold for a number of consecutive frames: the larger the shift, the
//   fewer frames are needed (RGBG_LEVELS levels, e.g. 6 us x 4 frames, 2 us x 12 frames,
//   1 us x 32 frames with pclk >= 36 MHz; the levels in use are logged at start).
//   A small flash write lags the DMA by ~1-3 us only.
// - The IDF driver configures the GDMA EOF when the last data is read from memory: that
//   EOF phase moves by up to ~30 pixels without any shift of the image (the GDMA FIFO fill
//   level varies). On ESP32-S3 the guard switches the GDMA to EOF when the last data is
//   popped to the LCD (GDMA_OUT_EOF_MODE): with the LCD FIFO full, this is paced by the
//   pixel clock. The EOF interrupt latency still moves the phase by ~10-20 px with the
//   load (LVGL, WiFi, web server), from one second to the next: a 1 px shift cannot be
//   told from it (measured, 12 MHz panel). The smallest threshold is set in pixels
//   (UDISPLAY_RGB_GUARD_FINE_THR_PX) and scales with the pixel clock.
//   -DUDISPLAY_RGB_GUARD_EOF_READ keeps the IDF mode.
// - The reference is lowered when a whole window is earlier (it was noisy), and adopted
//   from the measurement when restarts repeatedly do not change the phase (it was taken
//   in a shifted state).
// - The restart is requested from the VSYNC callback, which runs in the driver ISR just
//   before the driver checks for a restart, and only if this VSYNC is serviced early
//   enough in the vertical blanking. A late restart would itself shift the image.
// - A long flash operation (e.g. uploading a large file) can also stop the DMA entirely:
//   the screen stays black and no EOF comes anymore. RGBG_STALL_FRAMES VSYNC serviced on
//   time without any EOF in between are handled as a stall and trigger a restart too.
// - On ESP32-S3 the driver restarts the DMA 17 pixels after the frame start, because the
//   LCD FIFO keeps 16+1 pixels across the restart (IDF hardware workaround). This is exact
//   only if the FIFO is full, i.e. the DMA was running. After a stall the FIFO is empty
//   and the restarted image is shifted up to 17 pixels to the left: such a restart is
//   followed by a second one as soon as the DMA delivers frames again.
// - The same applies, by one or a few pixels, to any restart done while the FIFO was not
//   completely full, e.g. while the DMA was lagging. Such a small left shift is far below
//   what the EOF phase can detect, so every restart triggered by a detected shift is
//   confirmed by a second restart after RGBG_CONFIRM_FRAMES frames with the DMA running.
//   A followup restart is done with the DMA running after a full frame: it needs none.
//   Confirmations are not logged.
// - The IDF restart (ESP32-S3) resets the LCD async FIFO, then the GDMA, then starts it.
//   The LCD FIFO reset crosses into the pixel clock domain and races with the GDMA restart:
//   during LVGL animation 1 restart in 3 to 10 left the GDMA dead (no EOF anymore, a visible
//   flash), and some landed 1 px right. Measured with forced restarts (12 MHz panel):
//   IDF sequence ~3 dead in 8; the same register writes back to back nearly always dead;
//   GDMA reset then LCD FIFO reset: never dead, but the FIFO is emptied (left shift);
//   GDMA reset only: never dead, no shift. So the guard restarts the DMA itself, without
//   the LCD FIFO reset: GDMA reset and start at the IDF restart link (found from the GDMA
//   link register after the first IDF restart), interrupts masked. The IDF restart is
//   kept for the first restart and for a second stall in a row (the LCD FIFO may need it).
//   -DUDISPLAY_RGB_GUARD_IDF_RESTART always uses the IDF restart.
// - The confirm restart is not urgent: it waits until neither LVGL rendering
//   (udisp_rgb_guard_busy(), from the LVGL render events) nor a frame buffer write
//   happened for RGBG_QUIET_FRAMES frames, at most UDISPLAY_RGB_GUARD_DEFER_MAX_S seconds.
//   Without this wait, a few-pixel shift after a file delete was once left uncorrected
//   (measured with the IDF restart, kept). Shift, stall and followup restarts are not
//   delayed, the image is already wrong.
// - A restart can still leave the GDMA dead: no EOF anymore, GDMA FIFOs L2/L3 empty
//   (ESP32-S3), e.g. with the IDF restart still used for the first restart and a repeated
//   stall. A restart of the dead DMA works, so a restart done with the DMA running is
//   checked at the next VSYNC: no EOF since and GDMA FIFOs drained = restart again at once
//   (stall, "1 frames after"), instead of after RGBG_STALL_FRAMES frames.
// - There is no periodic safety restart: the guard restarts only when a shift or a stall
//   is detected.
// - All timings are in CPU cycles: a CPU frequency change at runtime (command CpuFrequency)
//   pauses the guard at the next VSYNC, then udisp_rgb_guard_poll() recalibrates it.
//
// - Diagnostics: each restart is reported with its cause, the detected shift and the
//   phase measured after the restart (earliest over RGBG_POST_FRAMES frames). A real shift
//   comes back close to 0 after the restart; a detection caused by sustained interrupt
//   latency keeps about the same value, the restart was useless. A stall also reports the
//   cause of the previous restart and the frames since, to tell a stall caused by a restart
//   from an external one.
//
// Disable with -DUDISPLAY_RGB_NO_GUARD. Not needed with CONFIG_LCD_RGB_RESTART_IN_VSYNC.
// Off with CONFIG_FREERTOS_CHECK_PORT_CRITICAL_COMPLIANCE: esp_lcd_rgb_panel_restart() takes
// a task-level critical section, which this check aborts on when called from the VSYNC ISR.
// ======================================================
#if !defined(CONFIG_LCD_RGB_RESTART_IN_VSYNC) && !defined(UDISPLAY_RGB_NO_GUARD) && !defined(CONFIG_FREERTOS_CHECK_PORT_CRITICAL_COMPLIANCE)
#define UDISPLAY_RGB_GUARD
#endif

#ifdef UDISPLAY_RGB_GUARD
#include "esp_private/cache_utils.h"        // spi_flash_cache_enabled()
#include "esp_rom_sys.h"                    // esp_rom_get_cpu_ticks_per_us(), in ROM: safe in ISR with cache disabled
#include "freertos/FreeRTOS.h"              // portSET_INTERRUPT_MASK_FROM_ISR()
#if CONFIG_IDF_TARGET_ESP32S3
#include "soc/gdma_reg.h"                   // GDMA FIFO status, to confirm a dead DMA after a restart
#include "soc/gdma_channel.h"
#define RGBG_GDMA_STRIDE    (GDMA_OUT_CONF0_CH1_REG - GDMA_OUT_CONF0_CH0_REG)
#ifndef UDISPLAY_RGB_GUARD_IDF_RESTART
#define RGBG_OWN_RESTART                    // GDMA restart without the LCD FIFO reset, see above
#endif
#endif

#ifndef UDISPLAY_RGB_GUARD_THR_US
#define UDISPLAY_RGB_GUARD_THR_US     6     // EOF phase shift (us) considered as a real shift
#endif
#ifndef UDISPLAY_RGB_GUARD_FINE_THR_PX
#define UDISPLAY_RGB_GUARD_FINE_THR_PX 36   // smallest EOF phase shift (pixels) detected, over 32 frames, above the FIFO uncertainty
#endif
#ifndef UDISPLAY_RGB_GUARD_FINE_THR_NS
#define UDISPLAY_RGB_GUARD_FINE_THR_NS 1000 // floor of the smallest EOF phase shift (ns), the largest of both is used
#endif
#ifndef UDISPLAY_RGB_GUARD_DEFER_MAX_S
#define UDISPLAY_RGB_GUARD_DEFER_MAX_S 10   // longest wait for a quiet frame buffer before a non-urgent restart
#endif
#define RGBG_SKIP_FRAMES    8       // VSYNC ignored after panel start
#define RGBG_CAL_FRAMES     128     // calibration span in frames
#define RGBG_CAL_WIN        16      // window at both ends of the calibration span
#define RGBG_LEVELS         3       // detection levels: threshold x consecutive frames
#define RGBG_REF_FRAMES     32      // EOF frames to build the reference, and to refine it
#define RGBG_INEFF_FRAMES   96      // a new detection this soon after a restart, with the same lag: restart was ineffective
#define RGBG_REF_RETRY      2       // ineffective restarts before adopting the measured phase as reference
static const DRAM_ATTR uint8_t rgbg_level_frames[RGBG_LEVELS] = { 4, 12, 32 };    // read in ISR, keep in internal RAM
#define RGBG_RESTART_SKIP   2       // EOF ignored after a restart
#define RGBG_DELTA_US       1       // tolerated drift of the VSYNC estimate per frame
#define RGBG_GATE_MIN_US    10      // minimum restart gate
#define RGBG_STALL_FRAMES   4       // consecutive frames without EOF considered as a DMA stall
#define RGBG_POST_FRAMES    8       // EOF frames measured after a restart, for diagnostics
#define RGBG_QUIET_FRAMES   2       // frames without frame buffer write before a non-urgent restart
#define RGBG_CONFIRM_FRAMES 12      // frames with the DMA running before confirming a corrective restart,
                                    // more than RGBG_RESTART_SKIP + RGBG_POST_FRAMES to keep the diagnostics

// cause of a restart, for diagnostics
enum {
    RGBG_CAUSE_NONE = 0,
    RGBG_CAUSE_INIT,        // initial restart after calibration
    RGBG_CAUSE_LEVEL,       // shift detected, + level (0 .. RGBG_LEVELS-1)
    RGBG_CAUSE_EARLY = RGBG_CAUSE_LEVEL + RGBG_LEVELS,   // EOF earlier than the reference
    RGBG_CAUSE_STALL,       // no EOF, DMA stalled
    RGBG_CAUSE_FOLLOWUP,    // second restart after a restart done while stalled
    RGBG_CAUSE_CONFIRM,     // second restart after a corrective one, removes a small left shift
};

// last restart, for diagnostics. Written in ISR, read by udisp_rgb_guard_poll()
typedef struct {
    uint32_t restarts;      // value of rgbg.restarts for this report
    uint32_t cause;
    int32_t  lag;           // detected shift in CPU cycles, for RGBG_CAUSE_LEVEL and RGBG_CAUSE_EARLY
                            // frames since the previous restart, for RGBG_CAUSE_STALL
    uint32_t prev_cause;    // cause of the previous restart (any, including confirm)
    int32_t  post;          // earliest phase after restart, relative to the reference, in CPU cycles
    uint32_t post_valid;    // 0 if not measured (no reference yet, or superseded by another restart)
} rgbg_report_t;

typedef struct {
    // configuration
    uint32_t active;
    uint32_t period_nom;        // nominal frame period in CPU cycles, from timings
    uint32_t period;            // calibrated frame period in CPU cycles, 0 while calibrating
    uint32_t delta;             // in CPU cycles
    uint32_t gate;              // max VSYNC lateness allowing a restart, in CPU cycles
    uint32_t thr;               // EOF phase threshold, in CPU cycles
    // calibration
    uint32_t vsync_count;
    uint32_t cal_prev;
    int32_t  cal_v;             // offset of current VSYNC from the nominal grid
    int32_t  cal_min[2];        // min offset at both ends of the span
    uint32_t cal_idx[2];
    // VSYNC tracking
    uint32_t est;               // estimated latency-free time of the last VSYNC
    int32_t  late;              // lateness of the last VSYNC interrupt, in CPU cycles
    // EOF phase
    uint32_t level_thr[RGBG_LEVELS];    // in CPU cycles, decreasing
    uint32_t skip;
    uint32_t ref;               // synced EOF phase after VSYNC, in CPU cycles (hardware constant)
    uint32_t ref_valid;
    uint32_t ref_count;
    uint32_t rwin_count;        // window to refine the reference
    int32_t  rwin_min;
    uint32_t run[RGBG_LEVELS];  // consecutive frames beyond the level threshold
    int32_t  run_min[RGBG_LEVELS];
    uint32_t run_neg;           // consecutive frames earlier than the reference
    int32_t  run_neg_max;
    uint32_t eof_since_restart;
    int32_t  prev_lag;
    uint32_t prev_valid;
    uint32_t ineff;             // consecutive ineffective restarts
    uint32_t followup;          // restart again as soon as the DMA runs (last restart was done while stalled)
    uint32_t confirm;           // frames with the DMA running left before the confirmation restart, 0 = none
    uint32_t confirms;          // statistics
    // non-urgent restart, waiting for a quiet frame buffer
    volatile uint32_t soft_cause;   // RGBG_CAUSE_xxx, 0 = none
    uint32_t soft_wait;         // frames waited
    uint32_t defer_max;         // max frames to wait, from UDISPLAY_RGB_GUARD_DEFER_MAX_S
    uint32_t busy_deferred;     // statistics: non-urgent restarts that had to wait
    volatile uint32_t fb_busy;          // frame buffer being written by the CPU
    volatile uint32_t lv_busy;          // LVGL rendering in progress
    volatile uint32_t fb_write_vsync;   // vsync_count at the end of the last frame buffer write or LVGL render
    uint32_t eof_seen;          // EOF received since last VSYNC
    uint32_t eof_missing_run;   // consecutive on-time frames without EOF
    // restart
    volatile uint32_t req;
    uint32_t req_cause;         // RGBG_CAUSE_xxx of the pending request
    int32_t  req_lag;
    uint32_t since_restart;     // frames
    // diagnostics of the last restart
    uint32_t cur_cause;
    int32_t  cur_lag;
    uint32_t cur_prev;          // cause of the restart before cur_cause
    uint32_t last_cause;        // cause of the last restart done, including confirm
    uint32_t last_running;      // the DMA was running at the last restart: check that it runs again
    uint32_t ran_since_restart; // EOF received since the last restart
    uint32_t post_count;        // EOF measured after the restart, RGBG_POST_FRAMES = done or none pending
    int32_t  post_min;
    // statistics
    uint32_t detected;          // shifts detected
    uint32_t stalls;            // DMA stalls detected (no EOF)
    uint32_t fine;              // small shifts detected (levels > 0, or earlier than reference)
    uint32_t adopted;           // reference adopted from measurement
    uint32_t restarts;          // restarts done
} rgbg_t;

// accessed from ISRs that may run with cache disabled: must be in internal RAM
static DRAM_ATTR rgbg_t rgbg;
static DRAM_ATTR rgbg_report_t rgbg_report;

// request a restart at the next VSYNC serviced on time; the first cause is kept
static IRAM_ATTR void rgbg_request(uint32_t cause, int32_t lag) {
    if (!rgbg.req) {
        rgbg.req_cause = cause;
        rgbg.req_lag = lag;
        rgbg.req = 1;
    }
}

// request a non-urgent restart, done when the frame buffer is quiet; the first cause is kept
static IRAM_ATTR void rgbg_request_soft(uint32_t cause) {
    if (!rgbg.soft_cause) {
        rgbg.soft_wait = 0;
        rgbg.soft_cause = cause;
    }
}

// frame buffer writes by the CPU (task context)
static inline void rgbg_fb_begin(void) { rgbg.fb_busy = 1; }
static inline void rgbg_fb_end(void) { rgbg.fb_write_vsync = rgbg.vsync_count; rgbg.fb_busy = 0; }

// publish the diagnostics of the last restart
static IRAM_ATTR void rgbg_report_done(uint32_t post_valid) {
    rgbg_report.cause = rgbg.cur_cause;
    rgbg_report.lag = rgbg.cur_lag;
    rgbg_report.prev_cause = rgbg.cur_prev;
    rgbg_report.post = rgbg.post_min;
    rgbg_report.post_valid = post_valid;
    rgbg_report.restarts = rgbg.restarts;      // last, the poll checks this one
    rgbg.post_count = RGBG_POST_FRAMES;
}

static esp_lcd_panel_handle_t rgbg_panel = nullptr;

#ifdef RGBG_OWN_RESTART
static DRAM_ATTR uint8_t *rgbg_fb = nullptr;        // frame buffer, set by RGBPanel
static DRAM_ATTR uint32_t rgbg_restart_link = 0;    // low 20 bits of the IDF restart link address, 0 = not found yet
static DRAM_ATTR uint32_t rgbg_link_tried = 0;
#endif
static DRAM_ATTR int32_t rgbg_dma_ch = -1;     // GDMA channel of the LCD (ESP32-S3), -1 if unknown
static uint32_t rgbg_cpu_mhz = 240;
static uint32_t rgbg_pclk_hz = 0;
static esp_lcd_rgb_timing_t rgbg_timings = {};          // panel timings, kept to recalibrate after a CPU frequency change
static DRAM_ATTR uint32_t rgbg_rom_mhz = 0;             // CPU MHz in ROM when the timings were computed, checked in ISR
static DRAM_ATTR volatile uint32_t rgbg_cpu_changed = 0;    // guard paused by a CPU frequency change, recalibrate
static uint32_t rgbg_poll_restarts = 0;

// a - b with a, b in [0, period), result in (-period/2, period/2]
static IRAM_ATTR int32_t rgbg_moddiff(uint32_t a, uint32_t b) {
    int32_t d = (int32_t)(a - b);
    int32_t half = rgbg.period / 2;
    if (d > half) { d -= rgbg.period; }
    else if (d <= -half) { d += rgbg.period; }
    return d;
}

static IRAM_ATTR void rgbg_calibrate(uint32_t now) {
    if (rgbg.vsync_count < RGBG_SKIP_FRAMES) { return; }
    uint32_t i = rgbg.vsync_count - RGBG_SKIP_FRAMES;
    if (0 == i) {
        rgbg.cal_v = 0;
    } else {
        int32_t dev = (int32_t)(now - rgbg.cal_prev - rgbg.period_nom);
        int32_t limit = rgbg.period_nom / 4;
        if (dev > limit || dev < -limit) {      // missed or spurious VSYNC: start over
            rgbg.vsync_count = RGBG_SKIP_FRAMES;
            rgbg.cal_v = 0;
            i = 0;
        } else {
            rgbg.cal_v += dev;
        }
    }
    rgbg.cal_prev = now;
    // latency only adds: the minimum offset in each window is the most accurate sample
    uint32_t w = (i < RGBG_CAL_WIN) ? 0 : 1;
    uint32_t w_start = w ? RGBG_CAL_FRAMES : 0;
    if (i >= w_start && i < w_start + RGBG_CAL_WIN) {
        if (i == w_start || rgbg.cal_v < rgbg.cal_min[w]) {
            rgbg.cal_min[w] = rgbg.cal_v;
            rgbg.cal_idx[w] = i;
        }
    }
    if (i == RGBG_CAL_FRAMES + RGBG_CAL_WIN - 1) {
        int32_t corr = (rgbg.cal_min[1] - rgbg.cal_min[0]) / (int32_t)(rgbg.cal_idx[1] - rgbg.cal_idx[0]);
        int32_t limit = rgbg.period_nom / 64;
        if (corr < limit && corr > -limit) {
            rgbg.period = rgbg.period_nom + corr;
            rgbg.est = now;
            rgbg.late = 0;
            rgbg_request(RGBG_CAUSE_INIT, 0);   // initial restart, so that the baseline is taken from a known synced state
        } else {
            rgbg.vsync_count = RGBG_SKIP_FRAMES - 1;    // start over
        }
    }
}

// update the latency-free VSYNC estimate and the lateness of this interrupt
static IRAM_ATTR void rgbg_track(uint32_t now) {
    int32_t half = rgbg.period / 2;
    uint32_t pred = rgbg.est + rgbg.period;
    int32_t late = (int32_t)(now - pred);
    for (uint32_t n = 0; late > half && n < 16; n++) {     // VSYNC interrupts missed, e.g. cache disabled
        pred += rgbg.period;
        late -= rgbg.period;
    }
    if (late > half || late < -half) {         // lost track, resync on this interrupt
        rgbg.est = now;
        rgbg.late = INT32_MAX;                  // unknown, no restart on this frame
        return;
    }
    // an interrupt earlier than predicted, or close to it, gives the new reference;
    // otherwise follow the prediction, allowing a small drift to catch calibration error
    rgbg.est = (late < (int32_t)rgbg.delta) ? now : pred + rgbg.delta;
    rgbg.late = late;
}

static IRAM_ATTR void rgbg_runs_clear(void) {
    for (uint32_t i = 0; i < RGBG_LEVELS; i++) { rgbg.run[i] = 0; }
    rgbg.run_neg = 0;
}

// true if the GDMA out FIFOs L2 and L3 are empty. In the blanking a running DMA has them
// filled with the start of the next frame; a dead one has them empty (ESP32-S3, measured).
// Without the GDMA status (other targets, channel unknown), rely on the missing EOF only.
static IRAM_ATTR bool rgbg_gdma_drained(void) {
#if CONFIG_IDF_TARGET_ESP32S3
    if (rgbg_dma_ch >= 0) {
        uint32_t fifo = REG_READ(GDMA_OUTFIFO_STATUS_CH0_REG + rgbg_dma_ch * RGBG_GDMA_STRIDE);
        const uint32_t empty = GDMA_OUTFIFO_EMPTY_L2_CH0_M | GDMA_OUTFIFO_EMPTY_L3_CH0_M;
        return (fifo & empty) == empty;
    }
#endif
    return true;
}

// ESP32-S3: GDMA EOF when the last data is popped to the LCD, not when read from memory.
// Checked at every VSYNC, in case a DMA reset clears it; the EOFs right after a restart are skipped anyway.
static IRAM_ATTR void rgbg_eof_mode_popped(void) {
#if CONFIG_IDF_TARGET_ESP32S3 && !defined(UDISPLAY_RGB_GUARD_EOF_READ)
    if (rgbg_dma_ch >= 0) {
        uint32_t reg = GDMA_OUT_CONF0_CH0_REG + rgbg_dma_ch * RGBG_GDMA_STRIDE;
        if (!(REG_READ(reg) & GDMA_OUT_EOF_MODE_CH0)) { REG_SET_BIT(reg, GDMA_OUT_EOF_MODE_CH0); }
    }
#endif
}

#ifdef RGBG_OWN_RESTART
// once, after the first IDF restart: the GDMA link register still points to the IDF restart link,
// a single item from fb + 17 px (16 bpp), chained to the frame link
static IRAM_ATTR void rgbg_find_restart_link(void) {
    rgbg_link_tried = 1;
    if (rgbg_dma_ch < 0 || !rgbg_fb) { return; }
    uint32_t low = REG_READ(GDMA_OUT_LINK_CH0_REG + rgbg_dma_ch * RGBG_GDMA_STRIDE) & GDMA_OUTLINK_ADDR_CH0_V;
    uint32_t addr = 0x3FC00000 | low;
    if (addr < SOC_DRAM_LOW || addr + 12 > SOC_DRAM_HIGH || (addr & 3)) { return; }
    const uint32_t *lli = (const uint32_t *)addr;     // dw0, buffer, next
    if (lli[1] != (uint32_t)(rgbg_fb + 34) || 0 == lli[2]) { return; }
    rgbg_restart_link = low;
}

// own restart: GDMA reset and start at the IDF restart link, no LCD FIFO reset, interrupts masked
static IRAM_ATTR void rgbg_own_restart(void) {
    uint32_t conf0 = GDMA_OUT_CONF0_CH0_REG + rgbg_dma_ch * RGBG_GDMA_STRIDE;
    uint32_t link = GDMA_OUT_LINK_CH0_REG + rgbg_dma_ch * RGBG_GDMA_STRIDE;
    uint32_t state = portSET_INTERRUPT_MASK_FROM_ISR();
    REG_SET_BIT(conf0, GDMA_OUT_RST_CH0);
    REG_CLR_BIT(conf0, GDMA_OUT_RST_CH0);
    REG_WRITE(link, (REG_READ(link) & ~GDMA_OUTLINK_ADDR_CH0_M) | rgbg_restart_link);
    REG_SET_BIT(link, GDMA_OUTLINK_START_CH0);
    portCLEAR_INTERRUPT_MASK_FROM_ISR(state);
}
#endif

// restart the DMA: the IDF driver does it right after this callback returns, the own sequence at once.
// A second stall in a row uses the IDF restart, with the LCD FIFO reset.
static IRAM_ATTR bool rgbg_restart(uint32_t cause) {
#ifdef RGBG_OWN_RESTART
    bool stalled_again = (RGBG_CAUSE_STALL == cause && RGBG_CAUSE_STALL == rgbg.last_cause);
    if (rgbg_restart_link && !stalled_again) {
        rgbg_own_restart();
        return true;
    }
#endif
    return (ESP_OK == esp_lcd_rgb_panel_restart(rgbg_panel));
}

static IRAM_ATTR void rgbg_on_vsync(uint32_t now) {
    if (!rgbg.active) { return; }
    // CPU frequency changed: every timing in CPU cycles is wrong, pause before any restart
    if (esp_rom_get_cpu_ticks_per_us() != rgbg_rom_mhz) {
        rgbg.active = 0;
        rgbg_cpu_changed = 1;
        return;
    }
#ifdef RGBG_OWN_RESTART
    if (!rgbg_link_tried && rgbg.restarts) { rgbg_find_restart_link(); }
#endif
    rgbg_eof_mode_popped();
    rgbg.vsync_count++;
    if (0 == rgbg.period) {
        rgbg_calibrate(now);
        return;
    }
    rgbg_track(now);
    rgbg.since_restart++;
    bool dma_running = rgbg.eof_seen;   // an EOF in the last frame: the LCD FIFO is full in the blanking
    // DMA stall: count only VSYNC serviced on time, a deferred interrupt (cache disabled)
    // may legitimately coalesce or reorder with the EOF interrupt
    if (rgbg.late < (int32_t)(rgbg.period / 4)) {
        rgbg.eof_missing_run = rgbg.eof_seen ? 0 : rgbg.eof_missing_run + 1;
        // a restart done with the DMA running can leave it dead: check once, at the first
        // VSYNC on time after it, and restart again at once (a restart of a dead DMA works)
        if (rgbg.last_running && rgbg.since_restart <= 2) {
            rgbg.last_running = 0;
            if (!rgbg.ran_since_restart && !rgbg.req && rgbg_gdma_drained()) {
                rgbg.stalls++;
                rgbg.eof_missing_run = 0;
                rgbg_request(RGBG_CAUSE_STALL, rgbg.since_restart);
            }
        }
        if (rgbg.eof_missing_run >= RGBG_STALL_FRAMES) {
            rgbg.eof_missing_run = 0;       // retried after RGBG_STALL_FRAMES if the restart did not help
            if (!rgbg.req) {
                rgbg.stalls++;
                rgbg_request(RGBG_CAUSE_STALL, rgbg.since_restart);    // diagnostics: was it caused by the previous restart?
            }
        }
    }
    rgbg.eof_seen = 0;
    if (rgbg.confirm && dma_running && !rgbg.req && !rgbg.followup) {
        if (0 == --rgbg.confirm) {
            rgbg.confirms++;
            rgbg_request_soft(RGBG_CAUSE_CONFIRM);
        }
    }
    if (rgbg.followup && dma_running && !rgbg.req) {
        rgbg.followup = 0;
        rgbg_request(RGBG_CAUSE_FOLLOWUP, 0);
    }
    // non-urgent restart: wait for a quiet frame buffer, no CPU writes and no LVGL rendering
    if (rgbg.soft_cause && !rgbg.req) {
        bool quiet = !rgbg.fb_busy && !rgbg.lv_busy && (rgbg.vsync_count - rgbg.fb_write_vsync) >= RGBG_QUIET_FRAMES;
        if (quiet || rgbg.soft_wait >= rgbg.defer_max) {
            rgbg_request(rgbg.soft_cause, rgbg.soft_wait);     // the lag field carries the frames waited
            rgbg.soft_cause = 0;
        } else {
            if (0 == rgbg.soft_wait) { rgbg.busy_deferred++; }
            rgbg.soft_wait++;
        }
    }
    if (rgbg.req) {
        // esp_lcd_rgb_panel_restart() is in flash, the ISR may run with cache disabled
        if (rgbg.late < (int32_t)rgbg.gate && spi_flash_cache_enabled()) {
            // own restart at once, or IDF restart flagged: the driver restarts right after this callback returns
            if (rgbg_restart(rgbg.req_cause)) {
                uint32_t cause = rgbg.req_cause;
                rgbg.soft_cause = 0;            // any restart also does the pending non-urgent one
                if (rgbg.post_count < RGBG_POST_FRAMES) { rgbg_report_done(0); }   // previous one not measured yet
                rgbg.restarts++;
                // only a restart triggered by a detection (and its confirmation) can be judged
                // ineffective by the next detection
                bool detection = (cause >= RGBG_CAUSE_LEVEL && cause <= RGBG_CAUSE_EARLY);
                if (!detection && RGBG_CAUSE_CONFIRM != cause) {
                    rgbg.prev_valid = 0;
                    rgbg.ineff = 0;
                }
                // a shift-detection restart may have been done with a lagging, not full FIFO: confirm it later;
                // a followup is done after a full frame with the DMA running, a stall restart is followed up;
                // any other restart serves as confirmation
                rgbg.confirm = detection ? RGBG_CONFIRM_FRAMES : 0;
                uint32_t prev_cause = rgbg.last_cause;
                rgbg.last_cause = cause;
                rgbg.last_running = dma_running;    // checked at the next VSYNC: did the DMA survive the restart?
                rgbg.ran_since_restart = 0;
                if (RGBG_CAUSE_CONFIRM != cause) {     // keep the diagnostics of the corrected restart
                    rgbg.cur_cause = cause;
                    rgbg.cur_lag = rgbg.req_lag;
                    rgbg.cur_prev = prev_cause;
                    rgbg.post_count = 0;
                    if (!rgbg.ref_valid) { rgbg_report_done(0); }   // no reference to measure against
                }
                rgbg.followup = !dma_running;   // FIFO not full: this restart leaves a left shift
            }
            rgbg.req = 0;
            rgbg.since_restart = 0;
            rgbg.skip = RGBG_RESTART_SKIP;
            rgbg_runs_clear();
            rgbg.rwin_count = 0;
            rgbg.eof_since_restart = 0;
            if (!rgbg.ref_valid) { rgbg.ref_count = 0; }
        }
    }
}

// a shift of `lag` was detected at `level` (RGBG_LEVELS = earlier than the reference)
static IRAM_ATTR void rgbg_detected(int32_t lag, uint32_t level) {
    rgbg_runs_clear();
    int32_t diff = lag - rgbg.prev_lag;
    if (diff < 0) { diff = -diff; }
    if (rgbg.prev_valid && rgbg.eof_since_restart < RGBG_INEFF_FRAMES && diff < (int32_t)rgbg.level_thr[RGBG_LEVELS - 1]) {
        // the restart did not change the phase: the reference is wrong, not the image
        if (++rgbg.ineff >= RGBG_REF_RETRY) {
            rgbg.ref = (rgbg.ref + rgbg.period + lag) % rgbg.period;
            rgbg.adopted++;
            rgbg.ineff = 0;
            rgbg.prev_valid = 0;
            rgbg.rwin_count = 0;
            return;
        }
    } else {
        rgbg.ineff = 0;
    }
    rgbg.prev_lag = lag;
    rgbg.prev_valid = 1;
    if (0 == level) { rgbg.detected++; } else { rgbg.fine++; }
    rgbg_request(RGBG_CAUSE_LEVEL + level, lag);
}

static IRAM_ATTR void rgbg_on_eof(uint32_t now) {
    rgbg.eof_seen = 1;
    rgbg.ran_since_restart = 1;
    if (!rgbg.active || 0 == rgbg.period || rgbg.req) { return; }
    if (rgbg.skip) {
        rgbg.skip--;
        return;
    }
    uint32_t phase = (now - rgbg.est) % rgbg.period;
    if (!rgbg.ref_valid) {
        if (rgbg.followup) { return; }      // a left-shifted image must not become the reference
        // latency only delays the EOF interrupt: the reference is the earliest phase
        if (0 == rgbg.ref_count || rgbg_moddiff(phase, rgbg.ref) < 0) { rgbg.ref = phase; }
        if (++rgbg.ref_count >= RGBG_REF_FRAMES) { rgbg.ref_valid = 1; }
        return;
    }
    int32_t d = rgbg_moddiff(phase, rgbg.ref);
    rgbg.eof_since_restart++;

    // diagnostics: earliest phase after the restart, latency only delays
    if (rgbg.post_count < RGBG_POST_FRAMES) {
        if (0 == rgbg.post_count || d < rgbg.post_min) { rgbg.post_min = d; }
        if (++rgbg.post_count >= RGBG_POST_FRAMES) { rgbg_report_done(1); }
    }

    // refine the reference: a whole window slightly earlier means the reference was noisy
    if (!rgbg.followup) {
        if (0 == rgbg.rwin_count || d < rgbg.rwin_min) { rgbg.rwin_min = d; }
        if (++rgbg.rwin_count >= RGBG_REF_FRAMES) {
            rgbg.rwin_count = 0;
            if (rgbg.rwin_min < 0 && rgbg.rwin_min > -(int32_t)rgbg.level_thr[1]) {
                rgbg.ref = (rgbg.ref + rgbg.period + rgbg.rwin_min) % rgbg.period;
                rgbg_runs_clear();
                return;
            }
        }
    }

    // DMA lag: phase later than the reference for enough consecutive frames. A single
    // frame on time resets the run, so interrupt latency spikes do not trigger it.
    for (uint32_t i = 0; i < RGBG_LEVELS; i++) {
        if (d > (int32_t)rgbg.level_thr[i]) {
            if (0 == rgbg.run[i] || d < rgbg.run_min[i]) { rgbg.run_min[i] = d; }
            if (++rgbg.run[i] >= rgbg_level_frames[i]) {
                rgbg_detected(rgbg.run_min[i], i);
                return;
            }
        } else {
            rgbg.run[i] = 0;
        }
    }
    // earlier than the reference by more than the noise: image ahead, or wrong reference
    if (d < -(int32_t)rgbg.level_thr[1]) {
        if (0 == rgbg.run_neg || d > rgbg.run_neg_max) { rgbg.run_neg_max = d; }
        if (++rgbg.run_neg >= rgbg_level_frames[1]) {
            rgbg_detected(rgbg.run_neg_max, RGBG_LEVELS);
        }
    } else {
        rgbg.run_neg = 0;
    }
}

// compute the timings in CPU cycles from rgbg_timings and rgbg_cpu_mhz, and reset the guard
// state (calibration starts over). The guard must be inactive.
static void rgbg_setup(void) {
    const esp_lcd_rgb_timing_t *t = &rgbg_timings;
    uint64_t h_total = t->h_res + t->hsync_pulse_width + t->hsync_back_porch + t->hsync_front_porch;
    uint64_t v_total = t->v_res + t->vsync_pulse_width + t->vsync_back_porch + t->vsync_front_porch;
    memset(&rgbg, 0, sizeof(rgbg));
    memset(&rgbg_report, 0, sizeof(rgbg_report));
    rgbg_poll_restarts = 0;
    rgbg.post_count = RGBG_POST_FRAMES;    // no measurement pending
    rgbg_rom_mhz = esp_rom_get_cpu_ticks_per_us();
    if (0 == t->pclk_hz) { return; }
    uint64_t cpu_hz = (uint64_t)rgbg_cpu_mhz * 1000000ULL;
    rgbg.period_nom = h_total * v_total * cpu_hz / t->pclk_hz;
    uint32_t gate = t->vsync_back_porch * h_total * cpu_hz / t->pclk_hz / 2;
    uint32_t gate_min = RGBG_GATE_MIN_US * rgbg_cpu_mhz;
    rgbg.gate = (gate > gate_min) ? gate : gate_min;
    rgbg_pclk_hz = t->pclk_hz;
    uint32_t fthr = UDISPLAY_RGB_GUARD_FINE_THR_NS * rgbg_cpu_mhz / 1000;
    uint32_t fthr_px = (uint64_t)UDISPLAY_RGB_GUARD_FINE_THR_PX * cpu_hz / t->pclk_hz;
    if (fthr_px > fthr) { fthr = fthr_px; }
    rgbg.thr = UDISPLAY_RGB_GUARD_THR_US * rgbg_cpu_mhz;
    if (rgbg.thr < 2 * fthr) { rgbg.thr = 2 * fthr; }   // keep the levels decreasing, on slow pixel clocks
    rgbg.level_thr[0] = rgbg.thr;
    rgbg.level_thr[1] = 2 * fthr;
    rgbg.level_thr[2] = fthr;
    rgbg.delta = RGBG_DELTA_US * rgbg_cpu_mhz;
    rgbg.defer_max = (uint64_t)UDISPLAY_RGB_GUARD_DEFER_MAX_S * t->pclk_hz / (h_total * v_total);
}

static void rgbg_init(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_config_t *config) {
    rgbg_timings = config->timings;
    rgbg_cpu_mhz = getCpuFrequencyMhz();
    rgbg_cpu_changed = 0;
    rgbg_setup();
    rgbg_panel = panel;
#if CONFIG_IDF_TARGET_ESP32S3
    rgbg_dma_ch = -1;
    for (uint32_t ch = 0; ch < SOC_GDMA_PAIRS_PER_GROUP; ch++) {
        if ((REG_READ(GDMA_OUT_PERI_SEL_CH0_REG + ch * RGBG_GDMA_STRIDE) & GDMA_PERI_OUT_SEL_CH0_V) == SOC_GDMA_TRIG_PERIPH_LCD0) {
            rgbg_dma_ch = ch;
            break;
        }
    }
    rgbg_eof_mode_popped();
#endif
}

// true if a new restart report is available, formatted in buf
static int32_t rgbg_cycles_to_ns(int32_t cycles) {
    return (int32_t)((int64_t)cycles * 1000 / (int32_t)rgbg_cpu_mhz);
}

static int32_t rgbg_cycles_to_px(int32_t cycles) {
    int64_t den = (int64_t)rgbg_cpu_mhz * 1000000;
    int64_t num = (int64_t)cycles * rgbg_pclk_hz;
    return (int32_t)((num + (num >= 0 ? den / 2 : -den / 2)) / den);     // rounded
}

static const char *rgbg_cause_name(uint32_t c) {
    static_assert(RGBG_CAUSE_EARLY == 5 && RGBG_CAUSE_CONFIRM == 8, "update the names below");
    static const char * const names[] = { "?", "init", "shift", "shift", "shift", "early", "stall", "followup", "confirm" };
    return names[(c < sizeof(names) / sizeof(names[0])) ? c : 0];
}

bool udisp_rgb_guard_poll(char *buf, size_t len) {
    // CPU frequency change: the VSYNC ISR pauses the guard at once; also checked here in case
    // the ROM value was not updated, then recalibrated at the next poll, once no ISR uses the old state
    uint32_t mhz = getCpuFrequencyMhz();
    if (rgbg.active && mhz != rgbg_cpu_mhz) {
        rgbg.active = 0;
        rgbg_cpu_changed = 1;
        return false;
    }
    if (rgbg_cpu_changed) {
        AddLog(3, "DSP: RGB sync guard recalibrating, CPU %u -> %u MHz", (unsigned)rgbg_cpu_mhz, (unsigned)mhz);
        rgbg_cpu_changed = 0;
        rgbg_cpu_mhz = mhz;
        rgbg_setup();
        __sync_synchronize();               // state written before the ISRs see the guard active
        rgbg.active = (rgbg.period_nom != 0);
        return false;
    }
    if (!rgbg.active || len < 2) { return false; }
    rgbg_report_t r = rgbg_report;      // snapshot, small tears are acceptable for diagnostics
    if (r.restarts == rgbg_poll_restarts) { return false; }
    rgbg_poll_restarts = r.restarts;
    char cause[32];
    if (r.cause >= RGBG_CAUSE_LEVEL && r.cause < RGBG_CAUSE_LEVEL + RGBG_LEVELS) {
        uint32_t level = r.cause - RGBG_CAUSE_LEVEL;
        snprintf(cause, sizeof(cause), "shift >%upx x%u", (unsigned)rgbg_cycles_to_px(rgbg.level_thr[level]), (unsigned)rgbg_level_frames[level]);
    } else {
        snprintf(cause, sizeof(cause), "%s", rgbg_cause_name(r.cause));
    }
    char lag[48] = "-";
    if (r.cause >= RGBG_CAUSE_LEVEL && r.cause <= RGBG_CAUSE_EARLY) {
        snprintf(lag, sizeof(lag), "shift %i px (%i ns)", (int)rgbg_cycles_to_px(r.lag), (int)rgbg_cycles_to_ns(r.lag));
    } else if (RGBG_CAUSE_STALL == r.cause) {
        snprintf(lag, sizeof(lag), "%u frames after %s restart", (unsigned)r.lag, rgbg_cause_name(r.prev_cause));
    }
    char post[32] = "n/a";
    if (r.post_valid) {
        snprintf(post, sizeof(post), "%i px (%i ns)", (int)rgbg_cycles_to_px(r.post), (int)rgbg_cycles_to_ns(r.post));
    }
    // a real shift comes back close to 0 after the restart, plus the interrupt latency (~10-20 px under load)
#ifdef RGBG_OWN_RESTART
    const char *link = (rgbg_link_tried && !rgbg_restart_link) ? ", IDF restart: link not found" : "";
#else
    const char *link = "";
#endif
    snprintf(buf, len, "#%u cause %s, %s, after restart %s (detected %u, stalls %u, confirms %u, busy deferred %u, ref adopted %u%s)",
             (unsigned)r.restarts, cause, lag, post,
             (unsigned)(rgbg.detected + rgbg.fine), (unsigned)rgbg.stalls, (unsigned)rgbg.confirms, (unsigned)rgbg.busy_deferred, (unsigned)rgbg.adopted,
             link);
    return true;
}
// LVGL rendering started (true) or finished (false): delay non-urgent DMA restarts
void udisp_rgb_guard_busy(bool busy) {
    if (busy) {
        rgbg.lv_busy = 1;
    } else {
        rgbg.fb_write_vsync = rgbg.vsync_count;
        rgbg.lv_busy = 0;
    }
}
#else  // UDISPLAY_RGB_GUARD
bool udisp_rgb_guard_poll(char *buf, size_t len) {
    return false;
}
void udisp_rgb_guard_busy(bool busy) {}
static inline void rgbg_fb_begin(void) {}
static inline void rgbg_fb_end(void) {}
#endif // UDISPLAY_RGB_GUARD

#ifdef UDISPLAY_RGB_GUARD
// driver callbacks of the sync guard
static IRAM_ATTR bool rgb_on_vsync(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *edata, void *user_ctx) {
    rgbg_on_vsync(esp_cpu_get_cycle_count());
    return false;
}

static IRAM_ATTR bool rgb_on_eof(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *edata, void *user_ctx) {
    rgbg_on_eof(esp_cpu_get_cycle_count());
    return false;
}

static void rgb_callbacks_init(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_config_t *config) {
    rgbg_init(panel, config);
    esp_lcd_rgb_panel_event_callbacks_t cbs = {};
    cbs.on_vsync = rgb_on_vsync;
    cbs.on_frame_buf_complete = rgb_on_eof;
    esp_err_t err = esp_lcd_rgb_panel_register_event_callbacks(panel, &cbs, nullptr);
    if (ESP_OK != err) {
        AddLog(1, "DSP: RGB panel callbacks failed (err %d)", (int)err);
        return;
    }
    rgbg.active = (rgbg.period_nom != 0);
#if CONFIG_IDF_TARGET_ESP32S3 && !defined(UDISPLAY_RGB_GUARD_EOF_READ)
    const char *eof_mode = (rgbg_dma_ch >= 0) ? "popped" : "read";
#else
    const char *eof_mode = "read";
#endif
    AddLog(3, "DSP: RGB sync guard %s, gate %u us, thresholds %u/%u/%u px, gdma ch %d, eof %s",
           rgbg.active ? "on" : "off", (unsigned)(rgbg.gate / rgbg_cpu_mhz),
           (unsigned)rgbg_cycles_to_px(rgbg.level_thr[0]), (unsigned)rgbg_cycles_to_px(rgbg.level_thr[1]), (unsigned)rgbg_cycles_to_px(rgbg.level_thr[2]),
           (int)rgbg_dma_ch, eof_mode);
}
#endif // UDISPLAY_RGB_GUARD

RGBPanel::RGBPanel(const esp_lcd_rgb_panel_config_t *config) {
    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(config, &panel_handle));
#ifdef UDISPLAY_RGB_GUARD
    rgb_callbacks_init(panel_handle, config);
#endif
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    width = config->timings.h_res;
    height = config->timings.v_res;
    framebuffer_size = width * height * 2; // 16 bpp
    void* buf = NULL;
    esp_lcd_rgb_panel_get_frame_buffer(panel_handle, 1, &buf);
    framebuffer = (uint16_t*)buf;
#ifdef RGBG_OWN_RESTART
    rgbg_fb = (uint8_t*)buf;
#endif
    uint16_t color = random(0xffff);
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, 1, 1, &color));
}

RGBPanel::~RGBPanel() {
    // TODO: Cleanup panel_handle if needed
}

bool RGBPanel::drawPixel(int16_t x, int16_t y, uint16_t color) {
    int16_t w = width, h = height;
    
    // Apply rotation
    switch (rotation) {
    case 1: std::swap(w, h); std::swap(x, y); x = w - x - 1; break;
    case 2: x = w - x - 1; y = h - y - 1; break;
    case 3: std::swap(w, h); std::swap(x, y); y = h - y - 1; break;
    }
    
    if ((x < 0) || (x >= w) || (y < 0) || (y >= h)) return true; // Handled (out of bounds)
    
    framebuffer[y * w + x] = color;
    framebuffer_dirty = true;
    rgbg_fb_end();      // recently written, the write back comes with updateFrame()
    return true; // Handled by RGB panel
}

bool RGBPanel::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    rgbg_fb_end();      // mark the frame buffer as recently written, the write back comes with updateFrame()
    for (int16_t yp = y; yp < y + h; yp++) {
        uint16_t* line_start = &framebuffer[yp * width + x];
        for (int16_t i = 0; i < w; i++) {
            line_start[i] = color;
        }
    }
    framebuffer_dirty = true;
    return true; // Handled by RGB panel
}

bool RGBPanel::setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    window_x1 = x0;
    window_y1 = y0; 
    window_x2 = x1;
    window_y2 = y1;
    return true; // Handled by RGB panel
}

bool RGBPanel::pushColors(uint16_t *data, uint32_t len, bool first) {
    rgbg_fb_begin();    // copy into the PSRAM frame buffer: delay non-urgent DMA restarts
    esp_lcd_panel_draw_bitmap(panel_handle, window_x1, window_y1, window_x2, window_y2, data);
    rgbg_fb_end();
    return true; // Handled by RGB panel
}

bool RGBPanel::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    rgbg_fb_begin();
    uint16_t* line_start = &framebuffer[y * width + x];
    for (int16_t i = 0; i < w; i++) {
        line_start[i] = color;
    }
    CACHE_WRITEBACK_ADDR((uint32_t)line_start, w * 2);
    rgbg_fb_end();
    return true; // Handled by RGB panel
}

bool RGBPanel::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    rgbg_fb_begin();
    for (int16_t j = 0; j < h; j++) {
        framebuffer[(y + j) * width + x] = color;
    }
    CACHE_WRITEBACK_ADDR((uint32_t)&framebuffer[y * width + x], h * 2);
    rgbg_fb_end();
    return true; // Handled by RGB panel
}

bool RGBPanel::displayOnff(int8_t on) {
    esp_lcd_panel_disp_on_off(panel_handle, on != 0);
    return false; // bpanel is controlled from display class
}

bool RGBPanel::invertDisplay(bool invert) {
    // TODO: Not supported by RGB panels in ESP-IDF API
    return false; // Not handled - let uDisplay handle if possible
}

bool RGBPanel::setRotation(uint8_t rotation) {
    this->rotation = rotation & 3;
    esp_lcd_panel_mirror(panel_handle, rotation == 1 || rotation == 2, rotation & 2);
    esp_lcd_panel_swap_xy(panel_handle, rotation & 1);
    return true; // Handled by RGB panel
}

bool RGBPanel::updateFrame() {
    if (!framebuffer_dirty) {
        return true;
    }
    rgbg_fb_begin();
    CACHE_WRITEBACK_ADDR((uint32_t)framebuffer, framebuffer_size); //KISS and fast enough!
    rgbg_fb_end();
    framebuffer_dirty = false;

    return true; // Handled (no-op is still handled)
}


#endif // #if SOC_LCD_RGB_SUPPORTED
