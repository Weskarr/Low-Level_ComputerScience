#pragma once

#include "CellRules.h"
#include "CellData.h"
#include "WorldData.h"
#include "WorldGenerator.h"

#include <vector>
#include <algorithm>

class WorldGenerator
{
public:
	void FirstGeneration(WorldData& worldData);
	void NextGeneration(WorldData& worldData);
	void ResetCellDataVector(std::vector<CellData>& cellDatas);
	void RandomizeAliveCells(std::vector<CellData>& cellDatas, int totalCells, int alivePercentage);
	void SyncDataVectors(std::vector<CellData>& cellDatasOne, std::vector<CellData>& cellDatasTwo);
	int CountAliveNeighborsAt(WorldData& worldData, const int x, const int y, const int w, const int h);
	int Wrapper(int value, int max);
	void PrintCellDataVector(const std::vector<CellData>& cellDatasVector, std::string name, int width, int height);
};