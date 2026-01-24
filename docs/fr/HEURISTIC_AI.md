Français | [English](../en/HEURISTIC_AI.md)

# IA heuristique

## Table des matieres
- [Objectif](#objectif)
- [Vue d'ensemble](#vue-densemble)
- [Classes et fichiers cles](#classes-et-fichiers-cles)
- [Flux de recherche (etapes)](#flux-de-recherche-etapes)
- [Entrees d'evaluation](#entrees-devaluation)
- [Configuration et difficulte](#configuration-et-difficulte)
- [Integration du tour CPU](#integration-du-tour-cpu)
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
Expliquer comment le CPU fonctionne et comment il est integre au jeu.

[Retour au README](../../README.md)

## Vue d'ensemble
Le CPU utilise une recherche alpha-beta avec profondeur limitee, ordre des coups et table de transposition.
Il evalue les positions avec une heuristique basee sur la distance a l'objectif, le nombre de murs,
et la mobilite locale.

## Classes et fichiers cles
- `include/heuristic/HeuristicEngine.hpp`
- `src/heuristic/HeuristicEngine.cpp`
- `include/heuristic/TranspositionTable.hpp`
- `src/heuristic/TranspositionTable.cpp`
- `include/game/GameRules.hpp` et `src/game/GameRules.cpp` (generation et evaluation des coups)
- `include/app/GameScreen.hpp` et `src/app/GameScreen.cpp` (integration du tour CPU)

## Flux de recherche (etapes)
1) `GameScreen` detecte un tour CPU et lance une recherche avec `std::async`.
2) `HeuristicEngine::findBestMove` prepare une copie de `GameState` et fixe une deadline
   avec `HeuristicSearchConfig::timeLimitMs`.
3) Generation de coups ordonnes avec heuristiques et (si dispo) le meilleur coup TT.
4) Alpha-beta sur chaque coup racine, avec pruning et profondeur limitee.
5) Les positions terminales retournent un score de victoire/perte; sinon evaluation heuristique.
6) La table de transposition stocke les resultats avec flags (exact/lower/upper).
7) Le meilleur coup est renvoye a `GameScreen` qui l'applique.

## Entrees d'evaluation
- Distance a l'objectif pour les deux joueurs (plus court est meilleur).
- Murs restants pour chaque joueur.
- Mobilite locale autour du pion (reducing neighbors).

## Configuration et difficulte
`HeuristicSearchConfig` controle:
- `maxDepth` (profondeur)
- `timeLimitMs` (budget temps)
- `difficulty` (enum `HeuristicDifficulty`)
- `weightDistance`, `weightWalls`, `weightWidth` (poids d'evaluation)

## Integration du tour CPU
- `GameScreen::runHeuristicTurn()` marque un tour CPU en attente.
- `GameScreen::updateHeuristicTurn()` lance `std::async` et verifie le future sans bloquer.
- Quand le future est pret, le coup est applique et le HUD est mis a jour.

