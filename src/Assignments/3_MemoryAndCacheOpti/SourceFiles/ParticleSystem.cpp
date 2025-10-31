
#include "../HeaderFiles/ParticleSystem.h"

#pragma region General

void ParticleSystem::Start(WayOfStorage way, bool multithreading, bool randomSeed)
{
    wayOfStorage = way;
    isMultiThreading = multithreading;
    isRandomSeed = randomSeed;

    //if (isMultiThreading)
    //    Pool.init();

    if (isRandomSeed)
        currentSeed = std::random_device{}();
    else
        currentSeed = fixedSeed;

    rng.seed(currentSeed);
}

void ParticleSystem::Spawn(int count, sf::Vector2f origin)
{
    // Spawn particles relative to way of storage.
    if (wayOfStorage == WayOfStorage::AoS)
        SpawnAoS(count, origin);
    else 
        SpawnSoA(count, origin);
}

void ParticleSystem::Update(float deltaTime)
{
    // Update particles relative to way of storage & multithreading method.
    if (wayOfStorage == WayOfStorage::AoS)
        isMultiThreading ? UpdateMultiAoS(deltaTime) : UpdateSingleAoS(deltaTime);
    else
        isMultiThreading ? UpdateMultiSoA(deltaTime) : UpdateSingleSoA(deltaTime);
}

void ParticleSystem::Render(sf::RenderWindow& window)
{
    // Render particles relative to way of storage.
    if (wayOfStorage == WayOfStorage::AoS)
        RenderAoS(window);
    else
        RenderSoA(window);
}

void ParticleSystem::Stop()
{
    //if (isMultiThreading)
    //    Pool.shutdown();
}

#pragma endregion

#pragma region Getters

// Get particle count.
size_t ParticleSystem::GetParticleCount() const
{
    return (wayOfStorage == WayOfStorage::AoS) ? particles.size() : positions.size();
}

// Get thread count.
size_t ParticleSystem::GetThreadCount() const
{
    return Pool.getThreadCount();
}

#pragma endregion

#pragma region Shared

void ParticleSystem::UpdateSharedPhysics
(
    sf::Vector2f& position,
    sf::Vector2f& velocity,
    float& lifetime,
    float maxLifetime,
    sf::Color& color,
    float deltaTime
)
{
    // Gravity
    velocity += sf::Vector2f(0.0f, 98.1f) * deltaTime;
    
    // Position update
    position += velocity * deltaTime;
    lifetime -= deltaTime;

    // Change alpha based on remaining lifetime.
    float alpha = std::clamp(lifetime / maxLifetime, 0.f, 1.f);
    color.a = static_cast<uint8_t>(alpha * 255);


    // Bounds & bounce.
    if (position.x < 0.f || position.x > 960.f) {
        velocity.x *= -0.8f;
        position.x = std::clamp(position.x, 0.f, 960.f);
    }
    if (position.y < 0.f || position.y > 540.f) {
        velocity.y *= -0.8f;
        position.y = std::clamp(position.y, 0.f, 540.f);
    }
}

#pragma endregion

#pragma region AoS

void ParticleSystem::SpawnAoS(int count, sf::Vector2f origin)
{
    // Pre-allocation
    particles.reserve(count);

    // Spawning of Particles.
    for (int i = 0; i < count; ++i)
    {
        float angle = randomRanges.angleRange(rng);
        float speed = randomRanges.speedRange(rng);
        sf::Color color(randomRanges.colorRange(rng), randomRanges.colorRange(rng), randomRanges.colorRange(rng), 255);
        float lifetime = randomRanges.lifeRange(rng);

        sf::Vector2f velocity(std::cos(angle) * speed, std::sin(angle) * speed);

        particles.emplace_back(origin, velocity, color, lifetime, lifetime);
    }
}

void ParticleSystem::UpdateSingleAoS(float deltaTime)
{
    for (auto& p : particles)
    {
        UpdateSharedPhysics
        (
            p.position,
            p.velocity,
            p.lifetime,
            p.fullLifetime,
            p.color,
            deltaTime
        );
    }

    RemoveDeadAoS();
}

