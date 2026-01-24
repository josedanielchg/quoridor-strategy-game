# QuoridorGame

A Quoridor-style strategy game with an isometric SFML UI and a heuristic CPU opponent.
Built in C++ (C++20) using SFML for graphics/audio/windowing.

## Table of Contents
- [Overview](#overview)
- [Key Features](#key-features)
- [Installation](#installation)
- [Demo](#demo)
- [Screenshots](#screenshots)
- [Documentation](#documentation)
- [Controls](#controls)
- [Project Structure](#project-structure)
- [Architecture Overview](#architecture-overview)
- [File Responsibility Table (ALL .hpp)](#file-responsibility-table-all-hpp)
- [Credits](#credits)
- [License](#license)
- [Contributing](#contributing)
- [Additional Notes](#additional-notes)

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

## Installation
Step-by-step:

1) Prerequisites
- C++20 compiler
  - Windows: MSVC (Visual Studio 2022 recommended)
  - Linux: g++ or clang++
- CMake >= 3.16
- Git
- SFML 3.0.0
  - This repo is configured to fetch and build SFML automatically via CMake FetchContent

2) Clone

```bash
git clone https://github.com/josedanielchg/quoridor-strategy-game.git
cd QuoridorGame
```

3) Configure and build

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

4) Run

- Windows (multi-config):
  - `build/Release/QuoridorGame.exe`
  - `build/Debug/QuoridorGame.exe`
- Linux (single-config):
  - `./build/QuoridorGame`

```bash
./build/QuoridorGame
```

5) Assets

- Windows: assets are copied next to the executable after build.
- Linux: run from the repo root or copy `assets/` next to the executable.

### Troubleshooting

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

## Demo
Quick try:

1) Build Release (see Installation).
2) Run the executable.
3) From the title/menu screen, choose Single Player or Multiplayer to start a match.

## Screenshots
Screenshots are stored in `docs/media/` (replace these placeholders with real captures if needed):

![Gameplay](docs/media/gameplay.png)
![Menu](docs/media/menu.png)
![Wall placement](docs/media/wall_placement.png)
![Win screen](docs/media/win_screen.png)

Files:
- `docs/media/gameplay.png`
- `docs/media/menu.png`
- `docs/media/wall_placement.png`
- `docs/media/win_screen.png`

## Documentation
- [Project Structure](docs/EN/PROJECT_STRUCTURE.md)
- [OOP Concepts Used](docs/EN/OOP_CONCEPTS_USED.md)
- [Code Quality](docs/EN/CODE_QUALITY.md)
- [Heuristic AI](docs/EN/HEURISTIC_AI.md)
- [Project Management](docs/EN/PROJECT_MANAGEMENT.md)

## Controls

| Action | Default Control |
|---|---|
| Open/close pause menu | `ESC` |
| Toggle wall placement mode | `W` |
| Rotate wall orientation | `R` |
| Select target / place move | Left mouse click |

## Project Structure

```text
QuoridorGame/
  CMakeLists.txt
  README.md
  LICENSE
  cmake/
    FindSFML.cmake
  assets/
    fonts/
    sound/
    textures/
    tutorial/
  docs/
    media/
      gameplay.png
      menu.png
      wall_placement.png
      win_screen.png
    EN/
      PROJECT_STRUCTURE.md
      OOP_CONCEPTS_USED.md
      CODE_QUALITY.md
      HEURISTIC_AI.md
      PROJECT_MANAGEMENT.md
    instructions.md
    structure.md
    Initial Report/
      initial_report.pdf
      imgs/
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

## Architecture Overview

High-level relationships:

```text
main
  -> Application (window + loop + screen routing)
      -> Screen (base)
          -> TitleScreen / MenuScreen / GameScreen / CreditsScreen / HowToPlayScreen
      -> GameScreen
          -> GameState (logical state)
          -> GameRules (validate/apply/generate/undo)
          -> Board (visual state rebuilt from GameState)
          -> GameRenderer + HUD + menus + bottom bar
          -> HeuristicEngine (async CPU move)
```

Game loop responsibilities:
- `processEvents()` polls SFML events and forwards them to the active screen.
- `update(dt)` advances animations and CPU turns.
- `render()` draws the active screen.

Resource loading:
- Textures, fonts, and audio are loaded from `assets/...` paths.
- The runtime expects the working directory to be the repo root (or `assets/` next to the executable).

## File Responsibility Table (ALL .hpp)

| File | Responsibility | Key classes/structs | Notes/Dependencies |
|---|---|---|---|
| `include/app/Application.hpp` | Window setup, main loop, screen routing | `App::Application` | Owns SFML window and switches screens |
| `include/app/Screen.hpp` | Base screen interface + shared music | `App::Screen` | Parent of all screens |
| `include/app/TitleScreen.hpp` | Title screen visuals + start trigger | `App::TitleScreen` | Uses SFML sprites/text |
| `include/app/MenuScreen.hpp` | Main menu + input handling | `App::MenuScreen` | Selects game/credits/how-to-play |
| `include/app/GameScreen.hpp` | Gameplay orchestration + CPU turn | `App::GameScreen` | Owns `GameState`, `Board`, `HeuristicEngine` |
| `include/app/CreditsScreen.hpp` | Credits view | `App::CreditsScreen` | Returns to menu |
| `include/app/HowToPlayScreen.hpp` | Tutorial screen | `App::HowToPlayScreen` | Script-driven dialogue |
| `include/audio/SfxManager.hpp` | SFX manager + sound pool | `Audio::SfxManager` | Uses `sf::SoundBuffer`/`sf::Sound` |
| `include/game/GameState.hpp` | Logical state container | `Game::GameState` | Source of truth for rules/AI |
| `include/game/MoveTypes.hpp` | Move and wall enums | `Game::MoveType`, `Game::Orientation` | Used across game/UI/AI |
| `include/game/Move.hpp` | Move value object | `Game::Move` | Produced by UI/AI |
| `include/game/GameRules.hpp` | Rules engine + distance cache | free functions, `Game::MoveUndoState` | Used by UI and AI |
| `include/game/WallRules.hpp` | Local wall legality | `isWallPlacementLegalLocal(...)` | Used by rules + wall entity |
| `include/game/Board.hpp` | Visual board model + entities | `Game::Board` | Rebuilt from `GameState` |
| `include/game/Field.hpp` | Board cell connectivity | `Game::Field`, `Game::Direction` | Graph edges for paths |
| `include/game/Entity.hpp` | Base entity interface | `Game::Entity` | Position + validation contract |
| `include/game/VisualEntity.hpp` | Entity + SFML sprite/texture | `Game::VisualEntity` | Base for Pawn/Wall |
| `include/game/Pawn.hpp` | Pawn entity | `Game::Pawn` | Delegates move legality to rules |
| `include/game/Wall.hpp` | Wall entity + preview sprites | `Game::Wall` | Validates locally via `WallRules` |
| `include/heuristic/HeuristicEngine.hpp` | CPU search engine | `Game::HeuristicEngine`, `Game::HeuristicSearchConfig` | Alpha-beta + heuristics |
| `include/heuristic/TranspositionTable.hpp` | TT + Zobrist hashing | `Game::TranspositionTable`, `Game::TTEntry`, `computeZobrist(...)` | Speeds up search |
| `include/heuristic/PathFinder.hpp` | A* path existence helper | `Game::PathFinder` | Not present in current repo (legacy placeholder) |
| `include/ui/UiConstants.hpp` | UI sizing/scaling constants | constants | Shared by renderer/menus |
| `include/ui/ViewUtils.hpp` | Letterbox view utility | `makeLetterboxView(...)` | Consistent scaling |
| `include/ui/GameRenderer.hpp` | Isometric renderer + mouse grid | `UI::GameRenderer` | Draws board/entities |
| `include/ui/Hud.hpp` | HUD UI | `UI::Hud` | Turn indicator + walls |
| `include/ui/Menu.hpp` | Generic menu container | `UI::Menu` | Base for pause/winner menus |
| `include/ui/StateComponent.hpp` | Hover/clickable sprite base | `UI::StateComponent` | Used by buttons |
| `include/ui/Button.hpp` | Labeled button widget | `UI::Button` | Click callbacks |
| `include/ui/PauseMenu.hpp` | Pause menu | `UI::PauseMenu` | Resume/Restart/Quit |
| `include/ui/WinnerMenu.hpp` | Winner menu | `UI::WinnerMenu` | Banner + actions |
| `include/ui/InGameBottomBar.hpp` | Bottom bar + controls | `UI::InGameBottomBar` | In-game menu button + wall controls |
| `include/ui/SpriteEntity.hpp` | Sprite helper variants | `UI::SpriteEntity`, `TileSprite`, `WallSprite`, `PawnSprite` | Utility / alternative approach |
| `include/ui/InputHandler.hpp` | Input module placeholder | - | Not present in current repo (legacy placeholder) |
| `include/ui/Renderer.hpp` | Renderer module placeholder | - | Not present in current repo (legacy placeholder) |

## Credits

- Developers:
  - `<DEVELOPER_1>`
  - `<DEVELOPER_2>`
- Music tool:
  - Suno (`<NOTES_ABOUT_TRACKS/USAGE>`)
- Image tool:
  - Gemini (`<NOTES_ABOUT_ASSETS/USAGE>`)

## License

This project is licensed under `<LICENSE_NAME>`. See `LICENSE` for details.

## Contributing

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/my-change`
3. Commit your changes
4. Push to your fork and open a Pull Request