
#include "../HeaderFiles/ChatServer.h"


ChatServer::ChatServer(int port) : context(1), socket(context, zmq::socket_type::router)
{
	socket.bind("tcp://*:" + std::to_string(port));

	std::cout
		<< "Chat server listening on port "
		<< port
		<< std::endl;
}

void ChatServer::run()
{
	running = true;

	std::cout << "Chat server started." << std::endl;
	std::thread ping_thread(&ChatServer::ping_clients, this);

	while (running)
	{
		try
		{
			zmq::multipart_t msg;

			if (msg.recv(socket, ZMQ_DONTWAIT))
				handle_message(msg);

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		catch (const zmq::error_t& e)
		{
			std::cerr
				<< "Error: "
				<< e.what()
				<< std::endl;
		}

		//std::cout << "ChatServer While Run Cycle Completed" << std::endl;
	}

	ping_thread.join();
}

void ChatServer::handle_message(zmq::multipart_t& msg)
{
	if (msg.size() < 2) return;

	std::string identity = msg[0].to_string();
	std::string msg_type = msg[1].to_string();

	// Update last seen time
	if (clients.find(identity) != clients.end())
	{
		clients[identity].last_ping = std::chrono::steady_clock::now();
	}

	if (msg_type == "CONNECT")
	{
		handle_connect(identity);
	}
	else if (msg_type == "PONG")
	{
		handle_pong(identity);
	}
	else if (msg_type == "SET_USERNAME" && msg.size() >= 3)
	{
		handle_set_username(identity, msg[2].to_string());
	}
	else if (msg_type == "PRIVATE_MSG" && msg.size() >= 4)
	{
		handle_private_message(identity, msg[2].to_string(),
			msg[3].to_string());
	}
	else if (msg_type == "PUBLIC_MSG" && msg.size() >= 3)
	{
		handle_public_message(identity, msg[2].to_string());
	}
	else if (msg_type == "LEFT")
	{
		handle_left_message(identity);
		disconnect_client(identity);
	}
	else if (msg_type == "LOGIN")
	{
		handle_password(identity, msg[2].to_string());
	}
}

void ChatServer::handle_password(const std::string& identity, const std::string& key)
{
	// Password Check.
	if (key == "Passkey123") 
	{
		// Accepted Entry.
		std::cout << "Login Server Accepted." << std::endl;
		handle_connect(identity);
		send_message(identity, "LOGIN_ACCEPTED");
	}
	else
	{
		// Denied Entry.
		std::cout << "Login Server Denied." << std::endl;
		send_message(identity, "LOGIN_DENIED");
	}
}

void ChatServer::handle_connect(const std::string& identity)
{
	clients[identity] = Client
	{
		.username = "",
		.identity = identity,
		.last_ping = std::chrono::steady_clock::now(),
		.authenticated = false
	};

	send_message(identity, "CONNECTED");

	std::cout
		<< "Client connected: "
		<< identity
		<< std::endl;
}

void ChatServer::handle_pong(const std::string& identity)
{
	if (clients.find(identity) != clients.end())
	{
		clients[identity].last_ping = std::chrono::steady_clock::now();
	}
}

void ChatServer::handle_set_username
(
	const std::string& identity,
	const std::string& username
)
{
	if (clients.find(identity) == clients.end()) return;

	// Check if username is already taken
	if (usernames.find(username) != usernames.end()
		&& usernames[username] != identity)
	{
		send_message(identity, "USERNAME_TAKEN");
		return;
	}

	// Remove old username mapping if exists
	if (!clients[identity].username.empty())
	{
		usernames.erase(clients[identity].username);
	}

	// Set new username
	clients[identity].username = username;
	clients[identity].authenticated = true;
	usernames[username] = identity;

	send_message(identity, "USERNAME_SET", username);

	std::cout
		<< "Username set: "
		<< username << " ("
		<< identity << ")"
		<< std::endl;

	for (auto& [target_identity, client] : clients)
	{
		// Forward message to all
		send_message(target_identity, "JOINED",
			clients[identity].username);
	}
}

void ChatServer::handle_private_message
(
	const std::string& sender_identity,
	const std::string& target,
	const std::string& message
)
{
	if (clients.find(sender_identity) == clients.end() ||
		!clients[sender_identity].authenticated)
	{
		send_message(sender_identity, "NOT_AUTHENTICATED");
		return;
	}

	std::string target_identity;

	// Check if target is a username or identity
	if (usernames.find(target) != usernames.end())
	{
		target_identity = usernames[target];
	}
	else if (clients.find(target) != clients.end())
	{
		target_identity = target;
	}
	else
	{
		send_message(sender_identity, "USER_NOT_FOUND");
		return;
	}

	// Send message to target
	send_message(target_identity, "PRIVATE_MSG",
		clients[sender_identity].username, message);

	// Confirm to sender
	send_message(sender_identity, "MSG_DELIVERED");

	std::cout
		<< "Private message: "
		<< clients[sender_identity].username
		<< " -> " << target
		<< std::endl;
}

void ChatServer::handle_public_message
(
	const std::string& sender_identity,
	const std::string& message
)
{
	if (clients.find(sender_identity) == clients.end()
		|| !clients[sender_identity].authenticated)
	{
		send_message(sender_identity, "NOT_AUTHENTICATED");
		return;
	}

	for (auto& [identity, client] : clients)
	{
		// Forward message to all
		send_message(identity, "PUBLIC_MSG",
			clients[sender_identity].username, message);
	}

	std::cout
		<< "Public message: "
		<< clients[sender_identity].username
		<< std::endl;
}

void ChatServer::handle_left_message(const std::string& sender_identity)
{
	// Skip this if only one or no name is set.
	if (clients.find(sender_identity) == clients.end()
		|| !clients[sender_identity].authenticated)
	{
		send_message(sender_identity, "NOT_AUTHENTICATED");
		return;
	}

	// Get the name safely
	const std::string& name = clients[sender_identity].username;

	// skip clients with no name
	if (name.empty()) 
		return; 

	// Forward message to all.
	for (auto& [identity, client] : clients)
		send_message(identity, "LEFT", name);

	// Also console it.
	std::cout
		<< "Left: "
		<< name
		<< std::endl;
}

void ChatServer::send_disconnect_message
(
	const std::string& sender_identity
)
{
	/*
		for (auto& [identity, client] : clients)
	{
		// Forward message to all
		send_message(identity, "LEFT",
			clients[sender_identity].username);
	}
	*/

	handle_left_message(sender_identity);
}

void ChatServer::send_message
(
	const std::string& identity,
	const std::string& msg_type
)
{
	zmq::multipart_t msg;
	msg.add(zmq::message_t(identity.data(), identity.size()));
	msg.add(zmq::message_t(msg_type.data(), msg_type.size()));
	msg.send(socket);
}

void ChatServer::send_message
(
	const std::string& identity,
	const std::string& msg_type,
	const std::string& data
)
{
	zmq::multipart_t msg;
	msg.add(zmq::message_t(identity.data(), identity.size()));
	msg.add(zmq::message_t(msg_type.data(), msg_type.size()));
	msg.add(zmq::message_t(data.data(), data.size()));
	msg.send(socket);
}

void ChatServer::send_message
(
	const std::string& identity,
	const std::string& msg_type,
	const std::string& data1,
	const std::string& data2
)
{
	zmq::multipart_t msg;
	msg.add(zmq::message_t(identity.data(), identity.size()));
	msg.add(zmq::message_t(msg_type.data(), msg_type.size()));
	msg.add(zmq::message_t(data1.data(), data1.size()));
	msg.add(zmq::message_t(data2.data(), data2.size()));
	msg.send(socket);
}

void ChatServer::ping_clients()
{
	while (running)
	{
		//std::cout << "ChatServer While Ping Cycle Completed" << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(1));
		auto now = std::chrono::steady_clock::now();

		std::vector<std::string> to_disconnect;

		for (auto& [identity, client] : clients)
		{
			auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
				now - client.last_ping).count();

			if (elapsed > 4)
			{
				// Client hasn't responded in 4 seconds, disconnect
				to_disconnect.push_back(identity);
				send_disconnect_message(identity);
			}
			else if (elapsed > 2)
			{
				// Send ping after 2 seconds of inactivity
				send_message(identity, "PING");
			}
		}

		// Disconnect unresponsive clients
		for (const auto& identity : to_disconnect)
		{
			disconnect_client(identity);
		}
	}
}

void ChatServer::disconnect_client(const std::string& identity)
{
	if (clients.find(identity) != clients.end())
	{
		if (!clients[identity].username.empty())
		{
			usernames.erase(clients[identity].username);
		}

		clients.erase(identity);

		std::cout
			<< "Client disconnected: "
			<< identity
			<< std::endl;
	}
}


/*
// clients for loop
for (const auto& [identity, client] : clients)
{
	std::cout << "Identity: " << identity
		<< ", Username: " << client.username
		<< ", Authenticated: " << client.authenticated
		<< std::endl;
}

// usernames for loop
for (const auto& [username, identity] : usernames)
{
	std::cout << "Username: " << username
		<< ", Identity: " << identity
		<< std::endl;
}
*/

