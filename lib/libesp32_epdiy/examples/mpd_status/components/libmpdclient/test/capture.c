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

#include "capture.h"

#include <mpd/connection.h>
#include <mpd/async.h>

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#  include <winsock2.h>
#  include <basetsd.h> /* for SSIZE_T */
typedef SSIZE_T ssize_t;
#else
#  include <sys/socket.h>
#  include <unistd.h>
#endif

struct mpd_connection *
test_capture_init(struct test_capture *tc)
{
	int sv[2];

	if (socketpair(AF_LOCAL, SOCK_STREAM, 0, sv) < 0) {
		perror("socketpair() failed");
		return NULL;
	}

	tc->fd = sv[0];

	struct mpd_async *async = mpd_async_new(sv[1]);
	if (async == NULL) {
		close(sv[0]);
		close(sv[1]);
		return NULL;
	}

	struct mpd_connection *c =
		mpd_connection_new_async(async, "OK MPD 0.21.0");
	if (c == NULL) {
		mpd_async_free(async);
		close(sv[0]);
		return NULL;
	}

	return c;
}

void
test_capture_deinit(struct test_capture *tc)
{
	close(tc->fd);
}

const char *
test_capture_receive(struct test_capture *tc)
{
	ssize_t nbytes = recv(tc->fd, tc->buffer, sizeof(tc->buffer) - 1,
			      MSG_DONTWAIT);
	if (nbytes < 0) {
		perror("recv() failed");
		return NULL;
	}

	tc->buffer[nbytes] = 0;
	return tc->buffer;
}

bool
test_capture_send(struct test_capture *tc, const char *response)
{
	ssize_t nbytes = send(tc->fd, response, strlen(response),
			      MSG_DONTWAIT);
	if (nbytes < 0) {
		perror("send() failed");
		return false;
	}

	return true;
}
