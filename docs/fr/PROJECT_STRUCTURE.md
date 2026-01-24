Français | [English](../en/PROJECT_STRUCTURE.md)

# Structure du projet

## Table des matieres
- [Objectif](#objectif)
- [Arborescence (haut niveau)](#arborescence-haut-niveau)
- [Carte des modules](#carte-des-modules)
- [Flux d'execution (court)](#flux-dexecution-court)
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
Donner une vue d'ensemble du layout et des modules.

[Retour au README](../../README.md)

## Arborescence (haut niveau)

```text
QuoridorGame/
  CMakeLists.txt
  README.md
  LICENSE
  assets/
    fonts/
    sound/
    textures/
    tutorial/
  cmake/
    FindSFML.cmake
  docs/
    en/
      PROJECT_STRUCTURE.md
      OOP_CONCEPTS_USED.md
      CODE_QUALITY.md
      HEURISTIC_AI.md
      PROJECT_MANAGEMENT.md
      BUILD_RUN.md
      CONTROLS.md
      FILE_RESPONSIBILITIES.md
    fr/
      PROJECT_STRUCTURE.md
      OOP_CONCEPTS_USED.md
      CODE_QUALITY.md
      HEURISTIC_AI.md
      PROJECT_MANAGEMENT.md
      BUILD_RUN.md
      CONTROLS.md
      FILE_RESPONSIBILITIES.md
    media/
      gameplay.png
      menu.png
      wall_placement.png
      win_screen.png
    instructions.md
    structure.md
    Initial Report/
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

## Carte des modules

- App: `include/app/`, `src/app/`
  - Cycle de vie, routage d'ecrans, et ecrans.
- Game: `include/game/`, `src/game/`
  - Regles, etat logique, entites du plateau.
- UI: `include/ui/`, `src/ui/`
  - Rendu, HUD, menus, widgets, et vues.
- Heuristic: `include/heuristic/`, `src/heuristic/`
  - Decisions CPU (recherche, evaluation, table de transposition).
- Audio: `include/audio/`, `src/audio/`
  - Gestionnaire d'effets sonores.

## Flux d'execution (court)

1) `main.cpp` cree `App::Application` et entre dans `run()`.
2) `Application` initialise tous les ecrans et choisit l'ecran actif.
3) La boucle principale tourne: events -> update -> render.
4) Le jeu gere l'input humain, les tours IA, et l'UI.
5) Quand l'utilisateur quitte, la fenetre se ferme.

