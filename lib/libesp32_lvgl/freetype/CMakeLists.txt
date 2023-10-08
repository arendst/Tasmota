# CMakeLists.txt
#
# Copyright (C) 2013-2021 by
# David Turner, Robert Wilhelm, and Werner Lemberg.
#
# Written originally by John Cary <cary@txcorp.com>
#
# This file is part of the FreeType project, and may only be used, modified,
# and distributed under the terms of the FreeType project license,
# LICENSE.TXT.  By continuing to use, modify, or distribute this file you
# indicate that you have read the license and understand and accept it
# fully.
#
#
# The following will 1. create a build directory and 2. change into it and
# call cmake to configure the build with default parameters as a static
# library. See https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html
# for information about Debug, Release, etc. builds.
#
#   cmake -B build -D CMAKE_BUILD_TYPE=Release
#
# For a dynamic library, use
#
#   cmake -B build -D BUILD_SHARED_LIBS=true -D CMAKE_BUILD_TYPE=Release
#
# For a framework on OS X, use
#
#   cmake -E chdir build cmake -G Xcode -D BUILD_FRAMEWORK:BOOL=true ..
#
# For an iOS static library, use
#
#   cmake -E chdir build cmake -G Xcode -D IOS_PLATFORM=OS ..
#
# or
#
#   cmake -E chdir build cmake -G Xcode -D IOS_PLATFORM=SIMULATOR ..
#
# or
#
#   cmake -E chdir build cmake -G Xcode -D IOS_PLATFORM=SIMULATOR64 ..
#
# Finally, build the project with:
#
#   cmake --build build
#
# Install it with
#
#   (sudo) cmake --build build --target install
#
# A binary distribution can be made with
#
#   cmake --build build --config Release --target package
#
# Please refer to the cmake manual for further options, in particular, how
# to modify compilation and linking parameters.
#
# Some notes.
#
# . `cmake' creates configuration files in
#
#     <build-directory>/include/freetype/config
#
#   which should be further modified if necessary.
#
# . You can use `cmake' directly on a freshly cloned FreeType git
#   repository.
#
# . `CMakeLists.txt' is provided as-is since it is normally not used by the
#   developer team.
#
# . Set the `FT_WITH_ZLIB', `FT_WITH_BZIP2', `FT_WITH_PNG',
#   `FT_WITH_HARFBUZZ', and `FT_WITH_BROTLI' CMake variables to `ON' to
#   force using a dependency.  Leave a variable undefined (which is the
#   default) to use the dependency only if it is available. Example:
#
#     cmake -B build -D FT_WITH_ZLIB=ON \
#                    -D FT_WITH_BZIP2=ON \
#                    -D FT_WITH_PNG=ON \
#                    -D FT_WITH_HARFBUZZ=ON \
#                    -D FT_WITH_BROTLI=ON [...]
#
#   Set `CMAKE_DISABLE_FIND_PACKAGE_XXX=TRUE' to disable a dependency completely
#   (where `XXX' is a CMake package name like `BZip2').  Example for disabling all
#   dependencies:
#
#     cmake -B build -D CMAKE_DISABLE_FIND_PACKAGE_ZLIB=TRUE \
#                    -D CMAKE_DISABLE_FIND_PACKAGE_BZip2=TRUE \
#                    -D CMAKE_DISABLE_FIND_PACKAGE_PNG=TRUE \
#                    -D CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz=TRUE \
#                    -D CMAKE_DISABLE_FIND_PACKAGE_BrotliDec=TRUE [...]
#
# . Installation of FreeType can be controlled with the CMake variables
#   `SKIP_INSTALL_HEADERS', `SKIP_INSTALL_LIBRARIES', and `SKIP_INSTALL_ALL'
#   (this is compatible with the same CMake variables in zlib's CMake
#   support).

# FreeType explicitly marks the API to be exported and relies on the compiler
# to hide all other symbols. CMake supports a C_VISBILITY_PRESET property
# starting with 2.8.12.
cmake_minimum_required(VERSION 2.8.12)

