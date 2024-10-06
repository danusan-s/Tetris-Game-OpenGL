#include "game.h"
#include "resource_manager.h"
#include <iostream>

std::vector<std::vector<BlockColor>> grid(24, std::vector<BlockColor>(10, BlockColor::NONE));
float fallTimeAccumulator = 0.0f;
float moveTimeAccumulator = 0.0f;

Game::Game(unsigned int width, unsigned int height)
    : State(GameState::GAME_ACTIVE), Keys(), Width(width), Height(height) {}

Game::~Game() {
    std::cout << "Attempting to delete Game Object" << std::endl;

    delete Renderer;
    Renderer = nullptr;
    std::cout << "Deleted Renderer\n";

    delete Text;
    Text = nullptr;
    std::cout << "Deleted Text\n";

    std::cout << "Game Object successfully deleted" << std::endl;
}

void Game::Init() {
    CELL_SIZE = Height / 28.0;
    
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(0)));

    // load shaders
    std::cout << "Loading Shaders" << std::endl;
    ResourceManager::LoadShader("../shaders/sprite_vertex.glsl", "../shaders/sprite_fragment.glsl", nullptr, "sprite");

    // configure shaders
    glm::mat4 projection =
        glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("../HoodBrothers.ttf", 96);

    // load textures
    std::cout << "Loading Textures" << std::endl;
    ResourceManager::LoadTexture("../textures/tetris_block.jpg", false, "block");
    ResourceManager::LoadTexture("../textures/solid_block.png", false, "solid");
    ResourceManager::LoadTexture("../textures/button.png", true, "button");

    SpawnTetromino();

    // add menu buttons
    buttons.push_back(ClickableObject(glm::vec2(350, 150), glm::vec2(500, 100), 0));
}

// Spawn a random Tetromino at the top of the grid
void Game::SpawnTetromino() {
    TetrominoType randomType = static_cast<TetrominoType>(rand() % 7);    // Random type between 0 and 6
    TetrominoColor randomColor = static_cast<TetrominoColor>(rand() % 3); // Random color R or G or B
    CurrentTetromino = new Tetromino(randomType, randomColor);            // Spawn new Tetromino
    if (CheckCollision(CurrentTetromino)) {
        // We cannot spawn piece
        this->State = GameState::GAME_WIN;
    }
}

void Game::Update(float dt) {
    if (this->State == GameState::GAME_ACTIVE) {
        fallTimeAccumulator += dt;
        if (fallTimeAccumulator >= 0.5f) {
            fallTimeAccumulator = 0.0f;
            // Game logic (moving, checking for collisions, etc.)
            CurrentTetromino->MoveDown(); // Move the Tetromino down if no collision
            if (CheckCollision(CurrentTetromino)) {
                // Fix the Tetromino to the grid and spawn a new one
                CurrentTetromino->MoveUp();
                FixTetrominoToGrid();
                ClearCompletedRows();
                SpawnTetromino();
            }
        }
    }
}

// Checks if moving the Tetromino would result in a collision
bool Game::CheckCollision(Tetromino* tetromino) {
    for (auto block : tetromino->GetCurrentShape()) {
        int gridX = static_cast<int>(tetromino->Position.x + block.x);
        int gridY = static_cast<int>(tetromino->Position.y + block.y);

        // Check if out of bounds or already occupied
        if (gridX < 0 || gridY < 0 || gridX >= 10 || gridY >= 24 || grid[gridY][gridX] != BlockColor::NONE) {
            return true;
        }
    }
    return false;
}

void Game::FixTetrominoToGrid() {
    // Fix the current Tetromino's blocks into the grid
    for (auto block : CurrentTetromino->GetCurrentShape()) {
        int gridX = static_cast<int>(CurrentTetromino->Position.x + block.x);
        int gridY = static_cast<int>(CurrentTetromino->Position.y + block.y);
        grid[gridY][gridX] = static_cast<BlockColor>(static_cast<int>(CurrentTetromino->Color) +
                                                     1); // Assign the Tetromino's color to the grid
    }
    delete CurrentTetromino; // Clean up the current Tetromino
    CurrentTetromino = nullptr;
}

void Game::ClearCompletedRows() {
    for (int i=23;i>=0;--i){
        bool completed = true;
        for (int j=0;j<10;++j){
            if (grid[i][j]==BlockColor::NONE){
                completed=false;
                break;
            }
        }
        if (completed){
            for (int row=i;row<23;++row){
                for (int col=0;col<10;++col){
                    grid[row][col] = grid[row+1][col];
                }
            }
        }
    }
}

