

#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream> // For Debugging.

#include "../HeaderFiles/ChatClientAssignment.h"

void ChatClientAssignment::Start()
{
	std::cout << "Start() called";
}

void ChatClientAssignment::Update()
{
}

void ChatClientAssignment::Render(sf::RenderWindow& window)
{
	client.render_gui();
}


