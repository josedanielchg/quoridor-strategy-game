## Fill these placeholders

Before publishing this README, replace:

- `<PROJECT_NAME>`: repository/project display name  
- `<EXECUTABLE_NAME>`: built binary name (e.g., `QuoridorGame`)  
- `<BUILD_FOLDER_NAME>`: build directory name (commonly `build`)  
- `<CMAKE_MIN_VERSION>`: minimum required CMake version  
- `<SFML_VERSION>`: SFML version used by the project  
- `<LINK>`: repository clone URL  
- `<LICENSE_NAME>`: license identifier  
- Credits: developer names and asset/tool notes  
- Add demo media under `docs/media/<...>`

---

# <PROJECT_NAME>

A Quoridor-style strategy game with an isometric SFML UI and a heuristic CPU opponent.  
Built in C++ (C++20) using SFML for graphics/audio/windowing.

---

## Overview

This project implements a 2‑player **Quoridor** board game on a **9×9 grid**. Players race to reach the opposite side while placing walls to slow the opponent—under the constraint that a path to the goal must always remain.

The game is built in **C++ + SFML**, featuring a screen-based UI, isometric rendering, and a CPU player based on heuristic search.

### Key Features

- Quoridor ruleset with legal wall placement enforcement
- Isometric board rendering with mouse hover + wall preview
- Human vs CPU gameplay (CPU computed asynchronously)
- Heuristic AI: alpha–beta search + move ordering + transposition table
- Fast distance cache (BFS) for legality checks and evaluation
- HUD showing turn and remaining walls
- Pause and winner menus
- CMake-based build with FetchContent SFML

---

## Demo / Screenshots

Place media files in `docs/media/` and reference them here:

- `docs/media/gameplay.gif`
- `docs/media/menu.png`
- `docs/media/wall_placement.png`
- `docs/media/win_screen.png`

---

## Installation & Build Instructions

### Prerequisites

- C++20 compiler:
  - Windows: MSVC (Visual Studio 2022 recommended)
  - Linux: `g++` or `clang++`
- CMake `>= <CMAKE_MIN_VERSION>`
- Git
- SFML `<SFML_VERSION>`  
  - This repo is configured to **fetch and build SFML automatically** via CMake.

### Clone

```bash
git clone <LINK>
cd <PROJECT_NAME>
```

### Build with CMake

> Replace `<BUILD_FOLDER_NAME>` with your preferred build directory (commonly `build`).

#### Windows (MSVC / Visual Studio)

```powershell
cmake -S . -B <BUILD_FOLDER_NAME> -G "Visual Studio 17 2022" -A x64
cmake --build <BUILD_FOLDER_NAME> --config Release
```

Debug build:

```powershell
cmake --build <BUILD_FOLDER_NAME> --config Debug
```

#### Linux (g++ / clang)

Release build:

```bash
cmake -S . -B <BUILD_FOLDER_NAME> -DCMAKE_BUILD_TYPE=Release
cmake --build <BUILD_FOLDER_NAME> -j
```

Debug build:

```bash
cmake -S . -B <BUILD_FOLDER_NAME> -DCMAKE_BUILD_TYPE=Debug
cmake --build <BUILD_FOLDER_NAME> -j
```

### Run

- Windows (multi-config):
  - `<BUILD_FOLDER_NAME>/Release/<EXECUTABLE_NAME>.exe`
  - `<BUILD_FOLDER_NAME>/Debug/<EXECUTABLE_NAME>.exe`
- Linux (single-config):
  - `./<BUILD_FOLDER_NAME>/<EXECUTABLE_NAME>`

```bash
./<BUILD_FOLDER_NAME>/<EXECUTABLE_NAME>
```

### Troubleshooting

**Assets not found / textures missing**  
Assets are loaded via relative paths (`assets/...`). Ensure your **working directory** is the repo root, or copy `assets/` next to the executable.

**SFML DLLs missing (Windows)**  
This project is configured for static SFML by default. If you switch to shared libraries, copy the SFML DLLs next to `<EXECUTABLE_NAME>.exe`.

**FetchContent fails to download SFML**  
Verify Git is installed and in PATH, then delete `<BUILD_FOLDER_NAME>/` and reconfigure:
```bash
cmake -S . -B <BUILD_FOLDER_NAME>
```

---

## Controls

| Action | Default Control |
|---|---|
| Open/close pause menu | `ESC` |
| Toggle wall placement mode | `W` |
| Rotate wall orientation | `R` |
| Select target / place move | Left mouse click |

---

## Project Structure

