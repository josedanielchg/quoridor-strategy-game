Français | [English](../../README.md)

# QuoridorGame

Un jeu de strategie type Quoridor avec une interface isometrique SFML et un adversaire CPU heuristique.
Construit en C++ (C++20) avec SFML pour les graphismes/audio/fenetre.

## Table des matieres
- [Apercu](#apercu)
- [Fonctionnalites cles](#fonctionnalites-cles)
- [Installation (Demarrage rapide)](#installation-demarrage-rapide)
- [Demo](#demo)
- [Captures d'ecran](#captures-decran)
- [Apercu de l'architecture](#apercu-de-larchitecture)
- [Credits](#credits)
- [Licence](#licence)
- [Contribuer](#contribuer)
- [Notes additionnelles](#notes-additionnelles)
- Docs
  - [Compilation et execution](BUILD_RUN.md)
  - [Jouabilite et commandes](CONTROLS.md)
  - [Structure du projet](PROJECT_STRUCTURE.md)
  - [Responsabilites des fichiers](FILE_RESPONSIBILITIES.md)
  - [IA heuristique](HEURISTIC_AI.md)
  - [Concepts OOP utilises](OOP_CONCEPTS_USED.md)
  - [Qualite du code](CODE_QUALITY.md)
  - [Gestion de projet](PROJECT_MANAGEMENT.md)

## Apercu
Ce projet implemente un jeu Quoridor a 2 joueurs sur une grille 9x9. Les joueurs tentent
d'atteindre le bord oppose tout en placant des murs pour ralentir l'adversaire, sous la
contrainte qu'un chemin vers l'objectif doit toujours exister.

Le jeu utilise une interface basee sur des ecrans, un rendu isometrique, et un adversaire
CPU base sur une recherche heuristique.

## Fonctionnalites cles
- Regles Quoridor avec placement legal des murs
- Rendu isometrique avec survol souris et apercu de mur
- Jeu Humain vs CPU (CPU calcule de facon asynchrone)
- IA heuristique: recherche alpha-beta, ordre des coups, table de transposition
- Cache de distance (BFS) pour verifications et evaluation
- HUD affichant le tour et les murs restants
- Menus pause et victoire
- Build CMake avec SFML via FetchContent

## Installation (Demarrage rapide)

Windows (MSVC / Visual Studio):

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

Linux (g++ / clang):

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

Run:
- Windows: `build/Release/QuoridorGame.exe`
- Linux: `./build/QuoridorGame`

Pour les prerequis complets, le mode debug et le depannage, voir
[BUILD_RUN.md](BUILD_RUN.md).

## Demo
Essai rapide:
1) Build Release (voir Installation).
2) Lancer l'executable.
3) Depuis le menu, choisir Single Player ou Multiplayer pour lancer une partie.

## Captures d'ecran
Les captures sont dans `docs/media/` (remplacez ces exemples par de vraies captures si besoin):

![Gameplay](../media/gameplay.png)
![Menu](../media/menu.png)
![Wall placement](../media/wall_placement.png)
![Win screen](../media/win_screen.png)

## Apercu de l'architecture
- Le point d'entree cree `App::Application`, qui possede la fenetre SFML et tous les ecrans.
- Les ecrans heritent de `App::Screen` et gerent events, update, render.
- `GameScreen` orchestre le jeu avec `GameState`, `GameRules`, `Board`, les composants UI et l'IA.
- Le tour CPU est lance avec `std::async` pour garder un rendu fluide.

Pour plus de details, voir [PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md).

## Credits
- Developers:
  - `<DEVELOPER_1>`
  - `<DEVELOPER_2>`
- Music tool:
  - Suno (`<NOTES_ABOUT_TRACKS/USAGE>`)
- Image tool:
  - Gemini (`<NOTES_ABOUT_ASSETS/USAGE>`)

## Licence
Ce projet est sous licence `<LICENSE_NAME>`. Voir `LICENSE`.

## Contribuer
1. Fork du depot
2. Creer une branche: `git checkout -b feature/my-change`
3. Commit les changements
4. Push et ouvrir une Pull Request

## Notes additionnelles
Champs a remplacer avant publication:
- `<LINK>`: URL du depot
- `<CMAKE_MIN_VERSION>`: version minimale de CMake
- `<SFML_VERSION>`: version SFML utilisee
- `<LICENSE_NAME>`: identifiant de licence
- Credits: noms des developpeurs et notes sur les assets/outils
- Ajouter ou remplacer les medias dans `docs/media/`
