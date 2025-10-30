
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
    //timeSinceLastLog += deltaTime;

    // Update fps counter.
    fpsCounterTwo.next_frame();

    // Log it to console.
    //if (timeSinceLastLog >= 1.0f)
    {
        lastReportTime = now;
        std::chrono::duration<float, std::milli> duration = endTime - startTime;

        int currentBallAmount = ballGame.balls.size();
        float currentSpeed = duration.count();
        float currentTime = deltaTime;
        int currentFPS = fpsCounterTwo.fps();
        int currentHashes = ballGame.hashGrid.size();

        totalBalls += currentBallAmount;
        totalSpeed += currentSpeed;
        totalTime += currentTime;
        totalFPS += currentFPS;
        totalHashes += currentHashes;

        std::cout
            << "[GEN]: "
            << generation
            << std::endl;

        std::cout
            << "[BALLS]: "
            << currentBallAmount
            << std::endl;

        std::cout
            << "[SPEED]: "
            << std::fixed
            << std::setprecision(3)
            << currentSpeed
            << "ms"
            << std::endl;

        std::cout
            << "[TIME]: "
            << currentTime
            << "s"
            << std::endl;

        std::cout
            << "[FPS]: "
            << currentFPS
            << "fps"
            << std::endl;

        // For spatial hashing below.

        std::cout
            << "[HASHES]: "
            << currentHashes
            << std::endl;

        std::cout
            << "[CELLSIZE]: "
            << ballGame.cellSize
            << std::endl;

        // Extra spacing.

        std::cout
            << std::endl;
    }

    if (generation == 1000) 
    {
        int averageBalls = totalBalls / generation;
        float averageSpeed = totalSpeed / generation;
        float averageTime = totalTime / generation;
        int averageFPS = totalFPS / generation;
        int averageHashes = totalHashes / generation;

        std::cout
            << "[AVERAGE OF GENERATIONS]: "
            << generation
            << std::endl;

        std::cout
            << "[BALLS AVERAGE]: "
            << averageBalls
            << std::endl;

        std::cout
            << "[SPEED AVERAGE]: "
            << std::fixed
            << std::setprecision(3)
            << averageSpeed
            << "ms"
            << std::endl;

        std::cout
            << "[TIME AVERAGE]: "
            << averageTime
            << "s"
            << std::endl;

        std::cout
            << "[FPS AVERAGE]: "
            << averageFPS
            << "fps"
            << std::endl;

        // For spatial hashing below:

        std::cout
            << "[HASHES AVERAGE]: "
            << averageHashes
            << std::endl;

        std::cout
            << "[CELLSIZE]: "
            << ballGame.cellSize
            << std::endl;

        // Extra spacing.

        std::cout
            << std::endl;

        Stop();
    }
}

void CollisionCrisisAssignment::Render(sf::RenderWindow& window)
{
    ballGame.drawBalls(window);
}
