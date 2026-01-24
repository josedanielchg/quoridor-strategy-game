Français | [English](../en/FILE_RESPONSIBILITIES.md)

# Responsabilites des fichiers

## Table des matieres
- [Objectif](#objectif)
- [Table des responsabilites](#table-des-responsabilites)
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
Donner la responsabilite principale de chaque header cle.

[Retour au README](../../README.md)

## Table des responsabilites

| Fichier | Responsabilite | Classes/structs | Notes/Dependances |
|---|---|---|---|
| `include/app/Application.hpp` | Configuration de la fenetre, boucle principale, routage des ecrans | `App::Application` | Possede la fenetre SFML et change d'ecran |
| `include/app/Screen.hpp` | Interface de base + musique partagee | `App::Screen` | Parent de tous les ecrans |
| `include/app/TitleScreen.hpp` | Ecran titre + declenchement start | `App::TitleScreen` | SFML sprites/texte |
| `include/app/MenuScreen.hpp` | Menu principal + input | `App::MenuScreen` | Choix game/credits/how-to-play |
| `include/app/GameScreen.hpp` | Orchestration gameplay + tour CPU | `App::GameScreen` | Possede `GameState`, `Board`, `HeuristicEngine` |
| `include/app/CreditsScreen.hpp` | Ecran credits | `App::CreditsScreen` | Retour menu |
| `include/app/HowToPlayScreen.hpp` | Ecran tutoriel | `App::HowToPlayScreen` | Dialogue script |
| `include/audio/SfxManager.hpp` | Manager SFX + pool sons | `Audio::SfxManager` | `sf::SoundBuffer`/`sf::Sound` |
| `include/game/GameState.hpp` | Etat logique | `Game::GameState` | Source de verite pour regles/IA |
| `include/game/MoveTypes.hpp` | Enums coup/mur | `Game::MoveType`, `Game::Orientation` | Utilise partout |
| `include/game/Move.hpp` | Objet Move | `Game::Move` | Produit par UI/IA |
| `include/game/GameRules.hpp` | Moteur de regles + cache distance | fonctions libres, `Game::MoveUndoState` | Utilise par UI et IA |
| `include/game/WallRules.hpp` | Legalite locale des murs | `isWallPlacementLegalLocal(...)` | Utilise par regles + mur |
| `include/game/Board.hpp` | Plateau visuel + entites | `Game::Board` | Reconstruit depuis `GameState` |
| `include/game/Field.hpp` | Connectivite d'une case | `Game::Field`, `Game::Direction` | Graphe des chemins |
| `include/game/Entity.hpp` | Interface base d'entite | `Game::Entity` | Position + validation |
| `include/game/VisualEntity.hpp` | Entite + sprite/texture | `Game::VisualEntity` | Base pour Pawn/Wall |
| `include/game/Pawn.hpp` | Pion | `Game::Pawn` | Validite deleguee aux regles |
| `include/game/Wall.hpp` | Mur + sprites preview | `Game::Wall` | Validation via `WallRules` |
| `include/heuristic/HeuristicEngine.hpp` | Moteur CPU | `Game::HeuristicEngine`, `Game::HeuristicSearchConfig` | Alpha-beta + heuristiques |
| `include/heuristic/TranspositionTable.hpp` | TT + Zobrist | `Game::TranspositionTable`, `Game::TTEntry`, `computeZobrist(...)` | Accelere la recherche |
| `include/heuristic/PathFinder.hpp` | A* helper | `Game::PathFinder` | Non present (placeholder) |
| `include/ui/UiConstants.hpp` | Constantes UI | constantes | Partage renderer/menus |
| `include/ui/ViewUtils.hpp` | Letterbox view | `makeLetterboxView(...)` | Echelle coherente |
| `include/ui/GameRenderer.hpp` | Rendu isometrique + mouse grid | `UI::GameRenderer` | Dessine plateau/entites |
| `include/ui/Hud.hpp` | HUD | `UI::Hud` | Tour + murs |
| `include/ui/Menu.hpp` | Conteneur menu | `UI::Menu` | Base pause/winner |
| `include/ui/StateComponent.hpp` | Base hover/click | `UI::StateComponent` | Utilise par boutons |
| `include/ui/Button.hpp` | Bouton avec label | `UI::Button` | Callbacks click |
| `include/ui/PauseMenu.hpp` | Menu pause | `UI::PauseMenu` | Resume/Restart/Quit |
| `include/ui/WinnerMenu.hpp` | Menu gagnant | `UI::WinnerMenu` | Banniere + actions |
| `include/ui/InGameBottomBar.hpp` | Barre bas + controles | `UI::InGameBottomBar` | Menu in-game + murs |
| `include/ui/SpriteEntity.hpp` | Helpers sprites | `UI::SpriteEntity`, `TileSprite`, `WallSprite`, `PawnSprite` | Utilitaire |
| `include/ui/InputHandler.hpp` | Placeholder input | - | Non present (placeholder) |
| `include/ui/Renderer.hpp` | Placeholder renderer | - | Non present (placeholder) |

