/*
  xdrv_52_3_tf_lite_micro.ino - Berry scripting language, High-Level Tensor Flow Lite for Microprocessors model deployer

  Copyright (C) 2022 Christian Baars & Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

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
*/


#ifdef USE_BERRY

#include <berry.h>

#ifdef USE_BERRY_TF_LITE

#include <TensorFlowLite.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/core/api/error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/c/c_api_types.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "freertos/ringbuf.h"
#include "tensorflow/lite/c/common.h"

#ifdef USE_I2S
#include "mfcc.h"
#endif //USE_I2S

/*********************************************************************************************\
 * Internal helper classes and constants
\*********************************************************************************************/
#ifdef USE_I2S

#define kObservationWindow    1000    //milliseconds
#define kAudioSampleFrequency 16000   
#define kAudioSampleBits      16

struct TFL_mic_descriptor_t{
  uint8_t channel_fmt;      // UNUSED NOW !!!
  uint8_t preamp;           // UNUSED NOW !!!
  uint8_t slice_dur;        // milliseconds
  uint8_t slice_stride;     // milliseconds
  uint8_t num_filter;       // mfe bins
  uint8_t num_coeff;        // mfcc coefficients, if 0 -> compute MFE only
  uint8_t fft_bins;         // 2^fft_bins
  uint8_t max_invocations;  // max. invocations per second
  uint8_t db_floor;        // filter out noise below decibel threshold, treated as negative value
  uint8_t preemphasis;     //  as <float>preemphasis/100.0f , 0 - no preemphasis
};

struct TFL_mic_ctx_t{
  TaskHandle_t audio_capture_task = nullptr;
  SemaphoreHandle_t feature_buffer_mutex = nullptr;
  MFCC * mfcc = nullptr;
  int8_t* model_input_buffer = nullptr;

  union{
    struct {
      uint32_t is_audio_initialized:1;
      uint32_t is_first_time:1;
      // uint32_t new_feature_data:1;
      uint32_t continue_audio_capture:1;
      uint32_t stop_audio_capture:1;
      uint32_t audio_capture_ended:1;
      uint32_t use_mfcc:1;
      uint32_t use_gain_filter:1;
    } flag;
    uint32_t flags;
  };
  int feature_buffer_idx = 0;
  int8_t *feature_buffer;
  // user input
  // int32_t channel_fmt;  // UNUSED
  int32_t preamp;       // setup by I2S driver
  int32_t slice_dur;    // milliseconds
  int32_t slice_stride; // milliseconds
  uint8_t num_filter;   // mfe filter bins
  uint8_t num_coeff;    // mfcc coefficients, if 0 -> compute MFE only
  int32_t fft_bins;     // 2^fft_bins
  // calculated
  int32_t slice_size;   // bytes
  int32_t slice_count;
  uint16_t i2s_samples_to_get;
  int16_t db_floor;    // filter out noise below decibel threshold, this is now a negative value
  float preemphasis;
};

#endif //USE_I2S

struct TFL_stats_t{
  uint32_t model_size = 0;
  uint32_t used_arena_bytes = 0;
  uint32_t invocations = 0;
  uint32_t loop_task_free_stack_bytes = 0;
  uint32_t mic_task_free_stack_bytes = 0;
};

struct TFL_ctx_t{
const tflite::Model* model = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
int8_t *berry_output_buf = nullptr;
size_t berry_output_bufsize;
int TensorArenaSize = 2000;
uint8_t max_invocations = 4;    // max. invocations per second

TaskHandle_t loop_task = nullptr;
// QueueHandle_t loop_queue = nullptr;
union{
    struct {
    uint32_t init_done:1;
    uint32_t delay_next_invocation:1;
    uint32_t running_invocation:1;
    uint32_t running_loop:1;
    // uint32_t stop_loop:1;
    uint32_t loop_ended:1;
    uint32_t unread_output:1;
    uint32_t new_input_data:1;
    uint32_t use_mic:1;
    } option;
    uint32_t options;
};
#ifdef USE_I2S
TFL_mic_ctx_t *mic = nullptr;
#endif // USE_I2S
TFL_stats_t *stats = nullptr;
};

TFL_ctx_t *TFL = nullptr;
RingbufHandle_t TFL_log_buffer = nullptr;


/*********************************************************************************************\
 * Internal driver functions
\*********************************************************************************************/

