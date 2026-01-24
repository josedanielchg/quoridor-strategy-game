English | [Français](../fr/BUILD_RUN.md)

# Build and Run

## Table of Contents
- [Purpose](#purpose)
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
  - [Project Management](PROJECT_MANAGEMENT.md)

## Purpose
Provide full build and run instructions for Windows and Linux.

[Back to README](../../README.md)

## Prerequisites
- C++20 compiler
  - Windows: MSVC (Visual Studio 2022 recommended)
  - Linux: g++ or clang++
- CMake >= <CMAKE_MIN_VERSION>
- Git
- SFML <SFML_VERSION>
  - This repo is configured to fetch and build SFML automatically via CMake FetchContent.

## Clone

```bash
git clone <LINK>
cd QuoridorGame
```

## Configure and build

Windows (MSVC / Visual Studio):

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

Debug build:

```powershell
cmake --build build --config Debug
```

Linux (g++ / clang):

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

Debug build:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
```

## Run
- Windows (multi-config):
  - `build/Release/QuoridorGame.exe`
  - `build/Debug/QuoridorGame.exe`
- Linux (single-config):
  - `./build/QuoridorGame`

## Assets
- Windows: assets are copied next to the executable after build.
- Linux: run from the repo root or copy `assets/` next to the executable.

## Troubleshooting

**Assets not found / textures missing**
Assets are loaded via relative paths (`assets/...`). Ensure your working directory is the repo root,
or copy `assets/` next to the executable.

**SFML DLLs missing (Windows)**
This project is configured for static SFML by default. If you switch to shared libraries, copy the
SFML DLLs next to `QuoridorGame.exe`.

**FetchContent fails to download SFML**
Verify Git is installed and in PATH, then delete `build/` and reconfigure:

```bash
cmake -S . -B build
```

