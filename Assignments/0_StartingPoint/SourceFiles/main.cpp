#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>

#include <iostream> // For Debugging.

int main()
{
    // Console Initialization.
    std::cout << "Initializing: Main (0_StartingPoint)" << std::endl;

    // Create SFML window.
    sf::RenderWindow window;
    window.create(sf::VideoMode({ 1280, 720 }), "My window");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    // Init check for ImGui.
    if (!ImGui::SFML::Init(window))
        return -1;

    // Clock for ImGui updates.
    sf::Clock deltaClock;

    // Main Loop.
    while (window.isOpen())
    {
        // Event Polling.
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            // "close requested" event: we close the window.
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Update ImGui state.
        ImGui::SFML::Update(window, deltaClock.restart());

        // Show ImGui Demo.
        //ImGui::ShowDemoWindow();

        // Render everything.
        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    return 0;
}