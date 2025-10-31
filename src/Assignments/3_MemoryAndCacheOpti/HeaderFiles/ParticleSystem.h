
#pragma once
#include <vector>
#include <memory>
#include <random>
#include <SFML/Graphics.hpp>
#include "Thread"
#include <algorithm>

//#include "ThreadPool.h"
#include "../../0_Extras/HeaderFiles/MultiThreading/ThreadPool.h"

struct RandomRanges
{
    std::uniform_real_distribution<float> angleRange{ 0, 2 * 3.14159f };
    std::uniform_real_distribution<float> speedRange{ 50, 200 };
    std::uniform_int_distribution<int> colorRange{ 0, 255 };
    std::uniform_real_distribution<float> lifeRange{ 1.0f, 5.0f };
};

struct SimpleParticle
{
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color;
    float lifetime;
    float fullLifetime;
};

enum class WayOfStorage
{
    AoS,
    SoA
};

// A little bit Inefficient particle system manager..
class ParticleSystem
{
public:

    // General:
    void Start(WayOfStorage way, bool multithreading, bool randomSeed);
    void Spawn(int count, sf::Vector2f origin);
    void Update(float deltaTime);
    void Render(sf::RenderWindow& window);
    void Stop();

    // Getters:
    size_t GetParticleCount() const;
    size_t GetThreadCount() const;

private:

    // Status:
    bool isMultiThreading;
    bool isRandomSeed;
    WayOfStorage wayOfStorage;
    unsigned currentSeed;

    // Randomness:
    RandomRanges randomRanges;
    unsigned fixedSeed = 12345;
    std::mt19937 rng;

    // Multi-threading:
    ThreadPool& Pool = ThreadPool::GetInstance();

    // Shared
    void UpdateSharedPhysics
    (
        sf::Vector2f& position,
        sf::Vector2f& velocity,
        float& lifetime,
        float maxLifetime,
        sf::Color& color,
        float deltaTime
    );

    // AoS:
    std::vector<SimpleParticle> particles;
    void SpawnAoS(int count, sf::Vector2f origin);
    void UpdateSingleAoS(float deltaTime);
    void UpdateMultiAoS(float deltaTime);
    void RenderAoS(sf::RenderWindow& window);
    void RemoveDeadAoS();

    // SoA:
    std::vector<sf::Vector2f> positions;
    std::vector<sf::Vector2f> velocities;
    std::vector<sf::Color> colors;
    std::vector<float> lifetimes;
    std::vector<float> fullLifetimes;
    void SpawnSoA(int count, sf::Vector2f origin);
    void UpdateSingleSoA(float deltaTime);
    void UpdateMultiSoA(float deltaTime);
    void RenderSoA(sf::RenderWindow& window);
    void RemoveDeadSoA();
};
