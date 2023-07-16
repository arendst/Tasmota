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

#include "sensirion_gas_index_algorithm.h"
#include <math.h>

static void GasIndexAlgorithm__init_instances(GasIndexAlgorithmParams* params);
static void GasIndexAlgorithm__mean_variance_estimator__set_parameters(
    GasIndexAlgorithmParams* params);
static void GasIndexAlgorithm__mean_variance_estimator__set_states(
    GasIndexAlgorithmParams* params, float mean, float std, float uptime_gamma);
static float GasIndexAlgorithm__mean_variance_estimator__get_std(
    const GasIndexAlgorithmParams* params);
static float GasIndexAlgorithm__mean_variance_estimator__get_mean(
    const GasIndexAlgorithmParams* params);
static bool GasIndexAlgorithm__mean_variance_estimator__is_initialized(
    GasIndexAlgorithmParams* params);
static void GasIndexAlgorithm__mean_variance_estimator___calculate_gamma(
    GasIndexAlgorithmParams* params);
static void GasIndexAlgorithm__mean_variance_estimator__process(
    GasIndexAlgorithmParams* params, float sraw);
static void
GasIndexAlgorithm__mean_variance_estimator___sigmoid__set_parameters(
    GasIndexAlgorithmParams* params, float X0, float K);
static float GasIndexAlgorithm__mean_variance_estimator___sigmoid__process(
    GasIndexAlgorithmParams* params, float sample);
static void
GasIndexAlgorithm__mox_model__set_parameters(GasIndexAlgorithmParams* params,
                                             float SRAW_STD, float SRAW_MEAN);
static float
GasIndexAlgorithm__mox_model__process(GasIndexAlgorithmParams* params,
                                      float sraw);
static void GasIndexAlgorithm__sigmoid_scaled__set_parameters(
    GasIndexAlgorithmParams* params, float X0, float K, float offset_default);
static float
GasIndexAlgorithm__sigmoid_scaled__process(GasIndexAlgorithmParams* params,
                                           float sample);
static void GasIndexAlgorithm__adaptive_lowpass__set_parameters(
    GasIndexAlgorithmParams* params);
static float
GasIndexAlgorithm__adaptive_lowpass__process(GasIndexAlgorithmParams* params,
                                             float sample);

void GasIndexAlgorithm_init_with_sampling_interval(
    GasIndexAlgorithmParams* params, int32_t algorithm_type,
    float sampling_interval) {
    params->mAlgorithm_Type = algorithm_type;
    params->mSamplingInterval = sampling_interval;
    if ((algorithm_type == GasIndexAlgorithm_ALGORITHM_TYPE_NOX)) {
        params->mIndex_Offset = GasIndexAlgorithm_NOX_INDEX_OFFSET_DEFAULT;
        params->mSraw_Minimum = GasIndexAlgorithm_NOX_SRAW_MINIMUM;
        params->mGating_Max_Duration_Minutes =
            GasIndexAlgorithm_GATING_NOX_MAX_DURATION_MINUTES;
        params->mInit_Duration_Mean = GasIndexAlgorithm_INIT_DURATION_MEAN_NOX;
        params->mInit_Duration_Variance =
            GasIndexAlgorithm_INIT_DURATION_VARIANCE_NOX;
        params->mGating_Threshold = GasIndexAlgorithm_GATING_THRESHOLD_NOX;
    } else {
        params->mIndex_Offset = GasIndexAlgorithm_VOC_INDEX_OFFSET_DEFAULT;
        params->mSraw_Minimum = GasIndexAlgorithm_VOC_SRAW_MINIMUM;
        params->mGating_Max_Duration_Minutes =
            GasIndexAlgorithm_GATING_VOC_MAX_DURATION_MINUTES;
        params->mInit_Duration_Mean = GasIndexAlgorithm_INIT_DURATION_MEAN_VOC;
        params->mInit_Duration_Variance =
            GasIndexAlgorithm_INIT_DURATION_VARIANCE_VOC;
        params->mGating_Threshold = GasIndexAlgorithm_GATING_THRESHOLD_VOC;
    }
    params->mIndex_Gain = GasIndexAlgorithm_INDEX_GAIN;
    params->mTau_Mean_Hours = GasIndexAlgorithm_TAU_MEAN_HOURS;
    params->mTau_Variance_Hours = GasIndexAlgorithm_TAU_VARIANCE_HOURS;
    params->mSraw_Std_Initial = GasIndexAlgorithm_SRAW_STD_INITIAL;
    GasIndexAlgorithm_reset(params);
}