/**
 * @brief This function is called from Microprint() from the Tensorflow framework
 *        Used to log from Tensorflow and from this (Tasmota) driver
 * 
 * @param s - message as c-string
 */
void TFL_Log(char *s){
  size_t len = strlen(s);
  if(len<5) return; // we assume this is for the trash

  xRingbufferSend(TFL_log_buffer, s, len+1 , pdMS_TO_TICKS(3));
}

bool TFL_create_task(){
    if (TFL->option.running_loop) return bfalse;
    if(TFL->loop_task!=nullptr) vTaskDelete(TFL->loop_task);
    
    xTaskCreatePinnedToCore(
    TFL_task_loop,                /* Function to implement the task */
    "tfl_loop",                   /* Name of the task */
    8000 + (TFL->TensorArenaSize),/* Stack size in words */
    NULL,                         /* Task input parameter */
    1,                            /* Priority of the task */
    &TFL->loop_task,              /* Task handle. */
    1);                           /* Core where the task should run */

    return btrue;
}

#ifdef USE_I2S
/**
 * @brief Set up some buffers and tables for feature extraction of audio samples. Must run once before starting audio capturing.
 * 
 * @return int - not used ATM
 */
int TFL_InitializeFeatures() {
  uint32_t samples_to_process = (TFL->mic->i2s_samples_to_get * TFL->mic->slice_dur)/TFL->mic->slice_stride;
  TFL->mic->mfcc = new MFCC(TFL->mic->num_coeff, samples_to_process, TFL->mic->num_filter, kAudioSampleFrequency, 300, 8000);
  TFL->mic->mfcc->set_preamp(TFL->mic->preamp);
  TFL->mic->mfcc->set_preemphasis(TFL->mic->preemphasis);
  MicroPrintf( PSTR( "MFCC %u initialized for %u samples, preamp: %u, preemphasis: %f"),TFL->mic->num_coeff,samples_to_process, TFL->mic->preamp, TFL->mic->preemphasis);
  return kTfLiteOk;
}

/**
 * @brief Computes features from every audio slice immediately after capturing it.
 * 
 * @param input - audio buffer
 * @param input_size - length auf audio input in samples (16-bit)
 * @param output_size - length of feature buffer in bytes (we use int8_t quantization)
 * @param output - feature buffer for one slice of audio
 * @param num_samples_read - not used anymore, to be removed
 * @return int 
 */
int TFL_GenerateFeatures(const int16_t* input, int input_size,
                                   int output_size, int8_t* output,
                                   size_t* num_samples_read) {

  float out_buf[output_size];

  TFL->mic->mfcc->mfcc_compute(input, out_buf);
  if(TFL->mic->num_coeff == 0){ // mfe only
    TFL->mic->mfcc->log10_normalize(out_buf, output_size, TFL->mic->db_floor);
  }
   
  float scale = TFL->input->params.scale;
  int32_t zero_point =  TFL->input->params.zero_point;
  float min_f = 1;
  float max_f = 0;

  for (size_t i = 0; i < output_size; ++i) {
    int32_t value = ((out_buf[i]/ scale) + zero_point);
    if(value <  -128){
      value = -128;
    }
    else if(value > 127){
      value = 127;
    }
    output[i] = value;
    // if(min_f>out_buf[i]) min_f = out_buf[i];
    // if(max_f<out_buf[i]) max_f = out_buf[i];
  }
  // MicroPrintf("%f %f %f %f %f %f %f %f %f %f %f %f %f",out_buf[0],out_buf[1] ,out_buf[2] ,out_buf[3] ,out_buf[4] ,out_buf[5] ,out_buf[6] ,out_buf[7] ,out_buf[8] ,out_buf[9] ,out_buf[10] ,out_buf[11] ,out_buf[12]);
  // MicroPrintf("min: %f max: %f",min_f,max_f);

  return kTfLiteOk;
}

/**
 * @brief Init I2S microphone. Pins must be configured in the "usual" Tasmota way. Some properties are variables stored in the descriptor.
 * 
 * @param descriptor - byte array passed from Berry. Arbitrary format - might change in the future!!
 * @return true - success
 * @return false - failure
 */
