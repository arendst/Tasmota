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

#include <mpd/search.h>
#include <mpd/send.h>
#include <mpd/pair.h>
#include <mpd/recv.h>
#include "internal.h"
#include "iso8601.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static bool
mpd_search_init(struct mpd_connection *connection, const char *cmd)
{
	assert(connection != NULL);
	assert(cmd != NULL);

	if (mpd_error_is_defined(&connection->error))
		return false;

	if (connection->request) {
		mpd_error_code(&connection->error, MPD_ERROR_STATE);
		mpd_error_message(&connection->error,
				  "search already in progress");
		return false;
	}

	connection->request = strdup(cmd);
	if (connection->request == NULL) {
		mpd_error_code(&connection->error, MPD_ERROR_OOM);
		return false;
	}

	return true;
}

bool
mpd_search_db_songs(struct mpd_connection *connection, bool exact)
{
	return mpd_search_init(connection,
			       exact ? "find" : "search");
}

bool
mpd_search_add_db_songs(struct mpd_connection *connection, bool exact)
{
	return mpd_search_init(connection,
			       exact ? "findadd" : "searchadd");
}

bool
mpd_search_queue_songs(struct mpd_connection *connection, bool exact)
{
	return mpd_search_init(connection,
			       exact ? "playlistfind" : "playlistsearch");
}

bool
mpd_search_db_tags(struct mpd_connection *connection, enum mpd_tag_type type)
{
	assert(connection != NULL);

	if (mpd_error_is_defined(&connection->error))
		return false;

	if (connection->request) {
		mpd_error_code(&connection->error, MPD_ERROR_STATE);
		mpd_error_message(&connection->error,
				  "search already in progress");
		return false;
	}

	const char *strtype = mpd_tag_name(type);
	if (strtype == NULL) {
		mpd_error_code(&connection->error, MPD_ERROR_ARGUMENT);
		mpd_error_message(&connection->error,
				  "invalid type specified");
		return false;
	}

	const size_t len = 5 + strlen(strtype) + 1;
	connection->request = malloc(len);
	if (connection->request == NULL) {
		mpd_error_code(&connection->error, MPD_ERROR_OOM);
		return false;
	}

	snprintf(connection->request, len, "list %s", strtype);

	return true;
}

bool
mpd_count_db_songs(struct mpd_connection *connection)
{
	assert(connection != NULL);

	return mpd_search_init(connection, "count");
}

static char *
mpd_search_prepare_append(struct mpd_connection *connection,
			  size_t add_length)
{
	assert(connection != NULL);

	if (mpd_error_is_defined(&connection->error))
		return NULL;

	if (connection->request == NULL) {
		mpd_error_code(&connection->error, MPD_ERROR_STATE);
		mpd_error_message(&connection->error,
				  "no search in progress");
		return NULL;
	}

	const size_t old_length = strlen(connection->request);
	char *new_request = realloc(connection->request,
				    old_length + add_length + 1);
	if (new_request == NULL) {
		mpd_error_code(&connection->error, MPD_ERROR_OOM);
		return NULL;
	}

	connection->request = new_request;
	return new_request + old_length;
}

static char *
mpd_sanitize_arg(const char *src)
{
	assert(src != NULL);

	/* instead of counting in that loop above, just
	 * use a bit more memory and half running time
	 */
	char *result = malloc(strlen(src) * 2 + 1);
	if (result == NULL)
		return NULL;

	char *dest = result;
	char ch;
	do {
		ch = *src++;
		if (ch == '"' || ch == '\\')
			*dest++ = '\\';
		*dest++ = ch;
	} while (ch != 0);

	return result;
}

static bool
mpd_search_add_constraint(struct mpd_connection *connection,
			  mpd_unused enum mpd_operator oper,
			  const char *name,
			  const char *value)
{
	assert(connection != NULL);
	assert(name != NULL);
	assert(value != NULL);

	char *arg = mpd_sanitize_arg(value);
	if (arg == NULL) {
		mpd_error_code(&connection->error, MPD_ERROR_OOM);
		return false;
	}

	const size_t add_length = 1 + strlen(name) + 2 + strlen(arg) + 1;

	char *dest = mpd_search_prepare_append(connection, add_length);
	if (dest == NULL) {
		free(arg);
		return false;
	}

	sprintf(dest, " %s \"%s\"", name, arg);

	free(arg);
	return true;
}
bool
mpd_search_add_base_constraint(struct mpd_connection *connection,
			       enum mpd_operator oper,
			       const char *value)
{
	return mpd_search_add_constraint(connection, oper, "base", value);
}

bool
mpd_search_add_uri_constraint(struct mpd_connection *connection,
			      enum mpd_operator oper,
			      const char *value)
{
	return mpd_search_add_constraint(connection, oper, "file", value);
}

bool
mpd_search_add_tag_constraint(struct mpd_connection *connection,
			      enum mpd_operator oper,
			      enum mpd_tag_type type, const char *value)
{
	assert(connection != NULL);
	assert(value != NULL);

