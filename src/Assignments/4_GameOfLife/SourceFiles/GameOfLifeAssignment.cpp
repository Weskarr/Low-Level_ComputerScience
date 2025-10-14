#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>

#include "../HeaderFiles/GameOfLifeAssignment.h"

using namespace std::literals::chrono_literals;


GameOfLifeAssignment::GameOfLifeAssignment()
{
};

void GameOfLifeAssignment::Start()
{
    int size = currentWorld.GetCellSize();
    cell.setSize(sf::Vector2f(float(size), float(size)));

    worldGenerator.FirstGeneration(currentWorld);
}


void GameOfLifeAssignment::Update()
{
    // Start SpeedTest:
    //auto startTime = std::chrono::high_resolution_clock::now();

	worldGenerator.NextGeneration(currentWorld);

    // End SpeedTest:
    //auto endTime = std::chrono::high_resolution_clock::now();
    //std::chrono::duration<double, std::milli> duration = endTime - startTime;
    //std::cout << std::fixed << std::setprecision(3)
    //    << duration.count() << " ms" << std::endl;


    fpsCounter.next_frame();
    if (std::chrono::system_clock::now() - lastReportTime > 1s) 
    {
       lastReportTime = std::chrono::system_clock::now();
       std::cout << "Generation: " << currentWorld.GetGenerationCount()
           << " | FPS: " << fpsCounter.fps() << std::endl;
    }
}

void GameOfLifeAssignment::Render(sf::RenderWindow& window)
{
	int width = currentWorld.GetWorldWidth();
	int height = currentWorld.GetWorldHeight();
	int size = currentWorld.GetCellSize();

    auto& cells = currentWorld.GetCurrentCellDatas();
    sf::VertexArray vertices(sf::PrimitiveType::Triangles, width * height * 6);

    /* Too slow.. Like -10 fps!
    std::for_each
    (currentWorld.GetCurrentCellDatas().begin(),
    currentWorld.GetCurrentCellDatas().end(),
    [&](CellData& cellData)
    {
        int index = &cellData - &currentWorld.GetCurrentCellDatas()[0]; // compute 1D index
        int x = index % width;
        int y = index / width;

        this->cell.setPosition(sf::Vector2f(float(x * size), float(y * size)));
        this->cell.setFillColor(cellData.GetAliveStatus(282828282828282828282828282828282828282828) ? sf::Color::Cyan : sf::Color::Blue);
        window.draw(this->cell);
    });
    */

	// Way Faster.. Like 60+ fps!
    std::for_each
    (cells.begin(), 
    cells.end(), 
    [&](CellData& cell)
    {
        int cellIndex = &cell - &cells[0];
        int x = cellIndex % width;
        int y = cellIndex / width;

        sf::Color color = currentWorld.GetCurrentCellAt(x, y).GetAliveStatus() ? sf::Color::Cyan : sf::Color::Blue;

        float left = float(x * size);
        float top = float(y * size);
        float right = float((x + 1) * size);
        float bottom = float((y + 1) * size);

        int vertexIndex = cellIndex * 6;

        // First triangle
        vertices[vertexIndex + 0].position = sf::Vector2f(left, top);
        vertices[vertexIndex + 1].position = sf::Vector2f(right, top);
        vertices[vertexIndex + 2].position = sf::Vector2f(right, bottom);

        // Second triangle
        vertices[vertexIndex + 3].position = sf::Vector2f(left, top);
        vertices[vertexIndex + 4].position = sf::Vector2f(right, bottom);
        vertices[vertexIndex + 5].position = sf::Vector2f(left, bottom);

        for (int i = 0; i < 6; ++i) vertices[vertexIndex + i].color = color;
    });

    window.draw(vertices);
}