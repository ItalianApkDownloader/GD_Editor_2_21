#ifndef NETWORKINGMANAGER_H
#define NETWORKINGMANAGER_H

#include "Utils.h"

#include "Connection.h"
#include "Socket.h"

#ifndef PASSWORD_CALLBACK_PTR
#define PASSWORD_CALLBACK_PTR
typedef std::string(*passwordCallbackPtr)(void);
#endif

class NetworkingManager
{
public:
	static unsigned long peer;
	static Connection* connectionManager;
	static Socket* socketManager;
	static const char* ip;

	static void connectToLobby(Connection* connection);
	static void startLobby(Socket* socket);
	static void Host(std::string lobbyName, std::string lobbyPassword = "", unsigned int lobbyAccessLevel = 0, unsigned int lobbyGameMode = 0);
	static void Connect(unsigned long lobbyID, passwordCallbackPtr passwordCallback);
	static void Update();
	static void Disconnect();
};

#endif // !NETWORKINGMANAGER_H