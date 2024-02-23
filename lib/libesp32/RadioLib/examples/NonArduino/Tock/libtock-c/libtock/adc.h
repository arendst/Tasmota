// driver for collecting analog samples
#pragma once

#include <stdint.h>

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_ADC 0x5

// mode of the ADC
// Used to tell which operation a callback corresponds to
typedef enum {
  SingleSample = 0,
  ContinuousSample = 1,
  SingleBuffer = 2,
  ContinuousBuffer = 3
} ADCMode_t;

// ***** System Call Interface *****

// set the function called by the ADC when operations are complete
// See `adc_cb` in adc.c for callback documentation
//
// callback - pointer to function to be called
// callback_args - pointer to data provided to the callback
int adc_set_callback(subscribe_upcall callback, void* callback_args);

// provides an application buffer to the ADC driver to fill with samples
//
// buffer - pointer to buffer to fill with samples
// length - number of samples requested, must be less than or equal to buffer
//          length
int adc_set_buffer(uint16_t* buffer, uint32_t length);

// provide an application buffer to fill with samples when continuously
// sampling
//
// buffer - pointer to buffer to fill with samples
// length - number of samples requested, must be less than or equal to buffer
//          length
int adc_set_double_buffer(uint16_t* buffer, uint32_t length);

// query whether the ADC driver is present
bool adc_is_present(void);

// query how many channels are available in the ADC driver
int adc_channel_count(int* count);

// request a single analog sample
//
// channel - number of the channel to be sampled
int adc_single_sample(uint8_t channel);

// request an repeated analog samples at a given frequency
//
// channel - number of the channel to be sampled
// frequency - rate in samples per second to collect data at
int adc_continuous_sample(uint8_t channel, uint32_t frequency);

// request a buffer full of samples from the ADC
//
// channel - number of the channel to be sampled
// frequency - rate in samples per second to collect data at
int adc_buffered_sample(uint8_t channel, uint32_t frequency);

// request continuous ADC sampling
// Alternates between the two provided application buffers
//
// channel - number of the channel to be sampled
// frequency - rate in samples per second to collect data at
int adc_continuous_buffered_sample(uint8_t channel, uint32_t frequency);

// cancel an outstanding ADC operation
// No callback will occur from the prior ADC operation. The ADC may not be
// immediately ready to use again if a single sample was canceled. Usually used
// to stop a continuous sampling operation
int adc_stop_sampling(void);


// ***** Callback Wrappers *****

// set the function called by the ADC when a single_sample operation
// completes.
//
// callback - pointer to function to be called
//      uint8_t - channel the sample was taken on
//      uint16_t - sample value
//      void* - user pointer to pass to callback
int adc_set_single_sample_callback(void(*callback)(uint8_t, uint16_t, void*),
                                   void* callback_args);

// set the function called by the ADC when a continuous_sample operation
// completes.
//
// callback - pointer to function to be called
//      uint8_t - channel the sample was taken on
//      uint16_t - sample value
//      void* - user pointer to pass to callback
int adc_set_continuous_sample_callback(void(*callback)(uint8_t, uint16_t, void*),
                                       void* callback_args);

// set the function called by the ADC when a buffered_sample operation
// completes.
//
// callback - pointer to function to be called
//      uint8_t - channel the sample was taken on
//      uint32_t - number of samples
//      uint16_t* - pointer to buffer containing samples
//      void* - user pointer to pass to callback
int adc_set_buffered_sample_callback(void(*callback)(uint8_t, uint32_t, uint16_t*, void*),
                                     void* callback_args);

// set the function called by the ADC when a continuous_buffered_sample operation
// completes.
//
// callback - pointer to function to be called
//      uint8_t - channel the sample was taken on
//      uint32_t - number of samples
//      uint16_t* - pointer to buffer containing samples
//      void* - user pointer to pass to callback
int adc_set_continuous_buffered_sample_callback(void(*callback)(uint8_t, uint32_t, uint16_t*, void*),
                                                void* callback_args);


// ***** Synchronous Calls *****

// request a single analog sample
// Wrapper providing a synchronous interface around the raw ADC system calls
//
// channel - number of the channel to be sampled
// sample - pointer to a uint16_t in which the sample will be stored
int adc_sample_sync(uint8_t channel, uint16_t* sample);

// request a buffer full of analog samples
// Wrapper providing a synchronous interface around the raw ADC system calls
//
// channel - number of the channel to be sampled
// frequency - rate in samples per second to collect data at
// buffer - pointer to buffer to be filled with samples
// length - number of samples requested, must be less than or equal to buffer
//          length
int adc_sample_buffer_sync(uint8_t channel, uint32_t frequency, uint16_t* buffer, uint32_t length);

// returns the reference voltage in mV or an error ( < 0)
// it this is not available
int adc_get_reference_voltage (void);

// returns the adc resolution bits
int adc_get_resolution_bits (void);

#ifdef __cplusplus
}
#endif