void GasIndexAlgorithm_init(GasIndexAlgorithmParams* params,
                            int32_t algorithm_type) {
    GasIndexAlgorithm_init_with_sampling_interval(
        params, algorithm_type, GasIndexAlgorithm_DEFAULT_SAMPLING_INTERVAL);
}

void GasIndexAlgorithm_reset(GasIndexAlgorithmParams* params) {
    params->mUptime = 0.f;
    params->mSraw = 0.f;
    params->mGas_Index = 0;
    GasIndexAlgorithm__init_instances(params);
}

static void GasIndexAlgorithm__init_instances(GasIndexAlgorithmParams* params) {

    GasIndexAlgorithm__mean_variance_estimator__set_parameters(params);
    GasIndexAlgorithm__mox_model__set_parameters(
        params, GasIndexAlgorithm__mean_variance_estimator__get_std(params),
        GasIndexAlgorithm__mean_variance_estimator__get_mean(params));
    if ((params->mAlgorithm_Type == GasIndexAlgorithm_ALGORITHM_TYPE_NOX)) {
        GasIndexAlgorithm__sigmoid_scaled__set_parameters(
            params, GasIndexAlgorithm_SIGMOID_X0_NOX,
            GasIndexAlgorithm_SIGMOID_K_NOX,
            GasIndexAlgorithm_NOX_INDEX_OFFSET_DEFAULT);
    } else {
        GasIndexAlgorithm__sigmoid_scaled__set_parameters(
            params, GasIndexAlgorithm_SIGMOID_X0_VOC,
            GasIndexAlgorithm_SIGMOID_K_VOC,
            GasIndexAlgorithm_VOC_INDEX_OFFSET_DEFAULT);
    }
    GasIndexAlgorithm__adaptive_lowpass__set_parameters(params);
}

void GasIndexAlgorithm_get_sampling_interval(
    const GasIndexAlgorithmParams* params, float* sampling_interval) {
    *sampling_interval = params->mSamplingInterval;
}

void GasIndexAlgorithm_get_states(const GasIndexAlgorithmParams* params,
                                  float* state0, float* state1) {

    *state0 = GasIndexAlgorithm__mean_variance_estimator__get_mean(params);
    *state1 = GasIndexAlgorithm__mean_variance_estimator__get_std(params);
    return;
}

void GasIndexAlgorithm_set_states(GasIndexAlgorithmParams* params, float state0,
                                  float state1) {

    GasIndexAlgorithm__mean_variance_estimator__set_states(
        params, state0, state1, GasIndexAlgorithm_PERSISTENCE_UPTIME_GAMMA);
    GasIndexAlgorithm__mox_model__set_parameters(
        params, GasIndexAlgorithm__mean_variance_estimator__get_std(params),
        GasIndexAlgorithm__mean_variance_estimator__get_mean(params));
    params->mSraw = state0;
}

void GasIndexAlgorithm_set_tuning_parameters(
    GasIndexAlgorithmParams* params, int32_t index_offset,
    int32_t learning_time_offset_hours, int32_t learning_time_gain_hours,
    int32_t gating_max_duration_minutes, int32_t std_initial,
    int32_t gain_factor) {

    params->mIndex_Offset = ((float)(index_offset));
    params->mTau_Mean_Hours = ((float)(learning_time_offset_hours));
    params->mTau_Variance_Hours = ((float)(learning_time_gain_hours));
    params->mGating_Max_Duration_Minutes =
        ((float)(gating_max_duration_minutes));
    params->mSraw_Std_Initial = ((float)(std_initial));
    params->mIndex_Gain = ((float)(gain_factor));
    GasIndexAlgorithm__init_instances(params);
}

