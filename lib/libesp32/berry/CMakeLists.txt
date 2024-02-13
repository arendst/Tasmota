cmake_minimum_required(VERSION 3.10)

project(Berry C)
set(CMAKE_C_STANDARD 99)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(BERRY_COC ${CMAKE_CURRENT_SOURCE_DIR}/tools/coc/coc)
set(BERRY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(BERRY_CONFIG_DIR default CACHE FILEPATH "The directory of berry_conf.h.")
set(BERRY_CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/${BERRY_CONFIG_DIR}/berry_conf.h)
set(BERRY_GENERATE ${CMAKE_CURRENT_SOURCE_DIR}/generate)

if (${CMAKE_HOST_WIN32})
  set(BERRY_COC python ${BERRY_COC})
endif ()

file(MAKE_DIRECTORY generate)

# berry library
file(GLOB SOURCES src/*.c)
add_library(libberry ${SOURCES})
target_include_directories(libberry PUBLIC src ${BERRY_CONFIG_DIR})

add_custom_target(berry-coc
  COMMAND ${BERRY_COC} -o ${BERRY_GENERATE} ${BERRY_SOURCE_DIR} -c ${BERRY_CONFIG}
  DEPENDS ${SOURCES} COMMAND_EXPAND_LISTS
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Generate coc objects"
)
add_dependencies(libberry berry-coc)

# berry default exe
file(GLOB SOURCES_EXE default/*.c)
add_executable(berry ${SOURCES_EXE})
target_link_libraries(berry PUBLIC libberry)
