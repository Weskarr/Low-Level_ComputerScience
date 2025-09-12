
#include <SFML/Graphics.hpp>

#include "Assignment.h"

class PlaygroundAssignment : public Assignment
{
public:
    void Update() override;
    void Render(sf::RenderWindow& window) override;
};