bool TFL_init_MIC(const uint8_t* descriptor){
   if (audio_i2s.in) {
      if(audio_i2s.in->getRxRate() != kAudioSampleFrequency || audio_i2s.in->getRxBitsPerSample() != kAudioSampleBits){
        AddLog(LOG_LEVEL_ERROR, PSTR("TFL: please configure microphone to 16 bits per sample at 16000 Hz"));
        return bfalse;
      }
      audio_i2s.in->startRx();
      AddLog(LOG_LEVEL_DEBUG, PSTR("TFL: init mic"));
   }
   else{
      AddLog(LOG_LEVEL_ERROR, PSTR("TFL: could not connect to I2S driver"));
      return bfalse;
   }
  
  TFL->mic = new TFL_mic_ctx_t;
  TFL->mic->flags = 0;
  TFL_set_mic_config(descriptor);

  TFL->mic->feature_buffer_mutex = xSemaphoreCreateMutex();

  AddLog(LOG_LEVEL_DEBUG, PSTR("TFL: MIC ctx created"));
  return btrue;
}

/**
 * @brief Function spawned as a task for capturing audio. Used for recording or inference.
 * 
 * @param arg - not used
 */
void TFL_capture_samples(void* arg) {
  MicroPrintf( PSTR( "Capture task started"));
  const int i2s_bytes_to_read = TFL->mic->i2s_samples_to_get * 2;  // according to slice duration

  const int buffer_size = (i2s_bytes_to_read * TFL->mic->slice_dur)/TFL->mic->slice_stride; // in bytes, current slice duration plus (potential) history data

  size_t samples_to_read;
  size_t bytes_read;
  int tf_status = 0;

  int16_t i2s_sample_buffer[buffer_size/2] = {0}; // in shorts, add the size to hold history data
  uint8_t  *i2s_byte_buffer = (uint8_t*)i2s_sample_buffer;
  uint32_t *i2s_long_buffer = (uint32_t*)i2s_sample_buffer;
  uint8_t  *i2s_read_buffer = i2s_byte_buffer + (buffer_size - i2s_bytes_to_read); // behind the history data, if slice duration != slice stride

  TFL_InitializeFeatures(); // TODO: check or not for success

  TFL->mic->flag.continue_audio_capture = 1;
  MicroPrintf( PSTR( "Enter capture samples loop"));

  while (TFL->mic->flag.continue_audio_capture == 1) {
    TFL->stats->mic_task_free_stack_bytes = uxTaskGetStackHighWaterMark(NULL);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    /* read slice data at once from i2s */
    // i2s_read(I2S_NUM, i2s_read_buffer, i2s_bytes_to_read, &bytes_read, pdMS_TO_TICKS(TFL->mic->slice_stride));
    esp_err_t err = i2s_channel_read(audio_i2s.in->getRxHandle(), (void*)i2s_read_buffer, i2s_bytes_to_read, &bytes_read, pdMS_TO_TICKS(TFL->mic->slice_stride));

    if (bytes_read <= 0) {
      MicroPrintf( PSTR( "Error %d in I2S, did read  %d bytes"), err, bytes_read);
    }
    else {
      if (bytes_read < i2s_bytes_to_read) {
       MicroPrintf(PSTR("Partial I2S read: %d"), bytes_read);
      }

      xSemaphoreTake(TFL->mic->feature_buffer_mutex, pdMS_TO_TICKS(TFL->mic->slice_stride) );

      tf_status = TFL_GenerateFeatures((const int16_t*)i2s_sample_buffer, buffer_size/2 , TFL->mic->slice_size,
                                            TFL->mic->feature_buffer + (TFL->mic->feature_buffer_idx * TFL->mic->slice_size),
                                            &samples_to_read);

      for(int i=0;i<(buffer_size - i2s_bytes_to_read)/4;i++){
        i2s_long_buffer[i] = i2s_long_buffer[i + ((buffer_size - i2s_bytes_to_read)/4)]; //move history to the front
      }

      TFL->mic->feature_buffer_idx += 1;
      if(TFL->mic->feature_buffer_idx == TFL->mic->slice_count){
        TFL->mic->feature_buffer_idx = 0;
      }
      TFL->option.new_input_data = 1;
      xSemaphoreGive(TFL->mic->feature_buffer_mutex);

    }
    // MicroPrintf( PSTR("t: %u"),xTaskGetTickCount()-xLastWakeTime);

    if(TFL->mic->flag.continue_audio_capture == 1) vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(TFL->mic->slice_stride) );
  }

  audio_i2s.in->stopRx();
  if(TFL->mic->mfcc != nullptr){
    delete TFL->mic->mfcc;
    TFL->mic->mfcc = nullptr;
  }
  MicroPrintf( PSTR("end capture task"));
  TFL->mic->flag.audio_capture_ended = 1;
  vTaskDelete(NULL);
}

