#ifndef CONNECTION_H
#define CONNECTION_H
#include <enet.h>
#include <stdio.h>
#include <string>
#include <fmt/format.h>
#include <iostream>
#include "Utils.h"

class Connection;

#ifndef CONNECTION_ONCONNECTED_CALLBACK_PTR
#define CONNECTION_ONCONNECTED_CALLBACK_PTR
typedef void (*onConnectedConnectionCallbackPtr)(Connection*);
#endif

#ifndef PASSWORD_CALLBACK_PTR
#define PASSWORD_CALLBACK_PTR
typedef std::string(*passwordCallbackPtr)(void);
#endif

class Connection
{
private:
	ENetHost* client;
	ENetPeer* enetPeer;
	ENetEvent event{};

	std::string tempMsg;
	size_t tempPos = 0;
	std::string tempToken = "";
	char* tempEndPtr; // or smth

	passwordCallbackPtr tempPasswordCallback;
public:
	unsigned long peer;
	unsigned long lobbyID;

	bool Connected() { return lobbyID != 0; }

	Connection(){}
	Connection(unsigned long peer, onConnectedConnectionCallbackPtr onConnectedCallback);
	void Disconnect();
	void SendPacketToMidServer(const char* str);
	void SendPacket(const char* str);
	void Connect(unsigned long ID, passwordCallbackPtr passwordCallback);
	void Receive();
	void OnMessageReceived(const char* str);
};

#endif // !CONNECTION_H