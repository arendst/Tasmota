#include "esp_assert.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_types.h"
#include "mpd/client.h"
#include "esp32/rom/tjpgd.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mpd_image.h"

static uint8_t tjpgd_work[4096];

typedef struct {
  char *image_uri;
  uint8_t *decoded_image;
  uint8_t *chunk_data;
  uint32_t chunk_data_offset;
  uint32_t chunk_data_size;
  uint32_t cover_offset;
  uint32_t scale;
  struct mpd_connection *conn;
  // use albumart exclusively
  bool use_albumart;
} image_fetch_context_t;

void free_album_cover(album_cover_t *cover) {
  free(cover->data);
  free(cover->source_uri);
  free(cover->identifier);
  free(cover);
}
void handle_error(struct mpd_connection **c);

static uint32_t
feed_jpg_chunk(JDEC *jd,      /* Decompressor object */
               uint8_t *buff, /* Pointer to the read buffer (NULL:skip) */
               uint32_t nd /* Number of bytes to read/skip from input stream */
) {
  image_fetch_context_t *context = jd->device;
  if (context->chunk_data == NULL) {
    char offset_s[32];
    snprintf(offset_s, sizeof(offset_s), "%u", context->cover_offset);

    printf("use albumart: %d\n", context->use_albumart);
    struct mpd_connection *c = context->conn;
    if (context->use_albumart) {
        if (!mpd_send_command(c, "albumart", context->image_uri, offset_s,
                              NULL)) {
          handle_error(&c);
          return -1;
        }
    } else {
        if (!mpd_send_command(c, "readpicture", context->image_uri, offset_s,
                              NULL)) {
          handle_error(&c);
          return -1;
        }
    }

    struct mpd_pair *pair = mpd_recv_pair_named(c, "size");
    if (pair == NULL) {
      if (mpd_connection_get_error(c) != MPD_ERROR_SUCCESS) {
        handle_error(&c);
        return -1;
      }
      if (!context->use_albumart) {
          context->use_albumart = true;
          ESP_LOGI("mpd_image", "no image with readpicture, trying albumart...\n");
          if (!mpd_response_finish(c)) {
            handle_error(c);
            return -1;
          }
          int returned = feed_jpg_chunk(jd, buff, nd);
          printf("returned: %d\n", returned);
          return returned;
      }

      if (!mpd_response_finish(c)) {
        handle_error(c);
        return -1;
      }
      fprintf(stderr, "No 'size'\n");

      return -1;
    }

    mpd_return_pair(c, pair);

    pair = mpd_recv_pair_named(c, "binary");
    if (pair == NULL) {
      if (mpd_connection_get_error(c) != MPD_ERROR_SUCCESS) {
        handle_error(c);
        return -1;
      }
      fprintf(stderr, "No 'binary'\n");
      return -1;
    }

    uint32_t chunk_size = strtoull(pair->value, NULL, 10);
    printf("chunk size: %d\n", chunk_size);
    mpd_return_pair(c, pair);

    if (chunk_size == 0) {
      if (!mpd_response_finish(c)) {
        handle_error(c);
        return -1;
      }
      return 0;
    }

    context->chunk_data = malloc(chunk_size);
    if (!mpd_recv_binary(c, context->chunk_data, chunk_size)) {
      handle_error(c);
      return -1;
    }

    context->chunk_data_size = chunk_size;
    context->chunk_data_offset = 0;

    if (!mpd_response_finish(c)) {
      handle_error(c);
      return -1;
    }
    context->cover_offset += chunk_size;
  }

  // read
  uint32_t n_read;
  uint8_t *src = context->chunk_data;
  src += context->chunk_data_offset;
  if (context->chunk_data_offset + nd <= context->chunk_data_size) {
    if (buff)
      memcpy(buff, src, nd);
    n_read = nd;
  } else {
    uint32_t remaining = context->chunk_data_size - context->chunk_data_offset;
    if (buff)
      memcpy(buff, src, remaining);
    n_read = remaining;
  }
  context->chunk_data_offset += n_read;
  if (context->chunk_data_offset >= context->chunk_data_size) {
    free(context->chunk_data);
    context->chunk_data = NULL;
  }
  return n_read;
}

