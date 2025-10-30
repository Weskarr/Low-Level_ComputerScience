
#include "../HeaderFiles/ParticleSystem.h"

ParticleSystem::ParticleSystem()
    : rng(std::random_device{} ())
{ 
}

void ParticleSystem::InitializeThreadPool() 
{
    Pool.init();
}

void ParticleSystem::ShutdownThreadPool() 
{
    Pool.shutdown();
}

void ParticleSystem::spawnParticles(WayOfStorage way, int count, sf::Vector2f origin)
{
    std::uniform_real_distribution<float> angleDist(0, 2 * 3.14159f);
    std::uniform_real_distribution<float> speedDist(50, 200);
    std::uniform_int_distribution<int> colorDist(0, 255);
    std::uniform_real_distribution<float> lifeDist(1.0f, 5.0f);

    // Pre-allocation
    particles.reserve(count);

    // Fixed seed for more accurate comparison.
    rng.seed(fixedSeed);

    if (way == WayOfStorage::AoS)
    {
        for (int i = 0; i < count; ++i)
        {
            float angle = angleDist(rng);
            float speed = speedDist(rng);

            sf::Vector2f velocity(std::cos(angle) * speed, std::sin(angle) * speed);
            sf::Color color(colorDist(rng), colorDist(rng), colorDist(rng), 255);

            float lifetime = lifeDist(rng);

            // Create particle
            //auto particle = std::make_unique<Particle>(origin, velocity, color, lifetime);

            // Store in containers
            //particles.push_back(std::move(particle));

            particles.push_back(std::make_unique<Particle>(origin, velocity, color, lifetime));
        }
    }
    else if (way == WayOfStorage::SoA)
    {
        positions.reserve(count);
        velocities.reserve(count);
        colors.reserve(count);
        lifetimes.reserve(count);

        for (int i = 0; i < count; ++i) 
        {
            float angle = angleDist(rng);
            float speed = speedDist(rng);
            sf::Vector2f velocity(std::cos(angle) * speed, std::sin(angle) * speed);
            sf::Color color(colorDist(rng), colorDist(rng), colorDist(rng), 255);
            float lifetime = lifeDist(rng);

            positions.push_back(origin);
            velocities.push_back(velocity);
            colors.push_back(color);
            lifetimes.push_back(lifetime);
        }
    }
}

void ParticleSystem::UpdateOriginal(WayOfStorage way, float deltaTime)
{
    if (way == WayOfStorage::AoS)
    {
        for (auto it = particles.begin(); it != particles.end(); ) 
{
            (*it)->update(deltaTime);
            if (!(*it)->isAlive()) 
                it = particles.erase(it);
            else 
                ++it;
        }
    }
    else if (way == WayOfStorage::SoA)
    {
        for (size_t i = 0; i < positions.size(); ++i) 
        {
            velocities[i] += sf::Vector2f(0, 98.1f) * deltaTime;
            positions[i] += velocities[i] * deltaTime;
            lifetimes[i] -= deltaTime;
        }

        // Remove dead particles.
        for (size_t i = 0; i < lifetimes.size(); ) 
        {
            if (lifetimes[i] <= 0) 
            {
                positions.erase(positions.begin() + i);
                velocities.erase(velocities.begin() + i);
                colors.erase(colors.begin() + i);
                lifetimes.erase(lifetimes.begin() + i);
            }
            else 
            {
                ++i;
            }
        }
    }
}

void ParticleSystem::UpdateThreadPool(WayOfStorage way, float deltaTime)
{
    if (way == WayOfStorage::SoA && !positions.empty())
    {
        size_t numThreads = Pool.getThreadCount();
        size_t chunkSize = positions.size() / numThreads;
        std::vector<std::future<void>> futures;

        for (size_t t = 0; t < numThreads; ++t)
        {
            size_t start = t * chunkSize;
            size_t end = (t == numThreads - 1) ? positions.size() : start + chunkSize;

            futures.push_back(Pool.submit([this, start, end, deltaTime]()
                {
                    for (size_t i = start; i < end; ++i)
                    {
                        velocities[i] += sf::Vector2f(0, 98.1f) * deltaTime;
                        positions[i] += velocities[i] * deltaTime;
                        lifetimes[i] -= deltaTime;
                    }
                }));
        }

        for (auto& f : futures)
            f.get();

        // Remove dead particles using swap & pop.
        // -> This is single-threaded since it's more sercure.
        for (size_t i = 0; i < lifetimes.size(); )
        {
            if (lifetimes[i] <= 0.0f)
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
    else if (way == WayOfStorage::AoS && !particles.empty())
    {
        size_t numThreads = Pool.getThreadCount();
        size_t chunkSize = particles.size() / numThreads;
        std::vector<std::future<void>> futures;

        for (size_t t = 0; t < numThreads; ++t)
        {
            size_t start = t * chunkSize;
            size_t end = (t == numThreads - 1) ? particles.size() : start + chunkSize;

            futures.push_back(Pool.submit([this, start, end, deltaTime]()
                {
                    for (size_t i = start; i < end; ++i)
                    {
                        if (particles[i])
                            particles[i]->update(deltaTime);
                    }
                }));
        }

        for (auto& f : futures)
            f.get();

        // Remove dead particles using swap & pop.
        for (size_t i = 0; i < particles.size(); )
        {
            if (!particles[i] || !particles[i]->isAlive())
            {
                size_t last = particles.size() - 1;
                std::swap(particles[i], particles[last]);
                particles.pop_back();
            }
            else ++i;
        }
    }
}



void ParticleSystem::render(WayOfStorage way, sf::RenderWindow& window)
{
    // Render all particles
    if (way == WayOfStorage::AoS)
    {
        for (auto& p : particles) 
            p->render(window);
    }
    else if (way == WayOfStorage::SoA)
    {
        for (size_t i = 0; i < positions.size(); ++i) {
            sf::CircleShape shape(2.0f);
            shape.setPosition(positions[i]);
            shape.setFillColor(colors[i]);
            window.draw(shape);
        }
    }
}

size_t ParticleSystem::getParticleCount(WayOfStorage way) const
{
    return (way == WayOfStorage::AoS) ? particles.size() : positions.size();
}
