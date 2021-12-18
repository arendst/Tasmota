if(IDF_TARGET STREQUAL "esp32" OR IDF_TARGET STREQUAL "esp32s2" OR IDF_TARGET STREQUAL "esp32s3")
  set(COMPONENT_SRCS
    driver/esp_camera.c
    driver/cam_hal.c
    driver/sccb.c
    driver/sensor.c
    sensors/ov2640.c
    sensors/ov3660.c
    sensors/ov5640.c
    sensors/ov7725.c
    sensors/ov7670.c
    sensors/nt99141.c
    sensors/gc0308.c
    sensors/gc2145.c
    sensors/gc032a.c
    conversions/yuv.c
    conversions/to_jpg.cpp
    conversions/to_bmp.c
    conversions/jpge.cpp
    conversions/esp_jpg_decode.c
    )

  set(COMPONENT_ADD_INCLUDEDIRS
    driver/include
    conversions/include
    )

  set(COMPONENT_PRIV_INCLUDEDIRS
    driver/private_include
    sensors/private_include
    conversions/private_include
    target/private_include
    )

  if(IDF_TARGET STREQUAL "esp32")
    list(APPEND COMPONENT_SRCS
      target/xclk.c
      target/esp32/ll_cam.c
      )
  endif()

  if(IDF_TARGET STREQUAL "esp32s2")
    list(APPEND COMPONENT_SRCS
      target/xclk.c
      target/esp32s2/ll_cam.c
      target/esp32s2/tjpgd.c
      )

    list(APPEND COMPONENT_PRIV_INCLUDEDIRS
      target/esp32s2/private_include
      )
  endif()

  if(IDF_TARGET STREQUAL "esp32s3")
    list(APPEND COMPONENT_SRCS
      target/esp32s3/ll_cam.c
      )
  endif()

  set(COMPONENT_REQUIRES driver)
  set(COMPONENT_PRIV_REQUIRES freertos nvs_flash)

  register_component()
endif()