void GasIndexAlgorithm_get_tuning_parameters(
    const GasIndexAlgorithmParams* params, int32_t* index_offset,
    int32_t* learning_time_offset_hours, int32_t* learning_time_gain_hours,
    int32_t* gating_max_duration_minutes, int32_t* std_initial,
    int32_t* gain_factor) {

    *index_offset = ((int32_t)(params->mIndex_Offset));
    *learning_time_offset_hours = ((int32_t)(params->mTau_Mean_Hours));
    *learning_time_gain_hours = ((int32_t)(params->mTau_Variance_Hours));
    *gating_max_duration_minutes =
        ((int32_t)(params->mGating_Max_Duration_Minutes));
    *std_initial = ((int32_t)(params->mSraw_Std_Initial));
    *gain_factor = ((int32_t)(params->mIndex_Gain));
    return;
}

void GasIndexAlgorithm_process(GasIndexAlgorithmParams* params, int32_t sraw,
                               int32_t* gas_index) {

    if ((params->mUptime <= GasIndexAlgorithm_INITIAL_BLACKOUT)) {
        params->mUptime = (params->mUptime + params->mSamplingInterval);
    } else {
        if (((sraw > 0) && (sraw < 65000))) {
            if ((sraw < (params->mSraw_Minimum + 1))) {
                sraw = (params->mSraw_Minimum + 1);
            } else if ((sraw > (params->mSraw_Minimum + 32767))) {
                sraw = (params->mSraw_Minimum + 32767);
            }
            params->mSraw = ((float)((sraw - params->mSraw_Minimum)));
        }
        if (((params->mAlgorithm_Type ==
              GasIndexAlgorithm_ALGORITHM_TYPE_VOC) ||
             GasIndexAlgorithm__mean_variance_estimator__is_initialized(
                 params))) {
            params->mGas_Index =
                GasIndexAlgorithm__mox_model__process(params, params->mSraw);
            params->mGas_Index = GasIndexAlgorithm__sigmoid_scaled__process(
                params, params->mGas_Index);
        } else {
            params->mGas_Index = params->mIndex_Offset;
        }
        params->mGas_Index = GasIndexAlgorithm__adaptive_lowpass__process(
            params, params->mGas_Index);
        if ((params->mGas_Index < 0.5f)) {
            params->mGas_Index = 0.5f;
        }
        if ((params->mSraw > 0.f)) {
            GasIndexAlgorithm__mean_variance_estimator__process(params,
                                                                params->mSraw);
            GasIndexAlgorithm__mox_model__set_parameters(
                params,
                GasIndexAlgorithm__mean_variance_estimator__get_std(params),
                GasIndexAlgorithm__mean_variance_estimator__get_mean(params));
        }
    }
    *gas_index = ((int32_t)((params->mGas_Index + 0.5f)));
    return;
}

