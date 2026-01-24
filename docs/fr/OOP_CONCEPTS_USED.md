Français | [English](../en/OOP_CONCEPTS_USED.md)

# Concepts OOP utilises

## Table des matieres
- [Objectif](#objectif)
- [Tableau des concepts](#tableau-des-concepts)
- Docs
  - [README](../../README.md)
  - [Compilation et execution](BUILD_RUN.md)
  - [Jouabilite et commandes](CONTROLS.md)
  - [Structure du projet](PROJECT_STRUCTURE.md)
  - [Responsabilites des fichiers](FILE_RESPONSIBILITIES.md)
  - [IA heuristique](HEURISTIC_AI.md)
  - [Concepts OOP utilises](OOP_CONCEPTS_USED.md)
  - [Qualite du code](CODE_QUALITY.md)
  - [Gestion de projet](PROJECT_MANAGEMENT.md)

## Objectif
Resumer les concepts OOP et C++ utilises dans ce projet, avec des references concretes.

[Retour au README](../../README.md)

## Tableau des concepts

| Concept OOP / C++ | Ou c'est utilise (Classe/Module) | Fichier(s) | Explication courte |
|---|---|---|---|
| Encapsulation | `Game::Board`, `UI::Hud` | `include/game/Board.hpp`, `src/game/Board.cpp`, `include/ui/Hud.hpp`, `src/ui/Hud.cpp` | Etat et comportement regroupes avec des methodes claires. |
| Heritage | `App::Screen` -> ecrans, `UI::StateComponent` -> `UI::Button` | `include/app/Screen.hpp`, `include/app/TitleScreen.hpp`, `include/ui/StateComponent.hpp`, `include/ui/Button.hpp` | Les classes de base definissent un comportement commun. |
| Polymorphisme | Cycle de vie des ecrans, override de Menu | `include/app/Screen.hpp`, `src/app/Application.cpp`, `include/ui/Menu.hpp`, `include/ui/PauseMenu.hpp` | Les appels virtuels passent par les pointeurs de base. |
| Composition | `App::Application` possede les ecrans; `App::GameScreen` possede jeu/UI | `include/app/Application.hpp`, `include/app/GameScreen.hpp` | Les objets sont construits avec des composants possedes. |
| RAII (ressources) | Ressources SFML en membres | `src/ui/GameRenderer.cpp`, `src/ui/Hud.cpp`, `src/app/Screen.cpp` | Textures, fonts, et musique liberees avec la vie des objets. |
| Smart pointers | `std::unique_ptr` ecrans, sprite `VisualEntity` | `include/app/Application.hpp`, `include/game/VisualEntity.hpp` | Propriete claire, pas de delete manuel. |
| Conteneurs STL | `std::vector`, `std::array`, `std::optional` | `include/game/Board.hpp`, `include/heuristic/TranspositionTable.hpp` | Collections dynamiques et donnees fixes. |
| Async et futures | Recherche CPU | `include/app/GameScreen.hpp`, `src/app/GameScreen.cpp` | `std::async` et `std::future` evitent de bloquer le rendu. |
| Templates / generique | Utilisation STL | Multiple | Utilise les conteneurs standard. |
| Exceptions (non utilisees) | Gestion d'erreurs | Multiple | Les fonctions renvoient `bool` et loggent les erreurs. |

