<p align="right">
    <a name="readme-top"></a>
    <a href="/LICENSE.txt"><img src="https://img.shields.io/badge/license-MIT-green" /></a> <a href=""><img src="https://img.shields.io/badge/version-0.5-green" /></a> 
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
| BlueDot UNISON     | Single pad IO board with cabinet lights. Built in a P1 and a P2 version. | Supported |

Each driver is compiled once per board, see [Target boards](#target-boards).

### Supported games and platforms

The following table lists the supported combinations of games and platforms and the corresponding suggested
driver to use:

| Game        | Platform | Driver                 |
|-------------|----------|------------------------|
| [Stepmania] | Win32    | parallel_lights_io.dll |
| [OpenITG]   | Win32    | parallel_lights_io.dll |
| [ITGMania]  | Win64    | PacDrive64.dll         |

## Compile from source

The project can be compiled using CMake both on Windows and Linux.

Windows platform will be able to generate only Windows binaries, while
Linux will be able to generate both Windows and Linux binaries using
a MinGW-w64 cross-toolchain.

Both MinGW-w64 and Visual Studio can be used to compile all the drivers.

### Target boards

The target board is selected at compile time with the `BD_BOARD_UNILIGHT` or `BD_BOARD_UNISON` macro.
The UNISON board drives a single pad, so its drivers also need `BD_UNISON_PLAYER` set to `1` or `2`
to select which player's pad lights it shows. Cabinet lights are driven by both versions.

By default CMake builds the drivers for every board, placing each board's DLLs in its own directory:

```
build/unilight/PacDrive32.dll
build/unilight/parallel_lights_io.dll
build/unison/p1/PacDrive32.dll
build/unison/p1/parallel_lights_io.dll
build/unison/p2/PacDrive32.dll
build/unison/p2/parallel_lights_io.dll
```

On 64-bit builds the PacDrive driver is named `PacDrive64.dll`.

To build only some boards, set the `BD_BOARDS` CMake variable, e.g. `cmake -DBD_BOARDS=unison ..`.

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

The compiled binaries will be located in the `build/<board>` directories, e.g. `build/unison/p1`.

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

3. Configure the project to use the MinGW-w64 cross-toolchain (from the repository root):

```shell
cmake --preset win64
```

You can swap `win64` with `win32` to generate 32-bit binaries.

4. Build the project:

```shell
cmake --build --preset win64
```

The compiled binaries will be located in the `build/<preset>/<board>` directories, e.g. `build/win64/unison/p1/PacDrive64.dll`.


[BlueDot Unilight]: https://docs.bluedotarcade.com/boards/unilight
[ITGMania]: https://www.itgmania.com/
[Stepmania]: https://www.stepmania.com/
[OpenITG]: https://github.com/openitg/openitg