static void GasIndexAlgorithm__mean_variance_estimator__set_parameters(
    GasIndexAlgorithmParams* params) {

    params->m_Mean_Variance_Estimator___Initialized = false;
    params->m_Mean_Variance_Estimator___Mean = 0.f;
    params->m_Mean_Variance_Estimator___Sraw_Offset = 0.f;
    params->m_Mean_Variance_Estimator___Std = params->mSraw_Std_Initial;
    params->m_Mean_Variance_Estimator___Gamma_Mean =
        (((GasIndexAlgorithm_MEAN_VARIANCE_ESTIMATOR__ADDITIONAL_GAMMA_MEAN_SCALING *
           GasIndexAlgorithm_MEAN_VARIANCE_ESTIMATOR__GAMMA_SCALING) *
          (params->mSamplingInterval / 3600.f)) /
         (params->mTau_Mean_Hours + (params->mSamplingInterval / 3600.f)));
    params->m_Mean_Variance_Estimator___Gamma_Variance =
        ((GasIndexAlgorithm_MEAN_VARIANCE_ESTIMATOR__GAMMA_SCALING *
          (params->mSamplingInterval / 3600.f)) /
         (params->mTau_Variance_Hours + (params->mSamplingInterval / 3600.f)));
    if ((params->mAlgorithm_Type == GasIndexAlgorithm_ALGORITHM_TYPE_NOX)) {
        params->m_Mean_Variance_Estimator___Gamma_Initial_Mean =
            (((GasIndexAlgorithm_MEAN_VARIANCE_ESTIMATOR__ADDITIONAL_GAMMA_MEAN_SCALING *
               GasIndexAlgorithm_MEAN_VARIANCE_ESTIMATOR__GAMMA_SCALING) *
              params->mSamplingInterval) /
             (GasIndexAlgorithm_TAU_INITIAL_MEAN_NOX +
              params->mSamplingInterval));
    } else {
        params->m_Mean_Variance_Estimator___Gamma_Initial_Mean =
            (((GasIndexAlgorithm_MEAN_VARIANCE_ESTIMATOR__ADDITIONAL_GAMMA_MEAN_SCALING *
               GasIndexAlgorithm_MEAN_VARIANCE_ESTIMATOR__GAMMA_SCALING) *
              params->mSamplingInterval) /
             (GasIndexAlgorithm_TAU_INITIAL_MEAN_VOC +
              params->mSamplingInterval));
    }
    params->m_Mean_Variance_Estimator___Gamma_Initial_Variance =
        ((GasIndexAlgorithm_MEAN_VARIANCE_ESTIMATOR__GAMMA_SCALING *
          params->mSamplingInterval) /
         (GasIndexAlgorithm_TAU_INITIAL_VARIANCE + params->mSamplingInterval));
    params->m_Mean_Variance_Estimator__Gamma_Mean = 0.f;
    params->m_Mean_Variance_Estimator__Gamma_Variance = 0.f;
    params->m_Mean_Variance_Estimator___Uptime_Gamma = 0.f;
    params->m_Mean_Variance_Estimator___Uptime_Gating = 0.f;
    params->m_Mean_Variance_Estimator___Gating_Duration_Minutes = 0.f;
}

static void GasIndexAlgorithm__mean_variance_estimator__set_states(
    GasIndexAlgorithmParams* params, float mean, float std,
    float uptime_gamma) {

    params->m_Mean_Variance_Estimator___Mean = mean;
    params->m_Mean_Variance_Estimator___Std = std;
    params->m_Mean_Variance_Estimator___Uptime_Gamma = uptime_gamma;
    params->m_Mean_Variance_Estimator___Initialized = true;
}

static float GasIndexAlgorithm__mean_variance_estimator__get_std(
    const GasIndexAlgorithmParams* params) {

    return params->m_Mean_Variance_Estimator___Std;
}

static float GasIndexAlgorithm__mean_variance_estimator__get_mean(
    const GasIndexAlgorithmParams* params) {

    return (params->m_Mean_Variance_Estimator___Mean +
            params->m_Mean_Variance_Estimator___Sraw_Offset);
}

static bool GasIndexAlgorithm__mean_variance_estimator__is_initialized(
    GasIndexAlgorithmParams* params) {

    return params->m_Mean_Variance_Estimator___Initialized;
}

