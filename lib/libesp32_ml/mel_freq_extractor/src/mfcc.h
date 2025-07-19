/*
  mfcc.h - mel frequency extractor for ESP32

  Computes features for slizes of audio data similiar to speechpy
  This is intended to provide a stripped down implementation that can work with Edgempulse trained models

  based on:
  https://github.com/astorfi/speechpy
  https://github.com/AIWintermuteAI/Speech-to-Intent-Micro/blob/main/inference_code/Wio_Terminal/wio_speech_to_intent_150_10/mfcc.cpp
  
  Copyright (C) 2022  Christian Baars
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.@
*/



#ifndef MELFREQUENCYEXTRACTOR_H
#define MELFREQUENCYEXTRACTOR_H

#include <Arduino.h>
#include <string.h>
#include "float.h"
#include "esp_dsp.h"


class MFCC{
  private:
    int num_mfcc_features;
    int frame_len;
    int frame_len_padded;
    int num_bank_bins;
    float * m_frame;
    float * m_buffer;
    float * m_mel_energies;
    float * m_dct_matrix;
    float * m_mel_fbank;

    uint8_t m_amplification;
    float m_preemphasis;

    float * create_dct_matrix(int32_t input_length, int32_t coefficient_count);
    void create_mel_filterbank(int samp_freq, int low_freq, int high_freq);
    
    static inline float InverseMelScale(float mel_freq) {
      return 700.0f * (expf (mel_freq / 1127.0f) - 1.0f);
    }

    static inline float MelScale(float freq) {
      return 1127.0f * logf (1.0f + freq / 700.0f);
    }
    

  public:
    MFCC(int num_mfcc_features, int frame_len,int num_bank_bins, int samp_freq, int low_freq, int high_freq);
    ~MFCC();
    void set_preamp(uint8_t amplification);
    void set_preemphasis(float preemphasis);
    void mfcc_compute(const int16_t* data, float* mfcc_out);
    void log10_normalize(float* out_buf, int out_buf_len, int noise_floor_db);
};


MFCC::MFCC(int num_mfcc_features, int frame_len, int num_bank_bins, int samp_freq, int low_freq, int high_freq)
:num_mfcc_features(num_mfcc_features),
 frame_len(frame_len),
 num_bank_bins(num_bank_bins)
{
  // Round-up to nearest power of 2.
  frame_len_padded = pow(2,ceil((log(frame_len)/log(2))));

  m_frame = new float[frame_len_padded];
  m_buffer = new float[frame_len_padded * 2];
  m_mel_energies = new float[num_bank_bins];

  //create window function
  // window_func = new float[frame_len];
  // dsps_wind_hann_f32(window_func, frame_len);

  m_amplification = 1;
  m_preemphasis = 0.0;

  //create mel filterbank
  create_mel_filterbank(samp_freq, low_freq, high_freq);
  
  //create DCT matrix for mfcc mode
  if(num_mfcc_features != 0){
    m_dct_matrix = create_dct_matrix(num_bank_bins, num_mfcc_features);
  }

  //initialize FFT
  int ret = dsps_fft2r_init_fc32(NULL, frame_len_padded);
    if(ret==0){
          MicroPrintf("Framelength: %u, (rounded: %u)", frame_len,frame_len_padded);
    }
    else{
        MicroPrintf("dsps_fft2r_init_fc32 error: %d",ret);
    }

}

MFCC::~MFCC() {
  delete []m_frame;
  delete []m_buffer;
  delete []m_mel_energies;
  // delete []window_func;

  delete []m_dct_matrix;

  // for(int i=0;i<num_bank_bins;i++)
  //   delete mel_fbank[i];
  delete [] m_mel_fbank;
  dsps_fft2r_deinit_fc32();
}

float * MFCC::create_dct_matrix(int32_t input_length, int32_t coefficient_count) {
  int32_t k, n;
  float * M = new float[input_length*coefficient_count];

  float normalizer = sqrt(2.0/(float)input_length);
  for (k = 0; k < coefficient_count; k++) {
    for (n = 0; n < input_length; n++) {
      M[k*input_length+n] = normalizer * cos( ((double)M_PI)/input_length * (n + 0.5) * k );
    }
  }
  return M;
}


