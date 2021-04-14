/* Simple firmware for a ESP32 displaying a static image on an EPaper Screen.
 *
 * Write an image into a header file using a 3...2...1...0 format per pixel,
 * for 4 bits color (16 colors - well, greys.) MSB first.  At 80 MHz, screen
 * clears execute in 1.075 seconds and images are drawn in 1.531 seconds.
 */

#include "esp_event.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_types.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "sdkconfig.h"
#include <stdio.h>
#include <string.h>

#include "epd_driver.h"
#include "epd_highlevel.h"
#include "firasans_16.h"
#include "firasans_16_bold.h"
#include "firasans_20.h"
#include "firasans_24.h"
#include "default_album.h"
#include "mpd/client.h"
#include "mpd_image.h"
#include "mpd_info.h"
#include "wifi_config.h"

const int queue_x_start = 900;
const int album_cover_x = 100;
const int album_cover_y = 100;
const int queue_x_end = 1500;
const int queue_y_start = 100;

EpdiyHighlevelState hl;

static bool got_ip = false;

static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data) {
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    esp_wifi_connect();
  } else if (event_base == WIFI_EVENT &&
             event_id == WIFI_EVENT_STA_DISCONNECTED) {
    esp_wifi_connect();
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    ESP_LOGI("scan", "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    got_ip = true;
  }
}

void show_status(struct mpd_status *status, struct mpd_song *song, int album_height) {

  int cursor_x = album_cover_x;
  int cursor_y = album_height + 100;
  if (mpd_song_get_tag(song, MPD_TAG_TITLE, 0)) {
    epd_write_default(&FiraSans24,
                 mpd_song_get_tag(song, MPD_TAG_TITLE, 0), &cursor_x,
                 &cursor_y, epd_hl_get_framebuffer(&hl));
  }

  if (mpd_song_get_tag(song, MPD_TAG_ALBUM, 0)) {
    cursor_x = album_cover_x;
    cursor_y = album_height + 100 + FiraSans24.advance_y + FiraSans24.advance_y;
    epd_write_default(&FiraSans20,
                 mpd_song_get_tag(song, MPD_TAG_ALBUM, 0), &cursor_x,
                 &cursor_y, epd_hl_get_framebuffer(&hl));
  }

  if (mpd_song_get_tag(song, MPD_TAG_ARTIST, 0)) {
    cursor_x = album_cover_x;
    cursor_y =
        album_height + 100 + FiraSans24.advance_y + FiraSans20.advance_y * 2;
    epd_write_default(&FiraSans20,
                 mpd_song_get_tag(song, MPD_TAG_ARTIST, 0), &cursor_x,
                 &cursor_y, epd_hl_get_framebuffer(&hl));
  }
}

void handle_error(struct mpd_connection **c) {
  enum mpd_error err = mpd_connection_get_error(*c);
  if (err == MPD_ERROR_SUCCESS)
    return;

  ESP_LOGE("mpd", "%d %s\n", err, mpd_connection_get_error_message(*c));
  // error is not recoverable
  if (!mpd_connection_clear_error(*c)) {
    mpd_connection_free(*c);
    *c = NULL;
  }
}

