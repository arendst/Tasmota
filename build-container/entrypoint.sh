# configure build via environment
#!/bin/bash

TASMOTA_VOLUME='/tasmota'
USER_CONFIG_OVERRIDE="${TASMOTA_VOLUME}/sonoff/user_config_override.h"

if [ -d $TASMOTA_VOLUME ]; then
	cd $TASMOTA_VOLUME
	if [ -n "$(env | grep ^TASMOTA_)" ]; then
		echo "Removing $USER_CONFIG_OVERRIDE and creating a new one."
		rm "$USER_CONFIG_OVERRIDE"
		#export PLATFORMIO_BUILD_FLAGS='-DUSE_CONFIG_OVERRIDE'
		sed -i 's/^; *-DUSE_CONFIG_OVERRIDE/                            -DUSE_CONFIG_OVERRIDE/' platformio.ini
		echo '#ifndef _USER_CONFIG_OVERRIDE_H_' >> $USER_CONFIG_OVERRIDE
		echo '#define _USER_CONFIG_OVERRIDE_H_' >> $USER_CONFIG_OVERRIDE
		echo '#warning **** user_config_override.h: Using Settings from this File ****' >> $USER_CONFIG_OVERRIDE	
		echo '#undef CFG_HOLDER' >> $USER_CONFIG_OVERRIDE
		echo '#define CFG_HOLDER	1' >> $USER_CONFIG_OVERRIDE
		for i in $(env | grep ^TASMOTA_); do
			config=${i#TASMOTA_}
			key=$(echo $config | cut -d '=' -f 1)
			value=$(echo $config | cut -d '=' -f 2)
			echo "#undef ${key}" >> $USER_CONFIG_OVERRIDE
			echo "#define ${key}	${value}" >> $USER_CONFIG_OVERRIDE
		done
		echo '#endif' >> $USER_CONFIG_OVERRIDE
	fi
	echo "Compiling..."
	#pio run -t clean
	pio run $@
	echo "Everything done you find your builds in .pioenvs/<build-flavour>/firmware.bin"
else
	echo ">>> NO TASMOTA VOLUME MOUNTED --> EXITING"
	exit 0;
fi
