#include "game.h"
#include "resource_manager.h"
#include <iostream>

std::vector<std::vector<BlockColor>> grid(24, std::vector<BlockColor>(24, BlockColor::NONE));

Game::Game(unsigned int width, unsigned int height) : State(GAME_ACTIVE), Keys(), Width(width), Height(height) {}

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
    ResourceManager::LoadTexture("../textures/tetris_block.png", false, "block");
    ResourceManager::LoadTexture("../textures/solid_block.png", false, "solid");
    ResourceManager::LoadTexture("../textures/button.png", true, "button");

    // add menu buttons
    buttons.push_back(ClickableObject(glm::vec2(350, 150), glm::vec2(500, 100), 0));
}

void Game::Update(float dt) {}

void Game::ProcessInput(float dt) {}

glm::vec3 getColor(BlockColor color){
    switch (color){
        case RED:
            return glm::vec3(1.0f,0.0f,0.0f);
        case GREEN:
            return glm::vec3(0.0f,1.0f,0.0f);
        case BLUE:
            return glm::vec3(0.0f,0.0f,1.0f);
        default:
            return glm::vec3(1.0f);
    }
    return glm::vec3(1.0f);
}

void Game::Render() {
    if (this->State == GAME_ACTIVE) {
        // Draw play area with a frame
        Renderer->DrawSprite(ResourceManager::GetTexture("solid"), glm::vec2(1.8 * CELL_SIZE),
                             glm::vec2(10.4 * CELL_SIZE, 24.4 * CELL_SIZE), 0.0f, glm::vec3(1.0f));
        Renderer->DrawSprite(ResourceManager::GetTexture("solid"), glm::vec2(2 * CELL_SIZE),
                             glm::vec2(10 * CELL_SIZE, 24 * CELL_SIZE), 0.0f, glm::vec3(0.0f));

        // Draw actual game
        for (int i = 0; i < 24; ++i) {
            for (int j = 0; j < 10; ++j) {
                Renderer->DrawSprite(ResourceManager::GetTexture("block"),glm::vec2((i+2)*CELL_SIZE,(j+2)*CELL_SIZE),glm::vec2(CELL_SIZE),0.0f,getColor(grid[i][j]));
            }
        }

    } else if (this->State == GAME_MENU) {
        for (ClickableObject button : buttons) {
            Renderer->DrawSprite(ResourceManager::GetTexture("button"), button.Position, button.Size, 0.0f);
        }
        Text->RenderText("Start game", 600.0f, 200.0f, 0.5f, glm::vec3(1.0f));
    }
}

void Game::ResetGame() {
    this->State = GAME_MENU;
}
