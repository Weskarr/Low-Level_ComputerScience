
#include "../HeaderFiles/ChatClient.h"


ChatClient::ChatClient() : context(1), socket(context, zmq::socket_type::dealer)
{
}

ChatClient::~ChatClient()
{
    running = false;

    if (receive_thread.joinable())
    {
        receive_thread.join();
    }
}

void ChatClient::random_identity() 
{
    // Seed random with high-resolution clock
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 9999);

    // Ranomly generate identity with the seed
    std::ostringstream ossOne;
    ossOne 
        << "Client_"
        << std::setw(4) 
        << std::setfill('0') 
        << dist(gen);
    std::string newIdentity = ossOne.str();

    // Message change of identity
    std::ostringstream ossTwo;
    if (identity.empty()) 
    {
        ossTwo
            << "ChatClient Identity: "
            << newIdentity
            << std::endl;
    }
    else 
    {
        ossTwo
            << "ChatClient Identity: "
            << identity
            << " -> "
            << newIdentity
            << std::endl;
    }
	add_message(ossTwo.str());

    // Set new identity
    identity = newIdentity;

	// Set identity on socket
    socket.set(zmq::sockopt::routing_id, identity);
    socket.set(zmq::sockopt::linger, 0);
}

void ChatClient::connect_to_server()
{
    try
    {
        // If socket already exists, disconnect & close it first.
        if (socket) 
        {
            socket.close();
        }

        // Create fresh socket to avoid auto ping disconnects.
        socket = zmq::socket_t(context, zmq::socket_type::dealer);

        // Randomize identity.
        random_identity();

        // Connect to server.
        socket.connect(server_buffer);
        server_address = server_buffer;
        running = true;

        // Start receiving thread if not running.
        if (!receive_thread.joinable())
            receive_thread = std::thread(&ChatClient::receive_loop, this);

		// Send login message to server.
        //send_message("CONNECT");
        send_message("LOGIN", std::string(password_buffer));

		// Visualise connection message
        add_message("Connecting to " + server_address + "...");
    }
    catch (const zmq::error_t& e)
    {
        add_message("Connection failed: " + std::string(e.what()));
    }
}

void ChatClient::disconnect()
{
    if (!connected) return;

    // Stop receiving loop
    running = false;
    if (receive_thread.joinable())
        receive_thread.join();

    // Disconnect & close socket completely
    socket.close();

    // Reset state
    connected = false;
    authenticated = false;
    username = "";
    messages.clear();

	// Visualise connection message
    add_message("Disconnected from server.");
}

void ChatClient::set_username()
{
    if (!connected) 
    { 
        add_message("Not connected"); 
        return; 
    }

    if (strlen(username_buffer) == 0) 
        return;

    std::string desired(username_buffer);
    send_message("SET_USERNAME", desired);
}

void ChatClient::send_private_message()
{
    if (!authenticated) 
    {
        add_message("Not authenticated"); 
        return; 
    }

    if (strlen(target_buffer) == 0 || strlen(message_buffer) == 0) 
        return;

    last_private_body = std::string(message_buffer);
    last_private_target = std::string(target_buffer);

    send_message("PRIVATE_MSG", std::string(target_buffer), std::string(message_buffer));
}

void ChatClient::send_public_message()
{
    if (!authenticated) 
    { 
        add_message("Not authenticated"); 
        return; 
    }

    if (strlen(message_buffer) == 0) 
        return;

    send_message("PUBLIC_MSG", std::string(message_buffer));
}

void ChatClient::send_leave_message()
{
    if (!connected) 
        return;
    send_message("LEFT");
}

void ChatClient::leave()
{
    send_leave_message();
}