/**
 * @brief Pass descriptor variables from Berry to the MIC context. Will also calculate some vars.
 * 
 * @param descriptor_buffer - byte array from Berry
 */
void TFL_set_mic_config(const uint8_t *descriptor_buffer){
  TFL_mic_descriptor_t *mic_descriptor = (TFL_mic_descriptor_t*)descriptor_buffer;
  // TFL->mic->channel_fmt = mic_descriptor->channel_fmt; // UNUSED!! - setup by I2S driver
  // TFL->mic->preamp = mic_descriptor->preamp; // UNUSED !!
  TFL->mic->preamp = audio_i2s.Settings->rx.gain / 16;  // setup by I2S driver
  TFL->mic->slice_dur = mic_descriptor->slice_dur;
  TFL->mic->slice_stride = mic_descriptor->slice_stride;
  TFL->mic->num_filter =  mic_descriptor->num_filter;
  TFL->mic->num_coeff =  mic_descriptor->num_coeff;
  TFL->mic->fft_bins = mic_descriptor->fft_bins;
  TFL->max_invocations = mic_descriptor->max_invocations;
  // now calculate the other settings
  TFL->mic->slice_size = mic_descriptor->num_coeff == 0 ? mic_descriptor->num_filter : mic_descriptor->num_coeff;
  TFL->mic->slice_count = (int32_t)((kObservationWindow/(float)TFL->mic->slice_stride) - 0.01); // floor(x) to int
  TFL->mic->preemphasis = (float)(mic_descriptor-> preemphasis)/100.0f;
  TFL->mic->i2s_samples_to_get = (TFL->mic->slice_stride * (kAudioSampleFrequency / 1000));
  TFL->mic->db_floor =  mic_descriptor->db_floor * -1;
  AddLog(LOG_LEVEL_DEBUG, PSTR("TFL: filter: %u, coefficients: %u"), TFL->mic->num_filter, TFL->mic->num_coeff);
  AddLog(LOG_LEVEL_DEBUG, PSTR("TFL: slice stride: %u ms -> slice count: %u, samples to read: %u"), TFL->mic->slice_stride, TFL->mic->slice_count, TFL->mic->i2s_samples_to_get);
}

/**
 * @brief Updates the input tensor with the data from the featuree buffer, which works as a ring buffer and is a shared resorce.
 * 
 */
void TFL_mic_feature_buf_to_input(){
  xSemaphoreTake(TFL->mic->feature_buffer_mutex, pdMS_TO_TICKS(TFL->mic->slice_stride) );
  // Copy feature buffer to input tensor
  int idx = TFL->mic->feature_buffer_idx + 1;  //oldest slice right after the newest slice
  if(idx == TFL->mic->slice_count) idx = 0;
  int slices_upperstack = TFL->mic->slice_count - idx;
  int slices_lowerstack = TFL->mic->slice_count - slices_upperstack;
  memcpy(TFL->mic->model_input_buffer,TFL->mic->feature_buffer + (idx * TFL->mic->slice_size), TFL->mic->slice_size * slices_upperstack);
  memcpy(TFL->mic->model_input_buffer +  (TFL->mic->slice_size * slices_upperstack),TFL->mic->feature_buffer, TFL->mic->slice_size * slices_lowerstack);
  xSemaphoreGive(TFL->mic->feature_buffer_mutex);
  return;
}

/**
 * @brief Helper function to stop audio capture task
 * 
 */
void TFL_stop_audio_capture(){
    MicroPrintf( PSTR("shall stop_capture_task"));
    if(TFL->mic->flag.continue_audio_capture == 0) return;
    TFL->mic->flag.continue_audio_capture = 0;
    uint32_t timeout = 0;
    while(TFL->mic->flag.audio_capture_ended == 0){
      if(timeout>3) break;
      vTaskDelay(pdMS_TO_TICKS(TFL->mic->slice_stride) );
      timeout++;
    }
    vSemaphoreDelete(TFL->mic->feature_buffer_mutex);
    delete[] TFL->mic->feature_buffer;
}
#endif //USE_I2S

