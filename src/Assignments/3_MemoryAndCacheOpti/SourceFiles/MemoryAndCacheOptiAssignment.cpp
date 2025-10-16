

#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream> // For Debugging.

#include "../HeaderFiles/MemoryAndCacheOptiAssignment.h"

void MemoryAndCacheOptiAssignment::Start()
{
    // ...
}

void MemoryAndCacheOptiAssignment::Stop() 
{
    stopping = true;
    stopped = true;
}

void MemoryAndCacheOptiAssignment::Update()
{
    // Start SpeedTest:
    std::chrono::high_resolution_clock::time_point startTime;
    auto endTime = std::chrono::high_resolution_clock::now();

    // Do something to test speed???

    // End SpeedTest:
    auto duration = std::chrono::duration(endTime - startTime);
    std::cout << std::setprecision(3) << duration.count() / 1000.0f << std::endl;


    // ...
}

void MemoryAndCacheOptiAssignment::Render(sf::RenderWindow& window)
{
    // ...
}
