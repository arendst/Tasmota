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

#include "resolver.h"
#include "config.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#  include <winsock2.h>
#  include <ws2tcpip.h>
#else
#  include <sys/socket.h>
#  include <sys/un.h>
#ifdef ENABLE_TCP
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <netdb.h>
#endif
#endif

struct resolver {
	enum {
		TYPE_ZERO, TYPE_ONE, TYPE_ANY
	} type;

#ifdef ENABLE_TCP
#ifdef HAVE_GETADDRINFO
	struct addrinfo *ai;
	const struct addrinfo *next;
#else
	struct sockaddr_in sin;
#endif
#endif

	struct resolver_address current;

#ifndef _WIN32
	struct sockaddr_un saun;
#endif
};

struct resolver *
resolver_new(const char *host, unsigned port)
{
	struct resolver *resolver;

	resolver = malloc(sizeof(*resolver));
	if (resolver == NULL)
		return NULL;

	if (host[0] == '/' || host[0] == '@') {
#ifndef _WIN32
		const bool is_abstract = *host == '@';
		/* sun_path must be null-terminated unless it's an abstract
		   socket */
		const size_t path_length = strlen(host) + !is_abstract;
		if (path_length > sizeof(resolver->saun.sun_path)) {
			free(resolver);
			return NULL;
		}

		resolver->saun.sun_family = AF_UNIX;
		memcpy(resolver->saun.sun_path, host, path_length);

		if (host[0] == '@')
			/* abstract socket */
			resolver->saun.sun_path[0] = 0;

		resolver->current.family = PF_UNIX;
		resolver->current.protocol = 0;
		resolver->current.addrlen = sizeof(resolver->saun)
			- sizeof(resolver->saun.sun_path) + path_length;
		resolver->current.addr = (const struct sockaddr *)&resolver->saun;
		resolver->type = TYPE_ONE;
#else /* _WIN32 */
		/* there are no UNIX domain sockets on Windows */
		free(resolver);
		return NULL;
#endif /* _WIN32 */
	} else {
#ifdef ENABLE_TCP
#ifdef HAVE_GETADDRINFO
		struct addrinfo hints;
		char service[20];
		int ret;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = PF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		snprintf(service, sizeof(service), "%d", port);

		ret = getaddrinfo(host, service, &hints, &resolver->ai);
		if (ret != 0) {
			free(resolver);
			return NULL;
		}

		resolver->next = resolver->ai;
		resolver->type = TYPE_ANY;
#else
		const struct hostent *he;

		he = gethostbyname(host);
		if (he == NULL) {
			free(resolver);
			return NULL;
		}

		if (he->h_addrtype != AF_INET) {
			free(resolver);
			return NULL;
		}


		memset(&resolver->sin, 0, sizeof(resolver->sin));
		resolver->sin.sin_family = AF_INET;
		resolver->sin.sin_port = htons(port);
		memcpy((char *)&resolver->sin.sin_addr.s_addr,
		       (char *)he->h_addr, he->h_length);

		resolver->current.family = PF_INET;
		resolver->current.protocol = 0;
		resolver->current.addrlen = sizeof(resolver->sin);
		resolver->current.addr = (const struct sockaddr *)&resolver->sin;

		resolver->type = TYPE_ONE;
#endif
#else /* !ENABLE_TCP */
		(void)port;
		free(resolver);
		return NULL;
#endif
	}

	return resolver;
}

void
resolver_free(struct resolver *resolver)
{
#if defined(ENABLE_TCP) && defined(HAVE_GETADDRINFO)
	if (resolver->type == TYPE_ANY)
		freeaddrinfo(resolver->ai);
#endif
	free(resolver);
}

const struct resolver_address *
resolver_next(struct resolver *resolver)
{
	if (resolver->type == TYPE_ZERO)
		return NULL;

	if (resolver->type == TYPE_ONE) {
		resolver->type = TYPE_ZERO;
		return &resolver->current;
	}

#if defined(ENABLE_TCP) && defined(HAVE_GETADDRINFO)
	if (resolver->next == NULL)
		return NULL;

	resolver->current.family = resolver->next->ai_family;
	resolver->current.protocol = resolver->next->ai_protocol;
	resolver->current.addrlen = resolver->next->ai_addrlen;
	resolver->current.addr = resolver->next->ai_addr;

	resolver->next = resolver->next->ai_next;

	return &resolver->current;
#else
	return NULL;
#endif
}
