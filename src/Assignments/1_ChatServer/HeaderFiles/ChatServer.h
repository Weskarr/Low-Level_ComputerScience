
#pragma once
#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <vector>

struct Client
{
	std::string username;
	std::string identity;
	std::chrono::steady_clock::time_point last_ping;
	bool authenticated = false;
};

class ChatServer
{
public:
	ChatServer(int port);
	void run();

	void handle_message(zmq::multipart_t& msg);
	void handle_connect(const std::string& identity);
	void handle_pong(const std::string& identity);
	void handle_set_username(const std::string& identity, const std::string& username);
	void handle_private_message(const std::string& sender_identity, const std::string& target, const std::string& message);
	void handle_public_message(const std::string& sender_identity, const std::string& message);
	void handle_left_message(const std::string& sender_identity);
	void send_disconnect_message(const std::string& sender_identity);
	void send_message(const std::string& identity, const std::string& msg_type);
	void send_message(const std::string& identity, const std::string& msg_type, const std::string& data);
	void send_message(const std::string& identity, const std::string& msg_type, const std::string& data1, const std::string& data2);
	void ping_clients();
	void disconnect_client(const std::string& identity);

private:
	zmq::context_t context;
	zmq::socket_t socket;
	std::unordered_map<std::string, Client> clients; // identity -> client
	std::unordered_map<std::string, std::string> usernames; // username -> identity

};