void ChatClient::render_gui()
{
    // Connection panel
    ImGui::Begin("Chat Client");

    // Debug flags for development
    ImGui::Text
    (
        "Debug: connected=%s authenticated=%s identity=%s name=%s",
        connected ? "true" : "false",
        authenticated ? "true" : "false",
        identity.c_str(),
        username.c_str()
    );

    if (!connected)
    {
        ImGui::InputText("Server", server_buffer, sizeof(server_buffer));
        ImGui::InputText("Password", password_buffer, sizeof(password_buffer));

        if (ImGui::Button("Connect"))
            connect_to_server();
    }
    else
    {
        ImGui::Text("Connected to: %s", server_address.c_str());
        ImGui::SameLine();

        if (ImGui::Button("Disconnect"))
            disconnect();
    }

    ImGui::Separator();

    // Username section
    if (connected && !authenticated)
    {
        ImGui::Text("Set your username:");
        ImGui::InputText("Username", username_buffer, sizeof(username_buffer));

        if (ImGui::Button("Set Username") && strlen(username_buffer) > 0)
            set_username();
    }
    else if (authenticated)
    {
        ImGui::Text("Logged in as: %s", username.c_str());
    }

    ImGui::Separator();

    // Message area
    ImGui::BeginChild("Messages", ImVec2(0, -100), true);

    for (const auto& msg : messages)
    {
        ImGui::TextWrapped("%s", msg.c_str());
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();

    // Send message section
    if (authenticated)
    {
        ImGui::InputText("Target", target_buffer, sizeof(target_buffer));
        ImGui::InputText("Message", message_buffer, sizeof(message_buffer));

        if (ImGui::Button("Send") || ImGui::IsKeyPressed(ImGuiKey_Enter))
        {
            // TODO: Send public & private message

            // if target is set, send private, otherwise public
            if (strlen(target_buffer) > 0)
            {
                if (target_buffer == username)
                {
                    add_message("[PRIVATE] You cannot send a private message to yourself.");
                    target_buffer[0] = '\0';
                }
                else 
                {
                    send_private_message();
                    message_buffer[0] = '\0';
                }
            }
            else 
            {
                send_public_message();
                message_buffer[0] = '\0';
            }
        }
    }

    ImGui::End();
}

void ChatClient::send_message
(
    const std::string& msg_type
)
{
    // TODO: Create multipart_t zmq message, add type, send
    zmq::multipart_t msg;
    msg.add(zmq::message_t(msg_type.begin(), msg_type.end()));
    msg.send(socket);
}

void ChatClient::send_message
(
    const std::string& msg_type,
    const std::string& data
)
{
    // TODO: Create multipart_t zmq message, add type & data, send
    zmq::multipart_t msg;
    msg.add(zmq::message_t(msg_type.begin(), msg_type.end()));
    msg.add(zmq::message_t(data.begin(), data.end()));
    msg.send(socket);
}

void ChatClient::send_message
(
    const std::string& msg_type,
    const std::string& data1,
    const std::string& data2
)
{
    // TODO: Create multipart_t zmq message, add type & data1 & data2, send
    zmq::multipart_t msg;
    msg.add(zmq::message_t(msg_type.begin(), msg_type.end()));
    msg.add(zmq::message_t(data1.begin(), data1.end()));
    msg.add(zmq::message_t(data2.begin(), data2.end()));
    msg.send(socket);
}

void ChatClient::receive_loop()
{
    while (running)
    {
        try
        {
            zmq::multipart_t msg;

            if (msg.recv(socket, ZMQ_DONTWAIT))
                handle_server_message(msg);

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        catch (const zmq::error_t& e)
        {
            if (running)
                add_message("Receive error: " + std::string(e.what()));

            break;
        }
    }
}

void ChatClient::handle_server_message(zmq::multipart_t& msg)
{
    if (msg.size() < 1) return;

    std::string msg_type = msg[0].to_string();

    if (msg_type == "CONNECTED")
    {
        add_message("[PRIVATE] Connected to server.");
    }
    else if (msg_type == "DISCONNECTED")
    {
        add_message("[PRIVATE] Disconnected from server.");
    }
    else if (msg_type == "PING")
    {
        send_message("[PRIVATE] PONG");
        return;
    }
    else if (msg_type == "USERNAME_SET")
    {
        if (msg.size() >= 2)
        {
            username = msg[1].to_string();
            authenticated = true;
            add_message("[PRIVATE] You are now known as: " + username);
        }
    }
    else if (msg_type == "USERNAME_TAKEN")
    {
        add_message("Username is already taken, choose another.");
    }
    else if (msg_type == "PRIVATE_MSG")
    {
        if (msg.size() >= 3)
        {
            std::string from = msg[1].to_string();
            std::string body = msg[2].to_string();
            add_message("[PRIVATE] " + from + " -> " + username + ": " + body);
        }
    }
    else if (msg_type == "PUBLIC_MSG")
    {
        if (msg.size() >= 3)
        {
            std::string from = msg[1].to_string();
            std::string body = msg[2].to_string();
            add_message("[PUBLIC] " + from + ": " + body);
        }
    }
    else if (msg_type == "MSG_DELIVERED")
    {
        if (last_private_target.empty() || last_private_body.empty())
			return;

        add_message("[PRIVATE] " + username + " -> " + last_private_target + ": " + last_private_body);

		last_private_body = "";
		last_private_target = "";
    }
    else if (msg_type == "USER_NOT_FOUND")
    {
        add_message("[PRIVATE] User not found.");
    }
    else if (msg_type == "NOT_AUTHENTICATED")
    {
        add_message("[PRIVATE] You are not authenticated.");
    }
    else if (msg_type == "PUBLIC_MSG" && msg.size() >= 3)
    {
        std::string sender = msg[1].to_string();
        std::string message = msg[2].to_string();
        add_message("[PUBLIC] " + sender + ": " + message);
    }
    else if (msg_type == "JOINED" && msg.size() >= 2)
    {
        std::string new_user = msg[1].to_string();
        add_message("[PUBLIC] " + new_user + " joined the chat.");
    }
    else if (msg_type == "LEFT" && msg.size() >= 2)
    {
        std::string user_left = msg[1].to_string();
        add_message("[PUBLIC] " + user_left + " left the chat.");
    }
    else if (msg_type == "LOGIN_ACCEPTED")
    {
        add_message("[PRIVATE] Login accepted, correct password.");
        connected = true;
    }
    else if (msg_type == "LOGIN_DENIED")
    {
        add_message("[PRIVATE] Login denied, incorrect password.");
        connected = false;
    }

    // TODO: Implement messages from server:
    // 
    // x    CONNECTED
    // x    DISCONNECTED
    // x    PING
    // x    USERNAME_SET
    // x    USERNAME_TAKEN
    // x    PRIVATE_MSG
    // x    MSG_DELIVERED
    // x    USER_NOT_FOUND
    // x    NOT_AUTHENTICATED
    // x    PUBLIC_MSG
    // x    JOINED
    // x    LEFT
    // x    LOGIN_ACCEPTED  (Own addition!)
    // x    LOGIN_DENIED    (Own addition!)
    // 
    // Use "add_message(...)" function to put things into the Chat Window
    // TIP: you can read the data out of a msg by msg[1], msg[2], etc., and
    // check the number of parts of the message with msg.size()
}

void ChatClient::add_message(const std::string& message)
{
    messages.push_back(message);

    // Limit message history
    if (messages.size() > 100)
    {
        messages.erase(messages.begin());
    }
}