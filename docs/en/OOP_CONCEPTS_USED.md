# OOP Concepts Used

Purpose: Summarize the OOP and C++ concepts used in this project, with concrete references.

[Back to README](../../README.md)

| OOP / C++ Concept | Where it is used (Class/Module) | File(s) | Short explanation |
|---|---|---|---|
| Encapsulation | `Game::Board`, `UI::Hud` | `include/game/Board.hpp`, `src/game/Board.cpp`, `include/ui/Hud.hpp`, `src/ui/Hud.cpp` | State and behavior are grouped behind clear methods. |
| Inheritance | `App::Screen` -> screens, `UI::StateComponent` -> `UI::Button` | `include/app/Screen.hpp`, `include/app/TitleScreen.hpp`, `include/ui/StateComponent.hpp`, `include/ui/Button.hpp` | Base interfaces define common behavior for derived types. |
| Polymorphism | Screen lifecycle calls, Menu layout override | `include/app/Screen.hpp`, `src/app/Application.cpp`, `include/ui/Menu.hpp`, `include/ui/PauseMenu.hpp` | Virtual methods let the app call derived behavior through base pointers. |
| Composition | `App::Application` owns screens; `App::GameScreen` owns game/UI objects | `include/app/Application.hpp`, `include/app/GameScreen.hpp` | Larger objects are built from smaller, owned components. |
| RAII (resource ownership) | SFML resources stored as members | `src/ui/GameRenderer.cpp`, `src/ui/Hud.cpp`, `src/app/Screen.cpp` | Textures, fonts, and music are released automatically with object lifetime. |
| Smart pointers | `std::unique_ptr` screens, `VisualEntity` sprite | `include/app/Application.hpp`, `include/game/VisualEntity.hpp` | Clear ownership, no manual delete. |
| STL containers | `std::vector`, `std::array`, `std::optional` | `include/game/Board.hpp`, `include/heuristic/TranspositionTable.hpp` | Used for dynamic collections and fixed-size data. |
| Async and futures | CPU search in game loop | `include/app/GameScreen.hpp`, `src/app/GameScreen.cpp` | `std::async` and `std::future` allow non-blocking AI turns. |
| Templates / generic programming | STL usage | Multiple | The code relies on standard template containers rather than custom templates. |
| Exceptions (not used) | Error handling | Multiple | Functions return bool and log errors instead of throwing exceptions. |
