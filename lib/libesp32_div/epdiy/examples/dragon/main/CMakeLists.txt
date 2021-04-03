set(app_sources "main.c")

idf_component_register(SRCS ${app_sources} REQUIRES epd_driver)
