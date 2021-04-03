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

#ifndef MPD_KVLIST_H
#define MPD_KVLIST_H

#include <mpd/pair.h>
#include <mpd/compiler.h>

#include <stddef.h>

struct mpd_kvlist {
	struct mpd_kvlist_item *head, **tail_r;

	const struct mpd_kvlist_item *cursor;
	struct mpd_pair pair;
};

void
mpd_kvlist_init(struct mpd_kvlist *l);

void
mpd_kvlist_deinit(struct mpd_kvlist *l);

void
mpd_kvlist_add(struct mpd_kvlist *l, const char *key, size_t key_length,
	       const char *value);

mpd_pure
const char *
mpd_kvlist_get(const struct mpd_kvlist *l, const char *name);

const struct mpd_pair *
mpd_kvlist_first(struct mpd_kvlist *l);

const struct mpd_pair *
mpd_kvlist_next(struct mpd_kvlist *l);

#endif
