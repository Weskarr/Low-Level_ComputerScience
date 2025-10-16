

#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream> // For Debugging.

#include "../HeaderFiles/ChatServerAssignment.h"

void ChatServerAssignment::Start()
{
	ChatServer server(5555);
	server.run();
}

void ChatServerAssignment::Update()
{
    // ...
}

void ChatServerAssignment::Render(sf::RenderWindow& window)
{
    // ...
}