if (NOT CMAKE_VERSION VERSION_LESS 3.3)
  # Allow symbol visibility settings also on static libraries. CMake < 3.3
  # only sets the property on a shared library build.
  cmake_policy(SET CMP0063 NEW)
endif ()

include(CheckIncludeFile)

# CMAKE_TOOLCHAIN_FILE must be set before `project' is called, which
# configures the base build environment and references the toolchain file
if (APPLE)
  if (DEFINED IOS_PLATFORM)
    if (NOT "${IOS_PLATFORM}" STREQUAL "OS"
        AND NOT "${IOS_PLATFORM}" STREQUAL "SIMULATOR"
        AND NOT "${IOS_PLATFORM}" STREQUAL "SIMULATOR64")
      message(FATAL_ERROR
        "IOS_PLATFORM must be set to either OS, SIMULATOR, or SIMULATOR64")
    endif ()
    if (NOT "${CMAKE_GENERATOR}" STREQUAL "Xcode")
      message(AUTHOR_WARNING
        "You should use Xcode generator with IOS_PLATFORM enabled to get Universal builds.")
    endif ()
    if (BUILD_SHARED_LIBS)
      message(FATAL_ERROR
        "BUILD_SHARED_LIBS can not be on with IOS_PLATFORM enabled")
    endif ()
    if (BUILD_FRAMEWORK)
      message(FATAL_ERROR
        "BUILD_FRAMEWORK can not be on with IOS_PLATFORM enabled")
    endif ()

    # iOS only uses static libraries
    set(BUILD_SHARED_LIBS OFF)

    set(CMAKE_TOOLCHAIN_FILE
      ${CMAKE_SOURCE_DIR}/builds/cmake/iOS.cmake)
  endif ()
else ()
  if (DEFINED IOS_PLATFORM)
    message(FATAL_ERROR "IOS_PLATFORM is not supported on this platform")
  endif ()
endif ()


project(freetype C)

set(VERSION_MAJOR "2")
set(VERSION_MINOR "10")
set(VERSION_PATCH "4")

# Generate LIBRARY_VERSION and LIBRARY_SOVERSION.
set(LIBTOOL_REGEX "version_info='([0-9]+):([0-9]+):([0-9]+)'")
file(STRINGS "${PROJECT_SOURCE_DIR}/builds/unix/configure.raw"
  VERSION_INFO
  REGEX ${LIBTOOL_REGEX})
string(REGEX REPLACE
  ${LIBTOOL_REGEX} "\\1"
  LIBTOOL_CURRENT "${VERSION_INFO}")
string(REGEX REPLACE
  ${LIBTOOL_REGEX} "\\2"
  LIBTOOL_REVISION "${VERSION_INFO}")
string(REGEX REPLACE
  ${LIBTOOL_REGEX} "\\3"
  LIBTOOL_AGE "${VERSION_INFO}")

# This is what libtool does internally on Unix platforms.
math(EXPR LIBRARY_SOVERSION "${LIBTOOL_CURRENT} - ${LIBTOOL_AGE}")
set(LIBRARY_VERSION "${LIBRARY_SOVERSION}.${LIBTOOL_AGE}.${LIBTOOL_REVISION}")

# External dependency library detection is automatic. See the notes at the top
# of this file, for how to force or disable dependencies completely.
option(FT_WITH_ZLIB "Use system zlib instead of internal library." OFF)
option(FT_WITH_BZIP2 "Support bzip2 compressed fonts." OFF)
option(FT_WITH_PNG "Support PNG compressed OpenType embedded bitmaps." OFF)
option(FT_WITH_HARFBUZZ "Improve auto-hinting of OpenType fonts." OFF)
option(FT_WITH_BROTLI "Support compressed WOFF2 fonts." OFF)


