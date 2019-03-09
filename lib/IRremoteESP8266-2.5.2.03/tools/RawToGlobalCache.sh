#!/bin/bash
# Convert IRremoteESP8266's rawData output into Global Cache format.

function isDigits()
{
  [[ "$1" =~ ^[0-9]+$ ]]
}

function usage()
{
cat << EOF
Usage: $0 Frequency_in_Hz
  Reads an IRremoteESP8266 rawData declaration from STDIN and converts it to
  GlobalCache format.
  e.g.
    uint16_t rawbuf[37] = {
        7930, 3952, 494, 1482, 520, 1482, 494, 1508,
        494, 520, 494, 1482, 494, 520, 494, 1482,
        494, 1482, 494, 3978, 494, 520, 494, 520,
        494, 520, 494, 520, 520, 520, 494, 520,
        494, 520, 494, 520, 494};
EOF
  exit 1
}

# We need a frequency argument.
if [[ $# -ne 1 ]]; then
  usage
fi
HZ="$1"
# HZ must be a positive number
if ! isDigits "${HZ}"; then
  usage
fi
# HZ must not be zero.
if [[ ${HZ} == 0 ]]; then
  usage
fi


PERIOD_OFFSET=0
period=$((((1000000 + (${HZ} / 2)) / ${HZ}) + ${PERIOD_OFFSET}))
result="${HZ},1,1"
while read line; do
  # Quick and Dirty Removal of any array declaration syntax, and any commas.
  line="$(echo ${line} | sed 's/uint.*{//i' | sed 's/,//g' | sed 's/};.*//g')"
  for msecs in ${line}; do
    if isDigits "${msecs}"; then
      result="${result},$((${msecs} / ${period}))"
    fi
  done
done
echo "GlobalCache code = \"${result}\""
