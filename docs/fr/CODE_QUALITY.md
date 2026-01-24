Anglais | [Français](../fr/CODE_QUALITY.md)

# Qualite du code

## Table des matieres
- [Conventions de nommage](#naming-conventions)
- [Nommage des fichiers](#file-naming)
- [Includes et declarations avancees](#includes-and-forward-declarations)
- [Separation des responsabilites](#separation-of-concerns)
- [Propriete et cycle de vie](#ownership-and-lifetime)
- [Gestion des erreurs](#error-handling)
- Docs
  - [README](../../README.md)
  - [Compilation et execution](BUILD_RUN.md)
  - [Jouabilite et commandes](CONTROLS.md)
  - [Structure du projet](PROJECT_STRUCTURE.md)
  - [Responsabilites des fichiers](FILE_RESPONSIBILITIES.md)
  - [IA heuristique](HEURISTIC_AI.md)
  - [Concepts OOP utilises](OOP_CONCEPTS_USED.md)
  - [Qualite du code](CODE_QUALITY.md)

## Conventions de nommage
- Classes et structs: PascalCase (ex: `GameScreen`, `HeuristicEngine`).
- Methodes et fonctions: lowerCamelCase (ex: `handleEvent`, `setGameMode`).
- Variables membres: prefixe `m_` (ex: `m_gameState`, `m_window`).
- Constantes: ALL_CAPS pour enums ou valeurs constantes (ex: `MAX_WALLS_PER_PLAYER`).
- Namespaces: courts et par module (`App`, `Game`, `UI`, `Audio`).

## Nommage des fichiers
- Fichiers header: `.hpp` et fichiers source: `.cpp`.
- Les fichiers correspondent au nom de la classe principale (ex: `GameScreen.hpp/.cpp`).
- Le layout des dossiers correspond aux modules: `include/app`, `include/game`, `include/ui`, `include/heuristic`, `include/audio`.

## Includes et declarations avancees
- Les headers incluent ce qu'ils utilisent.
- Les forward declarations sont utilisees quand possible pour reduire le couplage (ex: `Entity` declare `Board`).

## Separation des responsabilites
- App: cycle de vie, fenetre, routage d'ecrans (`include/app/`, `src/app/`).
- Game: regles, etat, et entites (`include/game/`, `src/game/`).
- UI: rendu et widgets (`include/ui/`, `src/ui/`).
- Heuristic: recherche CPU et cache (`include/heuristic/`, `src/heuristic/`).
- Audio: gestion des effets sonores (`include/audio/`, `src/audio/`).

## Propriete et cycle de vie
- `App::Application` possede les ecrans via `std::unique_ptr` et change l'ecran actif.
- `App::GameScreen` possede l'etat de jeu et les composants UI par valeur.
- `Game::Board` possede fields, pawns, et walls.
- `Audio::SfxManager` est un singleton qui possede les buffers sonores et un pool de sons.

## Gestion des erreurs
- La plupart des fonctions `init` ou de chargement retournent `bool` et loggent vers `std::cerr`.
- Le projet n'utilise pas les exceptions pour le controle de flux.

