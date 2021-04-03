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

#include <mpd/output.h>
#include <mpd/pair.h>
#include "kvlist.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>

struct mpd_output {
	unsigned id;
	char *name;
	char *plugin;

	struct mpd_kvlist attributes;

	bool enabled;
};

struct mpd_output *
mpd_output_begin(const struct mpd_pair *pair)
{
	struct mpd_output *output;

	assert(pair != NULL);

	if (strcmp(pair->name, "outputid") != 0)
		return NULL;

	output = malloc(sizeof(*output));
	if (output == NULL)
		return NULL;

	output->id = atoi(pair->value);

	output->name = NULL;
	output->plugin = NULL;
	mpd_kvlist_init(&output->attributes);
	output->enabled = false;

	return output;
}

bool
mpd_output_feed(struct mpd_output *output, const struct mpd_pair *pair)
{
	if (strcmp(pair->name, "outputid") == 0)
		return false;

	if (strcmp(pair->name, "outputname") == 0) {
		free(output->name);
		output->name = strdup(pair->value);
	} else if (strcmp(pair->name, "outputenabled") == 0)
		output->enabled = atoi(pair->value) != 0;
	else if (strcmp(pair->name, "plugin") == 0) {
		free(output->plugin);
		output->plugin = strdup(pair->value);
	} else if (strcmp(pair->name, "attribute") == 0) {
		const char *eq = strchr(pair->value, '=');
		if (eq != NULL && eq > pair->value)
			mpd_kvlist_add(&output->attributes,
				       pair->value, eq - pair->value,
				       eq + 1);
	}

	return true;
}

void
mpd_output_free(struct mpd_output *output)
{
	assert(output != NULL);

	free(output->name);
	free(output->plugin);
	mpd_kvlist_deinit(&output->attributes);
	free(output);
}

unsigned
mpd_output_get_id(const struct mpd_output *output)
{
	assert(output != NULL);

	return output->id;
}

const char *
mpd_output_get_name(const struct mpd_output *output)
{
	assert(output != NULL);

	return output->name;
}

const char *
mpd_output_get_plugin(const struct mpd_output *output)
{
	assert(output != NULL);

	return output->plugin;
}

bool
mpd_output_get_enabled(const struct mpd_output *output)
{
	assert(output != NULL);

	return output->enabled;
}

const char *
mpd_output_get_attribute(const struct mpd_output *output, const char *name)
{
	assert(output != NULL);

	return mpd_kvlist_get(&output->attributes, name);
}

const struct mpd_pair *
mpd_output_first_attribute(struct mpd_output *output)
{
	assert(output != NULL);

	return mpd_kvlist_first(&output->attributes);
}

const struct mpd_pair *
mpd_output_next_attribute(struct mpd_output *output)
{
	assert(output != NULL);

	return mpd_kvlist_next(&output->attributes);
}
