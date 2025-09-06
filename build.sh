#!/usr/bin/env bash

set -x
set -u 
set -e

Mode=Debug # Release or Debug
RunAfterBuild=1 # If != 0 then run the project executable after building

if ! command -v command &> /dev/null
then
    echo "You need 'command' command to check for required programs."
    exit 1
fi
if ! command -v git &> /dev/null
then
    echo "You need 'git' command to fetch dependencies"
    exit 1
fi
if ! command -v make &> /dev/null
then
    echo "You need 'make' (gnu) command to build dependencies"
    exit 1
fi
# TODO: maybe we should just use CXX variable?
CC=g++
if ! command -v g++ &> /dev/null
then
    if ! command -v clang++ &> /dev/null
    then
        echo "You need a c++ compiler ('g++' or 'clang++') to build the project"
        exit 1
    fi
    CC=clang++
fi

cd "$(dirname "$(readlink -f "$0")")" # move to script directory aka project root

# Fetch dependencies
git submodule update --init --recursive

# Build dependencies
cd rlimgui/
chmod +x premake5
./premake5 gmake
make -j$(nproc)
cd ..

# Build project
echo "Usage: $0 {program} (e.g. calculator, todo, stopwatch, etc. -- see src/)"
if [[ -z $1 ]]
then 
    echo "Missing argument: program name (e.g. calculator, todo, stopwatch, etc. -- see src/)"
    exit 1
fi

BUILD_DIR=build
EXEC=$BUILD_DIR/$1

SRC=src/$1.cpp

CFLAGS="-Wall -Wextra"
if [[ $Mode == "Debug" ]]
then
    CFLAGS="$CFLAGS -ggdb"
elif [[ $Mode == "Release" ]]
then
    CFLAGS="$CFLAGS -O3"
else
    echo "Invalid build mode provided. Should be one of: Debug, Release"
    exit 1
fi

INCLUDES="-Irlimgui -Irlimgui/raylib-master/src -Irlimgui/imgui-master"

# TODO: should we also conditionally build rlimgui using release config?
LIBS="-Lrlimgui/bin/Debug -l:libraylib.a -l:librlImGui.a"

mkdir -p $BUILD_DIR
$CC $CFLAGS $INCLUDES $SRC -o $EXEC $LIBS

# Conditionally run after building
if [[ ! $RunAfterBuild -eq 0 ]]
then
    ./$EXEC
fi
