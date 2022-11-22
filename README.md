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

# OneAPI Devices

The bug occurs on certain devices:

    FAIL [opencl:acc:0] Intel(R) FPGA Emulation Platform for OpenCL(TM), Intel(R) FPGA Emulation Device 1.2 [2022.14.10.0.20_160000.xmain-hotfix]
    FAIL [opencl:cpu:1] Intel(R) OpenCL, AMD Ryzen 9 3900X 12-Core Processor 3.0 [2022.14.10.0.20_160000.xmain-hotfix]
    GOOD [opencl:gpu:2] Intel(R) OpenCL HD Graphics, Intel(R) Graphics [0x56a0] 3.0 [22.32.23937]
    GOOD [ext_oneapi_level_zero:gpu:0] Intel(R) Level-Zero, Intel(R) Graphics [0x56a0] 1.3 [1.3.23937]
    FAIL [host:host:0] SYCL host platform, SYCL host device 1.2 [1.2]

Set `DEVICE_NO` at the top of `build.sh` or as the first argument to
`./out/main`.


# Environment

  - Ubuntu 22.04
  - AMD Ryzen 9 3900X 12-Core Processor
  - Intel A770
  - OneAPI version:

        Intel(R) oneAPI DPC++/C++ Compiler 2022.2.1 (2022.2.1.20221020)
        Target: x86_64-unknown-linux-gnu
        Thread model: posix
        InstalledDir: /opt/intel/oneapi/compiler/2022.2.1/linux/bin-llvm
        Configuration file: /opt/intel/oneapi/compiler/2022.2.1/linux/bin/icx.cfg
