# Pong Game with Multi-Ball Support

This repository contains a classic Pong game implemented in C++ using SDL2. Inspired by the book *Game Programming in C++* by Sanjay Madhav, this version extends the original design to include multi-ball support for an even more exciting gameplay experience.

## Features

- **Classic Pong Gameplay:** Enjoy the timeless two-player Pong game with responsive paddle controls.
- **Multi-Ball Support:** Experience enhanced gameplay with multiple balls active on screen simultaneously.
- **SDL2 Integration:** Utilizes SDL2 for window creation, rendering, and handling keyboard inputs.
- **Cross-Platform Compatibility:** Designed to build and run on various platforms, including Windows, macOS, and Linux.

## Project Structure

- **main.cpp:** Entry point of the application. Initializes the game and starts the main game loop.
- **Game.cpp / Game.h:** Contains the core game logic, including initialization, input processing, game state updates, and rendering.
- **README.md:** Provides project overview and build instructions.
- Additional files and configurations for building the project.

## Build and Run Instructions

### Prerequisites
- [SDL2](https://www.libsdl.org/) must be installed on your system.
- A C++ compiler supporting C++11 or later.

### Building from the Terminal
Navigate to the project directory and run the following commands:

```bash
# Navigate to the project folder
cd /path/to/your/project

# Compile the project (adjust include and library paths as needed)
g++ -std=c++11 -Wall -O0 -g -I/path/to/SDL2/include -L/path/to/SDL2/lib main.cpp Game.cpp -lSDL2 -o pong

# Run the game
./pong
