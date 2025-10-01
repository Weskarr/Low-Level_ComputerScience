#pragma once

#include "CellRules.h"
#include "CellData.h"
#include "CellOffsets.h"

#include <iostream>
#include <vector>
#include <algorithm>

class WorldData 
{
private:
    int worldWidth;
    int worldHeight;
    int cellSize;
    int startAlivePercentage;

    int generationCount = 0;

    std::vector<CellData> cellDatasCurrent;
    std::vector<CellData> cellDatasBuffer;

    CellRules* myCellRules;
    CellOffsets* myCellOffsets;

public:

	// Constructor:
    WorldData(int width, int height, int size, int alive, CellRules* cellRules, CellOffsets* cellOffsets) :
        worldWidth(width),
        worldHeight(height),
        cellSize(size),
        startAlivePercentage(alive),
        cellDatasCurrent(width* height),
        cellDatasBuffer(width* height),
        myCellRules(cellRules),
        myCellOffsets(cellOffsets)
    {};

    // Modifiers
	void IncrementGenerationCount(int amount) { generationCount += amount; };

    // Getters
	int GetGenerationCount() const { return generationCount; };
	int GetWorldWidth() const { return worldWidth; };
	int GetWorldHeight() const { return worldHeight; };
	int GetCellSize() const { return cellSize; };
	int GetStartAlivePercentage() const { return startAlivePercentage; };
	int GetWorldTotal() const { return worldWidth * worldHeight; };
	std::vector<CellData>& GetCurrentCellDatas() { return cellDatasCurrent; };
    std::vector<CellData>& GetBufferCellDatas() { return cellDatasBuffer; };
    int GetIndex(int x, int y) const { return y * worldWidth + x; }
    int GetPositionX(int index) const { return index % worldWidth; }
    int GetPositionY(int index) const { return index / worldWidth; }
    CellData& GetCurrentCellAt(int x, int y) { return cellDatasCurrent[GetIndex(x, y)]; }
    CellData& GetBufferCellAt(int x, int y) { return cellDatasBuffer[GetIndex(x, y)]; }
	CellRules* GetCellRules() const { return myCellRules; };
	CellOffsets* GetCellOffsets() const { return myCellOffsets; };
    
    // Setters
    std::vector<CellData>& SetCurrentCellDatas(std::vector<CellData>& newCellDatas) { cellDatasCurrent = newCellDatas; };
    std::vector<CellData>& SetBufferCellDatas(std::vector<CellData>& newCellDatas) { cellDatasBuffer = newCellDatas; };
};