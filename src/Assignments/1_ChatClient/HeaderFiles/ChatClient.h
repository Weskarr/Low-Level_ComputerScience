
#pragma once
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>
#include <random>

class ChatClient
{
public:
    ChatClient();
    ~ChatClient();

	void random_identity();
    void connect_to_server();
    void disconnect();
    void set_username();
    void send_private_message();
    void send_public_message();
    void send_leave_message();
    void render_gui();
    void leave();
    void send_message(const std::string& msg_type);
    void send_message(const std::string& msg_type, const std::string& data);
    void send_message(const std::string& msg_type, const std::string& data1, const std::string& data2);
    void receive_loop();
    void handle_server_message(zmq::multipart_t& msg);
    void add_message(const std::string& msg);

private:
    zmq::context_t context;
    zmq::socket_t socket;
    std::atomic<bool> connected{ false };
    std::atomic<bool> authenticated{ false };
    std::atomic<bool> running{ true };
    std::thread receive_thread;
    std::vector<std::string> messages;
    std::string username = "";
    std::string server_address = "tcp://localhost:5555";
	std::string identity = "";
	std::string last_private_target = "";
    std::string last_private_body = "";
    char username_buffer[64] = "";
    char target_buffer[64] = "";
    char message_buffer[256] = "";
    char server_buffer[128] = "tcp://localhost:5555";
    char password_buffer[32] = "Passkey123";
};