void MFCC::create_mel_filterbank(int samp_freq, int low_freq, int high_freq) {
  // MicroPrintf("Create FB ...");
  int coefficients = frame_len_padded/2 + 1;
  m_mel_fbank = new float[num_bank_bins * coefficients](); // zero-init
  uint32_t delta = (MelScale(high_freq) - MelScale(low_freq))/(num_bank_bins + 1);
  
  float mels[num_bank_bins+2];
  float hertz[num_bank_bins+2];
  int freq_index[num_bank_bins+2];

  for (int i = 0;i<num_bank_bins+2;i++){
    mels[i] = MelScale(low_freq) + (i * delta);
    hertz[i] = InverseMelScale(mels[i]);
    freq_index[i] = (coefficients + 1) * hertz[i] /samp_freq;
  }

  for (int i = 0;i<num_bank_bins;i++ ){
    int left = int(freq_index[i]);
    int middle = int(freq_index[i + 1]);
    int right = int(freq_index[i + 2]);
    float slope_up = 1/float(middle-left);
    float slope_down = 1/float(right-middle);
    //  MicroPrintf("%u %u %u %f %f",left,middle,right,slope_up,slope_down);
    for (int j = 0;j<(right-left+1);j++){
      if (j < middle-left + 1){
        m_mel_fbank[(i*coefficients)+left+j] = j*slope_up;
      }
      else{
        m_mel_fbank[(i*coefficients)+left+j] = m_mel_fbank[(i*coefficients)+left+j-1]-slope_down;
      }   
   }
  }
  // MicroPrintf("%f %f %f %f %f %f %f %f ",m_mel_fbank[5],m_mel_fbank[6] ,m_mel_fbank[7] ,m_mel_fbank[8] ,m_mel_fbank[9] ,m_mel_fbank[10] ,m_mel_fbank[11] ,m_mel_fbank[12]);
  // MicroPrintf("FB done");
}


void MFCC::log10_normalize(float* out_buf, int out_buf_len, int noise_floor_db) {
    const float noise = static_cast<float>(noise_floor_db * -1);
    const float noise_scale = 1.0f / (static_cast<float>(noise_floor_db * -1) + 12.0f);
    for (size_t ix = 0; ix < out_buf_len; ix++) {
        float f = out_buf[ix];
        if (f < 1e-30) {
            out_buf[ix] = 0;
            return;
        }
        f = 10.0f * log10(f); // scale by 10
        f += noise;
        f *= noise_scale;
        // clip again
        if (f < 0.0f) f = 0.0f;
        else if (f > 1.0f) f = 1.0f;
        out_buf[ix] = f;
    }
}

void MFCC::set_preamp(uint8_t amplification){
    m_amplification = amplification;
}

void MFCC::set_preemphasis(float preemphasis){
  m_preemphasis = preemphasis;
  // Speechpy computes this over the window of a sample, here we will compute only over the slize !!
}


