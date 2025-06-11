<p align="right">
    <a name="readme-top"></a>
    <a href="/LICENSE.txt"><img src="https://img.shields.io/badge/license-MIT-green" /></a> <a href=""><img src="https://img.shields.io/badge/version-0.1-green" /></a> 
</p>
<br><br>
<p align="center">
    <h1 align="center">BlueDot SM/ITG Light Drivers</h1>
    <p align="center">This repository contains the source code for the light drivers to use
with Stepmania/OpenITG/ITGMania enabling support of BlueDot light boards.</p>
    <p align="center"><strong><a href="https://docs.bluedotarcade.com/boards/bd845-pwb">Explore the docs</a></strong></p>
    <br><br>
</p>

## Support table

### Supported boards

| Board name | Description | Status |
|------------|-------------|--------|
| [BlueDot Unilight] | Driving DDR cabinet lights through USB. | Supported |

### Supported games

| Game name | Platform | Status | Driver |
|-----------|----------|--------|--------|
| [Stepmania] | All | Unsupported | - |
| [OpenITG] | All | Unsupported | - |
| [ITGMania] | Win64 | Supported | pacdrive64.dll |
| [ITGMania] | Linux | Unsupported | - |
| [ITGMania] | MacOS | Unsupported | - |

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
3. Make sure both CMake and MinGW-w64 are added to the system PATH.
3. Clone the repository by running the following commands in a terminal:

```shell
git clone git@github.com:bluedot-arcade/sm-light-drivers.git
cd sm-light-drivers
git submodule update --init --recursive
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
git submodule update --init --recursive
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


[BlueDot Unilight]: https://docs.bluedotarcade.com/boards/unilight
[ITGMania]: https://www.itgmania.com/
[Stepmania]: https://www.stepmania.com/
[OpenITG]: https://github.com/openitg/openitg