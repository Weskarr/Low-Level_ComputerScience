#pragma once

#include "CellRules.h"
#include "CellData.h"

#include <vector>
#include <algorithm>

#define WORLD_WIDTH 96
#define WORLD_HEIGHT 96
#define CELL_SIZE 6

class World 
{
private:
    CellRules* rules;

    int generationCount = 0;
    int active = 0;
    bool cells[WORLD_WIDTH][WORLD_HEIGHT][2];

public:
    World(CellRules* rules) : rules(rules) {};

    void generateGeneration0();
    void setCellRules(CellRules* rules);

    void nextGeneration();
    int generation() const;

    bool hasCellAt(int x, int y) const;
    [[nodiscard]] int countNeighbours(int x, int y) const;

    void GodGlider();
};