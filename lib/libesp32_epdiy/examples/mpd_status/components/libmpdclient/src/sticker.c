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

#include <mpd/sticker.h>
#include <mpd/connection.h>
#include <mpd/send.h>
#include <mpd/recv.h>
#include <mpd/pair.h>
#include <mpd/response.h>
#include "run.h"

#include <assert.h>
#include <string.h>

bool
mpd_send_sticker_set(struct mpd_connection *connection, const char *type,
		     const char *uri, const char *name, const char *value)
{
	return mpd_send_command(connection, "sticker", "set",
				type, uri, name, value, NULL);
}

bool
mpd_run_sticker_set(struct mpd_connection *connection, const char *type,
		    const char *uri, const char *name, const char *value)
{
	return mpd_run_check(connection) &&
		mpd_send_sticker_set(connection, type, uri, name, value) &&
		mpd_response_finish(connection);
}

bool
mpd_send_sticker_delete(struct mpd_connection *connection, const char *type,
			const char *uri, const char *name)
{
	assert(connection != NULL);
	assert(type != NULL);
	assert(uri != NULL);
	assert(name != NULL);

	return mpd_send_command(connection, "sticker", "delete",
				type, uri, name, NULL);
}

bool
mpd_run_sticker_delete(struct mpd_connection *connection, const char *type,
		       const char *uri, const char *name)
{
	return mpd_run_check(connection) &&
		mpd_send_sticker_delete(connection, type, uri, name) &&
		mpd_response_finish(connection);
}

bool
mpd_send_sticker_get(struct mpd_connection *connection, const char *type,
		     const char *uri, const char *name)
{
	assert(connection != NULL);
	assert(type != NULL);
	assert(uri != NULL);
	assert(name != NULL);

	return mpd_send_command(connection, "sticker", "get",
				type, uri, name, NULL);
}

bool
mpd_send_sticker_list(struct mpd_connection *connection, const char *type, const char *uri)
{
	assert(connection != NULL);
	assert(type != NULL);
	assert(uri != NULL);

	return mpd_send_command(connection, "sticker", "list",
				type, uri, NULL);
}

bool
mpd_send_sticker_find(struct mpd_connection *connection, const char *type,
		      const char *base_uri, const char *name)
{
	assert(connection != NULL);
	assert(type != NULL);
	assert(name != NULL);

	if (base_uri == NULL)
		base_uri = "";

	return mpd_send_command(connection, "sticker", "find",
				type, base_uri, name, NULL);
}

const char *
mpd_parse_sticker(const char *input, size_t *name_length_r)
{
	const char *eq;

	eq = strchr(input, '=');
	if (eq == NULL || eq == input)
		return NULL;

	*name_length_r = eq - input;
	return eq + 1;
}

struct mpd_pair *
mpd_recv_sticker(struct mpd_connection *connection)
{
	struct mpd_pair *pair;
	char *eq;

	pair = mpd_recv_pair_named(connection, "sticker");
	if (pair == NULL)
		return NULL;

	pair->name = pair->value;

	eq = strchr(pair->value, '=');
	if (eq != NULL) {
		/* we shouldn't modify a const string, but in this
		   case, we know that this points to the writable
		   input buffer */
		*eq = 0;
		pair->value = eq + 1;
	} else
		/* malformed response?  what to do now?  pretend
		   nothing has happened... */
		pair->value = "";

	return pair;
}

void
mpd_return_sticker(struct mpd_connection *connection, struct mpd_pair *pair)
{
	mpd_return_pair(connection, pair);
}
