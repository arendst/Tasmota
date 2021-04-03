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

/*! \file
 * \brief MPD client library
 *
 * This is a client library for the Music Player Daemon, written in C.
 *
 * You can choose one of several APIs, depending on your requirements:
 *
 * - struct mpd_async: a very low-level asynchronous API which knows
 *   the protocol syntax, but no specific commands
 *
 * - struct mpd_connection: a basic synchronous API which knows all
 *   MPD commands and parses all responses
 *
 * \author Max Kellermann (max.kellermann@gmail.com)
 */

#ifndef MPD_CLIENT_H
#define MPD_CLIENT_H

// IWYU pragma: begin_exports

#include "audio_format.h"
#include "capabilities.h"
#include "connection.h"
#include "database.h"
#include "directory.h"
#include "entity.h"
#include "fingerprint.h"
#include "idle.h"
#include "list.h"
#include "message.h"
#include "mixer.h"
#include "mount.h"
#include "neighbor.h"
#include "output.h"
#include "pair.h"
#include "partition.h"
#include "password.h"
#include "player.h"
#include "playlist.h"
#include "queue.h"
#include "recv.h"
#include "response.h"
#include "search.h"
#include "send.h"
#include "settings.h"
#include "song.h"
#include "stats.h"
#include "status.h"
#include "sticker.h"
#include "version.h"

// IWYU pragma: end_exports

#endif
