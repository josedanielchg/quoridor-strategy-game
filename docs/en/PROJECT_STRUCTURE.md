# Project Structure

Purpose: Give a first-look view of the codebase layout and module boundaries.

[Back to README](../../README.md)

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
    EN/
      PROJECT_STRUCTURE.md
      OOP_CONCEPTS_USED.md
      CODE_QUALITY.md
      HEURISTIC_AI.md
      PROJECT_MANAGEMENT.md
    media/
      gameplay.png
      menu.png
      wall_placement.png
      win_screen.png
    instructions.md
    structure.md
    Initial Report/
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
