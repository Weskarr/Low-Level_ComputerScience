
#include <SFML/Graphics.hpp>
#include <chrono>

#include "../../0_Extras/HeaderFiles/Assignment.h"

#include "FPSCounter.h"

#include "CellData.h"
#include "WorldData.h"
#include "WorldGenerator.h"

#include "Rules/ConwayRules.h"
#include "Rules/HighLifeRules.h"
#include "Rules/PedestrianLife.h"
#include "Rules/DayNight.h"

#include "Offsets/MooreOffsets.h"
#include "Offsets/VonNeumannOffsets.h"
#include "Offsets/NinjaStarOffsets.h"
#include "Offsets/SpacedStarOffsets.h"
#include "Offsets/SpacedSquareOffsets.h"
#include "Offsets/LargeSquareOffsets.h"
#include "Offsets/RoundedLargeSquareOffsets.h"

class GameOfLifeAssignment : public Assignment
{
public:
    GameOfLifeAssignment();
    void Start() override;
    void Update() override;
    void Render(sf::RenderWindow& window) override;

private:
    WorldData currentWorld = WorldData(100, 100, 5, 75, new ConwayRules, new MooreOffsets);
	//WorldData currentWorld = WorldData(500, 500, 1, 60, new HighLifeRules, new SpacedStarOffsets);
    //WorldData currentWorld = WorldData(500, 500, 1, 80, new DayNight, new NinjaStarOffsets);
	WorldGenerator worldGenerator;

    FrameCounter<> fpsCounter;
    std::chrono::system_clock::time_point lastReportTime;
    sf::RectangleShape cell;
};