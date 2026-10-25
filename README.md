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

| Board              | Description                                     |
|--------------------|-------------------------------------------------|
| [BlueDot Unilight] | Driving DDR cabinet lights through USB.         |
| BlueDot UNISON     | Single pad IO board, with cabinet lights.       |

| Game          | Platform    | Use                                          |
|---------------|-------------|----------------------------------------------|
| [Stepmania] 3.9 | Win32     | [parallel_lights_io.dll](#parallel_lights_iodll) |
| [OpenITG]     | Win32       | [parallel_lights_io.dll](#parallel_lights_iodll) |
| [Stepmania] 5 | Win32/Win64 | [PacDrive](#pacdrive)                        |
| [ITGMania]    | Win64       | [SextetStream](#sextetstream) (UNISON), built-in `HidBlueDot` driver (Unilight) |

Drivers are built once per board, in the `unilight`, `unison/p1` and `unison/p2` directories. A UNISON
board drives a single pad, so pick the build of the player whose pad it is connected to.

## Usage

### parallel_lights_io.dll

Emulates the parallel port lights output: LPT1 cabinet lights, LPT2 P1 pad, LPT3 P2 pad.

1. Copy `parallel_lights_io.dll` built for your board next to the game executable.
2. In `Preferences.ini` set:

```ini
LightsDriver=Parallel
```

### PacDrive

Emulates the Ultimarc PacDrive library, using the LumenAR/OpenITG light order.

1. Copy the DLL built for your board next to the game executable: `PacDrive32.dll` for 32-bit games,
   `PacDrive64.dll` for 64-bit games.
2. In `Preferences.ini` set:

```ini
LightsDriver=PacDrive
PacDriveLightOrdering=lumenar
```

### SextetStream

`UnisonSextet.exe` is a tray app that forwards the lights sent by the game SextetStream driver to up to
two UNISON boards: the first board drives the P1 pad, the second the P2 pad (swap them from the tray menu).

1. Start `UnisonSextet.exe` **before** the game, e.g. with a shortcut in the `shell:startup` folder.
2. In `Preferences.ini` set:

```ini
LightsDriver=SextetStreamToFile
SextetStreamOutputFilename=\\.\pipe\StepMania-Lights-SextetStream
```

## Compile from source

The project is compiled with CMake and MinGW-w64 (Visual Studio works too), either on Windows or
cross-compiling from Linux.

1. Install CMake and MinGW-w64 (on Linux: `sudo apt-get install cmake ninja-build mingw-w64`).
2. Clone the repository:

```shell
git clone git@github.com:bluedot-arcade/sm-light-drivers.git
cd sm-light-drivers
git submodule update --init --recursive
```

3. Configure and build, using `win64` or `win32` (the presets use the MinGW-w64 cross-toolchain):

```shell
cmake --preset win64
cmake --build --preset win64
```

On Windows you can also build without presets: `cmake -B build` and `cmake --build build`.

The binaries are placed in a directory per board:

```
build/win64/unilight/     PacDrive64.dll  parallel_lights_io.dll
build/win64/unison/p1/    PacDrive64.dll  parallel_lights_io.dll
build/win64/unison/p2/    PacDrive64.dll  parallel_lights_io.dll
build/win64/unison/       UnisonSextet.exe
```

The PacDrive driver is named `PacDrive32.dll` on 32-bit builds. To build only some boards, set
`BD_BOARDS`, e.g. `cmake --preset win64 -DBD_BOARDS=unison`. The board is selected in code with the
`BD_BOARD_UNILIGHT`/`BD_BOARD_UNISON` and `BD_UNISON_PLAYER` macros.


[BlueDot Unilight]: https://docs.bluedotarcade.com/boards/unilight
[ITGMania]: https://www.itgmania.com/
[Stepmania]: https://www.stepmania.com/
[OpenITG]: https://github.com/openitg/openitg
