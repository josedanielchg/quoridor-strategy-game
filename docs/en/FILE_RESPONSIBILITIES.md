English | [Français](../fr/FILE_RESPONSIBILITIES.md)

# File Responsibilities

## Table of Contents
- [Purpose](#purpose)
- [Responsibility table](#responsibility-table)
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
Provide a file-level responsibility map for key headers.

[Back to README](../../README.md)

## Responsibility table

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

