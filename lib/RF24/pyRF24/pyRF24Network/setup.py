#!/usr/bin/env python

from distutils.core import setup, Extension
import sys

if sys.version_info >= (3,): 
    BOOST_LIB = 'boost_python3' 
else: 
    BOOST_LIB = 'boost_python' 

module_RF24Network = Extension('RF24Network',
            libraries = ['rf24network', BOOST_LIB],
            sources = ['pyRF24Network.cpp'])

setup(name='RF24Network',
    version='1.0',
    ext_modules=[module_RF24Network]
      )
