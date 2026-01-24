English | [Français](../fr/PROJECT_STRUCTURE.md)

# Project Structure

## Table of Contents
- [High-level tree](#high-level-tree)
- [Module map](#module-map)
- [Runtime flow (short)](#runtime-flow-short)
- Docs
  - [README](../../README.md)
  - [Build & Run](BUILD_RUN.md)
  - [Gameplay & Controls](CONTROLS.md)
  - [Project Structure](PROJECT_STRUCTURE.md)
  - [File Responsibilities](FILE_RESPONSIBILITIES.md)
  - [Heuristic AI](HEURISTIC_AI.md)
  - [OOP Concepts Used](OOP_CONCEPTS_USED.md)
  - [Code Quality](CODE_QUALITY.md)

## High-level tree

```text
QuoridorGame/
  CMakeLists.txt
  README.md
  LICENSE
  assets/
    fonts/
    sound/
    textures/
    tutorial/
  cmake/
    FindSFML.cmake
  docs/
    en/
    fr/
    media/
  include/
    app/
    audio/
    game/
    heuristic/
    ui/
  src/
    app/
    audio/
    game/
    heuristic/
    ui/
    main.cpp
  build/ (generated)
```

## Module map

- App: `include/app/`, `src/app/`
  - Application lifecycle, screen routing, and screen implementations.
- Game: `include/game/`, `src/game/`
  - Core rules, logical state, and board entities.
- UI: `include/ui/`, `src/ui/`
  - Rendering, HUD, menus, input widgets, and view helpers.
- Heuristic: `include/heuristic/`, `src/heuristic/`
  - CPU decision-making (search + evaluation + transposition table).
- Audio: `include/audio/`, `src/audio/`
  - Sound effects manager and sound pooling.

## Runtime flow (short)

1) `main.cpp` creates `App::Application` and enters `run()`.
2) `Application` initializes all screens and sets the active one.
3) The main loop runs: process events -> update -> render.
4) Gameplay updates include human input, AI turns, and UI updates.
5) When the user quits, the window closes and the loop ends.

