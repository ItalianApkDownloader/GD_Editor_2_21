#include "NetworkingManager.h"
#include "cocos2d.h"

unsigned long NetworkingManager::peer;
const char* NetworkingManager::ip = "ip here";
Connection* NetworkingManager::connectionManager{};
Socket* NetworkingManager::socketManager{};

unsigned long tempLobbyID;
passwordCallbackPtr tempPasswordCallback;

std::string tempLobbyName;
std::string tempLobbyPassword;
unsigned int tempLobbyAccessLevel;
unsigned int tempLobbyGameMode;

void NetworkingManager::connectToLobby(Connection* connection)
{
	connection->Connect(tempLobbyID, tempPasswordCallback);
}

void NetworkingManager::startLobby(Socket* socket)
{
	socket->Start(tempLobbyName, tempLobbyPassword, tempLobbyAccessLevel, tempLobbyGameMode);
}

void NetworkingManager::Connect(unsigned long lobbyID, passwordCallbackPtr passwordCallback)
{
	fmtlog("Connect:{};{}", ip, peer);
	tempLobbyID = lobbyID;
	tempPasswordCallback = passwordCallback;

	connectionManager = new Connection(peer, connectToLobby);
}

void NetworkingManager::Host(std::string lobbyName, std::string lobbyPassword, unsigned int lobbyAccessLevel, unsigned int lobbyGameMode)
{
	tempLobbyName = lobbyName;
	tempLobbyPassword = lobbyPassword;
	tempLobbyAccessLevel = lobbyAccessLevel;
	tempLobbyGameMode = lobbyGameMode;

	socketManager = new Socket(peer, startLobby);
}

void NetworkingManager::Update()
{
	//fmtlog("NetworkingManager update");
	if (connectionManager != NULL) connectionManager->Receive();
	//else fmtlog("connectionManager == nullptr");
	
	if (socketManager != NULL) socketManager->Receive();
	//else fmtlog("socketManager == nullptr");
	
#ifdef _WIN32
	if (GetKeyState(peer + 48) & 0x8000) { std::string a = fmt::format("hello gordon!"); connectionManager->SendPacket(a.c_str()); }
	if (GetKeyState(peer + 48) & 0x8000 && GetKeyState(VK_CONTROL) & 0x8000) { Disconnect(); }
#endif
}

void NetworkingManager::Disconnect()
{
	if (connectionManager != NULL)
		connectionManager->Disconnect();
	if (socketManager != NULL)
		socketManager->Stop();
}