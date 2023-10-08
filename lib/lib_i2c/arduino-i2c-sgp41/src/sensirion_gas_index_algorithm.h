/*
 * Copyright (c) 2022, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef GASINDEXALGORITHM_H_
#define GASINDEXALGORITHM_H_

#include <stdint.h>

#ifndef __cplusplus

#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else

#ifndef bool
#define bool int
#define true 1
#define false 0
#endif  // bool

#endif  // __STDC_VERSION__

#endif  // __cplusplus

// Should be set by the building toolchain
#ifndef LIBRARY_VERSION_NAME
#define LIBRARY_VERSION_NAME "3.2.0"
#endif

#define GasIndexAlgorithm_ALGORITHM_TYPE_VOC (0)
#define GasIndexAlgorithm_ALGORITHM_TYPE_NOX (1)
#define GasIndexAlgorithm_DEFAULT_SAMPLING_INTERVAL (1.f)
#define GasIndexAlgorithm_INITIAL_BLACKOUT (45.f)
#define GasIndexAlgorithm_INDEX_GAIN (230.f)
#define GasIndexAlgorithm_SRAW_STD_INITIAL (50.f)
#define GasIndexAlgorithm_SRAW_STD_BONUS_VOC (220.f)
#define GasIndexAlgorithm_SRAW_STD_NOX (2000.f)
#define GasIndexAlgorithm_TAU_MEAN_HOURS (12.f)
#define GasIndexAlgorithm_TAU_VARIANCE_HOURS (12.f)
#define GasIndexAlgorithm_TAU_INITIAL_MEAN_VOC (20.f)
#define GasIndexAlgorithm_TAU_INITIAL_MEAN_NOX (1200.f)
#define GasIndexAlgorithm_INIT_DURATION_MEAN_VOC ((3600.f * 0.75f))
#define GasIndexAlgorithm_INIT_DURATION_MEAN_NOX ((3600.f * 4.75f))
#define GasIndexAlgorithm_INIT_TRANSITION_MEAN (0.01f)
#define GasIndexAlgorithm_TAU_INITIAL_VARIANCE (2500.f)
#define GasIndexAlgorithm_INIT_DURATION_VARIANCE_VOC ((3600.f * 1.45f))
#define GasIndexAlgorithm_INIT_DURATION_VARIANCE_NOX ((3600.f * 5.70f))
#define GasIndexAlgorithm_INIT_TRANSITION_VARIANCE (0.01f)
#define GasIndexAlgorithm_GATING_THRESHOLD_VOC (340.f)
#define GasIndexAlgorithm_GATING_THRESHOLD_NOX (30.f)
#define GasIndexAlgorithm_GATING_THRESHOLD_INITIAL (510.f)
#define GasIndexAlgorithm_GATING_THRESHOLD_TRANSITION (0.09f)
#define GasIndexAlgorithm_GATING_VOC_MAX_DURATION_MINUTES ((60.f * 3.f))
#define GasIndexAlgorithm_GATING_NOX_MAX_DURATION_MINUTES ((60.f * 12.f))
#define GasIndexAlgorithm_GATING_MAX_RATIO (0.3f)
#define GasIndexAlgorithm_SIGMOID_L (500.f)
#define GasIndexAlgorithm_SIGMOID_K_VOC (-0.0065f)
#define GasIndexAlgorithm_SIGMOID_X0_VOC (213.f)
#define GasIndexAlgorithm_SIGMOID_K_NOX (-0.0101f)
#define GasIndexAlgorithm_SIGMOID_X0_NOX (614.f)
#define GasIndexAlgorithm_VOC_INDEX_OFFSET_DEFAULT (100.f)
#define GasIndexAlgorithm_NOX_INDEX_OFFSET_DEFAULT (1.f)
#define GasIndexAlgorithm_LP_TAU_FAST (20.0f)
#define GasIndexAlgorithm_LP_TAU_SLOW (500.0f)
#define GasIndexAlgorithm_LP_ALPHA (-0.2f)
#define GasIndexAlgorithm_VOC_SRAW_MINIMUM (20000)
#define GasIndexAlgorithm_NOX_SRAW_MINIMUM (10000)
#define GasIndexAlgorithm_PERSISTENCE_UPTIME_GAMMA ((3.f * 3600.f))
#define GasIndexAlgorithm_TUNING_INDEX_OFFSET_MIN (1)
#define GasIndexAlgorithm_TUNING_INDEX_OFFSET_MAX (250)
#define GasIndexAlgorithm_TUNING_LEARNING_TIME_OFFSET_HOURS_MIN (1)
#define GasIndexAlgorithm_TUNING_LEARNING_TIME_OFFSET_HOURS_MAX (1000)
#define GasIndexAlgorithm_TUNING_LEARNING_TIME_GAIN_HOURS_MIN (1)
#define GasIndexAlgorithm_TUNING_LEARNING_TIME_GAIN_HOURS_MAX (1000)
#define GasIndexAlgorithm_TUNING_GATING_MAX_DURATION_MINUTES_MIN (0)
#define GasIndexAlgorithm_TUNING_GATING_MAX_DURATION_MINUTES_MAX (3000)
#define GasIndexAlgorithm_TUNING_STD_INITIAL_MIN (10)
#define GasIndexAlgorithm_TUNING_STD_INITIAL_MAX (5000)
#define GasIndexAlgorithm_TUNING_GAIN_FACTOR_MIN (1)
#define GasIndexAlgorithm_TUNING_GAIN_FACTOR_MAX (1000)
#define GasIndexAlgorithm_MEAN_VARIANCE_ESTIMATOR__GAMMA_SCALING (64.f)
#define GasIndexAlgorithm_MEAN_VARIANCE_ESTIMATOR__ADDITIONAL_GAMMA_MEAN_SCALING \
    (8.f)
#define GasIndexAlgorithm_MEAN_VARIANCE_ESTIMATOR__FIX16_MAX (32767.f)

/**
 * Struct to hold all parameters and states of the gas algorithm.
 */
