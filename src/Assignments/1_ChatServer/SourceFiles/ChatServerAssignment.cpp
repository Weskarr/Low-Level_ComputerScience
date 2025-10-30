



#include <iostream> // For Debugging.

#include "../HeaderFiles/ChatServerAssignment.h"

ChatServerAssignment::~ChatServerAssignment()
{
}

void ChatServerAssignment::Start()
{
    server = std::make_unique<ChatServer>(5555); // allocate on heap

    serverThread = std::thread([this]() {
        server->run(); // runs until server->running == false
    });
}

void ChatServerAssignment::Stop() 
{
	stopping = true;

    // signal the server to stop
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