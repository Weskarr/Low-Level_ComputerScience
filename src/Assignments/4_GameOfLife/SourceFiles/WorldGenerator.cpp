
#include "../HeaderFiles/WorldGenerator.h"

#include <random>

void WorldGenerator::FirstGeneration(WorldData& worldData)
{
    auto& buffer = worldData.GetBufferCellDatas();
    auto& current = worldData.GetCurrentCellDatas();

	//ResetCellDataVector(buffer);
	RandomizeAliveCells(buffer, worldData.GetWorldTotal(), worldData.GetStartAlivePercentage());

    std::swap(buffer, current);
}

void WorldGenerator::NextGeneration(WorldData& worldData)
{
    auto& buffer = worldData.GetBufferCellDatas();
    auto& current = worldData.GetCurrentCellDatas();

    const int w = worldData.GetWorldWidth();
    const int h = worldData.GetWorldHeight();

    //ResetCellDataVector(buffer);

    std::for_each
    (current.begin(), 
    current.end(), 
    [&](const CellData& cell)
    {
	    int index = &cell - &current[0];
        int x = index % w;
        int y = index / w;

		CellData& bufferCell = buffer[index];
        int aliveNeighborsCount = CountAliveNeighborsAt(worldData, x, y, w, h);
        bufferCell.SetAliveNeighborCount(aliveNeighborsCount);

        bool survivedRules = worldData.GetCellRules()->getNewState(cell.GetAliveStatus(), aliveNeighborsCount);
        bufferCell.SetAliveStatus(survivedRules);
    });

    /* // Not allowed to use nested for loops.
        for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; ++y)
        {
            const CellData& currentCell = worldData.GetCurrentCellAt(x, y);
			CellData& bufferCell = worldData.GetBufferCellAt(x, y);

			int aliveNeighborsCount = CountAliveNeighborsAt(worldData, x, y, w, h);
            bufferCell.SetAliveNeighborCount(aliveNeighborsCount);

			bool survivedRules = worldData.GetCellRules()->getNewState(currentCell.GetAliveStatus(), aliveNeighborsCount);
            bufferCell.SetAliveStatus(survivedRules);
        }
    }
    */


    std::swap(buffer, current);
	worldData.IncrementGenerationCount(1);
}

int WorldGenerator::CountAliveNeighborsAt(WorldData& worldData, const int x, const int y, const int w, const int h)
{
    int aliveNeighbors = 0;

    const std::vector<std::pair<int, int>>& offsets = worldData.GetCellOffsets()->getOffsets();

    for (const auto& offset : offsets)
    {
        int nx = Wrapper(x + offset.first, w);
        int ny = Wrapper(y + offset.second, h);
        aliveNeighbors += worldData.GetCurrentCellAt(nx, ny).GetAliveStatus();
    }

    return aliveNeighbors;
}

int WorldGenerator::Wrapper(int value, int max) 
{
    return (value + max) % max;
}

void WorldGenerator::ResetCellDataVector(std::vector<CellData>& cellDatasVector)
{
    for (auto& cell : cellDatasVector)
    {
        cell.SetAliveStatus(false);
    }
}

void WorldGenerator::RandomizeAliveCells(std::vector<CellData>& cellDatasVector, int worldTotal, int alivePercentage)
{
    int aliveCount = worldTotal * alivePercentage / 100;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, worldTotal - 1);

    for (int i = 0; i < aliveCount; i++)
    {
        int random = distrib(gen);
        cellDatasVector[random].SetAliveStatus(true);
    }

	std::cout << aliveCount << " cells set to alive." << std::endl;
}

void WorldGenerator::SyncDataVectors(std::vector<CellData>& cellDatasVectorNew, std::vector<CellData>& cellDatasVectorOld)
{
    cellDatasVectorOld = cellDatasVectorNew;
}

void WorldGenerator::PrintCellDataVector(const std::vector<CellData>& cellDatasVector, std::string name, int width, int height)
{
	int aliveCount = std::count_if(cellDatasVector.begin(), cellDatasVector.end(), [](const CellData& cell) { return cell.GetAliveStatus(); });
	int deadCount = width * height - aliveCount;

	std::cout << name << " [ Alive: " << aliveCount << "] [Dead: " << deadCount << "]" << std::endl;
}