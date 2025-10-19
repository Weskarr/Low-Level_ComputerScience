
#include "../HeaderFiles/Particle.h"

Particle::Particle(sf::Vector2f pos, sf::Vector2f vel, sf::Color col, float life)
    : isDying(false), active(true), position(pos), velocity(vel), color(col),
    lifetime(life), maxLifetime(life), hasGravity(true), isVisible(true),
    mass(1.0f), collisionEnabled(true), radius(2.0f), effectType('n')
{
    // Shape
    shape = new sf::CircleShape(radius);
    shape->setFillColor(color);
    shape->setPosition(position);

    // Debug info
    static int counter = 0;
    debugName = "Particle_" + std::to_string(counter);
    id = counter++;

    // Timing related
    creationTime = std::chrono::duration<double>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
    lastUpdateTime = creationTime;
    lastPosition = position;

    // Physics related
    acceleration = sf::Vector2f(0, 98.1f);
}

Particle::~Particle()
{
    delete shape;
}

void Particle::update(float deltaTime)
{
    if (!active) return;

    // Timing update
    auto now = std::chrono::high_resolution_clock::now();
    lastUpdateTime = std::chrono::duration<double>(now.time_since_epoch()).count();

    // Store last position
    lastPosition = position;

    // Physics calculations
    if (hasGravity) velocity += acceleration * deltaTime;
    position += velocity * deltaTime;

    // Color calculations
    float alpha = static_cast<float>(lifetime / maxLifetime);
    color.a = static_cast<uint8_t>(alpha * 255);

    // Update shape properties every frame
    shape->setPosition(position);
    shape->setFillColor(color);

    // Lifetime management
    lifetime -= deltaTime;
    if (lifetime <= 0) {
        isDying = true;
        active = false;
    }

    // Bounds checking
    if (position.x < 0 || position.x > 800 || position.y < 0 || position.y > 800) {
        if (collisionEnabled) {
            velocity *= -0.8f;
            position.x = std::clamp(position.x, 0.0f, 800.0f);
            position.y = std::clamp(position.y, 0.0f, 800.0f);
        }
    }
}

void Particle::render(sf::RenderWindow& window)
{
    if (isVisible && active) {
        window.draw(*shape);
    }
}

bool Particle::isAlive() const
{
    return active && !isDying;
}

// Expensive getter functions called frequently
sf::Vector2f Particle::getPosition() const { return position; }
sf::Vector2f Particle::getVelocity() const { return velocity; }
std::string Particle::getDebugName() const { return debugName; }
double Particle::getCreationTime() const { return creationTime; }
