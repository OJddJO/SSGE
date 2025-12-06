# SSGE - Simple SDL2 Game Engine

[![Release new version](https://github.com/OJddJO/SSGE/actions/workflows/release.yml/badge.svg)](https://github.com/OJddJO/SSGE/actions/workflows/release.yml)

## Description
`SSGE` is a simple game engine written in C. It is made on top of the [`SDL2`](https://github.com/libsdl-org/SDL/tree/SDL2) library. The engine is designed to be simple, lightweight and easy to use. It is not a full game engine, but rather a simple framework to build a game on top of.

## Features
- Simple object system
- Simple graphics / rendering system
- Event Handling
- Tilemap support
- Audio support
- Font and text rendering
- Fully compatible with `SDL2`
- and more ...

## Usage
To use the engine, download the release and copy the files into your project. You will need to link against `SSGE` libraries.
See [`example/makefile`](./example/makefile)

The Linux version is built on `Ubuntu 24.04`. If you are using a different OS, you may need to rebuild the engine on your system. Ensure that you have installed the `SDL2` library, along with `SDL2_image`, `SDL2_mixer` and `SDL2_ttf`, on your system.

### NixOS
You can use the [`shell.nix`](shell.nix) file.

### Ubuntu / Debian
Install the required libraries with:
```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev -yq
```

### Arch Linux
Install the required libraries with:
```bash
sudo pacman -Sq --noconfirm sdl2 sdl2_image sdl2_ttf sdl2_mixer
```

### Windows (MSYS2)
Install the required libraries with:
```bash
pacman -Sq --noconfirm mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_mixer mingw-w64-x86_64-SDL2_ttf
```

## Example
See [`example/src/tictactoe.c`](./example/src/tictactoe.c) for an example of how to use the engine.
You can also see the [Minesweeper repo](https://github.com/OJddJO/Minesweeper) I made with this engine. (**WARNING**: `SSGE` version may be different)

## Documentation
Each functions and structures are documented in the source code. See the header files for more information.
I will make a proper documentation later :/

## Todo
- ⬛ Z-Axis Map
- ✅ Animation System
- ⬛ 9-slice
- ⬛ (maybe) Switch to SDL3
- ⬛ (maybe) Make a GUI (game engine)

## License
This project is licensed under the `MIT License` - see the [LICENSE](./LICENSE) file for details.
