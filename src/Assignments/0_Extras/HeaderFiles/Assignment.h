
#pragma once
#include <SFML/Graphics.hpp>

class Assignment
{
public:
    bool stopping = false;
    bool stopped = false;
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Update() = 0;
    virtual void Render(sf::RenderWindow& window) = 0;
    virtual ~Assignment() = default;
};