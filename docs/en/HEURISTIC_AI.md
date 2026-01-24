English | [Français](../fr/HEURISTIC_AI.md)

# Heuristic AI

## Table of Contents
- [Overview](#overview)
- [Key classes and files](#key-classes-and-files)
- [Search flow (step-by-step)](#search-flow-step-by-step)
- [Evaluation inputs](#evaluation-inputs)
- [Configuration and difficulty](#configuration-and-difficulty)
- [CPU integration in gameplay](#cpu-integration-in-gameplay)
- Docs
  - [README](../../README.md)
  - [Build & Run](BUILD_RUN.md)
  - [Gameplay & Controls](CONTROLS.md)
  - [Project Structure](PROJECT_STRUCTURE.md)
  - [File Responsibilities](FILE_RESPONSIBILITIES.md)
  - [Heuristic AI](HEURISTIC_AI.md)
  - [OOP Concepts Used](OOP_CONCEPTS_USED.md)
  - [Code Quality](CODE_QUALITY.md)

## Overview
The CPU uses a depth-limited alpha-beta search with move ordering and a transposition table.
It evaluates positions using a heuristic function based on distance to goal, wall counts,
and local mobility.

## Key classes and files
- `include/heuristic/HeuristicEngine.hpp`
- `src/heuristic/HeuristicEngine.cpp`
- `include/heuristic/TranspositionTable.hpp`
- `src/heuristic/TranspositionTable.cpp`
- `include/game/GameRules.hpp` and `src/game/GameRules.cpp` (move generation and evaluation helpers)
- `include/app/GameScreen.hpp` and `src/app/GameScreen.cpp` (CPU turn integration)

## Search flow (step-by-step)
1) `GameScreen` detects a CPU turn and schedules a background search using `std::async`.
2) `HeuristicEngine::findBestMove` prepares a working copy of `GameState` and sets a deadline
   based on `HeuristicSearchConfig::timeLimitMs`.
3) It generates ordered moves using heuristics and the transposition table best move.
4) It runs alpha-beta search from each root move, with pruning and a depth limit.
5) Terminal positions return a large win/loss score; non-terminal positions use the heuristic
   evaluation (distance, walls, mobility).
6) The transposition table stores results with flags (exact/lower/upper) to speed up later nodes.
7) The best move is returned to `GameScreen`, which applies it to the live state.

## Evaluation inputs
- Distance to goal for both players (shorter is better).
- Remaining walls for each player.
- Local mobility near the pawn (reducing neighbors).

## Configuration and difficulty
`HeuristicSearchConfig` controls:
- `maxDepth` (search depth)
- `timeLimitMs` (time budget)
- `difficulty` (`HeuristicDifficulty` enum)
- `weightDistance`, `weightWalls`, `weightWidth` (evaluation weights)

## CPU integration in gameplay
- `GameScreen::runHeuristicTurn()` marks a CPU turn as pending.
- `GameScreen::updateHeuristicTurn()` launches `std::async` when needed and checks the future
  without blocking the render loop.
- When the future is ready, the move is applied and the HUD is updated.