void MFCC::mfcc_compute(const int16_t * audio_data, float* mfcc_out) {
    
  int32_t i, j, bin;
  int coefficients = frame_len_padded/2 + 1;
  int data_clipped = 0;
  int data_clipped_low = 0;
  float conv_factor = m_amplification;
  float clip_thres  = 0.99f * (float)(1<<15);

  // MicroPrintf("%d %d %d %d %d %d %d %d",audio_data[0],audio_data[1] ,audio_data[2] ,audio_data[3] ,audio_data[4] ,audio_data[5] ,audio_data[6] ,audio_data[7]);

  //TensorFlow way of normalizing .wav data to (-1,1) for speechpy's MFE
  if(num_mfcc_features == 0){
    conv_factor /= (float)(1<<15);
    clip_thres /= (float)(1<<15);
  }

  for (int i = 0; i < frame_len; i++) {
    m_buffer[i] = audio_data[i] * conv_factor; //mfe -1..1, mfcc int16_t as float, both with additional pre_amp factor
  }
  if(m_buffer[i]> clip_thres){
    m_buffer[i] /= m_amplification;
    data_clipped++;
  }
  else if( m_buffer[i]< -clip_thres){
    m_buffer[i] /= m_amplification;
    data_clipped_low++;
  }

  if(data_clipped>0)
    MicroPrintf("Clip: %d __ %d",data_clipped, data_clipped_low);
  
  // MicroPrintf("%f %f %f %f %f %f %f %f ",m_buffer[0],m_buffer[1] ,m_buffer[2] ,m_buffer[3] ,m_buffer[4] ,m_buffer[5] ,m_buffer[6] ,m_buffer[7]);

  //pre-emphasis
  if(m_preemphasis!=0.0){
    m_frame[0] = m_buffer[0] - m_preemphasis * m_buffer[frame_len - 1]; // roll through the frame "back" to the end
    for (i = 1; i < frame_len; i++){
        m_frame[i] = m_buffer[i] - m_preemphasis * m_buffer[i - 1];
    }
  }
  else{
    for (i = 1; i < frame_len; i++){
        m_frame[i] = m_buffer[i];
    }
  }

  // prepare buffer for FFT
  for (i = 0; i < frame_len_padded; i++) {
         m_buffer[i * 2] = i<frame_len ? m_frame[i] : 0;
  //     m_buffer[i * 2] = i<frame_len ? frame[i] * window_func[i] : 0; // in case we want to use a window function
        m_buffer[i*2 + 1] = 0;
      }
  //  MicroPrintf("%f %f %f %f %f %f %f %f ",frame[0],frame[1] ,frame[2] ,frame[3] ,frame[4] ,frame[5] ,frame[6] ,frame[7]);

  //Compute FFT
  int err = dsps_fft2r_fc32(m_buffer, frame_len_padded);
  err += dsps_bit_rev_fc32(m_buffer, frame_len_padded); //Bit reverse
  // err += dsps_cplx2reC_fc32(m_buffer, frame_len_padded);// Complex spectrum in y_cf

  if(err!=0){
      MicroPrintf("dsps_fft2r error: %u %f %f %f %f %f %f %f %f ",err,m_buffer[0],m_buffer[1] ,m_buffer[2] ,m_buffer[3] ,m_buffer[4] ,m_buffer[5] ,m_buffer[512] ,m_buffer[513]);
  }

  for (int i = 0 ; i < coefficients ; i++) {
      m_buffer[i] = (m_buffer[i*2] * m_buffer[i*2] + m_buffer[i*2 + 1] * m_buffer[i*2 + 1])/frame_len_padded;
  }
  // MicroPrintf(" pow spec: %f %f %f %f %f %f %f %f ",m_buffer[0],m_buffer[1] ,m_buffer[2] ,m_buffer[3] ,m_buffer[4] ,m_buffer[5] ,m_buffer[255] ,m_buffer[256]);
 
  //Apply mel filterbanks
  for (int i = 0;i<num_bank_bins;i++ ){
    m_mel_energies[i] = 0;
    for (int j = 0;j<coefficients;j++ ){
      m_mel_energies[i] += m_buffer[j] * m_mel_fbank[(i * coefficients) + j];
    }
  }

  // for MFE  copy and return - compute 10 * log10() later explicitely
  if(num_mfcc_features == 0){
      for (bin = 0; bin < num_bank_bins; bin++){
        mfcc_out[bin] = m_mel_energies[bin];
      }
      // MicroPrintf("%u feat: %f %f %f %f %f %f %f %f ",num_bank_bins,mfcc_out[0],mfcc_out[1] ,mfcc_out[2] ,mfcc_out[3] ,mfcc_out[4] ,mfcc_out[5] ,mfcc_out[6] ,mfcc_out[7]);
      return;
  }

  // Continue for MFCC
  // Take log
    for (bin = 0; bin < num_bank_bins; bin++){
        m_mel_energies[bin] = logf(m_mel_energies[bin]);
    }

  // Take DCT. Uses matrix mul.
  for (i = 1; i < num_mfcc_features; i++) {
    float sum = 0.0;
    for (j = 0; j < num_bank_bins; j++) {
      sum += m_dct_matrix[i*num_bank_bins+j] * m_mel_energies[j];
    }
    mfcc_out[i] = sum;
  }

  // replace first cepstral coefficient with log of frame energy for DC elimination
  for (i=0; i<frame_len_padded; i++){
    mfcc_out[0] += m_buffer[i];
  }
  mfcc_out[0] = logf(mfcc_out[0]);
}


#endif //MELFREQUENCYEXTRACTOR_H