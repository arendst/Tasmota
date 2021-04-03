set(app_sources "main.c" "mpd_image.c" "mpd_info.c")

idf_component_register(SRCS ${app_sources} REQUIRES epd_driver libmpdclient nvs_flash libsodium)
