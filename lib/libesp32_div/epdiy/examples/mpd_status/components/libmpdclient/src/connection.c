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

#include <mpd/connection.h>
#include <mpd/settings.h>
#include <mpd/async.h>
#include <mpd/parser.h>
#include <mpd/password.h>
#include <mpd/socket.h>

#include "resolver.h"
#include "sync.h"
#include "socket.h"
#include "internal.h"
#include "iasync.h"
#include "config.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define MPD_WELCOME_MESSAGE	"OK MPD "

static bool
mpd_parse_welcome(struct mpd_connection *connection, const char *output)
{
	const char *tmp;
	char * test;

	if (strncmp(output,MPD_WELCOME_MESSAGE,strlen(MPD_WELCOME_MESSAGE))) {
		mpd_error_code(&connection->error, MPD_ERROR_MALFORMED);
		mpd_error_message(&connection->error,
				  "Malformed connect message received");
		return false;
	}

	tmp = &output[strlen(MPD_WELCOME_MESSAGE)];
	connection->version[0] = strtoul(tmp, &test, 10);
	if (test == tmp) {
		mpd_error_code(&connection->error, MPD_ERROR_MALFORMED);
		mpd_error_message(&connection->error,
				  "Malformed version number in connect message");
		return false;
	}

	if (*test == '.') {
		connection->version[1] = strtoul(test + 1, &test, 10);
		if (*test == '.')
			connection->version[2] = strtoul(test + 1, &test, 10);
		else
			connection->version[2] = 0;
	} else {
		connection->version[1] = 0;
		connection->version[2] = 0;
	}

	return true;
}

void
mpd_connection_sync_error(struct mpd_connection *connection)
{
	if (mpd_async_copy_error(connection->async, &connection->error)) {
		/* no error noticed by async: must be a timeout in the
		   sync.c code */
		mpd_error_code(&connection->error, MPD_ERROR_TIMEOUT);
		mpd_error_message(&connection->error, "Timeout");
	}
}

struct mpd_connection *
mpd_connection_new(const char *host, unsigned port, unsigned timeout_ms)
{
	struct mpd_settings *settings =
		mpd_settings_new(host, port, timeout_ms, NULL, NULL);
	if (settings == NULL)
		return NULL;

	struct mpd_connection *connection = malloc(sizeof(*connection));
	if (connection == NULL) {
		mpd_settings_free(settings);
		return NULL;
	}

	connection->settings = settings;

	bool success;
	mpd_socket_t fd;
	const char *line;

	mpd_error_init(&connection->error);
	connection->async = NULL;
	connection->parser = NULL;
	connection->receiving = false;
	connection->sending_command_list = false;
	connection->pair_state = PAIR_STATE_NONE;
	connection->request = NULL;

	if (!mpd_socket_global_init(&connection->error))
		return connection;

	mpd_connection_set_timeout(connection,
				   mpd_settings_get_timeout_ms(settings));

	host = mpd_settings_get_host(settings);
	fd = mpd_socket_connect(host, mpd_settings_get_port(settings),
				&connection->timeout, &connection->error);
	if (fd == MPD_INVALID_SOCKET) {
#if defined(DEFAULT_SOCKET) && defined(ENABLE_TCP)
		if (host == NULL || strcmp(host, DEFAULT_SOCKET) == 0) {
			/* special case: try the default host if the
			   default socket failed */
			mpd_settings_free(settings);
			settings = mpd_settings_new(DEFAULT_HOST, DEFAULT_PORT,
						    timeout_ms, NULL, NULL);
			if (settings == NULL) {
				mpd_error_code(&connection->error,
					       MPD_ERROR_OOM);
				return connection;
			}
			connection->settings = settings;

			mpd_error_clear(&connection->error);
			fd = mpd_socket_connect(DEFAULT_HOST, DEFAULT_PORT,
						&connection->timeout,
						&connection->error);
		}
#endif

		if (fd == MPD_INVALID_SOCKET)
			return connection;
	}

	connection->async = mpd_async_new(fd);
	if (connection->async == NULL) {
		mpd_socket_close(fd);
		mpd_error_code(&connection->error, MPD_ERROR_OOM);
		return connection;
	}

	connection->parser = mpd_parser_new();
	if (connection->parser == NULL) {
		mpd_error_code(&connection->error, MPD_ERROR_OOM);
		return connection;
	}

	line = mpd_sync_recv_line(connection->async, &connection->timeout);
	if (line == NULL) {
		mpd_connection_sync_error(connection);
		return connection;
	}

	success = mpd_parse_welcome(connection, line);

	if (success) {
		const char *password = mpd_settings_get_password(settings);
		if (password != NULL)
			mpd_run_password(connection, password);
	}

	return connection;
}

struct mpd_connection *
mpd_connection_new_async(struct mpd_async *async, const char *welcome)
{
	struct mpd_connection *connection = malloc(sizeof(*connection));

	assert(async != NULL);
	assert(welcome != NULL);

	if (connection == NULL)
		return NULL;

	mpd_error_init(&connection->error);
	connection->settings = NULL;
	connection->async = async;
	connection->timeout.tv_sec = 30;
	connection->timeout.tv_usec = 0;
	connection->parser = NULL;
	connection->receiving = false;
	connection->sending_command_list = false;
	connection->pair_state = PAIR_STATE_NONE;
	connection->request = NULL;

	if (!mpd_socket_global_init(&connection->error))
		return connection;

	connection->parser = mpd_parser_new();
	if (connection->parser == NULL) {
		mpd_error_code(&connection->error, MPD_ERROR_OOM);
		return connection;
	}

	mpd_parse_welcome(connection, welcome);

	return connection;
}

void mpd_connection_free(struct mpd_connection *connection)
{
	assert(connection->pair_state != PAIR_STATE_FLOATING);

	if (connection->parser != NULL)
		mpd_parser_free(connection->parser);

	if (connection->async != NULL)
		mpd_async_free(connection->async);

	if (connection->request) free(connection->request);

	mpd_error_deinit(&connection->error);

	if (connection->settings != NULL)
		mpd_settings_free(connection->settings);

	free(connection);
}

bool
mpd_connection_set_keepalive(struct mpd_connection *connection,
			     bool keepalive)
{
	assert(connection != NULL);

	return mpd_async_set_keepalive(connection->async, keepalive);
}

const struct mpd_settings *
mpd_connection_get_settings(const struct mpd_connection *connection)
{
	assert(connection != NULL);

	return connection->settings;
}

void
mpd_connection_set_timeout(struct mpd_connection *connection,
			   unsigned timeout_ms)
{
	assert(timeout_ms > 0);

	connection->timeout.tv_sec = timeout_ms / 1000;
	connection->timeout.tv_usec = timeout_ms % 1000;
}

int
mpd_connection_get_fd(const struct mpd_connection *connection)
{
	return mpd_async_get_fd(connection->async);
}

struct mpd_async *
mpd_connection_get_async(struct mpd_connection *connection)
{
	return connection->async;
}

const unsigned *
mpd_connection_get_server_version(const struct mpd_connection *connection)
{
	return connection->version;
}

int
mpd_connection_cmp_server_version(const struct mpd_connection *connection,
				  unsigned major, unsigned minor,
				  unsigned patch)
{
	const unsigned *v = connection->version;

	if (v[0] > major || (v[0] == major &&
			     (v[1] > minor || (v[1] == minor &&
					       v[2] > patch))))
		return 1;
	else if (v[0] == major && v[1] == minor && v[2] == patch)
		return 0;
	else
		return -1;
}
