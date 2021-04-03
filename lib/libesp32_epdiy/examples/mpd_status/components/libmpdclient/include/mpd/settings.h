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
 * Library to determine connection settings prior to calling
 * mpd_connection_new().
 *
 * Do not include this header directly.  Use mpd/client.h instead.
 */

#ifndef MPD_SETTINGS_H
#define MPD_SETTINGS_H

#include <stdbool.h>

/**
 * \struct mpd_settings
 *
 * An object which describes configurable connection settings.
 */
struct mpd_settings;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a new #mpd_settings object.  The values which are not
 * passed by the caller are taken from environment variables.
 *
 * @param host the server's host name, IP address or Unix socket path.
 * An address starting with '@' denotes an "abstract socket".
 * NULL is allowed here, which will connect to the default host
 * (using the MPD_HOST environment variable if present).
 * @param port the TCP port to connect to, 0 for default port (using
 * the MPD_PORT environment variable if present).  If "host" is a Unix
 * socket path, this parameter is ignored.
 * @param timeout_ms the timeout in milliseconds, 0 for the default
 * timeout (the environment variable MPD_TIMEOUT may specify a timeout
 * in seconds)
 * @param reserved reserved for future use, pass NULL
 * @param password the password, or NULL to use the default (MPD_HOST
 * before "@")
 * @return a #mpd_settings object or NULL if out of memory
 *
 * @since libmpdclient 2.4
 */
struct mpd_settings *
mpd_settings_new(const char *host, unsigned port, unsigned timeout_ms,
		 const char *reserved, const char *password);

/**
 * Releases a #mpd_settings object.
 *
 * @since libmpdclient 2.4
 */
void
mpd_settings_free(struct mpd_settings *settings);

/**
 * Returns the host name (without password/port), or NULL if unknown.
 *
 * @since libmpdclient 2.4
 */
const char *
mpd_settings_get_host(const struct mpd_settings *settings);

/**
 * Returns the port number, or 0 if not applicable.
 *
 * @since libmpdclient 2.4
 */
unsigned
mpd_settings_get_port(const struct mpd_settings *settings);

/**
 * Returns the timeout in milliseconds, or 0 if unknown.
 *
 * @since libmpdclient 2.4
 */
unsigned
mpd_settings_get_timeout_ms(const struct mpd_settings *settings);

/**
 * Returns the password, or NULL if none was configured.
 *
 * @since libmpdclient 2.4
 */
const char *
mpd_settings_get_password(const struct mpd_settings *settings);

#ifdef __cplusplus
}
#endif

#endif
