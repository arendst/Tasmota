#pragma once

#include "sodium.h"
#include "mpd/client.h"

typedef struct {
  char hash[crypto_generichash_BYTES];

  struct mpd_status* status;
  struct mpd_song * current_song;
} mpd_playback_info_t;

void free_playback_info(mpd_playback_info_t*);

mpd_playback_info_t *fetch_playback_info(struct mpd_connection *);
