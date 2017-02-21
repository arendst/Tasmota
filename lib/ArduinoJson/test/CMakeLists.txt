# Copyright Benoit Blanchon 2014-2017
# MIT License
#
# Arduino JSON library
# https://github.com/bblanchon/ArduinoJson
# If you like this project, please add a star!

include(gtest.cmake)

file(GLOB TESTS_FILES *.hpp *.cpp)

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
	add_compile_options(
		-fno-exceptions
		-pedantic
		-Wall
		-Wcast-align
		-Wcast-qual
		-Wconversion
		-Wctor-dtor-privacy
		-Wdisabled-optimization
		-Werror
		-Wextra
		-Wformat=2
		-Winit-self
		-Wmissing-include-dirs
		-Wparentheses
		-Wnon-virtual-dtor
		-Wold-style-cast
		-Woverloaded-virtual
		-Wredundant-decls
		-Wshadow
		-Wsign-promo
		-Wstrict-overflow=5
		-Wundef
	)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	add_compile_options(
		-Wstrict-null-sentinel
	)

	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.5)
		add_compile_options(-Wlogical-op) # the flag exists in 4.4 but is buggy
	endif()

	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.6)
		add_compile_options(-Wnoexcept)
	endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_compile_options(
		-Wc++11-compat
		-Wdeprecated-register
	)
endif()

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_compile_options(
		/W4 # Set warning level
		/WX # Treats all compiler warnings as errors.
	)
endif()

add_executable(ArduinoJsonTests ${TESTS_FILES})
target_include_directories(ArduinoJsonTests PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../include)
target_link_libraries(ArduinoJsonTests gtest)

add_test(ArduinoJsonTests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ArduinoJsonTests)
