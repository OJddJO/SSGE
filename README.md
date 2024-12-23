# C-GameEngine

## Description
This is a simple game engine written in C. It is designed to be used with the SDL2 library. The engine is designed to be simple and easy to use. It is not designed to be a full game engine, but rather a simple framework to build a game on top of.

## Features
- Simple game loop
- Simple input handling
- Simple rendering
- Simple audio playback
- Simple collision detection
- Simple entity system

## Usage
To use the engine, download the release and copy the files into your project.
You will need to link against the SDL2 library.
```bash
gcc -o main main.c -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
```

## Example
See the [`example.c`](./src/example.c) and [`game.c`](./src/game.c) files for an example of how to use the engine.

## License
This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.