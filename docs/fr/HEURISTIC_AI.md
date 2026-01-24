Anglais | [Fran?is](../fr/HEURISTIC_AI.md)

# IA heuristique

## Table des matieres
- [Apercu](#overview)
- [Classes et fichiers cles](#key-classes-and-files)
- [Flux de recherche (pas a pas)](#search-flow-step-by-step)
- [Entrees d'evaluation](#evaluation-inputs)
- [Configuration et difficulte](#configuration-and-difficulty)
- [Integration CPU dans le gameplay](#cpu-integration-in-gameplay)
- Docs
  - [README](../../README.md)
  - [Compilation et execution](BUILD_RUN.md)
  - [Jouabilite et commandes](CONTROLS.md)
  - [Structure du projet](PROJECT_STRUCTURE.md)
  - [Responsabilites des fichiers](FILE_RESPONSIBILITIES.md)
  - [IA heuristique](HEURISTIC_AI.md)
  - [Concepts OOP utilises](OOP_CONCEPTS_USED.md)
  - [Qualite du code](CODE_QUALITY.md)

## Apercu
Le CPU utilise une recherche alpha-beta limitee en profondeur avec ordre des coups et une table de transposition.
Il evalue les positions avec une fonction heuristique basee sur la distance a l'objectif, le nombre de murs,
et la mobilite locale.

## Classes et fichiers cles
- `include/heuristic/HeuristicEngine.hpp`
- `src/heuristic/HeuristicEngine.cpp`
- `include/heuristic/TranspositionTable.hpp`
- `src/heuristic/TranspositionTable.cpp`
- `include/game/GameRules.hpp` et `src/game/GameRules.cpp` (generation des coups et aides a l'evaluation)
- `include/app/GameScreen.hpp` et `src/app/GameScreen.cpp` (integration du tour CPU)

## Flux de recherche (pas a pas)
1) `GameScreen` detecte un tour CPU et planifie une recherche en arriere-plan avec `std::async`.
2) `HeuristicEngine::findBestMove` prepare une copie de travail de `GameState` et fixe une limite
   basee sur `HeuristicSearchConfig::timeLimitMs`.
3) Il genere des coups ordonnes avec des heuristiques et le meilleur coup de la table de transposition.
4) Il lance une recherche alpha-beta depuis chaque coup racine, avec elagage et limite de profondeur.
5) Les positions terminales renvoient un score gagne/perdu eleve; les positions non terminales utilisent
   l'evaluation heuristique (distance, murs, mobilite).
6) La table de transposition stocke les resultats avec des drapeaux (exact/lower/upper) pour accelerer les noeuds suivants.
7) Le meilleur coup est retourne a `GameScreen`, qui l'applique a l'etat en direct.

## Entrees d'evaluation
- Distance a l'objectif pour les deux joueurs (plus court est mieux).
- Murs restants pour chaque joueur.
- Mobilite locale pres du pion (reduction des voisins).

## Configuration et difficulte
`HeuristicSearchConfig` controle:
- `maxDepth` (profondeur de recherche)
- `timeLimitMs` (budget temps)
- `difficulty` (enum `HeuristicDifficulty`)
- `weightDistance`, `weightWalls`, `weightWidth` (poids d'evaluation)

## Integration CPU dans le gameplay
- `GameScreen::runHeuristicTurn()` marque un tour CPU comme en attente.
- `GameScreen::updateHeuristicTurn()` lance `std::async` si besoin et verifie le futur
  sans bloquer la boucle de rendu.
- Quand le futur est pret, le coup est applique et le HUD est mis a jour.