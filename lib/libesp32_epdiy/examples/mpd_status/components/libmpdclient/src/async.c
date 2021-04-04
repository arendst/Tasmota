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

#include "iasync.h"
#include "buffer.h"
#include "ierror.h"
#include "quote.h"
#include "socket.h"
#include "binary.h"

#include <mpd/socket.h>

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <stdarg.h>

#ifdef _WIN32
#  include <basetsd.h> /* for SSIZE_T */
#ifndef __MINGW32__
typedef SSIZE_T ssize_t;
#endif
#else
#  include <sys/socket.h>
#endif

#ifndef MSG_DONTWAIT
#define MSG_DONTWAIT 0
#endif

struct mpd_async {
	mpd_socket_t fd;

	struct mpd_error_info error;

	struct mpd_buffer input;

	struct mpd_buffer output;
};

struct mpd_async *
mpd_async_new(int fd)
{
	struct mpd_async *async;

	assert(fd != MPD_INVALID_SOCKET);

	async = malloc(sizeof(*async));
	if (async == NULL)
		return NULL;

	async->fd = fd;
	mpd_error_init(&async->error);

	mpd_buffer_init(&async->input);
	mpd_buffer_init(&async->output);

	return async;
}

void
mpd_async_free(struct mpd_async *async)
{
	assert(async != NULL);

	mpd_socket_close(async->fd);
	mpd_error_deinit(&async->error);
	free(async);
}

enum mpd_error
mpd_async_get_error(const struct mpd_async *async)
{
	assert(async != NULL);

	return async->error.code;
}

const char *
mpd_async_get_error_message(const struct mpd_async *async)
{
	assert(async != NULL);

	return mpd_error_get_message(&async->error);
}

int
mpd_async_get_system_error(const struct mpd_async *async)
{
	assert(async != NULL);
	assert(async->error.code == MPD_ERROR_SYSTEM);

	return async->error.system;
}

bool
mpd_async_copy_error(const struct mpd_async *async,
		     struct mpd_error_info *dest)
{
	assert(async != NULL);

	return mpd_error_copy(dest, &async->error);
}

int
mpd_async_get_fd(const struct mpd_async *async)
{
	assert(async != NULL);
	assert(async->fd != MPD_INVALID_SOCKET);

	return async->fd;
}

bool
mpd_async_set_keepalive(struct mpd_async *async,
			bool keepalive)
{
	assert(async != NULL);
	assert(async->fd != MPD_INVALID_SOCKET);

	return mpd_socket_keepalive(async->fd, keepalive) == 0;
}

enum mpd_async_event
mpd_async_events(const struct mpd_async *async)
{
	enum mpd_async_event events;

	assert(async != NULL);

	if (mpd_error_is_defined(&async->error))
		return 0;

	/* always listen to hangups and errors */
	events = MPD_ASYNC_EVENT_HUP | MPD_ASYNC_EVENT_ERROR;

	if (mpd_buffer_room(&async->input) > 0)
		/* there's room left in the input buffer: attempt to
		   read */
		events |= MPD_ASYNC_EVENT_READ;

	if (mpd_buffer_size(&async->output) > 0)
		/* there's data in the output buffer: attempt to
		   write */
		events |= MPD_ASYNC_EVENT_WRITE;

	return events;
}

static bool
mpd_async_read(struct mpd_async *async)
{
	size_t room;
	ssize_t nbytes;

	assert(async != NULL);
	assert(async->fd != MPD_INVALID_SOCKET);
	assert(!mpd_error_is_defined(&async->error));

	room = mpd_buffer_room(&async->input);
	if (room == 0)
		return true;

	nbytes = recv(async->fd, mpd_buffer_write(&async->input), room,
		      MSG_DONTWAIT);
	if (nbytes < 0) {
		/* I/O error */

		if (mpd_socket_ignore_errno(mpd_socket_errno()))
			return true;

		mpd_error_errno(&async->error);
		return false;
	}

	if (nbytes == 0) {
		mpd_error_code(&async->error, MPD_ERROR_CLOSED);
		mpd_error_message(&async->error,
				  "Connection closed by the server");
		return false;
	}

	mpd_buffer_expand(&async->input, (size_t)nbytes);
	return true;
}

