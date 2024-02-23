#!/usr/bin/env bash

set -e

make clean

# Commit hash of the https://github.com/lab11/nrf5x-base repository
# to use to make the library.
NRF5X_BASE_SHA=f813b5dbfef3b7a71044bca72e2ab8b3c4d9e593

if [ ! -f $NRF5X_BASE_SHA.zip ]; then
	wget https://github.com/lab11/nrf5x-base/archive/$NRF5X_BASE_SHA.zip
fi

if [ ! -d "nrf5x-base-$NRF5X_BASE_SHA" ]; then
	unzip $NRF5X_BASE_SHA.zip
fi

make -j NRF_BASE_PATH=nrf5x-base-$NRF5X_BASE_SHA

echo ""
echo "Done."
