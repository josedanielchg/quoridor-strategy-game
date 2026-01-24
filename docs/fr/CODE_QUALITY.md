Français | [English](../en/CODE_QUALITY.md)

# Qualite du code

## Table des matieres
- [Objectif](#objectif)
- [Conventions de nommage](#conventions-de-nommage)
- [Nommage des fichiers](#nommage-des-fichiers)
- [Includes et declarations avancees](#includes-et-declarations-avancees)
- [Separation des responsabilites](#separation-des-responsabilites)
- [Propriete et cycle de vie](#propriete-et-cycle-de-vie)
- [Gestion des erreurs](#gestion-des-erreurs)
- [Formatage et outils](#formatage-et-outils)
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
Expliquer les conventions et pratiques de qualite dans ce depot.

[Retour au README](../../README.md)

## Conventions de nommage
- Classes et structs: PascalCase (ex: `GameScreen`, `HeuristicEngine`).
- Methodes et fonctions: lowerCamelCase (ex: `handleEvent`, `setGameMode`).
- Variables membres: prefixe `m_` (ex: `m_gameState`, `m_window`).
- Constantes: ALL_CAPS pour enums/constantes (ex: `MAX_WALLS_PER_PLAYER`).
- Namespaces: courts et par module (`App`, `Game`, `UI`, `Audio`).

## Nommage des fichiers
- Headers: `.hpp` et sources: `.cpp`.
- Les fichiers correspondent au nom de la classe principale.
- Layout par module: `include/app`, `include/game`, `include/ui`, `include/heuristic`, `include/audio`.

## Includes et declarations avancees
- Les headers incluent ce qu'ils utilisent.
- Les forward declarations sont utilisees quand possible pour reduire le couplage (ex: `Entity` -> `Board`).

## Separation des responsabilites
- App: cycle de vie, fenetre, routage d'ecrans (`include/app/`, `src/app/`).
- Game: regles, etat, entites (`include/game/`, `src/game/`).
- UI: rendu et widgets (`include/ui/`, `src/ui/`).
- Heuristic: recherche CPU et cache (`include/heuristic/`, `src/heuristic/`).
- Audio: effets sonores (`include/audio/`, `src/audio/`).

## Propriete et cycle de vie
- `App::Application` possede les ecrans via `std::unique_ptr`.
- `App::GameScreen` possede l'etat de jeu et les composants UI par valeur.
- `Game::Board` possede fields, pawns, walls.
- `Audio::SfxManager` est un singleton qui possede les buffers et un pool de sons.

## Gestion des erreurs
- La plupart des `init` ou chargements de ressources retournent `bool` et loggent vers `std::cerr`.
- Le projet n'utilise pas les exceptions pour le controle de flux.

## Formatage et outils
- Pas de `.clang-format` ni d'outil auto dans le depot.
- Style coherent: indentation 4 espaces, accolades sur nouvelle ligne, espacement clair.

