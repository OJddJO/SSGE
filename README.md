# SSGE - Simple SDL2 Game Engine

## Description
`SSGE` is a simple game engine written in C. It is made on top of the [`SDL2`](https://github.com/libsdl-org/SDL/tree/SDL2) library. The engine is designed to be simple, lightweight and easy to use. It is not a full game engine, but rather a simple framework to build a game on top of.

## Features
- Simple object system
- Simple graphics / rendering system
- Event Handling
- Tilemap support
- Audio support
- Font and text rendering
- ... and more

## Usage
To use the engine, download the release and copy the files into your project. You will need to link against `SDL2` and `SSGE` libraries.
See [`example/makefile`](./example/makefile)

## Example
See [`example/src/tictactoe.c`](./example/src/tictactoe.c) for an example of how to use the engine.
You can also see the [Minesweeper repo](https://github.com/OJddJO/Minesweeper) I made with this engine. (WARNING: `SSGE` version may be different)

## Documentation
Each functions and structures are documented in the source code. See the header files for more information.

## Todo
- ⬛ Z-Axis Map
- ✅ Animation System
- ⬛ (maybe) Switch to SDL3
- ⬛ (maybe) Make a GUI (game engine)

## License
This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.
