# Bug

`main.cpp` does not produce desired results unless seemingly unrelated changes
are made to the kernel functor.

Reproduce the failure followed by a potential `FIX1` and another potential
`FIX2` by running `./build.sh`

    $ ./build.sh

    + icpx -g -fsycl main.cpp -o out/main
    + ./out/main
    + icpx -g -fsycl main.cpp -o out/main -DFIX1
    + ./out/main
    + icpx -g -fsycl main.cpp -o out/main -DFIX2
    + ./out/main

The script verifies expected failure followed by two successes.

See `main.cpp` at `FIX1` and `FIX2` for one-line seemingly unrelated changes
to the function of the kernel.

See the end of `main()` to understand the expected results.

The problem seems to be inherent to many ArrayFire kernels and obstructs our
port to OneAPI.

# Environment

  - Ubuntu 22.04. Ryzen AMD Ryzen 9 3900X 12-Core Processor
  - Backend is:

         SYCL host platform: SYCL host device, 32029 MB -- 1.2 -- Device driver 1.2 --
           FP64 Support: True -- Unified Memory (True)`.

  - OneAPI says its version is:

        Intel(R) oneAPI DPC++/C++ Compiler 2022.2.0 (2022.2.0.20220730)
        Target: x86_64-unknown-linux-gnu
        Thread model: posix
        InstalledDir: /opt/intel/oneapi/compiler/2022.2.0/linux/bin-llvm
        Configuration file: /opt/intel/oneapi/compiler/2022.2.0/linux/bin/icpx.cfg
