Anglais | [Français](../fr/BUILD_RUN.md)

# Compilation et execution

## Table des matieres
- [Prerequis](#prerequisites)
- [Cloner](#clone)
- [Configurer et compiler](#configure-and-build)
- [Executer](#run)
- [Assets](#assets)
- [Depannage](#troubleshooting)
- Docs
  - [README](../../README.md)
  - [Compilation et execution](BUILD_RUN.md)
  - [Jouabilite et commandes](CONTROLS.md)
  - [Structure du projet](PROJECT_STRUCTURE.md)
  - [Responsabilites des fichiers](FILE_RESPONSIBILITIES.md)
  - [IA heuristique](HEURISTIC_AI.md)
  - [Concepts OOP utilises](OOP_CONCEPTS_USED.md)
  - [Qualite du code](CODE_QUALITY.md)

## Prerequis
- Compilateur C++20
  - Windows: MSVC (Visual Studio 2022 recommended)
  - Linux: g++ or clang++
- CMake >= 3.22
- Git
- SFML 3.0
  - Ce depot est configure pour recuperer et construire SFML automatiquement via CMake FetchContent.

### Dependances Linux
Installez les bibliotheques de developpement requises:
```bash
sudo apt install -y \
  build-essential git \
  libx11-dev libxrandr-dev libxcursor-dev libxi-dev \
  libudev-dev \
  libgl1-mesa-dev \
  libfreetype-dev \
  libvorbis-dev libflac-dev
```

## Cloner

```bash
git clone https://github.com/josedanielchg/quoridor-strategy-game.git
cd quoridor-strategy-game
```

## Configurer et compiler

Windows (MSVC / Visual Studio):

```powershell
cmake -S . -B build
cmake --build build
```

Linux (g++ / clang):

```bash
cmake -S . -B build
cmake --build build
```

Si votre version de CMake est plus recente que 3.5 et que vous voyez des avertissements de policy, reconfigurez avec:

```bash
cmake -S . -B build "-DCMAKE_POLICY_VERSION_MINIMUM=3.5"
```

## Executer
- Windows (multi-config):
  - `build/Debug/QuoridorGame.exe`
- Linux (single-config):
  - `./build/QuoridorGame`
