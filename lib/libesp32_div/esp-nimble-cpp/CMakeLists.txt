# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

idf_build_get_property(__hack_component_targets __COMPONENT_TARGETS)

if("esp-nimble-component" IN_LIST BUILD_COMPONENTS OR "__esp-nimble-component" IN_LIST __hack_component_targets)
    list(APPEND ESP_NIMBLE_PRIV_REQUIRES
        esp-nimble-component
    )
elseif("nimble" IN_LIST BUILD_COMPONENTS OR "__nimble" IN_LIST __hack_component_targets)
    list(APPEND ESP_NIMBLE_PRIV_REQUIRES
        nimble
    )
endif()

if("arduino" IN_LIST BUILD_COMPONENTS OR __hack_component_targets MATCHES "__idf_arduino")
    list(APPEND ESP_NIMBLE_PRIV_REQUIRES
        arduino
    )
elseif("framework-arduinoespressif32" IN_LIST BUILD_COMPONENTS OR __hack_component_targets MATCHES "___idf_framework-arduinoespressif32")
    list(APPEND ESP_NIMBLE_PRIV_REQUIRES
        framework-arduinoespressif32
    )
endif()

idf_component_register(
  REQUIRED_IDF_TARGETS
    "esp32"
    "esp32s3"
    "esp32c2"
    "esp32c3"
    "esp32c6"
    "esp32h2"
  INCLUDE_DIRS
    "src"
  SRCS
    "src/NimBLE2904.cpp"
    "src/NimBLEAddress.cpp"
    "src/NimBLEAdvertisedDevice.cpp"
    "src/NimBLEAdvertising.cpp"
    "src/NimBLEBeacon.cpp"
    "src/NimBLECharacteristic.cpp"
    "src/NimBLEClient.cpp"
    "src/NimBLEDescriptor.cpp"
    "src/NimBLEDevice.cpp"
    "src/NimBLEEddystoneTLM.cpp"
    "src/NimBLEEddystoneURL.cpp"
    "src/NimBLEExtAdvertising.cpp"
    "src/NimBLEHIDDevice.cpp"
    "src/NimBLERemoteCharacteristic.cpp"
    "src/NimBLERemoteDescriptor.cpp"
    "src/NimBLERemoteService.cpp"
    "src/NimBLEScan.cpp"
    "src/NimBLEServer.cpp"
    "src/NimBLEService.cpp"
    "src/NimBLEUtils.cpp"
    "src/NimBLEUUID.cpp"
  REQUIRES
    bt
    nvs_flash
    driver
  PRIV_REQUIRES
    ${ESP_NIMBLE_PRIV_REQUIRES}
)

