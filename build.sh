#!/bin/bash

CODE_PATH="$(dirname "$0")"

CommonFlags="-g -std=c++17 -fno-rtti -fno-exceptions"

mkdir -p "$CODE_PATH/assets"
mkdir -p "$CODE_PATH/build"
pushd "$CODE_PATH/build"

# Setup compiler
if [ -n "$(command -v clang++)" ]
then
  CXX=clang++
  CommonFlags+=" -Wno-switch -Wno-missing-braces -Wno-null-dereference -Wno-self-assign -Wno-extern-c-compat"
else
  CXX=c++
  CommonFlags+=" -Wno-unused-but-set-variable -Wno-aggressive-loop-optimizations"
fi


OpenGLFlags="-framework OpenGL"
PathFlags="-Wl,-rpath,@loader_path"

# Build a 64-bit version
$CXX $CommonFlags ../src/game.cpp -fPIC -shared -o game.so.temp -L/usr/local/lib -lSDL2 -ldl -l SDL2_image $OpenGLFlags
mv game.so.temp game.so

# Build SDL handmade
$CXX $CommonFlags ../src/sdl_platform.cpp -o firewood-x86_64  -L/usr/local/lib -lSDL2 -ldl -l SDL2_image $PathFlags $OpenGLFlags 

popd
