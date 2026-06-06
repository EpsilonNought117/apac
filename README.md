# APAC - Arbitrary Precision Arithmetic and Computation

**libapac** is a free and open-source, high-performance C library for
arbitrary-precision integer arithmetic and computation.

## Build Requirements

### Windows

- **Operating System:** 64-bit Windows
- **C11 Compiler:** MSVC (`cl.exe`) or Clang (`clang-cl`)
- **Build Generator:** CMake ≥ 3.12
- **Build System:** MSBuild or Ninja
- **x86-64 Assembler:** MASM (Microsoft Macro Assembler)

**NOTE:** MinGW toolchain is **not supported**.

### Linux

- **Operating System:** 64-bit Linux (x86-64)
- **C11 Compiler:** GCC or Clang
- **Build Generator:** CMake ≥ 3.12
- **Build System:** Make or Ninja
- **x86-64 Assembler:** GAS (GNU Assembler)

## Building the Library

1. **Clone the repository**

   ```sh
   git clone https://github.com/EpsilonNought117/libapac.git
   cd libapac
   ```

2. **Configure the project**

    - Windows

      ```sh
      cmake -S . -B build
      ```

    - Linux

      ```sh
      cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
      ```

3. **Build the project**

    - Windows

      ```sh
      cmake --build build --config Release
      ```
  
    - Linux

      ```sh
      cmake --build build --parallel
      ```

## CMake Variables

The following CMake options control optional components of the build.  
All options can be enabled or disabled during the CMake configuration step
(e.g. via `-D<OPTION>=ON|OFF`).

| Variable            | Description                                                          | Default |
|---------------------|----------------------------------------------------------------------|---------|
| `BUILD_SHARED_LIB`  | Build **libapac** as a shared library (`.dll` / `.so`)               | `OFF`   |
| `BUILD_APAC_TESTS`  | Build the `apn_tests` program for correctness testing                | `OFF`   |
| `BUILD_APN_TUNE`    | Build the `apn_tune` algorithm threshold tuning utility              | `OFF`   |

## Optimized Microarchitectures

The following x86-64 microarchitectures contain assembly code especially optimized for them

  - AMD Zen 4

## Example Usage

Before using any libapac routines, the library must be initialized to perform
CPU feature detection and set up optimized dispatch tables.

```c
#include "apac.h"

int main(void)
{
    /* Initialize (CPU detection, memory allocation functions etc.,) */
    apac_init();

    /* Library is now ready for use */

    return 0;
}
```

## Testing and Tuning

apac includes dedicated utilities for correctness testing and performance tuning on particular micro-architectures.

### Correctness Testing (BUILD_APAC_TESTS)

The testing programs validate the correctness of arbitrary-precision
operations across a wide range of operand sizes and edge cases. It is 
intended to be run after changes to core arithmetic or assembly routines.

By default, the test suite is not built 
automatically. To enable it during configuration:

    cmake -DBUILD_APAC_TESTS=ON

After building, the test executable can be run directly from the build directory:

    ./apn_tests <seed>

Where:

- `<seed>` is a hexadecimal PRNG seed used to deterministically generate test vectors

Example:

    ./apn_tests C0FFEE

### Algorithm Threshold Tuning (apn_tune)

The apn_tune utility benchmarks different algorithmic variants (e.g. basecase,
Karatsuba) to determine optimal size thresholds
for the target CPU.

This tool is also built by default.
To disable it during configuration:

    cmake -DBUILD_APN_TUNE=OFF

Running the tuner:

    ./apn_tune <core> <seed>

Where:

- `<core>` is the logical CPU core index to which the tuning process will be pinned
- `<seed>` is a hexadecimal PRNG seed used to generate benchmark operands

Example:

    ./apn_tune 4 C0FFEEC0DE

#### Turbo Boost Handling

On **Windows**, `apn_tune` automatically disables CPU turbo boost using OS APIs
for the duration of the tuning run, and restores it upon normal program exit.
This ensures stable and reproducible timing results.

> **Warning:** Terminating `apn_tune` prematurely (e.g. via forced termination)
> may prevent turbo boost from being re-enabled. In such cases, the user will have to manually turn or turbo boost 
or run the tuning utility again and let it terminate by itself. Rebooting the system might also re-enable turbo boost.

On **non-Windows platforms**, turbo boost control is not performed by libapac.
Users must manually disable frequency scaling or turbo features if required
to obtain stable benchmark results.

Note: Tuning results are CPU-specific. For best performance, run apn_tune
on the same machine where libapac will be deployed.

> **Note**: The library is currently WIP
