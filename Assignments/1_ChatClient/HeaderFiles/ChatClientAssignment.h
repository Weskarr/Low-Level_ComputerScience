
#include <SFML/Graphics.hpp>

#include "../../0_Extras/HeaderFiles/Assignment.h"

class ChatClientAssignment : public Assignment
{
public:
    void Start() override;
    void Update() override;
    void Render(sf::RenderWindow& window) override;
};