	const char *strtype = mpd_tag_name(type);
	if (strtype == NULL) {
		mpd_error_code(&connection->error, MPD_ERROR_ARGUMENT);
		mpd_error_message(&connection->error,
				  "invalid type specified");
		return false;
	}

	return mpd_search_add_constraint(connection, oper, strtype, value);
}

bool
mpd_search_add_any_tag_constraint(struct mpd_connection *connection,
				  enum mpd_operator oper,
				  const char *value)
{
	return mpd_search_add_constraint(connection, oper, "any", value);
}

bool
mpd_search_add_modified_since_constraint(struct mpd_connection *connection,
					 enum mpd_operator oper,
					 time_t value)
{
	char buffer[64];
	if (!iso8601_datetime_format(buffer, sizeof(buffer), value)) {
		mpd_error_code(&connection->error, MPD_ERROR_ARGUMENT);
		mpd_error_message(&connection->error,
				  "failed to format time stamp");
		return false;
	}

	return mpd_search_add_constraint(connection, oper,
					 "modified-since", buffer);
}

bool
mpd_search_add_expression(struct mpd_connection *connection,
			  const char *expression)
{
	assert(connection != NULL);
	assert(expression != NULL);

	char *arg = mpd_sanitize_arg(expression);
	if (arg == NULL) {
		mpd_error_code(&connection->error, MPD_ERROR_OOM);
		return false;
	}

	const size_t add_length = 2 + strlen(arg) + 1;

	char *dest = mpd_search_prepare_append(connection, add_length);
	if (dest == NULL) {
		free(arg);
		return false;
	}

	sprintf(dest, " \"%s\"", arg);

	free(arg);
	return true;
}

bool
mpd_search_add_group_tag(struct mpd_connection *connection,
			 enum mpd_tag_type type)
{
	assert(connection != NULL);

	const size_t size = 64;
	char *dest = mpd_search_prepare_append(connection, size);
	if (dest == NULL)
		return false;

	snprintf(dest, size, " group %s", mpd_tag_name(type));
	return true;
}

bool
mpd_search_add_sort_name(struct mpd_connection *connection,
			 const char *name, bool descending)
{
	assert(connection != NULL);

	const size_t size = 64;
	char *dest = mpd_search_prepare_append(connection, size);
	if (dest == NULL)
		return false;

	snprintf(dest, size, " sort %s%s",
		 descending ? "-" : "",
		 name);
	return true;
}

bool
mpd_search_add_sort_tag(struct mpd_connection *connection,
			enum mpd_tag_type type, bool descending)
{
	return mpd_search_add_sort_name(connection,
					mpd_tag_name(type),
					descending);
}

bool
mpd_search_add_window(struct mpd_connection *connection,
		      unsigned start, unsigned end)
{
	assert(connection != NULL);
	assert(start <= end);

	const size_t size = 64;
	char *dest = mpd_search_prepare_append(connection, size);
	if (dest == NULL)
		return false;

	snprintf(dest, size, " window %u:%u", start, end);
	return true;
}

bool
mpd_search_commit(struct mpd_connection *connection)
{
	assert(connection != NULL);

	if (mpd_error_is_defined(&connection->error)) {
		mpd_search_cancel(connection);
		return false;
	}

	if (connection->request == NULL) {
		mpd_error_code(&connection->error, MPD_ERROR_STATE);
		mpd_error_message(&connection->error,
				  "no search in progress");
		return false;
	}

	bool success = mpd_send_command(connection, connection->request, NULL);
	free(connection->request);
	connection->request = NULL;

	return success;
}

void
mpd_search_cancel(struct mpd_connection *connection)
{
	assert(connection != NULL);

	free(connection->request);
	connection->request = NULL;
}

struct mpd_pair *
mpd_recv_pair_tag(struct mpd_connection *connection, enum mpd_tag_type type)
{
	assert(connection != NULL);

	const char *name = mpd_tag_name(type);
	if (name == NULL)
		return NULL;

	return mpd_recv_pair_named(connection, name);
}

bool
mpd_search_add_db_songs_to_playlist(struct mpd_connection *connection,
				    const char *playlist_name)
{
	assert(connection != NULL);
	assert(playlist_name != NULL);

	if (mpd_error_is_defined(&connection->error))
		return false;

	if (connection->request) {
		mpd_error_code(&connection->error, MPD_ERROR_STATE);
		mpd_error_message(&connection->error,
				  "search already in progress");
		return false;
	}

	char *arg = mpd_sanitize_arg(playlist_name);
	if (arg == NULL) {
		mpd_error_code(&connection->error, MPD_ERROR_OOM);
		return false;
	}

	const size_t len = 13 + strlen(arg) + 2;
	connection->request = malloc(len);
	if (connection->request == NULL) {
		free(arg);
		mpd_error_code(&connection->error, MPD_ERROR_OOM);
		return false;
	}

	snprintf(connection->request, len, "searchaddpl \"%s\" ", arg);

	free(arg);
	return true;
}
