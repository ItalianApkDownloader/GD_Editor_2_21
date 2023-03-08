#include "Connection.h"
#include "NetworkingManager.h"
#include "cocos2d.h"

Connection::Connection(unsigned long peer, onConnectedConnectionCallbackPtr onConnectedCallback)
{
	client = enet_host_create(NULL, 1, 2, 0, 0);
	this->peer = peer;

	ENetAddress addr{ 0 };
	enet_address_set_host(&addr, NetworkingManager::ip);
	addr.port = 6722;
	enetPeer = enet_host_connect(client, &addr, 2, 0);

	if (enet_host_service(client, &event, 5000) > 0 &&
		event.type == ENET_EVENT_TYPE_CONNECT)
		(*onConnectedCallback)(this);
	else
		enet_peer_reset(enetPeer);
}
void Connection::Disconnect()
{
	if (enetPeer != NULL)
	{
		enet_peer_disconnect(enetPeer, 0);
		lobbyID = 0;
	}
}
void Connection::SendPacketToMidServer(const char* str)
{
	fmtlog("Connection::SendPacketToMidServer:{}\n", str);
	ENetPacket* packet = enet_packet_create(str, strlen(str) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(enetPeer, 0, packet);
}
void Connection::SendPacket(const char* str)
{
	fmtlog("Connection::SendPacket:{};{}\n", str, Connected());
	if (Connected())
	{
		std::string f = fmt::format("SendPacket;{};{};{}", peer, lobbyID, str);
		ENetPacket* packet = enet_packet_create(f.c_str(), f.length() + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(enetPeer, 0, packet);
	}
}
void Connection::Connect(unsigned long ID, passwordCallbackPtr passwordCallback)
{
	tempPasswordCallback = passwordCallback;
	std::string msg = fmt::format("ConnectLobby;{};{}", peer, ID);
	SendPacketToMidServer(msg.c_str());
}
void Connection::Receive()
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
				if (tempToken == "PasswordFail")
				{
					tempMsg.erase(0, tempPos + 1);
					tempPos = tempMsg.find(";");
					tempToken = tempMsg.substr(0, tempPos);
					unsigned long lobbyID = std::strtoul(tempToken.c_str(), &tempEndPtr, 10); // lobby you tried to connect to

					std::string password = (*tempPasswordCallback)();

					std::string msg = fmt::format("ConnectLobbyPassword;{};{};{}", peer, lobbyID, password);
					SendPacketToMidServer(msg.c_str());
				}
				if (tempToken == "ConnectedToLobby")
				{
					tempMsg.erase(0, tempPos + 1);
					tempPos = tempMsg.find(";");
					tempToken = tempMsg.substr(0, tempPos);
					unsigned long lobbyID = std::strtoul(tempToken.c_str(), &tempEndPtr, 10); // lobby you got connected to

					this->lobbyID = lobbyID;
				}
				if (tempToken == "MsgFromPeer")
				{
					tempMsg.erase(0, tempPos + 1);
					tempPos = tempMsg.find(";");
					tempToken = tempMsg.substr(0, tempPos);
					OnMessageReceived(tempToken.c_str());
				}
				tempMsg.erase(0, tempPos + 1);
			}
			enet_packet_destroy(event.packet);
			break;
		}
	}
}
void Connection::OnMessageReceived(const char* str)
{
	fmtlog("{}\n", str);
}