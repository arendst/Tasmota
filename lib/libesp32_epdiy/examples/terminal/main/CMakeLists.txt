set(app_sources "st.c" "main.c")

idf_component_register(SRCS ${app_sources} REQUIRES epd_driver)
