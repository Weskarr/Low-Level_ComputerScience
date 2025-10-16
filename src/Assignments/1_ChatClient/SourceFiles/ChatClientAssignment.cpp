

#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream> // For Debugging.

#include "../HeaderFiles/ChatClientAssignment.h"

void ChatClientAssignment::Start()
{
}

void ChatClientAssignment::Stop() 
{
	stopping = true;
	stopped = true;
}

void ChatClientAssignment::Update()
{
}

void ChatClientAssignment::Render(sf::RenderWindow& window)
{
	client.render_gui();
}


