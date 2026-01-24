Français | [English](../en/FUNCTIONALITIES.md)

# Fonctionnalites

## Table des matieres
- [Apercu](#apercu)
- [Fonctionnalites cles](#fonctionnalites-cles)
- [Comment utiliser](#comment-utiliser)
- [Limitations / contraintes connues](#limitations--contraintes-connues)
- Docs
  - [Compilation et execution](../fr/BUILD_RUN.md)
  - [Jouabilite et commandes](../fr/CONTROLS.md)
  - [Structure du projet](../fr/PROJECT_STRUCTURE.md)
  - [Responsabilites des fichiers](../fr/FILE_RESPONSIBILITIES.md)
  - [IA heuristique](../fr/HEURISTIC_AI.md)
  - [Concepts OOP utilises](../fr/OOP_CONCEPTS_USED.md)
  - [Qualite du code](../fr/CODE_QUALITY.md)

## Apercu
QuoridorGame est une implementation C++20 du jeu de plateau Quoridor a 2 joueurs sur une grille 9x9, construit avec SFML pour les graphismes, l'audio et la fenetre.
Les joueurs tentent d'atteindre le cote oppose tout en placant des murs, avec la regle qu'au moins un chemin vers l'objectif doit toujours rester.
Le jeu utilise une interface basee sur des ecrans avec rendu isometrique et inclut un adversaire CPU heuristique.

## Fonctionnalites cles
- Regles et plateau: regles Quoridor sur une grille 9x9 avec respect du placement legal des murs et la contrainte de chemin vers l'objectif.
- Mode de jeu: jeu Humain vs CPU, avec le tour CPU calcule de facon asynchrone pour garder le rendu repondant.
- Rendu et UI: rendu isometrique avec survol souris et apercu de mur, plus un HUD affichant le tour courant et les murs restants.
- Menus: menus pause et gagnant.
- IA: IA heuristique avec recherche alpha-beta, ordre des coups, et table de transposition; cache de distance (BFS) pour les verifications de legalite et l'evaluation.
- Build: build CMake avec SFML recupere via FetchContent.

## Comment utiliser
Les interactions a la souris sont utilisees pour le survol/apercu du plateau, et les commandes clavier sont documentees separement.
Voir [Jouabilite et commandes](../fr/CONTROLS.md) pour le mapping complet.