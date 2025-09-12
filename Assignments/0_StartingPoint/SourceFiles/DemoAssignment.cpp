

#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream> // For Debugging.

#include "../HeaderFiles/DemoAssignment.h"

void DemoAssignment::Update() 
{
    // Run ImGui Demo.
    ImGui::ShowDemoWindow();
}

void DemoAssignment::Render(sf::RenderWindow& window)
{
    // ...
}
