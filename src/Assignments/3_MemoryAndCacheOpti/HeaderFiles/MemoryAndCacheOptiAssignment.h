
#include <SFML/Graphics.hpp>

#include "../../0_Extras/HeaderFiles/Assignment.h"

#include "imgui.h"
#include "imgui-SFML.h"
#include <iostream>

#include "ParticleSystem.h"
#include "FPSCounterThree.h"
#include "MemoryTracker.h"
//#include "ThreadPool.h"

class MemoryAndCacheOptiAssignment : public Assignment
{
public:
    void Start() override;
    void Stop() override;
    void Update() override;
    void Render(sf::RenderWindow& window) override;

    // Important variables
    sf::Vector2f origin = { 480.0f, 270.0f };
    int startParticleCount = 100000;
    WayOfStorage thisWayOfStorage = WayOfStorage::AoS;
    bool thisMultiThreading = true;
    bool thisRandomSeed = false;
    bool thisFixedTime = true;
    float fixedTimeStep = 0.1f;

    ParticleSystem particleSystem;
    bool isFinished = false;
    int generation = 0;
    float deltaTime = 0.0f;
    float totalRuntime = 0.0f;
    float simSpeed = 0.0f;
    float simTime = 0.0f;
    float simFPS = 0.0f;
    FrameCounterThree<> fpsCounterThree;
    MemoryTracker memoryTracker;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastReportTime;
    size_t prevAllocated = 0;
    size_t prevAllocCount = 0;
    size_t prevDeallocCount = 0;



};