typedef struct {
    int mAlgorithm_Type;
    float mSamplingInterval;
    float mIndex_Offset;
    int32_t mSraw_Minimum;
    float mGating_Max_Duration_Minutes;
    float mInit_Duration_Mean;
    float mInit_Duration_Variance;
    float mGating_Threshold;
    float mIndex_Gain;
    float mTau_Mean_Hours;
    float mTau_Variance_Hours;
    float mSraw_Std_Initial;
    float mUptime;
    float mSraw;
    float mGas_Index;
    bool m_Mean_Variance_Estimator___Initialized;
    float m_Mean_Variance_Estimator___Mean;
    float m_Mean_Variance_Estimator___Sraw_Offset;
    float m_Mean_Variance_Estimator___Std;
    float m_Mean_Variance_Estimator___Gamma_Mean;
    float m_Mean_Variance_Estimator___Gamma_Variance;
    float m_Mean_Variance_Estimator___Gamma_Initial_Mean;
    float m_Mean_Variance_Estimator___Gamma_Initial_Variance;
    float m_Mean_Variance_Estimator__Gamma_Mean;
    float m_Mean_Variance_Estimator__Gamma_Variance;
    float m_Mean_Variance_Estimator___Uptime_Gamma;
    float m_Mean_Variance_Estimator___Uptime_Gating;
    float m_Mean_Variance_Estimator___Gating_Duration_Minutes;
    float m_Mean_Variance_Estimator___Sigmoid__K;
    float m_Mean_Variance_Estimator___Sigmoid__X0;
    float m_Mox_Model__Sraw_Std;
    float m_Mox_Model__Sraw_Mean;
    float m_Sigmoid_Scaled__K;
    float m_Sigmoid_Scaled__X0;
    float m_Sigmoid_Scaled__Offset_Default;
    float m_Adaptive_Lowpass__A1;
    float m_Adaptive_Lowpass__A2;
    bool m_Adaptive_Lowpass___Initialized;
    float m_Adaptive_Lowpass___X1;
    float m_Adaptive_Lowpass___X2;
    float m_Adaptive_Lowpass___X3;
} GasIndexAlgorithmParams;

/**
 * Initialize the gas index algorithm parameters for the specified algorithm
 * type and reset its internal states. Call this once at the beginning.
 * @param params            Pointer to the GasIndexAlgorithmParams struct
 * @param algorithm_type    0 (GasIndexAlgorithm_ALGORITHM_TYPE_VOC) for VOC or
 *                          1 (GasIndexAlgorithm_ALGORITHM_TYPE_NOX) for NOx
 */
void GasIndexAlgorithm_init(GasIndexAlgorithmParams* params,
                            int32_t algorithm_type);

/**
 * Initialize the gas index algorithm parameters for the specified algorithm
 * type and reset its internal states. Call this once at the beginning.
 * @param params            Pointer to the GasIndexAlgorithmParams struct
 * @param algorithm_type    0 (GasIndexAlgorithm_ALGORITHM_TYPE_VOC) for VOC or
 *                          1 (GasIndexAlgorithm_ALGORITHM_TYPE_NOX) for NOx
 * @param sampling_interval The sampling interval in seconds the algorithm is
 *                          called. Tested for 1s and 10s.
 */
void GasIndexAlgorithm_init_with_sampling_interval(
    GasIndexAlgorithmParams* params, int32_t algorithm_type,
    float sampling_interval);

