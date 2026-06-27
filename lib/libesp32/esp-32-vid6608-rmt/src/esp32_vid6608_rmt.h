/**
 * @file esp32_vid6608_rmt.h
 * @author Petr Golovachev (petro@petro.ws)
 * @brief Main class of ESP-32 VID6608 RMT driver
 *
 * @copyright Copyright (c) 2026
 *
 */

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "driver/gpio.h"
#include "driver/rmt_tx.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "soc/soc_caps.h"
#if !SOC_RMT_SUPPORTED
  #error "RMT is not supported on this platform"
#endif

class esp32_vid6608_rmt {
public:
    /**
     * @brief Configuration struct
     *
     */
    struct Config {
        gpio_num_t  stepPin;    ///< f(scx) pin
        gpio_num_t  dirPin;     ///< CC/CCWA pin
        uint16_t    maxSteps;   ///< Max steps er full rotation
        bool        useAccel = true; ///< Enable smoothness
    };

    /**
     * @brief Construct a new esp32_vid6608_rmt object
     *
     * @param cfg Configuration struct
     */
    explicit esp32_vid6608_rmt(const Config &cfg);
    ~esp32_vid6608_rmt();

    esp32_vid6608_rmt(const esp32_vid6608_rmt &)            = delete;
    esp32_vid6608_rmt &operator=(const esp32_vid6608_rmt &) = delete;

    enum MoveState {
      ZERO_BACK_FULL = 0,
      ZERO_BACK_HALF = -2,
    };

    /**
     * @brief Resets zero position to actual 0 position
     *
     * Moves full rotation forward and backward
     *
     * @param initialPos initial position to assume zeroing, default is 0. If you know the "old" gauge position, you can provide it here to avoid end-stop bounce
     *
     * @warning this function is blocking, execution is delayed upon done
     */
    void        zero(int32_t initialPos = ZERO_BACK_HALF);

    /**
     * @brief Wait for current move task to complete
     *
     * @param timeout_ms max time to wait, -1 for wait forever
     *
     */
    void        wait(int32_t timeout_ms = 10000);

    /**
     * @brief Shedules movement to defined absolute position
     *
     * Input is checked for sanity: must be in range 0...maxSteps-1. Values bigger are threated as maxSteps-1.
     * @warning this function is asynchronous, actual movement is done in another task.
     * @warning next move will be scheduled after current move is done to avoid drive jittering.
     *
     * @param position absolute position in range 0...maxSteps-1
     */
    void        setPos(int32_t steps);

    /**
     * @brief Get the driving pin
     *
     * @return gpio_num_t driving pin number
     */
    gpio_num_t  getPinStep() { return this->config.stepPin; }

    /**
     * @brief Get the Max Steps object
     *
     * @return uint16_t max steps, defined for this drive
     */
    uint16_t    getMaxSteps() { return this->config.maxSteps; }

    /**
     * @brief Get the Current Position object
     *
     * @return int32_t current position (sheduled at this moment, actual move may apply)
     */
    int32_t     getCurrentPosition() { return this->targetPosition; }

    // -------------------------------------------------------------------------------------------
    // Old Arduino-vid6608 compat layer
    // -------------------------------------------------------------------------------------------

    /**
     * @brief Shedules movement to defined absolute position
     *
     * Input is checked for sanity: must be in range 0...maxSteps-1. Values bigger are threated as maxSteps-1.
     * @warning this function is asynchronous, actual movement is done in the loop() function.
     * @warning next move will be scheduled after current move is done to avoid drive jittering.
     *
     * @param position absolute position in range 0...maxSteps-1
     */
    void moveTo(uint16_t position) { setPos(position); }

    /**
     * @brief Test if motor is moving
     *
     * Return true, if drive still have sheduled steps (that means that next loop() call will result impulse).
     *
     * @return true if drive is moveemnt
     * @return false if drive is stopped
     */
    bool isMoving();

    /**
     * @brief Test if motor is stopped
     *
     * @return true if drive is stopped
     * @return false if drive is moveemnt
     */
    bool isStopped() { return !isMoving(); }

    /**
     * @brief Returns current real absolute position
     *
     * @return uint16_t current real drive position in steps
     */
    uint16_t getPosition() { return (uint16_t)this->targetPosition; }

    /**
     * @brief Does nothing on RMT
     *
     */
    void loop() {}

private:
    /**
     * @brief Length of the acceleration ramp, in step pulses.
     *
     * The acceleration profile is baked into the library and is not
     * user-tunable. A move shorter than @c 2*kAccelSteps uses a triangular
     * profile and never reaches the cruise rate.
     */
    static constexpr size_t kAccelSteps = 32;

    /**
     * @brief Build the per-step half-period table at startup.
     *
     * @return Array of half-periods in RMT ticks (1 µs each).
     */
    static std::array<uint16_t, kAccelSteps> buildAccelCurve();

    /**
     * @brief Per-step half-period for the acceleration ramp (RMT ticks).
     *
     * Index 0 is the slowest pulse (start of motion); the last index is the
     * fastest pulse (cruise). Deceleration walks the table in reverse.
     */
    static const std::array<uint16_t, kAccelSteps> kAccelHalfPeriod;

    void        movePrepare(int32_t steps);
    void        moveRamp(int32_t steps);
    void        moveConst(int32_t steps, int32_t speed_hz);
    void        driverTask();
    static void driverTaskStart(void *arg);

    bool                 running    = false;
    Config               config;
    rmt_channel_handle_t chan       = nullptr;
    rmt_encoder_handle_t enc        = nullptr;
    SemaphoreHandle_t    infoMutex  = nullptr;
    SemaphoreHandle_t    taskNotify = nullptr;
    TaskHandle_t         taskHandle = nullptr;

    bool    targetPending      = false; ///< Do we have next move pending?
    int32_t targetPosition     = 0;    ///< Target position in steps
    int32_t targetPositionNext = 0;    ///< Target position in steps (scheduled for next move)
    uint8_t targetDir          = 254;  ///< Target direction to move

    /**
     * @brief Scratch RMT symbol buffers used by every move.
     *
     * The copy encoder pulls bytes from the source pointer asynchronously,
     * so these buffers must outlive each transmission — every move is
     * followed by @ref wait() before the next one reuses them.
     * @{
     */
    rmt_symbol_word_t accelBuf[kAccelSteps]{};
    rmt_symbol_word_t decelBuf[kAccelSteps]{};
    rmt_symbol_word_t cruisePulse{};
    /** @} */
};
