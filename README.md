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
