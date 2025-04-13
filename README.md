# ⚙️ libapac  

**libapac** is a high-performance, single-threaded, arbitrary-precision arithmetic library written in C.

---

## 🚀 Features

- 🖥️ Targeted for **x86-64** CPUs on **Windows** (ARM64 support planned).
- 🧱 Supports both **static** and **shared** library builds.
- 🧩 Customizable memory management through user-defined allocation functions.
- ⚡ Optimized for speed using Intel compiler intrinsics and carefully tuned assembly routines.

---

## 🛠️ Build prerequisites for libapac

1) Windows 10/11
2) _Visual Studio 2022_ or _Visual Studio Build Tools_ with ``Desktop development with C++`` option selected.
3) CMake 3.12+
4) Build generator like `Visual Studio 17 2022` or `Ninja`.

## 🧩 CMake Options

| Option            | Default	| Description									|
|-------------------|-----------|-----------------------------------------------|
| BUILD_SHARED_LIBS	| `OFF`		| Build a DLL if `ON`							|
| BUILD_UNIT_TESTS  | `ON`		| Build unit test executable `unit_tests.exe`	|

