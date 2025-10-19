#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>
#include <string>
#include <algorithm>

#include "GameObject.h"  // Base class

// Particle Class, inherits from GameObject
class Particle : public GameObject
{
private:
    
    // Statusses
    bool isDying;
    bool active;
    bool hasGravity;
    bool isVisible;
    bool collisionEnabled;

    // Attributes
    float radius;
    float mass;
    sf::Color color;
    sf::CircleShape* shape;

    // Position Related
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f lastPosition;
    sf::Vector2f acceleration;

    // Timing Related
    double lifetime;
    double maxLifetime;
    double creationTime;
    double lastUpdateTime;

    // Debugging
    std::string debugName;
    int id;

    // Useless At the moment..
    char effectType;

public:
    
    // Constructor
    Particle(sf::Vector2f pos, sf::Vector2f vel, sf::Color col, float life);
    
    // Inherited Functions
    ~Particle() override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    bool isAlive() const override;

    // Particle Class, inherits from GameObject
    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;
    std::string getDebugName() const;
    double getCreationTime() const;
};