static void GasIndexAlgorithm__mean_variance_estimator___calculate_gamma(
    GasIndexAlgorithmParams* params) {

    float uptime_limit;
    float sigmoid_gamma_mean;
    float gamma_mean;
    float gating_threshold_mean;
    float sigmoid_gating_mean;
    float sigmoid_gamma_variance;
    float gamma_variance;
    float gating_threshold_variance;
    float sigmoid_gating_variance;

    uptime_limit = (GasIndexAlgorithm_MEAN_VARIANCE_ESTIMATOR__FIX16_MAX -
                    params->mSamplingInterval);
    if ((params->m_Mean_Variance_Estimator___Uptime_Gamma < uptime_limit)) {
        params->m_Mean_Variance_Estimator___Uptime_Gamma =
            (params->m_Mean_Variance_Estimator___Uptime_Gamma +
             params->mSamplingInterval);
    }
    if ((params->m_Mean_Variance_Estimator___Uptime_Gating < uptime_limit)) {
        params->m_Mean_Variance_Estimator___Uptime_Gating =
            (params->m_Mean_Variance_Estimator___Uptime_Gating +
             params->mSamplingInterval);
    }
    GasIndexAlgorithm__mean_variance_estimator___sigmoid__set_parameters(
        params, params->mInit_Duration_Mean,
        GasIndexAlgorithm_INIT_TRANSITION_MEAN);
    sigmoid_gamma_mean =
        GasIndexAlgorithm__mean_variance_estimator___sigmoid__process(
            params, params->m_Mean_Variance_Estimator___Uptime_Gamma);
    gamma_mean = (params->m_Mean_Variance_Estimator___Gamma_Mean +
                  ((params->m_Mean_Variance_Estimator___Gamma_Initial_Mean -
                    params->m_Mean_Variance_Estimator___Gamma_Mean) *
                   sigmoid_gamma_mean));
    gating_threshold_mean =
        (params->mGating_Threshold +
         ((GasIndexAlgorithm_GATING_THRESHOLD_INITIAL -
           params->mGating_Threshold) *
          GasIndexAlgorithm__mean_variance_estimator___sigmoid__process(
              params, params->m_Mean_Variance_Estimator___Uptime_Gating)));
    GasIndexAlgorithm__mean_variance_estimator___sigmoid__set_parameters(
        params, gating_threshold_mean,
        GasIndexAlgorithm_GATING_THRESHOLD_TRANSITION);
    sigmoid_gating_mean =
        GasIndexAlgorithm__mean_variance_estimator___sigmoid__process(
            params, params->mGas_Index);
    params->m_Mean_Variance_Estimator__Gamma_Mean =
        (sigmoid_gating_mean * gamma_mean);
    GasIndexAlgorithm__mean_variance_estimator___sigmoid__set_parameters(
        params, params->mInit_Duration_Variance,
        GasIndexAlgorithm_INIT_TRANSITION_VARIANCE);
    sigmoid_gamma_variance =
        GasIndexAlgorithm__mean_variance_estimator___sigmoid__process(
            params, params->m_Mean_Variance_Estimator___Uptime_Gamma);
    gamma_variance =
        (params->m_Mean_Variance_Estimator___Gamma_Variance +
         ((params->m_Mean_Variance_Estimator___Gamma_Initial_Variance -
           params->m_Mean_Variance_Estimator___Gamma_Variance) *
          (sigmoid_gamma_variance - sigmoid_gamma_mean)));
    gating_threshold_variance =
        (params->mGating_Threshold +
         ((GasIndexAlgorithm_GATING_THRESHOLD_INITIAL -
           params->mGating_Threshold) *
          GasIndexAlgorithm__mean_variance_estimator___sigmoid__process(
              params, params->m_Mean_Variance_Estimator___Uptime_Gating)));
    GasIndexAlgorithm__mean_variance_estimator___sigmoid__set_parameters(
        params, gating_threshold_variance,
        GasIndexAlgorithm_GATING_THRESHOLD_TRANSITION);
    sigmoid_gating_variance =
        GasIndexAlgorithm__mean_variance_estimator___sigmoid__process(
            params, params->mGas_Index);
    params->m_Mean_Variance_Estimator__Gamma_Variance =
        (sigmoid_gating_variance * gamma_variance);
    params->m_Mean_Variance_Estimator___Gating_Duration_Minutes =
        (params->m_Mean_Variance_Estimator___Gating_Duration_Minutes +
         ((params->mSamplingInterval / 60.f) *
          (((1.f - sigmoid_gating_mean) *
            (1.f + GasIndexAlgorithm_GATING_MAX_RATIO)) -
           GasIndexAlgorithm_GATING_MAX_RATIO)));
    if ((params->m_Mean_Variance_Estimator___Gating_Duration_Minutes < 0.f)) {
        params->m_Mean_Variance_Estimator___Gating_Duration_Minutes = 0.f;
    }
    if ((params->m_Mean_Variance_Estimator___Gating_Duration_Minutes >
         params->mGating_Max_Duration_Minutes)) {
        params->m_Mean_Variance_Estimator___Uptime_Gating = 0.f;
    }
}

