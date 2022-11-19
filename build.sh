#!/bin/bash
# -*- compile-command: "./build.sh"; -*-
set -e

rm -rf out
mkdir -p out
set -x

icpx -g -fsycl main.cpp -o out/main
if ./out/main; then echo BAD! should have failed; exit 1; fi

icpx -g -fsycl main.cpp -o out/main -DFIX1
if ! ./out/main; then echo BAD! should have passed; exit 1; fi

icpx -g -fsycl main.cpp -o out/main -DFIX2
if ! ./out/main; then echo BAD! should have passed; exit 1; fi
