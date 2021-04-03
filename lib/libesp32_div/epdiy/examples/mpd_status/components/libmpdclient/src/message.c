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

#include <mpd/message.h>
#include <mpd/pair.h>

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct mpd_message {
	char *channel;

	char *text;
};

struct mpd_message *
mpd_message_begin(const struct mpd_pair *pair)
{
	struct mpd_message *output;

	assert(pair != NULL);

	if (strcmp(pair->name, "channel") != 0)
		return NULL;

	output = malloc(sizeof(*output));
	if (output == NULL)
		return NULL;

	output->channel = strdup(pair->value);
	output->text = NULL;

	return output;
}

bool
mpd_message_feed(struct mpd_message *output, const struct mpd_pair *pair)
{
	if (strcmp(pair->name, "channel") == 0)
		return false;

	if (strcmp(pair->name, "message") == 0) {
		free(output->text);
		output->text = strdup(pair->value);
	}

	return true;
}

void
mpd_message_free(struct mpd_message *message)
{
	assert(message != NULL);

	free(message->channel);
	free(message->text);
	free(message);
}

const char *
mpd_message_get_channel(const struct mpd_message *message)
{
	assert(message != NULL);

	return message->channel;
}

const char *
mpd_message_get_text(const struct mpd_message *message)
{
	assert(message != NULL);

	return message->text;
}
