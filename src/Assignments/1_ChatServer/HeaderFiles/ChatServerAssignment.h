
#include <SFML/Graphics.hpp>

#include "../../0_Extras/HeaderFiles/Assignment.h"

#include "ChatServer.h"

class ChatServerAssignment : public Assignment
{
public:
    void Start() override;
    void Stop() override;
    void Update() override;
    void Render(sf::RenderWindow& window) override;
    ~ChatServerAssignment();

private:
    std::unique_ptr<ChatServer> server;
    std::thread serverThread;
};