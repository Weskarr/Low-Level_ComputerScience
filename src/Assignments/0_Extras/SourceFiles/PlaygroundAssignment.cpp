

#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream> // For Debugging.

#include "../HeaderFiles/PlaygroundAssignment.h"

void PlaygroundAssignment::Start()
{
    std::cout << "Is for small scale experiments!" << std::endl;
}

void PlaygroundAssignment::Stop() 
{
    stopping = true;
    stopped = true;
}

void PlaygroundAssignment::Update()
{
    // Start SpeedTest:
    //std::chrono::high_resolution_clock::time_point startTime;
    //auto endTime = std::chrono::high_resolution_clock::now();

    // Do something to test speed???

    // End SpeedTest:
    //auto duration = std::chrono::duration(endTime - startTime);
    //std::cout << std::setprecision(3) << duration.count() / 1000.0f << std::endl;


    // ...
}

void PlaygroundAssignment::Render(sf::RenderWindow& window)
{
    // ...
}
