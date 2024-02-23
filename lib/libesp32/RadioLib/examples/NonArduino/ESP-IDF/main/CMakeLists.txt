# register the component and set "RadioLib", "esp_timer" and "driver" as required
idf_component_register(SRCS "main.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES RadioLib esp_timer driver)
