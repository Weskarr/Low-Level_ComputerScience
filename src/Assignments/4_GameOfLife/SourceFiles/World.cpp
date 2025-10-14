
#include "../HeaderFiles/World.h"

#include <random>

void World::setCellRules(CellRules* rules) 
{
    this->rules = rules;
}

void World::nextGeneration() 
{
    int next = (active + 1) % 2;
    for (int x = 0; x < WORLD_WIDTH; ++x) 
    {
        for (int y = 0; y < WORLD_HEIGHT; ++y) 
        {
            cells[y][x][next] = rules->getNewState(cells[y][x][active], countNeighbours(x, y));
        }
    }

    active = next;
    generationCount++;
}

int World::generation() const 
{
    return generationCount;
}

bool World::hasCellAt(const int x, const int y) const 
{
    const int wrappedX = (x + WORLD_WIDTH) % WORLD_WIDTH;
    const int wrappedY = (y + WORLD_HEIGHT) % WORLD_HEIGHT;
    return cells[wrappedY][wrappedX][active];
}

int World::countNeighbours(const int x, const int y) const 
{
    int neighbours = hasCellAt(x - 1, y) +
                     hasCellAt(x - 1, y - 1) +
                     hasCellAt(x, y - 1) +
                     hasCellAt(x + 1, y - 1) +
                     hasCellAt(x + 1, y) +
                     hasCellAt(x + 1, y + 1) +
                     hasCellAt(x, y + 1) +
                     hasCellAt(x - 1, y + 1);

    return neighbours;
}

void World::generateGeneration0() 
{
    const int totalCells = WORLD_HEIGHT * WORLD_WIDTH;

    // Fill cells
    for (int i = 0; i < totalCells; ++i) 
    {
        int x = i % 30;
        int y = i / 30;
        bool alive = false;
        //cellDatas.emplace_back(x, y, alive);
    }


    std::vector<std::pair<int, int>> positions;
    for (int y = 0; y < WORLD_HEIGHT; y++) 
    {
        for (int x = 0; x < WORLD_WIDTH; x++) 
        {
            cells[y][x][active] = false;
            positions.emplace_back(x, y);
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(positions.begin(), positions.end(), gen);

    int aliveCount = totalCells * 50 / 100 ;  // 50% Alive

    for (int i = 0; i < aliveCount; i++) 
    {
        int x = positions[i].first;
        int y = positions[i].second;
        cells[y][x][active] = true;
    }
}

void World::GodGlider() 
{
    // Add a glider at top-left
    int startX = 1;
    int startY = 1;
    cells[startY + 0][startX + 1][active] = true;
    cells[startY + 1][startX + 2][active] = true;
    cells[startY + 2][startX + 0][active] = true;
    cells[startY + 2][startX + 1][active] = true;
    cells[startY + 2][startX + 2][active] = true;
}
