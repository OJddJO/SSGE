# SSGE - Simple SDL2 Game Engine

## Description
SSGE is a simple game engine written in C. It is made on top of the [`SDL2`](https://github.com/libsdl-org/SDL/tree/SDL2) library. The engine is designed to be simple and easy to use. It is not a full game engine, but rather a simple framework to build a game on top of.

## Features
- Simple object system
- Simple graphics / rendering system
- Event Handling
- Tilemap support
- Audio support
- Font and text rendering

## Usage
To use the engine, download the release and copy the files into your project. You will need to link against `SDL2` and `SSGE` libraries.
See [`example/makefile`](./example/makefile)

## Example
See the [`example/src/tictactoe.c`](./example/src/tictactoe.c) and [`example/src/game.c`](./example/src/game.c) files for an example of how to use the engine.

## License
This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.
