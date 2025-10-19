
#include <SFML/Graphics.hpp>

#include "../../0_Extras/HeaderFiles/Assignment.h"

#include "ParticleSystem.h"
#include "FPSCounterThree.h"

class MemoryAndCacheOptiAssignment : public Assignment
{
public:
    void Start() override;
    void Stop() override;
    void Update() override;
    void Render(sf::RenderWindow& window) override;

    ParticleSystem particleSystem;
    sf::Vector2f origin = { 480.0f, 270.0f };
    int count = 10000;

    bool isFinished = false;
    int generation = 0;
    float deltaTime = 0.0f;
    float timeSinceLastLog = 0.0f;
    FrameCounterThree<> fpsCounterThree;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastReportTime;
};