#ifndef LAYER3_H
#define LAYER3_H

#include <stdint.h>

/* This is the struct used to tell the encoder about the input PCM */

//#define SHINE_DEBUG

enum channels {
  PCM_MONO   = 1,
  PCM_STEREO = 2
};

enum mpeg_versions {
  MPEG_I  = 3,
  MPEG_II = 2,
  MPEG_25 = 0
};

/* Only Layer III currently implemented. */
enum mpeg_layers {
  LAYER_III = 1
};

typedef struct {
    enum channels channels;
    int           samplerate;
} shine_wave_t;

/* This is the struct the encoder uses to tell the encoder about the output MP3 */

enum modes {
  STEREO       = 0,
  JOINT_STEREO = 1,
  DUAL_CHANNEL = 2,
  MONO         = 3
};

enum emph {
  NONE    = 0,
  MU50_15 = 1,
  CITT    = 3
};

typedef struct {
    enum modes mode;      /* Stereo mode */
    int        bitr;      /* Must conform to known bitrate */
    enum emph  emph;      /* De-emphasis */
    int        copyright;
    int        original;
} shine_mpeg_t;

typedef struct {
  shine_wave_t wave;
  shine_mpeg_t mpeg;
} shine_config_t;

/* Tables of supported audio parameters & format.
 *
 * Valid samplerates and bitrates.
 * const int samplerates[9] = {
 *   44100, 48000, 32000, // MPEG-I
 *   22050, 24000, 16000, // MPEG-II
 *   11025, 12000, 8000   // MPEG-2.5
 * };
 *
 * const int bitrates[16][4] = {
 * //  MPEG version:
 * //  2.5, reserved,  II,  I
 *   { -1,  -1,        -1,  -1},
 *   { 8,   -1,         8,  32},
 *   { 16,  -1,        16,  40},
 *   { 24,  -1,        24,  48},
 *   { 32,  -1,        32,  56},
 *   { 40,  -1,        40,  64},
 *   { 48,  -1,        48,  80},
 *   { 56,  -1,        56,  96},
 *   { 64,  -1,        64, 112},
 *   { 80,  -1,        80, 128},
 *   { 96,  -1,        96, 160},
 *   {112,  -1,       112, 192},
 *   {128,  -1,       128, 224},
 *   {144,  -1,       144, 256},
 *   {160,  -1,       160, 320},
 *   { -1,  -1,        -1,  -1}
 *  };
 *
 */


/* Abtract type for the shine encoder handle. */
typedef struct shine_global_flags *shine_t;

/* Fill in a `mpeg_t` structure with default values. */
void shine_set_config_mpeg_defaults(shine_mpeg_t *mpeg);

/* Check if a given bitrate is supported by the encoder (see `bitrates` above for a list
 * of acceptable values. */
int shine_find_bitrate_index(int bitr, int mpeg_version);

/* Check if a given samplerate is supported by the encoder (see `samplerates` above for a list
 * of acceptable values. */
int shine_find_samplerate_index(int freq);

/* Returns the MPEG version used for the given samplerate index. See above
 * `mpeg_versions` for a list of possible values. */
int shine_mpeg_version(int samplerate_index);

/* Check if a given bitrate and samplerate is supported by the encoder (see `samplerates`
 * and `bitrates` above for a list of acceptable values).
 *
 * Returns -1 on error, mpeg_version on success. */
int shine_check_config(int freq, int bitr);

/* Pass a pointer to a `config_t` structure and returns an initialized
 * encoder.
 *
 * Configuration data is copied over to the encoder. It is not possible
 * to change its values after initializing the encoder at the moment.
 *
 * Checking for valid configuration values is left for the application to
 * implement. You can use the `shine_find_bitrate_index` and
 * `shine_find_samplerate_index` functions or the `bitrates` and
 * `samplerates` arrays above to check those parameters. Mone and stereo
 * mode for wave and mpeg should also be consistent with each other.
 *
 * This function returns NULL if it was not able to allocate memory data for
 * the encoder. */
shine_t shine_initialise(shine_config_t *config);

/* Maximun possible value for the function below. */
#define SHINE_MAX_SAMPLES 1152

uint32_t *shine_get_counters();

/* Returns audio samples expected in each frame. */
int shine_samples_per_pass(shine_t s);

/* Encode audio data. Source data must have `shine_samples_per_pass(s)` audio samples per
 * channels. Mono encoder only expect one channel.
 *
 * Returns a pointer to freshly encoded data while `written` contains the size of
 * available data. This pointer's memory is handled by the library and is only valid
 * until the next call to `shine_encode_buffer` or `shine_close` and may be NULL if no data
 * was written. */
unsigned char *shine_encode_buffer(shine_t s, int16_t **data, int *written);

/* Encode interleaved audio data. Source data must have `shine_samples_per_pass(s)` audio samples per
 * channels. Mono encoder only expect one channel.
 *
 * Returns a pointer to freshly encoded data while `written` contains the size of
 * available data. This pointer's memory is handled by the library and is only valid
 * until the next call to `shine_encode_buffer` or `shine_close` and may be NULL if no data
 * was written. */
unsigned char *shine_encode_buffer_interleaved(shine_t s, int16_t *data, int *written);

/* Flush all data currently in the encoding buffer. Should be used before closing
 * the encoder, to make all encoded data has been written. */
unsigned char *shine_flush(shine_t s, int *written);

/* Close an encoder, freeing all associated memory. Encoder handler is not
 * valid after this call. */
void shine_close(shine_t s);

#endif
