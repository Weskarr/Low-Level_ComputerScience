

#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream> // For Debugging.

#include "../HeaderFiles/MemoryAndCacheOptiAssignment.h"

void MemoryAndCacheOptiAssignment::Start()
{
    lastReportTime = std::chrono::high_resolution_clock::now();
    particleSystem.spawnParticles(count, origin);
}

void MemoryAndCacheOptiAssignment::Stop() 
{
    stopping = true;
    stopped = true;
}

void MemoryAndCacheOptiAssignment::Update()
{
    // Is finished condition check.
    if (isFinished)
        return;

    // Increment cycles.
    generation++;

    // Start speed test.
    auto startTime = std::chrono::high_resolution_clock::now();

    // Everything being speed tested.
    particleSystem.update(deltaTime);

    // End speed test.
    auto endTime = std::chrono::high_resolution_clock::now();

    // Time tracking
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> frameDelta = now - lastReportTime;
    deltaTime = frameDelta.count();
    lastReportTime = now;

    // Add for reporting
    timeSinceLastLog += deltaTime;

    // Update fps counter.
    fpsCounterThree.next_frame();

    // Log it to console.
    int particleCount = particleSystem.getParticleCount();
    if (timeSinceLastLog >= 1.0f)
    {
        lastReportTime = now;
        std::chrono::duration<double, std::milli> duration = endTime - startTime;

        std::cout
            << std::endl;

        std::cout
            << "[GEN]: "
            << generation
            << std::endl;

        std::cout
            << "[SPEED]: "
            << std::fixed
            << std::setprecision(3)
            << duration.count()
            << "ms"
            << std::endl;

        std::cout
            << "[TIME]: "
            << deltaTime
            << "s"
            << std::endl;

        std::cout
            << "[FPS]: "
            << fpsCounterThree.fps()
            << "fps"
            << std::endl;

        std::cout
            << "[PARTICLES]: "
            << particleCount
            << std::endl;
    }

    // Is finished condition switch.
    if (particleCount <= 0)
        isFinished = true;
}

void MemoryAndCacheOptiAssignment::Render(sf::RenderWindow& window)
{
    particleSystem.render(window);
}
