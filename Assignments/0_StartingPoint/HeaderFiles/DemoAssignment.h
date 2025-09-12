
#include <SFML/Graphics.hpp>

#include "Assignment.h"

class DemoAssignment : public Assignment
{
public:
    void Update() override;
    void Render(sf::RenderWindow& window) override;
};