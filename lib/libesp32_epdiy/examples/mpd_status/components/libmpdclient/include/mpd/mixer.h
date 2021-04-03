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
 * Functions for manipulating MPD's mixer controls.
 *
 * Do not include this header directly.  Use mpd/client.h instead.
 */

#ifndef MPD_MIXER_H
#define MPD_MIXER_H

#include <stdbool.h>

struct mpd_connection;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Sets the volume of all output devices.
 *
 * @param connection the connection to MPD
 * @param volume the volume, an integer between 0 and 100
 * @return true on success, false on error
 */
bool
mpd_send_set_volume(struct mpd_connection *connection, unsigned volume);

/**
 * Shortcut for mpd_send_set_volume() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @param volume the volume, an integer between 0 and 100
 * @return true on success, false on error
 */
bool
mpd_run_set_volume(struct mpd_connection *connection, unsigned volume);

/**
 * Changes the volume of all output devices.
 *
 * @param connection the connection to MPD
 * @param relative_volume the relative volume, an integer between -100 and 100
 * @return true on success, false on error
 *
 * @since libmpdclient 2.9
 */
bool
mpd_send_change_volume(struct mpd_connection *connection, int relative_volume);

/**
 * Shortcut for mpd_send_change_volume() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @param relative_volume the relative volume, an integer between -100 and 100
 * @return true on success, false on error
 *
 * @since libmpdclient 2.9
 */
bool
mpd_run_change_volume(struct mpd_connection *connection, int relative_volume);

#ifdef __cplusplus
}
#endif

#endif
