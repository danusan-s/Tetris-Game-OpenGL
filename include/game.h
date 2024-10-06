#ifndef GAME_H
#define GAME_H

#include "clickable_object.h"
#include "sprite_renderer.h"
#include "text_renderer.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <vector>

// Represents the current state of the game
enum GameState { GAME_ACTIVE, GAME_MENU, GAME_WIN };

enum BlockColor { NONE, RED, GREEN, BLUE};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game {
   private:
    // Game-related State data
    SpriteRenderer* Renderer;
    TextRenderer* Text;
    float CELL_SIZE;

   public:
    // game state
    GameState State;
    bool Keys[1024];
    float clickX, clickY;
    unsigned int Width, Height;
    std::vector<ClickableObject> buttons;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void DoCollisions();
    // reset
    void ResetGame();
};

#endif
