#pragma once

#include <stdint.h>
#include <mpd/client.h>

typedef struct {
    uint8_t* data;
    uint32_t width;
    uint32_t height;
    char* source_uri;
    char* identifier;
} album_cover_t;

album_cover_t* readpicture(struct mpd_connection *c, char *uri, char* identifier);

void free_album_cover(album_cover_t*);
