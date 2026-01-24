English | [Français](../fr/CODE_QUALITY.md)

# Code Quality Notes

## Table of Contents
- [Purpose](#purpose)
- [Naming conventions](#naming-conventions)
- [File naming](#file-naming)
- [Includes and forward declarations](#includes-and-forward-declarations)
- [Separation of concerns](#separation-of-concerns)
- [Ownership and lifetime](#ownership-and-lifetime)
- [Error handling](#error-handling)
- [Formatting and tooling](#formatting-and-tooling)
- Docs
  - [README](../../README.md)
  - [Build & Run](BUILD_RUN.md)
  - [Gameplay & Controls](CONTROLS.md)
  - [Project Structure](PROJECT_STRUCTURE.md)
  - [File Responsibilities](FILE_RESPONSIBILITIES.md)
  - [Heuristic AI](HEURISTIC_AI.md)
  - [OOP Concepts Used](OOP_CONCEPTS_USED.md)
  - [Code Quality](CODE_QUALITY.md)
  - [Project Management](PROJECT_MANAGEMENT.md)

## Purpose
Explain the coding conventions and quality practices used in this repository.

[Back to README](../../README.md)

## Naming conventions
- Classes and structs: PascalCase (e.g., `GameScreen`, `HeuristicEngine`).
- Methods and functions: lowerCamelCase (e.g., `handleEvent`, `setGameMode`).
- Member variables: `m_` prefix (e.g., `m_gameState`, `m_window`).
- Constants: ALL_CAPS for enums or constant-like values (e.g., `MAX_WALLS_PER_PLAYER`).
- Namespaces: short, module-based (`App`, `Game`, `UI`, `Audio`).

## File naming
- Header files: `.hpp` and source files: `.cpp`.
- Files usually match the main class name (e.g., `GameScreen.hpp/.cpp`).
- Directory layout maps to modules: `include/app`, `include/game`, `include/ui`, `include/heuristic`, `include/audio`.

## Includes and forward declarations
- Headers include what they use.
- Forward declarations are used where possible to reduce header coupling (e.g., `Entity` forward-declares `Board`).

## Separation of concerns
- App: lifecycle, window, screen routing (`include/app/`, `src/app/`).
- Game: rules, state, and entities (`include/game/`, `src/game/`).
- UI: rendering and widgets (`include/ui/`, `src/ui/`).
- Heuristic: CPU search and cache (`include/heuristic/`, `src/heuristic/`).
- Audio: sound effects manager (`include/audio/`, `src/audio/`).

## Ownership and lifetime
- `App::Application` owns screens via `std::unique_ptr` and switches the active one.
- `App::GameScreen` owns gameplay state and UI components by value.
- `Game::Board` owns fields, pawns, and walls.
- `Audio::SfxManager` is a singleton that owns sound buffers and a sound pool.

## Error handling
- Most `init` or resource-load functions return `bool` and log to `std::cerr`.
- The project does not use exceptions for control flow.

## Formatting and tooling
- No `.clang-format` or automated formatter is present in the repo.
- Style is consistent: 4-space indentation, braces on new lines, and clear spacing.