# Disallow in-source builds
if ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
  message(FATAL_ERROR
    "In-source builds are not permitted!  Make a separate folder for"
    " building, e.g.,\n"
    "  cmake -E make_directory build\n"
    "  cmake -E chdir build cmake ..\n"
    "Before that, remove the files created by this failed run with\n"
    "  cmake -E remove CMakeCache.txt\n"
    "  cmake -E remove_directory CMakeFiles")
endif ()


# Add local cmake modules
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/builds/cmake)


if (BUILD_FRAMEWORK)
  if (NOT "${CMAKE_GENERATOR}" STREQUAL "Xcode")
    message(FATAL_ERROR
      "You should use Xcode generator with BUILD_FRAMEWORK enabled")
  endif ()
  set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD_32_64_BIT)")
  set(BUILD_SHARED_LIBS ON)
endif ()


# Find dependencies
set(HARFBUZZ_MIN_VERSION "2.0.0")
if (FT_WITH_HARFBUZZ)
  find_package(HarfBuzz ${HARFBUZZ_MIN_VERSION} REQUIRED)
else ()
  find_package(HarfBuzz ${HARFBUZZ_MIN_VERSION})
endif ()

if (FT_WITH_PNG)
  find_package(PNG REQUIRED)
else ()
  find_package(PNG)
endif ()

if (FT_WITH_ZLIB)
  find_package(ZLIB REQUIRED)
else ()
  find_package(ZLIB)
endif ()

if (FT_WITH_BZIP2)
  find_package(BZip2 REQUIRED)
else ()
  find_package(BZip2)
endif ()

if (FT_WITH_BROTLI)
  find_package(BrotliDec REQUIRED)
else ()
  find_package(BrotliDec)
endif ()

# Create the configuration file
if (UNIX)
  check_include_file("unistd.h" HAVE_UNISTD_H)
  check_include_file("fcntl.h" HAVE_FCNTL_H)

  file(READ "${PROJECT_SOURCE_DIR}/builds/unix/ftconfig.h.in"
    FTCONFIG_H)
  if (HAVE_UNISTD_H)
    string(REGEX REPLACE
      "#undef +(HAVE_UNISTD_H)" "#define \\1 1"
      FTCONFIG_H "${FTCONFIG_H}")
  endif ()
  if (HAVE_FCNTL_H)
    string(REGEX REPLACE
      "#undef +(HAVE_FCNTL_H)" "#define \\1 1"
      FTCONFIG_H "${FTCONFIG_H}")
  endif ()
else ()
  file(READ "${PROJECT_SOURCE_DIR}/include/freetype/config/ftconfig.h"
    FTCONFIG_H)
endif ()

set(FTCONFIG_H_NAME "${PROJECT_BINARY_DIR}/include/freetype/config/ftconfig.h")
if (EXISTS "${FTCONFIG_H_NAME}")
  file(READ "${FTCONFIG_H_NAME}" ORIGINAL_FTCONFIG_H)
else ()
  set(ORIGINAL_FTCONFIG_H "")
endif ()
if (NOT (ORIGINAL_FTCONFIG_H STREQUAL FTCONFIG_H))
  file(WRITE "${FTCONFIG_H_NAME}" "${FTCONFIG_H}")
endif ()


# Create the options file
file(READ "${PROJECT_SOURCE_DIR}/include/freetype/config/ftoption.h"
  FTOPTION_H)
if (ZLIB_FOUND)
  string(REGEX REPLACE
    "/\\* +(#define +FT_CONFIG_OPTION_SYSTEM_ZLIB) +\\*/" "\\1"
    FTOPTION_H "${FTOPTION_H}")
endif ()
if (BZIP2_FOUND)
  string(REGEX REPLACE
    "/\\* +(#define +FT_CONFIG_OPTION_USE_BZIP2) +\\*/" "\\1"
    FTOPTION_H "${FTOPTION_H}")
endif ()
if (PNG_FOUND)
  string(REGEX REPLACE
    "/\\* +(#define +FT_CONFIG_OPTION_USE_PNG) +\\*/" "\\1"
    FTOPTION_H "${FTOPTION_H}")
