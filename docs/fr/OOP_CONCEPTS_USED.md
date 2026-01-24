# Concepts POO utilises (audit)

Cette page audite le code dans `include/` et `src/` et mappe des concepts POO/C++ clefs vers des preuves concretes dans le depot. Le statut est base sur des preuves; si aucun usage n'a ete trouve, il est marque Non utilise.

## Legende des statuts

| Statut | Signification |
|---|---|
| Utilise | Usage clair et direct avec preuve concrete dans le code. |
| Partiel | Usage indirect ou limite; concept non central ou incomplet. |
| Non utilise | Aucune preuve trouvee dans `include/` ou `src/`. |
| Inconnu | Les preuves n'ont pas pu etre confirmees. |

## Liste principale

| Concept | Statut | Preuves | Comment c'est utilise | Notes / Risques |
|---|---|---|---|---|
| Classes et objets | Utilise | `include/game/Board.hpp (class Game::Board)`, `include/ui/Button.hpp (class UI::Button)` | Le gameplay, l'UI et les ecrans sont modeles en classes instanciees a l'execution. | - |
| Encapsulation et controle d'acces | Utilise | `include/game/Move.hpp (private data)`, `include/ui/Button.hpp (private members)` | L'etat est cache derriere des methodes publiques et des accesseurs. | - |
| Heritage | Utilise | `include/app/Screen.hpp` -> `include/app/GameScreen.hpp`, `include/game/VisualEntity.hpp` -> `include/game/Pawn.hpp` | Les classes derivees etendent le comportement et les contrats de la base. | - |
| Polymorphisme et override virtuel | Utilise | `include/app/Screen.hpp (virtual API)`, `src/app/Application.cpp (Screen* dispatch)` | Les pointeurs de base appellent les implementations derivees via des fonctions virtuelles. | - |
| Composition / propriete | Utilise | `include/app/Application.hpp (unique_ptr screens)`, `include/app/GameScreen.hpp (Board, Hud, Menu members)` | Les ecrans et l'etat du jeu sont possedes comme membres et objets composes. | - |
| Constructeurs et destructeurs | Utilise | `include/app/Screen.hpp (virtual ~Screen)`, `include/game/Entity.hpp (virtual ~Entity)` | Les destructeurs virtuels permettent un nettoyage correct; les constructeurs initialisent l'etat via les init lists. | - |
| RAII / gestion des ressources | Utilise | `include/game/VisualEntity.hpp (sf::Texture, sf::Sprite)`, `src/app/Screen.cpp (sf::Music)` | Les ressources sont possedees par les objets et liberees automatiquement. | - |
| Smart pointers (unique_ptr) | Utilise | `include/app/Application.hpp`, `include/ui/Menu.hpp`, `include/game/VisualEntity.hpp` | La propriete exclusive est exprimee avec `std::unique_ptr`. | - |
| Semantique de deplacement | Utilise | `include/game/VisualEntity.hpp (move ctor/assign)` | Les operations de deplacement transferrent la possession des textures/sprites en securite. | - |
| Templates (usage STL) | Partiel | `include/game/Board.hpp (std::vector)`, `include/audio/SfxManager.hpp (std::array)` | Les templates sont utilises indirectement via les conteneurs STL. | Pas de templates personnalises. |
| Conteneurs STL | Utilise | `include/game/Board.hpp (std::vector)`, `include/heuristic/TranspositionTable.hpp (std::vector, std::optional)`, `include/audio/SfxManager.hpp (std::array)` | Les conteneurs stockent l'etat du jeu, les tables d'IA et les pools audio. | - |
| Iterateurs et boucles range-based | Partiel | `src/game/Board.cpp (range-based for)` | L'iteration s'appuie sur `begin()/end()` avec des boucles range-based. | Pas d'iterateurs personnalises. |
| Surcharge d'operateurs | Utilise | `include/game/VisualEntity.hpp (operator=(VisualEntity &&))`, `src/game/Wall.cpp (Wall::operator=(Wall &&) calls VisualEntity::operator=)` | L'assignation par deplacement de Wall appelle explicitement `VisualEntity::operator=(std::move(other))` pour deplacer la texture/sprite en securite. | Surtout des surcharges d'assignation; pas d'operateurs arithmetiques. |
| Flux et E/S fichiers | Utilise | `src/app/HowToPlayScreen.cpp (std::ifstream, std::istringstream)`, `src/app/Application.cpp (std::cout/cerr)` | Lit le script du tutoriel et journalise via les flux standards. | - |
| Exceptions | Utilise | `src/game/Board.cpp (throw std::out_of_range)`, `src/app/Application.cpp (throw std::runtime_error in initScreen)`, `src/app/GameScreen.cpp (throw std::runtime_error in init steps)` | `Board::getField` lance pour hors limites; l'init des ecrans lance `std::runtime_error` et capture `std::exception` pour journaliser les erreurs. | Pas de catch au niveau `main`; le chargement de ressources utilise surtout des retours bool + logs. |
| Asynchronisme (std::async/std::future) | Utilise | `include/app/GameScreen.hpp (std::future)`, `src/app/GameScreen.cpp (std::async, wait_for, get)` | La recherche CPU tourne hors du thread principal et est sondee a chaque frame. | - |
| Synchronisation / atomics | Non utilise | Aucun usage de mutex/atomics trouve dans `include/` ou `src/`. | N/A. | - |
| Concepts / contraintes C++20 | Non utilise | Aucun usage de `concept` ou `requires` trouve dans `include/` ou `src/`. | N/A. | - |
| Casting (statique/dynamique) | Partiel | `src/app/HowToPlayScreen.cpp (static_cast)`, `src/ui/Button.cpp (static_cast)` | Des `static_cast` sont utilises pour des conversions numeriques; pas de dynamic_cast. | - |
| Namespaces et conception modulaire | Utilise | `include/app/*`, `include/game/*`, `include/ui/*`, `include/audio/*` | Le code est organise par namespaces et dossiers de modules. | - |

## Resume

| Element | Valeur |
|---|---|
| Utilise | 15 |
| Partiel | 3 |
| Non utilise | 2 |
| Inconnu | 0 |
| Top 10 concepts les plus importants utilises | Heritage; Polymorphisme et override virtuel; Encapsulation et controle d'acces; Composition / propriete; RAII / gestion des ressources; Smart pointers (unique_ptr); Constructeurs et destructeurs; Asynchronisme (std::async/std::future); Conteneurs STL; Namespaces et conception modulaire |

## Lacunes et prochaines etapes

| Concept non utilise ou partiel | Pourquoi il n'est pas utilise (ou partiel) | Comment l'integrer sans changer la portee |
|---|---|---|
| Concepts / contraintes C++20 | Aucun usage de `concept`/`requires` dans la base de code. | Utiliser les concepts si de nouveaux utilitaires templates sont introduits. |
| Synchronisation / atomics | Aucune concurrence avec etat partage au dela de `std::future`. | Ajouter seulement si plusieurs threads ecrivent des donnees partagees. |
| Templates (usage STL) | Seulement un usage indirect via STL. | Introduire des templates uniquement si la reutilisation le justifie. |
| Iterateurs et boucles range-based | Pas d'iterateurs personnalises ni d'algorithmes bases sur iterateurs. | Considerer seulement si un conteneur personnalise est ajoute. |
| Casting (statique/dynamique) | Seulement des `static_cast` numeriques. | Eviter `dynamic_cast` sauf si des verifications de type a l'execution sont requises. |
