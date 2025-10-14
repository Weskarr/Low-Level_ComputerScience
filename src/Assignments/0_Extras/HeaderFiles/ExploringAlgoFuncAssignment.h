
#include <SFML/Graphics.hpp>

#include "Assignment.h"

class ExploringAlgoFuncAssignment : public Assignment
{
public:
    void Start() override;
    void Update() override;
    void Render(sf::RenderWindow& window) override;
};