endif ()
if (HARFBUZZ_FOUND)
  string(REGEX REPLACE
    "/\\* +(#define +FT_CONFIG_OPTION_USE_HARFBUZZ) +\\*/" "\\1"
    FTOPTION_H "${FTOPTION_H}")
endif ()
if (BROTLIDEC_FOUND)
  string(REGEX REPLACE
    "/\\* +(#define +FT_CONFIG_OPTION_USE_BROTLI) +\\*/" "\\1"
    FTOPTION_H "${FTOPTION_H}")
endif ()

set(FTOPTION_H_NAME "${PROJECT_BINARY_DIR}/include/freetype/config/ftoption.h")
if (EXISTS "${FTOPTION_H_NAME}")
  file(READ "${FTOPTION_H_NAME}" ORIGINAL_FTOPTION_H)
else ()
  set(ORIGINAL_FTOPTION_H "")
endif ()
if (NOT (ORIGINAL_FTOPTION_H STREQUAL FTOPTION_H))
  file(WRITE "${FTOPTION_H_NAME}" "${FTOPTION_H}")
endif ()


file(GLOB PUBLIC_HEADERS "include/ft2build.h" "include/freetype/*.h")
file(GLOB PUBLIC_CONFIG_HEADERS "include/freetype/config/*.h")
file(GLOB PRIVATE_HEADERS "include/freetype/internal/*.h")


set(BASE_SRCS
  src/autofit/autofit.c
  src/base/ftbase.c
  src/base/ftbbox.c
  src/base/ftbdf.c
  src/base/ftbitmap.c
  src/base/ftcid.c
  src/base/ftfstype.c
  src/base/ftgasp.c
  src/base/ftglyph.c
  src/base/ftgxval.c
  src/base/ftinit.c
  src/base/ftmm.c
  src/base/ftotval.c
  src/base/ftpatent.c
  src/base/ftpfr.c
  src/base/ftstroke.c
  src/base/ftsynth.c
  src/base/fttype1.c
  src/base/ftwinfnt.c
  src/bdf/bdf.c
  src/bzip2/ftbzip2.c
  src/cache/ftcache.c
  src/cff/cff.c
  src/cid/type1cid.c
  src/gzip/ftgzip.c
  src/lzw/ftlzw.c
  src/pcf/pcf.c
  src/pfr/pfr.c
  src/psaux/psaux.c
  src/pshinter/pshinter.c
  src/psnames/psnames.c
  src/raster/raster.c
  src/sdf/sdf.c
  src/sfnt/sfnt.c
  src/smooth/smooth.c
  src/truetype/truetype.c
  src/type1/type1.c
  src/type42/type42.c
  src/winfonts/winfnt.c
)

if (UNIX)
  list(APPEND BASE_SRCS "builds/unix/ftsystem.c")
elseif (WIN32)
  list(APPEND BASE_SRCS "builds/windows/ftsystem.c")
else ()
  list(APPEND BASE_SRCS "src/base/ftsystem.c")
endif ()

if (WIN32)
  enable_language(RC)
  list(APPEND BASE_SRCS builds/windows/ftdebug.c
                        src/base/ftver.rc)
elseif (WINCE)
  list(APPEND BASE_SRCS builds/wince/ftdebug.c)
else ()
  list(APPEND BASE_SRCS src/base/ftdebug.c)
endif ()

if (BUILD_FRAMEWORK)
  list(APPEND BASE_SRCS builds/mac/freetype-Info.plist)
endif ()


