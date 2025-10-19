
#include "../HeaderFiles/Ball.h"

Ball::Ball(float x, float y, float radius, sf::Color color, float vx, float vy)
{
	shape.setRadius(radius);
	shape.setPosition(sf::Vector2f(x, y));
	shape.setFillColor(color);
	shape.setOrigin(sf::Vector2f(radius, radius)); // Center origin
	velocity = sf::Vector2f(vx, vy);
}