/* User defined call-back function to output RGB bitmap */
static uint32_t
tjd_output(JDEC *jd,     /* Decompressor object of current session */
           void *bitmap, /* Bitmap data to be output */
           JRECT *rect   /* Rectangular region to output */
) {
  image_fetch_context_t *context = jd->device;

  uint32_t w = rect->right - rect->left + 1;
  uint32_t h = rect->bottom - rect->top + 1;
  uint32_t image_width = jd->width >> context->scale;
  uint32_t even_img_width = image_width + image_width % 2;

  uint8_t *bitmap_ptr = bitmap;

  for (uint32_t i = 0; i < w * h; i++) {

    uint8_t r = *(bitmap_ptr++);
    uint8_t g = *(bitmap_ptr++);
    uint8_t b = *(bitmap_ptr++);

    // calculate weighted grayscale
    uint32_t val = ((r * 30 + g * 59 + b * 11) / 100) >> 4;

    int xx = rect->left + i % w;
    if (xx < 0 || xx >= even_img_width) {
      continue;
    }
    int yy = rect->top + i / w;
    if (yy < 0 || yy >= even_img_width) {
      continue;
    }
    uint8_t *buf_ptr =
        &context->decoded_image[yy * even_img_width / 2 + xx / 2];
    if (xx % 2) {
      *buf_ptr = (*buf_ptr & 0x0F) | (val << 4);
    } else {
      *buf_ptr = (*buf_ptr & 0xF0) | val;
    }
  }

  return 1;
}

album_cover_t *readpicture(struct mpd_connection *c, char *uri,
                           char *identifier) {

  JDEC jd; /* Decompression object (70 bytes) */
  JRESULT rc;
  uint8_t scale;

  image_fetch_context_t context;
  context.decoded_image = NULL;
  context.image_uri = uri;
  context.cover_offset = 0;
  context.chunk_data = NULL;
  context.chunk_data_size = 0;
  context.chunk_data_offset = 0;
  context.conn = c;
  context.use_albumart = false;

  /* Prepare to decompress the file */
  rc =
      jd_prepare(&jd, feed_jpg_chunk, tjpgd_work, sizeof(tjpgd_work), &context);
  if (rc != JDR_OK) {
    ESP_LOGW("mpd_image", "jpeg prepare failed: %d\n", rc);
    return NULL;
  }

  /* Determine scale factor */
  for (scale = 0; scale < 3; scale++) {
    if ((jd.width >> scale) <= 700 && (jd.height >> scale) <= 700)
      break;
  }

  uint32_t width = jd.width >> scale;
  uint32_t height = jd.height >> scale;

  uint32_t even_img_width = width + width % 2;
  uint8_t *buf =
      heap_caps_malloc(even_img_width / 2 * height, MALLOC_CAP_SPIRAM);
  if (!buf) {
    ESP_LOGW("mpd_image", "cannot allocate cover info!");
    return NULL;
  }
  context.decoded_image = buf;
  context.scale = scale;
  printf("orig width: %d orig height: %d\n", jd.width, jd.height);
  printf("scaled width: %d scaled height: %d\n", width, height);

  /* Start to decompress the JPEG file */
  rc = jd_decomp(&jd, tjd_output, scale);
  if (rc != JDR_OK) {
    ESP_LOGW("mpd_image", "jpeg decomp failed: %d\n", rc);
    return NULL;
  }

  album_cover_t *cover = malloc(sizeof(album_cover_t));
  if (!cover) {
    ESP_LOGW("mpd_image", "cannot allocate cover info!");
    free(buf);
    return NULL;
  }

  cover->data = buf;
  cover->width = width;
  cover->height = height;
  cover->source_uri = strndup(uri, 128);
  if (cover->source_uri == NULL) {
    free(buf);
    free(cover);
    ESP_LOGW("mpd_image", "cannot allocate uri dup!");
    return NULL;
  }
  if (identifier != NULL) {
    cover->identifier = strndup(identifier, 128);
    if (cover->identifier == NULL) {
      free(cover->source_uri);
      free(buf);
      free(cover);
      ESP_LOGW("mpd_image", "cannot allocate identifier dup!");
      return NULL;
    }
  } else {
    cover->identifier = NULL;
  }
  return cover;
}
