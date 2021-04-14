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

   - Neither the name of the Music Player Daemon nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

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

#include "iaf.h"
#include <mpd/audio_format.h>

#include <stdlib.h>
#include <string.h>

void
mpd_parse_audio_format(struct mpd_audio_format *audio_format, const char *p)
{
	char *endptr;

	if (strncmp(p, "dsd", 3) == 0) {
		/* allow format specifications such as "dsd64" which
		   implies the sample rate */

		unsigned long dsd = strtoul(p + 3, &endptr, 10);
		if (endptr > p + 3 && *endptr == ':' &&
		    dsd >= 32 && dsd <= 4096 && dsd % 2 == 0) {
			audio_format->sample_rate = dsd * 44100 / 8;
			audio_format->bits = MPD_SAMPLE_FORMAT_DSD;

			p = endptr + 1;
			audio_format->channels = strtoul(p, NULL, 10);
			return;
		}
	}

	audio_format->sample_rate = strtoul(p, &endptr, 10);
	if (*endptr == ':') {
		p = endptr + 1;

		if (p[0] == 'f' && p[1] == ':') {
			audio_format->bits = MPD_SAMPLE_FORMAT_FLOAT;
			p += 2;
		} else if (p[0] == 'd' && p[1] == 's' &&
			   p[2] == 'd' && p[3] == ':') {
			audio_format->bits = MPD_SAMPLE_FORMAT_DSD;
			p += 4;
		} else {
			audio_format->bits = strtoul(p, &endptr, 10);
			p = *endptr == ':' ? endptr + 1 : NULL;
		}

		audio_format->channels = p != NULL
			? strtoul(p, NULL, 10)
			: 0;
	} else {
		audio_format->bits = 0;
		audio_format->channels = 0;
	}
}
