/* libmpdclient
   (c) 2003-2019 The Music Player Daemon Project
   This project's homepage is: http://www.musicpd.org

   Redistribution and use in so and binary forms, with or without
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

#include "ierror.h"
#include "socket.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

void
mpd_error_deinit(struct mpd_error_info *error)
{
	assert(error != NULL);

	if (error->code != MPD_ERROR_SUCCESS)
		free(error->message);
}

void
mpd_error_message(struct mpd_error_info *error, const char *message)
{
	assert(error != NULL);
	assert(message != NULL);
	assert(mpd_error_is_defined(error));
	assert(error->message == NULL);

	error->message = strdup(message);
	if (error->message == NULL)
		error->code = MPD_ERROR_OOM;
}

void
mpd_error_message_n(struct mpd_error_info *error,
		    const char *message, size_t length)
{
	assert(error != NULL);
	assert(message != NULL);
	assert(mpd_error_is_defined(error));
	assert(error->message == NULL);

	error->message = malloc(length + 1);
	if (error->message != NULL) {
		memcpy(error->message, message, length);
		error->message[length] = 0;
	} else
		error->code = MPD_ERROR_OOM;
}

void
mpd_error_printf(struct mpd_error_info *error, const char *fmt, ...)
{
	char buffer[1024];
	va_list ap;

	assert(error != NULL);
	assert(fmt != NULL);

	va_start(ap, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, ap);
	va_end(ap);

	mpd_error_message(error, buffer);
}

void
mpd_error_system_message(struct mpd_error_info *error, int code)
{
#ifdef _WIN32
	char buffer[1024];
	DWORD nbytes;
#endif

	assert(error != NULL);

	mpd_error_system(error, code);

#ifdef _WIN32
	nbytes = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
			       FORMAT_MESSAGE_IGNORE_INSERTS |
			       FORMAT_MESSAGE_MAX_WIDTH_MASK, NULL, code, 0,
			       (LPSTR)buffer, sizeof(buffer), NULL);
	mpd_error_message(error, nbytes > 0 ? buffer : "Unknown error");
#else
	mpd_error_message(error, strerror(code));
#endif
}

void
mpd_error_errno(struct mpd_error_info *error)
{
	assert(error != NULL);

	mpd_error_system_message(error, mpd_socket_errno());
}

void
mpd_error_entity(struct mpd_error_info *error)
{
	if (errno == EINVAL) {
		mpd_error_code(error, MPD_ERROR_MALFORMED);
		mpd_error_message(error, "Malformed entity response line");
	} else
		mpd_error_code(error, MPD_ERROR_OOM);
}

bool
mpd_error_copy(struct mpd_error_info *dest, const struct mpd_error_info *src)
{
	assert(dest != NULL);
	assert(src != NULL);

	dest->code = src->code;
	if (src->code == MPD_ERROR_SUCCESS)
		return true;

	if (src->code == MPD_ERROR_SERVER) {
		dest->server = src->server;
		dest->at = src->at;
	} else if (src->code == MPD_ERROR_SYSTEM)
		dest->system = src->system;

	dest->message = src->message != NULL ? strdup(src->message) : NULL;
	return false;
}
