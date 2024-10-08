#include "game.h"
#include "resource_manager.h"
#include <iostream>

float fallTimeAccumulator = 0.0f;
float moveTimeAccumulator = 0.0f;
float flashTimer = 0.0f;

Game::Game(unsigned int width, unsigned int height)
    : State(GameState::GAME_MENU), Keys(), Width(width), Height(height), Score(0),
      grid(std::vector<std::vector<BlockColor>>(24, std::vector<BlockColor>(10, BlockColor::NONE))), rowCellCount(24),
      colCellCount(10), borderCellRatio(0.2f), paddingCellRatio(2.0f) {}

Game::~Game() {
    std::cout << "Attempting to delete Game Object" << std::endl;

    delete renderer;
    renderer = nullptr;
    std::cout << "Deleted renderer\n";

    delete text;
    text = nullptr;
    std::cout << "Deleted text\n";

    std::cout << "Game Object successfully deleted" << std::endl;
}

void Game::Init() {
    CELL_SIZE = static_cast<float>(Height) / rowCellCount+(2*paddingCellRatio);

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
    renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    text = new TextRenderer(this->Width, this->Height);
    text->Load("../fonts/JetBrainsMonoNerdFont-Bold.ttf", 72);

    // load textures
    std::cout << "Loading textures" << std::endl;
    ResourceManager::LoadTexture("../textures/tetris_block.jpg", false, "block");
    ResourceManager::LoadTexture("../textures/solid_block.png", false, "solid");

    SpawnTetromino();

    // add menu buttons
    startButton = new ClickableObject(glm::vec2(500, 500), glm::vec2(500, 100), 0);
}

// Spawn a random Tetromino at the top of the grid
void Game::SpawnTetromino() {
    if (nextTetromino == nullptr) {
        TetrominoType randomType = static_cast<TetrominoType>(rand() % 7); // Random type between 0 and 6
        nextTetromino = new Tetromino(randomType);                         // Spawn new Tetromino
    }
    currentTetromino = nextTetromino;
    TetrominoType randomType = static_cast<TetrominoType>(rand() % 7); // Random type between 0 and 6
    nextTetromino = new Tetromino(randomType);                         // Spawn new Tetromino
    if (CheckCollision(currentTetromino)) {
        // We cannot spawn piece
        this->State = GameState::GAME_OVER;
    }
}

