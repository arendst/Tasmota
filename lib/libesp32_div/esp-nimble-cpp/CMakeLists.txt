# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

if(__COMPONENT_TARGETS MATCHES "___idf_esp-nimble-component")
    list(APPEND ESP_NIMBLE_PRIV_REQUIRES
        esp-nimble-component
    )
elseif(__COMPONENT_TARGETS MATCHES "__idf_nimble")
    list(APPEND ESP_NIMBLE_PRIV_REQUIRES
        nimble
    )
endif()

# Arduino install using IDF component manager
if(__COMPONENT_TARGETS MATCHES "___idf_espressif__arduino-esp32")
    list(APPEND ESP_NIMBLE_PRIV_REQUIRES
        arduino-esp32
    )
# Manual installation of Arduino framework
elseif(__COMPONENT_TARGETS MATCHES "__idf_arduino")
    list(APPEND ESP_NIMBLE_PRIV_REQUIRES
        arduino
    )
# PlatformIO
elseif(__COMPONENT_TARGETS MATCHES "___idf_framework-arduinoespressif32")
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
    "esp32c5"
    "esp32c6"
    "esp32h2"
    "esp32p4"
  INCLUDE_DIRS
    "src"
  SRCS
    "src/NimBLE2904.cpp"
    "src/NimBLEAddress.cpp"
    "src/NimBLEAdvertisedDevice.cpp"
    "src/NimBLEAdvertisementData.cpp"
    "src/NimBLEAdvertising.cpp"
    "src/NimBLEAttValue.cpp"
    "src/NimBLEBeacon.cpp"
    "src/NimBLECharacteristic.cpp"
    "src/NimBLEClient.cpp"
    "src/NimBLEDescriptor.cpp"
    "src/NimBLEDevice.cpp"
    "src/NimBLEEddystoneTLM.cpp"
    "src/NimBLEExtAdvertising.cpp"
    "src/NimBLEHIDDevice.cpp"
    "src/NimBLEL2CAPChannel.cpp"
    "src/NimBLEL2CAPServer.cpp"
    "src/NimBLERemoteCharacteristic.cpp"
    "src/NimBLERemoteDescriptor.cpp"
    "src/NimBLERemoteService.cpp"
    "src/NimBLERemoteValueAttribute.cpp"
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

