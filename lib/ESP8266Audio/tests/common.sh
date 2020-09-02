#!/usr/bin/env bash

set -ex

function print_size_info()
{
    elf_file=$1

    if [ -z "$elf_file" ]; then
        printf "sketch                       data     rodata   bss      text     irom0.text   dram     flash\n"
        return 0
    fi

    elf_name=$(basename $elf_file)
    sketch_name="${elf_name%.*}"
    # echo $sketch_name
    declare -A segments
    while read -a tokens; do
        seg=${tokens[0]}
        seg=${seg//./}
        size=${tokens[1]}
        addr=${tokens[2]}
        if [ "$addr" -eq "$addr" -a "$addr" -ne "0" ] 2>/dev/null; then
            segments[$seg]=$size
        fi


    done < <(xtensa-lx106-elf-size --format=sysv $elf_file)

    total_ram=$((${segments[data]} + ${segments[rodata]} + ${segments[bss]}))
    total_flash=$((${segments[data]} + ${segments[rodata]} + ${segments[text]} + ${segments[irom0text]}))

    printf "%-28s %-8d %-8d %-8d %-8d %-8d     %-8d %-8d\n" $sketch_name ${segments[data]} ${segments[rodata]} ${segments[bss]} ${segments[text]} ${segments[irom0text]} $total_ram $total_flash
    return 0
}

function build_sketches()
{
    set +e
    local arduino=$1
    local srcpath=$2
    local build_arg=$3
    local build_dir=build.tmp
    local build_mod=$BUILD_MOD
    local build_rem=$BUILD_REM
    mkdir -p $build_dir
    local build_cmd="python3 $arduino/$BUILD_PY -p $PWD/$build_dir $build_arg "
    local sketches=$(find $srcpath -name *.ino)
    print_size_info >size.log
    export ARDUINO_IDE_PATH=$arduino
    for sketch in $sketches; do
        testcnt=$(( ($testcnt + 1) % $build_mod ))
        if [ $testcnt -ne $build_rem ]; then
            continue  # Not ours to do
        fi
        rm -rf $build_dir/*
        local sketchdir=$(dirname $sketch)
        local sketchdirname=$(basename $sketchdir)
        local sketchname=$(basename $sketch)
        if [[ "${sketchdirname}.ino" != "$sketchname" ]]; then
            echo "Skipping $sketch, beacause it is not the main sketch file";
            continue
        fi;
        if [[ -f "$sketchdir/.test.skip" ]]; then
            echo -e "\n ------------ Skipping $sketch ------------ \n";
            continue
        fi
        echo -e "\n ------------ Building $sketch ------------ \n";
        # $arduino --verify $sketch;
        echo "$build_cmd $sketch"
        time ($build_cmd $sketch >build.log)
        local result=$?
        if [ $result -ne 0 ]; then
            echo "Build failed ($1)"
            echo "Build log:"
            cat build.log
            set -e
            return $result
        fi
        rm build.log
        #print_size_info $build_dir/*.elf >>size.log
    done
    set -e
}

function install_libraries()
{
    mkdir -p $HOME/Arduino/libraries
    cp -a $TRAVIS_BUILD_DIR $HOME/Arduino/libraries/ESP8266Audio
    git clone https://github.com/earlephilhower/ESP8266SAM $HOME/Arduino/libraries/ESP8266SAM
}

function install_ide()
{
    local ide_path=$1
    wget -q -O arduino.tar.xz https://www.arduino.cc/download.php?f=/arduino-nightly-linux64.tar.xz
    tar xf arduino.tar.xz
    mv arduino-nightly $ide_path
    export PATH="$ide_path:$PATH"
}

function install_esp8266()
{
    local ide_path=$1
    mkdir -p $ide_path/hardware
    cd $ide_path/hardware
    mkdir esp8266com
    cd esp8266com
    git clone https://github.com/esp8266/Arduino esp8266
    pushd esp8266/tools
    # Set custom warnings for all builds (i.e. could add -Wextra at some point)
    echo "compiler.c.extra_flags=-Wall -Wextra -Werror $debug_flags" > ../platform.local.txt
    echo "compiler.cpp.extra_flags=-Wall -Wextra -Werror $debug_flags" >> ../platform.local.txt
    echo -e "\n----platform.local.txt----"
    cat ../platform.local.txt
    git submodule init
    git submodule update
    python3 get.py
    export PATH="$ide_path/hardware/esp8266com/esp8266/tools/xtensa-lx106-elf/bin:$PATH"
    popd

}

function install_esp32()
{
    local ide_path=$1
    sudo pip install pyserial
    cd $ide_path/hardware
    mkdir espressif
    cd espressif
    git clone https://github.com/espressif/arduino-esp32.git esp32
    pushd esp32
    # Set custom warnings for all builds (i.e. could add -Wextra at some point)
    echo "compiler.c.extra_flags=-Wall -Wextra -Werror $debug_flags" > platform.local.txt
    echo "compiler.cpp.extra_flags=-Wall -Wextra -Werror $debug_flags" >> platform.local.txt
    echo -e "\n----platform.local.txt----"
    cat platform.local.txt
    git submodule init
    git submodule update
    cd tools
    python3 get.py
    export PATH="$ide_path/hardware/espressif/esp32/tools/xtensa-esp32-elf/bin/:$PATH"
    popd

}

function install_arduino()
{
    # Install Arduino IDE and required libraries
    echo -e "travis_fold:start:sketch_test_env_prepare"
    cd $TRAVIS_BUILD_DIR
    install_ide $HOME/arduino_ide $TRAVIS_BUILD_DIR
    which arduino
    cd $TRAVIS_BUILD_DIR
    install_libraries
    echo -e "travis_fold:end:sketch_test_env_prepare"
}

function build_sketches_with_arduino()
{
    # Compile sketches
    echo -e "travis_fold:start:sketch_test"
    build_sketches $HOME/arduino_ide $HOME/Arduino/libraries "-l $HOME/Arduino/libraries"
    echo -e "travis_fold:end:sketch_test"
    # Generate size report
    echo -e "travis_fold:start:size_report"
    cat size.log
    echo -e "travis_fold:end:size_report"
}

set -e

if [ "$BUILD_MOD" == "" ]; then
    export BUILD_MOD=1
    export BUILD_REM=0
fi

if [ "$BUILD_TYPE" = "build" ]; then
    export BUILD_PY="hardware/esp8266com/esp8266/tools/build.py -b generic -s 4M1M -v -k "
    install_arduino
    install_esp8266 "$HOME/arduino_ide"
    build_sketches_with_arduino
elif [ "$BUILD_TYPE" = "build_esp32" ]; then
    install_arduino
    install_esp32 "$HOME/arduino_ide"
    export ide_path=$HOME/arduino_ide 
    export FQBN="espressif:esp32:esp32:PSRAM=enabled,PartitionScheme=huge_app"
    export GITHUB_WORKSPACE="$TRAVIS_BUILD_DIR"
    export GITHUB_REPOSITORY="$TRAVIS_REPO_SLUG"
    source $ide_path/hardware/espressif/esp32/.github/scripts/install-arduino-ide.sh
    source $ide_path/hardware/espressif/esp32/.github/scripts/install-arduino-core-esp32.sh
    build_sketches "$FQBN" "$HOME/Arduino/libraries" "$BUILD_REM" "$BUILD_MOD"
fi

