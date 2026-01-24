English | [Français](../fr/BUILD_RUN.md)

# Build and Run

## Table of Contents
- [Prerequisites](#prerequisites)
- [Clone](#clone)
- [Configure and build](#configure-and-build)
- [Run](#run)
- [Assets](#assets)
- [Troubleshooting](#troubleshooting)
- Docs
  - [README](../../README.md)
  - [Build & Run](BUILD_RUN.md)
  - [Gameplay & Controls](CONTROLS.md)
  - [Project Structure](PROJECT_STRUCTURE.md)
  - [File Responsibilities](FILE_RESPONSIBILITIES.md)
  - [Heuristic AI](HEURISTIC_AI.md)
  - [OOP Concepts Used](OOP_CONCEPTS_USED.md)
  - [Code Quality](CODE_QUALITY.md)

## Prerequisites
- C++20 compiler
  - Windows: MSVC (Visual Studio 2022 recommended)
  - Linux: g++ or clang++
- CMake >= 3.16
- Git
- SFML 3.0
  - This repo is configured to fetch and build SFML automatically via CMake FetchContent.

## Clone

```bash
git clone https://github.com/josedanielchg/quoridor-strategy-game.git
cd QuoridorGame
```

## Configure and build

Windows (MSVC / Visual Studio):

```powershell
cmake -S . -B build
cmake --build build
```

Debug build:

```powershell
cmake --build build
```

Linux (g++ / clang):

```bash
cmake -S . -B build =Release
cmake --build build
```

Debug build:

```bash
cmake -S . -B build =Debug
cmake --build build
```

## Run
- Windows (multi-config):
  - `build/Debug/QuoridorGame.exe`
- Linux (single-config):
  - `./build/QuoridorGame`