void ParticleSystem::UpdateMultiAoS(float deltaTime)
{
    if (particles.empty()) return;

    size_t numThreads = Pool.getThreadCount();
    size_t chunk = particles.size() / numThreads;

    std::vector<std::future<void>> futures;
    for (size_t t = 0; t < numThreads; ++t)
    {
        size_t start = t * chunk;
        size_t end = (t == numThreads - 1) ? particles.size() : start + chunk;

        futures.push_back(Pool.submit([this, start, end, deltaTime]()
        {
                for (size_t i = start; i < end; ++i)
                {
                    auto& p = particles[i];
                    UpdateSharedPhysics
                    (
                        p.position,
                        p.velocity,
                        p.lifetime,
                        p.fullLifetime,
                        p.color,
                        deltaTime
                    );
                }
        }));
    }

    for (auto& f : futures) 
        f.get();

    RemoveDeadAoS();
}

void ParticleSystem::RenderAoS(sf::RenderWindow& window)
{
    sf::CircleShape shape(2.0f);
    for (const auto& p : particles)
    {
        shape.setPosition(p.position);
        shape.setFillColor(p.color);
        window.draw(shape);
    }
}

void ParticleSystem::RemoveDeadAoS()
{
    // Remove dead particles using swap & pop.
    for (size_t i = 0; i < particles.size();)
    {
        if (particles[i].lifetime <= 0)
        {
            std::swap(particles[i], particles.back());
            particles.pop_back();
        }
        else ++i;
    }
}

#pragma endregion

#pragma region SoA

void ParticleSystem::SpawnSoA(int count, sf::Vector2f origin)
{
    // Pre-allocation.
    positions.reserve(count);
    velocities.reserve(count);
    colors.reserve(count);
    lifetimes.reserve(count);

    // Spawning of Particles.
    for (int i = 0; i < count; ++i)
    {
        float angle = randomRanges.angleRange(rng);
        float speed = randomRanges.speedRange(rng);
        sf::Color color(randomRanges.colorRange(rng), randomRanges.colorRange(rng), randomRanges.colorRange(rng), 255);
        float lifetime = randomRanges.lifeRange(rng);

        sf::Vector2f velocity(std::cos(angle) * speed, std::sin(angle) * speed);

        positions.push_back(origin);
        velocities.push_back(velocity);
        colors.push_back(color);
        lifetimes.push_back(lifetime);
        fullLifetimes.push_back(lifetime);
    }
}

void ParticleSystem::UpdateSingleSoA(float deltaTime) 
{
    for (size_t i = 0; i < positions.size(); ++i)
    {
        UpdateSharedPhysics
        (
            positions[i],
            velocities[i],
            lifetimes[i],
            fullLifetimes[i],
            colors[i],
            deltaTime
        );
    }

    RemoveDeadSoA();
}

void ParticleSystem::UpdateMultiSoA(float deltaTime)
{
    if (positions.empty()) return;

    size_t numThreads = Pool.getThreadCount();
    size_t chunk = positions.size() / numThreads;

    std::vector<std::future<void>> futures;
    for (size_t t = 0; t < numThreads; ++t)
    {
        size_t start = t * chunk;
        size_t end = (t == numThreads - 1) ? positions.size() : start + chunk;

        futures.push_back(Pool.submit([this, start, end, deltaTime]()
        {
                for (size_t i = start; i < end; ++i)
                {
                    UpdateSharedPhysics
                    (
                        positions[i],
                        velocities[i],
                        lifetimes[i],
                        fullLifetimes[i],
                        colors[i],
                        deltaTime
                    );
                }
        }));
    }

    for (auto& f : futures) 
        f.get();

    RemoveDeadSoA();
}

void ParticleSystem::RenderSoA(sf::RenderWindow& window)
{
    sf::CircleShape shape(2.0f);
    for (size_t i = 0; i < positions.size(); ++i)
    {
        shape.setPosition(positions[i]);
        shape.setFillColor(colors[i]);
        window.draw(shape);
    }
}

void ParticleSystem::RemoveDeadSoA()
{
    // Remove dead particles using swap & pop.
    for (size_t i = 0; i < lifetimes.size();)
    {
        if (lifetimes[i] <= 0)
        {
            size_t last = lifetimes.size() - 1;
            std::swap(positions[i], positions[last]);
            std::swap(velocities[i], velocities[last]);
            std::swap(colors[i], colors[last]);
            std::swap(lifetimes[i], lifetimes[last]);

            positions.pop_back();
            velocities.pop_back();
            colors.pop_back();
            lifetimes.pop_back();
        }
        else ++i;
    }
}

#pragma endregion