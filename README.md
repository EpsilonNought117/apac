# libapac - A library for arbitrary precision arithmetic in C

**libapac** is a high-performance arbitrary precision arithmetic library for scientific computing
and number crunching with infinite precision, only limited by the amount of memory on your system.

---

## Features

- Targeted for **x86-64** CPUs on **Windows OS** (Support for ARM64 in future).
- Supports both **static** and **shared** library builds.
- Customizable memory allocation functions.
- High-performance low-level APIs that focus on speed and space efficiency.
- Performance-critical functions written in hand-rolled assembly or using C compiler intrinsics.

---

## Build prerequisites for libapac

1) Windows 10/11
2) _Visual Studio 2022_ or _Visual Studio Build Tools_ with ``Desktop development with C++`` option selected.
3) CMake 3.8+
4) Build generator like `Visual Studio 17 2022` or `Ninja`.

## CMake Options

| Option            | Default	| Description									|
|-------------------|-----------|-----------------------------------------------|
| BUILD_SHARED_LIBS	| `OFF`		| Build a DLL if `ON`							|

