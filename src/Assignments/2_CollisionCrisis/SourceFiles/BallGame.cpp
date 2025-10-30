
#include "../HeaderFiles/BallGame.h"

BallGame::BallGame() 
{
	gen = std::mt19937(rd());
	posDist = std::uniform_real_distribution<float>(5.0f, 535.0f);
	velDist = std::uniform_real_distribution<float>(-200.0f, 200.0f);
	colorDist = std::uniform_int_distribution<int>(0, 255);
	radiusDist = std::uniform_real_distribution<float>(2.5f, 2.5f);

	// Generate random balls
	for (int i = 0; i < 2500; ++i) 
	{
		sf::Color randomColor(colorDist(gen), colorDist(gen), colorDist(gen));
		balls.emplace_back
		(
			posDist(gen), posDist(gen), // position
			radiusDist(gen), // radius
			randomColor, // color
			velDist(gen), velDist(gen) // velocity
		);
	}
}

void BallGame::updateBalls(const sf::Vector2u& windowSize, float deltaTime) 
{
	// Update positions
	for (auto& ball : balls) 
	{
		ball.shape.move(ball.velocity * deltaTime);
	}

	// Handle ball-to-ball collisions
	BallToBallSpatialHashing();
	//BallToBallOriginal();

	// Handle wall collisions
	for (auto& ball : balls) {
		sf::Vector2f pos = ball.shape.getPosition();
		float radius = ball.shape.getRadius();
		// Bounce off walls
		if (pos.x - radius <= 0 || pos.x + radius >= windowSize.x) {
			ball.velocity.x = -ball.velocity.x;
			// Clamp position to prevent sticking
			if (pos.x - radius <= 0) {
				ball.shape.setPosition(sf::Vector2f(radius, pos.y));
			}
			else {
				ball.shape.setPosition(sf::Vector2f(windowSize.x - radius,
					pos.y));
			}
		}
		if (pos.y - radius <= 0 || pos.y + radius >= windowSize.y) {
			ball.velocity.y = -ball.velocity.y;
			// Clamp position to prevent sticking
			if (pos.y - radius <= 0) {
				ball.shape.setPosition(sf::Vector2f(pos.x, radius));
			}
			else {
				ball.shape.setPosition(sf::Vector2f(pos.x, windowSize.y -
					radius));
			}
		}
	}
}

void BallGame::BallToBallOriginal()
{
	for (size_t i = 0; i < balls.size(); ++i)
	{
		for (size_t j = i + 1; j < balls.size(); ++j)
		{
			Ball& ball1 = balls[i];
			Ball& ball2 = balls[j];
			sf::Vector2f pos1 = ball1.shape.getPosition();
			sf::Vector2f pos2 = ball2.shape.getPosition();
			float radius1 = ball1.shape.getRadius();
			float radius2 = ball2.shape.getRadius();

			// Calculate distance between centers
			sf::Vector2f delta = pos2 - pos1;
			float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
			float minDistance = radius1 + radius2;

			if (distance < minDistance && distance > 0)
			{
				// Normalize collision vector
				sf::Vector2f normal = delta / distance;

				// Separate balls to prevent overlap
				float overlap = minDistance - distance;
				sf::Vector2f separation = normal * (overlap * 0.5f);
				ball1.shape.setPosition(pos1 - separation);
				ball2.shape.setPosition(pos2 + separation);

				// Calculate relative velocity
				sf::Vector2f relativeVel = ball2.velocity - ball1.velocity;
				float velAlongNormal = relativeVel.x * normal.x + relativeVel.y * normal.y;

				// Don't resolve if velocities are separating
				if (velAlongNormal > 0) continue;

				// Apply collision response (elastic collision)
				float restitution = 0.0f; // Bounce factor (0 = no bounce, 1 = perfect bounce)
				float impulse = -(1 + restitution) * velAlongNormal;

				// Assume equal mass for simplicity
				sf::Vector2f impulseVector = impulse * normal;
				ball1.velocity -= impulseVector;
				ball2.velocity += impulseVector;
			}
		}
	}
}

void BallGame::BallToBallSpatialHashing() 
{
	// Clear hash grid.
	hashGrid.clear();

	// Create hash function for converting 2D grid coordinates to a unique key.
	// -> Absolutely no idea how this part works, voodo blackmagic shit.
	auto hash = [&](int x, int y) 
	{
		return (static_cast<long long>(x) << 32) | (static_cast<long long>(y) & 0xffffffff);
	};

	// Assign each ball to a spatial hash cell based on its position.
	for (size_t i = 0; i < balls.size(); ++i) 
	{
		auto pos = balls[i].shape.getPosition();
		int cellX = static_cast<int>(pos.x / cellSize);
		int cellY = static_cast<int>(pos.y / cellSize);

		// Insert this index into the corresponding cell in the hash grid.
		hashGrid[hash(cellX, cellY)].push_back(i);
	}

	// Collision check using nearby cells.
	for (size_t i = 0; i < balls.size(); ++i) 
	{
		auto pos1 = balls[i].shape.getPosition();
		int cellX = static_cast<int>(pos1.x / cellSize);
		int cellY = static_cast<int>(pos1.y / cellSize);

		for (int ox = -1; ox <= 1; ++ox) 
		{
			for (int oy = -1; oy <= 1; ++oy) 
			{
				auto h = hash(cellX + ox, cellY + oy);
				if (!hashGrid.count(h)) continue;

				for (size_t j : hashGrid[h])
				{
					if (i >= j) continue;

					Ball& ball1 = balls[i];
					Ball& ball2 = balls[j];
					sf::Vector2f pos1 = ball1.shape.getPosition();
					sf::Vector2f pos2 = ball2.shape.getPosition();
					float radius1 = ball1.shape.getRadius();
					float radius2 = ball2.shape.getRadius();

					// Calculate distance between centers
					sf::Vector2f delta = pos2 - pos1;
					float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
					float minDistance = radius1 + radius2;

					if (distance < minDistance && distance > 0)
					{
						// Normalize collision vector
						sf::Vector2f normal = delta / distance;

						// Separate balls to prevent overlap
						float overlap = minDistance - distance;
						sf::Vector2f separation = normal * (overlap * 0.5f);
						ball1.shape.move(-separation);
						ball2.shape.move(separation);

						// Calculate relative velocity
						sf::Vector2f relativeVel = ball2.velocity - ball1.velocity;
						float velAlongNormal = relativeVel.x * normal.x + relativeVel.y * normal.y;

						// Don't resolve if velocities are separating
						if (velAlongNormal > 0) continue;


						// Apply collision response (elastic collision)
						float restitution = 0.0f; // Bounce factor (0 = no bounce, 1 = perfect bounce)
						float impulse = -(1 + restitution) * velAlongNormal;

						// Assume equal mass for simplicity
						sf::Vector2f impulseVector = impulse * normal;
						ball1.velocity -= impulseVector;
						ball2.velocity += impulseVector;
					}
				}
			}
		}
	}
}

void BallGame::drawBalls(sf::RenderWindow& window)
{
	for (const auto& ball : balls) 
	{
		window.draw(ball.shape);
	}
}
