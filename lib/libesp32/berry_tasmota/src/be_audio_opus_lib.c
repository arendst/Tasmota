/********************************************************************
 * Tasmota I2S audio classes
 * 
 * 
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_I2S
#ifdef USE_I2S_AUDIO_BERRY

#include "libopus/opus.h"
#include "be_mapping.h"

// Tasmota Logging
extern void tasmota_log_C(uint32_t loglevel, const char * berry_buf, ...);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};

// init(freq:int, channels:int)
void *be_audio_opus_decoder_init_ntv(int freq, int channels) {
  if (freq <= 0) { freq = 16000; }
  if (channels <= 0) { channels = 1; }
  int opus_size = opus_decoder_get_size(channels);   // get size for n channel
  tasmota_log_C(LOG_LEVEL_DEBUG, "AUD: allocated %i bytes for Opus decoder", opus_size);
  void * buf = BE_EXPLICIT_MALLOC(opus_size);
  if (!buf) { berry_log_C("OPUS: out of memory"); }

  int err = opus_decoder_init((OpusDecoder*)buf, freq, channels);
  if (err) { berry_log_C("OPUS: opus_encoder_init error=%i", err); }

  return buf;
}
int be_audio_opus_decoder_init(struct bvm *vm) {
  return be_call_c_func(vm, (void*) &be_audio_opus_decoder_init_ntv, "+.p", "i[i]");
}

// deinit()
void *be_audio_opus_decoder_deinit_ntv(OpusDecoder* buf) {
  if (buf) BE_EXPLICIT_FREE(buf);
  return NULL;
}
int be_audio_opus_decoder_deinit(struct bvm *vm) {
  return be_call_c_func(vm, (void*) &be_audio_opus_decoder_deinit_ntv, "", ".");
}


// decode(payload:bytes) -> pcm:bytes()
int be_audio_opus_decoder_decode(struct bvm *vm) {
  int32_t argc = be_top(vm);
  be_call_c_func(vm, NULL, NULL, ".(bytes)[ii]");

  OpusDecoder* st = (OpusDecoder*) be_convert_single_elt(vm, 1, NULL, NULL);    // get value of '.p'
  size_t bytes_len;
  const uint8_t * opus_frame = be_tobytes(vm, 2, &bytes_len);
  int32_t frame_start = 0;
  int32_t frame_len = bytes_len;
  if (argc >= 3) { frame_start = be_toint(vm, 3); if (frame_start < 0) frame_start = 0; }
  if (argc >= 4) { frame_len = be_toint(vm, 4); }

  if (frame_start >= bytes_len) { frame_len = 0; }              // send empty packet
  else if (frame_len < 0) { frame_len = bytes_len - frame_start; }    // send all packet, adjust len
  else if (frame_start + frame_len > bytes_len) { frame_len = bytes_len - frame_start; }    // len is too long, adjust
  // adjust start
  opus_frame = opus_frame + frame_start;

  int samples = opus_decoder_get_nb_samples(st, opus_frame, frame_len);
  // tasmota_log_C(LOG_LEVEL_DEBUG, "AUD: frame contains %i samples", samples);

  // allocate a buffer for the content
  void * pcm = be_pushbytes(vm, NULL, samples * 2);

  int ret = opus_decode(st, opus_frame, frame_len, pcm, samples, 0);
  if (ret != samples) { be_raisef(vm, "internal_error", "wrong number of frames %i (supposed to be %i", ret, samples); }

  be_return(vm);
}

#include "be_fixed_be_class_AudioOpusDecoder.h"

/* @const_object_info_begin

class be_class_AudioOpusDecoder (scope: global, name: AudioOpusDecoder, strings: weak) {
  .p, var
  init, func(be_audio_opus_decoder_init)
  deinit, func(be_audio_opus_decoder_deinit)

  decode, func(be_audio_opus_decoder_decode)
}

@const_object_info_end */

#endif // USE_I2S_AUDIO_BERRY
#endif // USE_I2S