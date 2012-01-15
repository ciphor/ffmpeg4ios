#! /bin/sh
./configure --prefix=/opt/ffmpeg-0.7-new --disable-encoders --disable-hwaccels --disable-muxers --disable-demuxers --disable-bsfs --disable-protocols --disable-devices --disable-filters --enable-static --disable-shared --enable-pthreads --enable-memalign-hack --enable-decoder=h264 --enable-parser=h264 --arch=x86 --target-os=mingw32 
