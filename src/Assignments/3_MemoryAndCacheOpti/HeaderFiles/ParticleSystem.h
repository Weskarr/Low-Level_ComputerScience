
#pragma once
#include <vector>
#include <memory>
#include <random>
#include <SFML/Graphics.hpp>
#include "Thread"

#include "Particle.h"
#include "GameObject.h"
#include "MultiThreading/ThreadPool.h"


enum class WayOfStorage
{
    AoS,
    SoA
};

// Inefficient particle system manager
class ParticleSystem
{
private:

    // AoS storage.
    std::vector<std::unique_ptr<Particle>> particles;

    // SoA storage.
    std::vector<sf::Vector2f> positions;
    std::vector<sf::Vector2f> velocities;
    std::vector<sf::Color> colors;
    std::vector<float> lifetimes;
    // std::vector<bool> aliveFlags;

    // Extras.
    std::mt19937 rng;
    uint32_t fixedSeed = 12345u;

public:

    ParticleSystem();

    void spawnParticles(WayOfStorage way, int count, sf::Vector2f origin);
    void UpdateOriginal(WayOfStorage way, float deltaTime);
    void render(WayOfStorage way, sf::RenderWindow& window);
    size_t getParticleCount(WayOfStorage way) const;

    // MultiThreading.
    ThreadPool Pool = std::thread::hardware_concurrency();

    void InitializeThreadPool();
    void ShutdownThreadPool();
    void UpdateThreadPool(WayOfStorage way, float deltaTime);
};
