COMPONENT_ADD_INCLUDEDIRS := driver/include conversions/include
COMPONENT_PRIV_INCLUDEDIRS := driver/private_include conversions/private_include sensors/private_include target/private_include
COMPONENT_SRCDIRS := driver conversions sensors target target/esp32
CXXFLAGS += -fno-rtti