static void GasIndexAlgorithm__mean_variance_estimator__process(
    GasIndexAlgorithmParams* params, float sraw) {

    float delta_sgp;
    float c;
    float additional_scaling;

    if ((params->m_Mean_Variance_Estimator___Initialized == false)) {
        params->m_Mean_Variance_Estimator___Initialized = true;
        params->m_Mean_Variance_Estimator___Sraw_Offset = sraw;
        params->m_Mean_Variance_Estimator___Mean = 0.f;
    } else {
        if (((params->m_Mean_Variance_Estimator___Mean >= 100.f) ||
             (params->m_Mean_Variance_Estimator___Mean <= -100.f))) {
            params->m_Mean_Variance_Estimator___Sraw_Offset =
                (params->m_Mean_Variance_Estimator___Sraw_Offset +
                 params->m_Mean_Variance_Estimator___Mean);
            params->m_Mean_Variance_Estimator___Mean = 0.f;
        }
        sraw = (sraw - params->m_Mean_Variance_Estimator___Sraw_Offset);
        GasIndexAlgorithm__mean_variance_estimator___calculate_gamma(params);
        delta_sgp = ((sraw - params->m_Mean_Variance_Estimator___Mean) /
                     GasIndexAlgorithm_MEAN_VARIANCE_ESTIMATOR__GAMMA_SCALING);
        if ((delta_sgp < 0.f)) {
            c = (params->m_Mean_Variance_Estimator___Std - delta_sgp);
        } else {
            c = (params->m_Mean_Variance_Estimator___Std + delta_sgp);
        }
        additional_scaling = 1.f;
        if ((c > 1440.f)) {
            additional_scaling = ((c / 1440.f) * (c / 1440.f));
        }
        params->m_Mean_Variance_Estimator___Std =
            (sqrtf((additional_scaling *
                    (GasIndexAlgorithm_MEAN_VARIANCE_ESTIMATOR__GAMMA_SCALING -
                     params->m_Mean_Variance_Estimator__Gamma_Variance))) *
             sqrtf(
                 ((params->m_Mean_Variance_Estimator___Std *
                   (params->m_Mean_Variance_Estimator___Std /
                    (GasIndexAlgorithm_MEAN_VARIANCE_ESTIMATOR__GAMMA_SCALING *
                     additional_scaling))) +
                  (((params->m_Mean_Variance_Estimator__Gamma_Variance *
                     delta_sgp) /
                    additional_scaling) *
                   delta_sgp))));
        params->m_Mean_Variance_Estimator___Mean =
            (params->m_Mean_Variance_Estimator___Mean +
             ((params->m_Mean_Variance_Estimator__Gamma_Mean * delta_sgp) /
              GasIndexAlgorithm_MEAN_VARIANCE_ESTIMATOR__ADDITIONAL_GAMMA_MEAN_SCALING));
    }
}

static void
GasIndexAlgorithm__mean_variance_estimator___sigmoid__set_parameters(
    GasIndexAlgorithmParams* params, float X0, float K) {

    params->m_Mean_Variance_Estimator___Sigmoid__K = K;
    params->m_Mean_Variance_Estimator___Sigmoid__X0 = X0;
}

static float GasIndexAlgorithm__mean_variance_estimator___sigmoid__process(
    GasIndexAlgorithmParams* params, float sample) {

    float x;

    x = (params->m_Mean_Variance_Estimator___Sigmoid__K *
         (sample - params->m_Mean_Variance_Estimator___Sigmoid__X0));
    if ((x < -50.f)) {
        return 1.f;
    } else if ((x > 50.f)) {
        return 0.f;
    } else {
        return (1.f / (1.f + expf(x)));
    }
}

static void
GasIndexAlgorithm__mox_model__set_parameters(GasIndexAlgorithmParams* params,
                                             float SRAW_STD, float SRAW_MEAN) {

    params->m_Mox_Model__Sraw_Std = SRAW_STD;
    params->m_Mox_Model__Sraw_Mean = SRAW_MEAN;
}

static float
GasIndexAlgorithm__mox_model__process(GasIndexAlgorithmParams* params,
                                      float sraw) {

    if ((params->mAlgorithm_Type == GasIndexAlgorithm_ALGORITHM_TYPE_NOX)) {
        return (((sraw - params->m_Mox_Model__Sraw_Mean) /
                 GasIndexAlgorithm_SRAW_STD_NOX) *
                params->mIndex_Gain);
    } else {
        return (((sraw - params->m_Mox_Model__Sraw_Mean) /
                 (-1.f * (params->m_Mox_Model__Sraw_Std +
                          GasIndexAlgorithm_SRAW_STD_BONUS_VOC))) *
                params->mIndex_Gain);
    }
}

