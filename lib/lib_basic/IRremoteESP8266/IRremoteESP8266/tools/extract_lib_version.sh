#!/bin/bash -e
# Copyright 2021 crankyoldgit
# Extract and constuct the string version of the IRremoteESP8266 Library Version
function getVerNum()
{
  echo $(egrep "^#define\s+_IRREMOTEESP8266_VERSION_$1\s+" \
             src/IRremoteESP8266.h | awk '{print $3;}')
}

MAJOR=$(getVerNum "MAJOR")
MINOR=$(getVerNum "MINOR")
PATCH=$(getVerNum "PATCH")
LIB_VERSION="${MAJOR}.${MINOR}.${PATCH}"
echo $LIB_VERSION
