Anglais | [Fran?is](../fr/FILE_RESPONSIBILITIES.md)

# Responsabilites des fichiers

## Table des matieres
- [Tableau des responsabilites](#responsibility-table)
- Docs
  - [README](../../README.md)
  - [Compilation et execution](BUILD_RUN.md)
  - [Jouabilite et commandes](CONTROLS.md)
  - [Structure du projet](PROJECT_STRUCTURE.md)
  - [Responsabilites des fichiers](FILE_RESPONSIBILITIES.md)
  - [IA heuristique](HEURISTIC_AI.md)
  - [Concepts OOP utilises](OOP_CONCEPTS_USED.md)
  - [Qualite du code](CODE_QUALITY.md)

## Tableau des responsabilites

| Fichier | Responsabilite | Classes/structs cles | Notes/Dependances |
|---|---|---|---|
| `include/app/Application.hpp` | Configuration de la fenetre, boucle principale, routage des ecrans | `App::Application` | Possede la fenetre SFML et change les ecrans |
| `include/app/Screen.hpp` | Interface de base des ecrans + musique partagee | `App::Screen` | Parent de tous les ecrans |
| `include/app/TitleScreen.hpp` | Visuels de l'ecran titre + demarrage | `App::TitleScreen` | Utilise sprites/textes SFML |
| `include/app/MenuScreen.hpp` | Menu principal + gestion d'input | `App::MenuScreen` | Selectionne jeu/credits/how-to-play |
| `include/app/GameScreen.hpp` | Orchestration du gameplay + tour CPU | `App::GameScreen` | Possede `GameState`, `Board`, `HeuristicEngine` |
| `include/app/CreditsScreen.hpp` | Vue credits | `App::CreditsScreen` | Retourne au menu |
| `include/app/HowToPlayScreen.hpp` | Ecran tutoriel | `App::HowToPlayScreen` | Dialogue pilote par script |
| `include/audio/SfxManager.hpp` | Gestionnaire SFX + pool de sons | `Audio::SfxManager` | Utilise `sf::SoundBuffer`/`sf::Sound` |
| `include/game/GameState.hpp` | Conteneur d'etat logique | `Game::GameState` | Source de verite pour regles/IA |
| `include/game/MoveTypes.hpp` | Enums de coups et murs | `Game::MoveType`, `Game::Orientation` | Utilise par jeu/UI/IA |
| `include/game/Move.hpp` | Objet valeur de coup | `Game::Move` | Produit par UI/IA |
| `include/game/GameRules.hpp` | Moteur de regles + cache de distance | fonctions libres, `Game::MoveUndoState` | Utilise par UI et IA |
| `include/game/WallRules.hpp` | Legalite locale des murs | `isWallPlacementLegalLocal(...)` | Utilise par regles + entite mur |
| `include/game/Board.hpp` | Modele visuel du plateau + entites | `Game::Board` | Reconstruit depuis `GameState` |
| `include/game/Field.hpp` | Connectivite des cases du plateau | `Game::Field`, `Game::Direction` | Aretes de graphe pour chemins |
| `include/game/Entity.hpp` | Interface de base des entites | `Game::Entity` | Position + contrat de validation |
| `include/game/VisualEntity.hpp` | Entite + sprite/texture SFML | `Game::VisualEntity` | Base pour Pawn/Wall |
| `include/game/Pawn.hpp` | Entite pion | `Game::Pawn` | Delegue la legalite des coups aux regles |
| `include/game/Wall.hpp` | Entite mur + sprites d'apercu | `Game::Wall` | Valide localement via `WallRules` |
| `include/heuristic/HeuristicEngine.hpp` | Moteur de recherche CPU | `Game::HeuristicEngine`, `Game::HeuristicSearchConfig` | Alpha-beta + heuristiques |
| `include/heuristic/TranspositionTable.hpp` | TT + hachage Zobrist | `Game::TranspositionTable`, `Game::TTEntry`, `computeZobrist(...)` | Accelere la recherche |
| `include/ui/UiConstants.hpp` | Constantes de taille/scale UI | constantes | Partagees par renderer/menus |
| `include/ui/ViewUtils.hpp` | Utilitaire de vue letterbox | `makeLetterboxView(...)` | Mise a l'echelle coherente |
| `include/ui/GameRenderer.hpp` | Rendu isometrique + grille souris | `UI::GameRenderer` | Dessine plateau/entites |
| `include/ui/Hud.hpp` | UI HUD | `UI::Hud` | Indicateur de tour + murs |
| `include/ui/Menu.hpp` | Conteneur de menu generique | `UI::Menu` | Base pour menus pause/gagnant |
| `include/ui/StateComponent.hpp` | Base de sprite survol/cliquable | `UI::StateComponent` | Utilise par les boutons |
| `include/ui/Button.hpp` | Widget bouton avec libelle | `UI::Button` | Callbacks de clic |
| `include/ui/PauseMenu.hpp` | Menu pause | `UI::PauseMenu` | Reprendre/Relancer/Quitter |
| `include/ui/WinnerMenu.hpp` | Menu gagnant | `UI::WinnerMenu` | Banniere + actions |
| `include/ui/InGameBottomBar.hpp` | Barre basse + commandes | `UI::InGameBottomBar` | Bouton menu en jeu + controles mur |
| `include/ui/SpriteEntity.hpp` | Variantes d'aide sprite | `UI::SpriteEntity`, `TileSprite`, `WallSprite`, `PawnSprite` | Utilitaire / approche alternative |
| `include/ui/InputHandler.hpp` | Placeholder module d'entree | - | Pas present dans le depot actuel (placeholder legacy) |
| `include/ui/Renderer.hpp` | Placeholder module de rendu | - | Pas present dans le depot actuel (placeholder legacy) |