void Game::Update(float dt) {
    if (this->State == GameState::GAME_ACTIVE) {
        fallTimeAccumulator += dt;
        if (fallTimeAccumulator >= 0.5f) {
            fallTimeAccumulator = 0.0f;
            // Game logic (moving, checking for collisions, etc.)
            currentTetromino->MoveDown(); // Move the Tetromino down if no collision
            if (CheckCollision(currentTetromino)) {
                // Fix the Tetromino to the grid and spawn a new one
                currentTetromino->MoveUp();
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
    for (auto block : currentTetromino->GetCurrentShape()) {
        int gridX = static_cast<int>(currentTetromino->Position.x + block.x);
        int gridY = static_cast<int>(currentTetromino->Position.y + block.y);
        grid[gridY][gridX] = static_cast<BlockColor>(static_cast<int>(currentTetromino->Type) +
                                                     1); // Assign the Tetromino's color to the grid
    }
    delete currentTetromino; // Clean up the current Tetromino
    currentTetromino = nullptr;
}

void Game::ClearCompletedRows() {
    int combo = 1;
    for (int i = 23; i >= 0; --i) {
        bool completed = true;
        for (int j = 0; j < 10; ++j) {
            if (grid[i][j] == BlockColor::NONE) {
                completed = false;
                break;
            }
        }
        if (completed) {
            flashTimer = 0.5f;
            Score += combo * 50;
            ++combo;
            for (int row = i; row < 23; ++row) {
                for (int col = 0; col < 10; ++col) {
                    grid[row][col] = grid[row + 1][col];
                }
            }
        }
    }
}

void Game::ProcessInput(float dt) {
    moveTimeAccumulator += dt;
    if (flashTimer > 0.0f) {
        flashTimer -= dt;
    }
    if (moveTimeAccumulator >= 0.1f) {
        moveTimeAccumulator = 0.0f;
        if (this->State == GameState::GAME_ACTIVE) {
            if (Keys['A']) {
                currentTetromino->MoveLeft();
                if (CheckCollision(currentTetromino)) {
                    currentTetromino->MoveRight(); // Revert move if there's a collision
                }
            }
            if (Keys['D']) {
                currentTetromino->MoveRight();
                if (CheckCollision(currentTetromino)) {
                    currentTetromino->MoveLeft(); // Revert move if there's a collision
                }
            }
            if (Keys['S']) {
                currentTetromino->MoveDown();
                if (CheckCollision(currentTetromino)) {
                    currentTetromino->MoveUp(); // Revert move if there's a collision
                }
            }
            if (Keys['E']) {
                currentTetromino->RotateClockwise();
                if (CheckCollision(currentTetromino)) {
                    // Revert rotation if collision happens
                    currentTetromino->RotateAntiClockwise();
                }
                Keys['E'] = false;
            }
            if (Keys['Q']) {
                currentTetromino->RotateAntiClockwise();
                if (CheckCollision(currentTetromino)) {
                    // Revert rotation if collision happens
                    currentTetromino->RotateClockwise();
                }
                Keys['Q'] = false;
            }
        }
    }
    if (this->State == GameState::GAME_MENU) {
        if (startButton->CheckClickPos(ClickX, ClickY)) {
            startButton->Clicked = true;
            startButton->ChangeScale(0.8f);
            ClickX = -1;
            ClickY = -1;
        }
        if (ReleaseX > 0 && ReleaseY > 0) {
            if (startButton->CheckClickPos(ReleaseX, ReleaseY) && startButton->Clicked) {
                this->State = GameState::GAME_ACTIVE;
            }
            startButton->ChangeScale(1.0f);
            startButton->Clicked = false;
            ReleaseX = -1;
            ReleaseY = -1;
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
        case BlockColor::CYAN:
            return glm::vec3(0.0f, 1.0f, 1.0f);
        case BlockColor::PURPLE:
            return glm::vec3(1.0f, 0.0f, 1.0f);
        case BlockColor::YELLOW:
            return glm::vec3(1.0f, 1.0f, 0.0f);
        case BlockColor::ORANGE:
            return glm::vec3(1.0f, 0.5f, 0.0f);
        default:
            return glm::vec3(1.0f);
    }
    return glm::vec3(1.0f);
}

void Game::Render() {
    if (this->State == GameState::GAME_ACTIVE || this->State == GameState::GAME_OVER) {
        // Draw play area with a frame
        renderer->DrawSprite(ResourceManager::GetTexture("solid"),
                             glm::vec2((paddingCellRatio - borderCellRatio) * CELL_SIZE),
                             glm::vec2((colCellCount + 2 * borderCellRatio) * CELL_SIZE,
                                       (rowCellCount + 2 * borderCellRatio) * CELL_SIZE),
                             0.0f, flashTimer > 0.0f ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(1.0f));

        renderer->DrawSprite(ResourceManager::GetTexture("solid"), glm::vec2(paddingCellRatio * CELL_SIZE),
                             glm::vec2(colCellCount * CELL_SIZE, rowCellCount * CELL_SIZE), 0.0f, glm::vec3(0.0f));

        // Draw actual game
        for (int i = 0; i < 24; ++i) {
            for (int j = 0; j < 10; ++j) {
                if (grid[i][j] != BlockColor::NONE) {
                    renderer->DrawSprite(ResourceManager::GetTexture("block"),
                                         glm::vec2((j + paddingCellRatio) * CELL_SIZE,
                                                   ((rowCellCount - 1 - i) + paddingCellRatio) * CELL_SIZE),
                                         glm::vec2(CELL_SIZE), 0.0f, getColor(grid[i][j]));
                }
            }
        }

        // Render current tetromino

        BlockColor currTetrominoColor = static_cast<BlockColor>(static_cast<int>(currentTetromino->Type) +
                                                                1); // Assign the Tetromino's color to the grid
        for (auto block : currentTetromino->GetCurrentShape()) {
            int gridX = static_cast<int>(currentTetromino->Position.x + block.x);
            int gridY = static_cast<int>(currentTetromino->Position.y + block.y);
            renderer->DrawSprite(ResourceManager::GetTexture("block"),
                                 glm::vec2((gridX + 2) * CELL_SIZE, ((23 - gridY) + 2) * CELL_SIZE),
                                 glm::vec2(CELL_SIZE), 0.0f, getColor(currTetrominoColor));
        }

        // Render Score
        text->RenderText("Score: " + std::to_string(Score), 0.75f*Width, 0.2f*Height, 1.0f);

        // Render next tetromino

        BlockColor nextTetrominoColor = static_cast<BlockColor>(static_cast<int>(nextTetromino->Type) +
                                                                1); // Assign the Tetromino's color to the grid
        renderer->DrawSprite(ResourceManager::GetTexture("solid"), glm::vec2(620.0f, 390.0f), glm::vec2(260.0f), 0.0f,
                             glm::vec3(1.0f));
        renderer->DrawSprite(ResourceManager::GetTexture("solid"), glm::vec2(630.0f, 400.0f), glm::vec2(240.0f), 0.0f,
                             glm::vec3(0.0f));

        for (auto block : nextTetromino->GetCurrentShape()) {
            int posX = static_cast<int>(670.0f + block.x * 40.0f);
            int posY = static_cast<int>(480.0f + block.y * 40.0f);
            // Center the 3 wide piece
            if (nextTetromino->Type != TetrominoType::I && nextTetromino->Type != TetrominoType::O) {
                posX += 20.0f;
            }
            renderer->DrawSprite(ResourceManager::GetTexture("block"), glm::vec2(posX, posY), glm::vec2(40.0f), 0.0f,
                                 getColor(nextTetrominoColor));
        }

        if (this->State == GameState::GAME_OVER) {
            text->RenderText("GAME OVER", 0.5f*Width, 0.5f*Height, 2.0f, glm::vec3(0.8f));
        }

    } else if (this->State == GameState::GAME_MENU) {
        renderer->DrawSprite(ResourceManager::GetTexture("solid"), startButton->Position, startButton->Size, 0.0f,
                             glm::vec3(1.0f));
        text->RenderText("TETRIS", 0.5f*Width, 0.2*Height, 2.0f);
        text->RenderText("Start game", 0.5f*Width, 0.5f*Width ,0.5f, glm::vec3(0.0f));
    }
}

void Game::ResetGame() {
    // Clear the Score and grid
    this->Score = 0;
    for (int i = 0; i < 24; ++i) {
        for (int j = 0; j < 10; ++j) {
            grid[i][j] = BlockColor::NONE;
        }
    }
    this->State = GameState::GAME_MENU;
}
