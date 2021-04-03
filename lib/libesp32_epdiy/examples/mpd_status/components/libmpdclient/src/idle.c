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

#include <mpd/idle.h>
#include <mpd/send.h>
#include <mpd/connection.h>
#include <mpd/pair.h>
#include <mpd/recv.h>
#include <mpd/response.h>
#include "internal.h"
#include "isend.h"
#include "run.h"

#include <string.h>

static const char *const idle_names[] = {
	"database",
	"stored_playlist",
	"playlist",
	"player",
	"mixer",
	"output",
	"options",
	"update",
	"sticker",
	"subscription",
	"message",
	"partition",
	NULL
};

const char *
mpd_idle_name(enum mpd_idle idle)
{
	for (unsigned i = 0; idle_names[i] != NULL; ++i)
		if (idle == (enum mpd_idle)(1 << i))
			return idle_names[i];

	return NULL;
}

enum mpd_idle
mpd_idle_name_parse(const char *name)
{
	assert(name != NULL);

	for (unsigned i = 0; idle_names[i] != NULL; ++i)
		if (strcmp(name, idle_names[i]) == 0)
			return 1 << i;

	return 0;
}

enum mpd_idle
mpd_idle_parse_pair(const struct mpd_pair *pair)
{
	assert(pair != NULL);

	if (strcmp(pair->name, "changed") != 0)
		return 0;

	return mpd_idle_name_parse(pair->value);
}

enum mpd_idle
mpd_recv_idle(struct mpd_connection *connection, bool disable_timeout)
{
	enum mpd_idle flags = 0;
	struct mpd_pair *pair;
	struct timeval old_timeout;

	assert(connection != NULL);

	/* temporarily disable the connection timeout */
	if (disable_timeout) {
		/* make sure that the output buffer is empty before we
		   turn off the timeout - this is important because we
		   want to detect a send failure more quickly */
		if (!mpd_flush(connection))
			return 0;

		old_timeout = connection->timeout;
		connection->timeout = (struct timeval){
			.tv_sec = 0,
			.tv_usec = 0,
		};
	} else {
		/* work around bogus gcc warning "may be used
		   uninitialized"; gcc is too dumb to see that we're
		   accessing old_timeout only if it was initialized */
		old_timeout = (struct timeval){
			.tv_sec = 0,
			.tv_usec = 0,
		};
	}

	while ((pair = mpd_recv_pair(connection)) != NULL) {
		flags |= mpd_idle_parse_pair(pair);

		mpd_return_pair(connection, pair);
	}

	/* re-enable timeout */
	if (disable_timeout)
		connection->timeout = old_timeout;

	return flags;
}

bool
mpd_send_idle(struct mpd_connection *connection)
{
	return mpd_send_command(connection, "idle", NULL);
}

bool
mpd_send_idle_mask(struct mpd_connection *connection, enum mpd_idle mask)
{
	/* this buffer is large enough even for the full mask */
	char buffer[128] = "idle";

	assert(mask != 0);

	if (mpd_error_is_defined(&connection->error))
		return false;

	for (unsigned i = 0; idle_names[i] != NULL; ++i) {
		if (mask & (1 << i)) {
			mask &= ~(1 << i);
			strcat(buffer, " ");
			strcat(buffer, idle_names[i]);
		}
	}

	if (mask != 0) {
		/* the client expects that all flags are supported,
		   because he might block forever if an event is not
		   delivered as expected */
		mpd_error_code(&connection->error, MPD_ERROR_ARGUMENT);
		mpd_error_printf(&connection->error,
				 "Unsupported idle flags: 0x%x", mask);
		return false;
	}

	return mpd_send_command(connection, buffer, NULL);
}

bool
mpd_send_noidle(struct mpd_connection *connection)
{
	/* we need this workaround, because "noidle" may be sent while
	   the "idle" response is being received */
	connection->receiving = false;

	return mpd_send_command(connection, "noidle", NULL);
}

enum mpd_idle
mpd_run_idle(struct mpd_connection *connection)
{
	enum mpd_idle flags;

	if (!mpd_run_check(connection) || !mpd_send_idle(connection))
		return 0;

	flags = mpd_recv_idle(connection, true);
	if (!mpd_response_finish(connection))
		return 0;

	return flags;
}

enum mpd_idle
mpd_run_idle_mask(struct mpd_connection *connection, enum mpd_idle mask)
{
	enum mpd_idle flags;

	if (!mpd_run_check(connection) ||
	    !mpd_send_idle_mask(connection, mask))
		return 0;

	flags = mpd_recv_idle(connection, true);
	if (!mpd_response_finish(connection))
		return 0;

	return flags;
}

enum mpd_idle
mpd_run_noidle(struct mpd_connection *connection)
{
	enum mpd_idle flags;

	if (!mpd_run_check(connection) || !mpd_send_noidle(connection))
		return 0;

	flags = mpd_recv_idle(connection, false);
	if (!mpd_response_finish(connection))
		return 0;

	return flags;
}
