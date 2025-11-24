# Quoridor Strategy Game - Build Instructions

## 📋 Prerequisites

Before building the project, ensure you have the following installed on your system:

1.  **C++ Compiler** compatible with **C++20** (e.g., MSVC via Visual Studio 2022).
2.  **CMake** (Version 3.16 or higher).
3.  **Git** (Required to automatically download SFML).

> **Note on SFML:** You do **NOT** need to download or install SFML manually. This project uses CMake to automatically download and compile SFML 3.0 specifically for this project.

---

## 🚀 Quick Start (Command Line / PowerShell)

1.  **Open a terminal** in the project root folder (where `CMakeLists.txt` is located).

2.  **Configure the project**:
    This step downloads SFML and prepares the build files.
    ```powershell
    cmake -S . -B build
    ```
    *(Note: The first time you run this, it may take 1-2 minutes to download and compile SFML).*

3.  **Build the executable**:
    ```powershell
    cmake --build build
    ```

4.  **Run the game**:
    ```powershell
    .\build\Debug\QuoridorGame.exe
    ```
    
---

## 📂 Project Structure

The project follows a Clean Architecture separation:

*   **`include/`**: Header files (`.hpp`). Public interface definitions.
*   **`src/`**: Source files (`.cpp`). Implementation logic.
*   **`assets/`**: Game resources (fonts, textures, sounds).
*   **`build/`**: (Generated) Compilation artifacts.

---

## ❓ Troubleshooting

**1. "CMake Error: could not find git"**
*   Ensure Git is installed and added to your system PATH.

**2. "Assets not found" or crashing immediately**
*   The build script automatically copies the `assets/` folder to the build directory.
*   If you move the `.exe` manually, ensure the `assets/` folder is in the same directory as the executable.

**3. "SFML library not found"**
*   Delete the `build/` folder and run `cmake -S . -B build` again to force a clean re-download of dependencies.