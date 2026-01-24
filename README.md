English | [Français](README_FR.md)

# QuoridorGame

A Quoridor-style strategy game with an isometric SFML UI and a heuristic CPU opponent.
Built in C++ (C++20) using SFML for graphics/audio/windowing.

## Table of Contents
- [Overview](#overview)
- [Key Features](#key-features)
- [Installation (Quick Start)](#installation-quick-start)
- [Screenshots](#screenshots)
- [Architecture Overview](#architecture-overview)
- [Credits](#credits)
- [Contributing](#contributing)
- Docs
  - [Build & Run](docs/en/BUILD_RUN.md)
  - [Gameplay & Controls](docs/en/CONTROLS.md)
  - [Project Structure](docs/en/PROJECT_STRUCTURE.md)
  - [File Responsibilities](docs/en/FILE_RESPONSIBILITIES.md)
  - [Heuristic AI](docs/en/HEURISTIC_AI.md)
  - [OOP Concepts Used](docs/en/OOP_CONCEPTS_USED.md)
  - [Code Quality](docs/en/CODE_QUALITY.md)

## Overview
This project implements a 2-player Quoridor board game on a 9x9 grid. Players race to
reach the opposite side while placing walls to slow the opponent, under the constraint
that a path to the goal must always remain.

The game uses a screen-based UI, isometric rendering, and a CPU opponent based on
heuristic search.

## Key Features
- Quoridor ruleset with legal wall placement enforcement
- Isometric board rendering with mouse hover and wall preview
- Human vs CPU gameplay (CPU computed asynchronously)
- Heuristic AI: alpha-beta search, move ordering, and transposition table
- Distance cache (BFS) for legality checks and evaluation
- HUD showing turn and remaining walls
- Pause and winner menus
- CMake-based build with SFML fetched via FetchContent

## Installation (Quick Start)

Windows (MSVC / Visual Studio):

```powershell
cmake -S . -B build
cmake --build build
```

Linux (g++ / clang):

```bash
cmake -S . -B build
cmake --build build
```

Run:
- Windows: `build/Release/QuoridorGame.exe`
- Linux: `./build/QuoridorGame`

For full prerequisites, debug builds, and troubleshooting, see
[docs/en/BUILD_RUN.md](docs/en/BUILD_RUN.md).

## Screenshots

![Gameplay](docs/media/gameplay.png)
![Start](docs/media/start.png)

## Architecture Overview
- The entry point creates `App::Application`, which owns the SFML window and all screens.
- Screens inherit from `App::Screen` and implement event handling, updates, and rendering.
- `GameScreen` orchestrates gameplay using `GameState`, `GameRules`, `Board`, UI components, and the heuristic AI.
- The CPU turn runs asynchronously with `std::async` so rendering stays responsive.

For more detail, see [docs/en/PROJECT_STRUCTURE.md](docs/en/PROJECT_STRUCTURE.md).

## Credits
- Developers:
  - CHACON GOMEZ Jose Daniel
  - GitHub: https://github.com/josedanielchg
  - MENESES GAMBOA Carlos Adrian
  - GitHub: https://github.com/MenesesCarlos29
- Music:
  - Suno (v4.5-all)
  - Tracks used:
    - Castle Circuits (Version 1)
    - Castle Circuits (Version 2)
  - Source: https://suno.com/
  - No commercial use
- Art / Images:
  - Gemini (Nano Banana Pro)
  - Source: https://gemini.google.com/
  - No commercial use

## Contributing
1. Fork the repository
2. Create a feature branch: `git checkout -b feature/my-change`
3. Commit your changes
4. Push to your fork and open a Pull Request