#include "game.h"
#include "resource_manager.h"
#include <iostream>

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_MENU), Keys(), Width(width), Height(height) {} 

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
    ResourceManager::LoadTexture("../textures/block.png", false, "block");
    ResourceManager::LoadTexture("../textures/button.png", true, "button");

    // add menu buttons
    buttons.push_back(ClickableObject(glm::vec2(350, 150), glm::vec2(500, 100), 0));
    buttons.push_back(ClickableObject(glm::vec2(150, 450), glm::vec2(400, 100), 1));
    buttons.push_back(ClickableObject(glm::vec2(650, 450), glm::vec2(400, 100), 2));
    buttons.push_back(ClickableObject(glm::vec2(150, 750), glm::vec2(400, 100), 3));
    buttons.push_back(ClickableObject(glm::vec2(650, 750), glm::vec2(400, 100), 4));
}

void Game::Update(float dt) {}

void Game::ProcessInput(float dt) {}

void Game::Render() {
    if (this->State == GAME_ACTIVE) {
    } else if (this->State == GAME_MENU) {
        for (ClickableObject button : buttons) {
            Renderer->DrawSprite(ResourceManager::GetTexture("button"), button.Position, button.Size, 0.0f);
        }
        Text->RenderText("Start game", 600.0f, 200.0f, 0.5f, glm::vec3(1.0f));
        Text->RenderText("Choose White", 350.0f, 500.0f, 0.5f, glm::vec3(1.0f));
        Text->RenderText("Choose Black", 850.0f, 500.0f, 0.5f, glm::vec3(1.0f));
        Text->RenderText("Diff increase", 350.0f, 800.0f, 0.5f, glm::vec3(1.0f));
        Text->RenderText("Diff decrease", 850.0f, 800.0f, 0.5f, glm::vec3(1.0f));
    }
}

void Game::ResetGame() {
    this->State = GAME_MENU;
}