```text
<PROJECT_NAME>/
├─ CMakeLists.txt
├─ README.md
├─ LICENSE
├─ cmake/
│  └─ FindSFML.cmake
├─ docs/
│  ├─ instructions.md
│  ├─ structure.md
│  └─ media/
│     └─ <place screenshots/gifs here>
├─ include/
│  ├─ app/
│  ├─ game/
│  ├─ heuristic/
│  └─ ui/
├─ src/
│  ├─ app/
│  ├─ game/
│  ├─ heuristic/
│  ├─ ui/
│  └─ main.cpp
└─ assets/
   ├─ fonts/
   ├─ sound/
   └─ textures/
```

---

## Architecture Overview

High-level relationships:

```
main
  -> Application (window + loop + screen routing)
      -> Screen (base)
          -> TitleScreen / MenuScreen / GameScreen / CreditsScreen
      -> GameScreen
          -> GameState (logical state)
          -> GameRules (validate/apply/generate/undo)
          -> Board (visual state rebuilt from GameState)
          -> GameRenderer + HUD + Menus
          -> HeuristicEngine (async CPU move)
```

**Game loop responsibilities**
- `processEvents()` polls SFML events and forwards them to the active screen.
- `update(dt)` advances animations and CPU turns.
- `render()` draws the active screen.

**Resource loading**
- Textures, fonts, and audio are loaded from `assets/...` paths.  
- The runtime expects the working directory to be the repo root (or `assets/` next to the executable).

---

## File Responsibility Table (ALL .hpp)

| File | Responsibility | Key classes/structs | Notes/Dependencies |
|---|---|---|---|
| `include/app/Application.hpp` | Window setup, main loop, screen routing | `App::Application` | Owns SFML window and switches screens |
| `include/app/Screen.hpp` | Base screen interface + shared music | `App::Screen` | Parent of all screens |
| `include/app/TitleScreen.hpp` | Title screen visuals + start trigger | `App::TitleScreen` | Uses SFML sprites/text |
| `include/app/MenuScreen.hpp` | Main menu + input handling | `App::MenuScreen` | Selects game/credits |
| `include/app/GameScreen.hpp` | Gameplay orchestration + CPU turn | `App::GameScreen` | Owns `GameState`, `Board`, `HeuristicEngine` |
| `include/app/CreditsScreen.hpp` | Credits view | `App::CreditsScreen` | Returns to menu |
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
| `include/heuristic/HeuristicEngine.hpp` | CPU search engine | `Game::HeuristicEngine`, `Game::HeuristicSearchConfig` | Alpha–beta + heuristics |
| `include/heuristic/TranspositionTable.hpp` | TT + Zobrist hashing | `Game::TranspositionTable`, `Game::TTEntry`, `computeZobrist(...)` | Speeds up search |
| `include/heuristic/PathFinder.hpp` | A* path existence helper | `Game::PathFinder` | Not in main rule flow |
| `include/ui/UiConstants.hpp` | UI sizing/scaling constants | constants | Shared by renderer/menus |
| `include/ui/ViewUtils.hpp` | Letterbox view utility | `makeLetterboxView(...)` | Consistent scaling |
| `include/ui/GameRenderer.hpp` | Isometric renderer + mouse grid | `UI::GameRenderer` | Draws board/entities |
| `include/ui/Hud.hpp` | HUD UI | `UI::Hud` | Turn indicator + walls |
| `include/ui/Menu.hpp` | Generic menu container | `UI::Menu` | Base for pause/winner menus |
| `include/ui/StateComponent.hpp` | Hover/clickable sprite base | `UI::StateComponent` | Used by buttons |
| `include/ui/Button.hpp` | Labeled button widget | `UI::Button` | Click callbacks |
| `include/ui/PauseMenu.hpp` | Pause menu | `UI::PauseMenu` | Resume/Restart/Quit |
| `include/ui/WinnerMenu.hpp` | Winner menu | `UI::WinnerMenu` | Banner + actions |
| `include/ui/SpriteEntity.hpp` | Sprite helper variants | `UI::SpriteEntity`, `TileSprite`, `WallSprite`, `PawnSprite` | Utility / alternative approach |
| `include/ui/InputHandler.hpp` | Input module placeholder | — | Stub |
| `include/ui/Renderer.hpp` | Renderer module placeholder | — | Stub |

---

## Credits

- Developers:
  - `<DEVELOPER_1>`
  - `<DEVELOPER_2>`
- Music tool:
  - Suno (`<NOTES_ABOUT_TRACKS/USAGE>`)
- Image tool:
  - Gemini (`<NOTES_ABOUT_ASSETS/USAGE>`)

---

## License

This project is licensed under `<LICENSE_NAME>`. See `LICENSE` for details.

---

## Contributing

1. Fork the repository  
2. Create a feature branch: `git checkout -b feature/my-change`  
3. Commit your changes  
4. Push to your fork and open a Pull Request
