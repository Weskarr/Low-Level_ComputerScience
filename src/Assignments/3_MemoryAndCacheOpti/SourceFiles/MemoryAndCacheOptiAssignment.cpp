
#include "../HeaderFiles/MemoryAndCacheOptiAssignment.h"

void MemoryAndCacheOptiAssignment::Start()
{
    lastReportTime = std::chrono::high_resolution_clock::now();
    particleSystem.spawnParticles(wayOfStorage, count, origin);

    if (multiThreading)
        particleSystem.InitializeThreadPool();
}

void MemoryAndCacheOptiAssignment::Stop() 
{
    stopping = true;
    stopped = true;

    if (multiThreading)
        particleSystem.ShutdownThreadPool();
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
    if (multiThreading)
        particleSystem.UpdateThreadPool(wayOfStorage, deltaTime);
    else
        particleSystem.UpdateOriginal(wayOfStorage, deltaTime);



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
    fpsCounterThree.next_frame();

    // Log it to console.
    int particleCount = particleSystem.getParticleCount(wayOfStorage);
    //if (timeSinceLastLog >= 1.0f)

        // Is finished condition switch.
    if (particleCount <= 0)
        isFinished = true;

    if (generation == 1 ||
        generation == 40 ||
        isFinished)
    {
        // Extra spacing.
        std::cout << std::endl;

        // General statistics related.
        {
            lastReportTime = now;
            std::chrono::duration<double, std::milli> duration = endTime - startTime;

            std::cout
                << "[GEN]:              "
                << generation
                << std::endl;

            std::cout
                << "[SPEED]:            "
                << std::fixed
                << std::setprecision(3)
                << duration.count()
                << "ms"
                << std::endl;

            std::cout
                << "[TIME]:             "
                << deltaTime
                << "s"
                << std::endl;

            std::cout
                << "[FPS]:              "
                << fpsCounterThree.fps()
                << "fps"
                << std::endl;

            std::cout
                << "[PARTICLES]:        "
                << particleCount
                << std::endl;
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
                << ((wayOfStorage == WayOfStorage::SoA) ? "SoA" : "AoS")
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
        }
        
        // Multi-threading related.
        {
            std::cout
                << "[THREADS USED]:     "
                << ((multiThreading) ? particleSystem.Pool.getThreadCount() : 1)
                << std::endl;
        }
    }
}

void MemoryAndCacheOptiAssignment::Render(sf::RenderWindow& window)
{
    particleSystem.render(wayOfStorage, window);
}