/**
 * Reset the internal states of the gas index algorithm. Previously set tuning
 * parameters are preserved. Call this when resuming operation after a
 * measurement interruption.
 * @param params    Pointer to the GasIndexAlgorithmParams struct
 */
void GasIndexAlgorithm_reset(GasIndexAlgorithmParams* params);

/**
 * Get current algorithm states. Retrieved values can be used in
 * GasIndexAlgorithm_set_states() to resume operation after a short
 * interruption, skipping initial learning phase.
 * NOTE: This feature can only be used for VOC algorithm type and after at least
 * 3 hours of continuous operation.
 * @param params    Pointer to the GasIndexAlgorithmParams struct
 * @param state0    State0 to be stored
 * @param state1    State1 to be stored
 */
void GasIndexAlgorithm_get_states(const GasIndexAlgorithmParams* params,
                                  float* state0, float* state1);

/**
 * Set previously retrieved algorithm states to resume operation after a short
 * interruption, skipping initial learning phase. This feature should not be
 * used after interruptions of more than 10 minutes. Call this once after
 * GasIndexAlgorithm_init() or GasIndexAlgorithm_reset() and the optional
 * GasIndexAlgorithm_set_tuning_parameters(), if desired. Otherwise, the
 * algorithm will start with initial learning phase.
 * NOTE: This feature can only be used for VOC algorithm type.
 * @param params    Pointer to the GasIndexAlgorithmParams struct
 * @param state0    State0 to be restored
 * @param state1    State1 to be restored
 */
void GasIndexAlgorithm_set_states(GasIndexAlgorithmParams* params, float state0,
                                  float state1);

/**
 * Set parameters to customize the gas index algorithm. Call this once after
 * GasIndexAlgorithm_init() and before optional GasIndexAlgorithm_set_states(),
 * if desired. Otherwise, the default values will be used.
 *
 * @param params                      Pointer to the GasIndexAlgorithmParams
 *                                    struct
 * @param index_offset                Gas index representing typical (average)
 *                                    conditions. Range 1..250,
 *                                    default 100 for VOC and 1 for NOx
 * @param learning_time_offset_hours  Time constant of long-term estimator for
 *                                    offset. Past events will be forgotten
 *                                    after about twice the learning time.
 *                                    Range 1..1000 [hours], default 12 [hours]
 * @param learning_time_gain_hours    Time constant of long-term estimator for
 *                                    gain. Past events will be forgotten
 *                                    after about twice the learning time.
 *                                    Range 1..1000 [hours], default 12 [hours]
 *                                    NOTE: This value is not relevant for NOx
 *                                    algorithm type
 * @param gating_max_duration_minutes Maximum duration of gating (freeze of
 *                                    estimator during high gas index signal).
 *                                    0 (no gating) or range 1..3000 [minutes],
 *                                    default 180 [minutes] for VOC and
 *                                    720 [minutes] for NOx
 * @param std_initial                 Initial estimate for standard deviation.
 *                                    Lower value boosts events during initial
 *                                    learning period, but may result in larger
 *                                    device-to-device variations.
 *                                    Range 10..5000, default 50
 *                                    NOTE: This value is not relevant for NOx
 *                                    algorithm type
 * @param gain_factor                 Factor used to scale applied gain value
 *                                    when calculating gas index. Range 1..1000,
 *                                    default 230
 */
void GasIndexAlgorithm_set_tuning_parameters(
    GasIndexAlgorithmParams* params, int32_t index_offset,
    int32_t learning_time_offset_hours, int32_t learning_time_gain_hours,
    int32_t gating_max_duration_minutes, int32_t std_initial,
    int32_t gain_factor);

/**
 * Get current parameters to customize the gas index algorithm.
 * Refer to GasIndexAlgorithm_set_tuning_parameters() for description of the
 * parameters.
 */
void GasIndexAlgorithm_get_tuning_parameters(
    const GasIndexAlgorithmParams* params, int32_t* index_offset,
    int32_t* learning_time_offset_hours, int32_t* learning_time_gain_hours,
    int32_t* gating_max_duration_minutes, int32_t* std_initial,
    int32_t* gain_factor);

/**
 * Get the sampling interval parameter used by the algorithm.
 */
void GasIndexAlgorithm_get_sampling_interval(
    const GasIndexAlgorithmParams* params, float* sampling_interval);

/**
 * Calculate the gas index value from the raw sensor value.
 *
 * @param params      Pointer to the GasIndexAlgorithmParams struct
 * @param sraw        Raw value from the SGP4x sensor
 * @param gas_index   Calculated gas index value from the raw sensor value. Zero
 *                    during initial blackout period and 1..500 afterwards
 */
void GasIndexAlgorithm_process(GasIndexAlgorithmParams* params, int32_t sraw,
                               int32_t* gas_index);

#endif /* GASINDEXALGORITHM_H_ */