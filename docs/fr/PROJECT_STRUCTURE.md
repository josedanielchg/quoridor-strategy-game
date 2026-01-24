Anglais | [Fran㌢is](../fr/PROJECT_STRUCTURE.md)

# Structure du projet

## Table des matieres
- [Arborescence (haut niveau)](#high-level-tree)
- [Carte des modules](#module-map)
- [Flux d'execution (court)](#runtime-flow-short)
- Docs
  - [README](../../README.md)
  - [Compilation et execution](BUILD_RUN.md)
  - [Jouabilite et commandes](CONTROLS.md)
  - [Structure du projet](PROJECT_STRUCTURE.md)
  - [Responsabilites des fichiers](FILE_RESPONSIBILITIES.md)
  - [IA heuristique](HEURISTIC_AI.md)
  - [Concepts OOP utilises](OOP_CONCEPTS_USED.md)
  - [Qualite du code](CODE_QUALITY.md)

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
    fr/
    media/
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
  - Cycle de vie de l'application, routage des ecrans, et ecrans.
- Game: `include/game/`, `src/game/`
  - Regles de base, etat logique, et entites du plateau.
- UI: `include/ui/`, `src/ui/`
  - Rendu, HUD, menus, widgets d'entree, et utilitaires de vue.
- Heuristic: `include/heuristic/`, `src/heuristic/`
  - Prise de decision CPU (recherche + evaluation + table de transposition).
- Audio: `include/audio/`, `src/audio/`
  - Gestionnaire d'effets sonores et pool de sons.

## Flux d'execution (court)

1) `main.cpp` cree `App::Application` et entre dans `run()`.
2) `Application` initialise tous les ecrans et definit l'ecran actif.
3) La boucle principale tourne: events -> update -> render.
4) Les mises a jour du jeu incluent l'input humain, les tours IA, et les mises a jour UI.
5) Quand l'utilisateur quitte, la fenetre se ferme.

