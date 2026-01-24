English | [Français](../FR/FUNCTIONALITIES.md)

# Functionalities

## Table of Contents
- [Overview](#overview)
- [Key Features](#key-features)
- [How to Use](#how-to-use)
- [Limitations / Known Constraints](#limitations--known-constraints)
- Docs
  - [Build & Run](BUILD_RUN.md)
  - [Gameplay & Controls](CONTROLS.md)
  - [Project Structure](PROJECT_STRUCTURE.md)
  - [File Responsibilities](FILE_RESPONSIBILITIES.md)
  - [Heuristic AI](HEURISTIC_AI.md)
  - [OOP Concepts Used](OOP_CONCEPTS_USED.md)
  - [Code Quality](CODE_QUALITY.md)

## Overview
QuoridorGame is a C++20 implementation of the 2-player Quoridor board game on a 9x9 grid, built with SFML for graphics, audio, and windowing.
Players race to the opposite side while placing walls, with the rule that at least one path to the goal must always remain.
The game uses a screen-based UI with isometric rendering and includes a heuristic CPU opponent.

## Key Features
- Game rules and board: Quoridor ruleset on a 9x9 grid with legal wall placement enforcement and the mandatory path-to-goal constraint.
- Game mode: Human vs CPU gameplay, with the CPU turn computed asynchronously to keep rendering responsive.
- Rendering and UI: Isometric board rendering with mouse hover and wall preview, plus a HUD showing the current turn and remaining walls.
- Menus: Pause and winner menus.
- AI: Heuristic AI with alpha-beta search, move ordering, and a transposition table; distance cache (BFS) for legality checks and evaluation.
- Build: CMake-based build with SFML fetched via FetchContent.

## How to Use
Mouse interaction is used for board hover/preview, and keyboard controls are documented separately.
See [Gameplay & Controls](CONTROLS.md) for the complete mapping.