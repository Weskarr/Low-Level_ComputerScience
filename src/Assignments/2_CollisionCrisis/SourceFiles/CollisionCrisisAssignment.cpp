
#include "../HeaderFiles/CollisionCrisisAssignment.h"

void CollisionCrisisAssignment::Start()
{
    lastReportTime = std::chrono::high_resolution_clock::now();
}

void CollisionCrisisAssignment::Stop() 
{
    stopping = true;
    stopped = true;
}

void CollisionCrisisAssignment::Update()
{
    // Increment cycles.
    generation++;

    // Start speed test.
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Everything being speed tested.
    ballGame.updateBalls(windowSize, deltaTime);
    
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
    fpsCounterTwo.next_frame();

    // Log it to console.
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
            << fpsCounterTwo.fps()
            << "fps"
            << std::endl;
    }
}

void CollisionCrisisAssignment::Render(sf::RenderWindow& window)
{
    ballGame.drawBalls(window);
}
