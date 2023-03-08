#ifndef SOCKET_H
#define SOCKET_H
#include "Connection.h"
#include <enet.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

class Socket;

#ifndef SOCKET_ONCONNECTED_CALLBACK_PTR
#define SOCKET_ONCONNECTED_CALLBACK_PTR
typedef void (*onConnectedSocketCallbackPtr)(Socket*);
#endif

#ifndef PASSWORD_CALLBACK_PTR
#define PASSWORD_CALLBACK_PTR
typedef std::string(*passwordCallbackPtr)(void);
#endif

class Socket
{
private:
	ENetHost* client;
	ENetPeer* enetPeer;
	ENetEvent event{};

	std::string tempMsg;
	size_t tempPos = 0;
	std::string tempToken = "";
	char* tempEndPtr; // or smth

	unsigned long lobbyID = 0;
	std::string lobbyName = "";
	std::string lobbyPassword = "";
	unsigned int lobbyAccessLevel = 0;
	unsigned int lobbyGameMode = 0;
public:
	static Socket* instance;

	static std::string getName() { return instance->lobbyName; }
	static std::string getPassword() { return instance->lobbyPassword; }
	static unsigned int getAccessLevel() { return instance->lobbyAccessLevel; }
	static unsigned int getGameMode() { return instance->lobbyGameMode; }

	std::vector<unsigned long> connections;
	unsigned long socketPeer;
	Socket() {}
	Socket(unsigned long peer, onConnectedSocketCallbackPtr onConnectedCallback);
	void SendPacketToMidServer(const char* str);
	void SendPacketToPeer(unsigned long peer, const char* str);
	void Start(const std::string& lobbyName, const std::string& lobbyPassword = "", unsigned int lobbyAccessLevel = 0, unsigned int lobbyGameMode = 0);
	void Receive();
	void ProcessMessage(unsigned long from, const char* str);
	void Stop();
};

#endif // !SOCKET_H