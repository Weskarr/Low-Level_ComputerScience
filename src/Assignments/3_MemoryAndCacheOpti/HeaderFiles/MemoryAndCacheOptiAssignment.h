
#include <SFML/Graphics.hpp>

#include "../../0_Extras/HeaderFiles/Assignment.h"

class MemoryAndCacheOptiAssignment : public Assignment
{
public:
    void Start() override;
    void Stop() override;
    void Update() override;
    void Render(sf::RenderWindow& window) override;
};