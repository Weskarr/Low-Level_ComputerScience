
#include "../../0_Extras/HeaderFiles/Assignment.h"

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "Thread"

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