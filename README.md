# libapac - A library for arbitrary precision arithmetic in C

**libapac** is a high-performance arbitrary precision arithmetic library for scientific computing  
and number crunching with infinite precision, only limited by the amount of memory on your system.

---

## Features

- Targeted for **x86-64** CPUs on **Windows OS** (Support for ARM64 in future).
- Supports both **static** and **shared** library builds.
- Customizable memory allocation functions.
- Dynamic Dispatch at runtime as per appropriate CPU (if supported).
- High-performance low-level APIs that focus on speed and space efficiency.
- Performance-critical functions written in hand-rolled assembly or using C compiler intrinsics.

---

## Build prerequisites for libapac

1) Windows 10/11  
2) _Visual Studio 17 2022_ or _Visual Studio Build Tools_ with `Desktop development with C++` option selected  
3) CMake 3.12 or later  
4) Build generator like `Visual Studio 17 2022 (MSBuild)` or `Ninja`  

---

## CMake Options

| Option             | Default | Description                        |
|--------------------|---------|------------------------------------|
| BUILD_SHARED_LIBS  | `OFF`   | Build a DLL if `ON`                |

---

## Example Build Steps (CMake GUI and Visual Studio 17 2022)

### Using CMake GUI:

1. Open **CMake GUI**.
2. In **"Where is the source code:"**, browse to the root folder where you have cloned `libapac`.
3. In **"Where to build the binaries:"**, create and select a `build` folder inside the project.
4. Click **Configure**:
   - Select *"Visual Studio 17 2022"*
   - Choose *"x64"* as the platform
5. Customize any CMake options (e.g., check `BUILD_SHARED_LIBS` if you want a DLL).
6. Click **Generate**.
7. Click **Open Project** to launch in Visual Studio 2022.

### From Visual Studio:

1. After opening the generated solution, right-click the `ALL_BUILD` target and select **Build**.
