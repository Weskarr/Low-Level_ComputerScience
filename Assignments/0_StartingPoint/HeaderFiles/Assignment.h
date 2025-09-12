
#pragma once
#include <SFML/Graphics.hpp>

class Assignment
{
public:
    virtual void Update() = 0;
    virtual void Render(sf::RenderWindow& window) = 0;
    virtual ~Assignment() = default;
};