/**
 * @brief Helper function to stop all runnning tasks
 * 
 */
void TFL_delete_tasks(){
  if(TFL == nullptr) return;
  TFL->option.running_loop = 0;
  while(TFL->option.loop_ended == 0){
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("TFL: task loop did stop"));
#ifdef USE_I2S
  if(TFL->mic != nullptr) {delete TFL->mic;} 
#endif //USE_I2S
  delete TFL;
  TFL = nullptr;
}

/**
 * @brief Starts inference task and the run loop. Should be terminated by signal from helper function.
 * 
 * @param pvParameters - not used
 */
void TFL_task_loop(void *pvParameters){
  uint8_t tensor_arena[TFL->TensorArenaSize];
  TFL->stats = new TFL_stats_t;
  tflite::AllOpsResolver resolver; //TODO: infer needed Ops from model??
  tflite::MicroInterpreter interpreter(
      TFL->model, resolver, tensor_arena, TFL->TensorArenaSize);
  int allocate_status = interpreter.AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    MicroPrintf( PSTR("AllocateTensors() failed"));
    goto loop_task_exit;
  }
  else{
    TFL->stats->used_arena_bytes = interpreter.arena_used_bytes();
  }
  // Obtain pointers to the model's input/output, we can use it externally
  TFL->input = interpreter.input(0);
  TFL->output = interpreter.output(0);

#ifdef USE_I2S
  if(TFL->option.use_mic == 1){
    TFL->mic->feature_buffer = new int8_t[TFL->mic->slice_size * TFL->mic->slice_count]();
    xTaskCreatePinnedToCore(TFL_capture_samples, "tfl_mic", 1024 * 5, NULL, 15, &TFL->mic->audio_capture_task, 0);
    if(TFL->mic->audio_capture_task == nullptr){
      MicroPrintf( PSTR("Creating capture task failed"));
      goto loop_task_exit;
    }
     MicroPrintf( PSTR("Created capture task"));
    TFL->mic->model_input_buffer = TFL->input->data.int8;
    vTaskDelay(pdMS_TO_TICKS(2000)); // wait for at least the time of the the microphone warm up
  }
#endif

  TFL->option.running_loop = 1;

// loop section
 MicroPrintf(PSTR("Enter task loop"));
  while (TFL->option.running_loop == 1)
  {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    TFL->stats->loop_task_free_stack_bytes = uxTaskGetStackHighWaterMark(NULL);

    while(TFL->option.delay_next_invocation == 1 && TFL->option.running_loop == 1){
      MicroPrintf(PSTR("delay_next_invocation"));
      vTaskDelay(10/ portTICK_PERIOD_MS);
    }
    TFL->option.delay_next_invocation = 1;

  #ifdef USE_I2S
    if(TFL->option.use_mic == 1){
      TFL->option.delay_next_invocation = 0; // Clean up later
      if(TFL->mic->flag.continue_audio_capture == 1){
        TFL_mic_feature_buf_to_input();
      }
    }
  #endif
    if(TFL->option.new_input_data){
      // MicroPrintf(PSTR("invocation requested"));
      TFL->option.running_invocation = 1;
      int invoke_status = interpreter.Invoke();
      if (invoke_status != kTfLiteOk) {
        MicroPrintf(PSTR("Invoke failed"));
        TFL->option.running_loop = 0;
      }
      if(TFL->berry_output_buf != nullptr){
        memcpy(TFL->berry_output_buf,(int8_t*)TFL->output->data.data,TFL->berry_output_bufsize);
      }
      TFL->stats->invocations++;
      TFL->option.unread_output = 1;
      TFL->option.running_invocation = 0;
      TFL->option.new_input_data == 0;
    }
    if(TFL->option.running_loop == 1) vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000 / TFL->max_invocations)); //maybe we already want to exit
  }

// end of loop section
loop_task_exit:
  delete TFL->stats;
#ifdef USE_I2S
  if(TFL->option.use_mic == 1) {TFL_stop_audio_capture();}
#endif //USE_I2S
  MicroPrintf(PSTR("end loop task"));
  TFL->option.loop_ended = 1;
  vTaskDelete( NULL );
}

