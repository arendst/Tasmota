idf_component_register(SRC_DIRS .
                       PRIV_INCLUDE_DIRS .
                       PRIV_REQUIRES test_utils esp32-camera nvs_flash 
                       EMBED_TXTFILES pictures/testimg.jpeg pictures/test_outside.jpeg pictures/test_inside.jpeg)
