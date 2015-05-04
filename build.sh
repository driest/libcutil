#!/usr/bin/env bash

BUILD_DIR="build"
RELEASE_DIR="release"
RELEASE_FILES="libcutil.a tests/cutil_tests"

CMAKE=`which cmake`
if [ $? -gt 0 ]; then
  echo "[-] Can't find cmake, please install it..."
  exit 1
fi

if [ ! -e $BUILD_DIR ]; then
  echo "[+] Creating build directory $BUILD_DIR"
  mkdir $BUILD_DIR
elif [ ! -d $BUILD_DIR ]; then
  echo "[-] Can't clean build environment, $BUILD_DIR not a directory"
  exit 1
elif [ "$(ls -A $BUILD_DIR)" ]; then
  echo "[+] Cleaning old build environment"
  rm -r $BUILD_DIR/*
fi

if [ ! -e $RELEASE_DIR ]; then
  echo "[+] Creating release directory $RELEASE_DIR"
  mkdir $RELEASE_DIR
elif [ ! -d $RELEASE_DIR ]; then
  echo "[-] Can't clean release dir, $RELEASE_DIR not a directory"
  exit 1
elif [ "$(ls -A $RELEASE_DIR)" ]; then
  echo "[+] Cleaning old release"
  rm -r $RELEASE_DIR/*
fi

echo "[+] Generating makefiles"
(cd build && $CMAKE ..)
if [ $? -gt 0 ]; then
  echo "[-] Can't create build files in $BUILD_DIR"
  exit 1
fi

echo "[+] Building project"
(cd build && make)
if [ $? -gt 0 ]; then
  echo "[-] Failed to build project"
  exit 1
fi

echo "[+] Copying binaries to release"
for FILE in $RELEASE_FILES; do
    cp $BUILD_DIR/$FILE $RELEASE_DIR/
    if [ $? -gt 0 ]; then
      echo "[-] Can't copy $FILE to $RELEASE_DIR"
      exit 1
    fi 
done

echo "[+] All done. Find your binaries in $RELEASE_DIR"
