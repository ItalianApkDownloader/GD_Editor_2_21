#include "Socket.h"
#include "NetworkingManager.h"
#include <ctime>
#include <cstdlib>
#include "Utils.h"

Socket* Socket::instance = nullptr;

Socket::Socket(unsigned long peer, onConnectedSocketCallbackPtr onConnectedCallback)
{
	client = enet_host_create(NULL, 1, 2, 0, 0);
	this->socketPeer = peer;

	ENetAddress addr{ 0 };
	enet_address_set_host(&addr, NetworkingManager::ip);
	addr.port = 6722;
	enetPeer = enet_host_connect(client, &addr, 2, 0);

	instance = this;

	if (enet_host_service(client, &event, 10000) > 0 &&
		event.type == ENET_EVENT_TYPE_CONNECT)
	{ 
		(*onConnectedCallback)(this);
	}
	else
		enet_peer_reset(enetPeer);
}
void Socket::SendPacketToMidServer(const char* str)
{
	ENetPacket* packet = enet_packet_create(str, strlen(str) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(enetPeer, 0, packet);
}
void Socket::SendPacketToPeer(unsigned long peer, const char* str)
{
	std::string msg = fmt::format("MsgToPeer;{};{}", peer, str);
	ENetPacket* packet = enet_packet_create(msg.c_str(), msg.length() + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(enetPeer, 0, packet);
}
void Socket::Start(const std::string& lobbyName, const std::string& lobbyPassword, unsigned int lobbyAccessLevel, unsigned int lobbyGameMode)
{
	this->lobbyID = Utils::randomUL();
	std::string msg = fmt::format("CreateLobby;{};{};{};{};{}", lobbyID, lobbyName.c_str(), lobbyPassword.c_str(), lobbyAccessLevel, lobbyGameMode);
	SendPacketToMidServer(msg.c_str());
	this->lobbyName = lobbyName;
	this->lobbyPassword = lobbyPassword;
	this->lobbyAccessLevel = lobbyAccessLevel;
	this->lobbyGameMode = lobbyGameMode;
}
void Socket::Receive()
{
	while (enet_host_service(client, &event, 1) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_RECEIVE:

			tempMsg = std::string((const char*)event.packet->data);
			tempPos = 0;
			tempToken = "";

			while ((tempPos = tempMsg.find(";")) != std::string::npos)
			{
				tempToken = tempMsg.substr(0, tempPos);
				if (tempToken == "CreatedLobby")
				{
					tempMsg.erase(0, tempPos + 1);
					tempPos = tempMsg.find(";");
					tempToken = tempMsg.substr(0, tempPos);
					unsigned long lobbyID = std::strtoul(tempToken.c_str(), &tempEndPtr, 10); // because uhh. yea. it can be changed by server stfu
					this->lobbyID = lobbyID;

					NetworkingManager::Connect(lobbyID, getPassword);
				}
				else if (tempToken == "Connection")
				{
					tempMsg.erase(0, tempPos + 1);
					tempPos = tempMsg.find(";");
					tempToken = tempMsg.substr(0, tempPos);
					unsigned long pid = std::strtoul(tempToken.c_str(), &tempEndPtr, 10);

					connections.push_back(pid);
				}
				else if (tempToken == "Disconnect")
				{
					tempMsg.erase(0, tempPos + 1);
					tempPos = tempMsg.find(";");
					tempToken = tempMsg.substr(0, tempPos);
					unsigned long pid = std::strtoul(tempToken.c_str(), &tempEndPtr, 10);

					connections.erase(Utils::find(connections.begin(), connections.end(), pid));
				}
				else if (tempToken == "Message")
				{
					tempMsg.erase(0, tempPos + 1);
					tempPos = tempMsg.find(";");
					tempToken = tempMsg.substr(0, tempPos);
					unsigned long pid = std::strtoul(tempToken.c_str(), &tempEndPtr, 10);

					tempMsg.erase(0, tempPos + 1);
					tempPos = tempMsg.find(";");
					tempToken = tempMsg.substr(0, tempPos);
					ProcessMessage(pid, tempToken.c_str());
				}
				tempMsg.erase(0, tempPos + 1);
			}

			enet_packet_destroy(event.packet);
			break;
		}
	}
}
void Socket::ProcessMessage(unsigned long from, const char* str)
{
	for (auto& connection : connections) 
		if (connection != from)
			SendPacketToPeer(connection, str);
}
void Socket::Stop()
{
	if (enetPeer != NULL) 
	{
		enet_peer_disconnect(enetPeer, 0);
	}
}