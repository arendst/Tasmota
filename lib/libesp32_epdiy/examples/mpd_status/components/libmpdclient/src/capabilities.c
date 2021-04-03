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

#include <mpd/capabilities.h>
#include <mpd/send.h>
#include <mpd/recv.h>
#include <mpd/response.h>
#include "internal.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

bool
mpd_send_allowed_commands(struct mpd_connection *connection)
{
	return mpd_send_command(connection, "commands", NULL);
}

bool
mpd_send_disallowed_commands(struct mpd_connection *connection)
{
	return mpd_send_command(connection, "notcommands", NULL);
}

bool
mpd_send_list_url_schemes(struct mpd_connection *connection)
{
	return mpd_send_command(connection, "urlhandlers", NULL);
}

bool
mpd_send_list_tag_types(struct mpd_connection *connection)
{
	return mpd_send_command(connection, "tagtypes", NULL);
}

static bool
mpd_send_tag_types_v(struct mpd_connection *connection,
		     const char *sub_command,
		     const enum mpd_tag_type *types, unsigned n)
{
	assert(connection != NULL);
	assert(types != NULL);
	assert(n > 0);

	if (mpd_error_is_defined(&connection->error))
		return false;

	char buffer[1024] = "tagtypes ";
	strcat(buffer, sub_command);
	size_t length = strlen(buffer);

	for (unsigned i = 0; i < n; ++i) {
		const char *t = mpd_tag_name(types[i]);
		assert(t != NULL);
		size_t t_length = strlen(t);

		if (length + 1 + t_length + 1 > sizeof(buffer)) {
			mpd_error_code(&connection->error, MPD_ERROR_ARGUMENT);
			mpd_error_message(&connection->error,
					  "Tag list is too long");
			return false;
		}

		buffer[length++] = ' ';
		memcpy(buffer + length, t, t_length);
		length += t_length;
	}

	buffer[length] = 0;

	return mpd_send_command(connection, buffer, NULL);
}

bool
mpd_send_disable_tag_types(struct mpd_connection *connection,
			   const enum mpd_tag_type *types, unsigned n)
{
	return mpd_send_tag_types_v(connection, "disable", types, n);
}

bool
mpd_run_disable_tag_types(struct mpd_connection *connection,
			  const enum mpd_tag_type *types, unsigned n)
{
	return mpd_send_disable_tag_types(connection, types, n) &&
		mpd_response_finish(connection);
}

bool
mpd_send_enable_tag_types(struct mpd_connection *connection,
			  const enum mpd_tag_type *types, unsigned n)
{
	return mpd_send_tag_types_v(connection, "enable", types, n);
}

bool
mpd_run_enable_tag_types(struct mpd_connection *connection,
			 const enum mpd_tag_type *types, unsigned n)
{
	return mpd_send_enable_tag_types(connection, types, n) &&
		mpd_response_finish(connection);
}

bool
mpd_send_clear_tag_types(struct mpd_connection *connection)
{
	return mpd_send_command(connection, "tagtypes", "clear", NULL);
}

bool
mpd_run_clear_tag_types(struct mpd_connection *connection)
{
	return mpd_send_clear_tag_types(connection) &&
		mpd_response_finish(connection);
}