if (NOT DISABLE_FORCE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif ()


add_library(freetype
  ${PUBLIC_HEADERS}
  ${PUBLIC_CONFIG_HEADERS}
  ${PRIVATE_HEADERS}
  ${BASE_SRCS}
)

set_target_properties(
  freetype PROPERTIES
    C_VISIBILITY_PRESET hidden)

target_compile_definitions(
  freetype PRIVATE FT2_BUILD_LIBRARY)

if (WIN32)
  target_compile_definitions(
    freetype PRIVATE _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS)
  if (BUILD_SHARED_LIBS)
    target_compile_definitions(
      freetype PRIVATE DLL_EXPORT)
  endif ()
endif ()

if (BUILD_SHARED_LIBS)
  set_target_properties(freetype PROPERTIES
    VERSION ${LIBRARY_VERSION}
    SOVERSION ${LIBRARY_SOVERSION})
endif ()

# Pick up ftconfig.h and ftoption.h generated above, first.
target_include_directories(
  freetype
    PUBLIC
      $<INSTALL_INTERFACE:include/freetype2>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
      ${CMAKE_CURRENT_BINARY_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/include
      
      # Make <ftconfig.h> available for builds/unix/ftsystem.c.
      ${CMAKE_CURRENT_BINARY_DIR}/include/freetype/config
)


if (BUILD_FRAMEWORK)
  set_property(SOURCE ${PUBLIC_CONFIG_HEADERS}
    PROPERTY MACOSX_PACKAGE_LOCATION Headers/config
  )
  set_target_properties(freetype PROPERTIES
    FRAMEWORK TRUE
    MACOSX_FRAMEWORK_INFO_PLIST builds/mac/freetype-Info.plist
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
    XCODE_ATTRIBUTE_INSTALL_PATH "@rpath"
  )
endif ()


set(PKG_CONFIG_REQUIRED_PRIVATE "")
set(PKG_CONFIG_LIBS_PRIVATE "")

if (ZLIB_FOUND)
  target_link_libraries(freetype PRIVATE ${ZLIB_LIBRARIES})
  target_include_directories(freetype PRIVATE ${ZLIB_INCLUDE_DIRS})
  list(APPEND PKG_CONFIG_REQUIRED_PRIVATE "zlib")
endif ()
if (BZIP2_FOUND)
  target_link_libraries(freetype PRIVATE ${BZIP2_LIBRARIES})
  target_include_directories(freetype PRIVATE ${BZIP2_INCLUDE_DIR}) # not BZIP2_INCLUDE_DIRS
  list(APPEND PKG_CONFIG_LIBS_PRIVATE "-lbz2")
endif ()
if (PNG_FOUND)
  target_link_libraries(freetype PRIVATE ${PNG_LIBRARIES})
  target_compile_definitions(freetype PRIVATE ${PNG_DEFINITIONS})
  target_include_directories(freetype PRIVATE ${PNG_INCLUDE_DIRS})
  list(APPEND PKG_CONFIG_REQUIRED_PRIVATE "libpng")
endif ()
if (HARFBUZZ_FOUND)
  target_link_libraries(freetype PRIVATE ${HARFBUZZ_LIBRARIES})
  target_include_directories(freetype PRIVATE ${HARFBUZZ_INCLUDE_DIRS})
  list(APPEND PKG_CONFIG_REQUIRED_PRIVATE "harfbuzz >= ${HARFBUZZ_MIN_VERSION}")
endif ()
if (BROTLIDEC_FOUND)
  target_link_libraries(freetype PRIVATE ${BROTLIDEC_LIBRARIES})
  target_compile_definitions(freetype PRIVATE ${BROTLIDEC_DEFINITIONS})
  target_include_directories(freetype PRIVATE ${BROTLIDEC_INCLUDE_DIRS})
  list(APPEND PKG_CONFIG_REQUIRED_PRIVATE "libbrotlidec")
endif ()


# Installation
include(GNUInstallDirs)

if (NOT SKIP_INSTALL_HEADERS AND NOT SKIP_INSTALL_ALL)
  install(
    # Note the trailing slash in the argument to `DIRECTORY'!
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/freetype2
      COMPONENT headers
      PATTERN "internal" EXCLUDE
      PATTERN "ftconfig.h" EXCLUDE
      PATTERN "ftoption.h" EXCLUDE)
  install(
    FILES ${PROJECT_BINARY_DIR}/include/freetype/config/ftconfig.h
          ${PROJECT_BINARY_DIR}/include/freetype/config/ftoption.h
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/freetype2/freetype/config
      COMPONENT headers)
endif ()

if (NOT SKIP_INSTALL_LIBRARIES AND NOT SKIP_INSTALL_ALL)
  # Generate the pkg-config file
  file(READ "${PROJECT_SOURCE_DIR}/builds/unix/freetype2.in" FREETYPE2_PC_IN)

  string(REPLACE ";" ", " PKG_CONFIG_REQUIRED_PRIVATE "${PKG_CONFIG_REQUIRED_PRIVATE}")

  string(REPLACE "%prefix%" ${CMAKE_INSTALL_PREFIX}
          FREETYPE2_PC_IN ${FREETYPE2_PC_IN})
  string(REPLACE "%exec_prefix%" "\${prefix}"
          FREETYPE2_PC_IN ${FREETYPE2_PC_IN})
  string(REPLACE "%libdir%" "\${prefix}/${CMAKE_INSTALL_LIBDIR}"
          FREETYPE2_PC_IN ${FREETYPE2_PC_IN})
  string(REPLACE "%includedir%" "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}"
          FREETYPE2_PC_IN ${FREETYPE2_PC_IN})
  string(REPLACE "%ft_version%" "${LIBTOOL_CURRENT}.${LIBTOOL_REVISION}.${LIBTOOL_AGE}"
          FREETYPE2_PC_IN ${FREETYPE2_PC_IN})
  string(REPLACE "%REQUIRES_PRIVATE%" "${PKG_CONFIG_REQUIRED_PRIVATE}"
          FREETYPE2_PC_IN ${FREETYPE2_PC_IN})
  string(REPLACE "%LIBS_PRIVATE%" "${PKG_CONFIG_LIBS_PRIVATE}"
          FREETYPE2_PC_IN ${FREETYPE2_PC_IN})

  set(FREETYPE2_PC_IN_NAME "${PROJECT_BINARY_DIR}/freetype2.pc")
  if (EXISTS "${FREETYPE2_PC_IN_NAME}")
    file(READ "${FREETYPE2_PC_IN_NAME}" ORIGINAL_FREETYPE2_PC_IN)
  else ()
    set(ORIGINAL_FREETYPE2_PC_IN "")
  endif ()
  if (NOT (ORIGINAL_FREETYPE2_PC_IN STREQUAL FREETYPE2_PC_IN))
    file(WRITE "${FREETYPE2_PC_IN_NAME}" ${FREETYPE2_PC_IN})
  endif ()

  install(
    FILES ${PROJECT_BINARY_DIR}/freetype2.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    COMPONENT pkgconfig)

  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    ${PROJECT_BINARY_DIR}/freetype-config-version.cmake
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
    COMPATIBILITY SameMajorVersion)

  install(
    TARGETS freetype
      EXPORT freetype-targets
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
      FRAMEWORK DESTINATION Library/Frameworks
      COMPONENT libraries)
  install(
    EXPORT freetype-targets
      DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/freetype
      FILE freetype-config.cmake
      COMPONENT headers)
  install(
    FILES ${PROJECT_BINARY_DIR}/freetype-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/freetype
    COMPONENT headers)
endif ()


# Packaging
set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The FreeType font rendering library.")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.TXT")

set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

if (WIN32)
  set(CPACK_GENERATOR ZIP)
else ()
  set(CPACK_GENERATOR TGZ)
endif ()

set(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Libraries")
set(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "C/C++ Headers")
set(CPACK_COMPONENT_LIBRARIES_DESCRIPTION
  "Library used to build programs which use FreeType")
set(CPACK_COMPONENT_HEADERS_DESCRIPTION
  "C/C++ header files for use with FreeType")
set(CPACK_COMPONENT_HEADERS_DEPENDS libraries)
set(CPACK_COMPONENT_LIBRARIES_GROUP "Development")
set(CPACK_COMPONENT_HEADERS_GROUP "Development")

include(CPack)
