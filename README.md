# APAC - Arbitrary Precision Arithmetic and Computation

**libapac** is a free, high-performance C library for
arbitrary-precision arithmetic and computation.

> **Note**: 
> - The library is currently under active development and far from complete
> - Only x86-64 CPUs are supported as of now
> - ARM64 CPU support is planned for a future release

## Build Requirements

### Windows

- **Operating System:** 64-bit Windows
- **C11 Compiler:** MSVC (`cl.exe`), Clang (`clang-cl`), or Clang with a
  GNU-like command-line interface (`clang`/`clang++`)
- **x86-64 Assembler:** MASM (Microsoft Macro Assembler)

**NOTE:** MinGW toolchain and it's compilers are **not supported**.

### Linux

- **Operating System:** 64-bit Linux (x86-64)
- **C11 Compiler:** GCC or Clang
- **x86-64 Assembler:** GAS (GNU Assembler) or Clang's integrated assembler

### Mac0S

- Yet to be tested.

## CMake Variables

The following CMake options control optional components of the build.  
All options can be enabled or disabled during the CMake configuration step
(e.g. via `-D<OPTION>=ON|OFF`).

| Variable            | Description                                                          | Default |
|---------------------|----------------------------------------------------------------------|---------|
| `APAC_SHARED`       | Build **libapac** as a shared library (`.dll` / `.so`)               | `OFF`   |
| `APAC_TESTS`        | Build the `apn_tests` program for correctness testing                | `OFF`   |
| `APAC_TUNE`         | Build the `apn_tune` algorithm threshold tuning utility              | `OFF`   |

## Optimized Microarchitectures

The following x86-64 microarchitectures contain assembly code especially optimized for them

  - AMD Zen 4

## Example Usage

Before using any apac routines, the library must be initialized to perform
CPU feature detection and set up optimized dispatch tables.

```c
#include "apac.h"

int main(void)
{
    /* Initialize (CPU detection) */
    apac_get_cpu_spec();

    /* Optionally, initialize custom memory allocators */
    apac_init_allocator(
        /* custom malloc here  */,
        /* custom realloc here */,
        /* custom free here    */,
        /* context buffer here */
    )

    /* Library is now ready for use */

    return 0;
}
```

## Testing and Tuning

apac includes dedicated utilities for correctness testing and performance tuning on particular micro-architectures.

### Correctness Testing (APAC_TESTS)

The testing programs validate the correctness of arbitrary-precision
operations across a wide range of operand sizes and edge cases. It is 
intended to be run after changes to core arithmetic or assembly routines.

By default, the test suite is not built 
automatically. To enable it during configuration:

    cmake -DAPAC_TESTS=ON

After building, the tests can be run via

    ctest --test-dir <dir-name> --parallel <proc-count>

Optionally, you can pass in `PRNG_SEED` and `ITERATIONS` environment variables via the command line,
where `PRNG_SEED` is a hexadecimal seed value for the pseudo-random number generator and `ITERATIONS`
tests the functions as many times as specified.

Example:

- Windows

```sh
set PRNG_SEED=0xC0FFEE && set ITERATIONS=16384 && ctest --test-dir build --parallel 8
```

- Linux

```sh
PRNG_SEED=0xC0FFEE ITERATIONS=16384 ctest --test-dir build --parallel 8
```

### Algorithm Threshold Tuning (apn_tune)

The apn_tune utility benchmarks different algorithmic variants (e.g. basecase,
Karatsuba) to determine optimal size thresholds
for the target CPU.

This tool is not built by default.
To enable it during configuration:

    cmake -DAPAC_TUNE=ON

Running the tuner:

    ./<build-dir-name>/apn_tune <core_id> <seed>

Where:

- `<core_id>` is the logical CPU core index to which the tuning process's main thread will be pinned
- `<seed>` is a hexadecimal PRNG seed used to generate benchmark operands

Example:

    ./apn_tune 4 0xC0FFEEC0DE

#### Turbo Boost Handling

On **Windows**, `apn_tune` automatically disables CPU turbo boost using OS APIs
for the duration of the tuning run, and restores it upon normal program exit.
This ensures stable and reproducible timing results.

> **Warning:** Terminating `apn_tune` prematurely (e.g. via forced termination)
> may prevent turbo boost from being re-enabled. In such cases, the user will have to manually turn or turbo boost 
or run the tuning utility again and let it terminate by itself. Rebooting the system might also re-enable turbo boost.

On **non-Windows platforms**, turbo boost control is not performed by `apn_tune`.
Users must manually disable frequency scaling or turbo features to obtain stable \
and accurate benchmark results.

Note: Tuning results are CPU-specific. For best performance, run `apn_tune`
on the same machine where libapac will be deployed.