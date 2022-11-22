#!/bin/bash
# -*- compile-command: "./build.sh"; -*-
set -e

rm -rf out
mkdir -p out
DEVICE_NO=0
set -x

icpx -g -fsycl main.cpp -o out/main
if ./out/main $DEVICE_NO; then echo BAD! should have failed; exit 1; fi

icpx -g -fsycl main.cpp -o out/main -DFIX1
if ! ./out/main $DEVICE_NO; then echo BAD! should have passed; exit 1; fi

icpx -g -fsycl main.cpp -o out/main -DFIX2
if ! ./out/main $DEVICE_NO; then echo BAD! should have passed; exit 1; fi
