# VLC for Android
=================
This a clone of the official [Android port of VLC](https://code.videolan.org/videolan/vlc-android)

## Build with UBUNTU 16.04LTS
=============================

Install dev tools:
------------------
///
sudo apt-get install autoconf automake ant libtool patch pkg-config protobuf-compiler ragel subversion unzip git autopoint cmake build-essential curl g++
///

VLC requires protobuf v3

get it from https://github.com/google/protobuf/tree/3.0.x/src

and apply the following

./autogen.sh
./configure
make
make check
sudo make install
sudo ldconfig

Install Android SDK & NDK
-------------------------
sudo add-apt-repository ppa:ubuntu-desktop/ubuntu-make
sudo apt update
sudo apt install ubuntu-make
sudo umake android android-ndk

Update some env vars
--------------------
edit /etc/profile or /etc/environment to add:
export ANDROID_SDK=/opt/Android/SDK (check it)
export ANDROID_NDK=/opt/Android/NDK/ndk
export PATH=$PATH:$ANDROID_SDK/platform-tools
export PATH=$PATH:$ANDROID_SDK/tools

clone this repository
---------------------
git clone https://github.com/sebhuet/VLC-Android.git

then launch compile.sh -a (your arch / edit compile.sh to find yours)
