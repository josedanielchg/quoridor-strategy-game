# Quoridor (C++/SFML) – Heuristic AI & Modern OOP

University project for the **IN204 – Object-Oriented Programming** course.  
Goal: build a **2D Quoridor board game** in **C++ + SFML** with a **clean OOP architecture** and a **configurable AI** based on search and heuristics.

> Authors: **MENESES Carlos ** & **CHACÓN José**

---

## 1. Game Overview

**Quoridor** is a 2-player strategy game played on a **9×9 grid**.

- Each player starts on opposite sides of the board.
- On your turn, you can either:
  - **Move your pawn** by one square (up / down / left / right, sometimes diagonals depending on rules), or  
  - **Place a wall** between two squares to slow down your opponent.
- **Objective:** be the first to reach the **opposite row**.
- **Key rule:** it is **forbidden** to completely block the opponent – there must always remain **at least one path** to the goal.

This project focuses on:

- A **playable 2D game** (Human vs Human, Human vs AI).
- A **simple but effective AI** based on graph search (Minimax + A* / BFS).
- A **modern OOP design** with clear separation of responsibilities.

---

## 2. Main Features

- 🎮 **Game modes**
  - Human vs Human (local)
  - Human vs AI (CPU)

- 🧠 **Heuristic AI**
  - **Minimax / Négamax** search with **α–β pruning**.
  - Bounded search depth (time/complexity control).
  - Evaluation based on:
    - Estimated distance to goal (shortest path),
    - Opponent’s distance,
    - Wall stock / wall placement impact.

- 🗺️ **Pathfinding**
  - **A\*** and/or **BFS** to compute shortest paths on the evolving board.
  - Recomputed after each move or wall placement.
  - Used both by the AI and for rule validation (check that at least one path remains).

- 🧩 **Modern OOP architecture (MVC-like)**
  - **Model:** game rules & board state.
  - **View:** 2D rendering with SFML (board, pawns, walls, HUD).
  - **Controller:** input handling (mouse/keyboard), game loop, state transitions.

- ⚙️ **Configurable CPU difficulty**
  - Levels: **Easy / Normal / Hard**.
  - Difficulty affects:
    - Minimax depth / branching control,
    - Types of moves considered (e.g. aggressive walls on higher levels),
    - **Light randomization** between moves with similar score to avoid fully deterministic behavior.

- ✅ **Code quality & tooling**
  - C++17/20, RAII, smart pointers where needed.
  - Encapsulated classes & clear interfaces.
  - **Unit tests** for core rules and pathfinding.
  - Basic **logging / replay** support via overloaded operators.

---

## 3. Architecture & Design

### 3.1 Core Model

Planned main classes (names may evolve):

- `Board`  
  - Stores cells, walls, pawn positions.
  - Validates wall placement (no overlap, inside bounds).
  - Provides a graph view for pathfinding.

- `Rules`  
  - Checks move legality.
  - Enforces the “at least one path” rule using A*/BFS.
  - Handles turn switching and victory detection.

- `State`  
  - Represents a snapshot of the game (for AI / undo / replay).
  - Contains board, players, remaining walls, current player, etc.

- `Game`  
  - High-level orchestration of a match.
  - Manages the main loop, game phases (menu, playing, end screen).

- `Player` / `HumanPlayer` / `AIPlayer`  
  - Common interface to “play a turn”.
  - `HumanPlayer` uses input events.
  - `AIPlayer` delegates decision making to the AI engine.

### 3.2 AI & Search

- `AI` / `AiEngine`
  - Implements **Minimax (or Négamax)** with α–β pruning.
  - Uses a configurable depth (per difficulty level).
  - Uses a **heuristic evaluator** based on:
    - Shortest path length (A* / BFS) for each player,
    - Difference between paths,
    - Wall stock & structure of the maze.

- `Pathfinder`
  - A* and/or BFS implementation on the board graph.
  - Reusable by both rules and AI.

### 3.3 Rendering & Input (SFML)

- Uses **SFML** components:
  - `sf::RenderWindow` for the main window,
  - `sf::Event` for input events,
  - `sf::RectangleShape`, `sf::Sprite`, `sf::Text` for board, walls, pawns and UI.
- Basic **MVC** separation:
  - Model: `Board`, `Rules`, `State`.
  - View: small rendering helpers (draw grid, pawns, walls, HUD).
  - Controller: main loop & event handling.

### 3.4 OOP / C++ Topics Covered (for the course)

The project is designed to showcase:

- **Encapsulation & classes:** `Board`, `Game`, `AI`, `State`, etc.
- **Composition & (where relevant) inheritance / polymorphism:**
  - Different player types implementing a common interface.
- **STL containers:** `std::vector`, `std::array`, `std::unordered_map`, etc.
- **Operator overloading:** e.g. `operator<<` for logging / replay.
- **Exceptions & error handling:** invalid moves, inconsistent states.
- **Unit tests:** small test suite for rules and pathfinding.
- Optionally: **concurrency** (e.g. async AI search) if time allows.

---

## 4. Building & Running

### 4.1 Dependencies

- **C++17/20** compatible compiler (g++/clang/MSVC).
- **CMake** ≥ 3.16 (or similar).
- **SFML** 2.5+ (graphics, window, system).

On Debian/Ubuntu, SFML can typically be installed via:

```bash
sudo apt-get install libsfml-dev
````

### 4.2 Build Instructions

```bash
git clone https://github.com/<your-user>/<your-repo>.git
cd <your-repo>

mkdir build
cd build
cmake ..
cmake --build .
```

This should produce an executable, for example:

```bash
./quoridor
```

(adjust the name if needed according to the final `CMakeLists.txt`).

---

## 5. How to Play

* Launch the executable.
* Choose game mode: **Human vs Human** or **Human vs AI**.
* Choose **CPU difficulty** if playing against the AI.
* On your turn, either:

  * Select your pawn and move it to a valid adjacent cell, or
  * Select and place a wall at a valid position.
* The game ends when one player reaches the opposite side of the board.

Controls & UI details will be documented once the first prototype is ready.

---

## 6. Project Status

* ✅ Project proposal & high-level design.
* 🚧 Implementation in progress:

  * Board & rules,
  * Pathfinding (A*/BFS),
  * First AI prototype.
* 📌 Next steps:

  * Refine AI heuristics,
  * Add menus / in-game UI,
  * Extend unit tests and logging.