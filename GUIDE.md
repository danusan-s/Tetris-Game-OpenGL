# Compiling the Tetris Game with CMake

## 1. Install Required Dependencies

Before compiling the game, ensure you have the following dependencies installed:
- **CMake** (version 3.10 or higher)
- **GLFW** (version 3.3 or higher)
- **OpenGL** (commonly available on most systems)
- **FreeType** (for text rendering)

### Installing Dependencies:

- **On Linux (Ubuntu/Debian)**:
  ```bash
  sudo apt-get install cmake libglfw3-dev libfreetype6-dev libglew-dev
  ```

- **On Windows**:
  Use a package manager like vcpkg or MSYS2 to install GLFW and FreeType, or download and build them manually.

  For example, using vcpkg:
    ```
    vcpkg install glfw3 freetype
    ```

- **On MacOS**:
  ```
  brew install cmake glfw freetype
  ```

## 2. Clone the repository:

Clone the repository to your device from github.
```
git clone https://github.com/yourusername/tetris-game.git
cd tetris-game
```

## 3. Configure the build with CMake:

```
mkdir build
cd build
cmake ..
```

## 4. Build the project:

Once Cmake is configured inside the build folder, the next step is to build it.

- **On Linux/MacOS:**
  ```
  make
  ```

- **On Windows:**
  ```
  cmake --build . --config Release
  ```

## 5. Run the game:

Once the build is successful, you can run the game.

- **On Linux/MacOS:**
  ```
  ./TetrisGame
  ```

- **On Windows:**
  ```
  ./Release/TetrisGame.exe
  ```