/*********************************************************************************************\
 * Native functions mapped to Berry functions
\*********************************************************************************************/
extern "C" {

/**
 * @brief Create a context for a tensor flow session, that will later run in a task
 * 
 * @param vm 
 * @param type        BUF - generic byte buffer, MIC - microphone input
 * @return btrue 
 * @return bfalse 
 */
  bbool be_TFL_begin(struct bvm *vm, const char* type, const uint8_t *descriptor, size_t len) {
    if (TFL_log_buffer == nullptr){
      TFL_log_buffer = xRingbufferCreate(1028, RINGBUF_TYPE_NOSPLIT);
      AddLog(LOG_LEVEL_DEBUG, PSTR("TFL: init log buffer"));
    }
    TFL_delete_tasks();
    if(strlen(type) == 0){
      AddLog(LOG_LEVEL_DEBUG, PSTR("TFL: context deleted"));
      return btrue;
    }
    TFL = new TFL_ctx_t{};
    TFL->options = 0;

    if(*(uint32_t*)type == 0x00465542){ //BUF
      AddLog(LOG_LEVEL_DEBUG, PSTR("TFL: mode generic buffer"));
    }
    else if(*(uint32_t*)type == 0x0043494D){ //MIC
#ifdef USE_I2S
      if(descriptor && len==sizeof(TFL_mic_descriptor_t)){
        if(TFL_init_MIC(descriptor)){
          TFL->option.use_mic = 1;
        }
        else{
          return bfalse;
        }
      }
      else{
        AddLog(LOG_LEVEL_DEBUG, PSTR("TFL: expected descriptor of size: %u"), sizeof(TFL_mic_descriptor_t));
        return bfalse;
      }
#else
      AddLog(LOG_LEVEL_ERROR, PSTR("TFL: firmware with I2S audio required !!"));
#endif //USE_I2S
    }
    else{
      AddLog(LOG_LEVEL_DEBUG, PSTR("TFL: unknown mode"));
      return bfalse;
    }
    if(TFL!=nullptr){
      AddLog(LOG_LEVEL_INFO, PSTR("TFL: start TFL context with type: %s"), type);
      TFL->option.init_done = 1;
      return btrue;
    }
    return bfalse;
  }

/**
 * @brief Load tensor flow lite model and then start the tensor flow session in a task
 * 
 * @param vm 
 * @param buf     Model in a byte buffer
 * @param size    Size of buffer, must be 8-byte-aligned (auto-calculated by Berry)
 * @param arena   Size of the Tensor Arena in the stack of the TFL task
 * @return btrue 
 * @return bfalse 
 */
  bbool be_TFL_load(struct bvm *vm, const uint8_t *model_buf, size_t model_size, const uint8_t *output_buf, size_t output_size,int arena) {
    if(TFL){
      if(TFL->option.init_done){
        TFL->model = tflite::GetModel(model_buf);
        if ( TFL->model->version() != TFLITE_SCHEMA_VERSION) {
          AddLog(LOG_LEVEL_INFO, PSTR("TFL: Model schema version %d not supported "
                      "version %d."),  TFL->model->version(), TFLITE_SCHEMA_VERSION);
          return bfalse;
        }
        if(model_size%8 != 0){
          AddLog(LOG_LEVEL_INFO, PSTR("TFL: model not 8-byte aligned"));
          return bfalse;
        }
        if(arena){
          TFL->TensorArenaSize = arena;
        }

        TFL->berry_output_buf = (int8_t*)output_buf;
        TFL->berry_output_bufsize = output_size;
        TFL_create_task();
        AddLog(LOG_LEVEL_DEBUG, PSTR("TFL: starting TFL task, model sz: %u, allocated arena sz: %u"),model_size,TFL->TensorArenaSize);
        AddLog(LOG_LEVEL_DEBUG, PSTR("TFL: Berry output buffer of size: %u"),output_size);
        return btrue;
      }
    }
    return bfalse;
  }

  /**
   * @brief Send new input data to the tensor flow session
   * 
   * @param vm 
   * @param buf                   Arbitrary data in a byte buffer, must fit to the TF model
   * @param size                  Size of buffer (auto-calculated by Berry)
   * @param quantize_to_int8      Optional: convert bytes to quantized int8 values
   * @return btrue 
   * @return bfalse 
   */

  bbool be_TFL_input(struct bvm *vm, const uint8_t *buf, size_t size, bbool quantize_to_int8){
    if(!TFL) return bfalse;
    if(TFL->option.running_loop == 1){
      uint32_t timeout = 0;
      int8_t* tensor_input_buffer = tflite::GetTensorData<int8_t>(TFL->input);
      while(!TFL->option.delay_next_invocation == 1) {
        if(timeout>4) return bfalse;;
        delay(5);
        timeout++;
      }
      AddLog(LOG_LEVEL_DEBUG, PSTR("TFL: imput new data and invoke"));
      if(quantize_to_int8){
        int16_t temp_int;
        for(int i = 0; i < size; i++){
          temp_int = buf[i];
          tensor_input_buffer[i] = (int8_t)(temp_int - 128);
        }
      } else {
        memcpy(tensor_input_buffer, buf,size);
      }
      TFL->option.delay_next_invocation = 0;
      TFL->option.new_input_data = 1;
      return btrue;
    }
    return bfalse;
  }

  /**
   * @brief Get copy of the output sensor of the tensor flow session
   * 
   * @param vm 
   * @param buf       Arbitrary data in a byte buffer, must fit in size to the TF model
   * @param size      Size of buffer (auto-calculated by Berry)
   * @return btrue    - new data
   * @return bfalse   - old data
   */
  bbool be_TFL_output(struct bvm *vm, const uint8_t *buf, size_t size){
    if(!TFL) return bfalse;
    if(TFL->option.running_loop == 0) return bfalse;
    if(TFL->option.unread_output == 1){
      AddLog(LOG_LEVEL_DEBUG, PSTR("TFL: read output  data"));
      if(TFL->output != nullptr){
        TFL->option.unread_output = 0;
        return btrue; //new data
      }
    }
    return bfalse; // old data
  }

/**
 * @brief Read from the logging buffer from the TFL tasks
 * 
 * @param vm 
 * @return const char* 
 */
  const char * be_TFL_log(struct bvm *vm){
      size_t size;
      char * item =  (char *)xRingbufferReceive(TFL_log_buffer, &size, pdMS_TO_TICKS(5));
      if(item != NULL){
        // item[size] = 0; // 0-terminate string
        vRingbufferReturnItem(TFL_log_buffer, (void *)item);
      }
      return (const char *)item;
  }

/**
 * @brief Shows statistics about the model and the running TFL session
 * 
 * @param vm 
 * @return json string
 */
  const char * be_TFL_stats(struct bvm *vm){

    const size_t size = 512;
    char * s = (char*)calloc(size,1);
    uint32_t pos = 0;
    uint32_t inc = 0;
    inc = snprintf_P(s + pos, size, PSTR("{\"model\":{\"input_shape\":["));
    pos += inc;
    uint32_t dims = TFL->input->dims->size;
    for(int i=0;i<dims;i++){
      inc = snprintf_P(s + pos, size-pos, PSTR("%u"),TFL->input->dims->data[i]);
      pos += inc;
      if (i != dims-1){
        inc = snprintf_P(s + pos, size-pos,",");
        pos += inc;
      } 
    }
    inc = snprintf_P(s + pos, size-pos, PSTR("],\"input_type\":%u,\"output_shape\":["),TFL->input->type);
    pos += inc;
    dims = TFL->output->dims->size;
    for(int i=0;i<dims;i++){
      inc = snprintf_P(s + pos, size-pos, PSTR("%u"),TFL->output->dims->data[i]);
      pos += inc;
      if (i != dims-1){
        inc = snprintf_P(s + pos, size-pos,",");
        pos += inc;
      } 
    }
    inc = snprintf_P(s + pos, size-pos, PSTR("],\"output_type\":%u}"),TFL->output->type);
    pos += inc;
    inc = snprintf_P(s + pos, size-pos, PSTR(",\"session\":{\"used_arena\":%u"),TFL->stats->used_arena_bytes);
    pos += inc;
    inc = snprintf_P(s + pos, size-pos, PSTR(",\"loop_stack\":%u"),TFL->stats->loop_task_free_stack_bytes);
    pos += inc;
    if(TFL->option.use_mic == 1){
      inc = snprintf_P(s + pos, size-pos, PSTR(",\"audio_stack\":%u"),TFL->stats->mic_task_free_stack_bytes);
      pos += inc;
    }
    inc = snprintf_P(s + pos, size-pos, PSTR(",\"invocations\":%u}}"),TFL->stats->invocations);
    be_pushstring(vm, s);
    free(s);
    return s;
  }

} //extern "C"

#endif // USE_BERRY_TF_LITE
#endif  // USE_BERRY