void Game::ProcessInput(float dt) {
    moveTimeAccumulator += dt;
    if (moveTimeAccumulator>=0.1f){
        moveTimeAccumulator=0.0f;
    if (this->State == GameState::GAME_ACTIVE) {
        if (Keys['A']) {
            CurrentTetromino->MoveLeft();
            if (CheckCollision(CurrentTetromino)) {
                CurrentTetromino->MoveRight(); // Revert move if there's a collision
            }
        }
        if (Keys['D']) {
            CurrentTetromino->MoveRight();
            if (CheckCollision(CurrentTetromino)) {
                CurrentTetromino->MoveLeft(); // Revert move if there's a collision
            }
        }
        if (Keys['S']) {
            CurrentTetromino->MoveDown();
            if (CheckCollision(CurrentTetromino)) {
                CurrentTetromino->MoveUp(); // Revert move if there's a collision
            }
        }
        if (Keys['E']) {
            CurrentTetromino->RotateClockwise();
            if (CheckCollision(CurrentTetromino)) {
                // Revert rotation if collision happens
                CurrentTetromino->RotateAntiClockwise();
            }
        }
        if (Keys['Q']) {
            CurrentTetromino->RotateAntiClockwise();
            if (CheckCollision(CurrentTetromino)) {
                // Revert rotation if collision happens
                CurrentTetromino->RotateClockwise();
            }
        }
    }
    }
}

glm::vec3 getColor(BlockColor color) {
    switch (color) {
        case BlockColor::RED:
            return glm::vec3(1.0f, 0.0f, 0.0f);
        case BlockColor::GREEN:
            return glm::vec3(0.0f, 1.0f, 0.0f);
        case BlockColor::BLUE:
            return glm::vec3(0.0f, 0.0f, 1.0f);
        default:
            return glm::vec3(1.0f);
    }
    return glm::vec3(1.0f);
}

void Game::Render() {
    if (this->State == GameState::GAME_ACTIVE) {
        // Draw play area with a frame
        Renderer->DrawSprite(ResourceManager::GetTexture("solid"), glm::vec2(1.8 * CELL_SIZE),
                             glm::vec2(10.4 * CELL_SIZE, 24.4 * CELL_SIZE), 0.0f, glm::vec3(1.0f));
        Renderer->DrawSprite(ResourceManager::GetTexture("solid"), glm::vec2(2 * CELL_SIZE),
                             glm::vec2(10 * CELL_SIZE, 24 * CELL_SIZE), 0.0f, glm::vec3(0.0f));

        // Draw actual game
        for (int i = 0; i < 24; ++i) {
            for (int j = 0; j < 10; ++j) {
                if (grid[i][j] != BlockColor::NONE) {
                    Renderer->DrawSprite(ResourceManager::GetTexture("block"),
                                         glm::vec2((j + 2) * CELL_SIZE, ((23 - i) + 2) * CELL_SIZE),
                                         glm::vec2(CELL_SIZE), 0.0f, getColor(grid[i][j]));
                }
            }
        }

        BlockColor tetrominoColor = static_cast<BlockColor>(static_cast<int>(CurrentTetromino->Color) +
                                                            1); // Assign the Tetromino's color to the grid

        // Render current tetromino
        for (auto block : CurrentTetromino->GetCurrentShape()) {
            int gridX = static_cast<int>(CurrentTetromino->Position.x + block.x);
            int gridY = static_cast<int>(CurrentTetromino->Position.y + block.y);
            Renderer->DrawSprite(ResourceManager::GetTexture("block"),
                                 glm::vec2((gridX + 2) * CELL_SIZE, ((23 - gridY) + 2) * CELL_SIZE),
                                 glm::vec2(CELL_SIZE), 0.0f, getColor(tetrominoColor));
        }

    } else if (this->State == GameState::GAME_MENU) {
        for (ClickableObject button : buttons) {
            Renderer->DrawSprite(ResourceManager::GetTexture("button"), button.Position, button.Size, 0.0f);
        }
        Text->RenderText("Start game", 600.0f, 200.0f, 0.5f, glm::vec3(1.0f));
    }
}

void Game::ResetGame() {
    this->State = GameState::GAME_MENU;
}
