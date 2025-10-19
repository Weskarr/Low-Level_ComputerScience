
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <chrono>

#include <iostream> // For Debugging.

#include "../../0_Extras/HeaderFiles/Assignment.h"

#include "FPSCounterTwo.h"
#include "BallGame.h"

class CollisionCrisisAssignment : public Assignment
{
public:
    void Start() override;
    void Stop() override;
    void Update() override;
    void Render(sf::RenderWindow& window) override;

private:
    BallGame ballGame;
    const sf::Vector2u windowSize = { 960, 540 };

    int generation = 0;
    float deltaTime = 0.0f;
    float timeSinceLastLog = 0.0f;
    FrameCounterTwo<> fpsCounterTwo;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastReportTime;
};