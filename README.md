# BlueDot SM/ITG Light Drivers

This project contains the source code for the light drivers to use
with Stepmania/OpenITG/ITGMania enabling support of BlueDot light boards.

Supported boards:
- BlueDot Unilight

Supported Stepmania versions:
- ITGMania (Windows x64)

## Compile from source

The project can be compiled using CMake both on Windows and Linux.

Windows platform will be able to generate only Windows binaries, while
Linux will be able to generate both Windows and Linux binaries using
a MinGW-w64 cross-toolchain.

### Compile on Windows

This is the suggested way to compile the project on Windows using
MinGW-w64. You can also use Visual Studio if you prefer.

1. Install CMake from https://cmake.org/download/
2. Install MinGW-w64 from https://sourceforge.net/projects/mingw-w64/
3. Add the MinGW-w64 bin directory to the system PATH
3. Clone the repository by running the following commands in a terminal:

```shell
git clone git@github.com:bluedot-arcade/sm-light-drivers.git
cd sm-light-drivers
```

4. Create a build directory and configure the project using CMake:

```shell
mkdir build
cd build
cmake ..
```

5. Build the project using the generated build system:

```shell
cmake --build .
```

The compiled binaries will be located in the `build` directory.

### Compile on Linux

1. Install CMake and MinGW-w64 cross-toolchain:

```shell
sudo apt-get install cmake mingw-w64
```

2. Clone the repository by running the following commands in a terminal:

```shell
git clone git@github.com:bluedot-arcade/sm-light-drivers.git
cd sm-light-drivers
```

3. Create a build directory and configure the project using CMake:

```shell
mkdir build
cd build
```

4. Configure the project to use the MinGW-w64 cross-toolchain:

```shell
cmake --preset=win64 ..
```

You can swap `win64` with `win32` to generate 32-bit binaries.

5. Build the project using the generated build system:

```shell
cmake --build .
```

The compiled binaries will be located in the `build` directory.



