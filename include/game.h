#ifndef GAME_H
#define GAME_H

#include "clickable_object.h"
#include "sprite_renderer.h"
#include "tetromino.h"
#include "text_renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

// Represents the current state of the game
enum class GameState { GAME_ACTIVE, GAME_MENU, GAME_OVER };

enum class BlockColor { NONE, RED, GREEN, BLUE, CYAN, PURPLE, YELLOW, ORANGE };

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game {
   private:
    // Game-related State data
    SpriteRenderer* renderer;
    TextRenderer* text;
    Tetromino* currentTetromino;
    Tetromino* nextTetromino;
    ClickableObject* startButton;
    int rowCellCount, colCellCount;
    float borderCellRatio, paddingCellRatio;
    glm::vec3 flashColor;
    float CELL_SIZE;

   public:
    // game state
    GameState State;
    int Score;
    bool Keys[1024];
    float ClickX, ClickY;
    float ReleaseX, ReleaseY;
    unsigned int Width, Height;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void SpawnTetromino();
    void Update(float dt);
    void FixTetrominoToGrid();
    void ClearCompletedRows();
    bool CheckCollision(Tetromino* tetromino);
    void Render();
    // reset
    void ResetGame();
};

#endif
