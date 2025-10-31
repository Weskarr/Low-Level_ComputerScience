



#include <iostream> // For Debugging.

#include "../HeaderFiles/ChatServerAssignment.h"

ChatServerAssignment::~ChatServerAssignment()
{
}

void ChatServerAssignment::Start()
{
    // Allocate on heap
    server = std::make_unique<ChatServer>(5555); 

    // Runs until server->running == false.
    serverThread = std::thread([this]() 
    {
        server->run();
    });
}

void ChatServerAssignment::Stop() 
{
	stopping = true;

    // Signal the server to stop.
    if (server)
        server->running = false;     
}

void ChatServerAssignment::Update()
{
    if (stopping && server)
    {
        std::cout << "Stopping ChatServer Before... " << stopped << std::endl;

        if (serverThread.joinable())
            serverThread.join();

        server.reset();
        stopped = true;

        std::cout << "Stopping ChatServer After... " << stopped << std::endl;
    }
}

void ChatServerAssignment::Render(sf::RenderWindow& window)
{
    // ...
}