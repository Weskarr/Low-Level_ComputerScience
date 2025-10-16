

#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream> // For Debugging.

#include "../HeaderFiles/DemoAssignment.h"

void DemoAssignment::Start()
{
    std::cout << "SFML version: "
        << SFML_VERSION_MAJOR << "."
        << SFML_VERSION_MINOR << "."
        << SFML_VERSION_PATCH
        << std::endl;
}

void DemoAssignment::Stop() 
{
    stopping = true;
	stopped = true;
}

void DemoAssignment::Update() 
{
    // Run ImGui Demo.
    ImGui::ShowDemoWindow();
}

void DemoAssignment::Render(sf::RenderWindow& window)
{
    // ...
}
