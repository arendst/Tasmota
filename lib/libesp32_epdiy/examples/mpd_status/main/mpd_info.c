
#include "mpd_info.h"
#include "esp_assert.h"
#include "esp_log.h"
#include "string.h"

extern void handle_error(struct mpd_connection **c);

void free_playback_info(mpd_playback_info_t *info) {
  mpd_song_free(info->current_song);
  mpd_status_free(info->status);
}

mpd_playback_info_t *fetch_playback_info(struct mpd_connection *mpd_conn) {
  if (mpd_connection_get_error(mpd_conn) != MPD_ERROR_SUCCESS) {
    handle_error(&mpd_conn);
    return NULL;
  }

  mpd_command_list_begin(mpd_conn, true);
  mpd_send_status(mpd_conn);
  mpd_send_current_song(mpd_conn);
  mpd_command_list_end(mpd_conn);

  struct mpd_status *status = mpd_recv_status(mpd_conn);
  handle_error(&mpd_conn);
  if (status == NULL) {
    return NULL;
  }

  if (mpd_status_get_error(status) != NULL) {
    ESP_LOGW("mpd_info", "error: %s\n", mpd_status_get_error(status));
    mpd_status_free(status);
    return NULL;
  }

  if (mpd_connection_get_error(mpd_conn) != MPD_ERROR_SUCCESS) {
    mpd_status_free(status);
    handle_error(&mpd_conn);
    return NULL;
  }

  mpd_response_next(mpd_conn);

  struct mpd_song *song = mpd_recv_song(mpd_conn);
  if (mpd_connection_get_error(mpd_conn) != MPD_ERROR_SUCCESS ||
      !mpd_response_finish(mpd_conn)) {

    mpd_status_free(status);
    mpd_song_free(song);
    handle_error(&mpd_conn);
    return NULL;
  }

  handle_error(&mpd_conn);
  if (song == NULL) {
    mpd_status_free(status);
    return NULL;
  }

  mpd_playback_info_t *info = malloc(sizeof(mpd_playback_info_t));
  if (info == NULL) {
    mpd_status_free(status);
    mpd_song_free(song);
    ESP_LOGW("mpd_info", "could not allocate info.");
    return NULL;
  }

  char str[12];
  crypto_generichash_state state;
  crypto_generichash_init(&state, NULL, 0, crypto_generichash_BYTES);

  sprintf(str, "%d", mpd_status_get_song_id(status));
  crypto_generichash_update(&state, (uint8_t*)str, strlen(str));

  //sprintf(str, "%d", mpd_status_get_volume(status));
  //crypto_generichash_update(&state, (uint8_t*)str, strlen(str));

  sprintf(str, "%d", mpd_status_get_queue_version(status));
  crypto_generichash_update(&state, (uint8_t*)str, strlen(str));

  crypto_generichash_final(&state, (uint8_t*)info->hash, crypto_generichash_BYTES);
  info->status = status;
  info->current_song = song;
  return info;
}
