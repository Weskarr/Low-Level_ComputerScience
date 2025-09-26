
#include <SFML/Graphics.hpp>
#include <chrono>

#include "../../0_Extras/HeaderFiles/Assignment.h"

#include "FPSCounter.h"
#include "World.h"

#include "Rules/ConwayRules.h"
#include "Rules/HighLifeRules.h"
#include "Rules/PedestrianLife.h"
#include "Rules/DayNight.h"

class GameOfLifeAssignment : public Assignment
{
public:
    GameOfLifeAssignment();
    void Start() override;
    void Update() override;
    void Render(sf::RenderWindow& window) override;
private:
    World currentWorld = World(new ConwayRules);
    FrameCounter<> fpsCounter;
    std::chrono::system_clock::time_point lastReportTime;
    sf::RectangleShape cell;

    std::vector<bool> cellCondition;
};