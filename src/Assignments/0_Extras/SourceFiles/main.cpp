#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>

#include <iostream> // For Debugging.
#include <chrono> // For Profilling-ish.

#include "../HeaderFiles/AssignmentHeaderPaths.h"

int main()
{
    // Unique pointer to the current assignment.
    std::unique_ptr<Assignment> currentAssignment = nullptr;

    // Console Initialization.
    std::cout << "Initializing: Main (0_StartingPoint)" << std::endl;

    // Create SFML window.
    sf::RenderWindow window;
    window.create(sf::VideoMode({ 960, 540 }), "My window");
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
        // Event polling.
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            // "close requested" event: we close the window.
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Update ImGui state.
        ImGui::SFML::Update(window, deltaClock.restart());

        // Main menu initialization.
        ImGui::Begin("Main Menu");

        // Assignment choosing or running?
        if (!currentAssignment) 
        {
            // Assignment is yet to be chosen, await decision.
            ImGui::Text("Choose an assignment to run:");

            // Add assignments buttons.
            if (ImGui::Button("0. Playground Assignment"))
            {
                // Set new current.
                // -> Is for small scale experiments!
                currentAssignment = std::make_unique<PlaygroundAssignment>();
                std::cout << "CurrentAssignment is TestAssignment" << std::endl;
                currentAssignment->Start();
            }
            if (ImGui::Button("0. Demo Assignment")) 
            {
                // Set new current.
                // -> Is for playing around with the ImGui demo!
                currentAssignment = std::make_unique<DemoAssignment>();
                std::cout << "CurrentAssignment is DemoAssignment" << std::endl;
                currentAssignment->Start();
            }
            if (ImGui::Button("0. Exploring Algo/Funcs Assignment"))
            {
                // Set new current.
                // -> Is for learning algorithms!
                currentAssignment = std::make_unique<ExploringAlgoFuncAssignment>();
                std::cout << "CurrentAssignment is ExploringAlgoFuncAssignment" << std::endl;
                currentAssignment->Start();
            }
            if (ImGui::Button("1. Chat Client Assignment"))
            {
                // Set new current.
                // -> Is for learning networking via a chat client.
                currentAssignment = std::make_unique<ChatClientAssignment>();
                std::cout << "CurrentAssignment is ChatClientAssignment" << std::endl;
                currentAssignment->Start();
            }
            if (ImGui::Button("1. Chat Server Assignment"))
            {
                // Set new current.
                // -> Is for learning networking via a server client.
                currentAssignment = std::make_unique<ChatServerAssignment>();
                std::cout << "CurrentAssignment is ChatServerAssignment" << std::endl;
                currentAssignment->Start();
            }
            if (ImGui::Button("2. Collision Crisis Assignment"))
            {
                // Set new current.
                // -> Is for learning physics optimizations via collisions.
                currentAssignment = std::make_unique<CollisionCrisisAssignment>();
                std::cout << "CurrentAssignment is CollisionCrisisAssignment" << std::endl;
                currentAssignment->Start();
            }
            if (ImGui::Button("3. Memory & Cache Optimization Assignment"))
            {
                // Set new current.
                // -> Is for learning general memory and cache optimizations.
                currentAssignment = std::make_unique<MemoryAndCacheOptiAssignment>();
                std::cout << "CurrentAssignment is MemoryAndCacheOptiAssignment" << std::endl;
                currentAssignment->Start();
            }
            if (ImGui::Button("4. Game Of Life Assignment"))
            {
                // Set new current.
                // -> Is for learning simulation programming and algorithms & collection classes.
                currentAssignment = std::make_unique<GameOfLifeAssignment>();
                std::cout << "CurrentAssignment is GameOfLifeAssignment" << std::endl;
                currentAssignment->Start();
            }
        }
        else 
        {
            // Assignment is running and updating.
            currentAssignment->Update();

            // Add a back button.
            if (ImGui::Button("Return")) 
            {
                // delete assignment, go back automatically.
                currentAssignment.reset(); 
                std::cout << "CurrentAssignment is Null" << std::endl;
            }
        }

        // End main menu or crazy error.
        ImGui::End();

        // Render everything, etc.
        window.clear();
        if (currentAssignment)
            currentAssignment->Render(window);
        ImGui::SFML::Render(window);
        window.display();
    }

    // Recommended for clean-up?
    ImGui::SFML::Shutdown();

    // Quit
    return 0;
}