set(GTEST_DIR ../third-party/gtest-1.7.0)

add_library(gtest
	${GTEST_DIR}/src/gtest-all.cc
    ${GTEST_DIR}/src/gtest_main.cc
)

target_include_directories(gtest
	PUBLIC
    ${GTEST_DIR}
    ${GTEST_DIR}/include
)


target_compile_definitions(gtest PUBLIC -DGTEST_HAS_PTHREAD=0)

if (MSVC)
	if (MSVC_VERSION EQUAL 1700)
		# Workaround for Visual Studio 2012
	    target_compile_definitions(gtest PUBLIC -D_VARIADIC_MAX=10)
	endif()

	target_compile_definitions(gtest PUBLIC -D_CRT_SECURE_NO_WARNINGS)
endif()
