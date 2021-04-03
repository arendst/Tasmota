/* libmpdclient
   (c) 2003-2019 The Music Player Daemon Project
   This project's homepage is: http://www.musicpd.org

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*! \file
 * \brief MPD client library
 *
 * Do not include this header directly.  Use mpd/client.h instead.
 */

#ifndef MPD_AUDIO_FORMAT_H
#define MPD_AUDIO_FORMAT_H

#include <stdint.h>

enum {
	/**
	 * The sample format is unknown or unspecified.
	 *
	 * @since libmpdclient 2.15
	 */
	MPD_SAMPLE_FORMAT_UNDEFINED = 0x00,

	/**
	 * 32 bit floating point samples.
	 */
	MPD_SAMPLE_FORMAT_FLOAT = 0xe0,

	/**
	 * DSD samples.
	 */
	MPD_SAMPLE_FORMAT_DSD = 0xe1,
};

/**
 * This structure describes the format of a raw PCM stream.
 */
struct mpd_audio_format {
	/**
	 * The sample rate in Hz.  A better name for this attribute is
	 * "frame rate", because technically, you have two samples per
	 * frame in stereo sound.
	 *
	 * The special value 0 means "unknown or unspecified".
	 */
	uint32_t sample_rate;

	/**
	 * The number of significant bits per sample.  Samples are
	 * currently always signed.  Supported values are 8, 16, 24,
	 * 32 and the special values #MPD_SAMPLE_FORMAT_FLOAT,
	 * #MPD_SAMPLE_FORMAT_DSD, #MPD_SAMPLE_FORMAT_UNDEFINED.
	 *
	 * @since libmpdclient 2.10 added support for #MPD_SAMPLE_FORMAT_FLOAT and
	 * #MPD_SAMPLE_FORMAT_DSD.
	 */
	uint8_t bits;

	/**
	 * The number of channels.  Only mono (1) and stereo (2) are
	 * fully supported currently.
	 *
	 * The special value 0 means "unknown or unspecified".
	 */
	uint8_t channels;

	/** reserved for future use */
	uint16_t reserved0;

	/** reserved for future use */
	uint32_t reserved1;
};

#endif
