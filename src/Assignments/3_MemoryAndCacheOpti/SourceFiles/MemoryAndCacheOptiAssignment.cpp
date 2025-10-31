
#include "../HeaderFiles/MemoryAndCacheOptiAssignment.h"

void MemoryAndCacheOptiAssignment::Start()
{
    memoryTracker.simAllocated = 0;

    particleSystem.Start
    (
        thisWayOfStorage,
        thisMultiThreading,
        thisRandomSeed
    );

    particleSystem.Spawn(count, origin);

    lastReportTime = std::chrono::high_resolution_clock::now();
}

void MemoryAndCacheOptiAssignment::Stop() 
{
    //particleSystem.Stop();

    stopping = true;
    stopped = true;
}

void MemoryAndCacheOptiAssignment::Update()
{
    // Is finished condition check.
    if (isFinished) 
    {
        Stop();
        return;
    }

    // Increment cycles.
    generation++;

    // Start speed test.
    auto startTime = std::chrono::high_resolution_clock::now();

    // Everything being speed tested.
    particleSystem.Update(deltaTime);

    // End speed test.
    auto endTime = std::chrono::high_resolution_clock::now();

    // Time tracking method.
    if (!thisFixedTime) 
    {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = now - lastReportTime;
        deltaTime = elapsed.count();
        lastReportTime = now;

    }
    else 
    {
        deltaTime = fixedTimeStep;
    }

    // Increment to total runtime.
    totalRuntime += deltaTime;

    // Update fps counter.
    fpsCounterThree.next_frame();

    // Log it to console.
    int particleCount = particleSystem.GetParticleCount();

    {
        // Extra spacing.
        std::cout << std::endl;

        // General statistics related.
        {
            float speed = std::chrono::duration<float, std::milli>(endTime - startTime).count();
            float fps = fpsCounterThree.fps();

            std::cout
                << "[GEN]:              "
                << generation
                << std::endl;

            std::cout
                << "[SPEED]:            "
                << std::fixed
                << std::setprecision(3)
                << speed
                << "ms"
                << std::endl;

            std::cout
                << "[TIME]:             "
                << totalRuntime
                << "s"
                << std::endl;

            std::cout
                << "[FPS]:              "
                << fps
                << "fps"
                << std::endl;

            std::cout
                << "[PARTICLES]:        "
                << particleCount
                << std::endl;

            std::cout
                << "[TIMING]:           "
                << ((thisFixedTime) ? "FIXED" : "DELTA")
                << std::endl;

            // Save for last summary.
            simSpeed += speed;
            simFPS += fps;
        }

        // Memory tracking related
        {
            // Currents:
            size_t currentAllocTotal = memoryTracker.totalAllocated;
            size_t currentAllocPeak = memoryTracker.peakAllocated;
            size_t currentAllocCount = memoryTracker.allocCount;
            size_t currentDeallocCount = memoryTracker.deallocCount;

            // Differences:
            long long differenceAllocated = static_cast<long long>(currentAllocTotal) - static_cast<long long>(prevAllocated);
            long long differenceAllocCount = static_cast<long long>(currentAllocCount) - static_cast<long long>(prevAllocCount);
            long long differenceDeallocCount = static_cast<long long>(currentDeallocCount) - static_cast<long long>(prevDeallocCount);

            std::cout
                << "[WAY OF STORAGE]:   "
                << ((thisWayOfStorage == WayOfStorage::SoA) ? "SoA" : "AoS")
                << std::endl;

            std::cout
                << "[MEMORY CURRENT]:   "
                << std::fixed
                << std::setprecision(2)
                << currentAllocTotal / 1024.0
                << " KB"
                << "    "
                << ((differenceAllocated >= 0) ? "+" : "")
                << differenceAllocated / 1024.0
                << " KB"
                << std::endl;

            std::cout
                << "[MEMORY PEAK]:      "
                << std::fixed
                << std::setprecision(2)
                << currentAllocPeak / 1024.0
                << " KB"
                << std::endl;

            std::cout
                << "[ALLOC COUNT]:      "
                << currentAllocCount
                << "    "
                << ((differenceAllocCount >= 0) ? "+" : "")
                << differenceAllocCount
                << std::endl;

            std::cout
                << "[DEALLOC COUNT]:    "
                << currentDeallocCount
                << "    "
                << ((differenceDeallocCount >= 0) ? "+" : "")
                << differenceDeallocCount
                << std::endl;

            // Save for next generation.
            prevAllocated = currentAllocTotal;
            prevAllocCount = currentAllocCount;
            prevDeallocCount = currentDeallocCount;

            // Save for last summary.
            memoryTracker.simAllocated += currentAllocTotal;
        }
        
        // Multi-threading related.
        {
            std::cout
                << "[THREADS USED]:     "
                << ((thisMultiThreading) ? std::to_string(particleSystem.GetThreadCount()) : "1")
                << std::endl;
        }
    }

    // Is finished condition switch & simulation summary.
    if (particleCount <= 0)
    {
        isFinished = true;

        // Extra-extra spacing.
        std::cout << std::endl;
        std::cout << std::endl;

        // General statistics related.
        {
            std::cout
                << "[GENERATIONS]:      "
                << generation
                << std::endl;

            std::cout
                << "[SIM SPEED]:        "
                << std::fixed
                << std::setprecision(3)
                << simSpeed
                << "ms"
                << std::endl;

            std::cout
                << "[SIM TIME]:         "
                << totalRuntime
                << "s"
                << std::endl;

            std::cout
                << "[AVERAGE FPS]:      "
                << (simFPS / generation)
                << "fps"
                << std::endl;

            std::cout
                << "[START PARTICLES]:  "
                << count
                << std::endl;

            std::cout
                << "[TIMING]:           "
                << ((thisFixedTime) ? "FIXED" : "DELTA")
                << std::endl;
        }

        // Memory tracking related
        {
            // Currents:
            size_t currentAllocTotal = memoryTracker.totalAllocated;
            size_t currentAllocPeak = memoryTracker.peakAllocated;
            size_t currentAllocCount = memoryTracker.allocCount;
            size_t currentDeallocCount = memoryTracker.deallocCount;
            size_t simulationAverage = memoryTracker.simAllocated / generation;

            std::cout
                << "[WAY OF STORAGE]:   "
                << ((thisWayOfStorage == WayOfStorage::SoA) ? "SoA" : "AoS")
                << std::endl;

            std::cout
                << "[MEMORY AVERAGE]:   "
                << std::fixed
                << std::setprecision(2)
                << simulationAverage / 1024.0
                << " KB"
                << std::endl;

            std::cout
                << "[MEMORY CURRENT]:   "
                << std::fixed
                << std::setprecision(2)
                << currentAllocTotal / 1024.0
                << " KB"
                << std::endl;

            std::cout
                << "[MEMORY PEAK]:      "
                << std::fixed
                << std::setprecision(2)
                << currentAllocPeak / 1024.0
                << " KB"
                << std::endl;

            std::cout
                << "[ALLOC COUNT]:      "
                << currentAllocCount
                << std::endl;

            std::cout
                << "[DEALLOC COUNT]:    "
                << currentDeallocCount
                << std::endl;
        }

        // Multi-threading related.
        {
            std::cout
                << "[THREADS USED]:     "
                << ((thisMultiThreading) ? std::to_string(particleSystem.GetThreadCount()) : "1")
                << std::endl;
        }

        // Extra-extra spacing.
        std::cout << std::endl;
        std::cout << std::endl;
    }
}

void MemoryAndCacheOptiAssignment::Render(sf::RenderWindow& window)
{
    particleSystem.Render(window);
}
