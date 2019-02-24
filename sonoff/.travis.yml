# Continuous Integration (CI) is the practice, in software
# engineering, of merging all developer working copies with a shared mainline
# several times a day < https://docs.platformio.org/page/ci/index.html >
#
# Documentation:
#
# * Travis CI Embedded Builds with PlatformIO
#   < https://docs.travis-ci.com/user/integration/platformio/ >
#
# * PlatformIO integration with Travis CI
#   < https://docs.platformio.org/page/ci/travis.html >
#
# * User Guide for `platformio ci` command
#   < https://docs.platformio.org/page/userguide/cmd_ci.html >
#
#
# Please choose one of the following templates (proposed below) and uncomment
# it (remove "# " before each line) or use own configuration according to the
# Travis CI documentation (see above).
#


#
# Template #1: General project. Test it using existing `platformio.ini`.
#

# language: python
# python:
#     - "2.7"
#
# sudo: false
# cache:
#     directories:
#         - "~/.platformio"
#
# install:
#     - pip install -U platformio
#     - platformio update
#
# script:
#     - platformio run


#
# Template #2: The project is intended to be used as a library with examples.
#

# language: python
# python:
#     - "2.7"
#
# sudo: false
# cache:
#     directories:
#         - "~/.platformio"
#
# env:
#     - PLATFORMIO_CI_SRC=path/to/test/file.c
#     - PLATFORMIO_CI_SRC=examples/file.ino
#     - PLATFORMIO_CI_SRC=path/to/test/directory
#
# install:
#     - pip install -U platformio
#     - platformio update
#
# script:
#     - platformio ci --lib="." --board=ID_1 --board=ID_2 --board=ID_N