static bool
mpd_async_write(struct mpd_async *async)
{
	size_t size;
	ssize_t nbytes;

	assert(async != NULL);
	assert(async->fd != MPD_INVALID_SOCKET);
	assert(!mpd_error_is_defined(&async->error));

	size = mpd_buffer_size(&async->output);
	if (size == 0)
		return true;

	nbytes = send(async->fd, mpd_buffer_read(&async->output), size,
		      MSG_DONTWAIT);
	if (nbytes < 0) {
		/* I/O error */

		if (mpd_socket_ignore_errno(mpd_socket_errno()))
			return true;

		mpd_error_errno(&async->error);
		return false;
	}

	mpd_buffer_consume(&async->output, (size_t)nbytes);
	return true;
}

bool
mpd_async_io(struct mpd_async *async, enum mpd_async_event events)
{
	bool success;

	assert(async != NULL);

	if (mpd_error_is_defined(&async->error))
		return false;

	if ((events & (MPD_ASYNC_EVENT_HUP|MPD_ASYNC_EVENT_ERROR)) != 0) {
		mpd_error_code(&async->error, MPD_ERROR_CLOSED);
		mpd_error_message(&async->error, "Socket connection aborted");
		return false;
	}

	if (events & MPD_ASYNC_EVENT_READ) {
		success = mpd_async_read(async);
		if (!success)
			return false;
	}

	assert(!mpd_error_is_defined(&async->error));

	if (events & MPD_ASYNC_EVENT_WRITE) {
		success = mpd_async_write(async);
		if (!success)
			return false;
	}

	assert(!mpd_error_is_defined(&async->error));

	return true;
}

bool
mpd_async_send_command_v(struct mpd_async *async, const char *command,
			 va_list args)
{
	size_t room, length;
	char *dest, *end, *p;
	const char *arg;

	assert(async != NULL);
	assert(command != NULL);

	if (mpd_error_is_defined(&async->error))
		return false;

	room = mpd_buffer_room(&async->output);
	length = strlen(command);
	if (room <= length)
		return false;

	dest = mpd_buffer_write(&async->output);
	/* -1 because we reserve space for the \n character */
	end = dest + room - 1;

	/* copy the command (no quoting, we asumme it is "clean") */

	memcpy(dest, command, length);
	p = dest + length;

	/* now append all arguments (quoted) */

	while ((arg = va_arg(args, const char *)) != NULL) {
		/* append a space separator */

		if (p >= end)
			return false;

		*p++ = ' ';

		/* quote the argument into the destination buffer */

		p = quote(p, end, arg);
		assert(p == NULL || (p >= dest && p <= end));
		if (p == NULL)
			return false;
	}


	/* append the newline to finish this command */

	*p++ = '\n';

	mpd_buffer_expand(&async->output, p - dest);
	return true;
}

bool
mpd_async_send_command(struct mpd_async *async, const char *command, ...)
{
	va_list args;
	bool success;

	assert(async != NULL);
	assert(command != NULL);

	va_start(args, command);
	success = mpd_async_send_command_v(async, command, args);
	va_end(args);

	return success;
}

char *
mpd_async_recv_line(struct mpd_async *async)
{
	size_t size;
	char *src, *newline;

	assert(async != NULL);

	size = mpd_buffer_size(&async->input);
	if (size == 0)
		return NULL;

	src = mpd_buffer_read(&async->input);
	assert(src != NULL);
	newline = memchr(src, '\n', size);
	if (newline == NULL) {
		/* line is not finished yet */
		if (mpd_buffer_full(&async->input)) {
			/* .. but the buffer is full - line is too
			   long, abort connection and bail out */
			mpd_error_code(&async->error, MPD_ERROR_MALFORMED);
			mpd_error_message(&async->error,
					  "Response line too large");
		}

		return NULL;
	}

	*newline = 0;
	mpd_buffer_consume(&async->input, newline + 1 - src);

	return src;
}

size_t
mpd_async_recv_raw(struct mpd_async *async, void *dest, size_t length)
{
	size_t max_size = mpd_buffer_size(&async->input);
	if (max_size == 0)
		return 0;

	if (length > max_size)
		length = max_size;

	memcpy(dest, mpd_buffer_read(&async->input), length);
	mpd_buffer_consume(&async->input, length);
	return length;
}
