#!/usr/bin/env python

import os
import sys
import setuptools
import crossunixccompiler

version = ''


def process_configparams():
    global version

    with open('../Makefile.inc') as f:
        config_lines = f.read().splitlines()

    cflags = os.getenv("CFLAGS", "")
    for line in config_lines:
        identifier, value = line.split('=', 1)
        if identifier == "CPUFLAGS":
            cflags += " " + value
        elif identifier == "HEADER_DIR":
            cflags += " -I" + os.path.dirname(value)
        elif identifier == "LIB_DIR":
            cflags += " -L" + value
        elif identifier == "LIB_VERSION":
            version = value
        elif identifier in ("CC", "CXX"):
            os.environ[identifier] = value

    os.environ["CFLAGS"] = cflags


if sys.version_info >= (3,):
    BOOST_LIB = 'boost_python3'
else:
    BOOST_LIB = 'boost_python'

process_configparams()
crossunixccompiler.register()

module_RF24 = setuptools.Extension('RF24',
                                   libraries=['rf24', BOOST_LIB],
                                   sources=['pyRF24.cpp'])

setuptools.setup(name='RF24',
                 version=version,
                 ext_modules=[module_RF24])
