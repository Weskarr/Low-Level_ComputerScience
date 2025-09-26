

#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream> // For Debugging.
#include <algorithm>

#include "../HeaderFiles/GameOfLifeAssignment.h"

using namespace std::literals::chrono_literals;


GameOfLifeAssignment::GameOfLifeAssignment()
{
};

void GameOfLifeAssignment::Start()
{
    cell.setSize(sf::Vector2f(float(CELL_SIZE), float(CELL_SIZE)));
    auto lastReportTime = std::chrono::system_clock::now();

    //currentWorld = World(new ConwayRules);
    //currentWorld = world(new HighLifeRules);
    //currentWorld = world(new DayNight);
    //currentWorld = world(new PedestrianLife);

    currentWorld.generateGeneration0();
    //currentWorld.GodGlider();
}

void GameOfLifeAssignment::Update()
{
    fpsCounter.next_frame();
    currentWorld.nextGeneration();

    if (std::chrono::system_clock::now() - lastReportTime > 1s) 
    {
       lastReportTime = std::chrono::system_clock::now();
       std::cout << "Generation: " << currentWorld.generation()
           << " | FPS: " << fpsCounter.fps() << std::endl;
    }
}

void GameOfLifeAssignment::Render(sf::RenderWindow& window)
{
    for (int x = 0; x < WORLD_WIDTH; ++x) {
        for (int y = 0; y < WORLD_HEIGHT; ++y) {
            cell.setPosition(sf::Vector2f(float(x * CELL_SIZE), float(y * CELL_SIZE)));
            cell.setFillColor(currentWorld.hasCellAt(x, y) ? sf::Color::Cyan : sf::Color::Blue);
            window.draw(cell);
        }
    }
}