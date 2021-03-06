#!/bin/bash

function die {
    echo $1
    exit 255
}

# if file $1 does not exist, call wget on $2
function wget_if_not_exists {
    test -f $1 || wget -q $2 || die "$1 does not exist and wget failed for $2"
}

# Try to figure out userid to use with git/gerrit
export GIT_USER=`ssh -G gerrit.iotivity.org | grep "^user " | sed 's/^user //'`
set ${GIT_USER:=${USER}}
set ${GIT_USER:=${USERNAME}}

# If you are behind a proxy, add all the environment variables required to
# set the proxy appropriately for your environment.
#source /vagrant/proxy_config

# Protect the private key for better security (and so that ssh will work)
chmod 600 .ssh/id_rsa

# Cannot use repo directory on host if host is Windows, because
# Windows does not handle symlinks properly
# Clone anonymously, then switch the origin URL - avoids key auth problems
# during provisioning
if [ ! -d "iotivity" ]
then
    git clone https://git.iotivity.org/iotivity.git
    wget https://gerrit.iotivity.org/gerrit/tools/hooks/commit-msg -P iotivity/.git/hooks/
    echo "Switching to guessed git user ${GIT_USER}"
    echo "If incorrect, fix with following cmd using correct GIT_USER:"
    echo "$ git remote set-url origin ssh://\${GIT_USER}@gerrit.iotivity.org/iotivity"
    pushd iotivity
    git remote set-url origin ssh://${GIT_USER}@gerrit.iotivity.org/iotivity
    popd
fi

export cborRevision='v0.4.1'
export TINYCBOR_DIR=iotivity/extlibs/tinycbor/tinycbor
if [ ! -d ${TINYCBOR_DIR} ]
then
    git clone https://github.com/01org/tinycbor.git ${TINYCBOR_DIR} -b ${cborRevision}
fi

export mbedtls_revision='mbedtls-2.4.2'
export MBEDTLS_DIR=iotivity/extlibs/mbedtls/mbedtls
if [ ! -d ${MBEDTLS_DIR} ]
then
    git clone https://github.com/ARMmbed/mbedtls.git ${MBEDTLS_DIR} -b ${mbedtls_revision}
fi

#######################################
# Android pre-provision (optional)
#######################################
function android_setup {
    echo "Set up Android NDK"
    export ANDROID_NDK=${PWD}/iotivity/tools/android-ndk
    if [ ! -d ${ANDROID_NDK} ]
    then
        mkdir -p ${ANDROID_NDK}
        pushd ${ANDROID_NDK}
        wget_if_not_exists "android-ndk-r10e-linux-x86_64.bin" http://dl.google.com/android/ndk/android-ndk-r10e-linux-x86_64.bin
        chmod a+x android-ndk-r10e-linux-x86_64.bin
        ./android-ndk-r10e-linux-x86_64.bin
        popd
    fi

    echo "Set up Android NDK"
    export ANDROID_SDK=${PWD}/iotivity/tools/android-sdk-linux
    if [ ! -d ${ANDROID_SDK} ]
    then
        pushd iotivity/tools
        cd ..
        wget_if_not_exists "android-sdk_r24.0.2-linux.tgz" http://dl.google.com/android/android-sdk_r24.0.2-linux.tgz
        tar -xzf http://dl.google.com/android/android-sdk_r24.0.2-linux.tgz
        popd
    fi
}

#######################################
# Arduino Dependencies (optional)
#######################################
function arduino_setup {
    # Arduino Mega
    echo "Set up Arduino Mega"
    export ARDUINO_MEGA_DIR=${PWD}/arduino-1.0.5
    if [ ! -d ${ARDUINO_MEGA_DIR} ]
    then
        wget_if_not_exists "arduino-1.0.5-linux64.tgz" http://arduino.googlecode.com/files/arduino-1.0.5-linux64.tgz
        tar -xzf arduino-1.0.5-linux64.tgz
        wget_if_not_exists "Time-1.0.5.zip" http://playground.arduino.cc/uploads/Code/Time.zip
        mv Time.zip Time-1.0.5.zip
        mkdir -p ${ARDUINO_MEGA_DIR}/libraries/Time
        pushd ${ARDUINO_MEGA_DIR}/libraries/Time
        unzip ../../../Time-1.0.5.zip
        cd ../..
        patch -p5 < ../iotivity/tools/arduino/patches/arduino-1.0.5_linux.patch
        popd
    fi

    # Arduino Due
    echo "Set up Arduino Due"
    ARDUINO_DUE_DIR=${PWD}/arduino-1.5.7
    if [ ! -d ${ARDUINO_DUE_DIR} ]
    then
        wget_if_not_exists "download.php?f=%2Farduino-1.5.7-linux64.tgz" http://arduino.cc/download.php?f=/arduino-1.5.7-linux64.tgz
        tar -xzf "download.php?f=%2Farduino-1.5.7-linux64.tgz"
        wget_if_not_exists "Time-1.5.7.zip" http://www.pjrc.com/teensy/arduino_libraries/Time.zip
        mv Time.zip Time-1.5.7.zip
        mkdir -p ${ARDUINO_DUE_DIR}/libraries/Time
        pushd ${ARDUINO_DUE_DIR}/libraries/Time
        unzip ../../../Time-1.5.7.zip
        cd ../..
        patch -p5 < ../iotivity/tools/arduino/patches/arduino-1.5.7_linux.patch
        popd
    fi

    # local.properties
    cat > iotivity/resource/csdk/local.properties <<-EOF
    ifeq (\$(PLATFORM), arduinomega)
        #Location of arduino sdk for Arduino Mega
        ARDUINO_DIR := ${ARDUINO_MEGA_DIR}
        ARDUINO_TOOLS_DIR := \$(ARDUINO_DIR)/hardware/tools/avr/bin
    else
        #Location of arduino sdk for Arduino Due
        ARDUINO_DIR := ${ARDUINO_DUE_DIR}
        ARDUINO_TOOLS_DIR := \$(ARDUINO_DIR)/hardware/tools/gcc-arm-none-eabi-4.8.3-2014q1/bin
    endif
EOF
}

# TODO: switch these off cmdline args
#android_setup
#arduino_setup
