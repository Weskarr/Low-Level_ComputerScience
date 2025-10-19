
#pragma once
#include "../HeaderFiles/Ball.h"

class BallGame
{
private:
	// Create balls
	std::vector<Ball> balls;
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_real_distribution<float> posDist;
	std::uniform_real_distribution<float> velDist;
	std::uniform_int_distribution<int> colorDist;
	std::uniform_real_distribution<float> radiusDist;

public:
	BallGame();
	void updateBalls(const sf::Vector2u& windowSize, float deltaTime);
	void drawBalls(sf::RenderWindow& window);
};