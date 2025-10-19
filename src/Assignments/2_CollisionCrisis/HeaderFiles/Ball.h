
#pragma once
#include <vector>
#include <random>
#include <cmath>
#include <SFML/Graphics.hpp>

struct Ball
{
public:
	Ball(float x, float y, float radius, sf::Color color, float vx, float vy);
	sf::CircleShape shape;
	sf::Vector2f velocity;
};