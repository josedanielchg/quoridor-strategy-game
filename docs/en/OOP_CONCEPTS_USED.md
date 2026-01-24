# OOP Concepts Used (Audit)

This page audits the code under `include/` and `src/` and maps key OOP/C++ concepts to concrete evidence in the repository. Status is evidence-based; if no usage was found, it is marked Not used.

## Status Legend

| Status | Meaning |
|---|---|
| Used | Clear, direct usage with concrete evidence in code. |
| Partial | Indirect or limited usage; concept not central or incomplete. |
| Not used | No evidence found in `include/` or `src/`. |
| Unknown | Evidence could not be confirmed. |

## Master Checklist

| Concept | Status | Evidence | How it is used | Notes / Risks |
|---|---|---|---|---|
| Classes and objects | Used | `include/game/Board.hpp (class Game::Board)`, `include/ui/Button.hpp (class UI::Button)` | Gameplay, UI, and screens are modeled as classes instantiated at runtime. | - |
| Encapsulation and access control | Used | `include/game/Move.hpp (private data)`, `include/ui/Button.hpp (private members)` | State is hidden behind public methods and accessors. | - |
| Inheritance | Used | `include/app/Screen.hpp` -> `include/app/GameScreen.hpp`, `include/game/VisualEntity.hpp` -> `include/game/Pawn.hpp` | Derived classes extend base behavior and contracts. | - |
| Polymorphism and virtual override | Used | `include/app/Screen.hpp (virtual API)`, `src/app/Application.cpp (Screen* dispatch)` | Base pointers call derived implementations via virtual functions. | - |
| Composition / ownership | Used | `include/app/Application.hpp (unique_ptr screens)`, `include/app/GameScreen.hpp (Board, Hud, Menu members)` | Screens and game state are owned as members and composed objects. | - |
| Constructors and destructors | Used | `include/app/Screen.hpp (virtual ~Screen)`, `include/game/Entity.hpp (virtual ~Entity)` | Virtual destructors enable safe cleanup; constructors initialize state via init lists. | - |
| RAII / resource management | Used | `include/game/VisualEntity.hpp (sf::Texture, sf::Sprite)`, `src/app/Screen.cpp (sf::Music)` | Resources are owned by objects and released automatically. | - |
| Smart pointers (unique_ptr) | Used | `include/app/Application.hpp`, `include/ui/Menu.hpp`, `include/game/VisualEntity.hpp` | Exclusive ownership is expressed with `std::unique_ptr`. | - |
| Move semantics | Used | `include/game/VisualEntity.hpp (move ctor/assign)` | Move operations transfer texture/sprite ownership safely. | - |
| Templates (STL usage) | Partial | `include/game/Board.hpp (std::vector)`, `include/audio/SfxManager.hpp (std::array)` | Templates are used indirectly via STL containers. | No custom templates. |
| STL containers | Used | `include/game/Board.hpp (std::vector)`, `include/heuristic/TranspositionTable.hpp (std::vector, std::optional)`, `include/audio/SfxManager.hpp (std::array)` | Containers store game state, AI tables, and audio pools. | - |
| Iterators and range-based loops | Partial | `src/game/Board.cpp (range-based for)` | Iteration relies on container `begin()/end()` with range-based loops. | No custom iterators. |
| Operator overloading | Used | `include/game/VisualEntity.hpp (operator=(VisualEntity &&))`, `src/game/Wall.cpp (Wall::operator=(Wall &&) calls VisualEntity::operator=)` | Wall move assignment explicitly invokes the base `VisualEntity::operator=(std::move(other))` to move the texture/sprite safely. | Mostly assignment overloads; no arithmetic operators. |
 | Streams and file I/O | Used | `src/app/HowToPlayScreen.cpp (std::ifstream, std::istringstream)`, `src/app/Application.cpp (std::cout/cerr)` | Reads tutorial script and logs via standard streams. | - |
| Exceptions | Used | `src/game/Board.cpp (throw std::out_of_range)`, `src/app/Application.cpp (throw std::runtime_error in initScreen)`, `src/app/GameScreen.cpp (throw std::runtime_error in init steps)` | `Board::getField` throws for out-of-range; screen init throws `std::runtime_error` and catches `std::exception` to log failures. | Most resource loading uses return-bool + logging instead of exceptions. |
| Asynchronism (std::async/std::future) | Used | `include/app/GameScreen.hpp (std::future)`, `src/app/GameScreen.cpp (std::async, wait_for, get)` | CPU search runs off the main thread and is polled each frame. | - |
| Synchronization / atomics | Not used | No mutex/atomic usage found in `include/` or `src/`. | N/A. | - |
| C++20 concepts / constraints | Not used | No `concept` or `requires` usage found in `include/` or `src/`. | N/A. | - |
| Namespaces and modular design | Used | `include/app/*`, `include/game/*`, `include/ui/*`, `include/audio/*` | Code is organized by module namespaces and folders. | - |

## Summary

| Item | Value |
|---|---|
| Used | 15 |
| Partial | 3 |
| Not used | 2 |
| Unknown | 0 |
| Top 10 most important concepts used | Inheritance; Polymorphism and virtual override; Encapsulation and access control; Composition / ownership; RAII / resource management; Smart pointers (unique_ptr); Constructors and destructors; Asynchronism (std::async/std::future); STL containers; Namespaces and modular design |