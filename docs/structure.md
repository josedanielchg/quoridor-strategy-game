```
quoridor-oop/
├─ CMakeLists.txt
├─ README.md
├─ .gitignore
├─ cmake/               # (optional) CMake helpers, toolchains
│   └─ FindSFML.cmake   # only if you don't use vcpkg/vs package
├─ include/             # all public headers go here
│   ├─ app/
│   │   └─ Application.hpp
│   ├─ game/
│   │   ├─ Board.hpp
│   │   ├─ GameState.hpp
│   │   ├─ Player.hpp
│   │   └─ Move.hpp
│   ├─ ai/
│   │   ├─ AiEngine.hpp
│   │   └─ Pathfinder.hpp
│   └─ ui/
│       ├─ Renderer.hpp
│       └─ InputHandler.hpp
├─ src/                 # all .cpp files, organized by module
│   ├─ main.cpp
│   ├─ app/
│   │   └─ Application.cpp
│   ├─ game/
│   │   ├─ Board.cpp
│   │   ├─ GameState.cpp
│   │   ├─ Player.cpp
│   │   └─ Move.cpp
│   ├─ ai/
│   │   ├─ AiEngine.cpp
│   │   └─ Pathfinder.cpp
│   └─ ui/
│       ├─ Renderer.cpp
│       └─ InputHandler.cpp
├─ assets/              # everything SFML loads at runtime
│   ├─ fonts/
│   ├─ textures/
│   └─ sounds/
└─ tests/               # unit tests (optional but recommended)
    ├─ CMakeLists.txt
    └─ test_main.cpp
```