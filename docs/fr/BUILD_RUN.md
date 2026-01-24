Français | [English](../en/BUILD_RUN.md)

# Compilation et execution

## Table des matieres
- [Objectif](#objectif)
- [Prerequis](#prerequis)
- [Cloner](#cloner)
- [Configurer et compiler](#configurer-et-compiler)
- [Executer](#executer)
- [Assets](#assets)
- [Depannage](#depannage)
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
Fournir les instructions de build et run pour Windows et Linux.

[Retour au README](../../README.md)

## Prerequis
- Compilateur C++20
  - Windows: MSVC (Visual Studio 2022 recommande)
  - Linux: g++ ou clang++
- CMake >= <CMAKE_MIN_VERSION>
- Git
- SFML <SFML_VERSION>
  - Ce depot telecharge et construit SFML via CMake FetchContent.

## Cloner

```bash
git clone <LINK>
cd QuoridorGame
```

## Configurer et compiler

Windows (MSVC / Visual Studio):

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

Compilation debug:

```powershell
cmake --build build --config Debug
```

Linux (g++ / clang):

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

Compilation debug:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
```

## Executer
- Windows (multi-config):
  - `build/Release/QuoridorGame.exe`
  - `build/Debug/QuoridorGame.exe`
- Linux (single-config):
  - `./build/QuoridorGame`

## Assets
- Windows: les assets sont copies a cote de l'executable apres le build.
- Linux: lancer depuis la racine du depot ou copier `assets/` a cote de l'executable.

## Depannage

**Assets introuvables / textures manquantes**
Les assets sont charges via des chemins relatifs (`assets/...`). Assurez-vous que le dossier
courant est la racine du depot, ou copiez `assets/` a cote de l'executable.

**SFML DLLs manquantes (Windows)**
Le projet est configure en SFML statique. Si vous passez en shared, copiez les DLL SFML
pres de `QuoridorGame.exe`.

**FetchContent ne telecharge pas SFML**
Verifiez que Git est installe et dans le PATH, puis supprimez `build/` et reconfigurez:

```bash
cmake -S . -B build
```
