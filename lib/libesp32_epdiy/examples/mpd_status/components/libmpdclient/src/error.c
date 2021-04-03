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

#include <mpd/connection.h>
#include "internal.h"

#include <assert.h>

enum mpd_error
mpd_connection_get_error(const struct mpd_connection *connection)
{
	return connection->error.code;
}

const char *
mpd_connection_get_error_message(const struct mpd_connection *connection)
{
	return mpd_error_get_message(&connection->error);
}

enum mpd_server_error
mpd_connection_get_server_error(const struct mpd_connection *connection)
{
	assert(connection->error.code == MPD_ERROR_SERVER);

	return connection->error.server;
}

unsigned
mpd_connection_get_server_error_location(const struct mpd_connection *connection)
{
	assert(connection->error.code == MPD_ERROR_SERVER);

	return connection->error.at;
}

int
mpd_connection_get_system_error(const struct mpd_connection *connection)
{
	assert(connection->error.code == MPD_ERROR_SYSTEM);

	return connection->error.system;
}

bool
mpd_connection_clear_error(struct mpd_connection *connection)
{
	if (mpd_error_is_fatal(&connection->error))
		/* impossible to recover */
		return false;

	mpd_error_clear(&connection->error);
	return true;
}