void epd_task() {
  epd_init(EPD_LUT_1K);

  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }

  ESP_ERROR_CHECK(ret);
  tcpip_adapter_init();
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                             &event_handler, NULL));
  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                             &event_handler, NULL));

  // Initialize and start WiFi
  wifi_config_t wifi_config = {
      .sta =
          {
              .ssid = WIFI_SSID,
              .password = WIFI_PASSWORD,
              .scan_method = WIFI_FAST_SCAN,
              .sort_method = WIFI_CONNECT_AP_BY_SIGNAL,
              .threshold.rssi = -127,
              .threshold.authmode = WIFI_AUTH_WPA2_PSK,
          },
  };
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());

  while (!got_ip) {
    vTaskDelay(100);
  }

  hl = epd_hl_init(EPD_BUILTIN_WAVEFORM);
  uint8_t* img_buf = epd_hl_get_framebuffer(&hl);

  album_cover_t *album_cover = NULL;
  mpd_playback_info_t *playback_info = NULL;

  bool init = true;
  bool already_idling = false;

  epd_poweron();
  epd_clear();
  epd_poweroff();

  struct mpd_connection *mpd_conn = NULL;
  while (true) {
    // connect / reconnect
    if (mpd_conn == NULL) {
      mpd_conn = mpd_connection_new("192.168.42.50", 6600, 3000);
      handle_error(&mpd_conn);
      int i;
      printf("mpd server version: ");
      for (i = 0; i < 3; i++) {
        printf("%d.", mpd_connection_get_server_version(mpd_conn)[i]);
      }
      printf("\n");
    }

    if (!init) {
      vTaskDelay(100);
      mpd_run_idle(mpd_conn);
      ESP_LOGW("main", "idle returned");
      handle_error(&mpd_conn);
    }

    ESP_LOGW("main", "fetching playback info");
    mpd_playback_info_t *new_info = fetch_playback_info(mpd_conn);
    handle_error(&mpd_conn);

    bool do_update = false;

    if (playback_info != NULL && new_info != NULL) {
      do_update = memcmp(new_info->hash, playback_info->hash,
                         crypto_generichash_BYTES) != 0;
    } else if (playback_info == NULL && new_info != NULL) {
        do_update = true;
    }

    if (playback_info)
      free_playback_info(playback_info);

    playback_info = new_info;
    ESP_LOGW("info", "info is null? %X", (uint32_t)new_info);
    ESP_LOGW("info", "do update? %d", do_update);

    // no song playing
    if (new_info == NULL && !already_idling) {
      ESP_LOGW("main", "no song playing");
      if (!already_idling) {
          int temperature = 25;
          epd_poweron();
          epd_fullclear(&hl, temperature);
          int x = album_cover_x;
          int y = album_cover_y;
          epd_write_default(&FiraSans24, "Warteschlange leer.", &x, &y, img_buf);
          enum EpdDrawError err = epd_hl_update_screen(&hl, MODE_GC16, temperature);
          assert(err == EPD_DRAW_SUCCESS);
          epd_poweroff();
      }
      vTaskDelay(100);
      already_idling = true;
      init = false;
      continue;
    }

    already_idling = false;

    if ((do_update || init) && new_info != NULL) {
      //epd_clear_area_cycles(epd_full_screen(), 2, 20);

      epd_hl_set_all_white(&hl);

      mpd_send_list_queue_meta(mpd_conn);
      handle_error(&mpd_conn);

      int queue_y = queue_y_start + FiraSans20.advance_y;
      epd_draw_line(queue_x_start, queue_y_start, queue_x_end, queue_y_start + 1, 0, img_buf);
      struct mpd_entity *entity;
      while ((entity = mpd_recv_entity(mpd_conn)) != NULL) {
        handle_error(&mpd_conn);
        const struct mpd_song *song;
        const struct mpd_directory *dir;
        const struct mpd_playlist *pl;

        switch (mpd_entity_get_type(entity)) {
        case MPD_ENTITY_TYPE_UNKNOWN:
          printf("Unknown type\n");
          break;

        case MPD_ENTITY_TYPE_SONG:
          song = mpd_entity_get_song(entity);
          unsigned duration = mpd_song_get_duration(song);

          const EpdFont *font = &FiraSans16;
          if (strcmp(mpd_song_get_uri(song),
                     mpd_song_get_uri(playback_info->current_song)) == 0) {
            font = &FiraSans16_Bold;
          }

          EpdFontProperties rightalign = epd_font_properties_default();
          rightalign.flags = EPD_DRAW_ALIGN_RIGHT;
          int queue_x = queue_x_start + 60;
          char timestr[12] = {0};
          snprintf(timestr, 12, "%02d:%02d", duration / 60, duration % 60);

          int timestamp_x = queue_x_end;
          int timestamp_y = queue_y;
          int track_x = queue_x_start;
          int track_y = queue_y;
          const char* title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
          const char* track = mpd_song_get_tag(song, MPD_TAG_TRACK, 0);
          epd_write_default(font, title, &queue_x, &queue_y, img_buf);
          epd_write_default(font, track, &track_x, &track_y, img_buf);
          epd_write_string(font, timestr, &timestamp_x, &timestamp_y, img_buf, &rightalign);
          break;

        case MPD_ENTITY_TYPE_DIRECTORY:
          dir = mpd_entity_get_directory(entity);
          printf("directory: %s\n", mpd_directory_get_path(dir));
          break;

        case MPD_ENTITY_TYPE_PLAYLIST:
          pl = mpd_entity_get_playlist(entity);
          ESP_LOGI("mpd", "playlist: %s", mpd_playlist_get_path(pl));
          break;
        }

        mpd_entity_free(entity);
      }
      epd_draw_line(queue_x_start, queue_y - FiraSans16.advance_y / 2, queue_x_end, queue_y + 1 - FiraSans16.advance_y / 2, 0, img_buf);

      mpd_response_finish(mpd_conn);
      handle_error(&mpd_conn);

      char *album = (char *)mpd_song_get_tag(playback_info->current_song,
                                             MPD_TAG_ALBUM, 0);
      if (album_cover == NULL || album_cover->identifier == NULL ||
          album == NULL || strncmp(album_cover->identifier, album, 128) != 0) {
        if (album_cover) {
          free_album_cover(album_cover);
          album_cover = NULL;
        }
        album_cover = readpicture(
            mpd_conn, (char *)mpd_song_get_uri(playback_info->current_song),
            album);
      }

      int album_height = 700;
      if (album_cover != NULL) {
        EpdRect area = {
            .width = album_cover->width,
            .height = album_cover->height,
            .x = album_cover_x,
            .y = album_cover_y,
        };
        printf("album cover dimensions: %dx%d\n", album_cover->width,
               album_cover->height);
        album_height = album_cover->height;
        epd_copy_to_framebuffer(area, album_cover->data, img_buf);
      } else {
        EpdRect area = {
            .width = DefaultAlbum_width,
            .height = DefaultAlbum_height,
            .x = album_cover_x,
            .y = album_cover_y,
        };
        printf("album cover dimensions: %dx%d\n", DefaultAlbum_width, DefaultAlbum_height);
        album_height = DefaultAlbum_height;
        epd_copy_to_framebuffer(area, (uint8_t*)DefaultAlbum_data, img_buf);
      }
      int temperature = 25;

      EpdRect upper_update = epd_full_screen();
      upper_update.height = album_cover_y + album_height;
      EpdRect lower_update = epd_full_screen();
      lower_update.height = EPD_HEIGHT - upper_update.height;
      lower_update.y = upper_update.height;
      show_status(playback_info->status, playback_info->current_song, album_cover_y + album_height);

      epd_poweron();
      enum EpdDrawError err = epd_hl_update_area(&hl, MODE_GL16, temperature, upper_update);
      assert(err == EPD_DRAW_SUCCESS);
      err = epd_hl_update_area(&hl, MODE_GL16, temperature, lower_update);
      assert(err == EPD_DRAW_SUCCESS);
      epd_poweroff();
    }
    init = false;
  }
}

void app_main() {

  heap_caps_print_heap_info(MALLOC_CAP_INTERNAL);
  heap_caps_print_heap_info(MALLOC_CAP_SPIRAM);

  epd_task();
}