static void GasIndexAlgorithm__sigmoid_scaled__set_parameters(
    GasIndexAlgorithmParams* params, float X0, float K, float offset_default) {

    params->m_Sigmoid_Scaled__K = K;
    params->m_Sigmoid_Scaled__X0 = X0;
    params->m_Sigmoid_Scaled__Offset_Default = offset_default;
}

static float
GasIndexAlgorithm__sigmoid_scaled__process(GasIndexAlgorithmParams* params,
                                           float sample) {

    float x;
    float shift;

    x = (params->m_Sigmoid_Scaled__K * (sample - params->m_Sigmoid_Scaled__X0));
    if ((x < -50.f)) {
        return GasIndexAlgorithm_SIGMOID_L;
    } else if ((x > 50.f)) {
        return 0.f;
    } else {
        if ((sample >= 0.f)) {
            if ((params->m_Sigmoid_Scaled__Offset_Default == 1.f)) {
                shift = ((500.f / 499.f) * (1.f - params->mIndex_Offset));
            } else {
                shift = ((GasIndexAlgorithm_SIGMOID_L -
                          (5.f * params->mIndex_Offset)) /
                         4.f);
            }
            return (((GasIndexAlgorithm_SIGMOID_L + shift) / (1.f + expf(x))) -
                    shift);
        } else {
            return ((params->mIndex_Offset /
                     params->m_Sigmoid_Scaled__Offset_Default) *
                    (GasIndexAlgorithm_SIGMOID_L / (1.f + expf(x))));
        }
    }
}

static void GasIndexAlgorithm__adaptive_lowpass__set_parameters(
    GasIndexAlgorithmParams* params) {

    params->m_Adaptive_Lowpass__A1 =
        (params->mSamplingInterval /
         (GasIndexAlgorithm_LP_TAU_FAST + params->mSamplingInterval));
    params->m_Adaptive_Lowpass__A2 =
        (params->mSamplingInterval /
         (GasIndexAlgorithm_LP_TAU_SLOW + params->mSamplingInterval));
    params->m_Adaptive_Lowpass___Initialized = false;
}

static float
GasIndexAlgorithm__adaptive_lowpass__process(GasIndexAlgorithmParams* params,
                                             float sample) {

    float abs_delta;
    float F1;
    float tau_a;
    float a3;

    if ((params->m_Adaptive_Lowpass___Initialized == false)) {
        params->m_Adaptive_Lowpass___X1 = sample;
        params->m_Adaptive_Lowpass___X2 = sample;
        params->m_Adaptive_Lowpass___X3 = sample;
        params->m_Adaptive_Lowpass___Initialized = true;
    }
    params->m_Adaptive_Lowpass___X1 =
        (((1.f - params->m_Adaptive_Lowpass__A1) *
          params->m_Adaptive_Lowpass___X1) +
         (params->m_Adaptive_Lowpass__A1 * sample));
    params->m_Adaptive_Lowpass___X2 =
        (((1.f - params->m_Adaptive_Lowpass__A2) *
          params->m_Adaptive_Lowpass___X2) +
         (params->m_Adaptive_Lowpass__A2 * sample));
    abs_delta =
        (params->m_Adaptive_Lowpass___X1 - params->m_Adaptive_Lowpass___X2);
    if ((abs_delta < 0.f)) {
        abs_delta = (-1.f * abs_delta);
    }
    F1 = expf((GasIndexAlgorithm_LP_ALPHA * abs_delta));
    tau_a = (((GasIndexAlgorithm_LP_TAU_SLOW - GasIndexAlgorithm_LP_TAU_FAST) *
              F1) +
             GasIndexAlgorithm_LP_TAU_FAST);
    a3 = (params->mSamplingInterval / (params->mSamplingInterval + tau_a));
    params->m_Adaptive_Lowpass___X3 =
        (((1.f - a3) * params->m_Adaptive_Lowpass___X3) + (a3 * sample));
    return params->m_Adaptive_Lowpass___X3;
}