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

#ifndef MPD_IERROR_H
#define MPD_IERROR_H

#include <mpd/error.h>
#include <mpd/protocol.h>
#include <mpd/compiler.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * Structure which holds detailed information about an error, both
 * machine and human readable.
 */
struct mpd_error_info {
	/**
	 * The error code of this error.  None of the variables below
	 * are valid if this is set to MPD_ERROR_SUCCESS.
	 */
	enum mpd_error code;

	/**
	 * An ACK code returned by MPD.  This field is only valid if
	 * #code is MPD_ERROR_SERVER.
	 */
	enum mpd_server_error server;

	/**
	 * The command list index of the command which emitted this
	 * error.  Zero if no command list was used.
	 */
	int at;

	/**
	 * The operating system's error code (i.e. errno or WSAGetLastError()).
	 */
	int system;

	/**
	 * Human readable error message; may be NULL if not available.
	 * This pointer is allocated on the heap, and must be freed by
	 * calling mpd_error_clear() or mpd_error_deinit().
	 */
	char *message;
};

/**
 * Initialize a new mpd_error_info struct.
 */
static inline void
mpd_error_init(struct mpd_error_info *error)
{
	assert(error != NULL);

	error->code = MPD_ERROR_SUCCESS;
}

/**
 * Free memory allocated by an mpd_error_info struct.
 */
void
mpd_error_deinit(struct mpd_error_info *error);

/**
 * Clear the error (if any), and free its memory.
 */
static inline void
mpd_error_clear(struct mpd_error_info *error)
{
	mpd_error_deinit(error);

	error->code = MPD_ERROR_SUCCESS;
}

/**
 * Returns true if an error has occurred.
 */
static inline bool
mpd_error_is_defined(const struct mpd_error_info *error)
{
	return error->code != MPD_ERROR_SUCCESS;
}

/**
 * Returns true if the error is not recoverable
 */
static inline bool
mpd_error_is_fatal(const struct mpd_error_info *error)
{
	return error->code != MPD_ERROR_SUCCESS &&
		error->code != MPD_ERROR_ARGUMENT &&
		error->code != MPD_ERROR_STATE &&
		error->code != MPD_ERROR_SERVER;
}

static inline const char *
mpd_error_get_message(const struct mpd_error_info *error)
{
	assert(error != NULL);
	assert(error->code != MPD_ERROR_SUCCESS);
	assert(error->message != NULL || error->code == MPD_ERROR_OOM);

	if (error->message == NULL)
		return "Out of memory";

	return error->message;
}

/**
 * Sets an error code.
 */
static inline void
mpd_error_code(struct mpd_error_info *error, enum mpd_error code)
{
	assert(!mpd_error_is_defined(error));

	error->code = code;
	error->message = NULL;
}

/**
 * Sets an ACK error code.
 */
static inline void
mpd_error_server(struct mpd_error_info *error,
		 enum mpd_server_error server, int at)
{
	mpd_error_code(error, MPD_ERROR_SERVER);
	error->server = server;
	error->at = at;
}

/**
 * Sets an system error code.
 */
static inline void
mpd_error_system(struct mpd_error_info *error, int system_error_code)
{
	mpd_error_code(error, MPD_ERROR_SYSTEM);
	error->system = system_error_code;
}

/**
 * Sets an error message.  Prior to that, an error code must have been
 * set.
 */
void
mpd_error_message(struct mpd_error_info *error, const char *message);

/**
 * Sets an error message (non-terminated string with specified
 * length).  Prior to that, an error code must have been set.
 */
void
mpd_error_message_n(struct mpd_error_info *error,
		    const char *message, size_t length);

/**
 * Sets an error message (printf() like format).  Prior to that, an
 * error code must have been set.
 */
mpd_printf(2, 3)
void
mpd_error_printf(struct mpd_error_info *error, const char *fmt, ...);

/**
 * Sets MPD_ERROR_SYSTEM and a message provided by the OS.
 */
void
mpd_error_system_message(struct mpd_error_info *error, int code);

/**
 * Sets MPD_ERROR_SYSTEM and strerror(errno).
 */
void
mpd_error_errno(struct mpd_error_info *error);

/**
 * An entity parser has failed.  The error is set depending on the
 * value of errno (ENOMEM or EINVAL).
 */
void
mpd_error_entity(struct mpd_error_info *error);

/**
 * Copies a #mpd_error_info onto another one.  Duplicates the error
 * message.
 *
 * @return true if there was no error in #src, false if an error
 * condition is stored in #src (in both cases, the information is
 * copied)
 */
bool
mpd_error_copy(struct mpd_error_info *dest, const struct mpd_error_info *src);

#endif
