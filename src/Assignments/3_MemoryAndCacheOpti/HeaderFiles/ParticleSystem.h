
#pragma once
#include <vector>
#include <memory>
#include <random>
#include <SFML/Graphics.hpp>
#include "Particle.h"
#include "GameObject.h"

// Inefficient particle system manager
class ParticleSystem
{
private:
    // Particle Container
    std::vector<std::unique_ptr<GameObject>> particles;

    // Separate vectors per property
    std::vector<sf::Vector2f> positions;
    std::vector<sf::Vector2f> velocities;
    std::vector<sf::Color> colors;
    std::vector<bool> aliveFlags;
    //sf::RenderWindow* window;
    std::mt19937 rng;

public:
    ParticleSystem();

    void spawnParticles(int count, sf::Vector2f origin);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    size_t getParticleCount() const;
};
