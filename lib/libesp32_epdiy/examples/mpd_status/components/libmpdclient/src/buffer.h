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

#ifndef MPD_BUFFER_H
#define MPD_BUFFER_H

#include <assert.h>
#include <string.h>
#include <stdbool.h>

/**
 * A fixed 4kB buffer which can be appended at the end, and consumed
 * at the beginning.
 */
struct mpd_buffer {
	/** the next buffer position to write to */
	unsigned write;

	/** the next buffer position to read from */
	unsigned read;

	/** the actual buffer */
	unsigned char data[4096];
};

/**
 * Initialize an empty buffer.
 */
static inline void
mpd_buffer_init(struct mpd_buffer *buffer)
{
	buffer->read = 0;
	buffer->write = 0;
}

/**
 * Move the start of the valid data to the beginning of the allocated
 * buffer.
 */
static inline void
mpd_buffer_move(struct mpd_buffer *buffer)
{
	memmove(buffer->data, buffer->data + buffer->read,
		buffer->write - buffer->read);

	buffer->write -= buffer->read;
	buffer->read = 0;
}

/**
 * Determines how many bytes can be written to the buffer returned by
 * mpd_buffer_write().
 */
static inline size_t
mpd_buffer_room(const struct mpd_buffer *buffer)
{
	assert(buffer->write <= sizeof(buffer->data));
	assert(buffer->read <= buffer->write);

	return sizeof(buffer->data) - (buffer->write - buffer->read);
}

/**
 * Checks if the buffer is full, i.e. nothing can be written.
 */
static inline bool
mpd_buffer_full(const struct mpd_buffer *buffer)
{
	return mpd_buffer_room(buffer) == 0;
}

/**
 * Returns a pointer to write new data into.  After you have done
 * that, call mpd_buffer_expand().
 */
static inline void *
mpd_buffer_write(struct mpd_buffer *buffer)
{
	assert(mpd_buffer_room(buffer) > 0);

	mpd_buffer_move(buffer);
	return buffer->data + buffer->write;
}

/**
 * Moves the "write" pointer.
 */
static inline void
mpd_buffer_expand(struct mpd_buffer *buffer, size_t nbytes)
{
	assert(mpd_buffer_room(buffer) >= nbytes);

	buffer->write += nbytes;
}

/**
 * Determines how many bytes can be read from the pointer returned by
 * mpd_buffer_read().
 */
static inline size_t
mpd_buffer_size(const struct mpd_buffer *buffer)
{
	assert(buffer->write <= sizeof(buffer->data));
	assert(buffer->read <= buffer->write);

	return buffer->write - buffer->read;
}

/**
 * Returns a pointer to the head of the filled buffer.  It is legal to
 * modify the returned buffer, for zero-copy parsing.
 */
static inline void *
mpd_buffer_read(struct mpd_buffer *buffer)
{
	assert(mpd_buffer_size(buffer) > 0);

	return buffer->data + buffer->read;
}

/**
 * Marks bytes at the beginning of the buffer as "consumed".
 */
static inline void
mpd_buffer_consume(struct mpd_buffer *buffer, size_t nbytes)
{
	assert(nbytes <= mpd_buffer_size(buffer));

	buffer->read += nbytes;
}

#endif
